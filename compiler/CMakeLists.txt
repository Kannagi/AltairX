cmake_minimum_required(VERSION 3.6)

project(altair_compiler VERSION 0.1.0)

find_package(LLVM REQUIRED
    COMPONENTS
        LLVMIRReader
        LLVMAnalysis
        LLVMSupport
        )

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVM located in: ${LLVM_DIR}")

add_executable(altair_compiler
    src/lifetime.hpp
    src/intrinsic.hpp
    src/early_transforms.hpp
    src/mid_transforms.hpp
    src/late_transforms.hpp
    src/translator.hpp
    src/utilities.hpp
    src/function_analyser.hpp
    src/register_allocator.hpp
    src/stack_allocator.hpp

    src/lifetime.cpp
    src/intrinsic.cpp
    src/early_transforms.cpp
    src/mid_transforms.cpp
    src/late_transforms.cpp
    src/translator.cpp
    src/utilities.cpp
    src/function_analyser.cpp
    src/register_allocator.cpp
    src/stack_allocator.cpp
    src/main.cpp)

llvm_map_components_to_libnames(llvm_libs support core irreader analysis)
target_include_directories(altair_compiler SYSTEM PRIVATE ${LLVM_INCLUDE_DIRS})
target_compile_definitions(altair_compiler PRIVATE ${LLVM_DEFINITIONS})
target_link_libraries(altair_compiler PRIVATE ${llvm_libs})
set_target_properties(altair_compiler PROPERTIES CXX_STANDARD 17)

#if(MSVC)
    #target_compile_options(altair_compiler PRIVATE "/permissive- /wd4456 /wd4459 /wd4514 /wd4711 /Zc:__cplusplus")
#endif()
