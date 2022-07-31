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
    moven,
    moveu,
    smove,
    spill,
    fill,
    ptradd
};

bool is_intrinsic(llvm::Instruction& instruction);
intrinsic_id get_intrinsic_id(llvm::Instruction& instruction);

//insert intrinsic before position
llvm::CallInst* insert_moven_intrinsic(llvm::Module& module, llvm::Value* value, llvm::Instruction* position);
llvm::CallInst* insert_moveu_intrinsic(llvm::Module& module, llvm::Value* value, llvm::Instruction* position);
llvm::CallInst* insert_smove_intrinsic(llvm::Module& module, llvm::Value* reg, llvm::Value* value, llvm::Value* shift, llvm::Instruction* position);
llvm::CallInst* insert_spill_intrinsic(llvm::Module& module, llvm::Value* value, llvm::Instruction* position);
llvm::CallInst* insert_fill_intrinsic(llvm::Module& module, llvm::Value* value, llvm::Instruction* position);
llvm::CallInst* insert_ptradd_intrinsic(llvm::Module& module, llvm::Value* ptr, llvm::Value* offset, llvm::Type* output_type, llvm::Instruction* position);

}

#endif
