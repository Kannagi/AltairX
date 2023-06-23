#include "mid_transforms.hpp"

#include <iostream>
#include <algorithm>
#include <fstream>
#include <numeric>

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

void insert_move_for_constant(function_analyser& analyser)
{
    auto& module{analyser.module()};

    for(llvm::BasicBlock& block : analyser.function())
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
            else if(auto call{llvm::dyn_cast<llvm::CallInst>(&instruction)}; call)
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
            else if(auto binary{llvm::dyn_cast<llvm::BinaryOperator>(&instruction)}; binary)
            {
                if(auto value{llvm::dyn_cast<llvm::ConstantInt>(binary->getOperand(0))}; value)
                {
                    binary->setOperand(0, insert_constant_int(module, value, 0, false, binary));
                }

                if(auto value{llvm::dyn_cast<llvm::ConstantInt>(binary->getOperand(1))}; value)
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
            else if(auto store{llvm::dyn_cast<llvm::StoreInst>(&instruction)}; store)
            {
                if(llvm::ConstantInt* value{llvm::dyn_cast<llvm::ConstantInt>(store->getOperand(0))}; value)
                {
                    store->setOperand(0, insert_constant_int(module, value, 0, false, store));
                }
            }
            else if(auto ret{llvm::dyn_cast<llvm::ReturnInst>(&instruction)}; ret)
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

    analyser.perform_analysis();
}

void insert_move_for_global_load(function_analyser& analyser)
{
    auto& module{analyser.module()};
    auto& function{analyser.function()};

    std::unordered_map<llvm::Value*, llvm::SmallVector<llvm::Instruction*>> constant_users;

    for(llvm::GlobalVariable& global : module.globals())
    {
        for(llvm::User* user : global.users())
        {
            if(analyser.index_of(user) != invalid_index)
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
            const auto user_index{analyser.index_of(user)};
            assert(user_index != invalid_index && "Broken logic, all users must already have been checked as members of this function.");
            first_user_index = std::min(first_user_index, user_index);
        }

        auto& first_user {analyser.values()[first_user_index]};
        auto& first_block{analyser.blocks()[first_user.block]};
        auto& first_scc  {analyser.sccs()[first_block.scc]};

        const auto insert_address = [&module, &constant_user](llvm::Instruction* position)
        {
            llvm::Instruction* low{insert_moveu_intrinsic(module, constant_user.first, position)};
            auto shift{llvm::ConstantInt::get(llvm::Type::getInt64Ty(module.getContext()), 1)};

            return insert_smove_intrinsic(module, low, constant_user.first, shift, position);
        };

        llvm::Value* constant{};

        if(first_scc.loop) // first use is in a loop, extract it out of it
        {
            auto& loop{analyser.info_of(first_scc.loop)};

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

    analyser.perform_analysis();
}

void fix_conflicting_phi_nodes(function_analyser& analyser)
{
    auto& module{analyser.module()};
    auto& function{analyser.function()};

    bool changed{};

    for(auto& group : analyser.groups())
    {
        if(group.type != group_type::phi)
        {
            continue;
        }

        for(std::size_t i{1}; i < group.members.size(); ++i)
        {
            for(std::size_t j{i + 1}; j < group.members.size(); ++j)
            {
                const auto& left {analyser.info_of(group.members[i])};
                const auto& right{analyser.info_of(group.members[j])};

                if(!left.lifetime.overlap(right.lifetime))
                {
                    continue; // ok
                }

                // if there is an overlap one value must be stored elsewhere and restored later
                // foreach phi nodes in group
                //     if phi uses right
                //         

                auto* phi{llvm::cast<llvm::PHINode>(group.members[0])};

                const auto& phi_info {analyser.info_of(phi)};
                const auto& phi_block{analyser.blocks()[phi_info.block]};

                // insert block before phi_block
                auto* copy_block{llvm::BasicBlock::Create(module.getContext(), "", &function, phi_block.block)};
                // br to phi_block
                auto* copy_branch{llvm::BranchInst::Create(phi_block.block, copy_block)};
                // make a copy of right
                auto* copy{insert_copy_intrinsic(module, right.value, copy_branch)};

                // change phi incomming [right, right_block] to [copy, copy_block]
                llvm::SmallVector<llvm::BasicBlock*> incoming_blocks{};
                for(std::uint32_t inc{}; inc < phi->getNumIncomingValues(); ++inc)
                {
                    if(phi->getIncomingValue(inc) == right.value)
                    {
                        incoming_blocks.emplace_back(phi->getIncomingBlock(static_cast<std::uint32_t>(i)));
                    }
                }

                for(auto* incoming_block : incoming_blocks)
                {
                    phi->removeIncomingValue(incoming_block);
                    // change right_block br to phi_block to copy_block
                    incoming_block->getTerminator()->replaceSuccessorWith(phi_block.block, copy_block);
                }

                phi->addIncoming(copy, copy_block);
                changed = true;
            }
        }
    }

    if(changed)
    {
        analyser.perform_analysis();
    }
}

void resolve_conflicting_affinities(function_analyser& analyser)
{
    auto& module{analyser.module()};

    for(auto& value : analyser.values())
    {
        const auto& first{value.affinities[0]};
        
        for(std::size_t i{}; i < value.affinities.size(); ++i)
        {
            const auto& second{value.affinities[i]};

            if(first.affinity == register_affinity::argument)
            {
                if(second.affinity == register_affinity::ret && first.default_register != 0)
                {
                    for(llvm::User* user : value.value->users())
                    {
                        if(auto* ret{llvm::dyn_cast<llvm::ReturnInst>(user)}; ret)
                        {
                            ret->setOperand(0, insert_copy_intrinsic(module, value.value, ret));
                        }
                    }
                }
                else if(second.affinity == register_affinity::callee_argument && first.default_register != second.default_register)
                {
                    for(llvm::User* user : value.value->users())
                    {
                        if(auto* call{llvm::dyn_cast<llvm::CallInst>(user)}; call)
                        {
                            if(call->getNumOperands() > second.default_register && call->getOperand(second.default_register) == value.value)
                            {
                                call->setOperand(second.default_register, ar::insert_copy_intrinsic(module, value.value, call));
                            }
                        }
                    }
                }
            }
            else if(first.affinity == register_affinity::callee_ret)
            {
                if(second.affinity == register_affinity::callee_argument && second.default_register != 0)
                {
                    for(llvm::User* user : value.value->users())
                    {
                        if(auto* call{llvm::dyn_cast<llvm::CallInst>(user)}; call)
                        {
                            if(call->getNumOperands() > second.default_register && call->getOperand(second.default_register) == value.value)
                            {
                                call->setOperand(second.default_register, ar::insert_copy_intrinsic(module, value.value, call));
                            }
                        }
                    }
                }
            }
            else if(first.affinity == register_affinity::flags)
            {
                // TODO, generate code to convert flags to bools
            }
        }
    }

    analyser.perform_analysis();
}

static std::size_t cycles_of(llvm::Instruction* inst)
{
    if(llvm::isa<llvm::TruncInst>(inst) || llvm::isa<llvm::ZExtInst>(inst))
    {
        return 0;
    }

    return 1;
}

static std::size_t latency_of(llvm::Instruction* inst)
{
    if(llvm::isa<llvm::TruncInst>(inst) || llvm::isa<llvm::ZExtInst>(inst))
    {
        return 0;
    }

    return 3;
}

static std::vector<std::size_t> optimize_range(function_analyser& analyser, const block_info& block, std::size_t begin, std::size_t end)
{
    /*
    look for instruction that depends on "external values"
        external values : a value that is defined out of the range
        example : func args, phi nodes, values defined before a side effect etc
        these instructions are DAG head nodes
    */

    const auto range_size{end - begin};

    struct vertex_info
    {
        std::size_t latency{}; // latency of this instruction, decremented when assigning an instruction to output
        std::size_t cycles{}; // number of cycle consumed by this instruction
        std::size_t in_degree{};
        bool visited{};
    };

    const auto to_dag = [begin](std::size_t index)
    {
        return index - begin;
    };

    const auto from_dag = [begin](std::size_t index)
    {
        return begin + index;
    };

    std::vector<vertex_info> vertices{};
    vertices.resize(range_size);
    std::vector<llvm::SmallVector<std::size_t, 8>> dag{};
    dag.resize(range_size);

    const auto push_dag = [&](std::size_t from, std::size_t to)
    {
        dag[to_dag(from)].emplace_back(to_dag(to));
        vertices[to_dag(to)].in_degree += 1;
    };

    // construct DAG
    // In the DAG, a dependency "FROM x TO y" means that y depends on x
    // Example:
    // %23 = shl nuw nsw i32 %22, 12
    // %24 = or i32 %23, %16
    // In this example, a dependency "FROM %23 to %24" will be defined
    for(std::size_t value_index{begin}; value_index < end; ++value_index)
    {
        const auto& value_info{analyser.values()[value_index]};
        auto* inst{llvm::cast<llvm::Instruction>(value_info.value)};
        vertices[to_dag(value_index)].latency = latency_of(inst);
        vertices[to_dag(value_index)].cycles = cycles_of(inst);

        for(const llvm::Use& op : inst->operands())
        {
            const auto op_index{analyser.index_of(op.get())};
            if(op_index == invalid_index) // constants...
            {
                // IMPROVEMENT: still store useful informations if any ?
                continue;
            }

            // an operand is a phi node value:
            // we add a dependency of any phi node incomming values than are 
            // part of current leaf range
            // Example:
            // %8 = phi i64[%53, %6], [0, %3]
            // %9 = getelementptr inbounds i8, ptr %0, i64 %8
            // %53 = add nuw i64 %8, 4
            // will add a dependency from %9 to %53 to ensure that the addition is not performed 
            // before the getelementptr
            if(llvm::isa<llvm::PHINode>(op))
            {
                auto& op_info{analyser.values()[op_index]};
                if(op_info.group != value_info.group)
                {
                    auto& group{analyser.groups()[op_info.group]};
                    if(group.type == group_type::phi)
                    {
                        for(llvm::Value* member : group.members)
                        {
                            const auto member_index{analyser.index_of(member)};
                            if(member_index == invalid_index)
                            {
                                continue;
                            }

                            if(value_index <= member_index && member_index < end)
                            {
                                // IMPROVEMENT: note that latency should not apply to this dependency ?
                                push_dag(value_index, member_index);
                            }
                        }
                    }
                }
            }

            // TODO: Add guarantees for cmp and block terminators

            // Operand is in range
            // We add a dependency from the operand to the current instruction
            if(begin <= op_index && op_index < end)
            {
                push_dag(op_index, value_index);
            }
        }
    }

    // new_order[i] = analyser.values() index
    std::vector<std::size_t> new_order{};
    new_order.reserve(range_size);
    // this is the queue with all the candite that can be place to the list
    std::vector<std::size_t> queue{};
    queue.reserve(range_size);

    // Initial roots
    for(std::size_t i{}; i < range_size; i++) 
    {
        if(vertices[i].in_degree == 0) 
        {
            // root have default to 0 latency
            // IMPROVEMENT: make more advanced analysis to enable more accurate latency for root vertices
            vertices[i].latency = 0;
            queue.emplace_back(i);
        }
    }

    const auto find_best_candidate = [&queue, &vertices]()
    {
        const auto it = std::min_element(std::begin(queue), std::end(queue), [&vertices](std::size_t vertex, std::size_t smallest)
        {
            if(vertices[vertex].latency < vertices[smallest].latency)
            {
                return true;
            }

            return vertices[vertex].cycles < vertices[smallest].cycles;
        });

        // more advanced checks ?
        const auto output{*it};
        queue.erase(it);

        return output;
    };

    while(!std::empty(queue))
    {
        // choose best candidate in queue
        const auto best_candidate{find_best_candidate()};
        new_order.emplace_back(begin + best_candidate);
        vertices[best_candidate].visited = true;
        const auto cycles{vertices[best_candidate].cycles};

        for(auto dependee : dag[best_candidate])
        {
            vertices[dependee].in_degree -= 1;
            if(vertices[dependee].in_degree == 0)
            {
                queue.push_back(dependee);
            }
        }

        for(auto& vertex : vertices)
        {
            if(!vertex.visited && vertex.latency > 0)
            {
                vertex.latency -= std::min(cycles, vertex.latency);
            }
        }
    }

    return new_order;
}

void optimize_pipeline(function_analyser& analyser)
{
    std::vector<std::size_t> new_order{};

    for(auto& block : analyser.blocks())
    {
        new_order.clear();
        new_order.reserve(block.end - block.begin_no_phi);

        std::size_t leaf_begin{block.begin_no_phi};
        for(std::size_t i{block.begin_no_phi}; i < block.end; ++i)
        {
            if(!analyser.values()[i].leaf)
            {
                if(i - leaf_begin > 1)
                {
                    auto range_new_order{optimize_range(analyser, block, leaf_begin, i)};
                    new_order.insert(std::end(new_order), std::begin(range_new_order), std::end(range_new_order));
                }

                new_order.emplace_back(i);

                leaf_begin = i + 1;
            }
        }

        if(block.end - 1 - leaf_begin > 1)
        {
            // exclude terminator
            auto range_new_order{optimize_range(analyser, block, leaf_begin, block.end - 1)};
            new_order.insert(std::end(new_order), std::begin(range_new_order), std::end(range_new_order));
        }
        else
        {
            new_order.emplace_back(block.end - 2);
        }

        const auto old_index_of = [&new_order, begin = block.begin_no_phi](std::size_t new_index)
        {
            const auto it{std::find(std::begin(new_order), std::end(new_order), new_index)};
            assert(it != std::end(new_order));
        
            return begin + std::distance(std::begin(new_order), it);
        };

        llvm::Instruction* position{block.block->getTerminator()};
        const auto index_offset{block.end - 1};
        for(std::size_t i{1}; i < std::size(new_order); ++i)
        {
            const auto old_index{new_order[index_offset - block.begin_no_phi - i]};
            auto current{llvm::cast<llvm::Instruction>(analyser.values()[old_index].value)};
            current->moveBefore(position);
            position = current;
        }
    }

    analyser.perform_analysis();
}

}
