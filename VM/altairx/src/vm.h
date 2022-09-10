
#define MEMORY_MAP_SPM1_BEGIN   (0x00000000)
#define MEMORY_MAP_IO_BEGIN     (0x02000000)
#define MEMORY_MAP_ROM_BEGIN    (0x04000000)
#define MEMORY_MAP_SPMT_BEGIN   (0x08000000)
#define MEMORY_MAP_SPM2_BEGIN   (0x10000000)
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

#define AX_IO_VOID  0
#define AX_IO_READ  1
#define AX_IO_WRITE 2
//---------------------

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

//---------------------

typedef struct core
{
	MMAP mmap;


	uint32_t *wram;

    uint64_t instruction;
    uint64_t cycle;
    uint64_t bandwidth;

	uint64_t icachemiss,dcachemiss;
    uint64_t icachemiss_cycle,dcachemiss_cycle;

    //uint32_t opcodes[AX_core_MAX_OPERATIONS];

    uint32_t pc; //program-counter
    uint32_t br; //buffer-register
    uint32_t lr; //link-register
    uint32_t ir; //interrupt-register

    uint32_t imm,error,iorw;
	uint32_t opcode1,opcode2;
	uint32_t io;

    /// \brief CPU Flags register
    ///
    /// Bit 0:
    /// Bit 1: Z flag, 1 if not equal, 0 if equal
    /// Bit 2: S flag, 1 if lesser, 0 if greater (signed comparison)
    /// Bit 3: U flag, 1 if lesser, 0 if greater (unsigned comparison)
    uint16_t flags;
    uint8_t delay,delayop,syscall;

	uint8_t busy_reg[AX_core_IREG_COUNT];
	uint8_t busy_vreg[AX_core_VREG_COUNT];
	uint8_t busy_flag;

    uint8_t spm[AX_core_SPM_SIZE];
    uint32_t icache[AR_core_ICACHE_SIZE];
    uint32_t dcache[AR_core_DCACHE_SIZE*2];
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

//---------------------




//---------------------
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

void AX_decode_execute(Core *core);
void AX_syscall(Core *core);
int AX_syscall_emul(Core *core);

void AX_debug(Core *core);
void AX_IO_Read(Core *core,uint64_t offset,void *data);
void AX_IO_Write(Core *core,uint64_t offset,void *data);

int AX_exe_core(Core *core);
int AX_exe_core_mode0(Core *core);
int AX_exe_core_mode1(Core *core);
int AX_exe_core_mode2(Core *core);
int AX_exe_core_mode3(Core *core);

void AX_boot_rom(Core *core,uint8_t* rom,int n);

int AX_add_core(Processor *processor);

int AX_init_proc(Processor *processor);
int AX_init_proc_mem(Processor *processor);
void AX_init_mem(Processor *processor,int nwram,int nvram,int nsram,int nspmt,int nspm2);
int AX_load_prog(char *name,MMAP *mmap);
void *AX_Memory_Map(Core *core,uint64_t offset);

void AX_Pipeline_stall(Core *core);
void AX_Cache_miss(Core *core);



//-------------------------------
enum {
    //------------- ALU-A -----

    AX_EXE_ALU_NOP,
    AX_EXE_ALU_SEXT,
    AX_EXE_ALU_BOOL,
    AX_EXE_ALU_SMOVE,

    AX_EXE_ALU_SLTS,
    AX_EXE_ALU_SLTU,
    AX_EXE_ALU_SLTSI,
    AX_EXE_ALU_SLTUI,

    AX_EXE_ALU_MOVEN,
    AX_EXE_ALU_MOVEU,
    AX_EXE_UNKNOWN1,
    AX_EXE_UNKNOWN2,

    AX_EXE_ALU_MOVEINS,
    AX_EXE_ALU_MOVECYCLE,
    AX_EXE_ALU_MOVERI,
    AX_EXE_ALU_MOVEIR,

    //------------- ALU-B -----

    AX_EXE_ALU_ADD,
    AX_EXE_ALU_SUB,
    AX_EXE_ALU_XOR,
    AX_EXE_ALU_OR,

    AX_EXE_ALU_AND,
    AX_EXE_ALU_LSL,
    AX_EXE_ALU_ASR,
    AX_EXE_ALU_LSR,

    AX_EXE_ALU_DIVS,
    AX_EXE_ALU_DIVU,
    AX_EXE_ALU_REMS,
    AX_EXE_ALU_REMU,

    AX_EXE_ALU_MUL,
    AX_EXE_UNKNOWN7,
    AX_EXE_UNKNOWN8,
    AX_EXE_UNKNOWN9,


    //------------- LSU -------
    AX_EXE_LSU_LDL,
    AX_EXE_LSU_STL,
    AX_EXE_LSU_LDVL,
    AX_EXE_LSU_STVL,

    AX_EXE_LSU_LDL1,
    AX_EXE_LSU_STL1,
    AX_EXE_LSU_LDVL1,
    AX_EXE_LSU_STVL1,


    AX_EXE_LSU_LD,
    AX_EXE_LSU_ST,
    AX_EXE_LSU_LDV,
    AX_EXE_LSU_STV,

    AX_EXE_LSU_LDI,
    AX_EXE_LSU_STI,
    AX_EXE_LSU_LDVI,
    AX_EXE_LSU_STVI,

    //------------- CMP -----
    AX_EXE_CMP_CMP = 0x40,
    AX_EXE_CMP_CMPFR,
    AX_EXE_CMP_CMPN,
    AX_EXE_CMP_CMPU,

    AX_EXE_CMP_FCMP,
    AX_EXE_CMP_DCMP,
    AX_EXE_CMP_FCMPI,
    AX_EXE_CMP_DCMPI,

    AX_EXE_OTHER_ENDP,
    AX_EXE_OTHER_EXE,
    AX_EXE_OTHER_WAIT,
    AX_EXE_UNKNOWN,

    AX_EXE_BRU_RET,
    AX_EXE_BRU_RETI,
    AX_EXE_BRU_SYSCALL,
    AX_EXE_BRU_INT,

    //------------- BRU -----
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
    AX_EXE_BRU_JUMPBR,
    AX_EXE_BRU_CALL,
    AX_EXE_BRU_CALLBR,

    //------------- VFPU-A -----
    AX_EXE_VFPU_FADD,
    AX_EXE_VFPU_FSUB,
    AX_EXE_VFPU_FMUL,
    AX_EXE_VFPU_FMULADD,

    AX_EXE_VFPU_VFADD,
    AX_EXE_VFPU_VFSUB,
    AX_EXE_VFPU_VFMUL,
    AX_EXE_VFPU_VFMULADD,

    AX_EXE_VFPU_VFADDS,
    AX_EXE_VFPU_VFSUBS,
    AX_EXE_VFPU_VFMULS,
    AX_EXE_VFPU_VFMULADDS,

    AX_EXE_VFPU_FMULSUB,
    AX_EXE_VFPU_VFMULSUB,
    AX_EXE_VFPU_VFMULSUBS,
    AX_EXE_VFPU_VFSHUFFLE,

    //------------- VFPU-B -----
    AX_EXE_VFPU_FMOVE,
    AX_EXE_VFPU_VFMOVE,
    AX_EXE_VFPU_FMOVEI,
    AX_EXE_VFPU_VFMOVEI,

    AX_EXE_VFPU_FNEG,
    AX_EXE_VFPU_FABS,
    AX_EXE_VFPU_VFNEG,
    AX_EXE_VFPU_VFABS,

    AX_EXE_VFPU_VFTOH,
    AX_EXE_VFPU_VHTOF,
    AX_EXE_VFPU_VFTOI,
    AX_EXE_VFPU_VITOF,

    AX_EXE_VFPU_VFMIN,
    AX_EXE_VFPU_VFMAX,
    AX_EXE_VFPU_FMIN,
    AX_EXE_VFPU_FMAX,

    //------------- EFU -----
    AX_EXE_EFU_FDIV = 0x60,
    AX_EXE_EFU_FSQRT,
    AX_EXE_EFU_DDIV,
    AX_EXE_EFU_DSQRT,

    AX_EXE_EFU_FATAN,
    AX_EXE_EFU_FATAN2,
    AX_EXE_EFU_FEXP,
    AX_EXE_UNKNOWN0,

    AX_EXE_EFU_FSUM,
    AX_EXE_EFU_FIPR,
    AX_EXE_EFU_FSIN,
    AX_EXE_EFU_DSIN,

    AX_EXE_UNKNOWN26,
    AX_EXE_UNKNOWN27,
    AX_EXE_UNKNOWN28,
    AX_EXE_UNKNOWN29,

    //------------- FPU-DOUBLE -----
    AX_EXE_FPU_DMOVEI,
    AX_EXE_FPU_DADD,
    AX_EXE_FPU_DSUB,
    AX_EXE_FPU_DMUL,

    AX_EXE_FPU_DABS,
    AX_EXE_FPU_DNEG,
    AX_EXE_FPU_DMIN,
    AX_EXE_FPU_DMAX,

    AX_EXE_FPU_FTOD,
    AX_EXE_FPU_DTOF,
    AX_EXE_UNKNOWN32,
    AX_EXE_UNKNOWN33,

    AX_EXE_UNKNOWN34,
    AX_EXE_UNKNOWN35,
    AX_EXE_UNKNOWN36,
    AX_EXE_UNKNOWN37,
};
