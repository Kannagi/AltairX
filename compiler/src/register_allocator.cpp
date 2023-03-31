#include "register_allocator.hpp"

#include <iostream>
#include <algorithm>
#include <numeric>

#include <llvm/ADT/SCCIterator.h>
#include <llvm/ADT/PostOrderIterator.h>

#include "intrinsic.hpp"
#include "utilities.hpp"

namespace ar
{

register_allocator::register_allocator(llvm::Module& module, llvm::Function& function)
:m_module{module}
,m_function{function}
{

}

void register_allocator::perform_analysis()
{
    m_tree = llvm::DominatorTree{m_function};
    m_loop_info = llvm::LoopInfo{m_tree};

    m_sccs.clear();
    m_blocks.clear();
    m_values.clear();
    m_loops.clear();
    m_groups.clear();

    extract_sccs();
    extract_blocks();
    extract_scc_edges();
    extract_loops();
    extract_values();
    extract_lifetimes();
    extract_groups();
}

void register_allocator::perform_register_allocation()
{
    compute_spill_weight();
    allocate_registers();
}

std::size_t register_allocator::get_scc(const llvm::BasicBlock* block)
{
    for(std::size_t index{}; index < std::size(m_sccs); ++index)
    {
        for(const llvm::BasicBlock* sccblock : m_sccs[index].blocks)
        {
            if(sccblock == block)
            {
                return index;
            }
        }
    }

    throw std::runtime_error{"Invalid AST"};
}

void register_allocator::extract_sccs()
{
    for(auto [it, end] = std::pair{llvm::scc_begin(&m_function), llvm::scc_end(&m_function)}; it != end; ++it)
    {
        auto& scc{m_sccs.emplace_back()};
        scc.blocks = std::move(*it);

        std::reverse(std::begin(scc.blocks), std::end(scc.blocks));
    }

    std::reverse(std::begin(m_sccs), std::end(m_sccs)); //By default the sccs are in inverse order (from last to first)

    std::size_t index{};
    for(auto& scc : m_sccs)
    {
        scc.begin = index;
        index += std::size(scc.blocks);
        scc.end = index;
    }
}

void register_allocator::extract_blocks()
{
    for(auto [it, end] = std::pair{llvm::po_begin(&m_function.getEntryBlock()), llvm::po_end(&m_function.getEntryBlock())}; it != end; ++it)
    {
        auto& block{m_blocks.emplace_back()};
        block.block = *it;
        block.name  = get_value_label(*block.block);
        block.scc   = get_scc(block.block);
    }

    std::reverse(std::begin(m_blocks), std::end(m_blocks));
}

void register_allocator::extract_scc_edges()
{
    for(const block_info& block : m_blocks)
    {
        scc_info& scc{m_sccs[block.scc]};

        for(llvm::BasicBlock* pred : llvm::predecessors(block.block))
        {
            if(info_of(pred).scc != block.scc)
            {
                scc.predecessors.emplace_back(pred);
            }
        }

        for(llvm::BasicBlock* succ : llvm::successors(block.block))
        {
            if(info_of(succ).scc != block.scc)
            {
                scc.successors.emplace_back(succ);
            }
        }
    }

    for(const scc_info& scc : m_sccs)
    {
        std::cout << "Predecessors of SCC " << get_value_label(*scc.blocks[0]) << ": ";
        for(llvm::BasicBlock* pred : scc.predecessors)
        {
            std::cout << get_value_label(*pred) << " ";
        }
        std::cout << std::endl;
    }

    for(const scc_info& scc : m_sccs)
    {
        std::cout << "Successors   of SCC " << get_value_label(*scc.blocks[0]) << ": ";
        for(llvm::BasicBlock* succ : scc.successors)
        {
            std::cout << get_value_label(*succ) << " ";
        }
        std::cout << std::endl;
    }
}

void register_allocator::extract_loops()
{
    store_loop_headers();

    for(llvm::Loop* loop : m_loop_info)
    {
        store_loop(loop);
    }

    for(const loop_info& loop : m_loops)
    {
        std::cout << "Predecessors of loop " << get_value_label(*loop.loop->getHeader()) << ": ";
        for(llvm::BasicBlock* pred : loop.predecessors)
        {
            std::cout << get_value_label(*pred) << " ";
        }
        std::cout << std::endl;
    }

    for(const loop_info& loop : m_loops)
    {
        std::cout << "Successors   of loop " << get_value_label(*loop.loop->getHeader()) << ": ";
        for(llvm::BasicBlock* succ : loop.successors)
        {
            std::cout << get_value_label(*succ) << " ";
        }
        std::cout << std::endl;
    }
}

void register_allocator::store_loop_headers()
{
    for(llvm::Loop* loop : m_loop_info)
    {
        store_loop_headers_recurse(loop);
    }
}

void register_allocator::store_loop_headers_recurse(llvm::Loop* loop)
{
    info_of(loop->getHeader()).loop = loop;

    llvm::SmallVector<llvm::Loop*, 32> inners{};
    loop->getInnerLoopsInPreorder(*loop, inners);

    for(llvm::Loop* inner : inners)
    {
        store_loop_headers_recurse(inner);
    }
}

void register_allocator::store_loop(llvm::Loop* loop)
{
    auto& info{m_loops.emplace_back()};
    info.loop = loop;
    info.predecessors = ar::loop_predecessors(*loop);
    info.successors = ar::loop_successors(*loop);

    const std::string ident(loop->getLoopDepth() * 4, ' ');
    std::cout << ident << "Loop " << ar::get_value_label(*loop->getHeader()) << std::endl;
    std::cout << ident << "  Blocks: ";
    for(auto block : loop->blocks())
    {
        std::cout << ar::get_value_label(*block) << " ";
    }
    std::cout << std::endl;

    llvm::SmallVector<llvm::Loop*, 32> inners{};
    loop->getInnerLoopsInPreorder(*loop, inners);

    for(llvm::Loop* inner : inners)
    {
        store_loop(inner);
    }
}

void register_allocator::extract_values()
{
    m_values.reserve(m_function.getInstructionCount() + m_function.arg_size());

    for(llvm::Argument& arg : m_function.args())
    {
        value_info& value{m_values.emplace_back()};
        value.value    = &arg;
        value.name     = get_value_label(arg);
        value.affinity = register_affinity::argument;
    }

    for(std::size_t i{}; i < std::size(m_blocks); ++i)
    {
        block_info& block{m_blocks[i]};
        block.begin = std::size(m_values);
        block.begin_no_phi = block.begin;

        for(llvm::Instruction& instruction : *block.block)
        {
            value_info& value{m_values.emplace_back()};
            value.value = &instruction;
            value.name  = instruction.getOpcodeName();
            value.block = i;

            if(llvm::isa<llvm::CmpInst>(instruction))
            {
                value.affinity = register_affinity::flags;
            }
            else if(llvm::isa<llvm::PHINode>(instruction))
            {
                ++block.begin_no_phi;
            }
        }

        block.end = std::size(m_values);
    }

    fill_affinity();
}

void register_allocator::fill_affinity()
{
    // Mark call arguments as arguments and set their required register
    for(std::size_t i{}; i < std::size(m_values); ++i)
    {
        if(auto ret{llvm::dyn_cast<llvm::ReturnInst>(m_values[i].value)}; ret)
        {
            if(llvm::Value* ret_value{ret->getReturnValue()}; ret_value)
            {
                if(const auto ret_index{index_of(ret_value)}; ret_index != std::size(m_values))
                {
                    auto& ret_info{m_values[ret_index]};
                    ret_info.affinity = register_affinity::ret;
                    ret_info.default_register = ret_register;
                }
            }
        }
        else if(auto cmp{llvm::dyn_cast<llvm::CmpInst>(m_values[i].value)}; cmp)
        {
            m_values[i].affinity = register_affinity::flags;
            m_values[i].default_register = flags_register;
        }
        else if(auto call{llvm::dyn_cast<llvm::CallInst>(m_values[i].value)}; call && !is_intrinsic(*call))
        {
            for(std::uint32_t i{}; i < call->arg_size(); ++i)
            {
                llvm::Value* arg{call->getArgOperand(i)};

                if(const auto arg_index{index_of(arg)}; arg_index != std::size(m_values))
                {
                    value_info& arg_info{m_values[arg_index]};

                    arg_info.affinity = register_affinity::argument;

                    if(i < 8) // arg need to be put in a register
                    {
                        arg_info.default_register = args_registers_begin + i;
                    }
                }
            }
        }
    }
}

void register_allocator::extract_lifetimes()
{
    const auto write_usage = [](value_info& value, std::size_t index)
    {
        value.usage[index] = true;
        value.first_use = std::min(index, value.first_use);
        value.last_use  = std::max(index, value.last_use);
    };

    for(std::size_t index{}; index < std::size(m_values); ++index)
    {
        value_info& value{m_values[index]};
        value.usage.resize(std::size(m_values));

        for(llvm::Value* user : value.value->users()) // register position of each user
        {
            write_usage(value, index_of(user));
        }

        auto instruction{llvm::dyn_cast<llvm::Instruction>(value.value)};
        if(index < m_function.arg_size() || (instruction && !instruction->getType()->isVoidTy()))
        {
            write_usage(value, index);
        }

        fill_lifetime(index);
    }
}

void register_allocator::fill_lifetime(size_t index)
{
    for(llvm::Value* user : m_values[index].value->users())
    {
        if(auto phi{llvm::dyn_cast<llvm::PHINode>(user)}; phi)
        {
            compute_phi_lifetime(index, phi);
        }
        else
        {
            compute_lifetime(index, user);
        }
    }
}

void register_allocator::compute_lifetime(std::size_t index, const llvm::Value* user)
{
    walk_to(index, index_of(user), [this](const walk_to_info& info) mutable
    {
        value_info& target{m_values[info.target]};
        block_info& block {m_blocks[info.block]};

        if(info.status == walk_to_status::local)
        {
            target.lifetime.add_range(info.target, info.start);
        }
        else if(info.status == walk_to_status::begin)
        {
            target.lifetime.add_range(block.begin, info.start);
        }
        else if(info.status == walk_to_status::step)
        {
            target.lifetime.add_range(block.begin, block.end);
        }
        else if(info.status == walk_to_status::end)
        {
            target.lifetime.add_range(info.target, block.end);
        }
    },
    [this](const walk_to_info& info) mutable
    {
        value_info& target{m_values[info.target]};
        block_info& block {m_blocks[info.block]};

        if(info.status == walk_to_status::local)
        {
            target.leaf = target.leaf && check_external_calls(info.target, info.start);
        }
        else if(info.status == walk_to_status::begin)
        {
            target.leaf = target.leaf && check_external_calls(block.begin, info.start);
        }
        else if(info.status == walk_to_status::step)
        {
            target.leaf = target.leaf && check_external_calls(block.begin, block.end);
        }
        else if(info.status == walk_to_status::end)
        {
            target.leaf = target.leaf && check_external_calls(info.target, block.end);
        }
    });
}

void register_allocator::compute_phi_lifetime(std::size_t index, const llvm::PHINode* phi)
{
    value_info& info{m_values[index]};

    for(const llvm::BasicBlock* pred : phi->blocks())
    {
        if(phi->getIncomingValueForBlock(pred) == info.value) // Find the pred that has the value
        {
            const block_info& pred_info{info_of(pred)};

            walk_to(index, index_of(pred_info.block->getTerminator()), [this](const walk_to_info& info) mutable
            {
                value_info& target{m_values[info.target]};
                block_info& block {m_blocks[info.block]};

                if(info.status == walk_to_status::local)
                {
                    target.lifetime.add_range(info.target, info.start + 1); // include terminator
                }
                else if(info.status == walk_to_status::begin)
                {
                    target.lifetime.add_range(block.begin, block.end);
                }
                else if(info.status == walk_to_status::step)
                {
                    target.lifetime.add_range(block.begin, block.end);
                }
                else if(info.status == walk_to_status::end)
                {
                    target.lifetime.add_range(info.target, block.end);
                }
            },
            [this](const walk_to_info& info) mutable
            {
                value_info& target{m_values[info.target]};
                block_info& block {m_blocks[info.block]};

                if(info.status == walk_to_status::local)
                {
                    target.leaf = target.leaf && check_external_calls(info.target, info.start + 1); // include terminator
                }
                else if(info.status == walk_to_status::begin)
                {
                    target.leaf = target.leaf && check_external_calls(block.begin, block.end);
                }
                else if(info.status == walk_to_status::step)
                {
                    target.leaf = target.leaf && check_external_calls(block.begin, block.end);
                }
                else if(info.status == walk_to_status::end)
                {
                    target.leaf = target.leaf && check_external_calls(info.target, block.end);
                }
            });
        }
    }
}

bool register_allocator::check_external_calls(std::size_t begin, std::size_t end) const
{
    for(std::size_t i{begin + 1}; i <= end; ++i)
    {
        if(auto call{llvm::dyn_cast<llvm::CallInst>(m_values[i].value)}; call)
        {
            return !is_external_call(*call);
        }
    }

    return true;
}

void register_allocator::extract_groups()
{
    compute_phi_groups();
    compute_smove_groups();
    compute_zext_trunc_groups();
    sync_groups_members();
}

void register_allocator::compute_phi_groups()
{
    for(llvm::BasicBlock& block : m_function)
    {
        for(llvm::PHINode& phi : block.phis())
        {
            const std::size_t group_index{std::size(m_groups)};
            group_info& group{m_groups.emplace_back()};

            for(llvm::Value* value : phi.incoming_values())
            {
                if(const auto value_index{index_of(value)}; value_index != std::size(m_values))
                {
                    m_values[value_index].group = group_index;
                    group.members.emplace_back(value);
                }
            }

            group.members.emplace_back(&phi);
        }
    }
}

void register_allocator::compute_smove_groups()
{
    for(llvm::BasicBlock& block : m_function)
    {
        for(llvm::Instruction& inst : block)
        {
            if(get_intrinsic_id(inst) == intrinsic_id::smove)
            {
                auto call{llvm::dyn_cast<llvm::CallInst>(&inst)};

                // if the register already has a group, keep it
                if(group_info* value_group{group_of(call->getArgOperand(0))}; value_group)
                {
                    value_group->members.emplace_back(call);
                }
                else
                {
                    const std::size_t group_index{std::size(m_groups)};

                    group_info& group{m_groups.emplace_back()};
                    group.members.emplace_back(call->getArgOperand(0));
                    group.members.emplace_back(call);

                    info_of(call->getArgOperand(0)).group = group_index;
                    info_of(call).group = group_index;
                }
            }
        }
    }
}

void register_allocator::compute_zext_trunc_groups()
{
    const auto process = [this](llvm::Instruction* inst)
    {
        // if the register already has a group, keep it
        if(group_info* value_group{group_of(inst->getOperand(0))}; value_group)
        {
            value_group->members.emplace_back(inst);
        }
        else
        {
            const std::size_t group_index{std::size(m_groups)};

            group_info& group{m_groups.emplace_back()};
            group.members.emplace_back(inst->getOperand(0));
            group.members.emplace_back(inst);

            info_of(inst->getOperand(0)).group = group_index;
            info_of(inst).group = group_index;
        }
    };

    for(llvm::BasicBlock& block : m_function)
    {
        for(llvm::Instruction& inst : block)
        {
            if(auto zext{llvm::dyn_cast<llvm::ZExtInst>(&inst)}; zext)
            {
                process(zext);
            }

            if(auto trunc{llvm::dyn_cast<llvm::TruncInst>(&inst)}; trunc)
            {
                process(trunc);
            }
        }
    }
}

void register_allocator::sync_groups_members()
{
    // if a member is non-leaf, no member can be since they need to share the same register
    for(group_info& group : m_groups)
    {
        for(llvm::Value* member : group.members)
        {
            if(!info_of(member).leaf)
            {
                group.leaf = false;
            }

            if(info_of(member).affinity == register_affinity::ret)
            {
                group.ret = true;
            }
        }
    }

    // The lifetime of the group is the coalescence of all members' lifetime
    for(group_info& group : m_groups)
    {
        for(llvm::Value* member : group.members)
        {
            value_info& info{info_of(member)};

            info.leaf = group.leaf;
            group.lifetime.coalesce(info.lifetime);
        }
    }

    for(std::size_t i{}; i < std::size(m_groups); ++i)
    {
        std::cout << "  Group #" << i << ": ";

        for(llvm::Value* member : m_groups[i].members)
        {
            std::cout << get_value_label(*member) << " ";
        }

        std::cout << " | " << m_groups[i].lifetime << " | ";
        std::cout << (m_groups[i].leaf ? "leaf" : "non leaf") << " | ";
        std::cout << (m_groups[i].ret ? "ret" : "non ret") << std::endl;
    }
}

void register_allocator::compute_spill_weight()
{
    /*
     * w = (def + use) * (block frec / entry frequency)
     * if loop: w *= 3
     * for all inst w += w
     * interval.w = w / size of liveinterval
     */

    for(value_info& info : m_values)
    {
        const bool loop{static_cast<bool>(m_sccs[m_blocks[info.block].scc].loop)};
        const double loop_factor{(loop ? 1.0 : 0.0) * 3.0};

        for(bool use : info.usage)
        {
            if(use)
            {
                info.spill_weight += 1.0 + loop_factor;
            }
        }
    }
}

/*
v0-v7   (8)  | functions arguments and return values (volatile)
v8-v19  (12) | general purpose (non-volatile)
v20-v60 (41) | general purpose (volatile)
v61-v63 (3)  | special registers (Accumulator, product, quotient)

Total volatile: 49

stack frame, all [] are optionnal:

 R0
 |
 v                                  CALLEE                                                          CALLER
{[V octets : locals][4 octets : caller addr][C octets : stack params]}{[V octets : locals][4 octets : caller addr][C octets : stack params]}
 ^                                                                                                                                         ^
 |                                                                                                                                         |
 X                                                                                                                                         Y


X < Y (the lesser R0 the bigger the stack = descending adresses)

[V octets : locals] is a stack by itself, and use the following structure:
    [C octets : spill area] space for register spilling
    [C octets : alloca area] space for statically sized stack allocations
    [V octets : malloca area] space for dynamically sized stack allocations
*/

void register_allocator::fill_register_info()
{
    m_registers[stack_register].type = register_type::special;
    m_registers[pointer_register].type = register_type::special;

    for(std::size_t i{args_registers_begin}; i < args_registers_end; ++i)
    {
        m_registers[i].type = register_type::generic_volatile;
    }

    for(std::size_t i{non_volatile_registers_begin}; i < non_volatile_registers_end; ++i)
    {
        m_registers[i].type = register_type::generic_non_volatile;
    }

    for(std::size_t i{volatile_registers_begin}; i < volatile_registers_end; ++i)
    {
        m_registers[i].type = register_type::generic_volatile;
    }

    m_registers[bypass_register].type = register_type::special;
    m_registers[accumulator_register].type = register_type::special;

    m_registers[flags_register].type = register_type::flags;
}

static bool allocation_priority_comparator(const register_allocator::value_info& left, const register_allocator::value_info& right)
{
    return (left.need_split && !right.need_split)
        && left.lifetime.total_span() < right.lifetime.total_span();
}

void register_allocator::generate_queue()
{
    m_queue.clear();
    m_queue.reserve(std::size(m_values));

    for(std::size_t i{m_function.arg_size()}; i < std::size(m_values); ++i)
    {
        if(!std::empty(m_values[i].lifetime) && m_values[i].affinity == register_affinity::generic)
        {
            m_queue.emplace_back(i);
        }
    }

    std::sort(std::begin(m_queue), std::end(m_queue), [this](std::size_t left, std::size_t right)
    {
        return allocation_priority_comparator(m_values[left], m_values[right]);
    });

    std::cout << "Queue: ";
    for(auto index : m_queue)
    {
        std::cout << get_value_label(*m_values[index].value) << ", ";
    }
    std::cout << std::endl;
}

void register_allocator::insert_queue(std::size_t value_index)
{
    const auto it = std::lower_bound(std::begin(m_queue), std::end(m_queue), value_index, [this](std::size_t right, std::size_t left)
    {
        return allocation_priority_comparator(m_values[right], m_values[left]);
    });

    m_queue.insert(it, value_index);
}

void register_allocator::allocate_registers()
{
    // Put the right register for arguments this function argument
    for(std::size_t i{}; i < std::min(m_function.arg_size(), std::size_t{8}); ++i)
    {
        if(m_values[i].leaf)
        {
            m_values[i].register_index = args_registers_begin + i;
            m_registers[m_values[i].register_index].lifetime.coalesce(m_values[i].lifetime);
        }
        else
        {
            //if a parameter is non-leaf then we need to move the parameter volatile register to a non-volatile one
            //or we need to spill/fill around calls
            //if the number of call is == 1 (call in loops count as 8 calls)
            //  - spill around the call
            //otherwise move (addi 0) it in a non volatile register
        }
    }

    //Other args are on the stack
    for(std::size_t i{8}; i < m_function.arg_size(); ++i)
    {
        //register stack entries
        //m_stack.emplace_back(m_function.getArg(i), m_function.getArg(i)->getType()->getScalarSizeInBits() / 8, m_values[i].lifetime);
        //insert load where needed
    }

    // assign the ret value to the ret register
    for(std::size_t i{}; i < std::size(m_values); ++i)
    {
        if(m_values[i].affinity == register_affinity::ret)
        {
            m_values[i].register_index = ret_register;
            m_values[i].spill_weight = unspillable;
            m_registers[m_values[i].register_index].lifetime.coalesce(m_values[i].lifetime);
        }
    }

    // Assign register for groups
    // Assume that there are enough registers to handle all groups lifetimes without overlaping
    // Assume phi group members are non overlapping
    // Leaf value must be synced between members of phi groups
    for(const group_info& group : m_groups)
    {
        if(group.ret)
        {
            for(llvm::Value* member : group.members)
            {
                auto& info{info_of(member)};

                info.register_index = ret_register;
                info.spill_weight = unspillable;
            }

            continue;
        }

        const std::size_t begin{group.leaf ? volatile_registers_begin : non_volatile_registers_begin};
        const std::size_t end  {group.leaf ? volatile_registers_end : non_volatile_registers_end};

        bool assigned{};
        for(std::size_t i{begin}; i < end; ++i)
        {
            if(!m_registers[i].lifetime.overlap(group.lifetime))
            {
                m_registers[i].lifetime.coalesce(group.lifetime);
                for(llvm::Value* member : group.members)
                {
                    auto& info{info_of(member)};

                    info.register_index = i;
                    info.spill_weight = unspillable;
                }

                assigned = true;
                break;
            }
        }

        if(!assigned)
        {
            throw std::runtime_error{"Group can not be assigned to any register"};
        }
    }

    generate_queue();

    while(!std::empty(m_queue))
    {
        const std::size_t value_index{m_queue.back()};
        m_queue.pop_back();

        auto& value{m_values[value_index]};
        auto instruction{llvm::dyn_cast<llvm::Instruction>(value.value)};

        if(value.register_index == std::numeric_limits<std::size_t>::max() && instruction && !instruction->getType()->isVoidTy())
        {
            allocate_register(value_index);
        }
    }
}

void register_allocator::allocate_register(std::size_t value_index)
{
    auto& value{m_values[value_index]};

    if(value.need_split)
    {
        assert(false && "Split is unsuported for now, please do not compile too complex algorithms, we have a lot of registers but not enough to build a web engine.");
        //find_best_split
    }

    if(value.affinity == register_affinity::ret)
    {
        assert(!m_registers[ret_register].lifetime.overlap(value.lifetime) && "Return value overlapping");

        m_registers[ret_register].lifetime.coalesce(value.lifetime);
        value.register_index = ret_register;
    }
    else if(value.affinity == register_affinity::flags)
    {
        assert(!m_registers[flags_register].lifetime.overlap(value.lifetime) && "The compiler don't reorder cmp yet");

        m_registers[flags_register].lifetime.coalesce(value.lifetime);
        value.register_index = flags_register;
    }
    else
    {
        if(auto reg = assing_register(value_index); reg != no_register)
        {
            value.register_index = reg;
        }
        else
        {
            std::size_t cheapest_index{no_register};
            for(std::size_t i{}; i < std::size(m_values); ++i)
            {
                if(m_values[i].spill_weight < value.spill_weight
                && (cheapest_index == no_register || m_values[i].spill_weight < m_values[cheapest_index].spill_weight))
                {
                    cheapest_index = i;
                }
            }

            if(cheapest_index == no_register)
            {
                value.need_split = true; //split
                insert_queue(value_index);
            }
            else
            {
                //evict
                auto& cheapest{m_values[cheapest_index]};
                m_registers[cheapest.register_index].lifetime.remove(cheapest.lifetime);
                m_registers[cheapest.register_index].lifetime.coalesce(value.lifetime);

                insert_queue(cheapest_index);
            }
        }
    }
}

std::size_t register_allocator::assing_register(std::size_t value_index)
{
    const value_info& value{m_values[value_index]};

    if(value.leaf)
    {
        // first try in the volatile generic registers
        for(std::size_t i{volatile_registers_begin}; i < volatile_registers_end; ++i)
        {
            if(!m_registers[i].lifetime.overlap(m_values[value_index].lifetime))
            {
                m_registers[i].lifetime.coalesce(m_values[value_index].lifetime);
                return i;
            }
        }

        // then try in the volatile args/ret registers
        for(std::size_t i{args_registers_begin}; i < args_registers_end; ++i)
        {
            if(!m_registers[i].lifetime.overlap(m_values[value_index].lifetime))
            {
                m_registers[i].lifetime.coalesce(m_values[value_index].lifetime);
                return i;
            }
        }

        // then try in the non volatile registers
        for(std::size_t i{non_volatile_registers_begin}; i < non_volatile_registers_end; ++i)
        {
            if(!m_registers[i].lifetime.overlap(m_values[value_index].lifetime))
            {
                m_registers[i].lifetime.coalesce(m_values[value_index].lifetime);
                return i;
            }
        }
    }
    else
    {
        for(std::size_t i{non_volatile_registers_begin}; i < non_volatile_registers_end; ++i) // try in the saved registers
        {
            if(!m_registers[i].lifetime.overlap(m_values[value_index].lifetime))
            {
                m_registers[i].lifetime.coalesce(m_values[value_index].lifetime);
                return i;
            }
        }
    }

    return no_register;
}

}
