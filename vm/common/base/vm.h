#ifndef ALTAIR_VM_H_INCLUDED
#define ALTAIR_VM_H_INCLUDED

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _MSC_VER
#   ifdef AR_EXPORT
#       define AR_API __declspec( dllexport )
#   else
#       define AR_API __declspec( dllimport )
#   endif
#else
#   define AR_API
#endif

#if defined(__cplusplus) || defined(_MSC_VER)
#   define AR_RESTRICT __restrict
#else
#   define AR_RESTRICT restrict
#endif

#define ALTAIR_DEFINE_TYPE(name) typedef struct name##_T* name

ALTAIR_DEFINE_TYPE(ArVirtualMachine);
ALTAIR_DEFINE_TYPE(ArProcessor);
ALTAIR_DEFINE_TYPE(ArPhysicalMemory);
ALTAIR_DEFINE_TYPE(ArGraphicsProcessor);
ALTAIR_DEFINE_TYPE(ArScreen);

typedef enum ArResult
{
    AR_END_OF_CODE = 1,
    AR_SUCCESS = 0,
    AR_ERROR_ILLEGAL_INSTRUCTION = -1,
    AR_ERROR_INVALID_CODE = -2,
    AR_ERROR_TOO_MANY_OBJECTS = -3,
    AR_ERROR_MEMORY_OUT_OF_RANGE = -4,
    AR_ERROR_PHYSICAL_MEMORY_OUT_OF_RANGE = -5,
    AR_ERROR_HOST_OUT_OF_MEMORY = -256,
} ArResult;

typedef enum ArStructureType
{
    AR_STRUCTURE_TYPE_VIRTUAl_MACHINE_CREATE_INFO = 0,
    AR_STRUCTURE_TYPE_PROCESSOR_CREATE_INFO = 1,
    AR_STRUCTURE_TYPE_PHYSICAL_MEMORY_CREATE_INFO = 2,
    AR_STRUCTURE_TYPE_GRAPHICS_PROCESSOR_CREATE_INFO = 3,
    AR_STRUCTURE_TYPE_SCREEN_CREATE_INFO = 4,
} ArStructureType;

