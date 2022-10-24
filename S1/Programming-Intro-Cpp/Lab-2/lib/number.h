#pragma once
#include <algorithm>
#include <cinttypes>
#include <cmath>
#include <complex>
#include <iostream>
#include <type_traits>
#include <vector>

/// @brief uint2022_t is an unsigned biginteger type limited only by available memory.
class uint2022_t {
   public:
    using DT = unsigned long long;
    using NT = std::vector<DT>;

    /// @note M_PI used in fft calculations.

    static constexpr size_t BYTES_DIGIT = sizeof(DT);
    static constexpr size_t BITS_DIGIT = BYTES_DIGIT << 3;
    static constexpr size_t BASE_SIZE = BITS_DIGIT / 2 - 1;
    static constexpr DT BASE = 1ULL << BASE_SIZE;

    /// @brief Result of uint2022_t::compare function.
    enum spaceship { LESS = -1,
                     EQUAL = 0,
                     GREATER = 1 };

   private:
    /// @brief value of this in reverse every element is a digit of BASE.
    NT value;

    /// @brief As number stored in reverse, deletes leading zeroes.
    void trim();

   public:
    /// @brief Class constructors and assignment operators.
    /// @note Defaulted empty, copy and move constructors and destructor.
    /// @note Polynom constructor.
    /// @note Integral type constructor.
    /// @note String constructor.

    uint2022_t() = default;
    uint2022_t(const uint2022_t& val) = default;
    uint2022_t(uint2022_t&& obj) = default;
    uint2022_t(const NT& val);

    uint2022_t(const unsigned char& val) {
        *this = compute_from<unsigned char>(val);
    }

    uint2022_t(const unsigned short& val) {
        *this = compute_from<unsigned short>(val);
    }

    template <typename T, class = typename std::enable_if<std::is_integral<T>::value>::type>
    uint2022_t(const T& val);

    uint2022_t(const std::string& val);
    uint2022_t(const char* val) : uint2022_t(std::string(val)) {}

    ~uint2022_t() = default;

    uint2022_t& operator=(const uint2022_t& val) = default;
    uint2022_t& operator=(uint2022_t&& obj) = default;
    uint2022_t& operator=(const NT& val);
    template <typename T, class = typename std::enable_if<std::is_integral<T>::value>::type>
    uint2022_t& operator=(const T& val) {
        return *this = compute_from(val);
    }

   protected:
    /// @brief Convert passed value of integral type to uint2022_t.
    /// @tparam T Integral type to convert from.
    /// @param val Value to convert.
    /// @return New uint2022_t instance with converted val.
    template <typename T, class = typename std::enable_if<std::is_integral<T>::value>::type>
    static uint2022_t compute_from(const T& val);

    /// @brief Converts passed uint2022_t instance to value of integral type.
    /// @tparam T Integral type (except bool) to convert to.
    /// @param val uint2022_t instance.
    /// @return New T object containing sizeof(T) first bits of uint2022_t.
    template <typename T, class = typename std::enable_if<std::is_integral<T>::value>::type>
    static T compute_to(const uint2022_t& val);

   public:
    /// @brief Cast uin2022_t instance to bool.
    /// @note No need to convert just check if not zero.
    operator bool() const { return operator!=(0); }

    // uint2022_t instance to base types casters.

    operator unsigned char() const { return compute_to<unsigned char>(*this); }
    operator unsigned short() const { return compute_to<unsigned short>(*this); }
    operator unsigned int() const { return compute_to<unsigned int>(*this); }
    operator unsigned long() const { return compute_to<unsigned long>(*this); }
    operator unsigned long long() const {
        return compute_to<unsigned long long>(*this);
    }

   public:
    /// @brief Compares uint2022_t instance with another.
    /// @param rhs uint2022_t instance to compare with.
    /// @return uint2022_t::spaceship value {LESS, EQUAL, GREATER}
    [[nodiscard]] spaceship compare(const uint2022_t& rhs) const;

