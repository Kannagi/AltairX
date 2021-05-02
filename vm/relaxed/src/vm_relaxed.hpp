#ifndef ALTAIR_VM_RELAXED_INTERNAL_H_DEFINED
#define ALTAIR_VM_RELAXED_INTERNAL_H_DEFINED

#include <base/vm.h>

#define MIN(x, y) (x < y ? x : y)
#define MAX(x, y) (x > y ? x : y)

#define XCHG_MASK (0x01u)
#define Z_MASK (0x02u)
#define S_MASK (0x04u)
#define U_MASK (0x08u)

#define MEMORY_MAP_ROM_BEGIN (0x0000000000)
#define MEMORY_MAP_RAM_BEGIN (0x8000000000)

#endif
