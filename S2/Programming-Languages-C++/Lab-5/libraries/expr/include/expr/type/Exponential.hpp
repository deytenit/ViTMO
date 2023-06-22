#ifndef EXPR_EXPONENTIAL_HPP
#define EXPR_EXPONENTIAL_HPP

#include <cmath>
#include <cstdint>
#include <string>

class Exponential {
public:
    using data_type = int64_t;

    static const data_type MAX_DT;
    static const data_type MIN_DT;

    static const Exponential ZERO;
    static const Exponential NA;
    static const Exponential INF;

    Exponential()                         = default;
    Exponential(const Exponential& other) = default;
    Exponential(Exponential&& other)      = default;
    Exponential(data_type significand, data_type exponent) noexcept;
    template <typename T, typename = std::enable_if_t<std::is_scalar_v<T>>>
    Exponential(T value);

    ~Exponential() = default;

    Exponential& operator=(const Exponential& other) = default;
    Exponential& operator=(Exponential&& other)      = default;
    template <typename T, typename = std::enable_if_t<std::is_scalar_v<T>>>
    Exponential& operator=(const T& value);

    template <typename T, typename = std::enable_if_t<std::is_scalar_v<T>>>
    operator T() const;

    friend bool operator==(const Exponential& lhs, const Exponential& rhs);
    friend bool operator!=(const Exponential& lhs, const Exponential& rhs);
    friend bool operator>(const Exponential& lhs, const Exponential& rhs);
    friend bool operator<(const Exponential& lhs, const Exponential& rhs);
    friend bool operator>=(const Exponential& lhs, const Exponential& rhs);
    friend bool operator<=(const Exponential& lhs, const Exponential& rhs);

    Exponential operator-() const;
    Exponential operator+() const;

    Exponential& operator++();
    Exponential operator++(int);

    Exponential& operator--();
    Exponential operator--(int);

    friend Exponential operator+(const Exponential& lhs, const Exponential& rhs);
    friend Exponential operator-(const Exponential& lhs, const Exponential& rhs);
    friend Exponential operator*(const Exponential& lhs, const Exponential& rhs);
    friend Exponential operator/(const Exponential& lhs, const Exponential& rhs);

    Exponential& operator+=(const Exponential& other);
    Exponential& operator-=(const Exponential& other);
    Exponential& operator*=(const Exponential& other);
    Exponential& operator/=(const Exponential& other);

    [[nodiscard]] data_type significand() const;
    [[nodiscard]] data_type exponent() const;
    [[nodiscard]] bool isinf() const;
    [[nodiscard]] bool isnan() const;

    [[nodiscard]] std::string str() const;

protected:
    Exponential& normalize();

    [[nodiscard]] int compare(const Exponential& other) const;

    template <typename T, typename = std::enable_if_t<std::is_scalar_v<T>>>
    static Exponential compute_from(T value);

    template <typename T, typename = std::enable_if_t<std::is_scalar_v<T>>>
    static T compute_to(const Exponential& value);

private:
    using calc_type  = __int128_t;
    using ucalc_type = __uint128_t;

    data_type m_significand = 0;
    data_type m_exponent    = 0;
};

template <typename T, typename>
Exponential::Exponential(const T value) : Exponential(compute_from(value)) {}

template <typename T, typename>
Exponential& Exponential::operator=(const T& value) {
    *this = compute_from(value);
    return *this;
}

template <typename T, typename>
Exponential::operator T() const {
    return compute_to<T>(*this);
}

