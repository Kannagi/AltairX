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
#elif defined(__ANDROID__) && defined(__ARM_ARCH) && __ARM_ARCH >= 7 && \
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
		AK1_EXE_ALU_B     = 0b0000,
		AK1_EXE_ALU_A     = 0b0001,
		AK1_EXE_LSU_SPM   = 0b0010,
		AK1_EXE_LSU_CACHE = 0b0011,
		AK1_EXE_CMP       = 0b0100,
		AK1_EXE_BRU       = 0b0101,
		AK1_EXE_VBRU      = 0b0110,
		AK1_EXE_VFPU_A    = 0b0101,
		AK1_EXE_VFPU_B    = 0b0110,
		AK1_EXE_EFU       = 0b1001,
		AK1_EXE_DMA       = 0b0111,
	};

	enum OpcodeALU_A
	{
		AK1_OPCODE_ADD  = 0b0000,
		AK1_OPCODE_SUB  = 0b0001,
		AK1_OPCODE_XOR  = 0b0010,
		AK1_OPCODE_OR   = 0b0011,
		AK1_OPCODE_AND  = 0b0100,
		AK1_OPCODE_LSL  = 0b0101,
		AK1_OPCODE_ASR  = 0b0110,
		AK1_OPCODE_LSR  = 0b0111,
		AK1_OPCODE_ADDI = 0b1000,
		AK1_OPCODE_SUBI = 0b1001,
		AK1_OPCODE_XORI = 0b1010,
		AK1_OPCODE_ORI  = 0b1011,
		AK1_OPCODE_ANDI = 0b1100,
		AK1_OPCODE_LSLI = 0b1101,
		AK1_OPCODE_ASRI = 0b1110,
		AK1_OPCODE_LSRI = 0b1111,
	};

	enum OpcodeALU_B
	{
		AK1_OPCODE_NOP    = 0b0000,
		AK1_OPCODE_SEXT   = 0b0001,
		AK1_OPCODE_BOOL   = 0b0010,
		AK1_OPCODE_SMOVE  = 0b0011,
		AK1_OPCODE_MOVEI  = 0b0100,
		AK1_OPCODE_MOVEIU = 0b0101,
	};

	enum OpcodeLSU_SPM
	{
		AK1_OPCODE_LOAD    = 0b0000,
		AK1_OPCODE_LOADI   = 0b0001,
		AK1_OPCODE_LOADL   = 0b0010,
		AK1_OPCODE_LOADL2  = 0b0011,
		AK1_OPCODE_STORE   = 0b0100,
		AK1_OPCODE_STOREI  = 0b0101,
		AK1_OPCODE_STOREL  = 0b0110,
		AK1_OPCODE_STOREL2 = 0b0111,
	};

	enum OpcodeLSU_Cache
	{
		AK1_OPCODE_LOAD   = 0b0000,
		AK1_OPCODE_LOADI  = 0b0001,
		AK1_OPCODE_LOADL  = 0b0010,
		AK1_OPCODE_LOADL2 = 0b0011,
	};

	enum OpcodeCMP
	{
		AK1_OPCODE_CMP   = 0b0000,
		AK1_OPCODE_CMPFR = 0b0001,
		AK1_OPCODE_CMPI  = 0b0010,
		AK1_OPCODE_CMPIU = 0b0011,
		AK1_OPCODE_ENDP  = 0b0100,
		AK1_OPCODE_RET   = 0b0101,
		AK1_OPCODE_ENDB  = 0b0110,
	};

	enum OpcodeVCMP
	{
		AK1_OPCODE_BCMPZ    = 0b0000,
		AK1_OPCODE_BCMP     = 0b0001,
		AK1_OPCODE_BCMPZI   = 0b0010,
		AK1_OPCODE_BCMPI    = 0b0011,
		AK1_OPCODE_BFCMPE   = 0b0100,
		AK1_OPCODE_BFCMPNE  = 0b0101,
		AK1_OPCODE_BFCMPEI  = 0b0110,
		AK1_OPCODE_BFCMPNEI = 0b0111,
		AK1_OPCODE_FCMP     = 0b1000,
		AK1_OPCODE_FCMPI    = 0b1001,
	};

	enum OpcodeBRU
	{
		AK1_OPCODE_BNE    = 0b0000,
		AK1_OPCODE_BEQ    = 0b0001,
		AK1_OPCODE_BL     = 0b0010,
		AK1_OPCODE_BLE    = 0b0011,
		AK1_OPCODE_BG     = 0b0100,
		AK1_OPCODE_BGE    = 0b0101,
		AK1_OPCODE_BLS    = 0b0110,
		AK1_OPCODE_BLES   = 0b0111,
		AK1_OPCODE_BGS    = 0b1000,
		AK1_OPCODE_BGES   = 0b1001,
		AK1_OPCODE_BRA    = 0b1010,
		AK1_OPCODE_JMP    = 0b1011,
		AK1_OPCODE_JMPBR  = 0b1100,
		AK1_OPCODE_CALL   = 0b1101,
		AK1_OPCODE_CALLBR = 0b1110,
		AK1_OPCODE_LOOP   = 0b1111,
	};

	enum OpcodeVFPU_A
	{
		AK1_OPCODE_FADD      = 0b0000,
		AK1_OPCODE_FSUB      = 0b0001,
		AK1_OPCODE_FMUL      = 0b0010,
		AK1_OPCODE_FMADD     = 0b0011,
		AK1_OPCODE_VFADD     = 0b0100,
		AK1_OPCODE_VFSUB     = 0b0101,
		AK1_OPCODE_VFMUL     = 0b0110,
		AK1_OPCODE_VFMADD    = 0b0111,
		AK1_OPCODE_VFADDS    = 0b1000,
		AK1_OPCODE_VFSUBS    = 0b1001,
		AK1_OPCODE_VFMULS    = 0b1010,
		AK1_OPCODE_VFMADDS   = 0b1011,
		AK1_OPCODE_FMULSUB   = 0b1100,
		AK1_OPCODE_VFMULSUB  = 0b1101,
		AK1_OPCODE_VFMULSUBS = 0b1110,
		AK1_OPCODE_SHUFFLE   = 0b1111,
	};

	enum OpcodeVFPU_B
	{
		AK1_OPCODE_FMOVE   = 0b0000,
		AK1_OPCODE_VMOVE   = 0b0001,
		AK1_OPCODE_FMOVEI  = 0b0010,
		AK1_OPCODE_VFMOVEI = 0b0011,
		AK1_OPCODE_FNEG    = 0b0100,
		AK1_OPCODE_FABS    = 0b0101,
		AK1_OPCODE_VFNEG   = 0b0110,
		AK1_OPCODE_VFABS   = 0b0111,
		AK1_OPCODE_VFTOH   = 0b1000,
		AK1_OPCODE_VHTOF   = 0b1001,
		AK1_OPCODE_VFTOI   = 0b1010,
		AK1_OPCODE_VITOF   = 0b1011,
		AK1_OPCODE_VFTOD   = 0b1100,
		AK1_OPCODE_VDTOF   = 0b1101,
		AK1_OPCODE_VFMIN   = 0b1110,
		AK1_OPCODE_VFMAX   = 0b1111,
	};

	enum OpcodeEFU
	{
		AK1_OPCODE_FDIV   = 0b0000,
		AK1_OPCODE_FSQRT  = 0b0001,
		AK1_OPCODE_FATAN  = 0b0100,
		AK1_OPCODE_FATAN2 = 0b0101,
		AK1_OPCODE_FEXP   = 0b0110,
		AK1_OPCODE_FSUM   = 0b1000,
		AK1_OPCODE_FIPR   = 0b1001,
		AK1_OPCODE_FSIN   = 0b1010,
	};

	enum OpcodeDMA
	{
		AK1_OPCODE_LDDMA  = 0b0000,
		AK1_OPCODE_STDMA  = 0b0001,
		AK1_OPCODE_DMAI   = 0b0100,
		AK1_OPCODE_LDDMAI = 0b1000,
		AK1_OPCODE_STDMAI = 0b1001,
		AK1_OPCODE_DMAII  = 0b1100,
		AK1_OPCODE_WAIT   = 0b1111,
	};

