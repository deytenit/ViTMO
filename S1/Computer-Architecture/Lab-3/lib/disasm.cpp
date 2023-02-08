#include <disasm.h>

#include <stdexcept>

uint32_t elf::GetCsrImm(const RawInst& inst) {
    return (inst >> 15) & 0x1f;
}

uint32_t elf::GetRD(const RawInst& inst) {
    return (inst >> 7) & 0x1f;
}

uint32_t elf::GetRS1(const RawInst& inst) {
    return (inst >> 15) & 0x1f;
}

uint32_t elf::GetRS2(const RawInst& inst) {
    return (inst >> 20) & 0x1f;
}

uint32_t elf::Bextr(
    const uint32_t src, const uint32_t start,
    const uint32_t len
) {
    return (src >> start) & ((1 << len) - 1);
}

int32_t elf::Shamt(const uint32_t value) {
    return Bextr(value, 20, 6);
}

uint32_t elf::ImmSign(const uint32_t value) {
    const int sign = Bextr(value, 31, 1);
    return sign == 1 ? static_cast<uint32_t>(-1) : 0;
}

int32_t elf::GetBImm(const uint32_t value) {
    return (Bextr(value, 8, 4) << 1) + (Bextr(value, 25, 6) << 5) +
        (Bextr(value, 7, 1) << 11) + (ImmSign(value) << 12);
}

uint32_t elf::GetIImmUnsigned(const uint32_t value) {
    return value >> 20;
}

int32_t elf::GetIImm(const uint32_t value) {
    int val = Bextr(value, 20, 12);
    int sign = Bextr(value, 31, 1) == 1 ? -1 : 1;

    if (sign == -1)
        val |= (0xfffff000);
    return val;
}

int32_t elf::GetSImm(const uint32_t value) {
    int sign = Bextr(value, 31, 1) == 1 ? -1 : 1;
    int val = Bextr(value, 7, 5) + (Bextr(value, 25, 7) << 5);

    if (sign == -1)
        val |= (0xfffff000);
    return val;
}

int32_t elf::GetJImm(const uint32_t value) {
    int sign = Bextr(value, 31, 1) == 1 ? -1 : 1;
    int val = (Bextr(value, 21, 10) << 1) + (Bextr(value, 20, 1) << 11) +
        (Bextr(value, 12, 8) << 12) + (Bextr(value, 31, 1) << 20);

    if (sign == -1)
        val |= (0xfff00000);
    return val;
}

elf::Inst elf::Add(const RawInst& inst) {
    return {
        "add", REGISTERS[GetRS1(inst)], REGISTERS[GetRS2(inst)],
        REGISTERS[GetRD(inst)], 0, "add\t%R,%1S,%2S"
    };
}

elf::Inst elf::Addi(const RawInst& inst) {
    const int32_t imm = GetIImm(inst);
    if (GetRS1(inst) == 0) {
        if (GetRD(inst) == 0 && imm == 0) {
            return {"nop", "", "", "", 0, "nop"};
        }

        return {"li", "", "", REGISTERS[GetRD(inst)], imm, "li\t%R,%I"};
    }

    if (imm == 0) {
        return {"mv", REGISTERS[GetRS1(inst)], "", REGISTERS[GetRD(inst)], 0, "mv\t%R,%1S"};
    }

    return {"addi", REGISTERS[GetRS1(inst)], "", REGISTERS[GetRD(inst)], imm, "addi\t%R,%1S,%I"};
}

elf::Inst elf::And(const RawInst& inst) {
    return {
        "and", REGISTERS[GetRS1(inst)], REGISTERS[GetRS2(inst)],
        REGISTERS[GetRD(inst)], 0, "and\t%R,%1S,%2S"
    };
}

elf::Inst elf::Andi(const RawInst& inst) {
    const int32_t imm = GetIImm(inst);

    return {"andi", REGISTERS[GetRS1(inst)], "", REGISTERS[GetRD(inst)], imm, "andi\t%R,0x%X"};
}

elf::Inst elf::Auipc(const RawInst& inst) {
    const auto* in = reinterpret_cast<const InstructionU*>(&inst);
    return {
        "auipc", "", "", REGISTERS[GetRD(inst)],
        (in->data << 12 >> 12) & 0xfffff, "auipc\t%R,0x%X"
    };
}

