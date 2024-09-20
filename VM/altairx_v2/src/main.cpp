
#include <string.h>
#include <stdint.h>

#include <iostream>
#include <vector>
#include <charconv>
#include <string_view>
#include <optional>

#include "altairx.hpp"

namespace
{

struct AxParameters
{
    std::size_t core_count{1};
    std::size_t wram_size{16};
    std::size_t spmt_size{256};
    std::size_t spm2_size{512};
    AxExecutionMode mode{};
    std::filesystem::path executable{};
};

std::vector<std::string_view> get_args(int argc, char* argv[])
{
    std::vector<std::string_view> output;
    output.reserve(static_cast<std::size_t>(argc));
    for(int i = 0; i < argc; ++i)
    {
        output.emplace_back(argv[i]);
    }

    return output;
}

std::int64_t get_value_for_arg(const std::vector<std::string_view>& args, std::size_t i, std::size_t size)
{
    if(i + 1 >= size)
    {
        throw std::runtime_error{"Expected value for " + std::string{args[i]}};
    }

    auto& value = args[i + 1];
    std::int64_t output{};
    auto [ptr, error] = std::from_chars(value.data(), value.data() + value.size(), output);
    if(error != std::errc{})
    {
        throw std::runtime_error{"Failed to parse value " + std::string{value} + " for argument " + std::string{args[i]}};
    }

    return output;
}

AxParameters parse_args(int argc, char* argv[])
{
    const auto args = get_args(argc, argv);

    AxParameters output;
    // skip first arg
    for(std::size_t i{1}; i < args.size(); ++i)
    {
        if(args[i] == "-ncore")
        {
            output.core_count = static_cast<std::size_t>(get_value_for_arg(args, i, args.size()));
            ++i;
        }
        else if(args[i] == "-wram")
        {
            output.wram_size = static_cast<std::size_t>(get_value_for_arg(args, i, args.size()));
            ++i;
        }
        else if(args[i] == "-spmt")
        {
            output.spmt_size = static_cast<std::size_t>(get_value_for_arg(args, i, args.size()));
            ++i;
        }
        else if(args[i] == "-spm2")
        {
            output.spm2_size = static_cast<std::size_t>(get_value_for_arg(args, i, args.size()));
            ++i;
        }
        else if(args[i] == "-mode")
        {
            output.mode = static_cast<AxExecutionMode>(get_value_for_arg(args, i, args.size()));
            ++i;
        }
        else if(i == args.size() - 1)
        {
            output.executable = args[i];
        }
        else
        {
            std::cerr << "Warning: unknown argument " << args[i] << std::endl;
        }
    }

    if(output.executable.empty())
    {
        throw std::runtime_error{"Missing executable file"};
    }

    return output;
}

void print_usage()
{
    std::cout << "Usage: vm_altairx [options] executable_file\n";
    std::cout << "Options:\n";
    std::cout << "    Core count: -ncore N\n";
    std::cout << "    WRAM size (MiB): -wram N\n";
    std::cout << "    SPMT size (KiB): -spmt N\n";
    std::cout << "    SPM2 size (KiB): -spm2 N\n";
    std::cout << "    Execution mode: -mode N\n";
    std::cout << "        Mode 0: console, syscall emulate, 1 core only\n";
    std::cout << "        Mode 1: mode 0 + debug\n";
    // std::cout << "        Mode 2: same mode 0 and cycle accurate\n";
    // std::cout << "        Mode 3: complete hardware\n";
    // std::cout << "        Mode 4: XSTAR OS\n";
}

}

int main(int argc, char* argv[])
{
    AxParameters parameters;
    try
    {
        parameters = parse_args(argc, argv);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        print_usage();
        return 0;
    }

    AltairX altairx{parameters.wram_size, parameters.spmt_size, parameters.spm2_size};
    altairx.load_prog(parameters.executable);

    return altairx.run(parameters.mode);
}