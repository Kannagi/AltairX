#include <cstdio>
#include <cstdlib>
#include <vector>
#include "altairx.hpp"


void altairx_syscall_emul(Core *core,char *wram)
{
	if(core->syscall != 1)
		return;

	uint64_t regA = core->ireg[1]&0xF0;
	uint64_t regB = core->ireg[1]&0x0F;
	uint64_t reg1 = core->ireg[2];
	uint64_t reg2 = core->ireg[3];
	uint64_t reg3 = core->ireg[4];

	char *adr;
	char *adr2;

	//printf("syscall : %.2lx %.2lx\n",core->ireg[4],reg1);
	//printf("syscall : %f\n",core->vreg[3*4]);

	if(regA == 0x00) //Kernel 1
	{

		switch(regB)
		{
			case 0:
				printf("AltairX K1 , VM Machine \n");
			break;

			case 1:
			    adr = (char *)&wram[reg1];
				printf(adr);
			break;

			case 2:
				core->ireg[1] = 0; //X
				core->ireg[2] = 0; //Y
			break;

			case 3:
				exit(reg1);
			break;

			case 4:

			break;

			case 5:
			break;

			case 6:

			break;

			case 7:

			break;

			case 8:
			break;

			case 9:

			break;

			case 0xA:

			break;

			case 0xB:
			break;

			case 0xC:
			break;

			case 0xD:
			break;

			case 0xE: // getchar
				core->ireg[1] = getchar();
			break;

			case 0xF: //GIF

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

}
