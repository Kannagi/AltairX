
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <vector>
#include "altairx.hpp"

static uint16_t FloattoHalf(float fval)
{
	uint32_t val;
	memcpy(&val, &fval, sizeof(val));

	uint32_t sign,exp,mant,texp;

	//float to half
	sign = (val>>16)&0x8000; //sign
	texp = (val>>23)&0x00FF; //exp
	mant = (val>>13)&0x03FF; //mantisse

	exp = (texp&0x0F)<<10;

	if(texp&0x80)
		exp |= 0x4000;

	return sign+mant+exp;
}


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

static const uint64_t sizemask[4] =
{
	0x00000000000000FFull,
	0x000000000000FFFFull,
	0x00000000FFFFFFFFull,
	0xFFFFFFFFFFFFFFFFull,
};

static const uint64_t signmask[4] =
{
	0xFFFFFFFFFFFFFF00ull,
	0xFFFFFFFFFFFF0000ull,
	0xFFFFFFFF00000000ull,
	0x0000000000000000ull,
};

static uint64_t value_immediate9(uint32_t opcode,uint64_t reg,uint32_t imm24)
{
    if(opcode&0x400)
	{
        uint64_t tmp = (opcode>>11)&0x1FF;

        if(tmp&0x100)
            tmp |= 0xFFFFFFFFFFFFFF00ull;

		return tmp ^ (imm24<<8);
	}
	else
	{
		return reg;
	}
}

//-----------------------
 __attribute__((optimize("-O2"), noinline)) void AltairX::executeStore(void *reg,uint64_t offset,uint32_t size)
 {
    void *address = this->memorymap.wram.data();
    uint64_t mask = this->wram_mask;

    if(offset&MEMORY_MAP_SPM2_BEGIN)
	{
		address = this->memorymap.spm2.data();
		mask = this->spm2_mask; //Max 16 Mio
	}
	else if(offset&MEMORY_MAP_SPMT_BEGIN)
	{
		address = this->memorymap.spmt.data();
		mask = this->spmt_mask; //Max 16 Mio
	}
	else if(offset&MEMORY_MAP_ROM_BEGIN)
	{
		address = this->memorymap.rom.data();
		mask = this->rom_mask; //Max 16 Mio
	}
	else if(offset&MEMORY_MAP_IO_BEGIN)
	{
		address = this->memorymap.io.data();
		mask = 0x7FFFF; //Max 512 Kio
		altairx_io_read(core,offset,reg);
	}
	else //SPM
	{
		address = this->core->spm;
		mask = 0x3FFF; //Max 16 Kio
	}

	offset &= mask;

	memcpy(address+offset, reg, size);
}


__attribute__((optimize("-O2"), noinline))  void AltairX::executeLoad(void *reg,uint64_t offset,uint32_t size)
{
    void *address = this->memorymap.wram.data();
    uint64_t mask = this->wram_mask;

    if(offset&MEMORY_MAP_SPM2_BEGIN)
	{
		address = this->memorymap.spm2.data();
		mask = this->spm2_mask; //Max 16 Mio
	}
	else if(offset&MEMORY_MAP_SPMT_BEGIN)
	{
		address = this->memorymap.spmt.data();
		mask = this->spmt_mask; //Max 16 Mio
	}
	else if(offset&MEMORY_MAP_ROM_BEGIN)
	{
		address = this->memorymap.rom.data();
		mask = this->rom_mask; //Max 16 Mio
	}
	else if(offset&MEMORY_MAP_IO_BEGIN)
	{
		address = this->memorymap.io.data();
		mask = 0x7FFFF; //Max 512 Kio
		altairx_io_write(core,offset,reg);
	}
	else //SPM
	{
		address = this->core->spm;
		mask = 0x3FFF; //Max 16 Kio
	}

	offset &= mask;

	memcpy(reg, address+offset, size);

}

