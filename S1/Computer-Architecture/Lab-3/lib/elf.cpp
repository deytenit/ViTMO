#include <array>
#include <elf.h>
#include <fstream>

#include <disasm.h>

bool elf::Validate(const fs::path& path) {
    const std::string error = "elf::Validate error: ";

    constexpr char kElfClass = 1;
    constexpr Elf32_Half kEMachine = 243;

    const Elf elf = Parse(path);

    if (memcmp(MAGIC, elf.header.e_ident, sizeof MAGIC) != 0) {
        throw std::invalid_argument(error + "Magic number is not compatible.");
    }

    if (elf.header.e_ident[4] != kElfClass) {
        throw std::invalid_argument(error + "Elf file should be '32-bit object'.");
    }

    if (elf.header.e_machine != kEMachine) {
        throw std::invalid_argument(error + "Elf file is not 'RISC-V ELF' compatible.");
    }

    return is_regular_file(path);
}

elf::Elf elf::Parse(const fs::path& path) {
    std::ifstream in{path, std::ios::in | std::ios::binary};
    in.exceptions(std::ios::failbit);

    Elf result;

    Elf32_Ehdr header = ParseEhdr(in);
    std::vector<Elf32_Shdr> sections = ParseShdr(in, header.e_shoff, header.e_shnum);
    std::vector<char> shstr = ParseShstr(in, sections[header.e_shstrndx]);
    std::vector<Elf32_Sym> symtab;
    std::vector<char> strtab;
    std::vector<std::pair<RawInst, Inst>> text;

    for (const auto& shdr : sections) {
        if (std::string(shstr.data() + shdr.sh_name) == ".symtab") {
            symtab = ParseSym(in, shdr);
        }

        if (std::string(shstr.data() + shdr.sh_name) == ".strtab") {
            strtab = ParseStrtab(in, shdr);
        }

        if (std::string(shstr.data() + shdr.sh_name) == ".text") {
            text = ParseText(in, shdr);
        }
    }

    return {header, sections, symtab, shstr, strtab, text};
}

elf::Elf32_Ehdr elf::ParseEhdr(std::istream& in) {
    std::array<char, sizeof(Elf32_Ehdr)> buffer{};

    in.read(buffer.data(), buffer.size());

    const Elf32_Ehdr& result = *reinterpret_cast<Elf32_Ehdr*>(buffer.data());

    return result;
}

std::vector<elf::Elf32_Shdr> elf::ParseShdr(std::istream& in, const Elf32_Off e_shoff, const Elf32_Half e_shnum) {
    in.seekg(e_shoff);

    std::vector<Elf32_Shdr> result;
    result.reserve(e_shnum);

    std::array<char, sizeof(Elf32_Shdr)> buffer{};

    for (Elf32_Half i = 0; i < e_shnum; ++i) {
        in.read(buffer.data(), buffer.size());

        result.push_back(*reinterpret_cast<Elf32_Shdr*>(buffer.data()));
    }

    return result;
}

std::vector<elf::Elf32_Sym> elf::ParseSym(std::istream& in, const Elf32_Shdr& shdr) {
    in.seekg(shdr.sh_offset);

    std::vector<Elf32_Sym> result;

    std::array<char, sizeof(Elf32_Sym)> buffer{};

    for (Elf32_Word i = 0; i < shdr.sh_size / sizeof(Elf32_Sym); ++i) {
        in.read(buffer.data(), buffer.size());

        result.push_back(*reinterpret_cast<Elf32_Sym*>(buffer.data()));
    }

    return result;
}

std::vector<char> elf::ParseShstr(std::istream& in, const Elf32_Shdr& shstr) {
    in.seekg(shstr.sh_offset);

    std::vector<char> result(shstr.sh_size);

    in.read(result.data(), shstr.sh_size);

    return {result};
}

std::vector<char> elf::ParseStrtab(std::istream& in, const Elf32_Shdr& strtab) {
    in.seekg(strtab.sh_offset);

    std::vector<char> result(strtab.sh_size);

    in.read(result.data(), strtab.sh_size);

    return {result};
}

std::vector<std::pair<elf::RawInst, elf::Inst>> elf::ParseText(std::istream& in, const Elf32_Shdr& text) {
    in.seekg(text.sh_offset);
    RawInst inst = 0;

    std::vector<std::pair<RawInst, Inst>> result;
    result.reserve(text.sh_size / sizeof(inst));

    for (Elf32_Word i = 0; i < result.capacity(); ++i) {
        in.read(reinterpret_cast<char*>(&inst), sizeof(inst));

        result.emplace_back(inst, DiInst(inst));
    }

    return result;
}
