#include <base/vm.h>

#include <stdlib.h>
#include <assert.h>

int main()
{
    const ArVirtualMachineCreateInfo machine_info =
    {
        .sType = AR_STRUCTURE_TYPE_VIRTUAl_MACHINE_CREATE_INFO,
        .pNext = NULL
    };

    ArVirtualMachine machine;
    arCreateVirtualMachine(&machine, &machine_info);

    static const uint32_t code[] = {42, 42, 42, 42};

    const ArProcessorCreateInfo processor_info =
    {
        .sType = AR_STRUCTURE_TYPE_PROCESSOR_CREATE_INFO,
        .pNext = NULL,
        .pBootCode = code,
        .bootCodeSize = 4
    };

    ArProcessor processor;
    arCreateProcessor(machine, &processor_info, &processor);

    const ArPhysicalMemoryCreateInfo memory_info =
    {
        .sType = AR_STRUCTURE_TYPE_PHYSICAL_MEMORY_CREATE_INFO,
        .pNext = NULL,
        .pMemory = malloc(1024 * 1024),
        .size = 1024 * 1024
    };

    assert(memory_info.pMemory);

    ArPhysicalMemory memory;
    arCreatePhysicalMemory(machine, &memory_info, &memory);

    for(;;)
    {
        ArResult result = AR_SUCCESS;

        result = arDecodeInstruction(processor);

        if(result != AR_SUCCESS)
        {
            return 1;
        }
        else if(result == AR_END_OF_CODE)
        {
            break;
        }

        result = arExecuteInstruction(processor);

        if(result != AR_SUCCESS)
        {
            return 1;
        }

        result = arExecuteDirectMemoryAccess(processor);

        if(result != AR_SUCCESS)
        {
            return 1;
        }
    }

    return 0;
}
