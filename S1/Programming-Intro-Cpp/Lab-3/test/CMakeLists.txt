include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  sandbox_test
  "sandbox_test.cpp"
)

target_link_libraries(
  sandbox_test
  sandbox
  GTest::gtest_main
  GTest::gmock_main
)

add_executable(
  sandpile_test
  "sandpile_test.cpp"
)

target_link_libraries(
  sandpile_test
  sandbox
  sandpile
  GTest::gtest_main
  GTest::gmock_main
)

target_include_directories(sandpile_test PUBLIC ${PROJECT_SOURCE_DIR})

include(GoogleTest)
gtest_discover_tests(sandbox_test)
gtest_discover_tests(sandpile_test)