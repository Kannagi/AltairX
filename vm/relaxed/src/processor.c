#include "vm.h"

#include <assert.h>
#include <string.h>

#define MIN(x, y) (x < y ? x : y)

static int decodeAGUBRU(uint32_t opcode, Operation* restrict output)
{


    return 1;
}

static int decodeLSU(uint32_t opcode, Operation* restrict output)
{
    const uint32_t type = (opcode >> 2u) & 0x03u;

    if(type == 0) //LDM/STM
    {
        const uint32_t incr  = (opcode >> 4u ) & 0x0001u;
        const uint32_t store = (opcode >> 5u ) & 0x0001u;
        const uint32_t size  = (opcode >> 6u ) & 0x0003u;
        const uint32_t value = (opcode >> 8u ) & 0x0FFFu;
        const uint32_t src   = (opcode >> 20u) & 0x003Fu;
        const uint32_t dest  = (opcode >> 26u) & 0x003Fu;

        output->data = incr;
        output->op   = store ? OPCODE_STM : OPCODE_LDM;
        output->size = size;
        output->operands[0] = value;
        output->operands[1] = src;
        output->operands[2] = dest;
    }
    else if(type == 1) //Subtypes
    {
        const uint32_t subtype = (opcode >> 4u) & 0x03u;

        if(subtype == 0) // LDMX/STMX
        {
            const uint32_t store = (opcode >> 6u ) & 0x0001u;
            const uint32_t size  = (opcode >> 7u ) & 0x0003u;
            const uint32_t value = (opcode >> 9u ) & 0xFFFFu;
            const uint32_t src   = (opcode >> 25u) & 0x0001u;
            const uint32_t dest  = (opcode >> 26u) & 0x003Fu;

            output->data = 0;
            output->op   = store ? OPCODE_STMX : OPCODE_LDMX;
            output->size = size;
            output->operands[0] = value;
            output->operands[1] = src + 62;
            output->operands[2] = dest;
        }
        else if(subtype == 1) //IN/OUT
        {
            const uint32_t store = (opcode >> 6u ) & 0x01u;
            const uint32_t size  = (opcode >> 7u ) & 0x03u;
            const uint32_t value = (opcode >> 16u) & 0xFFu;
            const uint32_t dest  = (opcode >> 26u) & 0x3Fu;

            output->data = 0;
            output->op   = store ? OPCODE_OUT : OPCODE_IN;
            output->size = size;
            output->operands[0] = value;
            output->operands[2] = dest;
        }
        else if(subtype == 2) //OUTI
        {
            const uint32_t size  = (opcode >> 7u ) & 0x0001u;
            const uint32_t value = (opcode >> 16u) & 0xFFFFu;
            const uint32_t dest  = (opcode >> 26u) & 0x00FFu;

            output->data = 0;
            output->op   = OPCODE_OUTI;
            output->size = size;
            output->operands[0] = value;
            output->operands[2] = dest;
        }
        else //LDV/STV
        {
            return 0;
        }
    }
    else if(type == 2) //LDC/STC
    {
        const uint32_t incr  = (opcode >> 4u ) & 0x0001u;
        const uint32_t store = (opcode >> 5u ) & 0x0001u;
        const uint32_t size  = (opcode >> 6u ) & 0x0003u;
        const uint32_t value = (opcode >> 8u ) & 0x0FFFu;
        const uint32_t src   = (opcode >> 20u) & 0x003Fu;
        const uint32_t dest  = (opcode >> 26u) & 0x003Fu;

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

static const Opcode ALURegRegRegOpcodes[] =
{
    OPCODE_ADD,
    OPCODE_SUB,
    OPCODE_MULS,
    OPCODE_MULU,
    OPCODE_DIVS,
    OPCODE_DIVU,
    OPCODE_AND,
    OPCODE_OR,
    OPCODE_XOR,
    OPCODE_ASL,
    OPCODE_LSL,
    OPCODE_ASR,
    OPCODE_LSR,
    OPCODE_UNKNOWN,
    OPCODE_UNKNOWN,
    OPCODE_UNKNOWN,
};

static const Opcode ALURegRegImmOpcodes[] =
{
    OPCODE_ADDI,
    OPCODE_SUBI,
    OPCODE_MULSI,
    OPCODE_MULUI,
    OPCODE_DIVSI,
    OPCODE_DIVUI,
    OPCODE_ANDI,
    OPCODE_ORI,
    OPCODE_XORI,
    OPCODE_ASLI,
    OPCODE_LSLI,
    OPCODE_ASRI,
    OPCODE_LSRI,
    OPCODE_UNKNOWN,
    OPCODE_UNKNOWN,
    OPCODE_UNKNOWN,
};

static const Opcode ALURegImmOpcodes[] =
{
    OPCODE_ADDQ,
    OPCODE_SUBQ,
    OPCODE_MULSQ,
    OPCODE_MULUQ,
    OPCODE_DIVSQ,
    OPCODE_DIVUQ,
    OPCODE_ANDQ,
    OPCODE_ORQ,
    OPCODE_XORQ,
    OPCODE_ASLQ,
    OPCODE_LSLQ,
    OPCODE_ASRQ,
    OPCODE_LSRQ,
    OPCODE_UNKNOWN,
    OPCODE_UNKNOWN,
    OPCODE_UNKNOWN,
};

static int decodeALU(uint32_t opcode, Operation* restrict output)
{
    const uint32_t category = (opcode >> 2u) & 0x03u;

    if(category == 0)
    {
        const uint32_t type = (opcode >> 4u) & 0x07u;

        if(type == 0) // ALU REG-REG-REG (ADD, SUB, ...)
        {
            const uint32_t op   = (opcode >> 8u ) & 0x0Fu;
            const uint32_t size = (opcode >> 12u) & 0x03u;
            const uint32_t src1 = (opcode >> 14u) & 0x3Fu;
            const uint32_t src2 = (opcode >> 20u) & 0x3Fu;
            const uint32_t dest = (opcode >> 26u) & 0x3Fu;

            output->op   = ALURegRegRegOpcodes[op];
            output->size = size;
            output->operands[0] = src1;
            output->operands[1] = src2;
            output->operands[2] = dest;

            if(output->op == OPCODE_UNKNOWN)
            {
                return 0;
            }
        }
        else if(type == 2) // XCHG
        {
            output->op = OPCODE_XCHG;
        }
        else if(type == 6) // NOP NOP.E
        {
            const uint32_t end = (opcode >> 7u) & 0x01u;

            output->data = end;
            output->op   = OPCODE_NOP;
        }
        else
        {
            return 0;
        }
    }
    else if(category == 1) // ALU REG-REG-IMM (ADDI, SUBI, ...)
    {
        const uint32_t op    = (opcode >> 4u ) & 0x000Fu;
        const uint32_t size  = (opcode >> 8u ) & 0x0003u;
        const uint32_t value = (opcode >> 10u) & 0x03FFu;
        const uint32_t src   = (opcode >> 20u) & 0x003Fu;
        const uint32_t dest  = (opcode >> 26u) & 0x003Fu;

        output->op   = ALURegRegImmOpcodes[op];
        output->size = size;
        output->operands[0] = value;
        output->operands[1] = src;
        output->operands[2] = dest;

        if(output->op == OPCODE_UNKNOWN)
        {
            return 0;
        }
    }
    else if(category == 2) // ALU REG-IMM (ADDQ, SUBQ, ...)
    {
        const uint32_t op    = (opcode >> 4u ) & 0x000Fu;
        const uint32_t size  = (opcode >> 8u ) & 0x0003u;
        const uint32_t value = (opcode >> 10u) & 0xFFFFu;
        const uint32_t dest  = (opcode >> 26u) & 0x003Fu;

        output->op   = ALURegImmOpcodes[op];
        output->size = size;
        output->operands[0] = value;
        output->operands[2] = dest;

        if(output->op == OPCODE_UNKNOWN)
        {
            return 0;
        }
    }
    else // MOVEI
    {
        const uint32_t value = (opcode >> 4u ) & 0x3FFFFFu;
        const uint32_t dest  = (opcode >> 26u) & 0x00003Fu;

        output->op = OPCODE_MOVEI;
        output->operands[0] = value;
        output->operands[2] = dest;
    }

    return 1;
}

static int decodeFPU(uint32_t opcode, Operation* restrict output)
{
    (void)opcode;
    (void)output;

    return 1;
}

static int decode(uint32_t opcode, Operation* restrict output)
{
    const uint32_t compute_unit = opcode & 0x03;

    if(compute_unit == 0) //AGU or BRU
    {
        return decodeAGUBRU(opcode, output);
    }
    else if(compute_unit == 1) //LSU
    {
        return decodeLSU(opcode, output);
    }
    else if(compute_unit == 2) //ALU
    {
        return decodeALU(opcode, output);
    }
    else //FPU
    {
        return decodeFPU(opcode, output);
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
    assert(processor);

    return AR_SUCCESS;
}

ArResult arExecuteDirectMemoryAccess(ArProcessor processor)
{
    assert(processor);

    return AR_SUCCESS;
}
