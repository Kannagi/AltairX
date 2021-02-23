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

    uint64_t* restrict const ireg = processor->ireg;
    uint64_t* restrict const vreg = processor->vreg;

    const ArOperation* restrict const op = &processor->operations[index];
    const uint32_t*    restrict const operands = op->operands;

    switch(op->op)
    {
        case AR_OPCODE_UNKNOWN:
            return AR_ERROR_ILLEGAL_INSTRUCTION;

        //AGU
        case AR_OPCODE_LDDMA:  //fallthrough
        case AR_OPCODE_STDMA:  //fallthrough
        case AR_OPCODE_LDDMAR: //fallthrough
        case AR_OPCODE_STDMAR: //fallthrough
        case AR_OPCODE_DMAIR:  //fallthrough
        case AR_OPCODE_LDDMAL: //fallthrough
        case AR_OPCODE_STDMAL: //fallthrough
        case AR_OPCODE_CLEARC: //fallthrough
        case AR_OPCODE_WAIT:
            processor->dma = 1;
            processor->dmaOperation = *op;
            break;

        //LSU
        case AR_OPCODE_LDM: //copy data from dsram to register
            memcpy(&ireg[operands[2]], processor->dsram + operands[0] + ireg[operands[1]], 1u << op->size);
            break;

        case AR_OPCODE_LDMI: //copy data from dsram to register
            memcpy(&ireg[operands[2]], processor->dsram + operands[0] + ireg[operands[1]], 1u << op->size);
            ireg[operands[1]] += 1;
            break;

        case AR_OPCODE_STM: //copy data from register to dsram
            memcpy(processor->dsram + operands[0] + ireg[operands[1]], &ireg[operands[2]], 1u << op->size);
            break;

        case AR_OPCODE_STMI: //copy data from register to dsram
            memcpy(processor->dsram + operands[0] + ireg[operands[1]], &ireg[operands[2]], 1u << op->size);
            ireg[operands[1]] += 1;
            break;

        case AR_OPCODE_LDC: //copy data from cache to register
            memcpy(&ireg[operands[2]], processor->cache + operands[0] + ireg[operands[1]], 1u << op->size);
            break;

        case AR_OPCODE_LDCI: //copy data from cache to register
            memcpy(&ireg[operands[2]], processor->cache + operands[0] + ireg[operands[1]], 1u << op->size);
            ireg[operands[1]] += 1;
            break;

        case AR_OPCODE_STC: //copy data from register to cache
            memcpy(processor->cache + operands[0] + ireg[operands[1]], &ireg[operands[2]], 1u << op->size);
            break;

        case AR_OPCODE_STCI: //copy data from register to cache
            memcpy(processor->cache + operands[0] + ireg[operands[1]], &ireg[operands[2]], 1u << op->size);
            ireg[operands[1]] += 1;
            break;

        case AR_OPCODE_IN: //copy data from iosram to register
            memcpy(&ireg[operands[2]], processor->iosram + operands[0], 1u << op->size);
            break;

        case AR_OPCODE_OUT: //copy data from register to iosram
            memcpy(processor->iosram + operands[0], &ireg[operands[2]], 1u << op->size);
            break;

        case AR_OPCODE_OUTI: //write data to iosram
            memcpy(processor->iosram + operands[0], &operands[2], 1u << op->size);
            break;

        case AR_OPCODE_LDMV: //copy data from dsram to vector register
            memcpy(&vreg[operands[2]], processor->dsram + (operands[0] + ireg[operands[1]]) * 8, 2 * (op->size + 1));
            break;

        case AR_OPCODE_LDMVI: //copy data from dsram to vector register
            memcpy(&vreg[operands[2]], processor->dsram + (operands[0] + ireg[operands[1]]) * 8, 2 * (op->size + 1));
            ireg[operands[1]] += 1;
            break;

        case AR_OPCODE_STMV: //copy data from vector register to dsram
            memcpy(processor->dsram + (operands[0] + ireg[operands[1]]) * 8, &vreg[operands[2]], 2 * (op->size + 1));
            break;

        case AR_OPCODE_STMVI: //copy data from vector register to dsram
            memcpy(processor->dsram + (operands[0] + ireg[operands[1]]) * 8, &vreg[operands[2]], 2 * (op->size + 1));
            ireg[operands[1]] += 1;
            break;

        case AR_OPCODE_LDCV: //copy data from cache to vector register
            memcpy(&vreg[operands[2]], processor->cache + (operands[0] + ireg[operands[1]]) * 8, 2 * (op->size + 1));
            break;

        case AR_OPCODE_LDCVI: //copy data from cache to vector register
            memcpy(&vreg[operands[2]], processor->cache + (operands[0] + ireg[operands[1]]) * 8, 2 * (op->size + 1));
            ireg[operands[1]] += 1;
            break;

        case AR_OPCODE_STCV: //copy data from vector register to cache
            memcpy(processor->cache + (operands[0] + ireg[operands[1]]) * 8, &vreg[operands[2]], 2 * (op->size + 1));
            break;

        case AR_OPCODE_STCVI: //copy data from vector register to cache
            memcpy(processor->cache + (operands[0] + ireg[operands[1]]) * 8, &vreg[operands[2]], 2 * (op->size + 1));
            ireg[operands[1]] += 1;
            break;

        //ALU
        case AR_OPCODE_NOP: //Nope
            break;

        case AR_OPCODE_MOVEI: //Write a value to a register
            ireg[operands[2]] = operands[0];
            break;

        case AR_OPCODE_MOVELRL: //Write LR value to a registre
            ireg[operands[0]] = processor->lr;
            break;

        case AR_OPCODE_MOVELRS: //Write a value to LR
            processor->lr = ireg[operands[0]];
            break;

        case AR_OPCODE_MOVEBR: //Write a value to BR
            processor->br = ireg[operands[0]];
            break;

        case AR_OPCODE_ADD: //REG = REG + REG
            ireg[operands[2]] = ireg[operands[1]] + ireg[operands[0]];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_ADDI: //REG = REG + IMM
            ireg[operands[2]] = ireg[operands[1]] + operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_ADDQ: //REG += IMM
            ireg[operands[2]] += operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_SUB: //REG = REG - REG
            ireg[operands[2]] = ireg[operands[1]] - ireg[operands[0]];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_SUBI: //REG = REG - IMM
            ireg[operands[2]] = ireg[operands[1]] - operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_SUBQ: //REG -= IMM
            ireg[operands[2]] -= operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_MULS: //REG = REG * REG (signed)
            (*(int64_t*)ireg[operands[2]]) = (int64_t)ireg[operands[1]] * (int64_t)ireg[operands[0]];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_MULSI: //REG = REG * IMM (signed)
            (*(int64_t*)ireg[operands[2]]) = (int64_t)ireg[operands[1]] * (int64_t)operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_MULSQ: //REG *= IMM (signed)
            (*(int64_t*)ireg[operands[2]]) *= (int64_t)operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_MULU: //REG = REG * REG
            ireg[operands[2]] = ireg[operands[1]] * ireg[operands[0]];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_MULUI: //REG = REG * IMM
            ireg[operands[2]] = ireg[operands[1]] * operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_MULUQ: //REG *= IMM
            ireg[operands[2]] *= operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_DIVS: //REG = REG / REG (signed)
            (*(int64_t*)ireg[operands[2]]) = (int64_t)ireg[operands[1]] / (int64_t)ireg[operands[0]];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_DIVSI: //REG = REG / IMM (signed)
            (*(int64_t*)ireg[operands[2]]) = (int64_t)ireg[operands[1]] / (int64_t)operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_DIVSQ: //REG /= IMM (signed)
            (*(int64_t*)ireg[operands[2]]) /= (int64_t)operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_DIVU: //REG = REG / REG
            ireg[operands[2]] = ireg[operands[1]] / ireg[operands[0]];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_DIVUI: //REG = REG / IMM
            ireg[operands[2]] = ireg[operands[1]] / operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_DIVUQ: //REG /= IMM
            ireg[operands[2]] /= operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_AND: //REG = REG & REG
            ireg[operands[2]] = ireg[operands[1]] & ireg[operands[0]];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_ANDI: //REG = REG & IMM
            ireg[operands[2]] = ireg[operands[1]] & operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_ANDQ: //REG &= IMM
            ireg[operands[2]] &= operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_OR: //REG = REG | REG
            ireg[operands[2]] = ireg[operands[1]] | ireg[operands[0]];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_ORI: //REG = REG | IMM
            ireg[operands[2]] = ireg[operands[1]] | operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_ORQ: //REG |= IMM
            ireg[operands[2]] |= operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_XOR: //REG = REG ^ REG
            ireg[operands[2]] = ireg[operands[1]] ^ ireg[operands[0]];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_XORI: //REG = REG ^ IMM
            ireg[operands[2]] = ireg[operands[1]] ^ operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_XORQ: //REG ^= IMM
            ireg[operands[2]] ^= operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_ASL: //REG = REG << REG (signed)
            (*(int64_t*)ireg[operands[2]]) = (int64_t)ireg[operands[1]] << (int64_t)ireg[operands[0]];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_ASLI: //REG = REG << IMM (signed)
            (*(int64_t*)ireg[operands[2]]) = (int64_t)ireg[operands[1]] << (int64_t)operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_ASLQ: //REG <<= IMM (signed)
            (*(int64_t*)ireg[operands[2]]) <<= (int64_t)operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_LSL: //REG = REG << REG
            ireg[operands[2]] = ireg[operands[1]] << ireg[operands[0]];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_LSLI: //REG = REG << IMM
            ireg[operands[2]] = ireg[operands[1]] << operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_LSLQ: //REG <<= IMM
            ireg[operands[2]] <<= operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_ASR: //REG = REG >> REG (signed)
            (*(int64_t*)ireg[operands[2]]) = (int64_t)ireg[operands[1]] >> (int64_t)ireg[operands[0]];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_ASRI: //REG = REG >> IMM (signed)
            (*(int64_t*)ireg[operands[2]]) = (int64_t)ireg[operands[1]] >> (int64_t)operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_ASRQ: //REG >>= IMM (signed)
            (*(int64_t*)ireg[operands[2]]) >>= (int64_t)operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_LSR: //REG = REG >> REG
            ireg[operands[2]] = ireg[operands[1]] >> ireg[operands[0]];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_LSRI: //REG = REG >> IMM
            ireg[operands[2]] = ireg[operands[1]] >> operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_LSRQ: //REG >>= IMM
            ireg[operands[2]] >>= operands[0];
            ireg[operands[2]] &= sizemask[op->size];
            break;

        //CMP

        case AR_OPCODE_CMP: // REG <=> REG
        {
            const uint64_t right = ireg[operands[0]] & sizemask[op->size];
            const uint64_t left  = ireg[operands[1]] & sizemask[op->size];

            processor->flags &= ZSUClearMask;
            processor->flags |= (left != right) << 1u;
            processor->flags |= ((int64_t)left < (int64_t)right) << 2u;
            processor->flags |= (left < right) << 3u;

            break;
        }

        case AR_OPCODE_CMPI: // REG <=> IMM
        {
            const uint64_t right = operands[0] & sizemask[op->size];
            const uint64_t left  = ireg[operands[1]] & sizemask[op->size];

            processor->flags &= ZSUClearMask;
            processor->flags |= (left != right) << 1u;
            processor->flags |= ((int64_t)left < (int64_t)right) << 2u;
            processor->flags |= (left < right) << 3u;

            break;
        }

        case AR_OPCODE_PCMP: //TODO
            break;
        case AR_OPCODE_PCMPI: //TODO
            break;

        //BRU
        case AR_OPCODE_BNE:   //fallthrough
        case AR_OPCODE_BEQ:   //fallthrough
        case AR_OPCODE_BL:    //fallthrough
        case AR_OPCODE_BLE:   //fallthrough
        case AR_OPCODE_BG:    //fallthrough
        case AR_OPCODE_BGE:   //fallthrough
        case AR_OPCODE_BLS:   //fallthrough
        case AR_OPCODE_BLES:  //fallthrough
        case AR_OPCODE_BGS:   //fallthrough
        case AR_OPCODE_BGES:  //fallthrough
        case AR_OPCODE_BRA:   //fallthrough
        case AR_OPCODE_JMP:   //fallthrough
        case AR_OPCODE_CALL:  //fallthrough
        case AR_OPCODE_JMPR:  //fallthrough
        case AR_OPCODE_CALLR: //fallthrough
        case AR_OPCODE_RET:   //fallthrough
        case AR_OPCODE_SWT:
            processor->delayedBits |= (1u << index);
            processor->delayed[index] = *op;
            break;

        case AR_OPCODE_ENDP:
            return AR_END_OF_CODE;

        case AR_OPCODE_MOVE: //Should never happens
            return AR_ERROR_ILLEGAL_INSTRUCTION;
    }

    return AR_SUCCESS;
}

