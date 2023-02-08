#pragma once

#include <array>
#include <bitset>

namespace elf {
    constexpr uint32_t MATCH_BEQ = 0x63;
    constexpr uint32_t MASK_BEQ = 0x707f;
    constexpr uint32_t MATCH_BNE = 0x1063;
    constexpr uint32_t MASK_BNE = 0x707f;
    constexpr uint32_t MATCH_BLT = 0x4063;
    constexpr uint32_t MASK_BLT = 0x707f;
    constexpr uint32_t MATCH_BGE = 0x5063;
    constexpr uint32_t MASK_BGE = 0x707f;
    constexpr uint32_t MATCH_BLTU = 0x6063;
    constexpr uint32_t MASK_BLTU = 0x707f;
    constexpr uint32_t MATCH_BGEU = 0x7063;
    constexpr uint32_t MASK_BGEU = 0x707f;
    constexpr uint32_t MATCH_JALR = 0x67;
    constexpr uint32_t MASK_JALR = 0x707f;
    constexpr uint32_t MATCH_JAL = 0x6f;
    constexpr uint32_t MASK_JAL = 0x7f;
    constexpr uint32_t MATCH_LUI = 0x37;
    constexpr uint32_t MASK_LUI = 0x7f;
    constexpr uint32_t MATCH_AUIPC = 0x17;
    constexpr uint32_t MASK_AUIPC = 0x7f;
    constexpr uint32_t MATCH_ADDI = 0x13;
    constexpr uint32_t MASK_ADDI = 0x707f;
    constexpr uint32_t MATCH_SLLI = 0x1013;
    constexpr uint32_t MASK_SLLI = 0xfc00707f;
    constexpr uint32_t MATCH_SLTI = 0x2013;
    constexpr uint32_t MASK_SLTI = 0x707f;
    constexpr uint32_t MATCH_SLTIU = 0x3013;
    constexpr uint32_t MASK_SLTIU = 0x707f;
    constexpr uint32_t MATCH_XORI = 0x4013;
    constexpr uint32_t MASK_XORI = 0x707f;
    constexpr uint32_t MATCH_SRLI = 0x5013;
    constexpr uint32_t MASK_SRLI = 0xfc00707f;
    constexpr uint32_t MATCH_SRAI = 0x40005013;
    constexpr uint32_t MASK_SRAI = 0xfc00707f;
    constexpr uint32_t MATCH_ORI = 0x6013;
    constexpr uint32_t MASK_ORI = 0x707f;
    constexpr uint32_t MATCH_ANDI = 0x7013;
    constexpr uint32_t MASK_ANDI = 0x707f;
    constexpr uint32_t MATCH_ADD = 0x33;
    constexpr uint32_t MASK_ADD = 0xfe00707f;
    constexpr uint32_t MATCH_SUB = 0x40000033;
    constexpr uint32_t MASK_SUB = 0xfe00707f;
    constexpr uint32_t MATCH_SLL = 0x1033;
    constexpr uint32_t MASK_SLL = 0xfe00707f;
    constexpr uint32_t MATCH_SLT = 0x2033;
    constexpr uint32_t MASK_SLT = 0xfe00707f;
    constexpr uint32_t MATCH_SLTU = 0x3033;
    constexpr uint32_t MASK_SLTU = 0xfe00707f;
    constexpr uint32_t MATCH_XOR = 0x4033;
    constexpr uint32_t MASK_XOR = 0xfe00707f;
    constexpr uint32_t MATCH_SRL = 0x5033;
    constexpr uint32_t MASK_SRL = 0xfe00707f;
    constexpr uint32_t MATCH_SRA = 0x40005033;
    constexpr uint32_t MASK_SRA = 0xfe00707f;
    constexpr uint32_t MATCH_OR = 0x6033;
    constexpr uint32_t MASK_OR = 0xfe00707f;
    constexpr uint32_t MATCH_AND = 0x7033;
    constexpr uint32_t MASK_AND = 0xfe00707f;
    constexpr uint32_t MATCH_ADDIW = 0x1b;
    constexpr uint32_t MASK_ADDIW = 0x707f;
    constexpr uint32_t MATCH_SLLIW = 0x101b;
    constexpr uint32_t MASK_SLLIW = 0xfe00707f;
    constexpr uint32_t MATCH_SRLIW = 0x501b;
    constexpr uint32_t MASK_SRLIW = 0xfe00707f;
    constexpr uint32_t MATCH_SRAIW = 0x4000501b;
    constexpr uint32_t MASK_SRAIW = 0xfe00707f;
    constexpr uint32_t MATCH_ADDW = 0x3b;
    constexpr uint32_t MASK_ADDW = 0xfe00707f;
    constexpr uint32_t MATCH_SUBW = 0x4000003b;
    constexpr uint32_t MASK_SUBW = 0xfe00707f;
    constexpr uint32_t MATCH_SLLW = 0x103b;
    constexpr uint32_t MASK_SLLW = 0xfe00707f;
    constexpr uint32_t MATCH_SRLW = 0x503b;
    constexpr uint32_t MASK_SRLW = 0xfe00707f;
    constexpr uint32_t MATCH_SRAW = 0x4000503b;
    constexpr uint32_t MASK_SRAW = 0xfe00707f;
    constexpr uint32_t MATCH_LB = 0x3;
    constexpr uint32_t MASK_LB = 0x707f;
    constexpr uint32_t MATCH_LH = 0x1003;
    constexpr uint32_t MASK_LH = 0x707f;
    constexpr uint32_t MATCH_LW = 0x2003;
    constexpr uint32_t MASK_LW = 0x707f;
    constexpr uint32_t MATCH_LD = 0x3003;
    constexpr uint32_t MASK_LD = 0x707f;
    constexpr uint32_t MATCH_LBU = 0x4003;
    constexpr uint32_t MASK_LBU = 0x707f;
    constexpr uint32_t MATCH_LHU = 0x5003;
    constexpr uint32_t MASK_LHU = 0x707f;
    constexpr uint32_t MATCH_LWU = 0x6003;
    constexpr uint32_t MASK_LWU = 0x707f;
    constexpr uint32_t MATCH_SB = 0x23;
    constexpr uint32_t MASK_SB = 0x707f;
    constexpr uint32_t MATCH_SH = 0x1023;
    constexpr uint32_t MASK_SH = 0x707f;
    constexpr uint32_t MATCH_SW = 0x2023;
    constexpr uint32_t MASK_SW = 0x707f;
    constexpr uint32_t MATCH_SD = 0x3023;
    constexpr uint32_t MASK_SD = 0x707f;
    constexpr uint32_t MATCH_FENCE = 0xf;
    constexpr uint32_t MASK_FENCE = 0x707f;
    constexpr uint32_t MATCH_FENCE_I = 0x100f;
    constexpr uint32_t MASK_FENCE_I = 0x707f;
    constexpr uint32_t MATCH_ECALL = 0x73;
    constexpr uint32_t MASK_ECALL = 0xffffffff;
    constexpr uint32_t MATCH_EBREAK = 0x100073;
    constexpr uint32_t MASK_EBREAK = 0xffffffff;

