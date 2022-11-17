#include <vector>
#include <gtest/gtest.h>

#include <sandpile.h>
#include <gmock/gmock-matchers.h>

void assert_matrixes(const std::vector<std::vector<uint64_t>>& a, const std::vector<std::vector<uint64_t>>& b) {
    ASSERT_EQ(a.size(), b.size());
    for (size_t i = 0; i < a.size(); ++i) {
        ASSERT_EQ(a[i].size(), b[i].size());
        for (size_t j = 0; j < a[i].size(); ++j) {
            ASSERT_EQ(a[i][j], b[i][j]);
        }
    }
};

TEST(SandpileTestSuit, BaseTest) {
    const std::vector<std::vector<uint64_t>> guess1 = {
        {0, 1, 0},
        {1, 0, 1},
        {0, 1, 0}
    };

    const std::vector<std::vector<uint64_t>> guess2 = {
        {0, 0, 1, 0, 0},
        {0, 2, 0, 2, 0},
        {1, 0, 4, 0, 1},    
        {0, 2, 0, 2, 0},
        {0, 0, 1, 0, 0}
    };

    sandpile a({{4}});

    a.simulate();

    assert_matrixes(guess1, a.to_vector());

    sandpile b({{16}});

    b.simulate();

    b.simulate();

    assert_matrixes(guess2, b.to_vector());
}
