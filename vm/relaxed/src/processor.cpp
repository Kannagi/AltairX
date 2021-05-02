#include "processor.hpp"
#include "virtual_machine.hpp"
#include "memory.hpp"

#include <cassert>
#include <cstring>

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

    const ArProcessor output = new ArProcessor_T;
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

    delete processor;
}

uint32_t opcodeSetSize(ArProcessor AR_RESTRICT processor)
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
