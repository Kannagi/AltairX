#include <assert.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>

#include "vm.h"


static int32_t executeDelayedInstruction(ComputeUnit *processor, uint16_t imm)
{

    switch(processor->delayop)
    {
        default:
            return AR_ERROR_ILLEGAL_INSTRUCTION;

        case AK1_OPCODE_BNE: // !=
            if(processor->flags & Z_MASK)
            {
                processor->pc += imm;
            }

            break;

        case AK1_OPCODE_BEQ: // ==
            if(!(processor->flags & Z_MASK))
            {
                processor->pc += imm;
            }
            break;

        case AK1_OPCODE_BL: // <
            if(processor->flags & U_MASK)
            {
                processor->pc += imm;
            }
            break;

        case AK1_OPCODE_BLE: // <=
            if((processor->flags & U_MASK) || !(processor->flags & Z_MASK))
            {
                processor->pc += imm;
            }
            break;

        case AK1_OPCODE_BG: // >
            if(!(processor->flags & U_MASK))
            {
                processor->pc += imm;
            }
            break;

        case AK1_OPCODE_BGE: // >=
            if(!(processor->flags & U_MASK) || !(processor->flags & Z_MASK))
            {
                processor->pc += imm;
            }
            break;

        case AK1_OPCODE_BLS: // <
            if(processor->flags & S_MASK)
            {
                processor->pc += imm;
            }
            break;

        case AK1_OPCODE_BLES: // <=
            if((processor->flags & S_MASK) || !(processor->flags & Z_MASK))
            {
                processor->pc += imm;
            }
            break;

        case AK1_OPCODE_BGS: // >
            if(!(processor->flags & S_MASK))
            {
                processor->pc += imm;
            }
            break;

        case AK1_OPCODE_BGES: // >=
            if(!(processor->flags & S_MASK) || !(processor->flags & Z_MASK))
            {
                processor->pc += imm;
            }
            break;

        case AK1_OPCODE_BRA:
            processor->pc += imm;
            break;

        case AK1_OPCODE_JMP:
            processor->pc = imm;
            break;

        case AK1_OPCODE_CALL:
            processor->lr = processor->pc;
            processor->pc = imm;
            break;

        case AK1_OPCODE_JMPBR:
            processor->pc = processor->br+imm;
            break;

        case AK1_OPCODE_CALLBR:
            processor->lr = processor->pc;
            processor->pc = processor->br+imm;
            break;

        case AK1_OPCODE_RET:
            processor->pc = processor->lr;
            break;
    }

    processor->delay = 0;

    return AR_SUCCESS;
}



static void executeLS(ComputeUnit *processor,void *reg,uint64_t offset,uint32_t size,uint32_t store)
{
    void *address;

    if(offset&MEMORY_MAP_RAM_BEGIN)
    {
        address = processor->ram;
        offset &= 0x7FFFFFFFF; //Max 32 Gio
    }else
    {
        if(offset&MEMORY_MAP_VRAM_BEGIN)
        {
            return;
        }else
        {
            if(offset == MEMORY_MAP_SPM2_BEGIN)
            {
                address = processor->spm2;
                offset &= 0xFFFFFFF; //Max 256 Mio

            }
            else if(offset == MEMORY_MAP_1TSRAM_BEGIN)
            {
                address = processor->tsram;
                offset &= 0x7FFFFFFF; //Max 2 Gio
            }
            else
            {

            }
        }

    }

    if(store)
    {
        memcpy(reg, address+offset, size);
    }
    else
    {
        memcpy(address+offset, reg, size);
    }
}