static ArResult executeDelayedInstruction(ArProcessor restrict processor, uint32_t index)
{
    static const uint32_t ZSUClearMask = ~(Z_MASK | S_MASK | U_MASK);

    const ArOperation* restrict op = &processor->delayed[index];
    const uint32_t*    restrict const operands = op->operands;

    switch(op->op)
    {
        default:
            return AR_ERROR_ILLEGAL_INSTRUCTION;

        case AR_OPCODE_BNE: // !=
            if(processor->flags & Z_MASK)
            {
                processor->pc = operands[0];
            }

            processor->flags &= ZSUClearMask;
            break;

        case AR_OPCODE_BEQ: // ==
            if(!(processor->flags & Z_MASK))
            {
                processor->pc = operands[0];
            }

            processor->flags &= ZSUClearMask;
            break;

        case AR_OPCODE_BL: // <
            if(processor->flags & U_MASK)
            {
                processor->pc = operands[0];
            }

            processor->flags &= ZSUClearMask;
            break;

        case AR_OPCODE_BLE: // <=
            if((processor->flags & U_MASK) || !(processor->flags & Z_MASK))
            {
                processor->pc = operands[0];
            }

            processor->flags &= ZSUClearMask;
            break;

        case AR_OPCODE_BG: // >
            if(!(processor->flags & U_MASK))
            {
                processor->pc = operands[0];
            }

            processor->flags &= ZSUClearMask;
            break;

        case AR_OPCODE_BGE: // >=
            if(!(processor->flags & U_MASK) || !(processor->flags & Z_MASK))
            {
                processor->pc = operands[0];
            }

            processor->flags &= ZSUClearMask;
            break;

        case AR_OPCODE_BLS: // <
            if(processor->flags & S_MASK)
            {
                processor->pc = operands[0];
            }

            processor->flags &= ZSUClearMask;
            break;

        case AR_OPCODE_BLES: // <=
            if((processor->flags & S_MASK) || !(processor->flags & Z_MASK))
            {
                processor->pc = operands[0];
            }

            processor->flags &= ZSUClearMask;
            break;

        case AR_OPCODE_BGS: // >
            if(!(processor->flags & S_MASK))
            {
                processor->pc = operands[0];
            }

            processor->flags &= ZSUClearMask;
            break;

        case AR_OPCODE_BGES: // >=
            if(!(processor->flags & S_MASK) || !(processor->flags & Z_MASK))
            {
                processor->pc = operands[0];
            }

            processor->flags &= ZSUClearMask;
            break;

        case AR_OPCODE_BRA:
            processor->pc = operands[0];
            break;

        case AR_OPCODE_JMP:
            processor->pc = operands[0];
            break;

        case AR_OPCODE_CALL:
            processor->pc = operands[0];
            processor->lr = operands[1];
            break;

        case AR_OPCODE_JMPR:
            processor->pc = operands[0];
            break;

        case AR_OPCODE_CALLR:
            processor->pc = operands[0];
            processor->lr = operands[1];
            break;

        case AR_OPCODE_RET:
            processor->pc = processor->lr;
            break;

        case AR_OPCODE_SWT: //Flip XCHG bit
            processor->flags = (processor->flags & 0xFFFFFFFEu) | operands[0];
            break;
    }

    return AR_SUCCESS;
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
        }
    }

    processor->delayedBits = 0;

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

