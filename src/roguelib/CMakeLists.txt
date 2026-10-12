FetchContent_Declare(flecs
    GIT_REPOSITORY https://github.com/SanderMertens/flecs.git
    GIT_TAG        v4.1.6
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)
FetchContent_MakeAvailable(flecs)

FetchContent_Declare(flecs
    GIT_REPOSITORY https://github.com/Auburn/FastNoiseLite.git
    GIT_TAG        785f37a9ad76e283586a379675085f2063ae03f7
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)
FetchContent_MakeAvailable(flecs)


add_library(roguelib
    base_map.h
    roguelib.cpp
)

target_include_directories(roguelib INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/roguelib>
    #$<INSTALL_INTERFACE:${YOUR_INSTALL_DIR}/roguelib>
)
add_library(roguelib::roguelib ALIAS roguelib)

install(
    TARGETS roguelib
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)
