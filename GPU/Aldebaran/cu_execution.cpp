#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cu.hpp>

int32_t executeDelayedInstruction(ComputeUnit cu, uint16_t imm)
{
	switch (cu->delayop)
	{
	default: return AR_ERROR_ILLEGAL_INSTRUCTION;

	case AK1_OPCODE_BNE:  // !=
		if (cu->flags & Z_MASK)
		{
			cu->pc += imm;
		}

		break;

	case AK1_OPCODE_BEQ:  // ==
		if (!(cu->flags & Z_MASK))
		{
			cu->pc += imm;
		}
		break;

	case AK1_OPCODE_BL:  // <
		if (cu->flags & U_MASK)
		{
			cu->pc += imm;
		}
		break;

	case AK1_OPCODE_BLE:  // <=
		if ((cu->flags & U_MASK) || !(cu->flags & Z_MASK))
		{
			cu->pc += imm;
		}
		break;

	case AK1_OPCODE_BG:  // >
		if (!(cu->flags & U_MASK))
		{
			cu->pc += imm;
		}
		break;

	case AK1_OPCODE_BGE:  // >=
		if (!(cu->flags & U_MASK) || !(cu->flags & Z_MASK))
		{
			cu->pc += imm;
		}
		break;

	case AK1_OPCODE_BLS:  // <
		if (cu->flags & S_MASK)
		{
			cu->pc += imm;
		}
		break;

	case AK1_OPCODE_BLES:  // <=
		if ((cu->flags & S_MASK) || !(cu->flags & Z_MASK))
		{
			cu->pc += imm;
		}
		break;

	case AK1_OPCODE_BGS:  // >
		if (!(cu->flags & S_MASK))
		{
			cu->pc += imm;
		}
		break;

	case AK1_OPCODE_BGES:  // >=
		if (!(cu->flags & S_MASK) || !(cu->flags & Z_MASK))
		{
			cu->pc += imm;
		}
		break;

	case AK1_OPCODE_BRA: cu->pc += imm; break;

	case AK1_OPCODE_JMP: cu->pc = imm; break;

	case AK1_OPCODE_CALL:
		cu->lr = cu->pc;
		cu->pc = imm;
		break;

	case AK1_OPCODE_JMPBR: cu->pc = cu->br + imm; break;

	case AK1_OPCODE_CALLBR:
		cu->lr = cu->pc;
		cu->pc = cu->br + imm;
		break;

	case AK1_OPCODE_RET: cu->pc = cu->lr; break;
	}

	cu->delay = 0;

	return AR_SUCCESS;
}

int32_t Aldebaran_ExcecuteInstruction(ComputeUnit cu)
{
	assert(cu);

	// printf("pc %x\n",cu->pc);

	if (cu->delay == 1)
	{
		int32_t result = executeDelayedInstruction(cu, cu->imm);
		if (result != AR_SUCCESS)
		{
			return result;
		}
	}
	/*
	    for(uint32_t i = 0; i < size; ++i)
	    {
	        /nt32_t result = executeInstruction(cu, i);
	        if(result != AR_SUCCESS)
	        {
	            return result;
	        }
	    }
	*/
	cu->cycle++;

	return AR_SUCCESS;
}
