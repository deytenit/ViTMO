project(expr)

file(GLOB_RECURSE expr_include
     "include/*.hpp"
)

file(GLOB_RECURSE expr_src
     "src/*.cpp"
)

add_library(${PROJECT_NAME} ${expr_include} ${expr_src})

find_package(Catch2 3 REQUIRED)

target_include_directories(${PROJECT_NAME} PUBLIC include)

add_library(expr::expr ALIAS ${PROJECT_NAME})

add_executable(tests test/test_exponential.cpp test/test_expression.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain expr::expr)

include(CTest)
include(Catch)
catch_discover_tests(tests)

if(COMPILE_OPTS)
    target_compile_options(${PROJECT_NAME} PUBLIC ${COMPILE_OPTS})
    target_link_options(${PROJECT_NAME} PUBLIC ${LINK_OPTS})

    target_compile_options(tests PUBLIC ${COMPILE_OPTS})
    target_link_options(tests PUBLIC ${LINK_OPTS})
endif()