#include <base/vm.h>

#include <iostream>
#include <stdexcept>
#include <utility>
#include <memory>
#include <filesystem>
#include <fstream>

#include "shared_library.hpp"

namespace ar
{

inline namespace functions
{

static PFN_arCreateVirtualMachine      arCreateVirtualMachine{};
static PFN_arCreateProcessor           arCreateProcessor{};
static PFN_arCreatePhysicalMemory      arCreatePhysicalMemory{};
static PFN_arDecodeInstruction         arDecodeInstruction{};
static PFN_arExecuteInstruction        arExecuteInstruction{};
static PFN_arExecuteDirectMemoryAccess arExecuteDirectMemoryAccess{};
static PFN_arDestroyVirtualMachine     arDestroyVirtualMachine{};
static PFN_arDestroyProcessor          arDestroyProcessor{};
static PFN_arDestroyPhysicalMemory     arDestroyPhysicalMemory{};

static void load_functions(nes::shared_library& library)
{
    arCreateVirtualMachine      = library.load<PFN_arCreateVirtualMachine>("arCreateVirtualMachine");
    arCreateProcessor           = library.load<PFN_arCreateProcessor>("arCreateProcessor");
    arCreatePhysicalMemory      = library.load<PFN_arCreatePhysicalMemory>("arCreatePhysicalMemory");
    arDecodeInstruction         = library.load<PFN_arDecodeInstruction>("arDecodeInstruction");
    arExecuteInstruction        = library.load<PFN_arExecuteInstruction>("arExecuteInstruction");
    arExecuteDirectMemoryAccess = library.load<PFN_arExecuteDirectMemoryAccess>("arExecuteDirectMemoryAccess");
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

struct machine_options
{
    enum : std::uint32_t
    {
        pedantic = 0x01
    };

    std::string boot_path{};
    std::uint32_t flags{};
};

static machine_options parse_arguments(const std::vector<std::string_view>& args)
{
    if(std::size(args) < 2)
    {
        throw std::runtime_error{"Usage: altair_vm [path_to_binary] [flags]"};
    }

    machine_options output{};
    output.boot_path = args[1];

    for(auto it{std::cbegin(args) + 2}; it != std::cend(args); ++it)
    {
        if(*it == "-pedantic")
        {
            output.flags |= machine_options::pedantic;
        }
        else
        {
            std::cout << "Unrecognised argument [" << *it << "]" << std::endl;
        }
    }

    return output;
}

#ifdef NES_WIN32_SHARED_LIBRARY
static constexpr const char* relaxed_default_path{"altair_vm_relaxed.dll"};
static constexpr const char* pedantic_default_path{"altair_vm_pedantic.dll"};
#else
static constexpr const char* relaxed_default_path{"altair_vm_relaxed.so"};
static constexpr const char* pedantic_default_path{"altair_vm_pedantic.so"};
#endif

static nes::shared_library open_implementation(std::uint32_t flags)
{
    if(static_cast<bool>(flags & machine_options::pedantic))
    {
        return nes::shared_library{pedantic_default_path};
    }
    else
    {
        return nes::shared_library{relaxed_default_path};
    }
}

static std::vector<std::uint32_t> read_binary(const std::filesystem::path& path)
{
    std::ifstream ifs{path, std::ios_base::binary};
    if(!ifs)
    {
        throw std::runtime_error{"Can not find file \"" + path.string() + "\"."};
    }

    std::vector<std::uint32_t> output{};
    output.resize(std::filesystem::file_size(path) / 4u);

    const auto bytes_size{static_cast<std::streamsize>(std::size(output) * 4)};
    if(ifs.read(reinterpret_cast<char*>(std::data(output)), bytes_size).gcount() != bytes_size)
    {
        throw std::runtime_error{"Can not read file \"" + path.string() + "\"."};
    }

    return output;
}

static void run(const machine_options& options)
{
    auto implementation {open_implementation(options.flags)};
    const auto boot_code{read_binary(options.boot_path)};

    ar::functions::load_functions(implementation);

    ar::virtual_machine machine{};
    ar::processor processor{machine, std::data(boot_code), std::size(boot_code)};
    ar::physical_memory memory{machine};

    while(true)
    {
        processor.decode();

        if(!processor.execute())
        {
            break;
        }

        processor.direct_memory_access();
    }
}

int main(int argc, char** argv)
{
    try
    {
        std::vector<std::string_view> args{};
        args.reserve(static_cast<std::size_t>(argc));
        for(int i{}; i < argc; ++i)
        {
            args.emplace_back(argv[i]);
        }

        run(parse_arguments(args));
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;

        return 1;
    }
}
