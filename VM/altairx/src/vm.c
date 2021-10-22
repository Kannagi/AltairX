
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdint.h>
#include "vm.h"

int AX_load_prog(char *name,MMAP *mmap)
{
	FILE *file = fopen(name,"rb");
	if(file == NULL)
		return 1;


	fseek(file, 0, SEEK_END);
	int psize = ftell(file);

	if(psize&0x1FFFF)
	{
		psize += 0x20000-(psize&0x1FFFF);
	}

	fseek(file, 0, SEEK_SET);
	uint8_t *rom = malloc(psize);

	fread(rom,1,psize,file);
	fclose(file);

	mmap->rom = rom;
	mmap->nrom = psize;

	return 0;
}

void AX_init_mem(Processor *processor,int nwram,int nvram,int nsram,int nspm3,int nspmt,int nspm2)
{
	nwram = 0x100000*nwram;  // nwram * MiB
	nvram = 0x100000*nvram;  // nvram * MiB
	nsram = 0x100000*nsram; // nsram * MiB

	nspm3 = 0x100000*nspm3; // nspm3 * MiB

	nspmt = 0x400*nspmt; // nspmt * KiB
	nspm2 = 0x400*nspm2; // nspm2 * KiB

	processor->mmap.nwram = nwram;
	processor->mmap.nvram = nvram;
	processor->mmap.nspmram = nsram;

	processor->mmap.nspmt = nspmt;
	processor->mmap.nspm2 = nspm2;
	processor->mmap.nspm3 = nspm3;
}


int AX_init_proc(Processor *processor)
{
	processor->icore = 0;
	processor->ncore = 1;
	processor->mode = 0;

	return 0;
}

int AX_init_proc_mem(Processor *processor)
{
	processor->mmap.wram = malloc(processor->mmap.nwram);
	processor->mmap.vram = malloc(processor->mmap.nvram);
	processor->mmap.spmram = malloc(processor->mmap.nspmram);

	processor->mmap.spm3 = malloc(processor->mmap.nspm3);

	processor->mmap.spmt = malloc(processor->mmap.nspmt);
	processor->mmap.spm2 = malloc(processor->mmap.nspm2);

	processor->mmap.io = malloc(0x10000); // 1 Mio
	return 0;
}


int AX_add_core(Processor *processor)
{
	if(processor->icore > AX_core_MAX)
		return 1;

	Core *core = malloc(sizeof(Core));

	core->mmap = processor->mmap;
	core->syscall = 0;
	core->instruction = 0;
	core->cycle = 0;
	core->delay = 0;
	core->pc = 0;

	processor->core[processor->icore] = core;

	processor->icore++;

	return 0;
}

int AX_exe_core(Core *core)
{
	clock_t tbegin;

	tbegin = clock();

	int error = 0,t = 0;

	while(error == 0)
	{
		error = AX_decode(core);
		//AX_debug(core);
		error += AX_execute(core);

		AX_syscall_emul(core);
		core->cycle++;

		t++;
		if(t > 0x10000)
		{
			if(clock() > (tbegin+CLOCKS_PER_SEC) )
			{
				//printf("Cycle/Second : %ld\n",core->cycle);
				float frq = (float)core->cycle / 1000000.0f;
				printf("Frequence : %.2f MHz\n",frq);

				core->cycle = 0;
				tbegin = clock();
			}
			t = 0;
		}

		//printf("%d\n",core->pc);

	}



	return error;
}

void AX_boot_rom(Core *core,uint8_t* rom,int n)
{
	if(n > 0x20000)
		n = 0x20000;
	memcpy(core->isram,rom,n);
}
