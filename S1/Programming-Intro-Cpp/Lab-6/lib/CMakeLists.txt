add_library(omfl_tools "misc.cpp")
target_include_directories(omfl_tools PRIVATE ${PROJECT_SOURCE_DIR}/include/)

add_library(omfl_parser "omfl_parser.cpp" "omfl.cpp")
target_include_directories(omfl_parser PUBLIC ${PROJECT_SOURCE_DIR}/include/)

target_link_libraries(omfl_parser omfl_tools)