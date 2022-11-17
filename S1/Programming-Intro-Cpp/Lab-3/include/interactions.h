#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <ctime>
#include <sstream>

#include <imgmanip.h>
#include <argument_parser.h>
#include <sandpile.h>

namespace fs = std::filesystem;

namespace interactions {
    class config {
    public:
        fs::path path;
        uint16_t length = 0;
        uint16_t width = 0;
        std::vector<std::vector<uint64_t>> input;
        fs::path output = fs::current_path();
        uint32_t iterations = 0;
        uint32_t freq = 0;
        imgmanip::color_distribution color;
        uint32_t upscale = 0;
        bool gaussian = false;
        double gaussian_rate = 0.111;

        config(int argc, char** argv);

        void print_help() const;
    };

    std::vector<std::vector<uint64_t>> parse_tsv_file(const fs::path& path);
    fs::path format_pattern(fs::path path, int idx);
    imgmanip::color_distribution parse_color(const std::string& data);

    void save_with_config(const sandpile& simulation, const config& config, uint32_t idx);
}