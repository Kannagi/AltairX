cmake_minimum_required(VERSION 3.21)

project(AltairXVM VERSION 1.0)

    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON CACHE BOOL "" FORCE)

find_package(LLVM REQUIRED COMPONENTS LLVMObject)

if(LLVM_FOUND)
	message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION} at ${LLVM_DIR}")
else()
	message(STATUS "ELF loading disabled")
endif()

set(headers
    include/altairx.hpp
    include/core.hpp
    include/memory.hpp
    include/opcode.hpp
    include/utilities.hpp
)

set(srcs
    src/altairx.cpp
    src/core.cpp
    src/memory.cpp
    src/core.cpp
    src/debug.cpp
    src/io.cpp
    src/main.cpp
    src/syscall.cpp
)

add_executable(AltairXVM ${srcs} ${headers})
target_include_directories(AltairXVM PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_compile_features(AltairXVM PRIVATE cxx_std_17)

include(CheckIPOSupported)
check_ipo_supported(RESULT LTO_SUPPORT OUTPUT LTO_OUTPUT)
if(LTO_SUPPORT)
    set_target_properties(AltairXVM PROPERTIES INTERPROCEDURAL_OPTIMIZATION ON)
    message("LTO is enabled.")
endif()

if(LLVM_FOUND)
	target_sources(AltairXVM PRIVATE src/elf.cpp include/elf.hpp)
	llvm_map_components_to_libnames(llvm_libs object)
	target_link_libraries(AltairXVM PRIVATE ${llvm_libs})
	target_include_directories(AltairXVM SYSTEM PRIVATE ${LLVM_INCLUDE_DIRS})
	target_compile_definitions(AltairXVM PRIVATE ${LLVM_DEFINITIONS} HAS_LLVM=1)
endif()
