#ifndef ALTAIR_COMPILER_REGISTER_ALLOCATOR_HPP_INCLUDED
#define ALTAIR_COMPILER_REGISTER_ALLOCATOR_HPP_INCLUDED

#include "lifetime.hpp"

#include <llvm/IR/Dominators.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Analysis/LoopInfo.h>

#include <variant>

namespace ar
{

/*
The goal of the register allocator is to find a place for each variable in either registers or memory
Another goal is to make this efficient, generates the smaller amount of memory accesses and moves
Puting the return values in the right registers at the time they are generated

Implementation details:
    Value lifetime range: from the declaration to the last use (excluded):
        - PHI nodes are handled using "phi groups". A phi group is an array of all values that belong to the phi node
          Example:
          | %7 = phi i64 [ %28, %27 ], [ %5, %2 ]
          %7, %28 and %5 are in the same phi group

    Value affinity:
        - Values' affinity depends on the usage, and may force to allocate specific register.
        - register_allocator::register_affinity contains details about each affinity.

    Value spill cost:
        - The spill cost represents the cost of load and store that would be required to make the variable live in memory
          instead of in register.

Algorithm:
    Extract informations:
        - Lots of informations about values, blocks, SCCs and loops are gathered, this step is just an initialization

    Compute groups for PHI nodes:
        - Loop over all PHI nodes. All of the return value of the phi instruction and the operands
          must be assigned to the same group.

    Compute lifetimes range:
        - Each value has a lifetime that contains all ranges (value indices) where the values must be alive to
          ensure value validity.
        - The ranges are computed using the following method:
          For all users, we start at the user index, then we mark down all paths that leads to the defintion.

    Assign register affinities:
        - The values specified in the returns must have ret affinity.
        - The arguments must all have argument affinity.
        - The return values of cmps must have branch affinity.
        - Other may have either generic or accumulator affinity.

    The 9 first arguments are assigne to register [2; 10] (since its where they are a the begining of the function)
        - Additionnal arguments are on the stack.
        - If the function make calls it must saves the arguments used after the calls since arguments registers are volatile.

    Assign all ret values the right register:
        - For all values with ret affinity, assign them to the corresponding register in the range [2; 10].
        - Unlike arguments there can not be more than 9 return values (i.e. nothing can be returned by stack).
        - Current ABI is the same as host, so return value fit in a single register for now

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
    static constexpr std::size_t no_use{std::numeric_limits<std::size_t>::max()};
    static constexpr std::size_t no_register{std::numeric_limits<std::size_t>::max()};
    static constexpr std::size_t no_group{std::numeric_limits<std::size_t>::max()};

public:
    enum class register_affinity : std::uint32_t
    {
        generic,     //This value is a variable with no special purpose
        argument,    //This value is a function argument, they have a predefined register that can not be changed
        spill,       //This value is spilled, must not assign any register
        flags,       //This value is related to branching, and uses a the special and unique register: FR
        ret,         //This value is part of the returned values (so it should be placed in the right register)
        accumulator, //This value is should be placed in a local accumulator (unused, yet)
    };

    enum class register_type : std::uint32_t
    {
        generic_volatile,
        generic_non_volatile,
        special,
        flags
    };

    struct value_info
    {
        llvm::Value* value{};
        std::string name{}; // For debug only
        std::size_t block{}; // Index of parent block
        std::vector<bool> usage{}; // Index of the value usages
        std::size_t definition{}; // Index of the value definition (may be located after first usage because of phi nodes)
        std::size_t first_use{no_use}; // Index of the first usage
        std::size_t last_use{}; // Index of the last usage
        ar::lifetime lifetime{};
        register_affinity affinity{register_affinity::generic}; //what kind of value it is
        std::uint32_t spill_weight{};
        std::size_t default_register{}; // For "argument" and "ret" affinity, register_index contains the forced register if any.
        std::size_t register_index{no_register}; // The actual assigned register
        std::size_t group{no_group};
        bool leaf{true}; // if true then this value can be assigned to volatile register (i.e lifetime does not include indirect call)
    };

    struct block_info
    {
        llvm::BasicBlock* block{};
        std::string name{}; // For debug only
        std::size_t begin{}; //Index of the first instruction of the block
        std::size_t begin_no_phi{}; //Index of the first instruction of the block (phi excluded)
        std::size_t end{}; //Index of the last instruction of the block (one past terminator)
        std::size_t scc{}; //Index of the scc
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

    struct group_info
    {
        std::vector<llvm::Value*> members{};
        ar::lifetime lifetime{}; // members coalesced lifetimes
        bool leaf{true}; // if a member is non leaf, the whole group is non leaf
    };

    struct register_info
    {
        register_type type{};
        ar::lifetime lifetime{};
    };

    struct stack_entry_info
    {
        llvm::Value* value{};
        std::size_t size{};
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
        for(auto& info : m_loops)
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

    const std::vector<group_info> groups() const noexcept
    {
        return m_groups;
    }

    const group_info* group_of(llvm::Value* value) const noexcept
    {
        if(const auto index{info_of(value).group}; index != std::numeric_limits<std::uint32_t>::max())
        {
            return &m_groups[index];
        }

        return nullptr;
    }

    group_info* group_of(llvm::Value* value) noexcept
    {
        if(const auto index{info_of(value).group}; index != std::numeric_limits<std::uint32_t>::max())
        {
            return &m_groups[index];
        }

        return nullptr;
    }

private:
    // walk_to_status::local is given once to callback when the target and start are in the same block
    // walk_to_status::begin is given once to callback when the target is not in the same block as the user, it contains the user block
    // walk_to_status::step is given X times to callback when a block is traversed and is a path valid to the user
    // walk_to_status::end is given X times to callback, each time a walked path reaches the target
    enum class walk_to_status : std::size_t
    {
        local,
        begin,
        step,
        end,
    };

    struct walk_to_info
    {
        walk_to_status status;
        std::size_t target;
        std::size_t start;
        std::size_t block;
    };

    template<typename... Func>
    void walk_to_loop(std::size_t target, std::size_t start, const llvm::BasicBlock* current, Func&&... func)
    {
        //If the predecessor is a loop header we direcly go to the predecessor of the loop
        //and add all loop's blocks to the calculation

        const block_info& current_info{info_of(current)};
        const std::size_t initial_size{std::size(m_walk_to_blocks)};

        for(const llvm::BasicBlock* block : current_info.loop->blocks())
        {
            const auto pred = [index = index_of(block)](auto&& info)
            {
                return info.block == index && info.status == walk_to_status::step;
            };

            if(std::find_if(std::begin(m_walk_to_blocks), std::end(m_walk_to_blocks), pred) == std::end(m_walk_to_blocks))
            {
                m_walk_to_blocks.emplace_back(walk_to_info{walk_to_status::step, target, start, index_of(block)});
            }
        }

        for(const llvm::BasicBlock* pred : info_of(current_info.loop).predecessors)
        {
            m_walk_to_blocks.emplace_back(walk_to_info{walk_to_status::step, target, start, index_of(pred)});

            walk_to_recurse(target, start, pred, std::forward<Func>(func)...);

            m_walk_to_blocks.pop_back();
        }

        m_walk_to_blocks.resize(initial_size);
    }

    template<typename... Func>
    void walk_to_recurse(std::size_t target, std::size_t start, const llvm::BasicBlock* current, Func&&... func)
    {
        const value_info& target_info {m_values[target]};
        const block_info& current_info{info_of(current)};

        //This is the block where the value is defined, stop here
        if(index_of(current) == target_info.block)
        {
            m_walk_to_blocks.back().status = walk_to_status::end;

            for(const auto& info : m_walk_to_blocks)
            {
                (std::invoke(std::forward<Func>(func), info), ...);
            }

            return;
        }

        if(current_info.loop)
        {
            walk_to_loop(target, start, current, std::forward<Func>(func)...);
        }
        else
        {
            for(const llvm::BasicBlock* pred : llvm::predecessors(current))
            {
                m_walk_to_blocks.emplace_back(walk_to_info{walk_to_status::step, target, start, index_of(pred)});

                walk_to_recurse(target, start, pred, std::forward<Func>(func)...);

                m_walk_to_blocks.pop_back();
            }
        }
    }

    template<typename... Func>
    void walk_to(std::size_t target, std::size_t start, Func&&... func)
    {
        const value_info& target_info{m_values[target]};
        const value_info& start_info {m_values[start]};

        if(target_info.block == start_info.block) //Used in same block
        {
            const walk_to_info info{walk_to_status::local, target, start, start_info.block};
            (std::invoke(std::forward<Func>(func), info), ...);
        }
        else
        {
            m_walk_to_blocks.clear();
            m_walk_to_blocks.reserve(m_function.size()); // this will only allocate the first time
            m_walk_to_blocks.emplace_back(walk_to_info{walk_to_status::begin, target, start, start_info.block});

            walk_to_recurse(target, start, m_blocks[start_info.block].block, std::forward<Func>(func)...);
        }
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

    //Store values info
    void extract_values();

    //analyze value lifetime
    void compute_lifetimes();
    void fill_lifetime(std::size_t index);
    void compute_lifetime(std::size_t index, const llvm::Value* user);
    void compute_phi_lifetime(std::size_t index, const llvm::PHINode* phi);
    bool check_external_calls(std::size_t begin, std::size_t end) const;

    //give static register to all phi node operands to make them disapear in hell
    void compute_phi_groups();
    void compute_smove_groups();
    void sync_groups_members();

    //Give each value (if applicable) a spill weight
    void compute_spill_weight();
    //Give each value its register affinity
    void compute_affinity();

    //Init register info
    void fill_register_info();
    //Give each value a register
    void generate_queue();
    void allocate_registers();
    std::size_t assing_register(std::size_t value_index);

private:
    llvm::Module& m_module;
    llvm::Function& m_function;
    llvm::DominatorTree m_tree;
    llvm::LoopInfo m_loop_info;
    std::size_t m_indent{};
    std::vector<scc_info> m_sccs{};
    std::vector<block_info> m_blocks{};
    std::vector<value_info> m_values{};
    std::vector<loop_info> m_loops{};
    std::vector<group_info> m_groups{};

    std::array<register_info, 65> m_registers{};
    std::vector<stack_entry_info> m_stack{};
    std::vector<std::size_t> m_queue{};

    std::vector<walk_to_info> m_walk_to_blocks{};
};

}

#endif