void AltairX::execute_unit_double(uint32_t rega,uint32_t regb,uint32_t regc,uint32_t unit,uint32_t imm24)
{
    uint64_t tmp,tmp2;

	switch(unit)
	{
        case AX_EX_FPU_FADD:
            core->dreg[rega] = core->dreg[regb] + core->dreg[regc];
            break;
        case AX_EX_FPU_FSUB:
            core->dreg[rega] = core->dreg[regb] - core->dreg[regc];
            break;
        case AX_EX_FPU_FMUL:
            core->dreg[rega] = core->dreg[regb] * core->dreg[regc];
            break;
        case AX_EX_FPU_FNMUL:
            core->dreg[rega] = -core->dreg[regb] * core->dreg[regc];
            break;
        case AX_EX_FPU_HTOF: //Double to float
            // Code pour AX_EX_FPU_HTOF
            break;
        case AX_EX_FPU_FTOH:
            // Code pour AX_EX_FPU_FTOH
            break;
        case AX_EX_FPU_ITOF:
            // Code pour AX_EX_FPU_ITOF
            break;
        case AX_EX_FPU_FTOI:
            // Code pour AX_EX_FPU_FTOI
            break;
        case AX_EX_FPU_FNEG:
            // Code pour AX_EX_FPU_FNEG
            break;
        case AX_EX_FPU_FABS:
            // Code pour AX_EX_FPU_FABS
            break;
        case AX_EX_FPU_FMIN:
            // Code pour AX_EX_FPU_FMIN
            break;
        case AX_EX_FPU_FMAX:
            // Code pour AX_EX_FPU_FMAX
            break;
        case AX_EX_FPU_FMOVEI:
            // Code pour AX_EX_FPU_FMOVEI
            break;
        case AX_EX_FPU_FMOVE:
        break;
        case AX_EX_FPU_FCMP:
        break;
        case AX_EX_FPU_FCMPI:
        break;

    }
}

void AltairX::execute_unit_float(uint32_t rega,uint32_t regb,uint32_t regc,uint32_t unit,uint32_t imm24)
{
    uint64_t tmp;
    float ftmp;

	switch(unit)
	{
        case AX_EX_FPU_FADD:
            core->freg[rega<<1] = core->freg[regb<<1] + core->freg[regc<<1];
            break;
        case AX_EX_FPU_FSUB:
            core->freg[rega<<1] = core->freg[regb<<1] - core->freg[regc<<1];
            break;
        case AX_EX_FPU_FMUL:
            core->freg[rega<<1] = core->freg[regb<<1] * core->freg[regc<<1];
            break;
        case AX_EX_FPU_FNMUL:
            core->freg[rega<<1] = -core->freg[regb<<1] * core->freg[regc<<1];
            break;
        case AX_EX_FPU_HTOF:
            core->freg[rega<<1] = HalftoFloat(core->hreg[regb<<2]);
            break;
        case AX_EX_FPU_FTOH:
            core->hreg[rega<<2] = FloattoHalf(core->freg[regb<<1]);
            break;
        case AX_EX_FPU_ITOF:
            core->freg[rega<<1] = core->fireg[regb<<1];
            break;
        case AX_EX_FPU_FTOI:
            core->fireg[rega<<1] = core->freg[regb<<1];
            break;
        case AX_EX_FPU_FNEG:
            core->freg[rega<<1] = -core->freg[regb<<1];
            break;
        case AX_EX_FPU_FABS:
            if(core->freg[regb<<1] < 0.0f)
                core->freg[rega<<1] = -core->freg[regb<<1];
            else
                core->freg[rega<<1] = core->freg[regb<<1];
        break;
        case AX_EX_FPU_FMIN:
            if(core->freg[regb<<1] < core->freg[regc<<1])
                core->freg[rega<<1] = core->freg[regb<<1];
            else
                core->freg[rega<<1] = core->freg[regb<<1];
            break;
        case AX_EX_FPU_FMAX:
            if(core->freg[regb<<1] > core->freg[regc<<1])
                core->freg[rega<<1] = core->freg[regb<<1];
            else
                core->freg[rega<<1] = core->freg[regb<<1];
            break;
        
        case AX_EX_FPU_FMOVE:
            core->freg[rega<<1] = core->freg[regb<<1];
        break;
        case AX_EX_FPU_FCMP:
            ftmp = core->freg[regb<<1] -  core->freg[regb<<1];

            if(ftmp == 0.0f)
                core->fr |= Z_MASK;

            if(ftmp > 0.0f)
                core->fr |= O_MASK;

            if(ftmp < 0.0f)
                core->fr |= N_MASK;

        break;

        case AX_EX_FPU_FMOVEI:
            tmp = ( (this->topcode>>10)&0xFFFF) | (imm24<<16) ;

            memcpy(&core->freg[rega<<1], &tmp, 4);

            break;
        case AX_EX_FPU_FCMPI:
            tmp = ( (this->topcode>>10)&0xFFFF) | (imm24<<16) ;

            memcpy(&ftmp, &tmp, 4);

            ftmp = core->freg[regb<<1] -  ftmp;

            if(ftmp == 0.0f)
                core->fr |= Z_MASK;

            if(ftmp > 0.0f)
                core->fr |= O_MASK;

            if(ftmp < 0.0f)
                core->fr |= N_MASK;

        break;

    }
}

