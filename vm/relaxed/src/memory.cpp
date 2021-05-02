#include "memory.hpp"
#include "virtual_machine.hpp"

#include <cassert>

ArPhysicalMemory* getMemoryByRole(ArVirtualMachine virtualMachine, ArPhysicalMemoryRole role)
{
    switch(role)
    {
        case AR_PHYSICAL_MEMORY_ROLE_ROM: return &virtualMachine->rom;
        case AR_PHYSICAL_MEMORY_ROLE_RAM: return &virtualMachine->ram;
        default: return NULL;
    }
}

ArResult arCreatePhysicalMemory(ArVirtualMachine virtualMachine, const ArPhysicalMemoryCreateInfo* pInfo, ArPhysicalMemory* pMemory)
{
    assert(virtualMachine);
    assert(pInfo);
    assert(pInfo->sType == AR_STRUCTURE_TYPE_PHYSICAL_MEMORY_CREATE_INFO);
    assert(pInfo->size > 0);
    assert(pInfo->pMemory);
    assert(pMemory);

    ArPhysicalMemory* memory = getMemoryByRole(virtualMachine, pInfo->role);

    if(*memory)
    {
        return AR_ERROR_TOO_MANY_OBJECTS;
    }

    const ArPhysicalMemory output = new ArPhysicalMemory_T;
    if(!output)
    {
        return AR_ERROR_HOST_OUT_OF_MEMORY;
    }

    output->parent = virtualMachine;
    output->memory = (uint8_t*)pInfo->pMemory;
    output->size   = pInfo->size;
    output->role   = pInfo->role;

    *memory  = output;
    *pMemory = output;

    return AR_SUCCESS;
}

void arDestroyPhysicalMemory(ArVirtualMachine virtualMachine, ArPhysicalMemory memory)
{
    assert(virtualMachine);

    ArPhysicalMemory* machineMemory = getMemoryByRole(virtualMachine, memory->role);

    assert(*machineMemory);
    assert(memory);

    *machineMemory = NULL;
    delete memory;
}