static ArResult copyFromRAM(ArPhysicalMemory memory, uint64_t memoryAddress, uint8_t* restrict output, size_t size)
{
    if(memoryAddress + size > memory->size)
    {
        return AR_ERROR_PHYSICAL_MEMORY_OUT_OF_RANGE;
    }

    memcpy(output, memory->memory + memoryAddress, size);

    return AR_SUCCESS;
}

static ArResult copyToRAM(ArPhysicalMemory memory, uint64_t memoryAddress, const uint8_t* restrict input, size_t size)
{
    if(memoryAddress + size > memory->size)
    {
        return AR_ERROR_PHYSICAL_MEMORY_OUT_OF_RANGE;
    }

    memcpy(memory->memory + memoryAddress, input, size);

    return AR_SUCCESS;
}

static ArResult executeDMA(ArProcessor restrict processor, int store)
{/*
    //RAM -> SDRAM
    uint64_t* restrict const ireg = processor->ireg;

    const ArOperation* restrict const op       = &processor->dmaOperation;
    const uint32_t*    restrict const operands = op->operands;

    const uint64_t sramb = (op->data)        & 0x0FFFu;
    const uint64_t ramb  = (op->data >> 12u) & 0x0FFFu;
    const uint64_t sram  = (ireg[operands[0]] + sramb) * 32ull;
    const uint64_t ram   = (ireg[operands[1]] + ramb)  * 32ull;
    const size_t   size  = (op->size + 1u) * 32u;

    if(sram + size > AR_PROCESSOR_DSRAM_SIZE)
    {
        return AR_ERROR_MEMORY_OUT_OF_RANGE;
    }

    if(ram > MEMORY_MAP_RAM_BEGIN)
    {
        if(store)
        {
            return copyToRAM(, ram - MEMORY_MAP_RAM_BEGIN, processor->dsram + sram, size);
        }
        else
        {
            return copyFromRAM(processor, ram - MEMORY_MAP_RAM_BEGIN, processor->dsram + sram, size);
        }
    }
    else
    {
        return AR_ERROR_ILLEGAL_INSTRUCTION;
    }*/

    (void)processor;
    (void)store;

    return AR_ERROR_ILLEGAL_INSTRUCTION;
}

