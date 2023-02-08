cmake_minimum_required(VERSION 3.12)

project("itmo-comp-arch22-lab4-unknowableshade")

# Add source to this project's executable.
add_executable(omp4 "bin/hard.cpp")
add_executable(cheat "bin/cheat.cpp")

set_property(TARGET omp4 PROPERTY CXX_STANDARD 20)
set_property(TARGET cheat PROPERTY CXX_STANDARD 20)

find_package(OpenMP)
if(OPENMP_CXX_FOUND)
	target_link_libraries(omp4 PUBLIC OpenMP::OpenMP_CXX)
	target_link_libraries(cheat PUBLIC OpenMP::OpenMP_CXX)
endif()