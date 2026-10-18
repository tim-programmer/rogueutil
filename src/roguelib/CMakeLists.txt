FetchContent_Declare(flecs
    GIT_REPOSITORY https://github.com/SanderMertens/flecs.git
    GIT_TAG        v4.1.6
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)
FetchContent_MakeAvailable(flecs)

FetchContent_Declare(fastnoiselite
    GIT_REPOSITORY https://github.com/Auburn/FastNoiseLite.git
    GIT_TAG        785f37a9ad76e283586a379675085f2063ae03f7
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)
FetchContent_MakeAvailable(fastnoiselite)
set(FASTNOISELITE_DIR ${fastnoiselite_SOURCE_DIR})
include(cmake/fastnoiselite.cmake)

FetchContent_Declare(nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.12.0
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)
FetchContent_MakeAvailable(nlohmann_json)

add_library(roguelib
    base_map.h
    log_throw.h
    roguelib.cpp
    components/abilities.h
    components/combat_intent.h
    components/damage.h
    components/damage.cpp
    components/harmable.h
    components/healing.h
    components/healing.cpp
    components/inventory.h
    components/items.h
    components/mana.h
    components/resolved_ability.h
    components/stamina.h
    components/stats.h
    components/stats.cpp
)

target_include_directories(roguelib PRIVATE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/components>
)

target_include_directories(roguelib INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/components>
    #$<INSTALL_INTERFACE:${YOUR_INSTALL_DIR}/roguelib>
)

target_link_libraries(roguelib PRIVATE flecs::flecs_static fastnoiselite::fastnoiselite nlohmann_json::nlohmann_json cpptrace::cpptrace)

add_library(roguelib::roguelib ALIAS roguelib)

install(
    TARGETS roguelib
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)