void AltairX::execute_unit_falu(uint32_t rega,uint32_t regb,uint32_t regc,uint32_t unit,uint32_t imm24)
{

}
void AltairX::execute_unit_half(uint32_t rega,uint32_t regb,uint32_t regc,uint32_t unit,uint32_t imm24)
{
    uint64_t tmp,tmp2;

	switch(unit)
	{
        case AX_EX_FPU_FADD:
            
            break;
        case AX_EX_FPU_FSUB:
            // Code pour AX_EX_FPU_FSUB
            break;
        case AX_EX_FPU_FMUL:
            // Code pour AX_EX_FPU_FMUL
            break;
        case AX_EX_FPU_FNMUL:
            // Code pour AX_EX_FPU_FNMUL
            break;
        case AX_EX_FPU_HTOF:
            // Code pour AX_EX_FPU_HTOF
            break;
        case AX_EX_FPU_FTOH:
            // Code pour AX_EX_FPU_FTOH
            break;
        case AX_EX_FPU_ITOF:
            // Code pour AX_EX_FPU_ITOF
            break;
        case AX_EX_FPU_FTOI:
            // Code pour AX_EX_FPU_FTOI
            break;
        case AX_EX_FPU_FNEG:
            // Code pour AX_EX_FPU_FNEG
            break;
        case AX_EX_FPU_FABS:
            // Code pour AX_EX_FPU_FABS
            break;
        case AX_EX_FPU_FMIN:
            // Code pour AX_EX_FPU_FMIN
            break;
        case AX_EX_FPU_FMAX:
            // Code pour AX_EX_FPU_FMAX
            break;
        case AX_EX_FPU_FMOVEI:
            // Code pour AX_EX_FPU_FMOVEI
            break;
        case AX_EX_FPU_FMOVE:
        break;
        case AX_EX_FPU_FCMP:
        break;
        case AX_EX_FPU_FCMPI:
        break;

    }
}


void AltairX::execute_unit1(uint32_t rega,uint32_t regb,uint32_t regc,uint32_t unit,uint32_t size,uint32_t opcode)
{
    uint64_t tmp,tmp2;

	switch(unit)
	{


    }
}

