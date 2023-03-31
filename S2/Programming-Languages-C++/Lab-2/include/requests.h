#pragma once

#include <algorithm>
#include <string>
#include <vector>

#define MMT_DEFAULT "12-------P----"

enum class LiquidityIndicator
{
    None,
    Added,
    Removed
};

struct ExecutionDetails
{
    std::string cl_ord_id;
    unsigned match_number{};
    unsigned filled_volume{};
    double price{};
    std::string counterpart;
    char mmt[15] = MMT_DEFAULT;
    LiquidityIndicator liquidity_indicator = LiquidityIndicator::None;
    bool internalized{};
    bool self_trade{};
};

ExecutionDetails decode_executed_order(const std::vector<unsigned char> & message);

enum class BreakReason
{
    Erroneous,
    Consent,
    Supervisory,
    External,
    Unknown
};

struct BrokenTradeDetails
{
    std::string cl_ord_id;
    unsigned match_number{};
    char mmt[15] = MMT_DEFAULT;
    BreakReason reason = BreakReason::Unknown;
};

BrokenTradeDetails decode_broken_trade(const std::vector<unsigned char> & message);
