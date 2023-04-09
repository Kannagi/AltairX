#include "mid_transforms.hpp"

#include <iostream>

#include <llvm/IR/Constants.h>
#include <llvm/ADT/PostOrderIterator.h>

#include "intrinsic.hpp"
#include "utilities.hpp"

namespace ar::mid_transforms
{

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

void insert_move_for_constant(register_allocator& allocator)
{
    auto& module{allocator.module()};

    for(llvm::BasicBlock& block : allocator.function())
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

                default:
                    break;
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

void insert_move_for_global_load(register_allocator& allocator)
{
    auto& module{allocator.module()};
    auto& function{allocator.function()};

    std::unordered_map<llvm::Value*, llvm::SmallVector<llvm::Instruction*>> constant_users;

    for(llvm::GlobalVariable& global : module.globals())
    {
        for(llvm::User* user : global.users())
        {
            if(allocator.index_of(user) != register_allocator::invalid_index)
            {
                auto it{constant_users.find(&global)};
                if(it == std::end(constant_users))
                {
                    it = constant_users.emplace(&global, llvm::SmallVector<llvm::Instruction*>{}).first;
                }

                it->second.emplace_back(llvm::dyn_cast<llvm::Instruction>(user));
            }
        }
    }

    // For now the algorithm just extract constant use out of loops
    // IMPROVEMENT: composing a constant address take two ALU cycle which could be done on channel two
    // or just pipelined enough to have no impact on performances. This will lesser register pressure.
    for(auto& constant_user : constant_users)
    {
        std::size_t first_user_index{std::numeric_limits<std::size_t>::max()};

        for(auto user : constant_user.second)
        {
            const auto user_index{allocator.index_of(user)};
            assert(user_index != register_allocator::invalid_index && "Broken logic, all users must already have been checked as members of this function.");
            first_user_index = std::min(first_user_index, user_index);
        }

        auto& first_user {allocator.values()[first_user_index]};
        auto& first_block{allocator.blocks()[first_user.block]};
        auto& first_scc  {allocator.sccs()[first_block.scc]};

        const auto insert_address = [&module, &constant_user](llvm::Instruction* position)
        {
            llvm::Instruction* low{insert_moveu_intrinsic(module, constant_user.first, position)};
            auto shift{llvm::ConstantInt::get(llvm::Type::getInt64Ty(module.getContext()), 1)};

            return insert_smove_intrinsic(module, low, constant_user.first, shift, position);
        };

        llvm::Value* constant{};

        if(first_scc.loop) // first use is in a loop, extract it out of it
        {
            auto& loop{allocator.info_of(first_scc.loop)};

            if(loop.predecessors.size() == 1)
            {
                llvm::BasicBlock* pred{loop.predecessors.front()};
                constant = insert_address(pred->getTerminator());
            }
            else
            {
                // IMPROVEMENT: for now constant is placed in first block to ensure that multiple entries loop
                // will always have the right value.
                // Assume entry block is never in a loop
                constant = insert_address(function.getEntryBlock().getTerminator());
            }
        }
        else // insert constant before first user
        {
            constant = insert_address(llvm::cast<llvm::Instruction>(first_user.value));
        }

        for(llvm::Instruction* user : constant_user.second)
        {
            user->replaceUsesOfWith(constant_user.first, constant);
        }
    }

    allocator.perform_analysis();
}

}