    constexpr uint32_t MATCH_MUL = 0x2000033;
    constexpr uint32_t MASK_MUL = 0xfe00707f;
    constexpr uint32_t MATCH_MULH = 0x2001033;
    constexpr uint32_t MASK_MULH = 0xfe00707f;
    constexpr uint32_t MATCH_MULHSU = 0x2002033;
    constexpr uint32_t MASK_MULHSU = 0xfe00707f;
    constexpr uint32_t MATCH_MULHU = 0x2003033;
    constexpr uint32_t MASK_MULHU = 0xfe00707f;
    constexpr uint32_t MATCH_DIV = 0x2004033;
    constexpr uint32_t MASK_DIV = 0xfe00707f;
    constexpr uint32_t MATCH_DIVU = 0x2005033;
    constexpr uint32_t MASK_DIVU = 0xfe00707f;
    constexpr uint32_t MATCH_REM = 0x2006033;
    constexpr uint32_t MASK_REM = 0xfe00707f;
    constexpr uint32_t MATCH_REMU = 0x2007033;
    constexpr uint32_t MASK_REMU = 0xfe00707f;

    const std::array<std::string, 32> REGISTERS{
        "zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2", "s0", "s1", "a0",
        "a1", "a2", "a3", "a4", "a5", "a6", "a7", "s2", "s3", "s4", "s5",
        "s6", "s7", "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
    };

