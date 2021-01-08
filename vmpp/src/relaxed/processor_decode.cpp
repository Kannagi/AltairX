#include "vm.hpp"

#include "common/base/ops.hpp"
#include "common/base/agu.hpp"
#include "common/base/alu.hpp"
#include "common/base/bru.hpp"
#include "common/base/lsu.hpp"
#include "common/base/vfpu.hpp"

#include <stdexcept>
#include <cassert>
#include <cstring>
#include <cmath>

using namespace ar;
using namespace op;

constexpr int32_t extendSign(uint32_t value, uint32_t bits)
{
	if(value > (1u << (bits - 1)))
	{
		return int32_t((0xFFFFFFFFu << bits) | value);
	}
	else
	{
		return int32_t(value);
	}
}

constexpr ArOpcode BRUComparators[16] =
{
	AR_OPCODE_BNE,
	AR_OPCODE_BEQ,
	AR_OPCODE_BL,
	AR_OPCODE_BLE,
	AR_OPCODE_BG,
	AR_OPCODE_BGE,
	AR_OPCODE_BLS,
	AR_OPCODE_BLES,
	AR_OPCODE_BGS,
	AR_OPCODE_BGES,
	AR_OPCODE_UNKNOWN,
	AR_OPCODE_UNKNOWN,
	AR_OPCODE_UNKNOWN,
	AR_OPCODE_UNKNOWN,
	AR_OPCODE_UNKNOWN,
	AR_OPCODE_UNKNOWN
};

constexpr ArOpcode BRUJumpsCalls[4] =
{
	AR_OPCODE_CALL,
	AR_OPCODE_JMP,
	AR_OPCODE_CALLR,
	AR_OPCODE_JMPR
};

constexpr int decodeBRU(uint32_t pc, BRU_op opcode, ArOperation* output)
{
	if(opcode.type == BRU_types::REGREG_branch)
	{
		const REGREG_branch_op REGREG_branch = bit_cast<REGREG_branch_op>(opcode);

		if(REGREG_branch.instruction == REGREG_branch_instructions::CMP)
		{
			const CMP cmp = bit_cast<CMP>(opcode);

			output->op = AR_OPCODE_CMP;
			output->size = cmp.size;
			output->operands[0] = cmp.source1;
			output->operands[1] = cmp.source2;
		}
		else if(REGREG_branch.instruction == REGREG_branch_instructions::FCMP)
		{
			const FCMP fcmp = bit_cast<FCMP>(opcode);

			output->op = AR_OPCODE_FCMP;
			output->operands[0] = fcmp.source1;
			output->operands[1] = fcmp.source2;
		}
		else if(REGREG_branch.instruction == REGREG_branch_instructions::DCMP)
		{
			const DCMP dcmp = bit_cast<DCMP>(opcode);

			output->op = AR_OPCODE_DCMP;
			output->operands[0] = dcmp.source1;
			output->operands[1] = dcmp.source2;
		}
		else if(REGREG_branch.instruction == REGREG_branch_instructions::branching)
		{
			const branching_op branching = bit_cast<branching_op>(opcode);

			if(branching.category == branch_categories::branch) //Branch (BCC, BCCS, FBCC, DBCC)
			{
				const branch_op branch = bit_cast<branch_op>(opcode);

				output->op = BRUComparators[uint32_t(branch.comparator)];
				output->operands[0] = pc + extendSign(uint32_t(branch.label), 14) * 2;

				if(output->op == AR_OPCODE_UNKNOWN)
				{
					return 0;
				}
			}
			else if(branching.category == branch_categories::jumps_calls)
			{
				const jumps_calls_op jumps_calls = bit_cast<jumps_calls_op>(opcode);

				output->op = BRUJumpsCalls[uint32_t(jumps_calls.subtype)];
				output->operands[0] = uint32_t(jumps_calls.subtype) > 1 ? pc + extendSign(uint32_t(jumps_calls.label), 14) * 2 //relative
												  : uint32_t(jumps_calls.label) * 2u; //absolute
			}
			else if (branching.category == branch_categories::RET)
			{
				output->op = AR_OPCODE_RET;
			}
			else // illegal
			{
				return 0;
			}
		}
		else // illegal
		{
			return 0;
		}
	}
	else if(opcode.type == BRU_types::CMPI)
	{
		const CMPI cmpi = bit_cast<CMPI>(opcode);

		output->op = AR_OPCODE_CMPI;
		output->size = cmpi.size;
		output->operands[0] = cmpi.immediate;
		output->operands[1] = cmpi.source;
	}
	else if (opcode.type == BRU_types::FCMPI)
	{
		const FCMPI fcmpi = bit_cast<FCMPI>(opcode);

		output->op = AR_OPCODE_FCMPI;
		output->operands[0] = fcmpi.immediate;
		output->operands[1] = fcmpi.source;
	}
	else if (opcode.type == BRU_types::DCMPI)
	{
		const DCMPI dcmpi = bit_cast<DCMPI>(opcode);

		output->op = AR_OPCODE_DCMPI;
		output->operands[0] = dcmpi.immediate;
		output->operands[1] = dcmpi.source;
	}
	else // illegal
	{
		return 0;
	}

	return 1;
}

