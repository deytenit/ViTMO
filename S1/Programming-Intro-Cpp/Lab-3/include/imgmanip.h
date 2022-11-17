#pragma once
#include <string>
#include <vector>
#include <string>
#include <fstream>

namespace imgmanip {
    constexpr char MAGIC[2] = {'B', 'M'};

    struct pixel {
        uint8_t red = 0;
        uint8_t green = 0;
        uint8_t blue = 0;

        pixel() = default;
        pixel(const uint8_t& red, const uint8_t& green, const uint8_t& blue);

        pixel(const char* hex);
    };

    const pixel STD_WHITE = {255, 255, 255};
    const pixel STD_GREEN = {46, 204, 64};
    const pixel STD_PURPLE = {177, 13, 201};
    const pixel STD_YELLOW = {255, 220, 0};
    const pixel STD_BLACK = {0, 0, 0};


    class color_distribution {
    private:
        std::vector<pixel> pixels_;
    public:
        color_distribution();
        color_distribution(const pixel& data);
        color_distribution(std::vector<pixel> data);
        color_distribution(const pixel& begin, const pixel& end, const size_t& length);

        [[nodiscard]] pixel get(const size_t& index) const;
    };

    struct bitmap_file_header {
        uint32_t bf_size;
        uint16_t bf_reserved_1;
        uint16_t bf_reserved_2;
        uint32_t bf_offset;
    };

    struct bitmap_dib_info {
        uint32_t bi_size;
        int32_t bi_width;
        int32_t bi_height;
        uint16_t bi_planes;
        uint16_t bi_bit_count;
        uint32_t bi_compression;
        uint32_t bit_size_image;
        int32_t bi_x_pels_per_meter;
        int32_t bi_y_pels_per_meter;
        uint32_t bi_clr_used;
        uint32_t bi_clr_important;
    };

    constexpr uint32_t BITMAP_HEADER_SIZE = sizeof(MAGIC) + sizeof(bitmap_file_header) + sizeof(bitmap_dib_info);

    using pixel_array = std::vector<std::vector<pixel>>;

    void save(const std::string& path, const pixel_array& data);

    void gaussian(pixel_array& data, const double_t& rate);

    void upscale(pixel_array& data, const size_t& rate);

    pixel_array from_numbers(const std::vector<std::vector<uint64_t>>& data, const color_distribution& colors = color_distribution());
}