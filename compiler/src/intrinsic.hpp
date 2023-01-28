#ifndef ALTAIR_COMPILER_INTRINSIC_HPP_INCLUDED
#define ALTAIR_COMPILER_INTRINSIC_HPP_INCLUDED

#include <string>
#include <cstdint>

#include <llvm/IR/Module.h>
#include <llvm/IR/Instructions.h>

namespace ar
{

enum class intrinsic_id : std::uint32_t
{
    unknown, // Invalid instruction, try a real call
    moven,   // <T> %value = call altair.moven_<T>(<T> %value)
    moveu,   // <T> %value = call altair.moveu_<T>(<T> %value)
    smove,   // <T> %reg = call altair.smove_<T>_<U>_<V>(<T> %reg, <U> %value, <T> %shift)
    load,    // <T> %i = call altair.load_<T>_<U>(ptr %ptr, <U> %offset)
    store,   // call altair.store_<T>_<U>(ptr %ptr, <T> %value, <U> %offset)
    ptradd,  // ptr %i = call altair.ptradd_<T>(ptr %ptr, <T> %offset)
    spill,   // ptr %i = call altair.spill_<T>(<T> %value)
    fill,    // <T> %i = call altair.fill_<T>(ptr %ptr)
};

bool is_intrinsic(const llvm::Instruction& instruction);
intrinsic_id get_intrinsic_id(const llvm::Instruction& instruction);

//insert intrinsic before position
llvm::CallInst* insert_moven_intrinsic(llvm::Module& module, llvm::Value* value, llvm::Instruction* position);
llvm::CallInst* insert_moveu_intrinsic(llvm::Module& module, llvm::Value* value, llvm::Instruction* position);
llvm::CallInst* insert_smove_intrinsic(llvm::Module& module, llvm::Value* reg, llvm::Value* value, llvm::Value* shift, llvm::Instruction* position);
llvm::CallInst* insert_load_intrinsic(llvm::Module& module, llvm::Value* ptr, llvm::Value* offset, llvm::Type* type, llvm::Instruction* position);
llvm::CallInst* insert_store_intrinsic(llvm::Module& module, llvm::Value* ptr, llvm::Value* offset, llvm::Value* value, llvm::Instruction* position);
llvm::CallInst* insert_ptradd_intrinsic(llvm::Module& module, llvm::Value* ptr, llvm::Value* offset, llvm::Type* output_type, llvm::Instruction* position);
llvm::CallInst* insert_spill_intrinsic(llvm::Module& module, llvm::Value* value, llvm::Instruction* position);
llvm::CallInst* insert_fill_intrinsic(llvm::Module& module, llvm::Type* type, llvm::Value* ptr, llvm::Instruction* position);
}

#endif
