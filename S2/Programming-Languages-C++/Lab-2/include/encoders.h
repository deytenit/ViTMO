#pragma once

#include "requests.h"

#include <string>

constexpr double PRICE_PRECISION = 10000.0;
constexpr char LIQUIDITY_ADDED = 0b00;
constexpr char LIQUIDITY_REMOVED = 0b10;

constexpr char REASON_ERRONEOUS = 'E';
constexpr char REASON_CONSENT = 'C';
constexpr char REASON_SUPERVISORY = 'S';
constexpr char REASON_EXTERNAL = 'X';

void decode_token(std::string & dest, const char * src);

void decode_token(std::string & dest, const char * src, const size_t len);

void decode_integer(unsigned & dest, const char * src);

void decode_price(double & dest, const char * src);

void decode_liquidity(LiquidityIndicator & dest, const char * src);

void decode_reason(BreakReason & dest, const char * src);

void decode_mmt(char * dest, const char * src, const uint8_t offset);

void decode_bit(bool & dest, const char * src, const uint8_t offset);