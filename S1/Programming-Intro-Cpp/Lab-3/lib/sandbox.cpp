#include "sandbox.h"

sandbox::iterator::reference sandbox::iterator::operator*() const {
    return *c_ptr_;
}

sandbox::iterator::pointer sandbox::iterator::operator->() const {
    return c_ptr_;
}

sandbox::iterator& sandbox::iterator::operator++() {
    if (c_ptr_->right_ == nullptr) {
        c_ptr_ = r_ptr_->down_;
        r_ptr_ = c_ptr_;
    }
    else {
        c_ptr_ = c_ptr_->right_;
    }
    return *this;
}

sandbox::iterator sandbox::iterator::operator++(int) {
    const iterator tmp = *this;
    ++(*this);
    return tmp;
}

bool operator==(const sandbox::iterator& a, const sandbox::iterator& b) {
    return a.c_ptr_ == b.c_ptr_;
}

bool operator!=(const sandbox::iterator& a, const sandbox::iterator& b) {
    return a.c_ptr_ != b.c_ptr_;
}


sandbox::sandbox(const std::vector<std::vector<uint64_t>>& data) {
    head_ = new cell(data[0][0]);
    cell* col_ptr = head_;
    cell* row_ptr = head_;

    for (uint16_t i = 1; i < data.front().size(); ++i) {
        col_ptr->right_ = new cell(data[0][i]);
        col_ptr->right_->left_ = col_ptr;
        col_ptr = col_ptr->right_;
    }

    for (uint16_t i = 1; i < data.size(); ++i) {
        row_ptr->down_ = new cell(data[i][0]);
        row_ptr->down_->up_ = row_ptr;
        row_ptr = row_ptr->down_;

        col_ptr = row_ptr;

        for (uint16_t j = 1; j < data[i].size(); ++j) {
            col_ptr->right_ = new cell(data[i][j]);
            col_ptr->right_->left_ = col_ptr;
            col_ptr->right_->up_ = col_ptr->up_->right_;
            col_ptr->right_->up_->down_ = col_ptr->right_;

            col_ptr = col_ptr->right_;
        }
    }

    height_ = data.size();
    width_ = data.front().size();
}

sandbox::iterator sandbox::begin() const {
    return {head_};
}

sandbox::iterator sandbox::end() const {
    return {nullptr};
}

void sandbox::set_max_height(const uint16_t& height) {
    max_height_ = height;
}

void sandbox::set_max_width(const uint16_t& width) {
    max_width_ = width;
}

void sandbox::distribute_up(iterator it, const uint64_t& value) {
    if (it->up_ == nullptr && height_ != max_height_) {
        it->up_ = new cell(value);
        it->up_->down_ = &(*it);

        normalize(it->up_);
        height_++;
    }
    else if (it->up_ != nullptr) {
        it->up_->value_ += value;
    }
}

void sandbox::distribute_down(iterator it, const uint64_t& value) {
    if (it->down_ == nullptr && height_ != max_height_) {
        it->down_ = new cell(value);
        it->down_->up_ = &(*it);

        normalize(it->down_);
        height_++;
    }
    else if (it->down_ != nullptr) {
        it->down_->value_ += value;
    }
}

void sandbox::distribute_right(iterator it, const uint64_t& value) {
    if (it->right_ == nullptr && width_ != max_width_) {
        it->right_ = new cell(value);
        it->right_->left_ = &(*it);

        normalize(it->right_);
        width_++;
    }
    else if (it->right_ != nullptr) {
        it->right_->value_ += value;
    }
}

void sandbox::distribute_left(iterator it, const uint64_t& value) {
    if (it->left_ == nullptr && width_ != max_width_) {
        it->left_ = new cell(value);
        it->left_->right_ = &(*it);

        normalize(it->left_);
        width_++;
    }
    else if (it->left_ != nullptr) {
        it->left_->value_ += value;
    }
}