    /// @brief Comparison operators.
    /// @param rhs uint2022_t instance to compare with.
    /// @return bool as of a result of uint2022_t::compare function.
    /// @note Will cast base type to uint2022_t if possible.

    bool operator==(const uint2022_t& rhs) const;
    template <typename T>
    bool operator==(const T& rhs) const {
        return operator==(uint2022_t(rhs));
    }

    bool operator!=(const uint2022_t& rhs) const;
    template <typename T>
    bool operator!=(const T& rhs) const {
        return operator!=(uint2022_t(rhs));
    }

    bool operator>(const uint2022_t& rhs) const;
    template <typename T>
    bool operator>(const T& rhs) const {
        return operator>(uint2022_t(rhs));
    }

    bool operator>=(const uint2022_t& rhs) const;
    template <typename T>
    bool operator>=(const T& rhs) const {
        return operator>=(uint2022_t(rhs));
    }

    bool operator<(const uint2022_t& rhs) const;
    template <typename T>
    bool operator<(const T& rhs) const {
        return operator<(uint2022_t(rhs));
    }

    bool operator<=(const uint2022_t& rhs) const;
    template <typename T>
    bool operator<=(const T& rhs) const {
        return operator<=(uint2022_t(rhs));
    }

   protected:
    // Arithmetic calculations helper functions.

    /// @brief Fast-Fourier-Transform algorithm for fast multiplication with complexity of O(n log n).
    /// @param data polynom of complex numbers to transform (mutable).
    /// @param invert "direction" of transformation.
    void fft(std::vector<std::complex<long double>>& data,
             const bool& invert = false) const;

    /// @brief Short division with module works faster then the big one down below.
    /// @param lhs lvalue is a divident.
    /// @param rhs rvalue is a divisor (using onlt first digit of it).
    /// @return Pair of (quotient and module) result of division.
    [[nodiscard]] std::pair<uint2022_t, uint2022_t> short_dm(const uint2022_t& lhs,
                                                             const uint2022_t& rhs) const;

    /// @brief Division with module using exponential search with complexity of O(n log^3 n).
    /// @param lhs lvalue is a divident.
    /// @param rhs rvalue is a divisor.
    /// @return Pair of (quotient and module) result of division.
    [[nodiscard]] std::pair<uint2022_t, uint2022_t> dm(const uint2022_t& lhs,
                                                       const uint2022_t& rhs) const;

   public:
    /// @brief Binary arithmetic operators.
    /// @param rhs uint2022_t instance to calc with.
    /// @return uint2022_t instance containing result of operation.
    /// @note Will cast base type to uint2022_t if possible.

    uint2022_t operator+(const uint2022_t& rhs) const;
    template <typename T>
    uint2022_t operator+(const T& rhs) const {
        return *this + uint2022_t(rhs);
    }

    uint2022_t operator-(const uint2022_t& rhs) const;
    template <typename T>
    uint2022_t operator-(const T& rhs) const {
        return *this - uint2022_t(rhs);
    }

    uint2022_t operator*(const uint2022_t& rhs) const;
    template <typename T>
    uint2022_t operator*(const T& rhs) const {
        return *this * uint2022_t(rhs);
    }

    uint2022_t operator/(const uint2022_t& rhs) const;
    template <typename T>
    uint2022_t operator/(const T& rhs) const {
        return *this / uint2022_t(rhs);
    }

    uint2022_t operator%(const uint2022_t& rhs) const;
    template <typename T>
    uint2022_t operator%(const T& rhs) const {
        return *this % uint2022_t(rhs);
    }

    /// @brief Unary arithmetic operators.
    /// @param rhs uint2022_t instance to calc with.
    /// @return Reference to this containing result of operation.
    /// @note Will cast base type to uint2022_t if possible.

    uint2022_t& operator+=(const uint2022_t& rhs);
    template <typename T>
    uint2022_t& operator+=(const T& rhs) {
        return *this += uint2022_t(rhs);
    }

