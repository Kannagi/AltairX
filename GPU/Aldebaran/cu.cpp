#include <cstdio>
#include <cstring>
#include <cu.hpp>
#include <gpu.hpp>

int ComputeUnit_t::decode()
{
	int res = decodeInstruction1();
	if (res != 0)
		return res;
	return decodeInstruction2();
}

int32_t ComputeUnit_t::execute()
{
	return executeInstructions(0);
}

int32_t ComputeUnit_t::executeInstructions(uint32_t index)
{
	for (size_t shaderi = 0; shaderi < N_SHADER; ++shaderi)
	{
		auto res = executeInstruction(shaderi, index);
		if (res != AR_SUCCESS)
			return res;
	}

	return AR_SUCCESS;
}

int32_t ComputeUnit_t::executeInstruction(size_t shaderi, uint32_t index)
{
	auto* ireg = shaders[0].ireg;
	auto* freg = shaders[0].vreg;
	auto* hreg = shaders[0].hreg;

	const uint32_t unit1 = operations[index].unit1;
	uint32_t unit2       = operations[index].unit2;
	const uint32_t size  = operations[index].size;
	const uint32_t id    = operations[index].id;

	const int32_t rA = operations[index].rA;
	const int32_t rB = operations[index].rB;
	const int32_t rC = operations[index].rC;

	// float* fopA, * fopB, * fopC;
	// double dopA, dopB, dopC;

	const int32_t opB = ireg[rB].x();
	int32_t opC;

	if (operations[index].immf == 0)
		opC = ireg[rC].x();
	else
		opC = operations[index].imm;

	switch (unit1)
	{
	case AK1_EXE_ALU:
		return executeALU(unit2, ireg, rA, opB, opC);
	case AK1_EXE_LSU:
		return executeLSU(unit2, freg, rA, opB, opC);
	case AK1_EXE_BRU:
	{
		delay = 1;
		break;
	}
	case AK1_EXE_CMP:
		return executeCMP(unit2, ireg, rA, opB, opC, fopA, fopB, size, id);
	case AK1_EXE_VFPU:
		executeVFPU(unit2, freg, rA, opB, opC, fopA, fopB, size, id);
	case AK1_EXE_DMA:
		return executeDMA(unit2, ireg, rA, opB, opC);
	case AK1_EXE_OTHER:
		return executeOther(unit2, ireg, freg, hreg, rA, opB);
	case AK1_EXE_NOP:
	{
		break;
	}
	default:
		return AR_ERROR_ILLEGAL_INSTRUCTION;
	}

	return AR_SUCCESS;
}

namespace
{
uint32_t readbits(uint32_t buffer, uint32_t index, uint32_t size)
{
	return (buffer >> index) & ((1u << size) - 1);
}
}  // namespace

int ComputeUnit_t::decodeInstruction1()
{
	const uint32_t opcode = opcodes[0];
	// printf("opcode %x\n",opcode);

	ArExecute_T* output = &operations[0];

	// Decode /issue
	switch (readbits(opcode, 0, 4))
	{
	case Exe::AK1_EXE_ALU_B:
		return decodeALU_B(opcode, output);
	case Exe::AK1_EXE_ALU_A:
		return decodeALU_A(opcode, output);
	case Exe::AK1_EXE_LSU_SPM:
		return decodeLSU_SPM(opcode, output);
	case Exe::AK1_EXE_LSU_CACHE:
		return decodeLSU_Cache(opcode, output);
	case Exe::AK1_EXE_CMP:
		return decodeCMP(opcode, output);
	case Exe::AK1_EXE_BRU:
		return decodeBRU(opcode, output);
	case Exe::AK1_EXE_VBRU:
		return decodeVBRU(opcode, output);
	default:
		return AK1_OPCODE_UNKNOWN;
	}
}

