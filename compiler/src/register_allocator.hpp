#ifndef ALTAIR_COMPILER_REGISTER_ALLOCATOR_HPP_INCLUDED
#define ALTAIR_COMPILER_REGISTER_ALLOCATOR_HPP_INCLUDED

#include "lifetime.hpp"

#include <llvm/IR/Dominators.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Analysis/LoopInfo.h>

namespace ar
{

/*
The goal of the register allocator is to find a place for each variable in either registers or memory
Another goal is to make this efficient, generates the smaller amount of memory accesses and moves
Puting the return values in the right registers at the time they are generated, not with any move
Values shared across multiple basic blocks should reside in the same registers, to prevent useless moves/spills

Implementation details:
    Value lifetime range: from the declaration to the last use (excluded):
        - PHI nodes are ignored for lifetime compute, instead moves intrinsic are put right before the terminator of the predecessor,
          which extend their lifetime until the said terminator, and until the PHI nodes that may use them.
          This method enable lifetimes of values used across multiple basic blocks to never overlap,
          so they can be assigned to the same register, but also guarentee that they are not wrongly overwritten.

    Value affinity:
        - Values' affinity depends on the usage, and may force to allocate specific register and instruction reordering.
        - register_allocator::register_affinity contains details about each affinity.

    Value group:
        - Values may have a group, the default one is the group 0. Other groups optains a register priotity over the group 0.
        - Groups are used especially for PHI nodes optimization: so, values shared across multiple basic blocks have
          more chances to live in the same register.

    Value spill cost:
        - The spill cost is equal to the number of load and store that would be required to make the variable live in memory.

Algorithm (altair.extend must have been generated):
    Compute lifetimes range:
        - Each value has a lifetime range that start at the declaration and ends at the last usage. PHI nodes are ignored.

    Assign register affinities:
        - The values specified in the returns must have ret affinity.
        - The arguments must all have argument affinity
        - The return values of cmps must have branch affinity
        - Other may have either generic or accumulator affinity

    Compute groups for PHI nodes:
        - Loop over all PHI nodes, values that are part of the phi nodes must come from altair.extend intrinsics,
          and altair.extend extend another value lifetime. All of, the return value of the phi instruction, the operands,
          and the extended values must be assigned to the same group.

    Assign the 9 first arguments to register [2; 10] (since its where they are a the begining of the function)
        - Additionnal arguments are on the stack, they may be put direcly in registers, or be kept on the stack until their first usage:
          If the function is a leaf function, and the register pressure is low, all used arguments should be put directly in registers.
        - If the function make calls it must saves the arguments used after the calls since arguments registers are volatile.

    Assign all ret values the right register:
        - For all values with ret affinity, assign them to the corresponding register in the range [2; 10].
          Unlike arguments there can not be more than 9 return values (i.e. nothing can be returned by stack, yet ???).

    Compute assignment priorities:
        - Priorities are used to discriminate register allocation order.
        - Only values that are not forced inside specific registers are considered by assignment priority.
        - The larger the lifetime range, the larger the priority.

    Start assignment loop (based on LLVM RA):
        - Take the next value on the priority queue
        - Try to assign it to a register, take the first register which lifetime does not overlap with the value's lifetime range.
        - If no register is available, try to evict one large range already assigned:
            - The evicted range must have a smaller spill cost, otherwise evict is not worth it.
        - If no range can be evicted, try to split current lifetime range into multiple ones,
          and put them back to the priority queue
            - To prevent spill of its whole lifetime, a value lifetime may be splitted into smallers one.
            - A lifetime can only be splitted if their is more than 1 usage of the value.
        - If the value can not be splitted, then spill it
            - To spill a value we have to assign a memory location to the value, and then insert load and store before and after each use
              This means that we still need a register when we reach the instruction that uses the value.
*/

class register_allocator
{
public:
    enum class register_affinity : std::uint32_t
    {
        generic,     //This value is a variable with no special purpose
        argument,    //This value is a function argument, they have a predefined register that can not be changed
        accumulator, //This value is should be placed in a local accumulator (unused, yet)
        branch,      //This value is related to branching, and uses a the special and unique register: BR
        ret          //This value is part of the returned values (so it should be placed in the right register)
    };

    enum class register_type : std::uint32_t
    {
        generic_volatile,
        generic_non_volatile,
        special,
        branch
    };

    struct value_info
    {
        llvm::Value* value{};
        std::string name{}; // For debug only
        std::size_t block{}; // Index of parent block
        std::vector<bool> usage{}; // Index of the value usages
        std::size_t definition{}; // Index of the value definition (may be located after first usage because of phi nodes)
        std::size_t first_use{std::numeric_limits<std::size_t>::max()}; // Index of the first usage
        std::size_t last_use{}; // Index of the last usage
        ar::lifetime lifetime{};
        register_affinity affinity{register_affinity::generic}; //what kind of value it is
        std::uint32_t spill_weight{};
        std::size_t register_affinity{}; // For "argument" and "ret" affinity, register_index contains the forced register if any.
        std::size_t register_index{std::numeric_limits<std::size_t>::max()}; // The actual assigned register
    };

    struct block_info
    {
        llvm::BasicBlock* block{};
        std::string name{}; // For debug only
        std::size_t begin{}; //Index of the first instruction of the block
        std::size_t begin_no_phi{}; //Index of the first instruction of the block (phi excluded)
        std::size_t end{}; //Index of the last instruction of the block (one past terminator)
        std::size_t scc{}; //Index of the scc
        std::size_t leaf{std::numeric_limits<std::size_t>::max()}; //Index of the first instruction that is leaf in that block (if any)
        llvm::Loop* loop{}; //Not null if this block is a header of a loop
    };

