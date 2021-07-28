#include "vm.h"

#include <assert.h>
#include <string.h>
#include <math.h>

#include <stdio.h>


static ArResult executeDelayedInstruction(ArProcessor restrict processor, uint16_t imm)
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



static void executeLS(ArProcessor processor,void *reg,uint64_t offset,uint32_t size,uint32_t store)
{
    void *address;
    uint64_t mmap = offset & MEMORY_MAP_OTHER;

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
            if(mmap == MEMORY_MAP_IO_BEGIN)
            {
                address = processor->io;
                offset &= 0xFFFFF; //Max bloc 1Mio

            }
            else if(mmap == MEMORY_MAP_SPM2_BEGIN)
            {
                address = processor->spm2;
                offset &= 0xFFFFFFF; //Max 256 Mio

            }
            else if(mmap == MEMORY_MAP_ROM_BEGIN)
            {
                address = processor->rom;
                offset &= 0xFFFFFFFF; //Max 4 Gio
            }
            else //if(address == MEMORY_MAP_1TSRAM_BEGIN)
            {
                address = processor->tsramCPU;
                offset &= 0x7FFFFFFF; //Max 2 Gio
            }
        }

    }
/*
    if(memoryAddress + size > memory->size)
    {
        return AR_ERROR_PHYSICAL_MEMORY_OUT_OF_RANGE;
    }
*/
    if(store)
    {
        memcpy(reg, address+offset, size);
    }
    else
    {
        memcpy(address+offset, reg, size);
    }
}


