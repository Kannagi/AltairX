#include "graphics_processor.hpp"
#include "memory.hpp"

#include <cassert>
#include <cmath>
#include <string>

ArResult executeInstruction(ArGraphicsProcessor AR_RESTRICT graphicsProcessor, uint32_t index)
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

    auto& reg = graphicsProcessor->reg;

    const ArOperation* AR_RESTRICT const op = &graphicsProcessor->operations[index];
    const uint32_t*    AR_RESTRICT const operands = op->operands;

    switch(op->op)
    {
        case AR_OPCODE_UNKNOWN:
            return AR_ERROR_ILLEGAL_INSTRUCTION;

        //LSU
        case AR_OPCODE_LDM: //copy data from dsram to register
            memcpy(&reg[operands[2]], graphicsProcessor->dsram.data() + reg[operands[0]].vectori[0] + reg[operands[1]].vectori[0], 1ull << op->size);
            break;

        case AR_OPCODE_LDMI: //copy data from dsram to register
            memcpy(&reg[operands[2]], graphicsProcessor->dsram.data() + operands[0] + reg[operands[1]].vectori[0], 1ull << op->size);
            break;

        case AR_OPCODE_STM: //copy data from register to dsram
            memcpy(graphicsProcessor->dsram.data() + operands[0] + reg[operands[1]], &reg[operands[2]], 1ull << op->size);
            break;

        case AR_OPCODE_STMI: //copy data from register to dsram
            memcpy(graphicsProcessor->dsram.data() + operands[0] + reg[operands[1]], &reg[operands[2]], 1ull << op->size);
            reg[operands[1]] += 1;
            break;

        case AR_OPCODE_LDC: //copy data from cache to register
            memcpy(&reg[operands[2]], graphicsProcessor->cache + operands[0] + reg[operands[1]], 1ull << op->size);
            break;

        case AR_OPCODE_LDCI: //copy data from cache to register
            memcpy(&reg[operands[2]], graphicsProcessor->cache + operands[0] + reg[operands[1]], 1ull << op->size);
            reg[operands[1]] += 1;
            break;

        case AR_OPCODE_STC: //copy data from register to cache
            memcpy(graphicsProcessor->cache + operands[0] + reg[operands[1]], &reg[operands[2]], 1ull << op->size);
            break;

        case AR_OPCODE_STCI: //copy data from register to cache
            memcpy(graphicsProcessor->cache + operands[0] + reg[operands[1]], &reg[operands[2]], 1ull << op->size);
            reg[operands[1]] += 1;
            break;

        case AR_OPCODE_IN: //copy data from iosram to register
            memcpy(&reg[operands[2]], graphicsProcessor->iosram + operands[0], 1ull << op->size);
            break;

        case AR_OPCODE_OUT: //copy data from register to iosram
            memcpy(graphicsProcessor->iosram + operands[0], &reg[operands[2]], 1ull << op->size);
            break;

        case AR_OPCODE_OUTI: //write data to iosram
            memcpy(graphicsProcessor->iosram + operands[0], &operands[2], 1ull << op->size);
            break;

        case AR_OPCODE_LDMV: //copy data from dsram to vector register
            memcpy(&vreg[operands[2]], graphicsProcessor->dsram + (operands[0] + reg[operands[1]]) * 8, 2 * (op->size + 1));
            break;

        case AR_OPCODE_LDMVI: //copy data from dsram to vector register
            memcpy(&vreg[operands[2]], graphicsProcessor->dsram + (operands[0] + reg[operands[1]]) * 8, 2 * (op->size + 1));
            reg[operands[1]] += 1;
            break;

        case AR_OPCODE_STMV: //copy data from vector register to dsram
            memcpy(graphicsProcessor->dsram + (operands[0] + reg[operands[1]]) * 8, &vreg[operands[2]], 2 * (op->size + 1));
            break;

        case AR_OPCODE_STMVI: //copy data from vector register to dsram
            memcpy(graphicsProcessor->dsram + (operands[0] + reg[operands[1]]) * 8, &vreg[operands[2]], 2 * (op->size + 1));
            reg[operands[1]] += 1;
            break;

        case AR_OPCODE_LDCV: //copy data from cache to vector register
            memcpy(&vreg[operands[2]], graphicsProcessor->cache + (operands[0] + reg[operands[1]]) * 8, 2 * (op->size + 1));
            break;

        case AR_OPCODE_LDCVI: //copy data from cache to vector register
            memcpy(&vreg[operands[2]], graphicsProcessor->cache + (operands[0] + reg[operands[1]]) * 8, 2 * (op->size + 1));
            reg[operands[1]] += 1;
            break;

        case AR_OPCODE_STCV: //copy data from vector register to cache
            memcpy(graphicsProcessor->cache + (operands[0] + reg[operands[1]]) * 8, &vreg[operands[2]], 2 * (op->size + 1));
            break;

        case AR_OPCODE_STCVI: //copy data from vector register to cache
            memcpy(graphicsProcessor->cache + (operands[0] + reg[operands[1]]) * 8, &vreg[operands[2]], 2 * (op->size + 1));
            reg[operands[1]] += 1;
            break;

        //ALU
        case AR_OPCODE_NOP: //Nope
            break;

        case AR_OPCODE_MOVEI: //Write a value to a register
            reg[operands[2]] = operands[0];
            break;

        case AR_OPCODE_MOVELRL: //Write LR value to a registre
            reg[operands[0]] = graphicsProcessor->lr;
            break;

        case AR_OPCODE_MOVELRS: //Write a value to LR
            graphicsProcessor->lr = reg[operands[0]];
            break;

        case AR_OPCODE_MOVEBR: //Write a value to BR
            graphicsProcessor->br = reg[operands[0]];
            break;

        case AR_OPCODE_ADD: //REG = REG + REG
            reg[operands[2]] = reg[operands[1]] + reg[operands[0]];
            reg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_ADDI: //REG = REG + IMM
            reg[operands[2]] = reg[operands[1]] + operands[0];
            reg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_ADDQ: //REG += IMM
            reg[operands[2]] += operands[0];
            reg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_SUB: //REG = REG - REG
            reg[operands[2]] = reg[operands[1]] - reg[operands[0]];
            reg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_SUBI: //REG = REG - IMM
            reg[operands[2]] = reg[operands[1]] - operands[0];
            reg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_SUBQ: //REG -= IMM
            reg[operands[2]] -= operands[0];
            reg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_MULS: //REG = REG * REG (signed)
            reg[operands[2]] = (uint64_t)((int64_t)reg[operands[1]] * (int64_t)reg[operands[0]]);
            reg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_MULSI: //REG = REG * IMM (signed)
            reg[operands[2]] = (uint64_t)((int64_t)reg[operands[1]] * (int64_t)operands[0]);
            reg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_MULSQ: //REG *= IMM (signed)
            reg[operands[2]] = (uint64_t)((int64_t)reg[operands[2]] * (int64_t)operands[0]);
            reg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_MULU: //REG = REG * REG
            reg[operands[2]] = reg[operands[1]] * reg[operands[0]];
            reg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_MULUI: //REG = REG * IMM
            reg[operands[2]] = reg[operands[1]] * operands[0];
            reg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_MULUQ: //REG *= IMM
            reg[operands[2]] *= operands[0];
            reg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_DIVS: //REG = REG / REG (signed)
            reg[operands[2]] = (uint64_t)((int64_t)reg[operands[1]] / (int64_t)reg[operands[0]]);
            reg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_DIVSI: //REG = REG / IMM (signed)
            reg[operands[2]] = (uint64_t)((int64_t)reg[operands[1]] / (int64_t)operands[0]);
            reg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_DIVSQ: //REG /= IMM (signed)
            reg[operands[2]] = (uint64_t)((int64_t)reg[operands[2]] / (int64_t)operands[0]);
            reg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_DIVU: //REG = REG / REG
            reg[operands[2]] = reg[operands[1]] / reg[operands[0]];
            reg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_DIVUI: //REG = REG / IMM
            reg[operands[2]] = reg[operands[1]] / operands[0];
            reg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_DIVUQ: //REG /= IMM
            reg[operands[2]] /= operands[0];
            reg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_AND: //REG = REG & REG
            reg[operands[2]] = reg[operands[1]] & reg[operands[0]];
            reg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_ANDI: //REG = REG & IMM
            reg[operands[2]] = reg[operands[1]] & operands[0];
            reg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_ANDQ: //REG &= IMM
            reg[operands[2]] &= operands[0];
            reg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_OR: //REG = REG | REG
            reg[operands[2]] = reg[operands[1]] | reg[operands[0]];
            reg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_ORI: //REG = REG | IMM
            reg[operands[2]] = reg[operands[1]] | operands[0];
            reg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_ORQ: //REG |= IMM
            reg[operands[2]] |= operands[0];
            reg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_XOR: //REG = REG ^ REG
            reg[operands[2]] = reg[operands[1]] ^ reg[operands[0]];
            reg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_XORI: //REG = REG ^ IMM
            reg[operands[2]] = reg[operands[1]] ^ operands[0];
            reg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_XORQ: //REG ^= IMM
            reg[operands[2]] ^= operands[0];
            reg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_ASL: //REG = REG << REG (signed)
            reg[operands[2]] = (uint64_t)((int64_t)reg[operands[1]] << (int64_t)reg[operands[0]]);
            reg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_ASLI: //REG = REG << IMM (signed)
            reg[operands[2]] = (uint64_t)((int64_t)reg[operands[1]] << (int64_t)operands[0]);
            reg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_ASLQ: //REG <<= IMM (signed)
            reg[operands[2]] = (uint64_t)((int64_t)reg[operands[2]] << (int64_t)operands[0]);
            reg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_LSL: //REG = REG << REG
            reg[operands[2]] = reg[operands[1]] << reg[operands[0]];
            reg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_LSLI: //REG = REG << IMM
            reg[operands[2]] = reg[operands[1]] << operands[0];
            reg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_LSLQ: //REG <<= IMM
            reg[operands[2]] <<= operands[0];
            reg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_ASR: //REG = REG >> REG (signed)
            reg[operands[2]] = (uint64_t)((int64_t)reg[operands[1]] >> (int64_t)reg[operands[0]]);
            reg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_ASRI: //REG = REG >> IMM (signed)
            reg[operands[2]] = (uint64_t)((int64_t)reg[operands[1]] >> (int64_t)operands[0]);
            reg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_ASRQ: //REG >>= IMM (signed)
            reg[operands[2]] = (uint64_t)((int64_t)reg[operands[2]] >> (int64_t)operands[0]);
            reg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_LSR: //REG = REG >> REG
            reg[operands[2]] = reg[operands[1]] >> reg[operands[0]];
            reg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_LSRI: //REG = REG >> IMM
            reg[operands[2]] = reg[operands[1]] >> operands[0];
            reg[operands[2]] &= sizemask[op->size];
            break;

        case AR_OPCODE_LSRQ: //REG >>= IMM
            reg[operands[2]] >>= operands[0];
            reg[operands[2]] &= sizemask[op->size];
            break;

        //CMP

        case AR_OPCODE_CMP: // REG <=> REG
        {
            const uint64_t right = reg[operands[0]] & sizemask[op->size];
            const uint64_t left  = reg[operands[1]] & sizemask[op->size];

            graphicsProcessor->flags &= ZSUClearMask;
            graphicsProcessor->flags |= (left != right) << 1u;
            graphicsProcessor->flags |= ((int64_t)left < (int64_t)right) << 2u;
            graphicsProcessor->flags |= (left < right) << 3u;

            break;
        }

        case AR_OPCODE_CMPI: // REG <=> IMM
        {
            const uint64_t right = operands[0] & sizemask[op->size];
            const uint64_t left  = reg[operands[1]] & sizemask[op->size];

            graphicsProcessor->flags &= ZSUClearMask;
            graphicsProcessor->flags |= (left != right) << 1u;
            graphicsProcessor->flags |= ((int64_t)left < (int64_t)right) << 2u;
            graphicsProcessor->flags |= (left < right) << 3u;

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
            graphicsProcessor->delayedBits |= (1ull << index);
            graphicsProcessor->delayed[index] = *op;
            break;

        case AR_OPCODE_ENDP:
            return AR_END_OF_CODE;

        case AR_OPCODE_MOVE: //Should never happens
            return AR_ERROR_ILLEGAL_INSTRUCTION;
    }

    return AR_SUCCESS;
}

