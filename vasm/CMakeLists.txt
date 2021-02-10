cmake_minimum_required(VERSION 3.0)

project(altair_vasm)

set(ALTAIR_CPU "AltairK1" CACHE STRING "")
set(ALTAIR_SYNTAX "mot" CACHE STRING "")

add_executable(vasm
    atom.c
    atom.h
    error.c
    error.h
    expr.c
    expr.h
    general_errors.h
    hugeint.c
    hugeint.h
    output_aout.c
    output_aout.h
    output_bin.c
    output_elf.c
    output_elf.h
    output_errors.h
    output_hunk.c
    output_hunk.h
    output_test.c
    output_tos.c
    output_tos.h
    output_vobj.c
    parse.c
    parse.h
    reloc.c
    reloc.h
    supp.c
    supp.h
    symbol.c
    symbol.h
    symtab.c
    symtab.h
    tfloat.h
    vasm.c
    vasm.h

    syntax/${ALTAIR_SYNTAX}/syntax.h
    syntax/${ALTAIR_SYNTAX}/syntax.c
    syntax/${ALTAIR_SYNTAX}/syntax_errors.h

    cpus/${ALTAIR_CPU}/cpu.h
    cpus/${ALTAIR_CPU}/cpu.c
    cpus/${ALTAIR_CPU}/cpu_errors.h
    cpus/${ALTAIR_CPU}/opcodes.h
)

set_target_properties(vasm PROPERTIES OUTPUT_NAME "vasm${ALTAIR_CPU}_${ALTAIR_SYNTAX}")
target_include_directories(vasm PRIVATE . cpus/${ALTAIR_CPU} syntax/${ALTAIR_SYNTAX})

add_executable(vasm_objdump
    vobjdump.c
    vobjdump.h
)