bool operator==(const Exponential& lhs, const Exponential& rhs);
template <typename T, typename = std::enable_if_t<std::is_scalar_v<T>>>
bool operator==(const Exponential& lhs, const T& rhs) {
    return lhs == Exponential(rhs);
}
template <typename T, typename = std::enable_if_t<std::is_scalar_v<T>>>
bool operator==(const T& lhs, const Exponential& rhs) {
    return Exponential(lhs) == rhs;
}
bool operator!=(const Exponential& lhs, const Exponential& rhs);
template <typename T, typename = std::enable_if_t<std::is_scalar_v<T>>>
bool operator!=(const Exponential& lhs, const T& rhs) {
    return lhs != Exponential(rhs);
}
template <typename T, typename = std::enable_if_t<std::is_scalar_v<T>>>
bool operator!=(const T& lhs, const Exponential& rhs) {
    return Exponential(lhs) != rhs;
}
bool operator>(const Exponential& lhs, const Exponential& rhs);
template <typename T, typename = std::enable_if_t<std::is_scalar_v<T>>>
bool operator>(const Exponential& lhs, const T& rhs) {
    return lhs > Exponential(rhs);
}
template <typename T, typename = std::enable_if_t<std::is_scalar_v<T>>>
bool operator>(const T& lhs, const Exponential& rhs) {
    return Exponential(lhs) > rhs;
}
bool operator>=(const Exponential& lhs, const Exponential& rhs);
template <typename T, typename = std::enable_if_t<std::is_scalar_v<T>>>
bool operator>=(const Exponential& lhs, const T& rhs) {
    return lhs >= Exponential(rhs);
}
template <typename T, typename = std::enable_if_t<std::is_scalar_v<T>>>
bool operator>=(const T& lhs, const Exponential& rhs) {
    return Exponential(lhs) >= rhs;
}
bool operator<(const Exponential& lhs, const Exponential& rhs);
template <typename T, typename = std::enable_if_t<std::is_scalar_v<T>>>
bool operator<(const Exponential& lhs, const T& rhs) {
    return lhs < Exponential(rhs);
}
template <typename T, typename = std::enable_if_t<std::is_scalar_v<T>>>
bool operator<(const T& lhs, const Exponential& rhs) {
    return Exponential(lhs) < rhs;
}
bool operator<=(const Exponential& lhs, const Exponential& rhs);
template <typename T, typename = std::enable_if_t<std::is_scalar_v<T>>>
bool operator<=(const Exponential& lhs, const T& rhs) {
    return lhs <= Exponential(rhs);
}
template <typename T, typename = std::enable_if_t<std::is_scalar_v<T>>>
bool operator<=(const T& lhs, const Exponential& rhs) {
    return Exponential(lhs) <= rhs;
}

Exponential operator+(const Exponential& lhs, const Exponential& rhs);
template <typename T, typename = std::enable_if_t<std::is_scalar_v<T>>>
Exponential operator+(const Exponential& lhs, const T& rhs) {
    return lhs + Exponential(rhs);
}
template <typename T, typename = std::enable_if_t<std::is_scalar_v<T>>>
Exponential operator+(const T& lhs, const Exponential& rhs) {
    return Exponential(lhs) + rhs;
}
Exponential operator-(const Exponential& lhs, const Exponential& rhs);
template <typename T, typename = std::enable_if_t<std::is_scalar_v<T>>>
Exponential operator-(const Exponential& lhs, const T& rhs) {
    return lhs - Exponential(rhs);
}
template <typename T, typename = std::enable_if_t<std::is_scalar_v<T>>>
Exponential operator-(const T& lhs, const Exponential& rhs) {
    return Exponential(lhs) - rhs;
}
Exponential operator*(const Exponential& lhs, const Exponential& rhs);
template <typename T, typename = std::enable_if_t<std::is_scalar_v<T>>>
Exponential operator*(const Exponential& lhs, const T& rhs) {
    return lhs * Exponential(rhs);
}
template <typename T, typename = std::enable_if_t<std::is_scalar_v<T>>>
Exponential operator*(const T& lhs, const Exponential& rhs) {
    return Exponential(lhs) * rhs;
}
Exponential operator/(const Exponential& lhs, const Exponential& rhs);
template <typename T, typename = std::enable_if_t<std::is_scalar_v<T>>>
Exponential operator/(const Exponential& lhs, const T& rhs) {
    return lhs / Exponential(rhs);
}
template <typename T, typename = std::enable_if_t<std::is_scalar_v<T>>>
Exponential operator/(const T& lhs, const Exponential& rhs) {
    return Exponential(lhs) / rhs;
}

template <typename T, typename>
Exponential Exponential::compute_from(T value) {
    if (std::isnan(value)) {
        return NA;
    }
    if (std::isinf(value)) {
        return value < 0 ? -INF : INF;
    }

    double significand;
    data_type exponent = 0;
    while (std::modf(value, &significand) != 0.0) {
        value *= 10;
        --exponent;
    }

    return {static_cast<data_type>(value), exponent};
}

template <typename T, typename>
T Exponential::compute_to(const Exponential& value) {
    if (value.isnan()) {
        return std::numeric_limits<T>::quiet_NaN();
    }
    if (value.isinf()) {
        return value > ZERO ? std::numeric_limits<T>::infinity() : -std::numeric_limits<T>::infinity();
    }

    return static_cast<T>(compute_to<__float128>(value));
}

template <>
bool Exponential::compute_to(const Exponential& value);

template <>
__float128 Exponential::compute_to(const Exponential& value);

std::ostream& operator<<(std::ostream& out, const Exponential& object);

#endif