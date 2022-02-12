#ifndef GPU_HPP
#define GPU_HPP 1

#include <aldebaran.h>
#include <buffer_t.hpp>

static_assert(sizeof(uint8_t*) == 8);

Aldebaran_Command command(uint64_t instruction)
{
	return Aldebaran_Command(instruction >> 60u);
}
uint32_t depend(uint64_t instruction)
{
	uint64_t d =
	    instruction &
	    0b00001111'11111111'11111111'11111111'00000000'00000000'00000000'00000000;
	d >= 32u;
	return uint32_t(d);
}
uint32_t address(uint64_t instruction)
{
	uint64_t a =
	    instruction &
	    0b00000000'00000000'00000000'00000000'11111111'11111111'11111111'11111111;
	return uint32_t(a);
}

void setCommand(uint64_t& instruction, Aldebaran_Command cmd)
{
	instruction &=
	    0b00001111'11111111'11111111'11111111'11111111'11111111'11111111'11111111;
	instruction |= uint64_t(cmd) << 60u;
}

struct GPU_t
{
	buffer_t<uint8_t> RAM;
	buffer_t<uint8_t> SPM2;
	buffer_t<uint8_t> TSRAM;

	buffer_t<uint8_t> CLUT{ 1'024 };   // 8 Kio;
	buffer_t<uint8_t> SPMI{ 16'384 };  // 128 Kio;
	buffer_t<uint8_t> SPMD{ 16'384 };  // 128 Kio;

	uint8_t* framebuffer = nullptr;
	uint8_t* zbuffer = nullptr;

	uint8_t* pc = nullptr;

	uint32_t flags = 0;

	uint64_t copySrc = 0;
	uint64_t copyDst = 0;

	GPU_t(int sRAM, int sSPM2, int sTSRAM)
	    : RAM(sRAM),
	      SPM2(sSPM2),
	      TSRAM(sTSRAM)
	{
	}

	void processCommandList(uint64_t* cmd);

	void execute();
};

#endif  // GPU_HPP
