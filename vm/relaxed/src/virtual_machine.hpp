#ifndef ALTAIR_VIRTUAL_MACHINE_INTERNAL_H_DEFINED
#define ALTAIR_VIRTUAL_MACHINE_INTERNAL_H_DEFINED

#include "vm_relaxed.hpp"

typedef struct ArVirtualMachine_T
{
    ArProcessor processor;
    ArPhysicalMemory ram;
    ArPhysicalMemory rom;

} ArVirtualMachine_T;

#endif
