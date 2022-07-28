#ifndef ALTAIR_COMPILER_UTILITIES_HPP_INCLUDED
#define ALTAIR_COMPILER_UTILITIES_HPP_INCLUDED

#include <string>
#include <cstdint>

#include <llvm/IR/Module.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Analysis/LoopInfo.h>

namespace ar
{

std::string get_value_label(const llvm::Value& value);
std::string type_name(const llvm::Type& type);

bool is_external_call(const llvm::CallInst& call);
bool has_external_call(const llvm::Function& func);
bool has_external_call(const llvm::BasicBlock& block);
std::size_t get_last_external_call(const llvm::BasicBlock& block);

std::vector<llvm::BasicBlock*> loop_predecessors(const llvm::Loop& loop);
std::vector<llvm::BasicBlock*> loop_successors(const llvm::Loop& loop);

bool is_complex_branch(llvm::Function::iterator current);

}

#endif