int ComputeUnit_t::decodeALU_A(uint64_t opcode, ArExecute_T* output) {}
int ComputeUnit_t::decodeALU_B(uint64_t opcode, ArExecute_T* output)
{
	switch (readbits(opcode, 4, 4))
	{
	case OpcodeALU_B::AK1_OPCODE_NOP:
		return 0;
	case OpcodeALU_B::AK1_OPCODE_SEXT:
		return 0;
	case OpcodeALU_B::AK1_OPCODE_BOOL:
		return 0;
	case OpcodeALU_B::AK1_OPCODE_SMOVE:
		return 0;
	case OpcodeALU_B::AK1_OPCODE_MOVEI:
		return 0;
	case OpcodeALU_B::AK1_OPCODE_MOVEIU:
		return 0;
	default:
		return AK1_OPCODE_UNKNOWN;
	}
}
int ComputeUnit_t::decodeLSU_SPM(uint64_t opcode, ArExecute_T* output) {}
int ComputeUnit_t::decodeLSU_Cache(uint64_t opcode, ArExecute_T* output) {}
int ComputeUnit_t::decodeCMP(uint64_t opcode, ArExecute_T* output) {}
int ComputeUnit_t::decodeBRU(uint64_t opcode, ArExecute_T* output) {}
int ComputeUnit_t::decodeVBRU(uint64_t opcode, ArExecute_T* output) {}
int ComputeUnit_t::decodeDMA(uint64_t opcode, ArExecute_T* output) {}

int ComputeUnit_t::decodeInstruction2()
{
	const uint32_t opcode       = opcodes[0];
	const uint32_t compute_unit = readbits(opcode, 0, 2);
	const uint32_t imm =
	    readbits(opcode, 10, 16) + (readbits(opcode, 4, 4) << 16);

	const uint32_t unit1 = readbits(opcode, 2, 2);
	const uint32_t unit2 = readbits(opcode, 4, 4);

	operations[0].size = readbits(opcode, 8, 2);
	operations[0].id   = readbits(opcode, 10, 2);
	uint64_t regA      = readbits(opcode, 26, 6);
	uint64_t regB      = readbits(opcode, 20, 6);
	uint64_t regC      = readbits(opcode, 14, 6);

	// Read Register
	operations[0].rA = regA;
	operations[0].rB = regB;
	operations[0].rC = regC;

	operations[0].immf = 0;

	// printf("opcode %x\n",opcode);

	ArExecute_T* output = &operations[0];

	if ((opcode & 0xFF) != 0x0A)  // NOP
		instruction++;

	// Decode /issue
	if (compute_unit == 0)
	{
		return decodeDMA(cu, output, unit2, imm);
	}
	else if (compute_unit == 1)  // LSU
	{
		return decodeLSU(cu, output, unit1, unit2, imm);
	}
	else if (compute_unit == 2)  // ALU
	{
		return decodeALU(cu, output, unit1, unit2, imm);
	}
	else  // VFPU
	{
		return decodeVFPU(cu, output, unit1, unit2, imm);
	}

	return 0;
}

int ComputeUnit_t::decodeALUV_A(uint64_t opcode, ArExecute_T* output) {}
int ComputeUnit_t::decodeALUV_B(uint64_t opcode, ArExecute_T* output) {}
int ComputeUnit_t::decodeLSUV_SPM(uint64_t opcode, ArExecute_T* output) {}
int ComputeUnit_t::decodeLSUV_Cache(uint64_t opcode, ArExecute_T* output) {}
int ComputeUnit_t::decodeVCMP(uint64_t opcode, ArExecute_T* output) {}
int ComputeUnit_t::decodeVFPU_A(uint64_t opcode, ArExecute_T* output) {}
int ComputeUnit_t::decodeVFPU_B(uint64_t opcode, ArExecute_T* output) {}
int ComputeUnit_t::decodeVFPU_C(uint64_t opcode, ArExecute_T* output) {}
int ComputeUnit_t::decodeEFU(uint64_t opcode, ArExecute_T* output) {}
int ComputeUnit_t::decodePPU(uint64_t opcode, ArExecute_T* output) {}

constexpr uint32_t ZSUClearMask = ~(Z_MASK | S_MASK | U_MASK);

void ComputeUnit_t::executeLS(void* reg,
                              uint64_t offset,
                              uint32_t size,
                              uint32_t store)
{
	uint8_t* address;

	if (offset & MEMORY_MAP_RAM_BEGIN)
	{
		address = ram;
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
				address = spm2;
				offset &= 0xFFFFFFF;  // Max 256 Mio
			}
			else if (offset == MEMORY_MAP_1TSRAM_BEGIN)
			{
				address = tsram;
				offset &= 0x7FFFFFFF;  // Max 2 Gio
			}
			else
			{
			}
		}
	}

	if (store)
		std::memcpy(reg, address + offset, size);
	else
		std::memcpy(address + offset, reg, size);
}

