#include <assert.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>

#include "vm.h"

static uint64_t extendSign(uint32_t value, uint32_t bits)
{
    if(value > (1u << (bits - 1)))
    {
        return ((0xFFFFFFFFu << bits) | value);
    }
    else
    {
        return value;
    }
}

static uint32_t readbits(uint32_t buffer, uint32_t index, uint32_t size)
{
    return (buffer >> index) & ((1u << size) - 1);
}

static float convertImmFloat(uint32_t imm)
{
    uint32_t xval;
    uint32_t exp = (imm&0x3C00)>>3;

    if(imm&0x4000)
    {
        exp |= 0x4000;
    }
    else
    {
        if(exp != 0)
            exp |= 0x3800;
    }

    exp = exp<<16;

    xval  = (imm&0x8000)<<16; //sign
    xval += (imm&0x03FF)<<13; //mantisse
    xval += exp;

    float fval;

    memcpy(&fval, &xval, sizeof(xval));

    return fval;
}


//-----------------------

static int decodeBRU(ComputeUnit *processor,ArExecute_T *output,uint32_t unit,uint32_t imm)
{
    output->unit1 = AK1_EXE_BRU;
    processor->delayop = output->unit2 = unit;
    processor->delay = 0;

    processor->imm = (imm&0xFFFF)<<1;

    return 0;
}

static int decodeCMP(ComputeUnit *processor,ArExecute_T *output,uint32_t unit,uint64_t imm)
{
	if(unit == 0) //CMP
	{
		output->unit1 = AK1_EXE_CMP;
		output->unit2 = AK1_OPCODE_CMP;
	}
	else if(unit == 0x1) //ENDP
	{
		output->unit1 = AK1_EXE_OTHER;
		output->unit2 = AK1_OPCODE_ENDP;

	}
	else if(unit == 0x2) //RET
	{
		output->unit1 = AK1_EXE_BRU;
		processor->delayop = output->unit2 = AK1_OPCODE_RET;
		processor->delay = 0;
	}
	else if(unit == 0x3) //ENDB
	{
		//output->unit1 = AK1_EXE_OTHER;
		//output->unit2 = AK1_OPCODE_INT;
	}
	else
	{
		return 1;
	}

    return 0;
}


static int decodeALU(ComputeUnit *processor,ArExecute_T *output,uint32_t unit1,uint32_t unit2,uint64_t imm)
{
    output->unit1 = AK1_EXE_ALU;

    if(unit1 == 0) //REG/REG
    {
        output->unit2 = unit2;
    }
    else if(unit1 == 1) //REG/IMM
    {
        output->unit2 = unit2;
        output->imm = extendSign(imm,10);
        output->immf = 1;
    }
    else if(unit1 == 2) //other
    {
        if(unit2 == 0)
        {
            output->unit1 = AK1_EXE_NOP;
        }
        else
        {
            if(unit2&0xF)
                output->unit2 = AK1_OPCODE_MOVESP;
            else
                output->unit2 = AK1_OPCODE_MOVE;
        }
    }
    else //MOVEI
    {
        output->unit2 = AK1_OPCODE_MOVE;
        output->imm = extendSign(imm,20);
        output->immf = 1;

    }
    return 0;
}


static int decodeLSU(ComputeUnit *processor,ArExecute_T *output,uint32_t unit1,uint32_t unit2,uint64_t imm)
{
    output->unit1 = AK1_EXE_LSU;
    if(unit1 == 0) //LS
    {
        output->unit2 = unit2&0x7;
        if(unit2&0x8)
		{
			output->imm = imm&0x3FF;
			output->immf = 1;
		}

    }
    else if(unit1 == 1)
    {
        output->unit2 = unit2;
        if(unit2&0x8) //PREFETCH/FLUSH
        {
            if(unit2&0x4)
                return 1;

            if( (unit2&0x2) == 0)
			{
				output->imm = imm&0xFFFF;
				output->immf = 1;
			}


        }else //LS Long IMM
        {
            output->imm = imm&0xFFFF;

            if(unit2&0x2)
                output->rC = 1;
            else
                output->rC = 0;
        }
    }
    else if(unit1 == 2)
    {
        return 1;
    }
    else //----
    {
        return 1;
    }

    return 0;
}