static int32_t executeInstruction(ComputeUnit *processor, uint32_t index)
{
    static const uint32_t ZSUClearMask  = ~(Z_MASK | S_MASK | U_MASK);

    uint32_t* const ireg = processor->ireg;
    float*    const freg = processor->vreg;
    double*   const dreg = processor->dreg;

    uint32_t unit1 = processor->operations[index].unit1;
    uint32_t unit2 = processor->operations[index].unit2;
    uint32_t size  = processor->operations[index].size;
    uint32_t id  = processor->operations[index].id;

    int32_t rA   = processor->operations[index].rA;
    int32_t rB   = processor->operations[index].rB;
    int32_t rC   = processor->operations[index].rC;


    float *fopA,*fopB,*fopC;
    double dopA,dopB,dopC;

    int32_t opB = ireg[rB];
    int32_t opC;

    if(processor->operations[index].immf == 0)
		opC = ireg[rC];
	else
		opC = processor->operations[index].imm;


    if(unit1 == AK1_EXE_ALU)
    {
        switch(unit2)
        {
            default:
                return AR_ERROR_ILLEGAL_INSTRUCTION;

            case AK1_OPCODE_ADD:
                ireg[rA] = opB+opC;
            break;

            case AK1_OPCODE_SUB:
                ireg[rA] = opB-opC;
            break;

            case AK1_OPCODE_XOR:
                ireg[rA] = opB^opC;
            break;

            case AK1_OPCODE_OR:
                ireg[rA] = opB|opC;
            break;

            case AK1_OPCODE_AND:
                ireg[rA] = opB&opC;
            break;

            case AK1_OPCODE_LSL:
                ireg[rA] = opB<<opC;
            break;

            case AK1_OPCODE_LSR:
                ireg[rA] = opB>>opC;
            break;

            case AK1_OPCODE_ASR:
                ireg[rA] = (int32_t)opB>>(int32_t)opC;
            break;

            case AK1_OPCODE_MOVE:
                ireg[rA] = opB;
            break;

            case AK1_OPCODE_MOVESP:
                ireg[rA] = 0;
            break;

        }
    }
    else if(unit1 == AK1_EXE_LSU)
    {
        uint64_t offset =  opB + opC;
        switch(unit2)
        {
            default:
                return AR_ERROR_ILLEGAL_INSTRUCTION;

            case AK1_OPCODE_LDM:
                memcpy(&freg[rA*4], processor->dsram + (offset&0x1FFFF), 16);
            break;

            case AK1_OPCODE_STM:
                memcpy(processor->dsram + (offset&0x1FFFF), &freg[rA*4],16);
            break;

            case AK1_OPCODE_LDC:
                executeLS(processor,&freg[rA*4],offset, 16,0);
            break;

            case AK1_OPCODE_FLUSH:
            case AK1_OPCODE_PREFETCH:

            break;

        }
    }
    else if(unit1 == AK1_EXE_CMP)
    {
        float ftmp;
        opC = ireg[rA];

        switch(unit2)
        {
            default:
                return AR_ERROR_ILLEGAL_INSTRUCTION;

            case AK1_OPCODE_CMP:

                processor->flags &= ZSUClearMask;
                processor->flags |= (opB != opC) << 1u;
                processor->flags |= ((int32_t)opB < (int32_t)opC) << 2u;
                processor->flags |= (opB < opC) << 3u;

            break;

            case AK1_OPCODE_FCMP:

                ftmp = fopA[size]-fopB[id];

                processor->flags &= ZSUClearMask;
                processor->flags |= (ftmp != 0.0f) << 1u;
                processor->flags |= (ftmp <  0.0f) << 2u;
                processor->flags |= (ftmp <  0.0f) << 3u;

            break;
        }
    }
    else if(unit1 == AK1_EXE_BRU)
    {
        processor->delay = 1;
    }
    else if(unit1 == AK1_EXE_DMA)
    {
        void *addressA = processor->dsram + ireg[rA];

        switch(unit2)
        {
            default:
                return AR_ERROR_ILLEGAL_INSTRUCTION;

            case AK1_OPCODE_LDDMA:
                executeLS(processor,addressA,ireg[rA],opC*64,1);
            break;


            case AK1_OPCODE_STDMA:
                executeLS(processor,addressA,opB,opC*64,0);
            break;

            case AK1_OPCODE_LDDMACL:
                for(int i = 0;i < opB;i++)
                {
                    //processor->dsram[ireg[opA]+i]
                    //executeLS(processor,addressA,opB,opC*64,0);
                }
            break;

            case AK1_OPCODE_STDMACL:

            case AK1_OPCODE_DMAI:
                addressA = processor->isram + ireg[opA];
                executeLS(processor,addressA,opB,opC*64,0);

            break;

            case AK1_OPCODE_WAIT:
                //We don't have anything to wait since we emulate it based on C memory model, all transfers are direcly coherent
            break;
        }

    }
    else if(unit1 == AK1_EXE_OTHER)
    {
        switch(unit2)
        {
            default:
                return AR_ERROR_ILLEGAL_INSTRUCTION;

            case AK1_OPCODE_INT:

                if(opB == 0)
                    printf("%c\n",(char)ireg[opA]);
                else if(opB == 1)
                    printf("%f %f %f %f\n",freg[(opA*4)+0],freg[(opA*4)+1],freg[(opA*4)+2],freg[(opA*4)+3]);
                else if(opB == 2)
                    printf("%lf\n",dreg[opA]);

            break;

            case AK1_OPCODE_ENDP:
                return AR_END_OF_CODE;
            break;
        }
    }
    else if(unit1 == AK1_EXE_VFPU)
    {
        uint32_t unit3 = unit2>>4;
        unit2 &= 0xF;

        fopB = processor->operations[index].fopB;
        fopC = processor->operations[index].fopC;

        if(unit3 == 0) //VFALU
        {
            opA *= 4;
            switch(unit2)
            {
                default:
                    return AR_ERROR_ILLEGAL_INSTRUCTION;

                case AK1_OPCODE_FADD:
                    freg[opA] = fopB[size]+fopC[id];
                break;

                case AK1_OPCODE_FSUB:
                    freg[opA] = fopB[size]-fopC[id];
                break;

                case AK1_OPCODE_FMUL:
                    freg[opA] = fopB[size]*fopC[id];
                break;

                case AK1_OPCODE_FMADD:
                    freg[opA] *= fopB[size]+fopC[id];
                break;

                case AK1_OPCODE_FMSUB:
                    freg[opA] *= fopB[size]-fopC[id];
                break;

                //------------

                case AK1_OPCODE_VFADD:
                    for(int i = 0;i < size;i++)
                        freg[opA+i] = fopB[i]+fopC[i];
                break;

                case AK1_OPCODE_VFSUB:
                    for(int i = 0;i < size;i++)
                        freg[opA+i] = fopB[i]-fopC[i];
                break;

                case AK1_OPCODE_VFMUL:
                    for(int i = 0;i < size;i++)
                        freg[opA+i] = fopB[i]*fopC[i];
                break;

                case AK1_OPCODE_VFMADD:
                    for(int i = 0;i < size;i++)
                        freg[opA+i] *= fopB[i]+fopC[i];
                break;

                case AK1_OPCODE_VFMSUB:
                    for(int i = 0;i < size;i++)
                        freg[opA+i] *= fopB[i]-fopC[i];
                break;

                //------------

                case AK1_OPCODE_VFADDS:
                    for(int i = 0;i < size;i++)
                        freg[opA+i] = fopB[i]+fopC[id];
                break;

                case AK1_OPCODE_VFSUBS:
                    for(int i = 0;i < size;i++)
                        freg[opA+i] = fopB[i]-fopC[id];
                break;

                case AK1_OPCODE_VFMULS:
                    for(int i = 0;i < size;i++)
                        freg[opA+i] = fopB[i]*fopC[id];
                break;

                case AK1_OPCODE_VFMADDS:
                    for(int i = 0;i < size;i++)
                        freg[opA+i] *= fopB[i]+fopC[id];
                break;

                case AK1_OPCODE_VFMSUBS:
                    for(int i = 0;i < size;i++)
                        freg[opA+i] *= fopB[i]-fopC[id];
                break;

                //------------


                //case AK1_OPCODE_VFSHUFFLE:

                //break;



            }
        }
        else if(unit3 == 1) //VFMOVE
        {
        	dopB = processor->operations[index].dopB;
            opA *= 4;

            int32_t* viregB = (int32_t*)processor->operations[index].fopB;

            switch(unit2)
            {
                default:
                    return AR_ERROR_ILLEGAL_INSTRUCTION;

                case AK1_OPCODE_FMOVE:
                    freg[opA+size] = fopB[id];
                break;

                case AK1_OPCODE_VFMOVE:
                    for(int i = 0;i < size;i++)
                        freg[opA+i] = fopB[i];
                break;

                case AK1_OPCODE_FNEG:
                    freg[opA+size] = -fopB[id];
                break;

                case AK1_OPCODE_FABS:
                    if(fopB[id] < 0)
                        freg[opA+size] = -fopB[id];
                break;

                case AK1_OPCODE_VFNEG:
                    for(int i = 0;i < size;i++)
                        freg[opA+i] = -fopB[i];
                break;

                case AK1_OPCODE_VFABS:
                    for(int i = 0;i < size;i++)
                    {
                        if(fopB[id] < 0)
                            freg[opA+i] = -fopB[i];
                    }

                break;

                case AK1_OPCODE_VFTOI:
                    for(int i = 0;i < size;i++)
                        vireg[opA+i] = fopB[i];
                break;

                case AK1_OPCODE_VITOF:
                    for(int i = i;i < size;i++)
                        freg[opA] = viregB[i];
                break;

                case AK1_OPCODE_VFTOD:
                    dreg[opA>>1] = fopB[0];
                break;

                case AK1_OPCODE_VDTOF:
                    freg[opA] = dopB;
                break;
            }

        }
        else if(unit3 == 2) //VF
        {


        }
        else //DOUBLE
        {
            dopB = processor->operations[index].dopB;
            dopC = processor->operations[index].dopC;

            opA *= 2;
            switch(unit2)
            {
                default:
                    return AR_ERROR_ILLEGAL_INSTRUCTION;

                case AK1_OPCODE_DMOVE:
                    dreg[opA] = dopB;
                break;

                case AK1_OPCODE_DADD:
                    dreg[opA] = dopB+dopC;
                break;

                case AK1_OPCODE_DSUB:
                    dreg[opA] = dopB-dopC;
                break;

                case AK1_OPCODE_DMUL:
                    dreg[opA] = dopB*dopC;
                break;

                case AK1_OPCODE_FNEG:
                    dreg[opA] = -dopB;
                break;

                case AK1_OPCODE_FABS:
                    if(dopB < 0)
                        dreg[opA] = -dopB;
                break;

                case AK1_OPCODE_DDIV:
                    dreg[opA] = dopB/dopC;
                break;
            }

        }

    }
    else if(unit1 == AK1_EXE_NOP)
    {
    }
    else
    {
        return AR_ERROR_ILLEGAL_INSTRUCTION;
    }

    return AR_SUCCESS;
}



int32_t Aldebaran_ExcecuteInstruction(ComputeUnit *processor)
{
    assert(processor);

    //printf("pc %x\n",processor->pc);

    if(processor->delay == 1)
    {
        int32_t result = executeDelayedInstruction(processor, processor->imm);
        if(result != AR_SUCCESS)
        {
            return result;
        }
    }
/*
    for(uint32_t i = 0; i < size; ++i)
    {
        /nt32_t result = executeInstruction(processor, i);
        if(result != AR_SUCCESS)
        {
            return result;
        }
    }
*/
    processor->cycle++;

    return AR_SUCCESS;
}

