
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

#include "vm.h"
#include "execute_opti.h"

static const uint32_t ZSUClearMask  = ~(Z_MASK | S_MASK | U_MASK);

/*
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
*/

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

static uint64_t extendSign(uint32_t value, uint32_t bits)
{
    if(value > (1u << (bits-1)))
    {
        return ((0xFFFFFFFFFFFFFFull << bits) | value);
    }
    else
    {
        return value;
    }
}


static uint64_t value_immediate9(uint32_t opcode,uint64_t reg)
{
    if(opcode&0x400)
	{
		return (opcode>>11)&0x1FF;
	}
	else
	{
		return reg;
	}
}

//-----------------------
static void __attribute__((optimize("-O2"), noinline)) executeStore(Core *core,void *reg,uint64_t offset,uint32_t size)
{
    void *address = NULL;
    uint64_t max;

    if(offset&MEMORY_MAP_WRAM_BEGIN)
    {
        address = core->mmap.wram;
        max = core->mmap.nwram-1; //Max 2 Gio
    }
    else if(offset&MEMORY_MAP_VRAM_BEGIN)
    {
		address = core->mmap.vram;
		max = (core->mmap.nvram-1); //Max 1 Gio
	}
	else if(offset&MEMORY_MAP_SRAM_BEGIN)
	{
		address = core->mmap.spmram;
		max = (core->mmap.nspmram-1); //Max 512 Mio
	}
	else if(offset&MEMORY_MAP_SPM2_BEGIN)
	{
		address = core->mmap.spm2;
		max = (core->mmap.nspm2-1); //Max 256 Mio
	}
	else if(offset&MEMORY_MAP_SPMT_BEGIN)
	{
		address = core->mmap.spmt;
		max = (core->mmap.nspmt-1); //Max 128 Mio
	}
	else if(offset&MEMORY_MAP_ROM_BEGIN)
	{
		address = core->mmap.rom;
		max = (core->mmap.nrom-1); //Max 64 Mio
	}
	else if(offset&MEMORY_MAP_IO_BEGIN)
	{
		address = core->mmap.io;
		max = 0x7FFFF; //Max 512 Kio (miroir 32 Mio)
		AX_IO_Write(core,offset,reg);
	}
	else //SPM
	{
		address = core->spm;
		max = 0x7FFF; //Max 32 Kio
	}

	offset &= max;

	memcpy(address+offset, reg, size);
}


static __attribute__((optimize("-O2"), noinline))  void executeLoad(Core *core,void *reg,uint64_t offset,uint32_t size)
{
    void *address = NULL;
    uint64_t max;

    if(offset&MEMORY_MAP_WRAM_BEGIN)
    {
        address = core->mmap.wram;
        max = core->mmap.nwram-1; //Max 2 Gio
    }
    else if(offset&MEMORY_MAP_VRAM_BEGIN)
    {
		address = core->mmap.vram;
		max = (core->mmap.nvram-1); //Max 1 Gio
	}
	else if(offset&MEMORY_MAP_SRAM_BEGIN)
	{
		address = core->mmap.spmram;
		max = (core->mmap.nspmram-1); //Max 512 Mio
	}
	else if(offset&MEMORY_MAP_SPM2_BEGIN)
	{
		address = core->mmap.spm2;
		max = (core->mmap.nspm2-1); //Max 256 Mio
	}
	else if(offset&MEMORY_MAP_SPMT_BEGIN)
	{
		address = core->mmap.spmt;
		max = (core->mmap.nspmt-1); //Max 128 Mio
	}
	else if(offset&MEMORY_MAP_ROM_BEGIN)
	{
		address = core->mmap.rom;
		max = (core->mmap.nrom-1); //Max 64 Mio
	}
	else if(offset&MEMORY_MAP_IO_BEGIN)
	{
		address = core->mmap.io;
		max = 0x7FFFF; //Max 512 Kio (miroir 32 Mio)
		AX_IO_Read(core,offset,reg);
	}
	else //SPM
	{
		address = core->spm;
		max = 0x7FFF; //Max 32 Kio
	}

	offset &= max;

	memcpy(reg, address+offset, size);

}
//--------------------------------------------------------

void executeDelayedInstruction(Core *core)
{
	uint32_t imm = core->imm;
	core->delay = 0;

    switch(core->delayop)
    {
        default:
            return;

        case AX_EXE_BRU_BNE: // !=
            if((core->flags & Z_MASK))
            {
                core->pc += imm;
            }

            break;

        case AX_EXE_BRU_BEQ: // ==
            if(!(core->flags & Z_MASK))
            {
                core->pc += imm;
            }
            break;

        case AX_EXE_BRU_BL: // <
            if( (core->flags & U_MASK) && (!(core->flags & Z_MASK)) )
            {
                core->pc += imm;
            }
            break;

        case AX_EXE_BRU_BLE: // <=
            if(core->flags & U_MASK)
            {
                core->pc += imm;
            }
            break;

        case AX_EXE_BRU_BG: // >
            if(!(core->flags & U_MASK) && (!(core->flags & Z_MASK)) )
            {
                core->pc += imm;
            }
            break;

        case AX_EXE_BRU_BGE: // >=
            if(!(core->flags & U_MASK))
            {
                core->pc += imm;
            }
            break;

        case AX_EXE_BRU_BLS: // <
            if( (core->flags & S_MASK)  && (!(core->flags & Z_MASK)) )
            {
                core->pc += imm;
            }
            break;

        case AX_EXE_BRU_BLES: // <=
            if(core->flags & S_MASK)
            {
                core->pc += imm;
            }
            break;

        case AX_EXE_BRU_BGS: // >
            if( (!(core->flags & S_MASK)) && (!(core->flags & Z_MASK)) )
            {
                core->pc += imm;
            }
            break;

        case AX_EXE_BRU_BGES: // >=
            if(!(core->flags & S_MASK) || !(core->flags & Z_MASK))
            {
                core->pc += imm;
            }
            break;

        case AX_EXE_BRU_BRA:
            core->pc += imm;
            break;

        case AX_EXE_BRU_JUMP:
            core->pc = imm;
            break;

        case AX_EXE_BRU_CALL:
            core->lr = core->pc;
            core->pc = imm;
            break;

        case AX_EXE_BRU_JUMPBR:
            core->pc = core->br+imm;
            break;

        case AX_EXE_BRU_CALLBR:
            core->lr = core->pc;
            core->pc = core->br+imm;
            break;

		case AX_EXE_BRU_LOOP:

            if(core->ireg[60] != 0)
			{
				core->ireg[60]--;
				core->pc += imm;
			}
            break;

        case AX_EXE_BRU_RET:
            core->pc = core->lr;
            break;

		case AX_EXE_BRU_RETI:
            core->pc = core->ir;
            break;

		case AX_EXE_BRU_INT:
			core->syscall = 2;
			core->ir = core->pc;
			core->pc = 2;
            break;

		case AX_EXE_BRU_SYSCALL:
			core->syscall = 1;
			core->ir = core->pc;
			core->pc = 0;
            break;
    }


}

