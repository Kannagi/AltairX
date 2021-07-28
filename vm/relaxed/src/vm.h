#ifndef ALTAIR_VM_INTERNAL_H_DEFINED
#define ALTAIR_VM_INTERNAL_H_DEFINED

#include <base/vm.h>

#define MIN(x, y) (x < y ? x : y)
#define MAX(x, y) (x > y ? x : y)

typedef struct ArVirtualMachine_T
{
    ArProcessor processor;
    ArPhysicalMemory ram;
    ArPhysicalMemory rom;

} ArVirtualMachine_T;

ArPhysicalMemory* getMemoryByRole(ArVirtualMachine virtualMachine, ArPhysicalMemoryRole role);

#define XCHG_MASK (0x01u)
#define Z_MASK (0x02u)
#define S_MASK (0x04u)
#define U_MASK (0x08u)

/*
0x0 0000 0000 - 0x0 FFFF FFFF ------- ROM
0x1 0000 0000 - 0x1 FFFF FFFF ------- 1T-SRAM Graphic/Program
0x2 0000 0000 - 0xF FFFF FFFF ------- SPM L2
0x3 0000 0000 - 0x0 FFFF FFFF ------- I/O

0x8 0000 0000 - 0xF FFFF FFFF ------- RAM (32 GB max )
36 bits

0x8XXX XXXX XXXX Uncached accelered (read)
0x4XXX XXXX XXXX cached accelered (write)

TLB 32 entries
X XXX0 0000 (bloc 1MB)

I/O
0x3 0000 0000 - 0x3 000F FFFF I/O core control
0x3 0010 0000 - 0x3 001F FFFF I/O GPU
0x3 0020 0000 - 0x3 002F FFFF I/O SOUND
0x3 0030 0000 - 0x3 003F FFFF I/O USB/SD
0x3 0040 0000 - 0x3 004F FFFF I/O SATA/NVME
0x3 0050 0000 - 0x3 005F FFFF I/O ETHERNET/WIFI
0x3 0060 0000 - 0x3 006F FFFF I/O EXTENSION/PCI
0x3 0070 0000 - 0x3 007F FFFF I/O OTHER (DMA) (PS/2)

*/

#define MEMORY_MAP_ROM_BEGIN    (0x000000000)
#define MEMORY_MAP_1TSRAM_BEGIN (0x100000000)
#define MEMORY_MAP_SPM2_BEGIN   (0x200000000)
#define MEMORY_MAP_IO_BEGIN     (0x300000000)
#define MEMORY_MAP_VRAM_BEGIN   (0x400000000)
#define MEMORY_MAP_RAM_BEGIN    (0x800000000)

#define MEMORY_MAP_OTHER        (0x300000000)


#define IO_CCIF (0x000000)
#define IO_GIF  (0x100000)
#define IO_SIF  (0x200000)
#define IO_US   (0x300000)
#define IO_SN   (0x400000)
#define IO_EW   (0x500000)
#define IO_EXT  (0x600000)
#define IO_DMA  (0x700000)

typedef struct
{
    uint64_t opA,opB,opC;
    uint32_t unit1,unit2,size,id;
    double dopA,dopB,dopC;
    float  fopA[4],fopB[4],fopC[4];

}ArExecute_T;

