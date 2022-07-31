#include "intrinsic.hpp"

#include "utilities.hpp"

namespace ar
{

bool is_intrinsic(llvm::Instruction& instruction)
{
    if(auto call{llvm::dyn_cast<llvm::CallInst>(&instruction)}; call)
    {
        if(auto callee{call->getCalledFunction()}; callee)
        {
            return callee->getName().startswith("altair.");
        }
    }

    return false;
}

intrinsic_id get_intrinsic_id(llvm::Instruction& instruction)
{
    if(auto call{llvm::dyn_cast<llvm::CallInst>(&instruction)}; call)
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
    auto func_type {llvm::FunctionType::get(value_type, llvm::ArrayRef<llvm::Type*>{value_type}, false)};
    auto function  {module.getOrInsertFunction("altair.moven_" + type_name(*value_type), func_type)};

    return llvm::CallInst::Create(func_type, function.getCallee(), llvm::ArrayRef<llvm::Value*>{value}, "", position);
}

llvm::CallInst* insert_moveu_intrinsic(llvm::Module& module, llvm::Value* value, llvm::Instruction* position)
{
    auto value_type{value->getType()};
    auto func_type {llvm::FunctionType::get(value_type, llvm::ArrayRef<llvm::Type*>{value_type}, false)};
    auto function  {module.getOrInsertFunction("altair.moveu_" + type_name(*value_type), func_type)};

    return llvm::CallInst::Create(func_type, function.getCallee(), llvm::ArrayRef<llvm::Value*>{value}, "", position);
}

llvm::CallInst* insert_smove_intrinsic(llvm::Module& module, llvm::Value* reg, llvm::Value* value, llvm::Value* shift, llvm::Instruction* position)
{
    auto reg_type  {reg->getType()};
    auto value_type{value->getType()};
    auto shift_type{value->getType()};
    auto func_type {llvm::FunctionType::get(value_type, llvm::ArrayRef<llvm::Type*>{reg_type, value_type, shift_type}, false)};
    auto function  {module.getOrInsertFunction("altair.smove_" + type_name(*reg_type) + "_" + type_name(*value_type) + "_" + type_name(*shift_type), func_type)};

    return llvm::CallInst::Create(func_type, function.getCallee(), llvm::ArrayRef<llvm::Value*>{reg, value, shift}, "", position);
}

llvm::CallInst* insert_spill_intrinsic(llvm::Module& module, llvm::Value* value, llvm::Instruction* position)
{
    auto value_type{value->getType()};
    auto func_type {llvm::FunctionType::get(value_type, llvm::ArrayRef<llvm::Type*>{value_type}, false)};
    auto function  {module.getOrInsertFunction("altair.spill_" + type_name(*value_type), func_type)};

    return llvm::CallInst::Create(func_type, function.getCallee(), llvm::ArrayRef<llvm::Value*>{value}, "", position);
}

llvm::CallInst* insert_fill_intrinsic(llvm::Module& module, llvm::Value* value, llvm::Instruction* position)
{
    auto value_type{value->getType()};
    auto func_type {llvm::FunctionType::get(value_type, llvm::ArrayRef<llvm::Type*>{value_type}, false)};
    auto function  {module.getOrInsertFunction("altair.fill_" + type_name(*value_type), func_type)};

    return llvm::CallInst::Create(func_type, function.getCallee(), llvm::ArrayRef<llvm::Value*>{value}, "", position);
}

llvm::CallInst* insert_ptradd_intrinsic(llvm::Module& module, llvm::Value* ptr, llvm::Value* offset, llvm::Type* output_type, llvm::Instruction* position)
{
    auto ptr_type   {ptr->getType()};
    auto offset_type{offset->getType()};

    auto func_type    {llvm::FunctionType::get(output_type, llvm::ArrayRef<llvm::Type*>{ptr_type, offset_type}, false)};
    auto function_name{"altair.ptradd_" + type_name(*offset_type)};
    auto function     {module.getOrInsertFunction(function_name, func_type)};

    return llvm::CallInst::Create(func_type, function.getCallee(), llvm::ArrayRef<llvm::Value*>{ptr, offset}, "", position);
}

}
