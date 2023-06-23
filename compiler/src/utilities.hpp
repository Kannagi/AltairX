#ifndef ALTAIR_COMPILER_UTILITIES_HPP_INCLUDED
#define ALTAIR_COMPILER_UTILITIES_HPP_INCLUDED

#include <string>
#include <cstdint>

#include <llvm/IR/Module.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Analysis/LoopInfo.h>

namespace ar
{

struct compiler_options
{
    std::string file{};
    bool verbose{};
    bool output_noop_instructions{};
    bool pretty_registers{};
    bool optimize{};
};

template<typename T>
constexpr bool has_single_bit(T x) noexcept
{
    return x != 0 && (x & (x - 1)) == 0;
}

template<typename T>
constexpr T ilog2(T n) noexcept
{
    return (n > 1) ? 1 + static_cast<T>(log2(n >> 1)) : 0;
}

constexpr std::size_t int_size(std::uint64_t value) noexcept
{
    if(value < (1ull << 8))
    {
        return 1;
    }
    else if(value < (1ull << 16))
    {
        return 2;
    }
    else if(value < (1ull << 32))
    {
        return 4;
    }
    else
    {
        return 8;
    }
}

std::string get_value_label(const llvm::Value& value);

std::string get_int_size_name(llvm::Value* value);
std::string get_int_size_name(std::size_t size);

/*
* For vectors of X components: returns "vector_<scalable_>X_<type_name(innertype)>"
* For arrays of X components: returns "array_X_<type_name(innertype)>"
* For non opaque pointer: returns "ptr_<type_name(pointedtype)>"
* For other: returns the llvm::Type::print text
*
* This function is useful to create intrinsics or output info in verbose mode
*/
std::string type_name(const llvm::Type& type);

bool is_external_call(const llvm::CallInst& call);
bool has_external_call(const llvm::Function& func);
bool has_external_call(const llvm::BasicBlock& block);
std::size_t get_last_external_call(const llvm::BasicBlock& block);

std::vector<llvm::BasicBlock*> loop_predecessors(const llvm::Loop& loop);
std::vector<llvm::BasicBlock*> loop_successors(const llvm::Loop& loop);

bool is_complex_branch(llvm::Function::iterator current, llvm::Function::iterator end);

}

#endif
