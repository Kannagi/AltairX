#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
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

static double convertImmDouble(uint32_t imm)
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

    float dval;

    memcpy(&dval, &xval, sizeof(xval));

    return dval;
}

static int decode(Core *core,uint32_t id)
{
    const uint32_t opcode = core->opcodes[id];

    const uint32_t compute_unit = readbits(opcode, 1, 3);
    const uint32_t imm = readbits(opcode, 8, 18);

    const uint32_t unit = readbits(opcode, 4, 4);

    core->operations[id].size = readbits(opcode, 8, 2);
    core->operations[id].id   = readbits(opcode, 10, 2);
    const uint64_t regA = readbits(opcode, 26, 6);
    const uint64_t regB = readbits(opcode, 20, 6);
    const uint64_t regC = readbits(opcode, 14, 6);

    //core->operations[id].regA = regA;
    core->operations[id].regB = regB;
	core->operations[id].regC = regC;

    //Read Register
    core->operations[id].opA = regA;
    core->operations[id].opB = core->ireg[regB];
    core->operations[id].opC = core->ireg[regC];


    //printf("opcode %x\n",opcode);

    Execute *output = &core->operations[id];

    //printf("cu%d %d / %x\n",id,compute_unit,opcode);

    //Decode /issue
    switch(compute_unit)
    {
    	//ALU-A
		case 0:
			output->unit2 = 0x10+unit;

			if(unit == 0)
				output->unit1 = AX_EXE_NOP;
			else
			{
				output->unit1 = AX_EXE_ALU;

				if(unit == 6) //SMOVE
					core->operations[id].opB = ( (imm>>2)&0xFFFF );
				else if(unit == 7) //MOVEI
					core->operations[id].opB = extendSign( imm&0x3FFFF,17 );
				else if(unit == 8) //SLTS
				{
					core->operations[id].opC = extendSign( (imm>>2)&0x3FF,9);
					output->unit2 = AX_OPCODE_SLTS;
				}
				else if(unit == 9) //SLTU
				{
					core->operations[id].opC = ( (imm>>2)&0x3FF );
					output->unit2 = AX_OPCODE_SLTU;
				}
				else if(unit == 11) //MOVEIU
				{
					core->operations[id].opB = imm&0x3FFFF;
					output->unit2 = AX_OPCODE_MOVE;
				}
			}


		break;

		//ALU-B
		case 1:
			output->unit1 = AX_EXE_ALU;
			output->unit2 = unit&0x0F;

			if(opcode&0x400)
			{
				if(unit == 1)
					core->operations[id].opC = extendSign( (imm>>3)&0x1FF,8);
				else if(unit == 2)
					core->operations[id].opC = extendSign( (imm>>3)&0x1FF,8);
				else if(unit == 3)
					core->operations[id].opC = extendSign( (imm>>3)&0x1FF,8);
				else if(unit == 8)
					core->operations[id].opC = extendSign( (imm>>3)&0x1FF,8);
				else
					core->operations[id].opC = ( (imm>>3)&0x1FF );
			}

		break;

		//LSU-SPM
		case 2:
			output->unit1 = AX_EXE_LSUM;
			output->unit2 = unit&0x03;

			if(unit&0x8)
			{
				core->operations[id].opC = ( (imm>>2)&0xFFFF );

				if(unit&0x4)
					core->operations[id].opB = core->ireg[1];
				else
					core->operations[id].opB = core->ireg[0];
			}
			else if(unit&0x4)
				core->operations[id].opC = extendSign( (imm>>2)&0x3FF,9);
		break;

		//LSU-Cache
		case 3:
			output->unit1 = AX_EXE_LSUM;
			output->unit2 = unit&0x03;


			if(unit&0x8)
			{
				core->operations[id].opC = ( (imm>>2)&0xFFFF );

				if(unit&0x4)
					core->operations[id].opB = core->ireg[3];
				else
					core->operations[id].opB = core->ireg[2];
			}
			else if(unit&0x4)
				core->operations[id].opC = extendSign( (imm>>2)&0x3FF,9);


			if( (id == 1) && ( (unit&0xC) == 0x0C) )
			{
				core->operations[id].opC = core->ireg[regC];
				core->operations[id].opC = core->ireg[regC];
				if(unit == 0xC)
				{
					output->unit2 = AX_OPCODE_PREFETCH;
					core->operations[id].opB = ( (imm>>2)&0xFFFF);
				}
				else if(unit == 0xD)
				{
					output->unit2 = AX_OPCODE_FLUSH;
					core->operations[id].opB = ( (imm>>2)&0xFFFF);
				}
				else if(unit == 0xE)
				{
					output->unit2 = AX_OPCODE_PREFETCH;
				}
				else if(unit == 0xF)
				{
					output->unit2 = AX_OPCODE_FLUSH;
				}
			}


		break;

		//CMP/OTHER - DMA
		case 4:
			if(id == 0)
			{
				if(unit&0x8)
				{

					if(unit == 8)
					{
						output->unit1 = AX_EXE_OTHER;
						output->unit2 = AX_OPCODE_ENDP;
					}
					else if(unit == 9)
					{
						output->unit1 = AX_EXE_OTHER;
						output->unit2 = AX_OPCODE_SYSCALL;
					}
					else if(unit == 10)
					{

						output->unit1 = AX_EXE_OTHER;
						output->unit2 = AX_OPCODE_INT;
					}
					else if(unit == 12)
					{

						output->unit1 = AX_EXE_BRU;
						core->delayop = output->unit2 = AX_OPCODE_RET;
						core->delay = 0;
					}else if(unit == 13)
					{
						output->unit1 = AX_EXE_BRU;
						core->delayop = output->unit2 = AX_OPCODE_RETI;
						core->delay = 0;
					}
					else
					{
						return 1;
					}
				}
				else
				{
					output->unit1 = AX_EXE_CMP;
					output->unit2 = unit;

					if(unit == 2)
					{
						output->opB = extendSign( (imm>>2)&0xFFFF,15);
						output->unit2 = AX_OPCODE_CMP;
					}
					else if(unit == 3)
					{
						output->opB = ( (imm>>2)&0xFFFF );
						output->unit2 = AX_OPCODE_CMP;
					}
					else if(unit == 6)
					{
						output->id = (output->size+1)&0x3;
						output->fopB[output->id] = convertImmFloat(imm>>2);
						output->unit2 = AX_OPCODE_FCMP;
					}
					else if(unit == 7)
					{
						output->dopB = convertImmDouble(imm>>2);
						output->unit2 = AX_OPCODE_DCMP;
					}
					else
					{

					}
				}
			}
			else if(id == 1)
			{
				output->unit1 = AX_EXE_DMA;
				output->unit2 = unit&7;

				if(unit&0x8)
				{
					if(unit == 10)
						output->opC = ( (imm>>2)&0xFFFF );
					else if(unit == 11)
						output->opC = ( (imm>>2)&0xFFFF );
					else
						output->opC = ( (imm>>2)&0x3FF );
				}

			}

		break;


		//BRU
		case 5:
			output->unit1 = AX_EXE_BRU;
			core->delayop = output->unit2 = unit;
			core->delay = 0;

			core->imm =  ( (imm>>2)&0xFFFF );

		break;

		//VFPU-A - FPU-Double
		case 6:
			output->unit1 = AX_EXE_VFPU;

			if(id == 0) //VFPU-A
			{
				output->unit2 = unit;

				for(int i = 0;i < 4;i++)
				{
					output->fopB[i] = core->vreg[(regB*4)+i];
					output->fopC[i] = core->vreg[(regC*4)+i];
				}

				if(unit == 0x0F)
				{
					output->opC = (imm>>2)&0x3FF;
				}
			}
			else if(id == 1) //FPU-Double
			{
				output->unit2 = unit+0x30;

				output->dopB = core->dreg[(regB*2)];
				output->dopC = core->dreg[(regC*2)];

				if(unit == 0)
				{
					output->dopB = convertImmDouble(imm>>2);
				}
			}

		break;

		//VFPU-B - EFU
		case 7:
			output->unit1 = AX_EXE_VFPU;

			for(int i = 0;i < 4;i++)
			{
				output->fopB[i] = core->vreg[(regB*4)+i];
				output->fopC[i] = core->vreg[(regC*4)+i];
			}

			output->dopB = core->dreg[(regB*2)];


			if(id == 0) //VFPU-B
			{
				output->unit2 = unit+0x10;

				if(unit == 2)
				{
					output->id = (output->size+1)&0x3;
					output->fopB[output->id] = convertImmFloat(imm>>2);
					output->unit2 = AX_OPCODE_FMOVE+0x10;
				}
				else if(unit == 3)
				{
					float tmp = convertImmFloat(imm>>2);
					for(uint32_t i = 0;i < output->size;i++)
						output->fopB[i] = tmp;
					output->unit2 = AX_OPCODE_VFMOVE+0x10;
				}
			}
			else if(id == 1) //EFU
			{
				output->unit2 = unit+0x20;
			}

		break;


		default :
			return AX_ERROR_OPCODE;
		break;

    }

    return 0;
}

int AX_decode(Core *core)
{
    //Fetch
    memcpy(core->opcodes, core->isram + (core->pc * 4), 4 * sizeof(uint32_t));


    //printf("pc : %d\n",core->pc);

    int error = 0;
    int pairing,i;
    pairing  = (core->opcodes[0]&1);
    pairing += (core->opcodes[1]&1)<<1;
    pairing += (core->opcodes[2]&1)<<2;
    pairing += (core->opcodes[3]&1)<<3;


    //issue
    if( (pairing&1) == 0b0 )
    {
        core->swt = 1;
        core->pc += 1;
    }
    else if( (pairing&3) == 0b01 )
    {
        core->swt = 2;
        core->pc += 2;
    }
    else if( (pairing&7) == 0b011 )
    {
        core->swt = 3;
        core->pc += 3;
    }
    else if( (pairing&0xF) == 0b0111 )
    {
        core->swt = 4;
        core->pc += 4;
    }
    else
    {
        return -1;
    }

	//core->instruction += core->swt;

	for(i = 0;i < core->swt;i++)
    	error += decode(core,i);




    return -error;
}

