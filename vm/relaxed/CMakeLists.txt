cmake_minimum_required(VERSION 3.0.0)

project(ALTAIR_VM_RELAXED
        LANGUAGES C
        VERSION 0.1.0)

add_library(altair_vm_relaxed STATIC
            src/vm.h

            src/vm.c)

target_link_libraries(altair_vm_relaxed PRIVATE altair_vm_base)
