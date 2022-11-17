add_library(interactions "interactions.cpp" "../include/interactions.h")
add_library(sandbox "sandbox.cpp" "../include/sandbox.h")
add_library(sandpile "sandpile.cpp" "../include/sandpile.h")
add_library(imgmanip "imgmanip.cpp" "../include/imgmanip.h")
add_library(argument_parser "argument_parser.cpp" "../include/argument_parser.h")

target_include_directories(interactions PUBLIC "${PROJECT_SOURCE_DIR}/include/")
target_include_directories(sandpile PUBLIC "${PROJECT_SOURCE_DIR}/include/")
target_include_directories(sandbox PUBLIC "${PROJECT_SOURCE_DIR}/include/")
target_include_directories(imgmanip PUBLIC "${PROJECT_SOURCE_DIR}/include/")
target_include_directories(argument_parser PUBLIC "${PROJECT_SOURCE_DIR}/include/")

target_link_libraries(interactions argument_parser sandpile sandbox imgmanip)