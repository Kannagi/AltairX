#include "function_analyser.hpp"

#include <iostream>
#include <algorithm>
#include <numeric>

#include <llvm/ADT/SCCIterator.h>
#include <llvm/ADT/PostOrderIterator.h>
#include <llvm/IR/Constants.h>

#include "intrinsic.hpp"
#include "utilities.hpp"

namespace ar
{

function_analyser::function_analyser(llvm::Module& module, llvm::Function& function)
    :m_module{module}
    ,m_function{function}
{

}

void function_analyser::perform_analysis()
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
    extract_groups();
}

namespace print_helper
{

struct indent_t
{
    std::size_t level{};
};

static indent_t indent(std::size_t level)
{
    return indent_t{level};
}

static std::ostream& operator<<(std::ostream& os, indent_t level)
{
    os << "| ";
    for(std::size_t i{}; i < level.level; ++i)
    {
        std::cout << "  ";
    }

    return os;
};

}

void function_analyser::print() const
{
    using namespace print_helper;

    std::cout << std::setfill(' ');
    std::cout << m_function.getName().str() << ":" << '\n';

    for(std::size_t i{}; i < std::size(m_sccs); ++i)
    {
        const auto& scc{m_sccs[i]};

        std::cout << indent(0) << "SCC #" << i << " starting at " << get_value_label(*scc.blocks[0]) << ":" << '\n';
        std::cout << indent(1) << "Predecessors: ";
        for(llvm::BasicBlock* pred : scc.predecessors)
        {
            std::cout << get_value_label(*pred) << " ";
        }
        std::cout << '\n';

        std::cout << indent(1) << "Successors:";
        for(llvm::BasicBlock* succ : scc.successors)
        {
            std::cout << get_value_label(*succ) << " ";
        }
        std::cout << '\n';
    }

    for(std::size_t i{}; i < std::size(m_loops); ++i)
    {
        const auto& loop{m_loops[i]};

        std::cout << indent(0) << "Loop " << ar::get_value_label(*loop.loop->getHeader()) << '\n';
        std::cout << indent(1) << "Blocks: ";
        for(auto block : loop.loop->blocks())
        {
            std::cout << ar::get_value_label(*block) << " ";
        }
        std::cout << '\n';

        std::cout << indent(1) << "Predecessors: ";
        for(llvm::BasicBlock* pred : loop.predecessors)
        {
            std::cout << get_value_label(*pred) << " ";
        }
        std::cout << '\n';

        std::cout << indent(1) << "Successors  : ";
        for(llvm::BasicBlock* succ : loop.successors)
        {
            std::cout << get_value_label(*succ) << " ";
        }
        std::cout << '\n';
    }

    for(std::size_t i{}; i < std::size(m_groups); ++i)
    {
        std::cout << indent(0) << "Group #" << i << ": ";

        std::cout << (m_groups[i].leaf ? "    leaf" : "non leaf") << " | ";
        std::cout << (m_groups[i].ret ? "    ret" : "non ret") << " | ";
        std::cout << m_groups[i].lifetime << " | ";

        for(llvm::Value* member : m_groups[i].members)
        {
            std::cout << get_value_label(*member) << " ";
        }

        std::cout << '\n';
    }

    const auto print_value = [this](const value_info& info)
    {
        std::cout << indent(1) << std::setw(4) << ar::get_value_label(*info.value) << " | ";
        std::cout << std::setw(4) << index_of(info) << " | ";
        std::cout << std::setw(8) << info.name << " | ";

        if(info.alloc.register_index != no_register)
        {
            if(info.alloc.register_index == 64)
            {
                std::cout << "BR | ";
                std::cout << "    leaf | ";
            }
            else
            {
                std::cout << std::setw(2) << info.alloc.register_index << " | ";
                std::cout << (info.leaf ? "    leaf" : "not leaf") << " | ";
            }

            for(auto&& range : info.lifetime)
            {
                std::cout << "[" << std::setw(3) << range.begin << "; " << std::setw(3) << range.end << "[ ";
            }

            if(std::empty(info.lifetime))
            {
                std::cout << std::setw(10) << ' ';
            }

            std::cout << "| ";

            for(auto&& affinity : info.affinities)
            {
                std::cout << "(" << to_string(affinity.affinity) << ": " << affinity.default_register << ") ";
            }
        }

        std::cout << '\n';
    };

    std::cout << indent(0) << "Args: \n";
    for(std::size_t i{}; i < m_function.arg_size(); ++i)
    {
        print_value(m_values[i]);
    }

    for(const auto& block_info : m_blocks)
    {
        std::cout << indent(0) << block_info.name << '\n';

        for(std::size_t i{block_info.begin}; i < block_info.end; ++i)
        {
            print_value(m_values[i]);
        }
    }

    std::cout << "-" << std::endl;

}

