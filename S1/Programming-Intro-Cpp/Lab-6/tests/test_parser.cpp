#include <omfl_parser.h>

#include <gtest/gtest.h>

using namespace omfl;

TEST(ParserTestSuite, EmptyTest) {
    const std::string data;

    const auto root = Parse(data);

    ASSERT_TRUE(root.Valid());
}

TEST(ParserTestSuite, IntTest) {
    const std::string data = R"(
        key1 = 100500
        key2 = -22
        key3 = +28)";


    const auto root = Parse(data);

    ASSERT_TRUE(root.Valid());

    ASSERT_TRUE(root.Get("key1").Is<int>());
    ASSERT_TRUE(root.Get("key2").Is<int>());
    ASSERT_TRUE(root.Get("key3").Is<int>());

    ASSERT_EQ(root.Get("key1").As<int>(), 100500);
    ASSERT_EQ(root.Get("key2").As<int>(), -22);
    ASSERT_EQ(root.Get("key3").As<int>(), 28);
}

TEST(ParserTestSuite, InvalidIntTest) {
    const std::string data = R"(
        key1 = true
        key2 = -22.1
        key3 = "ITMO")";


    const auto root = Parse(data);

    ASSERT_TRUE(root.Valid());

    ASSERT_FALSE(root.Get("key1").Is<int>());
    ASSERT_FALSE(root.Get("key2").Is<int>());
    ASSERT_FALSE(root.Get("key3").Is<int>());

    ASSERT_EQ(root.Get("key1").AsOrDefault<int>(1), 1);
    ASSERT_EQ(root.Get("key2").AsOrDefault<int>(-2), -2);
    ASSERT_EQ(root.Get("key3").AsOrDefault<int>(3), 3);
}

TEST(ParserTestSuite, FloatTest) {
    const std::string data = R"(
        key1 = 2.1
        key2 = -3.14
        key3 = -0.001)";


    const auto root = Parse(data);

    ASSERT_TRUE(root.Valid());

    ASSERT_TRUE(root.Get("key1").Is<float>());
    ASSERT_TRUE(root.Get("key2").Is<float>());
    ASSERT_TRUE(root.Get("key3").Is<float>());

    ASSERT_FLOAT_EQ(root.Get("key1").As<float>(), 2.1f);
    ASSERT_FLOAT_EQ(root.Get("key2").As<float>(), -3.14f);
    ASSERT_FLOAT_EQ(root.Get("key3").As<float>(), -0.001f);
}

TEST(ParserTestSuite, InvalidFloatTest) {
    const std::string data = R"(
        key1 = true
        key2 = -2
        key3 = "ITMO")";


    const auto root = Parse(data);

    ASSERT_TRUE(root.Valid());

    ASSERT_FALSE(root.Get("key1").Is<float>());
    ASSERT_FALSE(root.Get("key2").Is<float>());
    ASSERT_FALSE(root.Get("key3").Is<float>());

    ASSERT_FLOAT_EQ(root.Get("key1").AsOrDefault<float>(2.1), 2.1);
    ASSERT_FLOAT_EQ(root.Get("key2").AsOrDefault<float>(-3.14), -3.14);
    ASSERT_FLOAT_EQ(root.Get("key3").AsOrDefault<float>(-0.001), -0.001);
}

TEST(ParserTestSuite, StringTest) {
    const std::string data = R"(
        key = "value"
        key1 = "value1")";


    const auto root = Parse(data);

    ASSERT_TRUE(root.Valid());

    ASSERT_TRUE(root.Get("key").Is<std::string>());
    ASSERT_TRUE(root.Get("key1").Is<std::string>());

    ASSERT_EQ(root.Get("key").As<std::string>(), "value");
    ASSERT_EQ(root.Get("key1").As<std::string>(), "value1");
}

TEST(ParserTestSuite, InvalidStringTest) {
    const std::string data = R"(
        key = true
        key1 = ["1", "2", "3"])";


    const auto root = Parse(data);

    ASSERT_TRUE(root.Valid());

    ASSERT_FALSE(root.Get("key").Is<std::string>());
    ASSERT_FALSE(root.Get("key1").Is<std::string>());

    ASSERT_EQ(root.Get("key").AsOrDefault<std::string>("Hello"), "Hello");
    ASSERT_EQ(root.Get("key1").AsOrDefault<std::string>("World"), "World");
}

