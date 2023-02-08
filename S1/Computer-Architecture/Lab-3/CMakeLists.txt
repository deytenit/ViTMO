# CMakeList.txt : CMake project for Elf, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.12)

project("Elf")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_subdirectory("lib")

add_subdirectory("bin")

# TODO: Add tests and install targets if needed.
