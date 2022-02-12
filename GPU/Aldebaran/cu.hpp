#ifndef CU_HPP
#define CU_HPP 1

#include <aldebaran.h>
#include <buffer_t.hpp>

struct ArExecute_T
{
	uint32_t unit1, unit2, size, id, imm, immf;
	int32_t rA, rB, rC;
	float fimm;
	double dimm;
};

struct vec4f
{
	float v[4]{ 0 };

	float& x() { return v[0]; }
	float& y() { return v[1]; }
	float& z() { return v[2]; }
	float& w() { return v[3]; }
};

struct vec8h
{
	uint16_t v[8]{ 0 };

	uint16_t& x() { return v[0]; }
	uint16_t& y() { return v[1]; }
	uint16_t& z() { return v[2]; }
	uint16_t& w() { return v[3]; }
	uint16_t& p() { return v[4]; }
	uint16_t& q() { return v[5]; }
	uint16_t& r() { return v[6]; }
	uint16_t& s() { return v[7]; }
};

struct vec4i
{
	uint32_t v[4]{ 0 };

	uint32_t& x() { return v[0]; }
	uint32_t& y() { return v[1]; }
	uint32_t& z() { return v[2]; }
	uint32_t& w() { return v[3]; }
};

union regs
{
	vec4f vreg[AR_PROCESSOR_VREG_COUNT];
	vec8h hreg[AR_PROCESSOR_VREG_COUNT];
	vec4i ireg[AR_PROCESSOR_VREG_COUNT];
};

struct ComputeUnit_t
{
	ArExecute_T operations[AR_PROCESSOR_MAX_OPERATIONS];

	uint64_t instruction;
	uint64_t cycle;

	uint32_t ireg2[AR_PROCESSOR_IREG_COUNT];

	regs shaders[N_SHADER];

	uint32_t opcodes[AR_PROCESSOR_MAX_OPERATIONS];

	uint16_t delay, delayop, imm;
	uint16_t pc;  // program-counter
	uint16_t br;  // buffer-register
	uint16_t lr;  // link-register

	/// \brief CPU Flags register
	///
	/// Bit 0:
	/// Bit 1: Z flag, 1 if not equal, 0 if equal
	/// Bit 2: S flag, 1 if lesser, 0 if greater (signed comparison)
	/// Bit 3: U flag, 1 if lesser, 0 if greater (unsigned comparison)
	uint16_t flags;

	uint8_t dsram[AR_PROCESSOR_DSRAM_SIZE];
	uint8_t isram[AR_PROCESSOR_ISRAM_SIZE];

	uint8_t* ram;
	uint8_t* rom;
	uint8_t* tsram;
	uint8_t* spm2;
	uint8_t* io;
};

#endif  // CU_HPP
