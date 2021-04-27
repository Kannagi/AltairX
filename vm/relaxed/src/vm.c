#include "vm.h"

#include <SDL2/SDL.h>

#include <stdlib.h>
#include <assert.h>
#include <string.h>

ArResult arCreateVirtualMachine(ArVirtualMachine* pVirtualMachine, const ArVirtualMachineCreateInfo* pInfo)
{
    assert(pVirtualMachine);
    assert(pInfo);
    assert(pInfo->sType == AR_STRUCTURE_TYPE_VIRTUAl_MACHINE_CREATE_INFO);

    const ArVirtualMachine output = (ArVirtualMachine)malloc(sizeof(ArVirtualMachine_T));
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

    const ArProcessor output = (ArProcessor)malloc(sizeof(ArProcessor_T));
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

    const ArPhysicalMemory output = (ArPhysicalMemory)malloc(sizeof(ArPhysicalMemory_T));
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

ArResult arCreateScreen(ArVirtualMachine virtualMachine, const ArScreenCreateInfo* pInfo, ArScreen* pScreen)
{
    assert(virtualMachine);
    assert(pInfo);
    assert(pInfo->sType == AR_STRUCTURE_TYPE_PHYSICAL_MEMORY_CREATE_INFO);
    assert(pInfo->width > 0);
    assert(pInfo->height > 0);
    assert(pInfo->graphicsProcessor != NULL);
    assert(pScreen);

    const ArScreen output = (ArScreen)malloc(sizeof(ArScreen_T));
    if(!output)
    {
        return AR_ERROR_HOST_OUT_OF_MEMORY;
    }

    output->parent = virtualMachine;
    output->graphicsProcessor = pInfo->graphicsProcessor;

    int posX = 100, posY = 100;

    SDL_Init(SDL_INIT_VIDEO);

    output->win = SDL_CreateWindow("Hello World", posX, posY, pInfo->width, pInfo->height, 0);

    output->renderer = SDL_CreateRenderer(output->win, -1, SDL_RENDERER_ACCELERATED);

    output->texture = SDL_CreateTexture(output->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, pInfo->width, pInfo->height);
    //SDL_UpdateTexture(output->texture, NULL, const void* pixels, int pitch);

    //while (1)
    {
        SDL_Event e;
        if (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                //break;
            }
        }

        SDL_RenderClear(output->renderer);
        SDL_RenderCopy(output->renderer, output->texture, NULL, NULL);
        SDL_RenderPresent(output->renderer);
    }

//    Uint32 rmask, gmask, bmask, amask;
//
//#if SDL_BYTEORDER == SDL_BIG_ENDIAN
//    rmask = 0xff000000;
//    gmask = 0x00ff0000;
//    bmask = 0x0000ff00;
//    amask = 0x000000ff;
//#else
//    rmask = 0x000000ff;
//    gmask = 0x0000ff00;
//    bmask = 0x00ff0000;
//    amask = 0xff000000;
//#endif
//
//    output->surface = SDL_CreateRGBSurface(0, pInfo->width, pInfo->height, 0,
//        rmask, gmask, bmask, amask);
//    if (output->surface == NULL)
//    {
//        SDL_Log("SDL_CreateRGBSurface() failed: %s", SDL_GetError());
//        free(output);
//        return AR_ERROR_HOST_OUT_OF_MEMORY;
//    }

    *pScreen = output;

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

void arDestroyScreen(ArVirtualMachine virtualMachine, ArScreen screen)
{
    assert(virtualMachine);

    if (screen)
    {
        //SDL_FreeSurface(screen->surface);
        SDL_DestroyTexture(screen->texture);
        SDL_DestroyRenderer(screen->renderer);
        SDL_DestroyWindow(screen->win);

        SDL_Quit();

        free(screen);
    }
}

uint32_t opcodeSetSize(ArProcessor __restrict processor)
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
