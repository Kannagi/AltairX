#include "transform.hpp"

#include "intrinsic.hpp"

namespace ar::transforms
{

void insert_move_for_constant(llvm::Module& module, llvm::Function& function)
{
    for(llvm::BasicBlock& block : function)
    {
        for(llvm::PHINode& phi : block.phis())
        {
            for(llvm::BasicBlock* predecessor : phi.blocks())
            {
                if(llvm::ConstantInt* value{llvm::dyn_cast<llvm::ConstantInt>(phi.getIncomingValueForBlock(predecessor))}; value)
                {
                    phi.setIncomingValueForBlock(predecessor, ar::add_move_intrinsic(module, value, predecessor->getTerminator()));
                }
            }
        }

        if(llvm::ReturnInst* ret{llvm::dyn_cast<llvm::ReturnInst>(block.getTerminator())}; ret)
        {
            if(ret->getReturnValue())
            {
                if(llvm::ConstantInt* value{llvm::dyn_cast<llvm::ConstantInt>(ret->getReturnValue())}; value)
                {
                    ret->setOperand(0, ar::add_move_intrinsic(module, value, block.getTerminator()));
                }
            }
        }
    }
}

void invert_branch_condition(llvm::Module& module [[maybe_unused]], llvm::Function& function)
{
    const auto get_inverse = [](llvm::CmpInst::Predicate predicate)
    {
        switch(predicate)
        {
        case llvm::CmpInst::Predicate::ICMP_EQ : return llvm::CmpInst::Predicate::ICMP_NE;
        case llvm::CmpInst::Predicate::ICMP_NE : return llvm::CmpInst::Predicate::ICMP_EQ;
        case llvm::CmpInst::Predicate::ICMP_UGT: return llvm::CmpInst::Predicate::ICMP_ULE;
        case llvm::CmpInst::Predicate::ICMP_UGE: return llvm::CmpInst::Predicate::ICMP_ULT;
        case llvm::CmpInst::Predicate::ICMP_ULT: return llvm::CmpInst::Predicate::ICMP_UGE;
        case llvm::CmpInst::Predicate::ICMP_ULE: return llvm::CmpInst::Predicate::ICMP_UGT;
        case llvm::CmpInst::Predicate::ICMP_SGT: return llvm::CmpInst::Predicate::ICMP_SLE;
        case llvm::CmpInst::Predicate::ICMP_SGE: return llvm::CmpInst::Predicate::ICMP_SLT;
        case llvm::CmpInst::Predicate::ICMP_SLT: return llvm::CmpInst::Predicate::ICMP_SGE;
        case llvm::CmpInst::Predicate::ICMP_SLE: return llvm::CmpInst::Predicate::ICMP_SGT;
        default:
            throw std::runtime_error{"Bad or unsuported comparison predicate " + std::to_string(predicate)};
        }
    };

    for(auto it{function.begin()}; it != function.end(); ++it)
    {
        auto branch{llvm::dyn_cast<llvm::BranchInst>(it->getTerminator())};
        if(branch && branch->isConditional())
        {
            auto next_it{it};
            ++next_it;
            const auto& next_block{*next_it};

            // check if true path leads to the following block
            // if true, then invert the condition and swap the branch
            if(branch->getSuccessor(0) == &next_block)
            {
                if(auto compare{llvm::dyn_cast<llvm::CmpInst>(branch->getCondition())}; compare)
                {
                    compare->setPredicate(get_inverse(compare->getPredicate()));

                    auto true_path{branch->getSuccessor(0)};
                    auto false_path{branch->getSuccessor(1)};

                    branch->setSuccessor(0, false_path);
                    branch->setSuccessor(1, true_path);
                }
            }
        }
    }
}

}
