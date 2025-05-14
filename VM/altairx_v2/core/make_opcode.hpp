#ifndef AXMAKEOPCODE_HPP_INCLUDED
#define AXMAKEOPCODE_HPP_INCLUDED

#include "opcode.hpp"
#include "panic.hpp"
#include "utilities.hpp"

#include <array>

static constexpr uint32_t ax_no_reg = 0x3F;

inline std::array<AxOpcode, 2> make_bundle(AxOpcode first, AxOpcode second)
{
    return {first | 1, second};
}

inline AxOpcode make_noop_opcode()
{
    return 0u;
}

inline AxOpcode make_simple_opcode(uint32_t op)
{
    const AxOpcode output = 0u | (op << 1);

    ax_check(output.operation() == AX_EXE_ALU_MOVEI, "Invalid op encoding");

    return output;
}

inline AxOpcode make_movei_opcode(uint32_t ra, int64_t imm)
{
    ax_check(is_int<42>(imm), "movei out of range");

    const auto uimm = static_cast<uint64_t>(imm);
    const auto lowimm = (uimm & 0x0001FFFF) | ((uimm >> 41 & 0x01u) << 17);
    const AxOpcode output = 0u | AX_EXE_ALU_MOVEI << 1 | lowimm << 8 | (ra << 26);

    ax_check(output.operation() == AX_EXE_ALU_MOVEI, "Invalid op encoding");
    ax_check(output.alu_move_imm() == lowimm, "Invalid imm encoding");
    ax_check(output.reg_a() == ra, "Invalid ra encoding");

    return output;
}

inline AxOpcode make_alu_reg_reg_opcode(uint32_t op, uint32_t size, uint32_t ra, uint32_t rb, uint32_t rc, uint32_t shift)
{
    ax_check(is_uint<3>(shift), "shift out of range");

    const AxOpcode output = 0u | (op << 1) | (size << 8) | (shift << 11) | (rc << 14) | (rb << 20) | (ra << 26);

    ax_check(output.operation() == op, "Invalid op encoding");
    ax_check(output.size() == size, "Invalid size encoding");
    ax_check(output.reg_c() == rc, "Invalid rc encoding");
    ax_check(output.reg_b() == rb, "Invalid rb encoding");
    ax_check(output.reg_a() == ra, "Invalid ra encoding");
    ax_check(output.alu_shift() == shift, "Invalid alu shift encoding");

    return output;
}

inline AxOpcode make_alu_reg_imm_opcode(uint32_t op, uint32_t size, uint32_t ra, uint32_t rb, int32_t imm)
{
    ax_check(is_int<32>(imm), "alu imm out of range");

    const auto uimm = static_cast<uint32_t>(imm);
    const auto lowimm = (uimm & 0xFF) | ((uimm >> 31 & 0x01u) << 8);
    const AxOpcode output = 0u | (op << 1) | (size << 8) | (1u << 10) | (lowimm << 11) | (rb << 20) | (ra << 26);

    ax_check(output.operation() == op, "Invalid op encoding");
    ax_check(output.size() == size, "Invalid size encoding");
    ax_check(output.alu_has_imm(), "Invalid imm encoding");
    ax_check(output.alu_imm9() == lowimm, "Invalid imm encoding");
    ax_check(output.reg_b() == rb, "Invalid rb encoding");
    ax_check(output.reg_a() == ra, "Invalid ra encoding");

    return output;
}

inline AxOpcode make_alu_reg_imm_moveix(int32_t imm)
{
    ax_check(is_int<32>(imm), "alu imm out of range");

    const auto hiimm = (static_cast<uint32_t>(imm) >> 8 & 0x00FFFFFFu) ^ 0x00FFFFFFu;
    const AxOpcode output = 0u | hiimm << 8;

    ax_check(output.is_moveix(), "Invalid moveix encoding");
    ax_check(output.moveix_imm24() == hiimm, "Invalid imm encoding");

    return output;
}

inline AxOpcode make_lsu_reg_reg_opcode(uint32_t op, uint32_t size, uint32_t ra, uint32_t rb, uint32_t rc, uint32_t shift)
{
    return make_alu_reg_reg_opcode(op, size, ra, rb, rc, shift); // same encoding
}