//----------------------

void __attribute__((optimize("-O2"), noinline)) AX_execute_unit1A(Core *core,uint64_t rega,uint64_t regb,uint64_t regc,uint32_t unit,uint32_t size,uint32_t opcode)
{
	uint64_t tmp,tmp2,tmp3;
	uint32_t id;


	switch(unit)
	{
		//------------- LSU-A -----
		case AX_EXE_LSU_LDL:
			tmp2 = (opcode>>10)&0xFFFF;
			tmp  = core->ireg[0] + tmp2;
			executeLoad(core,&core->ireg[rega],tmp,1u << size);
		break;

		case AX_EXE_LSU_STL:
			tmp2 = (opcode>>10)&0xFFFF;
			tmp  = core->ireg[0] + tmp2;
			executeStore(core,&core->ireg[rega],tmp,1u << size);
		break;

		case AX_EXE_LSU_LDVL:
			tmp2 = (opcode>>10)&0xFFFF;
			tmp  = core->ireg[0] + tmp2;
			executeLoad(core,&core->vreg[rega],tmp,(1+size) << 4u);
		break;

		case AX_EXE_LSU_STVL:
			tmp2 = (opcode>>10)&0xFFFF;
			tmp  = core->ireg[0] + tmp2;
			executeStore(core,&core->vreg[rega],tmp,(1+size) << 4u);
		break;

		case AX_EXE_LSU_LDL1:
			tmp2 = (opcode>>10)&0xFFFF;
			tmp  = core->ireg[1] + tmp2;
			executeLoad(core,&core->ireg[rega],tmp,1u << size);
		break;

		case AX_EXE_LSU_STL1:
			tmp2 = (opcode>>10)&0xFFFF;
			tmp  = core->ireg[1] + tmp2;
			executeStore(core,&core->ireg[rega],tmp,1u << size);
		break;

		case AX_EXE_LSU_LDVL1:
			tmp2 = (opcode>>10)&0xFFFF;
			tmp  = core->ireg[1] + tmp2;
			executeLoad(core,&core->vreg[rega],tmp,(1+size) << 4u);
		break;

		case AX_EXE_LSU_STVL1:
			tmp2 = (opcode>>10)&0xFFFF;
			tmp  = core->ireg[1] + tmp2;
			executeStore(core,&core->vreg[rega],tmp,(1+size) << 4u);
		break;


		//------------- LSU-B -----
		case AX_EXE_LSU_LD:
			tmp  = core->ireg[regb] + core->ireg[regc];
			executeLoad(core,&core->ireg[rega],tmp,1u << size);
		break;

		case AX_EXE_LSU_ST:
			tmp  = core->ireg[regb] + core->ireg[regc];
			executeStore(core,&core->ireg[rega],tmp,1u << size);
		break;

		case AX_EXE_LSU_LDV:
			tmp  = core->ireg[regb] + core->ireg[regc];
			executeLoad(core,&core->vreg[rega],tmp,(1+size) << 4u);
		break;

		case AX_EXE_LSU_STV:
			tmp  = core->ireg[regb] + core->ireg[regc];
			executeStore(core,&core->vreg[rega],tmp,(1+size) << 4u);
		break;



		case AX_EXE_LSU_LDI:
			tmp2 = (opcode>>10)&0x3FF;
			tmp  = core->ireg[regb] + extendSign(tmp2,10);
			executeLoad(core,&core->ireg[rega],tmp,1u << size);
		break;

		case AX_EXE_LSU_STI:
			tmp2 = (opcode>>10)&0x3FF;
			tmp  = core->ireg[regb] + extendSign(tmp2,10);
			executeStore(core,&core->ireg[rega],tmp,1u << size);
		break;

		case AX_EXE_LSU_LDVI:
			tmp2 = (opcode>>10)&0x3FF;
			tmp  = core->ireg[regb] + extendSign(tmp2,10);
			executeLoad(core,&core->vreg[rega],tmp,(1+size) << 4u);
		break;

		case AX_EXE_LSU_STVI:
			tmp2 = (opcode>>10)&0x3FF;
			tmp  = core->ireg[regb] + extendSign(tmp2,10);
			executeStore(core,&core->vreg[rega],tmp,(1+size) << 4u);
		break;


		//------------- EFU -----
		case AX_EXE_EFU_FDIV:
			id   = (opcode>>10)&0x3;

			tmp  = (rega<<2);
			tmp2 = (regb<<2) + (size);
			tmp3 = (regc<<2) + (id);

			core->vreg[tmp] = core->vreg[tmp2]/core->vreg[tmp3];
		break;

		case AX_EXE_EFU_FSQRT:
			tmp  = (rega<<2);
			tmp2 = (regb<<2);

			core->vreg[tmp] = sqrtf(core->vreg[tmp2+size]);

		break;

		case AX_EXE_EFU_FSUM:
			tmp  = (rega<<2);
			tmp2 = (regb<<2);

			core->vreg[tmp] = core->vreg[tmp2+0] + core->vreg[tmp2+1] + core->vreg[tmp2+2] + core->vreg[tmp2+3];

		break;

		case AX_EXE_EFU_FIPR:
			tmp  = (rega<<2);
			tmp2 = (regb<<2);
			tmp3 = (regc<<2);

			core->vreg[tmp] = 0;

			for(int i;i < size;i++)
				core->vreg[tmp+i] += (core->vreg[tmp2+i]*core->vreg[tmp3+i]);

		break;

		case AX_EXE_EFU_FSIN:
			tmp  = (rega<<2);
			tmp2 = (regb<<2);

			core->vreg[tmp] = sin(core->vreg[tmp2+size]);

		break;



		case AX_EXE_EFU_FATAN:
			tmp  = (rega<<2);
			tmp2 = (regb<<2);

			core->vreg[tmp] = atan(core->vreg[tmp2+size]);

		break;

		case AX_EXE_EFU_FATAN2:
			id   = (opcode>>10)&0x3;
			tmp  = (rega<<2);
			tmp2 = (regb<<2);
			tmp3 = (regc<<2);

			core->vreg[tmp] = atan2(core->vreg[tmp2+size],core->vreg[tmp3+id]);

		break;

		case AX_EXE_EFU_FEXP:
			tmp  = (rega<<2);
			tmp2 = (regb<<2);

			core->vreg[tmp] = expf(core->vreg[tmp2+size]);

		break;


		case AX_EXE_EFU_DDIV:
			tmp  = (rega<<1);
			tmp2 = (regb<<1);
			tmp3 = (regc<<1);

			core->dreg[tmp] = core->dreg[tmp2]/core->dreg[tmp3];

		break;

		case AX_EXE_EFU_DSQRT:
			tmp  = (rega<<1);
			tmp2 = (regb<<1);

			core->dreg[tmp] = sqrt(core->dreg[tmp2]);

		break;

		case AX_EXE_EFU_DSIN:
			tmp  = (rega<<1);
			tmp2 = (regb<<1);

			core->dreg[tmp] = sin(core->dreg[tmp2]);

		break;

		//------------- FPU-D -----
		case AX_EXE_FPU_DMOVEI:
			tmp  = (rega<<1);
			tmp2 = (opcode>>10)&0xFFFF;

			core->dreg[tmp] = HalftoFloat(tmp2);

		break;


		case AX_EXE_FPU_DADD:
			tmp  = (rega<<1);
			tmp2 = (regb<<1);
			tmp3 = (regc<<1);

			core->dreg[tmp] = core->dreg[tmp2] + core->dreg[tmp3];
		break;


		case AX_EXE_FPU_DSUB:
			tmp  = (rega<<1);
			tmp2 = (regb<<1);
			tmp3 = (regc<<1);

			core->dreg[tmp] = core->dreg[tmp2] - core->dreg[tmp3];
		break;


		case AX_EXE_FPU_DMUL:
			tmp  = (rega<<1);
			tmp2 = (regb<<1);
			tmp3 = (regc<<1);

			core->dreg[tmp] = core->dreg[tmp2] * core->dreg[tmp3];

		break;


		case AX_EXE_FPU_DABS:
			tmp  = (rega<<1);
			tmp2 = (regb<<1);

			if(core->dreg[tmp2] < 0.0)
				core->dreg[tmp] = (core->dreg[tmp2]);
		break;

		case AX_EXE_FPU_DNEG:
			tmp  = (rega<<1);
			tmp2 = (regb<<1);

			core->dreg[tmp] = -(core->dreg[tmp2]);

		break;

		case AX_EXE_FPU_DMIN:
			tmp  = (rega<<1);
			tmp2 = (regb<<1);
			tmp3 = (regc<<1);

			core->dreg[tmp] = fmin(core->dreg[tmp2],core->dreg[tmp3]);

		break;

		case AX_EXE_FPU_DMAX:
			tmp  = (rega<<1);
			tmp2 = (regb<<1);
			tmp3 = (regc<<1);

			core->dreg[tmp] = fmax(core->dreg[tmp2],core->dreg[tmp3]);

		break;


		case AX_EXE_FPU_DTOF:
			tmp  = (rega<<1);
			tmp2 = (regb<<2);

			core->dreg[tmp] = (core->vreg[tmp2]);
		break;

		case AX_EXE_FPU_FTOD:
			tmp  = (rega<<2);
			tmp2 = (regb<<1);

			core->vreg[tmp] = (core->dreg[tmp2]);
		break;

		default:
            return;

	}

}

