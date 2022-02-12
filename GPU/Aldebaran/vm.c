#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>

#include "vm.h"

void Aldebaran_VM_CU_Init(ComputeUnit *processor)
{

}


int Aldebaran_VM_CU_Exe(ComputeUnit *processor)
{
	Aldebaran_DecodeInstruction(processor);
	Aldebaran_ExcecuteInstruction(processor);

	return 0;
}

void arDestroyPhysicalMemory()
{
    //assert(virtualMachine);


    //free(memory);
}



