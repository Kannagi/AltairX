set_project("altair_vm")

add_rules("mode.release", "mode.debug", "mode.releasedbg")

set_arch("x64")

target("aldebaran")
	set_kind("binary")
	set_languages("cxx17")
	add_files("*.cpp")
	add_headerfiles("*.h", "*.hpp")
	add_includedirs(".")
