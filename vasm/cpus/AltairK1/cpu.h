/* cpu.h KutaragiV1-description header-file */
/* (c) in 2002 by Volker Barthelmann */
/* (c) in 2020 by Samy Meguenoun */

#define LITTLEENDIAN 1
#define BIGENDIAN 0
#define VASM_CPU_VU 1

/* maximum number of operands in one mnemonic */
#define MAX_OPERANDS 3

/* maximum number of mnemonic-qualifiers per mnemonic */
#define MAX_QUALIFIERS 2

/* maximum number of additional command-line-flags for this cpu */

/* data type to represent a target-address */
typedef int32_t taddr;
typedef uint32_t utaddr;

/* minimum instruction alignment */
#define INST_ALIGN 4

/* default alignment for n-bit data */
#define DATA_ALIGN(n) 1

/* operand class for n-bit data definitions */
#define DATA_OPERAND(n) OP_DATA

/* type to store each operand */
typedef struct {
    int type;
    int val,reg;
    int size;
    float fval;
    expr *value;
} operand;

#define K1            0x01

#define OP_VOID            0
#define OP_REG             1
#define OP_VP              2

#define OP_IMM             7
#define OP_IM1             8
#define OP_IM2             9
#define OP_IM3             10
#define OP_IM4             11

#define OP_IMR             20

#define OP_DATA            100




typedef struct {
  unsigned int cpu;
  unsigned int opcode;
} mnemonic_extension;

