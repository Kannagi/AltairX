#include <base/vm.h>

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define RAMSIZE (8 * 1024 * 1024)

typedef struct VirtualMachineContext
{
    ArVirtualMachine machine;
    ArProcessor processor;
    void* ram;
    ArPhysicalMemory memory;
} VirtualMachineContext;

static int read_file(const char* path, uint32_t** out_code, uint32_t* out_size)
{
    FILE* file = fopen(path, "rb");
    if(!file)
    {
        return 1;
    }

    fseek(file, 0, SEEK_END);
    const long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    uint32_t* output = malloc(size);
    if(!output)
    {
        fclose(file);
        return 1;
    }

    if(!fread(output, size, 1, file))
    {
        free(output);
        fclose(file);
        return 1;
    }

    *out_code = output;
    *out_size = size / 4;

    fclose(file);

    return 0;
}

static void destroyVirtualMachine(VirtualMachineContext* context);

static ArResult makeVirtualMachine(uint32_t* code, uint32_t code_size, VirtualMachineContext* output)
{
    ArResult result = AR_SUCCESS;

    ArVirtualMachineCreateInfo machine_info;
    machine_info.sType = AR_STRUCTURE_TYPE_VIRTUAl_MACHINE_CREATE_INFO;
    machine_info.pNext = NULL;

    if((result = arCreateVirtualMachine(&output->machine, &machine_info)) != AR_SUCCESS)
    {
        goto error;
    }

    ArProcessorCreateInfo processor_info;
    processor_info.sType = AR_STRUCTURE_TYPE_PROCESSOR_CREATE_INFO;
    processor_info.pNext = NULL;
    processor_info.pBootCode = code;
    processor_info.bootCodeSize = code_size;

    if((result = arCreateProcessor(output->machine, &processor_info, &output->processor)) != AR_SUCCESS)
    {
        goto error;
    }

    output->ram = malloc(RAMSIZE);
    if(!output->ram)
    {
        result = AR_ERROR_HOST_OUT_OF_MEMORY;
        goto error;
    }

    ArPhysicalMemoryCreateInfo memory_info;
    memory_info.sType = AR_STRUCTURE_TYPE_PHYSICAL_MEMORY_CREATE_INFO;
    memory_info.pNext = NULL;
    memory_info.pMemory = output->ram;
    memory_info.size = RAMSIZE;

    if((result = arCreatePhysicalMemory(output->machine, &memory_info, &output->memory)) != AR_SUCCESS)
    {
        goto error;
    }

    return AR_SUCCESS;

error:
    destroyVirtualMachine(output);

    return result;
}

static void destroyVirtualMachine(VirtualMachineContext* context)
{
    if(context->memory)
    {
        arDestroyPhysicalMemory(context->machine, context->memory);
    }

    if(context->processor)
    {
        arDestroyProcessor(context->machine, context->processor);
    }

    if(context->machine)
    {
        arDestroyVirtualMachine(context->machine);
    }

    if(context->ram)
    {
        free(context->ram);
    }
}

static int runVirtualMachine(uint32_t* code, uint32_t code_size)
{
    ArResult result;
    VirtualMachineContext context = {0};

    result = makeVirtualMachine(code, code_size, &context);
    if(result != AR_SUCCESS)
    {
        printf("Failed to initialize the virtual machine.");
        return 1;
    }

    //Main loop
    /*for(;;)
    {
        result = arDecodeInstruction(context.processor);
        if(result != AR_SUCCESS)
        {
            break;
        }

        result = arExecuteInstruction(context.processor);
        if(result != AR_SUCCESS)
        {
            break;
        }

        result = arExecuteDirectMemoryAccess(context.processor);
        if(result != AR_SUCCESS){
            break;
        }
    }*/

    destroyVirtualMachine(&context);

    if(result != AR_END_OF_CODE)
    {
        return 1;
    }

    return 0;
}

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        printf("Usage: altair_vm [path_to_binary]");
        return 1;
    }

    uint32_t* code;
    uint32_t code_size;
    if(read_file(argv[1], &code, &code_size))
    {
        printf("Can not find file \"%s\"", argv[1]);
        return 1;
    }

    const int return_code = runVirtualMachine(code, code_size);

    free(code);

    return return_code;
}