std::size_t function_analyser::get_scc(const llvm::BasicBlock* block)
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

void function_analyser::extract_sccs()
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

void function_analyser::extract_blocks()
{
    for(auto [it, end] = std::pair{llvm::po_begin(&m_function.getEntryBlock()), llvm::po_end(&m_function.getEntryBlock())}; it != end; ++it)
    {
        auto& block{m_blocks.emplace_back()};
        block.block = *it;
        block.name = get_value_label(*block.block);
        block.scc = get_scc(block.block);
    }

    std::reverse(std::begin(m_blocks), std::end(m_blocks));
}

void function_analyser::extract_scc_edges()
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
}

void function_analyser::extract_loops()
{
    store_loop_headers();

    for(llvm::Loop* loop : m_loop_info)
    {
        store_loop(loop);
    }

    for(auto& scc : m_sccs)
    {
        scc.loop = m_blocks[scc.begin].loop;
    }
}

void function_analyser::store_loop_headers()
{
    for(llvm::Loop* loop : m_loop_info)
    {
        store_loop_headers_recurse(loop);
    }
}

void function_analyser::store_loop_headers_recurse(llvm::Loop* loop)
{
    info_of(loop->getHeader()).loop = loop;

    llvm::SmallVector<llvm::Loop*, 32> inners{};
    loop->getInnerLoopsInPreorder(*loop, inners);

    for(llvm::Loop* inner : inners)
    {
        store_loop_headers_recurse(inner);
    }
}

void function_analyser::store_loop(llvm::Loop* loop)
{
    auto& info{m_loops.emplace_back()};
    info.loop = loop;
    info.predecessors = ar::loop_predecessors(*loop);
    info.successors = ar::loop_successors(*loop);

    llvm::SmallVector<llvm::Loop*, 32> inners{};
    loop->getInnerLoopsInPreorder(*loop, inners);

    for(llvm::Loop* inner : inners)
    {
        store_loop(inner);
    }
}

void function_analyser::extract_values()
{
    m_values.reserve(m_function.getInstructionCount() + m_function.arg_size());

    for(std::uint32_t i{}; i < m_function.arg_size(); ++i)
    {
        value_info& info{m_values.emplace_back()};
        info.value = m_function.getArg(i);
        info.name = get_value_label(*info.value);
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
            value.block = i;

            if(is_intrinsic(instruction)) // give altair intrinsics real names
            {
                value.name = "." + get_intrinsic_name(instruction);
            }
            else
            {
                value.name = instruction.getOpcodeName();
            }

            if(llvm::isa<llvm::PHINode>(instruction))
            {
                ++block.begin_no_phi;
            }
        }

        block.end = std::size(m_values);
    }

    for(std::size_t i{}; i < std::size(m_values); ++i)
    {
        fill_affinity(i);
    }

    for(std::size_t i{}; i < std::size(m_values); ++i)
    {
        reduce_affinities(i);
    }

    for(std::size_t i{}; i < std::size(m_values); ++i)
    {
        fill_lifetime(i);
    }
}

