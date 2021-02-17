#ifndef VM_HPP_INCLUDED
#define VM_HPP_INCLUDED

#include <base/vm.h>

#include <stdexcept>
#include <utility>
#include <memory>

#include "shared_library.hpp"

namespace ar
{

inline namespace functions
{

static PFN_arCreateVirtualMachine arCreateVirtualMachine{};
static PFN_arCreateProcessor      arCreateProcessor{};
static PFN_arCreatePhysicalMemory arCreatePhysicalMemory{};

static PFN_arDecodeInstruction         arDecodeInstruction{};
static PFN_arExecuteInstruction        arExecuteInstruction{};
static PFN_arExecuteDirectMemoryAccess arExecuteDirectMemoryAccess{};

static PFN_arGetProcessorOperations arGetProcessorOperations{};
static PFN_arGetProcessorMemoryInfo arGetProcessorMemoryInfo{};

static PFN_arDestroyVirtualMachine arDestroyVirtualMachine{};
static PFN_arDestroyProcessor      arDestroyProcessor{};
static PFN_arDestroyPhysicalMemory arDestroyPhysicalMemory{};

inline void load_functions(nes::shared_library& library)
{
    arCreateVirtualMachine      = library.load<PFN_arCreateVirtualMachine>("arCreateVirtualMachine");
    arCreateProcessor           = library.load<PFN_arCreateProcessor>("arCreateProcessor");
    arCreatePhysicalMemory      = library.load<PFN_arCreatePhysicalMemory>("arCreatePhysicalMemory");

    arDecodeInstruction         = library.load<PFN_arDecodeInstruction>("arDecodeInstruction");
    arExecuteInstruction        = library.load<PFN_arExecuteInstruction>("arExecuteInstruction");
    arExecuteDirectMemoryAccess = library.load<PFN_arExecuteDirectMemoryAccess>("arExecuteDirectMemoryAccess");

    arGetProcessorOperations    = library.load<PFN_arGetProcessorOperations>("arGetProcessorOperations");
    arGetProcessorMemoryInfo    = library.load<PFN_arGetProcessorMemoryInfo>("arGetProcessorMemoryInfo");

    arDestroyVirtualMachine     = library.load<PFN_arDestroyVirtualMachine>("arDestroyVirtualMachine");
    arDestroyProcessor          = library.load<PFN_arDestroyProcessor>("arDestroyProcessor");
    arDestroyPhysicalMemory     = library.load<PFN_arDestroyPhysicalMemory>("arDestroyPhysicalMemory");
}

}

class virtual_machine
{
public:
    explicit virtual_machine()
    {
        ArVirtualMachineCreateInfo info;
        info.sType = AR_STRUCTURE_TYPE_VIRTUAl_MACHINE_CREATE_INFO;
        info.pNext = nullptr;

        const auto result{arCreateVirtualMachine(&m_virtual_machine, &info)};
        if(result != AR_SUCCESS)
        {
            throw std::runtime_error{"Can not create virtual machine."};
        }
    }

    ~virtual_machine()
    {
        if(m_virtual_machine)
        {
            arDestroyVirtualMachine(m_virtual_machine);
        }
    }

    virtual_machine(const virtual_machine&) = delete;
    virtual_machine& operator=(const virtual_machine&) = delete;

    virtual_machine(virtual_machine&& other) noexcept
    :m_virtual_machine{std::exchange(other.m_virtual_machine, nullptr)}
    {

    }

    virtual_machine& operator=(virtual_machine&& other) noexcept
    {
        m_virtual_machine = std::exchange(other.m_virtual_machine, m_virtual_machine);

        return *this;
    }

    ArVirtualMachine handle() const noexcept
    {
        return m_virtual_machine;
    }

private:
    ArVirtualMachine m_virtual_machine{};
};

class processor
{
public:
    using operation_set_t = std::pair<std::size_t, std::array<ArOperation, AR_PROCESSOR_MAX_OPERATIONS>>;

public:
    explicit processor(virtual_machine& machine, const std::uint32_t* code, std::size_t code_size)
    :m_virtual_machine{machine.handle()}
    {
        ArProcessorCreateInfo info;
        info.sType = AR_STRUCTURE_TYPE_PROCESSOR_CREATE_INFO;
        info.pNext = nullptr;
        info.pBootCode = code;
        info.bootCodeSize = code_size;

        const auto result{arCreateProcessor(m_virtual_machine, &info, &m_processor)};
        if(result != AR_SUCCESS)
        {
            throw std::runtime_error{"Can not create processor."};
        }
    }