constexpr int decodeLSU(LSU_op opcode, ArOperation* output)
{
	if (opcode.type == LSU_types::DSRAM_load_store)
	{
		const DSRAM_load_store_op mls = bit_cast<DSRAM_load_store_op>(opcode);

		output->data = mls.incrementation;
		output->op = mls.store ? AR_OPCODE_STM : AR_OPCODE_LDM;
		output->size = mls.size;
		output->operands[0] = mls.immediate;
		output->operands[1] = mls.source;
		output->operands[2] = mls.register_value;
	}
	else if(opcode.type == LSU_types::subtype)
	{
		const subtype_op subtype = bit_cast<subtype_op>(opcode);

		if(subtype.subtype == subtype_types::DSRAM_load_store_extended)
		{
			const DSRAM_ext dsram_ext = bit_cast<DSRAM_ext>(opcode);

			output->op   = dsram_ext.store ? AR_OPCODE_STMX : AR_OPCODE_LDMX;
			output->size = dsram_ext.size;
			output->operands[0] = dsram_ext.immediate;
			output->operands[1] = dsram_ext.source + 62; // + 62?
			output->operands[2] = dsram_ext.register_value;
		}
		else if(subtype.subtype == subtype_types::in_out)
		{
			const INOUT inout = bit_cast<INOUT>(opcode);

			output->op   = inout.store ? AR_OPCODE_OUT : AR_OPCODE_IN;
			output->size = inout.size;
			output->operands[0] = inout.address;
			output->operands[2] = inout.register_value;
		}
		else if(subtype.subtype == subtype_types::outi)
		{
			const OUTI outi = bit_cast<OUTI>(opcode);

			output->op   = AR_OPCODE_OUTI;
			output->size = outi.size;
			output->operands[0] = outi.address;
			output->operands[2] = outi.register_value;
		}
		else if (subtype.subtype == subtype_types::vector_DSRAM_or_cache_load_store)
		{
			const VDSRAM vdsram = bit_cast<VDSRAM>(opcode);

			if(vdsram.cache)
			{
				output->data = vdsram.incrementation;
				output->op = vdsram.store ? AR_OPCODE_STCV : AR_OPCODE_LDCV;
				output->operands[0] = vdsram.base_address;
				output->operands[1] = vdsram.source + 56;
				output->operands[2] = vdsram.register_value;
			}
			else
			{
				output->data = vdsram.incrementation;
				output->op = vdsram.store ? AR_OPCODE_STMV : AR_OPCODE_LDMV;
				output->operands[0] = vdsram.base_address;
				output->operands[1] = vdsram.source + 56;
				output->operands[2] = vdsram.register_value;
			}
		}
		else // illegal
		{
			return 0;
		}
	}
	else if(opcode.type == LSU_types::cache_load_store)
	{
		const cache_load_store cls = bit_cast<cache_load_store>(opcode);

		output->data = cls.incrementation;
		output->op   = cls.store ? AR_OPCODE_STC : AR_OPCODE_LDC;
		output->size = cls.size;
		output->operands[0] = cls.immediate;
		output->operands[1] = cls.source;
		output->operands[2] = cls.register_value;
	}
	else if (opcode.type == LSU_types::float_DSRAM_or_cache_load_store)
	{
		const FDSRAM_or_cache_load_store_op FDSRAM_or_cache_load_store = bit_cast<FDSRAM_or_cache_load_store_op>(opcode);

		if(FDSRAM_or_cache_load_store.size == floating_load_store_sizes::float_size)
		{
			const float_DSRAM_or_cache_load_store mf = bit_cast<float_DSRAM_or_cache_load_store>(opcode);

			if(mf.cache == load_store_cache::SDRAM)
			{
				output->data = mf.incrementation;
				output->op = uint32_t(mf.store) ? AR_OPCODE_STCF : AR_OPCODE_LDCF;
				output->operands[0] = mf.base_address;
				output->operands[1] = mf.source + 60;
				output->operands[2] = mf.register_value;
			}
			else
			{
				output->data = mf.incrementation;
				output->op = uint32_t(mf.store) ? AR_OPCODE_STMF : AR_OPCODE_LDMF;
				output->operands[0] = mf.base_address;
				output->operands[1] = mf.source + 60;
				output->operands[2] = mf.register_value;
			}
		}
		else if (FDSRAM_or_cache_load_store.size == floating_load_store_sizes::double_size)
		{
			const double_DSRAM_or_cache_load_store md = bit_cast<double_DSRAM_or_cache_load_store>(opcode);

			if(md.cache == load_store_cache::SDRAM)
			{
				output->data = md.incrementation;
				output->op = uint32_t(md.store) ? AR_OPCODE_STCD : AR_OPCODE_LDCD;
				output->operands[0] = md.base_address;
				output->operands[1] = md.source + 60;
				output->operands[2] = md.register_value;
			}
			else
			{
				output->data = md.incrementation;
				output->op = uint32_t(md.store) ? AR_OPCODE_STMD : AR_OPCODE_LDMD;
				output->operands[0] = md.base_address;
				output->operands[1] = md.source + 60;
				output->operands[2] = md.register_value;
			}
		}
		else // illegal
		{
			return 0;
		}
	}
	else // illegal
	{
		return 0;
	}

	return 1;
}

