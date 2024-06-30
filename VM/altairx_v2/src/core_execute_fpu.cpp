
#include <cstring>
#include <cmath>
#include <stdint.h>
#include <vector>

#include "altairx.hpp"
#include "utilities.hpp"
#include "opcode.hpp"


void AxCore::execute_unit_fp(Register rega, Register regb, Register regc, uint32_t unit, uint32_t imm24, uint32_t size)
{
    switch(unit)
    {
    case AX_EXE_FPU_FADD:
    case AX_EXE_FPU_FSUB:
    case AX_EXE_FPU_FMUL:
    case AX_EXE_FPU_FNMUL:
    case AX_EXE_FPU_HTOF:
    case AX_EXE_FPU_FTOH:
    case AX_EXE_FPU_ITOF:
    case AX_EXE_FPU_FTOI:
    case AX_EXE_FPU_FNEG:
    case AX_EXE_FPU_FABS:
    case AX_EXE_FPU_FMIN:
    case AX_EXE_FPU_FMAX:
    case AX_EXE_FPU_FMOVE:
    case AX_EXE_FPU_FCMP:
    case AX_EXE_FPU_FMOVEI:
    case AX_EXE_FPU_FCMPI:
        execute_unit_fp(rega, regb, regc, unit, imm24, size);
        break;

    default:
        if(tunit == 0)
        {
            execute_unit0(rega, regb, regc, unit, size, opcode, imm24);
        }
        else
        {
            execute_unit1(rega, regb, regc, unit, size, opcode);
        }
        break;
    }
}

void AxCore::execute_unit_fp(Register rega, Register regb, Register regc, uint32_t unit, uint32_t imm24, uint32_t size)
{
    switch(size)
    {
    case 0:
        execute_unit_half(rega, regb, regc, unit, imm24);
        break;

    case 1:
        execute_unit_float(rega, regb, regc, unit, imm24);
        break;

    case 2:
        execute_unit_double(rega, regb, regc, unit, imm24);
        break;

    case 3:
        execute_unit_falu(rega, regb, regc, unit, imm24);
        break;
    default:
        assert(false && "");
    }
}

void AxCore::execute_unit_double(uint32_t rega, uint32_t regb, uint32_t regc, uint32_t unit, uint32_t imm24)
{
  uint64_t tmp, tmp2;

  switch(unit)
  {
  case AX_EX_FPU_FADD:
    m_regs.dregs[rega] = m_regs.dregs[regb] + m_regs.dregs[regc];
    break;
  case AX_EX_FPU_FSUB:
    m_regs.dregs[rega] = m_regs.dregs[regb] - m_regs.dregs[regc];
    break;
  case AX_EX_FPU_FMUL:
    m_regs.dregs[rega] = m_regs.dregs[regb] * m_regs.dregs[regc];
    break;
  case AX_EX_FPU_FNMUL:
    m_regs.dregs[rega] = -m_regs.dregs[regb] * m_regs.dregs[regc];
    break;
  case AX_EX_FPU_HTOF: // Double to float
    // Code pour AX_EX_FPU_HTOF
    break;
  case AX_EX_FPU_FTOH:
    // Code pour AX_EX_FPU_FTOH
    break;
  case AX_EX_FPU_ITOF:
    // Code pour AX_EX_FPU_ITOF
    break;
  case AX_EX_FPU_FTOI:
    // Code pour AX_EX_FPU_FTOI
    break;
  case AX_EX_FPU_FNEG:
    // Code pour AX_EX_FPU_FNEG
    break;
  case AX_EX_FPU_FABS:
    // Code pour AX_EX_FPU_FABS
    break;
  case AX_EX_FPU_FMIN:
    // Code pour AX_EX_FPU_FMIN
    break;
  case AX_EX_FPU_FMAX:
    // Code pour AX_EX_FPU_FMAX
    break;
  case AX_EX_FPU_FMOVEI:
    // Code pour AX_EX_FPU_FMOVEI
    break;
  case AX_EX_FPU_FMOVE:
    break;
  case AX_EX_FPU_FCMP:
    break;
  case AX_EX_FPU_FCMPI:
    break;
  }
}

