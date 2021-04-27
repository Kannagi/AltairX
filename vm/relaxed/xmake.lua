add_requires("libsdl")

target("altair_vm_relaxed")
	set_kind("shared")
	set_languages("c99")
	add_files("src/vm.c", "src/processor_decode.c", "src/processor_execution.c")
	add_headerfiles("src/vm.h")
	add_deps("altair_vm_base", { public = true })
	add_packages("libsdl")
