project(${TASK_VARIANT})

add_library(${TASK_VARIANT} include/tree/Tree.hpp src/Tree.cpp)

target_include_directories(${TASK_VARIANT} PUBLIC include)

add_library(tree::${TASK_VARIANT} ALIAS ${TASK_VARIANT})

enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(tests tests/tests.cpp)
target_compile_definitions(tests PRIVATE TASK_VARIANT=${TASK_VARIANT})
target_link_libraries(tests PRIVATE GTest::GTest tree::${TASK_VARIANT})
gtest_discover_tests(tests)

if(COMPILE_OPTS)
    target_compile_options(${TASK_VARIANT} PUBLIC ${COMPILE_OPTS})
    target_link_options(${TASK_VARIANT} PUBLIC ${LINK_OPTS})

    target_compile_options(tests PUBLIC ${COMPILE_OPTS})
    target_link_options(tests PUBLIC ${LINK_OPTS})
endif()
