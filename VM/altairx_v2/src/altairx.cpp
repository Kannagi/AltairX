#include <chrono>
#include <iostream>
#include <fstream>
#include <vector>

#ifdef HAS_LLVM
    #include <elf.hpp>
#endif

#include "altairx.hpp"
#include "panic.hpp"

AltairX::AltairX(size_t nwram, size_t nspmt, size_t nspm2)
    : m_memory{nwram, nspmt, nspm2}
    , m_core{m_memory}
    , m_wram_begin{static_cast<const uint32_t*>(m_memory.map(m_core, AxMemory::WRAM_BEGIN))}
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

namespace
{

const AxELFSymbol& get_entry_point(const AxELFFile& elf, std::string_view entry_point_name)
{
    const AxELFSymbol* entry_point_symbol{};
    for(auto&& symbol : elf.symbols)
    {
        if(symbol.name == entry_point_name)
        {
            entry_point_symbol = &symbol;
        }
    }

    if(!entry_point_symbol)
    {
        ax_panic("Entry point \"", entry_point_name, "\" could not be found.");
    }

    if(entry_point_symbol->type != AX_STT_FUNC)
    {
        ax_panic("Entry point \"", entry_point_symbol->name, "\" exists but is not a function. Type: ", entry_point_symbol->type);
    }

    return *entry_point_symbol;
}

struct AxSectionBounds
{
    uint64_t begin{};
    uint64_t end{};
};

std::vector<AxSectionBounds> load_sections(const AxELFFile& elf, const AxELFSymbol& entry_point, AxMemory& memory, AxCore& core)
{
    std::vector<AxSectionBounds> output;
    output.reserve(elf.sections.size());

    bool entry_point_allocated{};
    for(auto&& section : elf.sections)
    {
        if(section.has_flag(AX_SHF_ALLOC))
        {
            if(section.addr + section.content.size() > memory.wram_bytesize())
            {
                ax_panic("Not enough memory to load program.");
            }

            auto* wram = reinterpret_cast<char*>(memory.map(core, AxMemory::WRAM_BEGIN + section.addr));
            std::memcpy(wram, section.content.data(), section.content.size());

            if(section.addr <= entry_point.value && section.addr + section.size >= entry_point.value + entry_point.size)
            {
                entry_point_allocated = true;
            }

            output.emplace_back(AxSectionBounds{section.addr, section.addr + section.content.size()});
        }
    }

    if(!entry_point_allocated)
    {
        ax_panic("Entry point location does not match any allocated section. Aborting.");
    }

    // function symbol value is the function addr
    core.registers().pc = entry_point.value / 4;

    return output;
}

}

void AltairX::load_program(const std::filesystem::path& path, std::string_view entry_point_name)
{
#ifdef HAS_LLVM
    if(auto elf{AxELFFile::from_file(path)}; elf)
    {
        auto& entry_point = get_entry_point(*elf, entry_point_name);
        load_sections(*elf, entry_point, m_memory, m_core);
        return;
    }
#endif

    // load raw executable file
    std::ifstream file{path, std::ios::binary};
    if(!file.is_open())
    {
        ax_panic("Error : Impossible open file \"", path.string(), "\"");
    }

    file.seekg(0, std::ios::end);
    std::streampos filesize = file.tellg();
    file.seekg(0, std::ios::beg);

    void* wram = m_memory.map(m_core, AxMemory::WRAM_BEGIN);
    file.read(reinterpret_cast<char*>(wram), filesize);
    m_core.registers().pc = 4;
}

