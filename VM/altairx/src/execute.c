#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

#include "vm.h"

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



static void executeLS(Core *core,void *reg,uint64_t offset,uint32_t size,uint32_t store)
{
    void *address = NULL;
    uint64_t max = 0;


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
		max = 0x7FFFFF; //Max 8 Mio (miroir 32 Mio)
	}
	else //SPM
	{
		address = core->spm;
		max = 0x7FFF; //Max 32 Kio
	}

	offset &= max;
	/*
    if( (offset+size) > max)
    {
        exit(-1);
    }*/

    if(store)
    {
		memcpy(address+offset, reg, size);
    }
    else
    {
        memcpy(reg, address+offset, size);
    }
}

static const uint32_t ZSUClearMask  = ~(Z_MASK | S_MASK | U_MASK);


void executeInstruction(Core *core,Execute *operations)
{
    //Mask to trunc results base on size (8 bits, 16 bits, 32 bits or 64 bits)
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

	uint64_t* const ireg = core->ireg;
    float*    const freg = core->vreg;
    double*   const dreg = core->dreg;
    int32_t*  const vireg = core->vireg;

    const uint32_t unit1 = operations->unit1;
    const uint32_t unit2 = operations->unit2;
    uint32_t size  = operations->size;
    const uint32_t id  = operations->id;

	uint64_t opA   = operations->opA;
	uint64_t opB   = operations->opB;
    const uint64_t opC   = operations->opC;
	uint32_t regB  = operations->regB;

    if(unit1 == AX_EXE_ALU)
    {
        switch(unit2)
        {
            default:
				core->error =  AX_ERROR_ILLEGAL_INSTRUCTION;
                return;

			case AX_OPCODE_NOP:

            break;

            case AX_OPCODE_ADD:
                ireg[opA] = opB+opC;
            break;

            case AX_OPCODE_SUB:
                ireg[opA] = opB-opC;
            break;

            case AX_OPCODE_XOR:
                ireg[opA] = opB^opC;
            break;

            case AX_OPCODE_OR:
                ireg[opA] = opB|opC;
            break;

            case AX_OPCODE_AND:
                ireg[opA] = opB&opC;
            break;

            case AX_OPCODE_LSL:
                ireg[opA] = opB<<opC;
            break;

            case AX_OPCODE_LSR:
                ireg[opA] = opB>>opC;
            break;

            case AX_OPCODE_ASR:
                ireg[opA] = (int64_t)opB>>(int64_t)opC;
            break;

            case AX_OPCODE_MUL:
                ireg[AX_REG_P] = opB*opC;
            break;

            case AX_OPCODE_DIVS:
                ireg[AX_REG_Q] = (int64_t)opB/(int64_t)opC;
            break;

            case AX_OPCODE_DIVU:
                ireg[AX_REG_Q] = opB/opC;
            break;

            case AX_OPCODE_REMS:
                ireg[AX_REG_Q] = (int64_t)opB%(int64_t)opC;
            break;

            case AX_OPCODE_REMU:
                ireg[AX_REG_Q] = opB%opC;
            break;


            case AX_OPCODE_BOOL:
            	ireg[opB] &= sizemask[size];
                ireg[opA] = (opB != 0);
            break;

            case AX_OPCODE_SEXT:
            	if(size == 0)
				{
					if(opB&0x80)
						opB |= signmask[0];
				}
				else if(size == 1)
				{
					if(opB&0x8000)
						opB |= signmask[1];
				}
				else if(size == 2)
				{
					if(opB&0x80000000)
						opB |= signmask[2];
				}

                ireg[opA] = opB;
            break;

            case AX_OPCODE_SLTS:
                ireg[opA] = ( (int64_t)opB < (int64_t)opC);
            break;

            case AX_OPCODE_SLTU:
                ireg[opA] = (opB < opC);
            break;

            case AX_OPCODE_SMOVE:
                ireg[opA] |= opB<<(size*16);
                size = 3;
            break;

            case AX_OPCODE_MOVE:
                ireg[opA] = opB;
                size = 3;
            break;

            case AX_OPCODE_MOVEINS:
                ireg[opA] = core->instruction;
                size = 3;
            break;

            case AX_OPCODE_MOVECYCLE:
                ireg[opA] = core->cycle;
                size = 3;
            break;

            case AX_OPCODE_MOVERI1:
            	regB = regB&3;
            	if(regB == 0)
					ireg[opA] = core->lr;
				else if(regB == 1)
					ireg[opA] = core->br;
				else if(regB == 2)
					ireg[opA] = core->flags;


                size = 3;
            break;

            case AX_OPCODE_MOVERI2:
                regB = regB&3;
            	if(regB == 0)
					core->lr = ireg[opA];
				else if(regB == 1)
					core->br = ireg[opA];
				else if(regB == 2)
					core->flags = ireg[opA];

                size = 3;
            break;

            case AX_OPCODE_CMPFR:

                core->flags &= ZSUClearMask;
                core->flags |= (core->flags & opA) << 0u;
				size = 3;
            break;

            case AX_OPCODE_CMP:

                opA = ireg[opA] & sizemask[size];
                opB = opB & sizemask[size];

                core->flags &= ZSUClearMask;
                core->flags |= (opB != opA) << 0u;
                core->flags |= ((int64_t)opB < (int64_t)opA) << 1u;
                core->flags |= (opB < opA) << 2u;
				size = 3;
            break;

            case AX_OPCODE_ENDP:
                core->error = AX_END_OF_CODE;
            break;
        }
        ireg[opA] &= sizemask[size];
    }
    else if(unit1 == AX_EXE_LSU)
    {
        uint64_t offset =  opB + opC;
        void *addressA = core->spm + ireg[opA];

        switch(unit2)
        {
            case AX_OPCODE_LD:
                executeLS(core,&ireg[opA],offset,1u << size,0);
            break;

            case AX_OPCODE_ST:
                executeLS(core,&ireg[opA],offset,1u << size,1);
            break;

            case AX_OPCODE_LDV:
                executeLS(core,&freg[opA*4],offset, (1+size) << 4u,0);
            break;

            case AX_OPCODE_STV:
                executeLS(core,&freg[opA*4],offset, (1+size) << 4u,1);
            break;

            case AX_OPCODE_LDDMA:
                executeLS(core,addressA,opB,opC*64,0);
            break;

            case AX_OPCODE_STDMA:
                executeLS(core,addressA,opB,opC*64,1);
            break;

            case AX_OPCODE_WAIT:
            case AX_OPCODE_FLUSH:
            case AX_OPCODE_PREFETCH:
            case AX_OPCODE_IFLUSH:
            case AX_OPCODE_IPREFETCH:

            break;

        }
    }
    else if(unit1 == AX_EXE_BRU)
    {
        core->delay = 1;
    }
    else if(unit1 == AX_EXE_VFPU)
    {
		float *fopA,*fopB,*fopC,ftmp;

		fopA = operations->fopA;
        fopB = operations->fopB;
        fopC = operations->fopC;

        int32_t* viregB = (int32_t*)operations->fopB;
		uint16_t* hiregB = (uint16_t*)operations->fopB;

		int i1,i2,i3,i4;
		opA *= 4;
		switch(unit2)
		{
			case AX_OPCODE_FADD:
				freg[opA] = fopB[size]+fopC[id];
			break;

			case AX_OPCODE_FSUB:
				freg[opA] = fopB[size]-fopC[id];
			break;

			case AX_OPCODE_FMUL:
				freg[opA] = fopB[size]*fopC[id];
			break;

			case AX_OPCODE_FMADD:
				freg[opA] *= fopB[size]+fopC[id];
			break;

			case AX_OPCODE_FMSUB:
				freg[opA] *= fopB[size]-fopC[id];
			break;

			//------------

			case AX_OPCODE_VFADD:
				for(uint32_t i = 0;i < size;i++)
					freg[opA+i] = fopB[i]+fopC[i];
			break;

			case AX_OPCODE_VFSUB:
				for(uint32_t i = 0;i < size;i++)
					freg[opA+i] = fopB[i]-fopC[i];
			break;

			case AX_OPCODE_VFMUL:
				for(uint32_t i = 0;i < size;i++)
					freg[opA+i] = fopB[i]*fopC[i];
			break;

			case AX_OPCODE_VFMADD:
				for(uint32_t i = 0;i < size;i++)
					freg[opA+i] *= fopB[i]+fopC[i];
			break;

			case AX_OPCODE_VFMSUB:
				for(uint32_t i = 0;i < size;i++)
					freg[opA+i] *= fopB[i]-fopC[i];
			break;

			//------------

			case AX_OPCODE_VFADDS:
				for(uint32_t i = 0;i < size;i++)
					freg[opA+i] = fopB[i]+fopC[id];
			break;

			case AX_OPCODE_VFSUBS:
				for(uint32_t i = 0;i < size;i++)
					freg[opA+i] = fopB[i]-fopC[id];
			break;

			case AX_OPCODE_VFMULS:
				for(uint32_t i = 0;i < size;i++)
					freg[opA+i] = fopB[i]*fopC[id];
			break;

			case AX_OPCODE_VFMADDS:
				for(uint32_t i = 0;i < size;i++)
					freg[opA+i] *= fopB[i]+fopC[id];
			break;

			case AX_OPCODE_VFMSUBS:
				for(uint32_t i = 0;i < size;i++)
					freg[opA+i] *= fopB[i]-fopC[id];
			break;

			//------------


			case AX_OPCODE_VFSHUFFLE:

				i1 = (opC>>0)&3;
				i2 = (opC>>2)&3;
				i3 = (opC>>4)&3;
				i4 = (opC>>6)&3;

				freg[opA+i1] = fopB[0];
				freg[opA+i2] = fopB[1];
				freg[opA+i3] = fopB[2];
				freg[opA+i4] = fopB[3];

			break;

			case AX_OPCODE_FCMP:

				ftmp = fopA[size]-fopB[id];

				core->flags &= ZSUClearMask;
				core->flags |= (ftmp != 0.0f) << 0u;
				core->flags |= (ftmp <  0.0f) << 1u;
				core->flags |= (ftmp <  0.0f) << 2u;

			break;




			case AX_OPCODE_FMOVE:
				freg[opA+size] = fopB[id];
			break;

			case AX_OPCODE_VFMOVE:
				for(uint32_t i = 0;i < size;i++)
					freg[opA+i] = fopB[i];


			break;

			case AX_OPCODE_FNEG:
				freg[opA+size] = -fopB[id];
			break;

			case AX_OPCODE_FABS:
				if(fopB[id] < 0)
					freg[opA+size] = -fopB[id];
			break;

			case AX_OPCODE_VFNEG:
				for(uint32_t i = 0;i < size;i++)
					freg[opA+i] = -fopB[i];
			break;

			case AX_OPCODE_VFABS:
				for(uint32_t i = 0;i < size;i++)
				{
					if(fopB[id] < 0)
						freg[opA+i] = -fopB[i];
				}

			break;

			case AX_OPCODE_VFTOI:
				for(uint32_t i = 0;i < size;i++)
					vireg[opA+i] = fopB[i];
			break;

			case AX_OPCODE_VITOF:
				for(uint32_t i = i;i < size;i++)
					freg[opA+i] = viregB[i];
			break;

			case AX_OPCODE_VFTOD:
				dreg[opA>>1] = fopB[0];
			break;

			case AX_OPCODE_VDTOF:
				freg[opA] = dreg[regB];
			break;

			case AX_OPCODE_VFTOH:
				for(uint32_t i = i;i < size;i++)
					hiregB[ (opA<<1)+i] = FloattoHalf(fopB[i]);
			break;

			case AX_OPCODE_VHTOF:
				for(uint32_t i = i;i < size;i++)
					freg[opA+i] = HalftoFloat(hiregB[i]);
			break;
		}

	}
	else if(unit1 == AX_EXE_EFU) //EFU
	{

	}
	else //DOUBLE
	{
		double dopA,dopB,dopC,dtmp;

		dopA = operations->dopA;
		dopB = operations->dopB;
		dopC = operations->dopC;

		opA *= 2;
		switch(unit2)
		{

			case AX_OPCODE_DMOVE:
				dreg[opA] = dopB;
			break;

			case AX_OPCODE_DADD:
				dreg[opA] = dopB+dopC;
			break;

			case AX_OPCODE_DSUB:
				dreg[opA] = dopB-dopC;
			break;

			case AX_OPCODE_DMUL:
				dreg[opA] = dopB*dopC;
			break;

			case AX_OPCODE_DNEG:
				dreg[opA] = -dopB;
			break;

			case AX_OPCODE_DABS:
				if(dopB < 0)
					dreg[opA] = -dopB;
			break;

			case AX_OPCODE_DCMP:

				dtmp = dopA-dopB;

				core->flags &= ZSUClearMask;
				core->flags |= (dtmp != 0.0f) << 0u;
				core->flags |= (dtmp <  0.0f) << 1u;
				core->flags |= (dtmp <  0.0f) << 2u;
			break;
		}


	}

}

int AX_execute(Core *core)
{
	if(core->delay == 1)
    {
        executeDelayedInstruction(core);
    }

	executeInstruction(core,&core->operations[0]);


	if(core->swt == 0)
		return 0;

	executeInstruction(core,&core->operations[1]);

/*
    for(uint32_t i = 0; i < core->swt; ++i)
    {
        int result = executeInstruction(core, i);
        if(result != 0)
        {
            return result;
        }
    }
*/


    return 0;
}
