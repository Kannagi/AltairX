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

//--------------------------------------------------------

static void executeDelayedInstruction(Core *core, uint32_t imm)
{
    switch(core->delayop)
    {
        default:
            return;

        case AX_OPCODE_BNE: // !=
            if((core->flags & Z_MASK))
            {
                core->pc += imm;
            }

            break;

        case AX_OPCODE_BEQ: // ==
            if(!(core->flags & Z_MASK))
            {
                core->pc += imm;
            }
            break;

        case AX_OPCODE_BL: // <
            if( (core->flags & U_MASK) && (!(core->flags & Z_MASK)) )
            {
                core->pc += imm;
            }
            break;

        case AX_OPCODE_BLE: // <=
            if(core->flags & U_MASK)
            {
                core->pc += imm;
            }
            break;

        case AX_OPCODE_BG: // >
            if(!(core->flags & U_MASK) && (!(core->flags & Z_MASK)) )
            {
                core->pc += imm;
            }
            break;

        case AX_OPCODE_BGE: // >=
            if(!(core->flags & U_MASK))
            {
                core->pc += imm;
            }
            break;

        case AX_OPCODE_BLS: // <
            if( (core->flags & S_MASK)  && (!(core->flags & Z_MASK)) )
            {
                core->pc += imm;
            }
            break;

        case AX_OPCODE_BLES: // <=
            if(core->flags & S_MASK)
            {
                core->pc += imm;
            }
            break;

        case AX_OPCODE_BGS: // >
            if( (!(core->flags & S_MASK)) && (!(core->flags & Z_MASK)) )
            {
                core->pc += imm;
            }
            break;

        case AX_OPCODE_BGES: // >=
            if(!(core->flags & S_MASK) || !(core->flags & Z_MASK))
            {
                core->pc += imm;
            }
            break;

        case AX_OPCODE_BRA:
            core->pc += imm;
            break;

        case AX_OPCODE_JMP:
            core->pc = imm;
            break;

        case AX_OPCODE_CALL:
            core->lr = core->pc;
            core->pc = imm;
            break;

        case AX_OPCODE_JMPBR:
            core->pc = core->br+imm;
            break;

        case AX_OPCODE_CALLBR:
            core->lr = core->pc;
            core->pc = core->br+imm;
            break;

		case AX_OPCODE_LOOP:

            if(core->ireg[60] != 0)
			{
				core->ireg[60]--;
				core->pc += imm;
			}
            break;

        case AX_OPCODE_RET:
            core->pc = core->lr;
            break;

		case AX_OPCODE_RETI:
            core->pc = core->ir;
            break;

		case AX_OPCODE_INT:
			core->syscall = 2;
			core->ir = core->pc;
			core->pc = 2;
            break;

		case AX_OPCODE_SYSCALL:
			core->syscall = 1;
			core->ir = core->pc;
			core->pc = 0;
            break;
    }

    core->delay = 0;

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
		max = (core->mmap.nspm2-1); //Max 64 Mio
	}
	else
	{
		uint64_t tmp = (offset>>25)&3;
		if(tmp == 0) // SPM L1
		{
			address = core->spm;
			max = 0x7FFF; //Max 32 Kio
		}else
		if(tmp == 1) // ROM
		{
			address = core->mmap.rom;
			max = (core->mmap.nrom-1); //Max 32 Mio
		}else
		if(tmp == 2) // I/O
		{
			address = core->mmap.io;
			max = 0xFFFFF; //Max 1 Mio (max 32 Mio)
		}else
		if(tmp == 3) // SPM Thread
		{
			address = core->mmap.spmt;
			max = (core->mmap.nspmt-1); //Max 32 Mio
		}

	}

	offset &= max;
    if( (offset+size) > max)
    {
        exit(-1);
    }

    if(store)
    {
		memcpy(address+offset, reg, size);
    }
    else
    {
        memcpy(reg, address+offset, size);
    }
}


