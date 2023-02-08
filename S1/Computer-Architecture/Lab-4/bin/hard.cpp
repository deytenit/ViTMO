#include <omp.h>

#include <array>
#include <cmath>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

bool omp_global_state = true;

namespace otsu {
using Thresholds = std::array<uint16_t, 3>;
using ClassStats = std::pair<uint32_t, double>;

inline double sqr(const double x) {
    return x * x;
}

struct PGM {
    static constexpr uint16_t kDepth = 256;
    static constexpr std::string_view kMagic = "P5";
    static constexpr std::array<uint8_t, 4> kTargets = {0, 84, 170, 255};

    using Histogram = std::array<uint32_t, kDepth>;

    uint32_t width{};
    uint32_t height{};
    std::vector<uint8_t> pixels;

    explicit PGM(const fs::path& path) {
        std::ifstream in(path, std::ios::in | std::ios::binary);
        in.exceptions(std::ios::failbit);

        if (std::string line; !(in >> line) || line != kMagic) {
            throw std::invalid_argument("PGM::PGM error: Not a PGM file was provided.");
        }

        in >> width >> height;

        pixels.resize(static_cast<size_t>(width) * height, 0);

        uint16_t depth;

        in >> depth;

        in.ignore(1);

        if (depth != kDepth - 1) {
            throw std::invalid_argument("PGM::PGM error: Not a PGM file was provided.");
        }

        in.read(reinterpret_cast<char*>(pixels.data()), static_cast<std::streamsize>(width) * height);
    }

    [[nodiscard]] Histogram GetHistogram() const {
        Histogram result{0};

#pragma omp parallel if (omp_global_state)
        {
            Histogram local{0};

#pragma omp for schedule(dynamic)
            for (size_t i = 0; i < pixels.size(); ++i) {
                ++local[pixels[i]];
            }

#pragma omp critical
            {
                for (uint16_t i = 0; i < local.size(); ++i) {
                    result[i] += local[i];
                }
            }
        }

        return result;
    }

    void Transform(const Thresholds& thresholds) {
#pragma omp parallel for schedule(dynamic) if (omp_global_state)
        for (size_t i = 0; i < pixels.size(); ++i) {
            if (pixels[i] <= thresholds[0]) {
                pixels[i] = kTargets[0];
            } else if (pixels[i] <= thresholds[1]) {
                pixels[i] = kTargets[1];
            } else if (pixels[i] <= thresholds[2]) {
                pixels[i] = kTargets[2];
            } else {
                pixels[i] = kTargets[3];
            }
        }
    }

