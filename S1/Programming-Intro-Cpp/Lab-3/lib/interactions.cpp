#include "interactions.h"

interactions::config::config(int argc, char** argv) {
    path = argv[0];

    const std::list<argument_parser::option> options = {
        {"length", 'l', argument_parser::STORE, 'l'},
        {"width", 'w', argument_parser::STORE, 'w'},
        {"input", 'i', argument_parser::STORE, 'i'},
        {"output", 'o', argument_parser::STORE, 'o'},
        {"max-iter", 'm', argument_parser::STORE, 'm'},
        {"freq", 'f', argument_parser::STORE, 'f'},
        {"color", 'c', argument_parser::STORE, 'c'},
        {"upscale", 'u', argument_parser::STORE, 'u'},
        {"gaussian", 'g', argument_parser::FLAG, 'g'},
        {"help", 'h', argument_parser::FLAG, 'h'}
    };

    argument_parser parser(options);

    parser.parse(argc, argv);

    int option;
    while ((option = parser.next()) != EOF) {
        switch (option) {
        case 'l':
            length = std::stoi(parser.value());
            break;
        case 'w':
            width = std::stoi(parser.value());
            break;
        case 'i':
            try {
                input = parse_tsv_file(parser.value());
            }
            catch (std::runtime_error& rerr) {
                std::cout << rerr.what() << std::endl;
                exit(0);
            }
            break;
        case 'o':
            output = parser.value();
            break;
        case 'm':
            iterations = std::stoi(parser.value());
            break;
        case 'f':
            freq = std::stoi(parser.value());
            break;
        case 'c':
            try {
                color = parse_color(parser.value());
            }
            catch (std::runtime_error& rerr) {
                std::cout << rerr.what() << std::endl;
                exit(0);
            }
            break;
        case 'u':
            upscale = std::stoi(parser.value());
            break;
        case 'g':
            gaussian = true;
            break;
        case 'h':
            print_help();
            exit(0);
        case '?':
            std::cout << "Try: '" << argv[0] << " -h' or '" << argv[0] << " --help' to learn about this utility." <<
                std::endl;
            exit(0);
        default:
            break;
        }
    }
}

void interactions::config::print_help() const {
    std::cout << "Sandpiller 2211.2\n"
        << "Usage: " << path << " [OPTIONS]...\n"
        << "OPTIONS configure the generation:\n"
        << "\t-h (--help):                 You're already here!\n"
        << "\t-l (--length)   [INTEGER]:   Length of simulation in pixels.\n"
        << "\t-w (--width)    [INTEGER]:   Width of simulation in pixels.\n"
        << "\t-i (--input)    [FILE]:      tsv input file (simulation base values).\n"
        << "\t-o (--output)   [DIRECTORY]: Output dir for files to save.\n"
        << "\t-m (--max-iter) [INTEGER]:   Print in FILE instead of a console.\n"
        << "\t-f (--freq)     [INTEGER]:   Print in FILE instead of a console.\n"
        << "\t-c (--color)    [ARRAY]:     Color array or range for output bmp.\n"
        << "\t-u (--upscale)  [INTEGER]:   Upscale every pixel of output bmp.\n"
        << "\t-g (--gaussian):             Apply gaussian-Blur filter."
        << std::endl;
}

std::vector<std::vector<uint64_t>> interactions::parse_tsv_file(const fs::path& path) {
    if (!exists(path) || !is_regular_file(path)) {
        throw std::runtime_error("interactions::parse_tsv_file error: Bad input file or does not exists.");
    }

    std::vector<std::vector<uint64_t>> result;

    std::ifstream file(path, std::ofstream::in);

    std::string line;
    while (std::getline(file, line)) {
        std::vector<uint64_t> tmp(1, 0);
        for (const char& c : line) {
            if (isdigit(c) != 0) {
                tmp[tmp.size() - 1] = tmp[tmp.size() - 1] * 10 + (c - '0');
            }
            else if (c == '\t') {
                tmp.emplace_back(0);
            }
            else {
                throw std::runtime_error("interactions::parse_tsv_file error: Bad input file or does not exists.");
            }
        }


        if (tmp.size() == 1) {
            return {{tmp[0]}};
        }
        else if (tmp.size() == 3) {
            if (result.size() < tmp[0]) {
                result.resize(tmp[0] + 1);
            }

            if (result[tmp[0]].size() < tmp[1]) {
                for (auto& row: result) {
                    row.resize(tmp[1]);
                }
            }

            result[tmp[0] - 1][tmp[1] - 1] = tmp[2];
        }
        else {
            throw std::runtime_error("interactions::parse_tsv_file error: Bad input file or does not exists.");
        }
    }

    file.close();

    return result;
}

fs::path interactions::format_pattern(fs::path path, int idx) {
    std::string pattern = path.string();
    if (fs::is_directory(pattern)) {
        pattern += "/%i";
    }

    if (pattern.size() < 4 || pattern.substr(pattern.size() - 4, 4) != ".bmp") {
        pattern += ".bmp";
    }

    size_t pos;
    while ((pos = pattern.find("%i")) != std::string::npos) {
        pattern.replace(pos, 2, std::to_string(idx));
    }

    const time_t time_since_epoch = time(nullptr);
    tm now;
    localtime_s(&now, &time_since_epoch);

    std::ostringstream oss;
    oss << std::put_time(&now, pattern.c_str());
    pattern = oss.str();

    return pattern;
}

imgmanip::color_distribution interactions::parse_color(const std::string& data) {
    if (data.front() != '[' || data.back() != ']') {
        throw std::runtime_error("interactions::parse_color error: Bad argument.");
    }

    bool is_range = data.find("...") != std::string::npos;

    std::vector<imgmanip::pixel> colors;

    std::string tmp;
    for (size_t i = 1; i < data.size() - 1; ++i) {
        const char chr = std::tolower(data[i]);
        if (('a' <= chr && chr <= 'z') || ('0' <= chr && chr <= '9')) {
            tmp += chr;
        }
        else if (!tmp.empty()) {
            colors.emplace_back(tmp.c_str());
            tmp.clear();
        }
    }

    if (!tmp.empty()) {
        colors.emplace_back(tmp.c_str());
    }

    return is_range
               ? imgmanip::color_distribution(colors.front(), colors.back(), 4)
               : imgmanip::color_distribution(colors);
}

void interactions::save_with_config(const sandpile& simulation, const config& config, uint32_t idx) {
    auto data = from_numbers(simulation.to_vector(), config.color);

    if (config.upscale > 0) {
        upscale(data, config.upscale);
    }

    if (config.gaussian) {
        gaussian(data, config.gaussian_rate);
    }

    save(format_pattern(config.output, idx).string(), data);
}
