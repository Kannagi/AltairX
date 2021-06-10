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
