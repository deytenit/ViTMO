add_library(elf_parser elf.cpp disasm.cpp)
target_include_directories(elf_parser PUBLIC ${PROJECT_SOURCE_DIR}/include/)
