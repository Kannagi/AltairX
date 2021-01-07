cmake_minimum_required(VERSION 3.0.0)

project(ALTAIR_VM_RELAXED
        LANGUAGES C
        VERSION 0.1.0)

add_library(altair_vm_relaxed SHARED
            src/vm.h

            src/vm.c
            src/processor_decode.c
            src/processor_execution.c)

set_target_properties(altair_vm_relaxed PROPERTIES PREFIX "")
target_link_libraries(altair_vm_relaxed PRIVATE altair_vm_base)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(altair_vm_relaxed PRIVATE -Wno-float-equal)
endif()

install(TARGETS altair_vm_relaxed
        CONFIGURATIONS Debug
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/../test/debug
        ARCHIVE DESTINATION ${PROJECT_SOURCE_DIR}/../test/debug
        COMPONENT library)

install(TARGETS altair_vm_relaxed
        CONFIGURATIONS Release
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/../test/release
        ARCHIVE DESTINATION ${PROJECT_SOURCE_DIR}/../test/release
        COMPONENT library)
