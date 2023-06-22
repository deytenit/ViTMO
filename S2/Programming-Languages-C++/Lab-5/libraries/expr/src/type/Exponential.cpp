#include "expr/type/Exponential.hpp"

#include <sstream>

const Exponential::data_type Exponential::MAX_DT = std::numeric_limits<data_type>::max();
const Exponential::data_type Exponential::MIN_DT = std::numeric_limits<data_type>::min();

const Exponential Exponential::ZERO(0, 0);
const Exponential Exponential::NA(0, MAX_DT);
const Exponential Exponential::INF(1, MAX_DT);

namespace {
const Exponential ONE(1, 0);

constexpr __float128 pow10f(int value) {
    __float128 result = 1;
    bool inv          = value < 0;

    value = std::abs(value);

    for (int i = 0; i < value; ++i) {
        result *= 10;
    }

    return inv ? static_cast<__float128>(1) / result : result;
}

template <typename T>
constexpr bool add_overflow(const T lhs, const T rhs) {
    return (lhs >= 0 && std::numeric_limits<T>::max() - lhs < rhs) ||
           (lhs < 0 && rhs < std::numeric_limits<T>::min() - lhs);
}

template <typename T>
constexpr bool mul_overflow(const T lhs, const T rhs) {
    return (lhs > 0 && rhs > 0 && lhs > std::numeric_limits<T>::max() / rhs) ||
           (lhs > 0 && rhs < 0 && rhs < std::numeric_limits<T>::min() / lhs) ||
           (lhs < 0 && rhs > 0 && lhs < std::numeric_limits<T>::min() / rhs) ||
           (lhs < 0 && rhs < 0 && rhs < std::numeric_limits<T>::max() / lhs);
}
}  // namespace

Exponential::Exponential(data_type significand, data_type exponent) noexcept
    : m_significand(significand), m_exponent(exponent) {
    normalize();
}

bool operator==(const Exponential& lhs, const Exponential& rhs) {
    if (lhs.isnan() || rhs.isnan()) {
        return false;
    }

    return lhs.compare(rhs) == 0;
}
bool operator!=(const Exponential& lhs, const Exponential& rhs) {
    return !(lhs == rhs);
}
bool operator>(const Exponential& lhs, const Exponential& rhs) {
    if (lhs.isnan() || rhs.isnan()) {
        return false;
    }

    return lhs.compare(rhs) > 0;
}
bool operator<(const Exponential& lhs, const Exponential& rhs) {
    if (lhs.isnan() || rhs.isnan()) {
        return false;
    }

    return lhs.compare(rhs) < 0;
}
bool operator>=(const Exponential& lhs, const Exponential& rhs) {
    return !(lhs < rhs);
}
bool operator<=(const Exponential& lhs, const Exponential& rhs) {
    if (lhs.isnan() || rhs.isnan()) {
        return false;
    }

    return !(lhs > rhs);
}

Exponential Exponential::operator-() const {
    if (m_significand == MIN_DT) {
        return {MAX_DT, m_exponent};
    }

    return {-m_significand, m_exponent};
}
Exponential Exponential::operator+() const {
    return {m_significand, m_exponent};
}

Exponential& Exponential::operator++() {
    return operator+=(ONE);
}
Exponential Exponential::operator++(int) {
    const Exponential tmp(*this);
    operator+=(ONE);
    return tmp;
}

Exponential& Exponential::operator--() {
    return operator-=(ONE);
}
Exponential Exponential::operator--(int) {
    const Exponential tmp(*this);
    operator-=(ONE);
    return tmp;
}

Exponential operator+(const Exponential& lhs, const Exponential& rhs) {
    return Exponential(lhs) += rhs;
}
Exponential operator-(const Exponential& lhs, const Exponential& rhs) {
    return Exponential(lhs) -= rhs;
}
Exponential operator*(const Exponential& lhs, const Exponential& rhs) {
    return Exponential(lhs) *= rhs;
}
Exponential operator/(const Exponential& lhs, const Exponential& rhs) {
    return Exponential(lhs) /= rhs;
}

