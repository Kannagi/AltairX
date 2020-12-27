#include "vm.h"

#include <assert.h>
#include <string.h>
#include <math.h>

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
    static const uint32_t cmptClearMask = ~CMPT_MASK;

    uint64_t* restrict const ireg = processor->ireg;

    float*    const freg = (float*)processor->freg;
    double*   const dreg = (double*)processor->freg;
    Vector4f* const vreg = (Vector4f*)processor->freg;

    const Operation* restrict const op = &processor->operations[index];
    const uint32_t*  restrict const operands = op->operands;

    switch(op->op)
    {
        case OPCODE_UNKNOWN:
            return AR_ERROR_ILLEGAL_INSTRUCTION;

        //AGU
        case OPCODE_LDDMA:  //fallthrough
        case OPCODE_STDMA:  //fallthrough
        case OPCODE_LDDMAR: //fallthrough
        case OPCODE_STDMAR: //fallthrough
        case OPCODE_DMAIR:  //fallthrough
        case OPCODE_WAIT:
            processor->dma = 1;
            processor->dmaOperation = *op;
            break;

        //LSU
        case OPCODE_LDM: //copy data from dsram to register
            memcpy(&ireg[operands[2]], &processor->dsram[operands[0] + ireg[operands[1]]], 1u << op->size);
            ireg[operands[1]] += op->data; //incr
            break;

        case OPCODE_STM: //copy data from register to dsram
            memcpy(&processor->dsram[operands[0] + ireg[operands[1]]], &ireg[operands[2]], 1u << op->size);
            ireg[operands[1]] += op->data; //incr
            break;

        case OPCODE_LDC: //copy data from cache to register
            memcpy(&ireg[operands[2]], &processor->cache[operands[0] + ireg[operands[1]]], 1u << op->size);
            ireg[operands[1]] += op->data; //incr
            break;

        case OPCODE_STC: //copy data from register to cache
            memcpy(&processor->cache[operands[0] + ireg[operands[1]]], &ireg[operands[2]], 1u << op->size);
            ireg[operands[1]] += op->data; //incr
            break;

        case OPCODE_LDMX: //copy data from dsram to register
            memcpy(&ireg[operands[2]], &processor->dsram[operands[0] + ireg[operands[1]]], 1u << op->size);
            ireg[operands[1]] += op->data; //incr
            break;

        case OPCODE_STMX: //copy data from register to dsram
            memcpy(&processor->dsram[operands[0] + ireg[operands[1]]], &ireg[operands[2]], 1u << op->size);
            ireg[operands[1]] += op->data; //incr
            break;

        case OPCODE_IN: //copy data from iosram to register
            memcpy(&ireg[operands[2]], &processor->iosram[operands[0]], 1u << op->size);
            break;

        case OPCODE_OUT: //copy data from register to iosram
            memcpy(&processor->iosram[operands[0]], &ireg[operands[2]], 1u << op->size);
            break;

        case OPCODE_OUTI: //write data to iosram
            memcpy(&processor->iosram[operands[0]], &ireg[operands[2]], 1u << op->size);
            break;

        case OPCODE_LDMV: //copy data from dsram to vector register
            memcpy(&vreg[operands[2]], &processor->dsram[operands[0] + ireg[operands[1]]], 16);
            ireg[operands[1]] += op->data; //incr
            break;

        case OPCODE_STMV: //copy data from vector register to dsram
            memcpy(&processor->dsram[operands[0] + ireg[operands[1]]], &vreg[operands[2]], 16);
            ireg[operands[1]] += op->data; //incr
            break;

        case OPCODE_LDCV: //copy data from cache to vector register
            memcpy(&vreg[operands[2]], &processor->cache[operands[0] + ireg[operands[1]]], 16);
            ireg[operands[1]] += op->data; //incr
            break;

        case OPCODE_STCV: //copy data from vector register to cache
            memcpy(&processor->cache[operands[0] + ireg[operands[1]]], &vreg[operands[2]], 16);
            ireg[operands[1]] += op->data; //incr
            break;

        case OPCODE_LDMF: //copy data from dsram to float register
            memcpy(&freg[operands[2]], &processor->dsram[operands[0] + ireg[operands[1]]], 4);
            ireg[operands[1]] += op->data; //incr
            break;

        case OPCODE_STMF: //copy data from float register to dsram
            memcpy(&processor->dsram[operands[0] + ireg[operands[1]]], &freg[operands[2]], 4);
            ireg[operands[1]] += op->data; //incr
            break;

        case OPCODE_LDCF: //copy data from cache to float register
            memcpy(&freg[operands[2]], &processor->cache[operands[0] + ireg[operands[1]]], 4);
            ireg[operands[1]] += op->data; //incr
            break;

        case OPCODE_STCF: //copy data from float register to cache
            memcpy(&processor->cache[operands[0] + ireg[operands[1]]], &freg[operands[2]], 4);
            ireg[operands[1]] += op->data; //incr
            break;

        case OPCODE_LDMD: //copy data from dsram to double register
            memcpy(&dreg[operands[2]], &processor->dsram[operands[0] + ireg[operands[1]]], 8);
            ireg[operands[1]] += op->data; //incr
            break;

        case OPCODE_STMD: //copy data from double register to dsram
            memcpy(&processor->dsram[operands[0] + ireg[operands[1]]], &dreg[operands[2]], 8);
            ireg[operands[1]] += op->data; //incr
            break;

        case OPCODE_LDCD: //copy data from cache to double register
            memcpy(&dreg[operands[2]], &processor->cache[operands[0] + ireg[operands[1]]], 8);
            ireg[operands[1]] += op->data; //incr
            break;

        case OPCODE_STCD: //copy data from double register to cache
            memcpy(&processor->cache[operands[0] + ireg[operands[1]]], &dreg[operands[2]], 8);
            ireg[operands[1]] += op->data; //incr
            break;

        //ALU
        case OPCODE_NOP: //In case of nop.e, we need to delay it
            if(op->data)
            {
                processor->delayedBits |= (1u << index);
                processor->delayed[index] = *op;
            }
            break;

        case OPCODE_XCHG: //Flip XCHG bit
            processor->delayedBits |= (1u << index);
            processor->delayed[index] = *op;
            break;

        case OPCODE_MOVEI: //Write a value to a register
            ireg[operands[2]] = operands[0];
            break;

        case OPCODE_ADD: //REG = REG + REG
            ireg[operands[2]] = ireg[operands[1]] + ireg[operands[0]];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case OPCODE_ADDI: //REG = REG + IMM
            ireg[operands[2]] = ireg[operands[1]] + operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case OPCODE_ADDQ: //REG += IMM
            ireg[operands[2]] += operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case OPCODE_SUB: //REG = REG - REG
            ireg[operands[2]] = ireg[operands[1]] - ireg[operands[0]];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case OPCODE_SUBI: //REG = REG - IMM
            ireg[operands[2]] = ireg[operands[1]] - operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case OPCODE_SUBQ: //REG -= IMM
            ireg[operands[2]] -= operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case OPCODE_MULS: //REG = REG * REG (signed)
            (*(int64_t*)ireg[operands[2]]) = (int64_t)ireg[operands[1]] * (int64_t)ireg[operands[0]];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case OPCODE_MULSI: //REG = REG * IMM (signed)
            (*(int64_t*)ireg[operands[2]]) = (int64_t)ireg[operands[1]] * (int64_t)operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case OPCODE_MULSQ: //REG *= IMM (signed)
            (*(int64_t*)ireg[operands[2]]) *= (int64_t)operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case OPCODE_MULU: //REG = REG * REG
            ireg[operands[2]] = ireg[operands[1]] * ireg[operands[0]];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case OPCODE_MULUI: //REG = REG * IMM
            ireg[operands[2]] = ireg[operands[1]] * operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case OPCODE_MULUQ: //REG *= IMM
            ireg[operands[2]] *= operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case OPCODE_DIVS: //REG = REG / REG (signed)
            (*(int64_t*)ireg[operands[2]]) = (int64_t)ireg[operands[1]] / (int64_t)ireg[operands[0]];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case OPCODE_DIVSI: //REG = REG / IMM (signed)
            (*(int64_t*)ireg[operands[2]]) = (int64_t)ireg[operands[1]] / (int64_t)operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case OPCODE_DIVSQ: //REG /= IMM (signed)
            (*(int64_t*)ireg[operands[2]]) /= (int64_t)operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case OPCODE_DIVU: //REG = REG / REG
            ireg[operands[2]] = ireg[operands[1]] / ireg[operands[0]];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case OPCODE_DIVUI: //REG = REG / IMM
            ireg[operands[2]] = ireg[operands[1]] / operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case OPCODE_DIVUQ: //REG /= IMM
            ireg[operands[2]] /= operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case OPCODE_AND: //REG = REG & REG
            ireg[operands[2]] = ireg[operands[1]] & ireg[operands[0]];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case OPCODE_ANDI: //REG = REG & IMM
            ireg[operands[2]] = ireg[operands[1]] & operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case OPCODE_ANDQ: //REG &= IMM
            ireg[operands[2]] &= operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case OPCODE_OR: //REG = REG | REG
            ireg[operands[2]] = ireg[operands[1]] | ireg[operands[0]];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case OPCODE_ORI: //REG = REG | IMM
            ireg[operands[2]] = ireg[operands[1]] | operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case OPCODE_ORQ: //REG |= IMM
            ireg[operands[2]] |= operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case OPCODE_XOR: //REG = REG ^ REG
            ireg[operands[2]] = ireg[operands[1]] ^ ireg[operands[0]];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case OPCODE_XORI: //REG = REG ^ IMM
            ireg[operands[2]] = ireg[operands[1]] ^ operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case OPCODE_XORQ: //REG ^= IMM
            ireg[operands[2]] ^= operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case OPCODE_ASL: //REG = REG << REG (signed)
            (*(int64_t*)ireg[operands[2]]) = (int64_t)ireg[operands[1]] << (int64_t)ireg[operands[0]];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case OPCODE_ASLI: //REG = REG << IMM (signed)
            (*(int64_t*)ireg[operands[2]]) = (int64_t)ireg[operands[1]] << (int64_t)operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case OPCODE_ASLQ: //REG <<= IMM (signed)
            (*(int64_t*)ireg[operands[2]]) <<= (int64_t)operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case OPCODE_LSL: //REG = REG << REG
            ireg[operands[2]] = ireg[operands[1]] << ireg[operands[0]];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case OPCODE_LSLI: //REG = REG << IMM
            ireg[operands[2]] = ireg[operands[1]] << operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case OPCODE_LSLQ: //REG <<= IMM
            ireg[operands[2]] <<= operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case OPCODE_ASR: //REG = REG >> REG (signed)
            (*(int64_t*)ireg[operands[2]]) = (int64_t)ireg[operands[1]] >> (int64_t)ireg[operands[0]];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case OPCODE_ASRI: //REG = REG >> IMM (signed)
            (*(int64_t*)ireg[operands[2]]) = (int64_t)ireg[operands[1]] >> (int64_t)operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case OPCODE_ASRQ: //REG >>= IMM (signed)
            (*(int64_t*)ireg[operands[2]]) >>= (int64_t)operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case OPCODE_LSR: //REG = REG >> REG
            ireg[operands[2]] = ireg[operands[1]] >> ireg[operands[0]];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case OPCODE_LSRI: //REG = REG >> IMM
            ireg[operands[2]] = ireg[operands[1]] >> operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case OPCODE_LSRQ: //REG >>= IMM
            ireg[operands[2]] >>= operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        //BRU
        case OPCODE_BNE:  // fallthrough
        case OPCODE_BEQ:  // fallthrough
        case OPCODE_BL:   // fallthrough
        case OPCODE_BLE:  // fallthrough
        case OPCODE_BG:   // fallthrough
        case OPCODE_BGE:  // fallthrough
        case OPCODE_BLS:  // fallthrough
        case OPCODE_BLES: // fallthrough
        case OPCODE_BGS:  // fallthrough
        case OPCODE_BGES:
            processor->delayedBits |= (1u << index);
            processor->delayed[index] = *op;
            break;

        case OPCODE_CMP: // REG <=> REG
        {
            const uint64_t right = ireg[operands[0]] & sizemask[op->size];
            const uint64_t left  = ireg[operands[1]] & sizemask[op->size];

            processor->flags &= ZSUClearMask;
            processor->flags |= (left != right) << 1u;
            processor->flags |= ((int64_t)left < (int64_t)right) << 2u;
            processor->flags |= (left < right) << 3u;
            processor->flags &= cmptClearMask;

            break;
        }

        case OPCODE_CMPI: // REG <=> IMM
        {
            const uint64_t right = operands[0] & sizemask[op->size];
            const uint64_t left  = ireg[operands[1]] & sizemask[op->size];

            processor->flags &= ZSUClearMask;
            processor->flags |= (left != right) << 1u;
            processor->flags |= ((int64_t)left < (int64_t)right) << 2u;
            processor->flags |= (left < right) << 3u;
            processor->flags &= cmptClearMask;

            break;
        }

        case OPCODE_FCMP: // REG <=> REG
        {
            const float right = freg[operands[0]];
            const float left  = freg[operands[1]];

            processor->flags &= ZSUClearMask;
            processor->flags |= (right != left) << 1u;
            processor->flags |= (left < right) << 2u;
            processor->flags &= cmptClearMask;
            processor->flags |= (0x01u << 30u);

            break;
        }

        case OPCODE_FCMPI:
        {
            const uint32_t iright = operands[0] << 11u;
            const float    fright = *(const float*)(&iright);
            const float    fleft  = freg[operands[1]];

            processor->flags &= ZSUClearMask;
            processor->flags |= (fleft != fright) << 1u;
            processor->flags |= (fleft < fright) << 2u;
            processor->flags &= cmptClearMask;
            processor->flags |= (0x01u << 30u);

            break;
        }

        case OPCODE_DCMP: // REG <=> REG
        {
            const double right = dreg[operands[0]];
            const double left  = dreg[operands[1]];

            processor->flags &= ZSUClearMask;
            processor->flags |= (left != right) << 1u;
            processor->flags |= (left < right) << 2u;
            processor->flags &= cmptClearMask;
            processor->flags |= (0x02u << 30u);

            break;
        }

        case OPCODE_DCMPI:
        {
            const uint64_t iright = (uint64_t)operands[0] << 42u;
            const double   dright = *(const double*)(&iright);
            const double   dleft  = dreg[operands[1]];

            processor->flags &= ZSUClearMask;
            processor->flags |= (dleft != dright) << 1u;
            processor->flags |= (dleft < dright) << 2u;
            processor->flags &= cmptClearMask;
            processor->flags |= (0x01u << 30u);

            break;
        }

        case OPCODE_JMP:   //fallthrough
        case OPCODE_CALL:  //fallthrough
        case OPCODE_JMPR:  //fallthrough
        case OPCODE_CALLR: //fallthrough
        case OPCODE_RET:
            processor->delayedBits |= (1u << index);
            processor->delayed[index] = *op;
            break;
    }

    return AR_SUCCESS;
}