elf::Inst elf::Beq(const RawInst& inst, const int32_t pc) {
    if (GetRS2(inst) == 0) {
        return {
            "begz", REGISTERS[GetRS1(inst)], "", "", GetBImm(inst) + pc, "beqz\t%1S,0x%X"

        };
    }

    return {
        "beq", REGISTERS[GetRS1(inst)], REGISTERS[GetRS2(inst)], "",
        GetBImm(inst) + pc, "beqz\t%1S,%2S,0x%X"
    };
}

elf::Inst elf::Bge(const RawInst& inst, const int32_t pc) {
    if (GetRS2(inst) == 0) {
        return {"bgez", REGISTERS[GetRS1(inst)], "", "", GetBImm(inst) + pc, "bgez\t%1S,0x%X"};
    }
    if (GetRS1(inst) == 0) {
        return {"blez", "", REGISTERS[GetRS2(inst)], "", GetBImm(inst) + pc, "blez\t%2S,0x%X"};
    }
    return {
        "bge", REGISTERS[GetRS1(inst)], REGISTERS[GetRS2(inst)], "",
        GetBImm(inst) + pc, "bge\t%1S,%2S,0x%X"
    };
}

elf::Inst elf::Bgeu(const RawInst& inst, const int32_t pc) {
    return {
        "bgeu", REGISTERS[GetRS1(inst)], REGISTERS[GetRS2(inst)], "",
        GetBImm(inst) + pc, "bgeu\t%1S,%2S,0x%X"
    };
}

elf::Inst elf::Blt(const RawInst& inst, const int32_t pc) {
    if (GetRS2(inst) == 0) {
        return {"bltz", REGISTERS[GetRS1(inst)], "", "", GetBImm(inst) + pc, "bltz\t%1S,0x%X"};
    }

    if (GetRS1(inst) == 0) {
        return {"bgtz", "", REGISTERS[GetRS2(inst)], "", GetBImm(inst) + pc, "bgtz\t%2S,0x%X"};
    }

    return {
        "blt", REGISTERS[GetRS1(inst)], REGISTERS[GetRS2(inst)], "",
        GetBImm(inst) + pc, "blt\t%1S,%2S,0x%X"
    };
}

elf::Inst elf::Bltu(const RawInst& inst, const int32_t pc) {
    return {
        "bltu", REGISTERS[GetRS1(inst)], REGISTERS[GetRS2(inst)], "",
        GetBImm(inst) + pc, "bltu\t%1S,%2S,0x%X"
    };
}

elf::Inst elf::Bne(const RawInst& inst, const int32_t pc) {
    if (GetRS2(inst) == 0) {
        return {"bnez", REGISTERS[GetRS1(inst)], "", "", GetBImm(inst) + pc, "bnez\t%1S,0x%X"};
    }

    return {
        "bne", REGISTERS[GetRS1(inst)], REGISTERS[GetRS2(inst)], "",
        GetBImm(inst) + pc, "bne\t%1S,%2S,0x%0X"
    };
}

elf::Inst elf::Fence(const RawInst& inst) {
    return {"fence", "", "", "", 0, "fence"};
}

elf::Inst elf::Fencei(const RawInst& inst) {
    return {"fence.i", "", "", "", 0, "fence.i"};
}

elf::Inst elf::Jal(const RawInst& inst, const int32_t pc) {
    const int32_t offset = GetJImm(inst);
    const int32_t rd = GetRD(inst);
    if (rd == 0) {
        return {"j", "", "", "", offset + pc, "j\t0x%0X"};
    }

    return {"jal", "", "", REGISTERS[rd], offset + pc, "jal\t%R,0x%0X"};
}

elf::Inst elf::Jalr(const RawInst& inst) {
    const int32_t offset = GetIImm(inst);
    if (offset == 0 && GetRD(inst) == 0 && GetRS1(inst) == 1) {
        return {"ret", "", "", "", 0, "ret"};
    }

    if (offset == 0 && GetRD(inst) == 1) {
        return {"jalr", REGISTERS[GetRS1(inst)], "", "", 0, "jalr\t%1S"};
    }
    if (offset == 0 && GetRD(inst) == 0) {
        return {"jr", REGISTERS[GetRS1(inst)], "", "", 0, "jr\t%1S"};
    }
    if (GetRD(inst) == GetRS1(inst)) {
        return {"jalr", REGISTERS[GetRS1(inst)], "", "", offset, "jalr\t%I(%1S)"};
    }

    return {"jalr", REGISTERS[GetRS1(inst)], "", REGISTERS[GetRD(inst)], offset, "jalr\t%R,%1S,%I"};
}

