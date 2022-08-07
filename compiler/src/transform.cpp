#include "transform.hpp"

#include <iostream>

#include <llvm/IR/Constants.h>
#include <llvm/ADT/PostOrderIterator.h>

#include "intrinsic.hpp"
#include "utilities.hpp"

namespace ar::transforms
{

void reorder_blocks(llvm::Module& module [[maybe_unused]], llvm::Function& function)
{
    std::vector<llvm::BasicBlock*> po_blocks;

    for(auto[it, end] = std::pair{llvm::po_begin(&function.getEntryBlock()), llvm::po_end(&function.getEntryBlock())}; it != end; ++it)
    {
        po_blocks.emplace_back(&(*(*it)));
    }

    std::reverse(std::begin(po_blocks), std::end(po_blocks));
    for(auto it{std::begin(po_blocks) + 1}; it != std::end(po_blocks); ++it)
    {
        (*it)->moveAfter(&(*(*(it - 1))));
    }
}

void swap_add_sub(llvm::Module& module [[maybe_unused]], llvm::Function& function)
{
    llvm::SmallVector<llvm::Instruction*> to_remove;

    for(llvm::BasicBlock& block : function)
    {
        for(llvm::Instruction& instruction : block)
        {
            llvm::BinaryOperator* binary{llvm::dyn_cast<llvm::BinaryOperator>(&instruction)};
            if(binary && (binary->getOpcode() == llvm::BinaryOperator::BinaryOps::Add || binary->getOpcode() == llvm::BinaryOperator::BinaryOps::Sub))
            {
                llvm::ConstantInt* value{llvm::dyn_cast<llvm::ConstantInt>(binary->getOperand(1))};
                if(value && value->isNegative() && value->getSExtValue() > -512)
                {
                    auto new_value{llvm::ConstantInt::get(binary->getType(), -value->getSExtValue())};

                    llvm::BinaryOperator* new_op{};
                    if(binary->getOpcode() == llvm::BinaryOperator::BinaryOps::Add)
                    {
                        new_op = llvm::BinaryOperator::CreateSub(binary->getOperand(0), new_value, "", binary);
                    }
                    else
                    {
                        new_op = llvm::BinaryOperator::CreateAdd(binary->getOperand(0), new_value, "", binary);
                    }

                    // copy users list since we modify it indirectly in the loop
                    for(llvm::User* user : llvm::SmallVector<llvm::User*>{instruction.user_begin(), instruction.user_end()})
                    {
                        user->replaceUsesOfWith(&instruction, new_op);
                    }

                    to_remove.emplace_back(&instruction);
                }
            }
        }
    }

    for(auto gep : to_remove)
    {
        gep->eraseFromParent();
    }
}

/*
size: imm
moveu:
    [0x00; 0x0003 FFFF]
moven:
    [0xFFFF FFFF FFFC 0000; 0xFFFF FFFF FFFF FFFF]
smove: NOTE: IT DOES A BITWISE OR WITH THE BASE VALUE ALREADY IN REGISTER
    [0x0000 0000 0000 0000; 0x0000 0000 0000 FFFF]
    [0x0000 0000 0001 0000; 0x0000 0000 FFFF 0000]
    [0x0000 0001 0000 0000; 0x0000 FFFF 0000 0000]
    [0x0001 0000 0000 0000; 0xFFFF 0000 0000 0000]
load ?
 */


// `is_signed` is for the `size` of the imm
static llvm::Value* insert_constant_int(llvm::Module& module, llvm::ConstantInt* value, std::size_t size, bool is_signed, llvm::Instruction* position)
{
    const auto compose_int = [&](std::uint64_t val)
    {
        auto low_word{llvm::ConstantInt::get(llvm::Type::getInt64Ty(module.getContext()), val & 0xFFFFull)};

        llvm::Instruction* last{insert_moveu_intrinsic(module, low_word, position)};

        for(std::uint64_t i{1}; i < 4; ++i)
        {
            const std::uint64_t bit_shift{16 * i};

            if(const std::uint64_t current{(val & (0xFFFFull << bit_shift)) >> bit_shift}; current != 0)
            {
                auto word {llvm::ConstantInt::get(llvm::Type::getInt64Ty(module.getContext()), current)};
                auto shift{llvm::ConstantInt::get(llvm::Type::getInt64Ty(module.getContext()), i)};

                last = insert_smove_intrinsic(module, last, word, shift, position);
            }
        }

        return last;
    };

    constexpr std::uint64_t moveu_max{(1ull << 18) - 1};
    constexpr std::int64_t  moven_min{-(1ll << 18)};
    // op codes are 32-bit so it will always fit
    const std::uint64_t inst_max{(1ull << (size - static_cast<std::int64_t>(is_signed))) - 1};
    const std::int64_t  inst_min{is_signed ? -static_cast<std::int64_t>(inst_max) - 1 : 0ll};

    if(value->isNegative())
    {
        const std::int64_t val{value->getSExtValue()};

        if(val >= inst_min && size > 0) //fit in size: do nothing
        {
            return value;
        }
        else if(val >= moven_min) // fit in moves
        {
            return insert_moven_intrinsic(module, value, position);
        }
        else
        {
            return compose_int(static_cast<std::uint64_t>(val));
        }
    }
    else
    {
        const std::uint64_t val{value->getZExtValue()};

        if(val <= inst_max && size > 0) //fit in size: do nothing
        {
            return value;
        }
        else if(val <= moveu_max) // fit in moveu
        {
            return insert_moveu_intrinsic(module, value, position);
        }
        else
        {
            return compose_int(val);
        }
    }
}

void insert_move_for_constant(llvm::Module& module, llvm::Function& function)
{
    for(llvm::BasicBlock& block : function)
    {
        for(llvm::Instruction& instruction : block)
        {
            //filter intrinsics to prevent altair.move(iX X) from generating infinite loops

            if(auto phi{llvm::dyn_cast<llvm::PHINode>(&instruction)}; phi)
            {
                // Insert move in predecessor if the phi value when comming from this predecessor is a contant

                for(llvm::BasicBlock* predecessor : phi->blocks())
                {
                    if(llvm::ConstantInt* value{llvm::dyn_cast<llvm::ConstantInt>(phi->getIncomingValueForBlock(predecessor))}; value)
                    {
                        phi->setIncomingValueForBlock(predecessor, insert_constant_int(module, value, 0, false, predecessor->getTerminator()));
                    }

                    //add float, bool and pointer support
                }
            }
            else if(llvm::CallInst* call{llvm::dyn_cast<llvm::CallInst>(&instruction)}; call)
            {
                switch(get_intrinsic_id(*call))
                {
                case intrinsic_id::unknown:
                    for(std::uint32_t i{}; i < call->arg_size(); ++i)
                    {
                        if(llvm::ConstantInt* value{llvm::dyn_cast<llvm::ConstantInt>(call->getArgOperand(i))}; value)
                        {
                            call->setArgOperand(i, insert_constant_int(module, value, 0, false, call));
                        }
                    }
                    break;

                case intrinsic_id::load:
                    if(llvm::ConstantInt* value{llvm::dyn_cast<llvm::ConstantInt>(call->getArgOperand(1))}; value)
                    {
                        call->setArgOperand(1, insert_constant_int(module, value, 10, true, call));
                    }
                    break;

                case intrinsic_id::store:
                    if(llvm::ConstantInt* value{llvm::dyn_cast<llvm::ConstantInt>(call->getArgOperand(1))}; value)
                    {
                        call->setArgOperand(1, insert_constant_int(module, value, 10, true, call));
                    }
                    if(llvm::ConstantInt* value{llvm::dyn_cast<llvm::ConstantInt>(call->getArgOperand(2))}; value)
                    {
                        call->setArgOperand(2, insert_constant_int(module, value, 0, false, call));
                    }
                    break;

                case intrinsic_id::moven:  break;
                case intrinsic_id::moveu:  break;
                case intrinsic_id::smove:  break;
                case intrinsic_id::ptradd: break;
                case intrinsic_id::spill:  break;
                case intrinsic_id::fill:   break;
                }
            }
            else if(llvm::BinaryOperator* binary{llvm::dyn_cast<llvm::BinaryOperator>(&instruction)}; binary)
            {
                if(llvm::ConstantInt* value{llvm::dyn_cast<llvm::ConstantInt>(binary->getOperand(0))}; value)
                {
                    binary->setOperand(0, insert_constant_int(module, value, 0, false, binary));
                }

                if(llvm::ConstantInt* value{llvm::dyn_cast<llvm::ConstantInt>(binary->getOperand(1))}; value)
                {
                    switch(binary->getOpcode())
                    {
                    case llvm::BinaryOperator::BinaryOps::SDiv: [[fallthrough]];
                    case llvm::BinaryOperator::BinaryOps::SRem:
                        binary->setOperand(1, insert_constant_int(module, value, 9, true, binary));
                        break;
                    default:
                        binary->setOperand(1, insert_constant_int(module, value, 9, false, binary));
                        break;
                    }
                }
            }
            else if(llvm::StoreInst* store{llvm::dyn_cast<llvm::StoreInst>(&instruction)}; store)
            {
                if(llvm::ConstantInt* value{llvm::dyn_cast<llvm::ConstantInt>(store->getOperand(0))}; value)
                {
                    store->setOperand(0, insert_constant_int(module, value, 0, false, store));
                }
            }
            else if(llvm::ReturnInst* ret{llvm::dyn_cast<llvm::ReturnInst>(&instruction)}; ret)
            {
                if(ret->getReturnValue())
                {
                    if(llvm::ConstantInt* value{llvm::dyn_cast<llvm::ConstantInt>(ret->getReturnValue())}; value)
                    {
                        ret->setOperand(0, insert_constant_int(module, value, 0, false, ret));
                    }
                }
            }
       }
    }
}

void invert_branch_condition(llvm::Module& module [[maybe_unused]], llvm::Function& function)
{
    const auto get_inverse = [](llvm::CmpInst::Predicate predicate)
    {
        switch(predicate)
        {
        case llvm::CmpInst::Predicate::ICMP_EQ : return llvm::CmpInst::Predicate::ICMP_NE;
        case llvm::CmpInst::Predicate::ICMP_NE : return llvm::CmpInst::Predicate::ICMP_EQ;
        case llvm::CmpInst::Predicate::ICMP_UGT: return llvm::CmpInst::Predicate::ICMP_ULE;
        case llvm::CmpInst::Predicate::ICMP_UGE: return llvm::CmpInst::Predicate::ICMP_ULT;
        case llvm::CmpInst::Predicate::ICMP_ULT: return llvm::CmpInst::Predicate::ICMP_UGE;
        case llvm::CmpInst::Predicate::ICMP_ULE: return llvm::CmpInst::Predicate::ICMP_UGT;
        case llvm::CmpInst::Predicate::ICMP_SGT: return llvm::CmpInst::Predicate::ICMP_SLE;
        case llvm::CmpInst::Predicate::ICMP_SGE: return llvm::CmpInst::Predicate::ICMP_SLT;
        case llvm::CmpInst::Predicate::ICMP_SLT: return llvm::CmpInst::Predicate::ICMP_SGE;
        case llvm::CmpInst::Predicate::ICMP_SLE: return llvm::CmpInst::Predicate::ICMP_SGT;
        default:
            throw std::runtime_error{"Bad or unsuported comparison predicate " + std::to_string(predicate)};
        }
    };

    auto end{function.end()};
    --end; // function can not be empty (they have at least 1 block)
    for(auto it{function.begin()}; it != end; ++it)
    {
        auto branch{llvm::dyn_cast<llvm::BranchInst>(it->getTerminator())};
        if(branch && branch->isConditional())
        {
            auto next_it{it};
            ++next_it;
            const auto& next_block{*next_it};

            // check if true path leads to the following block
            // if true, then invert the condition and swap the branch
            if(branch->getSuccessor(0) == &next_block)
            {
                if(auto compare{llvm::dyn_cast<llvm::CmpInst>(branch->getCondition())}; compare)
                {
                    compare->setPredicate(get_inverse(compare->getPredicate()));

                    auto true_path{branch->getSuccessor(0)};
                    auto false_path{branch->getSuccessor(1)};

                    branch->setSuccessor(0, false_path);
                    branch->setSuccessor(1, true_path);
                }
            }
        }
    }
}

static llvm::BinaryOperator* insert_const_mul(llvm::Module& module, llvm::Value* left, std::uint64_t right, llvm::Instruction* position)
{
    if(has_single_bit(right)) // if size is a pow of two then use a bit shift instead of a multiplication
    {
        auto size{llvm::ConstantInt::get(llvm::Type::getInt64Ty(module.getContext()), ilog2(right))};
        return llvm::BinaryOperator::CreateShl(left, size, "", position);
    }

    auto size{llvm::ConstantInt::get(llvm::Type::getInt64Ty(module.getContext()), right)};
    return llvm::BinaryOperator::CreateMul(left, size, "", position);
};

static void decompose_getelementptr_process(llvm::Module& module, llvm::GetElementPtrInst* gep)
{
    const llvm::DataLayout& data_layout{module.getDataLayout()}; //used for struct

    bool first_index{true}; // first index is always an array like index
    std::int64_t constant_offset{}; // accumulator of contant indices

    llvm::Type* type{gep->getSourceElementType()}; // currently indexed type
    llvm::Value* ptr{gep->getPointerOperand()}; // the ptr used for next ptradd

    const auto flush_constant = [&]()
    {
        if(constant_offset != 0)
        {
            auto offset{llvm::ConstantInt::get(llvm::Type::getInt64Ty(module.getContext()), constant_offset, true)};

            ptr = ar::insert_ptradd_intrinsic(module, ptr, offset, llvm::PointerType::getUnqual(module.getContext()), gep);
            constant_offset = 0;
        }
    };

    for(llvm::Use& index : gep->indices())
    {
        // Contant indices can be added together can just need to be split from dynamic indices
        if(auto cint{llvm::dyn_cast<llvm::ConstantInt>(index.get())}; cint)
        {
            if(auto struct_type{llvm::dyn_cast<llvm::StructType>(type)}; struct_type)
            {
                const llvm::StructLayout* struct_layout{data_layout.getStructLayout(struct_type)};

                if(first_index) // when indexing structs, the first index is an array-like index
                {
                    constant_offset += struct_layout->getSizeInBytes() * cint->getSExtValue();
                }
                else
                {
                    constant_offset += struct_layout->getElementOffset(static_cast<std::uint32_t>(cint->getZExtValue()));
                    type = struct_type->getStructElementType(static_cast<std::uint32_t>(cint->getZExtValue()));
                }
            }
            else // array like index
            {
                if(auto array_type{llvm::dyn_cast<llvm::ArrayType>(type)}; array_type)
                {
                    type = array_type->getElementType();
                }
                else if(auto vector_type{llvm::dyn_cast<llvm::VectorType>(type)}; vector_type)
                {
                    type = vector_type->getElementType();
                }

                constant_offset += data_layout.getTypeStoreSize(type) * cint->getSExtValue();
            }
        }
        else
        {
            // Split from previous contants, if any
            flush_constant();

            if(auto array_type{llvm::dyn_cast<llvm::ArrayType>(type)}; array_type)
            {
                type = array_type->getElementType();
            }
            else if(auto vector_type{llvm::dyn_cast<llvm::VectorType>(type)}; vector_type)
            {
                type = vector_type->getElementType();
            }

            // dynamic indices are always array-like access, so we simply use type size (padding included for structs)
            auto offset{insert_const_mul(module, index.get(), data_layout.getTypeStoreSize(type), gep)};
            auto ptradd{ar::insert_ptradd_intrinsic(module, ptr, offset, llvm::PointerType::getUnqual(module.getContext()), gep)};

            ptr = ptradd;
        }

        first_index = false;
    }

    flush_constant();

    // copy users list since we modify it indirectly in the loop
    for(llvm::User* user : llvm::SmallVector<llvm::User*>{gep->user_begin(), gep->user_end()})
    {
        user->replaceUsesOfWith(gep, ptr);
    }
}

void decompose_getelementptr(llvm::Module& module, llvm::Function& function)
{
    llvm::SmallVector<llvm::Instruction*> to_remove;

    for(llvm::BasicBlock& block : function)
    {
        for(llvm::Instruction& instruction : block)
        {
            if(auto gep{llvm::dyn_cast<llvm::GetElementPtrInst>(&instruction)}; gep)
            {
                decompose_getelementptr_process(module, gep);
                to_remove.emplace_back(gep);
            }
        }
    }

    for(auto gep : to_remove)
    {
        gep->eraseFromParent();
    }
}

void optimize_load_store(llvm::Module& module, llvm::Function& function)
{
    std::vector<llvm::Instruction*> to_remove{};

    const auto remove_if_useless = [&](llvm::CallInst* call)
    {
        bool can_remove{true};
        for(llvm::User* user : call->users())
        {
            if(const auto instruction{llvm::dyn_cast<llvm::Instruction>(user)}; instruction)
            {
                const auto id{get_intrinsic_id(*instruction)};
                if(!llvm::isa<llvm::LoadInst>(user) && !llvm::isa<llvm::StoreInst>(user) && id != intrinsic_id::load && id != intrinsic_id::store)
                {
                    can_remove = false;
                    break;
                }
            }
        }

        if(can_remove)
        {
            to_remove.emplace_back(call);
        }
    };

    for(llvm::BasicBlock& block : function)
    {
        for(llvm::Instruction& instruction : block)
        {
            if(auto load{llvm::dyn_cast<llvm::LoadInst>(&instruction)}; load)
            {
                if(auto ptradd{llvm::dyn_cast<llvm::CallInst>(load->getPointerOperand())}; ptradd && get_intrinsic_id(*ptradd) == intrinsic_id::ptradd)
                {
                    auto altair_load{insert_load_intrinsic(module, ptradd->getOperand(0), ptradd->getArgOperand(1), load->getType(), load)};

                    for(llvm::User* user : llvm::SmallVector<llvm::User*>{load->user_begin(), load->user_end()})
                    {
                        user->replaceUsesOfWith(load, altair_load);
                    }

                    to_remove.emplace_back(load);
                    remove_if_useless(ptradd);
                }
            }
            else if(auto store{llvm::dyn_cast<llvm::StoreInst>(&instruction)}; store)
            {
                if(auto ptradd{llvm::dyn_cast<llvm::CallInst>(store->getPointerOperand())}; ptradd && get_intrinsic_id(*ptradd) == intrinsic_id::ptradd)
                {
                    insert_store_intrinsic(module, ptradd->getOperand(0), ptradd->getArgOperand(1), store->getValueOperand(), store);

                    to_remove.emplace_back(store);
                    remove_if_useless(ptradd);
                }
            }
        }
    }

    std::sort(std::begin(to_remove), std::end(to_remove));
    to_remove.erase(std::unique(std::begin(to_remove), std::end(to_remove)), to_remove.end());

    //sort removed entries to ensure users are deleted before the used value
    //that's a pretty heavy algorithm but it works
    for(auto prev{std::begin(to_remove)}; prev != std::end(to_remove); ++prev)
    {
        for(auto next{prev}; next != std::end(to_remove); ++next)
        {
            if(std::find((*prev)->user_begin(), (*prev)->user_end(), *next) != (*prev)->user_end())
            {
                std::iter_swap(prev, next);
            }
        }
    }

    for(llvm::Instruction* instruction : to_remove)
    {
        instruction->eraseFromParent();
    }
}

}
