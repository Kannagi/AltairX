cmake_minimum_required(VERSION 3.0.0)

project(altair_vmpp
        LANGUAGES CXX
        VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(common)
add_subdirectory(relaxed)
add_subdirectory(pedantic)

add_executable(altair_vmpp
    src/vm.hpp
    src/debug.hpp
    src/execution.hpp

    src/main.cpp)

target_link_libraries(altair_vmpp PRIVATE altair_vmpp_base)
target_compile_definitions(altair_vmpp PRIVATE AR_NO_PROTOTYPES FMT_HEADER_ONLY=1)

install(TARGETS altair_vmpp
        CONFIGURATIONS Debug
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/test/debug
        ARCHIVE DESTINATION ${PROJECT_SOURCE_DIR}/test/debug
        COMPONENT executable)

install(TARGETS altair_vmpp
        CONFIGURATIONS Release
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/test/release
        ARCHIVE DESTINATION ${PROJECT_SOURCE_DIR}/test/release
        COMPONENT executable)
