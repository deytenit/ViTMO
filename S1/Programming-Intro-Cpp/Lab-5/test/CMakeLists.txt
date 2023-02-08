include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
    argparser_tests
    argparser_test.cpp
)

target_link_libraries(
    argparser_tests
    argument_parser
    GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(argparser_tests)

