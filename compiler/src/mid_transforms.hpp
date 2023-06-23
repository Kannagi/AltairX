#ifndef ALTAIR_COMPILER_MID_TRANSFORMS_HPP_INCLUDED
#define ALTAIR_COMPILER_MID_TRANSFORMS_HPP_INCLUDED

#include <llvm/IR/Instructions.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>

#include "function_analyser.hpp"

/*
Mid transforms are performed after register allocation analysis and before register allocation
Mid transforms must perform a new analysis before returning when they modify the AST
*/
namespace ar::mid_transforms
{

/*
Generates move instruction for constant values if needed
*/
void insert_move_for_constant(function_analyser& analyser);

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
void insert_move_for_global_load(function_analyser& analyser);

/*
Find and correct overlapping phi nodes

In the same group the lifetime of the values must not overlap
if there is an overlap the new value must be stored elsewhere and restored later

| define dso_local ptr @func(ptr noundef %0, ptr noundef readnone %1) local_unnamed_addr #8 {
|   %3 = icmp eq ptr %0, %1
|   br i1 %3, label %14, label %4
| 4:                                                ; preds = %2, %4
|   %5 = phi ptr [ %12, %4 ], [ %0, %2 ]
|   %12 = call ptr @someop(ptr %5)
|   br i1 %13, label %14, label %4, !llvm.loop !23
| 14:                                               ; preds = %4, %2
|   %15 = phi ptr [ %0, %2 ], [ %1, %4 ]
|   ret ptr %15
| }

Become:
| define dso_local ptr @func(ptr noundef %0, ptr noundef readnone %1) local_unnamed_addr #8 {
|   %3 = icmp eq ptr %0, %1
|   br i1 %3, label %16, label %4
| 4:                                                ; preds = %2, %4
|   %5 = phi ptr [ %12, %4 ], [ %0, %2 ]
|   %12 = call ptr @someop(ptr %5)
|   br i1 %13, label %14, label %4, !llvm.loop !23
| 14:
|   %15 = call ptr @altair.copy_ptr(ptr %1)
|   br label %16
| 16:                                      ; preds = %4, %14
|   %17 = phi ptr [ %0, %2 ], [ %15, %14 ]
|   ret ptr %17
| }
*/
void fix_conflicting_phi_nodes(function_analyser& analyser);

/*
If a value has multiple affinities it generate a conflit
These conflits must be resolved prior to register allocation

| define dso_local i32 @return_non_first_arg(i32 noundef %0, i32 noundef returned %1) local_unnamed_addr #0 {
|   ret i32 %1
| }

Here %1 is both [arg, 1] and [ret]
To solve this conflict, %1 must be copied in another register:

| define dso_local i32 @return_non_first_arg(i32 noundef %0, i32 noundef returned %1) local_unnamed_addr #0 {
|   %2 = call i32 @altair.copy_i32(i32 %1)
|   ret i32 %2
| }
*/
void resolve_conflicting_affinities(function_analyser& analyser);

/*
Reorder instructions
*/
void optimize_pipeline(function_analyser& analyser);

}

#endif
