#ifndef ALTAIR_COMPILER_REGISTER_ALLOCATOR_HPP_INCLUDED
#define ALTAIR_COMPILER_REGISTER_ALLOCATOR_HPP_INCLUDED

#include "lifetime.hpp"
#include "stack_allocator.hpp"
#include "function_analyser.hpp"

#include <llvm/IR/Dominators.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Analysis/LoopInfo.h>

#include <variant>

namespace ar
{

class register_allocator
{
public:
    static constexpr std::size_t stack_register{0}; // stack pointer (lds/sts)
    static constexpr std::size_t args_registers_begin{1};
    static constexpr std::size_t ret_register{args_registers_begin}; // share position 
    static constexpr std::size_t args_registers_end{9};
    static constexpr std::size_t non_volatile_registers_begin{9};
    static constexpr std::size_t non_volatile_registers_end{21};
    static constexpr std::size_t volatile_registers_begin{21};
    static constexpr std::size_t volatile_registers_end{32};
    static constexpr std::size_t renamed_registers_begin{32}; // volatile
    static constexpr std::size_t renamed_registers_end{56};
    static constexpr std::size_t accumulator_register{56};
    static constexpr std::size_t flags_register{64};

    static constexpr std::size_t stack_args_count{args_registers_end - args_registers_begin};

    static constexpr double unspillable{std::numeric_limits<double>::max()};

public:
    struct register_info
    {
        ar::lifetime lifetime{};
    };

    enum class entry_type : std::uint32_t {
        argument, // argument of this function, fixed position
        link, // saved LR (needed if this function performs external calls), at most one entry can have this type, fixed position
        saved_register, // saved non-volatile register in the entire function
        local, // stack allocated value (struct, arrays...)
        spill, // temporary value stored on stack to free registers
        callee_argument, // argument of called function in this function, fixed position (depends of called function)
    };

    struct stack_entry
    {
        entry_type type{}; // may affect allocation and meaning of other fields
        std::size_t size{}; // bytes (octets) size of entry
        std::size_t alignment{}; // alignment of entry in bytes (octets)
        // Position of fixed-position entries
        // - argument: offset from initial stack pointer at function begginning
        // - link: offset from initial stack pointer at function begginning
        // - callee arguments: offset from current stack pointer
        std::ptrdiff_t position{};
        ar::lifetime lifetime{}; // lifetime on stack
        llvm::Value* value{}; // value on stack
        llvm::CallInst* spill{}; // spill that generated this entry, if any
    };

    struct stack_info
    {
        bool exists{};
        llvm::Constant* push_size{}; // pointer to the constant int that decrement the stack pointer at the beggining of the function
        std::vector<llvm::Instruction*> header{}; // instructions of the generated header for stack management
        std::vector<std::vector<llvm::Instruction*>> footers{}; // instructions of each generated footer for stack management (basically all ret in the function)
    };

public:
    register_allocator(function_analyser& analyser);
    ~register_allocator() = default;
    register_allocator(const register_allocator&) = delete;
    register_allocator(register_allocator&&) = delete;
    register_allocator& operator=(const register_allocator&) = delete;
    register_allocator& operator=(register_allocator&&) = delete;

    function_analyser& analyser() noexcept
    {
        return m_analyser;
    }

    const function_analyser& analyser() const noexcept
    {
        return m_analyser;
    }

    const std::vector<stack_entry>& stack_entries() const noexcept
    {
        return m_stack;
    }

    const stack_entry* stack_entry_of(llvm::Value* value) const noexcept
    {
        for(const auto& entry : m_stack)
        {
            if(entry.value == value || entry.spill == value)
            {
                return &entry;
            }
        }

        return nullptr;
    }

    stack_entry* stack_entry_of(llvm::Value* value) noexcept
    {
        for(auto& entry : m_stack)
        {
            if(entry.value == value || entry.spill == value)
            {
                return &entry;
            }
        }

        return nullptr;
    }

    // Perform the actual register allocation
    void perform_register_allocation();

private:
    // Give each value (if applicable) a spill weight
    void compute_spill_weight(std::size_t index);
    void compute_spill_weight();
    // Give each value a register
    bool allocation_priority_predicate(std::size_t left, std::size_t right);
    void generate_queue();
    void insert_queue(std::size_t value_index);
    // Insert an instruction during register allocation keeping required context valid
    // Return instruction index in m_values
    void insert_instruction(llvm::Instruction* inst, llvm::Instruction* position);
    // preprocess
    void assign_group_register(const group_info& group);
    // generate stack push/pop
    void generate_stack_structure();
    void generate_link_save();
    // Entry point of main register allocation algorithm
    void allocate_registers();
    void allocate_register(std::size_t value_index);
    std::size_t assing_register(std::size_t value_index);

private:
    function_analyser& m_analyser;
    llvm::Module& m_module;
    llvm::Function& m_function;

    std::array<register_info, 65> m_registers{};
    std::vector<std::size_t> m_queue{};
    std::vector<stack_entry> m_stack{};
    stack_info m_stack_info{};
};

}

#endif
