#include "vm.h"

#include <assert.h>
#include <string.h>
#include <math.h>

static int32_t extendSign(uint32_t value, uint32_t bits)
{
    if(value > (1u << (bits - 1)))
    {
        return (int32_t)((0xFFFFFFFFu << bits) | value);
    }
    else
    {
        return (int32_t)value;
    }
}

static uint32_t readbits(uint32_t buffer, uint32_t index, uint32_t size)
{
    return (buffer >> index) & ((1u << size) - 1);
}

static const ArOpcode BRUComparators[16] =
{
    AR_OPCODE_BNE,
    AR_OPCODE_BEQ,
    AR_OPCODE_BL,
    AR_OPCODE_BLE,
    AR_OPCODE_BG,
    AR_OPCODE_BGE,
    AR_OPCODE_BLS,
    AR_OPCODE_BLES,
    AR_OPCODE_BGS,
    AR_OPCODE_BGES,
    AR_OPCODE_BRA,
    AR_OPCODE_UNKNOWN,
    AR_OPCODE_UNKNOWN,
    AR_OPCODE_UNKNOWN,
    AR_OPCODE_UNKNOWN,
    AR_OPCODE_UNKNOWN
};

static const ArOpcode BRUJumpsCalls[4] =
{
    AR_OPCODE_CALL,
    AR_OPCODE_JMP,
    AR_OPCODE_CALLR,
    AR_OPCODE_JMPR
};

static int decodeBRU(uint32_t br, uint32_t pc, uint32_t opcode, ArOperation* restrict output)
{
    const uint32_t type = readbits(opcode, 2, 2);

    if(type == 0) //REG-REG comparison or Branch
    {
        const uint32_t instruction = readbits(opcode, 4, 2);

        if(instruction == 0) //CMP
        {
            const uint32_t size  = readbits(opcode, 8,  2);
            const uint32_t right = readbits(opcode, 20, 6);
            const uint32_t left  = readbits(opcode, 26, 6);

            output->op = AR_OPCODE_CMP;
            output->size = size;
            output->operands[0] = right;
            output->operands[1] = left;
        }
        else if(instruction == 1) //PCMP.H
        {
            const uint32_t roff  = readbits(opcode, 6,  2);
            const uint32_t loff  = readbits(opcode, 8,  2);
            const uint32_t right = readbits(opcode, 20, 6);
            const uint32_t left  = readbits(opcode, 26, 6);

            output->op = AR_OPCODE_PCMP;
            output->operands[0] = right * 4 + roff;
            output->operands[1] = left  * 4 + loff;
            output->size = 0;
        }
        else if(instruction == 2) //PCMP.S
        {
            const uint32_t right = readbits(opcode, 20, 6);
            const uint32_t left  = readbits(opcode, 26, 6);

            output->op = AR_OPCODE_PCMP;
            output->operands[0] = right;
            output->operands[1] = left;
            output->size = 1;
        }
        else //Branching
        {
            const uint32_t category = readbits(opcode, 6, 2);

            if(category == 0) //Branch (BCC, BCCS, PBCC, BCA)
            {
                const uint32_t comp  = readbits(opcode, 8,  4);
                const uint32_t label = readbits(opcode, 18, 14);

                output->op = BRUComparators[comp];
                output->operands[0] = pc + extendSign(label, 14) * 2;

                if(output->op == AR_OPCODE_UNKNOWN)
                {
                    return 0;
                }
            }
            else if(category == 1) //Illegal
            {
                return 0;
            }
            else if(category == 2) //Jumps or calls
            {
                const uint32_t subtype = readbits(opcode, 8,  4);
                const uint32_t label   = readbits(opcode, 18, 14);

                if(subtype < 4)
                {
                    const uint32_t relative = readbits(opcode, 8, 1);

                    output->op = BRUJumpsCalls[subtype];
                    output->operands[0] = relative ? br + extendSign(label, 14) * 2 //relative
                                                   : label * 2u; //absolute
                    output->operands[1] = pc + 2;
                }
                else if(subtype == 4) //ENDP
                {
                    output->op = AR_OPCODE_ENDP;
                }
                else if((subtype & 0x06u) != 0) //SWITCH
                {
                    const uint32_t value = readbits(opcode, 8, 1);

                    output->op = AR_OPCODE_SWT;
                    output->operands[0] = value;
                }
            }
            else //Ret
            {
                output->op = AR_OPCODE_RET;
            }
        }
    }
    else if(type == 1) //CMPI
    {
        const uint32_t size  = readbits(opcode, 4,  2);
        const uint32_t value = readbits(opcode, 6,  20);
        const uint32_t reg   = readbits(opcode, 26, 6);

        output->op = AR_OPCODE_CMPI;
        output->size = size;
        output->operands[0] = value;
        output->operands[1] = reg;
    }
    else if(type == 2) //PCMPI.H
    {
        const uint32_t loff  = readbits(opcode, 4,  2);
        const uint32_t value = readbits(opcode, 10, 16);
        const uint32_t left  = readbits(opcode, 26, 6);

        output->op = AR_OPCODE_PCMPI;
        output->operands[0] = value;
        output->operands[1] = left * 4 + loff;
        output->size = 0;
    }
    else //PCMPI.S
    {
        const uint32_t value = readbits(opcode, 4,  22);
        const uint32_t reg   = readbits(opcode, 26, 6);

        output->op = AR_OPCODE_PCMPI;
        output->operands[0] = value;
        output->operands[1] = reg;
        output->size = 1;
    }

    return 1;
}

