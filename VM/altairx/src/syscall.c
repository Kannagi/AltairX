
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdint.h>
#include "vm.h"

void AX_syscall(Core *core)
{
	if(core->syscall != 1)
		return;

	core->ir = core->pc;
	core->pc = 0;

	core->syscall = 0;
}


int AX_syscall_emul(Core *core)
{
	static FILE *file;
	if(core->syscall != 1)
		return 0;

	uint64_t regA = core->ireg[4]&0xF0;
	uint64_t regB = core->ireg[4]&0x0F;

	uint64_t reg1 = core->ireg[5];
	uint64_t reg2 = core->ireg[6];
	uint64_t reg3 = core->ireg[7];

	printf("syscall : %.2lx %.2lx\n",core->ireg[4],reg1);
	//printf("syscall : %f\n",core->vreg[3*4]);

	if(regA == 0x00) //Kernel 1
	{

		switch(regB)
		{
			case 0:
				printf("Altair K1 , VM Machine \n");
			break;

			case 1:
				puts((char *)&core->dsram[reg1]);
			break;

			case 2:
				core->ireg[4] = 0; //X
				core->ireg[5] = 0; //Y
			break;

			case 3:

			break;

			case 4:

			break;

			case 5:
			break;

			case 6:

			break;

			case 7:

			break;

			case 8: //FOPEN
				file = fopen((  const char *__restrict)&core->mmap.wram[reg2],(const char *__restrict)&core->mmap.wram[reg2]);
				core->ireg[4] = (uint64_t)file;
			break;

			case 9:

			break;

			case 0xA:

			break;

			case 0xB: //FCLOSE
				fclose((FILE *)reg1);
			break;

			case 0xC: //FSEEK
				fseek ((FILE *)reg1, reg2, reg3);
			break;

			case 0xD: //FTELL
				ftell ((FILE *)reg1);
			break;

			case 0xE: // READ mouse
				core->ireg[4] = getchar();
			break;

			case 0xF: //IF

			break;
		}

	}
	else if (regA == 0x10) //Kernel 2
	{

	}
	else if (regA == 0x20) //Sound
	{

	}
	else if (regA == 0x30) //GUI
	{

	}
	else if (regA == 0x40) //Net
	{

	}

	core->syscall = 0;


	return 0;

}
