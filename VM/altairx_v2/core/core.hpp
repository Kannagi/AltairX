#ifndef AXCORE_HPP_INCLUDED
#define AXCORE_HPP_INCLUDED

#include <cstdint>
#include <vector>
#include <array>

#include "opcode.hpp"

class AxMemory;

class AxCore
{
public:
    using Register = uint32_t;
    static constexpr Register REG_ACC = 56;
    static constexpr Register REG_BA1 = 57;
    static constexpr Register REG_BA2 = 58;
    static constexpr Register REG_BF1 = 59;
    static constexpr Register REG_BF2 = 60;
    static constexpr Register REG_BL1 = 61;
    static constexpr Register REG_BL2 = 62;
    static constexpr Register REG_ZERO = 63;

    static constexpr uint64_t ICACHE_SIZE = 0x10000 / 1024; //(64 KiB , 4-way)
    static constexpr uint64_t DCACHE_SIZE = 0x8000 / 128;   //(32 KiB , 4-way)

    static constexpr uint32_t IO_VOID = 0;
    static constexpr uint32_t IO_READ = 1;
    static constexpr uint32_t IO_WRITE = 2;

    static constexpr uint32_t Z_MASK = 0x01;
    static constexpr uint32_t C_MASK = 0x02;
    static constexpr uint32_t N_MASK = 0x04;
    static constexpr uint32_t O_MASK = 0x08;

    static constexpr uint64_t MAX_CORES = 64;
    static constexpr uint64_t IREG_COUNT = 64;
    static constexpr uint64_t VREG_COUNT = 64;
    static constexpr uint64_t SPM_SIZE = 0x4000;

    struct RegisterSet
    {
        uint32_t lr = 0; // link-register
        uint32_t br = 0; // branch-register
        uint32_t lc = 0; // loop counter
        uint32_t fr = 0; // flag register
        uint32_t pc = 0; // program-counter
        uint32_t ir = 0; // interrupt-register
        uint32_t cc = 0; // cycle counter
        uint32_t ic = 0; // instruction counter

        // General purpose integer regs
        std::array<uint64_t, IREG_COUNT> gpi{};
        // General purpose fp regs, use accessor functions for typed access (float ect)
        std::array<double, VREG_COUNT> gpf{};
        // MDU registers (Q, QR, PL, PH)
        std::array<uint64_t, 4> mdu{};

        // EFU special registers
        float efu_q = 0.0f;
        double efu_qd = 0.0;
    };

    AxCore(AxMemory& memory);
    ~AxCore() = default;
    AxCore(const AxCore&) = delete;
    AxCore& operator=(const AxCore&) = delete;
    AxCore(AxCore&&) noexcept = delete;
    AxCore& operator=(AxCore&&) noexcept = delete;

    // Execute opcode1 and, if possible, opcode2. Returns the number of opcodes run (1 or 2)
    // Used internally in cycle(), may be used in tests.
    uint32_t execute(AxOpcode first, AxOpcode second);
    
    // Emulate a whole cycle. Read next instructions from current PC and update it.
    void cycle()
    {
        const auto real_pc = m_regs.pc & 0x7FFFFFFF;
        const auto opcode1 = m_wram_begin[real_pc];
        const auto opcode2 = m_wram_begin[real_pc + 1u];
        const auto count = execute(opcode1, opcode2);

        m_regs.cc += 1;
        m_regs.ic += count;
        m_regs.pc += count;
    }

    // Emulate a syscalls if last executed included a syscall instruction.
    void syscall()
    {
        if(m_syscall == 0)
        {
            return;
        }

        execute_syscall();
    }

    AxMemory& memory() noexcept
    {
        return *m_memory;
    }

    const AxMemory& memory() const noexcept
    {
        return *m_memory;
    }

    RegisterSet& registers() noexcept
    {
        return m_regs;
    }

    const RegisterSet& registers() const noexcept
    {
        return m_regs;
    }

    uint8_t* smp_data() noexcept
    {
        return m_spm.data();
    }

    const uint8_t* smp_data() const noexcept
    {
        return m_spm.data();
    }

    int error() const noexcept
    {
        return m_error;
    }

private:
    void do_store(uint64_t src, uint64_t addr, uint32_t size);
    uint64_t do_load(uint64_t addr, uint32_t size);

    void io_read(uint64_t offset, void* reg);
    void io_write(uint64_t offset, void* reg);

    void execute_unit(AxOpcode opcode, uint32_t slot, uint64_t imm24);

    /*
    UNIT ID |    UNIT NAME
            | INST 1 | INST 2
       0    |  ALU1  |  ALU2
       1    |  ALU1  |  ALU2
       2    |  LSU1  |  LSU2
       3    |  FPU1  |  FPU2
       4    |  /     |   /
       5    |  EFU   |   CU
       6    |  MDU   |   VU
       7    |  BRU   |   /
    */
    void execute_alu(AxOpcode op, uint32_t slot, uint64_t imm24);
    void execute_mdu(AxOpcode op, uint64_t imm24);
    void execute_lsu(AxOpcode op, uint32_t slot, uint64_t imm24);
    void execute_bru(AxOpcode op, uint64_t imm24);
    void execute_fpu(AxOpcode op, uint32_t slot, uint64_t imm24);
    void execute_efu(AxOpcode op, uint64_t imm24);
    void execute_cu(AxOpcode op, uint64_t imm24);
    void execute_vu(AxOpcode op, uint64_t imm24);

    void execute_syscall();

    std::array<uint8_t, SPM_SIZE> m_spm{};
    RegisterSet m_regs;
    AxMemory* m_memory{};
    const uint32_t* m_wram_begin{};

    int m_error = 0;
    uint32_t m_cycle = 0;
    uint32_t m_instruction = 0;
    uint32_t m_syscall = 0;
};

#endif