static int executeInstruction(Core *core, uint32_t index)
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

    static const uint32_t ZSUClearMask  = ~(Z_MASK | S_MASK | U_MASK);

    uint64_t* const ireg = core->ireg;
    float*    const freg = core->vreg;
    double*   const dreg = core->dreg;
    int32_t*  const vireg = core->vireg;

    uint32_t unit1 = core->operations[index].unit1;
    uint32_t unit2 = core->operations[index].unit2;
    uint32_t size  = core->operations[index].size;
    uint32_t id  = core->operations[index].id;

    uint64_t opA   = core->operations[index].opA;
    uint64_t opB   = core->operations[index].opB;
    uint64_t opC   = core->operations[index].opC;
    uint32_t regB  = core->operations[index].regB;


    float *fopA,*fopB,*fopC;
    double dopA,dopB,dopC;

    if(unit1 == AX_EXE_ALU)
    {
        switch(unit2)
        {
            default:
                return AX_ERROR_ILLEGAL_INSTRUCTION;

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

            case AX_OPCODE_MULS:
                ireg[AX_REG_P] = (int64_t)opB*(int64_t)opC;
            break;

            case AX_OPCODE_MULU:
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
            	//printf("%ld %ld\n",opB,opC);
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
        }
        ireg[opA] &= sizemask[size];
    }
    else if(unit1 == AX_EXE_LSU)
    {
        uint64_t offset =  opB + opC;
        switch(unit2)
        {
            default:
                return AX_ERROR_ILLEGAL_INSTRUCTION;

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

            case AX_OPCODE_FLUSH:
            case AX_OPCODE_PREFETCH:
            case AX_OPCODE_IFLUSH:
            case AX_OPCODE_IPREFETCH:

            break;

        }
    }
    else if(unit1 == AX_EXE_CMP)
    {
        fopA = core->operations[index].fopA;
        fopB = core->operations[index].fopB;

        dopA = core->operations[index].dopA;
        dopB = core->operations[index].dopB;

        float ftmp;
        double dtmp;

        switch(unit2)
        {
            default:
                return AX_ERROR_ILLEGAL_INSTRUCTION;

            case AX_OPCODE_CMPFR:

                core->flags &= ZSUClearMask;
                core->flags |= (core->flags & opA) << 0u;

            break;

            case AX_OPCODE_CMP:

                opA = ireg[opA] & sizemask[size];
                opB = opB & sizemask[size];

                core->flags &= ZSUClearMask;
                core->flags |= (opB != opA) << 0u;
                core->flags |= ((int64_t)opB < (int64_t)opA) << 1u;
                core->flags |= (opB < opA) << 2u;

            break;

            case AX_OPCODE_FCMP:

                ftmp = fopA[size]-fopB[id];

                core->flags &= ZSUClearMask;
                core->flags |= (ftmp != 0.0f) << 0u;
                core->flags |= (ftmp <  0.0f) << 1u;
                core->flags |= (ftmp <  0.0f) << 2u;

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
    else if(unit1 == AX_EXE_BRU)
    {
        core->delay = 1;
    }
    else if(unit1 == AX_EXE_DMA)
    {
        void *addressA = core->spm + ireg[opA];

        switch(unit2)
        {
            default:
                return AX_ERROR_ILLEGAL_INSTRUCTION;

            case AX_OPCODE_LDDMA:
                executeLS(core,addressA,opB,opC*64,0);
            break;


            case AX_OPCODE_STDMA:
                executeLS(core,addressA,opB,opC*64,1);
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
            default:
                return AX_ERROR_ILLEGAL_INSTRUCTION;

            case AX_OPCODE_ENDP:

                return AX_END_OF_CODE;
            break;
        }
    }
    else if(unit1 == AX_EXE_VFPU)
    {
        uint32_t unit3 = unit2>>4;
        unit2 &= 0xF;

        fopB = core->operations[index].fopB;
        fopC = core->operations[index].fopC;


        if(unit3 == 0) //VFALU
        {
        	int i1,i2,i3,i4;
            opA *= 4;
            switch(unit2)
            {
                default:
                    return AX_ERROR_ILLEGAL_INSTRUCTION;

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



            }
        }
        else if(unit3 == 1) //VFMOVE
        {
            opA *= 4;

            int32_t* viregB = (int32_t*)core->operations[index].fopB;
            uint16_t* hiregB = (uint16_t*)core->operations[index].fopB;

            switch(unit2)
            {
                default:
                    return AX_ERROR_ILLEGAL_INSTRUCTION;

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
        else if(unit3 == 2) //EFU
        {

        }
        else //DOUBLE
        {
            dopB = core->operations[index].dopB;
            dopC = core->operations[index].dopC;

            opA *= 2;
            switch(unit2)
            {
                default:
                    return AX_ERROR_ILLEGAL_INSTRUCTION;

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

                case AX_OPCODE_DDIV:
                    dreg[opA] = dopB/dopC;
                break;
            }

        }

    }
    else if(unit1 == AX_EXE_NOP)
    {
    }
    else
    {
        return AX_ERROR_EXE;
    }

    return 0;
}

int AX_execute(Core *core)
{
	if(core->delay == 1)
    {
        executeDelayedInstruction(core, core->imm);
    }

	int result = executeInstruction(core, 0);
	if(result != 0)
	{
		return result;
	}

	if(core->swt == 0)
		return 0;

	result = executeInstruction(core, 1);
	if(result != 0)
	{
		return result;
	}
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
