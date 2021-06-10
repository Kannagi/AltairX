#include "processor.hpp"
#include "decode.hpp"

#include <cassert>
#include <cstring>

int decodeCPU(uint32_t index, uint32_t br, uint32_t pc, uint32_t opcode, ArOperation* AR_RESTRICT output)
{
    const uint32_t compute_unit = readbits(opcode, 0, 2);

    if (index == 0)
    {
        if (compute_unit == 0) //BRU
        {
            return decodeBRU(br, pc, opcode, output);
        }
        else if (compute_unit == 1) //LSU
        {
            return decodeLSU(opcode, output);
        }
        else if (compute_unit == 2) //ALU
        {
            return decodeALU(opcode, output);
        }
        else //VFPU/VDIV
        {
            return decodeVPU(opcode, output);
        }
    }
    else if (index == 1)
    {
        if (compute_unit == 0) //AGU
        {
            return decodeAGU(opcode, output);
        }
        else if (compute_unit == 1) //LSU
        {
            return decodeLSU(opcode, output);
        }
        else if (compute_unit == 2) //ALU
        {
            return decodeALU(opcode, output);
        }
        else //VFPU
        {
            return decodeVPU(opcode, output);
        }
    }
    else //2 or 3
    {
        if (compute_unit == 2) //ALU
        {
            return decodeALU(opcode, output);
        }
        else //VFPU
        {
            return false;
        }
    }

    return false;
}

ArResult arProcessorDecodeInstruction(ArProcessor processor)
{
    assert(processor);

    const uint32_t size = opcodeSetSize(processor);
    memcpy(processor->opcodes, processor->isram + (processor->pc * 4), size * sizeof(uint32_t));

    for(uint32_t i = 0; i < size; ++i)
    {
        if(!decodeCPU(i, processor->br, processor->pc, processor->opcodes[i], &processor->operations[i]))
        {
            return AR_ERROR_ILLEGAL_INSTRUCTION;
        }
    }

    processor->pc += size;

    return AR_SUCCESS;
}
