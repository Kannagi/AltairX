#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

#include "vm.h"
#include "decode.h"

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

//--------------------------------------------------------
void decode_x64(const uint64_t opcode);

static void decode(Core *core,Execute *out,const uint16_t *opcode_dec,uint32_t opcode)
{
    const uint64_t unit = readbits(opcode, 1, 7);

    out->size = readbits(opcode, 8, 2);
    out->id   = readbits(opcode, 10, 2);
    const uint64_t regA = readbits(opcode, 26, 6);
    const uint64_t regB = readbits(opcode, 20, 6);
    const uint64_t regC = readbits(opcode, 14, 6);

    //core->operations[id].regA = regA;
    out->regB = regB;
	out->regC = regC;

    //Read Register
    out->opA = regA;
    out->opB = core->ireg[regB];
    out->opC = core->ireg[regC];

    //Decode /issue
    uint16_t tmp,immop;

	tmp = opcode_dec[unit];

	out->unit2 = tmp&0xFF;
	out->unit1 = tmp&0x7000;


	uint32_t imm = 0;

    if(tmp&AX_DEC_RX)
	{
		out->opC = core->ireg[0];
		return;
	}

	immop = tmp&0xF00;
	switch(immop)
	{
		case AX_IMM_S9BITS:
			imm = readbits(opcode, 11, 9);

			if(opcode&0400)
				out->opC = extendSign(imm,8);
		break;

		case AX_IMM_U9BITS:
			imm = readbits(opcode, 11, 9);

			if(opcode&0400)
				out->opC = (imm);
		break;

		case AX_IMM_S10BITS:
			imm = readbits(opcode, 10, 10);
			out->opC = extendSign(imm,10);
		break;

		case AX_IMM_S16BITS:
			imm = readbits(opcode, 10, 16);
			out->opB = extendSign(imm,15);
		break;

		case AX_IMM_S18BITS:
			imm = readbits(opcode, 8, 18);
			out->opB = extendSign(imm,17);
		break;

		case AX_IMM_OS18BITS:
			imm = readbits(opcode, 8, 18);
			out->opB = 0xFFFFFFFFFFFC0000 |imm;
		break;


		case AX_IMM_U10BITS:
			out->opC = readbits(opcode, 10, 10);
		break;

		case AX_IMM_U16BITS:
			out->opB = readbits(opcode, 10, 16);
		break;

		case AX_IMM_U18BITS:
			out->opB = readbits(opcode, 8, 18);
		break;

		case AX_IMM_S22BITS:
			imm = readbits(opcode, 10, 22);
			imm = extendSign(imm,21);
		break;

		case AX_IMM_U24BITS:
			imm = readbits(opcode, 8, 24);
		break;

		case AX_IMM_HTOF:
			imm = readbits(opcode, 10, 16);
			out->id = (out->size+1)&0x3;
			out->fopB[out->id] = convertImmFloat(imm);
		break;

		case AX_IMM_HTOD:
			imm = readbits(opcode, 10, 16);
			out->dopB = convertImmDouble(imm);
		break;

	}

	if(out->unit1 == AX_EXE_BRU)
	{
		core->delayop = out->unit2;
		core->delay = 0;

		core->imm = imm;
	}
}

void executeDelayedInstruction(Core *core);
void executeInstruction(Core *core,Execute *operations);
void decode_x64(const uint64_t opcode);

void AX_decode_execute(Core *core)
{

    //Fetch
    uint32_t n = core->pc;
    core->opcodes[0] = core->wram[n+0];
    core->opcodes[1] = core->wram[n+1];

	uint32_t pairing  = (core->opcodes[0]&1);

	uint8_t swt;

    //issue
    if(pairing == 0)
    {
        swt = 0;
        //core->pc += 1;
        decode(core,&core->operations[0],opcode1,core->opcodes[0]);
    }else
    {
		swt = 1;
        core->pc += 2;
        decode(core,&core->operations[0],opcode1,core->opcodes[0]);
        decode(core,&core->operations[1],opcode2,core->opcodes[1]);
    }

    core->instruction += swt+1;
    core->swt = swt;

    //Execute

    if(core->delay == 1)
    {
        executeDelayedInstruction(core);
    }

	executeInstruction(core,&core->operations[0]);

	if(swt == 0)
		return;

	executeInstruction(core,&core->operations[1]);

}

