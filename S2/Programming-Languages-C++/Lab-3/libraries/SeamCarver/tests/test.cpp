#include <cmath>

#include "SeamCarver.hpp"
#include "gtest/gtest.h"

TEST(SeamCarvingTests, ImageEnergy3x4) {
    const std::vector<Image::Pixel> col0 = {Image::Pixel(255, 101, 51), Image::Pixel(255, 153, 51),
                                            Image::Pixel(255, 203, 51), Image::Pixel(255, 255, 51)};
    const std::vector<Image::Pixel> col1 = {Image::Pixel(255, 101, 153), Image::Pixel(255, 153, 153),
                                            Image::Pixel(255, 204, 153), Image::Pixel(255, 255, 153)};
    const std::vector<Image::Pixel> col2 = {Image::Pixel(255, 101, 255), Image::Pixel(255, 153, 255),
                                            Image::Pixel(255, 205, 255), Image::Pixel(255, 255, 255)};

    const Image image({col0, col1, col2});
    SeamCarver carver(image);

    EXPECT_DOUBLE_EQ(sqrt(20808.0), carver.GetPixelEnergy(0, 0));
    EXPECT_DOUBLE_EQ(sqrt(52020.0), carver.GetPixelEnergy(1, 0));
    EXPECT_DOUBLE_EQ(sqrt(20808.0), carver.GetPixelEnergy(2, 0));

    EXPECT_DOUBLE_EQ(sqrt(20808.0), carver.GetPixelEnergy(0, 1));
    EXPECT_DOUBLE_EQ(sqrt(52225.0), carver.GetPixelEnergy(1, 1));
    EXPECT_DOUBLE_EQ(sqrt(21220.0), carver.GetPixelEnergy(2, 1));

    EXPECT_DOUBLE_EQ(sqrt(20809.0), carver.GetPixelEnergy(0, 2));
    EXPECT_DOUBLE_EQ(sqrt(52024.0), carver.GetPixelEnergy(1, 2));
    EXPECT_DOUBLE_EQ(sqrt(20809.0), carver.GetPixelEnergy(2, 2));

    EXPECT_DOUBLE_EQ(sqrt(20808.0), carver.GetPixelEnergy(0, 3));
    EXPECT_DOUBLE_EQ(sqrt(52225.0), carver.GetPixelEnergy(1, 3));
    EXPECT_DOUBLE_EQ(sqrt(21220.0), carver.GetPixelEnergy(2, 3));
}

TEST(SeamCarvingTests, ImageVerticalSeam3x4) {
    const std::vector<Image::Pixel> col0 = {Image::Pixel(255, 101, 51), Image::Pixel(255, 153, 51),
                                            Image::Pixel(255, 203, 51), Image::Pixel(255, 255, 51)};
    const std::vector<Image::Pixel> col1 = {Image::Pixel(255, 101, 153), Image::Pixel(255, 153, 153),
                                            Image::Pixel(255, 204, 153), Image::Pixel(255, 255, 153)};
    const std::vector<Image::Pixel> col2 = {Image::Pixel(255, 101, 255), Image::Pixel(255, 153, 255),
                                            Image::Pixel(255, 205, 255), Image::Pixel(255, 255, 255)};

    const Image image({col0, col1, col2});
    SeamCarver carver(image);

    std::vector<size_t> seam = carver.FindVerticalSeam();
    ASSERT_EQ(4, seam.size());
    EXPECT_EQ(0, seam.at(0));
    EXPECT_EQ(0, seam.at(1));
    EXPECT_EQ(0, seam.at(2));
    EXPECT_EQ(0, seam.at(3));

    carver.RemoveVerticalSeam(seam);
    ASSERT_EQ(2, carver.GetImageWidth());
    ASSERT_EQ(4, carver.GetImageHeight());
}

TEST(SeamCarvingTests, ImageHorizontalSeam3x4) {
    const std::vector<Image::Pixel> col0 = {Image::Pixel(255, 101, 51), Image::Pixel(255, 153, 51),
                                            Image::Pixel(255, 203, 51), Image::Pixel(255, 255, 51)};
    const std::vector<Image::Pixel> col1 = {Image::Pixel(255, 101, 153), Image::Pixel(255, 153, 153),
                                            Image::Pixel(255, 204, 153), Image::Pixel(255, 255, 153)};
    const std::vector<Image::Pixel> col2 = {Image::Pixel(255, 101, 255), Image::Pixel(255, 153, 255),
                                            Image::Pixel(255, 205, 255), Image::Pixel(255, 255, 255)};

    const Image image({col0, col1, col2});
    SeamCarver carver(image);

    std::vector<size_t> seam = carver.FindHorizontalSeam();
    ASSERT_EQ(3, seam.size());
    EXPECT_EQ(0, seam.at(0));
    EXPECT_EQ(0, seam.at(1));
    EXPECT_EQ(0, seam.at(2));

    carver.RemoveHorizontalSeam(seam);
    ASSERT_EQ(3, carver.GetImageWidth());
    ASSERT_EQ(3, carver.GetImageHeight());
}

