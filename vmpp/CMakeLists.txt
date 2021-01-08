cmake_minimum_required(VERSION 3.0.0)

project(altair_vmpp
	LANGUAGES CXX
	VERSION 0.1.0
)

set(CMAKE_CXX_STANDARD 20)

set(COMMON_SOURCES
	src/main.cpp
	src/vm_wrapper.hpp
	src/execution.hpp

	src/common/base/agu.hpp
	src/common/base/alu.hpp
	src/common/base/bru.hpp
	src/common/base/lsu.hpp
	src/common/base/ops.hpp
	src/common/base/vfpu.hpp
	src/common/base/vm.h
)

# set(PEDANTIC_SOURCES
# 	src/pedantic/
# )

set(RELAXED_SOURCES
	src/relaxed/processor_decode.cpp
	src/relaxed/processor_execution.cpp
	src/relaxed/vm.cpp
	src/relaxed/vm.hpp
)

add_executable(altair_vmpp_relaxed
	${COMMON_SOURCES}
	${RELAXED_SOURCES}
)

target_include_directories(altair_vmpp_relaxed PRIVATE src)

install(TARGETS altair_vmpp_relaxed
	CONFIGURATIONS Debug
	RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/test/debug
	ARCHIVE DESTINATION ${PROJECT_SOURCE_DIR}/test/debug
	COMPONENT executable
)

install(TARGETS altair_vmpp_relaxed
	CONFIGURATIONS Release
	RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/test/release
	ARCHIVE DESTINATION ${PROJECT_SOURCE_DIR}/test/release
	COMPONENT executable
)