constexpr ArOpcode ALURegRegRegOpcodes[16] =
{
	AR_OPCODE_ADD,
	AR_OPCODE_SUB,
	AR_OPCODE_MULS,
	AR_OPCODE_MULU,
	AR_OPCODE_DIVS,
	AR_OPCODE_DIVU,
	AR_OPCODE_AND,
	AR_OPCODE_OR,
	AR_OPCODE_XOR,
	AR_OPCODE_ASL,
	AR_OPCODE_LSL,
	AR_OPCODE_ASR,
	AR_OPCODE_LSR,
	AR_OPCODE_UNKNOWN,
	AR_OPCODE_UNKNOWN,
	AR_OPCODE_UNKNOWN,
};

constexpr ArOpcode ALURegRegImmOpcodes[16] =
{
	AR_OPCODE_ADDI,
	AR_OPCODE_SUBI,
	AR_OPCODE_MULSI,
	AR_OPCODE_MULUI,
	AR_OPCODE_DIVSI,
	AR_OPCODE_DIVUI,
	AR_OPCODE_ANDI,
	AR_OPCODE_ORI,
	AR_OPCODE_XORI,
	AR_OPCODE_ASLI,
	AR_OPCODE_LSLI,
	AR_OPCODE_ASRI,
	AR_OPCODE_LSRI,
	AR_OPCODE_UNKNOWN,
	AR_OPCODE_UNKNOWN,
	AR_OPCODE_UNKNOWN,
};