void function_analyser::fill_affinity(std::size_t index)
{
    auto& info{m_values[index]};

    const auto push_affinity = [&info](register_affinity affinity, std::uint32_t index = 0)
    {
        info.affinities.push_back(value_register_affinity{affinity, index});
    };

    if(auto cmp{llvm::dyn_cast<llvm::CmpInst>(info.value)}; cmp)
    {
        push_affinity(register_affinity::flags);
    }
    else if(auto call{llvm::dyn_cast<llvm::CallInst>(info.value)}; call)
    {
        if(get_intrinsic_id(*call) == intrinsic_id::spill)
        {
            push_affinity(register_affinity::spill);
        }
        else if(is_intrinsic(*call)) // other instrinsic result are considered generic
        {
            push_affinity(register_affinity::generic);
        }
        else if(!call->getType()->isVoidTy())// "normal" function call
        {
            push_affinity(register_affinity::callee_ret);
        }
    }
    else if(index < m_function.arg_size()) // argument
    {
        push_affinity(register_affinity::argument, static_cast<std::uint32_t>(index));
    }
    else if(info.value->getType()->isVoidTy())
    {
        push_affinity(register_affinity::none);
    }
    else
    {
        push_affinity(register_affinity::generic);
    }

    for(llvm::User* user : info.value->users()) // check if this value is used somewhere that need an affinity
    {
        if(llvm::isa<llvm::ReturnInst>(user))
        {
            push_affinity(register_affinity::ret);
        }
        else if(auto call{llvm::dyn_cast<llvm::CallInst>(user)}; call && !is_intrinsic(*call))
        {
            for(std::uint32_t i{}; i < call->arg_size(); ++i) // find usage indices
            {
                if(call->getArgOperand(i) == info.value)
                {
                    push_affinity(register_affinity::callee_argument, i);
                }
            }
        }
    }
}

void function_analyser::reduce_affinities(std::size_t index)
{
    // There is 2 types of affinities:
    // - One that is assigned at value definition, the main affinity: generic, none, argument, callee_ret, spill, flags
    // - One that is assigned at value usages, the second affinities: ret, callee_argument
    // Possible combinations is stricly limited, due to algorithm implementation, to:
    // - [generic|none|argument|callee_ret|spill|flags]
    // - (ret) + (callee_argument...)

    auto& value{m_values[index]};

    if(value.affinities.size() == 1)
    {
        return;
    }

    const auto affinity_order = [](register_affinity left)
    {
        switch(left)
        {
        default:
            return 0;
        case register_affinity::ret:
            return 1;
        case register_affinity::callee_argument:
            return 2;
        }
    };

    std::sort(std::begin(value.affinities), std::end(value.affinities), [affinity_order](const auto& left, const auto& right)
    {
        if(affinity_order(left.affinity) == affinity_order(right.affinity))
        {
            return left.default_register < right.default_register;
        }

        return affinity_order(left.affinity) < affinity_order(right.affinity);
    });

    const auto comp = [](const auto& left, const auto& right)
    {
        return left.affinity == right.affinity && left.default_register == right.default_register;
    };

    value.affinities.erase(std::unique(std::begin(value.affinities), std::end(value.affinities), comp), std::end(value.affinities));

    if(value.affinities.size() == 2 && value.affinities[0].affinity == register_affinity::generic)
    {
        value.affinities[0] = value.affinities[1];
        value.affinities.resize(1);
    }
}

void function_analyser::fill_lifetime(std::size_t index)
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

    m_info.leaf = m_info.leaf && m_values[index].leaf;
}

