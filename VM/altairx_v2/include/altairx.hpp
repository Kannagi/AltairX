

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

#define AX_SPM_SIZE 0x4000

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

    union
	{
        uint16_t hreg[AX_core_VREG_COUNT * 4];
		float freg[AX_core_VREG_COUNT * 2];
		double dreg[AX_core_VREG_COUNT];

		int64_t direg[AX_core_VREG_COUNT];
		int32_t fireg[AX_core_VREG_COUNT * 2];
	};

    uint8_t spm[AX_SPM_SIZE];
    
}Core;

class AltairX
{
private:
    std::vector<Core> array_core;
    Core *core;

    uint32_t opcode1,opcode2;
	uint32_t io;
    
    void execute_unit(uint32_t rega,uint32_t regb,uint32_t regc,uint32_t unit,uint32_t size,uint32_t opcode,uint32_t tunit,uint32_t imm24);
    void execute_unit0(uint32_t rega,uint32_t regb,uint32_t regc,uint32_t unit,uint32_t size,uint32_t opcode,uint32_t imm24);
    void execute_unit1(uint32_t rega,uint32_t regb,uint32_t regc,uint32_t unit,uint32_t size,uint32_t opcode);

    

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

    uint64_t rom_mask,spmt_mask,spm2,mask,wram_mask;
};