constexpr ArOpcode ALURegImmOpcodes[16] =
{
	AR_OPCODE_ADDQ,
	AR_OPCODE_SUBQ,
	AR_OPCODE_MULSQ,
	AR_OPCODE_MULUQ,
	AR_OPCODE_DIVSQ,
	AR_OPCODE_DIVUQ,
	AR_OPCODE_ANDQ,
	AR_OPCODE_ORQ,
	AR_OPCODE_XORQ,
	AR_OPCODE_ASLQ,
	AR_OPCODE_LSLQ,
	AR_OPCODE_ASRQ,
	AR_OPCODE_LSRQ,
	AR_OPCODE_UNKNOWN,
	AR_OPCODE_UNKNOWN,
	AR_OPCODE_UNKNOWN,
};

constexpr int decodeALU(ALU_op opcode, ArOperation* output)
{
	if(opcode.category == ALU_categories::NOP_XCHG_REGREGREG)
	{
		const NOP_and_XCHG_op NOP_and_XCHG = bit_cast<NOP_and_XCHG_op>(opcode);

		if(NOP_and_XCHG.type == NOP_and_XCHG_types::REGREGREG) // ALU REG-REG-REG (ADD, SUB, ...)
		{
			const REGREGREG regregreg = bit_cast<REGREGREG>(opcode);

			output->op = ALURegRegRegOpcodes[uint32_t(regregreg.op)];
			output->size = regregreg.size;
			output->operands[0] = regregreg.source1;
			output->operands[1] = regregreg.source2;
			output->operands[2] = regregreg.destination;

			if(output->op == AR_OPCODE_UNKNOWN)
			{
				return 0;
			}
		}
		else if(NOP_and_XCHG.type == NOP_and_XCHG_types::XCHG) // XCHG
		{
			output->op = AR_OPCODE_XCHG;
		}
		else if(NOP_and_XCHG.type == NOP_and_XCHG_types::NOP) // NOP NOP.E
		{
			const NOP nop = bit_cast<NOP>(opcode);

			output->data = nop.end;
			output->op   = AR_OPCODE_NOP;
		}
		else // illegal
		{
			return 0;
		}
	}
	else if (opcode.category == ALU_categories::REGREGIMM) // ALU REG-REG-IMM (ADDI, SUBI, ...)
	{
		const REGREGIMM regregimm = bit_cast<REGREGIMM>(opcode);

		output->op = ALURegRegImmOpcodes[uint32_t(regregimm.op)];
		output->size = regregimm.size;
		output->operands[0] = regregimm.immediate;
		output->operands[1] = regregimm.source2;
		output->operands[2] = regregimm.destination;

		if(output->op == AR_OPCODE_UNKNOWN)
		{
			return 0;
		}
	}
	else if (opcode.category == ALU_categories::REGIMM) // ALU REG-IMM (ADDQ, SUBQ, ...)
	{
		const REGIMM regimm = bit_cast<REGIMM>(opcode);

		output->op   = ALURegImmOpcodes[uint32_t(regimm.op)];
		output->size = regimm.size;
		output->operands[0] = regimm.immediate;
		output->operands[2] = regimm.destination;

		if(output->op == AR_OPCODE_UNKNOWN)
		{
			return 0;
		}
	}
	else if (opcode.category == ALU_categories::MOVEI) // MOVEI
	{
		const MOVEI movei = bit_cast<MOVEI>(opcode);

		output->op = AR_OPCODE_MOVEI;
		output->operands[0] = movei.immediate;
		output->operands[2] = movei.destination;
	}
	else // illegal
	{
		return 0;
	}


	return 1;
}

