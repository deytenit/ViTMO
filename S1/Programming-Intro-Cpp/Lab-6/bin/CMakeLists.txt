add_executable(lab6 main.cpp)

target_link_libraries(lab6 omfl_parser)
target_include_directories(lab6 PRIVATE ${PROJECT_SOURCE_DIR})