TEST(SeamCarvingTests, ImageVerticalSeam6x5) {
    const std::vector<Image::Pixel> col0 = {Image::Pixel(78, 209, 79), Image::Pixel(224, 191, 182),
                                            Image::Pixel(117, 189, 149), Image::Pixel(163, 222, 132),
                                            Image::Pixel(211, 120, 173)};
    const std::vector<Image::Pixel> col1 = {Image::Pixel(63, 118, 247), Image::Pixel(108, 89, 82),
                                            Image::Pixel(171, 231, 153), Image::Pixel(187, 117, 183),
                                            Image::Pixel(188, 218, 244)};
    const std::vector<Image::Pixel> col2 = {Image::Pixel(92, 175, 95), Image::Pixel(80, 196, 230),
                                            Image::Pixel(149, 164, 168), Image::Pixel(92, 145, 69),
                                            Image::Pixel(214, 103, 68)};
    const std::vector<Image::Pixel> col3 = {Image::Pixel(243, 73, 183), Image::Pixel(112, 156, 180),
                                            Image::Pixel(107, 119, 71), Image::Pixel(158, 143, 79),
                                            Image::Pixel(163, 166, 246)};
    const std::vector<Image::Pixel> col4 = {Image::Pixel(210, 109, 104), Image::Pixel(176, 178, 120),
                                            Image::Pixel(120, 105, 138), Image::Pixel(220, 75, 222),
                                            Image::Pixel(79, 125, 246)};
    const std::vector<Image::Pixel> col5 = {Image::Pixel(252, 101, 119), Image::Pixel(141, 151, 142),
                                            Image::Pixel(163, 174, 196), Image::Pixel(189, 73, 214),
                                            Image::Pixel(211, 201, 98)};

    const Image image({col0, col1, col2, col3, col4, col5});
    SeamCarver carver(image);

    std::vector<size_t> seam = carver.FindVerticalSeam();
    ASSERT_EQ(5, seam.size());

    EXPECT_EQ(3, seam.at(0));
    EXPECT_EQ(4, seam.at(1));
    EXPECT_EQ(3, seam.at(2));
    EXPECT_EQ(2, seam.at(3));
    EXPECT_EQ(2, seam.at(4));

    carver.RemoveVerticalSeam(seam);
    ASSERT_EQ(5, carver.GetImageWidth());
    ASSERT_EQ(5, carver.GetImageHeight());
}

TEST(SeamCarvingTests, ImageHorizontalSeam6x5) {
    const std::vector<Image::Pixel> col0 = {Image::Pixel(78, 209, 79), Image::Pixel(224, 191, 182),
                                            Image::Pixel(117, 189, 149), Image::Pixel(163, 222, 132),
                                            Image::Pixel(211, 120, 173)};
    const std::vector<Image::Pixel> col1 = {Image::Pixel(63, 118, 247), Image::Pixel(108, 89, 82),
                                            Image::Pixel(171, 231, 153), Image::Pixel(187, 117, 183),
                                            Image::Pixel(188, 218, 244)};
    const std::vector<Image::Pixel> col2 = {Image::Pixel(92, 175, 95), Image::Pixel(80, 196, 230),
                                            Image::Pixel(149, 164, 168), Image::Pixel(92, 145, 69),
                                            Image::Pixel(214, 103, 68)};
    const std::vector<Image::Pixel> col3 = {Image::Pixel(243, 73, 183), Image::Pixel(112, 156, 180),
                                            Image::Pixel(107, 119, 71), Image::Pixel(158, 143, 79),
                                            Image::Pixel(163, 166, 246)};
    const std::vector<Image::Pixel> col4 = {Image::Pixel(210, 109, 104), Image::Pixel(176, 178, 120),
                                            Image::Pixel(120, 105, 138), Image::Pixel(220, 75, 222),
                                            Image::Pixel(79, 125, 246)};
    const std::vector<Image::Pixel> col5 = {Image::Pixel(252, 101, 119), Image::Pixel(141, 151, 142),
                                            Image::Pixel(163, 174, 196), Image::Pixel(189, 73, 214),
                                            Image::Pixel(211, 201, 98)};

    const Image image({col0, col1, col2, col3, col4, col5});
    SeamCarver carver(image);

    std::vector<size_t> seam = carver.FindHorizontalSeam();
    ASSERT_EQ(6, seam.size());

    EXPECT_EQ(2, seam.at(0));
    EXPECT_EQ(2, seam.at(1));
    EXPECT_EQ(1, seam.at(2));
    EXPECT_EQ(2, seam.at(3));
    EXPECT_EQ(1, seam.at(4));
    EXPECT_EQ(2, seam.at(5));

    carver.RemoveHorizontalSeam(seam);
    ASSERT_EQ(6, carver.GetImageWidth());
    ASSERT_EQ(4, carver.GetImageHeight());
}

TEST(SeamCarvingTests, ImageEnergy1x3) {
    const std::vector<Image::Pixel> col0 = {Image::Pixel(0, 1, 2), Image::Pixel(30, 40, 50), Image::Pixel(6, 7, 8)};

    const Image image({col0});
    SeamCarver carver(image);

    EXPECT_DOUBLE_EQ(sqrt(3429), carver.GetPixelEnergy(0, 0));
    EXPECT_DOUBLE_EQ(sqrt(108), carver.GetPixelEnergy(0, 1));
    EXPECT_DOUBLE_EQ(sqrt(4725), carver.GetPixelEnergy(0, 2));
}

TEST(SeamCarvingTests, ImageEnergy3x1) {
    const std::vector<Image::Pixel> col0 = {Image::Pixel(0, 1, 2)};
    const std::vector<Image::Pixel> col1 = {Image::Pixel(30, 40, 50)};
    const std::vector<Image::Pixel> col2 = {Image::Pixel(6, 7, 8)};

    const Image image({col0, col1, col2});
    SeamCarver carver(image);

    EXPECT_DOUBLE_EQ(sqrt(3429), carver.GetPixelEnergy(0, 0));
    EXPECT_DOUBLE_EQ(sqrt(108), carver.GetPixelEnergy(1, 0));
    EXPECT_DOUBLE_EQ(sqrt(4725), carver.GetPixelEnergy(2, 0));
}
