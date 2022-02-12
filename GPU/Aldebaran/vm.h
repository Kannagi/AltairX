#ifndef ALTAIR_VM_INTERNAL_H_DEFINED
#define ALTAIR_VM_INTERNAL_H_DEFINED


#define MIN(x, y) (x < y ? x : y)
#define MAX(x, y) (x > y ? x : y)

typedef struct ArVirtualMachine_T
{


} ArVirtualMachine_T;

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
#define MEMORY_MAP_SPM2_BEGIN   (0x200000000)
#define MEMORY_MAP_VRAM_BEGIN   (0x400000000)
#define MEMORY_MAP_RAM_BEGIN    (0x800000000)

#define MEMORY_MAP_OTHER        (0x300000000)



typedef struct
{
    uint32_t unit1,unit2,size,id,imm,immf;
    int32_t rA,rB,rC;
    float  fimm;
    double dimm;

}ArExecute_T;

typedef struct
{
    ArExecute_T operations[AR_PROCESSOR_MAX_OPERATIONS];

    uint64_t instruction;
    uint64_t cycle;

    uint32_t ireg2[AR_PROCESSOR_IREG_COUNT];
    float vreg[AR_PROCESSOR_VREG_COUNT*4*N_SHADER];
    double *dreg;
    uint32_t *ireg;

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

    uint8_t dsram [AR_PROCESSOR_DSRAM_SIZE];
    uint8_t isram [AR_PROCESSOR_ISRAM_SIZE];

    uint8_t *ram;
    uint8_t *rom;
    uint8_t *tsram;
    uint8_t *spm2;
    uint8_t *io;

}ComputeUnit;

int32_t Aldebaran_ExcecuteInstruction(ComputeUnit *processor);
int32_t Aldebaran_DecodeInstruction(ComputeUnit *processor);


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


    AK1_OPCODE_MOVE = 0x10,
    AK1_OPCODE_MOVESP,
};

//CMP
enum {
    AK1_OPCODE_CMP,
    AK1_OPCODE_FCMP =0x04,
};

//LSU
enum {
    AK1_OPCODE_LDM,
    AK1_OPCODE_STM,
    AK1_OPCODE_LDC,

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

    AK1_OPCODE_VFTOH = 0x08,
    AK1_OPCODE_VHTOF,
    AK1_OPCODE_VFTOI,
    AK1_OPCODE_VITOF,
    AK1_OPCODE_VFTOD,
    AK1_OPCODE_VDTOF,

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