static ArResult executeDMAR(ArProcessor restrict processor, int store)
{
    //RAM -> SDRAM
    uint64_t* restrict const ireg = processor->ireg;

    const ArOperation* restrict const op       = &processor->dmaOperation;
    const uint32_t*    restrict const operands = op->operands;

    const size_t   size = ireg[operands[0]] * 32u;
    const uint64_t ram  = ireg[operands[1]] * 32ull;
    const uint64_t sram = ireg[operands[2]] * 32ull;

    if(sram + size > AR_PROCESSOR_DSRAM_SIZE)
    {
        return AR_ERROR_MEMORY_OUT_OF_RANGE;
    }

    if(ram > MEMORY_MAP_RAM_BEGIN)
    {
        ArPhysicalMemory* memory = getMemoryByRole(processor->parent, AR_PHYSICAL_MEMORY_ROLE_RAM);

        if(!*memory)
        {
            return AR_ERROR_PHYSICAL_MEMORY_OUT_OF_RANGE;
        }

        if(store)
        {
            return copyToRAM(*memory, ram - MEMORY_MAP_RAM_BEGIN, processor->dsram + sram, size);
        }
        else
        {
            return copyFromRAM(*memory, ram - MEMORY_MAP_RAM_BEGIN, processor->dsram + sram, size);
        }
    }
    else
    {
        ArPhysicalMemory* memory = getMemoryByRole(processor->parent, AR_PHYSICAL_MEMORY_ROLE_ROM);

        if(!*memory)
        {
            return AR_ERROR_PHYSICAL_MEMORY_OUT_OF_RANGE;
        }

        if(store)
        {
            return copyToRAM(*memory, ram - MEMORY_MAP_ROM_BEGIN, processor->dsram + sram, size);
        }
        else
        {
            return copyFromRAM(*memory, ram - MEMORY_MAP_ROM_BEGIN, processor->dsram + sram, size);
        }
    }
}

