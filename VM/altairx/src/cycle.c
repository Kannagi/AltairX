
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

#include "vm.h"

void AX_Cache_miss(Core *core)
{
	//I-Cache
	uint32_t index = (core->pc>>7);
	index = (index&0x3F);

	if(core->icache[index] != core->pc)
	{
		core->icache[index] = core->pc;
		core->cycle += 24 + (5*16); //5cycle /64 B
	}

	uint32_t unit1 = core->operations[0].unit1;
    uint32_t unit2 = core->operations[0].unit2;

	if(unit1 == AX_EXE_LSU)
    {
        switch(unit2)
        {
            case AX_OPCODE_LD:

            break;

            case AX_OPCODE_ST:

            break;

            case AX_OPCODE_LDV:

            break;

            case AX_OPCODE_STV:

            break;

            case AX_OPCODE_FLUSH:

			break;

            case AX_OPCODE_PREFETCH:

            break;

            case AX_OPCODE_IFLUSH:

			break;

            case AX_OPCODE_IPREFETCH:

            break;

        }
    }
}

void AX_Pipeline_stall(Core *core,uint32_t index)
{
	//core->register_busy

}