void AX_execute_unit1(Core *core,uint64_t rega,uint64_t regb,uint64_t regc,uint32_t unit,uint32_t size,uint32_t opcode)
{
	uint64_t tmp,tmp2;

	switch(unit)
	{
		//------------- ALU-A -----

		case AX_EXE_ALU_NOP:

		break;

		case AX_EXE_ALU_SEXT:
			if(size == 0)
			{
				if(core->ireg[regb]&0x80)
					core->ireg[regb] |= signmask[0];
			}
			else if(size == 1)
			{
				if(core->ireg[regb]&0x8000)
					core->ireg[regb] |= signmask[1];
			}
			else if(size == 2)
			{
				if(core->ireg[regb]&0x80000000)
					core->ireg[regb] |= signmask[2];
			}

			core->ireg[rega] = core->ireg[regb];
		break;

		case AX_EXE_ALU_BOOL:
			tmp = core->ireg[regb] & sizemask[size];
			core->ireg[rega] = (tmp != 0);
		break;

		case AX_EXE_ALU_SMOVE:
			tmp = (opcode>>10)&0xFFFF;
			core->ireg[rega] |= tmp<<(size*16);
		break;

		case AX_EXE_ALU_SLTS:
			tmp  = core->ireg[rega] & sizemask[size];
			tmp2 = core->ireg[regb] & sizemask[size];

			if(size == 0)
			{
				tmp  = extendSign(tmp,8);
				tmp2 = extendSign(tmp2,8);
			}
			else if(size == 1)
			{
				tmp  = extendSign(tmp,16);
				tmp2 = extendSign(tmp2,16);
			}
			else if(size == 2)
			{
				tmp  = extendSign(tmp,32);
				tmp2 = extendSign(tmp2,32);
			}

			core->flags &= ZSUClearMask;
			core->flags |= (tmp < tmp2) << 2u;
		break;

		case AX_EXE_ALU_SLTU:
			tmp  = core->ireg[rega] & sizemask[size];
			tmp2 = core->ireg[regb] & sizemask[size];

			core->flags &= ZSUClearMask;
			core->flags |= (tmp < tmp2) << 2u;
		break;

		case AX_EXE_ALU_SLTSI:
			tmp  = core->ireg[rega] & sizemask[size];
			tmp2 = (opcode>>10)&0x3FF;
			tmp2 = extendSign(tmp2,10);

			if(size == 0)
			{
				tmp  = extendSign(tmp,8);

			}
			else if(size == 1)
			{
				tmp  = extendSign(tmp,16);

			}
			else if(size == 2)
			{
				tmp  = extendSign(tmp,32);

			}

			core->flags &= ZSUClearMask;
			core->flags |= (tmp < tmp2) << 2u;
		break;

		case AX_EXE_ALU_SLTUI:
			tmp  = core->ireg[rega] & sizemask[size];
			tmp2 = (opcode>>10)&0x3FF;
			tmp2 = extendSign(tmp2,10);

			core->flags &= ZSUClearMask;
			core->flags |= (tmp < tmp2) << 2u;
		break;

		case AX_EXE_ALU_MOVEN:
			tmp = (opcode>>8)&0x3FFFF;
			core->ireg[rega] = tmp|0xFFFFFFFFFFFC0000;
		break;

		case AX_EXE_ALU_MOVEU:
			tmp = (opcode>>8)&0x3FFFF;
			core->ireg[rega] = tmp;
		break;

		//------------- ALU-B -----

		case AX_EXE_ALU_ADD:
			core->ireg[rega] = (core->ireg[regb] + value_immediate9(opcode,core->ireg[regc]) )&sizemask[size];
		break;

		case AX_EXE_ALU_SUB:
			core->ireg[rega] = (core->ireg[regb] - value_immediate9(opcode,core->ireg[regc]) )&sizemask[size];
		break;

		case AX_EXE_ALU_XOR:
			core->ireg[rega] = (core->ireg[regb] ^ value_immediate9(opcode,core->ireg[regc]) )&sizemask[size];
		break;

		case AX_EXE_ALU_OR:
			core->ireg[rega] = (core->ireg[regb] | value_immediate9(opcode,core->ireg[regc]) )&sizemask[size];
		break;

		case AX_EXE_ALU_AND:
			core->ireg[rega] = (core->ireg[regb] & value_immediate9(opcode,core->ireg[regc]) )&sizemask[size];
		break;

		case AX_EXE_ALU_LSL:
			core->ireg[rega] = (core->ireg[regb] << value_immediate9(opcode,core->ireg[regc]) )&sizemask[size];
		break;

		case AX_EXE_ALU_ASR:
			core->ireg[rega] = (int64_t)(core->ireg[regb] >> (int64_t)value_immediate9(opcode,core->ireg[regc]) )&sizemask[size];
		break;

		case AX_EXE_ALU_LSR:
			core->ireg[rega] = (core->ireg[regb] >> value_immediate9(opcode,core->ireg[regc]) )&sizemask[size];
		break;


		default:
            return;

	}

}