ArResult executeDelayedInstruction(ArGraphicsProcessor AR_RESTRICT graphicsProcessor, uint32_t index)
{
    static const uint32_t ZSUClearMask = ~(Z_MASK | S_MASK | U_MASK);

    const ArOperation* AR_RESTRICT op = &graphicsProcessor->delayed[index];
    const uint32_t*    AR_RESTRICT const operands = op->operands;

    switch(op->op)
    {
        default:
            return AR_ERROR_ILLEGAL_INSTRUCTION;

        case AR_OPCODE_BNE: // !=
            if(graphicsProcessor->flags & Z_MASK)
            {
                graphicsProcessor->pc = operands[0];
            }

            graphicsProcessor->flags &= ZSUClearMask;
            break;

        case AR_OPCODE_BEQ: // ==
            if(!(graphicsProcessor->flags & Z_MASK))
            {
                graphicsProcessor->pc = operands[0];
            }

            graphicsProcessor->flags &= ZSUClearMask;
            break;

        case AR_OPCODE_BL: // <
            if(graphicsProcessor->flags & U_MASK)
            {
                graphicsProcessor->pc = operands[0];
            }

            graphicsProcessor->flags &= ZSUClearMask;
            break;

        case AR_OPCODE_BLE: // <=
            if((graphicsProcessor->flags & U_MASK) || !(graphicsProcessor->flags & Z_MASK))
            {
                graphicsProcessor->pc = operands[0];
            }

            graphicsProcessor->flags &= ZSUClearMask;
            break;

        case AR_OPCODE_BG: // >
            if(!(graphicsProcessor->flags & U_MASK))
            {
                graphicsProcessor->pc = operands[0];
            }

            graphicsProcessor->flags &= ZSUClearMask;
            break;

        case AR_OPCODE_BGE: // >=
            if(!(graphicsProcessor->flags & U_MASK) || !(graphicsProcessor->flags & Z_MASK))
            {
                graphicsProcessor->pc = operands[0];
            }

            graphicsProcessor->flags &= ZSUClearMask;
            break;

        case AR_OPCODE_BLS: // <
            if(graphicsProcessor->flags & S_MASK)
            {
                graphicsProcessor->pc = operands[0];
            }

            graphicsProcessor->flags &= ZSUClearMask;
            break;

        case AR_OPCODE_BLES: // <=
            if((graphicsProcessor->flags & S_MASK) || !(graphicsProcessor->flags & Z_MASK))
            {
                graphicsProcessor->pc = operands[0];
            }

            graphicsProcessor->flags &= ZSUClearMask;
            break;

        case AR_OPCODE_BGS: // >
            if(!(graphicsProcessor->flags & S_MASK))
            {
                graphicsProcessor->pc = operands[0];
            }

            graphicsProcessor->flags &= ZSUClearMask;
            break;

        case AR_OPCODE_BGES: // >=
            if(!(graphicsProcessor->flags & S_MASK) || !(graphicsProcessor->flags & Z_MASK))
            {
                graphicsProcessor->pc = operands[0];
            }

            graphicsProcessor->flags &= ZSUClearMask;
            break;

        case AR_OPCODE_BRA:
            graphicsProcessor->pc = operands[0];
            break;

        case AR_OPCODE_JMP:
            graphicsProcessor->pc = operands[0];
            break;

        case AR_OPCODE_CALL:
            graphicsProcessor->pc = operands[0];
            graphicsProcessor->lr = operands[1];
            break;

        case AR_OPCODE_JMPR:
            graphicsProcessor->pc = operands[0];
            break;

        case AR_OPCODE_CALLR:
            graphicsProcessor->pc = operands[0];
            graphicsProcessor->lr = operands[1];
            break;

        case AR_OPCODE_RET:
            graphicsProcessor->pc = graphicsProcessor->lr;
            break;

        case AR_OPCODE_SWT: //Flip XCHG bit
            graphicsProcessor->flags = (graphicsProcessor->flags & 0xFFFFFFFEu) | operands[0];
            break;
    }

    return AR_SUCCESS;
}