Exponential& Exponential::operator+=(const Exponential& other) {
    if (isnan() || other.isnan()) {
        return *this = NA;
    }

    if (isinf() && other.isinf()) {
        return *this != other ? *this = NA : *this;
    }

    if (isinf()) {
        return *this;
    }

    if (other.isinf()) {
        return *this = other;
    }

    Exponential tmp(other);
    while (m_exponent > other.m_exponent && !mul_overflow(m_significand, static_cast<data_type>(10))) {
        m_significand *= 10;
        --m_exponent;
    }
    while (tmp.m_exponent > m_exponent && !mul_overflow(tmp.m_significand, static_cast<data_type>(10))) {
        tmp.m_significand *= 10;
        --tmp.m_exponent;
    }
    while (m_exponent < tmp.m_exponent && m_significand != 0) {
        m_significand /= 10;
        ++m_exponent;
    }
    while (tmp.m_exponent < m_exponent && tmp.m_significand != 0) {
        tmp.m_significand /= 10;
        ++tmp.m_exponent;
    }

    while (add_overflow(m_significand, tmp.m_significand)) {
        if (m_exponent == MAX_DT) {
            return *this = normalize();
        }
        m_significand /= 10;
        ++m_exponent;
        tmp.m_significand /= 10;
        ++tmp.m_exponent;
    }

    m_significand += tmp.m_significand;
    m_exponent = std::max(m_exponent, tmp.m_exponent);

    return normalize();
}
Exponential& Exponential::operator-=(const Exponential& other) {
    return *this += -other;
}
Exponential& Exponential::operator*=(const Exponential& other) {
    if (isnan() || other.isnan()) {
        return *this = NA;
    }

    if (isinf() || other.isinf()) {
        return *this = *this > 0 != other > 0 ? INF : -INF;
    }

    Exponential tmp(other);

    if (add_overflow(m_exponent, tmp.m_exponent)) {
        m_exponent   = MAX_DT;
        return *this = normalize();
    }

    calc_type significand = static_cast<calc_type>(m_significand) * static_cast<calc_type>(tmp.m_significand);

    while (std::abs(significand) > std::numeric_limits<data_type>::max()) {
        significand /= 10;
        ++m_exponent;
    }

    m_significand = static_cast<data_type>(significand);

    m_exponent += tmp.m_exponent;

    return normalize();
}
Exponential& Exponential::operator/=(const Exponential& other) {
    if (isnan() || other.isnan() || (isinf() && other.isinf())) {
        return *this = NA;
    }

    if (isinf()) {
        return *this = *this > 0 != other > 0 ? INF : -INF;
    }

    if (other.isinf()) {
        return *this = *this > 0 != other > 0 ? ZERO : -ZERO;
    }

    if (other.m_significand == 0) {
        m_exponent   = MAX_DT;
        return *this = normalize();
    }

    const bool sign = m_significand > 0 != other.m_significand > 0;

    ucalc_type significand_a = std::abs(static_cast<calc_type>(m_significand));
    ucalc_type significand_b = std::abs(static_cast<calc_type>(other.m_significand));

    data_type adjust = 0;
    while (significand_a < significand_b) {
        significand_a *= 10;
        ++adjust;
    }
    while (significand_a >= 10 * significand_b) {
        significand_b *= 10;
        --adjust;
    }
    data_type significand = 0;
    while (true) {
        while (significand_b <= significand_a) {
            significand_a -= significand_b;
            ++significand;
        }
        if ((significand_a == 0 || mul_overflow(significand, static_cast<data_type>(10))) && adjust >= 0) {
            break;
        }
        significand *= 10;
        significand_a *= 10;
        ++adjust;
    }

    m_significand = sign ? -significand : significand;
    m_exponent    = m_exponent - other.m_exponent - adjust;

    return normalize();
}

Exponential::data_type Exponential::significand() const {
    return m_significand;
}
Exponential::data_type Exponential::exponent() const {
    return m_exponent;
}
bool Exponential::isinf() const {
    return m_exponent == MAX_DT && m_significand != 0;
}
bool Exponential::isnan() const {
    return m_exponent == MAX_DT && m_significand == 0;
}

std::string Exponential::str() const {
    std::stringstream ss;
    ss << m_significand;
    if (m_exponent > 0) {
        ss << "e+" << m_exponent;
    }
    if (m_exponent < 0) {
        ss << 'e' << m_exponent;
    }
    return ss.str();
}

Exponential& Exponential::normalize() {
    if (m_exponent == MAX_DT) {
        return *this;
    }

    while (m_significand != 0 && m_significand % 10 == 0) {
        m_significand /= 10;
        ++m_exponent;
    }

    return *this;
}

int Exponential::compare(const Exponential& other) const {
    if (isinf() && other.isinf()) {
        if (m_significand > 0 != other.m_significand > 0) {
            return m_significand < 0 ? -1 : 1;
        }

        return 0;
    }

    if (m_exponent != other.m_exponent) {
        return m_exponent < other.m_exponent ? -1 : 1;
    }

    if (m_significand != other.m_significand) {
        return m_significand < other.m_significand ? -1 : 1;
    }

    return 0;
}

template <>
bool Exponential::compute_to(const Exponential& value) {
    return value != ZERO;
}

template <>
__float128 Exponential::compute_to(const Exponential& value) {
    if (value.isnan()) {
        return std::numeric_limits<__float128>::quiet_NaN();
    }
    if (value.isinf()) {
        return value > ZERO ? std::numeric_limits<__float128>::infinity()
                            : -std::numeric_limits<__float128>::infinity();
    }

    return value.m_significand * pow10f(static_cast<int>(value.m_exponent));
}

std::ostream& operator<<(std::ostream& out, const Exponential& object) {
    return out << object.str();
}