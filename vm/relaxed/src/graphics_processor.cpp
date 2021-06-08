#include "graphics_processor.hpp"

#include <cassert>
#include <cstring>

ArResult arCreateGraphicsProcessor(ArVirtualMachine virtualMachine, const ArGraphicsProcessorCreateInfo* pInfo, ArGraphicsProcessor* pGraphicsProcessor)
{
    assert(virtualMachine);
    assert(pInfo);
    assert(pInfo->sType == AR_STRUCTURE_TYPE_GRAPHICS_PROCESSOR_CREATE_INFO);
    assert(pGraphicsProcessor);

    ArGraphicsProcessor output = new ArGraphicsProcessor_T;
    if (!output)
    {
        return AR_ERROR_HOST_OUT_OF_MEMORY;
    }

    output->parent = virtualMachine;

    //insertProcessor(virtualMachine, output);
    *pGraphicsProcessor = output;

    return AR_SUCCESS;
}

void arDestroyGraphicsProcessor(ArVirtualMachine virtualMachine, ArGraphicsProcessor graphicsProcessor)
{
    assert(virtualMachine);

    delete graphicsProcessor;
}

uint32_t opcodeSetSize(ArGraphicsProcessor AR_RESTRICT graphicsProcessor)
{
    uint32_t size;
    if (graphicsProcessor->flags & 0x01)
    {
        const uint32_t available = graphicsProcessor->pc - (AR_PROCESSOR_ISRAM_SIZE / 4u); //we may overflow otherwise
        size = MIN(available, 4u);
    }
    else
    {
        size = 2;
    }

    return size;
}