ArResult arExecuteInstruction(ArGraphicsProcessor graphicsProcessor)
{
    assert(graphicsProcessor);

    const uint32_t size = opcodeSetSize(graphicsProcessor);

    for(uint32_t i = 0; i < size; ++i)
    {
        if(graphicsProcessor->delayedBits & (1ull << i))
        {
            ArResult result = executeDelayedInstruction(graphicsProcessor, i);
            if(result != AR_SUCCESS)
            {
                return result;
            }
        }
    }

    graphicsProcessor->delayedBits = 0;

    for(uint32_t i = 0; i < size; ++i)
    {
        ArResult result = executeInstruction(graphicsProcessor, i);
        if(result != AR_SUCCESS)
        {
            return result;
        }
    }

    return AR_SUCCESS;
}

ArResult executeDMA(ArGraphicsProcessor AR_RESTRICT graphicsProcessor, int store)
{/*
    //RAM -> SDRAM
    uint64_t* AR_RESTRICT const reg = graphicsProcessor->reg;

    const ArOperation* AR_RESTRICT const op       = &graphicsProcessor->dmaOperation;
    const uint32_t*    AR_RESTRICT const operands = op->operands;

    const uint64_t sramb = (op->data)        & 0x0FFFu;
    const uint64_t ramb  = (op->data >> 12u) & 0x0FFFu;
    const uint64_t sram  = (reg[operands[0]] + sramb) * 32ull;
    const uint64_t ram   = (reg[operands[1]] + ramb)  * 32ull;
    const size_t   size  = (op->size + 1u) * 32u;

    if(sram + size > AR_PROCESSOR_DSRAM_SIZE)
    {
        return AR_ERROR_MEMORY_OUT_OF_RANGE;
    }

    if(ram > MEMORY_MAP_RAM_BEGIN)
    {
        if(store)
        {
            return copyToRAM(, ram - MEMORY_MAP_RAM_BEGIN, graphicsProcessor->dsram + sram, size);
        }
        else
        {
            return copyFromRAM(graphicsProcessor, ram - MEMORY_MAP_RAM_BEGIN, graphicsProcessor->dsram + sram, size);
        }
    }
    else
    {
        return AR_ERROR_ILLEGAL_INSTRUCTION;
    }*/

    (void)graphicsProcessor;
    (void)store;

    return AR_ERROR_ILLEGAL_INSTRUCTION;
}

