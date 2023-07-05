#include "register_allocator.hpp"

#include <iostream>
#include <algorithm>
#include <numeric>

#include <llvm/ADT/SCCIterator.h>
#include <llvm/ADT/PostOrderIterator.h>
#include <llvm/IR/Constants.h>

#include "intrinsic.hpp"
#include "utilities.hpp"

#include <fstream>

namespace ar
{

register_allocator::register_allocator(function_analyser& analyser)
:m_analyser{analyser}
,m_module{analyser.module()}
,m_function{analyser.function()}
{

}

void register_allocator::perform_register_allocation()
{
    compute_spill_weight();
    allocate_registers();
}

void register_allocator::compute_spill_weight(std::size_t index)
{
    auto& value{m_analyser.values()[index]};

    const bool loop{static_cast<bool>(m_analyser.sccs()[m_analyser.blocks()[value.block].scc].loop)};
    const double loop_factor{(loop ? 1.0 : 0.0) * 3.0};

    /*
     * TODO:
     * w = (def + use) * (block frec / entry frequency)
     * if loop: w *= 3
     * for all inst w += w
     * interval.w = w / size of liveinterval
     */

    for(auto& _ : value.value->uses())
    {
        value.alloc.spill_weight += 1.0 + loop_factor;
    }
}

void register_allocator::compute_spill_weight()
{
    for(std::size_t i{}; i < m_analyser.values().size(); ++i)
    {
        compute_spill_weight(i);
    }
}

/*
v0-v7   (8)  | functions arguments and return values (volatile)
v8-v19  (12) | general purpose (non-volatile)
v20-v60 (41) | general purpose (volatile)
v61-v63 (3)  | special registers (Accumulator, product, quotient)

Total volatile: 49

in function stack structure:
 X | Parameters of this function, they are actually in the caller stack frame
   | To be gathered they can either be loaded immediately after the beginning of the function
   | Parameters are passed in order in ascending order meaning that the first argument
   | Is at the lower address (i.e. sp[0]) and the following at higher addresses 
   | depending on their and previous parameters size and alignment
   | Example: for a function defined as `func(<regargs>, i32 i, i64 j)`, 
   | at the beginning of the function:
   | `i` will be accessible using lds.l rX, sp[0] 
   |   -> offset 0 from stack base
   | `j` will be accessible using lds.q rY, sp[8] 
   |   -> offset 8 from stack base because previous parameter as a size of 4, but i64 has an alignment of 8
   | 
 0 | SP is here at function beggining
   | When the function begins and ends, SP must be aligned on 8 octets (i.e. SP % 8 == 0)
   |
 - | Saved LR
 - | Saved registers 
 - | Stack allocated variables (structs, arrays... if not not promoted to registers)
 - | Spill area (local variables), located at the bottom to increase accessibility through `sts`
 - | Args for callees, if any
   |
 -X| SP that callees will receive
   V 

first: one qword arg
second: two qword args
caller:
    ; allocate space so any called function in caller can have all their parameters that fit on the stack ?
    subi.q sp, sp, 16 ; max(first, second) stack size
    
    moveu.q t0, 42  ; first arg
    sts.q t0, 0     ; store it at the lower address of the stack
    call first
    
    moveu.q t0, 12  ; first arg
    moveu.q t1, 69  ; second arg
    sts.q t0, 0     ; store it at the lower address of the stack
    sts.q t1, 8     ; store it at the upper address of the stack
    call second


X < Y (the lesser R0 the bigger the stack = descending adresses)

[V octets : locals] is a stack by itself, and use the following structure:
    [C octets : spill area] space for register spilling
    [C octets : alloca area] space for statically sized stack allocations
    [V octets : malloca area] space for dynamically sized stack allocations
*/

bool register_allocator::allocation_priority_predicate(std::size_t left, std::size_t right)
{
    return m_analyser.values()[left].alloc.need_split && !m_analyser.values()[right].alloc.need_split
        && m_analyser.values()[left].lifetime.total_span() < m_analyser.values()[right].lifetime.total_span();
}

void register_allocator::generate_queue()
{
    m_queue.clear();
    m_queue.reserve(std::size(m_analyser.values()));

    for(std::size_t i{m_function.arg_size()}; i < std::size(m_analyser.values()); ++i)
    {
        if(!std::empty(m_analyser.values()[i].lifetime)
        && m_analyser.values()[i].affinities.size() == 1
        && m_analyser.values()[i].affinities[0].affinity == register_affinity::generic)
        {
            m_queue.emplace_back(i);
        }
    }

    std::sort(std::begin(m_queue), std::end(m_queue), [this](std::size_t left, std::size_t right)
    {
        return allocation_priority_predicate(left, right);
    });
}

void register_allocator::insert_queue(std::size_t value_index)
{
    const auto it = std::lower_bound(std::begin(m_queue), std::end(m_queue), value_index, [this](std::size_t right, std::size_t left)
    {
        return allocation_priority_predicate(left, right);
    });

    m_queue.insert(it, value_index);
}

void register_allocator::assign_group_register(const group_info& group)
{
    // If a member is an ret, all group members must share its register
    // TODO: increase checks
    if(group.ret && group.leaf)
    {
        for(llvm::Value* member : group.members)
        {
            auto& value{m_analyser.info_of(member)};

            value.alloc.register_index = ret_register;
            value.alloc.spill_weight = unspillable;

            m_registers[ret_register].lifetime.coalesce(value.lifetime);
        }

        return;
    }
    else if(group.leaf)
    {
        for(llvm::Value* member : group.members)
        {
            auto& value{m_analyser.info_of(member)};
            auto& affinity{value.affinities[0]};

            // If a member is an argument, all group members must share its register
            // TODO: increase checks
            if(affinity.affinity == register_affinity::argument && affinity.default_register < stack_args_count)
            {
                for(llvm::Value* member2 : group.members)
                {
                    auto& value2{m_analyser.info_of(member2)};
                    value.alloc.register_index = args_registers_begin + affinity.default_register;
                    value.alloc.spill_weight = unspillable;

                    m_registers[affinity.default_register].lifetime.coalesce(value2.lifetime);
                }

                return;
            }
        }
    }

    // Generic group (= not special register)
    const std::size_t begin{group.leaf ? volatile_registers_begin : non_volatile_registers_begin};
    const std::size_t end{group.leaf ? volatile_registers_end : non_volatile_registers_end};

    bool assigned{};
    for(std::size_t i{begin}; i < end; ++i)
    {
        if(!m_registers[i].lifetime.overlap(group.lifetime))
        {
            m_registers[i].lifetime.coalesce(group.lifetime);
            for(llvm::Value* member : group.members)
            {
                auto& value{m_analyser.info_of(member)};

                value.alloc.register_index = i;
                value.alloc.spill_weight = unspillable;
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

void register_allocator::generate_stack_structure()
{/*
    if(m_stack_info.exists)
    {
        return;
    }

    llvm::Type* i64_type{llvm::Type::getInt64Ty(m_module.getContext())};
    m_stack_info.header_last = m_function.getEntryBlock().getFirstNonPHI();

    m_stack_info.push_size = llvm::ConstantInt::get(i64_type, 8);
    ar::insert_push_intrinsic(m_module, m_stack_info.push_size, m_stack_info.header_last);

    for(auto& block : m_blocks)
    {
        if(auto ret{llvm::dyn_cast<llvm::ReturnInst>(block.block->getTerminator())}; ret)
        {
            m_stack_info.footers_last.emplace_back(ret);
        }
    }

    for(auto& ret : m_stack_info.footers_last)
    {
        ret = ar::insert_pop_intrinsic(m_module, m_stack_info.push_size, ret);
    }

    m_stack_info.exists = true;*/
}

void register_allocator::generate_link_save()
{/*
    llvm::Type* i64_type{llvm::Type::getInt64Ty(m_module.getContext())};

    auto index{llvm::ConstantInt::get(i64_type, 0)};
    auto link{ar::insert_getri_intrinsic(m_module, index, m_stack_info.header_last)};
    auto spill{ar::insert_spill_intrinsic(m_module, link, m_stack_info.header_last)};

    stack_entry entry{};
    entry.type = entry_type::link;
    entry.size = 8;
    entry.alignment = 8;
    entry.position = -8;
    entry.value = link;
    entry.spill = spill;
    m_stack.emplace_back(entry);

    for(auto& ret : m_stack_info.footers_last)
    {
        auto restored{ar::insert_fill_intrinsic(m_module, i64_type, spill, ret)};
        ret = ar::insert_setri_intrinsic(m_module, index, restored, ret);
    }*/
}

void register_allocator::allocate_registers()
{
    // Save LR if needed
    if(!m_analyser.info().leaf)
    {
        generate_stack_structure();
        generate_link_save();

        for(std::size_t i{}; i < std::min(m_function.arg_size(), std::size_t{8}); ++i)
        {
            auto& info{m_analyser.values()[i]};

            if(!info.leaf)
            {
                //if a parameter is non-leaf then we need to move the parameter volatile register to a non-volatile one
                //or we need to spill/fill around calls ?
                //if the number of call is == 1 (call in loops count as 8 calls) spill around the call ?
                
                //Current: copy it in a non volatile register
                auto stable{insert_copy_intrinsic(m_module, info.value, m_analyser.blocks()[0].block->getFirstNonPHI())};
                info.value->replaceAllUsesWith(stable);

                // replace all uses will also affect stable, so we force it back to the right value
                stable->setOperand(0, info.value);
            }
        }

        m_analyser.perform_analysis();

        for(const group_info& group : m_analyser.groups())
        {
            if(group.ret && !group.leaf)
            {
                for(auto member : group.members)
                {
                    for(auto user : member->users())
                    {
                        if(auto ret{llvm::dyn_cast<llvm::ReturnInst>(user)}; ret)
                        {
                            auto ret_value{insert_copy_intrinsic(m_module, ret->getReturnValue(), ret)};
                            ret->setOperand(0, ret_value);
                        }
                    }
                }
            }
        }

        m_analyser.perform_analysis();
    }

    const llvm::DataLayout& data_layout{m_module.getDataLayout()};
    
    std::uint64_t arg_position{};
    // assign callees arguments register
    for(std::size_t i{}; i < std::size(m_analyser.values()); ++i)
    {
        auto& value{m_analyser.values()[i]};
        auto& affinity{value.affinities[0]};

        /*
        generic,        
        none,           
        argument,       
        spill,          
        flags,          
        ret,            
        callee_argument,
        callee_ret,     
        accumulator,    
        bypass,         
        */

        switch(affinity.affinity)
        {
            case register_affinity::argument: [[fallthrough]];
            case register_affinity::callee_argument:
            {
                if(affinity.default_register < stack_args_count)
                {
                    value.alloc.register_index = args_registers_begin + affinity.default_register;
                    value.alloc.spill_weight = unspillable;
                    m_registers[value.alloc.register_index].lifetime.coalesce(value.lifetime);
                }
                else
                {
                    //register stack entries, insert load later, where needed
                    //auto arg_type{value.value->getType()};
                    //auto arg_align{data_layout.getABITypeAlign(arg_type)};
                    //
                    //stack_entry entry{};
                    //entry.type = entry_type::argument;
                    //entry.size = data_layout.getTypeAllocSize(arg_type);
                    //entry.alignment = arg_align.value();
                    //entry.position = arg_position + llvm::offsetToAlignment(arg_position, arg_align);
                    //entry.value = value.value;
                    //m_stack.emplace_back(entry);
                    //
                    //arg_position = entry.position;
                }

                break;
            }

            case register_affinity::flags:
            {
                value.alloc.register_index = flags_register;
                value.alloc.spill_weight = unspillable;
                assert(!m_registers[value.alloc.register_index].lifetime.overlap(value.lifetime) && "Flags register uses must not overlap");
                m_registers[value.alloc.register_index].lifetime.coalesce(value.lifetime);
                break;
            }

            case register_affinity::ret: [[fallthrough]];
            case register_affinity::callee_ret:
            {
                value.alloc.register_index = ret_register;
                value.alloc.spill_weight = unspillable;
                m_registers[value.alloc.register_index].lifetime.coalesce(value.lifetime);
                break;
            }

            case register_affinity::accumulator: [[fallthrough]];
            case register_affinity::bypass:
                break;

            default:
                break;
        }
    }

    // Alloca...

    // Assign register for groups
    // Assume that there are enough registers to handle all groups lifetimes without overlaping
    // Assume phi group members are non overlapping
    // Leaf value must be synced between members of phi groups
    for(const group_info& group : m_analyser.groups())
    {
        assign_group_register(group);
    }

    generate_queue();

    while(!std::empty(m_queue))
    {
        const std::size_t value_index{m_queue.back()};
        m_queue.pop_back();

        auto& value{m_analyser.values()[value_index]};
        auto instruction{llvm::dyn_cast<llvm::Instruction>(value.value)};

        if(instruction && !instruction->getType()->isVoidTy() && value.alloc.register_index == no_register)
        {
            allocate_register(value_index);
        }
    }

    for(auto& block : m_analyser.blocks())
    {
        auto ret{llvm::dyn_cast<llvm::ReturnInst>(block.block->getTerminator())};

        for(std::size_t i{non_volatile_registers_begin}; i < non_volatile_registers_end; ++i)
        {
            if(!std::empty(m_registers[i].lifetime))
            {
                
            }
        }
    }
}

void register_allocator::allocate_register(std::size_t value_index)
{
    auto& value{m_analyser.values()[value_index]};

    if(value.alloc.need_split)
    {
        assert(false && "Split is unsuported for now, please do not compile too complex algorithms, we have a lot of registers but not enough to build a web engine.");
        //find_best_split
    }
    
    // dead code, for now ?
    if(value.affinities[0].affinity == register_affinity::ret)
    {
        assert(!m_registers[ret_register].lifetime.overlap(value.lifetime) && "Return value overlapping");

        m_registers[ret_register].lifetime.coalesce(value.lifetime);
        value.alloc.register_index = ret_register;
    }
    else if(value.affinities[0].affinity == register_affinity::flags)
    {
        assert(!m_registers[flags_register].lifetime.overlap(value.lifetime) && "The compiler don't reorder cmp yet");

        m_registers[flags_register].lifetime.coalesce(value.lifetime);
        value.alloc.register_index = flags_register;
    }
    else
    {
        if(auto reg{assing_register(value_index)}; reg != no_register)
        {
            value.alloc.register_index = reg;
        }
        else
        {
            std::size_t cheapest_index{no_register};
            for(std::size_t i{}; i < std::size(m_analyser.values()); ++i)
            {
                auto& value{m_analyser.values()[i]};

                if(value.alloc.spill_weight < value.alloc.spill_weight
                && (cheapest_index == no_register || value.alloc.spill_weight < m_analyser.values()[cheapest_index].alloc.spill_weight))
                {
                    cheapest_index = i;
                }
            }

            if(cheapest_index == no_register)
            {
                value.alloc.need_split = true; //split
                insert_queue(value_index);
            }
            else
            {
                //evict
                auto& cheapest{m_analyser.values()[cheapest_index]};
                m_registers[cheapest.alloc.register_index].lifetime.remove(cheapest.lifetime);
                m_registers[cheapest.alloc.register_index].lifetime.coalesce(value.lifetime);

                insert_queue(cheapest_index);
            }
        }
    }
}

std::size_t register_allocator::assing_register(std::size_t value_index)
{
    const value_info& value{m_analyser.values()[value_index]};

    const auto try_register = [this, &value](std::size_t begin, std::size_t end)
    {
        for(std::size_t i{begin}; i < end; ++i)
        {
            if(!m_registers[i].lifetime.overlap(value.lifetime))
            {
                m_registers[i].lifetime.coalesce(value.lifetime);
                return i;
            }
        }

        return no_register;
    };

    if(value.leaf)
    {
        // first try in the renamed generic registers
        if(auto reg{try_register(renamed_registers_begin, renamed_registers_end)}; reg != no_register)
        {
            return reg;
        }

        // first try in the volatile generic registers
        if(auto reg{try_register(volatile_registers_begin, volatile_registers_end)}; reg != no_register)
        {
            return reg;
        }

        // then try in the volatile args/ret registers
        if(auto reg{try_register(args_registers_begin, args_registers_end)}; reg != no_register)
        {
            return reg;
        }
    }

    // then try in the non volatile registers
    return try_register(non_volatile_registers_begin, non_volatile_registers_end);
}

}
