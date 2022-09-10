
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

#include "vm.h"
#include "cycle.h"

//DDR3 1600 MHz
//14 cycles latency RAM
//5 cycles 64B transfert
//1 GHz CPU

void AX_Cache_miss(Core *core)
{
	//I-Cache
	uint32_t pc = (core->pc>>8);
	uint32_t index = (pc&0x3F);

	if(core->icache[index] != pc)
	{
		core->icache[index] = pc;
		//core->cycle += 14 + (5*16); //5cycle /64 B
		core->bandwidth += 1024;
		core->icachemiss++;
		core->icachemiss_cycle += 14 + (5*16);
	}

	//D-Cache
	uint32_t opcode = core->opcode1,unit;
	uint64_t regA,regB,regC;

	uint64_t imm10,imm16;

	unit = (opcode>>1)&0x7F;

	regA = (opcode>>26)&0x3F;
	regB = (opcode>>20)&0x3F;
	regC = (opcode>>14)&0x3F;

	imm10 = (opcode>>10)&0x3FF;
	imm16 = (opcode>>10)&0xFFFF;

	uint64_t offset;

	switch(unit)
	{
		case AX_EXE_LSU_LDL:
		case AX_EXE_LSU_STL:
		case AX_EXE_LSU_LDVL:
		case AX_EXE_LSU_STVL:
		case AX_EXE_LSU_LDL1:
		case AX_EXE_LSU_STL1:
		case AX_EXE_LSU_LDVL1:
		case AX_EXE_LSU_STVL1:

			offset =  core->ireg[regB] + imm16;

			offset = (offset>>7);
			index = (offset&0x3FF);

			if(core->dcache[index] != offset)
			{
				core->dcache[index] = offset;
				//core->cycle += (14 + 5 )*2; //5cycle /64 B

				core->bandwidth += 128;
				core->dcachemiss++;
				core->dcachemiss_cycle += (14 + 5)*2;
			}
		break;

		case AX_EXE_LSU_LDI:
		case AX_EXE_LSU_STI:
		case AX_EXE_LSU_LDVI:
		case AX_EXE_LSU_STVI:
			offset =  core->ireg[regB] + imm10;

			offset = (offset>>7);
			index = (offset&0x3FF);

			if(core->dcache[index] != offset)
			{
				core->dcache[index] = offset;
				//core->cycle += (14 + 5 )*2; //5cycle /64 B

				core->bandwidth += 128;
				core->dcachemiss++;
				core->dcachemiss_cycle += (14 + 5)*2;
			}
		break;

		case AX_EXE_LSU_LD:
		case AX_EXE_LSU_ST:
		case AX_EXE_LSU_LDV:
		case AX_EXE_LSU_STV:
			offset =  core->ireg[regB] + core->ireg[regC];

			offset = (offset>>7);
			index = (offset&0x3FF);

			if(core->dcache[index] != offset)
			{
				core->dcache[index] = offset;
				//core->cycle += (14 + 5 )*2; //5cycle /64 B

				core->bandwidth += 128;
				core->dcachemiss++;
				core->dcachemiss_cycle += (14 + 5)*2;
			}
		break;

		case AX_EXE_OTHER_WAIT:
			core->bandwidth += 64*core->ireg[regC];
		break;
/*
		case AX_OPCODE_FLUSH:
		case AX_OPCODE_PREFETCH:
		case AX_OPCODE_IFLUSH:
		case AX_OPCODE_IPREFETCH:

		break;*/
	}


}


void AX_Pipeline_stall(Core *core)
{
	int i,swt = 1;


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

	if(core->busy_flag > 0)
			core->busy_flag--;

	uint32_t opcodes[2];
	opcodes[0] = core->opcode1;
	opcodes[1] = core->opcode2;

	uint32_t clear = 0,tmp,pq,tmp2;
	for(i = 0;i < swt;i++)
	{
		const uint32_t opcode = opcodes[i];

		uint8_t regA = (opcode>>26)&0x3F;
		const uint8_t regB = (opcode>>20)&0x3F;
		const uint8_t regC = (opcode>>14)&0x3F;

		uint32_t unit = (opcode>>1)&0x7F;

		uint8_t opid;

		if(i == 0)
			opid = opcode1_rw[unit];
		else
			opid = opcode2_rw[unit];

		if( (unit >= AX_STALL_BRU) && (unit <= AX_STALL_BRU_END) && (i == 0) )
			clear = core->busy_flag;

		pq = 0;

		if(opid&AX_OPID_RX)
		{
			regA = (opcode>>3)&1;
		}

		if(opid&AX_OPID_RA_R)
		{

			if(opid & (AX_OPID_VA) )
			{
				tmp = core->busy_vreg[regA];
				if( (tmp > clear) && (regA != AX_ACCU) )
					clear = tmp;
			}
		}

		if(opid & AX_OPID_VR)
		{
			if(opid&AX_OPID_RA_R)
			{
				tmp = core->busy_vreg[regA];
				if( (tmp > clear) && (regA != AX_ACCU) )
					clear = tmp;
			}

			if(opid&AX_OPID_RB)
			{
				tmp = core->busy_vreg[regB];
				if( (tmp > clear) && (regB != AX_ACCU) )
					clear = tmp;
			}

			if(opid&AX_OPID_RC)
			{
				tmp = core->busy_vreg[regC];
				if( (tmp > clear) && (regC != AX_ACCU) )
					clear = tmp;
			}

			if((i == 1) && (regA > AX_ACCU) )
			{
				clear = AX_VPQ_CYCLE;
			}

		}
		else
		{
			if(opid&AX_OPID_RA_R)
			{
				tmp = core->busy_reg[regA];
				if( (tmp > clear) && (regA != AX_ACCU) )
					clear = tmp;
			}

			if(opid&AX_OPID_RB)
			{
				tmp = core->busy_reg[regB];
				if( (tmp > clear) && (regB != AX_ACCU) )
					clear = tmp;
			}

			if(opid&AX_OPID_RC)
			{
				tmp = core->busy_reg[regC];
				if( (tmp > clear) && (regC != AX_ACCU) )
					clear = tmp;
			}

			if(opid&AX_OPID_IM)
			{
				if(!(opcode&0x400) )
				{
					tmp = core->busy_reg[regC];
					if( (tmp > clear) && (regC != AX_ACCU) )
						clear = tmp;
				}

				if(regA > AX_ACCU)
					pq = AX_PQ_CYCLE;

			}
		}




		if(opid&AX_OPID_RA_W)
		{
			if(opid & (AX_OPID_VA+AX_OPID_VR) )
				core->busy_vreg[regA] = AX_FPU_CYCLE+pq;
			else
				core->busy_reg[regA] = AX_ALU_CYCLE+pq;
		}

		if( (unit >= AX_STALL_CMP) && (unit <= AX_STALL_CMP_END) && (i == 0) )
			core->busy_flag = 3;

	}

	printf("Cycle %d\n",clear+1);
	core->cycle += clear;
}

