cmake_minimum_required(VERSION 3.28)

project(SkyBound VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(SKY_BUILD_TESTS "Build the Catch2 test suite" ON)
option(SKY_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(CompilerWarnings)
include(Dependencies)

add_subdirectory(engine)
add_subdirectory(game)

if(SKY_BUILD_TESTS)
    enable_testing()
    add_subdirectory(engine/tests)
endif()
