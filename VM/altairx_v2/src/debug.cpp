


static void AX_execute_unit0(Core *core,uint64_t rega,uint64_t regb,uint64_t regc,uint32_t unit,uint32_t size,uint32_t opcode)
{
	uint64_t tmp,tmp2;

	switch(unit)
	{
		//------------- ALU-A -----
		case AX_EXE_ALU_MOVEIX:
            printf("MOVEIX %x\n",imm24);
		break;

        case AX_EXE_ALU_COP:
            printf("COP %x\n",imm24);
		break;

        case AX_EXE_ALU_EXT:
            printf("EXT R%d,R%d,%x\n",rega,regb,imm12);
		break;

        case AX_EXE_ALU_INS:
            printf("INS R%d,R%d,%x\n",rega,regb,imm12);
		break;

        case AX_EXE_ALU_MOVEI:
            printf("MOVEI %d\n",imm18);
		break;

		case AX_EXE_ALU_MOVEN:
            printf("MOVEN %d\n",-imm18);
		break;

        case AX_EXE_ALU_SMOVE:
            printf("SMOVE %d<<%d\n",imm16,size*16);
		break;


        case AX_EXE_ALU_CMP:
            printf("CMP R%d,R%d\n",regb,regc);
            break;
        case AX_EXE_ALU_TEST:
            printf("TEST R%d,R%d\n",regb,regc);
            break;
        case AX_EXE_ALU_CMPFR:
            printf("CMP R%d,FR\n",regb);
            break;
        case AX_EXE_ALU_TESTFR:
            printf("TEST R%d,FR\n",regb);
            break;
        case AX_EXE_ALU_CMOVE:
            printf("CMOVE R%d,R%d,R%d\n",rega,regb,regc);
            break;
        case AX_EXE_ALU_SEXT:
            printf("CMOVE R%d,R%d,R%d\n",rega,regb,regc);
            break;
        case AX_EXE_ALU_ADC:
            printf("CMOVE R%d,R%d,R%d\n",rega,regb,regc);
            break;
        case AX_EXE_ALU_SBC:
            printf("CMOVE R%d,R%d,R%d\n",rega,regb,regc);
            break;

		//------------- ALU-B -----

		case AX_EXE_ALU_ADD:
            printf("CMOVE R%d,R%d,R%d\n",rega,regb,regc);
		break;

		case AX_EXE_ALU_SUB:
            printf("CMOVE R%d,R%d,R%d\n",rega,regb,regc);
		break;

		case AX_EXE_ALU_XOR:
            printf("CMOVE R%d,R%d,R%d\n",rega,regb,regc);
		break;

		case AX_EXE_ALU_OR:
            printf("CMOVE R%d,R%d,R%d\n",rega,regb,regc);
		break;

		case AX_EXE_ALU_AND:
            printf("CMOVE R%d,R%d,R%d\n",rega,regb,regc);
		break;

		case AX_EXE_ALU_LSL:
            printf("CMOVE R%d,R%d,R%d\n",rega,regb,regc);
		break;

		case AX_EXE_ALU_ASR:
            printf("CMOVE R%d,R%d,R%d\n",rega,regb,regc);
		break;

		case AX_EXE_ALU_LSR:
            printf("CMOVE R%d,R%d,R%d\n",rega,regb,regc);
		break;

        case AX_EXE_ALU_SE:
            printf("CMOVE R%d,R%d,R%d\n",rega,regb,regc);
            break;
        case AX_EXE_ALU_SLTS:
            printf("CMOVE R%d,R%d,R%d\n",rega,regb,regc);
            break;
        case AX_EXE_ALU_SLTU:
            printf("CMOVE R%d,R%d,R%d\n",rega,regb,regc);
            break;
        case AX_EXE_ALU_SAND:
            printf("CMOVE R%d,R%d,R%d\n",rega,regb,regc);
            break;
        case AX_EXE_ALU_HADD:
            printf("CMOVE R%d,R%d,R%d\n",rega,regb,regc);
            break;
        case AX_EXE_ALU_HSUB:
            printf("CMOVE R%d,R%d,R%d\n",rega,regb,regc);
            break;
        case AX_EXE_ALU_HMUL:
            printf("CMOVE R%d,R%d,R%d\n",rega,regb,regc);
            break;
        case AX_EXE_ALU_HTOI:
            printf("CMOVE R%d,R%d,R%d\n",rega,regb,regc);
            break;

        case AX_EXE_LSU_LD:
            printf("CMOVE R%d,R%d[R%d]\n",rega,regb,regc);
            break;
        case AX_EXE_LSU_ST:
            printf("CMOVE R%d,R%d[R%d]\n",rega,regb,regc);
            break;
        case AX_EXE_LSU_LDV:
            printf("CMOVE R%d,R%d[R%d]\n",rega,regb,regc);
            break;
        case AX_EXE_LSU_STV:
            printf("CMOVE R%d,R%d[R%d]\n",rega,regb,regc);
            break;
        case AX_EXE_LSU_LDI:
            printf("CMOVE R%d,%d[R%d]\n",rega,imm10,regc);
            break;
        case AX_EXE_LSU_STI:
            printf("CMOVE R%d,%d[R%d]\n",rega,imm10,regc);
            break;
        case AX_EXE_LSU_LDVI:
            printf("CMOVE R%d,%d[R%d]\n",rega,imm10,regc);
            break;
        case AX_EXE_LSU_STVI:
            printf("CMOVE R%d,%d[R%d]\n",rega,imm10,regc);
            break;
        case AX_EXE_LSU_LDSP:
            printf("CMOVE R%d,%d[SP]\n",rega,imm16);
            break;
        case AX_EXE_LSU_STSP:
            printf("CMOVE R%d,%d[SP]\n",rega,imm16);
            break;
        case AX_EXE_LSU_LDVSP:
            printf("CMOVE R%d,%d[SP]\n",rega,imm16);
            break;
        case AX_EXE_LSU_STVSP:
            printf("CMOVE R%d,%d[SP]\n",rega,imm16);
            break;

        case AX_EXE_FPU_FADD:
            printf("CMOVE.%c V%d,V%d,V%d\n",szf,rega,regb,regc);
            break;
        case AX_EXE_FPU_FSUB:
            printf("CMOVE.%c V%d,V%d,V%d\n",szf,rega,regb,regc);
            break;
        case AX_EXE_FPU_FMUL:
            printf("CMOVE.%c V%d,V%d,V%d\n",szf,rega,regb,regc);
            break;
        case AX_EXE_FPU_FNMUL:
            printf("CMOVE.%c V%d,V%d,V%d\n",szf,rega,regb,regc);
            break;
        case AX_EXE_FPU_HTOF:
            printf("CMOVE.%c V%d,V%d\n",szf,rega,regb);
            break;
        case AX_EXE_FPU_FTOH:
            printf("CMOVE.%c V%d,V%d\n",szf,rega,regb);
            break;
        case AX_EXE_FPU_ITOF:
            printf("CMOVE.%c V%d,V%d\n",szf,rega,regb);
            break;
        case AX_EXE_FPU_FTOI:
            printf("CMOVE.%c V%d,V%d\n",szf,rega,regb);
            break;
        case AX_EXE_FPU_FNEG:
            printf("CMOVE.%c V%d,V%d\n",szf,rega,regb);
            break;
        case AX_EXE_FPU_FABS:
            printf("CMOVE.%c V%d,V%d\n",szf,rega,regb);
            break;
        case AX_EXE_FPU_FMIN:
            printf("CMOVE.%c V%d,V%d,V%d\n",szf,rega,regb,regc);
            break;
        case AX_EXE_FPU_FMAX:
            printf("CMOVE.%c V%d,V%d,V%d\n",szf,rega,regb,regc);
            break;
        
        case AX_EXE_FPU_FMOVE:
            printf("CMOVE.%c V%d,V%d,V%d\n",szf,rega,regb,regc);
            break;
        case AX_EXE_FPU_FCMP:
            printf("CMOVE.%c V%d,V%d\n",szf,regb,regc);
            break;
        case AX_EXE_FPU_FMOVEI:
            printf("CMOVE.%c V%d,V%d,%f\n",szf,rega,regb,immf);
            break;
        case AX_EXE_FPU_FCMPI:
            printf("CMOVE.%c V%d,%f\n",szf,rega,immf);
            break;

        // Instructions ALU3(4)
        case AX_EXE_ALU_ADD3:
            // Code pour AX_EXE_ALU_ADD3
            break;
        case AX_EXE_ALU_SUB3:
            // Code pour AX_EXE_ALU_SUB3
            break;
        case AX_EXE_ALU_XOR3:
            // Code pour AX_EXE_ALU_XOR3
            break;
        case AX_EXE_ALU_OR3:
            // Code pour AX_EXE_ALU_OR3
            break;
        case AX_EXE_ALU_AND3:
            // Code pour AX_EXE_ALU_AND3
            break;
        case AX_EXE_ALU_LSL3:
            // Code pour AX_EXE_ALU_LSL3
            break;
        case AX_EXE_ALU_ASR3:
            // Code pour AX_EXE_ALU_ASR3
            break;
        case AX_EXE_ALU_LSR3:
            // Code pour AX_EXE_ALU_LSR3
            break;
        case AX_EXE_ALU_SE3:
            // Code pour AX_EXE_ALU_SE3
            break;
        case AX_EXE_ALU_SLTS3:
            // Code pour AX_EXE_ALU_SLTS3
            break;
        case AX_EXE_ALU_SLTU3:
            // Code pour AX_EXE_ALU_SLTU3
            break;
        case AX_EXE_ALU_SAND3:
            // Code pour AX_EXE_ALU_SAND3
            break;
        case AX_EXE_ALU_HADD3:
            // Code pour AX_EXE_ALU_HADD3
            break;
        case AX_EXE_ALU_HSUB3:
            // Code pour AX_EXE_ALU_HSUB3
            break;
        case AX_EXE_ALU_HMUL3:
            // Code pour AX_EXE_ALU_HMUL3
            break;
        case AX_EXE_ALU_HTOI3:
            // Code pour AX_EXE_ALU_HTOI3
            break;

        // Instructions EFU(5)
        case AX_EXE_EFU_FDIV:
            // Code pour AX_EXE_EFU_FDIV
            break;
        case AX_EXE_EFU_FATAN2:
            // Code pour AX_EXE_EFU_FATAN2
            break;
        case AX_EXE_EFU_DDIV:
            // Code pour AX_EXE_EFU_DDIV
            break;
        case AX_EXE_EFU_DATAN2:
            // Code pour AX_EXE_EFU_DATAN2
            break;
        case AX_EXE_EFU_FSQRT:
            // Code pour AX_EXE_EFU_FSQRT
            break;
        case AX_EXE_EFU_FSIN:
            // Code pour AX_EXE_EFU_FSIN
            break;
        case AX_EXE_EFU_FATAN:
            // Code pour AX_EXE_EFU_FATAN
            break;
        case AX_EXE_EFU_FEXP:
            // Code pour AX_EXE_EFU_FEXP
            break;
        case AX_EXE_EFU_DSQRT:
            // Code pour AX_EXE_EFU_DSQRT
            break;
        case AX_EXE_EFU_DSIN:
            // Code pour AX_EXE_EFU_DSIN
            break;
        case AX_EXE_EFU_DATAN:
            // Code pour AX_EXE_EFU_DATAN
            break;
        case AX_EXE_EFU_DEXP:
            // Code pour AX_EXE_EFU_DEXP
            break;
        case AX_EXE_EFU_FPOW:
            // Code pour AX_EXE_EFU_FPOW
            break;
        case AX_EXE_EFU_DPOW:
            // Code pour AX_EXE_EFU_DPOW
            break;
        case AX_EXE_EFU_EMOVETO:
            // Code pour AX_EXE_EFU_EMOVETO
            break;
        case AX_EXE_EFU_EMOVEFROM:
            // Code pour AX_EXE_EFU_EMOVEFROM
            break;

        // Instructions MDU(6)
        case AX_EXE_MDU_DIV:
            // Code pour AX_EXE_MDU_DIV
            break;
        case AX_EXE_MDU_DIVU:
            // Code pour AX_EXE_MDU_DIVU
            break;
        case AX_EXE_MDU_MUL:
            // Code pour AX_EXE_MDU_MUL
            break;
        case AX_EXE_MDU_MULU:
            // Code pour AX_EXE_MDU_MULU
            break;
        case AX_EXE_MDU_MOVETO:
            // Code pour AX_EXE_MDU_MOVETO
            break;
        case AX_EXE_MDU_MOVEFROM:
            // Code pour AX_EXE_MDU_MOVEFROM
            break;

        // Instructions BRU(7)
        case AX_EXE_BRU_BNE:
            // Code pour AX_EXE_BRU_BNE
            break;
        case AX_EXE_BRU_BEQ:
            // Code pour AX_EXE_BRU_BEQ
            break;
        case AX_EXE_BRU_BL:
            // Code pour AX_EXE_BRU_BL
            break;
        case AX_EXE_BRU_BLE:
            // Code pour AX_EXE_BRU_BLE
            break;
        case AX_EXE_BRU_BG:
            // Code pour AX_EXE_BRU_BG
            break;
        case AX_EXE_BRU_BGE:
            // Code pour AX_EXE_BRU_BGE
            break;
        case AX_EXE_BRU_BLS:
            // Code pour AX_EXE_BRU_BLS
            break;
        case AX_EXE_BRU_BLES:
            // Code pour AX_EXE_BRU_BLES
            break;
        case AX_EXE_BRU_BGS:
            // Code pour AX_EXE_BRU_BGS
            break;
        case AX_EXE_BRU_BGES:
            // Code pour AX_EXE_BRU_BGES
            break;
        case AX_EXE_BRU_BRA:
            // Code pour AX_EXE_BRU_BRA
            break;
        case AX_EXE_BRU_LOOP:
            // Code pour AX_EXE_BRU_LOOP
            break;
        case AX_EXE_BRU_JUMP:
            // Code pour AX_EXE_BRU_JUMP
            break;
        case AX_EXE_BRU_CALL:
            // Code pour AX_EXE_BRU_CALL
            break;
        case AX_EXE_BRU_THREAD:
            // Code pour AX_EXE_BRU_THREAD
            break;
        case AX_EXE_BRU_OTHER:
            // Code pour AX_EXE_BRU_OTHER
            break;
            
		default:
            
            return;

	}

}