# ---------------------------------------------------------------------------
# Tracy root — defaults to three directories above this CMakeLists.txt.
# ---------------------------------------------------------------------------
option(TRACY_ENABLE "Enable profiling w/tracy" ON)
option(TRACY_ON_DEMAND " " OFF)

FetchContent_Declare(SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        release-3.4.18
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)
FetchContent_MakeAvailable(SDL3)

FetchContent_Declare(SDL3_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG        e7cf7ceeb41fbb59f67f27af6aa99401cc9eb888
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)
FetchContent_MakeAvailable(SDL3_ttf)

FetchContent_Declare(imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.92.9b-docking
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)
FetchContent_MakeAvailable(imgui)

FetchContent_Declare(glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.3
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(argpase
    GIT_REPOSITORY https://github.com/p-ranav/argparse.git
    GIT_TAG        7d093f8e1201db2dded2eee94de77665698ee9dc
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)
FetchContent_MakeAvailable(argpase)

FetchContent_Declare(spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.17.0
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare (tracy
    GIT_REPOSITORY  https://github.com/wolfpld/tracy.git
    GIT_TAG         v0.14.1
    GIT_SHALLOW     TRUE
    GIT_PROGRESS    TRUE
)
FetchContent_MakeAvailable(tracy)

set(IMGUI_DIR ${imgui_SOURCE_DIR})
include(${PROJECT_SOURCE_DIR}/cmake/imgui.cmake)

add_executable(autoexplore)

target_sources(autoexplore PRIVATE autoexplore.cpp)
target_compile_options(autoexplore PRIVATE -fconcepts-diagnostics-depth=2)
target_link_libraries(autoexplore PRIVATE
    SDL3::SDL3-static
    imgui-lib
    argparse
    gtest
    spdlog
    cpptrace::cpptrace
    roguelib::roguelib
)

if(TRACY_ENABLE)
  target_link_libraries(autoexplore PRIVATE Tracy::TracyClient)
endif()
