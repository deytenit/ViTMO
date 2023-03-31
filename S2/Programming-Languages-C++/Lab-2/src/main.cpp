#include "requests.h"

#include <iostream>

namespace {

std::ostream & operator<<(std::ostream & strm, const LiquidityIndicator li)
{
    switch (li) {
    case LiquidityIndicator::None: return strm << "None";
    case LiquidityIndicator::Added: return strm << "Added";
    case LiquidityIndicator::Removed: return strm << "Removed";
    }
    return strm;
}

void print_details(const ExecutionDetails & details)
{
    std::cout
            << "ClientOrderID: " << details.cl_ord_id << "\n"
            << "MatchNumber: " << details.match_number << "\n"
            << "FilledVolume: " << details.filled_volume << "\n"
            << "Price: " << details.price << "\n"
            << "Counterpart: " << details.counterpart << "\n"
            << "LiquidityIndicator: " << details.liquidity_indicator << "\n"
            << "Internalized: " << std::boolalpha << details.internalized << "\n"
            << "Self-trade: " << std::boolalpha << details.self_trade << std::endl;
}

} // anonymous namespace

int main()
{
    std::vector<unsigned char> message = {
            'E', 0x00, 0x00, 0x1a, 0x31, 0x85, 0xc5, 0x00, 0x00, '1', '2', '3', '4', '5', '6', '7', '8', '9', '1', '2', '3', '4', '5', 0x00, 0x01, 0x86, 0xa0, 0x00, 0x02, 0x40, 0x04, 'A', 0x00, 0x00, 0x12, 0xdf, 'B', 'o', 'f', 'A', '2', '-', 'H', 0b01101000};
    print_details(decode_executed_order(message));
}
