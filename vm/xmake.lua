set_project("altair_vm")

includes("common")
includes("relaxed")
includes("pedantic")

set_arch("x64")

add_rules("mode.release", "mode.debug", "mode.releasedbg")

target("altair_vm")
	set_kind("binary")
	set_languages("c++17")
	add_files("src/main.cpp")
	add_headerfiles("src/vm.hpp", "src/io.hpp", "src/debug.hpp", "src/execution.hpp")
	add_deps("altair_vm_base")
	add_defines("AR_NO_PROTOTYPES", "FMT_HEADER_ONLY=1")