elf::Inst elf::Lb(const RawInst& inst) {
    const int32_t offset = GetIImm(inst);

    return {"lb", REGISTERS[GetRS1(inst)], "", REGISTERS[GetRD(inst)], offset, "lb\t%R,%I(%1S)"};
}

elf::Inst elf::Lbu(const RawInst& inst) {
    const int32_t offset = GetIImm(inst);

    return {"lbu", REGISTERS[GetRS1(inst)], "", REGISTERS[GetRD(inst)], offset, "lbu\t%R,%I(%1S)"};
}

elf::Inst elf::Lh(const RawInst& inst) {
    const int32_t offset = GetIImm(inst);

    return {"lh", REGISTERS[GetRS1(inst)], "", REGISTERS[GetRD(inst)], offset, "lh\t%R,%I(%1S)"};
}

elf::Inst elf::Lhu(const RawInst& inst) {
    const int32_t offset = GetIImm(inst);

    return {"lhu", REGISTERS[GetRS1(inst)], "", REGISTERS[GetRD(inst)], offset, "lhu\t%R,%I(%1S)"};
}

elf::Inst elf::Lui(const RawInst& inst) {
    const auto* in = reinterpret_cast<const InstructionU*>(&inst);
    return {
        "lui", "", "", REGISTERS[GetRD(inst)],
        in->data << 12 >> 12 & 0xfffff, "lui\t%R,0x%X"
    };
}

elf::Inst elf::Lw(const RawInst& inst) {
    const int32_t offset = GetIImm(inst);

    return {"lw", REGISTERS[GetRS1(inst)], "", REGISTERS[GetRD(inst)], offset, "lw\t%R,%I(%1S)"};
}

elf::Inst elf::Or(const RawInst& inst) {
    return {
        "or", REGISTERS[GetRS1(inst)], REGISTERS[GetRS2(inst)],
        REGISTERS[GetRD(inst)], 0, "or\t%R,%1S,%2S"
    };
}

elf::Inst elf::Ori(const RawInst& inst) {
    const auto* in = reinterpret_cast<const InstructionI*>(&inst);

    return {"ori", REGISTERS[GetRS1(inst)], "", REGISTERS[GetRD(inst)], in->imm, "ori\t%R,%1S,%I"};
}

elf::Inst elf::Sb(const RawInst& inst) {
    const int32_t offset = GetSImm(inst);

    return {"sb", REGISTERS[GetRS1(inst)], REGISTERS[GetRS2(inst)], "", offset, "sb\t%2S,%I(%1S)"};
}

elf::Inst elf::Sh(const RawInst& inst) {
    const int32_t offset = GetSImm(inst);

    return {"sh", REGISTERS[GetRS1(inst)], REGISTERS[GetRS2(inst)], "", offset, "sh\t%2S,%I(%1S)"};
}

elf::Inst elf::Sll(const RawInst& inst) {
    return {
        "sll", REGISTERS[GetRS1(inst)], REGISTERS[GetRS2(inst)],
        REGISTERS[GetRD(inst)], 0, "sll\t%R,%1S,%2S"
    };
}

elf::Inst elf::Slli(const RawInst& inst) {
    const auto* in = reinterpret_cast<const InstructionIShift*>(&inst);

    return {
        "slli", REGISTERS[GetRS1(inst)], "", REGISTERS[GetRD(inst)],
        in->shamt & 0x1F, "slli\t%R,%1S,0x%X"
    };
}

elf::Inst elf::Slt(const RawInst& inst) {
    if (GetRS1(inst) == 0) {
        return {"sgtz", "", REGISTERS[GetRS2(inst)], REGISTERS[GetRD(inst)], 0, "sgtz\t%R,%2S"};
    }

    return {
        "slt", REGISTERS[GetRS1(inst)], REGISTERS[GetRS2(inst)],
        REGISTERS[GetRD(inst)], 0, "slt\t%R,%1S,%2S"
    };
}

elf::Inst elf::Sltu(const RawInst& inst) {
    if (GetRS1(inst) == 0) {
        return {"snez", "", REGISTERS[GetRS2(inst)], REGISTERS[GetRD(inst)], 0, "snez\t%R,%2S"};
    }
    return {
        "sltu", REGISTERS[GetRS1(inst)], REGISTERS[GetRS2(inst)],
        REGISTERS[GetRD(inst)], 0, "sltu\t%R,%1S,%2S"
    };
}

