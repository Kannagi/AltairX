#include "vm.hpp"

#include <cstdlib>
#include <cassert>
#include <cstring>
#include <utility>
#include <algorithm>

ArResult arCreateVirtualMachine(ArVirtualMachine* pVirtualMachine, const ArVirtualMachineCreateInfo* pInfo)
{
	assert(pVirtualMachine);
	assert(pInfo);
	assert(pInfo->sType == AR_STRUCTURE_TYPE_VIRTUAl_MACHINE_CREATE_INFO);

	*pVirtualMachine = new ArVirtualMachine_T;

	if(*pVirtualMachine == nullptr)
	{
		return AR_ERROR_HOST_OUT_OF_MEMORY;
	}

	(void)pInfo;

	return AR_SUCCESS;
}

void insertProcessor(ArVirtualMachine virtualMachine, ArProcessor processor)
{
	ArProcessor previous = virtualMachine->processor;
	if(previous)
	{
		while(previous->next) //Find last processor in virtual machine
		{
			previous = previous->next;
		}

		previous->next = processor;
	}
	else
	{
		virtualMachine->processor = processor; //Its the first processor
	}
}

ArResult arCreateProcessor(ArVirtualMachine virtualMachine, const ArProcessorCreateInfo* pInfo, ArProcessor* pProcessor)
{
	assert(virtualMachine);
	assert(pInfo);
	assert(pInfo->sType == AR_STRUCTURE_TYPE_PROCESSOR_CREATE_INFO);
	assert(pInfo->pBootCode);
	assert(pInfo->bootCodeSize % 4 == 0); //if not true then input is obviously truncated
	assert(pProcessor);

	*pProcessor = new ArProcessor_T;

	if(*pProcessor == nullptr)
	{
		return AR_ERROR_HOST_OUT_OF_MEMORY;
	}

	(*pProcessor)->parent = virtualMachine;
	memcpy((*pProcessor)->isram, pInfo->pBootCode, pInfo->bootCodeSize * sizeof(uint32_t));

	insertProcessor(virtualMachine, *pProcessor);

	return AR_SUCCESS;
}

ArResult arCreatePhysicalMemory(ArVirtualMachine virtualMachine, const ArPhysicalMemoryCreateInfo* pInfo, ArPhysicalMemory* pMemory)
{
	assert(virtualMachine);
	assert(pInfo);
	assert(pInfo->sType == AR_STRUCTURE_TYPE_PHYSICAL_MEMORY_CREATE_INFO);
	assert(pInfo->size > 0);
	assert(pInfo->pMemory);
	assert(pMemory);

	if(virtualMachine->memory)
	{
		return AR_ERROR_TOO_MANY_OBJECTS;
	}

	*pMemory = new ArPhysicalMemory_T;

	if(*pMemory == nullptr)
	{
		return AR_ERROR_HOST_OUT_OF_MEMORY;
	}

	(*pMemory)->parent = virtualMachine;
	(*pMemory)->memory = static_cast<uint8_t*>(pInfo->pMemory);
	(*pMemory)->size = pInfo->size;

	virtualMachine->memory = *pMemory;

	return AR_SUCCESS;
}

void arGetProcessorOperations(ArProcessor processor, ArOperation* pOutput, size_t* pCount)
{
	assert(processor);
	assert(pOutput);
	assert(pCount);

	const uint32_t count = opcodeSetSize(processor);

	memcpy(pOutput, processor->operations, sizeof(ArOperation) * count);
	*pCount = count;
}

void arGetProcessorMemoryInfo(ArProcessor processor, ArProcessorMemoryInfo* pOutput)
{
	assert(processor);
	assert(pOutput);

	pOutput->dsram  = processor->dsram;
	pOutput->isram  = processor->isram;
	pOutput->cache  = processor->cache;
	pOutput->iosram = processor->iosram;
	pOutput->ireg   = processor->ireg;
	pOutput->freg   = processor->freg;
}

void arDestroyVirtualMachine(ArVirtualMachine virtualMachine)
{
	if (virtualMachine)
		delete virtualMachine;
}

void arDestroyProcessor(ArVirtualMachine virtualMachine, ArProcessor processor)
{
	assert(virtualMachine);
	assert(virtualMachine->processor);

	if (processor == nullptr)
		return;

	if(virtualMachine->processor == processor)
	{
		virtualMachine->processor = processor->next;
	}
	else
	{
		ArProcessor previous = virtualMachine->processor;
		while(previous->next && previous->next != processor)
		{
			previous = previous->next;
		}

		previous->next = processor->next;
	}

	delete processor;
}

void arDestroyPhysicalMemory(ArVirtualMachine virtualMachine, ArPhysicalMemory memory)
{
	assert(virtualMachine);
	assert(virtualMachine->memory);

	if (memory == nullptr)
		return;

	virtualMachine->memory = nullptr;

	delete memory;
}

uint32_t opcodeSetSize(ArProcessor processor)
{
	uint32_t size;
	if(processor->flags & 0x01)
	{
		const uint32_t available = processor->pc - (AR_PROCESSOR_ISRAM_SIZE / 4u); //we may overflow otherwise
		size = std::min(available, 4u);
	}
	else
	{
		size = 2;
	}

	return size;
}