constexpr int decodeAGU(AGU_op opcode, ArOperation* output)
{
	if(opcode.category == AGU_categories::direct_load_store)
	{
		const direct_load_store_op dls = bit_cast<direct_load_store_op>(opcode);

		output->op = dls.store ? AR_OPCODE_STDMA : AR_OPCODE_LDDMA;
		output->size = dls.size;
		output->operands[0] = dls.DSRAM_address + 60;
		output->operands[1] = dls.RAM_address + 58;
		output->data = (dls.RAM_base << 12u) | dls.DSRAM_base;
	}
	else if (opcode.category == AGU_categories::load_store_list)
	{
		const load_store_list_op lsl = bit_cast<load_store_list_op>(opcode);

		if(lsl.type == load_store_list_types::LDDMAR_STDMAR) //LDDMAR/STDMAR
		{
			const LDDMAR_STDMAR_op LDDMAR_STDMAR = bit_cast<LDDMAR_STDMAR_op>(opcode);

			output->op = LDDMAR_STDMAR.store ? AR_OPCODE_STDMAR : AR_OPCODE_LDDMAR;
			output->size = LDDMAR_STDMAR.size;
			output->operands[0] = LDDMAR_STDMAR.RAM_address;
			output->operands[1] = LDDMAR_STDMAR.DSRAM_address;
		}
		else if(lsl.type == load_store_list_types::DMAIR) //DMAIR
		{
			const DMAIR dmair = bit_cast<DMAIR>(opcode);

			output->op = AR_OPCODE_DMAIR;
			output->size = dmair.size;
			output->operands[0] = dmair.RAM_address;
			output->operands[1] = dmair.DSRAM_address;
		}
		else if(lsl.type == load_store_list_types::WAIT) //WAIT
		{
			output->op = AR_OPCODE_WAIT;
		}
		else // illegal
		{
			return 0;
		}
	}
	else // illegal
	{
		return 0;
	}

	return 1;
}

constexpr int decodeVFPU(VFPU_op opcode, ArOperation* output)
{
	if (opcode.type == VFPU_types::subtype)
	{
		const VFPU_subtype subtype = bit_cast<VFPU_subtype>(opcode);

		if (subtype.subtype == VFPU_subtype_types::arithmetic)
		{
			const arithmetic_op arithmetic = bit_cast<arithmetic_op>(opcode);

			if (arithmetic.category == arithmetic_categories::vector_vector)
			{
				const vector_vector_op vector_vector = bit_cast<vector_vector_op>(opcode);

				throw std::runtime_error("not implemented yet");
			}
			else if (arithmetic.category == arithmetic_categories::vector_float)
			{
				const vector_float_op vector_float = bit_cast<vector_float_op>(opcode);

				throw std::runtime_error("not implemented yet");
			}
			else if (arithmetic.category == arithmetic_categories::vector_accumulator)
			{
				const vector_accumulator_op vector_accumulator = bit_cast<vector_accumulator_op>(opcode);

				throw std::runtime_error("not implemented yet");
			}
			else if (arithmetic.category == arithmetic_categories::double_double)
			{
				const double_double_op double_double = bit_cast<double_double_op>(opcode);

				throw std::runtime_error("not implemented yet");
			}
			else // illegal
			{
				return 0;
			}
		}
		else if (subtype.subtype == VFPU_subtype_types::moves)
		{
			const arithmetic_op moves = bit_cast<arithmetic_op>(opcode);

			// To be defined
			throw std::runtime_error("not implemented yet");
		}
		else if (subtype.subtype == VFPU_subtype_types::float_conversion)
		{
			const float_conversion_op float_conversion = bit_cast<float_conversion_op>(opcode);

			throw std::runtime_error("not implemented yet");
		}
		else if (subtype.subtype == VFPU_subtype_types::float_int_conversion_or_VDIV)
		{
			const float_int_conversion_or_VDIV_op float_int_conversion_or_VDIV = bit_cast<float_int_conversion_or_VDIV_op>(opcode);

			if (float_int_conversion_or_VDIV.operation == float_int_conversion_or_VDIV_operations::fixed_point_conversion)
			{
				const fixed_point_conversion_op fpc = bit_cast<fixed_point_conversion_op>(opcode);

				throw std::runtime_error("not implemented yet");
			}
			else if (float_int_conversion_or_VDIV.operation == float_int_conversion_or_VDIV_operations::float_int_conversion)
			{
				const float_int_conversion_op fic = bit_cast<float_int_conversion_op>(opcode);

				throw std::runtime_error("not implemented yet");
			}
			else if (float_int_conversion_or_VDIV.operation == float_int_conversion_or_VDIV_operations::double_int_conversion)
			{
				const double_int_conversion_op dic = bit_cast<double_int_conversion_op>(opcode);

				throw std::runtime_error("not implemented yet");
			}
			else if (float_int_conversion_or_VDIV.operation == float_int_conversion_or_VDIV_operations::float_double_division_and_square_root)
			{
				const float_double_division_and_square_root_op fddiv_sqrt = bit_cast<float_double_division_and_square_root_op>(opcode);

				throw std::runtime_error("not implemented yet");
			}
			else // illegal
			{
				return 0;
			}
		}
		else // illegal
		{
			return 0;
		}
	}
	else if (opcode.type == VFPU_types::MOVEFI)
	{
		const MOVEFI movefi = bit_cast<MOVEFI>(opcode);

		throw std::runtime_error("not implemented yet");
	}
	else if (opcode.type == VFPU_types::MOVEDI)
	{
		const MOVEDI movedi = bit_cast<MOVEDI>(opcode);

		throw std::runtime_error("not implemented yet");
	}
	else if (opcode.type == VFPU_types::MOVEVI)
	{
		const MOVEVI movevi = bit_cast<MOVEVI>(opcode);

		throw std::runtime_error("not implemented yet");
	}
	else // illegal
	{
		return 0;
	}

	return 1;
}