typedef struct ArProcessor_T
{
    ArProcessor next;
    ArVirtualMachine parent;

    ArExecute_T operations[AR_PROCESSOR_MAX_OPERATIONS];

    uint8_t dsram [AR_PROCESSOR_DSRAM_SIZE];
    uint8_t isram [AR_PROCESSOR_ISRAM_SIZE];
    //uint8_t cache [AR_PROCESSOR_CACHE_SIZE];

    uint64_t ireg[AR_PROCESSOR_IREG_COUNT];
    float vreg[AR_PROCESSOR_VREG_COUNT*4];
    double *dreg;

    uint64_t instruction;;
    uint64_t cycle;
    uint32_t swt;

    uint32_t opcodes[AR_PROCESSOR_MAX_OPERATIONS];

    uint16_t delay,delayop,imm;
    uint16_t pc; //program-counter
    uint16_t br; //buffer-register
    uint16_t lr; //link-register
    
    /// \brief CPU Flags register
    ///
    /// Bit 0: 
    /// Bit 1: Z flag, 1 if not equal, 0 if equal
    /// Bit 2: S flag, 1 if lesser, 0 if greater (signed comparison)
    /// Bit 3: U flag, 1 if lesser, 0 if greater (unsigned comparison)
    uint16_t flags;

    uint8_t *ram;
    uint8_t *rom;
    uint8_t *tsramCPU;
    uint8_t *tsramGPU;
    uint8_t *spm2;
    uint8_t *io;

} ArProcessor_T;

typedef struct ArPhysicalMemory_T
{
    ArVirtualMachine parent;

    uint8_t* memory;
    size_t size;
    ArPhysicalMemoryRole role;

} ArPhysicalMemory_T;

uint32_t opcodeSetSize(ArProcessor restrict processor);


enum {
    AK1_EXE_ALU,
    AK1_EXE_LSU,
    AK1_EXE_BRU,
    AK1_EXE_CMP,
    AK1_EXE_VFPU,
    AK1_EXE_DMA,
    AK1_EXE_OTHER,
    AK1_EXE_NOP,
};

//ALU
enum {
    AK1_OPCODE_ADD,
    AK1_OPCODE_SUB,
    AK1_OPCODE_XOR,
    AK1_OPCODE_OR,
    AK1_OPCODE_AND,
    AK1_OPCODE_LSL,
    AK1_OPCODE_ASR,
    AK1_OPCODE_LSR,

    AK1_OPCODE_MULS,
    AK1_OPCODE_MULU,
    AK1_OPCODE_DIVS,
    AK1_OPCODE_DIVU,
    AK1_OPCODE_REMS,
    AK1_OPCODE_REMU,
    
    
    AK1_OPCODE_MOVE = 0x10,
    AK1_OPCODE_MOVESP,
    AK1_OPCODE_MOVEINS,
    AK1_OPCODE_MOVECYCLE,
};

//CMP
enum {
    AK1_OPCODE_CMP,
    AK1_OPCODE_FCMP =0x04,
    AK1_OPCODE_DCMP,
};

//LSU
enum {
    AK1_OPCODE_LDM,
    AK1_OPCODE_STM,
    AK1_OPCODE_LDC,
    AK1_OPCODE_STC,

    AK1_OPCODE_LDMV,
    AK1_OPCODE_STMV,
    AK1_OPCODE_LDCV,
    AK1_OPCODE_STCV,

    AK1_OPCODE_PREFETCH,
    AK1_OPCODE_FLUSH,
};

//BRU
enum {
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
    AK1_OPCODE_RETI,

};

//DMA
enum {
    AK1_OPCODE_LDDMA,
    AK1_OPCODE_STDMA,
    AK1_OPCODE_LDDMACL,
    AK1_OPCODE_STDMACL,
    AK1_OPCODE_DMAI,
    AK1_OPCODE_WAIT = 7,
};

//VFPU
enum {
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

    AK1_OPCODE_VFTOD = 0x0C,
    AK1_OPCODE_VDTOF,

    AK1_OPCODE_DMOVE = 0x00,
    AK1_OPCODE_DADD,
    AK1_OPCODE_DSUB,
    AK1_OPCODE_DMUL,

    AK1_OPCODE_DABS,
    AK1_OPCODE_DNEG,

    AK1_OPCODE_DDIV = 0x08,
};

//OTHER
enum {
    AK1_OPCODE_INT,
    AK1_OPCODE_ENDP,
};


enum {
    AK1_REG_A = 61,
    AK1_REG_P,
    AK1_REG_Q,
};
#define AK1_OPCODE_UNKNOWN -1

#endif
