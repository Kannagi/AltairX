#ifndef ALTAIR_VM_INTERNAL_H_DEFINED
#define ALTAIR_VM_INTERNAL_H_DEFINED

#include "common/base/vm.h"

typedef struct ArVirtualMachine_T
{
	ArProcessor processor = nullptr;
	ArPhysicalMemory memory = nullptr;

} ArVirtualMachine_T;

constexpr uint32_t XCHG_MASK=0x01u;
constexpr uint32_t Z_MASK = 0x02u;
constexpr uint32_t S_MASK = 0x04u;
constexpr uint32_t U_MASK = 0x08u;
constexpr uint32_t R_MASK = 0x03FFF0u;

typedef struct ArProcessor_T
{
	ArProcessor next = nullptr;
	ArVirtualMachine parent = nullptr;

	uint8_t dsram [AR_PROCESSOR_DSRAM_SIZE]{0};
	uint8_t isram [AR_PROCESSOR_ISRAM_SIZE]{0};
	uint8_t cache [AR_PROCESSOR_CACHE_SIZE]{0};
	uint8_t iosram[AR_PROCESSOR_IOSRAM_SIZE]{0};

	uint64_t ireg[AR_PROCESSOR_IREG_COUNT]{0};
	uint64_t freg[AR_PROCESSOR_FREG_COUNT / 2u]{0};

	uint32_t pc = 0; //program-counter
	uint32_t opcodes[AR_PROCESSOR_MAX_OPERATIONS]{0};

	/// \brief CPU Flags register
	///
	/// Bit 0: XCHG flag, 1 is 4-way decode, 0 is 2-way
	/// Bit 1: Z flag, 1 if not equal, 0 if equal
	/// Bit 2: S flag, 1 if lesser, 0 if greater (signed comparison)
	/// Bit 3: U flag, 1 if lesser, 0 if greater (unsigned comparison)
	/// Bit 4-17: R value, the PC address of the last call
	uint32_t flags = 0;

	ArOperation operations[AR_PROCESSOR_MAX_OPERATIONS]{};
	uint32_t delayedBits = 0;
	ArOperation delayed[AR_PROCESSOR_MAX_OPERATIONS]{};
	uint32_t dma = 0; //1 if dmaOperation is to be treated
	ArOperation dmaOperation{};

} ArProcessor_T;

typedef struct Vector4f
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 0.0f;

} Vector4f;

typedef struct ArPhysicalMemory_T
{
	ArVirtualMachine parent = nullptr;

	uint8_t* memory = nullptr;
	size_t size = 0;

} ArPhysicalMemory_T;

uint32_t opcodeSetSize(ArProcessor processor);

#endif
