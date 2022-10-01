#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


uint64_t testmove(uint64_t i);
uint64_t testmove(uint64_t i)
{
    return i + 184467447091478ull;
}

int32_t bar(int32_t a, int32_t b, int32_t c, int32_t d);
int32_t bar(int32_t a, int32_t b, int32_t c, int32_t d)
{
    while(a < d)
    {
        b = b + a;
        a = b * 4;
    }
    
    return a + b;
}

void foo(int* array, size_t count);
void foo(int* array, size_t count)
{
    array[0] = 1;

    for(size_t i = 1; i < count - 1; ++i) 
    {
        array[i] *= 2;
    }

    array[count - 1] = 2;
    
    for(size_t i = 0; i < count; ++i) 
    {
        array[i] += 1;
    }
}

struct A {
  int8_t a;
  int32_t b[10][20];
  int8_t c;
};
struct B {
  int32_t x;
  int64_t y;
  struct A z[];
};

int* testptr(struct B* s, size_t index);
int* testptr(struct B* s, size_t index) {
  return &s[0].z[index].b[5][13];
}

int negate(int i);
int negate(int i)
{
    return -i;
}

void bubble_sort(int* array, size_t count);
void bubble_sort(int* array, size_t count)
{
    for(size_t i = 0; i < count - 1; ++i) 
    {
        for(size_t j = 0; j < count - i - 1; ++j) 
        {
            if(array[j] > array[j + 1]) 
            {
                int temp     = array[j];
                array[j]     = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}



/*
static double a0 = +1.0;
static double a1 = -1.666666666666580809419428987894207e-1;
static double a2 = +8.333333333262716094425037738346873e-3;
static double a3 = -1.984126982005911439283646346964929e-4;
static double a4 = +2.755731607338689220657382272783309e-6;
static double a5 = -2.505185130214293595900283001271652e-8;
static double a6 = +1.604729591825977403374012010065495e-10;
static double a7 = -7.364589573262279913270651228486670e-13;

double sin1(double x)
{
    return a0 * x
         + a1 * x * x * x
         + a2 * x * x * x * x * x
         + a3 * x * x * x * x * x * x * x
         + a4 * x * x * x * x * x * x * x * x * x
         + a5 * x * x * x * x * x * x * x * x * x * x * x
         + a6 * x * x * x * x * x * x * x * x * x * x * x * x * x
         + a7 * x * x * x * x * x * x * x * x * x * x * x * x * x * x * x;
}

int functest(int *t1,int t2,int *t3,int x)
{
    int mavar = t1[x+0] +5;
    mavar += t1[x+1] +2;
    mavar += t1[x+2] +8;
    
    return mavar;
}*/


typedef struct data
{
    uint8_t a; 
    uint32_t b; 
    uint32_t c; 
    uint64_t d;
} data; 

data make_data(uint8_t a, uint32_t b, uint32_t c, uint64_t d);
data make_data(uint8_t a, uint32_t b, uint32_t c, uint64_t d)
{
    data output = {a, b, c, d};

    if(a > b)
    {
        output.a = b;
    }
    else if(c > d)
    {
        output.c = d + a;
    }
    
    return output;
}

uint64_t use_data(data d);
uint64_t use_data(data d)
{
    return d.a * d.d + d.c;
}

int external_func(int i);

int* external_func_user(int* array, int* end);
int* external_func_user(int* array, int* end)
{
    while(array != end)
    {
        int temp = external_func(array[0]);
        array[0] = array[0] * temp;
        array[0] = external_func(array[0] * 2);

        array++;
    }

    return array;
}

int main()
{
    return bar(0, 5, 3, 1);
}