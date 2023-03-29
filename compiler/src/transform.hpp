#ifndef ALTAIR_COMPILER_TRANSFORM_HPP_INCLUDED
#define ALTAIR_COMPILER_TRANSFORM_HPP_INCLUDED

#include <llvm/IR/Instructions.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>

namespace ar::transforms
{

/*
Clang, and probably LLVM optimization passes tend to put block in a kinda random
and suboptimal order leading to a lot of unneeded branches
Current implementation simply reorder them in post order, which works well in most cases
*/
void reorder_blocks(llvm::Module& module, llvm::Function& function);

/*
Replace select with normal branching, example:
  | %cond = icmp sgt i32 %a, %b
  | %value = select %cond, %a, %b

is equivalent to:
  | cmp r1, r2
  | bgs label
  | addi r3, r1, 0 ; assign true path on delay slot
  | addi r3, r2, 0 ; replace with false path if jump did not trigger
  | label:
*/
void remove_select(llvm::Module& module, llvm::Function& function);

/*
Add with negative immediate on right operand should be replace by a sub of -operand2
Sub with negative immediate on right operand should be replace by a add of -operand2
Only do this if it fits in [0; 511] (otherwise it won't have any interest since it will generates a move anyway)

Example:
| %0 = add i64 %1, -1
| %1 = sub i64 %1, -1

Will be transformed into:

| %0 = sub i64 %1, 1
| %1 = add i64 %1, 1
*/
void swap_add_sub(llvm::Module& module, llvm::Function& function);

/*
LLVM front-ends may generate code that can not translate exactly to asm directly
cmp in LLVM IR generate a bool value that can be used for integer arithmetic, or conditionnal statements (br, select, ...)
but with an Altair cmp always writes to a specific register so multiple cmp may affect each others.
Example:
|  %9  = icmp slt i32 %5, %8
|  %10 = icmp sgt i32 %5, %8
|  %11 = zext i1 %10 to i32
|  %12 = select i1 %9, i32 -1, i32 %11
|  ret i32 %12

Will be transformed into:

|  %10 = icmp sgt i32 %5, %8
|  %11 = zext i1 %10 to i32
|  %9  = icmp slt i32 %5, %8
|  %12 = select i1 %9, i32 -1, i32 %11
|  ret i32 %12
*/
void check_cmp(llvm::Module& module, llvm::Function& function);

/*
LLVM use cmp to cast integers to bools, this should be replaced by altair.bool when possible (i.e not used in br)
Example:
| %2 = icmp ne i32 %0, 0
| ret i1 %2

Will be transformed into:

| %2 = call i1 altair.bool_i32(i32 %0)
| ret i1 %2

a != b -> bool(a - b)
a == b -> bool(a - b) xor 1
a < b  -> slt(a, b)
a > b  -> slt(b, a)
a <= b -> slt(b, a) xor 1
a >= b -> slt(a, b) xor 1
*/
void bool_conversions(llvm::Module& module, llvm::Function& function);

/*
Generates move instruction for constant values if needed
*/
void insert_move_for_constant(llvm::Module& module, llvm::Function& function);

/*
Generates move instruction for global load if needed

| .data
| adr_data : "Hello World"
|
| .text
| moveu r2,adr_data&0xFFFF
| smove.w r2,r2,(adr_data&0xFFFF0000)>>16
| ld r3,0[r2]

Example:

| @global_str = internal constant [13 x i8] c"Hello world!\00", align 1
| define dso_local nonnull ptr @get_message() local_unnamed_addr #0 {
|   ret ptr @global_str
| }

Will be transformed into:

| @global_str = internal constant [13 x i8] c"Hello world!\00", align 1
| define dso_local nonnull ptr @get_message() local_unnamed_addr #0 {
|   %2 = call ptr @altair.moveu_ptr(ptr @global_str)
|   %3 = call ptr @altair.smove_ptr_ptr_i64(ptr %2, ptr @global_str, i64 1)
|   ret ptr %3
| }
*/
void insert_move_for_global_load(llvm::Module& module, llvm::Function& function);

/*
Sometimes, we may benefit from fallthrough by inverting condition
If a br jumps to a label that is a block right after the br,
we ensure that fallthrough is done by making the false path the one
that jumps to that block.

Example:

|  %5 = icmp slt i32 %0, %3
|  br i1 %5, label %6, label %12 ;true path can fallthrough
|
|6:                                                ; preds = %6, %4
|  %7 = phi i32 [ %10, %6 ], [ %0, %4 ]
|  ...

Will be transformed into:

|  %5 = icmp sge i32 %0, %3 ; "<" inverse is ">="
|  br i1 %5, label %12, label %6 ;false path can fallthrough (translator handles this)
|
|6:                                                ; preds = %6, %4
|  %7 = phi i32 [ %10, %6 ], [ %0, %4 ]
|  ...
*/
void invert_branch_condition(llvm::Module& module, llvm::Function& function);

/*
getelementptr instruction must be decomposed into additions (altair.ptradd)and multiplications
ptradd intrinsics are generated by this function since llvm does not allow ptr arithmetic through add instructions
ptradd may be negative, both contant and not, they will be translated to raw add[i].q

| %struct.RT = type { i8, [10 x [20 x i32]], i8 }
| %struct.ST = type { i32, double, %struct.RT }
| (%struct.ST* %0, i64 %1, i64 %2)
| %4 = getelementptr inbounds %struct.ST, %struct.ST* %0, i64 %1, i32 2, i32 1, i64 9, i64 %2, !dbg !36
| ret i32* %6

Will be transformed into:

| %struct.RT = type { i8, [10 x [20 x i32]], i8 }
| %struct.ST = type { i32, double, %struct.RT }
| (%struct.ST* %0, i64 %1, i64 %2)
| %off1 = mul nuw nsw i64 %1, 824 ;sizeof(%struct.ST), should check if pow of 2 to optimize by a shl log2(value)
| %4 = call %struct.ST* @altair.ptradd_structST_structST_i64(%struct.ST* %0, i64 %off1)
| %5 = call [20 x i32]* @altair.ptradd_array20xi32_structST_i64(%struct.ST* %4, i64 740)
| %off2 = shl %2, 2 ; mul by 4 (= size of i32)
| %6 = call i32* @altair.ptradd_i32_array20xi32_i64([20 x i32]* %5, i64 %off2)
| ret i32* %6
| declare %struct.ST* @altair.ptradd_struct.ST_i64(i64)
| declare i32* @altair.ptradd_i32_i64(i64)
*/
void decompose_getelementptr(llvm::Module& module, llvm::Function& function);

/*
Load and store in altair do an additional addition to the pointer before the op
this can remove adds and somtimes even moven/u + add (imm version)
*/
void optimize_load_store(llvm::Module& module, llvm::Function& function);

/*
Performs all transforms in the right order
*/
//void perform_transforms(llvm::Module& module, llvm::Function& function);

llvm::Value* find_best_position(llvm::Module& module, llvm::Function& function, llvm::Value* value);

}

#endif
