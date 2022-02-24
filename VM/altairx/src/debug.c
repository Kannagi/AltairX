
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

#include "vm.h"


static void debug_inst(Core *core, uint32_t index)
{
    uint32_t unit1 = core->operations[index].unit1;
    uint32_t unit2 = core->operations[index].unit2;
    uint32_t size  = core->operations[index].size;

    uint64_t opA   = core->operations[index].opA;
    uint32_t regB  = core->operations[index].regB;
	uint32_t regC  = core->operations[index].regC;

    if(unit1 == AX_EXE_ALU)
    {
        switch(unit2)
        {
            case AX_OPCODE_ADD:
            	printf("ADD R%ld,R%d,R%d",opA,regB,regC);
            break;

            case AX_OPCODE_SUB:
                printf("SUB R%ld,R%d,R%d",opA,regB,regC);
            break;

            case AX_OPCODE_XOR:
                printf("XOR R%ld,R%d,R%d",opA,regB,regC);
            break;

            case AX_OPCODE_OR:
                printf("OR R%ld,R%d,R%d",opA,regB,regC);
            break;

            case AX_OPCODE_AND:
                printf("AND R%ld,R%d,R%d",opA,regB,regC);
            break;

            case AX_OPCODE_LSL:
                printf("LSL R%ld,R%d,R%d",opA,regB,regC);
            break;

            case AX_OPCODE_LSR:
                printf("LSR R%ld,R%d,R%d",opA,regB,regC);
            break;

            case AX_OPCODE_ASR:
                printf("ASR R%ld,R%d,R%d",opA,regB,regC);
            break;

            case AX_OPCODE_MULS:
                printf("MULS P,R%d,R%d",regB,regC);
            break;

            case AX_OPCODE_MULU:
                printf("MULU P,R%d,R%d",regB,regC);
            break;

            case AX_OPCODE_DIVS:
                printf("DIVS Q,R%d,R%d",regB,regC);
            break;

            case AX_OPCODE_DIVU:
                printf("DIVS U,R%d,R%d",regB,regC);
            break;

            case AX_OPCODE_REMS:
                printf("REMS Q,R%d,R%d",regB,regC);
            break;

            case AX_OPCODE_REMU:
                printf("REMU Q,R%d,R%d",regB,regC);
            break;


            case AX_OPCODE_BOOL:
            	printf("BOOL R%ld,R%d",opA,regB);
            break;

            case AX_OPCODE_SEXT:
            	printf("SEXT.%d R%ld,R%d",size,opA,regB);
            break;

            case AX_OPCODE_SLTS:
            	printf("SLTS %d %d",regB,regC);
            break;

            case AX_OPCODE_SLTU:
                printf("SLTU %d %d",regB,regC);
            break;

            case AX_OPCODE_SMOVE:
                printf("SMOVE.%d R%ld,R%d",size,opA,regB);
            break;

            case AX_OPCODE_MOVE:
                printf("MOVE R%ld,R%d",opA,regB);
            break;

            case AX_OPCODE_MOVEINS:
                printf("MOVEINS R%ld",opA);
            break;

            case AX_OPCODE_MOVECYCLE:
                printf("MOVECYCLE R%ld",opA);
            break;

            case AX_OPCODE_MOVERI1:
                printf("MOVE R%ld,RI%d",opA,regB);
            break;

            case AX_OPCODE_MOVERI2:
                printf("MOVE RI%d,R%ld",regB,opA);
            break;
        }
    }
    else if(unit1 == AX_EXE_LSU)
    {
        switch(unit2)
        {
            case AX_OPCODE_LD:
            	printf("LD R%ld,R%d[R%d]",opA,regC,regB);
            break;

            case AX_OPCODE_ST:
                printf("ST R%ld,R%d[R%d]",opA,regC,regB);
            break;

            case AX_OPCODE_LDV:
            	printf("LDV V%ld,R%d[R%d]",opA,regC,regB);
            break;

            case AX_OPCODE_STV:
                printf("STV V%ld,R%d[R%d]",opA,regC,regB);
            break;

            case AX_OPCODE_FLUSH:
            	printf("FLUSH R%d[R%ld]",regB,opA);
			break;

            case AX_OPCODE_PREFETCH:
				printf("PREFETCH R%d[R%ld]",regB,opA);
            break;

            case AX_OPCODE_IFLUSH:
            	printf("IFLUSH R%d[R%ld]",regB,opA);
			break;

            case AX_OPCODE_IPREFETCH:
				printf("IPREFETCH R%d[R%ld]",regB,opA);
            break;

        }
    }
    else if(unit1 == AX_EXE_CMP)
    {
        switch(unit2)
        {
            case AX_OPCODE_CMPFR:
				printf("CMPRFR FR,R%ld",opA);
            break;

            case AX_OPCODE_CMP:
				printf("CMP R%ld,R%d",opA,regB);
            break;

            case AX_OPCODE_FCMP:
				printf("CMPF V%ld,V%d",opA,regB);
            break;

            case AX_OPCODE_DCMP:
				printf("CMPD V%ld,V%d",opA,regB);
            break;


        }
    }
    else  if(unit1 == AX_EXE_BRU)
	{
		int imm = core->imm;
		switch(unit2)
		{
			case AX_OPCODE_BNE: // !=
				printf("BNE %d",imm);
			break;

			case AX_OPCODE_BEQ: // ==
				printf("BEQ %d",imm);
			break;

			case AX_OPCODE_BLE: // <=
				printf("BLE %d",imm);
			break;

			case AX_OPCODE_BG: // >
				printf("BG %d",imm);
			break;

			case AX_OPCODE_BGE: // >=
				printf("BGE %d",imm);
			break;

			case AX_OPCODE_BLS: // <
				printf("BLS %d",imm);
			break;

			case AX_OPCODE_BLES: // <=
				printf("BLES %d",imm);
			break;

			case AX_OPCODE_BGS: // >
				printf("BGS %d",imm);
			break;

			case AX_OPCODE_BGES: // >=
				printf("BGES %d",imm);
			break;

			case AX_OPCODE_BRA:
				printf("BRA: %d",imm);
			break;

			case AX_OPCODE_JMP:
				printf("JMP: %d",imm);
			break;

			case AX_OPCODE_CALL:
				printf("CALL: %d",imm);
			break;

			case AX_OPCODE_JMPBR:
				printf("JMPBR: %d+%d",imm,core->br);
			break;

			case AX_OPCODE_CALLBR:
				printf("CALLBR: %d+%d",imm,core->br);
			break;

			case AX_OPCODE_LOOP:
				printf("LOOP: %d",imm);
			break;

			case AX_OPCODE_INT:
            	printf("INT");
            break;

			case AX_OPCODE_SYSCALL:
				printf("SYSCALL");
            break;

			case AX_OPCODE_RET:
				printf("RET");
			break;

			case AX_OPCODE_RETI:
				printf("RETI");
			break;
		}

	}
    else if(unit1 == AX_EXE_DMA)
    {
        switch(unit2)
        {
            case AX_OPCODE_LDDMA:
				printf("LDDMA R%ld,R%d,R%d",opA,regB,regC);
            break;


            case AX_OPCODE_STDMA:
                printf("STDMA R%ld,R%d,R%d",opA,regB,regC);
            break;

            case AX_OPCODE_WAIT:
                //We don't have anything to wait since we emulate it based on C memory model, all transfers are direcly coherent
            break;
        }

    }
    else if(unit1 == AX_EXE_OTHER)
    {
        switch(unit2)
        {
            case AX_OPCODE_ENDP:
				puts("ENDP");
            break;
        }
    }
    else if(unit1 == AX_EXE_VFPU)
    {
        uint32_t unit3 = unit2>>4;
        unit2 &= 0xF;


        if(unit3 == 0) //VFALU
        {
            opA *= 4;
            switch(unit2)
            {
                case AX_OPCODE_FADD:
                	printf("FADD V%ld,V%d,V%d",opA,regB,regC);
                break;

                case AX_OPCODE_FSUB:
                    printf("FSUB V%ld,V%d,V%d",opA,regB,regC);
                break;

                case AX_OPCODE_FMUL:
                    printf("FMUL V%ld,V%d,V%d",opA,regB,regC);
                break;

                case AX_OPCODE_FMADD:
                    printf("FMADD V%ld,V%d,V%d",opA,regB,regC);
                break;

                case AX_OPCODE_FMSUB:
                    printf("FMSUB V%ld,V%d,V%d",opA,regB,regC);
                break;

                //------------

                case AX_OPCODE_VFADD:
					printf("VFADD V%ld,V%d,V%d",opA,regB,regC);
                break;

                case AX_OPCODE_VFSUB:
                    printf("VFSUB V%ld,V%d,V%d",opA,regB,regC);
                break;

                case AX_OPCODE_VFMUL:
                    printf("VFMUL V%ld,V%d,V%d",opA,regB,regC);
                break;

                case AX_OPCODE_VFMADD:
                    printf("VFMADD V%ld,V%d,V%d",opA,regB,regC);
                break;

                case AX_OPCODE_VFMSUB:
                	printf("VFMSUB V%ld,V%d,V%d",opA,regB,regC);
                break;

                //------------

                case AX_OPCODE_VFADDS:
                	printf("VFADDS V%ld,V%d,V%d",opA,regB,regC);
                break;

                case AX_OPCODE_VFSUBS:
                	printf("VFSUBS V%ld,V%d,V%d",opA,regB,regC);
                break;

                case AX_OPCODE_VFMULS:
                	printf("VFMULS V%ld,V%d,V%d",opA,regB,regC);
                break;

                case AX_OPCODE_VFMADDS:
                	printf("VFMADDS V%ld,V%d,V%d",opA,regB,regC);
                break;

                case AX_OPCODE_VFMSUBS:
                	printf("VFMSUBS V%ld,V%d,V%d",opA,regB,regC);
                break;

                //------------


                case AX_OPCODE_VFSHUFFLE:
                	printf("VFSHUFFLE V%ld,V%d,V%d",opA,regB,regC);
                break;



            }
        }
        else if(unit3 == 1) //VFMOVE
        {

            switch(unit2)
            {
                case AX_OPCODE_FMOVE:
                    printf("FMOVE V%ld,V%d",opA,regB);
                break;

                case AX_OPCODE_VFMOVE:
                    printf("VFMOVE V%ld,V%d",opA,regB);
                break;

                case AX_OPCODE_FNEG:
                	printf("FNEG V%ld,V%d",opA,regB);
                break;

                case AX_OPCODE_FABS:
                    printf("FABS V%ld,V%d",opA,regB);
                break;

                case AX_OPCODE_VFNEG:
                	printf("VFNEG V%ld,V%d",opA,regB);
                break;

                case AX_OPCODE_VFABS:
                	printf("VFABS V%ld,V%d",opA,regB);
                break;

                case AX_OPCODE_VFTOI:
                	printf("VFTOI V%ld,V%d",opA,regB);
                break;

                case AX_OPCODE_VITOF:
                	printf("VITOF V%ld,V%d",opA,regB);
                break;

                case AX_OPCODE_VFTOD:
                	printf("VFTOD V%ld,V%d",opA,regB);
                break;

                case AX_OPCODE_VDTOF:
                	printf("VDTOF V%ld,V%d",opA,regB);
                break;

                case AX_OPCODE_VFTOH:
                	printf("VFTOH V%ld,V%d",opA,regB);
                break;

                case AX_OPCODE_VHTOF:
                	printf("VHTOF V%ld,V%d",opA,regB);
                break;
            }

        }
        else if(unit3 == 2) //EFU
        {/*
			case AX_OPCODE_DDIV:
				printf("DDIV Q,V%d,V%d",regB,regC);
			break;*/
        }
        else //DOUBLE
        {
            switch(unit2)
            {
                case AX_OPCODE_DMOVE:
                    printf("DMOVE V%ld,V%d",opA,regB);
                break;

                case AX_OPCODE_DADD:
                    printf("DADD V%ld,V%d,V%d",opA,regB,regC);
                break;

                case AX_OPCODE_DSUB:
                    printf("DSUB V%ld,V%d,V%d",opA,regB,regC);
                break;

                case AX_OPCODE_DMUL:
                    printf("DMUL V%ld,V%d,V%d",opA,regB,regC);
                break;

                case AX_OPCODE_DNEG:
                    printf("DNEG V%ld,V%d",opA,regB);
                break;

                case AX_OPCODE_DABS:
                    printf("DABS V%ld,V%d",opA,regB);
                break;


            }

        }

    }
    else if(unit1 == AX_EXE_NOP)
    {
    	printf("NOP");
    }

}

void AX_debug(Core *core)
{
	printf("PC %d: ",core->pc-1-core->swt);

	debug_inst(core, 0);

	printf("    ");

	if(core->swt == 1)
		debug_inst(core, 1);

	printf("\n");
}
