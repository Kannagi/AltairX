
#include <cstring>
#include <cmath>
#include <stdint.h>
#include <vector>

#include "altairx.hpp"
#include "utilities.hpp"
#include "opcode.hpp"

void AxCore::execute_unit0(uint32_t rega, uint32_t regb, uint32_t regc, uint32_t unit, uint32_t size, uint32_t opcode, uint32_t imm24)
{
  uint64_t tmp, tmp2;

  switch(unit)
  {
  case AX_EXE_EFU_FDIV:
    m_core->efuQ = m_core->freg[regb << 1] / m_core->freg[regc << 1];
    break;
  case AX_EXE_EFU_FATAN2:
    m_core->efuQ = std::atan2(m_core->freg[regb << 1], m_core->freg[regc << 1]);
    break;
  case AX_EXE_EFU_DDIV:
    m_core->efuQd = m_core->dreg[regb] / m_core->dreg[regc];
    break;
  case AX_EXE_EFU_DATAN2:
    m_core->efuQd = std::atan2(m_core->dreg[regb], m_core->dreg[regc]);
    break;
  case AX_EXE_EFU_FSQRT:
    m_core->efuQ = std::sqrt(m_core->freg[regb << 1]);
    break;
  case AX_EXE_EFU_FSIN:
    m_core->efuQ = std::sin(m_core->freg[regb << 1]);
    break;
  case AX_EXE_EFU_FATAN:
    m_core->efuQ = std::atan(m_core->freg[regb << 1]);
    break;
  case AX_EXE_EFU_FEXP:
    m_core->efuQ = std::exp(m_core->freg[regb << 1]);
    break;
  case AX_EXE_EFU_DSQRT:
    m_core->efuQd = std::sqrt(m_core->dreg[regb]);
    break;
  case AX_EXE_EFU_DSIN:
    m_core->efuQd = std::sin(m_core->dreg[regb]);
    break;
  case AX_EXE_EFU_DATAN:
    m_core->efuQd = std::atan(m_core->dreg[regb]);
    break;
  case AX_EXE_EFU_DEXP:
    m_core->efuQd = std::exp(m_core->dreg[regb]);
    break;
  case AX_EXE_EFU_FPOW:
    // m_core->efuQ  = powf(m_core->freg[regb<<1]);
    break;
  case AX_EXE_EFU_DPOW:
    // m_core->efuQd = pow(m_core->dreg[regb]);
    break;
  case AX_EXE_EFU_EMOVETO:
    m_core->efuQd = m_core->dreg[regb];
    m_core->efuQ = m_core->freg[regb];
    break;
  case AX_EXE_EFU_EMOVEFROM:
    // Code pour AX_EXE_EFU_EMOVEFROM
    break;

  default:

    break;
  }
}