//bit 0: load or store
//bit 1: cache or sdram
//bit 2: increment or not

static const ArOpcode LSUREGLoadStore[8] =
{
    AR_OPCODE_LDM,
    AR_OPCODE_STM,
    AR_OPCODE_LDC,
    AR_OPCODE_STC,
    AR_OPCODE_LDMI,
    AR_OPCODE_STMI,
    AR_OPCODE_LDCI,
    AR_OPCODE_STCI,
};

static const ArOpcode LSUPositLoadStore[8] =
{
    AR_OPCODE_LDMV,
    AR_OPCODE_STMV,
    AR_OPCODE_LDCV,
    AR_OPCODE_STCV,
    AR_OPCODE_LDMVI,
    AR_OPCODE_STMVI,
    AR_OPCODE_LDCVI,
    AR_OPCODE_STCVI,
};

static int decodeLSU(uint32_t opcode, ArOperation* restrict output)
{
    const uint32_t type = readbits(opcode, 2, 2);

    if(type < 2) //REG Load/Store
    {
        const uint32_t cache = readbits(opcode, 2,  1);
        const uint32_t store = readbits(opcode, 4,  1);
        const uint32_t incr  = readbits(opcode, 5,  1);
        const uint32_t size  = readbits(opcode, 6,  2);
        const uint32_t value = readbits(opcode, 8,  14);
        const uint32_t src   = readbits(opcode, 23, 3);
        const uint32_t reg   = readbits(opcode, 26, 6);

        const size_t index = (incr << 2u) | (cache << 1u) | store;

        output->op   = LSUREGLoadStore[index];
        output->size = size;
        output->operands[0] = value;
        output->operands[1] = src;
        output->operands[2] = reg;
    }
    else if(type == 2) //Input/output
    {
        const uint32_t subtype = readbits(opcode, 4, 2);

        if(subtype == 0) //IN
        {
            const uint32_t size = readbits(opcode, 8,  2);
            const uint32_t addr = readbits(opcode, 18, 8);
            const uint32_t reg  = readbits(opcode, 26, 6);

            output->op   = AR_OPCODE_IN;
            output->size = size;
            output->operands[0] = addr;
            output->operands[2] = reg;
        }
        else if(subtype == 1) //OUT
        {
            const uint32_t size = readbits(opcode, 8,  2);
            const uint32_t addr = readbits(opcode, 18, 8);
            const uint32_t reg  = readbits(opcode, 26, 6);

            output->op   = AR_OPCODE_OUT;
            output->size = size;
            output->operands[0] = addr;
            output->operands[2] = reg;
        }
        else //OUTI
        {
            const uint32_t size  = readbits(opcode, 4,  1);
            const uint32_t value = readbits(opcode, 8,  16);
            const uint32_t addr  = readbits(opcode, 24, 8);

            output->op   = AR_OPCODE_OUTI;
            output->size = size;
            output->operands[0] = addr;
            output->operands[2] = value;
        }
    }
    else //Posit Load/Store
    {
        const uint32_t index = readbits(opcode, 4,  3);
        const uint32_t size  = readbits(opcode, 8,  2);
        const uint32_t value = readbits(opcode, 10, 14);
        const uint32_t addr  = readbits(opcode, 23, 3);
        const uint32_t reg   = readbits(opcode, 26, 6);

        output->op   = LSUPositLoadStore[index];
        output->size = size;
        output->operands[0] = value;
        output->operands[1] = addr;
        output->operands[2] = reg;
    }

    return 1;
}

static const ArOpcode ALURegRegRegOpcodes[16] =
{
    AR_OPCODE_ADD,
    AR_OPCODE_SUB,
    AR_OPCODE_MULS,
    AR_OPCODE_MULU,
    AR_OPCODE_DIVS,
    AR_OPCODE_DIVU,
    AR_OPCODE_AND,
    AR_OPCODE_OR,
    AR_OPCODE_XOR,
    AR_OPCODE_ASL,
    AR_OPCODE_LSL,
    AR_OPCODE_ASR,
    AR_OPCODE_LSR,
    AR_OPCODE_UNKNOWN,
    AR_OPCODE_UNKNOWN,
    AR_OPCODE_UNKNOWN,
};

