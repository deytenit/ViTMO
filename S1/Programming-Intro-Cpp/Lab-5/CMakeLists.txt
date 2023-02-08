cmake_minimum_required(VERSION 3.15)

project(
    labwork5
    VERSION 1.0
    DESCRIPTION "C++ Year 2022 labwork 5"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_subdirectory(lib)
add_subdirectory(src)


enable_testing()
add_subdirectory(test)
