#include "vm.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

ArResult arCreateVirtualMachine(ArVirtualMachine* pVirtualMachine, const ArVirtualMachineCreateInfo* pInfo)
{
    assert(pVirtualMachine);
    assert(pInfo);
    assert(pInfo->sType == AR_STRUCTURE_TYPE_VIRTUAl_MACHINE_CREATE_INFO);

    const ArVirtualMachine output = malloc(sizeof(ArVirtualMachine_T));
    if(!output)
    {
        return AR_ERROR_HOST_OUT_OF_MEMORY;
    }

    memset(output, 0, sizeof(ArVirtualMachine_T));

    *pVirtualMachine = output;
    (void)pInfo;

    return AR_SUCCESS;
}

static void insertProcessor(ArVirtualMachine virtualMachine, ArProcessor processor)
{
    ArProcessor previous = virtualMachine->processor;
    if(previous)
    {
        while(previous->next) //Find last processor in virtual machine
        {
            previous = previous->next;
        }

        previous->next = processor;
    }
    else
    {
        virtualMachine->processor = processor; //Its the first processor
    }
}

ArResult arCreateProcessor(ArVirtualMachine virtualMachine, const ArProcessorCreateInfo* pInfo, ArProcessor* pProcessor)
{
    assert(virtualMachine);
    assert(virtualMachine->rom);
    assert(virtualMachine->rom->size >= pInfo->bootCodeSize * sizeof(uint32_t));
    assert(pInfo);
    assert(pInfo->sType == AR_STRUCTURE_TYPE_PROCESSOR_CREATE_INFO);
    assert(pInfo->pBootCode);
    assert(pInfo->bootCodeSize % 2 == 0); //if not true then input is obviously truncated
    assert(AR_PROCESSOR_ISRAM_SIZE >= pInfo->bootCodeSize * sizeof(uint32_t));
    assert(pProcessor);

    const ArProcessor output = malloc(sizeof(ArProcessor_T));
    if(!output)
    {
        return AR_ERROR_HOST_OUT_OF_MEMORY;
    }

    memset(output, 0, sizeof(ArProcessor_T));

    output->parent = virtualMachine;
    memcpy(output->isram,               pInfo->pBootCode, pInfo->bootCodeSize * sizeof(uint32_t));
    memcpy(virtualMachine->rom->memory, pInfo->pBootCode, pInfo->bootCodeSize * sizeof(uint32_t));

    insertProcessor(virtualMachine, output);
    *pProcessor = output;

    return AR_SUCCESS;
}

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

    const ArPhysicalMemory output = malloc(sizeof(ArPhysicalMemory_T));
    if(!output)
    {
        return AR_ERROR_HOST_OUT_OF_MEMORY;
    }

    output->parent = virtualMachine;
    output->memory = pInfo->pMemory;
    output->size   = pInfo->size;
    output->role   = pInfo->role;

    *memory  = output;
    *pMemory = output;

    return AR_SUCCESS;
}

void arGetProcessorOperations(ArProcessor processor, ArOperation* pOutput, size_t* pCount)
{
    assert(processor);
    assert(pOutput);
    assert(pCount);

    const uint32_t count = opcodeSetSize(processor);

    memcpy(pOutput, processor->operations, sizeof(ArOperation) * count);
    *pCount = count;
}

void arGetProcessorMemoryInfo(ArProcessor processor, ArProcessorMemoryInfo* pOutput)
{
    assert(processor);
    assert(pOutput);

    pOutput->dsram  = processor->dsram;
    pOutput->isram  = processor->isram;
    pOutput->cache  = processor->cache;
    pOutput->iosram = processor->iosram;
    pOutput->ireg   = processor->ireg;
    pOutput->vreg   = processor->vreg;
}

void arDestroyVirtualMachine(ArVirtualMachine virtualMachine)
{
    assert(virtualMachine);
    assert(!virtualMachine->rom);
    assert(!virtualMachine->ram);
    assert(!virtualMachine->processor);

    free(virtualMachine);
}

void arDestroyProcessor(ArVirtualMachine virtualMachine, ArProcessor processor)
{
    assert(virtualMachine);
    assert(virtualMachine->processor);
    assert(processor);

    if(virtualMachine->processor == processor)
    {
        virtualMachine->processor = processor->next;
    }
    else
    {
        ArProcessor previous = virtualMachine->processor;
        while(previous->next && previous->next != processor)
        {
            previous = previous->next;
        }

        previous->next = processor->next;
    }

    free(processor);
}

void arDestroyPhysicalMemory(ArVirtualMachine virtualMachine, ArPhysicalMemory memory)
{
    assert(virtualMachine);

    ArPhysicalMemory* machineMemory = getMemoryByRole(virtualMachine, memory->role);

    assert(*machineMemory);
    assert(memory);

    *machineMemory = NULL;
    free(memory);
}

uint32_t opcodeSetSize(ArProcessor restrict processor)
{
    uint32_t size;
    if(processor->flags & 0x01)
    {
        const uint32_t available = processor->pc - (AR_PROCESSOR_ISRAM_SIZE / 4u); //we may overflow otherwise
        size = MIN(available, 4u);
    }
    else
    {
        size = 2;
    }

    return size;
}