static const ArOpcode ALURegRegImmOpcodes[16] =
{
    AR_OPCODE_ADDI,
    AR_OPCODE_SUBI,
    AR_OPCODE_MULSI,
    AR_OPCODE_MULUI,
    AR_OPCODE_DIVSI,
    AR_OPCODE_DIVUI,
    AR_OPCODE_ANDI,
    AR_OPCODE_ORI,
    AR_OPCODE_XORI,
    AR_OPCODE_ASLI,
    AR_OPCODE_LSLI,
    AR_OPCODE_ASRI,
    AR_OPCODE_LSRI,
    AR_OPCODE_UNKNOWN,
    AR_OPCODE_UNKNOWN,
    AR_OPCODE_UNKNOWN,
};

static const ArOpcode ALURegImmOpcodes[16] =
{
    AR_OPCODE_ADDQ,
    AR_OPCODE_SUBQ,
    AR_OPCODE_MULSQ,
    AR_OPCODE_MULUQ,
    AR_OPCODE_DIVSQ,
    AR_OPCODE_DIVUQ,
    AR_OPCODE_ANDQ,
    AR_OPCODE_ORQ,
    AR_OPCODE_XORQ,
    AR_OPCODE_ASLQ,
    AR_OPCODE_LSLQ,
    AR_OPCODE_ASRQ,
    AR_OPCODE_LSRQ,
    AR_OPCODE_UNKNOWN,
    AR_OPCODE_UNKNOWN,
    AR_OPCODE_UNKNOWN,
};

static int decodeALU(uint32_t opcode, ArOperation* restrict output)
{
    const uint32_t category = readbits(opcode, 2, 2);

    if(category == 0)
    {
        const uint32_t type = readbits(opcode, 4, 4);

        if(type == 0) // ALU REG-REG-REG (ADD, SUB, ...)
        {
            const uint32_t op    = readbits(opcode, 8,  4);
            const uint32_t size  = readbits(opcode, 12, 2);
            const uint32_t right = readbits(opcode, 14, 6);
            const uint32_t left  = readbits(opcode, 20, 6);
            const uint32_t dest  = readbits(opcode, 26, 6);

            output->op   = ALURegRegRegOpcodes[op];
            output->size = size;
            output->operands[0] = right;
            output->operands[1] = left;
            output->operands[2] = dest;

            if(output->op == AR_OPCODE_UNKNOWN)
            {
                return 0;
            }
        }
        else if(type == 1) //NOP
        {
            output->op = AR_OPCODE_NOP;
        }
        else if(type == 2) //MOVELRL
        {
            const uint32_t dest = readbits(opcode, 26, 6);

            output->op = AR_OPCODE_MOVELRL;
            output->operands[0] = dest;
        }
        else if(type == 3) //MOVELRS
        {
            const uint32_t src = readbits(opcode, 26, 6);

            output->op = AR_OPCODE_MOVELRS;
            output->operands[0] = src;
        }
        else if(type == 4) //MOVEBR
        {
            const uint32_t src = readbits(opcode, 26, 6);

            output->op = AR_OPCODE_MOVEBR;
            output->operands[0] = src;
        }
        else
        {
            return 0;
        }
    }
    else if(category == 1) // ALU REG-REG-IMM (ADDI, SUBI, ...)
    {
        const uint32_t op    = readbits(opcode, 4,  4);
        const uint32_t size  = readbits(opcode, 8,  2);
        const uint32_t value = readbits(opcode, 10, 10);
        const uint32_t src   = readbits(opcode, 20, 6);
        const uint32_t dest  = readbits(opcode, 26, 6);

        output->op   = ALURegRegImmOpcodes[op];
        output->size = size;
        output->operands[0] = value;
        output->operands[1] = src;
        output->operands[2] = dest;

        if(output->op == AR_OPCODE_UNKNOWN)
        {
            return 0;
        }
    }
    else if(category == 2) // ALU REG-IMM (ADDQ, SUBQ, ...)
    {
        const uint32_t op    = readbits(opcode, 4,  4);
        const uint32_t size  = readbits(opcode, 8,  2);
        const uint32_t value = readbits(opcode, 10, 16);
        const uint32_t dest  = readbits(opcode, 26, 6);

        output->op   = ALURegImmOpcodes[op];
        output->size = size;
        output->operands[0] = value;
        output->operands[2] = dest;

        if(output->op == AR_OPCODE_UNKNOWN)
        {
            return 0;
        }
    }
    else // MOVEI
    {
        const uint32_t size  = readbits(opcode, 4,  2);
        const uint32_t value = readbits(opcode, 6,  20);
        const uint32_t dest  = readbits(opcode, 26, 6);

        output->op   = AR_OPCODE_MOVEI;
        output->size = size;
        output->operands[0] = value;
        output->operands[2] = dest;
    }

    return 1;
}

