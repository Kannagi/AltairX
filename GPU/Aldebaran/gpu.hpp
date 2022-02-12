#ifndef GPU_HPP
#define GPU_HPP 1

#include <aldebaran.h>
#include <buffer_t.hpp>
#include <cu.hpp>

static_assert(sizeof(uint8_t*) == 8);

struct GPU_t
{
	buffer_t<uint8_t> RAM;

	// L2
	buffer_t<uint8_t> TRAM;  // 1'048'576 bytes expected (1 MiB)
	buffer_t<uint8_t> SPM2;  // 2'097'152 bytes expected (2 MiB)
	buffer_t<uint8_t> BRAM;  // 1'048'576 bytes expected (1 MiB)

	// L1
	buffer_t<uint8_t> SPMD{131'072};  // 128 Kio;
	buffer_t<uint8_t> SPMI{131'072};  // 128 Kio;
	buffer_t<uint8_t> DATA{32'768};   // 32 Kio;

	uint8_t* framebuffer = nullptr;
	uint8_t* zbuffer     = nullptr;

	uint8_t* pc = nullptr;

	uint32_t flags = 0;

	uint64_t copySrc = 0;
	uint64_t copyDst = 0;

	ComputeUnit_t units[4]{
	    {*this},
	    {*this},
	    {*this},
	    {*this},
	};

	GPU_t(uint64_t sRAM,
	      uint64_t sSPM2,
	      uint64_t sTextureRAM,
	      uint64_t sBufferRAM)
	    : RAM{sRAM}, TRAM{sTextureRAM}, SPM2{sSPM2}, BRAM{sBufferRAM}
	{
	}

	void processCommandList(const GpuInstruction* cmds);

	void execute();
};

#endif  // GPU_HPP