void function_analyser::compute_lifetime(std::size_t index, const llvm::Value* user)
{
    walk_to(index, index_of(user), [this](const walk_to_info& info) mutable
    {
        value_info& target{m_values[info.target]};
        block_info& block{m_blocks[info.block]};

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
        block_info& block{m_blocks[info.block]};

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

void function_analyser::compute_phi_lifetime(std::size_t index, const llvm::PHINode* phi)
{
    value_info& info{m_values[index]};

    for(const llvm::BasicBlock* pred : phi->blocks())
    {
        // may come from other block you dumb shit
        if(phi->getIncomingValueForBlock(pred) == info.value) // Find the pred that has the value
        {
            const block_info& pred_info{info_of(pred)};

            walk_to(index, index_of(pred_info.block->getTerminator()), [this](const walk_to_info& info) mutable
            {
                value_info& target{m_values[info.target]};
                block_info& block{m_blocks[info.block]};

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
                block_info& block{m_blocks[info.block]};

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

bool function_analyser::check_external_calls(std::size_t begin, std::size_t end) const
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

void function_analyser::extract_groups()
{
    compute_phi_groups();
    compute_smove_groups();
    compute_zext_trunc_groups();
    sync_groups_members();
}

void function_analyser::compute_phi_groups()
{
    // makes no sense to have a value in multiple groups
    // -> coalesce groups
    // 
    for(llvm::BasicBlock& block : m_function)
    {
        for(llvm::PHINode& phi : block.phis())
        {
            const std::size_t group_index{std::size(m_groups)};
            group_info& group{m_groups.emplace_back()};
            group.type = group_type::phi;
            group.members.emplace_back(&phi);
            info_of(&phi).group = group_index;

            for(llvm::Value* value : phi.incoming_values())
            {
                if(const auto value_index{index_of(value)}; value_index != invalid_index)
                {
                    m_values[value_index].group = group_index;
                    group.members.emplace_back(value);
                }
            }
        }
    }
}

void function_analyser::compute_smove_groups()
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

                    if(value_group->type != group_type::smove)
                    {
                        value_group->type = group_type::generic;
                    }
                }
                else
                {
                    const std::size_t group_index{std::size(m_groups)};

                    group_info& group{m_groups.emplace_back()};
                    group.type = group_type::smove;
                    group.members.emplace_back(call->getArgOperand(0));
                    group.members.emplace_back(call);

                    info_of(call->getArgOperand(0)).group = group_index;
                    info_of(call).group = group_index;
                }
            }
        }
    }
}

void function_analyser::compute_zext_trunc_groups()
{
    const auto process = [this](llvm::Instruction* inst)
    {
        // if the register already has a group, keep it
        if(group_info* value_group{group_of(inst->getOperand(0))}; value_group)
        {
            value_group->members.emplace_back(inst);

            if(value_group->type != group_type::zext_trunc)
            {
                value_group->type = group_type::generic;
            }
        }
        else
        {
            const std::size_t group_index{std::size(m_groups)};

            group_info& group{m_groups.emplace_back()};
            group.type = group_type::zext_trunc;
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

void function_analyser::sync_groups_members()
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

            if(info_of(member).affinities[0].affinity == register_affinity::ret)
            {
                group.ret = true;
            }
        }

        // The lifetime of the group is the coalescence of all members' lifetime
        for(llvm::Value* member : group.members)
        {
            value_info& info{info_of(member)};

            info.leaf = info.leaf && group.leaf;
            group.lifetime.coalesce(info.lifetime);
        }
    }
}

std::size_t function_analyser::insert_instruction(llvm::Instruction* inst, llvm::Instruction* position)
{
    inst->insertBefore(position);

    const auto position_index{index_of(position)};

    value_info info{};
    info.value = inst;
    info.name = get_value_label(*inst);
    info.block = m_values[position_index].block;
    m_values.insert(m_values.begin() + position_index, std::move(info));

    fill_affinity(position_index);
    reduce_affinities(position_index);
    fill_lifetime(position_index);

    for(auto& block : m_blocks)
    {
        if(position_index < block.begin)
        {
            block.begin += 1;
        }

        if(position_index < block.end)
        {
            block.end += 1;
        }
    }

    for(auto& value : m_values)
    {
        for(auto& range : value.lifetime)
        {
            if(position_index < range.begin)
            {
                range.begin += 1;
            }

            if(position_index < range.end)
            {
                range.end += 1;
            }
        }
    }

    return position_index;
}

}