static ArOpcode AGUOpcodes[16] =
{
    AR_OPCODE_LDDMAR,
    AR_OPCODE_STDMAR,
    AR_OPCODE_DMAIR,
    AR_OPCODE_LDDMAL,
    AR_OPCODE_STDMAL,
    AR_OPCODE_UNKNOWN,
    AR_OPCODE_UNKNOWN,
    AR_OPCODE_UNKNOWN,
    AR_OPCODE_UNKNOWN,
    AR_OPCODE_UNKNOWN,
    AR_OPCODE_CLEARC,
    AR_OPCODE_WAIT,
    AR_OPCODE_UNKNOWN,
    AR_OPCODE_UNKNOWN,
    AR_OPCODE_UNKNOWN,
    AR_OPCODE_UNKNOWN,
};

static int decodeAGU(uint32_t opcode, ArOperation* restrict output)
{
    const uint32_t category = readbits(opcode, 2, 1);

    if(category == 0) //LDDMA/STDMA
    {
        const uint32_t store = readbits(opcode, 3,  1);
        const uint32_t size  = readbits(opcode, 4,  1);
        const uint32_t sram  = readbits(opcode, 5,  2);
        const uint32_t ram   = readbits(opcode, 7,  1);
        const uint32_t sramb = readbits(opcode, 8,  12);
        const uint32_t ramb  = readbits(opcode, 20, 12);

        output->op   = store ? AR_OPCODE_STDMA : AR_OPCODE_LDDMA;
        output->size = size;
        output->operands[0] = sram + 60;
        output->operands[1] = ram + 58;
        output->data = (ramb << 12u) | sramb;
    }
    else
    {
        const uint32_t type = readbits(opcode, 4, 4);

        output->op = AGUOpcodes[type];

        if(type < 8) //LDDMAR/STDMAR/DMAIR/LDDMAL/STDMAL
        {
            const uint32_t value = readbits(opcode, 8,  12);
            const uint32_t src   = readbits(opcode, 20, 6);
            const uint32_t dest  = readbits(opcode, 26, 6);

            output->operands[0] = value;
            output->operands[1] = src;
            output->operands[2] = dest;
        }

        if(output->op == AR_OPCODE_UNKNOWN)
        {
            return 0;
        }
    }

    return 1;
}

static int decodeVPU(uint32_t opcode, ArOperation* restrict output)
{
    (void)opcode;
    (void)output;

    return 1;
}

static int decode(uint32_t index, uint32_t br, uint32_t pc, uint32_t opcode, ArOperation* restrict output)
{
    const uint32_t compute_unit = readbits(opcode, 0, 2);

    if(index == 0)
    {
        if(compute_unit == 0) //BRU
        {
            return decodeBRU(br, pc, opcode, output);
        }
        else if(compute_unit == 1) //LSU
        {
            return decodeLSU(opcode, output);
        }
        else if(compute_unit == 2) //ALU
        {
            return decodeALU(opcode, output);
        }
        else //VFPU/VDIV
        {
            return decodeVPU(opcode, output);
        }
    }
    else if(index == 1)
    {
        if(compute_unit == 0) //AGU
        {
            return decodeAGU(opcode, output);
        }
        else if(compute_unit == 1) //LSU
        {
            return decodeLSU(opcode, output);
        }
        else if(compute_unit == 2) //ALU
        {
            return decodeALU(opcode, output);
        }
        else //VFPU
        {
            return decodeVPU(opcode, output);
        }
    }
    else //2 or 3
    {
        if(compute_unit == 2) //ALU
        {
            return decodeALU(opcode, output);
        }
        else //VFPU
        {
            return 0;
        }
    }
}


ArResult arDecodeInstruction(ArProcessor processor)
{
    assert(processor);

    const uint32_t size = opcodeSetSize(processor);
    memcpy(processor->opcodes, processor->isram + (processor->pc * 4), size * sizeof(uint32_t));

    for(uint32_t i = 0; i < size; ++i)
    {
        if(!decode(i, processor->br, processor->pc, processor->opcodes[i], &processor->operations[i]))
        {
            return AR_ERROR_ILLEGAL_INSTRUCTION;
        }
    }

    processor->pc += size;

    return AR_SUCCESS;
}
