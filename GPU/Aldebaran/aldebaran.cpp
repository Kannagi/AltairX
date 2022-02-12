#include <aldebaran.h>
#include <gpu.hpp>

int32_t ALDEBARANAPI_CALL Aldebaran_VM_Init(uint64_t sizeRAM,
                                            uint64_t sizeSPM2,
                                            uint64_t sizeTexureRAM,
                                            uint64_t sizeBufferRAM,
                                            GPU* gpu)
{
	*gpu = new GPU_t{sizeRAM, sizeSPM2, sizeTexureRAM, sizeBufferRAM};
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