static ArResult executeDelayedInstruction(ArProcessor restrict processor, uint32_t index)
{
    static const uint32_t ZSUClearMask = ~(Z_MASK | S_MASK | U_MASK);
    static const uint32_t retClearMask = ~R_MASK;

    const Operation* restrict op = &processor->delayed[index];
    const uint32_t* restrict const operands = op->operands;

    switch(op->op)
    {
        default:
            return AR_ERROR_ILLEGAL_INSTRUCTION;

        case OPCODE_NOP: //nop.e only
            return AR_END_OF_CODE;

        case OPCODE_XCHG: //Flip XCHG bit
            processor->flags = (processor->flags & 0xFFFFFFFEu) | ((processor->flags & 0x01u) ^ 0x01u);
            break;

        case OPCODE_BNE: // !=
            if(processor->flags & Z_MASK)
            {
                processor->pc = (int32_t)operands[0];
            }

            processor->flags &= ZSUClearMask;
            break;

        case OPCODE_BEQ: // ==
            if(!(processor->flags & Z_MASK))
            {
                processor->pc = (int32_t)operands[0];
            }

            processor->flags &= ZSUClearMask;
            break;

        case OPCODE_BL: // <
            if(processor->flags & U_MASK)
            {
                processor->pc = (int32_t)operands[0];
            }

            processor->flags &= ZSUClearMask;
            break;

        case OPCODE_BLE: // <=
            if((processor->flags & U_MASK) || !(processor->flags & Z_MASK))
            {
                processor->pc = (int32_t)operands[0];
            }

            processor->flags &= ZSUClearMask;
            break;

        case OPCODE_BG: // >
            if(!(processor->flags & U_MASK))
            {
                processor->pc = (int32_t)operands[0];
            }

            processor->flags &= ZSUClearMask;
            break;

        case OPCODE_BGE: // >=
            if(!(processor->flags & U_MASK) || !(processor->flags & Z_MASK))
            {
                processor->pc = (int32_t)operands[0];
            }

            processor->flags &= ZSUClearMask;
            break;

        case OPCODE_BLS: // <
            if(processor->flags & S_MASK)
            {
                processor->pc = (int32_t)operands[0];
            }

            processor->flags &= ZSUClearMask;
            break;

        case OPCODE_BLES: // <=
            if((processor->flags & S_MASK) || !(processor->flags & Z_MASK))
            {
                processor->pc = (int32_t)operands[0];
            }

            processor->flags &= ZSUClearMask;
            break;

        case OPCODE_BGS: // >
            if(!(processor->flags & S_MASK))
            {
                processor->pc = (int32_t)operands[0];
            }

            processor->flags &= ZSUClearMask;
            break;

        case OPCODE_BGES: // >=
            if(!(processor->flags & S_MASK) || !(processor->flags & Z_MASK))
            {
                processor->pc = (int32_t)operands[0];
            }

            processor->flags &= ZSUClearMask;
            break;

        case OPCODE_JMP:
            processor->pc = operands[0];
            break;

        case OPCODE_CALL:
            processor->flags &= retClearMask;
            processor->flags |= (processor->pc << 4u);
            processor->pc = operands[0];
            break;

        case OPCODE_JMPR:
            processor->pc = (int32_t)operands[0];
            break;

        case OPCODE_CALLR:
            processor->flags &= retClearMask;
            processor->flags |= (processor->pc << 4u);
            processor->pc = (int32_t)operands[0];
            break;

        case OPCODE_RET:
            processor->pc = (processor->flags & R_MASK) >> 4u;
            break;
    }

    return AR_SUCCESS;
}

