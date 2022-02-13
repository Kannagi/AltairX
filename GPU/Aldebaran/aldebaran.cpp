#include <aldebaran.h>
#include <cstring>
#include <gpu.hpp>

int32_t ALDEBARANAPI_CALL Aldebaran_VM_Init(GPU* gpu)
{
	*gpu = new GPU_t;
	return AR_SUCCESS;
}

int32_t ALDEBARANAPI_CALL Aldebaran_VM_WriteToRAM(GPU gpu,
                                                  size_t RAMOffset,
                                                  const uint8_t* data,
                                                  size_t size)
{
	if (gpu->RAM.cbegin() + RAMOffset >= gpu->RAM.cend())
		return AR_ERROR_OUT_OF_BOUNDS;

	std::memcpy(gpu->RAM.data() + RAMOffset, data, size);
	return AR_SUCCESS;
}

int32_t ALDEBARANAPI_CALL Aldebaran_VM_CMDList(GPU gpu, const uint64_t* cmds)
{
	gpu->processCommandList(reinterpret_cast<const GpuInstruction*>(cmds));
	return AR_SUCCESS;
}

int32_t ALDEBARANAPI_CALL Aldebaran_SPIRV(uint32_t* spirvCode,
                                          uint64_t spirvSize,
                                          uint8_t** aldebaranCode,
                                          uint64_t* aldebaranSize)
{
	/// TODO
	*aldebaranCode = nullptr;
	return AR_SUCCESS;
}

void ALDEBARANAPI_CALL Aldebaran_SPIRV_Free(uint8_t* aldebaranCode)
{
	delete aldebaranCode;
}

void ALDEBARANAPI_CALL Aldebaran_VM_End(GPU gpu)
{
	delete gpu;
}
