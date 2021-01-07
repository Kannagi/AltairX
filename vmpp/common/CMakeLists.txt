cmake_minimum_required(VERSION 3.0.0)

project(altair_vmpp_base
        LANGUAGES CXX
        VERSION 0.1.0)

add_library(altair_vmpp_base INTERFACE)
target_sources(altair_vmpp_base INTERFACE ${PROJECT_SOURCE_DIR}/base/vm.h)
target_include_directories(altair_vmpp_base INTERFACE ${PROJECT_SOURCE_DIR})

