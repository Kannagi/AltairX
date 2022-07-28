#include "utilities.hpp"

namespace ar
{

std::string get_value_label(const llvm::Value& value)
{
    const auto name{value.getName()};
    if(!std::empty(name))
    {
        return name.str();
    }

    std::string output{};
    llvm::raw_string_ostream os{output};
    value.printAsOperand(os, false);
    os.flush();

    if(output == "<badref>")
    {
        return "void";
    }

    return output;
}

std::string type_name(const llvm::Type& type)
{
    std::string output{};
    llvm::raw_string_ostream os{output};
    type.print(os);
    os.flush();

    return output;
}

bool is_external_call(const llvm::CallInst& call)
{
    if(auto function{call.getCalledFunction()}; function)
    {
        if(function->isIntrinsic()) // LLVM intrinsics are internal
        {
            return false;
        }

        if(function->isDeclaration())
        {
            if(function->getName().startswith("altair.")) //Altair intrinsics are internal
            {
                return false;
            }

            return true; //External function
        }

        return false; //Internal function
    }

    return true; //Indirect call is always external
}

bool has_external_call(const llvm::Function& func)
{
    for(const llvm::BasicBlock& block : func)
    {
        if(has_external_call(block))
        {
            return true;
        }
    }

    return false;
}

bool has_external_call(const llvm::BasicBlock& block)
{
    for(const llvm::Instruction& instruction : block)
    {
        if(const llvm::CallInst* call{llvm::dyn_cast<llvm::CallInst>(&instruction)}; call)
        {
            if(is_external_call(*call))
            {
                return true;
            }
        }
    }

    return false;
}

std::size_t get_last_external_call(const llvm::BasicBlock& block)
{
    std::size_t output{};

    for(const llvm::Instruction& instruction : block)
    {
        if(const llvm::CallInst* call{llvm::dyn_cast<llvm::CallInst>(&instruction)}; call)
        {
            if(is_external_call(*call))
            {
                return true;
            }

            ++output;
        }
    }

    return false;
}

std::vector<llvm::BasicBlock*> loop_predecessors(const llvm::Loop& loop)
{
    std::vector<llvm::BasicBlock*> output{};

    if(llvm::BasicBlock* pred{loop.getLoopPredecessor()}; pred)
    {
        output.emplace_back(pred);
        return output;
    }

    for(llvm::BasicBlock* block : llvm::predecessors(loop.getHeader()))
    {
        if(!loop.contains(block))
        {
            output.emplace_back(block);
        }
    }

    return output;
}

std::vector<llvm::BasicBlock*> loop_successors(const llvm::Loop& loop)
{
    llvm::SmallVector<llvm::BasicBlock*, 4> output{};

    loop.getExitBlocks(output);

    return std::vector<llvm::BasicBlock*>{std::begin(output), std::end(output)};
}

bool is_complex_branch(llvm::Function::iterator current)
{
    if(auto branch{llvm::dyn_cast<llvm::BranchInst>(current->getTerminator())}; branch)
    {
        auto next{current};
        ++next;

        return branch->isConditional() && branch->getSuccessor(1) != &(*next);
    }

    return false;
}

}
