#ifndef ALTAIR_COMPILER_MID_TRANSFORMS_HPP_INCLUDED
#define ALTAIR_COMPILER_MID_TRANSFORMS_HPP_INCLUDED

#include <llvm/IR/Instructions.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>

#include "register_allocator.hpp"

/*
Mid transforms are performed after register allocation analysis and before register allocation
Mid transforms must perform a new analysis before returning when they modify the AST
*/
namespace ar::mid_transforms
{

/*
Generates move instruction for constant values if needed
*/
void insert_move_for_constant(register_allocator& allocator);

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
void insert_move_for_global_load(register_allocator& allocator);


/*
Reorder instructions
*/
void optimize_pipeline(register_allocator& allocator);

}

#endif