ArResult executeDMAR(ArGraphicsProcessor AR_RESTRICT graphicsProcessor, int store)
{
    //RAM -> SDRAM
    uint64_t* AR_RESTRICT const reg = graphicsProcessor->reg;

    const ArOperation* AR_RESTRICT const op       = &graphicsProcessor->dmaOperation;
    const uint32_t*    AR_RESTRICT const operands = op->operands;

    const size_t   size = reg[operands[0]] * 32u;
    const uint64_t ram  = reg[operands[1]] * 32ull;
    const uint64_t sram = reg[operands[2]] * 32ull;

    if(sram + size > AR_PROCESSOR_DSRAM_SIZE)
    {
        return AR_ERROR_MEMORY_OUT_OF_RANGE;
    }

    if(ram > MEMORY_MAP_RAM_BEGIN)
    {
        ArPhysicalMemory* memory = getMemoryByRole(graphicsProcessor->parent, AR_PHYSICAL_MEMORY_ROLE_RAM);

        if(!*memory)
        {
            return AR_ERROR_PHYSICAL_MEMORY_OUT_OF_RANGE;
        }

        if(store)
        {
            return copyToRAM(*memory, ram - MEMORY_MAP_RAM_BEGIN, graphicsProcessor->dsram + sram, size);
        }
        else
        {
            return copyFromRAM(*memory, ram - MEMORY_MAP_RAM_BEGIN, graphicsProcessor->dsram + sram, size);
        }
    }
    else
    {
        ArPhysicalMemory* memory = getMemoryByRole(graphicsProcessor->parent, AR_PHYSICAL_MEMORY_ROLE_ROM);

        if(!*memory)
        {
            return AR_ERROR_PHYSICAL_MEMORY_OUT_OF_RANGE;
        }

        if(store)
        {
            return copyToRAM(*memory, ram - MEMORY_MAP_ROM_BEGIN, graphicsProcessor->dsram + sram, size);
        }
        else
        {
            return copyFromRAM(*memory, ram - MEMORY_MAP_ROM_BEGIN, graphicsProcessor->dsram + sram, size);
        }
    }
}

