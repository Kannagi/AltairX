#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

#include "vm.h"
#include "decode.h"

void decode_x64(const uint64_t opcode);
static uint64_t reg[16];
/*
	asm (
		".section .rodata\n	"
		".align 16\n	"
		"if_AX_EXE_IMM:\n	"
		".long if_AX_DEC_EXE\n	"

		);
*/

void AX_decode_executex64(Core *core)
{
	uint32_t out[8];
	//0 : delay
	//1 : swt
	//2 : ninstruction
	//3 : unit1
	//4 : unit2
	//5 :


	asm (
	".intel_syntax noprefix\n"
	"push r8\n	"
	"push r9\n	"
	"push r10\n	"
	"push r11\n	"
	"push r12\n	"
	"push r13\n	"
	"push r14\n	"
	"push r15\n	"

/*
	"mov r9,%4\n	"
	"mov r8,5\n	"
	"mov [r9+0],r8d\n	"
*/

	//opcode (wram+pc)
	"mov r9d,%1\n	"
	"shl r9,2\n	"
	"mov r8,[r9+%0]\n	"

	//--------

	"AX_EXE:\n	"

	//unit
	"mov r9,r8\n	"
	"and r9,0xFE\n	"

	//instruction
	"mov r10,[r9+%2]\n	"

	//rA
	"mov r13,r8\n	"
	"shr r13,26\n	"
	"and r13,0x3F\n	"

	//rB
	"mov r14,r8\n	"
	"shr r14,20\n	"
	"and r14,0x3F\n	"

	//rC
	"mov r15,r8\n	"
	"shr r15,14\n	"
	"and r15,0x3F\n	"

	//unit 1 & 2
	"mov r11,r10\n	"
	"and r10,0xFF\n	"
	"and r11,0x7000\n	"

	"mov r9,%4\n	"
	"mov [r9+12],r10\n	"
	"mov [r9+16],r11\n	"


	//if(tmp&AX_DEC_RX)
	"mov r11,r10\n	"
	"and r11,0x8000\n	"
	"cmp r11,0x8000\n	"
	"jne  if_AX_DEC_EXE\n	"

	"mov r15,0\n	"

	"if_AX_DEC_EXE:\n	"





	//

	/*
	//rA
	"mov r13,r8\n	"
	"shr r13,26\n	"
	"and r13,0x3F\n	"

	//rB
	"mov r14,r8\n	"
	"shr r14,20\n	"
	"and r14,0x3F\n	"

	//rC
	"mov r15,r8\n	"
	"shr r15,14\n	"
	"and r15,0x3F\n	"*/







	"pop r15\n	"
	"pop r14\n	"
	"pop r13\n	"
	"pop r12\n	"
	"pop r11\n	"
	"pop r10\n	"
	"pop r9\n	"
	"pop r8\n	"


	".att_syntax prefix\n"
	: : "r"(core->wram) , "r"(core->pc) , "r"(opcode1) , "r"(opcode2) , "r"(out): "memory");



	asm (
	".intel_syntax noprefix\n"







	".att_syntax prefix\n"
	: :  "r"(reg): "memory");

	return;
/*
	uint64_t var;
	asm (
	".intel_syntax noprefix\n"
	"mov %0,rax\n	"

	".att_syntax prefix\n"
	: : "rw"(var) : "memory");*/


	//printf("%d / %x\n",out[0],out[1]);

}

void decode_x64(const uint64_t opcode)
{

	asm (
		".intel_syntax noprefix\n"
		"push r8\n	"
		"push r9\n	"
		"push r10\n	"
		"push r11\n	"
		"push r12\n	"
		"push r13\n	"
		"push r14\n	"
		"push r15\n	"

		//unit
		"mov r8,%0\n	"
		"shr r8,1\n	"
		"and r8,0x7F\n	"


		//rA
		"mov r13,%0\n	"
		"shr r13,26\n	"
		"and r13,0x3F\n	"

		//rB
		"mov r13,%0\n	"
		"shr r13,20\n	"
		"and r13,0x3F\n	"

		//rC
		"mov r13,%0\n	"
		"shr r13,14\n	"
		"and r13,0x3F\n	"

		"pop r15\n	"
		"pop r14\n	"
		"pop r13\n	"
		"pop r12\n	"
		"pop r11\n	"
		"pop r10\n	"
		"pop r9\n	"
		"pop r8\n	"


		".att_syntax prefix\n"
		: : "r"(opcode) : "memory");
}



