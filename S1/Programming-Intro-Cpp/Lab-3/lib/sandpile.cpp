#include "sandpile.h"

sandpile::sandpile() : sandbox({{generate_random_init_number()}}) {
    unresolved_.insert(&(*begin()));
}

sandpile::sandpile(const std::vector<std::vector<uint64_t>>& data) : sandbox(data) {
    for (auto& it : *this) {
        if (it.value_ >= cell_limit_) {
            unresolved_.insert(&it);
        }
    }
}

void sandpile::set_limit(const uint64_t& limit) {
    cell_limit_ = limit + limit % 4;
    cell_limit_power_ = log2(cell_limit_);
}

void sandpile::topple(const iterator& it) {
    const uint64_t overflow = it->value_ >> cell_limit_power_;
    const uint64_t leftover = it->value_ - (overflow << cell_limit_power_);

    it->value_ = leftover;

    distribute_up(it, overflow);
    distribute_down(it, overflow);
    distribute_right(it, overflow);
    distribute_left(it, overflow);

    if (it->up_ != nullptr && it->up_->value_ >= cell_limit_) {
        next_unresolved_.insert(it->up_);
    }

    if (it->down_ != nullptr && it->down_->value_ >= cell_limit_) {
        next_unresolved_.insert(it->down_);
    }

    if (it->right_ != nullptr && it->right_->value_ >= cell_limit_) {
        next_unresolved_.insert(it->right_);
    }

    if (it->left_ != nullptr && it->left_->value_ >= cell_limit_) {
        next_unresolved_.insert(it->left_);
    }
}

void sandpile::simulate() {
    while (!unresolved_.empty()) {
        topple(*unresolved_.begin());
        unresolved_.erase(unresolved_.begin());
    }

    unresolved_ = std::move(next_unresolved_);
}

void sandpile::compile() {
    while (!unresolved_.empty()) {
        simulate();
    }
}

void sandpile::compile(const uint32_t& iterations) {
    for (uint32_t i = 0; i < iterations && !unresolved_.empty(); ++i) {
        simulate();
    }
}

uint64_t sandpile::generate_random_init_number() {
    std::random_device rd;
    std::mt19937_64 mersenne(rd());

    return mersenne();
}

uint64_t sandpile::log2(uint64_t value) {
    uint64_t level = 0;
    while (value >>= 1) {
        ++level;
    }

    return level;
}

bool sandpile::resolved() const {
    return unresolved_.empty();
}