int32_t ComputeUnit_t::executeALU(uint32_t unit2,
                                  vec4i* ireg,
                                  int32_t rA,
                                  int32_t opB,
                                  int32_t opC)
{
	switch (unit2)
	{
	default:
		return AR_ERROR_ILLEGAL_INSTRUCTION;

	case AK1_OPCODE_ADD:
		ireg[rA].x() = opB + opC;
		break;

	case AK1_OPCODE_SUB:
		ireg[rA].x() = opB - opC;
		break;

	case AK1_OPCODE_XOR:
		ireg[rA].x() = opB ^ opC;
		break;

	case AK1_OPCODE_OR:
		ireg[rA].x() = opB | opC;
		break;

	case AK1_OPCODE_AND:
		ireg[rA].x() = opB & opC;
		break;

	case AK1_OPCODE_LSL:
		ireg[rA].x() = opB << opC;
		break;

	case AK1_OPCODE_LSR:
		ireg[rA].x() = opB >> opC;
		break;

	case AK1_OPCODE_ASR:
		ireg[rA].x() = int32_t(opB) >> int32_t(opC);
		break;

	case AK1_OPCODE_MOVE:
		ireg[rA].x() = opB;
		break;

	case AK1_OPCODE_MOVESP:
		ireg[rA].x() = 0;
		break;
	}

	return AR_SUCCESS;
}

int32_t ComputeUnit_t::executeLSU(uint32_t unit2,
                                  vec4f* freg,
                                  int32_t rA,
                                  int32_t opB,
                                  int32_t opC)
{
	uint64_t offset = opB + opC;
	switch (unit2)
	{
	default:
		return AR_ERROR_ILLEGAL_INSTRUCTION;

	case AK1_OPCODE_LDM:
		std::memcpy(&freg[rA * 4], dsram + (offset & 0x1FFFF), 16);
		break;

	case AK1_OPCODE_STM:
		std::memcpy(dsram + (offset & 0x1FFFF), &freg[rA * 4], 16);
		break;

	case AK1_OPCODE_LDC:
		executeLS(&freg[rA * 4], offset, 16, 0);
		break;

	case AK1_OPCODE_FLUSH:
	case AK1_OPCODE_PREFETCH:
		break;
	}

	return AR_SUCCESS;
}