    struct scc_info
    {
        std::vector<llvm::BasicBlock*> blocks{};
        std::vector<llvm::BasicBlock*> predecessors{}; //LLVM does not provide easy function for multiple predecessors
        std::vector<llvm::BasicBlock*> successors{}; //LLVM does not provide easy function for multiple successors
        std::size_t begin{}; //Index of the first block
        std::size_t end{}; //Index of the last block
        llvm::Loop* loop{}; //Not null if the blocks can be executed multiple times
    };

    struct loop_info
    {
        llvm::Loop* loop{};
        std::vector<llvm::BasicBlock*> predecessors{}; //LLVM does not provide easy function for multiple predecessors
        std::vector<llvm::BasicBlock*> successors{}; //LLVM does not provide easy function for multiple successors
    };

    struct register_info
    {
        register_type type{};
        ar::lifetime lifetime{};
    };

public:
    register_allocator(llvm::Module& module, llvm::Function& function);

    ~register_allocator() = default;
    register_allocator(const register_allocator&) = delete;
    register_allocator(register_allocator&&) = delete;
    register_allocator& operator=(const register_allocator&) = delete;
    register_allocator& operator=(register_allocator&&) = delete;

    //sccs accessors
    const std::vector<scc_info>& sccs() const noexcept
    {
        return m_sccs;
    }

    std::size_t index_of(const scc_info& scc) const noexcept
    {
        return &scc - std::data(m_sccs);
    }

    //loops accessors
    const std::vector<loop_info>& loops() const noexcept
    {
        return m_loops;
    }

    std::size_t index_of(const llvm::Loop* loop) const noexcept
    {
        std::size_t index{};
        for(auto& info : m_blocks)
        {
            if(info.loop == loop)
            {
                return index;
            }

            ++index;
        }

        return index;
    }

    std::size_t index_of(const loop_info& loop) const noexcept
    {
        return &loop - std::data(m_loops);
    }

    loop_info& info_of(const llvm::Loop* loop) noexcept
    {
        return m_loops[index_of(loop)];
    }

    const loop_info& info_of(const llvm::Loop* loop) const noexcept
    {
        return m_loops[index_of(loop)];
    }

    //blocks accessors
    const std::vector<block_info>& blocks() const noexcept
    {
        return m_blocks;
    }

    std::size_t index_of(const llvm::BasicBlock* block) const noexcept
    {
        std::size_t index{};
        for(auto& info : m_blocks)
        {
            if(info.block == block)
            {
                return index;
            }

            ++index;
        }

        return index;
    }

    std::size_t index_of(const block_info& block) const noexcept
    {
        return &block - std::data(m_blocks);
    }

    block_info& info_of(const llvm::BasicBlock* block) noexcept
    {
        return m_blocks[index_of(block)];
    }

    const block_info& info_of(const llvm::BasicBlock* block) const noexcept
    {
        return m_blocks[index_of(block)];
    }

    //values accessors
    const std::vector<value_info>& values() const noexcept
    {
        return m_values;
    }

    std::size_t index_of(const llvm::Value* value) const noexcept
    {
        std::size_t index{};
        for(auto& info : m_values)
        {
            if(info.value == value)
            {
                return index;
            }

            ++index;
        }

        return index;
    }

    std::size_t index_of(const value_info& value) const noexcept
    {
        return &value - std::data(m_values);
    }

    value_info& info_of(const llvm::Value* value) noexcept
    {
        return m_values[index_of(value)];
    }

    const value_info& info_of(const llvm::Value* value) const noexcept
    {
        return m_values[index_of(value)];
    }

    const std::vector<std::vector<llvm::Value*>> phi_groups() const noexcept
    {
        return m_phi_groups;
    }

private:
    //Extract sccs informations
    //sccs are stored in preorder
    void extract_sccs();

    //Extract basic blocks informations
    //Blocks are stored in preorder
    void extract_blocks();

    //Find the scc block belong to
    std::size_t get_scc(const llvm::BasicBlock* block);

    //Give to all loop header the reference over the loop instance
    void extract_loop_headers();
    void extract_loop_headers_recurse(llvm::Loop* loop);

    //Store edges (predecessors and successors) of each loop
    void extract_loops();
    void extract_loop(llvm::Loop* loop);

    //Store edges (predecessors and successors) of each scc
    void extract_edges();

    //mark leaf block
    void fill_leaf();

    //Store values info
    void extract_values();

    //give static register to all phi node operands to make them disapear in hell
    void compute_phi_groups();

    //analyze value lifetime
    void compute_lifetimes();
    void fill_lifetime(std::size_t index);
    //Returns true if we hit the definition of values[index] starting in block current
    bool compute_lifetime_loop(std::size_t index, const llvm::BasicBlock* current);
    void compute_lifetime(std::size_t index, const llvm::Value* user);
    void compute_phi_lifetime(std::size_t index, const llvm::PHINode* phi);

    //Give each value (if applicable) a spill weight
    void compute_spill_weight();
    //Give each value its register affinity
    void compute_affinity();

    //Init register info
    void fill_register_info();
    //Give each value a register
    void allocate_registers();
    std::size_t assing_register(std::size_t value_index);

private:
    llvm::Module& m_module;
    llvm::Function& m_function;
    llvm::DominatorTree m_tree;
    llvm::LoopInfo m_loop_info;
    std::vector<scc_info> m_sccs{};
    std::vector<block_info> m_blocks{};
    std::vector<value_info> m_values{};
    std::vector<loop_info> m_loops{};
    std::vector<std::vector<llvm::Value*>> m_phi_groups{};
    std::vector<std::size_t> m_order{};
    std::array<register_info, 65> m_registers{};
};

}

#endif
