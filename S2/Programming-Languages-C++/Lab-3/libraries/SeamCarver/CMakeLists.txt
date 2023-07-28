project(SeamCarver)

add_library(${PROJECT_NAME} include/${PROJECT_NAME}.hpp src/${PROJECT_NAME}.cpp
                            include/Image.hpp           src/Image.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC include)

add_library(big1::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(tests tests/test.cpp)
target_link_libraries(tests PRIVATE GTest::GTest big1::${PROJECT_NAME})
gtest_discover_tests(tests)

if(COMPILE_OPTS)
    target_compile_options(${PROJECT_NAME} PUBLIC ${COMPILE_OPTS})
    target_link_options(${PROJECT_NAME} PUBLIC ${LINK_OPTS})

    target_compile_options(tests PUBLIC ${COMPILE_OPTS})
    target_link_options(tests PUBLIC ${LINK_OPTS})
endif()
