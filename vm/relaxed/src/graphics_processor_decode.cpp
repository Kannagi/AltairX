#include "graphics_processor.hpp"
#include "decode.hpp"

#include <cassert>
#include <cstring>

namespace gpu
{
constexpr std::array LSULoadStore =
{
    AR_OPCODE_LDM,
    AR_OPCODE_STM,
    AR_OPCODE_LDC,
    AR_OPCODE_STC,
    AR_OPCODE_LDMI,
    AR_OPCODE_STMI,
    AR_OPCODE_LDCI,
    AR_OPCODE_STCI,
    AR_OPCODE_LDMV,
    AR_OPCODE_STMV,
    AR_OPCODE_LDCV,
    AR_OPCODE_STCV,
    AR_OPCODE_LDMVI,
    AR_OPCODE_STMVI,
    AR_OPCODE_LDCVI,
    AR_OPCODE_STCVI,
};

constexpr std::array LSULoadStoreL =
{
    AR_OPCODE_LDMIL,
    AR_OPCODE_STMIL,
    AR_OPCODE_LDCIL,
    AR_OPCODE_STCIL,
    AR_OPCODE_LDMVIL,
    AR_OPCODE_STMVIL,
    AR_OPCODE_LDCVIL,
    AR_OPCODE_STCVIL,
};

bool decodeLSU(uint32_t opcode, ArOperation* AR_RESTRICT output)
{
    const uint32_t type = readbits(opcode, 2, 2);

    if (type == 0) //REG Load/Store
    {
        const uint32_t index = readbits(opcode, 4, 4);
        const uint32_t value = readbits(opcode, 10, 10);
        const uint32_t src = readbits(opcode, 20, 6);
        const uint32_t reg = readbits(opcode, 26, 6);

        output->op = LSULoadStore[index];
        output->size = 1;
        output->operands[0] = value;
        output->operands[1] = src;
        output->operands[2] = reg;
    }
    else if (type == 2) //Input/output
    {
        const uint32_t subtype = readbits(opcode, 4, 2);

        if (subtype == 0) //IN
        {
            const uint32_t size = readbits(opcode, 8, 2);
            const uint32_t addr = readbits(opcode, 18, 8);
            const uint32_t reg = readbits(opcode, 26, 6);

            output->op = AR_OPCODE_IN;
            output->size = size;
            output->operands[0] = addr;
            output->operands[2] = reg;
        }
        else if (subtype == 1) //OUT
        {
            const uint32_t size = readbits(opcode, 8, 2);
            const uint32_t addr = readbits(opcode, 18, 8);
            const uint32_t reg = readbits(opcode, 26, 6);

            output->op = AR_OPCODE_OUT;
            output->size = size;
            output->operands[0] = addr;
            output->operands[2] = reg;
        }
        else //OUTI
        {
            const uint32_t size = readbits(opcode, 4, 1);
            const uint32_t value = readbits(opcode, 8, 16);
            const uint32_t addr = readbits(opcode, 24, 8);

            output->op = AR_OPCODE_OUTI;
            output->size = size;
            output->operands[0] = addr;
            output->operands[2] = value;
        }
    }

    return true;
}

bool decode(uint32_t index, uint32_t br, uint32_t pc, uint32_t opcode, ArOperation* AR_RESTRICT output)
{
    const uint32_t compute_unit = readbits(opcode, 0, 2);

    if (index == 0)
    {
        if (compute_unit == 0) //CMP/BRU
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
        else if (compute_unit == 3) //VFPU/VDIV
        {
            return decodeVPU(opcode, output);
        }
    }
    else if (index == 1)
    {
        if (compute_unit == 3) //VFPU
        {
            return decodeVPU(opcode, output);
        }
    }

    output->op = AR_OPCODE_UNKNOWN;
    output->data = 0;
    output->size = 0;
    output->operands[0] = 0;
    output->operands[1] = 0;
    output->operands[2] = 0;

    return false;
}
}

ArResult arGraphicsProcessorDecodeInstruction(ArGraphicsProcessor graphicsProcessor)
{
    assert(graphicsProcessor);

    constexpr uint32_t size = 2;
    memcpy(graphicsProcessor->opcodes.data(), graphicsProcessor->isram.data() + (graphicsProcessor->pc * 4), size * sizeof(uint32_t));

    for(uint32_t i = 0; i < size; ++i)
    {
        if(gpu::decode(i, graphicsProcessor->br, graphicsProcessor->pc, graphicsProcessor->opcodes[i], &graphicsProcessor->operations[i]) == false)
        {
            return AR_ERROR_ILLEGAL_INSTRUCTION;
        }
    }

    graphicsProcessor->pc += size;

    return AR_SUCCESS;
}
