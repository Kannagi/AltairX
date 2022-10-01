#include "intrinsic.hpp"

#include "utilities.hpp"

#include <llvm/IR/Constants.h>

namespace ar
{

bool is_intrinsic(const llvm::Instruction& instruction)
{
    if(auto call{llvm::dyn_cast<const llvm::CallInst>(&instruction)}; call)
    {
        if(auto callee{call->getCalledFunction()}; callee)
        {
            return callee->getName().startswith("altair.");
        }
    }

    return false;
}

intrinsic_id get_intrinsic_id(const llvm::Instruction& instruction)
{
    if(auto call{llvm::dyn_cast<const llvm::CallInst>(&instruction)}; call)
    {
        if(auto callee{call->getCalledFunction()}; callee)
        {
            const auto name{callee->getName()};

            if(name.startswith("altair.moven"))
            {
                return intrinsic_id::moven;
            }
            else if(name.startswith("altair.moveu"))
            {
                return intrinsic_id::moveu;
            }
            else if(name.startswith("altair.smove"))
            {
                return intrinsic_id::smove;
            }
            else if(name.startswith("altair.load"))
            {
                return intrinsic_id::load;
            }
            else if(name.startswith("altair.store"))
            {
                return intrinsic_id::store;
            }
            else if(name.startswith("altair.spill"))
            {
                return intrinsic_id::spill;
            }
            else if(name.startswith("altair.fill"))
            {
                return intrinsic_id::fill;
            }
            else if(name.startswith("altair.ptradd"))
            {
                return intrinsic_id::ptradd;
            }
        }
    }

    return intrinsic_id::unknown;
}

llvm::CallInst* insert_moven_intrinsic(llvm::Module& module, llvm::Value* value, llvm::Instruction* position)
{
    auto value_type{value->getType()};
    auto func_type {llvm::FunctionType::get(value_type, {value_type}, false)};
    auto function  {module.getOrInsertFunction("altair.moven_" + type_name(*value_type), func_type)};

    return llvm::CallInst::Create(func_type, function.getCallee(), {value}, "", position);
}

llvm::CallInst* insert_moveu_intrinsic(llvm::Module& module, llvm::Value* value, llvm::Instruction* position)
{
    auto value_type{value->getType()};
    auto func_type {llvm::FunctionType::get(value_type, {value_type}, false)};
    auto function  {module.getOrInsertFunction("altair.moveu_" + type_name(*value_type), func_type)};

    return llvm::CallInst::Create(func_type, function.getCallee(), {value}, "", position);
}

llvm::CallInst* insert_smove_intrinsic(llvm::Module& module, llvm::Value* reg, llvm::Value* value, llvm::Value* shift, llvm::Instruction* position)
{
    auto reg_type  {reg->getType()};
    auto value_type{value->getType()};
    auto shift_type{shift->getType()};
    auto func_type {llvm::FunctionType::get(value_type, {reg_type, value_type, shift_type}, false)};
    auto function  {module.getOrInsertFunction("altair.smove_" + type_name(*reg_type) + "_" + type_name(*value_type) + "_" + type_name(*shift_type), func_type)};

    return llvm::CallInst::Create(func_type, function.getCallee(), {reg, value, shift}, "", position);
}

llvm::CallInst* insert_load_intrinsic(llvm::Module& module, llvm::Value* ptr, llvm::Value* offset, llvm::Type* type, llvm::Instruction* position)
{
    auto ptr_type   {ptr->getType()};
    auto offset_type{offset->getType()};

    auto func_type    {llvm::FunctionType::get(type, {ptr_type, offset_type}, false)};
    auto function_name{"altair.load_" + type_name(*type) + "_" + type_name(*offset_type)};
    auto function     {module.getOrInsertFunction(function_name, func_type)};

    return llvm::CallInst::Create(func_type, function.getCallee(), {ptr, offset}, "", position);
}

llvm::CallInst* insert_store_intrinsic(llvm::Module& module, llvm::Value* ptr, llvm::Value* offset, llvm::Value* value, llvm::Instruction* position)
{
    auto ptr_type   {ptr->getType()};
    auto offset_type{offset->getType()};
    auto value_type {value->getType()};

    auto func_type    {llvm::FunctionType::get(llvm::Type::getVoidTy(module.getContext()), {ptr_type, offset_type, value_type}, false)};
    auto function_name{"altair.store_" + type_name(*offset_type) + "_" + type_name(*value_type)};
    auto function     {module.getOrInsertFunction(function_name, func_type)};

    return llvm::CallInst::Create(func_type, function.getCallee(), {ptr, offset, value}, "", position);
}

llvm::CallInst* insert_ptradd_intrinsic(llvm::Module& module, llvm::Value* ptr, llvm::Value* offset, llvm::Type* output_type, llvm::Instruction* position)
{
    auto ptr_type   {ptr->getType()};
    auto offset_type{offset->getType()};

    auto func_type    {llvm::FunctionType::get(output_type, {ptr_type, offset_type}, false)};
    auto function_name{"altair.ptradd_" + type_name(*offset_type)};
    auto function     {module.getOrInsertFunction(function_name, func_type)};

    return llvm::CallInst::Create(func_type, function.getCallee(), {ptr, offset}, "", position);
}

llvm::CallInst* insert_spill_intrinsic(llvm::Module& module, llvm::Value* value, llvm::Instruction* position)
{
    auto value_type{value->getType()};
    auto func_type {llvm::FunctionType::get(llvm::PointerType::get(module.getContext(), 0), {value_type}, false)};
    auto function  {module.getOrInsertFunction("altair.spill_" + type_name(*value_type), func_type)};

    return llvm::CallInst::Create(func_type, function.getCallee(), {value}, "", position);
}

llvm::CallInst* insert_fill_intrinsic(llvm::Module& module, llvm::Type* type, llvm::Value* ptr, llvm::Instruction* position)
{
    auto ptr_type{ptr->getType()};

    auto func_type {llvm::FunctionType::get(type, llvm::ArrayRef<llvm::Type*>{ptr_type}, false)};
    auto function  {module.getOrInsertFunction("altair.fill_" + type_name(*type), func_type)};

    return llvm::CallInst::Create(func_type, function.getCallee(), {ptr}, "", position);
}

}