elf::Inst elf::Slti(const RawInst& inst) {
    const auto* in = reinterpret_cast<const InstructionI*>(&inst);

    return {"slti", REGISTERS[GetRS1(inst)], "", REGISTERS[GetRD(inst)], in->imm, "slti\t%R,%1S,%I"};
}

elf::Inst elf::Sltiu(const RawInst& inst) {
    const auto* in = reinterpret_cast<const InstructionI*>(&inst);
    if (in->imm == 1) {
        return {"seqz", REGISTERS[GetRS1(inst)], "", REGISTERS[GetRD(inst)], 0, "seqz\t%R,%1S"};
    }

    return {
        "sltiu", REGISTERS[GetRS1(inst)], "", REGISTERS[GetRD(inst)],
        in->imm, "sltiu\t%R,%1S,%I"
    };
}

elf::Inst elf::Sra(const RawInst& inst) {
    return {
        "sra", REGISTERS[GetRS1(inst)], REGISTERS[GetRS2(inst)],
        REGISTERS[GetRD(inst)], 0, "sra\t%R,%1S,%2S"
    };
}

elf::Inst elf::Srai(const RawInst& inst) {
    const auto* in = reinterpret_cast<const InstructionIShift*>(&inst);

    return {
        "srai", REGISTERS[GetRS1(inst)], "", REGISTERS[GetRD(inst)],
        in->shamt & 0x1F, "srai\t%R,%1S,0x%X"
    };
}

elf::Inst elf::Srl(const RawInst& inst) {
    return {
        "srl", REGISTERS[GetRS1(inst)], REGISTERS[GetRS2(inst)],
        REGISTERS[GetRD(inst)], 0, "srl\t%R,%1S,%2S"
    };
}

elf::Inst elf::Srli(const RawInst& inst) {
    const auto* in = reinterpret_cast<const InstructionIShift*>(&inst);

    return {
        "srli", REGISTERS[GetRS1(inst)], "", REGISTERS[GetRD(inst)],
        in->shamt & 0x1F, "srli\t%R,%1S,0x%X"
    };
}

elf::Inst elf::Sub(const RawInst& inst) {
    if (GetRS1(inst) == 0) {
        return {"neg", "", REGISTERS[GetRS2(inst)], REGISTERS[GetRD(inst)], 0, "neg\t%R,%2S"};
    }

    return {
        "sub", REGISTERS[GetRS1(inst)], REGISTERS[GetRS2(inst)],
        REGISTERS[GetRD(inst)], 0, "sub\t%R,%1S,%2S"
    };
}

elf::Inst elf::Sw(const RawInst& inst) {
    int32_t offset = GetSImm(inst);

    return {"sw", REGISTERS[GetRS1(inst)], REGISTERS[GetRS2(inst)], "", offset, "sw\t%2S,%I(%1S)"};
}

elf::Inst elf::Xor(const RawInst& inst) {
    return {
        "xor", REGISTERS[GetRS1(inst)], REGISTERS[GetRS2(inst)],
        REGISTERS[GetRD(inst)], 0, "xor\t%R,%1S,%2S"
    };
}

elf::Inst elf::Xori(const RawInst& inst) {
    const auto* in = reinterpret_cast<const InstructionI*>(&inst);
    if (in->imm == -1) {
        return {"not", REGISTERS[GetRS1(inst)], "", REGISTERS[GetRD(inst)], 0, "not\t%R,%1S"};
    }

    return {"xori", REGISTERS[GetRS1(inst)], "", REGISTERS[GetRD(inst)], in->imm, "xori\t%R,%1S,%I"};
}

elf::Inst elf::Mret(const RawInst& inst) {
    return {"mret", "", "", "", 0, "mret"};
}

elf::Inst elf::Sret(const RawInst& inst) {
    return {"sret", "", "", "", 0, "sret"};
}

elf::Inst elf::Uret(const RawInst& inst) {
    return {"uret", "", "", "", 0, "uret"};
}

elf::Inst elf::SfenceVma(const RawInst& inst) {
    return {"sfence.vma", "", "", "", 0, "sfence.vma"};
}

elf::Inst elf::Wfi(const RawInst& inst) {
    return {"wfi", "", "", "", 0, "wfi"};
}

elf::Inst elf::ECall(const RawInst& inst) {
    return {
        "ecall",
        "",
        "",
        "",
        0,
        "ecall"
    };
}

elf::Inst elf::EBreak(const RawInst& inst) {
    return {
        "ebreak",
        "",
        "",
        "",
        0,
        "ebreak"
    };
}

