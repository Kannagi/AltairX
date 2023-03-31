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
#include "mid_transforms.hpp"
#include "late_transforms.hpp"
#include "register_allocator.hpp"
#include "translator.hpp"

/*
-Buffer user est divisé en 2 buffers (chaqu'un de 8kio) un qu'on lis/écrit , et le second qu'en précharge utilisé pour les tableaux uniquement
-temp user , c'est juste pour des variables temporaire ou global qui est utilisé tout le long du programme , par exemple  copier un int dans un float ,
peut se faire dedans , ou toute opération qui copie dans la mémoire de façon très "éphémère"
-le Stack ,c'est la pile,  mais à utilisé uniquement sur des fonctions en fin du coup (pas d'appel de fonction ou très peu , ça doit rentrer sur 44 Kio) ,
d'ailleurs on peut stocker les appel/return de fonction dedans
--Buffer A et Buffer B , sont en miroir avec le Buffer A et B en ROM ,
cela stocke les DATA/BSS utilisé (techniquement j'aurais aimé pour les variables ),
et l'autre buffer est préchargé pendant ce temps là

0x00000 - 0x03FFF Buffer user (16 Kio)
0x04000 - 0x04FFF temp user (4 Kio)
0x05000 - 0x0FFFF Stack (44 Kio)
0x10000 - 0x17FFF Buffer A (32 Kio) (prefetch)
0x18000 - 0x1FFFF Buffer B (32 Kio) (prefetch)
*/

/*
template<typename ForwardIt>
static ForwardIt coalesce(ForwardIt begin, ForwardIt end)
{
    ForwardIt result{begin};
    ForwardIt last  {begin};

    while(++begin != end)
    {
        if(last->end >= begin->begin) //Overlaping or contiguous
        {
            last->end = begin->end;
        }
        else
        {
            *result++ = *last;
            last = begin;
        }
    }

    *result++ = *last;

    return result;
}
*/


static std::ostream& print(const std::vector<bool>& vec)
{
    for(auto value : vec)
    {
        std::cout << (value ? '1' : '0');
    }

    return std::cout;
};

static void print_scc(llvm::Function& function)
{
    std::cout << "SCCs for " << function.getName().str() << " in post-order:\n";

    for(auto[it, end] = std::pair{llvm::scc_begin(&function), llvm::scc_end(&function)}; it != end; ++it)
    {
         std::cout << "  SCC: ";
         for(llvm::BasicBlock* block : *it)
         {
             std::cout << ar::get_value_label(*block) << "  ";
         }
         std::cout << std::endl;
    }
}

static void print_post_order(llvm::Function& function)
{
    std::cout << "Basic blocks of " << function.getName().str() << " in post-order:\n";

    for(auto[it, end] = std::pair{llvm::po_begin(&function.getEntryBlock()), llvm::po_end(&function.getEntryBlock())}; it != end; ++it)
    {
         std::cout << ar::get_value_label(**it) << "  ";
    }
    std::cout << std::endl;
}

template<typename Range>
static std::ostream& print_values(Range&& range)
{
    for(llvm::Value* value : range)
    {
        std::cout << ar::get_value_label(*value) << " ";
    }

    return std::cout;
}


static void print_predecessors(llvm::Function& function)
{
    for(llvm::BasicBlock& block : function)
    {
        std::cout << "Predecessors of " << ar::get_value_label(block) << ": ";

        for(llvm::BasicBlock* pred : llvm::predecessors(&block))
        {
            std::cout << ar::get_value_label(*pred) << ", ";
        }

        std::cout << std::endl;
    }
}

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

///Code generation steps:
/// 1: Transformation of the IR for analysis:
///    Transforms the IR by adding, removing, or rewriting part or it,
///    in order to facilitate (or enable) further analysis and transformations
///
/// 2: Compliance pass:
///    Transforms the IR to make it compliant to Altair ASM:
///      - transforming impossible cmp
///      - add movei/loads for constants
///      - check immediate values
///      - ...
///
/// 3: Optimization pass:
///    Modifies the IR to increase performances of the future bytecode.
///
/// 4: Register allocation:
///    Performs register allocation to get rid of the SSA form.
///
/// 5: Translation
///    Translates the IR to Altair ASM
///

static void run(llvm::Module& module, const std::string& filename)
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
            return;
        }

        std::cout << function.getName().str() << ":" << std::endl;

        // Early transforms
        ar::early_transforms::swap_add_sub(module, function);
        ar::early_transforms::decompose_getelementptr(module, function);
        ar::early_transforms::optimize_load_store(module, function);
        ar::early_transforms::reorder_blocks(module, function);
        ar::early_transforms::invert_branch_condition(module, function);

        ar::register_allocator allocator{module, function};
        allocator.perform_analysis(); // initial analysis

        // Mid transforms
        ar::mid_transforms::insert_move_for_constant(allocator);
        ar::mid_transforms::insert_move_for_global_load(allocator);

        allocator.perform_register_allocation();

        // Late transforms

        const auto print_value = [&allocator](const llvm::Value& value)
        {
            auto&& info{allocator.info_of(&value)};

            std::cout << "    " << std::setw(4) << ar::get_value_label(*info.value) << " | ";
            std::cout << std::setw(4) << allocator.index_of(info) << " | ";
            std::cout << std::setw(8) << info.name << " | ";

            if(info.register_index == std::numeric_limits<std::size_t>::max())
            {
                std::cout << "void | ";
                std::cout << (info.leaf ? "    leaf" : "not leaf") << " | ";
            }
            else if(info.register_index == 64)
            {
                std::cout << "  BR | ";
                std::cout << "    leaf | ";
            }
            else
            {
                std::cout << std::setw(4) << info.register_index << " | ";
                std::cout << (info.leaf ? "    leaf" : "not leaf") << " | ";
            }

            for(auto&& range : info.lifetime)
            {
                std::cout << "[" <<  std::setw(3) << range.begin << "; " << std::setw(3) << range.end << "[ ";
            }

            std::cout << std::endl;
        };

        for(std::size_t i{}; i < function.arg_size(); ++i)
        {
            print_value(*allocator.values()[i].value);
        }

        for(auto&& block_info : allocator.blocks())
        {
            std::cout << "  " << block_info.name << std::endl;

            for(auto&& value : *block_info.block)
            {
                print_value(value);
            }
        }

        std::cout << "---" << std::endl;

        ar::function_translator translator{allocator};
        output_file << translator.translate() << std::endl;
    }
}

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <IR file>\n";
        return 1;
    }

    llvm::LLVMContext context{};
    llvm::SMDiagnostic err{};
    auto module{llvm::parseIRFile(argv[1], err, context)};

    if(!module)
    {
        std::cerr << err.getMessage().str() << std::endl;
        return 1;
    }

    std::cout << std::setfill(' ');

    std::string filename{argv[1]};
    filename.erase(std::find(std::begin(filename), std::end(filename), '.'), filename.end());

    using clock = std::chrono::steady_clock;
    const auto tp1{clock::now()};

    // Compile module!
    run(*module, filename);

    std::cout << "Compilation done in " << std::chrono::duration_cast<std::chrono::milliseconds>(clock::now() - tp1).count() << "ms" << std::endl;

    // Write back the module with all modification done by the compiler
    std::string output{};
    llvm::raw_string_ostream os{output};
    os << *module;
    os.flush();

    std::ofstream{filename + ".comp.ll"} << output;
}
