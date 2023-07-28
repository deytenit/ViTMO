#include "SeamCarver.hpp"

#include <algorithm>
#include <iterator>
#include <limits>

namespace {
inline double sqr(const double value) {
    return value * value;
}

class ColumnAdapter {
public:
    explicit ColumnAdapter(size_t column) : m_column(column) {}
    bool operator()(const std::vector<double>& lhs, const std::vector<double>& rhs) const {
        return lhs.at(m_column) < rhs.at(m_column);
    }

private:
    size_t m_column;
};
}  // namespace

SeamCarver::SeamCarver(Image image) : m_image(std::move(image)) {}

const Image& SeamCarver::GetImage() const {
    return m_image;
}

size_t SeamCarver::GetImageWidth() const {
    return m_image.m_table.size();
}

size_t SeamCarver::GetImageHeight() const {
    return !m_image.m_table.empty() ? m_image.m_table.front().size() : 0;
}

double SeamCarver::GetPixelEnergy(const size_t columnId, const size_t rowId) const {
    const Image::Pixel l_pixel = m_image.GetPixel(columnId != 0 ? columnId - 1 : GetImageWidth() - 1, rowId);
    const Image::Pixel r_pixel = m_image.GetPixel(columnId + 1 != GetImageWidth() ? columnId + 1 : 0, rowId);
    const Image::Pixel u_pixel = m_image.GetPixel(columnId, rowId != 0 ? rowId - 1 : GetImageHeight() - 1);
    const Image::Pixel d_pixel = m_image.GetPixel(columnId, rowId + 1 != GetImageHeight() ? rowId + 1 : 0);

    const double dy = sqr(r_pixel.m_red - l_pixel.m_red) + sqr(r_pixel.m_green - l_pixel.m_green) +
                      sqr(r_pixel.m_blue - l_pixel.m_blue);
    const double dx = sqr(u_pixel.m_red - d_pixel.m_red) + sqr(u_pixel.m_green - d_pixel.m_green) +
                      sqr(u_pixel.m_blue - d_pixel.m_blue);

    return std::sqrt(dx + dy);
}

SeamCarver::Seam SeamCarver::FindHorizontalSeam() const {
    const size_t W = GetImageWidth();
    const size_t H = GetImageHeight();

    SeamSums sums(W, std::vector<double>(H, 0));

    for (size_t row = 0; row < H; ++row) {
        sums[0][row] = GetPixelEnergy(0, row);
    }

    for (size_t col = 1; col < W; ++col) {
        for (size_t row = 0; row < H; ++row) {
            const double u_sum  = sums[col - 1][row];
            const double ul_sum = row != 0 ? sums[col - 1][row - 1] : std::numeric_limits<double>::max();
            const double ur_sum = row + 1 != H ? sums[col - 1][row + 1] : std::numeric_limits<double>::max();

            sums[col][row] = GetPixelEnergy(col, row) + std::min({u_sum, ul_sum, ur_sum});
        }
    }

    SeamCarver::Seam result(W, 0);

    result[W - 1] = std::distance(sums.back().begin(), std::min_element(sums.back().begin(), sums.back().end()));

    for (size_t row = W - 1; row-- > 0;) {
        const size_t prev = result[row + 1];

        auto begin = sums[row].begin();
        auto end   = sums[row].begin();

        std::advance(begin, prev != 0 ? prev - 1 : prev);
        std::advance(end, prev + 1 != W ? prev + 2 : prev + 1);

        result[row] = std::distance(sums[row].begin(), std::min_element(begin, end));
    }

    return result;
}

SeamCarver::Seam SeamCarver::FindVerticalSeam() const {
    const size_t W = GetImageWidth();
    const size_t H = GetImageHeight();

    SeamSums sums(W, std::vector<double>(H, 0));

    for (size_t col = 0; col < W; ++col) {
        sums[col][0] = GetPixelEnergy(col, 0);
    }

    for (size_t row = 1; row < H; ++row) {
        for (size_t col = 0; col < W; ++col) {
            const double u_sum  = sums[col][row - 1];
            const double ul_sum = col != 0 ? sums[col - 1][row - 1] : std::numeric_limits<double>::max();
            const double ur_sum = col + 1 != W ? sums[col + 1][row - 1] : std::numeric_limits<double>::max();

            sums[col][row] = GetPixelEnergy(col, row) + std::min({u_sum, ul_sum, ur_sum});
        }
    }

    SeamCarver::Seam result(H, 0);

    result[H - 1] = std::distance(sums.begin(), std::min_element(sums.begin(), sums.end(), ColumnAdapter(H - 1)));

    for (size_t row = H - 1; row-- > 0;) {
        const size_t prev = result[row + 1];

        auto begin = sums.begin();
        auto end   = sums.begin();

        std::advance(begin, prev != 0 ? prev - 1 : prev);
        std::advance(end, prev + 1 != W ? prev + 2 : prev + 1);

        result[row] = std::distance(sums.begin(), std::min_element(begin, end, ColumnAdapter(row)));
    }

    return result;
}

void SeamCarver::RemoveHorizontalSeam(const Seam& seam) {
    for (size_t col = 0; col < seam.size(); ++col) {
        auto pos = m_image.m_table[col].begin();

        std::advance(pos, seam[col]);

        m_image.m_table[col].erase(pos);
    }
}

void SeamCarver::RemoveVerticalSeam(const Seam& seam) {
    for (size_t row = 0; row < seam.size(); ++row) {
        for (size_t col = seam[row]; col < GetImageWidth() - 1; ++col) {
            m_image.m_table[col][row] = m_image.m_table[col + 1][row];
        }
    }

    m_image.m_table.pop_back();
}
