#include <memory.hpp>

#include <core.hpp>

AxMemory::AxMemory(size_t nwram, size_t nspmt, size_t nspm2)
{
    m_io.resize(512ull * 1024ull / 8ull); // 512 Kio
    m_rom.resize(16ull * 1024ull * 1024ull / 8ull); // 16 Mio
    m_wram.resize(0x100000ull * nwram / 8ull); // 1Mio pages
    m_spmt.resize(0x400ull * nspmt / 8ull); // 1Kio pages
    m_spm2.resize(0x400ull * nspm2 / 8ull); // 1Kio pages

    m_spmt_mask = (m_spmt.size() * 8ull) - 1;
    m_spm2_mask = (m_spm2.size() * 8ull) - 1;
    m_wram_mask = (m_wram.size() * 8ull) - 1;
}

void* AxMemory::map(AxCore& core, uint64_t addr)
{
    uint8_t* base{};
    uint64_t mask{};
    if(addr & WRAM_BEGIN)
    {
        base = reinterpret_cast<uint8_t*>(m_wram.data());
        mask = m_wram_mask;
    }
    else if(addr & SPM2_BEGIN)
    {
        base = reinterpret_cast<uint8_t*>(m_spm2.data());
        mask = m_spm2_mask; // Max 16 Mio
    }
    else if(addr & SPMT_BEGIN)
    {
        base = reinterpret_cast<uint8_t*>(m_spmt.data());
        mask = m_spmt_mask; // Max 16 Mio
    }
    else if(addr & ROM_BEGIN)
    {
        base = reinterpret_cast<uint8_t*>(m_rom.data());
        mask = 0xFFFFFF; // Max 16 Mio
    }
    else if(addr & IO_BEGIN)
    {
        base = reinterpret_cast<uint8_t*>(m_io.data());
        mask = 0x7FFFF; // Max 512 Kio
    }
    else // SPM
    {
        base = core.smp_data();
        mask = 0x3FFF; // Max 16 Kio
    }

    const auto offset = addr & mask;
    return base + offset;
}