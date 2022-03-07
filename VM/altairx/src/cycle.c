
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

#include "vm.h"

void AX_Cache_miss(Core *core)
{
	//I-Cache
	uint32_t index = (core->pc>>10);
	index = (index&0x3F);

	if(core->icache[index] != core->pc)
	{
		core->icache[index] = core->pc;
		core->cycle += 14 + (5*16); //5cycle /64 B
		core->bandwidth += 1024;
		core->icachemiss++;
		core->icachemiss_cycle += 14 + (5*16);
	}


	//D-Cache
	for(int i = 0;i < core->swt+1;i++)
	{
		uint32_t unit1 = core->operations[i].unit1;
		uint32_t unit2 = core->operations[i].unit2;
		uint64_t opB   = core->operations[i].opB;
		uint64_t opC   = core->operations[i].opC;
		uint64_t offset;

		if(unit1 == AX_EXE_LSU)
		{
			switch(unit2)
			{

				case AX_OPCODE_LD:
				case AX_OPCODE_ST:
				case AX_OPCODE_LDV:
				case AX_OPCODE_STV:
					offset =  opB + opC;
					index = (offset>>7);
					index = (index&0x3FF);

					if(core->dcache[index] != offset)
					{
						core->dcache[index] = offset;
						core->cycle += (14 + 5 )*2; //5cycle /64 B

						core->bandwidth += 128;
						core->dcachemiss++;
						core->dcachemiss_cycle += (14 + 5)*2;
					}
				break;

				case AX_OPCODE_FLUSH:
				case AX_OPCODE_PREFETCH:
				case AX_OPCODE_IFLUSH:
				case AX_OPCODE_IPREFETCH:

				break;
			}
		}
		else
		if(unit1 == AX_EXE_DMA)
		{
			if(unit2 != AX_OPCODE_WAIT)
			{
				core->cycle += (14 + (5*opC) );
				core->bandwidth += 64*opC;
			}

		}
	}
}

#define AX_ALU_CYCLE 3
#define AX_FPU_CYCLE 4
#define AX_FPUD_CYCLE 4
/*
void AX_Pipeline_stall(Core *core)
{
	for(i = 0;i < AX_core_IREG_COUNT;i++)
	{
		if(core->busy_reg[i] > 0)
			core->busy_reg[i]--;
	}

	for(i = 0;i < AX_core_VREG_COUNT;i++)
	{
		if(core->busy_vreg[i] > 0)
			core->busy_vreg[i]--;
	}


	for(int i = 0;i < core->swt+1;i++)
	{



	}


    //Decode
    switch(compute_unit)
    {
    	//ALU-A
		case 0:

			switch(unit1)
			{
				case 1:
				case 2:
				case 6:
				case 7:
					regB = 1;
				break;

				case 4:
				case 5:
					regB = 1;
					regC = 1;
				break;


			}

			if(i == 0)
			{
				if( (opcode&0x400) && (unit1 > 9) )
				unit1
			}

			core->busy_reg[regA] = AX_ALU_CYCLE;


				if(unit == 3) //SMOVE
				{
					core->operations[id].opB = ( (imm>>2)&0xFFFF );
				}
				else if(unit == 6) //SLTS
				{
					core->operations[id].opC = extendSign( (imm>>2)&0x3FF,9);
					output->unit2 = AX_OPCODE_SLTS;
				}
				else if(unit ==7) //SLTU
				{
					core->operations[id].opC = ( (imm>>2)&0x3FF );
					output->unit2 = AX_OPCODE_SLTU;
				}
				else if(unit == 8) //MOVEI
				{
					core->operations[id].opB = extendSign( imm&0x3FFFF,17 );
					output->unit2 = AX_OPCODE_MOVE;
				}
				else if(unit == 9) //MOVEIU
				{
					core->operations[id].opB = imm&0x3FFFF;
					output->unit2 = AX_OPCODE_MOVE;
				}
				else if(unit > 9)
				{
					if(id == 0) //MUL/DIV/REM
					{
						output->unit2 = unit;
						if(opcode&0x400)
						{
							if(unit == 0)
								core->operations[id].opC = extendSign( (imm>>3)&0x1FF,8);
							else
								core->operations[id].opC = ( (imm>>3)&0x1FF );
						}

					}
					else
					{

					}

				}



		break;

		//ALU-B
		case 1:
			output->unit1 = AX_EXE_ALU;
			output->unit2 = unit&0x07;

			if(unit&0x08)
			{
				if(unit == 2) //XOR
					core->operations[id].opC = extendSign( (imm>>2)&0x3FF,9);
				else
					core->operations[id].opC = ( (imm>>2)&0x3FF );
			}

		break;

		//LSU-A
		case 2:
			output->unit1 = AX_EXE_LSU;
			output->unit2 = unit&0x03;

			core->operations[id].opC = ( (imm>>2)&0xFFFF );

			tmp = unit>>2;
			core->operations[id].opB = core->ireg[tmp];
		break;

		//LSU-B/DMA
		case 3:
			output->unit1 = AX_EXE_LSU;
			output->unit2 = unit&0x03;

			tmp = unit>>2;

			if(tmp == 0)
			{
				//----
			}
			else if(tmp == 1)
			{
				core->operations[id].opC = extendSign( (imm>>2)&0x3FF,9);
			}
			else
			{
				if(id == 0) //DMA
				{
					output->unit1 = AX_EXE_DMA;
					output->unit2 = unit&1;

					if(unit&0x2)
					{
						output->opC = ( (imm>>2)&0xFFFF );
					}
				}
				else //PREFETCH
				{
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

			}


		break;

		//CMP/OTHER
		case 4:

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
					output->unit2 = AX_OPCODE_EXE;
				}
				else if(unit == 10)
				{
					output->unit1 = AX_EXE_DMA;
					output->unit2 = AX_OPCODE_WAIT;
				}
				else if(unit == 12)
				{
					output->unit1 = AX_EXE_BRU;
					core->delayop = output->unit2 = AX_OPCODE_RET;
					core->delay = 0;
				}
				else if(unit == 13)
				{
					output->unit1 = AX_EXE_BRU;
					core->delayop = output->unit2 = AX_OPCODE_RETI;
					core->delay = 0;
				}
				else if(unit == 14)
				{
					output->unit1 = AX_EXE_BRU;
					core->delayop = output->unit2 = AX_OPCODE_SYSCALL;
					core->delay = 0;
				}
				else if(unit == 15)
				{
					output->unit1 = AX_EXE_BRU;
					core->delayop = output->unit2 = AX_OPCODE_INT;
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
					output->opB = extendSign(imm&0x3FFFF,17);
					output->unit2 = AX_OPCODE_CMP;
				}
				else if(unit == 3)
				{
					output->opB = (imm&0x3FFFF);
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


		break;


		//BRU
		case 5:
			output->unit1 = AX_EXE_BRU;
			core->delayop = output->unit2 = unit;
			core->delay = 0;
			tmp = readbits(opcode, 8, 24);

			if((unit&0xC) == 0xC)
			{
				core->imm = tmp;
			}
			else
			{
				core->imm = extendSign( (tmp>>2),21);
			}

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
}*/