elf::Inst elf::Mul(const RawInst& inst) {
    return {
        "mul",
        REGISTERS[GetRS1(inst)],
        REGISTERS[GetRS2(inst)],
        REGISTERS[GetRD(inst)],
        0,
        "mul\t%R,%1S,%2S"
    };
}

elf::Inst elf::Mulh(const RawInst& inst) {
    return {
        "mulh",
        REGISTERS[GetRS1(inst)],
        REGISTERS[GetRS2(inst)],
        REGISTERS[GetRD(inst)],
        0,
        "mulh\t%R,%1S,%2S"
    };
}

elf::Inst elf::Mulhu(const RawInst& inst) {
    return {
        "mulhu",
        REGISTERS[GetRS1(inst)],
        REGISTERS[GetRS2(inst)],
        REGISTERS[GetRD(inst)],
        0,
        "mulhu\t%R,%1S,%2S"
    };
}

elf::Inst elf::Mulhsu(const RawInst& inst) {
    return {
        "mulhsu",
        REGISTERS[GetRS1(inst)],
        REGISTERS[GetRS2(inst)],
        REGISTERS[GetRD(inst)],
        0,
        "mulhsu\t%R,%1S,%2S"
    };
}

elf::Inst elf::Rem(const RawInst& inst) {
    return {
        "rem",
        REGISTERS[GetRS1(inst)],
        REGISTERS[GetRS2(inst)],
        REGISTERS[GetRD(inst)],
        0,
        "rem\t%R,%1S,%2S"
    };
}

elf::Inst elf::Remu(const RawInst& inst) {
    return {
        "remu",
        REGISTERS[GetRS1(inst)],
        REGISTERS[GetRS2(inst)],
        REGISTERS[GetRD(inst)],
        0,
        "remu\t%R,%1S,%2S"
    };
}

elf::Inst elf::OpDiv(const RawInst& inst) {
    return {
        "div",
        REGISTERS[GetRS1(inst)],
        REGISTERS[GetRS2(inst)],
        REGISTERS[GetRD(inst)],
        0,
        "div\t%R,%1S,%2S"
    };
}

elf::Inst elf::Divu(const RawInst& inst) {
    return {
        "divu",
        REGISTERS[GetRS1(inst)],
        REGISTERS[GetRS2(inst)],
        REGISTERS[GetRD(inst)],
        0,
        "divu\t%R,%1S,%2S"
    };
}

