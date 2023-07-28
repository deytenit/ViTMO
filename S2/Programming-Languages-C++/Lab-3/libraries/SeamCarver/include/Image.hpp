#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <cmath>
#include <iostream>
#include <vector>

struct Image {
    struct Pixel {
        Pixel(int red, int green, int blue);

        int m_red;
        int m_green;
        int m_blue;
    };

    Image(std::vector<std::vector<Pixel>> table);

    [[nodiscard]] Pixel GetPixel(const size_t columnId, const size_t rowId) const;

    std::vector<std::vector<Pixel>> m_table;
};

#endif  // IMAGE_HPP