void sandbox::clear() const {
    for (auto& it : *this) {
        if (it.up_ != nullptr) {
            it.up_->down_ = nullptr;
        }
        if (it.down_ != nullptr) {
            it.down_->up_ = nullptr;
        }
        if (it.right_ != nullptr) {
            it.right_->left_ = nullptr;
        }
        if (it.left_ != nullptr) {
            it.left_->right_ = nullptr;
        }

        delete &it;
    }

    for (auto& it : *this) {
        if (it.down_ != nullptr) {
            it.down_->up_ = nullptr;
        }
        if (it.right_ != nullptr) {
            it.right_->left_ = nullptr;
        }
        if (it.left_ != nullptr) {
            it.left_->right_ = nullptr;
        }
        if (it.up_ != nullptr) {
            it.up_->down_ = nullptr;
        }

        delete &it;
    }
}

std::vector<std::vector<uint64_t>> sandbox::to_vector() const {
    std::vector<std::vector<uint64_t>> result;

    for (const auto& it : *this) {
        if (it.left_ == nullptr) {
            result.emplace_back();
        }

        result.back().push_back(it.value_);
    }

    return result;
}

void sandbox::normalize(cell* overflowed) {
    if (overflowed->up_ != nullptr) {
        cell* tmp = overflowed;
        while (tmp->up_->right_ != nullptr) {
            tmp->right_ = new cell();
            tmp->right_->left_ = tmp;
            tmp->right_->up_ = tmp->up_->right_;
            tmp->right_->up_->down_ = tmp->right_;

            tmp = tmp->right_;
        }

        tmp = overflowed;

        while (tmp->up_->left_ != nullptr) {
            tmp->left_ = new cell();
            tmp->left_->right_ = tmp;
            tmp->left_->up_ = tmp->up_->left_;
            tmp->left_->up_->down_ = tmp->left_;

            tmp = tmp->left_;
        }
    }
    else if (overflowed->down_ != nullptr) {
        cell* tmp = overflowed;
        while (tmp->down_->right_ != nullptr) {
            tmp->right_ = new cell();
            tmp->right_->left_ = tmp;
            tmp->right_->down_ = tmp->down_->right_;
            tmp->right_->down_->up_ = tmp->right_;

            tmp = tmp->right_;
        }

        tmp = overflowed;

        while (tmp->down_->left_ != nullptr) {
            tmp->left_ = new cell();
            tmp->left_->right_ = tmp;
            tmp->left_->down_ = tmp->down_->left_;
            tmp->left_->down_->up_ = tmp->left_;

            tmp = tmp->left_;
        }

        head_ = tmp;
    }
    else if (overflowed->right_ != nullptr) {
        cell* tmp = overflowed;
        while (tmp->right_->up_ != nullptr) {
            tmp->up_ = new cell();
            tmp->up_->down_ = tmp;
            tmp->up_->right_ = tmp->right_->up_;
            tmp->up_->right_->left_ = tmp->up_;

            tmp = tmp->up_;
        }

        head_ = tmp;

        tmp = overflowed;

        while (tmp->right_->down_ != nullptr) {
            tmp->down_ = new cell();
            tmp->down_->up_ = tmp;
            tmp->down_->right_ = tmp->right_->down_;
            tmp->down_->right_->left_ = tmp->down_;

            tmp = tmp->down_;
        }
    }
    else if (overflowed->left_ != nullptr) {
        cell* tmp = overflowed;
        while (tmp->left_->up_ != nullptr) {
            tmp->up_ = new cell();
            tmp->up_->down_ = tmp;
            tmp->up_->left_ = tmp->left_->up_;
            tmp->up_->left_->right_ = tmp->up_;

            tmp = tmp->up_;
        }

        tmp = overflowed;

        while (tmp->left_->down_ != nullptr) {
            tmp->down_ = new cell();
            tmp->down_->up_ = tmp;
            tmp->down_->left_ = tmp->left_->down_;
            tmp->down_->left_->right_ = tmp->down_;

            tmp = tmp->down_;
        }
    }
}
