#include <aldebaran.h>
#include <gpu.hpp>

int ALDEBARANAPI_CALL Aldebaran_VM_Init(int sizeRAM, int sizeSPM2,
                                        int sizeTSRAM, GPU* gpu)
{
	*gpu = new GPU_t(sizeRAM, sizeSPM2, sizeTSRAM);
	return 0;
}

int ALDEBARANAPI_CALL Aldebaran_VM_CMDList(GPU gpu, uint64_t* cmd)
{
	gpu->processCommandList(cmd);
	return 0;
}

int ALDEBARANAPI_CALL Aldebaran_SPIRV(uint32_t* spirvCode, uint64_t spirvSize,
                                      uint8_t** aldebaranCode,
                                      uint64_t* aldebaranSize)
{
	/// TODO
	*aldebaranCode = nullptr;
	return 0;
}

int ALDEBARANAPI_CALL Aldebaran_SPIRV_Free(uint8_t* aldebaranCode)
{
	delete aldebaranCode;
	return 0;
}

int ALDEBARANAPI_CALL Aldebaran_VM_End(GPU gpu)
{
	delete gpu;
	return 0;
}
