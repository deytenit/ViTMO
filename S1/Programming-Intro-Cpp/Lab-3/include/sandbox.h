#pragma once
#include <cstdint>
#include <iterator>
#include <vector>

class sandbox {
protected:
    struct cell {

        uint64_t value_ = 0;
        cell* up_ = nullptr;
        cell* down_ = nullptr;
        cell* right_ = nullptr;
        cell* left_ = nullptr;

        cell() = default;
        cell(const uint64_t& value) : value_(value) {}
    };
public:
    class iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = cell;
        using pointer           = cell*;
        using reference         = cell&;

    private:
        pointer c_ptr_;
        pointer r_ptr_;
    public:
        iterator(pointer ptr) : c_ptr_(ptr), r_ptr_(ptr) {}

        reference operator*() const;

        pointer operator->() const;

        iterator& operator++();

        iterator operator++(int);

        friend bool operator== (const iterator& a, const iterator& b);

        friend bool operator!= (const iterator& a, const iterator& b);
    };
private:
    uint16_t max_height_ = UINT16_MAX;
    uint16_t max_width_ = UINT16_MAX;

    cell* head_;

    uint16_t height_ = 0;
    uint16_t width_ = 0;

public:
    sandbox() = default;
    sandbox(const std::vector<std::vector<uint64_t>>& data);

    [[nodiscard]] iterator begin() const;

    [[nodiscard]] iterator end() const;

    void set_max_height(const uint16_t& height);
    void set_max_width(const uint16_t& width);

    void distribute_up(iterator it, const uint64_t& value);

    void distribute_down(iterator it, const uint64_t& value);

    void distribute_right(iterator it, const uint64_t& value);

    void distribute_left(iterator it, const uint64_t& value);

    void clear() const;

    [[nodiscard]] std::vector<std::vector<uint64_t>> to_vector() const;
private:
    void normalize(cell* overflowed);
};
