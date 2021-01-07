#ifndef AGU_HPP_INCLUDED
#define AGU_HPP_INCLUDED

#include "ops.hpp"

namespace ar
{

namespace op
{

enum class AGU_categories : uint32_t
{
	direct_load_store = 0,
	load_store_list   = 1,
};

struct AGU_op
{
	compute_units compute_unit     : 2;
	AGU_categories category        : 1;
	uint32_t                       : 29;
};
static_assert(sizeof(AGU_op) == sizeof(uint32_t));

struct direct_load_store
{
	compute_units compute_unit     : 2 = compute_units::AGU;
	AGU_categories category        : 1 = AGU_categories::direct_load_store;
	uint32_t store                 : 1;
	uint32_t size                  : 1;
	uint32_t DSRAM_address         : 2;
	uint32_t RAM_address           : 1;
	uint32_t DSRAM_base            : 12;
	uint32_t RAM_base              : 12;
};
static_assert(sizeof(direct_load_store) == sizeof(uint32_t));

struct LDDMA
{
	compute_units compute_unit     : 2 = compute_units::AGU;
	AGU_categories category        : 1 = AGU_categories::direct_load_store;
	uint32_t store                 : 1 = 0;
	uint32_t size                  : 1;
	uint32_t DSRAM_address         : 2;
	uint32_t RAM_address           : 1;
	uint32_t DSRAM_base            : 12;
	uint32_t RAM_base              : 12;
};
static_assert(sizeof(LDDMA) == sizeof(uint32_t));

struct STDMA
{
	compute_units compute_unit     : 2 = compute_units::AGU;
	AGU_categories category        : 1 = AGU_categories::direct_load_store;
	uint32_t store                 : 1 = 1;
	uint32_t size                  : 1;
	uint32_t DSRAM_address         : 2;
	uint32_t RAM_address           : 1;
	uint32_t DSRAM_base            : 12;
	uint32_t RAM_base              : 12;
};
static_assert(sizeof(STDMA) == sizeof(uint32_t));

enum class load_store_list_types : uint32_t
{
	LDDMAR_STDMAR = 0,
	DMAIR         = 1,
	// illegal       = 2,
	// illegal       = 3,
	// illegal       = 4,
	// illegal       = 5,
	// illegal       = 6,
	// illegal       = 7,
	// illegal       = 8,
	// illegal       = 9,
	// illegal       = 10,
	// illegal       = 11,
	// illegal       = 12,
	// illegal       = 13,
	// illegal       = 14,
	WAIT          = 15,
};

struct load_store_list
{
	compute_units compute_unit     : 2 = compute_units::AGU;
	AGU_categories category        : 1 = AGU_categories::load_store_list;
	uint32_t store                 : 1;
	load_store_list_types type     : 4;
	uint32_t                       : 24;
};
static_assert(sizeof(load_store_list) == sizeof(uint32_t));

struct LDDMAR
{
	compute_units compute_unit     : 2 = compute_units::AGU;
	AGU_categories category        : 1 = AGU_categories::load_store_list;
	uint32_t store                 : 1 = 0;
	load_store_list_types type     : 4 = load_store_list_types::LDDMAR_STDMAR;
	uint32_t size                  : 12;
	uint32_t RAM_address           : 6;
	uint32_t DSRAM_address         : 6;
};
static_assert(sizeof(LDDMAR) == sizeof(uint32_t));

struct STDMAR
{
	compute_units compute_unit     : 2 = compute_units::AGU;
	AGU_categories category        : 1 = AGU_categories::load_store_list;
	uint32_t store                 : 1 = 1;
	load_store_list_types type     : 4 = load_store_list_types::LDDMAR_STDMAR;
	uint32_t size                  : 12;
	uint32_t RAM_address           : 6;
	uint32_t DSRAM_address         : 6;
};
static_assert(sizeof(STDMAR) == sizeof(uint32_t));

struct DMAIR
{
	compute_units compute_unit     : 2 = compute_units::AGU;
	AGU_categories category        : 1 = AGU_categories::load_store_list;
	uint32_t store                 : 1 = 1;
	load_store_list_types type     : 4 = load_store_list_types::DMAIR;
	uint32_t size                  : 12;
	uint32_t RAM_address           : 6;
	uint32_t DSRAM_address         : 6;
};
static_assert(sizeof(DMAIR) == sizeof(uint32_t));

struct WAIT
{
	compute_units compute_unit     : 2 = compute_units::AGU;
	AGU_categories category        : 1 = AGU_categories::load_store_list;
	uint32_t store                 : 1 = 1;
	load_store_list_types type     : 4 = load_store_list_types::WAIT;
	uint32_t                       : 24;
};
static_assert(sizeof(WAIT) == sizeof(uint32_t));

} // namespace op

} // namespace ar

#endif // AGU_HPP_INCLUDED
