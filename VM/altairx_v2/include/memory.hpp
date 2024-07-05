#ifndef AXMEMORY_HPP_INCLUDED
#define AXMEMORY_HPP_INCLUDED

#include <cstdint>
#include <vector>

class AxCore;

class AxMemory
{
public:
    static constexpr uint64_t SPM1_BEGIN = 0x0000'0000ull;
    static constexpr uint64_t IO_BEGIN = 0x0800'0000ull;
    static constexpr uint64_t ROM_BEGIN = 0x1000'0000ull;
    static constexpr uint64_t SPMT_BEGIN = 0x2000'0000ull;
    static constexpr uint64_t SPM2_BEGIN = 0x4000'0000ull;
    static constexpr uint64_t WRAM_BEGIN = 0x8000'0000ull;

    // nwram: wram size in Mio
    // nspmt: spm thread size in kio
    // nspm2: spm L2 size in kio
    AxMemory(size_t nwram, size_t nspmt, size_t nspm2);
    ~AxMemory() = default;
    AxMemory(const AxMemory&) = delete;
    AxMemory& operator=(const AxMemory&) = delete;
    AxMemory(AxMemory&&) noexcept = delete;
    AxMemory& operator=(AxMemory&&) noexcept = delete;

    void* map(AxCore& core, uint64_t offset);

    void store(AxCore& core, const void* src, uint64_t addr, uint32_t size)
    {
        std::memcpy(map(core, addr), src, size);
    }

    void load(AxCore& core, void* dest, uint64_t offset, uint32_t size)
    {
        std::memcpy(dest, map(core, offset), size);
    }

    template<typename T>
    T load(AxCore& core, uint64_t offset)
    {
        T output{};
        load(core, &output, offset, sizeof(T));
        return output;
    }

    template<typename T>
    void store(AxCore& core, T val, uint64_t offset)
    {
        store(core, &val, offset, sizeof(T));
    }

    std::uint64_t wram_size() const noexcept
    {
      return m_wram.size();
    }

private:
    // stored as uint64_t to enforce alignment!
    std::vector<uint64_t> m_io;
    std::vector<uint64_t> m_rom;
    std::vector<uint64_t> m_spmt;
    std::vector<uint64_t> m_spm2;
    std::vector<uint64_t> m_wram;

    uint64_t m_spmt_mask = 0;
    uint64_t m_spm2_mask = 0;
    uint64_t m_wram_mask = 0;
};

#endif
