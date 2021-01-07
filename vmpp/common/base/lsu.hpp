#ifndef LSU_HPP_INCLUDED
#define LSU_HPP_INCLUDED

#include "ops.hpp"

namespace ar
{

namespace op
{

enum class LSU_types : uint32_t
{
	DSRAM_load_store                = 0,
	subtype                         = 1,
	cache_load_store                = 2,
	float_DSRAM_or_cache_load_store = 3,
};

struct LSU_op
{
	compute_units compute_unit     : 2;
	LSU_types type                 : 2;
	uint32_t                       : 28;
};
static_assert(sizeof(LSU_op) == sizeof(uint32_t));

struct DSRAM
{
	compute_units compute_unit     : 2 = compute_units::LSU;
	LSU_types type                 : 2 = LSU_types::DSRAM_load_store;
	uint32_t incrementation        : 1;
	uint32_t store                 : 1;
	uint32_t size                  : 2;
	uint32_t immediate             : 12;
	uint32_t source                : 6;
	uint32_t register_value        : 6;
};
static_assert(sizeof(DSRAM) == sizeof(uint32_t));

enum class subtype_types : uint32_t
{
	DSRAM_load_store_extended        = 0,
	in_out                           = 1,
	outi                             = 2,
	vector_DSRAM_or_cache_load_store = 3,
};

struct subtype_op
{
	compute_units compute_unit           : 2;
	LSU_types type                       : 2;
	subtype_types subtype                : 2;
	uint32_t                             : 26;
};
static_assert(sizeof(subtype_op) == sizeof(uint32_t));

struct DSRAM_ext
{
	compute_units compute_unit     : 2 = compute_units::LSU;
	LSU_types type                 : 2 = LSU_types::subtype;
	subtype_types subtype          : 2 = subtype_types::DSRAM_load_store_extended;
	uint32_t store                 : 1;
	uint32_t size                  : 2;
	uint32_t immediate             : 16;
	uint32_t source                : 1;
	uint32_t register_value        : 6;
};
static_assert(sizeof(DSRAM_ext) == sizeof(uint32_t));

struct INOUT
{
	compute_units compute_unit     : 2 = compute_units::LSU;
	LSU_types type                 : 2 = LSU_types::subtype;
	subtype_types subtype          : 2 = subtype_types::in_out;
	uint32_t store                 : 1;
	uint32_t size                  : 2;
	uint32_t                       : 7;
	uint32_t address               : 8;
	uint32_t                       : 2;
	uint32_t register_value        : 6;
};
static_assert(sizeof(INOUT) == sizeof(uint32_t));

struct OUTI
{
	compute_units compute_unit     : 2 = compute_units::LSU;
	LSU_types type                 : 2 = LSU_types::subtype;
	subtype_types subtype          : 2 = subtype_types::outi;
	uint32_t store                 : 1;
	uint32_t size                  : 2;
	uint32_t                       : 7;
	uint32_t address               : 8;
	uint32_t                       : 2;
	uint32_t register_value        : 6;
};
static_assert(sizeof(OUTI) == sizeof(uint32_t));

struct VDSRAM
{
	compute_units compute_unit     : 2 = compute_units::LSU;
	LSU_types type                 : 2 = LSU_types::subtype;
	subtype_types subtype          : 2 = subtype_types::vector_DSRAM_or_cache_load_store;
	uint32_t store                 : 1;
	uint32_t cache                 : 1;
	uint32_t incrementation        : 1;
	uint32_t base_address          : 15;
	uint32_t source                : 3;
	uint32_t register_value        : 5;
};
static_assert(sizeof(VDSRAM) == sizeof(uint32_t));

struct cache_load_store
{
	compute_units compute_unit     : 2 = compute_units::LSU;
	LSU_types type                 : 2 = LSU_types::cache_load_store;
	uint32_t incrementation        : 1;
	uint32_t store                 : 1;
	uint32_t size                  : 2;
	uint32_t immediate             : 12;
	uint32_t source                : 6;
	uint32_t register_value        : 6;
};
static_assert(sizeof(cache_load_store) == sizeof(uint32_t));

enum class floating_load_store_sizes : uint32_t
{
	float_size  = 0,
	double_size = 1,
};

struct FDSRAM_or_cache_load_store
{
	compute_units compute_unit     : 2;
	LSU_types type                 : 2;
	floating_load_store_sizes size : 1;
	uint32_t                       : 27;
};
static_assert(sizeof(FDSRAM_or_cache_load_store) == sizeof(uint32_t));

enum class float_load_store_cache : uint32_t
{
	SDRAM = 0,
	cache = 1,
};

enum class float_load_store_store : uint32_t
{
	load  = 0,
	store = 1,
};

struct float_DSRAM_or_cache_load_store
{
	compute_units compute_unit     : 2;
	LSU_types type                 : 2;
	floating_load_store_sizes size : 1;
	float_load_store_cache cache   : 1;
	float_load_store_store store   : 1;
	uint32_t incrementation        : 1;
	uint32_t base_address          : 15;
	uint32_t source                : 2;
	uint32_t register_value        : 7;
};
static_assert(sizeof(float_DSRAM_or_cache_load_store) == sizeof(uint32_t));

struct LDMF
{
	compute_units compute_unit     : 2 = compute_units::LSU;
	LSU_types type                 : 2 = LSU_types::cache_load_store;
	floating_load_store_sizes size : 1 = floating_load_store_sizes::float_size;
	float_load_store_cache cache   : 1 = float_load_store_cache::SDRAM;
	float_load_store_store store   : 1 = float_load_store_store::load;
	uint32_t incrementation        : 1;
	uint32_t base_address          : 15;
	uint32_t source                : 2;
	uint32_t register_value        : 7;
};
static_assert(sizeof(LDMF) == sizeof(uint32_t));

struct STMF
{
	compute_units compute_unit     : 2 = compute_units::LSU;
	LSU_types type                 : 2 = LSU_types::cache_load_store;
	floating_load_store_sizes size : 1 = floating_load_store_sizes::float_size;
	float_load_store_cache cache   : 1 = float_load_store_cache::SDRAM;
	float_load_store_store store   : 1 = float_load_store_store::store;
	uint32_t incrementation        : 1;
	uint32_t base_address          : 15;
	uint32_t source                : 2;
	uint32_t register_value        : 7;
};
static_assert(sizeof(STMF) == sizeof(uint32_t));

struct LDCF
{
	compute_units compute_unit     : 2 = compute_units::LSU;
	LSU_types type                 : 2 = LSU_types::cache_load_store;
	floating_load_store_sizes size : 1 = floating_load_store_sizes::float_size;
	float_load_store_cache cache   : 1 = float_load_store_cache::cache;
	float_load_store_store store   : 1 = float_load_store_store::load;
	uint32_t incrementation        : 1;
	uint32_t base_address          : 15;
	uint32_t source                : 2;
	uint32_t register_value        : 7;
};
static_assert(sizeof(LDCF) == sizeof(uint32_t));

struct STCF
{
	compute_units compute_unit     : 2 = compute_units::LSU;
	LSU_types type                 : 2 = LSU_types::cache_load_store;
	floating_load_store_sizes size : 1 = floating_load_store_sizes::float_size;
	float_load_store_cache cache   : 1 = float_load_store_cache::cache;
	float_load_store_store store   : 1 = float_load_store_store::store;
	uint32_t incrementation        : 1;
	uint32_t base_address          : 15;
	uint32_t source                : 2;
	uint32_t register_value        : 7;
};
static_assert(sizeof(STCF) == sizeof(uint32_t));



struct double_DSRAM_or_cache_load_store
{
	compute_units compute_unit     : 2;
	LSU_types type                 : 2;
	floating_load_store_sizes size : 1;
	float_load_store_cache cache   : 1;
	float_load_store_store store   : 1;
	uint32_t incrementation        : 1;
	uint32_t base_address          : 16;
	uint32_t source                : 2;
	uint32_t register_value        : 6;
};
static_assert(sizeof(float_DSRAM_or_cache_load_store) == sizeof(uint32_t));

struct LDMD
{
	compute_units compute_unit     : 2 = compute_units::LSU;
	LSU_types type                 : 2 = LSU_types::cache_load_store;
	floating_load_store_sizes size : 1 = floating_load_store_sizes::double_size;
	float_load_store_cache cache   : 1 = float_load_store_cache::SDRAM;
	float_load_store_store store   : 1 = float_load_store_store::load;
	uint32_t incrementation        : 1;
	uint32_t base_address          : 16;
	uint32_t source                : 2;
	uint32_t register_value        : 6;
};
static_assert(sizeof(LDMD) == sizeof(uint32_t));

struct STMD
{
	compute_units compute_unit     : 2 = compute_units::LSU;
	LSU_types type                 : 2 = LSU_types::cache_load_store;
	floating_load_store_sizes size : 1 = floating_load_store_sizes::double_size;
	float_load_store_cache cache   : 1 = float_load_store_cache::SDRAM;
	float_load_store_store store   : 1 = float_load_store_store::store;
	uint32_t incrementation        : 1;
	uint32_t base_address          : 16;
	uint32_t source                : 2;
	uint32_t register_value        : 6;
};
static_assert(sizeof(STMD) == sizeof(uint32_t));

struct LDCD
{
	compute_units compute_unit     : 2 = compute_units::LSU;
	LSU_types type                 : 2 = LSU_types::cache_load_store;
	floating_load_store_sizes size : 1 = floating_load_store_sizes::double_size;
	float_load_store_cache cache   : 1 = float_load_store_cache::cache;
	float_load_store_store store   : 1 = float_load_store_store::load;
	uint32_t incrementation        : 1;
	uint32_t base_address          : 16;
	uint32_t source                : 2;
	uint32_t register_value        : 6;
};
static_assert(sizeof(LDCD) == sizeof(uint32_t));

struct STCD
{
	compute_units compute_unit     : 2 = compute_units::LSU;
	LSU_types type                 : 2 = LSU_types::cache_load_store;
	floating_load_store_sizes size : 1 = floating_load_store_sizes::double_size;
	float_load_store_cache cache   : 1 = float_load_store_cache::cache;
	float_load_store_store store   : 1 = float_load_store_store::store;
	uint32_t incrementation        : 1;
	uint32_t base_address          : 16;
	uint32_t source                : 2;
	uint32_t register_value        : 6;
};
static_assert(sizeof(STCD) == sizeof(uint32_t));

} // namespace op

} // namespace ar

#endif // LSU_HPP_INCLUDED
