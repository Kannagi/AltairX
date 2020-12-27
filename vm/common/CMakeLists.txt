cmake_minimum_required(VERSION 3.0.0)

project(altair_vm_base
        LANGUAGES C
        VERSION 0.1.0)

add_library(altair_vm_base INTERFACE)
target_sources(altair_vm_base INTERFACE ${PROJECT_SOURCE_DIR}/base/vm.h)
target_include_directories(altair_vm_base INTERFACE ${PROJECT_SOURCE_DIR})

