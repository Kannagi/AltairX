#ifndef ALTAIR_COMPILER_FUNCTION_ANALYSER_HPP_INCLUDED
#define ALTAIR_COMPILER_FUNCTION_ANALYSER_HPP_INCLUDED

#include "lifetime.hpp"
#include "stack_allocator.hpp"

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
    Lots of informations about values, blocks, SCCs and loops are gathered

    Compute groups for PHI nodes:
        - Loop over all PHI nodes. All of the return value of the phi instruction and the operands
          must be assigned to the same group.

    Compute lifetimes range:
        - Each value has a lifetime that contains all ranges (value indices) where the values must be alive to
          ensure value validity.
        - The ranges are computed using the following method:
          For all users, we start at the user index, then we mark down all paths that leads to the defintion.

    Value affinity:
        - Values' affinity depends on the usage, and may force to allocate specific register.
        - register_allocator::register_affinity contains details about each affinity.
*/

enum class register_affinity : std::uint32_t
{
    generic,         // This value is a variable with no special purpose
    none,            // This is a void returning value
    argument,        // This value is a function argument (of this function)
    spill,           // This value is spilled, must not be assigned to any register
    flags,           // This value is related to branching, and uses a special and unique register: FR
    ret,             // This value is part of the returned values (for this function)
    callee_argument, // This value is a function argument (for callees)
    callee_ret,      // This value is part of the returned values (for callees)
    accumulator,     // This value is should be placed in unit accumulator (unused, yet)
    bypass,          // This value is should be placed in bypass accumulator (unused, yet)
};

static std::string_view to_string(register_affinity affinity) noexcept
{
    switch(affinity)
    {
    case register_affinity::generic: return "generic";
    case register_affinity::none: return "none";
    case register_affinity::argument: return "argument";
    case register_affinity::callee_argument: return "callee_argument";
    case register_affinity::ret: return "ret";
    case register_affinity::callee_ret: return "callee_ret";
    case register_affinity::spill: return "spill";
    case register_affinity::flags: return "flags";
    case register_affinity::accumulator: return "accumulator";
    case register_affinity::bypass: return "bypass";
    default:
        return "unknown";
    }
}

inline constexpr std::size_t invalid_index{std::numeric_limits<std::size_t>::max()};
inline constexpr std::size_t no_use{std::numeric_limits<std::size_t>::max()};
inline constexpr std::size_t no_group{std::numeric_limits<std::size_t>::max()};
inline constexpr std::size_t no_register{std::numeric_limits<std::size_t>::max()};

struct value_register_affinity
{
    register_affinity affinity{register_affinity::generic}; //what kind of value it is
    // default register for this value, meansing depends on affinity
    // argument, callee_argument: index of argument [0, n[
    // default: ignored
    std::uint32_t default_register{};
};

struct allocation_info
{
    std::size_t register_index{no_register};
    double spill_weight{};
    bool need_split{};
};

struct value_info
{
    llvm::Value* value{};
    std::string name{}; // For debug only
    std::size_t block{}; // Index of parent block
    std::vector<value_register_affinity> affinities{}; // affinities of this values, may conflict
    ar::lifetime lifetime{};
    std::size_t group{no_group};
    bool leaf{true}; // if true then this value can be assigned to volatile register (i.e lifetime does not include indirect call)
    allocation_info alloc{}; // register_allocator informations
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
    llvm::Loop* loop{}; //Contains the outer most loop if any
};

struct loop_info
{
    llvm::Loop* loop{};
    std::vector<llvm::BasicBlock*> predecessors{}; //LLVM does not provide easy function for multiple predecessors
    std::vector<llvm::BasicBlock*> successors{}; //LLVM does not provide easy function for multiple successors
};

enum class group_type : std::uint32_t
{
    generic, // group as multiple usages. If it falls under "generic", it is because not much can be done with a group that as multiple usage.
    phi, // group is used to handle a phi node
    smove, // group is composed of moveu, moven and smove
    zext_trunc, // group is composed of a single instruction + zext or trunc
};

struct group_info
{
    std::vector<llvm::Value*> members{};
    ar::lifetime lifetime{}; // members coalesced lifetimes
    group_type type{}; // group type
    bool leaf{true}; // if a member is non leaf, the whole group is non leaf
    bool ret{}; // if a value of the group is used in ret instruction, the whole group is a return value
};

struct function_info
{
    bool leaf{true}; // true is every instruction in the function is leaf
};

class function_analyser
{
public:
    function_analyser(llvm::Module& module, llvm::Function& function);
    ~function_analyser() = default;
    function_analyser(const function_analyser&) = delete;
    function_analyser(function_analyser&&) = delete;
    function_analyser& operator=(const function_analyser&) = delete;
    function_analyser& operator=(function_analyser&&) = delete;

