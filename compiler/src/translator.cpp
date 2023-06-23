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

function_translator::function_translator(const register_allocator& allocator, const compiler_options& options)
:m_module{allocator.module()}
,m_function{allocator.function()}
,m_allocator{allocator}
,m_options{options}
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

    if(llvm::isa<llvm::BinaryOperator>(instruction.value))
    {
        auto binary{llvm::cast<llvm::BinaryOperator>(instruction.value)};

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
    else if(llvm::isa<llvm::ZExtInst>(instruction.value))
    {
        auto zext{llvm::cast<llvm::ZExtInst>(instruction.value)};

        // this op is a no-op on our arch
        // zero-extending a register is simply changing the operand size on the user
        if(m_options.output_noop_instructions)
        {
            m_asm_code << indent << "; "; // comment line
            m_asm_code << translate_instruction(zext) << "." << get_int_size_name(zext) << " ";
            m_asm_code << get_operand(zext) << ", ";
            m_asm_code << get_operand(zext->getOperand(0)) << std::endl;
        }
    }
    else if(llvm::isa<llvm::TruncInst>(instruction.value))
    {
        auto trunc{llvm::cast<llvm::TruncInst>(instruction.value)};

        // this op is a no-op on our arch
        // trunc-ing a register is simply changing the operand size on the user
        if(m_options.output_noop_instructions)
        {
            m_asm_code << indent << "; "; // NOP (commented "debug information") !!
            m_asm_code << translate_instruction(trunc) << "." << get_int_size_name(trunc) << " ";
            m_asm_code << get_operand(trunc) << ", ";
            m_asm_code << get_operand(trunc->getOperand(0)) << std::endl;
        }
    }
    else if(llvm::isa<llvm::SExtInst>(instruction.value))
    {
        auto sext{llvm::cast<llvm::SExtInst>(instruction.value)};

        m_asm_code << indent;
        m_asm_code << translate_instruction(sext) << "." << get_int_size_name(sext) << " ";
        m_asm_code << get_operand(sext) << ", ";
        m_asm_code << get_operand(sext->getOperand(0)) << std::endl;
    }
    else if(llvm::isa<llvm::LoadInst>(instruction.value))
    {
        auto load{llvm::cast<llvm::LoadInst>(instruction.value)};

        m_asm_code << indent;
        m_asm_code << translate_instruction(load) << "." << get_int_size_name(load) << " ";
        m_asm_code << get_operand(load) << ", ";
        m_asm_code << get_operand(load->getPointerOperand()) << "[0]" << std::endl;
    }
    else if(llvm::isa<llvm::StoreInst>(instruction.value))
    {
        auto store{llvm::cast<llvm::StoreInst>(instruction.value)};

        m_asm_code << indent;
        m_asm_code << translate_instruction(store) << "." << get_int_size_name(store->getValueOperand()) << " ";
        m_asm_code << get_operand(store->getValueOperand()) << ", ";
        m_asm_code << get_operand(store->getPointerOperand()) << "[0]"<< std::endl;
    }
    else if(llvm::isa<llvm::ICmpInst>(instruction.value))
    {
        auto compare{llvm::dyn_cast<llvm::ICmpInst>(instruction.value)};

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
    else if(llvm::isa<llvm::BranchInst>(instruction.value))
    {
        auto branch{llvm::cast<llvm::BranchInst>(instruction.value)};
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
    else if(llvm::isa<llvm::CallInst>(instruction.value))
    {
        auto call{llvm::cast<llvm::CallInst>(instruction.value)};
    
        if(is_intrinsic(*call))
        {
            translate_intrinsic(call);
        }
        else
        {
            m_asm_code << indent;
            m_asm_code << "call " << call->getCalledFunction()->getName().str() << std::endl;
        }
    }
    else if(llvm::isa<llvm::ReturnInst>(instruction.value))
    {
        auto ret{llvm::cast<llvm::ReturnInst>(instruction.value)};

        m_asm_code << indent;
        m_asm_code << "ret" << std::endl;
    }
    else
    {
        m_asm_code << indent << "<unimplemented>" << std::endl;
    }
}

void function_translator::translate_intrinsic(llvm::CallInst* call)
{
    switch(get_intrinsic_id(*call)) // use switch without default to get warning when adding new intrinsic
    {
        case intrinsic_id::copy:
        {
            m_asm_code << indent;
            m_asm_code << "addi." << get_int_size_name(call) << " ";
            m_asm_code << get_operand(call) << ", ";
            m_asm_code << get_operand(call->getOperand(0)) << ", 0 ; copy" << std::endl;

            break;
        }
        case intrinsic_id::moven:
        {
            m_asm_code << indent;
            m_asm_code << "moven " << get_operand(call) << ", ";
            m_asm_code << llvm::cast<llvm::ConstantInt>(call->getArgOperand(0))->getSExtValue() << std::endl;

            break;
        }
        case intrinsic_id::moveu:
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

            break;
        }
        case intrinsic_id::smove:
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

            break;
        }
        case intrinsic_id::getri:
        {
            constexpr std::array ri_names{"LR", "FR", "BR", "IR", "LC", "IC", "CC", "PC"};

            llvm::ConstantInt* index{llvm::dyn_cast<llvm::ConstantInt>(call->getOperand(0))};
            assert(index->getZExtValue() < std::size(ri_names));

            m_asm_code << indent;
            m_asm_code << "move ";
            m_asm_code << get_operand(call) << ", ";
            m_asm_code << ri_names[index->getZExtValue()] << std::endl;

            break;
        }
        case intrinsic_id::setri:
        {
            constexpr std::array ri_names{"LR", "FR", "BR", "IR", "LC", "IC", "CC", "PC"};

            llvm::ConstantInt* index{llvm::dyn_cast<llvm::ConstantInt>(call->getOperand(0))};
            assert(index->getZExtValue() < std::size(ri_names));

            m_asm_code << indent;
            m_asm_code << "move ";
            m_asm_code << ri_names[index->getZExtValue()] << ", ";
            m_asm_code << get_operand(call->getOperand(1)) << std::endl;

            break;
        }
        case intrinsic_id::load:
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

            break;
        }
        case intrinsic_id::store:
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

            break;
        }
        case intrinsic_id::ptradd:
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

            break;
        }
        case intrinsic_id::spill:
        {
            auto value{call->getArgOperand(0)};
            auto entry{m_allocator.stack_entry_of(value)};
            assert(entry && "Spill argument must have an associated stack entry.");

            m_asm_code << indent;
            m_asm_code << "sps." << get_int_size_name(value) << " ";
            m_asm_code << get_operand(value) << ", ";
            m_asm_code << std::to_string(m_stack_offset + entry->position) << std::endl;

            break;
        }
        case intrinsic_id::fill:
        {
            auto value{call->getArgOperand(0)};
            auto entry{m_allocator.stack_entry_of(value)};
            assert(entry && "Fill argument must have an associated stack entry.");

            m_asm_code << indent;
            m_asm_code << "lds." << get_int_size_name(entry->size) << " "; // use entry size to handle both spill and other types of stack entries
            m_asm_code << get_operand(call) << ", ";
            m_asm_code << std::to_string(m_stack_offset + entry->position) << std::endl;

            break;
        }
        case intrinsic_id::push:
        {
            llvm::ConstantInt* bytes{llvm::dyn_cast<llvm::ConstantInt>(call->getOperand(0))};

            m_asm_code << indent;
            m_asm_code << "subi.q sp, sp, ";
            m_asm_code << std::to_string(bytes->getZExtValue()) << std::endl;

            m_stack_offset += bytes->getZExtValue();

            break;
        }
        case intrinsic_id::pop:
        {
            llvm::ConstantInt* bytes{llvm::dyn_cast<llvm::ConstantInt>(call->getOperand(0))};

            m_asm_code << indent;
            m_asm_code << "addi.q sp, sp, ";
            m_asm_code << std::to_string(bytes->getZExtValue()) << std::endl;

            m_stack_offset -= bytes->getZExtValue();

            break;
        }
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

    const auto register_index{m_allocator.info_of(value).register_index};
    
    if(m_options.pretty_registers)
    {
        if(register_index == register_allocator::stack_register)
        {
            return "sp";
        }
        else if(register_index >= register_allocator::args_registers_begin && register_index < register_allocator::args_registers_end)
        {
            return "a" + std::to_string(register_index - register_allocator::args_registers_begin);
        }
        else if(register_index >= register_allocator::non_volatile_registers_begin && register_index < register_allocator::non_volatile_registers_end)
        {
            return "s" + std::to_string(register_index - register_allocator::non_volatile_registers_begin);
        }
        else if(register_index >= register_allocator::volatile_registers_begin && register_index < register_allocator::volatile_registers_end)
        {
            return "t" + std::to_string(register_index - register_allocator::volatile_registers_begin);
        }
        else if(register_index >= register_allocator::renamed_registers_begin && register_index < register_allocator::renamed_registers_end)
        {
            return "n" + std::to_string(register_index - register_allocator::volatile_registers_begin);
        }
        else if(register_index == register_allocator::accumulator_register)
        {
            return "acc";
        }
    }

    return "r" + std::to_string(register_index);
}

}
