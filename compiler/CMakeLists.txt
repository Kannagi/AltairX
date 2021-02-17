cmake_minimum_required(VERSION 3.0)

project(altair_compiler VERSION 0.1.0)

find_package(LLVM REQUIRED CONFIG)


add_executable(altair_compiler
    src/main.cpp
    )

llvm_map_components_to_libnames(llvm_libs support core irreader)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
target_link_libraries(altair_compiler ${llvm_libs})
