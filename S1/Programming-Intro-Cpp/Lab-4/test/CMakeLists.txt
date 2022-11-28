include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  encoder_test
    "encoder_test.cpp"
)

target_link_libraries(
    encoder_test
    encoder
    GTest::gtest_main
    GTest::gmock_main
)

include(GoogleTest)
gtest_discover_tests(encoder_test)
