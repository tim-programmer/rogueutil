cmake_minimum_required(VERSION 3.20)
project(imgui)

if(NOT IMGUI_DIR)
	message(FATAL_ERROR "IMGUI_DIR must be set to the imgui root directory")
endif()

set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED)

#find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SDL3 REQUIRED)

set(ROOT ${IMGUI_DIR})

add_library(imgui-lib)
target_sources(imgui-lib PUBLIC
    ${ROOT}/imgui.cpp
    ${ROOT}/imgui.h
    ${ROOT}/imgui_internal.h
    ${ROOT}/imgui_demo.cpp
    ${ROOT}/imgui_draw.cpp
    ${ROOT}/imgui_tables.cpp
    ${ROOT}/imgui_widgets.cpp
    ${ROOT}/imstb_rectpack.h
  	${ROOT}/imstb_textedit.h
  	${ROOT}/imstb_truetype.h
    ${ROOT}/misc/cpp/imgui_stdlib.cpp
    ${ROOT}/misc/cpp/imgui_stdlib.h
    ${ROOT}/backends/imgui_impl_sdl3.cpp
    ${ROOT}/backends/imgui_impl_opengl3.cpp
 )
target_include_directories(imgui-lib PUBLIC
    ${ROOT}
    ${ROOT}/backends
    ${ROOT}/misc/cpp
)
target_link_libraries(imgui-lib PUBLIC SDL3::SDL3-static)

target_include_directories(imgui-lib PUBLIC
    $<BUILD_INTERFACE:${ROOT}>
    $<INSTALL_INTERFACE:include>)

set(INSTALL_TARGETS imgui-lib)
set(INSTALL_HEADERS ${ROOT}/imgui.h ${ROOT}/imconfig.h)
