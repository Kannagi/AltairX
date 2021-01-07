#include <base/vm.h>

#include <base/agu.hpp>
#include <base/alu.hpp>
#include <base/bru.hpp>
#include <base/lsu.hpp>
#include <base/ops.hpp>
#include <base/vfpu.hpp>

#include <filesystem>
#include <fstream>

#include "shared_library.hpp"
#include "vm.hpp"
#include "execution.hpp"
#include "debug.hpp"

struct machine_options
{
    enum : std::uint32_t
    {
        pedantic = 0x01,
        debugger = 0x02
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
        else if(*it == "-dbg" || *it == "-debugger")
        {
            output.flags |= machine_options::debugger;
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

    if(static_cast<bool>(options.flags & machine_options::debugger))
    {
        run_debugger(processor);
    }
    else
    {
        execute(processor);
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
