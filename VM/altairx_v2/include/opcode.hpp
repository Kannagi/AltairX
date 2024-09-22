#ifndef AXOPCODE_HPP_INCLUDED
#define AXOPCODE_HPP_INCLUDED

#include <cstdint>

struct AxOpcode
{
    std::uint32_t value{};

    // implicit conversion
    AxOpcode(std::uint32_t val)
    : value{val}
    {
    }

    operator std::uint32_t() const noexcept
    {
        return value;
    }

    std::uint32_t opcode() const noexcept
    {
        return (value >> 1) & 0x0Fu;
    }

    std::uint32_t unit() const noexcept
    {
        return (value >> 5) & 0x07u;
    }

    // opcode + unit
    std::uint32_t operation() const noexcept
    {
        return (value >> 1) & 0x7Fu;
    }

    std::uint32_t size() const noexcept
    {
        return (value >> 8) & 0x03u;
    }

    std::uint32_t sext_insize() const noexcept
    {
        return (value >> 11) & 0x03u;
    }

    std::uint32_t bru_other_index() const noexcept
    {
        return (value >> 8) & 0x0Fu;
    }

    std::uint32_t reg_c() const noexcept
    {
        return (value >> 14) & 0x3Fu;
    }

    std::uint32_t reg_b() const noexcept
    {
        return (value >> 20) & 0x3Fu;
    }

    std::uint32_t reg_a() const noexcept
    {
        return (value >> 26) & 0x3Fu;
    }

    bool alu_has_imm() const noexcept
    {
        return (value & 0x0400u) != 0;
    }

    std::uint64_t alu_shift() const noexcept
    {
        return (value >> 11) & 0x07u;
    }

    std::uint64_t alu_imm9() const noexcept
    {
        return (value >> 11) & 0x01FFu;
    }

    std::uint64_t ext_ins_imm1() const noexcept
    {
        return (value >> 8) & 0x3Fu;
    }

    std::uint64_t ext_ins_imm2() const noexcept
    {
        return (value >> 14) & 0x3Fu;
    }

    std::uint64_t alu_move_imm() const noexcept
    {
        return (value >> 8) & 0x0003FFFFu;
    }

    std::uint32_t mdu_pq() const noexcept
    {
        return (value >> 11) & 0x03U;
    }

    std::uint64_t lsu_shift() const noexcept
    {
        return (value >> 11) & 0x07u;
    }

    std::uint64_t lsu_imm10() const noexcept
    {
        return (value >> 10) & 0x03FFu;
    }

    std::uint64_t lsu_imm16() const noexcept
    {
        return (value >> 10) & 0x0FFFFu;
    }

    std::uint64_t moveix_imm24() const noexcept
    {
        return (value >> 8) & 0x00FFFFFFu;
    }

    std::uint64_t bru_imm24() const noexcept
    {
        return (value >> 8) & 0x00FFFFFFu;
    }

    // return true if next opcode is packaged with this one.
    bool is_package() const noexcept
    {
        return (value & 1) != 0;
    }

    bool is_moveix() const noexcept
    {
        return (value & 0xFE) == 0;
    }
};

//-------------------------------
enum AxOpcodes : std::uint32_t
{
    //------------- ALU(0) -----

    AX_EXE_ALU_MOVEIX,
    AX_EXE_ALU_COP,
    AX_EXE_ALU_EXT,
    AX_EXE_ALU_INS,

    AX_EXE_ALU_MOVEI,
    AX_EXE_ALU_MOVEN,
    AX_EXE_ALU_UNK0,
    AX_EXE_ALU_MOVEUP,

    AX_EXE_ALU_UNK1,
    AX_EXE_ALU_SEXT,
    AX_EXE_ALU_RTL,
    AX_EXE_ALU_RTR,

    AX_EXE_ALU_ADC = 0x0A,
    AX_EXE_ALU_SBC,

    AX_EXE_ALU_CMP,
    AX_EXE_ALU_TEST,
    AX_EXE_ALU_TESTFR,
    AX_EXE_ALU_CMPBIT,

    //------------- ALU(1) -----

    AX_EXE_ALU_ADD,
    AX_EXE_ALU_SUB,
    AX_EXE_ALU_XOR,
    AX_EXE_ALU_OR,

    AX_EXE_ALU_AND,
    AX_EXE_ALU_LSL,
    AX_EXE_ALU_ASR,
    AX_EXE_ALU_LSR,

    AX_EXE_ALU_SE,
    AX_EXE_ALU_SEN,
    AX_EXE_ALU_SLTS,
    AX_EXE_ALU_SLTU,

    AX_EXE_ALU_SAND,
    AX_EXE_ALU_SBIT,
    AX_EXE_ALU_CMOVEN,
    AX_EXE_ALU_CMOVE,

    //------------- LSU(2) -------
    AX_EXE_LSU_LD,
    AX_EXE_LSU_ST,
    AX_EXE_LSU_FLD,
    AX_EXE_LSU_FST,

    AX_EXE_LSU_LDI,
    AX_EXE_LSU_STI,
    AX_EXE_LSU_FLDI,
    AX_EXE_LSU_FSTI,

    AX_EXE_LSU_LDSP,
    AX_EXE_LSU_STSP,
    AX_EXE_LSU_FLDSP,
    AX_EXE_LSU_FSTSP,

    AX_EXE_LSU_LDC,
    AX_EXE_LSU_STC,
    AX_EXE_LSU_FLDC,
    AX_EXE_LSU_FSTC,

    //------------- FPU(3) -----
    AX_EXE_FPU_FADD,
    AX_EXE_FPU_FSUB,
    AX_EXE_FPU_FMUL,
    AX_EXE_FPU_FNMUL,

