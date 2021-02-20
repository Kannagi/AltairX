#ifndef ALTAIR_VM_INTERNAL_H_DEFINED
#define ALTAIR_VM_INTERNAL_H_DEFINED

#include <base/vm.h>

#define MIN(x, y) (x < y ? x : y)
#define MAX(x, y) (x > y ? x : y)

typedef struct ArVirtualMachine_T
{
    ArProcessor processor;
    ArPhysicalMemory memory;

} ArVirtualMachine_T;

#define XCHG_MASK (0x01u)
#define Z_MASK (0x02u)
#define S_MASK (0x04u)
#define U_MASK (0x08u)

typedef struct ArProcessor_T
{
    ArProcessor next;
    ArVirtualMachine parent;

    uint8_t dsram [AR_PROCESSOR_DSRAM_SIZE];
    uint8_t isram [AR_PROCESSOR_ISRAM_SIZE];
    uint8_t cache [AR_PROCESSOR_CACHE_SIZE];
    uint8_t iosram[AR_PROCESSOR_IOSRAM_SIZE];

    uint64_t ireg[AR_PROCESSOR_IREG_COUNT];
    uint64_t vreg[AR_PROCESSOR_VREG_COUNT];

    uint32_t pc; //program-counter
    uint32_t br; //buffer-register
    uint32_t lr; //link-register
    uint32_t opcodes[AR_PROCESSOR_MAX_OPERATIONS];

    /// \brief CPU Flags register
    ///
    /// Bit 0: XCHG flag, 1 is 4-way decode, 0 is 2-way
    /// Bit 1: Z flag, 1 if not equal, 0 if equal
    /// Bit 2: S flag, 1 if lesser, 0 if greater (signed comparison)
    /// Bit 3: U flag, 1 if lesser, 0 if greater (unsigned comparison)
    uint32_t flags;

    ArOperation operations[AR_PROCESSOR_MAX_OPERATIONS];
    uint32_t delayedBits;
    ArOperation delayed[AR_PROCESSOR_MAX_OPERATIONS];
    uint32_t dma; //1 if dmaOperation is to be treated
    ArOperation dmaOperation;

} ArProcessor_T;

typedef struct ArPhysicalMemory_T
{
    ArVirtualMachine parent;

    uint8_t* memory;
    size_t size;

} ArPhysicalMemory_T;

uint32_t opcodeSetSize(ArProcessor restrict processor);

#endif