    llvm::Module& module() const noexcept
    {
        return m_module;
    }

    llvm::Function& function() const noexcept
    {
        return m_function;
    }

    //sccs accessors
    const std::vector<scc_info>& sccs() const noexcept
    {
        return m_sccs;
    }

    std::size_t index_of(const scc_info& scc) const noexcept
    {
        return &scc - std::data(m_sccs);
    }

    std::size_t get_scc(const llvm::BasicBlock* block);

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

        return invalid_index;
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

        return invalid_index;
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
    std::vector<value_info>& values() noexcept
    {
        return m_values;
    }

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

        return invalid_index;
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

    const std::vector<group_info>& groups() const noexcept
    {
        return m_groups;
    }

    const group_info* group_of(llvm::Value* value) const noexcept
    {
        if(const auto index{info_of(value).group}; index != no_group)
        {
            return &m_groups[index];
        }

        return nullptr;
    }

    group_info* group_of(llvm::Value* value) noexcept
    {
        if(const auto index{info_of(value).group}; index != no_group)
        {
            return &m_groups[index];
        }

        return nullptr;
    }

    std::size_t index_of(const group_info& group) const noexcept
    {
        return &group - std::data(m_groups);
    }

    function_info& info() noexcept
    {
        return m_info;
    }

    const function_info& info() const noexcept
    {
        return m_info;
    }

    // This function calls all analysis functions in the right order
    // It reset the whole state before doing it
    // It must be called after modifying the AST to have up to date information
    void perform_analysis();

    std::size_t insert_instruction(llvm::Instruction* inst, llvm::Instruction* position);

    // Print allocator information about current function
    void print() const;

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
        walk_to_status status{};
        std::size_t target{};
        std::size_t start{};
        std::size_t block{};
    };

private: // private helpers
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

            walk_to_recurse(target, start, pred, func...);

            m_walk_to_blocks.pop_back();
        }

        m_walk_to_blocks.resize(initial_size);
    }

    template<typename... Func>
    void walk_to_recurse(std::size_t target, std::size_t start, const llvm::BasicBlock* current, Func&&... func)
    {
        const value_info& target_info{m_values[target]};
        const block_info& current_info{info_of(current)};

        //This is the block where the value is defined, stop here
        if(index_of(current) == target_info.block)
        {
            m_walk_to_blocks.back().status = walk_to_status::end;

            for(const auto& info : m_walk_to_blocks)
            {
                (std::invoke(func, info), ...);
            }

            return;
        }

        if(current_info.loop)
        {
            walk_to_loop(target, start, current, func...);
        }
        else
        {
            for(const llvm::BasicBlock* pred : llvm::predecessors(current))
            {
                m_walk_to_blocks.emplace_back(walk_to_info{walk_to_status::step, target, start, index_of(pred)});

                walk_to_recurse(target, start, pred, func...);

                m_walk_to_blocks.pop_back();
            }
        }
    }

public:
    template<typename... Func>
    void walk_to(std::size_t target, std::size_t start, Func&&... func)
    {
        const value_info& target_info{m_values[target]};
        const value_info& start_info{m_values[start]};

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
    // Extract SSCs informations
    // SCCs are stored in preorder
    void extract_sccs();
    // Extract basic blocks informations
    // Blocks are stored in preorder
    void extract_blocks();
    // Store edges (predecessors and successors) of each scc
    void extract_scc_edges();
    // Give to all loop header the reference over the loop instance
    void extract_loops();
    void store_loop_headers();
    void store_loop_headers_recurse(llvm::Loop* loop);
    void store_loop(llvm::Loop* loop);
    // Store values info
    void extract_values();

    void fill_affinity(std::size_t index);
    void reduce_affinities(std::size_t index);
    void fill_lifetime(std::size_t index);
    void compute_lifetime(std::size_t index, const llvm::Value* user);
    void compute_phi_lifetime(std::size_t index, const llvm::PHINode* phi);
    bool check_external_calls(std::size_t begin, std::size_t end) const; // called by fill_lifetime

    // Give static register to all phi node operands to make them disapear in hell
    void extract_groups();
    void compute_phi_groups();
    void compute_smove_groups();
    void compute_zext_trunc_groups();
    void sync_groups_members();

private:
    // Context
    llvm::Module& m_module;
    llvm::Function& m_function;
    llvm::DominatorTree m_tree;
    llvm::LoopInfo m_loop_info;

    // Infomations
    std::vector<scc_info> m_sccs{};
    std::vector<block_info> m_blocks{};
    std::vector<loop_info> m_loops{};
    std::vector<value_info> m_values{};
    std::vector<group_info> m_groups{};
    function_info m_info{};

    // Internal logic
    std::vector<walk_to_info> m_walk_to_blocks{};
};

}

#endif
