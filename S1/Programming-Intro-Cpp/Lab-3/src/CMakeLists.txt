﻿# CMakeList.txt : CMake project for src, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (src "src.cpp" "src.h")

target_link_libraries(
	src
	sandbox
	imgmanip
	sandpile
	interactions
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET src PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
