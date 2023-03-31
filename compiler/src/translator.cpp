#include "translator.hpp"

#include <sstream>

#include "intrinsic.hpp"
#include "utilities.hpp"
#include "early_transforms.hpp"

#include <llvm/IR/Constants.h>

/*
// Terminator Instructions - These instructions are used to terminate a basic
// block of the program.   Every basic block must end with one of these
// instructions for it to be a well formed basic block.
Ret           , ReturnInst
Br            , BranchInst
Switch        , SwitchInst
IndirectBr    , IndirectBrInst
Invoke        , InvokeInst
Resume        , ResumeInst
Unreachable   , UnreachableInst
CleanupRet    , CleanupReturnInst
CatchRet      , CatchReturnInst
CatchSwitch   , CatchSwitchInst
CallBr        , CallBrInst // A call-site terminator

// Standard unary operators...
FNeg  , UnaryOperator

// Standard binary operators...
Add  , BinaryOperator
FAdd , BinaryOperator
Sub  , BinaryOperator
FSub , BinaryOperator
Mul  , BinaryOperator
FMul , BinaryOperator
UDiv , BinaryOperator
SDiv , BinaryOperator
FDiv , BinaryOperator
URem , BinaryOperator
SRem , BinaryOperator
FRem , BinaryOperator
// Logical operators (integer operands)
Shl  , BinaryOperator // Shift left  (logical)
LShr , BinaryOperator // Shift right (logical)
AShr , BinaryOperator // Shift right (arithmetic)
And  , BinaryOperator
Or   , BinaryOperator
Xor  , BinaryOperator

// Memory operators...
Alloca, AllocaInst  // Stack management
Load  , LoadInst    // Memory manipulation instrs
Store , StoreInst
GetElementPtr, GetElementPtrInst
Fence , FenceInst
AtomicCmpXchg , AtomicCmpXchgInst
AtomicRMW , AtomicRMWInst

// Cast operators ...
Trunc   , TruncInst     // Truncate integers
ZExt    , ZExtInst      // Zero extend integers
SExt    , SExtInst      // Sign extend integers
FPToUI  , FPToUIInst    // floating point -> UInt
FPToSI  , FPToSIInst    // floating point -> SInt
UIToFP  , UIToFPInst    // UInt -> floating point
SIToFP  , SIToFPInst    // SInt -> floating point
FPTrunc , FPTruncInst   // Truncate floating point
FPExt   , FPExtInst     // Extend floating point
PtrToInt, PtrToIntInst  // Pointer -> Integer
IntToPtr, IntToPtrInst  // Integer -> Pointer
BitCast , BitCastInst   // Type cast
AddrSpaceCast, AddrSpaceCastInst  // addrspace cast

CleanupPad, CleanupPadInst
CatchPad  , CatchPadInst

// Other operators...
ICmp   , ICmpInst     // Integer comparison instruction
FCmp   , FCmpInst     // Floating point comparison instr.
PHI    , PHINode      // PHI node instruction
Call   , CallInst     // Call a function
Select , SelectInst   // select instruction
UserOp1, Instruction  // May be used internally in a pass
UserOp2, Instruction  // Internal to passes only
VAArg  , VAArgInst    // vaarg instruction
ExtractElement, ExtractElementInst // extract from vector
InsertElement, InsertElementInst   // insert into vector
ShuffleVector, ShuffleVectorInst   // shuffle two vectors.
ExtractValue, ExtractValueInst     // extract from aggregate
InsertValue, InsertValueInst       // insert into aggregate
LandingPad, LandingPadInst         // Landing pad instruction.
Freeze, FreezeInst                 // Freeze instruction.
*/

