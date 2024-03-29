

#include <stdint.h>

typedef struct
{
    std::vector<uint32_t> rom;
    std::vector<uint32_t> spmt;
    std::vector<uint32_t> spm2;
    std::vector<uint32_t> io;
    std::vector<uint32_t> wram;

}MemoryMap;

#define MEMORY_MAP_SPM1_BEGIN   (0x00000000)
#define MEMORY_MAP_IO_BEGIN     (0x08000000)
#define MEMORY_MAP_ROM_BEGIN    (0x10000000)
#define MEMORY_MAP_SPMT_BEGIN   (0x20000000)
#define MEMORY_MAP_SPM2_BEGIN   (0x40000000)
#define MEMORY_MAP_WRAM_BEGIN   (0x80000000)

//IO
#define IO_CCIF (0x00000)
#define IO_GIF  (0x10000)
#define IO_SIF  (0x20000)
#define IO_US   (0x30000)
#define IO_SN   (0x40000)
#define IO_EW   (0x50000)
#define IO_EXT  (0x60000)
#define IO_DMA  (0x70000)

#define AX_core_MAX 64
#define AX_core_IREG_COUNT 64
#define AX_core_VREG_COUNT 64
#define AX_SPM_SIZE 0x4000

#define AR_core_ICACHE_SIZE (0x10000/1024) //(64 KiB , 4-way)
#define AR_core_DCACHE_SIZE (0x8000/128)  //(32 KiB , 4-way)

#define AX_IO_VOID  0
#define AX_IO_READ  1
#define AX_IO_WRITE 2

#define REG_ACC 56

#define REG_ZERO 63

#define Z_MASK 0x01
#define O_MASK 0x02
#define N_MASK 0x04

typedef struct
{
    uint32_t lr; //link-register
    uint32_t br; //branch-register
    uint32_t lc; //loop counter
    uint32_t fr; //flag register

    uint32_t pc; //program-counter
    uint32_t ir; //interrupt-register
    uint32_t cc; //cycle counter
    uint32_t ic; //instruction counter

    uint64_t ireg[AX_core_IREG_COUNT];

    uint64_t mduP,mduQ,mduQR;
    float efuQ;
    double efuQd;

    union
	{
        uint16_t hreg[AX_core_VREG_COUNT * 4];
		float freg[AX_core_VREG_COUNT * 2];
		double dreg[AX_core_VREG_COUNT];

		int64_t direg[AX_core_VREG_COUNT];
		int32_t fireg[AX_core_VREG_COUNT * 2];
	};

    uint8_t spm[AX_SPM_SIZE];
    int error;
    uint32_t cycle,instruction,syscall;

    
}Core;

class AltairX
{
private:
    std::vector<Core> array_core;
    

    uint32_t opcode1,opcode2;
	uint32_t ioe;
    
    void execute_unit(uint32_t rega,uint32_t regb,uint32_t regc,uint32_t unit,uint32_t size,uint32_t opcode,uint32_t tunit,uint32_t imm24);
    void execute_unit0(uint32_t rega,uint32_t regb,uint32_t regc,uint32_t unit,uint32_t size,uint32_t opcode,uint32_t imm24);
    void execute_unit1(uint32_t rega,uint32_t regb,uint32_t regc,uint32_t unit,uint32_t size,uint32_t opcode);

    void executeStore(void *reg,uint64_t offset,uint32_t size);
    void executeLoad(void *reg,uint64_t offset,uint32_t size);

    void execute_unit_double(uint32_t rega,uint32_t regb,uint32_t regc,uint32_t unit,uint32_t imm24);
    void execute_unit_float(uint32_t rega,uint32_t regb,uint32_t regc,uint32_t unit,uint32_t imm24);
    void execute_unit_half(uint32_t rega,uint32_t regb,uint32_t regc,uint32_t unit,uint32_t imm24);
    void execute_unit_falu(uint32_t rega,uint32_t regb,uint32_t regc,uint32_t unit,uint32_t imm24);

public:
    AltairX(int nwram,int nspmt,int nspm2);
    void load_prog(char *path);
    void load_kernel(char *path);

    void init_mask();

    void mode0();
    void mode1();
    void mode2();
    void mode3();
    void mode4();

    void execute();

