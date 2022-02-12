#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cu.hpp>

static int32_t executeDelayedInstruction(ComputeUnit cu, uint16_t imm)
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

static void executeLS(ComputeUnit cu, void* reg, uint64_t offset,
                      uint32_t size, uint32_t store)
{
	uint8_t* address;

	if (offset & MEMORY_MAP_RAM_BEGIN)
	{
		address = cu->ram;
		offset &= 0x7FFFFFFFF;  // Max 32 Gio
	}
	else
	{
		if (offset & MEMORY_MAP_VRAM_BEGIN)
		{
			return;
		}
		else
		{
			if (offset == MEMORY_MAP_SPM2_BEGIN)
			{
				address = cu->spm2;
				offset &= 0xFFFFFFF;  // Max 256 Mio
			}
			else if (offset == MEMORY_MAP_1TSRAM_BEGIN)
			{
				address = cu->tsram;
				offset &= 0x7FFFFFFF;  // Max 2 Gio
			}
			else
			{
			}
		}
	}

	if (store)
	{
		memcpy(reg, address + offset, size);
	}
	else
	{
		memcpy(address + offset, reg, size);
	}
}

static int32_t executeInstruction(ComputeUnit cu, uint32_t index)
{
	static const uint32_t ZSUClearMask = ~(Z_MASK | S_MASK | U_MASK);

	auto* ireg = cu->shaders[0].ireg;
	auto* freg = cu->shaders[0].vreg;
	auto* hreg = cu->shaders[0].hreg;

	uint32_t unit1 = cu->operations[index].unit1;
	uint32_t unit2 = cu->operations[index].unit2;
	uint32_t size = cu->operations[index].size;
	uint32_t id = cu->operations[index].id;

	int32_t rA = cu->operations[index].rA;
	int32_t rB = cu->operations[index].rB;
	int32_t rC = cu->operations[index].rC;

	float *fopA, *fopB, *fopC;
	double dopA, dopB, dopC;

	int32_t opB = ireg[rB].x();
	int32_t opC;

	if (cu->operations[index].immf == 0)
		opC = ireg[rC].x();
	else
		opC = cu->operations[index].imm;

	if (unit1 == AK1_EXE_ALU)
	{
		switch (unit2)
		{
		default: return AR_ERROR_ILLEGAL_INSTRUCTION;

		case AK1_OPCODE_ADD: ireg[rA].x() = opB + opC; break;

		case AK1_OPCODE_SUB: ireg[rA].x() = opB - opC; break;

		case AK1_OPCODE_XOR: ireg[rA].x() = opB ^ opC; break;

		case AK1_OPCODE_OR: ireg[rA].x() = opB | opC; break;

		case AK1_OPCODE_AND: ireg[rA].x() = opB & opC; break;

		case AK1_OPCODE_LSL: ireg[rA].x() = opB << opC; break;

		case AK1_OPCODE_LSR: ireg[rA].x() = opB >> opC; break;

		case AK1_OPCODE_ASR:
			ireg[rA].x() = int32_t(opB) >> int32_t(opC);
			break;

		case AK1_OPCODE_MOVE: ireg[rA].x() = opB; break;

		case AK1_OPCODE_MOVESP: ireg[rA].x() = 0; break;
		}
	}
	else if (unit1 == AK1_EXE_LSU)
	{
		uint64_t offset = opB + opC;
		switch (unit2)
		{
		default: return AR_ERROR_ILLEGAL_INSTRUCTION;

		case AK1_OPCODE_LDM:
			memcpy(&freg[rA * 4], cu->dsram + (offset & 0x1FFFF), 16);
			break;

		case AK1_OPCODE_STM:
			memcpy(cu->dsram + (offset & 0x1FFFF), &freg[rA * 4], 16);
			break;

		case AK1_OPCODE_LDC:
			executeLS(cu, &freg[rA * 4], offset, 16, 0);
			break;

		case AK1_OPCODE_FLUSH:
		case AK1_OPCODE_PREFETCH: break;
		}
	}
	else if (unit1 == AK1_EXE_CMP)
	{
		float ftmp;
		opC = ireg[rA].x();

		switch (unit2)
		{
		default: return AR_ERROR_ILLEGAL_INSTRUCTION;

		case AK1_OPCODE_CMP:

			cu->flags &= ZSUClearMask;
			cu->flags |= (opB != opC) << 1u;
			cu->flags |= ((int32_t)opB < (int32_t)opC) << 2u;
			cu->flags |= (opB < opC) << 3u;

			break;

		case AK1_OPCODE_FCMP:

			ftmp = fopA[size] - fopB[id];

			cu->flags &= ZSUClearMask;
			cu->flags |= (ftmp != 0.0f) << 1u;
			cu->flags |= (ftmp < 0.0f) << 2u;
			cu->flags |= (ftmp < 0.0f) << 3u;

			break;
		}
	}
	else if (unit1 == AK1_EXE_BRU)
	{
		cu->delay = 1;
	}
	else if (unit1 == AK1_EXE_DMA)
	{
		void* addressA = cu->dsram + ireg[rA].x();

		switch (unit2)
		{
		default: return AR_ERROR_ILLEGAL_INSTRUCTION;

		case AK1_OPCODE_LDDMA:
			executeLS(cu, addressA, ireg[rA].x(), opC * 64, 1);
			break;

		case AK1_OPCODE_STDMA:
			executeLS(cu, addressA, opB, opC * 64, 0);
			break;

		case AK1_OPCODE_LDDMACL:
			for (int i = 0; i < opB; i++)
			{
				// cu->dsram[ireg[opA]+i]
				// executeLS(cu,addressA,opB,opC*64,0);
			}
			break;

		case AK1_OPCODE_STDMACL:

		case AK1_OPCODE_DMAI:
			addressA = cu->isram + ireg[rA].x();
			executeLS(cu, addressA, opB, opC * 64, 0);

			break;

		case AK1_OPCODE_WAIT:
			// We don't have anything to wait since we emulate it based on C
			// memory model, all transfers are direcly coherent
			break;
		}
	}
	else if (unit1 == AK1_EXE_OTHER)
	{
		switch (unit2)
		{
		default: return AR_ERROR_ILLEGAL_INSTRUCTION;

		case AK1_OPCODE_INT:

			if (opB == 0)
				printf("%c\n", (char)ireg[rA].x());
			else if (opB == 1)
				printf("%f %f %f %f\n", freg[(rA * 4) + 0], freg[(rA * 4) + 1],
				       freg[(rA * 4) + 2], freg[(rA * 4) + 3]);
			else if (opB == 2)
				printf("%d\n", hreg[rA].x());

			break;

		case AK1_OPCODE_ENDP: return AR_END_OF_CODE; break;
		}
	}
	else if (unit1 == AK1_EXE_VFPU)
	{
		uint32_t unit3 = unit2 >> 4;
		unit2 &= 0xF;

		fopB = cu->operations[index].fopB;
		fopC = cu->operations[index].fopC;

		if (unit3 == 0)  // VFALU
		{
			opA *= 4;
			switch (unit2)
			{
			default: return AR_ERROR_ILLEGAL_INSTRUCTION;

			case AK1_OPCODE_FADD: freg[opA] = fopB[size] + fopC[id]; break;

			case AK1_OPCODE_FSUB: freg[opA] = fopB[size] - fopC[id]; break;

			case AK1_OPCODE_FMUL: freg[opA] = fopB[size] * fopC[id]; break;

			case AK1_OPCODE_FMADD: freg[opA] *= fopB[size] + fopC[id]; break;

			case AK1_OPCODE_FMSUB:
				freg[opA] *= fopB[size] - fopC[id];
				break;

				//------------

			case AK1_OPCODE_VFADD:
				for (int i = 0; i < size; i++)
					freg[opA + i] = fopB[i] + fopC[i];
				break;

			case AK1_OPCODE_VFSUB:
				for (int i = 0; i < size; i++)
					freg[opA + i] = fopB[i] - fopC[i];
				break;

			case AK1_OPCODE_VFMUL:
				for (int i = 0; i < size; i++)
					freg[opA + i] = fopB[i] * fopC[i];
				break;

			case AK1_OPCODE_VFMADD:
				for (int i = 0; i < size; i++)
					freg[opA + i] *= fopB[i] + fopC[i];
				break;

			case AK1_OPCODE_VFMSUB:
				for (int i = 0; i < size; i++)
					freg[opA + i] *= fopB[i] - fopC[i];
				break;

				//------------

			case AK1_OPCODE_VFADDS:
				for (int i = 0; i < size; i++)
					freg[opA + i] = fopB[i] + fopC[id];
				break;

			case AK1_OPCODE_VFSUBS:
				for (int i = 0; i < size; i++)
					freg[opA + i] = fopB[i] - fopC[id];
				break;

			case AK1_OPCODE_VFMULS:
				for (int i = 0; i < size; i++)
					freg[opA + i] = fopB[i] * fopC[id];
				break;

			case AK1_OPCODE_VFMADDS:
				for (int i = 0; i < size; i++)
					freg[opA + i] *= fopB[i] + fopC[id];
				break;

			case AK1_OPCODE_VFMSUBS:
				for (int i = 0; i < size; i++)
					freg[opA + i] *= fopB[i] - fopC[id];
				break;

				//------------

				// case AK1_OPCODE_VFSHUFFLE:

				// break;
			}
		}
		else if (unit3 == 1)  // VFMOVE
		{
			dopB = cu->operations[index].dopB;
			opA *= 4;

			int32_t* viregB = (int32_t*)cu->operations[index].fopB;

			switch (unit2)
			{
			default: return AR_ERROR_ILLEGAL_INSTRUCTION;

			case AK1_OPCODE_FMOVE: freg[opA + size] = fopB[id]; break;

			case AK1_OPCODE_VFMOVE:
				for (int i = 0; i < size; i++)
					freg[opA + i] = fopB[i];
				break;

			case AK1_OPCODE_FNEG: freg[opA + size] = -fopB[id]; break;

			case AK1_OPCODE_FABS:
				if (fopB[id] < 0)
					freg[opA + size] = -fopB[id];
				break;

			case AK1_OPCODE_VFNEG:
				for (int i = 0; i < size; i++)
					freg[opA + i] = -fopB[i];
				break;

			case AK1_OPCODE_VFABS:
				for (int i = 0; i < size; i++)
				{
					if (fopB[id] < 0)
						freg[opA + i] = -fopB[i];
				}

				break;

			case AK1_OPCODE_VFTOI:
				for (int i = 0; i < size; i++)
					vireg[opA + i] = fopB[i];
				break;

			case AK1_OPCODE_VITOF:
				for (int i = i; i < size; i++)
					freg[opA] = viregB[i];
				break;

			case AK1_OPCODE_VFTOD: dreg[opA >> 1] = fopB[0]; break;

			case AK1_OPCODE_VDTOF: freg[opA] = dopB; break;
			}
		}
		else if (unit3 == 2)  // VF
		{
		}
		else  // DOUBLE
		{
			dopB = cu->operations[index].dopB;
			dopC = cu->operations[index].dopC;

			opA *= 2;
			switch (unit2)
			{
			default: return AR_ERROR_ILLEGAL_INSTRUCTION;

			case AK1_OPCODE_DMOVE: dreg[opA] = dopB; break;

			case AK1_OPCODE_DADD: dreg[opA] = dopB + dopC; break;

			case AK1_OPCODE_DSUB: dreg[opA] = dopB - dopC; break;

			case AK1_OPCODE_DMUL: dreg[opA] = dopB * dopC; break;

			case AK1_OPCODE_FNEG: dreg[opA] = -dopB; break;

			case AK1_OPCODE_FABS:
				if (dopB < 0)
					dreg[opA] = -dopB;
				break;

			case AK1_OPCODE_DDIV: dreg[opA] = dopB / dopC; break;
			}
		}
	}
	else if (unit1 == AK1_EXE_NOP)
	{
	}
	else
	{
		return AR_ERROR_ILLEGAL_INSTRUCTION;
	}

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
