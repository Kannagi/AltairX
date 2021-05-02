#include "virtual_machine.hpp"

#include <cassert>
#include <cstring>

ArResult arCreateVirtualMachine(ArVirtualMachine* pVirtualMachine, const ArVirtualMachineCreateInfo* pInfo)
{
    assert(pVirtualMachine);
    assert(pInfo);
    assert(pInfo->sType == AR_STRUCTURE_TYPE_VIRTUAl_MACHINE_CREATE_INFO);

    const ArVirtualMachine output = new ArVirtualMachine_T;
    if(!output)
    {
        return AR_ERROR_HOST_OUT_OF_MEMORY;
    }

    memset(output, 0, sizeof(ArVirtualMachine_T));

    *pVirtualMachine = output;
    (void)pInfo;

    return AR_SUCCESS;
}

void arDestroyVirtualMachine(ArVirtualMachine virtualMachine)
{
    assert(virtualMachine);
    assert(!virtualMachine->rom);
    assert(!virtualMachine->ram);
    assert(!virtualMachine->processor);

    delete virtualMachine;
}
