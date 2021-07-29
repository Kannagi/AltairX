#include "vm.h"

#include <assert.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#include <stdio.h>

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

static double convertImmDouble(uint64_t imm)
{
    uint64_t xval;
    uint64_t exp = (imm&0x3C00)>>3;

    if(imm&0x4000)
    {
        exp |= 0x4000;
    }
    else
    {
        if(exp != 0)
            exp |= 0x3800;
    }

    exp = exp<<48;

    xval  = (imm&0x8000)<<48; //sign
    xval += (imm&0x03FF)<<42; //mantisse
    xval += exp;

    double dval;

    memcpy(&dval, &xval, sizeof(xval));

    return dval;
}

//-----------------------

static int decodeBRU(ArProcessor processor,ArExecute_T *output,uint32_t unit,uint32_t imm)
{
    output->unit1 = AK1_EXE_BRU;
    processor->delayop = output->unit2 = unit;
    processor->delay = 0;

    processor->imm = (imm&0xFFFF)<<1;

    return 0;
}

static int decodeCMP(ArProcessor processor,ArExecute_T *output,uint32_t unit,uint64_t imm)
{
    if(unit&0x8) //OTHER
    {
        if(unit == 0x8) //ENDP
        {
            output->unit1 = AK1_EXE_OTHER;
            output->unit2 = AK1_OPCODE_ENDP;
            
        }
        else if(unit == 0x9) //RET
        {
            output->unit1 = AK1_EXE_BRU;
            processor->delayop = output->unit2 = AK1_OPCODE_RET;
            processor->delay = 0;
        }
        else if(unit == 0xA) //INT
        {
            output->unit1 = AK1_EXE_OTHER;
            output->unit2 = AK1_OPCODE_INT;
            output->opB = imm&0xFFFF;
        }
        else if(unit == 0xB) //RETI
        {
            output->unit1 = AK1_EXE_BRU;
            output->unit2 = AK1_OPCODE_RETI;
        }
        else
        {
            return 1;
        }

    }else //CMP
    {
        output->unit1 = AK1_EXE_CMP;
        output->unit2 = (unit&0x1)+0x4;

        if(unit == 0) //CMP
        {
            output->opA = processor->ireg[output->opA];
        }
        else if(unit == 6) //FCMPI
        {
            output->id = (output->size+1)&0x3;

            output->fopA[output->size] = processor->vreg[(output->opA*4)+output->size];
            output->fopB[output->id] = convertImmFloat(imm);
        }
        else if(unit == 7) //DCMPI
        {
            output->dopA = processor->dreg[(output->opA*2)];
            output->dopB = convertImmDouble(imm);
        }

    }
    return 0;
}


static int decodeALU(ArProcessor processor,ArExecute_T *output,uint32_t unit1,uint32_t unit2,uint64_t imm)
{
    output->unit1 = AK1_EXE_ALU;

    if(unit1 == 0) //REG/REG
    {
        output->unit2 = unit2;
    }
    else if(unit1 == 1) //REG/IMM
    {
        output->unit2 = unit2;
        output->opC = extendSign(imm,10);
    }
    else if(unit1 == 2) //other
    {
        if(unit2 == 0)
            output->unit1 = AK1_EXE_NOP;
        else
            output->unit2 = AK1_OPCODE_MOVE+0x10;
    }
    else //MOVEI
    {
        output->unit2 = AK1_OPCODE_MOVE;
        output->opB = extendSign(imm,20);

    }
    return 0;
}


