#include "requests.h"

#include <gtest/gtest.h>
#include <string>

TEST(ExecutedOrderTest, decode_bofa_executed_order)
{
    std::vector<unsigned char> message = { 'E', 0x00, 0x00, 0x1a, 0x31, 0x85, 0xc5, 0x00, 0x00 };
    // '12345678912345'
    std::vector<unsigned char> orderId = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '1', '2', '3', '4', '5'};
    message.insert(message.end(), orderId.begin(), orderId.end());
    // 100000
    std::vector<unsigned char> filled_volume = { 0x00, 0x01, 0x86, 0xa0 };
    message.insert(message.end(), filled_volume.begin(), filled_volume.end());
    // 14.746
    std::vector<unsigned char> fill_price = { 0x00, 0x02, 0x40, 0x04 };
    message.insert(message.end(), fill_price.begin(), fill_price.end());
    // Continuous market trade
    message.push_back('A');
    // 4831
    std::vector<unsigned char> match_number = { 0x00, 0x00, 0x12, 0xdf };
    message.insert(message.end(), match_number.begin(), match_number.end());
    // BofA
    std::vector<unsigned char> counter_part = { 'B', 'o', 'f', 'A' };
    message.insert(message.end(), counter_part.begin(), counter_part.end());
    // Continuous trading
    message.push_back('2');
    // None apply
    message.push_back('-');
    // Algorithmic trade
    message.push_back('H');
    // Removed, Internalized, Not-Self traded
    message.push_back(0b01101000);
    ASSERT_EQ(44, message.size());

    ExecutionDetails details = decode_executed_order(message);

    EXPECT_EQ(std::string("12345678912345"), details.cl_ord_id);
    EXPECT_EQ(4831, details.match_number);
    EXPECT_EQ(100000, details.filled_volume);
    EXPECT_DOUBLE_EQ(14.746, details.price);
    EXPECT_EQ(std::string("BofA"), details.counterpart);
    EXPECT_EQ(LiquidityIndicator::Removed, details.liquidity_indicator);
    EXPECT_TRUE(details.internalized);
    EXPECT_FALSE(details.self_trade);
    EXPECT_EQ("12-------PH---", std::string{details.mmt});
}

TEST(ExecutedOrderTest, decode_sgen_executed_order)
{
    std::vector<unsigned char> message = { 'E', 0x00, 0x00, 0x2B, 0xDF, 0xC0, 0x10, 0x60, 0x00 };
    // '00042000000042'
    std::vector<unsigned char> orderId = { '0', '0', '0', '4', '2', '0', '0', '0', '0', '0', '0', '0', '4', '2'};
    message.insert(message.end(), orderId.begin(), orderId.end());
    // 450000
    std::vector<unsigned char> filled_volume = { 0x00, 0x06, 0xdd, 0xd0 };
    message.insert(message.end(), filled_volume.begin(), filled_volume.end());
    // 1909.2754
    std::vector<unsigned char> fill_price = { 0x01, 0x23, 0x55, 0x12 };
    message.insert(message.end(), fill_price.begin(), fill_price.end());
    // Continuous market trade
    message.push_back('A');
    // 74847
    std::vector<unsigned char> match_number = { 0x00, 0x01, 0x24, 0x5f };
    message.insert(message.end(), match_number.begin(), match_number.end());
    // SGen
    std::vector<unsigned char> counter_part = { 'S', 'G', 'e', 'n' };
    message.insert(message.end(), counter_part.begin(), counter_part.end());
    // Scheduled intraday auction
    message.push_back('I');
    // None apply
    message.push_back('-');
    // Manual trade
    message.push_back('-');
    // Added, Non Internalized, Self-traded
    message.push_back(0b11000000);
    ASSERT_EQ(44, message.size());

    ExecutionDetails details = decode_executed_order(message);

    EXPECT_EQ(std::string("00042000000042"), details.cl_ord_id);
    EXPECT_EQ(74847, details.match_number);
    EXPECT_EQ(450000, details.filled_volume);
    EXPECT_DOUBLE_EQ(1909.2754, details.price);
    EXPECT_EQ(std::string("SGen"), details.counterpart);
    EXPECT_EQ(LiquidityIndicator::Added, details.liquidity_indicator);
    EXPECT_FALSE(details.internalized);
    EXPECT_TRUE(details.self_trade);
    EXPECT_EQ("1I-------P----", std::string{details.mmt});
}

