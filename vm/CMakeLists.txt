cmake_minimum_required(VERSION 3.0.0)

project(altair_vm
        LANGUAGES C
        VERSION 0.1.0)

add_subdirectory(common)
add_subdirectory(relaxed)
add_subdirectory(pedantic)

add_executable(altair_vm
               src/main.c)

target_link_libraries(altair_vm PRIVATE altair_vm_base)
target_link_libraries(altair_vm PRIVATE altair_vm_relaxed)