    uint2022_t& operator-=(const uint2022_t& rhs);
    template <typename T>
    uint2022_t& operator-=(const T& rhs) {
        return *this -= uint2022_t(rhs);
    }

    uint2022_t& operator*=(const uint2022_t& rhs);
    template <typename T>
    uint2022_t& operator*=(const T& rhs) {
        return *this *= uint2022_t(rhs);
    }

    uint2022_t& operator/=(const uint2022_t& rhs);
    template <typename T>
    uint2022_t& operator/=(const T& rhs) {
        return *this /= uint2022_t(rhs);
    }

    uint2022_t& operator%=(const uint2022_t& rhs);
    template <typename T>
    uint2022_t& operator%=(const T& rhs) {
        return *this %= uint2022_t(rhs);
    }

    /// @brief Binary bitwise operators.
    /// @param rhs uint2022_t instance to calc with.
    /// @return uint2022_t instance containing result of operation.
    /// @note Will cast base type to uint2022_t if possible.
    /// @note Bit-shifts is not implemented.

    uint2022_t operator&(const uint2022_t& rhs) const;
    template <typename T>
    uint2022_t operator&(const T& rhs) const {
        return *this & uint2022_t(rhs);
    }

    uint2022_t operator|(const uint2022_t& rhs) const;
    template <typename T>
    uint2022_t operator|(const T& rhs) const {
        return *this | uint2022_t(rhs);
    }

    uint2022_t operator^(const uint2022_t& rhs) const;
    template <typename T>
    uint2022_t operator^(const T& rhs) const {
        return *this ^ uint2022_t(rhs);
    }

    /// @brief Unary bitwise operators.
    /// @param rhs uint2022_t instance to calc with.
    /// @return Reference to this containing result of operation.
    /// @note Will cast base type to uint2022_t if possible.
    /// @note Bit-shifts is not implemented.

    uint2022_t& operator&=(const uint2022_t& rhs);
    template <typename T>
    uint2022_t& operator&=(const T& rhs) {
        return *this &= uint2022_t(rhs);
    }

    uint2022_t& operator|=(const uint2022_t& rhs);
    template <typename T>
    uint2022_t& operator|=(const T& rhs) {
        return *this |= uint2022_t(rhs);
    }

    uint2022_t& operator^=(const uint2022_t& rhs);
    template <typename T>
    uint2022_t& operator^=(const T& rhs) {
        return *this ^= uint2022_t(rhs);
    }

    /// @brief Prefix increment/decrement operators.
    /// @return Reference to this after + 1.

    uint2022_t& operator++();
    uint2022_t& operator--();

    /// @brief Postfix increment/decrement operators.
    /// @return Copy of passed uint2022_t instance.

    uint2022_t operator++(int);
    uint2022_t operator--(int);

    /// @brief Convert value of this to string.
    /// @return String containing uint2022_t value in radix of 10.
    [[nodiscard]] std::string to_string() const;
};

static_assert(sizeof(uint2022_t) <= 300,
              "Size of uint2022_t must be no higher than 300 bytes");

/// @brief Comparison operators (lhs is not uint2022_t).
/// @typedef T
/// @param rhs uint2022_t instance to compare with.
/// @return bool as of a result of uint2022_t::compare function.
/// @note Will cast base type to uint2022_t if possible.

template <typename T>
bool operator==(const T& lhs, const uint2022_t& rhs) {
    return uint2022_t(lhs) == rhs;
}

template <typename T>
bool operator!=(const T& lhs, const uint2022_t& rhs) {
    return uint2022_t(lhs) != rhs;
}

template <typename T>
bool operator>(const T& lhs, const uint2022_t& rhs) {
    return uint2022_t(lhs) > rhs;
}

template <typename T>
bool operator>=(const T& lhs, const uint2022_t& rhs) {
    return uint2022_t(lhs) >= rhs;
}

template <typename T>
bool operator<(const T& lhs, const uint2022_t& rhs) {
    return uint2022_t(lhs) == rhs;
}

