add_executable(Skybound
    src/main.cpp
    src/skybound_game.cpp
)
target_link_libraries(Skybound PRIVATE sky::engine)
sky_enable_warnings(Skybound)
