#include "number.h"

void uint2022_t::trim() {
    while (!value.empty() && value.back() == 0) {
        value.pop_back();
    }
}

uint2022_t::uint2022_t(const NT& val) {
    value = val;

    trim();
}

template <typename T, class>
uint2022_t::uint2022_t(const T& val) {
    *this = compute_from(val);
}

uint2022_t::uint2022_t(const std::string& val) {
    uint2022_t power(1);

    for (size_t i = val.size(); i > 0; i--) {
        *this += power * (val[i - 1] - '0');
        power *= 10;
    }
}

uint2022_t& uint2022_t::operator=(const NT& val) {
    value = val;

    trim();

    return *this;
}

template <typename T, class>
uint2022_t uint2022_t::compute_from(const T& val) {
    uint2022_t out;
    DT next = 0;

    for (size_t i = 0; i < sizeof(T) << 3; i++) {
        if (i != 0 && i % BITS_DIGIT == 0) {
            out.value.push_back(next);
            next = 0;
        }

        next |= val & static_cast<T>(1 << i);
    }

    if (next != 0) {
        out.value.push_back(next);
    }

    return out;
}

template <typename T, class>
T uint2022_t::compute_to(const uint2022_t& val) {
    T out = 0;

    const size_t allocated =
        std::min(val.value.size(), sizeof(T) / BYTES_DIGIT +
                                       (sizeof(T) % BYTES_DIGIT == 0 ? 0 : 1));

    for (size_t i = allocated; i > 0; --i) {
        if (sizeof(T) << 3 <= BASE_SIZE) {
            out = 0;
        } else {
            out <<= BASE_SIZE;
        }
        out |= val.value[i - 1];
    }

    return out;
}

uint2022_t::spaceship uint2022_t::compare(const uint2022_t& rhs) const {
    if (value.size() < rhs.value.size()) {
        return LESS;
    }

    if (value.size() > rhs.value.size()) {
        return GREATER;
    }

    for (size_t i = value.size(); i > 0; i--) {
        if (value[i - 1] == rhs.value[i - 1]) {
            continue;
        }

        if (value[i - 1] < rhs.value[i - 1]) {
            return LESS;
        }

        return GREATER;
    }

    return EQUAL;
}

bool uint2022_t::operator==(const uint2022_t& rhs) const {
    return compare(rhs) == EQUAL;
}

bool uint2022_t::operator!=(const uint2022_t& rhs) const {
    return compare(rhs) != EQUAL;
}

bool uint2022_t::operator>(const uint2022_t& rhs) const {
    return compare(rhs) == GREATER;
}

bool uint2022_t::operator>=(const uint2022_t& rhs) const {
    return compare(rhs) != LESS;
}

bool uint2022_t::operator<(const uint2022_t& rhs) const {
    return compare(rhs) == LESS;
}

bool uint2022_t::operator<=(const uint2022_t& rhs) const {
    return compare(rhs) != GREATER;
}

void uint2022_t::fft(std::vector<std::complex<long double>>& data,
                     const bool& invert) const {
    const size_t n = data.size();

    for (size_t i = 1, j = 0; i < n; i++) {
        size_t bit = n >> 1;
        while (j >= bit) {
            j -= bit;
            bit >>= 1;
        }
        j += bit;
        if (i < j) {
            std::swap(data[i], data[j]);
        }
    }

    for (size_t len = 2; len <= n; len <<= 1) {
        const long double angle = 2 * M_PI / static_cast<long double>(len) * (invert ? -1 : 1);
        const std::complex<long double> v_size(cos(angle), sin(angle));
        for (size_t i = 0; i < n; i += len) {
            std::complex<long double> w(1);
            for (size_t j = 0; j < len / 2; ++j) {
                std::complex<long double> u = data[i + j],
                                          v = data[i + j + len / 2] * w;
                data[i + j] = u + v;
                data[i + j + len / 2] = u - v;
                w *= v_size;
            }
        }
    }
    if (invert) {
        for (size_t i = 0; i < n; ++i) {
            data[i] /= static_cast<DT>(n);
        }
    }
}

