#ifndef GPU_HPP
#define GPU_HPP 1

#include <aldebaran.h>
#include <buffer_t.hpp>
#include <cu.hpp>

static_assert(sizeof(uint8_t*) == 8);

struct GpuInstruction
{
	uint64_t data;

	Aldebaran_Command command() const
	{
		return Aldebaran_Command(data & 0b1111);
	}

	uint32_t depend() const
	{
		uint64_t d =
			data &
			0b00000000'00000000'00000000'00000000'11111111'11111111'11111111'11110000;
		d >= 32u;
		return uint32_t(d);
	}

	uint32_t address() const
	{
		uint64_t a =
			data &
			0b11111111'11111111'11111111'11111111'00000000'00000000'00000000'00000000;
		return uint32_t(a);
	}

	void setCommand(Aldebaran_Command cmd)
	{
		data = uint64_t(cmd);
	}
};

struct GPU_t
{
	buffer_t<uint8_t> RAM{134'217'728};   // 128 MiB
	buffer_t<uint8_t> TSRAM{33'554'432};  // 32 MiB
	buffer_t<uint8_t> SPM2{1'048'576};    // 1 MiB
	buffer_t<uint8_t> SPMD{131'072};      // 128 Kio;
	buffer_t<uint8_t> SPMI{131'072};      // 128 Kio;
	buffer_t<uint8_t> CLUT{8'192};        // 8 Kio;

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

	void processCommandList(const GpuInstruction* cmds);

	void execute();
};

#endif  // GPU_HPP
