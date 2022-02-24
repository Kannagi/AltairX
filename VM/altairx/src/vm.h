
#define MEMORY_MAP_SPM1_BEGIN   (0x00000000)
#define MEMORY_MAP_ROM_BEGIN    (0x02000000)
#define MEMORY_MAP_IO_BEGIN     (0x04000000)
#define MEMORY_MAP_SPMT_BEGIN   (0x06000000)


#define MEMORY_MAP_SPM2_BEGIN   (0x08000000)

//32 BITS
#define MEMORY_MAP_SRAM_BEGIN   (0x20000000)
#define MEMORY_MAP_VRAM_BEGIN   (0x40000000)
#define MEMORY_MAP_WRAM_BEGIN   (0x80000000)

//IO
#define IO_CCIF (0x000000)
#define IO_GIF  (0x100000)
#define IO_SIF  (0x200000)
#define IO_US   (0x300000)
#define IO_SN   (0x400000)
#define IO_EW   (0x500000)
#define IO_EXT  (0x600000)
#define IO_DMA  (0x700000)

#define Z_MASK (0x01u)
#define S_MASK (0x02u)
#define U_MASK (0x04u)

#define AX_OPCODE_UNKNOWN -1
#define AX_ERROR_ILLEGAL_INSTRUCTION -1
#define AX_END_OF_CODE 1

#define AX_core_MAX 64
#define AX_core_MAX_OPERATIONS 4
#define AX_core_IREG_COUNT 64
#define AX_core_VREG_COUNT 64
#define AX_core_SPM_SIZE 0x8000 //(32 KiB)


#define AR_core_ICACHE_SIZE (0x10000/1024) //(64 KiB , 4-way)
#define AR_core_DCACHE_SIZE (0x8000/128)  //(32 KiB , 4-way)


typedef struct
{
    uint64_t opA,opB,opC;
    uint32_t unit1,unit2,size,id;
    double dopA,dopB,dopC;
    float  fopA[4],fopB[4],fopC[4];
    uint32_t regA,regB,regC;
}Execute;

typedef struct MMAP
{
    uint8_t *rom;
    uint8_t *spmt;
    uint8_t *spm2;
    uint8_t *io;

    uint8_t *spmram;
    uint8_t *vram;
    uint8_t *wram;


	uint32_t nrom;
	uint32_t nspmt;
	uint32_t nspm2;

	uint32_t nspmram;
	uint32_t nvram;
	uint32_t nwram;

}MMAP;



typedef struct core
{
	MMAP mmap;
	Execute operations[AX_core_MAX_OPERATIONS];

	uint32_t *wram;

    uint64_t instruction;
    uint64_t cycle;

    uint32_t opcodes[AX_core_MAX_OPERATIONS];

    uint32_t pc; //program-counter
    uint32_t br; //buffer-register
    uint32_t lr; //link-register
    uint32_t ir; //interrupt-register

    uint32_t imm;


    /// \brief CPU Flags register
    ///
    /// Bit 0:
    /// Bit 1: Z flag, 1 if not equal, 0 if equal
    /// Bit 2: S flag, 1 if lesser, 0 if greater (signed comparison)
    /// Bit 3: U flag, 1 if lesser, 0 if greater (unsigned comparison)
    uint16_t flags;
    uint8_t delay,delayop,swt,syscall;

    uint8_t spm[AX_core_SPM_SIZE];
    uint32_t icache[AR_core_ICACHE_SIZE];
    uint32_t dcacher[AR_core_DCACHE_SIZE];
    uint32_t dcacherw[AR_core_DCACHE_SIZE];

    uint64_t ireg[AX_core_IREG_COUNT];
    union
	{
		float vreg[AX_core_VREG_COUNT * 4];
		double dreg[AX_core_VREG_COUNT * 2];
		int32_t vireg[AX_core_VREG_COUNT * 4];
		uint16_t hreg[AX_core_VREG_COUNT * 8];
	};

}Core;

typedef struct Processor
{
	Core *core[AX_core_MAX];
    MMAP mmap;
    int ncore;
    int icore;
    int mode;

}Processor;



enum {
    AX_EXE_ALU = 0,
    AX_EXE_LSU,
    AX_EXE_BRU,
    AX_EXE_CMP,
    AX_EXE_VFPU,
    AX_EXE_FPUD,
    AX_EXE_EFU,
    AX_EXE_DMA,
    AX_EXE_OTHER,
    AX_EXE_NOP,
};

//ALU
enum {
    AX_OPCODE_ADD,
    AX_OPCODE_SUB,
    AX_OPCODE_XOR,
    AX_OPCODE_OR,
    AX_OPCODE_AND,
    AX_OPCODE_LSL,
    AX_OPCODE_ASR,
    AX_OPCODE_LSR,

