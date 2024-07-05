#ifndef ALTAIRX_HPP_INCLUDED
#define ALTAIRX_HPP_INCLUDED

#include <cstdint>
#include <array>
#include <filesystem>

#include "memory.hpp"
#include "core.hpp"

enum class AxExecutionMode
{
  DEFAULT = 0,
  DEBUG = 1
};

class AltairX
{
public:
    AltairX(size_t nwram, size_t nspmt, size_t nspm2);

    void load_prog(const std::filesystem::path& path);
    void load_kernel(const std::filesystem::path& path);

    int run(AxExecutionMode mode);

private:
    void execute();

    AxMemory m_memory;
    AxCore m_core;
    const uint32_t* m_wram_begin{};
};

//----------------------------
void altairx_debug(uint32_t opcode);

#endif
