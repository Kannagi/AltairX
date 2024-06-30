#include <chrono>
#include <iostream>
#include <fstream>
#include <vector>

#ifdef HAS_LLVM
#include <elf.hpp>
#endif

#include "altairx.hpp"

AltairX::AltairX(size_t nwram, size_t nspmt, size_t nspm2)
: m_memory{nwram, nspmt, nspm2}
, m_core{m_memory}
{
}

void AltairX::load_kernel(const std::filesystem::path& path)
{
    std::ifstream file{path, std::ios::binary};
    if(!file.is_open())
    {
        std::cerr << "Error : Impossible open kernel" << std::endl;
        return;
    }

    file.seekg(0, std::ios::end);
    std::streampos filesize = file.tellg();
    file.seekg(0, std::ios::beg);

    void* rom = m_memory.map(m_core, AxMemory::ROM_BEGIN);
    file.read(reinterpret_cast<char*>(rom), filesize);
}

void AltairX::load_prog(const std::filesystem::path& path)
{
#ifdef HAS_LLVM
    if(auto elf{AxELFFile::from_file(path)}; elf)
    {
      for(auto&& section : elf->sections)
      {
        std::cout << section.has_flag(AX_SHF_ALLOC) << std::endl;
        std::cout << section.addr << std::endl;
        std::cout << section.content.size() << std::endl;
      }

      return;
    }
#endif

    std::ifstream file{path, std::ios::binary};
    if(!file.is_open())
    {
        std::cerr << "Error : Impossible open binary" << std::endl;
        return;
    }

    file.seekg(0, std::ios::end);
    std::streampos filesize = file.tellg();
    file.seekg(0, std::ios::beg);

    void* wram = m_memory.map(m_core, AxMemory::WRAM_BEGIN);
    file.read(reinterpret_cast<char*>(wram), filesize);
}

int AltairX::run(AxExecutionMode mode)
{
  using clock = std::chrono::steady_clock;
  using seconds = std::chrono::duration<double>;

  static constexpr std::size_t threshold = 8192;

  auto tp1 = clock::now();
  std::size_t counter = 0;
  while(m_core.error() == 0)
  {
    execute();
    m_core.syscall_emul();

    // if(mode == AxExecutionMode::DEBUG)
    //     altairx_debug(m_opcodes[0], 0);
    // if(core->cycle > 20) exit(0);

    counter += 1;
    if(counter > threshold) // only check each few cycles...
    {
      const auto tp2 = clock::now();
      const auto delta = std::chrono::duration_cast<seconds>(tp2 - tp1).count();
      if(delta > 1.0) // ...and display if more than one second elapsed...
      {
        double frequency = (static_cast<double>(counter) / delta) / (1000 * 1000);
        std::cout << "Frequence : " << frequency << "MHz\n"; // no flush

        tp1 = clock::now();
        counter = 0;
      }
    }
  }

  return m_core.error();
}

void AltairX::execute()
{
  const auto real_pc = m_core.registers().pc & 0x7FFFFFFF;
  const auto opcode1 = m_memory.load<uint32_t>(m_core, AxMemory::WRAM_BEGIN + (real_pc * 4ull));
  const auto opcode2 = m_memory.load<uint32_t>(m_core, AxMemory::WRAM_BEGIN + (real_pc * 4ull) + 4ull);
  const auto count = m_core.execute(opcode1, opcode2);

  m_core.registers().cc += 1;
  m_core.registers().ic += count;
  m_core.registers().pc += count;
}
