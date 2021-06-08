#ifndef ALTAIR_MEMORY_INTERNAL_H_DEFINED
#define ALTAIR_MEMORY_INTERNAL_H_DEFINED

#include "vm_relaxed.hpp"

typedef struct ArPhysicalMemory_T
{
    ArVirtualMachine parent;

    uint8_t* memory;
    size_t size;
    ArPhysicalMemoryRole role;

} ArPhysicalMemory_T;

ArPhysicalMemory* getMemoryByRole(ArVirtualMachine virtualMachine, ArPhysicalMemoryRole role);

ArResult copyFromRAM(ArPhysicalMemory memory, uint64_t memoryAddress, uint8_t* AR_RESTRICT output, size_t size);
ArResult copyToRAM(ArPhysicalMemory memory, uint64_t memoryAddress, const uint8_t* AR_RESTRICT input, size_t size);

#endif
