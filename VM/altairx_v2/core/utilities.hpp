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

inline uint64_t sext_bitsize(uint64_t val, uint64_t bitsize) noexcept
{
    const auto mask = 1ull << (bitsize - 1ull);
    return (val ^ mask) - mask;
}

inline uint64_t sext_bytesize(uint64_t val, uint64_t bytesize) noexcept
{
    return sext_bitsize(val, 8ull * bytesize);
}

// Note: these function may take non-floating point values (e.g. u16 for half)
template<typename T>
inline T to_floating_point(uint64_t val) noexcept
{
  static_assert(sizeof(T) <= sizeof(uint64_t), "Output too small for T");

  T output{};
  std::memcpy(&output, &val, sizeof(T));
  return output;
}

template<typename T>
inline uint64_t from_floating_point(T val) noexcept
{
  static_assert(sizeof(T) <= sizeof(uint64_t), "Input too large");

  uint64_t output{};
  std::memcpy(&output, &val, sizeof(T));
  return output;
}

inline uint16_t float_to_half(float fval)
{
    uint32_t val;
    std::memcpy(&val, &fval, sizeof(val));

    // float to half
    const uint16_t mant = (val >> 13) & 0x03FF; // mantisse
    const uint16_t sign = (val >> 16) & 0x8000; // sign
    const uint16_t texp = (val >> 23) & 0x00FF; // exp
    uint16_t exp = (texp & 0x0F) << 10;
    if(texp & 0x80)
    {
        exp |= 0x4000;
    }

    return sign | mant | exp;
}

inline float half_to_float(uint16_t half)
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

template<unsigned N>
constexpr inline bool is_int(int64_t x [[maybe_unused]] ) noexcept
{
    if constexpr(N == 8)
    {
        return static_cast<int8_t>(x) == x;
    }
    if constexpr(N == 16)
    {
        return static_cast<int16_t>(x) == x;
    }
    if constexpr(N == 32)
    {
        return static_cast<int32_t>(x) == x;
    }
    if constexpr(N < 64)
    {
        return -(1ll << (N - 1)) <= x && x < (1ll << (N - 1));
    }

    return true;
}

template<unsigned N>
constexpr inline bool is_uint(uint64_t x [[maybe_unused]] ) noexcept
{
    static_assert(N > 0, "isUInt<0> doesn't make sense");
    if constexpr(N == 8)
    {
        return static_cast<uint8_t>(x) == x;
    }
    if constexpr(N == 16)
    {
        return static_cast<uint16_t>(x) == x;
    }
    if constexpr(N == 32)
    {
        return static_cast<uint32_t>(x) == x;
    }
    if constexpr(N < 64)
    {
        return x < (1ull << (N));
    }

    return true;
}

template<typename T>
bool is_real(T value) noexcept
{
  const auto value_class = std::fpclassify(value);
  return value_class == FP_ZERO || value_class == FP_NORMAL;
}

#endif