template <typename T>
bool operator<=(const T& lhs, const uint2022_t& rhs) {
    return uint2022_t(lhs) == rhs;
}

/// @brief Binary arithmetic operators (lhs is T).
/// @typedef T
/// @param rhs uint2022_t instance to calc with.
/// @return uint2022_t instance containing result of operation.
/// @note Will cast base type to uint2022_t if possible.

template <typename T>
uint2022_t operator+(const T& lhs, const uint2022_t& rhs) {
    return uint2022_t(lhs) + rhs;
}

template <typename T>
uint2022_t operator-(const T& lhs, const uint2022_t& rhs) {
    return uint2022_t(lhs) - rhs;
}

template <typename T>
uint2022_t operator*(const T& lhs, const uint2022_t& rhs) {
    return uint2022_t(lhs) * rhs;
}

template <typename T>
uint2022_t operator/(const T& lhs, const uint2022_t& rhs) {
    return uint2022_t(lhs) / rhs;
}

template <typename T>
uint2022_t operator%(const T& lhs, const uint2022_t& rhs) {
    return uint2022_t(lhs) % rhs;
}

/// @brief Unary arithmetic operators (lhs is T).
/// @typedef T
/// @param rhs uint2022_t instance to calc with.
/// @return T-typed value containing result of operation.
/// @note Will cast base type to uint2022_t if possible.

template <typename T>
T operator+=(T& lhs, const uint2022_t& rhs) {
    return lhs = static_cast<T>(uint2022_t(lhs) + rhs);
}

template <typename T>
T operator-=(T& lhs, const uint2022_t& rhs) {
    return lhs = static_cast<T>(uint2022_t(lhs) - rhs);
}

template <typename T>
T operator*=(T& lhs, const uint2022_t& rhs) {
    return lhs = static_cast<T>(uint2022_t(lhs) * rhs);
}

template <typename T>
T operator/=(T& lhs, const uint2022_t& rhs) {
    return lhs = static_cast<T>(uint2022_t(lhs) / rhs);
}

template <typename T>
T operator%=(T& lhs, const uint2022_t& rhs) {
    return lhs = static_cast<T>(uint2022_t(lhs) % rhs);
}

/// @brief Binary bitwise operators (lhs is T).
/// @typedef T
/// @param rhs uint2022_t instance to calc with.
/// @return uint2022_t instance containing result of operation.
/// @note Will cast base type to uint2022_t if possible.
/// @note Bit-shifts is not implemented.

template <typename T>
uint2022_t operator&(const T& lhs, const uint2022_t& rhs) {
    return uint2022_t(lhs) & rhs;
}

template <typename T>
uint2022_t operator|(const T& lhs, const uint2022_t& rhs) {
    return uint2022_t(lhs) | rhs;
}

template <typename T>
uint2022_t operator^(const T& lhs, const uint2022_t& rhs) {
    return uint2022_t(lhs) ^ rhs;
}

/// @brief Unary bitwise operators (lhs is T).
/// @typedef T
/// @param rhs uint2022_t instance to calc with.
/// @return T-typed value containing result of operation.
/// @note Will cast base type to uint2022_t if possible.
/// @note Bit-shifts is not implemented.

template <typename T>
T operator&=(T& lhs, const uint2022_t& rhs) {
    return lhs = static_cast<T>(uint2022_t(lhs) & rhs);
}

template <typename T>
T operator|=(T& lhs, const uint2022_t& rhs) {
    return lhs = static_cast<T>(uint2022_t(lhs) | rhs);
}

template <typename T>
T operator^=(T& lhs, const uint2022_t& rhs) {
    return lhs = static_cast<T>(uint2022_t(lhs) ^ rhs);
}

/// @brief I/O operators using std::string as buffer.

std::ostream& operator<<(std::ostream& out, const uint2022_t& rhs);
std::istream& operator>>(std::istream& in, uint2022_t& rhs);
