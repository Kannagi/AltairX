#ifndef UTILITIES_HPP_INCLUDED
#define UTILITIES_HPP_INCLUDED

#include <stdint.h>

#include <array>
#include <cstring>

inline const std::array<uint64_t, 4> sizemask =
{
0x00000000000000FFull,
0x000000000000FFFFull,
0x00000000FFFFFFFFull,
0xFFFFFFFFFFFFFFFFull,
};

inline const std::array<uint64_t, 4> signmask =
{
0xFFFFFFFFFFFFFF00ull,
0xFFFFFFFFFFFF0000ull,
0xFFFFFFFF00000000ull,
0x0000000000000000ull,
};

inline uint16_t float_to_half(float fval)
{
    uint32_t val;
    std::memcpy(&val, &fval, sizeof(val));

    // float to half
    const uint32_t mant = (val >> 13) & 0x03FF; // mantisse
    const uint32_t sign = (val >> 16) & 0x8000; // sign

    const uint32_t texp = (val >> 23) & 0x00FF; // exp
    uint32_t exp = (texp & 0x0F) << 10;
    if(texp & 0x80)
    {
        exp |= 0x4000;
    }

    return sign + mant + exp;
}

inline float half_to_float(uint32_t half)
{
    uint32_t exp = (half & 0x3C00) >> 3;

    if(half & 0x4000)
    {
        exp |= 0x4000;
    }
    else
    {
        if(exp != 0)
        {
            exp |= 0x3800;
        }
    }

    exp <<= 16;

    uint32_t xval = (half & 0x8000) << 16; // sign
    xval += (half & 0x03FF) << 13;         // mantisse
    xval += exp;

    float fval;
    std::memcpy(&fval, &xval, sizeof(xval));

    return fval;
}

#endif