elf::Inst elf::DiInst(const RawInst& inst) {
    if ((inst & MASK_BEQ) == MATCH_BEQ) {
        return Beq(inst, 0);
    }
    if ((inst & MASK_BNE) == MATCH_BNE) {
        return Bne(inst, 0);
    }
    if ((inst & MASK_BLT) == MATCH_BLT) {
        return Blt(inst, 0);
    }
    if ((inst & MASK_BGE) == MATCH_BGE) {
        return Bge(inst, 0);
    }
    if ((inst & MASK_BLTU) == MATCH_BLTU) {
        return Bltu(inst, 0);
    }
    if ((inst & MASK_BGEU) == MATCH_BGEU) {
        return Bgeu(inst, 0);
    }
    if ((inst & MASK_JALR) == MATCH_JALR) {
        return Jalr(inst);
    }
    if ((inst & MASK_JAL) == MATCH_JAL) {
        return Jal(inst, 0);
    }
    if ((inst & MASK_LUI) == MATCH_LUI) {
        return Lui(inst);
    }
    if ((inst & MASK_AUIPC) == MATCH_AUIPC) {
        return Auipc(inst);
    }
    if ((inst & MASK_ADDI) == MATCH_ADDI) {
        return Addi(inst);
    }
    if ((inst & MASK_SLLI) == MATCH_SLLI) {
        return Slli(inst);
    }
    if ((inst & MASK_SLTI) == MATCH_SLTI) {
        return Slti(inst);
    }
    if ((inst & MASK_SLTIU) == MATCH_SLTIU) {
        return Sltu(inst);
    }
    if ((inst & MASK_XORI) == MATCH_XORI) {
        return Xori(inst);
    }
    if ((inst & MASK_SRLI) == MATCH_SRLI) {
        return Srli(inst);
    }
    if ((inst & MASK_SRAI) == MATCH_SRAI) {
        return Srai(inst);
    }
    if ((inst & MASK_ORI) == MATCH_ORI) {
        return Ori(inst);
    }
    if ((inst & MASK_ANDI) == MATCH_ANDI) {
        return Andi(inst);
    }
    if ((inst & MASK_ADD) == MATCH_ADD) {
        return Add(inst);
    }
    if ((inst & MASK_SUB) == MATCH_SUB) {
        return Sub(inst);
    }
    if ((inst & MASK_SLL) == MATCH_SLL) {
        return Sll(inst);
    }
    if ((inst & MASK_SLT) == MATCH_SLT) {
        return Slt(inst);
    }
    if ((inst & MASK_SLTU) == MATCH_SLTU) {
        return Sltu(inst);
    }
    if ((inst & MASK_XOR) == MATCH_XOR) {
        return Xor(inst);
    }
    if ((inst & MASK_SRL) == MATCH_SRL) {
        return Srl(inst);
    }
    if ((inst & MASK_SRA) == MATCH_SRA) {
        return Sra(inst);
    }
    if ((inst & MASK_OR) == MATCH_OR) {
        return Or(inst);
    }
    if ((inst & MASK_AND) == MATCH_AND) {
        return And(inst);
    }
    if ((inst & MASK_ADDIW) == MATCH_ADDIW) {
        return Addi(inst);
    }
    if ((inst & MASK_SLLIW) == MATCH_SLLIW) {
        return Sll(inst);
    }
    if ((inst & MASK_SRLIW) == MATCH_SRLIW) {
        return Srli(inst);
    }
    if ((inst & MASK_SRAIW) == MATCH_SRAIW) {
        return Srai(inst);
    }
    if ((inst & MASK_ADDW) == MATCH_ADDW) {
        return Add(inst);
    }
    if ((inst & MASK_SUBW) == MATCH_SUBW) {
        return Sub(inst);
    }
    if ((inst & MASK_SLLW) == MATCH_SLLW) {
        return Sll(inst);
    }
    if ((inst & MASK_SRLW) == MATCH_SRLW) {
        return Srl(inst);
    }
    if ((inst & MASK_SRAW) == MATCH_SRAW) {
        return Sra(inst);
    }
    if ((inst & MASK_LB) == MATCH_LB) {
        return Lb(inst);
    }
    if ((inst & MASK_LH) == MATCH_LH) {
        return Lh(inst);
    }
    if ((inst & MASK_LW) == MATCH_LW) {
        return Lw(inst);
    }

    /*if ((inst & MASK_LD) == MATCH_LD) {
        return func(inst);
    }*/

    if ((inst & MASK_LBU) == MATCH_LBU) {
        return Lbu(inst);
    }

    if ((inst & MASK_LHU) == MATCH_LHU) {
        return Lhu(inst);
    }

    /*if ((inst & MASK_LWU) == MATCH_LWU) {
        return func(inst);
    }*/

    if ((inst & MASK_SB) == MATCH_SB) {
        return Sb(inst);
    }
    if ((inst & MASK_SH) == MATCH_SH) {
        return Sh(inst);
    }
    if ((inst & MASK_SW) == MATCH_SW) {
        return Sw(inst);
    }

    /*if ((inst & MASK_SD) == MATCH_SD) {
        return func(inst);
    }*/

    if ((inst & MASK_FENCE) == MATCH_FENCE) {
        return Fence(inst);
    }
    if ((inst & MASK_FENCE_I) == MATCH_FENCE_I) {
        return Fencei(inst);
    }
    if ((inst & MASK_ECALL) == MATCH_ECALL) {
        return ECall(inst);
    }
    if ((inst & MASK_EBREAK) == MATCH_EBREAK) {
        return EBreak(inst);
    }

    if ((inst & MASK_MUL) == MATCH_MUL) {
        return Mul(inst);
    }
    if ((inst & MASK_MULH) == MATCH_MULH) {
        return Mulh(inst);
    }
    if ((inst & MASK_MULHSU) == MATCH_MULHSU) {
        return Mulhsu(inst);
    }
    if ((inst & MASK_MULHU) == MATCH_MULHU) {
        return Mulhu(inst);
    }
    if ((inst & MASK_DIV) == MATCH_DIV) {
        return OpDiv(inst);
    }
    if ((inst & MASK_DIVU) == MATCH_DIVU) {
        return Divu(inst);
    }
    if ((inst & MASK_REM) == MATCH_REM) {
        return Rem(inst);
    }
    if ((inst & MASK_REMU) == MATCH_REMU) {
        return Remu(inst);
    }

    throw std::invalid_argument("elf::DiInst error: Passed instruction cannot be recognized.");
}
