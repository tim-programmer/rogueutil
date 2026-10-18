cmake_minimum_required(VERSION 3.20)
project(fastnoiselite)

if(NOT FASTNOISELITE_DIR)
	message(FATAL_ERROR "FASTNOISELITE_DIR must be set to the FastNoiseLite root directory")
endif()

set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED)

set(ROOT ${FASTNOISELITE_DIR})

add_library(fastnoiselite-lib INTERFACE)
target_sources(fastnoiselite-lib PUBLIC
    ${ROOT}/Cpp/FastNoiseLite.h
 )

target_include_directories(fastnoiselite-lib INTERFACE
    $<BUILD_INTERFACE:${ROOT}/Cpp>
    $<INSTALL_INTERFACE:include>)

set(INSTALL_TARGETS fastnoiselite-lib)
set(INSTALL_HEADERS ${ROOT}/Cpp/FastNoiseLite.h)

add_library(fastnoiselite::fastnoiselite ALIAS fastnoiselite-lib)
