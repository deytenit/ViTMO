#include <gtest/gtest.h>
#include <parser.h>

#include <sstream>

using namespace argument_parser;

std::vector<std::string> SplitString(const std::string& str) {
    std::istringstream iss(str);

    return {
        std::istream_iterator<std::string>(iss),
        std::istream_iterator<std::string>()
    };
}

TEST(ParserTestSuite, EmptyTest) {
    const Parser parser("My Empty Parser");

    parser.parse(SplitString("app"));
}

TEST(ParserTestSuite, StringTest) {
    Parser parser("My Parser");
    parser.add_string_option("param1");

    parser.parse(SplitString("app --param1=value1"));
    ASSERT_EQ(parser.get_string_option("param1"), "value1");
}

TEST(ParserTestSuite, ShortNameTest) {
    Parser parser("My Parser");
    parser.add_string_option('p', "param1");

    parser.parse(SplitString("app -p=value1"));
    ASSERT_EQ(parser.get_string_option("param1"), "value1");
}

TEST(ParserTestSuite, DefaultTest) {
    Parser parser("My Parser");
    parser.add_string_option("param1").set_default("value1");

    parser.parse(SplitString("app"));
    ASSERT_EQ(parser.get_string_option("param1"), "value1");
}

TEST(ParserTestSuite, NoDefaultTest) {
    Parser parser("My Parser");
    parser.add_string_option("param1");

    EXPECT_ANY_THROW(parser.parse(SplitString("app")));
}

TEST(ParserTestSuite, StoreValueTest) {
    Parser parser("My Parser");
    std::string value;
    parser.add_string_option("param1").store(value);

    parser.parse(SplitString("app --param1=value1"));
    ASSERT_EQ(value, "value1");
}

TEST(ParserTestSuite, MultiStringTest) {
    Parser parser("My Parser");
    std::string value;
    parser.add_string_option("param1").store(value);
    parser.add_string_option('a', "param2");

    parser.parse(SplitString("app --param1=value1 --param2=value2"));
    ASSERT_EQ(parser.get_string_option("param2"), "value2");
}

TEST(ParserTestSuite, IntTest) {
    Parser parser("My Parser");
    parser.add_int_option("param1");

    parser.parse(SplitString("app --param1=100500"));
    ASSERT_EQ(parser.get_int_option("param1"), 100500);
}

TEST(ParserTestSuite, MultiValueTest) {
    Parser parser("My Parser");
    std::vector<int> int_values;
    parser.add_int_option('p', "param1").multiple().store(int_values);

    parser.parse(SplitString("app --param1=1 --param1=2 --param1=3"));
    ASSERT_EQ(parser.get_int_option("param1", 0), 1);
    ASSERT_EQ(int_values[1], 2);
    ASSERT_EQ(int_values[2], 3);
}

TEST(ParserTestSuite, MinCountMultiValueTest) {
    Parser parser("My Parser");
    std::vector<int> int_values;
    constexpr size_t min_args_count = 10;
    parser.add_int_option('p', "param1")
          .multiple(min_args_count)
          .store(int_values);

    EXPECT_ANY_THROW(parser.parse(SplitString("app --param1=1 --param1=2 --param1=3")));
}

TEST(ParserTestSuite, FlagTest) {
    Parser parser("My Parser");
    parser.add_flag('f', "flag1");

    parser.parse(SplitString("app --flag1"));
    ASSERT_TRUE(parser.get_flag("flag1"));
}

TEST(ParserTestSuite, FlagsTest) {
    Parser parser("My Parser");
    bool flag3;
    parser.add_flag('a', "flag1");
    parser.add_flag('b', "flag2").set_default(true);
    parser.add_flag('c', "flag3").store(flag3);

    parser.parse(SplitString("app -ac"));
    ASSERT_TRUE(parser.get_flag("flag1"));
    ASSERT_TRUE(parser.get_flag("flag2"));
    ASSERT_TRUE(flag3);
}

TEST(ParserTestSuite, PositionalArgTest) {
    Parser parser("My Parser");
    std::vector<int> values;
    parser.add_int_option("Param1").multiple(1).positional().store(values);

    parser.parse(SplitString("app 1 2 3 4 5"));
    ASSERT_EQ(values[0], 1);
    ASSERT_EQ(values[2], 3);
    ASSERT_EQ(values.size(), 5);
}

TEST(ParserTestSuite, HelpTest) {
    Parser parser("My Parser");
    parser.add_help('h', "help", "Some Description about program");

    parser.parse(SplitString("app --help"));
}

TEST(ParserTestSuite, HelpStringTest) {
    Parser parser("My Parser");
    parser.add_help('h', "help", "Some Description about program");
    parser.add_string_option('i', "input", "File path for input file")
          .multiple(1);
    parser.add_flag('s', "flag1", "Use some logic").set_default(true);
    parser.add_flag('p', "flag2", "Use some logic");
    parser.add_int_option("number", "Some Number");

    //parser.parse(SplitString("app --help"));
    ASSERT_EQ(
        parser.get_help(),
        "My Parser\n"
        "Some Description about program\n"
        "\n"
        "-i, --input=<string>, File path for input file [repeated, min args = 1]\n"
        "-s, --flag1, Use some logic [default = true]\n"
        "-p, --flag2, Use some logic\n"
        "    --number=<int>, Some Number\n"
        "\n"
        "-h, --help Display this help and exit\n"
    );
}
