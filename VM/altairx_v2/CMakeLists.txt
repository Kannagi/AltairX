cmake_minimum_required(VERSION 3.21..3.30 FATAL_ERROR)

project(AltairXVM VERSION 1.0)

option(AltairXVM_USE_LTO "If ON, try to enable LTO" ON)
option(AltairXVM_ELF_SUPPORT "If ON, try to find LLVM to enable ELF support" ON)

# Create VM executable
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

# Optional LTO support
if(AltairXVM_USE_LTO)
	include(CheckIPOSupported)
	check_ipo_supported(RESULT lto_support OUTPUT lto_output_message)
	if(lto_support)
		set_target_properties(AltairXVM PROPERTIES INTERPROCEDURAL_OPTIMIZATION ON)
		message(STATUS "LTO is enabled.")
	else()
		message(WARNING "LTO can not be enabled. Reason: ${lto_output_message}")
	endif()
endif()

# Optional ELF support
if(AltairXVM_ELF_SUPPORT)
	find_package(LLVM COMPONENTS LLVMObject)
	if(LLVM_FOUND)
		message(STATUS "ELF support enabled, found LLVM ${LLVM_PACKAGE_VERSION} at ${LLVM_DIR}")
		target_sources(AltairXVM PRIVATE src/elf.cpp include/elf.hpp)
		llvm_map_components_to_libnames(llvm_libs object)
		target_link_libraries(AltairXVM PRIVATE ${llvm_libs})
		target_include_directories(AltairXVM SYSTEM PRIVATE ${LLVM_INCLUDE_DIRS})
		target_compile_definitions(AltairXVM PRIVATE ${LLVM_DEFINITIONS} HAS_LLVM=1)
	else()
		message(WARNING "ELF loading disabled, could not found LLVM.")
	endif()
endif()