void __attribute__((optimize("-O2"), noinline)) AX_execute_unit0A(Core *core,uint64_t rega,uint64_t regb,uint64_t regc,uint32_t unit,uint32_t size,uint32_t opcode)
{
	uint64_t tmp,tmp2,tmp3;
	uint32_t id;
	float ftmp;
	double dtmp;

	switch(unit)
	{
		case AX_EXE_ALU_MOVEINS:
			core->ireg[rega] = core->instruction;
		break;

		case AX_EXE_ALU_MOVECYCLE:
			core->ireg[rega] = core->cycle;
		break;

		case AX_EXE_ALU_MOVERI:
			tmp = regb&3;

			if(tmp == 0)
				core->ireg[rega] = core->lr;
			else if(tmp == 1)
				core->ireg[rega] = core->br;
			else if(tmp == 2)
				core->ireg[rega] = core->flags;
		break;

		case AX_EXE_ALU_MOVEIR:
			tmp = regb&3;

			if(tmp == 0)
				core->lr = core->ireg[rega];
			else if(tmp == 1)
				core->br = core->ireg[rega];
			else if(tmp == 2)
				core->flags = core->ireg[rega];
		break;


		//------------- LSU-A -----
		case AX_EXE_LSU_LDL:
			tmp2 = (opcode>>10)&0xFFFF;
			tmp  = core->ireg[0] + tmp2;
			executeLoad(core,&core->ireg[rega],tmp,1u << size);
		break;

		case AX_EXE_LSU_STL:
			tmp2 = (opcode>>10)&0xFFFF;
			tmp  = core->ireg[0] + tmp2;
			executeStore(core,&core->ireg[rega],tmp,1u << size);
		break;

		case AX_EXE_LSU_LDVL:
			tmp2 = (opcode>>10)&0xFFFF;
			tmp  = core->ireg[0] + tmp2;
			executeLoad(core,&core->vreg[rega],tmp,(1+size) << 4u);
		break;

		case AX_EXE_LSU_STVL:
			tmp2 = (opcode>>10)&0xFFFF;
			tmp  = core->ireg[0] + tmp2;
			executeStore(core,&core->vreg[rega],tmp,(1+size) << 4u);
		break;

		case AX_EXE_LSU_LDL1:
			tmp2 = (opcode>>10)&0xFFFF;
			tmp  = core->ireg[1] + tmp2;
			executeLoad(core,&core->ireg[rega],tmp,1u << size);
		break;

		case AX_EXE_LSU_STL1:
			tmp2 = (opcode>>10)&0xFFFF;
			tmp  = core->ireg[1] + tmp2;
			executeStore(core,&core->ireg[rega],tmp,1u << size);
		break;

		case AX_EXE_LSU_LDVL1:
			tmp2 = (opcode>>10)&0xFFFF;
			tmp  = core->ireg[1] + tmp2;
			executeLoad(core,&core->vreg[rega],tmp,(1+size) << 4u);
		break;

		case AX_EXE_LSU_STVL1:
			tmp2 = (opcode>>10)&0xFFFF;
			tmp  = core->ireg[1] + tmp2;
			executeStore(core,&core->vreg[rega],tmp,(1+size) << 4u);
		break;


		//------------- LSU-B -----
		case AX_EXE_LSU_LD:
			tmp  = core->ireg[regb] + core->ireg[regc];
			executeLoad(core,&core->ireg[rega],tmp,1u << size);
		break;

		case AX_EXE_LSU_ST:
			tmp  = core->ireg[regb] + core->ireg[regc];
			executeStore(core,&core->ireg[rega],tmp,1u << size);
		break;

		case AX_EXE_LSU_LDV:
			tmp  = core->ireg[regb] + core->ireg[regc];
			executeLoad(core,&core->vreg[rega],tmp,(1+size) << 4u);
		break;

		case AX_EXE_LSU_STV:
			tmp  = core->ireg[regb] + core->ireg[regc];
			executeStore(core,&core->vreg[rega],tmp,(1+size) << 4u);
		break;

		case AX_EXE_LSU_LDI:
			tmp2 = (opcode>>10)&0x3FF;
			tmp  = core->ireg[regb] + extendSign(tmp2,10);
			executeLoad(core,&core->ireg[rega],tmp,1u << size);
		break;

		case AX_EXE_LSU_STI:
			tmp2 = (opcode>>10)&0x3FF;
			tmp  = core->ireg[regb] + extendSign(tmp2,10);
			executeStore(core,&core->ireg[rega],tmp,1u << size);
		break;

		case AX_EXE_LSU_LDVI:
			tmp2 = (opcode>>10)&0x3FF;
			tmp  = core->ireg[regb] + extendSign(tmp2,10);
			executeLoad(core,&core->vreg[rega],tmp,(1+size) << 4u);
		break;

		case AX_EXE_LSU_STVI:
			tmp2 = (opcode>>10)&0x3FF;
			tmp  = core->ireg[regb] + extendSign(tmp2,10);
			executeStore(core,&core->vreg[rega],tmp,(1+size) << 4u);
		break;


		//------------- VFPU -----

		case AX_EXE_CMP_FCMP:
			id   = (opcode>>10)&0x3;

			tmp  = (rega<<2) + (size);
			tmp2 = (regb<<2) + (id);
			ftmp = core->vreg[tmp] - core->vreg[tmp2];

			core->flags &= ZSUClearMask;
			core->flags |= (ftmp != 0.0f) << 0u;
			core->flags |= (ftmp <  0.0f) << 1u;
			core->flags |= (ftmp <  0.0f) << 2u;

		break;

		case AX_EXE_CMP_FCMPI:
			id   = (opcode>>10)&0x3;

			tmp  = (rega<<2) + (size);
			tmp2 = (opcode>>10)&0xFFFF;
			ftmp = core->vreg[tmp] - HalftoFloat(tmp2);

			core->flags &= ZSUClearMask;
			core->flags |= (ftmp != 0.0f) << 0u;
			core->flags |= (ftmp <  0.0f) << 1u;
			core->flags |= (ftmp <  0.0f) << 2u;

		break;

		case AX_EXE_CMP_DCMP:

			tmp  = (rega<<1);
			tmp2 = (regb<<1);
			ftmp = core->dreg[tmp] - core->dreg[tmp2];

			core->flags &= ZSUClearMask;
			core->flags |= (dtmp != 0.0) << 0u;
			core->flags |= (dtmp <  0.0) << 1u;
			core->flags |= (dtmp <  0.0) << 2u;

		break;

		case AX_EXE_CMP_DCMPI:

			tmp  = (rega<<1);
			tmp2 = (opcode>>10)&0xFFFF;
			dtmp = core->vreg[tmp] - (double)HalftoFloat(tmp2);

			core->flags &= ZSUClearMask;
			core->flags |= (dtmp != 0.0) << 0u;
			core->flags |= (dtmp <  0.0) << 1u;
			core->flags |= (dtmp <  0.0) << 2u;

		break;

		case AX_EXE_VFPU_FADD:
			id   = (opcode>>10)&0x3;

			tmp  = (regb<<2) + (size);
			tmp2 = (regc<<2) + (id);

			core->vreg[rega<<2] = core->vreg[tmp] + core->vreg[tmp2];
		break;

		case AX_EXE_VFPU_FSUB:
			id   = (opcode>>10)&0x3;

			tmp  = (regb<<2) + (size);
			tmp2 = (regc<<2) + (id);

			core->vreg[rega<<2] = core->vreg[tmp] - core->vreg[tmp2];
		break;

		case AX_EXE_VFPU_FMUL:
			id   = (opcode>>10)&0x3;

			tmp  = (regb<<2) + (size);
			tmp2 = (regc<<2) + (id);

			core->vreg[rega<<2] = core->vreg[tmp] * core->vreg[tmp2];
		break;

		case AX_EXE_VFPU_FMULADD:
			id   = (opcode>>10)&0x3;

			tmp  = (regb<<2) + (size);
			tmp2 = (regc<<2) + (id);

			core->vreg[rega<<2] *= core->vreg[tmp] + core->vreg[tmp2];
		break;


		case AX_EXE_VFPU_FMULSUB:
			id   = (opcode>>10)&0x3;

			tmp  = (regb<<2) + (size);
			tmp2 = (regc<<2) + (id);

			core->vreg[rega<<2] *= core->vreg[tmp] - core->vreg[tmp2];
		break;


		//----

		case AX_EXE_VFPU_VFADD:
			tmp  = (regb<<2);
			tmp2 = (regc<<2);

			for(uint32_t i = 0;i < size+1;i++)
				core->vreg[(rega<<2)+i] = core->vreg[tmp+i] + core->vreg[tmp2+i];
		break;

		case AX_EXE_VFPU_VFSUB:
			tmp  = (regb<<2);
			tmp2 = (regc<<2);

			for(uint32_t i = 0;i < size+1;i++)
				core->vreg[(rega<<2)+i] = core->vreg[tmp+i] - core->vreg[tmp2+i];
		break;

		case AX_EXE_VFPU_VFMUL:
			tmp  = (regb<<2);
			tmp2 = (regc<<2);

			for(uint32_t i = 0;i < size+1;i++)
				core->vreg[(rega<<2)+i] = core->vreg[tmp+i] * core->vreg[tmp2+i];
		break;

		case AX_EXE_VFPU_VFMULADD:
			tmp  = (regb<<2);
			tmp2 = (regc<<2);

			for(uint32_t i = 0;i < size+1;i++)
				core->vreg[(rega<<2)+i] *= core->vreg[tmp+i] + core->vreg[tmp2+i];
		break;


		case AX_EXE_VFPU_VFMULSUB:
			tmp  = (regb<<2);
			tmp2 = (regc<<2);

			for(uint32_t i = 0;i < size+1;i++)
				core->vreg[(rega<<2)+i] *= core->vreg[tmp+i] - core->vreg[tmp2+i];
		break;

		//----

		case AX_EXE_VFPU_VFADDS:
			tmp  = (rega<<2);
			tmp2 = (regb<<2);

			id   = (opcode>>10)&0x3;
			ftmp = core->vreg[(regc<<2)+id];

			for(uint32_t i = 0;i < size+1;i++)
				core->vreg[tmp+i] = core->vreg[tmp2+i] + ftmp;
		break;

		case AX_EXE_VFPU_VFSUBS:
			tmp  = (rega<<2);
			tmp2 = (regb<<2);

			id   = (opcode>>10)&0x3;
			ftmp = core->vreg[(regc<<2)+id];

			for(uint32_t i = 0;i < size+1;i++)
				core->vreg[tmp+i] = core->vreg[tmp2+i] - ftmp;
		break;

		case AX_EXE_VFPU_VFMULS:
			tmp  = (rega<<2);
			tmp2 = (regb<<2);

			id   = (opcode>>10)&0x3;
			ftmp = core->vreg[(regc<<2)+id];

			for(uint32_t i = 0;i < size+1;i++)
				core->vreg[tmp+i] = core->vreg[tmp2+i] * ftmp;
		break;

		case AX_EXE_VFPU_VFMULADDS:
			tmp  = (rega<<2);
			tmp2 = (regb<<2);

			id   = (opcode>>10)&0x3;
			ftmp = core->vreg[(regc<<2)+id];

			for(uint32_t i = 0;i < size+1;i++)
				core->vreg[tmp+i] *= core->vreg[tmp2+i] + ftmp;
		break;


		case AX_EXE_VFPU_VFMULSUBS:
			tmp  = (rega<<2);
			tmp2 = (regb<<2);

			id   = (opcode>>10)&0x3;
			ftmp = core->vreg[(regc<<2)+id];

			for(uint32_t i = 0;i < size+1;i++)
				core->vreg[tmp+i] *= core->vreg[tmp2+i] - ftmp;
		break;


		case AX_EXE_VFPU_VFSHUFFLE:

			tmp  = (rega<<2);
			tmp2 = (regb<<2);

			tmp3 = (opcode>>10)&0x3;
			core->vreg[tmp+tmp3] = core->vreg[tmp2+0];

			tmp3 = (opcode>>12)&0x3;
			core->vreg[tmp+tmp3] = core->vreg[tmp2+1];

			tmp3 = (opcode>>14)&0x3;
			core->vreg[tmp+tmp3] = core->vreg[tmp2+2];

			tmp3 = (opcode>>16)&0x3;
			core->vreg[tmp+tmp3] = core->vreg[tmp2+3];

		break;
		//-------------



		case AX_EXE_VFPU_FMOVE:
			id   = (opcode>>10)&0x3;

			tmp  = (rega<<2) + (size);
			tmp2 = (regb<<2) + (id);

			core->vreg[tmp] = core->vreg[tmp2];
		break;

		case AX_EXE_VFPU_VFMOVE:
			tmp  = (regb<<2);

			for(uint32_t i = 0;i < size+1;i++)
				core->vreg[(rega<<2)+i] = core->vreg[tmp+i];
		break;

		case AX_EXE_VFPU_FMOVEI:
			tmp  = (rega<<2) + (size);
			tmp2 = (opcode>>10)&0xFFFF;
			ftmp = core->vreg[tmp] - HalftoFloat(tmp2);

			core->vreg[tmp] = ftmp;
		break;

		case AX_EXE_VFPU_VFMOVEI:
			tmp  = (rega<<2);
			tmp2 = (opcode>>10)&0xFFFF;
			ftmp = core->vreg[tmp] - HalftoFloat(tmp2);

			for(uint32_t i = 0;i < size+1;i++)
				core->vreg[tmp+i] = ftmp;
		break;

		case AX_EXE_VFPU_FNEG:
			id   = (opcode>>10)&0x3;

			tmp  = (rega<<2) + (size);
			tmp2 = (regb<<2) + (id);

			core->vreg[tmp] = -core->vreg[tmp2];
		break;

		case AX_EXE_VFPU_FABS:
			id   = (opcode>>10)&0x3;

			tmp  = (rega<<2) + (size);
			tmp2 = (regb<<2) + (id);

			if(core->vreg[tmp2] < 0.0f)
				core->vreg[tmp] = -core->vreg[tmp2];
		break;

		case AX_EXE_VFPU_VFNEG:
			tmp  = (rega<<2);
			tmp2 = (regb<<2);

			for(uint32_t i = 0;i < size+1;i++)
				core->vreg[tmp+i] = -core->vreg[tmp2+i];
		break;

		case AX_EXE_VFPU_VFABS:
			tmp  = (rega<<2);
			tmp2 = (regb<<2);

			for(uint32_t i = 0;i < size+1;i++)
			{
				if(core->vreg[tmp2+i] < 0.0f)
					core->vreg[tmp+i] = -core->vreg[tmp2+i];
			}
		break;


		case AX_EXE_VFPU_VFTOI:
			tmp  = (rega<<2);
			tmp2 = (regb<<2);

			for(uint32_t i = 0;i < size+1;i++)
				core->vreg[tmp+i] = core->vireg[tmp2+i];

		break;

		case AX_EXE_VFPU_VITOF:
			tmp  = (rega<<2);
			tmp2 = (regb<<2);

			for(uint32_t i = 0;i < size+1;i++)
				core->vireg[tmp+i] = core->vreg[tmp2+i];
		break;


		case AX_EXE_VFPU_FMAX:
			id   = (opcode>>10)&0x3;

			tmp  = (rega<<2) + (size);
			tmp2 = (regb<<2) + (id);

			if(core->vreg[tmp] < core->vreg[tmp2])
				core->vreg[tmp] = core->vreg[tmp2];

		break;

		case AX_EXE_VFPU_FMIN:
			id   = (opcode>>10)&0x3;

			tmp  = (rega<<2) + (size);
			tmp2 = (regb<<2) + (id);

			if(core->vreg[tmp] > core->vreg[tmp2])
				core->vreg[tmp] = core->vreg[tmp2];
		break;

		case AX_EXE_OTHER_ENDP:
			core->error = AX_END_OF_CODE;
		break;

		case AX_EXE_BRU_RET:
		case AX_EXE_BRU_RETI:
		case AX_EXE_BRU_SYSCALL:
		case AX_EXE_BRU_INT:
			core->delayop = unit;
			core->delay = 1;
		break;

		default:
            return;

	}

}

