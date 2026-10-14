cmake_minimum_required(VERSION 3.25)

set(FETCHCONTENT_QUIET FALSE)
include(FetchContent)

FetchContent_Declare(gtest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.18.0
)
FetchContent_MakeAvailable(gtest)

include(FetchContent)
FetchContent_Declare(cpptrace
    GIT_REPOSITORY https://github.com/jeremy-rifkin/cpptrace.git
    GIT_TAG        v1.0.4
)
FetchContent_MakeAvailable(cpptrace)

project(rogueutil
    VERSION 0.0.1
    SPDX_LICENSE "MIT"
    DESCRIPTION "Some utilities for roguelike games"
    LANGUAGES C CXX
)

add_subdirectory(src)
