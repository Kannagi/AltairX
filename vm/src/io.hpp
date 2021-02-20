#ifndef IO_HPP_INCLUDED
#define IO_HPP_INCLUDED

#include "vm.hpp"

#include <iostream>

enum class io_operation : std::uint8_t
{
    none = 0,
    fputc = 1
};

inline void run_io(std::uint8_t* iosram)
{
    if(iosram[0] != 0)
    {
        switch(static_cast<io_operation>(iosram[1]))
        {
            case io_operation::fputc:
                std::cout.put(iosram[32]);
                break;

            default:
                break;
        }

        iosram[0] = 0;
    }
}

#endif