    AX_EXE_FPU_HTOF,
    AX_EXE_FPU_FTOH,
    AX_EXE_FPU_ITOF,
    AX_EXE_FPU_FTOI,

    AX_EXE_FPU_FNEG,
    AX_EXE_FPU_FABS,
    AX_EXE_FPU_FMIN,
    AX_EXE_FPU_FMAX,

    AX_EXE_FPU_FMOVE,
    AX_EXE_FPU_FCMP,
    AX_EXE_FPU_FMOVEI,
    AX_EXE_FPU_FCMPI,

    //------------- ALU3(4) -----
    AX_EXE_ALU_ADD3,
    AX_EXE_ALU_SUB3,
    AX_EXE_ALU_XOR3,
    AX_EXE_ALU_OR3,

    AX_EXE_ALU_AND3,
    AX_EXE_ALU_LSL3,
    AX_EXE_ALU_ASR3,
    AX_EXE_ALU_LSR3,

    AX_EXE_ALU_SE3,
    AX_EXE_ALU_SEn3,
    AX_EXE_ALU_SLTS3,
    AX_EXE_ALU_SLTU3,

    AX_EXE_ALU_SAND3,
    AX_EXE_ALU_UNK33,
    AX_EXE_ALU_CMOVEN3,
    AX_EXE_ALU_CMOVE3,

    //------------- EFU(5) -----
    AX_EXE_EFU_FDIV,
    AX_EXE_EFU_FATAN2,
    AX_EXE_EFU_DDIV,
    AX_EXE_EFU_DATAN2,

    AX_EXE_EFU_FSQRT,
    AX_EXE_EFU_FSIN,
    AX_EXE_EFU_FATAN,
    AX_EXE_EFU_FEXP,

    AX_EXE_EFU_DSQRT,
    AX_EXE_EFU_DSIN,
    AX_EXE_EFU_DATAN,
    AX_EXE_EFU_DEXP,

    AX_EXE_EFU_FPOW,
    AX_EXE_EFU_DPOW,
    AX_EXE_EFU_EMOVETO,
    AX_EXE_EFU_EMOVEFROM,

    //------------- MDU(6) -----
    AX_EXE_MDU_DIV,
    AX_EXE_MDU_DIVU,
    AX_EXE_MDU_MUL,
    AX_EXE_MDU_MULU,

    AX_EXE_MDU_MOVETO,
    AX_EXE_MDU_MOVEFROM,
    AX_EXE_MDU_UNK0,
    AX_EXE_MDU_UNK1,

    AX_EXE_MDU_UNK2,
    AX_EXE_MDU_UNK3,
    AX_EXE_MDU_UNK4,
    AX_EXE_MDU_UNK5,

    AX_EXE_MDU_UNK6,
    AX_EXE_MDU_UNK7,
    AX_EXE_MDU_UNK8,
    AX_EXE_MDU_UNK9,

    //------------- BRU(7) -----
    AX_EXE_BRU_BNE,
    AX_EXE_BRU_BEQ,
    AX_EXE_BRU_BL,
    AX_EXE_BRU_BLE,

    AX_EXE_BRU_BG,
    AX_EXE_BRU_BGE,
    AX_EXE_BRU_BLS,
    AX_EXE_BRU_BLES,

    AX_EXE_BRU_BGS,
    AX_EXE_BRU_BGES,
    AX_EXE_BRU_BRA,
    AX_EXE_BRU_LOOP,

    AX_EXE_BRU_JUMP,
    AX_EXE_BRU_CALL,
    AX_EXE_BRU_CALLR,
    AX_EXE_BRU_OTHER = 0x7F,
    AX_EXE_BRU_JUMPBR = 0x7F,
    AX_EXE_BRU_CALLBR = 0x7F,
    AX_EXE_BRU_RET = 0x7F,
    AX_EXE_BRU_RETI = 0x7F,
    AX_EXE_BRU_SYSCALL = 0x7F,

    AX_EXE_BRU_OTHER_JUMPBR_ID = 0x01,
    AX_EXE_BRU_OTHER_CALLBR_ID = 0x09,
    AX_EXE_BRU_OTHER_RET_ID = 0x02,
    AX_EXE_BRU_OTHER_RETI_ID = 0x03,
    AX_EXE_BRU_OTHER_SYSCALL_ID = 0x04,

    //------------- CU(5) -----
    AX_EXE_CU_MOVETO = 0x50,
    AX_EXE_CU_MOVEFROM,
    AX_EXE_CU_SYNC,
    AX_EXE_CU_MMU,

    //------------- VU(5) -----
    AX_EXE_VU_VECTOR2 = 0x60,
    AX_EXE_VU_VECTOR4,
    AX_EXE_VU_VECTOR8,
    AX_EXE_VU_INV,

    AX_EXE_VU_VECTOR2X2,
    AX_EXE_VU_VECTOR4X2,
    AX_EXE_VU_VECTOR8X2,
    AX_EXE_VU_INVX2,

    AX_EX_FPU_FADD = 0x00,
    AX_EX_FPU_FSUB,
    AX_EX_FPU_FMUL,
    AX_EX_FPU_FNMUL,

    AX_EX_FPU_HTOF,
    AX_EX_FPU_FTOH,
    AX_EX_FPU_ITOF,
    AX_EX_FPU_FTOI,

    AX_EX_FPU_FNEG,
    AX_EX_FPU_FABS,
    AX_EX_FPU_FMIN,
    AX_EX_FPU_FMAX,

    AX_EX_FPU_FMOVE,
    AX_EX_FPU_FCMP,
    AX_EX_FPU_FMOVEI,
    AX_EX_FPU_FCMPI,
};
//-------------

#endif // !AXOPCODE_HPP_INCLUDED
