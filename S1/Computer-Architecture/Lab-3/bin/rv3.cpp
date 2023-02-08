#include "rv3.h"

std::pair<fs::path, fs::path> ParseArguments(const int argc, char** argv) {
	if (argc != 3) {
		throw std::runtime_error("rv3 error: Wrong amount of arguments (should be 3): " + std::to_string(argc));
	}

	std::pair<fs::path, fs::path> result = { argv[1], argv[2] };

	if (!elf::Validate(result.first)) {
		throw std::invalid_argument("rv3 error: Invalid argument was passed: " + result.first.string());
	}

	return result;
}

std::string ToHex(const int32_t value, const bool padding = true) {
	std::stringstream ss;

	ss << std::hex << value;

	std::string hex = ss.str();

	return padding ? std::string(8 - ss.str().size(), '0') + ss.str() : ss.str();
}

std::string BindType(const unsigned char info) {
	constexpr auto bind = [](const unsigned char info) {
		switch (info >> 4) {
		case 0:
			return 'l';
		case 1:
			return 'g';
		case 2:
			return 'w';
		case 13:
			return 'L';
		case 15:
			return 'H';
		default:
			return ' ';
		}
	};
	constexpr auto type = [](const unsigned char info) -> std::string {
		switch (info & 0xf) {
		case 0:
			return "  ";
		case 1:
			return " O";
		case 2:
			return " F";
		case 3:
			return "d ";
		case 4:
			return "df";
		case 13:
			return "LO";
		case 15:
			return "HI";
		default:
			return "  ";
		}
	};

	std::stringstream ss;

	ss << bind(info) << std::string(4, ' ') << type(info);

	return ss.str();
}

std::string Visibility(const unsigned char other) {
	switch (other & 0x3) {
	case 0:
		return "Default";
	case 1:
		return "Internal";
	case 2:
		return "Hidden";
	case 3:
		return "Protected";
	default:
		return "";
	}
}

std::string Association(const elf::Elf& elf, const elf::Elf32_Half st_shndx) {
	if (st_shndx >= elf.sections.size() || st_shndx < 0) {
		return "*ABS*";
	}

	std::string result{ elf.shstr.data() + elf.sections[st_shndx].sh_name };

	return result.empty() ? "*UND*" : result;
}

std::string StringifyIter(
	const elf::Inst& inst, const elf::Elf32_Addr addr, const std::map<elf::Elf32_Addr, std::string>& symtab,
	std::map<elf::Elf32_Addr, std::string>& marks
) {
	std::string result = inst.fmt;

	if (const size_t pos = result.find("%R"); pos != std::string::npos) {
		result.replace(pos, 2, inst.rd);
	}

	if (const size_t pos = result.find("%1S"); pos != std::string::npos) {
		result.replace(pos, 3, inst.rs1);
	}

	if (const size_t pos = result.find("%2S"); pos != std::string::npos) {
		result.replace(pos, 3, inst.rs2);
	}

	if (const size_t pos = result.find("%I"); pos != std::string::npos) {
		result.replace(pos, 2, std::to_string(inst.imm));
	}

	if (const size_t pos = result.find("%0X"); pos != std::string::npos) {
		std::stringstream ss;

		ss << std::hex << addr + inst.imm;

		std::string associate;
		if (symtab.contains(addr + inst.imm)) {
			associate = symtab.at(addr + inst.imm);
		}
		else {
			associate = "L" + std::to_string(marks.size());
			marks[addr + inst.imm] = associate;
		}

		result.replace(pos, 3, ss.str() + " <" + associate + '>');
	}

	if (const size_t pos = result.find("%X"); pos != std::string::npos) {
		std::stringstream ss;

		ss << std::hex << inst.imm;

		result.replace(pos, 2, ss.str());
	}

	return result;
}

void SymtabOut(std::ostream& out, const elf::Elf& elf) {
	out << "SYMBOL TABLE\n\n";

	for (const auto& sym : elf.symtab) {
		out << ToHex(sym.st_value) << ' ';
		out << BindType(sym.st_info) << ' ';
		out << Association(elf, sym.st_shndx) << ' ';
		out << ToHex(sym.st_size) << ' ';
		out << (sym.st_name == 0 ? Association(elf, sym.st_shndx) : std::string{ elf.strtab.data() + sym.st_name }) <<
			'\n';
	}

	out << std::endl;
}

void DisAsmOut(std::ostream& out, const elf::Elf& elf) {
	out << "Disassembly of section .text:\n";

	std::map<elf::Elf32_Addr, std::string> functions;
	std::map<elf::Elf32_Addr, std::string> symtab;
	std::map<elf::Elf32_Addr, std::string> marks;

	for (const auto& sym : elf.symtab) {
		if (BindType(sym.st_info).back() == 'F') {
			functions[sym.st_value] = std::string{ elf.strtab.data() + sym.st_name };
		}

		symtab[sym.st_value] = std::string{ elf.strtab.data() + sym.st_name };
	}

	auto addr_iter = functions.begin();
	elf::Elf32_Addr addr = addr_iter->first;

	for (const auto& inst : elf.text) {
		StringifyIter(inst.second, addr, symtab, marks);

		addr += sizeof(elf::RawInst);
	}

	addr = addr_iter->first;

	for (const auto& inst : elf.text) {
		if (addr_iter != functions.end() && addr_iter->first == addr) {
			out << '\n';
			out << ToHex(addr_iter->first) << " <" << addr_iter->second << ">:\n";

			++addr_iter;
		}

		out << '\t' << ToHex(addr, false) << ":\t" << ToHex(inst.first) << "\t\t" << StringifyIter(
			inst.second, addr, symtab, marks
		);

		if (marks.contains(addr)) {
			out << ", " << marks[addr];
		}

		out << '\n';

		addr += sizeof(elf::RawInst);
	}

	out << std::endl;
}

void Print(const elf::Elf& data, const fs::path& path) {
	std::ofstream out{ path, std::ios::out };
	out.exceptions(std::ios::failbit);

	SymtabOut(out, data);
	DisAsmOut(out, data);
}

int main(int argc, char** argv) {

	try {
		std::pair<fs::path, fs::path> args = ParseArguments(argc, argv);
		Print(elf::Parse(args.first), args.second);
	}
	catch (std::exception& err) {
		std::cout << err.what();
	}

	return 0;
}