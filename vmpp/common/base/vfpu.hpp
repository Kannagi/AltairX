#ifndef VFPU_HPP_INCLUDED
#define VFPU_HPP_INCLUDED

#include "ops.hpp"

namespace ar
{

namespace op
{

enum class VFPU_types : uint32_t
{
	subtype = 0,
	MOVEFI  = 1,
	MOVEDI  = 2,
	MOVEVI  = 3,
};

struct VFPU_op
{
	compute_units compute_unit     : 2;
	VFPU_types type                : 2;
	uint32_t                       : 28;
};
static_assert(sizeof(VFPU_op) == sizeof(uint32_t));

enum class VFPU_subtype_types : uint32_t
{
	arithmetic                   = 0,
	moves                        = 1,
	float_conversion             = 2,
	float_int_conversion_or_VDIV = 3,
};

struct VFPU_subtype
{
	compute_units compute_unit     : 2 = compute_units::VFPU;
	VFPU_types type                : 2 = VFPU_types::subtype;
	VFPU_subtype_types subtype     : 2;
	uint32_t                       : 26;
};
static_assert(sizeof(VFPU_subtype) == sizeof(uint32_t));

enum class arithmetic_categories : uint32_t
{
	vector_vector      = 0,
	vector_float       = 1,
	vector_accumulator = 2,
	double_double      = 3,
};

struct arithmetic
{
	compute_units compute_unit     : 2 = compute_units::VFPU;
	VFPU_types type                : 2 = VFPU_types::subtype;
	VFPU_subtype_types subtype     : 2 = VFPU_subtype_types::arithmetic;
	uint32_t                       : 2;
	arithmetic_categories category : 2;
	uint32_t                       : 22;
};
static_assert(sizeof(arithmetic) == sizeof(uint32_t));

enum class operations : uint32_t
{
	FADD    = 0,
	FSUB    = 1,
	FMUL    = 2,
	FMULADD = 3,
};

struct vector_vector_operation
{
	compute_units compute_unit     : 2 = compute_units::VFPU;
	VFPU_types type                : 2 = VFPU_types::subtype;
	VFPU_subtype_types subtype     : 2 = VFPU_subtype_types::arithmetic;
	uint32_t size                  : 2;
	arithmetic_categories category : 2 = arithmetic_categories::vector_vector;
	operations operation           : 2;
	uint32_t                       : 5;
	uint32_t source2               : 5;
	uint32_t source1               : 5;
	uint32_t destination           : 5;
};
static_assert(sizeof(vector_vector_operation) == sizeof(uint32_t));

struct vector_float_operation
{
	compute_units compute_unit     : 2 = compute_units::VFPU;
	VFPU_types type                : 2 = VFPU_types::subtype;
	VFPU_subtype_types subtype     : 2 = VFPU_subtype_types::arithmetic;
	uint32_t size                  : 2;
	arithmetic_categories category : 2 = arithmetic_categories::vector_float;
	operations operation           : 2;
	uint32_t                       : 3;
	uint32_t source2               : 7;
	uint32_t source1               : 5;
	uint32_t destination           : 5;
};
static_assert(sizeof(vector_float_operation) == sizeof(uint32_t));

enum class operations2 : uint32_t
{
	FMULVA     = 0,
	FMULADDVA  = 1,
	FMULADDVAO = 2,
	FIPR       = 3,
};

struct vector_accumulator_operation
{
	compute_units compute_unit     : 2 = compute_units::VFPU;
	VFPU_types type                : 2 = VFPU_types::subtype;
	VFPU_subtype_types subtype     : 2 = VFPU_subtype_types::arithmetic;
	uint32_t size                  : 2;
	arithmetic_categories category : 2 = arithmetic_categories::vector_accumulator;
	operations2 operation2         : 2;
	uint32_t                       : 3;
	uint32_t float_register        : 7;
	uint32_t vector1               : 5;
	uint32_t vector2               : 5;
};
static_assert(sizeof(vector_accumulator_operation) == sizeof(uint32_t));

struct double_double_operation
{
	compute_units compute_unit     : 2 = compute_units::VFPU;
	VFPU_types type                : 2 = VFPU_types::subtype;
	VFPU_subtype_types subtype     : 2 = VFPU_subtype_types::arithmetic;
	operations operation           : 2;
	arithmetic_categories category : 2 = arithmetic_categories::double_double;
	uint32_t                       : 4;
	uint32_t source2               : 6;
	uint32_t source1               : 6;
	uint32_t destination           : 6;
};
static_assert(sizeof(double_double_operation) == sizeof(uint32_t));

struct moves
{
	compute_units compute_unit     : 2 = compute_units::VFPU;
	VFPU_types type                : 2 = VFPU_types::subtype;
	VFPU_subtype_types subtype     : 2 = VFPU_subtype_types::moves;
	uint32_t                       : 26;
};
static_assert(sizeof(moves) == sizeof(uint32_t));

enum class float_conversion_directions : uint32_t
{
	float_to_double = 0,
	double_to_float = 1,
};

struct float_conversion
{
	compute_units compute_unit     : 2 = compute_units::VFPU;
	VFPU_types type                : 2 = VFPU_types::subtype;
	VFPU_subtype_types subtype     : 2 = VFPU_subtype_types::float_conversion;
	float_conversion_directions direction
	                               : 1;
	uint32_t                       : 12;
	uint32_t float_register        : 7;
	uint32_t double_register       : 6;
};
static_assert(sizeof(float_conversion) == sizeof(uint32_t));

enum class float_int_conversion_or_VDIV_operations : uint32_t
{
	fixed_point_conversion = 0,
	float_int_conversion = 1,
	double_int_conversion = 2,
	float_double_division_and_square_root = 3,
};

struct float_int_conversion_or_VDIV
{
	compute_units compute_unit     : 2 = compute_units::VFPU;
	VFPU_types type                : 2 = VFPU_types::subtype;
	VFPU_subtype_types subtype     : 2 = VFPU_subtype_types::float_int_conversion_or_VDIV;
	uint32_t                       : 2;
	float_int_conversion_or_VDIV_operations operation
	                               : 2;
	uint32_t float_register        : 7;
	uint32_t double_register       : 6;
};
static_assert(sizeof(float_int_conversion_or_VDIV) == sizeof(uint32_t));

enum class fixed_point_conversion_instructions : uint32_t
{
	ITOF0  = 0,
	ITOF4  = 1,
	ITOF8  = 2,
	ITOF15 = 3,
	FTOI0  = 4,
	FTOI4  = 5,
	FTOI8  = 6,
	FTOI15 = 7,
};

struct fixed_point_conversion
{
	compute_units compute_unit     : 2 = compute_units::VFPU;
	VFPU_types type                : 2 = VFPU_types::subtype;
	VFPU_subtype_types subtype     : 2 = VFPU_subtype_types::float_int_conversion_or_VDIV;
	uint32_t size                  : 2;
	float_int_conversion_or_VDIV_operations operation
	                               : 2 = float_int_conversion_or_VDIV_operations::fixed_point_conversion;
	uint32_t                       : 2;
	fixed_point_conversion_instructions instruction
	                               : 3;
	uint32_t                       : 6;
	uint32_t vector                : 5;
	uint32_t register_value        : 6;
};
static_assert(sizeof(fixed_point_conversion) == sizeof(uint32_t));

enum class float_int_conversion_directions : uint32_t
{
	register_to_float = 0,
	float_to_register = 1,
};

struct float_int_conversion
{
	compute_units compute_unit     : 2 = compute_units::VFPU;
	VFPU_types type                : 2 = VFPU_types::subtype;
	VFPU_subtype_types subtype     : 2 = VFPU_subtype_types::float_int_conversion_or_VDIV;
	uint32_t size                  : 2;
	float_int_conversion_or_VDIV_operations operation
	                               : 2 = float_int_conversion_or_VDIV_operations::float_int_conversion;
	float_int_conversion_directions direction
	                               : 1;
	uint32_t                       : 8;
	uint32_t float_register        : 7;
	uint32_t register_value        : 6;
};
static_assert(sizeof(float_int_conversion) == sizeof(uint32_t));

enum class double_int_conversion_directions : uint32_t
{
	register_to_double = 0,
	double_to_register = 1,
};

struct double_int_conversion
{
	compute_units compute_unit     : 2 = compute_units::VFPU;
	VFPU_types type                : 2 = VFPU_types::subtype;
	VFPU_subtype_types subtype     : 2 = VFPU_subtype_types::float_int_conversion_or_VDIV;
	uint32_t size                  : 2;
	float_int_conversion_or_VDIV_operations operation
	                               : 2 = float_int_conversion_or_VDIV_operations::double_int_conversion;
	double_int_conversion_directions direction
	                               : 1;
	uint32_t                       : 9;
	uint32_t double_register       : 6;
	uint32_t register_value        : 6;
};
static_assert(sizeof(double_int_conversion) == sizeof(uint32_t));

enum class float_double_division_and_square_root_instructions : uint32_t
{
	DIV  = 0,
	SQRT = 1,
};

enum class float_double_division_and_square_root_sizes : uint32_t
{
	float_size  = 0,
	double_size = 1,
};

struct float_double_division_and_square_root
{
	compute_units compute_unit     : 2 = compute_units::VFPU;
	VFPU_types type                : 2 = VFPU_types::subtype;
	VFPU_subtype_types subtype     : 2 = VFPU_subtype_types::float_int_conversion_or_VDIV;
	float_double_division_and_square_root_instructions instruction
	                               : 1;
	float_double_division_and_square_root_sizes size
	                               : 1;
	float_int_conversion_or_VDIV_operations operation
	                               : 2 = float_int_conversion_or_VDIV_operations::float_double_division_and_square_root;
	uint32_t                       : 22;
};
static_assert(sizeof(float_double_division_and_square_root) == sizeof(uint32_t));

struct FDIV_FSQRT
{
	compute_units compute_unit     : 2 = compute_units::VFPU;
	VFPU_types type                : 2 = VFPU_types::subtype;
	VFPU_subtype_types subtype     : 2 = VFPU_subtype_types::float_int_conversion_or_VDIV;
	float_double_division_and_square_root_instructions instruction
	                               : 1;
	float_double_division_and_square_root_sizes size
	                               : 1 = float_double_division_and_square_root_sizes::float_size;
	float_int_conversion_or_VDIV_operations operation
	                               : 2 = float_int_conversion_or_VDIV_operations::float_double_division_and_square_root;
	uint32_t                       : 1; /// TODO: what's this?
	uint32_t source1               : 7;
	uint32_t source2               : 7;
	uint32_t destination           : 7;
};
static_assert(sizeof(FDIV_FSQRT) == sizeof(uint32_t));

struct FDIV
{
	compute_units compute_unit     : 2 = compute_units::VFPU;
	VFPU_types type                : 2 = VFPU_types::subtype;
	VFPU_subtype_types subtype     : 2 = VFPU_subtype_types::float_int_conversion_or_VDIV;
	float_double_division_and_square_root_instructions instruction
	                               : 1 = float_double_division_and_square_root_instructions::DIV;
	float_double_division_and_square_root_sizes size
	                               : 1 = float_double_division_and_square_root_sizes::float_size;
	float_int_conversion_or_VDIV_operations operation
	                               : 2 = float_int_conversion_or_VDIV_operations::float_double_division_and_square_root;
	uint32_t                       : 1; /// TODO: what's this?
	uint32_t source1               : 7;
	uint32_t source2               : 7;
	uint32_t destination           : 7;
};
static_assert(sizeof(FDIV) == sizeof(uint32_t));

struct FSQRT
{
	compute_units compute_unit     : 2 = compute_units::VFPU;
	VFPU_types type                : 2 = VFPU_types::subtype;
	VFPU_subtype_types subtype     : 2 = VFPU_subtype_types::float_int_conversion_or_VDIV;
	float_double_division_and_square_root_instructions instruction
	                               : 1 = float_double_division_and_square_root_instructions::SQRT;
	float_double_division_and_square_root_sizes size
	                               : 1 = float_double_division_and_square_root_sizes::float_size;
	float_int_conversion_or_VDIV_operations operation
	                               : 2 = float_int_conversion_or_VDIV_operations::float_double_division_and_square_root;
	uint32_t                       : 1; /// TODO: what's this?
	uint32_t source1               : 7;
	uint32_t source2               : 7;
	uint32_t destination           : 7;
};
static_assert(sizeof(FSQRT) == sizeof(uint32_t));

struct DDIV_DSQRT
{
	compute_units compute_unit     : 2 = compute_units::VFPU;
	VFPU_types type                : 2 = VFPU_types::subtype;
	VFPU_subtype_types subtype     : 2 = VFPU_subtype_types::float_int_conversion_or_VDIV;
	float_double_division_and_square_root_instructions instruction
	                               : 1;
	float_double_division_and_square_root_sizes size
	                               : 1 = float_double_division_and_square_root_sizes::double_size;
	float_int_conversion_or_VDIV_operations operation
	                               : 2 = float_int_conversion_or_VDIV_operations::float_double_division_and_square_root;
	uint32_t                       : 4; /// TODO: what's this?
	uint32_t source1               : 6;
	uint32_t source2               : 6;
	uint32_t destination           : 6;
};
static_assert(sizeof(DDIV_DSQRT) == sizeof(uint32_t));

struct DDIV
{
	compute_units compute_unit     : 2 = compute_units::VFPU;
	VFPU_types type                : 2 = VFPU_types::subtype;
	VFPU_subtype_types subtype     : 2 = VFPU_subtype_types::float_int_conversion_or_VDIV;
	float_double_division_and_square_root_instructions instruction
	                               : 1 = float_double_division_and_square_root_instructions::DIV;
	float_double_division_and_square_root_sizes size
	                               : 1 = float_double_division_and_square_root_sizes::double_size;
	float_int_conversion_or_VDIV_operations operation
	                               : 2 = float_int_conversion_or_VDIV_operations::float_double_division_and_square_root;
	uint32_t                       : 4; /// TODO: what's this?
	uint32_t source1               : 6;
	uint32_t source2               : 6;
	uint32_t destination           : 6;
};
static_assert(sizeof(DDIV) == sizeof(uint32_t));

struct DSQRT
{
	compute_units compute_unit     : 2 = compute_units::VFPU;
	VFPU_types type                : 2 = VFPU_types::subtype;
	VFPU_subtype_types subtype     : 2 = VFPU_subtype_types::float_int_conversion_or_VDIV;
	float_double_division_and_square_root_instructions instruction
	                               : 1 = float_double_division_and_square_root_instructions::SQRT;
	float_double_division_and_square_root_sizes size
	                               : 1 = float_double_division_and_square_root_sizes::double_size;
	float_int_conversion_or_VDIV_operations operation
	                               : 2 = float_int_conversion_or_VDIV_operations::float_double_division_and_square_root;
	uint32_t                       : 4; /// TODO: what's this?
	uint32_t source1               : 6;
	uint32_t source2               : 6;
	uint32_t destination           : 6;
};
static_assert(sizeof(DSQRT) == sizeof(uint32_t));

struct MOVEFI
{
	compute_units compute_unit     : 2 = compute_units::VFPU;
	VFPU_types type                : 2 = VFPU_types::MOVEFI;
	uint32_t immediate             : 21;
	uint32_t destination           : 7;
};
static_assert(sizeof(MOVEFI) == sizeof(uint32_t));

struct MOVEDI
{
	compute_units compute_unit     : 2 = compute_units::VFPU;
	VFPU_types type                : 2 = VFPU_types::MOVEDI;
	uint32_t immediate             : 22;
	uint32_t destination           : 6;
};
static_assert(sizeof(MOVEDI) == sizeof(uint32_t));

struct MOVEVI
{
	compute_units compute_unit     : 2 = compute_units::VFPU;
	VFPU_types type                : 2 = VFPU_types::MOVEVI;
	uint32_t immediate             : 23;
	uint32_t destination           : 5;
};
static_assert(sizeof(MOVEVI) == sizeof(uint32_t));

} // namespace op

} // namespace ar

#endif // VFPU_HPP_INCLUDED