    void Print(const fs::path& path) const {
        std::ofstream out(path, std::ios::out | std::ios::binary | std::ios::trunc);
        out.exceptions(std::ios::failbit);

        out << kMagic << '\n';
        out << width << ' ' << height << '\n';
        out << kDepth - 1 << '\n';

        out.write(reinterpret_cast<const char*>(pixels.data()), static_cast<std::streamsize>(width) * height);
    }
};

using HistogramStats = std::array<ClassStats, PGM::kDepth>;

inline ClassStats GetClassStats(const HistogramStats& stats, const uint16_t begin, const uint16_t end) {
    if (begin == 0) {
        return {stats[end].first, stats[end].second / static_cast<double>(stats[end].first)};
    }

    size_t sum = stats[end].first - stats[begin - 1].first;
    double prb = (stats[end].second - stats[begin - 1].second) / static_cast<double>(sum);

    return {sum, prb};
}

double Variance(const HistogramStats& stats, const uint16_t k0, const uint16_t k1, const uint16_t k2) {
    const ClassStats cs0 = GetClassStats(stats, 0, k0 - 1);

    const ClassStats cs1 = GetClassStats(stats, k0, k1 - 1);

    const ClassStats cs2 = GetClassStats(stats, k1, k2 - 1);

    const ClassStats cs3 = GetClassStats(stats, k2, stats.size() - 1);

    const double mg = (static_cast<double>(cs0.first) * cs0.second + static_cast<double>(cs1.first) * cs1.second) + (static_cast<double>(cs2.first) * cs2.second + static_cast<double>(cs3.first) * cs3.second);

    return (static_cast<double>(cs0.first) * sqr(cs0.second - mg) + static_cast<double>(cs1.first) * sqr(cs1.second - mg)) + (static_cast<double>(cs2.first) * sqr(cs2.second - mg) + static_cast<double>(cs3.first) * sqr(cs3.second - mg));
}

HistogramStats PrecalcStats(const PGM::Histogram& histogram) {
    HistogramStats result;
    std::vector<ClassStats> locals;

#pragma omp parallel if (omp_global_state)
    {
        const int32_t thread = omp_get_thread_num();
        const int32_t threads = omp_get_num_threads();
#pragma omp single
        {
            locals.resize(threads + 1);
            locals[0] = {0, 0};
        }

        uint32_t class_sum = 0;
        double class_prb = 0;
#pragma omp for schedule(static) nowait
        for (uint16_t i = 0; i < histogram.size(); ++i) {
            class_sum += histogram[i];
            class_prb += static_cast<double>(i * histogram[i]);

            result[i] = {class_sum, class_prb};
        }

        locals[thread + 1] = {class_sum, class_prb};

#pragma omp barrier

        uint32_t class_sum_offset = 0;
        double class_prb_offset = 0;
        for (uint16_t i = 0; i < thread + 1; ++i) {
            class_sum_offset += locals[i].first;
            class_prb_offset += locals[i].second;
        }

#pragma omp for schedule(static)
        for (uint16_t i = 0; i < histogram.size(); ++i) {
            result[i].first += class_sum_offset;
            result[i].second += class_prb_offset;
        }
    }

    return result;
}

Thresholds GetThresholds(const PGM::Histogram& histogram) {
    Thresholds result;
    double variance = 0;

    const HistogramStats stats = PrecalcStats(histogram);

#pragma omp parallel if (omp_global_state)
    {
        double local_variance = variance;
        Thresholds local_result{};

#pragma omp for schedule(dynamic) nowait
        for (uint16_t k0 = 1; k0 < histogram.size() - 3; ++k0) {
            for (uint16_t k1 = k0 + 1; k1 < histogram.size() - 2; ++k1) {
                for (uint16_t k2 = k1 + 1; k2 < histogram.size() - 1; ++k2) {
                    if (const double tmp = Variance(stats, k0, k1, k2); std::isless(
                            local_variance, tmp)) {
                        local_variance = tmp;
                        local_result = {k0, k1, k2};
                    }
                }
            }
        }

#pragma omp critical
        {
            if (std::isless(variance, local_variance)) {
                variance = local_variance;
                result = local_result;
            }
        }
    }

    return result;
}
}  // namespace otsu

struct Config {
    constexpr static int32_t kArgAmount = 4;

    int32_t threads;
    bool open_mp;
    fs::path in_path;
    fs::path out_path;

    Config(const int argc, char** argv) {
        if (argc < kArgAmount) {
            throw std::invalid_argument("Config::Parse error: Not enough arguments.");
        }

        if (argc > kArgAmount) {
            std::cout << "Config::Parse warning: Extra arguments will be ignored.";
        }

        SetThreads(std::stoi(argv[1]));

        in_path = argv[2];
        out_path = argv[3];
    }

   private:
    void SetThreads(const int32_t value) {
        if (value < 0) {
            threads = 1;
            open_mp = false;
            omp_set_dynamic(0);
            omp_set_num_threads(1);
        } else if (value == 0) {
            open_mp = true;
            threads = omp_get_max_threads();
        } else if (value > 0) {
            open_mp = true;
            threads = std::min(omp_get_max_threads(), value);
            omp_set_dynamic(0);
            omp_set_num_threads(threads);
        }
    }
};

int main(const int argc, char** argv) {
    try {
        const Config config(argc, argv);
        otsu::PGM image{config.in_path};

        omp_global_state = config.open_mp;

        const double start = omp_get_wtime();

        const otsu::Thresholds thresholds = otsu::GetThresholds(image.GetHistogram());

        image.Transform(thresholds);

        const double end = omp_get_wtime();

        std::cout << thresholds[0] << ' ' << thresholds[1] << ' ' << thresholds[2] << '\n';
        std::cout << "Time (" << config.threads << " thread(s)): " << (end - start) * 1000 << " ms" << std::endl;

        image.Print(config.out_path);
    } catch (std::exception& err) {
        std::cout << err.what() << '\n';
    }
    return 0;
}
