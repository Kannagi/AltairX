#include <core.hpp>

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <vector>

#include <memory.hpp>
#include <opcode.hpp>
#include <utilities.hpp>

namespace
{

uint64_t sext(uint64_t val, uint64_t from) noexcept
{
    const auto bitsize = (8ull << from);
    const auto mask = 1ull << (bitsize - 1ull);
    return (val ^ mask) - mask;
}

}

AxCore::AxCore(AxMemory& memory)
: m_memory{&memory}
{
}

void AxCore::do_store(const void* src, uint64_t addr, uint32_t size)
{
    m_memory->store(*this, src, addr, size);
}

void AxCore::do_load(void* dest, uint64_t addr, uint32_t size)
{
    m_memory->load(*this, dest, addr, size);
}

std::uint32_t AxCore::execute(AxOpcode first, AxOpcode second)
{
    // get moveix imm24 value if present
    const uint64_t imm24 = first.is_package() && second.is_moveix() ? second.moveix_imm24() : 0ull;
    execute_unit(first, 0, imm24);

    // execute second instruction not
    if(first.is_package() && !second.is_moveix())
    {
        execute_unit(second, 1, imm24);
        return 2;
    }

    return 1;
}

/*
UNIT ID |    UNIT NAME    |     Issue ID
        | INST 1 | INST 2 | INST 1 | INST 2
   0    |  ALU1  |  ALU2  |   0    |   8
   1    |  ALU1  |  ALU2  |   1    |   9
   2    |  LSU1  |  LSU2  |   2    |   10
   3    |  FPU1  |  FPU2  |   3    |   11
   4    |  /     |   /    |   /    |   /
   5    |  EFU   |   CU   |   5    |   13
   6    |  MDU   |   VU   |   6    |   14
   7    |  BRU   |   /    |   7    |   /
*/
void AxCore::execute_unit(AxOpcode opcode, uint32_t slot, uint64_t imm24)
{
    // reset regs before doing anything
    m_regs.gpi[REG_ZERO] = 0;
    m_regs.gpf[REG_ZERO] = 0.0;

    const auto issue = (slot << 3) | opcode.unit();
    switch(issue)
    {
    case 0:
        [[fallthrough]];
    case 1:
        [[fallthrough]];
    case 8:
        [[fallthrough]];
    case 9:
        execute_alu(opcode, imm24);
        break;
    case 2:
        [[fallthrough]];
    case 10:
        execute_lsu(opcode, imm24);
        break;
    case 3:
        [[fallthrough]];
    case 11:
        execute_fpu(opcode, imm24);
        break;
    case 5:
        execute_efu(opcode, imm24);
        break;
    case 6:
        execute_mdu(opcode, imm24);
        break;
    case 7:
        execute_bru(opcode, imm24);
        break;
    case 13:
        execute_cu(opcode, imm24);
        break;
    case 14:
        execute_vu(opcode, imm24);
        break;
    default:
        assert(false && "Wrong issue ID");
        break;
    }
}

uint64_t AxCore::alu_src2_value(AxOpcode op, Register reg, uint64_t imm24) const noexcept
{
    if(!op.alu_has_imm())
    {
        return m_regs.gpi[reg] << op.alu_shift();
    }

    uint64_t tmp = op.alu_imm9();
    if(tmp & 0x100) // sext
    {
        tmp |= 0xFFFFFFFFFFFFFF00ull;
    }

    // apply imm24
    return tmp ^ (imm24 << 8);
}

namespace
{

// same as alu_src2_value but for lsu, call this only with the imm opcode!
uint64_t lsu_imm_value(AxOpcode op, uint64_t imm24) noexcept
{
    auto base = op.lsu_imm10();
    if(base & 0x200) // sign extend
    {
        base |= 0xFFFFFFFFFFFFFC00ull;
    }

    return base ^ (imm24 << 9);
}

}