TEST(ExecutedOrderTest, decode_short_token_executed_order)
{
    std::vector<unsigned char> message = { 'E', 0x00, 0x00, 0x33, 0x62, 0xb2, 0xd3, 0x3e, 0x00 };
    // '1234'
    std::vector<unsigned char> orderId = { '1', '2', '3', '4', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
    message.insert(message.end(), orderId.begin(), orderId.end());
    // 100000
    std::vector<unsigned char> filled_volume = { 0x00, 0x01, 0x86, 0xa0 };
    message.insert(message.end(), filled_volume.begin(), filled_volume.end());
    // 14.746
    std::vector<unsigned char> fill_price = { 0x00, 0x02, 0x40, 0x04 };
    message.insert(message.end(), fill_price.begin(), fill_price.end());
    // Continuous market trade
    message.push_back('A');
    // 268559727
    std::vector<unsigned char> match_number = { 0x10, 0x01, 0xe5, 0x6f };
    message.insert(message.end(), match_number.begin(), match_number.end());
    // BofA
    std::vector<unsigned char> counter_part = { 'B', 'o', 'f', 'A' };
    message.insert(message.end(), counter_part.begin(), counter_part.end());
    // Continuous trading
    message.push_back('2');
    // None apply
    message.push_back('-');
    // Algorithmic trade
    message.push_back('H');
    // None, Internalized, Self traded
    message.push_back(0b10110000);
    ASSERT_EQ(44, message.size());

    ExecutionDetails details = decode_executed_order(message);

    EXPECT_EQ(std::string("1234"), details.cl_ord_id);
    EXPECT_EQ(268559727, details.match_number);
    EXPECT_EQ(100000, details.filled_volume);
    EXPECT_DOUBLE_EQ(14.746, details.price);
    EXPECT_EQ(std::string("BofA"), details.counterpart);
    EXPECT_EQ(LiquidityIndicator::None, details.liquidity_indicator);
    EXPECT_TRUE(details.internalized);
    EXPECT_TRUE(details.self_trade);
    EXPECT_EQ("12-------PH---", std::string{details.mmt});
}

TEST(ExecutedOrderTest, decode_thin_price_executed_order)
{
    std::vector<unsigned char> message = { 'E', 0x00, 0x00, 0x2B, 0xDF, 0xC0, 0x10, 0x60, 0x00 };
    // '00042000000042'
    std::vector<unsigned char> orderId = { '0', '0', '0', '4', '2', '0', '0', '0', '0', '0', '0', '0', '4', '2'};
    message.insert(message.end(), orderId.begin(), orderId.end());
    // 450000
    std::vector<unsigned char> filled_volume = { 0x00, 0x06, 0xdd, 0xd0 };
    message.insert(message.end(), filled_volume.begin(), filled_volume.end());
    // 1.0001
    std::vector<unsigned char> fill_price = { 0x00, 0x00, 0x27, 0x11 };
    message.insert(message.end(), fill_price.begin(), fill_price.end());
    // Continuous market trade
    message.push_back('A');
    // 74847
    std::vector<unsigned char> match_number = { 0x00, 0x01, 0x24, 0x5f };
    message.insert(message.end(), match_number.begin(), match_number.end());
    // SGen
    std::vector<unsigned char> counter_part = { 'S', 'G', 'e', 'n' };
    message.insert(message.end(), counter_part.begin(), counter_part.end());
    // Unscheduled auction
    message.push_back('U');
    // None apply
    message.push_back('-');
    // Manual trade
    message.push_back('-');
    // Added, Non Internalized, Self-traded
    message.push_back(0b11000000);
    ASSERT_EQ(44, message.size());

    ExecutionDetails details = decode_executed_order(message);

    EXPECT_EQ(std::string("00042000000042"), details.cl_ord_id);
    EXPECT_EQ(74847, details.match_number);
    EXPECT_EQ(450000, details.filled_volume);
    EXPECT_DOUBLE_EQ(1.0001, details.price);
    EXPECT_EQ(std::string("SGen"), details.counterpart);
    EXPECT_EQ(LiquidityIndicator::Added, details.liquidity_indicator);
    EXPECT_FALSE(details.internalized);
    EXPECT_TRUE(details.self_trade);
    EXPECT_EQ("1U-------P----", std::string{details.mmt});
}

TEST(BrokenTradeTest, short_order_token)
{
    std::vector<unsigned char> message = {
        'B',
        0x00, 0x00, 0x1A, 0x31, 0x85, 0xC5, 0x00, 0x00, // Timestamp
        '1', '2', '3', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', // Order Token
        0x00, 0x9B, 0xA3, 0x02, // Match number
        'C', // Reason
        'O', // Trading mode
        'D', // Transaction category
        '-', // Algorithm indicator
    };

    BrokenTradeDetails details = decode_broken_trade(message);

    EXPECT_EQ(std::string{"123"}, details.cl_ord_id);
    EXPECT_EQ(10199810, details.match_number);
    EXPECT_EQ(BreakReason::Consent, details.reason);
    EXPECT_EQ("1OD------P----", std::string{details.mmt});
}

TEST(BrokenTradeTest, external_reason)
{
    std::vector<unsigned char> message = {
        'B',
        0x00, 0x00, 0x1A, 0x31, 0x85, 0xC5, 0x00, 0x00, // Timestamp
        '1', '2', '3', '8', '9', '5', '5', '5', '3', '1', '7', '9', '6', '6', // Order Token
        0x00, 0x9B, 0xA3, 0x02, // Match number
        'X', // Reason
        '2', // Trading mode
        'D', // Transaction category
        'H', // Algorithm indicator
    };

    BrokenTradeDetails details = decode_broken_trade(message);

    EXPECT_EQ(std::string{"12389555317966"}, details.cl_ord_id);
    EXPECT_EQ(10199810, details.match_number);
    EXPECT_EQ(BreakReason::External, details.reason);
    EXPECT_EQ("12D------PH---", std::string{details.mmt});
}