void AltairX::execute_unit0(uint32_t rega,uint32_t regb,uint32_t regc,uint32_t unit,uint32_t size,uint32_t opcode,uint32_t imm24)
{
    uint64_t tmp,tmp2;

	switch(unit)
	{
        case AX_EXE_EFU_FDIV:
            core->efuQ = core->freg[regb<<1] / core->freg[regc<<1];
            break;
        case AX_EXE_EFU_FATAN2:
            core->efuQ = atan2(core->freg[regb<<1],core->freg[regc<<1]);
            break;
        case AX_EXE_EFU_DDIV:
            core->efuQd = core->dreg[regb] / core->dreg[regc];
            break;
        case AX_EXE_EFU_DATAN2:
            core->efuQd = atan2(core->dreg[regb],core->dreg[regc]);
            break;
        case AX_EXE_EFU_FSQRT:
            core->efuQ = sqrt(core->freg[regb<<1]);
            break;
        case AX_EXE_EFU_FSIN:
            core->efuQ = sin(core->freg[regb<<1]);
            break;
        case AX_EXE_EFU_FATAN:
            core->efuQ = atan(core->freg[regb<<1]);
            break;
        case AX_EXE_EFU_FEXP:
            core->efuQ = exp(core->freg[regb<<1]);
            break;
        case AX_EXE_EFU_DSQRT:
            core->efuQd = sqrt(core->dreg[regb]);
            break;
        case AX_EXE_EFU_DSIN:
            core->efuQd = sin(core->dreg[regb]);
            break;
        case AX_EXE_EFU_DATAN:
            core->efuQd = atan(core->dreg[regb]);
            break;
        case AX_EXE_EFU_DEXP:
            core->efuQd = exp(core->dreg[regb]);
            break;
        case AX_EXE_EFU_FPOW:
            //core->efuQ  = powf(core->freg[regb<<1]);
            break;
        case AX_EXE_EFU_DPOW:
            //core->efuQd = pow(core->dreg[regb]);
            break;
        case AX_EXE_EFU_EMOVETO:
            core->efuQd = core->dreg[regb];
            core->efuQ = core->freg[regb];
            break;
        case AX_EXE_EFU_EMOVEFROM:
            // Code pour AX_EXE_EFU_EMOVEFROM
            break;

        // Instructions MDU(6)
        case AX_EXE_MDU_DIV:
            core->mduQ  = (core->ireg[regb] / value_immediate9(opcode,core->ireg[regc],imm24) )&sizemask[size];
            core->mduQR = (core->ireg[regb] / value_immediate9(opcode,core->ireg[regc],imm24) )&sizemask[size];
            break;
        case AX_EXE_MDU_DIVU:
            core->mduQ  = (core->ireg[regb] / value_immediate9(opcode,core->ireg[regc],imm24) )&sizemask[size];
            core->mduQR = (core->ireg[regb] / value_immediate9(opcode,core->ireg[regc],imm24) )&sizemask[size];
            break;
        case AX_EXE_MDU_MUL:
            core->mduP  = (core->ireg[regb] * value_immediate9(opcode,core->ireg[regc],imm24) )&sizemask[size];
            break;
        case AX_EXE_MDU_MULU:
            core->mduP  = (core->ireg[regb] * value_immediate9(opcode,core->ireg[regc],imm24) )&sizemask[size];
            break;
        case AX_EXE_MDU_MOVETO:
            // Code pour AX_EXE_MDU_MOVETO
            break;
        case AX_EXE_MDU_MOVEFROM:
            // Code pour AX_EXE_MDU_MOVEFROM
            break;

        // Instructions BRU(7)
        case AX_EXE_BRU_BNE:
            tmp = (opcode>>8) | (imm24<<24);

            tmp2 = core->fr&Z_MASK;

            if(tmp2 != 0)
                core->pc += tmp;
            break;
        case AX_EXE_BRU_BEQ:
            tmp = (opcode>>8) | (imm24<<24);

            tmp2 = core->fr&Z_MASK;

            if(tmp2 == 0)
                core->pc += tmp;
            break;
        case AX_EXE_BRU_BL:
            tmp = (opcode>>8) | (imm24<<24);

            tmp2 = core->fr&N_MASK;

            if(tmp2 != 0)
                core->pc += tmp;
            break;
        case AX_EXE_BRU_BLE:
            tmp = (opcode>>8) | (imm24<<24);

            tmp2 = core->fr&(Z_MASK|N_MASK);

            if(tmp2 != 0)
                core->pc += tmp;
            break;
        case AX_EXE_BRU_BG:
            tmp = (opcode>>8) | (imm24<<24);

            tmp2 = core->fr&N_MASK;

            if(tmp2 == 0)
                core->pc += tmp;
            break;
        case AX_EXE_BRU_BGE:
            tmp = (opcode>>8) | (imm24<<24);

            tmp2 = core->fr&(Z_MASK|N_MASK);

            if(tmp2 == 0)
                core->pc += tmp;
            break;
        case AX_EXE_BRU_BLS:
            tmp = (opcode>>8) | (imm24<<24);

            tmp2 = core->fr&Z_MASK;

            if(tmp2 != 0)
                core->pc += tmp;
            break;
        case AX_EXE_BRU_BLES:
            tmp = (opcode>>8) | (imm24<<24);

            tmp2 = core->fr&Z_MASK;

            if(tmp2 != 0)
                core->pc += tmp;
            break;
        case AX_EXE_BRU_BGS:
            tmp = (opcode>>8) | (imm24<<24);

            tmp2 = core->fr&Z_MASK;

            if(tmp2 != 0)
                core->pc += tmp;
            break;
        case AX_EXE_BRU_BGES:
            tmp = (opcode>>8) | (imm24<<24);

            tmp2 = core->fr&Z_MASK;

            if(tmp2 != 0)
                core->pc += tmp;
            break;
        case AX_EXE_BRU_BRA:
            tmp = (opcode>>8) | (imm24<<24);
            core->pc += tmp;
            break;
        case AX_EXE_BRU_LOOP:
            tmp = (opcode>>8) | (imm24<<24);

            if(core->lc == 0)
                core->pc += tmp;
            else
                core->lc--;
            break;
        case AX_EXE_BRU_JUMP:
            tmp = opcode>>8;
            core->pc = tmp | (imm24<<24);
            break;
        case AX_EXE_BRU_CALL:
            tmp = opcode>>8;
            core->lr = core->pc;
            core->pc = tmp | (imm24<<24);
            break;
        case AX_EXE_BRU_CALLR:
            tmp = opcode>>8;
            core->lr = core->pc;
            core->pc += tmp | (imm24<<24);
            break;
        case AX_EXE_BRU_OTHER:
            tmp = opcode>>8;

            if(tmp&0x8)
                core->lr = core->pc;

            switch(tmp&7)
            {

                case 1:
                    core->pc = core->br;
                break;

                case 2:
                    core->pc = core->lr;
                break;

                case 3:
                    core->pc = core->ir;
                break;

                case 4:
                    core->ir = core->pc;
                    core->pc = 0x80000000;
                    core->syscall = 1;
                break;

                case 5:
                    core->ir = core->pc;
                    core->pc = 0x80000002;
                    core->syscall = 2;
                break;

            }



            break;


        default:

        break;

    }
}

