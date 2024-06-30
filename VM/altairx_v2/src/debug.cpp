#include <cstdio>
#include <vector>
#include "altairx.hpp"

void altairx_debug(uint32_t opcode)
{
    char szf = 's';
    float immf = 0.0f;

    uint32_t unit = (opcode >> 1) & 0x7F;
    uint32_t size = (opcode >> 8) & 0x3;

    uint32_t rega = (opcode >> 26) & 0x3F;
    uint32_t regb = (opcode >> 20) & 0x3F;
    uint32_t regc = (opcode >> 14) & 0x3F;

    uint32_t imm24 = (opcode >> 8) & 0xFFFFFF;
    uint32_t imm18 = (opcode >> 8) & 0x3FFFF;
    uint32_t imm12 = (opcode >> 8) & 0xFFF;

    //uint32_t imm9 = (opcode >> 10) & 0x1FF;
    uint32_t imm10 = (opcode >> 10) & 0x3FF;
    uint32_t imm16 = (opcode >> 10) & 0xFFFF;

    switch(unit)
    {
    //------------- ALU-A -----
    case AX_EXE_ALU_MOVEIX:
        printf("MOVEIX %x\n", imm24);
        break;

    case AX_EXE_ALU_COP:
        printf("COP %x\n", imm24);
        break;

    case AX_EXE_ALU_EXT:
        printf("EXT R%d,R%d,%x\n", rega, regb, imm12);
        break;

    case AX_EXE_ALU_INS:
        printf("INS R%d,R%d,%x\n", rega, regb, imm12);
        break;

    case AX_EXE_ALU_MOVEI:
        printf("MOVEI %d\n", imm18);
        break;

    case AX_EXE_ALU_MOVEN:
        printf("MOVEN %d\n", -imm18);
        break;

    case AX_EXE_ALU_MOVEUP:
        printf("SMOVE %d<<%d\n", imm16, size * 16);
        break;

    case AX_EXE_ALU_CMP:
        printf("CMP R%d,R%d\n", regb, regc);
        break;
    case AX_EXE_ALU_TEST:
        printf("TEST R%d,R%d\n", regb, regc);
        break;
    case AX_EXE_ALU_CMPBIT:
        printf("CMPBIT R%d,R%d\n", regb, regc);
        break;
    case AX_EXE_ALU_TESTFR:
        printf("TESTFR R%d,FR\n", regb);
        break;
    case AX_EXE_ALU_SEXT:
        printf("SEXT R%d,R%d,R%d\n", rega, regb, regc);
        break;

        //------------- ALU-B -----

    case AX_EXE_ALU_ADD:
        printf("ADD R%d,R%d,R%d\n", rega, regb, regc);
        break;

    case AX_EXE_ALU_SUB:
        printf("SUB R%d,R%d,R%d\n", rega, regb, regc);
        break;

    case AX_EXE_ALU_XOR:
        printf("XOR R%d,R%d,R%d\n", rega, regb, regc);
        break;

    case AX_EXE_ALU_OR:
        printf("OR R%d,R%d,R%d\n", rega, regb, regc);
        break;

    case AX_EXE_ALU_AND:
        printf("AND R%d,R%d,R%d\n", rega, regb, regc);
        break;

    case AX_EXE_ALU_LSL:
        printf("LSL R%d,R%d,R%d\n", rega, regb, regc);
        break;

    case AX_EXE_ALU_ASR:
        printf("ASR R%d,R%d,R%d\n", rega, regb, regc);
        break;

    case AX_EXE_ALU_LSR:
        printf("LSR R%d,R%d,R%d\n", rega, regb, regc);
        break;

    case AX_EXE_ALU_SE:
        printf("SE R%d,R%d,R%d\n", rega, regb, regc);
        break;
    case AX_EXE_ALU_SLTS:
        printf("SLTS R%d,R%d,R%d\n", rega, regb, regc);
        break;
    case AX_EXE_ALU_SLTU:
        printf("SLTU R%d,R%d,R%d\n", rega, regb, regc);
        break;
    case AX_EXE_ALU_SAND:
        printf("SAND R%d,R%d,R%d\n", rega, regb, regc);
        break;
    case AX_EXE_ALU_SBIT:
        printf("SBIT R%d,R%d,R%d\n", rega, regb, regc);
        break;
    case AX_EXE_ALU_SEN:
        printf("SEN R%d,R%d,R%d\n", rega, regb, regc);
        break;
    case AX_EXE_ALU_CMOVEN:
        printf("CMOVEN R%d,R%d,R%d\n", rega, regb, regc);
        break;
    case AX_EXE_ALU_CMOVE:
        printf("CMOVE R%d,R%d,R%d\n", rega, regb, regc);
        break;

    case AX_EXE_LSU_LD:
        printf("LD R%d,R%d[R%d]\n", rega, regb, regc);
        break;
    case AX_EXE_LSU_ST:
        printf("ST R%d,R%d[R%d]\n", rega, regb, regc);
        break;
    case AX_EXE_LSU_FLD:
        printf("FLD R%d,R%d[R%d]\n", rega, regb, regc);
        break;
    case AX_EXE_LSU_FST:
        printf("FST R%d,R%d[R%d]\n", rega, regb, regc);
        break;
    case AX_EXE_LSU_LDI:
        printf("LDI R%d,%d[R%d]\n", rega, imm10, regc);
        break;
    case AX_EXE_LSU_STI:
        printf("STI R%d,%d[R%d]\n", rega, imm10, regc);
        break;
    case AX_EXE_LSU_FLDI:
        printf("FLDI R%d,%d[R%d]\n", rega, imm10, regc);
        break;
    case AX_EXE_LSU_FSTI:
        printf("FSTI R%d,%d[R%d]\n", rega, imm10, regc);
        break;
    case AX_EXE_LSU_LDSP:
        printf("LDSP R%d,%d[SP]\n", rega, imm16);
        break;
    case AX_EXE_LSU_STSP:
        printf("STSP R%d,%d[SP]\n", rega, imm16);
        break;
    case AX_EXE_LSU_FLDSP:
        printf("FLDSP R%d,%d[SP]\n", rega, imm16);
        break;
    case AX_EXE_LSU_FSTSP:
        printf("FSTSP R%d,%d[SP]\n", rega, imm16);
        break;

    case AX_EXE_FPU_FADD:
        printf("FADD.%c V%d,V%d,V%d\n", szf, rega, regb, regc);
        break;
    case AX_EXE_FPU_FSUB:
        printf("FSUB.%c V%d,V%d,V%d\n", szf, rega, regb, regc);
        break;
    case AX_EXE_FPU_FMUL:
        printf("FMUL.%c V%d,V%d,V%d\n", szf, rega, regb, regc);
        break;
    case AX_EXE_FPU_FNMUL:
        printf("FNMUL.%c V%d,V%d,V%d\n", szf, rega, regb, regc);
        break;
    case AX_EXE_FPU_HTOF:
        printf("HTOF.%c V%d,V%d\n", szf, rega, regb);
        break;
    case AX_EXE_FPU_FTOH:
        printf("FTOH.%c V%d,V%d\n", szf, rega, regb);
        break;
    case AX_EXE_FPU_ITOF:
        printf("ITOF.%c V%d,V%d\n", szf, rega, regb);
        break;
    case AX_EXE_FPU_FTOI:
        printf("FTOI.%c V%d,V%d\n", szf, rega, regb);
        break;
    case AX_EXE_FPU_FNEG:
        printf("FNEG.%c V%d,V%d\n", szf, rega, regb);
        break;
    case AX_EXE_FPU_FABS:
        printf("FABS.%c V%d,V%d\n", szf, rega, regb);
        break;
    case AX_EXE_FPU_FMIN:
        printf("FMIN.%c V%d,V%d,V%d\n", szf, rega, regb, regc);
        break;
    case AX_EXE_FPU_FMAX:
        printf("FMAX.%c V%d,V%d,V%d\n", szf, rega, regb, regc);
        break;

    case AX_EXE_FPU_FMOVE:
        printf("FMOVE.%c V%d,V%d,V%d\n", szf, rega, regb, regc);
        break;
    case AX_EXE_FPU_FCMP:
        printf("FCMP.%c V%d,V%d\n", szf, regb, regc);
        break;
    case AX_EXE_FPU_FMOVEI:
        printf("FMOVEI.%c V%d,V%d,%f\n", szf, rega, regb, immf);
        break;
    case AX_EXE_FPU_FCMPI:
        printf("FCMPI.%c V%d,%f\n", szf, rega, immf);
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
        printf("BNE %x \n", imm24);
        break;
    case AX_EXE_BRU_BEQ:
        printf("BEQ %x \n", imm24);
        break;
    case AX_EXE_BRU_BL:
        printf("BL %x \n", imm24);
        break;
    case AX_EXE_BRU_BLE:
        printf("BLE %x \n", imm24);
        break;
    case AX_EXE_BRU_BG:
        printf("BG %x \n", imm24);
        break;
    case AX_EXE_BRU_BGE:
        printf("BGE %x \n", imm24);
        break;
    case AX_EXE_BRU_BLS:
        printf("BLS %x \n", imm24);
        break;
    case AX_EXE_BRU_BLES:
        printf("BLES %x \n", imm24);
        break;
    case AX_EXE_BRU_BGS:
        printf("BGS %x \n", imm24);
        break;
    case AX_EXE_BRU_BGES:
        printf("BGES %x \n", imm24);
        break;
    case AX_EXE_BRU_BRA:
        printf("BRA %x \n", imm24);
        break;
    case AX_EXE_BRU_LOOP:
        printf("LOOP %x \n", imm24);
        break;
    case AX_EXE_BRU_JUMP:
        printf("JUMP %x \n", imm24);
        break;
    case AX_EXE_BRU_CALL:
        printf("CALL %x \n", imm24);
        break;
    case AX_EXE_BRU_CALLR:
        printf("CALLR %x \n", imm24);
        break;
    case AX_EXE_BRU_OTHER:
        printf("BRU_OTHER %x \n", imm24);
        break;

    default:

        return;
    }
}