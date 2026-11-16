add_library(SkyEngine STATIC
    src/core/log.cpp
    src/core/time.cpp
    src/core/fixed_step.cpp
    src/platform/input.cpp
    src/platform/window.cpp
    src/app/game_loop.cpp
    src/app/application.cpp
)
add_library(sky::engine ALIAS SkyEngine)

# Public headers expose engine types only; SDL and spdlog are PRIVATE.
target_include_directories(SkyEngine PUBLIC include)
target_link_libraries(SkyEngine PRIVATE SDL3::SDL3 spdlog::spdlog)
sky_enable_warnings(SkyEngine)