std::pair<uint2022_t, uint2022_t> uint2022_t::short_dm(
    const uint2022_t& lhs, const uint2022_t& rhs) const {
    uint2022_t out;

    DT carry = 0;

    for (size_t i = lhs.value.size(); i > 0; i--) {
        const DT current = lhs.value[i - 1] + 1ULL * carry * BASE;
        out.value.push_back(static_cast<DT>(current / rhs.value.front()));
        carry = current % rhs.value.front();
    }

    std::reverse(out.value.begin(), out.value.end());

    out.trim();

    return {out, lhs - (out * rhs)};
}

std::pair<uint2022_t, uint2022_t> uint2022_t::dm(const uint2022_t& lhs,
                                                 const uint2022_t& rhs) const {
    if (rhs == 0) {
        throw std::runtime_error("uint2022_t::dm: Division by zero.");
    }

    if (rhs == 1) {
        return {lhs, 0};
    }

    if (rhs == lhs) {
        return {1, 0};
    }

    if (lhs < rhs) {
        return {0, lhs};
    }

    uint2022_t add = 1;
    uint2022_t prev = 1;
    uint2022_t pos = 1;
    uint2022_t end = lhs;

    while (prev <= end) {
        uint2022_t tmp = rhs * pos;

        if (lhs == tmp) {
            break;
        }

        if (lhs > tmp) {
            prev = pos + 1;
            pos = std::min(pos + add, end);
            add += add;
        } else {
            end = pos - 1;
            pos = prev;
            add = 1;
        }
    }

    return {pos, lhs - (rhs * pos)};
}

uint2022_t uint2022_t::operator+(const uint2022_t& rhs) const {
    if (operator==(0)) {
        return rhs;
    }

    if (rhs == 0) {
        return *this;
    }

    uint2022_t out(*this);
    DT reminder = 0;

    for (size_t i = 0; i < rhs.value.size() || reminder != 0; i++) {
        if (out.value.size() <= i) {
            out.value.push_back(0);
        }

        const DT available = BASE - out.value[i];

        if (rhs.value.size() <= i) {
            out.value[i] = reminder;
            reminder = 0;
            continue;
        }

        if (available > rhs.value[i] && available - rhs.value[i] > reminder) {
            out.value[i] += rhs.value[i] + reminder;
            reminder = 0;
        } else {
            out.value[i] = rhs.value[i] - available + reminder;
            reminder = 1;
        }
    }

    return out;
}

uint2022_t uint2022_t::operator-(const uint2022_t& rhs) const {
    if (rhs == 0) {
        return *this;
    }

    if (*this < rhs) {
        return 0;
    }

    uint2022_t out(*this);
    bool borrowed = false;

    for (size_t i = 0; i < rhs.value.size() || borrowed; i++) {
        if (borrowed) {
            if (out.value[i] > 0) {
                out.value[i]--;
                borrowed = false;
            } else {
                out.value[i] = BASE - 1;
            }
        }

        if (rhs.value.size() <= i) {
            continue;
        }

        if (out.value[i] >= rhs.value[i]) {
            out.value[i] -= rhs.value[i];
        } else {
            out.value[i] = BASE - rhs.value[i] + out.value[i];
            borrowed = true;
        }
    }

    out.trim();

    return out;
}

uint2022_t uint2022_t::operator*(const uint2022_t& rhs) const {
    std::vector<std::complex<long double>> fa(value.begin(), value.end());
    std::vector<std::complex<long double>> fb(rhs.value.begin(), rhs.value.end());
    size_t n = 1;
    while (n < std::max(value.size(), rhs.value.size())) {
        n <<= 1;
    }
    n <<= 1;
    fa.resize(n);
    fb.resize(n);

    fft(fa, false);
    fft(fb, false);
    for (size_t i = 0; i < n; ++i) {
        fa[i] *= fb[i];
    }
    fft(fa, true);

    uint2022_t out;

    DT carry = 0;
    for (size_t i = 0; i < n; i++) {
        const DT current = carry + static_cast<DT>(fa[i].real() + 0.5);
        out.value.push_back(current % BASE);
        carry = static_cast<DT>(current) / BASE;
    }

    out.trim();

    return out;
}

