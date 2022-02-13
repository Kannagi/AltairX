set_project("altair_vm")

add_rules("mode.release", "mode.debug", "mode.releasedbg")

set_arch("x64")

target("aldebaran")
	set_kind("static")
	set_languages("cxx20")
	add_files("*.cpp")
	add_headerfiles("*.h", "*.hpp")
	add_includedirs(".")

target("test")
	set_kind("binary")
	set_languages("cxx20")
	add_files("test/test.cpp")
	add_headerfiles("aldebaran.h")
	add_includedirs(".")
	add_deps("aldebaran")
