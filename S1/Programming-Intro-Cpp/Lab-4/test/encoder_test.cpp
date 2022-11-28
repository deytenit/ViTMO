#include <cstdint>
#include <vector>
#include <filesystem>
#include <gtest/gtest.h>

#include <encoder.h>

namespace fs = std::filesystem;

void assert_vec(const std::vector<bool>& a, const std::vector<bool>& b) {
    ASSERT_EQ(a.size(), b.size());
    for (size_t i = 0; i < a.size(); ++i) {
        ASSERT_EQ(a[i], b[i]);
    }
}

TEST(EncoderTestSuite, EncodingTest) {
    std::vector<bool> in = {1, 1, 0, 0};
    std::vector<bool> should = {0, 1, 1, 1, 1, 0, 0};

    encoder::encode(in);

    assert_vec(in, should);

    in = {1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1};
    should = {1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1};
    encoder::encode(in);

    assert_vec(in, should);
}

TEST(EncoderTestSuite, DecodingTest) {
    std::vector<bool> should = {1, 1, 0, 0};
    std::vector<bool> in = {0, 1, 1, 1, 1, 0, 0};

    encoder::decode(in);

    assert_vec(in, should);

    should = {1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1};
    in = {1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1};

    encoder::decode(in);

    assert_vec(in, should);
}

TEST(EncoderTestSuite, HealingTest) {
    std::vector<bool> in = {0, 1, 1, 0, 1, 0, 0};
    std::vector<bool> should = {0, 1, 1, 1, 1, 0, 0};

    encoder::recover(in);

    assert_vec(in, should);

    in     = {1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1};
    should = {1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1};

    encoder::recover(in);

    assert_vec(in, should);
}


TEST(EncoderTestSuite, IOTEST) {

    fs::path file_chr = fs::temp_directory_path() / fs::path("encoder_test_chr.cpp.txt");

    std::ofstream tmp_chr(file_chr, std::ios::out | std::ios::trunc);
    tmp_chr.close();

    encoder::OEStream eout_chr(file_chr, 8);

    char chr_1[2] = {'a', 0};
    char chr_2[2] = {'b', 0};

    eout_chr.write(chr_1, 2);
    eout_chr.write(chr_2, 2);

    eout_chr.close();

    encoder::IEStream ein_chr(file_chr, 8);

    char out_chr_1[2] = {0};
    char out_chr_2[2] = {0};

    ein_chr.read(out_chr_1, 2);
    ein_chr.read(out_chr_2, 2);

    ein_chr.close();

    for (int i = 0; i < 2; ++i) {
        ASSERT_EQ(chr_1[i], out_chr_1[i]);
    }

    for (int i = 0; i < 2; ++i) {
        ASSERT_EQ(chr_2[i], out_chr_2[i]);
    }

    fs::path file = fs::temp_directory_path() / fs::path("encoder_test.cpp.txt");

    std::ofstream tmp(file, std::ios::out | std::ios::trunc);
    tmp.close();

    encoder::OEStream eout(file, 8);

    char in_data[512] = "chimingus_imposter_sus_baka_abobus";
    char in_data_2[512] = "Follow up string.";

    eout.write(in_data, 512);
    eout.write(in_data_2, 512);

    eout.close();

    encoder::IEStream ein(file, 8);

    char out_data[512] = {0};
    char out_data_2[512] = {0};

    ein.read(out_data, 512);
    ein.read(out_data_2, 512);

    ein.close();

    for (int i = 0; i < 35; ++i) {
        ASSERT_EQ(in_data[i], out_data[i]);
    }

    for (int i = 0; i < 17; ++i) {
        ASSERT_EQ(in_data_2[i], out_data_2[i]);
    }
}