static ArResult executeDMAIR(ArProcessor restrict processor)
{/*
    //RAM -> SDRAM
    uint64_t* restrict const ireg = processor->ireg;

    const ArOperation* restrict const op       = &processor->dmaOperation;
    const uint32_t*    restrict const operands = op->operands;

    const uint64_t sram = ireg[operands[0]] * 32ull;
    const uint64_t ram  = ireg[operands[1]] * 32ull;
    const size_t   size = op->size * 32u;

    if(sram + size > AR_PROCESSOR_ISRAM_SIZE)
    {
        return AR_ERROR_MEMORY_OUT_OF_RANGE;
    }

    if(ram > MEMORY_MAP_RAM_BEGIN)
    {
        return copyFromRAM(processor, ram, processor->isram + sram, size);
    }
    else
    {
        return AR_ERROR_ILLEGAL_INSTRUCTION;
    }*/

    (void)processor;

    return AR_ERROR_ILLEGAL_INSTRUCTION;
}

static ArResult executeDMAL(ArProcessor restrict processor, int store)
{/*
    //RAM -> SDRAM
    uint64_t* restrict const ireg = processor->ireg;

    const ArOperation* restrict const op       = &processor->dmaOperation;
    const uint32_t*    restrict const operands = op->operands;

    const uint64_t sram = ireg[operands[0]] * 32ull;
    const uint64_t ram  = ireg[operands[1]] * 32ull;
    const size_t   size = op->size * 32u;

    if(sram + size > AR_PROCESSOR_DSRAM_SIZE)
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
    }*/

    //TODO

    (void)processor;
    (void)store;

    return AR_ERROR_ILLEGAL_INSTRUCTION;
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

            case AR_OPCODE_LDDMA:
                return executeDMA(processor, 0);

            case AR_OPCODE_STDMA:
                return executeDMA(processor, 1);

            case AR_OPCODE_LDDMAR:
                return executeDMAR(processor, 0);

            case AR_OPCODE_STDMAR:
                return executeDMAR(processor, 1);

            case AR_OPCODE_DMAIR:
                return executeDMAIR(processor);

            case AR_OPCODE_LDDMAL:
                return executeDMAL(processor, 0);

            case AR_OPCODE_STDMAL:
                return executeDMAL(processor, 1);

            case AR_OPCODE_CLEARC: //fallthrough
            case AR_OPCODE_WAIT:
                //We don't have anything to wait since we emulate it based on C memory model, all transfers are direcly coherent
                break;
        }
    }

    return AR_SUCCESS;
}
