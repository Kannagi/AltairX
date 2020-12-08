cmake_minimum_required(VERSION 3.0.0)

project(altair_vm
        LANGUAGES CXX
        VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(common)
add_subdirectory(relaxed)
add_subdirectory(pedantic)

add_executable(altair_vm src/main.cpp)

target_link_libraries(altair_vm PRIVATE altair_vm_base)
target_compile_definitions(altair_vm PRIVATE AR_NO_PROTOTYPES)

install(TARGETS altair_vm
        CONFIGURATIONS Debug
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/test/debug
        ARCHIVE DESTINATION ${PROJECT_SOURCE_DIR}/test/debug
        COMPONENT executable)

install(TARGETS altair_vm
        CONFIGURATIONS Release
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/test/release
        ARCHIVE DESTINATION ${PROJECT_SOURCE_DIR}/test/release
        COMPONENT executable)
