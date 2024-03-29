#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <vector>

#include "altairx.hpp"

AltairX::AltairX(int nwram,int nspmt,int nspm2)
{
    this->icore = 0;
    this->ncore = 1;
    this->mode = 0;

    this->memorymap.rom.resize(0x100000*0x10,0);
	this->memorymap.wram.resize(0x100000*nwram,0);
	this->memorymap.spmt.resize(0x400*nspmt,0);
	this->memorymap.spm2.resize(0x400*nspm2,0);
    this->memorymap.io.resize(0x80000,0);
}


void AltairX::init_mask()
{
    this->rom_mask  = 0xFFFFFF;
    this->spmt_mask = this->memorymap.spmt.size()-1;
    this->spm2_mask = this->memorymap.spm2.size()-1;
    this->wram_mask = this->memorymap.wram.size()-1;
}

void AltairX::load_kernel(char *path)
{
    std::ifstream file(path, std::ios::binary);

    if(!file.is_open())
    {
        std::cerr << "Error : Impossible open kernel" << std::endl;
        return;
    }

    file.seekg(0, std::ios::end);
    std::streampos filesize = file.tellg();
    file.seekg(0, std::ios::beg);

    file.read((char *)this->memorymap.rom.data(), filesize);

    file.close();
}

void AltairX::load_prog(char *path)
{
    std::ifstream file(path, std::ios::binary);

    if(!file.is_open())
    {
        std::cerr << "Error : Impossible open binary" << std::endl;
        return;
    }

    file.seekg(0, std::ios::end);
    std::streampos filesize = file.tellg();
    file.seekg(0, std::ios::beg);

    file.read((char *)this->memorymap.wram.data(), filesize);

    file.close();
}

void AltairX::mode0()
{
    clock_t tbegin;

	tbegin = clock();

	int t = 0;

	core->error = 0;

	while(core->error == 0)
	{
		execute();

		//if(core->cycle > 20) exit(0);

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

    this->error = core->error;
}

void AltairX::mode1()
{
    clock_t tbegin;



	tbegin = clock();

	int t = 0;

	core->error = 0;

	while(core->error == 0)
	{
        
		execute();

        altairx_debug(this->opcode1,0);

        altairx_syscall_emul(this->core,(char *)this->wram);

		if(core->cc > 20) exit(0);

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

    this->error = core->error;
}

void AltairX::mode2()
{
    
}

void AltairX::mode3()
{
    
}

void AltairX::mode4()
{
    
}