/* cpu.h KutaragiV1-description header-file */
/* (c) in 2002 by Volker Barthelmann */
/* (c) in 2020 by Samy Meguenoun */

#define LITTLEENDIAN 1
#define BIGENDIAN 0 
#define VASM_CPU_ALTAIRK1 1 

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
#define DATA_ALIGN(n) (n>>8)

/* operand class for n-bit data definitions */
#define DATA_OPERAND(n) OP_DATA

/* type to store each operand */
typedef struct {
    int type;
    int val,reg,reg2;
    int size;
    float fval;
    expr *value; 
} operand;

#define K1                 0x01

#define OP_VOID            0x00

#define OP_REG             0x01
#define OP_VP              0x02
#define OP_VPI             0x03

#define OP_RFR             0x04
#define OP_RBR             0x05
#define OP_RLR             0x06
#define OP_RIR             0x07

#define OP_RGP             0x08
#define OP_RGQ             0x09

#define OP_RFF             0x0A

#define OP_IMM             0x10
#define OP_IMB             0x11
#define OP_IMH             0x12
#define OP_IMF             0x13
#define OP_IMD             0x14

#define OP_IMR             0x15
#define OP_RRG             0x16


#define OP_DATA            0x3e




typedef struct {
  unsigned int cpu;
  unsigned int opcode;
} mnemonic_extension;