    AX_OPCODE_MULS = 10,
    AX_OPCODE_MULU,
    AX_OPCODE_DIVS,
    AX_OPCODE_DIVU,
    AX_OPCODE_REMS,
    AX_OPCODE_REMU,


    AX_OPCODE_VOID = 0x10,
    AX_OPCODE_SEXT,
    AX_OPCODE_BOOL,
    AX_OPCODE_SMOVE,

    AX_OPCODE_SLTS,
    AX_OPCODE_SLTU,
    AX_OPCODE_VOID1,
    AX_OPCODE_MOVE,


    AX_OPCODE_MOVEINS = 0x1C,
    AX_OPCODE_MOVECYCLE,
    AX_OPCODE_MOVERI1,
    AX_OPCODE_MOVERI2,
};

//CMP
enum {
    AX_OPCODE_CMP,
    AX_OPCODE_CMPFR,
    AX_OPCODE_FCMP =0x04,
    AX_OPCODE_DCMP,
};

//LSU
enum {
    AX_OPCODE_LD,
    AX_OPCODE_ST,
    AX_OPCODE_LDV,
    AX_OPCODE_STV,

    AX_OPCODE_IPREFETCH,
    AX_OPCODE_IFLUSH,
    AX_OPCODE_PREFETCH,
    AX_OPCODE_FLUSH,

};

//BRU
enum {
    AX_OPCODE_BNE,
    AX_OPCODE_BEQ,
    AX_OPCODE_BL,
    AX_OPCODE_BLE,
    AX_OPCODE_BG,
    AX_OPCODE_BGE,
    AX_OPCODE_BLS,
    AX_OPCODE_BLES,
    AX_OPCODE_BGS,
    AX_OPCODE_BGES,
    AX_OPCODE_BRA,
    AX_OPCODE_LOOP,

    AX_OPCODE_JMP,
    AX_OPCODE_JMPBR,
    AX_OPCODE_CALL,
    AX_OPCODE_CALLBR,


    AX_OPCODE_RET = 0x10,
    AX_OPCODE_RETI,
    AX_OPCODE_SYSCALL,
    AX_OPCODE_INT,

};

//DMA
enum {
    AX_OPCODE_LDDMA,
    AX_OPCODE_STDMA,
    AX_OPCODE_WAIT,
};

//VFPU
enum {
    AX_OPCODE_FADD,
    AX_OPCODE_FSUB,
    AX_OPCODE_FMUL,
    AX_OPCODE_FMADD,

    AX_OPCODE_VFADD,
    AX_OPCODE_VFSUB,
    AX_OPCODE_VFMUL,
    AX_OPCODE_VFMADD,

    AX_OPCODE_VFADDS,
    AX_OPCODE_VFSUBS,
    AX_OPCODE_VFMULS,
    AX_OPCODE_VFMADDS,

    AX_OPCODE_FMSUB,
    AX_OPCODE_VFMSUB,
    AX_OPCODE_VFMSUBS,
    AX_OPCODE_VFSHUFFLE,


    AX_OPCODE_FMOVE = 0x00,
    AX_OPCODE_VFMOVE,

    AX_OPCODE_FNEG = 0x04,
    AX_OPCODE_FABS,
    AX_OPCODE_VFNEG,
    AX_OPCODE_VFABS,

    AX_OPCODE_VFTOH = 0x08,
    AX_OPCODE_VHTOF,
    AX_OPCODE_VFTOI,
    AX_OPCODE_VITOF,
    AX_OPCODE_VFTOD,
    AX_OPCODE_VDTOF,

    AX_OPCODE_DMOVE = 0x00,
    AX_OPCODE_DADD,
    AX_OPCODE_DSUB,
    AX_OPCODE_DMUL,

    AX_OPCODE_DABS,
    AX_OPCODE_DNEG,

    AX_OPCODE_DDIV = 0x08,
};

//OTHER
enum {
	AX_OPCODE_ENDP,
	AX_OPCODE_EXE,
};


enum {
    AX_REG_A = 61,
    AX_REG_P,
    AX_REG_Q,
};

enum {
	AX_ERROR_OPCODE = 0x01,
	AX_ERROR_EXE = 0x10,
	AX_ERROR_MEM = 0xFF,

};

int AX_decode(Core *core);
int AX_execute(Core *core);
void AX_syscall(Core *core);
int AX_syscall_emul(Core *core);

void AX_debug(Core *core);

int AX_exe_core(Core *core);

void AX_boot_rom(Core *core,uint8_t* rom,int n);

int AX_add_core(Processor *processor);

int AX_init_proc(Processor *processor);
int AX_init_proc_mem(Processor *processor);
void AX_init_mem(Processor *processor,int nwram,int nvram,int nsram,int nspmt,int nspm2);
int AX_load_prog(char *name,MMAP *mmap);
void *AX_Memory_Map(Core *core,uint64_t offset,uint32_t size);
