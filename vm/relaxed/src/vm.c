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
    assert(pInfo);
    assert(pInfo->sType == AR_STRUCTURE_TYPE_PROCESSOR_CREATE_INFO);
    assert(pInfo->pBootCode);
    assert(pInfo->bootCodeSize % 4 != 0);
    assert(pProcessor);

    const ArProcessor output = malloc(sizeof(ArProcessor_T));
    if(!output)
    {
        return AR_ERROR_HOST_OUT_OF_MEMORY;
    }

    memset(output, 0, sizeof(ArProcessor_T));

    output->parent = virtualMachine;
    memcpy(output->isram, pInfo->pBootCode, pInfo->bootCodeSize * sizeof(uint32_t));

    insertProcessor(virtualMachine, output);
    *pProcessor = output;

    return AR_SUCCESS;
}

static void insertPhysicalMemory(ArVirtualMachine virtualMachine, ArPhysicalMemory memory)
{
    ArPhysicalMemory previous = virtualMachine->memory;
    if(previous)
    {
        while(previous->next) //Find last memory in virtual machine
        {
            previous = previous->next;
        }

        previous->next = memory;
    }
    else
    {
        virtualMachine->memory = memory; //Its the first memory
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

    const ArPhysicalMemory output = malloc(sizeof(ArPhysicalMemory_T));
    if(!output)
    {
        return AR_ERROR_HOST_OUT_OF_MEMORY;
    }

    output->next = NULL;
    output->parent = virtualMachine;
    output->memory = pInfo->pMemory;
    output->size = pInfo->size;

    insertPhysicalMemory(virtualMachine, output);
    *pMemory = output;

    return AR_SUCCESS;
}

void arDestroyVirtualMachine(ArVirtualMachine virtualMachine)
{
    assert(virtualMachine);
    assert(!virtualMachine->memory);
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
    assert(virtualMachine->memory);
    assert(memory);

    if(virtualMachine->memory == memory)
    {
        virtualMachine->memory = memory->next;
    }
    else
    {
        ArPhysicalMemory previous = virtualMachine->memory;
        while(previous->next && previous->next != memory)
        {
            previous = previous->next;
        }

        previous->next = memory->next;
    }

    free(memory);
}