inline AxOpcode make_lsu_reg_imm_opcode(uint32_t op, uint32_t size, uint32_t ra, uint32_t rb, int64_t imm)
{
    ax_check(is_int<33>(imm), "alu imm out of range");

    const auto uimm = static_cast<uint64_t>(imm);
    const auto lowimm = (uimm & 0x01FFu) | ((uimm >> 32 & 0x01u) << 9);
    const AxOpcode output = 0u | (op << 1) | (size << 8) | (lowimm << 10) | (rb << 20) | (ra << 26);

    ax_check(output.operation() == op, "Invalid op encoding");
    ax_check(output.size() == size, "Invalid size encoding");
    ax_check(output.lsu_imm10() == lowimm, "Invalid imm encoding");
    ax_check(output.reg_b() == rb, "Invalid rb encoding");
    ax_check(output.reg_a() == ra, "Invalid ra encoding");

    return output;
}

inline AxOpcode make_lsu_reg_imm_moveix(int64_t imm)
{
    ax_check(is_int<33>(imm), "lsu imm out of range");

    const auto hiimm = (static_cast<uint64_t>(imm) >> 9 & 0x00FFFFFFu) ^ 0x00FFFFFFu;
    const AxOpcode output = hiimm << 8;

    ax_check(output.is_moveix(), "Invalid moveix encoding");
    ax_check(output.moveix_imm24() == hiimm, "Invalid imm encoding");

    return output;
}
inline AxOpcode make_mdu_reg_reg_opcode(uint32_t op, uint32_t size, uint32_t rb, uint32_t rc, uint32_t shift)
{
    return make_alu_reg_reg_opcode(op, size, ax_no_reg, rb, rc, shift); // same encoding
}

inline AxOpcode make_mdu_reg_imm_opcode(uint32_t op, uint32_t size, uint32_t rb, int32_t imm)
{
    return make_alu_reg_imm_opcode(op, size, ax_no_reg, rb, imm); // same encoding
}

inline AxOpcode make_bru_brc_opcode(uint32_t op, int32_t imm)
{
    ax_check(is_int<32>(imm), "bru imm out of range");

    const auto uimm = static_cast<uint32_t>(imm);
    const auto lowimm = (uimm & 0x007FFFFFu) | ((uimm >> 31 & 0x01u) << 9);
    const AxOpcode output = 0u | (op << 1) | (lowimm << 9);

    ax_check(output.operation() == op, "Invalid op encoding");
    ax_check(output.bru_imm23() == lowimm, "Invalid imm encoding");

    return output;
}

inline AxOpcode make_bru_bra_opcode(uint32_t op, int32_t imm)
{
    ax_check(is_int<32>(imm), "bru imm out of range");

    const auto uimm = static_cast<uint32_t>(imm);
    const auto lowimm = (uimm & 0x00FFFFFFu) | ((uimm >> 31 & 0x01u) << 8);
    const AxOpcode output = 0u | (op << 1) | (lowimm << 8);

    ax_check(output.operation() == op, "Invalid op encoding");
    ax_check(output.bru_imm24() == lowimm, "Invalid imm encoding");

    return output;
}

inline AxOpcode make_bru_jump_opcode(uint32_t op, uint32_t imm)
{
    ax_check(is_uint<32>(imm), "bru imm out of range");

    const auto lowimm = (imm & 0x00FFFFFFu);
    const AxOpcode output = 0u | (op << 1) | (lowimm << 8);

    ax_check(output.operation() == op, "Invalid op encoding");
    ax_check(output.bru_imm24() == lowimm, "Invalid imm encoding");

    return output;
}

inline AxOpcode make_bru_indirect_opcode(uint32_t op, uint32_t ra, uint32_t rb)
{
    return make_alu_reg_reg_opcode(op, 0, ra, rb, ax_no_reg, 0); // same encoding
}

inline AxOpcode make_fpu_reg_reg_opcode(uint32_t op, uint32_t size, uint32_t ra, uint32_t rb, uint32_t rc)
{
  const AxOpcode output = 0u | (op << 1) | (size << 8) | (rc << 14) | (rb << 20) | (ra << 26);

  ax_check(output.operation() == op, "Invalid op encoding");
  ax_check(output.size() == size, "Invalid size encoding");
  ax_check(output.reg_c() == rc, "Invalid rc encoding");
  ax_check(output.reg_b() == rb, "Invalid rb encoding");
  ax_check(output.reg_a() == ra, "Invalid ra encoding");

  return output;
}

#endif