static int decodeDMA(ComputeUnit *processor,ArExecute_T *output,uint32_t unit2,uint64_t imm)
{
    output->unit1 = AK1_EXE_DMA;
    output->unit2 = unit2&0x7;

    if(unit2&0x8)
	{
		output->imm = imm&0x3FF;
		output->immf = 1;
	}

    return 0;
}


static int decodeVFPU(ComputeUnit *processor,ArExecute_T *output,uint32_t unit1,uint32_t unit2,uint64_t imm)
{
    output->unit1 = AK1_EXE_VFPU;
    output->unit2 = unit2 + (unit1<<4);

    if(output->unit2 == 0x0F) //VFSHUFFLE
    {
        output->imm = imm&0x3FF;
    }
    else if(output->unit2 == 0x12) //FMOVEI
    {
        output->fimm = convertImmFloat(imm);
        output->unit2 = AK1_OPCODE_FMOVE + (unit1<<4);
        output->immf = 1;
    }
    else if(output->unit2 == 0x13) //VFMOVEI
    {
		output->fimm = convertImmFloat(imm);
        output->unit2 = AK1_OPCODE_VFMOVE + (unit1<<4);
        output->immf = 1;
    }


    return 0;
}


static int decode(ComputeUnit *processor,uint32_t id)
{
    const uint32_t opcode = processor->opcodes[id];
    const uint32_t compute_unit = readbits(opcode, 0, 2);
    const uint32_t imm = readbits(opcode, 10, 16) + (readbits(opcode, 4, 4)<<16);

    const uint32_t unit1 = readbits(opcode, 2, 2);
    const uint32_t unit2 = readbits(opcode, 4, 4);


    processor->operations[id].size = readbits(opcode, 8, 2);
    processor->operations[id].id   = readbits(opcode, 10, 2);
    uint64_t regA = readbits(opcode, 26, 6);
    uint64_t regB = readbits(opcode, 20, 6);
    uint64_t regC = readbits(opcode, 14, 6);


    //Read Register
    processor->operations[id].rA = regA;
    processor->operations[id].rB = regB;
    processor->operations[id].rC = regC;

    processor->operations[id].immf = 0;


    //printf("opcode %x\n",opcode);

    ArExecute_T *output = &processor->operations[id];

    if( (opcode&0xFF) != 0x0A) //NOP
        processor->instruction++;

    //Decode /issue
    if(compute_unit == 0)
    {
        if(id == 0) //CMP/BRU
        {
            if(unit1 == 0)
            {
                return decodeCMP(processor,output,unit2,imm);
            }
            else if(unit1 == 1)
            {
                return decodeBRU(processor,output,unit2,imm);
            }
            else if(unit1 == 2)
            {
                return 1;
            }
            else //CMPI
            {
                output->unit1 = AK1_EXE_CMP;
                output->unit2 = AK1_OPCODE_CMP;
                output->imm   = extendSign(imm,20);
                output->immf = 1;
            }
        }
        else //DMA
        {
            return decodeDMA(processor,output,unit2,imm);

        }
    }
    else if(compute_unit == 1) //LSU
    {
        return decodeLSU(processor,output,unit1,unit2,imm);
    }
    else if(compute_unit == 2) //ALU
    {
        return decodeALU(processor,output,unit1,unit2,imm);
    }
    else //VFPU
    {
        return decodeVFPU(processor,output,unit1,unit2,imm);
    }

    return 0;
}


int32_t Aldebaran_DecodeInstruction(ComputeUnit *processor)
{
    int error = 0;
    assert(processor);

    //Fetch
    memcpy(processor->opcodes, processor->isram + (processor->pc * 4), 2 * sizeof(uint32_t));

    error += decode(processor,0);
    error += decode(processor,1);

    processor->pc += 2;

    if(error != 0)
        return 0;

    return AR_SUCCESS;
}