ArResult executeDMAIR(ArGraphicsProcessor AR_RESTRICT graphicsProcessor)
{/*
    //RAM -> SDRAM
    uint64_t* AR_RESTRICT const reg = graphicsProcessor->reg;

    const ArOperation* AR_RESTRICT const op       = &graphicsProcessor->dmaOperation;
    const uint32_t*    AR_RESTRICT const operands = op->operands;

    const uint64_t sram = reg[operands[0]] * 32ull;
    const uint64_t ram  = reg[operands[1]] * 32ull;
    const size_t   size = op->size * 32u;

    if(sram + size > AR_PROCESSOR_ISRAM_SIZE)
    {
        return AR_ERROR_MEMORY_OUT_OF_RANGE;
    }

    if(ram > MEMORY_MAP_RAM_BEGIN)
    {
        return copyFromRAM(graphicsProcessor, ram, graphicsProcessor->isram + sram, size);
    }
    else
    {
        return AR_ERROR_ILLEGAL_INSTRUCTION;
    }*/

    (void)graphicsProcessor;

    return AR_ERROR_ILLEGAL_INSTRUCTION;
}

ArResult executeDMAL(ArGraphicsProcessor AR_RESTRICT graphicsProcessor, int store)
{/*
    //RAM -> SDRAM
    uint64_t* AR_RESTRICT const reg = graphicsProcessor->reg;

    const ArOperation* AR_RESTRICT const op       = &graphicsProcessor->dmaOperation;
    const uint32_t*    AR_RESTRICT const operands = op->operands;

    const uint64_t sram = reg[operands[0]] * 32ull;
    const uint64_t ram  = reg[operands[1]] * 32ull;
    const size_t   size = op->size * 32u;

    if(sram + size > AR_PROCESSOR_DSRAM_SIZE)
    {
        return AR_ERROR_MEMORY_OUT_OF_RANGE;
    }

    if(store)
    {
        return copyToRAM(graphicsProcessor, ram, graphicsProcessor->dsram + sram, size);
    }
    else
    {
        return copyFromRAM(graphicsProcessor, ram, graphicsProcessor->dsram + sram, size);
    }*/

    //TODO

    (void)graphicsProcessor;
    (void)store;

    return AR_ERROR_ILLEGAL_INSTRUCTION;
}

