#pragma once

#include <cstdint>
#include <filesystem>

#include "disasm.h"

namespace fs = std::filesystem;

namespace elf {
    constexpr char MAGIC[4] = {0x7f, 'E', 'L', 'F'};

    using Elf32_Half = uint16_t;

    using Elf32_Word = uint32_t;
    using Elf32_Sword = int32_t;

    using Elf32_Xword = uint64_t;
    using Elf32_Sxword = int64_t;

    using Elf32_Addr = int32_t;

    using Elf32_Off = uint32_t;

    using Elf32_Section = uint16_t;

    using Elf32_Versym = Elf32_Half;

    constexpr uint32_t EI_NIDENT = 16;

    struct Elf32_Ehdr {
        unsigned char e_ident[EI_NIDENT]; // Magic number and other info
        Elf32_Half e_type; // Object file type
        Elf32_Half e_machine; // Architecture
        Elf32_Word e_version; // Object file version
        Elf32_Addr e_entry; // Entry point virtual address
        Elf32_Off e_phoff; // Program header table file offset
        Elf32_Off e_shoff; // Section header table file offset
        Elf32_Word e_flags; // Processor-specific flags
        Elf32_Half e_ehsize; // ELF header size in bytes
        Elf32_Half e_phentsize; // Program header table entry size
        Elf32_Half e_phnum; // Program header table entry count
        Elf32_Half e_shentsize; // Section header table entry size
        Elf32_Half e_shnum; // Section header table entry count
        Elf32_Half e_shstrndx; // Section header string table index
    };

    struct Elf32_Shdr {
        Elf32_Word sh_name; // Section name (string tbl index)
        Elf32_Word sh_type; // Section type
        Elf32_Word sh_flags; // Section flags
        Elf32_Addr sh_addr; // Section virtual addr at execution
        Elf32_Off sh_offset; // Section file offset
        Elf32_Word sh_size; // Section size in bytes
        Elf32_Word sh_link; // Link to another section
        Elf32_Word sh_info; // Additional section information
        Elf32_Word sh_addralign; // Section alignment
        Elf32_Word sh_entsize; // Entry size if section holds table
    };

    struct Elf32_Sym {
        Elf32_Word st_name;
        Elf32_Addr st_value;
        Elf32_Word st_size;
        unsigned char st_info;
        unsigned char st_other;
        Elf32_Half st_shndx;
    };

    struct Elf {
        Elf32_Ehdr header;
        std::vector<Elf32_Shdr> sections;
        std::vector<Elf32_Sym> symtab;
        std::vector<char> shstr;
        std::vector<char> strtab;
        std::vector<std::pair<RawInst, Inst>> text;
    };

    bool Validate(const fs::path& path);

    Elf Parse(const fs::path& path);

    Elf32_Ehdr ParseEhdr(std::istream& in);
    std::vector<Elf32_Shdr> ParseShdr(std::istream& in, Elf32_Off e_shoff, Elf32_Half e_shnum);
    std::vector<Elf32_Sym> ParseSym(std::istream& in, const Elf32_Shdr& shdr);

    std::vector<char> ParseShstr(std::istream& in, const Elf32_Shdr& shstr);
    std::vector<char> ParseStrtab(std::istream& in, const Elf32_Shdr& strtab);
    std::vector<std::pair<RawInst, Inst>> ParseText(std::istream& in, const Elf32_Shdr& text);
};
