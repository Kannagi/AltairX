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
    unknown,
    copy,    // <T> %value = call.altair.copy_<T>(<T> %value)
    moven,   // <T> %value = call altair.moven_<T>(<T> %value)
    moveu,   // <T> %value = call altair.moveu_<T>(<T> %value)
    smove,   // <T> %reg = call altair.smove_<T>_<U>_<V>(<T> %reg, <U> %value, <T> %shift)
    getri,   // <T> %value = call altair.getri_<T>(<T> %index)
    setri,   // <T> %value = call altair.setri_<T>_<U>(<T> %index, <U> %value)
    load,    // <T> %i = call altair.load_<T>_<U>(ptr %ptr, <U> %offset)
    store,   // call altair.store_<T>_<U>(ptr %ptr, <T> %value, <U> %offset)
    ptradd,  // ptr %i = call altair.ptradd_<T>(ptr %ptr, <T> %offset)
    spill,   // ptr %i = call altair.spill_<T>(<T> %value)
    fill,    // <T> %i = call altair.fill_<T>(ptr %ptr)
    push,    // call altair.push_<T>(<T> %bytes)
    pop,     // call altair.pop_<T>(<T> %bytes)
};

bool is_intrinsic(const llvm::Instruction& instruction);
intrinsic_id get_intrinsic_id(const llvm::Instruction& instruction);
std::string get_intrinsic_name(const llvm::Instruction& instruction);

//insert intrinsic before position
llvm::CallInst* insert_copy_intrinsic(llvm::Module& module, llvm::Value* value, llvm::Instruction* position = nullptr);
llvm::CallInst* insert_moven_intrinsic(llvm::Module& module, llvm::Value* value, llvm::Instruction* position = nullptr);
llvm::CallInst* insert_moveu_intrinsic(llvm::Module& module, llvm::Value* value, llvm::Instruction* position = nullptr);
llvm::CallInst* insert_smove_intrinsic(llvm::Module& module, llvm::Value* reg, llvm::Value* value, llvm::Value* shift, llvm::Instruction* position = nullptr);
llvm::CallInst* insert_getri_intrinsic(llvm::Module& module, llvm::Value* index, llvm::Instruction* position = nullptr);
llvm::CallInst* insert_setri_intrinsic(llvm::Module& module, llvm::Value* index, llvm::Value* value, llvm::Instruction* position = nullptr);
llvm::CallInst* insert_load_intrinsic(llvm::Module& module, llvm::Value* ptr, llvm::Value* offset, llvm::Type* type, llvm::Instruction* position = nullptr);
llvm::CallInst* insert_store_intrinsic(llvm::Module& module, llvm::Value* ptr, llvm::Value* offset, llvm::Value* value, llvm::Instruction* position = nullptr);
llvm::CallInst* insert_ptradd_intrinsic(llvm::Module& module, llvm::Value* ptr, llvm::Value* offset, llvm::Type* output_type, llvm::Instruction* position = nullptr);
llvm::CallInst* insert_spill_intrinsic(llvm::Module& module, llvm::Value* value, llvm::Instruction* position = nullptr);
llvm::CallInst* insert_fill_intrinsic(llvm::Module& module, llvm::Type* type, llvm::Value* ptr, llvm::Instruction* position = nullptr);
llvm::CallInst* insert_push_intrinsic(llvm::Module& module, llvm::Value* bytes, llvm::Instruction* position = nullptr);
llvm::CallInst* insert_pop_intrinsic(llvm::Module& module, llvm::Value* bytes, llvm::Instruction* position = nullptr);

}

#endif
