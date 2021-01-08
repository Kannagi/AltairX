#ifndef BRU_HPP_INCLUDED
#define BRU_HPP_INCLUDED

#include "ops.hpp"

namespace ar
{

namespace op
{

enum class BRU_types : uint32_t
{
	REGREG_branch = 0,
	CMPI           = 1,
	FCMPI          = 2,
	DCMPI          = 3,
};

struct BRU_op
{
	compute_units compute_unit : 2;
	BRU_types type             : 2;
	uint32_t                   : 28;
};
static_assert(sizeof(BRU_op) == sizeof(uint32_t));

// REG-REG

enum class REGREG_branch_instructions : uint32_t
{
	CMP       = 0,
	FCMP      = 1,
	DCMP      = 2,
	branching = 3,
};

struct REGREG_branch_op
{
	compute_units compute_unit : 2;
	BRU_types type             : 2;
	REGREG_branch_instructions instruction
	                           : 2;
	uint32_t                   : 26;
};
static_assert(sizeof(REGREG_branch_op) == sizeof(uint32_t));

struct CMP
{
	compute_units compute_unit : 2 = compute_units::BRU;
	BRU_types type             : 2 = BRU_types::REGREG_branch;
	REGREG_branch_instructions instruction
	                           : 2 = REGREG_branch_instructions::CMP;
	uint32_t size              : 2;
	uint32_t source1           : 5;
	uint32_t source2           : 5;
};
static_assert(sizeof(CMP) == sizeof(uint32_t));

struct FCMP
{
	compute_units compute_unit : 2 = compute_units::BRU;
	BRU_types type             : 2 = BRU_types::REGREG_branch;
	REGREG_branch_instructions instruction
	                           : 2 = REGREG_branch_instructions::FCMP;
	uint32_t source1           : 7;
	uint32_t source2           : 7;
};
static_assert(sizeof(FCMP) == sizeof(uint32_t));

struct DCMP
{
	compute_units compute_unit : 2 = compute_units::BRU;
	BRU_types type             : 2 = BRU_types::REGREG_branch;
	REGREG_branch_instructions instruction
	                           : 2 = REGREG_branch_instructions::DCMP;
	uint32_t source1           : 6;
	uint32_t source2           : 6;
};
static_assert(sizeof(DCMP) == sizeof(uint32_t));

// Branching

// Branch

enum class branch_categories : uint32_t
{
	branch      = 0,
	// illegal     = 1,
	jumps_calls = 2,
	RET         = 3,
};

struct branching_op
{
	compute_units compute_unit : 2;
	BRU_types type             : 2;
	REGREG_branch_instructions instruction
	                           : 2;
	branch_categories category : 2;
	uint32_t                   : 24;
};
static_assert(sizeof(branching_op) == sizeof(uint32_t));

enum class branch_comparators : uint32_t
{
	not_equal               = 0,
	equal                   = 1,
	less                    = 2,
	less_or_equal           = 3,
	greater                 = 4,
	greater_or_equal        = 5,
	less_signed             = 6,
	less_or_equal_signed    = 7,
	greater_signed          = 8,
	greater_or_equal_signed = 9,
	// illegal                 = 10,
	// illegal                 = 11,
	// illegal                 = 12,
	// illegal                 = 13,
	// illegal                 = 14,
	// illegal                 = 15,
};

struct branch_op
{
	compute_units compute_unit : 2 = compute_units::BRU;
	BRU_types type : 2 = BRU_types::REGREG_branch;
	REGREG_branch_instructions instruction
		: 2 = REGREG_branch_instructions::branching;
	branch_categories category : 2 = branch_categories::branch;
	branch_comparators comparator
		: 4 = branch_comparators::not_equal;
	uint32_t label : 14;
};
static_assert(sizeof(branch_op) == sizeof(uint32_t));

struct BNE
{
	compute_units compute_unit : 2 = compute_units::BRU;
	BRU_types type             : 2 = BRU_types::REGREG_branch;
	REGREG_branch_instructions instruction
	                           : 2 = REGREG_branch_instructions::branching;
	branch_categories category : 2 = branch_categories::branch;
	branch_comparators comparator
	                           : 4 = branch_comparators::not_equal;
	uint32_t label             : 14;
};
static_assert(sizeof(BNE) == sizeof(uint32_t));

struct BEQ
{
	compute_units compute_unit : 2 = compute_units::BRU;
	BRU_types type             : 2 = BRU_types::REGREG_branch;
	REGREG_branch_instructions instruction
	                           : 2 = REGREG_branch_instructions::branching;
	branch_categories category : 2 = branch_categories::branch;
	branch_comparators comparator
	                           : 4 = branch_comparators::equal;
	uint32_t label             : 14;
};
static_assert(sizeof(BEQ) == sizeof(uint32_t));

struct BL
{
	compute_units compute_unit : 2 = compute_units::BRU;
	BRU_types type             : 2 = BRU_types::REGREG_branch;
	REGREG_branch_instructions instruction
	                           : 2 = REGREG_branch_instructions::branching;
	branch_categories category : 2 = branch_categories::branch;
	branch_comparators comparator
	                           : 4 = branch_comparators::less;
	uint32_t label             : 14;
};
static_assert(sizeof(BL) == sizeof(uint32_t));

struct BLE
{
	compute_units compute_unit : 2 = compute_units::BRU;
	BRU_types type             : 2 = BRU_types::REGREG_branch;
	REGREG_branch_instructions instruction
	                           : 2 = REGREG_branch_instructions::branching;
	branch_categories category : 2 = branch_categories::branch;
	branch_comparators comparator
	                           : 4 = branch_comparators::less_or_equal;
	uint32_t label             : 14;
};
static_assert(sizeof(BLE) == sizeof(uint32_t));

struct BG
{
	compute_units compute_unit : 2 = compute_units::BRU;
	BRU_types type             : 2 = BRU_types::REGREG_branch;
	REGREG_branch_instructions instruction
	                           : 2 = REGREG_branch_instructions::branching;
	branch_categories category : 2 = branch_categories::branch;
	branch_comparators comparator
	                           : 4 = branch_comparators::greater;
	uint32_t label             : 14;
};
static_assert(sizeof(BG) == sizeof(uint32_t));

struct BGE
{
	compute_units compute_unit : 2 = compute_units::BRU;
	BRU_types type             : 2 = BRU_types::REGREG_branch;
	REGREG_branch_instructions instruction
	                           : 2 = REGREG_branch_instructions::branching;
	branch_categories category : 2 = branch_categories::branch;
	branch_comparators comparator
	                           : 4 = branch_comparators::greater_or_equal;
	uint32_t label             : 14;
};
static_assert(sizeof(BGE) == sizeof(uint32_t));

struct BLS
{
	compute_units compute_unit : 2 = compute_units::BRU;
	BRU_types type             : 2 = BRU_types::REGREG_branch;
	REGREG_branch_instructions instruction
	                           : 2 = REGREG_branch_instructions::branching;
	branch_categories category : 2 = branch_categories::branch;
	branch_comparators comparator
	                           : 4 = branch_comparators::less_signed;
	uint32_t label             : 14;
};
static_assert(sizeof(BLS) == sizeof(uint32_t));

struct BLES
{
	compute_units compute_unit : 2 = compute_units::BRU;
	BRU_types type             : 2 = BRU_types::REGREG_branch;
	REGREG_branch_instructions instruction
	                           : 2 = REGREG_branch_instructions::branching;
	branch_categories category : 2 = branch_categories::branch;
	branch_comparators comparator
	                           : 4 = branch_comparators::less_or_equal_signed;
	uint32_t label             : 14;
};
static_assert(sizeof(BLES) == sizeof(uint32_t));

struct BGS
{
	compute_units compute_unit : 2 = compute_units::BRU;
	BRU_types type             : 2 = BRU_types::REGREG_branch;
	REGREG_branch_instructions instruction
	                           : 2 = REGREG_branch_instructions::branching;
	branch_categories category : 2 = branch_categories::branch;
	branch_comparators comparator
	                           : 4 = branch_comparators::greater_signed;
	uint32_t label             : 14;
};
static_assert(sizeof(BGS) == sizeof(uint32_t));

struct BGES
{
	compute_units compute_unit : 2 = compute_units::BRU;
	BRU_types type             : 2 = BRU_types::REGREG_branch;
	REGREG_branch_instructions instruction
	                           : 2 = REGREG_branch_instructions::branching;
	branch_categories category : 2 = branch_categories::branch;
	branch_comparators comparator
	                           : 4 = branch_comparators::greater_or_equal_signed;
	uint32_t label             : 14;
};
static_assert(sizeof(BGES) == sizeof(uint32_t));

// Float Branch

enum class jumps_calls_subtypes : uint32_t
{
	call          = 0,
	jump          = 1,
	call_relative = 2,
	jump_relative = 3,
};

struct jumps_calls_op
{
	compute_units compute_unit : 2 = compute_units::BRU;
	BRU_types type             : 2 = BRU_types::REGREG_branch;
	REGREG_branch_instructions instruction
	                           : 2 = REGREG_branch_instructions::branching;
	branch_categories category : 2 = branch_categories::jumps_calls;
	jumps_calls_subtypes subtype
	                           : 2;
	uint32_t                   : 2;
	uint32_t label             : 14;
	uint32_t                   : 6;
};
static_assert(sizeof(jumps_calls_op) == sizeof(uint32_t));

struct CALL
{
	compute_units compute_unit : 2 = compute_units::BRU;
	BRU_types type             : 2 = BRU_types::REGREG_branch;
	REGREG_branch_instructions instruction
	                           : 2 = REGREG_branch_instructions::branching;
	branch_categories category : 2 = branch_categories::jumps_calls;
	jumps_calls_subtypes subtype
	                           : 2 = jumps_calls_subtypes::call;
	uint32_t                   : 2;
	uint32_t label             : 14;
	uint32_t                   : 6;
};
static_assert(sizeof(CALL) == sizeof(uint32_t));

struct JMP
{
	compute_units compute_unit : 2 = compute_units::BRU;
	BRU_types type             : 2 = BRU_types::REGREG_branch;
	REGREG_branch_instructions instruction
	                           : 2 = REGREG_branch_instructions::branching;
	branch_categories category : 2 = branch_categories::jumps_calls;
	jumps_calls_subtypes subtype
	                           : 2 = jumps_calls_subtypes::jump;
	uint32_t                   : 2;
	uint32_t label             : 14;
	uint32_t                   : 6;
};
static_assert(sizeof(JMP) == sizeof(uint32_t));

struct CALLR
{
	compute_units compute_unit : 2 = compute_units::BRU;
	BRU_types type             : 2 = BRU_types::REGREG_branch;
	REGREG_branch_instructions instruction
	                           : 2 = REGREG_branch_instructions::branching;
	branch_categories category : 2 = branch_categories::jumps_calls;
	jumps_calls_subtypes subtype
	                           : 2 = jumps_calls_subtypes::call_relative;
	uint32_t                   : 2;
	uint32_t label             : 14;
	uint32_t                   : 6;
};
static_assert(sizeof(CALLR) == sizeof(uint32_t));

struct JMPR
{
	compute_units compute_unit : 2 = compute_units::BRU;
	BRU_types type             : 2 = BRU_types::REGREG_branch;
	REGREG_branch_instructions instruction
	                           : 2 = REGREG_branch_instructions::branching;
	branch_categories category : 2 = branch_categories::jumps_calls;
	jumps_calls_subtypes subtype
	                           : 2 = jumps_calls_subtypes::jump_relative;
	uint32_t                   : 2;
	uint32_t label             : 14;
	uint32_t                   : 6;
};
static_assert(sizeof(JMPR) == sizeof(uint32_t));

struct RET
{
	compute_units compute_unit : 2 = compute_units::BRU;
	BRU_types type             : 2 = BRU_types::REGREG_branch;
	REGREG_branch_instructions instruction
	                           : 2 = REGREG_branch_instructions::branching;
	branch_categories category : 2 = branch_categories::RET;
	uint32_t                   : 24;
};
static_assert(sizeof(RET) == sizeof(uint32_t));

struct CMPI
{
	compute_units compute_unit : 2 = compute_units::BRU;
	BRU_types type             : 2 = BRU_types::CMPI;
	uint32_t size              : 2;
	uint32_t immediate         : 20;
	uint32_t source            : 6;
};
static_assert(sizeof(CMPI) == sizeof(uint32_t));

struct FCMPI
{
	compute_units compute_unit : 2 = compute_units::BRU;
	BRU_types type             : 2 = BRU_types::FCMPI;
	uint32_t immediate         : 21;
	uint32_t source            : 7;
};
static_assert(sizeof(FCMPI) == sizeof(uint32_t));

struct DCMPI
{
	compute_units compute_unit : 2 = compute_units::BRU;
	BRU_types type             : 2 = BRU_types::DCMPI;
	uint32_t immediate         : 22;
	uint32_t source            : 6;
};
static_assert(sizeof(DCMPI) == sizeof(uint32_t));

} // namespace op

} // namespace ar

#endif // BRU_HPP_INCLUDED
