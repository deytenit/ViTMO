add_library(argument_parser "parser.cpp" "builders.cpp" "options.cpp" "exceptions.cpp")
target_include_directories(argument_parser PUBLIC ${PROJECT_SOURCE_DIR}/include/)

