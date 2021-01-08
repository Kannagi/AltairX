#ifndef ALU_HPP_INCLUDED
#define ALU_HPP_INCLUDED

#include "ops.hpp"

namespace ar
{

namespace op
{

enum class ALU_categories : uint32_t
{
	NOP_XCHG_REGREGREG = 0,
	REGREGIMM          = 1,
	REGIMM             = 2,
	MOVEI              = 3,
};

struct ALU_op
{
	compute_units compute_unit     : 2;
	ALU_categories category        : 2;
	uint32_t                       : 28;
};
static_assert(sizeof(ALU_op) == sizeof(uint32_t));

enum class NOP_and_XCHG_types : uint32_t
{
	REGREGREG = 0,
	// illegal   = 1,
	XCHG      = 2,
	// illegal   = 3,
	// illegal   = 4,
	// illegal   = 5,
	NOP       = 6,
	// illegal   = 7,
};

struct NOP_and_XCHG_op
{
	compute_units compute_unit     : 2;
	ALU_categories category        : 2;
	NOP_and_XCHG_types type        : 3;
	uint32_t                       : 25;
};
static_assert(sizeof(NOP_and_XCHG_op) == sizeof(uint32_t));

struct XCHG
{
	compute_units compute_unit     : 2 = compute_units::ALU;
	ALU_categories category        : 2 = ALU_categories::NOP_XCHG_REGREGREG;
	NOP_and_XCHG_types type        : 3 = NOP_and_XCHG_types::XCHG;
	uint32_t                       : 25;
};
static_assert(sizeof(XCHG) == sizeof(uint32_t));

struct NOP
{
	compute_units compute_unit     : 2 = compute_units::ALU;
	ALU_categories category        : 2 = ALU_categories::NOP_XCHG_REGREGREG;
	NOP_and_XCHG_types type        : 3 = NOP_and_XCHG_types::NOP;
	uint32_t end                   : 1;
	uint32_t                       : 24;
};
static_assert(sizeof(NOP) == sizeof(uint32_t));

enum class arithmetic_bitwise_op : uint32_t
{
	addition              = 0,
	substraction          = 1,
	multiplication_signed = 2,
	multiplication        = 3,
	division_signed       = 4,
	division              = 5,
	bitwise_and           = 6,
	bitwise_or            = 7,
	bitwise_xor           = 8,
	arithmetic_left_shift = 9,
	logic_left_shift      = 10,
	arithmetic_right_shift= 11,
	logic_right_shift     = 12,
	// illegal               = 13,
	// illegal               = 14,
	// illegal               = 15,
};

struct REGREGREG
{
	compute_units compute_unit     : 2 = compute_units::ALU;
	ALU_categories category        : 2 = ALU_categories::NOP_XCHG_REGREGREG;
	NOP_and_XCHG_types type        : 4 = NOP_and_XCHG_types::REGREGREG;
	arithmetic_bitwise_op op       : 4;
	uint32_t size                  : 2;
	uint32_t source1               : 6;
	uint32_t source2               : 6;
	uint32_t destination           : 6;
};
static_assert(sizeof(REGREGREG) == sizeof(uint32_t));

struct REGREGIMM
{
	compute_units compute_unit     : 2 = compute_units::ALU;
	ALU_categories category        : 2 = ALU_categories::REGREGIMM;
	arithmetic_bitwise_op op       : 4;
	uint32_t size                  : 2;
	uint32_t immediate             : 10;
	uint32_t source2               : 6;
	uint32_t destination           : 6;
};
static_assert(sizeof(REGREGIMM) == sizeof(uint32_t));

struct REGIMM
{
	compute_units compute_unit     : 2 = compute_units::ALU;
	ALU_categories category        : 2 = ALU_categories::REGIMM;
	arithmetic_bitwise_op op       : 4;
	uint32_t size                  : 2;
	uint32_t immediate             : 16;
	uint32_t destination           : 6;
};
static_assert(sizeof(REGIMM) == sizeof(uint32_t));

struct MOVEI
{
	compute_units compute_unit     : 2 = compute_units::ALU;
	ALU_categories category        : 2 = ALU_categories::MOVEI;
	uint32_t immediate             : 22;
	uint32_t destination           : 6;
};
static_assert(sizeof(MOVEI) == sizeof(uint32_t));

} // namespace op

} // namespace ar

#endif // ALU_HPP_INCLUDED
