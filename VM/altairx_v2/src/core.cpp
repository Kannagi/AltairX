#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>


#include <vector>

#include "altairx.hpp"


void core_init(Core &core)
{
    core.lr = 0;
    core.br = 0;
    core.lc = 0;
    core.fr = 0;

    core.pc = 4;
    core.ir = 0;
    core.cc = 0;
    core.ic = 0;

    core.syscall = 0;
}