uint2022_t uint2022_t::operator/(const uint2022_t& rhs) const {
    return rhs.value.size() == 1 ? short_dm(*this, rhs).first
                                 : dm(*this, rhs).first;
}

uint2022_t uint2022_t::operator%(const uint2022_t& rhs) const {
    return rhs.value.size() == 1 ? short_dm(*this, rhs).second
                                 : dm(*this, rhs).second;
}

uint2022_t& uint2022_t::operator+=(const uint2022_t& rhs) {
    *this = *this + rhs;
    return *this;
}

uint2022_t& uint2022_t::operator-=(const uint2022_t& rhs) {
    *this = *this - rhs;
    return *this;
}

uint2022_t& uint2022_t::operator*=(const uint2022_t& rhs) {
    *this = *this * rhs;
    return *this;
}

uint2022_t& uint2022_t::operator/=(const uint2022_t& rhs) {
    *this = *this / rhs;
    return *this;
}

uint2022_t& uint2022_t::operator%=(const uint2022_t& rhs) {
    *this = *this % rhs;
    return *this;
}

uint2022_t uint2022_t::operator&(const uint2022_t& rhs) const {
    uint2022_t out;

    for (size_t i = 0; i < std::min(value.size(), rhs.value.size()); i++) {
        out.value.push_back(value[i] & rhs.value[i]);
    }

    out.trim();

    return out;
}

uint2022_t uint2022_t::operator|(const uint2022_t& rhs) const {
    uint2022_t out;

    for (size_t i = 0; i < std::max(value.size(), rhs.value.size()); i++) {
        if (value.size() <= i) {
            out.value.push_back(rhs.value[i]);
        } else if (rhs.value.size() <= i) {
            out.value.push_back(value[i]);
        } else {
            out.value.push_back(value[i] | rhs.value[i]);
        }
    }

    return out;
}

uint2022_t uint2022_t::operator^(const uint2022_t& rhs) const {
    uint2022_t out;

    for (size_t i = 0; i < std::max(value.size(), rhs.value.size()); i++) {
        if (value.size() <= i) {
            out.value.push_back(rhs.value[i]);
        } else if (rhs.value.size() <= i) {
            out.value.push_back(value[i]);
        } else {
            out.value.push_back(value[i] ^ rhs.value[i]);
        }
    }

    return out;
}

uint2022_t& uint2022_t::operator&=(const uint2022_t& rhs) {
    *this = *this & rhs;
    return *this;
}

uint2022_t& uint2022_t::operator|=(const uint2022_t& rhs) {
    *this = *this | rhs;
    return *this;
}

uint2022_t& uint2022_t::operator^=(const uint2022_t& rhs) {
    *this = *this ^ rhs;
    return *this;
}

uint2022_t& uint2022_t::operator++() { return *this += 1; }

uint2022_t& uint2022_t::operator--() { return *this -= 1; }

uint2022_t uint2022_t::operator++(int) {
    uint2022_t out(*this);
    ++*this;
    return out;
}

uint2022_t uint2022_t::operator--(int) {
    uint2022_t out(*this);
    --*this;
    return out;
}

std::string uint2022_t::to_string() const {
    std::string out;
    uint2022_t tmp(*this);

    if (tmp == 0) {
        return "0";
    }

    while (tmp != 0) {
        out += '0' + static_cast<unsigned char>(tmp % 10);
        tmp /= 10;
    }

    std::reverse(out.begin(), out.end());

    return out;
}

std::ostream& operator<<(std::ostream& out, const uint2022_t& rhs) {
    out << rhs.to_string();
    return out;
}

std::istream& operator>>(std::istream& in, uint2022_t& rhs) {
    std::string val;
    in >> val;
    rhs = uint2022_t(val);
    return in;
}