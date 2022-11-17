#include <vector>
#include <gtest/gtest.h>

#include <sandbox.h>

void assert_matrixes(const std::vector<std::vector<uint64_t>>& a, const std::vector<std::vector<uint64_t>>& b) {
    ASSERT_EQ(a.size(), b.size());
    for (size_t i = 0; i < a.size(); ++i) {
        ASSERT_EQ(a[i].size(), b[i].size());
        for (size_t j = 0; j < a[i].size(); ++j) {
            ASSERT_EQ(a[i][j], b[i][j]);
        }
    }
};

TEST(SandboxTestSuit, IOTest) {
    const std::vector<std::vector<uint64_t>> in = {{1, 2, 3}, {4, 5, 6}};

    const sandbox a(in);

    assert_matrixes(in, a.to_vector());
}

TEST(SandboxTestSuit, MutationTest) {
    const std::vector<std::vector<uint64_t>> base = {{1, 2, 3}};
    const std::vector<std::vector<uint64_t>> dist_up = {
        {2, 0, 0},
        {1, 2, 3}
    };

    const std::vector<std::vector<uint64_t>> dist_down = {
        {2, 0, 0},
        {1, 2, 3},
        {0, 0, 4}
    };

    const std::vector<std::vector<uint64_t>> dist_left = {
        {3, 2, 0, 0},
        {0, 1, 2, 3},
        {0, 0, 0, 4}
    };

    const std::vector<std::vector<uint64_t>> dist_right = {
        {3, 2, 0, 0, 0},
        {0, 1, 2, 3, 0},
        {0, 0, 0, 4, 5}
    };


    sandbox a(base);

    a.distribute_up(a.begin(), 2);

    assert_matrixes(dist_up, a.to_vector());

    for (auto it = a.begin(); it != a.end(); ++it) {
        if (it->right_ == nullptr && it->down_ == nullptr) {
            a.distribute_down(it, 4);
            break;
        }
    }

    assert_matrixes(dist_down, a.to_vector());

    a.distribute_left(a.begin(), 3);

    assert_matrixes(dist_left, a.to_vector());

    for (auto it = a.begin(); it != a.end(); ++it) {
        if (it->right_ == nullptr && it->down_ == nullptr) {
            a.distribute_right(it, 5);
            break;
        }
    }

    assert_matrixes(dist_right, a.to_vector());
}
