
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

#include "vm.h"

static float HalftoFloat(uint32_t half)
{
	uint32_t xval;
    uint32_t exp = (half&0x3C00)>>3;

    if(half&0x4000)
    {
        exp |= 0x4000;
    }
    else
    {
        if(exp != 0)
            exp |= 0x3800;
    }

    exp = exp<<16;

    xval  = (half&0x8000)<<16; //sign
    xval += (half&0x03FF)<<13; //mantisse
    xval += exp;

    float fval;

    memcpy(&fval, &xval, sizeof(xval));

    return fval;
}

void AX_debug_core1(Core *core)
{
	uint32_t opcode = core->opcode1,unit;
	uint64_t regA,regB,regC;

	uint64_t imm9,imm10,imm16,imm18;
	float immf;

	unit = (opcode>>1)&0x7F;

	regA = (opcode>>26)&0x3F;
	regB = (opcode>>20)&0x3F;
	regC = (opcode>>14)&0x3F;

	imm9  = (opcode>>11)&0x1FF;
	imm10 = (opcode>>10)&0x3FF;
	imm16 = (opcode>>10)&0xFFFF;
	imm18 = (opcode>>8)&0x3FFFF;

	immf = HalftoFloat(imm16);

	switch(unit)
	{
		//------------- ALU-A -----
		case AX_EXE_ALU_NOP:
			printf("NOP");
		break;

		case AX_EXE_ALU_SEXT:
			printf("SEXT R%d,R%d",regA,regB);
		break;

		case AX_EXE_ALU_BOOL:
			printf("BOOL R%d,R%d",regA,regB);
		break;

		case AX_EXE_ALU_SMOVE:
			printf("SMOVE R%d,%d",regA,imm16);
		break;

		case AX_EXE_ALU_SLTS:
			printf("SLTS R%d,R%d,R%d",regA,regB,regC);
		break;

		case AX_EXE_ALU_SLTU:
			printf("SLTU R%d,R%d,R%d",regA,regB,regC);
		break;

		case AX_EXE_ALU_SLTSI:
			printf("SLTSI R%d,R%d,%d",regA,regB,imm10);
		break;

		case AX_EXE_ALU_SLTUI:
			printf("SLTUI R%d,R%d,%d",regA,regB,imm10);
		break;

		case AX_EXE_ALU_MOVEN:
			printf("MOVEN R%d,%d",regA,imm18);
		break;

		case AX_EXE_ALU_MOVEU:
			printf("MOVEU R%d,%d",regA,imm18);
		break;

		//------------- ALU-B -----

		case AX_EXE_ALU_ADD:
			printf("ADD R%d,R%d,R%d (%d)",regA,regB,regC,imm9);
		break;

		case AX_EXE_ALU_SUB:
			printf("SUB R%d,R%d,R%d (%d)",regA,regB,regC,imm9);
		break;

		case AX_EXE_ALU_XOR:
			printf("XOR R%d,R%d,R%d (%d)",regA,regB,regC,imm9);
		break;

		case AX_EXE_ALU_OR:
			printf("OR R%d,R%d,R%d (%d)",regA,regB,regC,imm9);
		break;

		case AX_EXE_ALU_AND:
			printf("AND R%d,R%d,R%d (%d)",regA,regB,regC,imm9);
		break;

		case AX_EXE_ALU_LSL:
			printf("LSL R%d,R%d,R%d (%d)",regA,regB,regC,imm9);
		break;

		case AX_EXE_ALU_ASR:
			printf("ASR R%d,R%d,R%d (%d)",regA,regB,regC,imm9);
		break;

		case AX_EXE_ALU_LSR:
			printf("LSR R%d,R%d,R%d (%d)",regA,regB,regC,imm9);
		break;


		//------------- LSU-A -----
		case AX_EXE_LSU_LDL:
			printf("LDL R%d,%d",regA,imm16);
		break;

		case AX_EXE_LSU_STL:
			printf("STL R%d,%d",regA,imm16);
		break;

		case AX_EXE_LSU_LDVL:
			printf("LDVL R%d,%d",regA,imm16);
		break;

		case AX_EXE_LSU_STVL:
			printf("STVL R%d,%d",regA,imm16);
		break;

		case AX_EXE_LSU_LDL1:
			printf("LDL1 R%d,%d",regA,imm16);
		break;

		case AX_EXE_LSU_STL1:
			printf("STL1 R%d,%d",regA,imm16);
		break;

		case AX_EXE_LSU_LDVL1:
			printf("LDVL1 R%d,%d",regA,imm16);
		break;

		case AX_EXE_LSU_STVL1:
			printf("STVL1 R%d,%d",regA,imm16);
		break;


		//------------- LSU-B -----
		case AX_EXE_LSU_LD:
			printf("LD R%d,R%d[R%d]",regA,regB,regC);
		break;

		case AX_EXE_LSU_ST:
			printf("ST R%d,R%d[R%d]",regA,regB,regC);
		break;

		case AX_EXE_LSU_LDV:
			printf("LDV R%d,R%d[R%d]",regA,regB,regC);
		break;

		case AX_EXE_LSU_STV:
			printf("STV R%d,R%d[R%d]",regA,regB,regC);
		break;

		case AX_EXE_LSU_LDI:
			printf("LDI R%d,%d[R%d]",regA,imm10,regB);
		break;

		case AX_EXE_LSU_STI:
			printf("STI R%d,%d[R%d]",regA,imm10,regB);
		break;

		case AX_EXE_LSU_LDVI:
			printf("LDVI R%d,%d[R%d]",regA,imm10,regB);
		break;

		case AX_EXE_LSU_STVI:
			printf("STVI R%d,%d[R%d]",regA,imm10,regB);
		break;

		//------------- EFU -----

		case AX_EXE_EFU_FDIV:
			printf("FDIV V%d,V%d,V%d",regA,regB,regC);
		break;

		case AX_EXE_EFU_DDIV:
			printf("DDIV V%d,V%d,V%d",regA,regB,regC);
		break;

		case AX_EXE_EFU_FSQRT:
			printf("FSQRT V%d,V%d",regA,regB);
		break;

		case AX_EXE_EFU_DSQRT:
			printf("DSQRT V%d,V%d",regA,regB);
		break;

		case AX_EXE_EFU_FSIN:
			printf("FSIN V%d,V%d",regA,regB);
		break;

		case AX_EXE_EFU_DSIN:
			printf("DSIN V%d,V%d",regA,regB);
		break;

		case AX_EXE_EFU_FSUM:
			printf("FSUM V%d,V%d",regA,regB);
		break;

		case AX_EXE_EFU_FEXP:
			printf("FEXP V%d,V%d",regA,regB);
		break;


		case AX_EXE_EFU_FATAN:
			printf("FATAN V%d,V%d,V%d",regA,regB,regC);
		break;

		case AX_EXE_EFU_FATAN2:
			printf("FATAN2 V%d,V%d,V%d",regA,regB,regC);
		break;

		case AX_EXE_EFU_FIPR:
			printf("FATAN V%d,V%d,V%d",regA,regB,regC);
		break;


		//------------- FPU-D -----

		case AX_EXE_FPU_DMOVEI:
			printf("DMOVEI V%d,V%d",regA,regB);
		break;

		case AX_EXE_FPU_DADD:
			printf("DADD V%d,V%d,V%d",regA,regB,regC);
		break;

		case AX_EXE_FPU_DSUB:
			printf("DSUB V%d,V%d,V%d",regA,regB,regC);
		break;

		case AX_EXE_FPU_DMUL:
			printf("DMUL V%d,V%d,V%d",regA,regB,regC);
		break;

		case AX_EXE_FPU_DABS:
			printf("DABS V%d,V%d",regA,regB);
		break;

		case AX_EXE_FPU_DNEG:
			printf("DNEG V%d,V%d",regA,regB);
		break;


		case AX_EXE_FPU_DMIN:
			printf("DMIN V%d,V%d,V%d",regA,regB,regC);
		break;

		case AX_EXE_FPU_DMAX:
			printf("DMAX V%d,V%d,V%d",regA,regB,regC);
		break;

		case AX_EXE_FPU_DTOF:
			printf("VDTOF V%d,V%d,V%d",regA,regB,regC);
		break;

		case AX_EXE_FPU_FTOD:
			printf("VFTOD V%d,V%d,V%d",regA,regB,regC);
		break;


	}

}

