
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "vm.h"


void __attribute__((optimize("-O3"), noinline)) AX_IO_Read(Core *core,uint64_t offset,void *data)
{

	uint32_t mmap_io = (offset&0xF0000)>>16;
	uint32_t io = (offset&0x00FFFF);
	uint32_t ioadr = (offset&0xFFFFF);

	uint32_t  *iorw32 = &core->mmap.io[ioadr];
	uint64_t  *iorw64 = &core->mmap.io[ioadr];

	printf("%x\n",mmap_io);


	switch(mmap_io)
	{
		//-------------CORE CONTROL-----
		case 0:

			switch(io)
			{
				//0xBMVVVVCC B = Bit version (32 or 64) B ,M Model , VVVV Version , CC number core
				/*
				0 : unknow version
				1 : VM version
				2 : AX

				*/
				case 0x0000: //Info CPU (1 = VM V 1)
					iorw64[0] = 0x80000100;
				break;

				case 0x0200: //Control

				break;


			}
		break;

		//-------------GPU-----
		case 1:

			switch(io)
			{
				case 0x0000: //info GPU

				break;

			}

		break;

		//-------------SOUND-----
		case 2:

			switch(io)
			{
				case 0x0000: //info SOUND

				break;

			}
		break;

		//-------------USB/SD-----
		case 3:

		break;

		//-------------SATA / NVME-----
		case 4:

		break;

		//-------------ETHERNET/WIFI-----
		case 5:

		break;

		//-------------EXTENSION-----
		case 6:

		break;

		//-------------OTHER/PS-2/DMA-----
		case 7:

			switch(io)
			{
				case 0x0000: //info DMA / status

				break;

				case 0xFF00: //mouse position

				break;
			}

		break;
	}
	//printf("%x (%x/%x)", core->io,core->iorw);
	//void *addrio = core->mmap.io;
}


void __attribute__((optimize("-O3"), noinline)) AX_IO_Write(Core *core,uint64_t offset,void *data)
{

	uint32_t mmap_io = (offset&0xF0000)>>16;
	uint32_t io = (offset&0x00FFFF);
	uint32_t ioadr = (offset&0xFFFFF);

	uint32_t  *iorw32 = &core->mmap.io[ioadr];
	uint64_t  *iorw64 = &core->mmap.io[ioadr];

	printf("%x\n",mmap_io);


	switch(mmap_io)
	{
		//-------------CORE CONTROL-----
		case 0:

			switch(io)
			{
				case 0x0040: //Source DMA

				break;

				case 0x0048: //Destination DMA

				break;

				case 0x0050: //CMD/Core/size

				break;

				case 0x0100: //Write Control

				break;
			}

		break;

		//-------------GPU-----
		case 1:

			switch(io)
			{
				case 0x0100: //SRC Command List

				break;

				case 0x0108: //start

				break;
			}

		break;

		//-------------SOUND-----
		case 2:

			switch(io)
			{
				case 0x0100: //Channel 1 enable

				break;

				case 0x0108: //Channel 2 enable

				break;

				case 0x0110: //Channel 3 enable

				break;

				case 0x0118: //Channel 4 enable

				break;


				//-----Channel 1
				case 0x1000: //Buffer 1 Left

				break;

				case 0x1008: //Buffer 1 Right

				break;

				case 0x1010: //Size

				break;

				case 0x1018: //reloop/FRQ/BCM/ADPCM

				break;

				//-----Channel 2
				case 0x1040: //Buffer 1 Left

				break;

				case 0x1048: //Buffer 1 Right

				break;

				case 0x1050: //Size

				break;

				case 0x1058: //reloop/FRQ/BCM/ADPCM

				break;

				//-----Channel 3
				case 0x1080: //Buffer 1 Left

				break;

				case 0x1088: //Buffer 1 Right

				break;

				case 0x1090: //Size

				break;

				case 0x1098: //reloop/FRQ/BCM/ADPCM

				break;

				//-----Channel 4
				case 0x10C0: //Buffer 1 Left

				break;

				case 0x10C8: //Buffer 1 Right

				break;

				case 0x10D0: //Size

				break;

				case 0x10D8: //reloop/FRQ/BCM/ADPCM

				break;
			}

		break;

		//-------------USB/SD-----
		case 3:

		break;

		//-------------SATA / NVME-----
		case 4:

		break;

		//-------------ETHERNET/WIFI-----
		case 5:

		break;

		//-------------EXTENSION-----
		case 6:

		break;

		//-------------OTHER/PS-2/DMA-----
		case 7:

			switch(io)
			{
				case 0x0100: //SRC DMA

				break;

				case 0x0108: //DSR DMA

				break;

				case 0x0110: //SIZE DMA

				break;

				case 0x0118: //Start

				break;

				//----

				case 0xFF10: //DD offset

				break;

				case 0xFF18: //WRAM offset

				break;

				case 0xFF20: //RW/Size/start

				break;


			}

		break;
	}
	//printf("%x (%x/%x)", core->io,core->iorw);
	//void *addrio = core->mmap.io;
}
