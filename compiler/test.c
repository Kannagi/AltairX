#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static const char global_str[] = "Hello world!";

extern int global_int;
int global_int = 0;

const char* get_message();
const char* get_message()
{
    return global_str;
}

void set_global(int value);
void set_global(int value)
{
    global_int = value;
}

static const uint8_t base64_table[128] = 
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x3f,
    0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b,
    0x3c, 0x3d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
    0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
    0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
    0x17, 0x18, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20,
    0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
    0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30,
    0x31, 0x32, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00
};

static uint32_t from_base64(char value)
{
    return base64_table[(uint8_t)(value) & 0x7Fu];
}

void parse_base64(const char* data, size_t size, uint8_t* output);
void parse_base64(const char* data, size_t size, uint8_t* output)
{
    for(size_t i = 0; i < size; i += 4)
    {
        uint32_t buffer = 0;
        buffer |= from_base64(data[i]) << 18;
        buffer |= from_base64(data[i + 1]) << 12;
        buffer |= from_base64(data[i + 2]) << 6;
        buffer |= from_base64(data[i + 3]);

        *output++ = (uint8_t)(buffer >> 16);
        *output++ = (uint8_t)(buffer >> 8);
        *output++ = (uint8_t)(buffer);
    }
}

uint64_t testmove(uint64_t i);
uint64_t testmove(uint64_t i)
{
    return i + 184467447091478ull;
}

int32_t cmove(int32_t left, int32_t right)
{
    return left < right ? left : right;
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

int sext(short s);
int sext(short s)
{
    return (int)(s);
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
        int temp = external_func(*array);
        *array = *array * temp;
        *array = external_func(*array * 2);

        array++;
    }

    return array;
}

int use_arg_at_other_pos(int first, int second)
{
    return external_func(second);
}

uint32_t lots_of_args(uint32_t* restrict array, size_t size, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f, uint32_t g, uint32_t h);
uint32_t lots_of_args(uint32_t* restrict array, size_t size, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f, uint32_t g, uint32_t h)
{
    uint32_t output = 0;

    for(size_t i = 0; i < size; ++i)
    {
        output += array[i] + a + b;
        output -= array[i] - c + d;
        output -= external_func(output);
        output -= array[i] + e - f; 
        output += array[i] - g + h;
    }

    return output;
}

void loading(int* ptr, size_t size);
void loading(int* ptr, size_t size)
{
    for(size_t i = 0; i < size; ++i)
    {
        ptr[i] += 5;
    }
}

int return_non_first_arg(int first, int second);
int return_non_first_arg(int first, int second)
{
    return second;
}

int callee(int i);

int caller(int i);
int caller(int i)
{
    return callee(i) + 5;
}

int main()
{
    return bar(0, 5, 3, 1);
}