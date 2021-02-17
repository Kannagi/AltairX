#include "main.h"

char *pointeur;
int global1 = 4;
extern int global2;
const int global3 =5;
int global4;
short tableau[100];

float virgule = 1.0f;
double virgule2 = 0.1f;

unsigned char DATA_ROM[] = { 0,10,15,20,25,30 };
static unsigned char DATA_ROM2[] = { 0,10,15,20,25,30 };

void LKS_printf(char *str);
void WaitVBlank(void);

typedef struct
{
	int type;
	int align;
	int varType;
	int option[4];
	int args[4];
	float unk;

	char name[50];
}LLVM;

void printfd(unsigned char *data);
LLVM test_st[2];
void LKS_test(LLVM *llvm);
void LKS_test3(LLVM *llvm);
void LKS_test(LLVM *llvm)
{
printfd(DATA_ROM2);
	return;
}

LLVM2 test5;
LLVM2 testf()
{
static int var = 1;
var++;



test5.type[0] = DATA_ROM2[1];
test5.type[0] = DATA_ROM2[1];

/*
	  if(lastGameStatus != gameStatus)
        {
            void (*init)() = funcStatusInit[gameStatus];
            if((*init))
                (*init)();

            lastGameStatus = gameStatus;
        }*/
        
    return test5;
}

int main(void)
{
	LLVM test;
	test.varType = 5;
test.unk = 0.5;

    int a = 155+15;
    global2 = 45;
    asm
    (
    "lda #5\n"
    );
    global1 = 0x88;
    
    LKS_printf("hello");
    LKS_printf("hello2");
    WaitVBlank();
    
    LKS_test3(&test);

    return 0;

}

void func()
{
	int i;
	for(i = 0; i < 5;i++)	
		WaitVBlank();
}

char func2(int a,int b)
{

	return a+b;

}


int func3(int a,int b)
{
	return a*b;

}

int func4(int a,int b)
{
	if(a == b)
	     return 1;
	     
	 return 0;
}

const int func5(int *a,int *b)
{
	*b = func2(5,6);
	*a = 56;
	return *b;
}