void AxCore::execute_alu(AxOpcode op, uint64_t imm24)
{
    const auto dst = op.reg_a();
    const auto src1 = op.reg_b();
    const auto src2 = op.reg_c();

    switch(op.operation())
    {
    // ALU-A (000)
    case AX_EXE_ALU_MOVEIX: // no-op
        break;

    case AX_EXE_ALU_EXT:
        m_regs.gpi[dst] = (m_regs.gpi[src1] >> op.ext_ins_imm1()) & ((1 << op.ext_ins_imm2()) - 1);
        break;

    case AX_EXE_ALU_INS:
        m_regs.gpi[dst] |= (m_regs.gpi[src1] << op.ext_ins_imm1()) & ((1 << op.ext_ins_imm2()) - 1);
        break;

    case AX_EXE_ALU_MOVEI:
        m_regs.gpi[dst] = op.alu_move_imm() | (imm24 << 18);
        break;

    case AX_EXE_ALU_MOVEN:
        m_regs.gpi[dst] = (op.alu_move_imm() | 0xFFFFFFFFFFFC0000) ^ (imm24 << 18);
        break;

    case AX_EXE_ALU_MOVEUP:
        m_regs.gpi[dst] = (op.alu_move_imm() << 32) | (imm24 << 42);
        break;

    case AX_EXE_ALU_SEXT:
        m_regs.gpi[dst] = sext(m_regs.gpi[src1], op.sext_insize()) & sizemask[op.size()];
        break;

    case AX_EXE_ALU_CMP:
    {
        const auto tmp = m_regs.gpi[src1] - m_regs.gpi[src2];
        if(tmp == 0)
        {
            m_regs.fr |= Z_MASK;
        }
        else
        {
            m_regs.fr &= 0xFFFFFFFFu - Z_MASK;
        }

        if(tmp & 0x8000'0000'0000'0000ull)
        {
            m_regs.fr |= N_MASK;
        }
        else
        {
            m_regs.fr &= 0xFFFFFFFFu - N_MASK;
        }
    }

    break;
    case AX_EXE_ALU_TEST:
        assert(false && "AX_EXE_ALU_TEST");
        break;
    case AX_EXE_ALU_TESTFR:
        assert(false && "AX_EXE_ALU_TESTFR");
        break;

    case AX_EXE_ALU_CMPBIT:
        assert(false && "AX_EXE_ALU_CMPBIT");
        break;

    // ALU-B (100)
    case AX_EXE_ALU_ADD:
        m_regs.gpi[dst] = (m_regs.gpi[src1] + alu_src2_value(op, src2, imm24)) & sizemask[op.size()];
        break;

    case AX_EXE_ALU_SUB:
        m_regs.gpi[dst] = (m_regs.gpi[src1] - alu_src2_value(op, src2, imm24)) & sizemask[op.size()];
        break;

    case AX_EXE_ALU_XOR:
        m_regs.gpi[dst] = (m_regs.gpi[src1] ^ alu_src2_value(op, src2, imm24)) & sizemask[op.size()];
        break;

    case AX_EXE_ALU_OR:
        m_regs.gpi[dst] = (m_regs.gpi[src1] | alu_src2_value(op, src2, imm24)) & sizemask[op.size()];
        break;

    case AX_EXE_ALU_AND:
        m_regs.gpi[dst] = (m_regs.gpi[src1] & alu_src2_value(op, src2, imm24)) & sizemask[op.size()];
        break;

    case AX_EXE_ALU_LSL:
        m_regs.gpi[dst] = (m_regs.gpi[src1] << alu_src2_value(op, src2, imm24)) & sizemask[op.size()];
        break;

    case AX_EXE_ALU_ASR:
        m_regs.gpi[dst] = static_cast<int64_t>(m_regs.gpi[src1] >> static_cast<int64_t>(alu_src2_value(op, src2, imm24))) & sizemask[op.size()];
        break;

    case AX_EXE_ALU_LSR:
        m_regs.gpi[dst] = (m_regs.gpi[src1] >> alu_src2_value(op, src2, imm24)) & sizemask[op.size()];
        break;

    case AX_EXE_ALU_SE:
        m_regs.gpi[dst] = static_cast<uint64_t>(m_regs.gpi[src1] == alu_src2_value(op, src2, imm24));
        break;

    case AX_EXE_ALU_SEN:
        m_regs.gpi[dst] = static_cast<uint64_t>(m_regs.gpi[src1] != alu_src2_value(op, src2, imm24));
        break;

    case AX_EXE_ALU_SLTS:
        m_regs.gpi[dst] = static_cast<uint64_t>(static_cast<int64_t>(m_regs.gpi[src1]) < static_cast<int64_t>(alu_src2_value(op, src2, imm24)));
        break;

    case AX_EXE_ALU_SLTU:
        m_regs.gpi[dst] = static_cast<uint64_t>(m_regs.gpi[src1] < alu_src2_value(op, src2, imm24));
        break;

    case AX_EXE_ALU_SAND:
        m_regs.gpi[dst] = static_cast<uint64_t>((m_regs.gpi[src1] & alu_src2_value(op, src2, imm24)));
        break;

    case AX_EXE_ALU_CMOVE:
        if(alu_src2_value(op, src2, imm24) != 0)
        {
            m_regs.gpi[dst] = m_regs.gpi[src1] & sizemask[op.size()];
        }
        break;

    case AX_EXE_ALU_CMOVEN:
        if(alu_src2_value(op, src2, imm24) == 0)
        {
            m_regs.gpi[dst] = m_regs.gpi[src1] & sizemask[op.size()];
        }
        break;
    default:
        assert(false && "Wrong operation");
    }
}

void AxCore::execute_mdu(AxOpcode op, uint64_t imm24)
{
    const auto dst = op.reg_a();
    const auto src1 = op.reg_b();
    const auto src2 = op.reg_c();

    switch(op.operation())
    {
    case AX_EXE_MDU_DIV:
        m_regs.mdu[0] = (m_regs.gpi[src1] / alu_src2_value(op, src2, imm24)) & sizemask[op.size()];
        m_regs.mdu[1] = (m_regs.gpi[src1] % alu_src2_value(op, src2, imm24)) & sizemask[op.size()];
        break;
    case AX_EXE_MDU_DIVU:
        m_regs.mdu[0] = (m_regs.gpi[src1] / alu_src2_value(op, src2, imm24)) & sizemask[op.size()];
        m_regs.mdu[1] = (m_regs.gpi[src1] % alu_src2_value(op, src2, imm24)) & sizemask[op.size()];
        break;
    case AX_EXE_MDU_MUL:
        m_regs.mdu[2] = (m_regs.gpi[src1] * alu_src2_value(op, src2, imm24)) & sizemask[op.size()];
        break;
    case AX_EXE_MDU_MULU:
        m_regs.mdu[2] = (m_regs.gpi[src1] * alu_src2_value(op, src2, imm24)) & sizemask[op.size()];
        break;
    case AX_EXE_MDU_MOVETO:
        m_regs.gpi[dst] = m_regs.mdu[op.mdu_pq()];
        break;
    case AX_EXE_MDU_MOVEFROM:
        m_regs.mdu[op.mdu_pq()] = m_regs.gpi[src1];
        break;
    default:
        assert(false && "Wrong operation");
    }
}

void AxCore::execute_lsu(AxOpcode op, uint64_t imm24)
{
    const auto reg = op.reg_a();
    const auto addr = op.reg_b();
    const auto off = op.reg_c();
    const auto size = op.size();

    switch(op.operation())
    {
    // reg version
    case AX_EXE_LSU_LD:
        do_load(&m_regs.gpi[reg], m_regs.gpi[addr] + (m_regs.gpi[off] << op.lsu_shift()), 1 << size);
        break;
    case AX_EXE_LSU_ST:
        do_store(&m_regs.gpi[reg], m_regs.gpi[addr] + (m_regs.gpi[off] << op.lsu_shift()), 1 << size);
        break;
    case AX_EXE_LSU_FLD:
        do_load(&m_regs.gpf[reg], m_regs.gpi[addr] + (m_regs.gpi[off] << op.lsu_shift()), 1 << size);
        break;
    case AX_EXE_LSU_FST:
        do_store(&m_regs.gpf[reg], m_regs.gpi[addr] + (m_regs.gpi[off] << op.lsu_shift()), 1 << size);
        break;
    // imm version
    case AX_EXE_LSU_LDI:
        do_load(&m_regs.gpi[reg], m_regs.gpi[addr] + lsu_imm_value(op, imm24), 1 << size);
        break;
    case AX_EXE_LSU_STI:
        do_store(&m_regs.gpi[reg], m_regs.gpi[addr] + lsu_imm_value(op, imm24), 1 << size);
        break;
    case AX_EXE_LSU_FLDI:
        do_load(&m_regs.gpf[reg], m_regs.gpi[addr] + lsu_imm_value(op, imm24), 1 << size);
        break;
    case AX_EXE_LSU_FSTI:
        do_store(&m_regs.gpf[reg], m_regs.gpi[addr] + lsu_imm_value(op, imm24), 1 << size);
        break;
    // sp version
    case AX_EXE_LSU_LDSP:
        do_load(&m_regs.gpi[reg], m_regs.gpi[0] + (op.lsu_imm16() ^ (imm24 << 16)), 1 << size);
        break;
    case AX_EXE_LSU_STSP:
        do_store(&m_regs.gpi[reg], m_regs.gpi[0] + (op.lsu_imm16() ^ (imm24 << 16)), 1 << size);
        break;
    case AX_EXE_LSU_FLDSP:
        do_load(&m_regs.gpf[reg], m_regs.gpi[0] + (op.lsu_imm16() ^ (imm24 << 16)), 1 << size);
        break;
    case AX_EXE_LSU_FSTSP:
        do_store(&m_regs.gpf[reg], m_regs.gpi[0] + (op.lsu_imm16() ^ (imm24 << 16)), 1 << size);
        break;
    }
}

void AxCore::execute_bru(AxOpcode op, uint64_t imm24)
{
    switch(op.operation())
    {
    case AX_EXE_BRU_BNE:
        if((m_regs.fr & Z_MASK) != 0)
        {
            m_regs.pc += (op.bru_imm24() | (imm24 << 24));
        }
        break;
    case AX_EXE_BRU_BEQ:
        if((m_regs.fr & Z_MASK) == 0)
        {
            m_regs.pc += (op.bru_imm24() | (imm24 << 24));
        }
        break;
    case AX_EXE_BRU_BL:
        if((m_regs.fr & N_MASK) != 0)
        {
            m_regs.pc += (op.bru_imm24() | (imm24 << 24));
        }
        break;
    case AX_EXE_BRU_BLE:
        if((m_regs.fr & (Z_MASK | N_MASK)) != 0)
        {
            m_regs.pc += (op.bru_imm24() | (imm24 << 24));
        }
        break;
    case AX_EXE_BRU_BG:
        if((m_regs.fr & N_MASK) == 0)
        {
            m_regs.pc += (op.bru_imm24() | (imm24 << 24));
        }
        break;
    case AX_EXE_BRU_BGE:
        if((m_regs.fr & (Z_MASK | N_MASK)) == 0)
        {
            m_regs.pc += (op.bru_imm24() | (imm24 << 24));
        }
        break;
    case AX_EXE_BRU_BLS:
        if((m_regs.fr & Z_MASK) != 0)
        {
            m_regs.pc += (op.bru_imm24() | (imm24 << 24));
        }
        break;
    case AX_EXE_BRU_BLES:
        if((m_regs.fr & Z_MASK) != 0)
        {
            m_regs.pc += (op.bru_imm24() | (imm24 << 24));
        }
        break;
    case AX_EXE_BRU_BGS:
        if((m_regs.fr & Z_MASK) != 0)
        {
            m_regs.pc += (op.bru_imm24() | (imm24 << 24));
        }
        break;
    case AX_EXE_BRU_BGES:
        if((m_regs.fr & Z_MASK) != 0)
        {
            m_regs.pc += (op.bru_imm24() | (imm24 << 24));
        }
        break;
    case AX_EXE_BRU_BRA:
        m_regs.pc += (op.bru_imm24() | (imm24 << 24));
        break;
    case AX_EXE_BRU_LOOP:
        if(m_regs.lc == 0)
        {
            m_regs.pc += (op.bru_imm24() | (imm24 << 24));
        }
        else
        {
            m_regs.lc -= 1;
        }
        break;
    case AX_EXE_BRU_JUMP:
        m_regs.pc = (op.bru_imm24() | (imm24 << 24));
        break;
    case AX_EXE_BRU_CALL:
        m_regs.lr = m_regs.pc;
        m_regs.pc = (op.bru_imm24() | (imm24 << 24));
        break;
    case AX_EXE_BRU_CALLR:
        m_regs.lr = m_regs.pc;
        m_regs.pc += (op.bru_imm24() | (imm24 << 24));
        break;
    case AX_EXE_BRU_OTHER:
        execute_bru_other(op.bru_other_index());
        break;
    default:
        assert(false && "Invalid BRU operation");
        break;
    }
}

void AxCore::execute_bru_other(uint32_t op)
{
    switch(op)
    {
    case 1:
        m_regs.pc = m_regs.br;
        break;
    case 2:
        m_regs.pc = m_regs.lr;
        break;
    case 3:
        m_regs.pc = m_regs.ir;
        break;
    case 4:
        m_regs.ir = m_regs.pc;
        m_regs.pc = 0x80000000u;
        m_syscall = 1;
        break;
    case 5:
        m_regs.ir = m_regs.pc;
        m_regs.pc = 0x80000002u;
        m_syscall = 2;
        break;
    case 9:
        m_regs.pc = m_regs.br;
        m_regs.lr = m_regs.pc;
    default:
        assert(false && "Invalid BRU operation");
        break;
    }
}

void AxCore::execute_fpu(AxOpcode op, uint64_t imm24)
{
    assert(false && "FPU not supported yet");
}

void AxCore::execute_efu(AxOpcode op, uint64_t imm24)
{
    assert(false && "EFU not supported yet");
}

void AxCore::execute_cu(AxOpcode op, uint64_t imm24)
{
    assert(false && "CU not supported yet");
}

void AxCore::execute_vu(AxOpcode op, uint64_t imm24)
{
    assert(false && "VU not supported yet");
}