void AX_debug_core0(Core *core)
{
	uint32_t opcode = core->opcode1,unit;
	uint64_t regA,regB,regC;

	uint64_t imm9,imm10,imm16,imm18;
	float immf;

	unit = (opcode>>1)&0x7F;

	regA = (opcode>>26)&0x3F;
	regB = (opcode>>20)&0x3F;
	regC = (opcode>>14)&0x3F;

	imm9  = (opcode>>11)&0x1FF;
	imm10 = (opcode>>10)&0x3FF;
	imm16 = (opcode>>10)&0xFFFF;
	imm18 = (opcode>>8)&0x3FFFF;

	immf = HalftoFloat(imm16);

	switch(unit)
	{
		//------------- ALU-A -----
		case AX_EXE_ALU_NOP:
			printf("NOP");
		break;

		case AX_EXE_ALU_SEXT:
			printf("SEXT R%d,R%d",regA,regB);
		break;

		case AX_EXE_ALU_BOOL:
			printf("BOOL R%d,R%d",regA,regB);
		break;

		case AX_EXE_ALU_SMOVE:
			printf("SMOVE R%d,%d",regA,imm16);
		break;

		case AX_EXE_ALU_SLTS:
			printf("SLTS R%d,R%d,R%d",regA,regB,regC);
		break;

		case AX_EXE_ALU_SLTU:
			printf("SLTU R%d,R%d,R%d",regA,regB,regC);
		break;

		case AX_EXE_ALU_SLTSI:
			printf("SLTSI R%d,R%d,%d",regA,regB,imm10);
		break;

		case AX_EXE_ALU_SLTUI:
			printf("SLTUI R%d,R%d,%d",regA,regB,imm10);
		break;

		case AX_EXE_ALU_MOVEN:
			printf("MOVEN R%d,%d",regA,imm18);
		break;

		case AX_EXE_ALU_MOVEU:
			printf("MOVEU R%d,%d",regA,imm18);
		break;

		//------------- ALU-B -----

		case AX_EXE_ALU_ADD:
			printf("ADD R%d,R%d,R%d (%d)",regA,regB,regC,imm9);
		break;

		case AX_EXE_ALU_SUB:
			printf("SUB R%d,R%d,R%d (%d)",regA,regB,regC,imm9);
		break;

		case AX_EXE_ALU_XOR:
			printf("XOR R%d,R%d,R%d (%d)",regA,regB,regC,imm9);
		break;

		case AX_EXE_ALU_OR:
			printf("OR R%d,R%d,R%d (%d)",regA,regB,regC,imm9);
		break;

		case AX_EXE_ALU_AND:
			printf("AND R%d,R%d,R%d (%d)",regA,regB,regC,imm9);
		break;

		case AX_EXE_ALU_LSL:
			printf("LSL R%d,R%d,R%d (%d)",regA,regB,regC,imm9);
		break;

		case AX_EXE_ALU_ASR:
			printf("ASR R%d,R%d,R%d (%d)",regA,regB,regC,imm9);
		break;

		case AX_EXE_ALU_LSR:
			printf("LSR R%d,R%d,R%d (%d)",regA,regB,regC,imm9);
		break;

		case AX_EXE_ALU_MUL:
			printf("MUL R%d,R%d,R%d (%d)",regA,regB,regC,imm9);
		break;

		case AX_EXE_ALU_DIVS:
			printf("DIVS R%d,R%d,R%d (%d)",regA,regB,regC,imm9);
		break;

		case AX_EXE_ALU_DIVU:
			printf("DIVU R%d,R%d,R%d (%d)",regA,regB,regC,imm9);
		break;

		case AX_EXE_ALU_REMS:
			printf("REMS R%d,R%d,R%d (%d)",regA,regB,regC,imm9);
		break;

		case AX_EXE_ALU_REMU:
			printf("REMU R%d,R%d,R%d (%d)",regA,regB,regC,imm9);
		break;

		//------------- CMP -----
		case AX_EXE_CMP_CMP:
			printf("CMP R%d,R%d",regA,regB);
		break;

		case AX_EXE_CMP_CMPN:
			printf("CMPN R%d,%d",regA,imm18);
		break;

		case AX_EXE_CMP_CMPU:
			printf("CMPU R%d,%d",regA,imm18);
		break;

		case AX_EXE_CMP_CMPFR:
			printf("CMPFR R%d,%d",regA,imm16);
		break;

		//------------- BRU -----




		case AX_EXE_BRU_BEQ:
			printf("BEQ %d",core->imm );
		break;
		case AX_EXE_BRU_BNE:
			printf("BNE %d",core->imm );
		break;
		case AX_EXE_BRU_BLE:
			printf("BLE %d",core->imm );
		break;
		case AX_EXE_BRU_BGE:
			printf("BGE %d",core->imm );
		break;
		case AX_EXE_BRU_BL:
			printf("BL %d",core->imm );
		break;
		case AX_EXE_BRU_BG:
			printf("BG %d",core->imm );
		break;
		case AX_EXE_BRU_BLES:
			printf("BLES %d",core->imm );
		break;
		case AX_EXE_BRU_BGES:
			printf("BGES %d",core->imm );
		break;
		case AX_EXE_BRU_BLS:
			printf("BLS %d",core->imm );
		break;
		case AX_EXE_BRU_BGS:
			printf("BGS %d",core->imm );
		break;
		case AX_EXE_BRU_BRA:
			printf("BRA %d",core->imm );
		break;
		case AX_EXE_BRU_LOOP:
			printf("LOOP %d",core->imm );
		break;

		case AX_EXE_BRU_JUMPBR:
			printf("JUMPBR %d",core->imm );
		break;
		case AX_EXE_BRU_CALLBR:
			printf("CALLBR %d",core->imm );
		break;

		case AX_EXE_BRU_JUMP:
			printf("JUMP %d",core->imm );
		break;
		case AX_EXE_BRU_CALL:
			printf("CALL %d",core->imm );
		break;


		case AX_EXE_BRU_SYSCALL:
			printf("SYSCALL");
		break;

		case AX_EXE_BRU_INT:
			printf("INT");
		break;

		case AX_EXE_BRU_RET:
			printf("RET");
		break;

		case AX_EXE_BRU_RETI:
			printf("RETI");
		break;

		//-----

		case AX_EXE_ALU_MOVEINS:
			printf("MOVEINS R%d",regA);
		break;

		case AX_EXE_ALU_MOVECYCLE:
			printf("MOVECYCLE R%d",regA);
		break;

		case AX_EXE_ALU_MOVERI:
			printf("MOVERI R%d,R%d",regA,regB);
		break;

		case AX_EXE_ALU_MOVEIR:
			printf("MOVEIR R%d,R%d",regA,regB);
		break;


		//------------- LSU-A -----
		case AX_EXE_LSU_LDL:
			printf("LDL R%d,%d",regA,imm16);
		break;

		case AX_EXE_LSU_STL:
			printf("STL R%d,%d",regA,imm16);
		break;

		case AX_EXE_LSU_LDVL:
			printf("LDVL R%d,%d",regA,imm16);
		break;

		case AX_EXE_LSU_STVL:
			printf("STVL R%d,%d",regA,imm16);
		break;

		case AX_EXE_LSU_LDL1:
			printf("LDL1 R%d,%d",regA,imm16);
		break;

		case AX_EXE_LSU_STL1:
			printf("STL1 R%d,%d",regA,imm16);
		break;

		case AX_EXE_LSU_LDVL1:
			printf("LDVL1 R%d,%d",regA,imm16);
		break;

		case AX_EXE_LSU_STVL1:
			printf("STVL1 R%d,%d",regA,imm16);
		break;


		//------------- LSU-B -----
		case AX_EXE_LSU_LD:
			printf("LD R%d,R%d[R%d]",regA,regB,regC);
		break;

		case AX_EXE_LSU_ST:
			printf("ST R%d,R%d[R%d]",regA,regB,regC);
		break;

		case AX_EXE_LSU_LDV:
			printf("LDV R%d,R%d[R%d]",regA,regB,regC);
		break;

		case AX_EXE_LSU_STV:
			printf("STV R%d,R%d[R%d]",regA,regB,regC);
		break;

		case AX_EXE_LSU_LDI:
			printf("LDI R%d,%d[R%d]",regA,imm10,regB);
		break;

		case AX_EXE_LSU_STI:
			printf("STI R%d,%d[R%d]",regA,imm10,regB);
		break;

		case AX_EXE_LSU_LDVI:
			printf("LDVI R%d,%d[R%d]",regA,imm10,regB);
		break;

		case AX_EXE_LSU_STVI:
			printf("STVI R%d,%d[R%d]",regA,imm10,regB);
		break;

		//------------- VFPU -----

		case AX_EXE_CMP_FCMP:
			printf("FCMP V%d,V%d",regA,regB);
		break;

		case AX_EXE_CMP_DCMP:
			printf("DCMP V%d,V%d",regA,regB);
		break;

		case AX_EXE_CMP_FCMPI:
			printf("FCMPI V%d,%f",regA,immf);
		break;

		case AX_EXE_CMP_DCMPI:
			printf("DCMPI V%d,%f",regA,immf);
		break;

		//---------
		case AX_EXE_VFPU_FADD:
			printf("FADD V%d,V%d,V%d",regA,regB,regC);
		break;

		case AX_EXE_VFPU_FSUB:
			printf("FSUB V%d,V%d,V%d",regA,regB,regC);
		break;

		case AX_EXE_VFPU_FMUL:
			printf("FMUL V%d,V%d,V%d",regA,regB,regC);
		break;

		case AX_EXE_VFPU_FMULADD:
			printf("FMULADD V%d,V%d,V%d",regA,regB,regC);
		break;

		case AX_EXE_VFPU_FMULSUB:
			printf("FMULSUB V%d,V%d,V%d",regA,regB,regC);
		break;




		case AX_EXE_VFPU_VFADD:
			printf("VFADD V%d,V%d,V%d",regA,regB,regC);
		break;

		case AX_EXE_VFPU_VFSUB:
			printf("VFSUB V%d,V%d,V%d",regA,regB,regC);
		break;

		case AX_EXE_VFPU_VFMUL:
			printf("VFMUL V%d,V%d,V%d",regA,regB,regC);
		break;

		case AX_EXE_VFPU_VFMULADD:
			printf("VFMULADD V%d,V%d,V%d",regA,regB,regC);
		break;

		case AX_EXE_VFPU_VFMULSUB:
			printf("VFMULSUB V%d,V%d,V%d",regA,regB,regC);
		break;




		case AX_EXE_VFPU_VFADDS:
			printf("VFADDS V%d,V%d,V%d",regA,regB,regC);
		break;

		case AX_EXE_VFPU_VFSUBS:
			printf("VFSUBS V%d,V%d,V%d",regA,regB,regC);
		break;

		case AX_EXE_VFPU_VFMULS:
			printf("VFMULS V%d,V%d,V%d",regA,regB,regC);
		break;

		case AX_EXE_VFPU_VFMULADDS:
			printf("VFMULADDS V%d,V%d,V%d",regA,regB,regC);
		break;

		case AX_EXE_VFPU_VFMULSUBS:
			printf("VFMULSUBS V%d,V%d,V%d",regA,regB,regC);
		break;

		case AX_EXE_VFPU_VFSHUFFLE:
			printf("VFMULSUBS V%d,V%d,%d",regA,regB,imm10);
		break;
		//-------------

		case AX_EXE_VFPU_FMOVE:
			printf("FMOVE V%d,V%d",regA,regB);
		break;

		case AX_EXE_VFPU_VFMOVE:
			printf("VFMOVE V%d,V%d",regA,regB);
		break;

		case AX_EXE_VFPU_FMOVEI:
			printf("FMOVEI V%d,%f",regA,immf);
		break;

		case AX_EXE_VFPU_VFMOVEI:
			printf("VFMOVEI V%d,%f",regA,immf);
		break;

		case AX_EXE_VFPU_FNEG:
			printf("FNEG V%d,V%d",regA,regB);
		break;

		case AX_EXE_VFPU_FABS:
			printf("FABS V%d,V%d",regA,regB);
		break;

		case AX_EXE_VFPU_VFNEG:
			printf("VFNEG V%d,V%d",regA,regB);
		break;

		case AX_EXE_VFPU_VFABS:
			printf("VFABS V%d,V%d",regA,regB);
		break;


		case AX_EXE_VFPU_VFTOI:
			printf("VFTOI V%d,V%d,V%d",regA,regB);
		break;

		case AX_EXE_VFPU_VITOF:
			printf("VITOF V%d,V%d,V%d",regA,regB);
		break;

		case AX_EXE_VFPU_FMAX:
			printf("FMAX V%d,V%d",regA,regB);
		break;

		case AX_EXE_VFPU_FMIN:
			printf("FMIN V%d,V%d",regA,regB);
		break;

		case AX_EXE_OTHER_WAIT:
			printf("WAIT");
		break;

		case AX_EXE_OTHER_EXE:
			printf("EXE %a",regA);
		break;

		case AX_EXE_OTHER_ENDP:
			printf("ENDP");
		break;

	}

}

void AX_debug(Core *core)
{
	printf("PC %d: ",core->pc-1);

	AX_debug_core0(core);

	printf("    ");

	if(core->opcode1&1)
		AX_debug_core1(core);

	printf("\n");
}