    ~processor()
    {
        if(m_processor)
        {
            arDestroyProcessor(m_virtual_machine, m_processor);
        }
    }

    processor(const processor&) = delete;
    processor& operator=(const processor&) = delete;

    processor(processor&& other) noexcept
    :m_virtual_machine{other.m_virtual_machine}
    ,m_processor{std::exchange(other.m_processor, nullptr)}
    {

    }

    processor& operator=(processor&& other) noexcept
    {
        m_virtual_machine = other.m_virtual_machine;
        m_processor = std::exchange(other.m_processor, m_processor);

        return *this;
    }

    void decode()
    {
        const auto result{arDecodeInstruction(m_processor)};
        if(result != AR_SUCCESS)
        {
            //TODO: put a backtrace and opcode that generated the error
            throw std::runtime_error{"Can not decode instruction."};
        }
    }

    bool execute()
    {
        const auto result{arExecuteInstruction(m_processor)};

        if(result == AR_END_OF_CODE)
        {
            return false;
        }
        else if(result != AR_SUCCESS)
        {
            //TODO: put a backtrace and opcode that generated the error
            throw std::runtime_error{"Can not execute instruction."};
        }

        return true;
    }

    void direct_memory_access()
    {
        const auto result{arExecuteDirectMemoryAccess(m_processor)};

        if(result != AR_SUCCESS)
        {
            //TODO: put a backtrace
            throw std::runtime_error{"Can not execute direct memory access."};
        }
    }

    operation_set_t get_operations() const
    {
        operation_set_t output{};
        arGetProcessorOperations(m_processor, std::data(output.second), &output.first);

        return output;
    }

    ArProcessorMemoryInfo get_memory_info() const noexcept
    {
        ArProcessorMemoryInfo output{};
        arGetProcessorMemoryInfo(m_processor, &output);

        return output;
    }

    ArProcessor handle() const noexcept
    {
        return m_processor;
    }

private:
    ArVirtualMachine m_virtual_machine{};
    ArProcessor m_processor{};
};

class physical_memory
{
public:
    static constexpr std::size_t default_size{8 * 1024 * 1024};

public:
    explicit physical_memory(virtual_machine& machine, std::size_t size = default_size)
    :m_virtual_machine{machine.handle()}
    ,m_memory{std::make_unique<std::uint8_t[]>(size)}
    {
        ArPhysicalMemoryCreateInfo info;
        info.sType = AR_STRUCTURE_TYPE_PHYSICAL_MEMORY_CREATE_INFO;
        info.pNext = nullptr;
        info.pMemory = m_memory.get();
        info.size = size;

        const auto result{arCreatePhysicalMemory(m_virtual_machine, &info, &m_physical_memory)};
        if(result != AR_SUCCESS)
        {
            throw std::runtime_error{"Can not create physical_memory."};
        }
    }

    ~physical_memory()
    {
        if(m_physical_memory)
        {
            arDestroyPhysicalMemory(m_virtual_machine, m_physical_memory);
        }
    }

    physical_memory(const physical_memory&) = delete;
    physical_memory& operator=(const physical_memory&) = delete;

    physical_memory(physical_memory&& other) noexcept
    :m_virtual_machine{other.m_virtual_machine}
    ,m_memory{std::move(other.m_memory)}
    ,m_physical_memory{std::exchange(other.m_physical_memory, nullptr)}
    {

    }

    physical_memory& operator=(physical_memory&& other) noexcept
    {
        m_virtual_machine = other.m_virtual_machine;
        m_memory = std::move(other.m_memory);
        m_physical_memory = std::exchange(other.m_physical_memory, m_physical_memory);

        return *this;
    }

    ArPhysicalMemory handle() const noexcept
    {
        return m_physical_memory;
    }

private:
    ArVirtualMachine m_virtual_machine{};
    std::unique_ptr<std::uint8_t[]> m_memory;
    ArPhysicalMemory m_physical_memory{};
};

}

#endif