    using RawInst = uint32_t;

    struct Inst {
        std::string name;
        std::string rs1;
        std::string rs2;
        std::string rd;
        int32_t imm;
        std::string fmt;
    };

    uint32_t GetCsrImm(const RawInst& inst);
    uint32_t GetRD(const RawInst& inst);
    uint32_t GetRS1(const RawInst& inst);
    uint32_t GetRS2(const RawInst& inst);

    inline uint32_t Bextr(
        uint32_t src, uint32_t start,
        uint32_t len
    );

    inline int32_t Shamt(uint32_t value);

    inline uint32_t ImmSign(uint32_t value);

    inline int32_t GetBImm(uint32_t value);

    inline uint32_t GetIImmUnsigned(uint32_t value);

    inline int32_t GetIImm(uint32_t value);

    inline int32_t GetSImm(uint32_t value);

    inline int32_t GetJImm(uint32_t value);

    inline Inst Add(const RawInst& inst);

    inline Inst Addi(const RawInst& inst);

    inline Inst And(const RawInst& inst);

    inline Inst Andi(const RawInst& inst);

    struct InstructionU {
        int opcode : 7;
        int rd : 5;
        int data : 20;
    };

    inline Inst Auipc(const RawInst& inst);

    inline Inst Beq(const RawInst& inst, int32_t pc);

    inline Inst Bge(const RawInst& inst, int32_t pc);

    inline Inst Bgeu(const RawInst& inst, int32_t pc);

    inline Inst Blt(const RawInst& inst, int32_t pc);

    inline Inst Bltu(const RawInst& inst, int32_t pc);

    inline Inst Bne(const RawInst& inst, int32_t pc);

    inline Inst Fence(const RawInst& inst);

    inline Inst Fencei(const RawInst& inst);

    inline Inst Jal(const RawInst& inst, int32_t pc);

    inline Inst Jalr(const RawInst& inst);

    inline Inst Lb(const RawInst& inst);

    inline Inst Lbu(const RawInst& inst);

    inline Inst Lh(const RawInst& inst);

    inline Inst Lhu(const RawInst& inst);

    inline Inst Lui(const RawInst& inst);

    inline Inst Lw(const RawInst& inst);

    inline Inst Or(const RawInst& inst);

    struct InstructionI {
        int opcode : 7;
        int rd : 5;
        int func : 3;
        int rs1 : 5;
        int imm : 12;
    };

    inline Inst Ori(const RawInst& inst);

    inline Inst Sb(const RawInst& inst);

    inline Inst Sh(const RawInst& inst);

    inline Inst Sll(const RawInst& inst);

    struct InstructionIShift {
        int opcode : 7;
        int rd : 5;
        int func : 3;
        int rs1 : 5;
        int shamt : 5;
        int imm : 7;
    };

    inline Inst Slli(const RawInst& inst);

    inline Inst Slt(const RawInst& inst);

    inline Inst Sltu(const RawInst& inst);

    inline Inst Slti(const RawInst& inst);

    inline Inst Sltiu(const RawInst& inst);

    inline Inst Sra(const RawInst& inst);

    inline Inst Srai(const RawInst& inst);

    inline Inst Srl(const RawInst& inst);

    inline Inst Srli(const RawInst& inst);

    inline Inst Sub(const RawInst& inst);

    inline Inst Sw(const RawInst& inst);

    inline Inst Xor(const RawInst& inst);

    inline Inst Xori(const RawInst& inst);

    inline Inst Mret(const RawInst& inst);

    inline Inst Sret(const RawInst& inst);

    inline Inst Uret(const RawInst& inst);

    inline Inst SfenceVma(const RawInst& inst);

    inline Inst Wfi(const RawInst& inst);

    inline Inst ECall(const RawInst& inst);

    inline Inst EBreak(const RawInst& inst);

    inline Inst Mul(const RawInst& inst);

    inline Inst Mulh(const RawInst& inst);

    inline Inst Mulhu(const RawInst& inst);

    inline Inst Mulhsu(const RawInst& inst);

    inline Inst Rem(const RawInst& inst);

    inline Inst Remu(const RawInst& inst);

    inline Inst OpDiv(const RawInst& inst);

    inline Inst Divu(const RawInst& inst);

    Inst DiInst(const RawInst& inst);
} // namespace elf
