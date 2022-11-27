
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

#include "vm.h"
#define NOP 0
//static uint8_t AXS_signal_

static uint8_t AXS_signal_Fetch    = 0;
static uint8_t AXS_signal_Decode   = 0;
static uint8_t AXS_signal_Window   = 0;
static uint8_t AXS_signal_Register = 0;
static uint8_t AXS_signal_Execute  = 0;
static uint8_t AXS_signal_Memory   = 0;


static uint8_t AXS_signal_DecodeDelay = 0;
//----------------

typedef struct SWindow
{
	uint32_t micro_op;
	uint8_t ireg_busy[64];
	uint8_t vreg_busy[64];
	uint32_t wcounter;


	uint32_t opcode_in[4],opcode_out[8];

}SWindow;

SWindow swindow;
//----------------

//----------------

//----------------



//----------------


static void PreDecode(Core *core,uint32_t *opcode,uint32_t *opcode_out)
{
	uint32_t vliw0 = (opcode[0]&1) | ((opcode[1]&1)<<1) | ((opcode[2]&1)<<2);
	uint32_t vliw1 = vliw0&0b0101;
	uint32_t vliw2 = vliw0&0b0011;
	uint32_t vliw3 = (opcode[3]&1);
	uint8_t end = 0;

	if(vliw0 == 0b000)
	{
		opcode_out[0] = opcode[0];
		opcode_out[1] = 0;

		opcode_out[2] = opcode[1];
		opcode_out[3] = 0;

		opcode_out[4] = opcode[2];
		opcode_out[5] = 0;

		opcode_out[6] = opcode[3];
		opcode_out[7] = 0;

		end = 1;
	}else
	if(vliw0 == 0b100)
	{
		opcode_out[0] = opcode[0];
		opcode_out[1] = 0;

		opcode_out[2] = opcode[1];
		opcode_out[3] = 0;

		opcode_out[4] = opcode[2];
		opcode_out[5] = opcode[3];

		opcode_out[6] = 0;
		opcode_out[7] = 0;
	}else
	if(vliw2 == 0b010)
	{
		opcode_out[0] = opcode[0];
		opcode_out[1] = 0;

		opcode_out[2] = opcode[1];
		opcode_out[3] = opcode[2];

		opcode_out[4] = opcode[3];
		opcode_out[5] = 0;

		opcode_out[6] = 0;
		opcode_out[7] = 0;

		end = 2;
	}else
	if(vliw1 == 0b001)
	{
		opcode_out[0] = opcode[0];
		opcode_out[1] = opcode[1];

		opcode_out[2] = opcode[2];
		opcode_out[3] = 0;

		opcode_out[4] = opcode[3];
		opcode_out[5] = 0;

		opcode_out[6] = 0;
		opcode_out[7] = 0;

		end = 2;
	}else
	if(vliw1 == 0b101)
	{
		opcode_out[0] = opcode[0];
		opcode_out[1] = opcode[1];

		opcode_out[2] = opcode[2];
		opcode_out[3] = opcode[3];

		opcode_out[4] = 0;
		opcode_out[5] = 0;

		opcode_out[6] = 0;
		opcode_out[7] = 0;
	}


	if(vliw3)
	{
		if(end == 1)
		{
			opcode_out[6] = 0;
			opcode_out[7] = 0;
		}else
		{
			opcode_out[4] = 0;
			opcode_out[5] = 0;
		}

		core->pc -= 1;
	}




}


static void EliminateBranch(Core *core,uint32_t *opcode)
{
	uint32_t op1 = (opcode[0]&0xFE)>>1;
	uint32_t op2 = (opcode[2]&0xFE)>>1;
	uint32_t op3 = (opcode[4]&0xFE)>>1;
	uint32_t op4 = (opcode[6]&0xFE)>>1;


	if( (op1 >= 0x4C) && (op1 <= 0x5F) )
	{
		opcode[4] = 0;
		opcode[5] = 0;

		opcode[6] = 0;
		opcode[7] = 0;
		AXS_signal_DecodeDelay = 1;
	}else
	if( (op2 >= 0x4C) && (op2 <= 0x5F) )
	{
		opcode[6] = 0;
		opcode[7] = 0;
		AXS_signal_DecodeDelay = 1;
	}else
	if( (op3 >= 0x4C) && (op3 <= 0x5F) )
	{
		AXS_signal_DecodeDelay = 1;
	}else
	if( (op4 >= 0x4C) && (op4 <= 0x5F) )
	{
		opcode[6] = 0;
		opcode[7] = 0;

		core->pc -= 1;

		AXS_signal_DecodeDelay = 1;
	}
}

static void Decode(Core *core)
{
	if(AXS_signal_DecodeDelay == 1)
	{
		AXS_signal_DecodeDelay = 0;
		return;
	}

	if(AXS_signal_Decode == 0)
		return;

	PreDecode(core,swindow.opcode_in,swindow.opcode_out);
	EliminateBranch(core,swindow.opcode_out);


	AXS_signal_Window = 1;
}


//----------------

static void Fetch(Core *core,uint32_t *opcode)
{
	if(AXS_signal_Fetch != 0)
		return;

	opcode[0] = core->wram[core->pc+0];
	opcode[1] = core->wram[core->pc+1];
	opcode[2] = core->wram[core->pc+2];
	opcode[3] = core->wram[core->pc+3];

	core->pc += 4;

	AXS_signal_Decode = 1;
}

//----------------


void AXS_decode_execute(Core *core)
{
	uint32_t opcode,unit;
	uint64_t rega,regb,regc,size;

	core->cycle++;
	//core->instruction++;

	opcode = core->wram[core->pc+0];

	if(core->delay == 1)
	{
		executeDelayedInstruction(core);
	}

	core->opcode1 = opcode;
	core->pc++;

	unit = (opcode>>1)&0x7F;
	size = (opcode>>8)&0x3;

	rega = (opcode>>26)&0x3F;
	regb = (opcode>>20)&0x3F;
	regc = (opcode>>14)&0x3F;

	AX_execute_unit0(core,rega,regb,regc,unit,size,opcode);

	if(opcode&1)
	{
		core->instruction++;
		opcode = core->wram[core->pc+1];
		core->opcode2 = opcode;
		core->pc++;
		unit = (opcode>>1)&0x7F;
		size = (opcode>>8)&0x3;

		rega = (opcode>>26)&0x3F;
		regb = (opcode>>20)&0x3F;
		regc = (opcode>>14)&0x3F;

		AX_execute_unit1(core,rega,regb,regc,unit,size,opcode);
	}

/*
	Execute();
	Window();
	Decode();
	Fetch();*/

}