static ArResult executeInstruction(ArProcessor restrict processor, uint32_t index)
{
    //Mask to trunc results base on size (8 bits, 16 bits, 32 bits or 64 bits)
    static const uint64_t sizemask[4] =
    {
        0x00000000000000FFull,
        0x000000000000FFFFull,
        0x00000000FFFFFFFFull,
        0xFFFFFFFFFFFFFFFFull,
    };

    static const uint32_t ZSUClearMask  = ~(Z_MASK | S_MASK | U_MASK);

    uint64_t* restrict const ireg = processor->ireg;
    float*    restrict const freg = processor->vreg;
    double*   restrict const dreg = processor->dreg;
    
    uint32_t unit1 = processor->operations[index].unit1;
    uint32_t unit2 = processor->operations[index].unit2;
    uint32_t size  = processor->operations[index].size;
    uint32_t id  = processor->operations[index].id;

    uint64_t opA   = processor->operations[index].opA;
    uint64_t opB   = processor->operations[index].opB;
    uint64_t opC   = processor->operations[index].opC;


    float *fopA,*fopB,*fopC;
    double dopA,dopB,dopC;

    if(unit1 == AK1_EXE_ALU)
    {
        switch(unit2)
        {
            default:
                return AR_ERROR_ILLEGAL_INSTRUCTION;

            case AK1_OPCODE_ADD:
                ireg[opA] = opB+opC;
            break;

            case AK1_OPCODE_SUB:
                ireg[opA] = opB-opC;
            break;

            case AK1_OPCODE_XOR:
                ireg[opA] = opB^opC;
            break;

            case AK1_OPCODE_OR:
                ireg[opA] = opB|opC;
            break;

            case AK1_OPCODE_AND:
                ireg[opA] = opB&opC;
            break;

            case AK1_OPCODE_LSL:
                ireg[opA] = opB<<opC;
            break;

            case AK1_OPCODE_LSR:
                ireg[opA] = opB>>opC;
            break;

            case AK1_OPCODE_ASR:
                ireg[opA] = (int64_t)opB>>(int64_t)opC;
            break;

            case AK1_OPCODE_MULS:
                ireg[AK1_REG_P] = (int64_t)opB*(int64_t)opC;
            break;

            case AK1_OPCODE_MULU:
                ireg[AK1_REG_P] = opB*opC;
            break;

            case AK1_OPCODE_DIVS:
                ireg[AK1_REG_Q] = (int64_t)opB/(int64_t)opC;
            break;

            case AK1_OPCODE_DIVU:
                ireg[AK1_REG_Q] = opB/opC;
            break;

            case AK1_OPCODE_REMS:
                ireg[AK1_REG_Q] = (int64_t)opB%(int64_t)opC;
            break;

            case AK1_OPCODE_REMU:
                ireg[AK1_REG_Q] = opB%opC;
            break;

            case AK1_OPCODE_MOVE:
                ireg[opA] = opB;
                size = 3;
            break;

            case AK1_OPCODE_MOVESP:
                ireg[opA] = 0;
                size = 3;
            break;

            case AK1_OPCODE_MOVEINS:
                ireg[opA] = processor->instruction;
                size = 3;
            break;

            case AK1_OPCODE_MOVECYCLE:
                ireg[opA] = processor->cycle;
                size = 3;
            break;
        }
        ireg[opA] &= sizemask[size];
    }
    else if(unit1 == AK1_EXE_LSU)
    {
        uint64_t offset =  opB + opC;
        switch(unit2)
        {
            default:
                return AR_ERROR_ILLEGAL_INSTRUCTION;

            case AK1_OPCODE_LDM:
                memcpy(&ireg[opA], processor->dsram + (offset&0x1FFFF), 1u << size);
            break;

            case AK1_OPCODE_STM:
                memcpy(processor->dsram + (offset&0x1FFFF), &ireg[opA], 1u << size);
            break;

            case AK1_OPCODE_LDC:
                executeLS(processor,&ireg[opA*4],offset,1u << size,0);
            break;

            case AK1_OPCODE_STC:
                executeLS(processor,&ireg[opA*4],offset,1u << size,1);
            break;

            case AK1_OPCODE_LDMV:
                memcpy(&freg[opA*4], processor->dsram + (offset&0x1FFFF), (1+size) << 4u);
            break;

            case AK1_OPCODE_STMV:
                memcpy(processor->dsram + (offset&0x1FFFF), &freg[opA*4], (1+size) << 4u);
            break;

            case AK1_OPCODE_LDCV:
                executeLS(processor,&freg[opA*4],offset, (1+size) << 4u,0);
            break;

            case AK1_OPCODE_STCV:
                executeLS(processor,&freg[opA*4],offset, (1+size) << 4u,1);
            break;

            case AK1_OPCODE_FLUSH:
            case AK1_OPCODE_PREFETCH:

            break;

        }
    }
    else if(unit1 == AK1_EXE_CMP)
    {
        fopA = processor->operations[index].fopA;
        fopB = processor->operations[index].fopB;

        dopA = processor->operations[index].dopA;
        dopB = processor->operations[index].dopB;

        float ftmp;
        double dtmp;

        switch(unit2)
        {
            default:
                return AR_ERROR_ILLEGAL_INSTRUCTION;

            case AK1_OPCODE_CMP:

                opA = opA & sizemask[size];
                opB = opB & sizemask[size];

                processor->flags &= ZSUClearMask;
                processor->flags |= (opB != opA) << 1u;
                processor->flags |= ((int64_t)opB < (int64_t)opA) << 2u;
                processor->flags |= (opB < opA) << 3u;

            break;

            case AK1_OPCODE_FCMP:

                ftmp = fopA[size]-fopB[id];

                processor->flags &= ZSUClearMask;
                processor->flags |= (ftmp != 0.0f) << 1u;
                processor->flags |= (ftmp <  0.0f) << 2u;
                processor->flags |= (ftmp <  0.0f) << 3u;

            break;

            case AK1_OPCODE_DCMP:
            
                dtmp = dopA-dopB;

                processor->flags &= ZSUClearMask;
                processor->flags |= (dtmp != 0.0f) << 1u;
                processor->flags |= (dtmp <  0.0f) << 2u;
                processor->flags |= (dtmp <  0.0f) << 3u;
            break;


        }
    }
    else if(unit1 == AK1_EXE_BRU)
    {
        processor->delay = 1;
    }
    else if(unit1 == AK1_EXE_DMA)
    {
        void *addressA = processor->dsram + ireg[opA];

        switch(unit2)
        {
            default:
                return AR_ERROR_ILLEGAL_INSTRUCTION;

            case AK1_OPCODE_LDDMA:
                executeLS(processor,addressA,opB,opC*64,1);

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
                executeLS(processor,addressA,opB|MEMORY_MAP_RAM_BEGIN,opC*64,0);

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
                    printf("%c\n",ireg[opA]);
                else if(opB == 1)
                    printf("%f %f %f %f\n",freg[(opA*4)+0],freg[(opA*4)+1],freg[(opA*4)+2],freg[(opA*4)+3]);
                else if(opB == 2)
                    printf("%d\n",dreg[opA]);

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
            opA *= 4;

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


                case AK1_OPCODE_VFTOD:
                    dreg[opA>>1] = fopB[opA];
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

ArResult arExecuteInstruction(ArProcessor processor)
{
    assert(processor);

    //printf("pc %x\n",processor->pc);

    const uint32_t size = processor->swt;

    if(processor->delay == 1)
    {
        ArResult result = executeDelayedInstruction(processor, processor->imm);
        if(result != AR_SUCCESS)
        {
            return result;
        }
    }
    
    for(uint32_t i = 0; i < size; ++i)
    {
        ArResult result = executeInstruction(processor, i);
        if(result != AR_SUCCESS)
        {
            return result;
        }
    }

    processor->cycle++;

    return AR_SUCCESS;
}
