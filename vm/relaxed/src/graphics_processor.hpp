#ifndef ALTAIR_GRAPHICS_PROCESSOR_INTERNAL_H_DEFINED
#define ALTAIR_GRAPHICS_PROCESSOR_INTERNAL_H_DEFINED

#include "vm_relaxed.hpp"

#include <array>
#include <cstddef>

template<size_t S>
using buffer_t = std::array<std::byte, S>;

union register_t
{
    std::array<uint32_t, 4> vectori;
    std::array<float, 4> vectorf;
    buffer_t<sizeof(float) * 4> buffer;
};
static_assert(sizeof(float) == sizeof(uint32_t));

struct ArGraphicsProcessor_T
{
    ArGraphicsProcessor next = nullptr;
    ArVirtualMachine parent = nullptr;

    buffer_t<AR_GRAPHICS_PROCESSOR_DSRAM_SIZE> dsram{}; // Data RAM L1
    buffer_t<AR_GRAPHICS_PROCESSOR_ISRAM_SIZE> isram{}; // Instruction RAM
    buffer_t<AR_GRAPHICS_PROCESSOR_L1SPM_SIZE> l1smp{};  // Data Scratchpad memory

    buffer_t<AR_GRAPHICS_PROCESSOR_BUFFER_CACHE_SIZE>  buffercache{};
    buffer_t<AR_GRAPHICS_PROCESSOR_TEXTURE_CACHE_SIZE> texturecache{};
    buffer_t<AR_GRAPHICS_PROCESSOR_L2SPM_SIZE>         l2spm{};

    buffer_t<AR_GRAPHICS_PROCESSOR_1T_SRAM_SIZE> sram1t{};

    std::array<register_t, AR_GRAPHICS_PROCESSOR_REG_COUNT> reg{};

    uint32_t pc{ 0 }; //program-counter
    uint32_t br{ 0 }; //buffer-register
    std::array<uint32_t, AR_GRAPHICS_PROCESSOR_MAX_OPERATIONS> opcodes{};

    /// \brief GPU Flags register
    ///
    /// Bit 1: Z flag, 1 if not equal, 0 if equal
    /// Bit 2: S flag, 1 if lesser, 0 if greater (signed comparison)
    /// Bit 3: U flag, 1 if lesser, 0 if greater (unsigned comparison)
    uint32_t flags{ 0 };

    std::array<ArOperation, AR_GRAPHICS_PROCESSOR_MAX_OPERATIONS> operations{};
    uint32_t delayedBits{ 0 };
    std::array<ArOperation, AR_GRAPHICS_PROCESSOR_MAX_OPERATIONS> delayed{};
    // true if dmaOperation is to be treated
    bool dma{ false };
    ArOperation dmaOperation;
};

#endif
