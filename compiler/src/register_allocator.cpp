#include "register_allocator.hpp"

#include <iostream>

#include <llvm/ADT/SCCIterator.h>
#include <llvm/ADT/PostOrderIterator.h>

#include "transform.hpp"
#include "utilities.hpp"

namespace ar
{

register_allocator::register_allocator(llvm::Module& module, llvm::Function& function)
:m_module{module}
,m_function{function}
{
    ar::transforms::swap_add_sub(m_module, m_function);
    ar::transforms::decompose_getelementptr(m_module, m_function);
    ar::transforms::optimize_load_store(m_module, m_function);
    ar::transforms::insert_move_for_constant(m_module, m_function);
    ar::transforms::reorder_blocks(m_module, m_function);
    ar::transforms::invert_branch_condition(m_module, m_function);

    m_tree = llvm::DominatorTree{m_function};
    m_loop_info = llvm::LoopInfo{m_tree};

    extract_sccs();
    extract_blocks();
    extract_loop_headers();
    extract_loops();
    extract_edges();
    extract_values();

    compute_phi_groups();
    compute_lifetimes();

    compute_affinity();
    allocate_registers();
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

size_t register_allocator::get_scc(const llvm::BasicBlock* block)
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

void register_allocator::extract_loop_headers()
{
    for(llvm::Loop* loop : m_loop_info)
    {
        extract_loop_headers_recurse(loop);
    }
}

void register_allocator::extract_loop_headers_recurse(llvm::Loop* loop)
{
    info_of(loop->getHeader()).loop = loop;

    llvm::SmallVector<llvm::Loop*, 32> inners{};
    loop->getInnerLoopsInPreorder(*loop, inners);

    for(llvm::Loop* inner : inners)
    {
        extract_loop_headers_recurse(inner);
    }
}

void register_allocator::extract_loops()
{
    for(llvm::Loop* loop : m_loop_info)
    {
        extract_loop(loop);
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

void register_allocator::extract_loop(llvm::Loop* loop)
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
        extract_loop(inner);
    }
}

void register_allocator::extract_edges()
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

void register_allocator::fill_leaf()
{
    assert(false && "not implemented");

    //If there is external calls, we will only be leaf after the last external call in each possible execution path
    if(ar::has_external_call(m_function))
    {
        for(scc_info& scc : m_sccs)
        {
            if(scc.loop)
            {
                for(llvm::BasicBlock* block : scc.blocks)
                {
                    if(ar::has_external_call(*block))
                    {

                    }
                }
            }
            else
            {

            }
        }
    }
    else //Otherwise there is no external call at all, then all block are leaf
    {
        for(block_info& block : m_blocks)
        {
            block.leaf = 0;
        }
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
                value.affinity = register_affinity::branch;
            }
            else if(llvm::isa<llvm::PHINode>(instruction))
            {
                ++block.begin_no_phi;
            }
        }

        block.end = std::size(m_values);
    }
}

void register_allocator::compute_phi_groups()
{
    for(llvm::BasicBlock& block : m_function)
    {
        for(llvm::PHINode& phi : block.phis())
        {
            std::vector<llvm::Value*>& group{m_phi_groups.emplace_back()};

            for(llvm::Value* value : phi.incoming_values())
            {
                group.emplace_back(value);
            }

            group.emplace_back(&phi);
        }
    }

    for(std::size_t i{}; i < std::size(m_phi_groups); ++i)
    {
        std::cout << "  Group #" << i << ": ";

        for(llvm::Value* member : m_phi_groups[i])
        {
            std::cout << get_value_label(*member) << " ";
        }

        std::cout << std::endl;
    }
}

void register_allocator::compute_lifetimes()
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