typedef enum ArOpcode
{
    AR_OPCODE_UNKNOWN,

    //AGU
    AR_OPCODE_LDDMA,
    AR_OPCODE_STDMA,
    AR_OPCODE_LDDMAR,
    AR_OPCODE_STDMAR,
    AR_OPCODE_DMAIR,
    AR_OPCODE_LDDMAL,
    AR_OPCODE_STDMAL,
    AR_OPCODE_CLEARC,
    AR_OPCODE_WAIT,

    //LSU
    AR_OPCODE_LDM,
    AR_OPCODE_LDMI,
    AR_OPCODE_STM,
    AR_OPCODE_STMI,
    AR_OPCODE_LDC,
    AR_OPCODE_LDCI,
    AR_OPCODE_STC,
    AR_OPCODE_STCI,
    AR_OPCODE_IN,
    AR_OPCODE_OUT,
    AR_OPCODE_OUTI,
    AR_OPCODE_LDMV,
    AR_OPCODE_LDMVI,
    AR_OPCODE_LDCV,
    AR_OPCODE_LDCVI,
    AR_OPCODE_STMV,
    AR_OPCODE_STMVI,
    AR_OPCODE_STCV,
    AR_OPCODE_STCVI,
    AR_OPCODE_LDMIL,
    AR_OPCODE_STMIL,
    AR_OPCODE_LDCIL,
    AR_OPCODE_STCIL,
    AR_OPCODE_LDMVIL,
    AR_OPCODE_STMVIL,
    AR_OPCODE_LDCVIL,
    AR_OPCODE_STCVIL,

    //ALU
    AR_OPCODE_NOP,
    AR_OPCODE_MOVEI,
    AR_OPCODE_MOVELRL,
    AR_OPCODE_MOVELRS,
    AR_OPCODE_MOVEBR,
    AR_OPCODE_ADD,
    AR_OPCODE_ADDI,
    AR_OPCODE_ADDQ,
    AR_OPCODE_SUB,
    AR_OPCODE_SUBI,
    AR_OPCODE_SUBQ,
    AR_OPCODE_MULS,
    AR_OPCODE_MULSI,
    AR_OPCODE_MULSQ,
    AR_OPCODE_MULU,
    AR_OPCODE_MULUI,
    AR_OPCODE_MULUQ,
    AR_OPCODE_DIVS,
    AR_OPCODE_DIVSI,
    AR_OPCODE_DIVSQ,
    AR_OPCODE_DIVU,
    AR_OPCODE_DIVUI,
    AR_OPCODE_DIVUQ,
    AR_OPCODE_AND,
    AR_OPCODE_ANDI,
    AR_OPCODE_ANDQ,
    AR_OPCODE_OR,
    AR_OPCODE_ORI,
    AR_OPCODE_ORQ,
    AR_OPCODE_XOR,
    AR_OPCODE_XORI,
    AR_OPCODE_XORQ,
    AR_OPCODE_ASL,
    AR_OPCODE_ASLI,
    AR_OPCODE_ASLQ,
    AR_OPCODE_LSL,
    AR_OPCODE_LSLI,
    AR_OPCODE_LSLQ,
    AR_OPCODE_ASR,
    AR_OPCODE_ASRI,
    AR_OPCODE_ASRQ,
    AR_OPCODE_LSR,
    AR_OPCODE_LSRI,
    AR_OPCODE_LSRQ,

    //CMP
    AR_OPCODE_CMP,
    AR_OPCODE_CMPI,
    AR_OPCODE_PCMP,
    AR_OPCODE_PCMPI,

    //BRU
    AR_OPCODE_BNE,
    AR_OPCODE_BEQ,
    AR_OPCODE_BL,
    AR_OPCODE_BLE,
    AR_OPCODE_BG,
    AR_OPCODE_BGE,
    AR_OPCODE_BLS,
    AR_OPCODE_BLES,
    AR_OPCODE_BGS,
    AR_OPCODE_BGES,
    AR_OPCODE_BRA,
    AR_OPCODE_JMP,
    AR_OPCODE_CALL,
    AR_OPCODE_JMPR,
    AR_OPCODE_CALLR,
    AR_OPCODE_SWT,
    AR_OPCODE_ENDP,
    AR_OPCODE_RET,

    //VPU
    //AR_OPCODE_PADD,
    //AR_OPCODE_PSUB,
    //AR_OPCODE_PMUL,
    //AR_OPCODE_PMULADD,

    //Fake instructions
    AR_OPCODE_MOVE,
} ArOpcode;

typedef enum ArPhysicalMemoryRole
{
    AR_PHYSICAL_MEMORY_ROLE_ROM,
    AR_PHYSICAL_MEMORY_ROLE_RAM,
} ArPhysicalMemoryRole;

typedef struct ArVirtualMachineCreateInfo
{
    ArStructureType sType; //< The type of this structure
    void* pNext;           //< A pointer to the next structure
} ArVirtualMachineCreateInfo;

typedef struct ArProcessorCreateInfo
{
    ArStructureType sType;     //< The type of this structure
    void* pNext;               //< A pointer to the next structure
    const uint32_t* pBootCode; //< The boot code as an array of 32-bits unsigned integers
    uint32_t bootCodeSize;     //< The number of op-codes in pBootCode array
} ArProcessorCreateInfo;

typedef struct ArPhysicalMemoryCreateInfo
{
    ArStructureType sType; //< The type of this structure
    void* pNext;           //< A pointer to the next structure
    void* pMemory;         //< A pointer to the memory beginning
    uint64_t size;         //< The number of bytes of the memory
    ArPhysicalMemoryRole role;
} ArPhysicalMemoryCreateInfo;

typedef struct ArGraphicsProcessorCreateInfo
{
    ArStructureType sType;     //< The type of this structure
    void* pNext;               //< A pointer to the next structure
} ArGraphicsProcessorCreateInfo;

typedef struct ArScreenCreateInfo
{
    ArStructureType sType;     //< The type of this structure
    void* pNext;               //< A pointer to the next structure
    ArGraphicsProcessor graphicsProcessor; //< The GPU the screen is bound to
    uint32_t width; //< The screen's width
    uint32_t height; //< The screen's height
} ArScreenCreateInfo;

#define AR_OPERATION_MAX_OPERANDS 3