constexpr int decode(uint32_t index, uint32_t pc, opcode opcode, ArOperation* output)
{
	if (index == 0)
	{
		if (opcode.compute_unit == compute_units::BRU)
		{
			return decodeBRU(pc, bit_cast<BRU_op>(opcode), output);
		}
		else if (opcode.compute_unit == compute_units::LSU)
		{
			return decodeLSU(bit_cast<LSU_op>(opcode), output);
		}
		else if (opcode.compute_unit == compute_units::ALU)
		{
			return decodeALU(bit_cast<ALU_op>(opcode), output);
		}
		else if (opcode.compute_unit == compute_units::VFPU) //VDIV
		{
			return decodeVFPU(bit_cast<VFPU_op>(opcode), output);
		}
	}
	else if (index == 1)
	{
		if (opcode.compute_unit == compute_units::AGU)
		{
			return decodeAGU(bit_cast<AGU_op>(opcode), output);
		}
		else if (opcode.compute_unit == compute_units::LSU)
		{
			return decodeLSU(bit_cast<LSU_op>(opcode), output);
		}
		else if (opcode.compute_unit == compute_units::ALU)
		{
			return decodeALU(bit_cast<ALU_op>(opcode), output);
		}
		else if (opcode.compute_unit == compute_units::VFPU)
		{
			return decodeVFPU(bit_cast<VFPU_op>(opcode), output);
		}
	}
	else //2 or 3
	{
		if (opcode.compute_unit == compute_units::ALU)
		{
			return decodeALU(bit_cast<ALU_op>(opcode), output);
		}
	}

	return 0;
}


ArResult arDecodeInstruction(ArProcessor processor)
{
	assert(processor);

	const uint32_t size = opcodeSetSize(processor);
	memcpy(processor->opcodes, processor->isram + (processor->pc * 4ull), size * sizeof(uint32_t));

	for(uint32_t i = 0; i < size; ++i)
	{
		if (!decode(i, processor->pc, bit_cast<opcode>(processor->opcodes[i]), &processor->operations[i]))
		{
			return AR_ERROR_ILLEGAL_INSTRUCTION;
		}
	}

	processor->pc += size;

	return AR_SUCCESS;
}
