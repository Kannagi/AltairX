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

    // load an ELF file and put PC at specified entry point location
    void load_program(const std::filesystem::path& path, std::string_view entry_point_name);

    // load an ELF file
    // Entry point is always main, and main always receive the argc and argv arguments
    // r0 (=sp) is initialized at 0x801FFFF8 leaving 1Mio of stack size
    // Argv values are stack allocated and given in a0 and a1
    // This function adds code to emulate an hosted environment (i.e. can receive arguments):
    // ```altairx-asm
    // _entry:
    //    ; argc and argv are initialized by the VM by writting to memory and registers
    //    ; at this point:
    //    ; - r0 == stack begin
    //    ; - r1 == argc
    //    ; - r2 == argv
    //    call @main ; init LR too to come back here after main returns!
    //    moveix @main
    //    nop
    //    add.d r2, r1, 0 ; exit code, returned by main
    //    movei r1, 3 ; exit syscall
    //    syscall
    //    nop
    // ```
    void load_hosted_program(const std::filesystem::path& path, const std::vector<std::string_view>& argv);

    // tbd
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
