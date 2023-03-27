#ifndef ALTAIR_COMPILER_TRANSLATOR_HPP_INCLUDED
#define ALTAIR_COMPILER_TRANSLATOR_HPP_INCLUDED

#include "register_allocator.hpp"

#include <sstream>

namespace ar
{

class function_translator
{
    static constexpr std::string_view indent{"    "};

public:
    function_translator(llvm::Module& module, llvm::Function& function, const register_allocator& allocator);

    ~function_translator() = default;
    function_translator(const function_translator&) = delete;
    function_translator(function_translator&&) = delete;
    function_translator& operator=(const function_translator&) = delete;
    function_translator& operator=(function_translator&&) = delete;

    std::string translate();

private:
    // For branch instruction, when conditional, only generate the first jump (true path),
    // the false path is generated in caller because it needs more context
    void translate_instruction(const register_allocator::value_info& instruction);
    void delay_slot();
    void branch_to(const llvm::BasicBlock* block);

    std::string get_opcode(const llvm::BinaryOperator* binary);
    std::string translate_instruction(const llvm::ICmpInst* compare);
    std::string translate_instruction(const llvm::ZExtInst* zext);
    std::string translate_instruction(const llvm::TruncInst* trunc);
    std::string translate_instruction(const llvm::SExtInst* sext);
    std::string translate_instruction(const llvm::LoadInst* load);
    std::string translate_instruction(const llvm::StoreInst* store);
    std::string translate_instruction(const llvm::BranchInst* branch, const llvm::CmpInst* compare);

    std::string get_block_label(const register_allocator::block_info& block);
    std::string get_register(llvm::Value* value);
    std::string get_size_name(llvm::Value* value);

private:
    llvm::Module& m_module;
    llvm::Function& m_function;
    const register_allocator& m_allocator;
    std::ostringstream m_asm_code;
};

}

#endif
