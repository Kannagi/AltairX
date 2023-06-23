#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <charconv>
#include <chrono>

#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/ADT/SCCIterator.h>
#include <llvm/ADT/PostOrderIterator.h>
#include <llvm/IR/Constants.h>

#include "utilities.hpp"
#include "early_transforms.hpp"
#include "function_analyser.hpp"
#include "mid_transforms.hpp"
#include "late_transforms.hpp"
#include "register_allocator.hpp"
#include "translator.hpp"

static std::string stringify(llvm::StringRef data)
{
    std::string output{"\""};

    for(auto value : data)
    {
        if(value == '\\')
        {
            output += "\\\\";
        }
        else if(std::isprint(static_cast<unsigned char>(value)))
        {
            output += value;
        }
        else
        {
            std::array<char, 2> tmp{};
            auto [ptr, ec] = std::to_chars(tmp.data(), tmp.data() + tmp.size(), static_cast<unsigned char>(value), 16);
            if (ec != std::errc())
            {
                throw std::runtime_error{"To chars failed"};
            }

            output += "\\x";
            if(std::distance(std::data(tmp), ptr) > 1)
            {
                output += tmp[0];
                output += tmp[1];
            }
            else
            {
                output += '0';
                output += tmp[0];
            }
        }
    }

    output += '\"';

    return output;
}

static std::string constant_to_string(llvm::Constant* constant)
{
    if(auto sequential{llvm::dyn_cast<llvm::ConstantDataSequential>(constant)}; sequential)
    {
        return stringify(sequential->getAsString());
    }

    throw std::runtime_error{"unsupported constant type"};
}

static std::string generate_dot_data(llvm::Module& module)
{
    std::string output{};

    for(llvm::GlobalVariable& global : module.globals())
    {
        if(global.isConstant())
        {
            output += ar::get_value_label(global);
            output += " : ";
            output += constant_to_string(global.getInitializer());
            output += "\n";
        }
        else
        {
            std::cout << "Non constant global are not supported, yet.\n";
            std::cout << "Ignoring \"" << ar::get_value_label(global) << "\"" << std::endl;
        }
    }

    return output;
}

static void write_module(llvm::Module& module, std::string filename)
{
    // Write back the module with all modification done by the compiler
    std::string output{};
    llvm::raw_string_ostream os{output};
    os << module;
    os.flush();

    std::ofstream{filename + ".comp.ll"} << output;
}

//Code generation steps:
// 1: Transformation of the IR for analysis:
//    Transforms the IR by adding, removing, or rewriting part or it,
//    in order to facilitate (or enable) further analysis and transformations
//
// 2: Compliance pass:
//    Transforms the IR to make it compliant to Altair ASM:
//      - transforming impossible cmp
//      - add movei/loads for constants
//      - check immediate values
//      - ...
//
// 3: Optimization pass:
//    Modifies the IR to increase performances of the future bytecode.
//
// 4: Register allocation:
//    Performs register allocation to get rid of the SSA form.
//
// 5: Translation
//    Translates the IR to Altair ASM
//
static void generate_asm(llvm::Module& module, const std::string& filename, const ar::compiler_options& options)
{
    std::ofstream output_file{filename + ".asm"};
    output_file << "jump main ;This is the bootstrap\n";
    output_file << "nop\n\n";
    output_file << ".data\n";
    output_file << generate_dot_data(module);
    output_file << "\n";
    output_file << ".text\n";

    for(llvm::Function& function : module.functions())
    {
        if(function.isDeclaration())
        {
            continue;
        }

        // Early transforms
        ar::early_transforms::swap_add_sub(module, function);
        ar::early_transforms::decompose_getelementptr(module, function);
        ar::early_transforms::optimize_load_store(module, function);
        ar::early_transforms::reorder_blocks(module, function);
        ar::early_transforms::invert_branch_condition(module, function);

        ar::function_analyser analyser{module, function};
        analyser.perform_analysis(); // initial analysis

        // Mid transforms
        ar::mid_transforms::insert_move_for_constant(analyser);
        ar::mid_transforms::insert_move_for_global_load(analyser);
        ar::mid_transforms::fix_conflicting_phi_nodes(analyser);
        ar::mid_transforms::resolve_conflicting_affinities(analyser);

        if(options.optimize)
        {
            write_module(module, filename + ".noopt");
            ar::mid_transforms::optimize_pipeline(analyser);
        }

        ar::register_allocator allocator{analyser};
        allocator.perform_register_allocation();

        // Late transforms
        // ...

        if(options.verbose)
        {
            allocator.print();
        }

        ar::function_translator translator{allocator, options};
        output_file << translator.translate() << std::endl;
    }
}

static void print_help()
{
    std::cout << "Usage:\n altair_compiler <IR file> [options]" << std::endl;
}

static ar::compiler_options parse_args(int argc, char* argv[])
{
    using namespace std::string_view_literals;

    if(argc < 2)
    {
        print_help();
        throw std::runtime_error{"Not enough arguments"};
    }

    ar::compiler_options output{};

    for(int i{1}; i < argc; ++i)
    {
        if(argv[i] == "-O"sv || argv[i] == "-optimize"sv)
        {
            output.optimize = true;
        }
        else if(argv[i] == "--verbose"sv)
        {
            output.verbose = true;
        }
        else if(argv[i] == "--output-noop-instructions"sv)
        {
            output.output_noop_instructions = true;
        }
        else if(argv[i] == "--pretty-registers"sv)
        {
            output.pretty_registers = true;
        }
        else
        {
            if(!std::empty(output.file))
            {
                print_help();
                throw std::runtime_error{"Multiple arguments not stating with \"-\""};
            }

            output.file = argv[i];
        }
    }

    return output;
}

static void run(const ar::compiler_options& options)
{
    llvm::LLVMContext context{};
    llvm::SMDiagnostic err{};
    auto module{llvm::parseIRFile(options.file, err, context)};

    if(!module)
    {
        throw std::runtime_error{err.getMessage().str()};
    }

    std::string filename{options.file};
    filename.erase(std::find(std::begin(filename), std::end(filename), '.'), filename.end());

    using clock = std::chrono::steady_clock;
    const auto tp1{clock::now()};

    // Compile module!
    generate_asm(*module, filename, options);

    std::cout << "Compilation done in " << std::chrono::duration_cast<std::chrono::milliseconds>(clock::now() - tp1).count() << "ms" << std::endl;

    write_module(*module, filename);
}

int main(int argc, char* argv[])
{
    try
    {
        const auto options{parse_args(argc, argv)};
        run(options);
    }
    catch(const std::exception& e)
    {
        std::cerr << "Compilation error:\n" << e.what() << std::endl;
        return 1;
    }
    catch(...)
    {
        std::cerr << "Compilation error: Unknown error." << std::endl;
        return 1;
    }
}
