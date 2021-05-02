set_project("altair_vm")

add_rules("mode.release", "mode.debug", "mode.releasedbg")

set_runtimes(is_mode("release") and "MT" or "MTd")
set_arch("x64")

if is_plat("windows") then
	add_ldflags("-nodefaultlib:msvcrt.lib")
end

add_requires("libsdl")

includes("common")
includes("relaxed")
includes("pedantic")

target("altair_vm")
	set_kind("binary")
	set_languages("c++17")
	add_files("src/main.cpp")
	add_headerfiles("src/vm.hpp", "src/io.hpp", "src/debug.hpp", "src/execution.hpp")
	add_deps("altair_vm_base")
	add_packages("libsdl", {links={}})
	add_deps("altair_vm_relaxed", {inherit = false})
	add_defines("AR_NO_PROTOTYPES", "FMT_HEADER_ONLY=1")
