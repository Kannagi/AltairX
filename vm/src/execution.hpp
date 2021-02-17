#ifndef EXECUTION_HPP_INCLUDED
#define EXECUTION_HPP_INCLUDED

#include "vm.hpp"
#include "io.hpp"

inline void execute(ar::processor& processor)
{
    const auto memory_info{processor.get_memory_info()};

    while(true)
    {
        processor.decode();

        if(!processor.execute())
        {
            break;
        }

        processor.direct_memory_access();

        run_io(memory_info.iosram);
    }
}

#endif