void AltairX::execute_unit_float(uint32_t rega, uint32_t regb, uint32_t regc, uint32_t unit, uint32_t imm24)
{
  uint64_t tmp;
  float ftmp;

  switch(unit)
  {
  case AX_EX_FPU_FADD:
    m_core->freg[rega << 1] = m_core->freg[regb << 1] + m_core->freg[regc << 1];
    break;
  case AX_EX_FPU_FSUB:
    m_core->freg[rega << 1] = m_core->freg[regb << 1] - m_core->freg[regc << 1];
    break;
  case AX_EX_FPU_FMUL:
    m_core->freg[rega << 1] = m_core->freg[regb << 1] * m_core->freg[regc << 1];
    break;
  case AX_EX_FPU_FNMUL:
    m_core->freg[rega << 1] = -m_core->freg[regb << 1] * m_core->freg[regc << 1];
    break;
  case AX_EX_FPU_HTOF:
    m_core->freg[rega << 1] = half_to_float(m_core->hreg[regb << 2]);
    break;
  case AX_EX_FPU_FTOH:
    m_core->hreg[rega << 2] = float_to_half(m_core->freg[regb << 1]);
    break;
  case AX_EX_FPU_ITOF:
    m_core->freg[rega << 1] = m_core->fireg[regb << 1];
    break;
  case AX_EX_FPU_FTOI:
    m_core->fireg[rega << 1] = m_core->freg[regb << 1];
    break;
  case AX_EX_FPU_FNEG:
    m_core->freg[rega << 1] = -m_core->freg[regb << 1];
    break;
  case AX_EX_FPU_FABS:
    if(m_core->freg[regb << 1] < 0.0f)
    {
      m_core->freg[rega << 1] = -m_core->freg[regb << 1];
    }
    else
    {
      m_core->freg[rega << 1] = m_core->freg[regb << 1];
    }
    break;
  case AX_EX_FPU_FMIN:
    if(m_core->freg[regb << 1] < m_core->freg[regc << 1])
    {
      m_core->freg[rega << 1] = m_core->freg[regb << 1];
    }
    else
    {
      m_core->freg[rega << 1] = m_core->freg[regb << 1];
    }
    break;
  case AX_EX_FPU_FMAX:
    if(m_core->freg[regb << 1] > m_core->freg[regc << 1])
    {
      m_core->freg[rega << 1] = m_core->freg[regb << 1];
    }
    else
    {
      m_core->freg[rega << 1] = m_core->freg[regb << 1];
    }
    break;

  case AX_EX_FPU_FMOVE:
    m_core->freg[rega << 1] = m_core->freg[regb << 1];
    break;
  case AX_EX_FPU_FCMP:
    ftmp = m_core->freg[regb << 1] - m_core->freg[regb << 1];

    if(ftmp == 0.0f)
    {
      m_core->fr |= Z_MASK;
    }

    if(ftmp > 0.0f)
    {
      m_core->fr |= O_MASK;
    }

    if(ftmp < 0.0f)
    {
      m_core->fr |= N_MASK;
    }

    break;

  case AX_EX_FPU_FMOVEI:
    tmp = ((m_topcode >> 10) & 0xFFFF) | (imm24 << 16);

    memcpy(&m_core->freg[rega << 1], &tmp, 4);

    break;
  case AX_EX_FPU_FCMPI:
    tmp = ((m_topcode >> 10) & 0xFFFF) | (imm24 << 16);

    memcpy(&ftmp, &tmp, 4);

    ftmp = m_core->freg[regb << 1] - ftmp;

    if(ftmp == 0.0f)
    {
      m_core->fr |= Z_MASK;
    }

    if(ftmp > 0.0f)
    {
      m_core->fr |= O_MASK;
    }

    if(ftmp < 0.0f)
    {
      m_core->fr |= N_MASK;
    }

    break;
  }
}

void AxCore::execute_unit_falu(uint32_t rega, uint32_t regb, uint32_t regc, uint32_t unit, uint32_t imm24)
{
}

void AxCore::execute_unit_half(uint32_t rega, uint32_t regb, uint32_t regc, uint32_t unit, uint32_t imm24)
{
  uint64_t tmp, tmp2;

  switch(unit)
  {
  case AX_EX_FPU_FADD:

    break;
  case AX_EX_FPU_FSUB:
    // Code pour AX_EX_FPU_FSUB
    break;
  case AX_EX_FPU_FMUL:
    // Code pour AX_EX_FPU_FMUL
    break;
  case AX_EX_FPU_FNMUL:
    // Code pour AX_EX_FPU_FNMUL
    break;
  case AX_EX_FPU_HTOF:
    // Code pour AX_EX_FPU_HTOF
    break;
  case AX_EX_FPU_FTOH:
    // Code pour AX_EX_FPU_FTOH
    break;
  case AX_EX_FPU_ITOF:
    // Code pour AX_EX_FPU_ITOF
    break;
  case AX_EX_FPU_FTOI:
    // Code pour AX_EX_FPU_FTOI
    break;
  case AX_EX_FPU_FNEG:
    // Code pour AX_EX_FPU_FNEG
    break;
  case AX_EX_FPU_FABS:
    // Code pour AX_EX_FPU_FABS
    break;
  case AX_EX_FPU_FMIN:
    // Code pour AX_EX_FPU_FMIN
    break;
  case AX_EX_FPU_FMAX:
    // Code pour AX_EX_FPU_FMAX
    break;
  case AX_EX_FPU_FMOVEI:
    // Code pour AX_EX_FPU_FMOVEI
    break;
  case AX_EX_FPU_FMOVE:
    break;
  case AX_EX_FPU_FCMP:
    break;
  case AX_EX_FPU_FCMPI:
    break;
  }
}
