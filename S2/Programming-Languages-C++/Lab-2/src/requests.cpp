#include "encoders.h"

ExecutionDetails decode_executed_order(const std::vector<unsigned char> & message)
{
    ExecutionDetails exec_details;

    const char * msg_ptr = reinterpret_cast<const char *>(message.data());

#define FIELD(name, protocol, offset) decode_##protocol(exec_details.name, msg_ptr + offset);
#define ALPHA(name, protocol, offset, inner) decode_##protocol(exec_details.name, msg_ptr + offset, inner);
#include "executed_order.inl"

    return exec_details;
}

BrokenTradeDetails decode_broken_trade(const std::vector<unsigned char> & message)
{
    BrokenTradeDetails break_details;

    const char * msg_ptr = reinterpret_cast<const char *>(message.data());

#define FIELD(name, protocol, offset) decode_##protocol(break_details.name, msg_ptr + offset);
#define ALPHA(name, protocol, offset, bit) decode_##protocol(break_details.name, msg_ptr + offset, bit);
#include "broken_order.inl"

    return break_details;
}
