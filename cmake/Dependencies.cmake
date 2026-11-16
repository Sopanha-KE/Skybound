# Single source of truth for third-party versions. Exact tags only.
include(FetchContent)

set(SKY_SDL3_TAG    "release-3.4.20")
set(SKY_SPDLOG_TAG  "v1.17.0")
set(SKY_CATCH2_TAG  "v3.16.1")

# SDL3: static, no tests/examples/install
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_TESTS OFF CACHE BOOL "" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
set(SDL_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SDL_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_Declare(SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG ${SKY_SDL3_TAG}
    GIT_SHALLOW TRUE
    SYSTEM)

# spdlog: compiled library, bundled fmt
set(SPDLOG_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(SPDLOG_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_Declare(spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG ${SKY_SPDLOG_TAG}
    GIT_SHALLOW TRUE
    SYSTEM)

FetchContent_MakeAvailable(SDL3 spdlog)

if(SKY_BUILD_TESTS)
    set(CATCH_INSTALL_DOCS OFF CACHE BOOL "" FORCE)
    set(CATCH_INSTALL_EXTRAS OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG ${SKY_CATCH2_TAG}
        GIT_SHALLOW TRUE
        SYSTEM)
    FetchContent_MakeAvailable(Catch2)
    list(APPEND CMAKE_MODULE_PATH "${catch2_SOURCE_DIR}/extras")
endif()