TEST(ParserTestSuite, ArrayTest) {
    const std::string data = R"(
        key1 = [1, 2, 3, 4, 5, 6])";


    const auto root = Parse(data);

    ASSERT_TRUE(root.Valid());

    ASSERT_TRUE(root.Get("key1").Is<omfl::Array>());

    ASSERT_EQ(root.Get("key1")[0].As<int>(), 1);
    ASSERT_EQ(root.Get("key1")[1].As<int>(), 2);
    ASSERT_EQ(root.Get("key1")[2].As<int>(), 3);

    ASSERT_EQ(root.Get("key1")[100500].AsOrDefault<int>(99), 99);
}

TEST(ParserTestSuite, DiffTypesArrayTest) {
    std::string data = R"(
        key1 = [1, true, 3.14, "ITMO", [1, 2, 3], ["a", "b", 28]])";

    const auto root = Parse(data);

    ASSERT_TRUE(root.Valid());

    ASSERT_TRUE(root.Get("key1").Is<omfl::Array>());

    ASSERT_TRUE(root.Get("key1")[0].Is<int>());
    ASSERT_EQ(root.Get("key1")[0].As<int>(), 1);

    ASSERT_TRUE(root.Get("key1")[1].Is<bool>());
    ASSERT_EQ(root.Get("key1")[1].As<bool>(), true);

    ASSERT_TRUE(root.Get("key1")[2].Is<float>());
    ASSERT_FLOAT_EQ(root.Get("key1")[2].As<float>(), 3.14);

    ASSERT_TRUE(root.Get("key1")[3].Is<std::string>());
    ASSERT_EQ(root.Get("key1")[3].As<std::string>(), "ITMO");

    ASSERT_TRUE(root.Get("key1")[4].Is<omfl::Array>());
    ASSERT_EQ(root.Get("key1")[4][0].As<int>(), 1);
    ASSERT_EQ(root.Get("key1")[4][1].As<int>(), 2);
    ASSERT_EQ(root.Get("key1")[4][2].As<int>(), 3);

    ASSERT_TRUE(root.Get("key1")[5].Is<omfl::Array>());
    ASSERT_EQ(root.Get("key1")[5][0].As<std::string>(), "a");
    ASSERT_EQ(root.Get("key1")[5][1].As<std::string>(), "b");
    ASSERT_EQ(root.Get("key1")[5][2].As<int>(), 28);
}

TEST(ParserTestSuite, CommentsTest) {
    const std::string data = R"(
        key1 = 100500  # some important value

        # It's more then university)";

    const auto root = Parse(data);

    ASSERT_TRUE(root.Valid());
    ASSERT_EQ(root.Get("key1").As<int>(), 100500);
}

TEST(ParserTestSuite, SectionTest) {
    const std::string data = R"(
        [section1]
        key1 = 1
        key2 = true

        [section1]
        key3 = "value")";

    const auto root = Parse(data);
    ASSERT_TRUE(root.Valid());

    ASSERT_EQ(root.Get("section1.key1").As<int>(), 1);
    ASSERT_EQ(root.Get("section1.key2").As<bool>(), true);
    ASSERT_EQ(root.Get("section1.key3").As<std::string>(), "value");
}

TEST(ParserTestSuite, MultiSectionTest) {
    const std::string data = R"(
        [level1]
        key1 = 1
        [level1.level2-1]
        key2 = 2

        [level1.level2-2]
        key3 = 3)";

    const auto root = Parse(data);
    ASSERT_TRUE(root.Valid());

    ASSERT_EQ(root.Get("level1.key1").As<int>(), 1);
    ASSERT_EQ(root.Get("level1.level2-1.key2").As<int>(), 2);
    ASSERT_EQ(root.Get("level1.level2-2.key3").As<int>(), 3);
}


TEST(ParserTestSuite, GetSectionTest) {
    const std::string data = R"(
        [level1.level2.level3]
        key1 = 1)";

    const auto root = Parse(data);
    ASSERT_TRUE(root.Valid());

    ASSERT_EQ(root.Get("level1").Get("level2").Get("level3").Get("key1").As<int>(), 1);
}