ArResult arExecuteDirectMemoryAccess(ArGraphicsProcessor graphicsProcessor)
{
    assert(graphicsProcessor);

    if(graphicsProcessor->dma)
    {
        graphicsProcessor->dma = 0;

        switch(graphicsProcessor->dmaOperation.op)
        {
            default:
                return AR_ERROR_ILLEGAL_INSTRUCTION;

            case AR_OPCODE_LDDMA:
                return executeDMA(graphicsProcessor, 0);

            case AR_OPCODE_STDMA:
                return executeDMA(graphicsProcessor, 1);

            case AR_OPCODE_LDDMAR:
                return executeDMAR(graphicsProcessor, 0);

            case AR_OPCODE_STDMAR:
                return executeDMAR(graphicsProcessor, 1);

            case AR_OPCODE_DMAIR:
                return executeDMAIR(graphicsProcessor);

            case AR_OPCODE_LDDMAL:
                return executeDMAL(graphicsProcessor, 0);

            case AR_OPCODE_STDMAL:
                return executeDMAL(graphicsProcessor, 1);

            case AR_OPCODE_CLEARC: //fallthrough
            case AR_OPCODE_WAIT:
                //We don't have anything to wait since we emulate it based on C memory model, all transfers are direcly coherent
                break;
        }
    }

    return AR_SUCCESS;
}
