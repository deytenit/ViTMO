#include "encoders.h"

namespace {
void trimStr(std::string & str)
{
    while (!str.empty() && str.front() == ' ') {
        str.erase(str.begin());
    }

    while (!str.empty() && str.back() == ' ') {
        str.pop_back();
    }
}

bool isBigEndianSystem()
{
    int i = 1;
    char * p = reinterpret_cast<char *>(&i);

    return p[0] == 0;
}

template <class T>
void endianSwap(T * obj_ptr)
{
    if (isBigEndianSystem()) {
        return;
    }

    char * mem_ptr = reinterpret_cast<char *>(obj_ptr);
    std::reverse(mem_ptr, mem_ptr + sizeof(T));
}
} // namespace

void decode_token(std::string & dest, const char * src)
{
    dest.assign(src);
    trimStr(dest);
}

void decode_token(std::string & dest, const char * src, const size_t len)
{
    dest.assign(src, len);
}

void decode_integer(unsigned & dest, const char * src)
{
    dest = *reinterpret_cast<const unsigned *>(src);
    endianSwap(&dest);
}

void decode_price(double & dest, const char * src)
{
    int price = *reinterpret_cast<const int *>(src);
    endianSwap(&price);

    dest = price / PRICE_PRECISION;
}

void decode_liquidity(LiquidityIndicator & dest, const char * src)
{
    const char liquidity_attributes = *src;

    switch ((liquidity_attributes >> 4) & 0b11) {
    case LIQUIDITY_ADDED: {
        dest = LiquidityIndicator::Added;
        break;
    }
    case LIQUIDITY_REMOVED: {
        dest = LiquidityIndicator::Removed;
        break;
    }
    default: {
        dest = LiquidityIndicator::None;
    }
    }
}

void decode_reason(BreakReason & dest, const char * src)
{
    const char reason = *src;
    switch (reason) {
    case REASON_ERRONEOUS: {
        dest = BreakReason::Erroneous;
        break;
    }
    case REASON_CONSENT: {
        dest = BreakReason::Consent;
        break;
    }
    case REASON_SUPERVISORY: {
        dest = BreakReason::Supervisory;
        break;
    }
    case REASON_EXTERNAL: {
        dest = BreakReason::External;
        break;
    }
    default: {
        dest = BreakReason::Unknown;
    }
    }
}

void decode_mmt(char * dest, const char * src, const uint8_t offset)
{
    dest[offset] = *src;
}

void decode_bit(bool & dest, const char * src, const uint8_t offset)
{
    dest = static_cast<bool>((*src >> offset) & 1);
}