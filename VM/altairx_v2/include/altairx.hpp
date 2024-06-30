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

struct AxParameters
{
  std::size_t core_count{1};
  std::size_t wram_size{16};
  std::size_t spmt_size{256};
  std::size_t spm2_size{512};
  AxExecutionMode mode{};
  std::filesystem::path executable{};
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
};

//----------------------------
void altairx_debug(uint32_t opcode);

#endif