namespace
{

uint64_t setup_stack(AxMemory& memory, AxCore& core, const std::vector<AxSectionBounds>& bounds)
{
    AxSectionBounds total_bounds{std::numeric_limits<uint64_t>::max(), 0};
    for(auto&& section : bounds)
    {
        total_bounds.begin = std::min(total_bounds.begin, section.begin);
        total_bounds.end = std::max(total_bounds.end, section.end);
    }

    // align end on next multiple of 64Kio
    const auto aligned_end = ((total_bounds.end >> 16) + 1) << 16;
    // setup stack pointer 1Mio after aligned bounds giving at least 1Mio space
    const auto stack_size = 0x100000ull;

    if(memory.wram_bytesize() < aligned_end + stack_size)
    {
        ax_panic("WRAM is too short to allocate ", ((stack_size >> 10) + 1) << 10, "Kio of stack!\nRequires at least ",
            ((total_bounds.end >> 20) + 1) << 20, "Mio of VRAM.");
    }

    core.registers().gpi[0] = AxMemory::WRAM_BEGIN + (aligned_end + stack_size - 8);

    return aligned_end + stack_size;
}

void load_host_argv(AxMemory& memory, AxCore& core, std::string_view program_name, const std::vector<std::string_view>& argv)
{
    const auto ax_alloca = [&core](uint64_t size)
    {
        return core.registers().gpi[0] -= size;
    };

    const auto add_argument = [&memory, &core, &ax_alloca](std::string_view arg)
    {
        auto addr = ax_alloca(arg.size() + 1); // size + null terminator
        auto* wram = reinterpret_cast<uint8_t*>(memory.map(core, addr));
        std::memcpy(wram, arg.data(), arg.size());
        wram[arg.size()] = 0;

        return addr; // current pointer for the real argv buffer
    };

    std::vector<uint64_t> argv_buffer;
    argv_buffer.emplace_back(add_argument(program_name));
    for(auto&& arg : argv)
    {
        argv_buffer.emplace_back(add_argument(arg));
    }
    argv_buffer.emplace_back(0);

    const auto argv_buffer_size = argv_buffer.size() * 8;
    const auto addr = ax_alloca(argv_buffer_size);
    std::memcpy(memory.map(core, addr), argv_buffer.data(), argv_buffer_size);

    // int argc
    core.registers().gpi[1] = static_cast<int>(argv.size() + 1);
    // char* argv[]
    core.registers().gpi[2] = addr;
    // forward host env in char* env[] ?
}

void write_entry_code(AxMemory& memory, AxCore& core, uint64_t main_addr, uint64_t entry_addr)
{
    const auto main_pc = static_cast<uint32_t>(main_addr / 4ull);

    const std::array<std::uint32_t, 8> entry_code =
        {
            0x000000FBu | ((main_pc & 0x00FFFFFFu) << 8),    // call @main; init LR too to come back here after main returns!
            0x00000000u | (((main_pc >> 24) & 0x07Fu) << 8), // moveix @main
            0x00000000u,                                     // nop
            0x08100620u,                                     // add.d r2, r1, 0; exit code, returned by main
            0x00000302u,                                     // movei r1, 3; exit syscall
            0x000004FEu,                                     // syscall
            0x00000000u,                                     // nop
            0x00000000u,                                     // nop
        };

    const auto required_space = entry_addr + entry_code.size() * 4;
    if(memory.wram_bytesize() < required_space)
    {
        ax_panic("WRAM is too short to allocate entry code!\n"
                 "Current configuration requires at least ",
            ((required_space >> 20) + 1) << 20, "Mio of VRAM.");
    }

    std::memcpy(memory.map(core, AxMemory::WRAM_BEGIN + entry_addr), entry_code.data(), entry_code.size() * 4);
    core.registers().pc = entry_addr / 4ull;
}

}

void AltairX::load_hosted_program(const std::filesystem::path& path, const std::vector<std::string_view>& argv)
{
#ifndef HAS_LLVM
    ax_panic("Host emulation requires a build with LLVM enabled!");
#else
    auto elf{AxELFFile::from_file(path)};
    if(!elf)
    {
        ax_panic("Could not read ELF file \"", path.string(), "\"");
    }

    // look for main
    const auto& entry_point = get_entry_point(*elf, "main");
    const auto sections_bounds = load_sections(*elf, entry_point, m_memory, m_core);

    // setup stack since we need it to store argv and also to position entry code right after
    const auto stack_end = setup_stack(m_memory, m_core, sections_bounds);

    write_entry_code(m_memory, m_core, entry_point.value, stack_end);
    load_host_argv(m_memory, m_core, path.filename().string(), argv);
#endif
}

int AltairX::run(AxExecutionMode mode)
{
    using clock = std::chrono::steady_clock;
    using seconds = std::chrono::duration<double>;

    static constexpr std::size_t threshold = 512 * 1024;

    auto tp1 = clock::now();
    std::size_t counter = 0;
    std::size_t cycles = 0;
    while(m_core.error() == 0)
    {
        execute();
        m_core.syscall_emul();

        // if(mode == AxExecutionMode::DEBUG)
        //     altairx_debug(m_opcodes[0], 0);
        // if(core->cycle > 20) exit(0);

        counter += 1;
        cycles += 1;
        if(counter > threshold) // only check each few cycles...
        {
            const auto tp2 = clock::now();
            const auto delta = std::chrono::duration_cast<seconds>(tp2 - tp1).count();
            if(delta > 1.0) // ...and display if more than one second elapsed...
            {
                double frequency = static_cast<double>(cycles) / delta;
                std::cout << "Frequence : " << frequency / 1'000'000.0 << "MHz\n"; // no flush

                tp1 = clock::now();
                cycles = 0;
            }

            counter = 0;
        }
    }

    return m_core.error();
}

void AltairX::execute()
{
    auto& regs = m_core.registers();
    const auto real_pc = regs.pc & 0x7FFFFFFF;
    const auto opcode1 = m_wram_begin[real_pc];
    const auto opcode2 = m_wram_begin[real_pc + 1u];
    const auto count = m_core.execute(opcode1, opcode2);

    regs.cc += 1;
    regs.ic += count;
    regs.pc += count;
}
