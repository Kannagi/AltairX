
typedef struct
{
	char a[7];
	int type[10];
	short b[9];
}LLVM2;

//clang main.c -S -emit-llvm -o main.o
//clang main.c -O3 -emit-llvm -o main.o