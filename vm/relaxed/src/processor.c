#include "vm.h"

#include <assert.h>
#include <string.h>

#define MIN(x, y) (x < y ? x : y)

static int decodeLSU(uint32_t opcode, Operation* restrict output)
{
    const uint32_t type = opcode & (0x03u << 2u);

    if(type == 0) //LDM/STM
    {
        const uint32_t incr  = opcode & (0x0001u << 4u);
        const uint32_t store = opcode & (0x0001u << 5u);
        const uint32_t size  = opcode & (0x0003u << 6u);
        const uint32_t value = opcode & (0x0FFFu << 8u);
        const uint32_t src   = opcode & (0x003Fu << 20u);
        const uint32_t dest  = opcode & (0x003Fu << 26u);

        output->data = incr;
        output->op   = store ? OPCODE_STM : OPCODE_LDM;
        output->size = size;
        output->operands[0] = value;
        output->operands[1] = src;
        output->operands[2] = dest;
    }
    else if(type == 1) //Subtypes
    {
        const uint32_t subtype = opcode & (0x03u << 4u);

        if(subtype == 0) // LDMX/STMX
        {
            const uint32_t store = opcode & (0x0001u << 6u);
            const uint32_t size  = opcode & (0x0003u << 7u);
            const uint32_t value = opcode & (0xFFFFu << 9u);
            const uint32_t src   = opcode & (0x0001u << 25u);
            const uint32_t dest  = opcode & (0x003Fu << 26u);

            output->data = 0;
            output->op   = store ? OPCODE_STMX : OPCODE_LDMX;
            output->size = size;
            output->operands[0] = value;
            output->operands[1] = src + 62;
            output->operands[2] = dest;
        }
        else if(subtype == 1) //IN/OUT
        {
            const uint32_t store = opcode & (0x01u << 6u);
            const uint32_t size  = opcode & (0x03u << 7u);
            const uint32_t value = opcode & (0xFFu << 16u);
            const uint32_t dest  = opcode & (0x3Fu << 26u);

            output->data = 0;
            output->op   = store ? OPCODE_OUT : OPCODE_IN;
            output->size = size;
            output->operands[0] = value;
            output->operands[1] = 0;
            output->operands[2] = dest;
        }
        else if(subtype == 2) //OUTI
        {
            const uint32_t size  = opcode & (0x0001u << 7u);
            const uint32_t value = opcode & (0xFFFFu << 16u);
            const uint32_t dest  = opcode & (0x00FFu << 26u);

            output->data = 0;
            output->op   = OPCODE_OUTI;
            output->size = size;
            output->operands[0] = value;
            output->operands[1] = 0;
            output->operands[2] = dest;
        }
        else //LDV/STV
        {
            return 0;
        }
    }
    else if(type == 2) //LDC/STC
    {
        const uint32_t incr  = opcode & (0x0001u << 4u);
        const uint32_t store = opcode & (0x0001u << 5u);
        const uint32_t size  = opcode & (0x0003u << 6u);
        const uint32_t value = opcode & (0x0FFFu << 8u);
        const uint32_t src   = opcode & (0x003Fu << 20u);
        const uint32_t dest  = opcode & (0x003Fu << 26u);

        output->data = incr;
        output->op   = store ? OPCODE_STC : OPCODE_LDC;
        output->size = size;
        output->operands[0] = value;
        output->operands[1] = src;
        output->operands[2] = dest;
    }
    else //LDF/STF-LDD/STD
    {
        return 0;
    }

    return 1;
}

static int decodeALU(uint32_t opcode, Operation* restrict output)
{
    return 1;
}

static int decode(uint32_t opcode, Operation* restrict output)
{
    enum
    {
        AGU_BRU = 0x00,
        LSU = 0x01,
        ALU = 0x10,
        //FPU = 0x00
    };

    const uint32_t compute_unit = opcode & 0x03;

    switch(compute_unit)
    {
        case AGU_BRU:
            return 1;

        case LSU:
            return decodeLSU(opcode, output);

        case ALU:
            return decodeALU(opcode, output);

        //case FPU:
        //   return 1;

        default:
            return 0;
    }
}

ArResult arDecodeInstruction(ArProcessor processor)
{
    assert(processor);

    uint32_t size;
    if(processor->xchg)
    {
        const uint32_t available = processor->programCounter - (ISRAM_SIZE / 4u); //we may overflow otherwise
        size = MIN(available, 4u);
    }
    else
    {
        size = 2;
    }

    memcpy(processor->opcodes, processor->isram + processor->programCounter, size * sizeof(uint32_t));
    processor->programCounter += size;

    for(uint32_t i = 0; i < size; ++i)
    {
        if(!decode(processor->opcodes[i], &processor->operations[i]))
        {
            return AR_ERROR_ILLEGAL_INSTRUCTION;
        }
    }

    return AR_SUCCESS;
}

ArResult arExecuteInstruction(ArProcessor processor)
{

}

ArResult arExecuteDirectMemoryAccess(ArProcessor processor)
{

}