    MemoryMap memorymap;
    int icore,ncore,mode,error;
    uint64_t rom_mask,spmt_mask,spm2_mask,wram_mask;
    uint32_t *wram,*spmt,*rom,*spm2,*io,topcode;
    
    Core *core;
};

//----------------------------
void altairx_debug(uint32_t opcode,int unit);
void altairx_io_read(Core *pcore,uint64_t offset,void *reg);

void altairx_io_write(Core *pcore,uint64_t offset,void *reg);

void core_init(Core &core);

void altairx_syscall_emul(Core *core,char *wram);

//-------------------------------
enum {
    //------------- ALU(0) -----

    AX_EXE_ALU_MOVEIX,
    AX_EXE_ALU_COP,
    AX_EXE_ALU_EXT,
    AX_EXE_ALU_INS,

    AX_EXE_ALU_MOVEI,
    AX_EXE_ALU_MOVEN,
    AX_EXE_ALU_UNK0,
    AX_EXE_ALU_MOVEUP,


    AX_EXE_ALU_UNK1,
    AX_EXE_ALU_SEXT,
    AX_EXE_ALU_RTL,
    AX_EXE_ALU_RTR,

    AX_EXE_ALU_ADC = 0x0A,
    AX_EXE_ALU_SBC,

    AX_EXE_ALU_CMP,
    AX_EXE_ALU_TEST,
    AX_EXE_ALU_TESTFR,
    AX_EXE_ALU_CMPBIT,

    //------------- ALU(1) -----

    AX_EXE_ALU_ADD,
    AX_EXE_ALU_SUB,
    AX_EXE_ALU_XOR,
    AX_EXE_ALU_OR,

    AX_EXE_ALU_AND,
    AX_EXE_ALU_LSL,
    AX_EXE_ALU_ASR,
    AX_EXE_ALU_LSR,

    AX_EXE_ALU_SE,
    AX_EXE_ALU_SEN,
    AX_EXE_ALU_SLTS,
    AX_EXE_ALU_SLTU,

    AX_EXE_ALU_SAND,
    AX_EXE_ALU_SBIT,
    AX_EXE_ALU_CMOVEN,
    AX_EXE_ALU_CMOVE,
    


    //------------- LSU(2) -------
    AX_EXE_LSU_LD,
    AX_EXE_LSU_ST,
    AX_EXE_LSU_FLD,
    AX_EXE_LSU_FST,

    AX_EXE_LSU_LDI,
    AX_EXE_LSU_STI,
    AX_EXE_LSU_FLDI,
    AX_EXE_LSU_FSTI,


    AX_EXE_LSU_LDSP,
    AX_EXE_LSU_STSP,
    AX_EXE_LSU_FLDSP,
    AX_EXE_LSU_FSTSP,

    AX_EXE_LSU_LDC,
    AX_EXE_LSU_STC,
    AX_EXE_LSU_FLDC,
    AX_EXE_LSU_FSTC,

    //------------- FPU(3) -----
    AX_EXE_FPU_FADD,
    AX_EXE_FPU_FSUB,
    AX_EXE_FPU_FMUL,
    AX_EXE_FPU_FNMUL,

    AX_EXE_FPU_HTOF,
    AX_EXE_FPU_FTOH,
    AX_EXE_FPU_ITOF,
    AX_EXE_FPU_FTOI,

    AX_EXE_FPU_FNEG,
    AX_EXE_FPU_FABS,
    AX_EXE_FPU_FMIN,
    AX_EXE_FPU_FMAX,

    AX_EXE_FPU_FMOVE,
    AX_EXE_FPU_FCMP,
    AX_EXE_FPU_FMOVEI,
    AX_EXE_FPU_FCMPI,

    //------------- ALU3(4) -----
    AX_EXE_ALU_ADD3,
    AX_EXE_ALU_SUB3,
    AX_EXE_ALU_XOR3,
    AX_EXE_ALU_OR3,

    AX_EXE_ALU_AND3,
    AX_EXE_ALU_LSL3,
    AX_EXE_ALU_ASR3,
    AX_EXE_ALU_LSR3,

    AX_EXE_ALU_SE3,
    AX_EXE_ALU_SEn3,
    AX_EXE_ALU_SLTS3,
    AX_EXE_ALU_SLTU3,