static int decodeLSU(ArProcessor processor,ArExecute_T *output,uint32_t unit1,uint32_t unit2,uint64_t imm)
{
    output->unit1 = AK1_EXE_LSU;
    if(unit1 == 0) //LS
    {
        output->unit2 = unit2&0x7;
        if(unit2&0x8)
            output->opC = imm&0x3FF;
    }
    else if(unit1 == 1) 
    {
        output->unit2 = unit2;
        if(unit2&0x8) //PREFETCH/FLUSH
        {
            if(unit2&0x4)
                return 1;

            if( (unit2&0x2) == 0)
                output->opB = imm&0xFFFF;

        }else //LS Long IMM
        {
            output->opB = imm&0xFFFF;
            if(unit2&0x2) 
                output->opC = processor->ireg[1];
            else
                output->opC = processor->ireg[0];
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


static int decodeDMA(ArProcessor processor,ArExecute_T *output,uint32_t unit2,uint64_t imm)
{
    output->unit1 = AK1_EXE_DMA;
    output->unit2 = unit2&0x7;

    if(unit2&0x8)
        output->opC = imm&0x3FF;

    return 0;
}


static int decodeVFPU(ArProcessor processor,ArExecute_T *output,uint32_t unit1,uint32_t unit2,uint64_t imm)
{
    output->unit1 = AK1_EXE_VFPU;
    output->unit2 = unit2 + (unit1<<4);

    if(output->unit2 == 0x0F) //VFSHUFFLE
    {
        output->opC = imm&0x3FF;
    }
    else if(output->unit2 == 0x12) //FMOVEI
    {
        output->id = (output->size+1)&0x3;
        output->fopB[output->id] = convertImmFloat(imm);
        output->unit2 = AK1_OPCODE_FMOVE + (unit1<<4);
    }
    else if(output->unit2 == 0x13) //VFMOVEI
    {
        float tmp = convertImmFloat(imm);
        for(int i = 0;i < output->size;i++)
            output->fopB[i] = tmp;
        output->unit2 = AK1_OPCODE_VFMOVE + (unit1<<4);
    }
    else if(output->unit2 == 0x30) //DMOVEI
    {
        output->dopB = convertImmDouble(imm);
        output->unit2 = AK1_OPCODE_DMOVE + (unit1<<4);
    }


    return 0;
}


static int decode(ArProcessor processor,uint32_t id)
{
    const uint32_t opcode = processor->opcodes[id];
    const uint32_t compute_unit = readbits(opcode, 0, 2);
    const uint32_t imm = readbits(opcode, 10, 16) + (readbits(opcode, 4, 4)<<16);
    
    const uint32_t unit1 = readbits(opcode, 2, 2);
    const uint32_t unit2 = readbits(opcode, 4, 4);
    
    //Read Register
    processor->operations[id].size = readbits(opcode, 8, 2);
    processor->operations[id].id   = readbits(opcode, 10, 2);
    uint64_t regA = readbits(opcode, 26, 6);
    uint64_t regB = readbits(opcode, 20, 6);
    uint64_t regC = readbits(opcode, 14, 6);
    processor->operations[id].opA = regA;
    processor->operations[id].opB = processor->ireg[regB];
    processor->operations[id].opC = processor->ireg[regC];


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
                output->opB   = extendSign(imm,20);
                output->opA   = processor->ireg[regA];
            }
        }
        else if(id == 1) //DMA
        {
            return decodeDMA(processor,output,unit2,imm);

        }
        else if(id == 2) //ALU
        {
            return decodeALU(processor,output,unit1,unit2,imm);
        }
        else if(id ==3) //...
        {
            
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
        for(int i = 0;i < 4;i++)
        {
            processor->operations[id].fopB[i] = processor->vreg[(regB*4)+i];
            processor->operations[id].fopC[i] = processor->vreg[(regC*4)+i];
        }

        processor->operations[id].dopB = processor->dreg[(regB*2)];
        processor->operations[id].dopC = processor->dreg[(regC*2)];
        
        return decodeVFPU(processor,output,unit1,unit2,imm);
    }

    return 0;
}


ArResult arDecodeInstruction(ArProcessor processor)
{
    int error = 0;
    assert(processor);
    
    //Fetch
    memcpy(processor->opcodes, processor->isram + (processor->pc * 4), 4 * sizeof(uint32_t));

    error += decode(processor,0);
    error += decode(processor,1);
    
    uint32_t size = 2;
    const uint32_t swt =  readbits(processor->opcodes[3], 4, 0);
    if(swt == 0xC)
    {
        error += decode(processor,2);
        error += decode(processor,3);
        size = 4;
    }
    
    processor->swt = size;
    processor->pc += size;

    if(error != 0)
        return 0;

    return AR_SUCCESS;
}