static uint32_t opcodeSetSize(ArProcessor restrict processor)
{
    uint32_t size;
    if(processor->flags & 0x01)
    {
        const uint32_t available = processor->pc - (ISRAM_SIZE / 4u); //we may overflow otherwise
        size = MIN(available, 4u);
    }
    else
    {
        size = 2;
    }

    return size;
}

ArResult arExecuteInstruction(ArProcessor processor)
{
    assert(processor);

    const uint32_t size = opcodeSetSize(processor);

    for(uint32_t i = 0; i < size; ++i)
    {
        if(processor->delayedBits & (1u << i))
        {
            ArResult result = executeDelayedInstruction(processor, i);
            if(result != AR_SUCCESS)
            {
                return result;
            }

            processor->delayedBits &= ~(1u << i);
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

    return AR_SUCCESS;
}

static ArResult copyFromRAM(ArProcessor restrict processor, uint64_t ramAddress, uint8_t* restrict output, size_t size)
{
    ArPhysicalMemory memory = processor->parent->memory; //First memory
    if(!memory)
    {
        return AR_ERROR_ILLEGAL_INSTRUCTION;
    }

    if(ramAddress + size > memory->size)
    {
        return AR_ERROR_PHYSICAL_MEMORY_OUT_OF_RANGE;
    }

    memcpy(output, memory->memory + ramAddress, size);

    return AR_SUCCESS;
}

static ArResult copyToRAM(ArProcessor restrict processor, uint64_t ramAddress, const uint8_t* restrict input, size_t size)
{
    ArPhysicalMemory memory = processor->parent->memory; //First memory
    if(!memory)
    {
        return AR_ERROR_ILLEGAL_INSTRUCTION;
    }

    if(ramAddress + size > memory->size)
    {
        return AR_ERROR_PHYSICAL_MEMORY_OUT_OF_RANGE;
    }

    memcpy(memory->memory + ramAddress, input, size);

    return AR_SUCCESS;
}

static ArResult executeDMA(ArProcessor restrict processor, int store)
{
    //RAM -> SDRAM
    uint64_t* restrict const ireg = processor->ireg;

    const Operation* restrict const op       = &processor->dmaOperation;
    const uint32_t*  restrict const operands = op->operands;

    const uint64_t sramb = (op->data)        & 0x0FFFu;
    const uint64_t ramb  = (op->data >> 12u) & 0x0FFFu;
    const uint64_t sram  = (ireg[operands[0]] + sramb) * 32ull;
    const uint64_t ram   = (ireg[operands[1]] + ramb)  * 32ull;
    const size_t   size  = (op->size + 1u) * 32u;

    if(sram + size > DSRAM_SIZE)
    {
        return AR_ERROR_MEMORY_OUT_OF_RANGE;
    }

    if(store)
    {
        return copyToRAM(processor, ram, processor->dsram + sram, size);
    }
    else
    {
        return copyFromRAM(processor, ram, processor->dsram + sram, size);
    }
}

static ArResult executeDMAR(ArProcessor restrict processor, int store)
{
    //RAM -> SDRAM
    uint64_t* restrict const ireg = processor->ireg;

    const Operation* restrict const op       = &processor->dmaOperation;
    const uint32_t*  restrict const operands = op->operands;

    const uint64_t sram = ireg[operands[0]] * 32ull;
    const uint64_t ram  = ireg[operands[1]] * 32ull;
    const size_t   size = op->size * 32u;

    if(sram + size > DSRAM_SIZE)
    {
        return AR_ERROR_MEMORY_OUT_OF_RANGE;
    }

    if(store)
    {
        return copyToRAM(processor, ram, processor->dsram + sram, size);
    }
    else
    {
        return copyFromRAM(processor, ram, processor->dsram + sram, size);
    }
}

static ArResult executeDMAIR(ArProcessor restrict processor)
{
    //RAM -> SDRAM
    uint64_t* restrict const ireg = processor->ireg;

    const Operation* restrict const op       = &processor->dmaOperation;
    const uint32_t*  restrict const operands = op->operands;

    const uint64_t sram = ireg[operands[0]] * 32ull;
    const uint64_t ram  = ireg[operands[1]] * 32ull;
    const size_t   size = op->size * 32u;

    if(sram + size > ISRAM_SIZE)
    {
        return AR_ERROR_MEMORY_OUT_OF_RANGE;
    }

    return copyFromRAM(processor, ram, processor->isram + sram, size);
}

ArResult arExecuteDirectMemoryAccess(ArProcessor processor)
{
    assert(processor);

    if(processor->dma)
    {
        processor->dma = 0;

        switch(processor->dmaOperation.op)
        {
            default:
                return AR_ERROR_ILLEGAL_INSTRUCTION;

            case OPCODE_LDDMA:
                return executeDMA(processor, 0);

            case OPCODE_STDMA:
                return executeDMA(processor, 1);

            case OPCODE_LDDMAR:
                return executeDMAR(processor, 0);

            case OPCODE_STDMAR:
                return executeDMAR(processor, 1);

            case OPCODE_DMAIR:
                return executeDMAIR(processor);

            case OPCODE_WAIT:
                //We don't have anything to wait since we emulate it based on C memory model
                break;
        }
    }

    return AR_SUCCESS;
}
