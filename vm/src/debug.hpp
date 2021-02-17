#ifndef DEBUG_HPP_INCLUDED
#define DEBUG_HPP_INCLUDED

#include <iostream>

#include "vm.hpp"
#include "io.hpp"
#include "fmt/format.h"

namespace dbg
{

struct user_operation
{
    enum : std::uint32_t
    {
        step = 0,
        quit = 1
    };

    std::uint32_t operation{};
    std::uint64_t data{};
};

inline user_operation get_next_user_input()
{
    std::string input{};
    input.reserve(128);

    while(true)
    {
        std::getline(std::cin, input);

        if(input == "step" || std::empty(input))
        {
            return user_operation{user_operation::step};
        }
        else if(input == "quit")
        {
            return user_operation{user_operation::quit};
        }
    }
}

inline constexpr std::array byte_size_names{'b', 'w', 'l', 'q'};

inline void output_op(const ar::processor::operation_set_t& operation_set)
{
    const auto& [count, operations] = operation_set;

    for(std::size_t i{}; i < count; ++i)
    {
        const auto& op{operations[i]};

        switch(op.op)
        {
            default:
                fmt::print("???");
                break;
        /*
            //AGU
            case AR_OPCODE_LDDMA:
            case AR_OPCODE_STDMA:
            case AR_OPCODE_LDDMAR:
            case AR_OPCODE_STDMAR:
            case AR_OPCODE_DMAIR:
            case AR_OPCODE_WAIT:
                break;
        */
            //LSU
            case AR_OPCODE_LDM:
                fmt::print("ldm.{} r{}, ${:X}[r{}{}]", byte_size_names[op.size], op.operands[2], op.operands[0], op.operands[1], op.data ? "+" : "");
                break;

            case AR_OPCODE_STM:
                fmt::print("stm.{} r{}, ${:X}[r{}{}]", byte_size_names[op.size], op.operands[2], op.operands[0], op.operands[1], op.data ? "+" : "");
                break;
        /*
            case AR_OPCODE_LDC: //copy data from cache to register
                memcpy(&ireg[operands[2]], &processor->cache[operands[0] + ireg[operands[1]]], 1u << op->size);
                ireg[operands[1]] += op->data; //incr
                break;

            case AR_OPCODE_STC: //copy data from register to cache
                memcpy(&processor->cache[operands[0] + ireg[operands[1]]], &ireg[operands[2]], 1u << op->size);
                ireg[operands[1]] += op->data; //incr
                break;

            case AR_OPCODE_LDMX: //copy data from dsram to register
                memcpy(&ireg[operands[2]], &processor->dsram[operands[0] + ireg[operands[1]]], 1u << op->size);
                ireg[operands[1]] += op->data; //incr
                break;

            case AR_OPCODE_STMX: //copy data from register to dsram
                memcpy(&processor->dsram[operands[0] + ireg[operands[1]]], &ireg[operands[2]], 1u << op->size);
                ireg[operands[1]] += op->data; //incr
                break;

            case AR_OPCODE_IN: //copy data from iosram to register
                memcpy(&ireg[operands[2]], &processor->iosram[operands[0]], 1u << op->size);
                break;

            case AR_OPCODE_OUT: //copy data from register to iosram
                memcpy(&processor->iosram[operands[0]], &ireg[operands[2]], 1u << op->size);
                break;

            case AR_OPCODE_OUTI: //write data to iosram
                memcpy(&processor->iosram[operands[0]], &ireg[operands[2]], 1u << op->size);
                break;

            case AR_OPCODE_LDMV: //copy data from dsram to vector register
                memcpy(&vreg[operands[2]], &processor->dsram[operands[0] + ireg[operands[1]]], 16);
                ireg[operands[1]] += op->data; //incr
                break;

            case AR_OPCODE_STMV: //copy data from vector register to dsram
                memcpy(&processor->dsram[operands[0] + ireg[operands[1]]], &vreg[operands[2]], 16);
                ireg[operands[1]] += op->data; //incr
                break;

            case AR_OPCODE_LDCV: //copy data from cache to vector register
                memcpy(&vreg[operands[2]], &processor->cache[operands[0] + ireg[operands[1]]], 16);
                ireg[operands[1]] += op->data; //incr
                break;

            case AR_OPCODE_STCV: //copy data from vector register to cache
                memcpy(&processor->cache[operands[0] + ireg[operands[1]]], &vreg[operands[2]], 16);
                ireg[operands[1]] += op->data; //incr
                break;

            case AR_OPCODE_LDMF: //copy data from dsram to float register
                memcpy(&freg[operands[2]], &processor->dsram[operands[0] + ireg[operands[1]]], 4);
                ireg[operands[1]] += op->data; //incr
                break;

            case AR_OPCODE_STMF: //copy data from float register to dsram
                memcpy(&processor->dsram[operands[0] + ireg[operands[1]]], &freg[operands[2]], 4);
                ireg[operands[1]] += op->data; //incr
                break;

            case AR_OPCODE_LDCF: //copy data from cache to float register
                memcpy(&freg[operands[2]], &processor->cache[operands[0] + ireg[operands[1]]], 4);
                ireg[operands[1]] += op->data; //incr
                break;

            case AR_OPCODE_STCF: //copy data from float register to cache
                memcpy(&processor->cache[operands[0] + ireg[operands[1]]], &freg[operands[2]], 4);
                ireg[operands[1]] += op->data; //incr
                break;

            case AR_OPCODE_LDMD: //copy data from dsram to double register
                memcpy(&dreg[operands[2]], &processor->dsram[operands[0] + ireg[operands[1]]], 8);
                ireg[operands[1]] += op->data; //incr
                break;

            case AR_OPCODE_STMD: //copy data from double register to dsram
                memcpy(&processor->dsram[operands[0] + ireg[operands[1]]], &dreg[operands[2]], 8);
                ireg[operands[1]] += op->data; //incr
                break;

            case AR_OPCODE_LDCD: //copy data from cache to double register
                memcpy(&dreg[operands[2]], &processor->cache[operands[0] + ireg[operands[1]]], 8);
                ireg[operands[1]] += op->data; //incr
                break;

            case AR_OPCODE_STCD: //copy data from double register to cache
                memcpy(&processor->cache[operands[0] + ireg[operands[1]]], &dreg[operands[2]], 8);
                ireg[operands[1]] += op->data; //incr
                break;

            //ALU
            case AR_OPCODE_NOP: //In case of nop.e, we need to delay it
                if(op->data)
                {
                    processor->delayedBits |= (1u << index);
                    processor->delayed[index] = *op;
                }
                break;

            case AR_OPCODE_XCHG: //Flip XCHG bit
                processor->delayedBits |= (1u << index);
                processor->delayed[index] = *op;
                break;
*/
            case AR_OPCODE_MOVEI:
                fmt::print("movei r{}, ${:08X}", op.operands[2], op.operands[0]);
                break;
/*
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

            //BRU
            case AR_OPCODE_BNE:
            case AR_OPCODE_BEQ:
            case AR_OPCODE_BL:
            case AR_OPCODE_BLE:
            case AR_OPCODE_BG:
            case AR_OPCODE_BGE:
            case AR_OPCODE_BLS:
            case AR_OPCODE_BLES:
            case AR_OPCODE_BGS:
            case AR_OPCODE_BGES:
            case AR_OPCODE_CMP:
            case AR_OPCODE_CMPI:
            case AR_OPCODE_FCMP:
            case AR_OPCODE_FCMPI:
            case AR_OPCODE_DCMP:
            case AR_OPCODE_DCMPI:
            case AR_OPCODE_JMP:
            case AR_OPCODE_CALL:
            case AR_OPCODE_JMPR:
            case AR_OPCODE_CALLR:
            case AR_OPCODE_RET:
            case AR_OPCODE_MOVE:*/
        }

        if(i != count - 1)
        {
            fmt::print(" | ");
        }
    }

    fmt::print("\n");
}

}

inline void run_debugger(ar::processor& processor)
{
    const auto memory_info{processor.get_memory_info()};

    while(true)
    {
        const auto input{dbg::get_next_user_input()};

        if(input.operation == dbg::user_operation::quit)
        {
            return;
        }

        processor.decode();

        dbg::output_op(processor.get_operations());

        if(!processor.execute())
        {
            return;
        }

        processor.direct_memory_access();

        run_io(memory_info.iosram);
    }
}

#endif

