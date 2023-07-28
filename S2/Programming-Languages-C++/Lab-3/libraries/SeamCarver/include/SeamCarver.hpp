#ifndef SEAMCARVER_HPP
#define SEAMCARVER_HPP

#include <algorithm>
#include <array>
#include <cmath>

#include "Image.hpp"

class SeamCarver {
public:
    using Seam     = std::vector<size_t>;
    using SeamSums = std::vector<std::vector<double>>;

public:
    explicit SeamCarver(Image image);

    /**
     * Returns current image
     */
    [[nodiscard]] const Image& GetImage() const;

    /**
     * Gets current image width
     */
    [[nodiscard]] size_t GetImageWidth() const;

    /**
     * Gets current image height
     */
    [[nodiscard]] size_t GetImageHeight() const;

    /**
     * Returns pixel energy
     * @param columnId column index (x)
     * @param rowId row index (y)
     */
    [[nodiscard]] double GetPixelEnergy(size_t columnId, size_t rowId) const;

    /**
     * Returns sequence of pixel row indexes (y)
     * (x indexes are [0:W-1])
     */
    [[nodiscard]] Seam FindHorizontalSeam() const;

    /**
     * Returns sequence of pixel column indexes (x)
     * (y indexes are [0:H-1])
     */
    [[nodiscard]] Seam FindVerticalSeam() const;

    /**
     * Removes sequence of pixels from the image
     */
    void RemoveHorizontalSeam(const Seam& seam);

    /**
     * Removes sequence of pixes from the image
     */
    void RemoveVerticalSeam(const Seam& seam);

private:
    Image m_image;
};

#endif  // SEAMCARVER_HPP
