cmake_minimum_required(VERSION 3.21)

project(AltairXVM VERSION 1.0)

set(headers
    include/altairx.hpp
    include/core.hpp
    include/memory.hpp
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
