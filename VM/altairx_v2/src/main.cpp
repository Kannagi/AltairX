
#include <string.h>
#include <stdint.h>

#include <iostream>
#include <vector>
#include <charconv>
#include <string_view>
#include <optional>

#include "altairx.hpp"
#include "panic.hpp"

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
    std::string entry_point{"main"};
    bool hosted{false};
    std::vector<std::string_view> forwarded_args{};
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
        ax_panic("Expected value for ", args[i]);
    }

    auto& value = args[i + 1];
    std::int64_t output{};
    auto [ptr, error] = std::from_chars(value.data(), value.data() + value.size(), output);
    if(error != std::errc{})
    {
        ax_panic("Failed to parse value ", value, " for argument ", args[i]);
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
        else if(args[i] == "-entry-point")
        {
            if(i == args.size() - 1) // last arg
            {
                ax_panic("Expected value for ", args[i]);
            }

            output.entry_point = std::string{argv[i + 1]};
            ++i;
        }
        else if(args[i] == "-hosted")
        {
            output.hosted = true;
        }
        else if(args[i] == "--")
        {
            if(i > 1)
            {
                output.executable = args[i - 1];
            }

            output.forwarded_args = std::vector<std::string_view>{args.begin() + i + 1, args.end()};
            break;
        }
        else // last arg
        {
            output.executable = args[i];
        }
    }

    if(output.executable.empty())
    {
        ax_panic("Missing executable file");
    }

    if(!output.forwarded_args.empty() && !output.hosted)
    {
        std::cerr << "Warning: arguments forwarding only work with host simulation!" << std::endl;
    }

    return output;
}

void print_usage()
{
    std::cout << "Usage: vm_altairx [options] executable_file [-- args...]\n";
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

    std::cout << std::endl; // flush and newline!
}

int run_vm(const AxParameters& parameters)
{
    AltairX altairx{parameters.wram_size, parameters.spmt_size, parameters.spm2_size};

    if(parameters.hosted)
    {
        altairx.load_hosted_program(parameters.executable, parameters.forwarded_args);
    }
    else
    {
        altairx.load_program(parameters.executable, parameters.entry_point);
    }

    return altairx.run(parameters.mode);
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

    try
    {
        return run_vm(parameters);
    }
    catch(const std::exception& e)
    {
        std::cerr << "VM error: " << e.what() << std::endl;
        return 1;
    }
}