    AX_EXE_ALU_SAND3,
    AX_EXE_ALU_UNK33,
    AX_EXE_ALU_CMOVEN3,
    AX_EXE_ALU_CMOVE3,

    //------------- EFU(5) -----
    AX_EXE_EFU_FDIV,
    AX_EXE_EFU_FATAN2,
    AX_EXE_EFU_DDIV,
    AX_EXE_EFU_DATAN2,

    AX_EXE_EFU_FSQRT,
    AX_EXE_EFU_FSIN,
    AX_EXE_EFU_FATAN,
    AX_EXE_EFU_FEXP,

    AX_EXE_EFU_DSQRT,
    AX_EXE_EFU_DSIN,
    AX_EXE_EFU_DATAN,
    AX_EXE_EFU_DEXP,

    AX_EXE_EFU_FPOW,
    AX_EXE_EFU_DPOW,
    AX_EXE_EFU_EMOVETO,
    AX_EXE_EFU_EMOVEFROM,

    //------------- MDU(6) -----
    AX_EXE_MDU_DIV,
    AX_EXE_MDU_DIVU,
    AX_EXE_MDU_MUL,
    AX_EXE_MDU_MULU,

    AX_EXE_MDU_MOVETO,
    AX_EXE_MDU_MOVEFROM,
    AX_EXE_MDU_UNK0,
    AX_EXE_MDU_UNK1,

    AX_EXE_MDU_UNK2,
    AX_EXE_MDU_UNK3,
    AX_EXE_MDU_UNK4,
    AX_EXE_MDU_UNK5,

    AX_EXE_MDU_UNK6,
    AX_EXE_MDU_UNK7,
    AX_EXE_MDU_UNK8,
    AX_EXE_MDU_UNK9,


    //------------- BRU(7) -----
    AX_EXE_BRU_BNE,
    AX_EXE_BRU_BEQ,
    AX_EXE_BRU_BL,
    AX_EXE_BRU_BLE,

    AX_EXE_BRU_BG,
    AX_EXE_BRU_BGE,
    AX_EXE_BRU_BLS,
    AX_EXE_BRU_BLES,

    AX_EXE_BRU_BGS,
    AX_EXE_BRU_BGES,
    AX_EXE_BRU_BRA,
    AX_EXE_BRU_LOOP,

    AX_EXE_BRU_JUMP,
    AX_EXE_BRU_CALL,
    AX_EXE_BRU_CALLR,
    AX_EXE_BRU_OTHER  = 0x7F,
    AX_EXE_BRU_JUMPBR = 0x7F,
    AX_EXE_BRU_CALLBR = 0x7F,
    AX_EXE_BRU_RETI   = 0x7F,
    AX_EXE_BRU_RET    = 0x7F,
    AX_EXE_BRU_SYSCALL= 0x7F,
    AX_EXE_BRU_INT    = 0x7F,


    //------------- CU(5) -----
    AX_EXE_CU_MOVETO  = 0x50,
    AX_EXE_CU_MOVEFROM,
    AX_EXE_CU_SYNC,
    AX_EXE_CU_MMU,


    //------------- VU(5) -----
    AX_EXE_VU_VECTOR2  = 0x60,
    AX_EXE_VU_VECTOR4,
    AX_EXE_VU_VECTOR8,
    AX_EXE_VU_INV,

    AX_EXE_VU_VECTOR2X2,
    AX_EXE_VU_VECTOR4X2,
    AX_EXE_VU_VECTOR8X2,
    AX_EXE_VU_INVX2,



    AX_EX_FPU_FADD  = 0x00,
    AX_EX_FPU_FSUB,
    AX_EX_FPU_FMUL,
    AX_EX_FPU_FNMUL,

    AX_EX_FPU_HTOF,
    AX_EX_FPU_FTOH,
    AX_EX_FPU_ITOF,
    AX_EX_FPU_FTOI,

    AX_EX_FPU_FNEG,
    AX_EX_FPU_FABS,
    AX_EX_FPU_FMIN,
    AX_EX_FPU_FMAX,

    AX_EX_FPU_FMOVE,
    AX_EX_FPU_FCMP,
    AX_EX_FPU_FMOVEI,
    AX_EX_FPU_FCMPI,

};
//-------------