typedef struct ArOperation
{
    ArOpcode op; //< The opcode
    uint32_t operands[AR_OPERATION_MAX_OPERANDS]; //< Either register or immediate values
    uint32_t size; //< The size parameter, real meaning depend on instruction
    uint32_t data; //< Additionnal data, real meaning depend on instruction
} ArOperation;

#define AR_PROCESSOR_DSRAM_SIZE     (128u * 1024u)
#define AR_PROCESSOR_ISRAM_SIZE     (128u * 1024u)
#define AR_PROCESSOR_CACHE_SIZE     (32u * 1024u)
#define AR_PROCESSOR_IOSRAM_SIZE    (256u)
#define AR_PROCESSOR_IREG_COUNT     (64u)
#define AR_PROCESSOR_VREG_COUNT     (64u)
#define AR_PROCESSOR_MAX_OPERATIONS (4u)

#define AR_GRAPHICS_PROCESSOR_REG_COUNT (64u)
// L1
#define AR_GRAPHICS_PROCESSOR_DSRAM_SIZE (128u * 1024u) // 128 KiB
#define AR_GRAPHICS_PROCESSOR_ISRAM_SIZE (128u * 1024u) // 128 KiB
#define AR_GRAPHICS_PROCESSOR_L1SPM_SIZE (32u  * 1024u) // 32 KiB
// L2
#define AR_GRAPHICS_PROCESSOR_TEXTURE_CACHE_SIZE (1u * 1'048'576u) // 1 MiB
#define AR_GRAPHICS_PROCESSOR_BUFFER_CACHE_SIZE  (1u * 1'048'576u) // 1 MiB
#define AR_GRAPHICS_PROCESSOR_L2SPM_SIZE         (2u * 1'048'576u) // 2 MiB
// L3
#define AR_GRAPHICS_PROCESSOR_1T_SRAM_SIZE   (64u * 1'048'576u) // 64 MiB

#define AR_GRAPHICS_PROCESSOR_MAX_OPERATIONS (2u)

typedef struct ArProcessorMemoryInfo
{
    uint8_t*  dsram;
    uint8_t*  isram;
    uint8_t*  cache;
    uint8_t*  iosram;
    uint64_t* ireg;
    uint64_t* vreg;
} ArProcessorMemoryInfo;

#ifndef AR_NO_PROTOTYPES

/** \brief Creates a new virtual machine

    \param pVirtualMachine A pointer to a ArVirtualMachine handle
    \param pInfo A pointer on a valid ArVirtualMachineCreateInfo instance

    \return AR_SUCCESS in case of success
            AR_ERROR_HOST_OUT_OF_MEMORY if a host memory allocation failed
*/
AR_API ArResult arCreateVirtualMachine(ArVirtualMachine* pVirtualMachine, const ArVirtualMachineCreateInfo* pInfo);

/** \brief Creates a new processor within a virtual machine

    \param virtualMachine An ArVirtualMachine handle
    \param pInfo A pointer on a valid ArProcessorCreateInfo instance
    \param pProcessor A pointer to a ArProcessor handle

    \return AR_SUCCESS in case of success
            AR_ERROR_INVALID_CODE if the code is empty, or its size is not a multiple of 4
            AR_ERROR_HOST_OUT_OF_MEMORY if a host memory allocation failed
*/
AR_API ArResult arCreateProcessor(ArVirtualMachine virtualMachine, const ArProcessorCreateInfo* pInfo, ArProcessor* pProcessor);

/** \brief Creates a new physical memory device within a virtual machine

    \param virtualMachine An ArVirtualMachine handle
    \param pInfo A pointer on a valid ArPhysicalMemoryCreateInfo instance
    \param pProcessor A pointer to a ArPhysicalMemory handle

    \return AR_SUCCESS in case of success
            AR_ERROR_HOST_OUT_OF_MEMORY if a host memory allocation failed
            AR_ERROR_TOO_MANY_OBJECTS if there is already a physical memory instance inside the virtual machine
*/
AR_API ArResult arCreatePhysicalMemory(ArVirtualMachine virtualMachine, const ArPhysicalMemoryCreateInfo* pInfo, ArPhysicalMemory* pMemory);

