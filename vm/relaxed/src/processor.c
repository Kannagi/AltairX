#include "vm.h"

#include <assert.h>
#include <string.h>

#define MIN(x, y) (x < y ? x : y)

static int decodeLSU(uint32_t opcode, Operation* restrict output)
{
    //LDM  : 0x00
    //STM  : 0x08
    //LDC  : 0x02
    //STC  : 0x0A
    //LDMX : 0x01
    //STMX : 0x11
    //IN   : 0x05
    //OUT  : 0x15
    //INI  : 0x09
    //OUTI : 0x19

    const uint32_t type = opcode & (0x03u << 2u);

    if(type == 0) //LDM/STM
    {
        const uint32_t incr  = opcode & (0x0001u << 4u);
        const uint32_t store = opcode & (0x0001u << 5u);
        const uint32_t size  = opcode & (0x0003u << 6u);
        const uint32_t value = opcode & (0x0FFFu << 8u);
        const uint32_t reg   = opcode & (0x003Fu << 20u);
        const uint32_t dest  = opcode & (0x003Fu << 26u);

        output->data = incr;
        output->op   = store ? OPCODE_STM : OPCODE_LDM;
        output->size = size;
        output->operands[0] = value;
        output->operands[1] = reg;
        output->operands[2] = dest;
    }
    else if(type == 1) //LDC/STC
    {
        const uint32_t incr  = opcode & (0x0001u << 4u);
        const uint32_t store = opcode & (0x0001u << 5u);
        const uint32_t size  = opcode & (0x0003u << 6u);
        const uint32_t value = opcode & (0x0FFFu << 8u);
        const uint32_t reg   = opcode & (0x003Fu << 20u);
        const uint32_t dest  = opcode & (0x003Fu << 26u);

        output->data = incr;
        output->op   = store ? OPCODE_STC : OPCODE_LDC;
        output->size = size;
        output->operands[0] = value;
        output->operands[1] = reg;
        output->operands[2] = dest;
    }
    else if(type == 2) //Others
    {
        const uint32_t subtype = opcode & (0x03u << 4u);

        if(subtype == 0)
        {

        }
        else if(subtype == 1)
        {

        }
        else if(subtype == 2)
        {

        }
        else
        {

        }
    }
    else //LDF/STF-LDD/STD
    {
        return 0;
    }
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
            return 1;

        //case COMPUTE_UNIT_FPU:
        //   return 1;

        default:
            return 1;
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