int32_t ComputeUnit_t::executeVFPU(const ArExecute_T& op,
                                   vec4f* freg,
                                   int32_t rA,
                                   int32_t opB,
                                   int32_t opC,
                                   const float* fopA,
                                   const float* fopB,
                                   uint32_t size,
                                   uint32_t id)
{
	uint32_t unit3 = unit2 >> 4;
	unit2 &= 0xF;

	fopB = operations[index].fopB;
	fopC = operations[index].fopC;

	if (unit3 == 0)  // VFALU
	{
		opA *= 4;
		switch (unit2)
		{
		default:
			return AR_ERROR_ILLEGAL_INSTRUCTION;

		case AK1_OPCODE_FADD:
			freg[opA] = fopB[size] + fopC[id];
			break;

		case AK1_OPCODE_FSUB:
			freg[opA] = fopB[size] - fopC[id];
			break;

		case AK1_OPCODE_FMUL:
			freg[opA] = fopB[size] * fopC[id];
			break;

		case AK1_OPCODE_FMADD:
			freg[opA] *= fopB[size] + fopC[id];
			break;

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
		dopB = operations[index].dopB;
		opA *= 4;

		int32_t* viregB = (int32_t*)operations[index].fopB;

		switch (unit2)
		{
		default:
			return AR_ERROR_ILLEGAL_INSTRUCTION;

		case AK1_OPCODE_FMOVE:
			freg[opA + size] = fopB[id];
			break;

		case AK1_OPCODE_VFMOVE:
			for (int i = 0; i < size; i++)
				freg[opA + i] = fopB[i];
			break;

		case AK1_OPCODE_FNEG:
			freg[opA + size] = -fopB[id];
			break;

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

		case AK1_OPCODE_VFTOD:
			dreg[opA >> 1] = fopB[0];
			break;

		case AK1_OPCODE_VDTOF:
			freg[opA] = dopB;
			break;
		}
	}
	else if (unit3 == 2)  // VF
	{
	}
	else  // DOUBLE
	{
		dopB = operations[index].dopB;
		dopC = operations[index].dopC;

		opA *= 2;
		switch (unit2)
		{
		default:
			return AR_ERROR_ILLEGAL_INSTRUCTION;

		case AK1_OPCODE_DMOVE:
			dreg[opA] = dopB;
			break;

		case AK1_OPCODE_DADD:
			dreg[opA] = dopB + dopC;
			break;

		case AK1_OPCODE_DSUB:
			dreg[opA] = dopB - dopC;
			break;

		case AK1_OPCODE_DMUL:
			dreg[opA] = dopB * dopC;
			break;

		case AK1_OPCODE_FNEG:
			dreg[opA] = -dopB;
			break;

		case AK1_OPCODE_FABS:
			if (dopB < 0)
				dreg[opA] = -dopB;
			break;

		case AK1_OPCODE_DDIV:
			dreg[opA] = dopB / dopC;
			break;
		}
	}
}

int32_t ComputeUnit_t::executeCMP(uint32_t unit2,
                                  vec4i* ireg,
                                  int32_t rA,
                                  int32_t opB,
                                  int32_t opC,
                                  const float* fopA,
                                  const float* fopB,
                                  uint32_t size,
                                  uint32_t id)
{
	float ftmp;
	opC = ireg[rA].x();

	switch (unit2)
	{
	default:
		return AR_ERROR_ILLEGAL_INSTRUCTION;

	case AK1_OPCODE_CMP:

		flags &= ZSUClearMask;
		flags |= (opB != opC) << 1u;
		flags |= ((int32_t)opB < (int32_t)opC) << 2u;
		flags |= (opB < opC) << 3u;

		break;

	case AK1_OPCODE_FCMP:

		ftmp = fopA[size] - fopB[id];

		flags &= ZSUClearMask;
		flags |= (ftmp != 0.0f) << 1u;
		flags |= (ftmp < 0.0f) << 2u;
		flags |= (ftmp < 0.0f) << 3u;

		break;
	}

	return AR_SUCCESS;
}

int32_t ComputeUnit_t::executeDMA(uint32_t unit2,
                                  vec4i* ireg,
                                  int32_t rA,
                                  int32_t opB,
                                  int32_t opC)
{
	void* addressA = dsram + ireg[rA].x();

	switch (unit2)
	{
	default:
		return AR_ERROR_ILLEGAL_INSTRUCTION;

	case AK1_OPCODE_LDDMA:
		executeLS(addressA, ireg[rA].x(), opC * 64, 1);
		break;

	case AK1_OPCODE_STDMA:
		executeLS(addressA, opB, opC * 64, 0);
		break;

	case AK1_OPCODE_LDDMACL:
		for (int i = 0; i < opB; i++)
		{
			// dsram[ireg[opA]+i]
			// executeLS(cu,addressA,opB,opC*64,0);
		}
		break;

	case AK1_OPCODE_STDMACL:

	case AK1_OPCODE_DMAI:
		addressA = isram + ireg[rA].x();
		executeLS(addressA, opB, opC * 64, 0);

		break;

	case AK1_OPCODE_WAIT:
		// We don't have anything to wait since we emulate it based on C
		// memory model, all transfers are direcly coherent
		break;
	}

	return AR_SUCCESS;
}

int32_t ComputeUnit_t::executeOther(uint32_t unit2,
                                    vec4i* ireg,
                                    vec4f* freg,
                                    vec8h* hreg,
                                    int32_t rA,
                                    int32_t opB)
{
	switch (unit2)
	{
	default:
		return AR_ERROR_ILLEGAL_INSTRUCTION;

	case AK1_OPCODE_INT:

		if (opB == 0)
		{
			printf("%c\n", (char)ireg[rA].x());
		}
		else if (opB == 1)
		{
			printf("%f %f %f %f\n",     //
			       freg[(rA * 4) + 0],  //
			       freg[(rA * 4) + 1],  //
			       freg[(rA * 4) + 2],  //
			       freg[(rA * 4) + 3]   //
			);
		}
		else if (opB == 2)
		{
			printf("%d\n", hreg[rA].x());
		}

		break;

	case AK1_OPCODE_ENDP:
		return AR_END_OF_CODE;
	}

	return AR_SUCCESS;
}
