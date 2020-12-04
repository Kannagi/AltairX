#ifndef ALTAIR_VM_INTERNAL_H_DEFINED
#define ALTAIR_VM_INTERNAL_H_DEFINED

#include <base/vm.h>

typedef struct ArVirtualMachine_T
{
    ArProcessor processor;
    ArPhysicalMemory memory;
} ArVirtualMachine_T;


typedef enum Opcode
{
    OPCODE_UNKNOWN,

    //LSU
    OPCODE_LDM,
    OPCODE_STM,
    OPCODE_LDC,
    OPCODE_STC,
    OPCODE_LDMX,
    OPCODE_STMX,
    OPCODE_IN,
    OPCODE_OUT,
    OPCODE_OUTI,

    //ALU
    OPCODE_NOP,
    OPCODE_XCHG,
    //OPCODE_MOVE,
    OPCODE_MOVEI,
    OPCODE_ADD,
    OPCODE_ADDI,
    OPCODE_ADDQ,
    OPCODE_SUB,
    OPCODE_SUBI,
    OPCODE_SUBQ,
    OPCODE_MULS,
    OPCODE_MULSI,
    OPCODE_MULSQ,
    OPCODE_MULU,
    OPCODE_MULUI,
    OPCODE_MULUQ,
    OPCODE_DIVS,
    OPCODE_DIVSI,
    OPCODE_DIVSQ,
    OPCODE_DIVU,
    OPCODE_DIVUI,
    OPCODE_DIVUQ,
    OPCODE_AND,
    OPCODE_ANDI,
    OPCODE_ANDQ,
    OPCODE_OR,
    OPCODE_ORI,
    OPCODE_ORQ,
    OPCODE_XOR,
    OPCODE_XORI,
    OPCODE_XORQ,
    OPCODE_ASL,
    OPCODE_ASLI,
    OPCODE_ASLQ,
    OPCODE_LSL,
    OPCODE_LSLI,
    OPCODE_LSLQ,
    OPCODE_ASR,
    OPCODE_ASRI,
    OPCODE_ASRQ,
    OPCODE_LSR,
    OPCODE_LSRI,
    OPCODE_LSRQ,

    //BRU
    OPCODE_FBNE,
    OPCODE_FBEQ,
    OPCODE_FBL,
    OPCODE_FBLE,
    OPCODE_FBG,
    OPCODE_FBGE,
    OPCODE_BNE,
    OPCODE_BEQ,
    OPCODE_BLU,
    OPCODE_BLEU,
    OPCODE_BGU,
    OPCODE_BGEU,
    OPCODE_BLS,
    OPCODE_BLES,
    OPCODE_BGS,
    OPCODE_BGES,
    OPCODE_CMP,
    OPCODE_CMPI,
    OPCODE_FCMP,
    OPCODE_FCMPI,
    OPCODE_DCMP,
    OPCODE_DCMPI,
    OPCODE_JMP,
    OPCODE_CALL,
    OPCODE_JMPR,
    OPCODE_CALLR,
    OPCODE_RET,
} Opcode;

#define MAX_OPERANDS 3

typedef struct Operation
{
    Opcode op; //< the op code
    uint32_t operands[MAX_OPERANDS]; //< either register or immediate values
    uint32_t size; //< 0 = byte, 1 = word, 2 = doubleword, 3 = quadword
    uint32_t data; //< additionnal data, op dependent
} Operation;

#define DSRAM_SIZE     (128u * 1024u)
#define ISRAM_SIZE     (128u * 1024u)
#define CACHE_SIZE     (32u * 1024u)
#define IOSRAM_SIZE    (256u)
#define INTREG_COUNT   (64u)
#define FLOATREG_COUNT (128u)
#define MAX_OPCODE     (4u)

typedef struct ArProcessor_T
{
    ArProcessor next;
    ArVirtualMachine parent;

    uint8_t dsram [DSRAM_SIZE];
    uint8_t isram [ISRAM_SIZE];
    uint8_t cache [CACHE_SIZE];
    uint8_t iosram[IOSRAM_SIZE];

    uint64_t intRegisters  [INTREG_COUNT];
    uint32_t floatRegisters[FLOATREG_COUNT]; //It must be aligned on 64bits, previous field should force it, but it should be checked on every untested implementation

    uint32_t programCounter;
    uint32_t opcodes[MAX_OPCODE];
    uint32_t xchg;

    Operation operations[MAX_OPCODE];

} ArProcessor_T;

typedef struct ArPhysicalMemory_T
{
    ArPhysicalMemory next;
    ArVirtualMachine parent;

    uint8_t* memory;
    size_t size;

} ArPhysicalMemory_T;

#endif
