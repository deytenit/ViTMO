cmake_minimum_required(VERSION 3.12)
project(
    OMFL_Parser
    VERSION 1.0
    DESCRIPTION "C++ Year 2022 Labwork 6"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)

link_directories(lib)

add_subdirectory(lib)
add_subdirectory(bin)

enable_testing()
add_subdirectory(tests)