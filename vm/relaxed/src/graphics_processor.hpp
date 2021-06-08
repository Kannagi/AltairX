#ifndef ALTAIR_GRAPHICS_PROCESSOR_INTERNAL_H_DEFINED
#define ALTAIR_GRAPHICS_PROCESSOR_INTERNAL_H_DEFINED

#include "vm_relaxed.hpp"

#include <array>
#include <cstddef>

template<size_t S>
using buffer_t = std::array<std::byte, S>;

union register_t
{
    std::array<float, 4> vector;
    buffer_t<sizeof(float) * 4> buffer;
};

struct ArGraphicsProcessor_T
{
    ArGraphicsProcessor next = nullptr;
    ArVirtualMachine parent = nullptr;

    buffer_t<AR_GRAPHICS_PROCESSOR_L1_DSRAM_SIZE> l1dsram{};
    buffer_t<AR_GRAPHICS_PROCESSOR_L1_ISRAM_SIZE> l1isram{};
    buffer_t<AR_GRAPHICS_PROCESSOR_L1_CACHE_SIZE> l1cache{};

    buffer_t<AR_GRAPHICS_PROCESSOR_L2_TEXTURE_CACHE_SIZE> l2texturecache{};
    buffer_t<AR_GRAPHICS_PROCESSOR_L2_BUFFER_CACHE_SIZE> l2buffercache{};
    buffer_t<AR_GRAPHICS_PROCESSOR_L2_CACHE_SIZE> l2cache{};
    buffer_t<AR_GRAPHICS_PROCESSOR_L2_SPM_SIZE> l2spm{};
    buffer_t<AR_GRAPHICS_PROCESSOR_L2_DSRAM_SIZE> l2dsram{};

    buffer_t<AR_GRAPHICS_PROCESSOR_1T_SRAM_SIZE> sram1t{};

    std::array<register_t, AR_GRAPHICS_PROCESSOR_REG_COUNT> reg{};

    uint32_t pc{ 0 }; //program-counter
    uint32_t br{ 0 }; //buffer-register
    std::array<uint32_t, AR_GRAPHICS_PROCESSOR_MAX_OPERATIONS> opcodes{};

    /// \brief CPU Flags register
    ///
    /// Bit 0: XCHG flag, 1 is 4-way decode, 0 is 2-way
    /// Bit 1: Z flag, 1 if not equal, 0 if equal
    /// Bit 2: S flag, 1 if lesser, 0 if greater (signed comparison)
    /// Bit 3: U flag, 1 if lesser, 0 if greater (unsigned comparison)
    uint32_t flags{ 0 };

    std::array<ArOperation, AR_PROCESSOR_MAX_OPERATIONS> operations{};
} ArGraphicsProcessor_T;

uint32_t opcodeSetSize(ArGraphicsProcessor AR_RESTRICT graphicsProcessor);

#endif
