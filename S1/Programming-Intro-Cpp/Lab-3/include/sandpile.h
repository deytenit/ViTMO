#pragma once
#include <cstdint>
#include <vector>
#include <unordered_set>
#include <random>
#include <cmath>

#include <sandbox.h>

class sandpile : public sandbox {
private:
    uint64_t cell_limit_ = 4;
    uint64_t cell_limit_power_ = 2;

    std::unordered_set<cell*> unresolved_;
    std::unordered_set<cell*> next_unresolved_;
public:
    sandpile();
    sandpile(const std::vector<std::vector<uint64_t>>& data);

    void set_limit(const uint64_t& limit);

    bool resolved() const;

    void topple(const iterator& it);
    void simulate();
    void compile();
    void compile(const uint32_t& iterations);
private:
    [[nodiscard]] static uint64_t generate_random_init_number();
    static uint64_t log2(uint64_t value);
};