/** \brief Creates a new graphics processor within a virtual machine

    \param virtualMachine An ArVirtualMachine handle
    \param pInfo A pointer on a valid ArGraphicsProcessorCreateInfo instance
    \param pGraphicsProcessor A pointer to a ArGraphicsProcessor handle

    \return AR_SUCCESS in case of success
            AR_ERROR_HOST_OUT_OF_MEMORY if a host memory allocation failed
*/
AR_API ArResult arCreateGraphicsProcessor(ArVirtualMachine virtualMachine, const ArGraphicsProcessorCreateInfo* pInfo, ArGraphicsProcessor* pGraphicsProcessor);

/** \brief Creates a new screen within a virtual machine

    \param virtualMachine An ArVirtualMachine handle
    \param pInfo A pointer on a valid ArScreenCreateInfo instance
    \param pScreen A pointer to a ArScreen handle

    \return AR_SUCCESS in case of success
            AR_ERROR_HOST_OUT_OF_MEMORY if a host memory allocation failed
*/
AR_API ArResult arCreateScreen(ArVirtualMachine virtualMachine, const ArScreenCreateInfo* pInfo, ArScreen* pScreen);

/** \brief Decode the next instructions and increment the program counter

    \param processor An ArProcessor handle

    \return AR_SUCCESS in case of success
            AR_ERROR_ILLEGAL_INSTRUCTION if the op-code is illegal
            AR_ERROR_HOST_OUT_OF_MEMORY if a host memory allocation failed
*/
AR_API ArResult arProcessorDecodeInstruction(ArProcessor processor);

/** \brief Run the instruction on the processor, and update its status

    \param processor An ArProcessor handle

    \return AR_SUCCESS in case of success
            AR_END_OF_CODE if the processor reached the end of its code
            AR_ERROR_ILLEGAL_INSTRUCTION if the op-code is illegal
            AR_ERROR_HOST_OUT_OF_MEMORY if a host memory allocation failed
*/
AR_API ArResult arProcessorExecuteInstruction(ArProcessor processor);

/** \brief Run the instruction on the processor, and update its status

    \param processor An ArProcessor handle

    \return AR_SUCCESS in case of success
            AR_ERROR_ILLEGAL_INSTRUCTION if the op-code is illegal, or if the instruction tries to access non-existing physical memory
            AR_ERROR_MEMORY_OUT_OF_RANGE if the final address + size of processor's SRAM is out of range\
            AR_ERROR_PHYSICAL_MEMORY_OUT_OF_RANGE if the final address + size of machine's physical memory is out of range\
            AR_ERROR_HOST_OUT_OF_MEMORY if a host memory allocation failed
*/
AR_API ArResult arProcessorExecuteDirectMemoryAccess(ArProcessor processor);

/** \brief Decode the next instructions and increment the program counter

    \param graphicsProcessor An ArGraphicsProcessor handle

    \return AR_SUCCESS in case of success
            AR_ERROR_ILLEGAL_INSTRUCTION if the op-code is illegal
            AR_ERROR_HOST_OUT_OF_MEMORY if a host memory allocation failed
*/
AR_API ArResult arGraphicsProcessorDecodeInstruction(ArGraphicsProcessor graphicsProcessor);

/** \brief Run the instruction on the graphics processor, and update its status

    \param graphicsProcessor An ArGraphicsProcessor handle

    \return AR_SUCCESS in case of success
            AR_END_OF_CODE if the graphics processor reached the end of its code
            AR_ERROR_ILLEGAL_INSTRUCTION if the op-code is illegal
            AR_ERROR_HOST_OUT_OF_MEMORY if a host memory allocation failed
*/
AR_API ArResult arGraphicsProcessorExecuteInstruction(ArGraphicsProcessor graphicsProcessor);

/** \brief Get a copy of current processor fetch operations.

    \param processor A ArProcessor handle
    \param pOutput A pointer to a ArOperation structure array to be filled. The array's size must be of at least AR_PROCESSOR_MAX_OPERATIONS.
    \param pCount A pointer to a size_t, the value is updated with the current amount of decoded instructions.
*/
AR_API void arGetProcessorOperations(ArProcessor processor, ArOperation* pOutput, size_t* pCount);

/** \brief Get a pointer on internal memories of the processor.

    \param processor An ArProcessor handle
    \param pOutput A pointer to a ArProcessorMemoryInfo structure to be filled. The pointers returned by this fonction never expire until the destruction of the processor.
*/
AR_API void arGetProcessorMemoryInfo(ArProcessor processor, ArProcessorMemoryInfo* pOutput);

