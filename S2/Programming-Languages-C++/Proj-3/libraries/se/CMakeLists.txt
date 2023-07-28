project(se)

add_library(${PROJECT_NAME}
    include/se/Searcher.hpp src/Searcher.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC include)

add_library(se::se ALIAS ${PROJECT_NAME})

enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(tests tests/load_test.cpp tests/test.cpp tests/list.inl tests/queries.inl)
target_link_libraries(tests PRIVATE GTest::GTest se::se)
gtest_discover_tests(tests)

file(GLOB ETC_FILES RELATIVE ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/etc/*)

# Copy data files for tests
add_custom_command(
    OUTPUT ${ETC_FILES}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/etc ${CMAKE_CURRENT_BINARY_DIR}/test/etc
    DEPENDS ${ETC_FILES}
    COMMENT "Copying test data")

add_custom_target(
    etc DEPENDS ${ETC_FILES})

add_custom_target(
    etc_docs
    COMMAND ${CMAKE_COMMAND} -E tar "xf" ${PROJECT_SOURCE_DIR}/test.tar.bz2
    COMMENT "Extracting test data")

add_dependencies(tests etc_docs etc)

if(COMPILE_OPTS)
    target_compile_options(${PROJECT_NAME} PUBLIC ${COMPILE_OPTS})
    target_link_options(${PROJECT_NAME} PUBLIC ${LINK_OPTS})

    target_compile_options(tests PUBLIC ${COMPILE_OPTS})
    target_link_options(tests PUBLIC ${LINK_OPTS})
endif()
