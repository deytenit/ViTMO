#include "imgmanip.h"

imgmanip::pixel::pixel(const uint8_t& red, const uint8_t& green, const uint8_t& blue) :
    red(red), green(green), blue(blue) {}

imgmanip::pixel::pixel(const char* hex) {
    std::string hex_str(hex);
    if (hex_str.size() < 6) {
        hex_str += std::string(6 - hex_str.size(), '0');
    }

    red = std::stoul(hex_str.substr(0, 2), nullptr, 16);
    green = std::stoul(hex_str.substr(2, 2), nullptr, 16);
    blue = std::stoul(hex_str.substr(4, 2), nullptr, 16);
}


imgmanip::color_distribution::color_distribution() {
    pixels_ = {STD_WHITE, STD_GREEN, STD_PURPLE, STD_YELLOW, STD_BLACK};
}

imgmanip::color_distribution::color_distribution(const pixel& data) {
    pixels_.emplace_back(data);
}

imgmanip::color_distribution::color_distribution(std::vector<pixel> data) : pixels_(std::move(data)) {}

imgmanip::color_distribution::color_distribution(const pixel& begin, const pixel& end, const size_t& length) {
    const pixel diff = {
        static_cast<uint8_t>((end.red - begin.red) / length),
        static_cast<uint8_t>((end.green - begin.green) / length),
        static_cast<uint8_t>((end.blue - begin.blue) / length)
    };


    for (size_t i = 0; i < length; i++) {
        pixels_.emplace_back(
            begin.red + diff.red * i,
            begin.green + diff.green * i,
            begin.blue + diff.blue * i);
    }
}

imgmanip::pixel imgmanip::color_distribution::get(const size_t& index) const {
    return (index >= pixels_.size() ? pixels_.back() : pixels_[index]);
}


void imgmanip::save(const std::string& path, const std::vector<std::vector<pixel>>& data) {
    std::ofstream file(path.c_str(), std::ios::out | std::ios::binary);

    file.write(MAGIC, sizeof(MAGIC));

    bitmap_file_header file_header = {
        BITMAP_HEADER_SIZE + static_cast<uint32_t>((data.size() * 3 + data[0].size() % 4) * data.size()),
        0,
        0,
        BITMAP_HEADER_SIZE
    };

    file.write(reinterpret_cast<char*>(&file_header), sizeof(file_header));

    bitmap_dib_info core_header = {
        sizeof(bitmap_dib_info),
        static_cast<uint16_t>(data[0].size()),
        static_cast<uint16_t>(data.size()),
        1,
        24,
        0,
        0,
        80,
        80,
        0,
        0
    };

    file.write(reinterpret_cast<char*>(&core_header), sizeof(core_header));

    for (size_t row = data.size(); row >= 1; --row) {
        const std::vector<pixel>& row_data = data[row - 1];

        for (const auto& col : row_data) {
            file.put(col.blue);
            file.put(col.green);
            file.put(col.red);
        }

        for (size_t i = 0; i < row_data.size() % 4; ++i) {
            file.put(0);
        }

    }

    file.close();
}

void imgmanip::gaussian(pixel_array& data, const double_t& rate) {
    for (size_t h = 0; h < data.size(); ++h) {
        for (size_t w = 0; w < data.size(); ++w) {
            uint8_t avg_r = 0;
            uint8_t avg_g = 0;
            uint8_t avg_b = 0;

            for (size_t i = (h == 0 ? 0 : h - 1); i < std::min(h + 2, data.size()); ++i) {
                for (size_t j = (w == 0 ? 0 : w - 1); j < std::min(w + 2, data[h].size()); ++j) {
                    avg_r += static_cast<uint8_t>(rate * data[i][j].red);
                    avg_g += static_cast<uint8_t>(rate * data[i][j].green);
                    avg_b += static_cast<uint8_t>(rate * data[i][j].blue);
                }
            }

            data[h][w].red = avg_r;
            data[h][w].green = avg_g;
            data[h][w].blue = avg_b;
        }
    }
}


void imgmanip::upscale(pixel_array& data, const size_t& rate) {
    pixel_array result(data.size() * rate);

    for (auto& i : result) {
        i.resize(data[0].size() * rate);
    }

    size_t scaled_row = 0;
    for (const auto& row : data) {
        size_t scaled_col = 0;
        for (const auto& pix : row) {
            for (size_t i = 0; i < rate; ++i) {
                for (size_t j = 0; j < rate; ++j) {
                    result[scaled_row + i][scaled_col + j] = pix;
                }
            }
            scaled_col += rate;
        }
        scaled_row += rate;
    }

    data = std::move(result);
}

imgmanip::pixel_array imgmanip::from_numbers(const std::vector<std::vector<uint64_t>>& data,
                                             const color_distribution& colors) {
    pixel_array result(data.size());

    for (size_t i = 0; i < result.size(); ++i) {
        result[i].resize(data[0].size());
        for (size_t j = 0; j < result[i].size(); ++j) {
            result[i][j] = colors.get(data[i][j]);
        }
    }

    return result;
}
