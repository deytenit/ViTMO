add_library(config "config.cpp")
target_include_directories(config PUBLIC ${PROJECT_SOURCE_DIR}/include/)

add_library(encoder "encoder.cpp")
target_include_directories(encoder PUBLIC ${PROJECT_SOURCE_DIR}/include/)

add_library(argument_parser "argument_parser.cpp")
target_include_directories(argument_parser PUBLIC ${PROJECT_SOURCE_DIR}/include/)

add_library(hamming_archive "hamming_archive.cpp")
target_include_directories(hamming_archive PUBLIC ${PROJECT_SOURCE_DIR}/include/)

add_library(interactions "interactions.cpp")
target_include_directories(interactions PUBLIC ${PROJECT_SOURCE_DIR}/include/)

target_link_libraries(
    hamming_archive
    encoder
)

target_link_libraries(
    config
    argument_parser
)

target_link_libraries(
    interactions
    hamming_archive
    config
)

