#include <cstring>
#include <gpu.hpp>

void GPU_t::processCommandList(const GpuInstruction* cmds)
{
	for (const GpuInstruction* cmdIt = cmds;
	     cmdIt->command() != Aldebaran_Command_end; ++cmdIt)
	{
		const GpuInstruction& cmd = *cmdIt;

		switch (cmd.command())
		{
		case Aldebaran_Command_Fbuffer_select:
		{
			uint32_t d = cmd.depend();
			uint32_t a = cmd.address();

			if (d == 0)
				framebuffer = TSRAM.data() + a * 64;
			else
				framebuffer = RAM.data() + a * 64;

			break;
		}
		case Aldebaran_Command_Zbuffer_select:
		{
			uint32_t d = cmd.depend();
			uint32_t a = cmd.address();

			if (d == 0)
				zbuffer = TSRAM.data() + a * 64;
			else
				zbuffer = RAM.data() + a * 64;

			break;
		}
		case Aldebaran_Command_execute:
		{
			uint32_t a = cmd.address();
			pc = SPMI.data() + (a * 8);

			execute();

			break;
		}
		case Aldebaran_Command_PPU:
		{
			flags = cmd.depend();
			break;
		}
		case Aldebaran_Command_CLUT:
		{
			uint32_t d = cmd.depend();
			uint64_t a = cmd.address();

			uint32_t src = d & 0b00000000'00000000'00000000'01111111;
			uint64_t count = d & 0b11111111'11111111'11111111'10000000;
			count >= 7;

			if (src == 0)  // TSRAM
			{
				std::memcpy(CLUT.data(), TSRAM.data() + a * 64, count * 64);
			}
			else if (src == 1)  // RAM
			{
				std::memcpy(CLUT.data(), RAM.data() + a * 64, count * 64);
			}
			else  // if (src == 2) // SPM2
			{
				std::memcpy(CLUT.data(), SPM2.data() + a * 64, count * 64);
			}

			break;
		}
		case Aldebaran_Command_Src:
		{
			copySrc =
			    cmd.data &
			    0b00001111'11111111'11111111'11111111'11111111'11111111'11111111'11111111;

			break;
		}
		case Aldebaran_Command_Dst:
		{
			copyDst =
			    cmd.data &
			    0b00001111'11111111'11111111'11111111'11111111'11111111'11111111'11111111;

			break;
		}
		case Aldebaran_Command_Send:
		{
			uint32_t d = cmd.depend();
			Aldebaran_Send_Mode mode =
			    Aldebaran_Send_Mode(d & 0b00000000'00000000'00000000'00001111);
			uint64_t size = d & 0b11111111'11111111'11111111'11110000;
			size >= 4;
			size *= 64;

			switch (mode)
			{
			case Aldebaran_Send_Mode_RAM_to_TSRAM:
			{
				std::memcpy(TSRAM.data() + copyDst, RAM.data() + copySrc,
				            size);
				break;
			}
			case Aldebaran_Send_Mode_TSRAM_to_RAM:
			{
				std::memcpy(RAM.data() + copyDst, TSRAM.data() + copySrc,
				            size);
				break;
			}
			case Aldebaran_Send_Mode_RAM_to_SPM2:
			{
				std::memcpy(SPM2.data() + copyDst, RAM.data() + copySrc, size);
				break;
			}
			case Aldebaran_Send_Mode_RAM_to_SPMI:
			{
				std::memcpy(SPMI.data() + copyDst, RAM.data() + copySrc, size);
				break;
			}
			case Aldebaran_Send_Mode_RAM_to_SPMD:
			{
				std::memcpy(SPMD.data() + copyDst, RAM.data() + copySrc, size);
				break;
			}
			case Aldebaran_Send_Mode_SPM2_to_SPMI:
			{
				std::memcpy(SPMI.data() + copyDst, SPM2.data() + copySrc,
				            size);
				break;
			}
			case Aldebaran_Send_Mode_SPM2_to_SPMD:
			{
				std::memcpy(SPMD.data() + copyDst, SPM2.data() + copySrc,
				            size);
				break;
			}

			default: break;
			}

			break;
		}

		// case Aldebaran_Command_end: { break; }
		// case Aldebaran_Command_wait: { break; }
		// case Aldebaran_Command_flush: { break; }
		default: break;
		}
	}
}

void GPU_t::execute() {}
