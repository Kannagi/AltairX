

int main() {
	char cde[31];
	Formation f;
	initform(&f);
	Epreuve e;

	f.nbUE = -1;
	do {
		scanf("%s", &cde);
		if (strcmp(cde, "formation") == 0) // C2
		{
			formation(&f);
		} // TODO
		else if (strcmp(cde, "epreuve") == 0) // C3
		{
			if (!formation) {
				printf("Le nombre d'UE n'est pas defini\n");
				continue;
			}
			epreuve(&f);
		} // TODO
		else if (strcmp(cde, "coefficients") == 0) // C4
		{
			coefficients(&f);
		} // TODO
		else if (strcmp(cde, "note") == 0) // C5
		{
			note(&f);
		} // TODO
		else if (strcmp(cde, "notes") == 0) // C6
		{
			Verifnote(&f);
		} // TODO
		else if (strcmp(cde, "releve") == 0) // C7
		{
		} // TODO
		else if (strcmp(cde, "decision") == 0) // C8
		{
		} // TODO
	} while (strcmp(cde, "exit") != 0); // C1
}

//-----------------------










#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdint.h>

#include "src/vm.h"



int main2(int argc, char** argv)
{
	test_perf2();
	return 0;

	int i,arg = 0;

    char address[500];
    address[0] = 0;

    Processor processor;

    AX_init_proc(&processor);

	// 16 MiB WRAM , 1 MiB VRAM , 1 MiB SPMRAM , 8 KiB SPMT , 512 kiB SPM2
	AX_init_mem(&processor,16,1,1,8,512);

    for(i = 1; i < argc;i++)
    {
        if(argv[i][0] == '-')
        {
        	if(strcmp(argv[i],"-ncore")  == 0) arg = 1;

        	if(strcmp(argv[i],"-wram")   == 0) arg = 2;
        	if(strcmp(argv[i],"-vram")   == 0) arg = 3;
        	if(strcmp(argv[i],"-spmram") == 0) arg = 4;
        	if(strcmp(argv[i],"-spmt")   == 0) arg = 5;
        	if(strcmp(argv[i],"-spm2")   == 0) arg = 6;

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
						processor.ncore = atoi(argv[i]);
					break;

					case 2:
						processor.mmap.nwram = atoi(argv[i]);
					break;

					case 3:
						processor.mmap.nvram = atoi(argv[i]);
					break;

					case 4:
						processor.mmap.nspmram = atoi(argv[i]);
					break;

					case 5:
						processor.mmap.nspmt = atoi(argv[i]);
					break;

					case 6:
						processor.mmap.nspm2 = atoi(argv[i]);
					break;

					case 7:
						processor.mode = atoi(argv[i]);
					break;

				}
				arg = 0;
			}

        }
    }
/*
    if(address[0] == 0)
    {
        printf("Enter a program\n");
        printf("option numbre core  : -ncore \n");
        printf("option size memory : -wram -vram -spmram -spmt -spm2\n");
        printf("option mode  : -mode\n");
        printf("mode 0 console,syscall emulate ,1 core only\n");
        printf("mode 1 same mode 0 and debug\n");
        printf("mode 2 same mode 0 and cycle accurate\n");
        printf("mode 3 complete hardware\n");
        printf("mode 4 XSTAR OS\n");

        printf("\nExemple :\nvm_altairx prog.bin -wram 32\n");
        return 0;
    }
*/


	AX_init_proc_mem(&processor);

	AX_load_prog("prog.bin",&processor.mmap);

	AX_add_core(&processor);
	AX_boot_rom(processor.core[0],processor.mmap.rom,processor.mmap.nrom);

	int error;

	switch(processor.mode)
	{
		case 0:
			//error = AX_exe_core(processor.core[0]);
			//error = AX_exe_core_mode0(processor.core[0]);
			error = AX_exe_core_mode1(processor.core[0]);
		break;

		case 1:
			error = AX_exe_core_mode1(processor.core[0]);
		break;

		case 2:
			error = AX_exe_core_mode2(processor.core[0]);
		break;

		case 3:
			error = AX_exe_core_mode3(processor.core[0]);
		break;

		case 4:
			//
		break;
	}



	if(error != 1)
		printf("%d\n",error);


	return 0;
}
