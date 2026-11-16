#include "skybound_game.hpp"

#include "sky/app/application.hpp"

#include <cstdlib>
#include <cstring>

int main(int argc, char** argv) {
    sky::ApplicationConfig config;
    config.window.title = "Skybound";

    for (int i = 1; i + 1 < argc; ++i)
        if (std::strcmp(argv[i], "--frames") == 0) config.maxFrames = std::strtoull(argv[i + 1], nullptr, 10);

    SkyboundGame game;
    return sky::runApplication(config, game);
}