/** \brief Destroy a virtual machine

    All subobjects must have been freed

    \param virtualMachine An ArVirtualMachine handle
*/
AR_API void arDestroyVirtualMachine(ArVirtualMachine virtualMachine);

/** \brief Destroy a processor within the virtual machine

    Must be called before any call to arDestroyVirtualMachine

    \param virtualMachine An ArVirtualMachine handle
    \param processor An ArProcessor handle
*/
AR_API void arDestroyProcessor(ArVirtualMachine virtualMachine, ArProcessor processor);

/** \brief Destroy a physical memory device within a virtual machine

    Must be called before any call to arDestroyVirtualMachine

    \param virtualMachine An ArVirtualMachine handle
    \param processor An ArPhysicalMemory handle
*/
AR_API void arDestroyPhysicalMemory(ArVirtualMachine virtualMachine, ArPhysicalMemory memory);

/** \brief Destroy a graphics processor within the virtual machine

    Must be called before any call to arDestroyVirtualMachine

    \param virtualMachine An ArVirtualMachine handle
    \param graphicsProcessor An ArGraphicsProcessor handle
*/
AR_API void arDestroyGraphicsProcessor(ArVirtualMachine virtualMachine, ArGraphicsProcessor graphicsProcessor);

/** \brief Destroy a screen within the virtual machine

    Must be called before any call to arDestroyVirtualMachine

    \param virtualMachine An ArVirtualMachine handle
    \param screen An ArScreen handle
*/
AR_API void arDestroyScreen(ArVirtualMachine virtualMachine, ArScreen screen);

#endif

typedef ArResult (*PFN_arCreateVirtualMachine)(ArVirtualMachine* pVirtualMachine, const ArVirtualMachineCreateInfo* pInfo);
typedef ArResult (*PFN_arCreateProcessor)(ArVirtualMachine virtualMachine, const ArProcessorCreateInfo* pInfo, ArProcessor* pProcessor);
typedef ArResult (*PFN_arCreatePhysicalMemory)(ArVirtualMachine virtualMachine, const ArPhysicalMemoryCreateInfo* pInfo, ArPhysicalMemory* pMemory);
typedef ArResult(*PFN_arCreateGraphicsProcessor)(ArVirtualMachine virtualMachine, const ArGraphicsProcessorCreateInfo* pInfo, ArGraphicsProcessor* pGraphicsProcessor);
typedef ArResult(*PFN_arCreateScreen)(ArVirtualMachine virtualMachine, const ArScreenCreateInfo* pInfo, ArScreen* pScreen);

typedef ArResult (*PFN_arProcessorDecodeInstruction)(ArProcessor processor);
typedef ArResult (*PFN_arProcessorExecuteInstruction)(ArProcessor processor);
typedef ArResult (*PFN_arProcessorExecuteDirectMemoryAccess)(ArProcessor processor);

typedef ArResult (*PFN_arGraphicsProcessorDecodeInstruction)(ArGraphicsProcessor graphicsProcessor);
typedef ArResult (*PFN_arGraphicsProcessorExecuteInstruction)(ArGraphicsProcessor graphicsProcessor);

typedef void (*PFN_arGetProcessorOperations)(ArProcessor processor, ArOperation* pOutput, size_t* pCount);
typedef void (*PFN_arGetProcessorMemoryInfo)(ArProcessor processor, ArProcessorMemoryInfo* pOutput);

typedef void (*PFN_arDestroyVirtualMachine)(ArVirtualMachine virtualMachine);
typedef void (*PFN_arDestroyProcessor)(ArVirtualMachine virtualMachine, ArProcessor processor);
typedef void (*PFN_arDestroyPhysicalMemory)(ArVirtualMachine virtualMachine, ArPhysicalMemory memory);
typedef void (*PFN_arDestroyGraphicsProcessor)(ArVirtualMachine virtualMachine, ArGraphicsProcessor graphicsProcessor);
typedef void (*PFN_arDestroyScreen)(ArVirtualMachine virtualMachine, ArScreen screen);

#undef ALTAIR_DEFINE_TYPE

#ifdef __cplusplus
}
#endif

#endif