void AX_execute_unit0(Core *core,uint64_t rega,uint64_t regb,uint64_t regc,uint32_t unit,uint32_t size,uint32_t opcode)
{
	uint64_t tmp,tmp2;

	switch(unit)
	{

		//------------- ALU-A -----
		case AX_EXE_ALU_NOP:

		break;

		case AX_EXE_ALU_SEXT:
			if(size == 0)
			{
				if(core->ireg[regb]&0x80)
					core->ireg[regb] |= signmask[0];
			}
			else if(size == 1)
			{
				if(core->ireg[regb]&0x8000)
					core->ireg[regb] |= signmask[1];
			}
			else if(size == 2)
			{
				if(core->ireg[regb]&0x80000000)
					core->ireg[regb] |= signmask[2];
			}

			core->ireg[rega] = core->ireg[regb];
		break;

		case AX_EXE_ALU_BOOL:
			tmp = core->ireg[regb] & sizemask[size];
			core->ireg[rega] = (tmp != 0);
		break;

		case AX_EXE_ALU_SMOVE:
			tmp = (opcode>>10)&0xFFFF;
			core->ireg[rega] |= tmp<<(size*16);
		break;

		case AX_EXE_ALU_SLTS:
			tmp  = core->ireg[rega] & sizemask[size];
			tmp2 = core->ireg[regb] & sizemask[size];

			if(size == 0)
			{
				tmp  = extendSign(tmp,8);
				tmp2 = extendSign(tmp2,8);
			}
			else if(size == 1)
			{
				tmp  = extendSign(tmp,16);
				tmp2 = extendSign(tmp2,16);
			}
			else if(size == 2)
			{
				tmp  = extendSign(tmp,32);
				tmp2 = extendSign(tmp2,32);
			}

			core->flags &= ZSUClearMask;
			core->flags |= (tmp < tmp2) << 2u;
		break;

		case AX_EXE_ALU_SLTU:
			tmp  = core->ireg[rega] & sizemask[size];
			tmp2 = core->ireg[regb] & sizemask[size];

			core->flags &= ZSUClearMask;
			core->flags |= (tmp < tmp2) << 2u;
		break;

		case AX_EXE_ALU_SLTSI:
			tmp  = core->ireg[rega] & sizemask[size];
			tmp2 = (opcode>>10)&0x3FF;
			tmp2 = extendSign(tmp2,10);

			if(size == 0)
			{
				tmp  = extendSign(tmp,8);

			}
			else if(size == 1)
			{
				tmp  = extendSign(tmp,16);

			}
			else if(size == 2)
			{
				tmp  = extendSign(tmp,32);

			}

			core->flags &= ZSUClearMask;
			core->flags |= (tmp < tmp2) << 2u;
		break;

		case AX_EXE_ALU_SLTUI:
			tmp  = core->ireg[rega] & sizemask[size];
			tmp2 = (opcode>>10)&0x3FF;
			tmp2 = extendSign(tmp2,10);

			core->flags &= ZSUClearMask;
			core->flags |= (tmp < tmp2) << 2u;
		break;

		case AX_EXE_ALU_MOVEN:
			tmp = (opcode>>8)&0x3FFFF;
			core->ireg[rega] = tmp|0xFFFFFFFFFFFC0000;
		break;

		case AX_EXE_ALU_MOVEU:
			tmp = (opcode>>8)&0x3FFFF;
			core->ireg[rega] = tmp;
		break;

		//------------- ALU-B -----

		case AX_EXE_ALU_ADD:
			core->ireg[rega] = (core->ireg[regb] + value_immediate9(opcode,core->ireg[regc]) )&sizemask[size];
		break;

		case AX_EXE_ALU_SUB:
			core->ireg[rega] = (core->ireg[regb] - value_immediate9(opcode,core->ireg[regc]) )&sizemask[size];
		break;

		case AX_EXE_ALU_XOR:
			core->ireg[rega] = (core->ireg[regb] ^ value_immediate9(opcode,core->ireg[regc]) )&sizemask[size];
		break;

		case AX_EXE_ALU_OR:
			core->ireg[rega] = (core->ireg[regb] | value_immediate9(opcode,core->ireg[regc]) )&sizemask[size];
		break;

		case AX_EXE_ALU_AND:
			core->ireg[rega] = (core->ireg[regb] & value_immediate9(opcode,core->ireg[regc]) )&sizemask[size];
		break;

		case AX_EXE_ALU_LSL:
			core->ireg[rega] = (core->ireg[regb] << value_immediate9(opcode,core->ireg[regc]) )&sizemask[size];
		break;

		case AX_EXE_ALU_ASR:
			core->ireg[rega] = (int64_t)(core->ireg[regb] >> (int64_t)value_immediate9(opcode,core->ireg[regc]) )&sizemask[size];
		break;

		case AX_EXE_ALU_LSR:
			core->ireg[rega] = (core->ireg[regb] >> value_immediate9(opcode,core->ireg[regc]) )&sizemask[size];
		break;


		case AX_EXE_ALU_MUL:
			core->ireg[rega] = core->ireg[regb] * core->ireg[regc];
		break;

		case AX_EXE_ALU_DIVS:
			core->ireg[rega] = core->ireg[regb] / core->ireg[regc];
		break;

		case AX_EXE_ALU_DIVU:
			core->ireg[rega] = core->ireg[regb] / core->ireg[regc];
		break;

		case AX_EXE_ALU_REMS:
			core->ireg[rega] = core->ireg[regb] % core->ireg[regc];
		break;

		case AX_EXE_ALU_REMU:
			core->ireg[rega] = core->ireg[regb] % core->ireg[regc];
		break;

		//------------- CMP -----
		case AX_EXE_CMP_CMP:
			tmp  = core->ireg[rega] & sizemask[size];
			tmp2 = core->ireg[rega] & sizemask[size];

			core->flags &= ZSUClearMask;
			core->flags |= (tmp != tmp2) << 0u;
			core->flags |= ((int64_t)tmp < (int64_t)tmp2) << 1u;
			core->flags |= (tmp < tmp2) << 2u;
		break;

		case AX_EXE_CMP_CMPN:
			tmp  = core->ireg[rega] & sizemask[size];
			tmp2 = ( (opcode>>8)&0x3FFFF)|0xFFFFFFFFFFFC0000;

			core->flags &= ZSUClearMask;
			core->flags |= (tmp != tmp2) << 0u;
			core->flags |= ((int64_t)tmp < (int64_t)tmp2) << 1u;
			core->flags |= (tmp < tmp2) << 2u;
		break;

		case AX_EXE_CMP_CMPU:
			tmp  = core->ireg[rega] & sizemask[size];
			tmp2 = ( (opcode>>8)&0x3FFFF);

			core->flags &= ZSUClearMask;
			core->flags |= (tmp != tmp2) << 0u;
			core->flags |= ((int64_t)tmp < (int64_t)tmp2) << 1u;
			core->flags |= (tmp < tmp2) << 2u;
		break;

		case AX_EXE_CMP_CMPFR:
			tmp2 = (opcode>>10)&0xFFFF;
			core->flags &= ZSUClearMask;
			core->flags |= (core->flags & tmp) << 0u;
		break;

		//------------- BRU -----

		case AX_EXE_BRU_BEQ:
		case AX_EXE_BRU_BNE:
		case AX_EXE_BRU_BLE:
		case AX_EXE_BRU_BGE:
		case AX_EXE_BRU_BL:
		case AX_EXE_BRU_BG:
		case AX_EXE_BRU_BLES:
		case AX_EXE_BRU_BGES:
		case AX_EXE_BRU_BLS:
		case AX_EXE_BRU_BGS:
		case AX_EXE_BRU_BRA:
		case AX_EXE_BRU_LOOP:

			core->delayop = unit;
			core->delay = 1;

			tmp = (opcode>>10)&0x3FFFFF;
			core->imm = extendSign(tmp,22);
		break;

		case AX_EXE_BRU_JUMPBR:
		case AX_EXE_BRU_CALLBR:
		case AX_EXE_BRU_JUMP:
		case AX_EXE_BRU_CALL:
			core->delayop = unit;
			core->delay = 1;

			core->imm = (opcode>>10)&0xFFFFFF;

		break;







		//-----

		default:
			AX_execute_unit0A(core,rega,regb,regc,unit,size,opcode);
            return;

	}

}

//----------------

void AX_decode_execute(Core *core)
{
	uint32_t opcode,unit;
	uint64_t rega,regb,regc,size;

	core->cycle++;
	core->instruction++;

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

}
