
#include <string.h>
#include <stdint.h>

#include <iostream>
#include <vector>

#include "altairx.hpp"

int main(int argc, char** argv)
{
    char address[256];
    address[0] = 0;
    int arg = 0;

    // 16 MiB WRAM , 256 KiB SPMT , 512 kiB SPM2
    AltairX altairx(16,256,512);

    for(int i = 1; i < argc;i++)
    {
        if(argv[i][0] == '-')
        {
        	if(strcmp(argv[i],"-ncore")  == 0) arg = 1;

        	if(strcmp(argv[i],"-wram")   == 0) arg = 2;
        	if(strcmp(argv[i],"-spmt")   == 0) arg = 3;
        	if(strcmp(argv[i],"-spm2")   == 0) arg = 4;

        	if(strcmp(argv[i],"-mode")   == 0) arg = 7;
        }else
        {
        	if(arg == 0)
			{
				strcpy(address,argv[i]);
			}else
			{
				switch(arg)
				{
					case 1:
						altairx.ncore = atoi(argv[i]);
					break;

					case 2:
						altairx.memorymap.wram.resize(atoi(argv[i]));
					break;

					case 3:
						altairx.memorymap.spmt.resize(atoi(argv[i]));
					break;

					case 4:
						altairx.memorymap.spm2.resize(atoi(argv[i]));
					break;

					case 7:
						altairx.mode = atoi(argv[i]);
					break;

				}
				arg = 0;
			}

        }
    }

    if(address[0] == 0)
    {
        std::cout<<"Enter a program\n";
        std::cout<<"option numbre core  : -ncore \n";
        std::cout<<"option size memory : -wram (1MiB) -spmt (1KiB) -spm2 (1KiB)\n";
        std::cout<<"option mode  : -mode\n";
        std::cout<<"mode 0 console,syscall emulate ,1 core only\n";
        std::cout<<"mode 1 same mode 0 and debug\n";
        std::cout<<"mode 2 same mode 0 and cycle accurate\n";
        std::cout<<"mode 3 complete hardware\n";
        std::cout<<"mode 4 XSTAR OS\n";

        std::cout<<"\nExemple :\nvm_altairx prog.bin -wram 32\n";
        return 0;
    }

	altairx.wram = altairx.memorymap.wram.data();
	altairx.spm2 = altairx.memorymap.spm2.data();
	altairx.spmt = altairx.memorymap.spmt.data();
	altairx.io = altairx.memorymap.io.data();

	altairx.init_mask();
	altairx.load_prog(address);

	Core core;
	core_init(core);

	//altairx.array_core.push_back(core);

	altairx.core = &core;

	switch(altairx.mode)
	{
		case 0:
			altairx.mode0();
		break;

		case 1:
			std::cout<<"mode1\n";
			altairx.mode1();
		break;

		case 2:
			altairx.mode2();
		break;

		case 3:
			altairx.mode3();
		break;

		case 4:
			altairx.mode4();
		break;
	}


	if(altairx.error != 1)
		std::cout << "Error core : " << altairx.error << std::endl;

    return 0;
}