bool register_allocator::compute_lifetime_loop(size_t index, const llvm::BasicBlock* current)
{
    value_info& info{m_values[index]};
    const block_info& current_info{info_of(current)};

    //This is the block where the value is defined, stop here
    if(index_of(current) == info.block)
    {
        info.lifetime.add_range(index, m_blocks[info.block].end);

        std::cout << "Add range [" << index << "; " << m_blocks[info.block].end << "] to " << get_value_label(*info.value) << std::endl;

        return true;
    }

    bool output = false;

    if(current_info.loop)
    {
        //If the predecessor is part of a loop header we direcly go to the predecessor of the loop
        //It will mark the whole loop as a part of the lifetime
        for(const llvm::BasicBlock* pred : info_of(current_info.loop).predecessors)
        {
            if(compute_lifetime_loop(index, pred))
            {
                const block_info& pred_info{info_of(pred)};
                info.lifetime.add_range(pred_info.begin, pred_info.end);

                std::cout << "Add range [" << pred_info.begin << "; " << pred_info.end << "] to " << get_value_label(*info.value)
                          << " because of block " << get_value_label(*pred) << std::endl;

                for(const llvm::BasicBlock* block : current_info.loop->blocks())
                {
                    const block_info& loop_block{info_of(block)};

                    std::cout << "Add range [" << loop_block.begin << "; " << loop_block.end << "] to " << get_value_label(*info.value)
                              << " because of loop " << current_info.name << " block " << get_value_label(*block) << std::endl;
                    info.lifetime.add_range(loop_block.begin, loop_block.end);
                }

                output = true;
            }
        }
    }
    else
    {
        //Otherwise simply go through all predecessors
        for(const llvm::BasicBlock* pred : llvm::predecessors(current))
        {
            if(compute_lifetime_loop(index, pred))
            {
                const block_info& pred_info{info_of(pred)};
                info.lifetime.add_range(pred_info.begin, pred_info.end);

                std::cout << "Add range [" << pred_info.begin << "; " << pred_info.end << "] to " << get_value_label(*info.value)
                          << " because of block " << get_value_label(*pred) << std::endl;

                output = true;
            }
        }
    }

    return output;
}

void register_allocator::compute_lifetime(size_t index, const llvm::Value* user)
{
    value_info& info{m_values[index]};

    const value_info& user_info {info_of(user)};
    const std::size_t user_index{index_of(user_info)};
    const block_info& user_block{m_blocks[user_info.block]};

    if(user_info.block == info.block) //Used in same block
    {
        info.lifetime.add_range(index, user_index);
    }
    else //Used in other block
    {
        compute_lifetime_loop(index, user_block.block);
    }
}

void register_allocator::compute_phi_lifetime(size_t index, const llvm::PHINode* phi)
{
    value_info& info{m_values[index]};

    for(const llvm::BasicBlock* pred : phi->blocks())
    {
        if(phi->getIncomingValueForBlock(pred) == info.value) // Find the pred that has the value
        {
            const block_info& pred_info {info_of(pred)};
            const std::size_t pred_index{index_of(pred_info)};

            if(pred_index == info.block)
            {
                info.lifetime.add_range(index, pred_info.end); //Extend lifetime till end of block
            }
            else //Used in other block
            {
                info.lifetime.add_range(pred_info.begin, pred_info.end); // must live in this block
                compute_lifetime_loop(index, pred_info.block);
            }
        }
    }
}

void register_allocator::compute_spill_weight()
{
    for(value_info& info : m_values)
    {
        for(bool use : info.usage)
        {
            if(use)
            {
                ++info.spill_weight;
            }
        }

        if(m_sccs[m_blocks[info.block].scc].loop)
        {
            info.spill_weight *= 2;
        }
    }
}

void register_allocator::compute_affinity()
{
    for(llvm::BasicBlock& block : m_function)
    {
        for(llvm::Value& value : block)
        {
            if(auto ret{llvm::dyn_cast<llvm::ReturnInst>(&value)}; ret)
            {
                if(llvm::Value* ret_value{ret->getReturnValue()}; ret_value)
                {
                    info_of(ret_value).affinity = register_affinity::ret;
                }
            }
            else if(llvm::isa<llvm::CmpInst>(&value))
            {
                info_of(&value).affinity = register_affinity::branch;
            }
        }
    }
}

