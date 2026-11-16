include(Catch)

add_executable(SkyEngineTests
    result_tests.cpp
    log_tests.cpp
    fixed_step_tests.cpp
    input_tests.cpp
    game_loop_tests.cpp
    window_tests.cpp
    public_header_tests.cpp
)
target_link_libraries(SkyEngineTests PRIVATE sky::engine Catch2::Catch2WithMain)
target_compile_definitions(SkyEngineTests PRIVATE
    SKY_PUBLIC_HEADER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../include/sky")
sky_enable_warnings(SkyEngineTests)

# SDL must never need a real display in tests.
catch_discover_tests(SkyEngineTests PROPERTIES ENVIRONMENT "SDL_VIDEODRIVER=dummy")

add_test(NAME Skybound.SmokeRun COMMAND Skybound --frames 30)
set_tests_properties(Skybound.SmokeRun PROPERTIES ENVIRONMENT "SDL_VIDEODRIVER=dummy")