namespace ar
{

function_translator::function_translator(const register_allocator& allocator)
:m_module{allocator.module()}
,m_function{allocator.function()}
,m_allocator{allocator}
{

}

std::string function_translator::translate()
{
    m_asm_code = std::ostringstream{};
    m_asm_code << m_function.getName().str() << ":" << std::endl;

    const auto end{m_function.end()};
    for(auto it{m_function.begin()}; it != end; ++it)
    {
        const auto& block{m_allocator.info_of(&(*it))};

        const llvm::BasicBlock* next_block{};
        if(auto next{it}; ++next != end)
        {
            next_block = &(*next);
        }

        m_asm_code << get_block_label(block) << ":" << std::endl;

        // Phi nodes will be implicitely removed thanks to register allocation
        const std::size_t block_size{block.end - block.begin_no_phi};

        if(block_size == 1) // only a terminator
        {
            auto branch{llvm::dyn_cast<llvm::BranchInst>(m_allocator.values()[block.begin_no_phi].value)};
            if(!branch || branch->isConditional() || branch->getSuccessor(0) != next_block)
            {
                translate_instruction(m_allocator.values()[block.begin_no_phi]);
                delay_slot();
            }
        }
        else
        {
            if(block_size > 2)
            {
                for(std::size_t i{block.begin_no_phi}; i < block.end - 2; ++i)
                {
                    translate_instruction(m_allocator.values()[i]);
                }
            }

            // Swap instruction to make the last instruction run during delay slot only if it is not a cmp
            if(!llvm::isa<llvm::CmpInst>(m_allocator.values()[block.end - 2].value))
            {
                // Only branch is necessary
                auto branch{llvm::dyn_cast<llvm::BranchInst>(m_allocator.values()[block.end - 1].value)};
                if(!branch || branch->isConditional() || branch->getSuccessor(0) != next_block)
                {
                    translate_instruction(m_allocator.values()[block.end - 1]);
                }

                translate_instruction(m_allocator.values()[block.end - 2]);
            }
            else
            {
                translate_instruction(m_allocator.values()[block.end - 2]);
                translate_instruction(m_allocator.values()[block.end - 1]);
                delay_slot();
            }

            // Complex branch is a br with a false path that need a real jump instead of a fallthrough
            // translate_instruction() only covers the true (or unconditionnal) path, the false path is handled here
            if(is_complex_branch(it, end))
            {
                branch_to(it->getTerminator()->getSuccessor(1));
                delay_slot();
            }
        }

        m_asm_code << std::endl;
    }

    return m_asm_code.str();
}

void function_translator::delay_slot()
{
    m_asm_code << indent << "nop" << std::endl;
}

void function_translator::branch_to(const llvm::BasicBlock* block)
{
    m_asm_code << indent << "bra " << get_block_label(m_allocator.info_of(block)) << std::endl;
}

void function_translator::translate_instruction(const register_allocator::value_info& instruction)
{
    //Assumed for most op:
    //  - only the second operator may be a constant
    //    - if it is, then it is already in the valid range (x bits)
    //      and signed (no instruction can go higher than 20ish bits)
    //  - otherwise both are registers

    if(auto binary{llvm::dyn_cast<llvm::BinaryOperator>(instruction.value)}; binary)
    {
        if(llvm::ConstantInt* contant{llvm::dyn_cast<llvm::ConstantInt>(binary->getOperand(1))}; contant)
        {
            m_asm_code << indent;
            m_asm_code << get_opcode(binary) << "i." << get_int_size_name(binary) << " ";
            m_asm_code << get_operand(binary) << ", ";
            m_asm_code << get_operand(binary->getOperand(0)) << ", ";
            m_asm_code << std::to_string(contant->getSExtValue()) << std::endl;
        }
        else
        {
            m_asm_code << indent;
            m_asm_code << get_opcode(binary) << "." << get_int_size_name(binary) << " ";
            m_asm_code << get_operand(binary) << ", ";
            m_asm_code << get_operand(binary->getOperand(0)) << ", ";
            m_asm_code << get_operand(binary->getOperand(1)) << std::endl;
        }

    }
    else if(auto zext{llvm::dyn_cast<llvm::ZExtInst>(instruction.value)}; zext)
    {
        // this op is a no-op on our arch
        // zero-extending a register is simply changing the operand size on the user
        m_asm_code << indent << "; "; // NOP (commented "debug information") !!
        m_asm_code << translate_instruction(zext) << "." << get_int_size_name(zext) << " ";
        m_asm_code << get_operand(zext) << ", ";
        m_asm_code << get_operand(zext->getOperand(0)) << std::endl;
    }
    else if(auto trunc{llvm::dyn_cast<llvm::TruncInst>(instruction.value)}; trunc)
    {
        // this op is a no-op on our arch
        // trunc-ing a register is simply changing the operand size on the user
        m_asm_code << indent << "; "; // NOP (commented "debug information") !!
        m_asm_code << translate_instruction(trunc) << "." << get_int_size_name(trunc) << " ";
        m_asm_code << get_operand(trunc) << ", ";
        m_asm_code << get_operand(trunc->getOperand(0)) << std::endl;
    }
    else if(auto sext{llvm::dyn_cast<llvm::SExtInst>(instruction.value)}; sext)
    {
        m_asm_code << indent;
        m_asm_code << translate_instruction(sext) << "." << get_int_size_name(sext) << " ";
        m_asm_code << get_operand(sext) << ", ";
        m_asm_code << get_operand(sext->getOperand(0)) << std::endl;
    }
    else if(auto load{llvm::dyn_cast<llvm::LoadInst>(instruction.value)}; load)
    {
        m_asm_code << indent;
        m_asm_code << translate_instruction(load) << "." << get_int_size_name(load) << " ";
        m_asm_code << get_operand(load) << ", ";
        m_asm_code << get_operand(load->getPointerOperand()) << "[0]" << std::endl;
    }
    else if(auto store{llvm::dyn_cast<llvm::StoreInst>(instruction.value)}; store)
    {
        m_asm_code << indent;
        m_asm_code << translate_instruction(store) << "." << get_int_size_name(store->getValueOperand()) << " ";
        m_asm_code << get_operand(store->getValueOperand()) << ", ";
        m_asm_code << get_operand(store->getPointerOperand()) << "[0]"<< std::endl;
    }
    else if(auto compare{llvm::dyn_cast<llvm::ICmpInst>(instruction.value)}; compare)
    {
        if(llvm::ConstantInt* contant{llvm::dyn_cast<llvm::ConstantInt>(compare->getOperand(1))}; contant)
        {
            const std::int64_t value{contant->getSExtValue()};
            if(value < 0) // signed
            {
                m_asm_code << indent;
                m_asm_code << translate_instruction(compare) << "i ";
                m_asm_code << get_operand(compare->getOperand(0)) << ", ";
                m_asm_code << std::to_string(value) << std::endl;
            }
            else
            {
                m_asm_code << indent;
                m_asm_code << translate_instruction(compare) + "iu ";
                m_asm_code << get_operand(compare->getOperand(0)) + ", ";
                m_asm_code << std::to_string(value) << std::endl;
            }
        }
        else
        {
            m_asm_code << indent;
            m_asm_code << translate_instruction(compare) + " ";
            m_asm_code << get_operand(compare->getOperand(0)) + ", ";
            m_asm_code << get_operand(compare->getOperand(1)) << std::endl;
        }

    }
    else if(auto branch{llvm::dyn_cast<llvm::BranchInst>(instruction.value)}; branch)
    {
        if(branch->isConditional())
        {
            m_asm_code << indent;
            m_asm_code << translate_instruction(branch, llvm::dyn_cast<llvm::CmpInst>(branch->getCondition())) << " ";
            m_asm_code << get_block_label(m_allocator.info_of(branch->getSuccessor(0))) << std::endl;
        }
        else
        {
            m_asm_code << indent;
            m_asm_code << translate_instruction(branch, nullptr) << " ";
            m_asm_code << get_block_label(m_allocator.info_of(branch->getSuccessor(0))) << std::endl;
        }
    }
    else if(auto call{llvm::dyn_cast<llvm::CallInst>(instruction.value)}; call)
    {
        const auto intrinsic{get_intrinsic_id(*call)};

        if(intrinsic == intrinsic_id::moven)
        {
            m_asm_code << indent;
            m_asm_code << "moven " << get_operand(call) << ", ";
            m_asm_code << llvm::cast<llvm::ConstantInt>(call->getArgOperand(0))->getSExtValue() << std::endl;
        }
        else if(intrinsic == intrinsic_id::moveu)
        {
            m_asm_code << indent;
            m_asm_code << "moveu " << get_operand(call) << ", ";

            if(auto int_value{llvm::dyn_cast<llvm::ConstantInt>(call->getArgOperand(0))}; int_value)
            {
                m_asm_code << int_value->getZExtValue() << std::endl;
            }
            else if(auto global_value{llvm::dyn_cast<llvm::GlobalVariable>(call->getArgOperand(0))}; global_value)
            {
                m_asm_code << get_value_label(*global_value) << "&0xFFFF" << std::endl;
            }
        }
        else if(intrinsic == intrinsic_id::smove)
        {
            constexpr std::array shift_name{"b", "w", "l", "q"};

            assert(m_allocator.info_of(call).register_index == m_allocator.info_of(call->getArgOperand(0)).register_index);

            const auto shift_value{llvm::cast<llvm::ConstantInt>(call->getArgOperand(2))->getZExtValue()};
            m_asm_code << indent;
            m_asm_code << "smove." << shift_name[shift_value] << " ";
            m_asm_code << get_operand(call) << ", ";

            if(auto int_value{llvm::dyn_cast<llvm::ConstantInt>(call->getArgOperand(1))}; int_value)
            {
                m_asm_code << int_value->getZExtValue() << std::endl;
            }
            else if(auto global_value{llvm::dyn_cast<llvm::GlobalVariable>(call->getArgOperand(1))}; global_value)
            {
                if(shift_value != 1)
                {
                    throw std::runtime_error{"Can not compose addresse on more than 32-bits"};
                }

                m_asm_code << "(" << get_value_label(*global_value) << "&0xFFFF0000)>>16" << std::endl;
            }
        }
        else if(intrinsic == intrinsic_id::load)
        {
            if(llvm::ConstantInt* contant{llvm::dyn_cast<llvm::ConstantInt>(call->getOperand(1))}; contant)
            {
                m_asm_code << indent;
                m_asm_code << "ldi." << get_int_size_name(call) << " ";
                m_asm_code << get_operand(call) << ", ";
                m_asm_code << get_operand(call->getArgOperand(0)) << "[";
                m_asm_code << std::to_string(contant->getSExtValue()) << "]" << std::endl;
            }
            else
            {
                m_asm_code << indent;
                m_asm_code << "ld." << get_int_size_name(call) << " ";
                m_asm_code << get_operand(call) << ", ";
                m_asm_code << get_operand(call->getArgOperand(0)) << "[";
                m_asm_code << get_operand(call->getArgOperand(1)) << "]" << std::endl;
            }
        }
        else if(intrinsic == intrinsic_id::store)
        {
            if(llvm::ConstantInt* contant{llvm::dyn_cast<llvm::ConstantInt>(call->getOperand(1))}; contant)
            {
                m_asm_code << indent;
                m_asm_code << "sti." << get_int_size_name(call->getArgOperand(2)) << " ";
                m_asm_code << get_operand(call->getArgOperand(2)) << ", ";
                m_asm_code << get_operand(call->getArgOperand(0)) << "[";
                m_asm_code << std::to_string(contant->getSExtValue()) << "]" << std::endl;
            }
            else
            {
                m_asm_code << indent;
                m_asm_code << "st." << get_int_size_name(call->getArgOperand(2)) << " ";
                m_asm_code << get_operand(call->getArgOperand(2)) << ", ";
                m_asm_code << get_operand(call->getArgOperand(0)) << "[";
                m_asm_code << get_operand(call->getArgOperand(1)) << "]" << std::endl;
            }
        }
        else if(intrinsic == intrinsic_id::spill)
        {

        }
        else if(intrinsic == intrinsic_id::fill)
        {

        }
        else if(intrinsic == intrinsic_id::ptradd)
        {
            if(llvm::ConstantInt* contant{llvm::dyn_cast<llvm::ConstantInt>(call->getOperand(1))}; contant)
            {
                m_asm_code << indent;
                m_asm_code << "addi.q ";
                m_asm_code << get_operand(call) << ", ";
                m_asm_code << get_operand(call->getArgOperand(0)) << ", ";
                m_asm_code << std::to_string(contant->getSExtValue()) << std::endl;
            }
            else
            {
                m_asm_code << indent;
                m_asm_code << "add.q ";
                m_asm_code << get_operand(call) << ", ";
                m_asm_code << get_operand(call->getArgOperand(0)) << ", ";
                m_asm_code << get_operand(call->getArgOperand(1)) << std::endl;
            }
        }
        else
        {
            m_asm_code << indent;
            m_asm_code << "<unimplemented intrinsic>" << std::endl;
        }
    }
    else if(auto ret{llvm::dyn_cast<llvm::ReturnInst>(instruction.value)}; ret)
    {
        m_asm_code << indent;
        m_asm_code << "ret" << std::endl;
    }
    else
    {
        m_asm_code << indent << "<unimplemented>" << std::endl;
    }
}

std::string function_translator::get_opcode(const llvm::BinaryOperator* binary)
{
    switch(binary->getOpcode())
    {
    case llvm::BinaryOperator::BinaryOps::Add : return "add";
    //case llvm::BinaryOperator::BinaryOps::FAdd: return "";
    case llvm::BinaryOperator::BinaryOps::Sub : return "sub";
    //case llvm::BinaryOperator::BinaryOps::FSub: return "";
    case llvm::BinaryOperator::BinaryOps::Mul : return "mul";
    //case llvm::BinaryOperator::BinaryOps::FMul: return "";
    case llvm::BinaryOperator::BinaryOps::UDiv: return "divu";
    case llvm::BinaryOperator::BinaryOps::SDiv: return "divs";
    //case llvm::BinaryOperator::BinaryOps::FDiv: return "";
    case llvm::BinaryOperator::BinaryOps::URem: return "remu";
    case llvm::BinaryOperator::BinaryOps::SRem: return "srem";
    //case llvm::BinaryOperator::BinaryOps::FRem: return "";
    case llvm::BinaryOperator::BinaryOps::Shl : return "lsl";
    case llvm::BinaryOperator::BinaryOps::LShr: return "lsr";
    case llvm::BinaryOperator::BinaryOps::AShr: return "asl";
    case llvm::BinaryOperator::BinaryOps::And : return "and";
    case llvm::BinaryOperator::BinaryOps::Or  : return "or";
    case llvm::BinaryOperator::BinaryOps::Xor : return "xor";
    default:
        throw std::runtime_error{"Bad or unsuported opcode for binary operation: " + std::to_string(binary->getOpcode())};
    }
}

std::string function_translator::translate_instruction(const llvm::ZExtInst* zext [[maybe_unused]])
{
    return "zext";
}

std::string function_translator::translate_instruction(const llvm::TruncInst* trunc [[maybe_unused]])
{
    return "trunc";
}

std::string function_translator::translate_instruction(const llvm::SExtInst* sext [[maybe_unused]])
{
    return "sext";
}

std::string function_translator::translate_instruction(const llvm::LoadInst* load [[maybe_unused]])
{
    return "ldi";
}

std::string function_translator::translate_instruction(const llvm::StoreInst* store [[maybe_unused]])
{
    return "sti";
}

std::string function_translator::translate_instruction(const llvm::ICmpInst* compare [[maybe_unused]])
{
    return "cmp";
}

std::string function_translator::translate_instruction(const llvm::BranchInst* branch, const llvm::CmpInst* compare)
{
    if(branch->isConditional())
    {
        assert(compare && "compare must be a valid llvm::CmpInst if branch is conditional");

        switch(compare->getPredicate())
        {
        case llvm::CmpInst::Predicate::ICMP_EQ : return "beq";
        case llvm::CmpInst::Predicate::ICMP_NE : return "bne";
        case llvm::CmpInst::Predicate::ICMP_UGT: return "bg";
        case llvm::CmpInst::Predicate::ICMP_UGE: return "bge";
        case llvm::CmpInst::Predicate::ICMP_ULT: return "bl";
        case llvm::CmpInst::Predicate::ICMP_ULE: return "ble";
        case llvm::CmpInst::Predicate::ICMP_SGT: return "bgs";
        case llvm::CmpInst::Predicate::ICMP_SGE: return "bges";
        case llvm::CmpInst::Predicate::ICMP_SLT: return "bls";
        case llvm::CmpInst::Predicate::ICMP_SLE: return "bles";
        default:
            throw std::runtime_error{"Bad or unsuported comparison predicate " + std::to_string(compare->getPredicate())};
        }
    }
    else
    {
        return "bra";
    }
}

std::string function_translator::get_block_label(const register_allocator::block_info& block)
{
    return m_function.getName().str() + "_" + get_value_label(*block.block);
}

std::string function_translator::get_operand(llvm::Value* value)
{
    if(auto global{llvm::dyn_cast<llvm::GlobalValue>(value)}; global)
    {
        return get_value_label(*global);
    }

    return "r" + std::to_string(m_allocator.info_of(value).register_index);
}

}
