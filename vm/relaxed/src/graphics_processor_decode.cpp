#include "graphics_processor.hpp"
#include "decode.hpp"

#include <cassert>
#include <cstring>

ArResult arGraphicsProcessorDecodeInstruction(ArGraphicsProcessor graphicsProcessor)
{
    assert(graphicsProcessor);

    const uint32_t size = opcodeSetSize(graphicsProcessor);
    memcpy(graphicsProcessor->opcodes.data(), graphicsProcessor->l1isram.data() + (graphicsProcessor->pc * 4), size * sizeof(uint32_t));

    for(uint32_t i = 0; i < size; ++i)
    {
        if(!decode(i, graphicsProcessor->br, graphicsProcessor->pc, graphicsProcessor->opcodes[i], &graphicsProcessor->operations[i]))
        {
            return AR_ERROR_ILLEGAL_INSTRUCTION;
        }
    }

    graphicsProcessor->pc += size;

    return AR_SUCCESS;
}