void register_allocator::fill_register_info()
{
    m_registers[0].type = register_type::special; //SP (SPM)
    m_registers[1].type = register_type::special; //SP

    for(std::size_t i{2}; i <= 10; ++i) //Args and return
    {
        m_registers[i].type = register_type::generic_volatile;
    }

    for(std::size_t i{11}; i <= 26; ++i) //Generic non volatile
    {
        m_registers[i].type = register_type::generic_non_volatile;
    }

    for(std::size_t i{27}; i <= 19; ++i) //Generic volatile
    {
        m_registers[i].type = register_type::generic_volatile;
    }

    m_registers[60].type = register_type::special; //Loop
    m_registers[61].type = register_type::special; //Accumulator
    m_registers[62].type = register_type::special; //Product
    m_registers[63].type = register_type::special; //Quotient

    m_registers[64].type = register_type::branch; //BR
}

/*
r0      (1)  | SP/Buffer SPM
r1      (1)  | SP
r2-r10  (9)  | functions arguments and return values (volatile)
r11-r26 (16) | general purpose (non-volatile)
r27-r59 (33) | general purpose (volatile)
r60-r63 (4)  | special registers (loop, accumulator, product, quotient)
BR           | Branch Register

Total volatile: 43

v0-v7   (8)  | functions arguments and return values (volatile)
v8-v19  (12) | general purpose (non-volatile)
v20-v60 (41) | general purpose (volatile)
v61-v63 (3)  | special registers (Accumulator, product, quotient)

Total volatile: 49
*/

void register_allocator::allocate_registers()
{
    for(std::size_t i{}; i < std::min(m_function.arg_size(), std::size_t{9}); ++i)
    {
        const std::size_t index{i + 2};

        m_registers[index].lifetime.coalesce(m_values[i].lifetime);
        m_values[i].register_index = index;
    }

    //Other args are on the stack
    assert(m_function.arg_size() <= 9 && "The compiler does not handle more that 9 args yet");
    /*
        for(const std::vector<llvm::Value*>& group : m_phi_groups)
        {
            for(llvm::Value* member : group)
            {
                std::size_t value_index{index_of(member)};

                for(std::size_t i{2}; i < 61; ++i)
                {
                    if(!m_registers[i].lifetime.overlap(m_values[value_index].lifetime))
                    {
                        m_registers[i].lifetime.coalesce(m_values[value_index].lifetime);
                        m_values[value_index].register_index = i;
                    }
                }
            }
        }*/

    for(std::size_t i{m_function.arg_size()}; i < std::size(m_values); ++i)
    {
        auto& value{m_values[i]};

        auto instruction{llvm::dyn_cast<llvm::Instruction>(value.value)};
        if(value.register_index == std::numeric_limits<std::size_t>::max() && (instruction && !instruction->getType()->isVoidTy()))
        {
            if(value.affinity == register_affinity::ret)
            {
                m_registers[2].lifetime.coalesce(value.lifetime);
                value.register_index = 2;
            }
            else if(value.affinity == register_affinity::branch)
            {
                assert(!m_registers[64].lifetime.overlap(value.lifetime) && "The compiler don't reorder cmp yet");

                m_registers[64].lifetime.coalesce(value.lifetime);
                value.register_index = 64;
            }
            else
            {
                value.register_index = assing_register(i);
            }
        }
    }
}

size_t register_allocator::assing_register(size_t value_index)
{
    for(std::size_t i{2}; i < 61; ++i)
    {
        if(!m_registers[i].lifetime.overlap(m_values[value_index].lifetime))
        {
            m_registers[i].lifetime.coalesce(m_values[value_index].lifetime);
            return i;
        }
    }

    return std::numeric_limits<std::size_t>::max();
}

}
