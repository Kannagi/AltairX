#ifndef ALDEBARAN_H
#define ALDEBARAN_H 1

#ifdef __cplusplus
extern "C"
{
#endif

#if defined(_WIN32)
#define ALDEBARANAPI_ATTR
#define ALDEBARANAPI_CALL __stdcall
#define ALDEBARANAPI_PTR ALDEBARANAPI_CALL
#elif defined(__ANDROID__) && defined(__ARM_ARCH) && __ARM_ARCH < 7
#error "Aldebaran isn't supported for the 'armeabi' NDK ABI"
#elif defined(__ANDROID__) && defined(__ARM_ARCH) && __ARM_ARCH >= 7 &&       \
    defined(__ARM_32BIT_STATE)
#define ALDEBARANAPI_ATTR __attribute__((pcs("aapcs-vfp")))
#define ALDEBARANAPI_CALL
#define ALDEBARANAPI_PTR ALDEBARANAPI_ATTR
#else
#define ALDEBARANAPI_ATTR
#define ALDEBARANAPI_CALL
#define ALDEBARANAPI_PTR
#endif

#include <stddef.h>

#if !defined(ALDEBARAN_NO_STDINT_H)
#if defined(_MSC_VER) && (_MSC_VER < 1600)
	typedef signed __int8 int8_t;
	typedef unsigned __int8 uint8_t;
	typedef signed __int16 int16_t;
	typedef unsigned __int16 uint16_t;
	typedef signed __int32 int32_t;
	typedef unsigned __int32 uint32_t;
	typedef signed __int64 int64_t;
	typedef unsigned __int64 uint64_t;
#else
#include <stdint.h>
#endif
#endif

#define MIN(x, y) (x < y ? x : y)
#define MAX(x, y) (x > y ? x : y)

#define XCHG_MASK (0x01u)
#define Z_MASK (0x02u)
#define S_MASK (0x04u)
#define U_MASK (0x08u)

#define AR_PROCESSOR_MAX_OPERATIONS 2
#define AR_PROCESSOR_DSRAM_SIZE 0x20000
#define AR_PROCESSOR_ISRAM_SIZE 0x20000
#define AR_PROCESSOR_IREG_COUNT 64
#define AR_PROCESSOR_VREG_COUNT 64
#define AR_SUCCESS 1
#define AR_ERROR_ILLEGAL_INSTRUCTION -1
#define AR_END_OF_CODE 2

#define N_SHADER 16
	/*
	0x1 0000 0000 - 0x1 FFFF FFFF ------- 1T-SRAM Graphic
	0x2 0000 0000 - 0x2 FFFF FFFF ------- SPM L2
	0x8 0000 0000 - 0xF FFFF FFFF ------- RAM (32 GB max )
	36 bits
	*/

#define MEMORY_MAP_1TSRAM_BEGIN (0x100000000)
#define MEMORY_MAP_SPM2_BEGIN (0x200000000)
#define MEMORY_MAP_VRAM_BEGIN (0x400000000)
#define MEMORY_MAP_RAM_BEGIN (0x800000000)

#define MEMORY_MAP_OTHER (0x300000000)

	typedef enum Aldebaran_Command
	{
		Aldebaran_Command_Fbuffer_select = 0,
		Aldebaran_Command_Zbuffer_select,
		Aldebaran_Command_execute,
		Aldebaran_Command_end,
		Aldebaran_Command_wait,
		Aldebaran_Command_flush,
		Aldebaran_Command_PPU,
		Aldebaran_Command_CLUT,
		Aldebaran_Command_Src = 9,
		Aldebaran_Command_Dst,
		Aldebaran_Command_Send
	} Aldebaran_Command;

	typedef enum Aldebaran_Send_Mode
	{
		Aldebaran_Send_Mode_RAM_to_TSRAM = 0,
		Aldebaran_Send_Mode_TSRAM_to_RAM,
		Aldebaran_Send_Mode_RAM_to_SPM2,
		Aldebaran_Send_Mode_RAM_to_SPMI,
		Aldebaran_Send_Mode_RAM_to_SPMD,
		Aldebaran_Send_Mode_SPM2_to_SPMI,
		Aldebaran_Send_Mode_SPM2_to_SPMD,
	} Aldebaran_Send_Mode;

	enum Exe
	{
		AK1_EXE_ALU,
		AK1_EXE_LSU,
		AK1_EXE_BRU,
		AK1_EXE_CMP,
		AK1_EXE_VFPU,
		AK1_EXE_DMA,
		AK1_EXE_OTHER,
		AK1_EXE_NOP,
	};

	// ALU
	enum OpcodeALU
	{
		AK1_OPCODE_ADD,
		AK1_OPCODE_SUB,
		AK1_OPCODE_XOR,
		AK1_OPCODE_OR,
		AK1_OPCODE_AND,
		AK1_OPCODE_LSL,
		AK1_OPCODE_ASR,
		AK1_OPCODE_LSR,

		AK1_OPCODE_MOVE = 0x10,
		AK1_OPCODE_MOVESP,
	};

	// CMP
	enum OpcodeCMP
	{
		AK1_OPCODE_CMP,
		AK1_OPCODE_FCMP = 0x04,
	};

	// LSU
	enum OpcodeLSU
	{
		AK1_OPCODE_LDM,
		AK1_OPCODE_STM,
		AK1_OPCODE_LDC,

		AK1_OPCODE_PREFETCH,
		AK1_OPCODE_FLUSH,
	};

	// BRU
	enum OpcodeBRU
	{
		AK1_OPCODE_BNE,
		AK1_OPCODE_BEQ,
		AK1_OPCODE_BL,
		AK1_OPCODE_BLE,
		AK1_OPCODE_BG,
		AK1_OPCODE_BGE,
		AK1_OPCODE_BLS,
		AK1_OPCODE_BLES,
		AK1_OPCODE_BGS,
		AK1_OPCODE_BGES,
		AK1_OPCODE_BRA,
		AK1_OPCODE_JMP,
		AK1_OPCODE_JMPBR,
		AK1_OPCODE_CALL,
		AK1_OPCODE_CALLBR,

		AK1_OPCODE_RET = 0x10,
	};

	// DMA
	enum OpcodeDMA
	{
		AK1_OPCODE_LDDMA,
		AK1_OPCODE_STDMA,
		AK1_OPCODE_LDDMACL,
		AK1_OPCODE_STDMACL,
		AK1_OPCODE_DMAI,
		AK1_OPCODE_WAIT = 7,
	};

	// VFPU
	enum OpcodeVFPU
	{
		AK1_OPCODE_FADD,
		AK1_OPCODE_FSUB,
		AK1_OPCODE_FMUL,
		AK1_OPCODE_FMADD,

		AK1_OPCODE_VFADD,
		AK1_OPCODE_VFSUB,
		AK1_OPCODE_VFMUL,
		AK1_OPCODE_VFMADD,

		AK1_OPCODE_VFADDS,
		AK1_OPCODE_VFSUBS,
		AK1_OPCODE_VFMULS,
		AK1_OPCODE_VFMADDS,

		AK1_OPCODE_FMSUB,
		AK1_OPCODE_VFMSUB,
		AK1_OPCODE_VFMSUBS,

		AK1_OPCODE_FMOVE = 0x00,
		AK1_OPCODE_VFMOVE,

		AK1_OPCODE_FNEG = 0x04,
		AK1_OPCODE_FABS,
		AK1_OPCODE_VFNEG,
		AK1_OPCODE_VFABS,

		AK1_OPCODE_VFTOH = 0x08,
		AK1_OPCODE_VHTOF,
		AK1_OPCODE_VFTOI,
		AK1_OPCODE_VITOF,
		AK1_OPCODE_VFTOD,
		AK1_OPCODE_VDTOF,

	};

	// OTHER
	enum OpcodeOther
	{
		AK1_OPCODE_INT,
		AK1_OPCODE_ENDP,
	};

	enum Reg
	{
		AK1_REG_A = 61,
		AK1_REG_P,
		AK1_REG_Q,
	};
#define AK1_OPCODE_UNKNOWN -1

	struct ArVirtualMachine_T;
	typedef ArVirtualMachine_T* ArVirtualMachine;

	struct GPU_t;
	typedef GPU_t* GPU;

	struct ComputeUnit_t;
	typedef ComputeUnit_t* ComputeUnit;

	struct PixelProcessUnit_t;
	typedef PixelProcessUnit_t* PixelProcessUnit;

	int ALDEBARANAPI_CALL Aldebaran_VM_Init(int sizeRAM, int sizeSPM2,
	                                        int sizeTSRAM, GPU* gpu);
	int ALDEBARANAPI_CALL Aldebaran_VM_CMDList(GPU gpu, uint64_t* cmd);
	int ALDEBARANAPI_CALL Aldebaran_SPIRV(uint32_t* spirvCode,
	                                      uint64_t spirvSize,
	                                      uint8_t** aldebaranCode,
	                                      uint64_t* aldebaranSize);
	int ALDEBARANAPI_CALL Aldebaran_SPIRV_Free(uint8_t* aldebaranCode);
	int ALDEBARANAPI_CALL Aldebaran_VM_End(GPU gpu);

#ifdef __cplusplus
}
#endif

#endif  // ALDEBARAN_H
