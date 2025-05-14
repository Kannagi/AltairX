# Uses SDL3
find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3)

include(FetchContent)
FetchContent_Declare(imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG docking
)
FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
    # Fetch the content using previously declared details
    FetchContent_Populate(imgui)
	# ImGUI provides no CMakeLists, so lets build it on-flight
    add_library(ImGUI STATIC
	    ${imgui_SOURCE_DIR}/imgui.cpp
	    ${imgui_SOURCE_DIR}/imgui_draw.cpp
	    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
	    ${imgui_SOURCE_DIR}/imgui_tables.cpp
		${imgui_SOURCE_DIR}/imgui_demo.cpp
	    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
	    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp
    )
    target_include_directories(ImGUI PUBLIC
	    ${imgui_SOURCE_DIR}
	    ${imgui_SOURCE_DIR}/backends
    )
	target_link_libraries(ImGUI PRIVATE SDL3::SDL3)
endif()

add_library(AltairXVMGUI STATIC
	altairx_gui.hpp
	altairx_gui.cpp
)

target_link_libraries(AltairXVMGUI PRIVATE AltairXVMCore ImGUI SDL3::SDL3)
target_include_directories(AltairXVMGUI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(AX_HAS_LTO)
	set_target_properties(ImGUI PROPERTIES INTERPROCEDURAL_OPTIMIZATION ON)
	set_target_properties(AltairXVMGUI PROPERTIES INTERPROCEDURAL_OPTIMIZATION ON)
endif()
