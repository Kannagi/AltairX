#include <aldebaran.h>

int main()
{
	GPU gpu;
	Aldebaran_VM_Init(&gpu);

	static const uint32_t program[]{
	    //  rA     rB     rC     --   unop unit
	    0b000000'000000'000000'000000'0000'0001,
	    0,

	    0b000000000000000000000000'0101'0100,
	    0,
	};

	Aldebaran_VM_WriteToRAM(gpu, 0, reinterpret_cast<const uint8_t*>(program),
	                        sizeof(program));

	static const uint64_t cmdList[]{
	    uint64_t(Aldebaran_Command_Src),
	    uint64_t(Aldebaran_Command_Dst),
	    uint64_t(Aldebaran_Command_Send) |
	        0b00000000'00000000'00000000'00000000'00110000'00000000'00000000'00010000,
	    uint64_t(Aldebaran_Command_execute),
	    Aldebaran_Command_end,
	};

	Aldebaran_VM_CMDList(gpu, cmdList);

	Aldebaran_VM_End(gpu);
}