#define AK1_OPCODE_UNKNOWN -1

	enum Reg
	{
		AK1_REG_A = 61,
		AK1_REG_P,
		AK1_REG_Q,
	};

	struct ArVirtualMachine_T;
	typedef ArVirtualMachine_T* ArVirtualMachine;

	struct GPU_t;
	typedef GPU_t* GPU;

	struct ComputeUnit_t;
	typedef ComputeUnit_t* ComputeUnit;

	struct PixelProcessUnit_t;
	typedef PixelProcessUnit_t* PixelProcessUnit;

	int32_t ALDEBARANAPI_CALL Aldebaran_VM_Init(uint64_t sizeRAM,
	                                            uint64_t sizeSPM2,
	                                            uint64_t sizeTexureRAM,
	                                            uint64_t sizeBufferRAM,
	                                            GPU* gpu);

	int32_t ALDEBARANAPI_CALL Aldebaran_VM_CMDList(GPU gpu,
	                                               const uint64_t* cmds);

	int32_t ALDEBARANAPI_CALL Aldebaran_SPIRV(uint32_t* spirvCode,
	                                          uint64_t spirvSize,
	                                          uint8_t** aldebaranCode,
	                                          uint64_t* aldebaranSize);

	void ALDEBARANAPI_CALL Aldebaran_SPIRV_Free(uint8_t* aldebaranCode);

	void ALDEBARANAPI_CALL Aldebaran_VM_End(GPU gpu);

#ifdef __cplusplus
}
#endif

#endif  // ALDEBARAN_H
