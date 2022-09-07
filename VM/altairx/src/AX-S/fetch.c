#include <stdint.h>

#include "vm.h"


void AXS1_fetch(Core *core)
{
	uint32_t pc = core->pc;
    core->opcodes[0] = core->wram[pc+0];
    core->opcodes[1] = core->wram[pc+1];

	uint8_t pairing = (core->opcodes[0]&1);

    if(pairing == 0)
    {
        core->pc += 1;
        core->opcodes[1] = 0;
    }
    else
    {
        core->pc += 2;
    }
}