void AltairX::execute_unit(uint32_t rega,uint32_t regb,uint32_t regc,uint32_t unit,uint32_t size,uint32_t opcode,uint32_t tunit,uint32_t imm24)
{
    uint64_t tmp,tmp2;

	switch(unit)
	{

        case AX_EXE_ALU_MOVEIX:

		break;

        case AX_EXE_ALU_EXT:
			tmp  = (opcode>>8)&0x3F;
            tmp2 = (1<<((opcode>>14)&0x3F) )-1;
			core->ireg[rega] = (core->ireg[regb]>>tmp) & tmp2;
		break;


        case AX_EXE_ALU_INS:
			tmp  = (opcode>>8)&0x3F;
            tmp2 = (1<<((opcode>>14)&0x3F) )-1;
			core->ireg[rega] |= (core->ireg[regb]&tmp2) << tmp;
		break;


        case AX_EXE_ALU_MOVEI:
			tmp = (opcode>>8)&0x3FFFF;
			core->ireg[rega] = tmp | (imm24<<18);
		break;

        case AX_EXE_ALU_MOVEN:
			tmp = (opcode>>8)&0x3FFFF;
			core->ireg[rega] = (tmp|0xFFFFFFFFFFFC0000) ^ (imm24<<18);
		break;

        case AX_EXE_ALU_MOVEUP:
			tmp = (opcode>>8)&0x3FFFF;
			core->ireg[rega] = (tmp<<32) | ((uint64_t)imm24<<42);
		break;

        case AX_EXE_ALU_SEXT:
            tmp = core->ireg[regb];
            tmp2 = (opcode>>11)&3;
			if(tmp2 == 0)
			{
				if(tmp&0x80)
					tmp |= signmask[0];
			}
			else if(tmp2 == 1)
			{
				if(tmp&0x8000)
					tmp |= signmask[1];
			}
			else if(tmp2 == 2)
			{
				if(tmp&0x80000000)
					tmp |= signmask[2];
			}

			core->ireg[rega] = tmp&sizemask[size];
		break;


        case AX_EXE_ALU_CMP:
            tmp = core->ireg[regb] - core->ireg[regc];

            if(tmp == 0)
                core->fr |= Z_MASK;
            else
                core->fr &= 0xFFFFFFFF-Z_MASK;

            if(tmp & 0x8000'0000'0000'0000)
                core->fr |= N_MASK;
            else
                core->fr &= 0xFFFFFFFF-N_MASK;


            break;
        case AX_EXE_ALU_TEST:
            // Code pour AX_EXE_ALU_TEST
            break;
        case AX_EXE_ALU_TESTFR:
            // Code pour AX_EXE_ALU_TESTFR
            break;

        case AX_EXE_ALU_CMPBIT:
            break;
		

		//------------- ALU-B -----

		case AX_EXE_ALU_ADD:
			core->ireg[rega] = (core->ireg[regb] + value_immediate9(opcode,core->ireg[regc],imm24) )&sizemask[size];
		break;

		case AX_EXE_ALU_SUB:
			core->ireg[rega] = (core->ireg[regb] - value_immediate9(opcode,core->ireg[regc],imm24) )&sizemask[size];
		break;

		case AX_EXE_ALU_XOR:
			core->ireg[rega] = (core->ireg[regb] ^ value_immediate9(opcode,core->ireg[regc],imm24) )&sizemask[size];
		break;

		case AX_EXE_ALU_OR:
			core->ireg[rega] = (core->ireg[regb] | value_immediate9(opcode,core->ireg[regc],imm24) )&sizemask[size];
		break;

		case AX_EXE_ALU_AND:
			core->ireg[rega] = (core->ireg[regb] & value_immediate9(opcode,core->ireg[regc],imm24) )&sizemask[size];
		break;

		case AX_EXE_ALU_LSL:
			core->ireg[rega] = (core->ireg[regb] << value_immediate9(opcode,core->ireg[regc],imm24) )&sizemask[size];
		break;

		case AX_EXE_ALU_ASR:
			core->ireg[rega] = (int64_t)(core->ireg[regb] >> (int64_t)value_immediate9(opcode,core->ireg[regc],imm24) )&sizemask[size];
		break;

		case AX_EXE_ALU_LSR:
			core->ireg[rega] = (core->ireg[regb] >> value_immediate9(opcode,core->ireg[regc],imm24) )&sizemask[size];
		break;


        case AX_EXE_ALU_SE:
            tmp = value_immediate9(opcode,core->ireg[regc],imm24);
            if(core->ireg[regb] == tmp)
            {
                core->ireg[rega] = 1;
            }
            else
            {
                core->ireg[rega] = 0;
            }
        break;

        case AX_EXE_ALU_SEN:
            tmp = value_immediate9(opcode,core->ireg[regc],imm24);
            if(core->ireg[regb] != tmp)
            {
                core->ireg[rega] = 1;
            }
            else
            {
                core->ireg[rega] = 0;
            }
        break;

        case AX_EXE_ALU_SLTS:
			tmp = value_immediate9(opcode,core->ireg[regc],imm24);
            if((int64_t)core->ireg[regb] < (int64_t)tmp)
            {
                core->ireg[rega] = 1;
            }
            else
            {
                core->ireg[rega] = 0;
            }
                
		break;

		case AX_EXE_ALU_SLTU:
			tmp = value_immediate9(opcode,core->ireg[regc],imm24);
            if(core->ireg[regb] < tmp)
            {
                core->ireg[rega] = 1;
            }
            else
            {
                core->ireg[rega] = 0;
            }
		break;


        case AX_EXE_ALU_SAND:
            tmp = value_immediate9(opcode,core->ireg[regc],imm24);
            if(core->ireg[regb] & tmp)
            {
                core->ireg[rega] = 1;
            }
            else
            {
                core->ireg[rega] = 0;
            }
        break;

        case AX_EXE_ALU_CMOVE:
            tmp = value_immediate9(opcode,core->ireg[regc],imm24);
            if(tmp != 0)
                core->ireg[rega] = core->ireg[regb]&sizemask[size];
        break;

        case AX_EXE_ALU_CMOVEN:
            tmp = value_immediate9(opcode,core->ireg[regc],imm24);
            if(tmp == 0)
                core->ireg[rega] = core->ireg[regb]&sizemask[size];
        break;


        //LSU
        case AX_EXE_LSU_LD:
            tmp2 = ((opcode2>>11)&7);
            tmp = core->ireg[regb] + (core->ireg[regc]<< tmp2);
            executeLoad((void *)&core->ireg[rega],tmp,1<<size);
            break;
        case AX_EXE_LSU_ST:
            tmp2 = ((opcode2>>11)&7);
            tmp = core->ireg[regb] + (core->ireg[regc]<< tmp2);
            executeStore((void *)&core->ireg[rega],tmp,1<<size);
            break;
        case AX_EXE_LSU_FLD:
            tmp2 = ((opcode2>>11)&7);
            tmp = core->ireg[regb] + (core->ireg[regc]<< tmp2);
            executeLoad((void *)&core->dreg[rega],tmp,1<<size);
            break;
        case AX_EXE_LSU_FST:
            tmp2 = ((opcode2>>11)&7);
            tmp = core->ireg[regb] + (core->ireg[regc]<< tmp2);
            executeStore((void *)&core->dreg[rega],tmp,1<<size);
            break;

        case AX_EXE_LSU_LDI:
            tmp2 = ((opcode2>>10)&0x3FF);
            if(tmp2&0x200)
                tmp2 |= 0xFFFFFFFFFFFFFC00ull;
            tmp = core->ireg[regb] + (tmp2 ^ (imm24<<9));
            executeLoad((void *)&core->ireg[rega],tmp,1<<size);
            break;
        case AX_EXE_LSU_STI:
            tmp2 = ((opcode2>>10)&0x3FF);
            if(tmp2&0x200)
                tmp2 |= 0xFFFFFFFFFFFFFC00ull;
            tmp = core->ireg[regb] + (tmp2 ^ (imm24<<9));
            executeStore((void *)&core->ireg[rega],tmp,1<<size);
            break;
        case AX_EXE_LSU_FLDI:
            tmp2 = ((opcode2>>10)&0x3FF);
            if(tmp2&0x200)
                tmp2 |= 0xFFFFFFFFFFFFFC00ull;
            tmp = core->ireg[regb] + (tmp2 ^ (imm24<<9));
            executeLoad((void *)&core->dreg[rega],tmp,1<<size);
            break;
        case AX_EXE_LSU_FSTI:
            tmp2 = ((opcode2>>10)&0x3FF);
            if(tmp2&0x200)
                tmp2 |= 0xFFFFFFFFFFFFFC00ull;
            tmp = core->ireg[regb] + (tmp2 ^ (imm24<<9));
            executeStore((void *)&(core->dreg[rega]),tmp,1<<size);
            break;

        case AX_EXE_LSU_LDSP:
            tmp2 = ((opcode2>>10)&0xFFFF);
            tmp = core->ireg[0] + (tmp2 ^ (imm24<<16));
            executeLoad((void *)&(core->ireg[rega]),tmp,1<<size);
            break;
        case AX_EXE_LSU_STSP:
            tmp2 = ((opcode2>>10)&0xFFFF);
            tmp = core->ireg[0] + (tmp2 ^ (imm24<<16));
            executeStore((void *)&(core->ireg[rega]),tmp,1<<size);
            break;
        case AX_EXE_LSU_FLDSP:
            tmp2 = ((opcode2>>10)&0xFFFF);
            tmp = core->ireg[0] + (tmp2 ^ (imm24<<16));
            executeLoad((void *)&(core->dreg[rega]),tmp,1<<size);
            break;
        case AX_EXE_LSU_FSTSP:
            tmp2 = ((opcode2>>10)&0xFFFF);
            tmp = core->ireg[0] + (tmp2 ^ (imm24<<16));
            executeStore((void *)&(core->dreg[rega]),tmp,1<<size);
        break;

        case AX_EXE_FPU_FADD:
        case AX_EXE_FPU_FSUB:
        case AX_EXE_FPU_FMUL:
        case AX_EXE_FPU_FNMUL:
        case AX_EXE_FPU_HTOF:
        case AX_EXE_FPU_FTOH:
        case AX_EXE_FPU_ITOF:
        case AX_EXE_FPU_FTOI:
        case AX_EXE_FPU_FNEG:
        case AX_EXE_FPU_FABS:
        case AX_EXE_FPU_FMIN:
        case AX_EXE_FPU_FMAX:
        case AX_EXE_FPU_FMOVE:
        case AX_EXE_FPU_FCMP:
        case AX_EXE_FPU_FMOVEI:
        case AX_EXE_FPU_FCMPI:

            switch(size)
            {
                case 0:
                    this->execute_unit_half(rega,regb,regc,unit,imm24);
                break;

                case 1:
                    this->execute_unit_float(rega,regb,regc,unit,imm24);
                break;

                case 2:
                    this->execute_unit_double(rega,regb,regc,unit,imm24);
                break;

                case 3:
                    this->execute_unit_falu(rega,regb,regc,unit,imm24);
                break;

            }

        break;

        default:
            if(tunit == 0)
                execute_unit0(rega,regb,regc,unit,size,opcode,imm24);
            else
                execute_unit1(rega,regb,regc,unit,size,opcode);
        break;

    }
}


void AltairX::execute()
{
	uint32_t opcode1,opcode2,unit,imm24 = 0;
	uint32_t rega,regb,regc,size;

	this->core->cc++;
	this->core->ic++;

    uint32_t pc = this->core->pc&0x7FFFFFFF;

	opcode1 = this->wram[pc+0];
    opcode2 = this->wram[pc+1];
	this->opcode1 = opcode1;
	this->core->pc++;

    this->core->ireg[REG_ZERO]  = 0;
    this->core->fireg[REG_ZERO] = 0;

    if( (opcode1&1) && ( (opcode2&0xFE) == 0) )
    {
        imm24 = opcode2>>8;
    }
        

	unit = (opcode1>>1)&0x7F;
	size = (opcode1>>8)&0x3;

	rega = (opcode1>>26)&0x3F;
	regb = (opcode1>>20)&0x3F;
	regc = (opcode1>>14)&0x3F;

	this->execute_unit(rega,regb,regc,unit,size,opcode1,0,imm24);

	if( (opcode1&1) && ( (opcode2&0xFE) != 0) )
	{
		this->core->ic++;
        this->core->pc++;
		
		this->opcode2 = opcode2;
		
		unit = (opcode2>>1)&0x7F;
		size = (opcode2>>8)&0x3;

		rega = (opcode2>>26)&0x3F;
		regb = (opcode2>>20)&0x3F;
		regc = (opcode2>>14)&0x3F;

		this->execute_unit(rega,regb,regc,unit,size,opcode2,1,imm24);
	}

}