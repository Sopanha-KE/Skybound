#include "sky/app/application.hpp"
#include "sky/platform/window.hpp"

#include <catch2/catch_test_macros.hpp>

// These run under SDL_VIDEODRIVER=dummy (set by CTest), so no display is needed.

TEST_CASE("Window can be created, pumped and destroyed headlessly", "[window]") {
    auto window = sky::Window::create({"test", 320, 240, false});
    REQUIRE(window.ok());
    sky::Input input;
    window.value().pumpEvents(input);
    REQUIRE_FALSE(window.value().closeRequested());
}

TEST_CASE("runApplication runs maxFrames and shuts down cleanly", "[window][app]") {
    struct Game final : sky::IGame {
        int init = 0, shutdown = 0, renders = 0;
        sky::Result<void> onInit() override { ++init; return {}; }
        void onFixedUpdate(double, const sky::InputSnapshot&) override {}
        void onUpdate(double, const sky::InputSnapshot&) override {}
        void onRender(double) override { ++renders; }
        void onShutdown() override { ++shutdown; }
    } game;

    sky::ApplicationConfig config;
    config.window = {"test", 320, 240, false};
    config.maxFrames = 3;
    REQUIRE(sky::runApplication(config, game) == 0);
    REQUIRE(game.init == 1);
    REQUIRE(game.renders == 3);
    REQUIRE(game.shutdown == 1);
}

TEST_CASE("runApplication fails and skips shutdown when onInit fails", "[window][app]") {
    struct Game final : sky::IGame {
        int shutdown = 0;
        sky::Result<void> onInit() override { return sky::Error{"no"}; }
        void onFixedUpdate(double, const sky::InputSnapshot&) override {}
        void onUpdate(double, const sky::InputSnapshot&) override {}
        void onRender(double) override {}
        void onShutdown() override { ++shutdown; }
    } game;

    sky::ApplicationConfig config;
    config.window = {"test", 320, 240, false};
    REQUIRE(sky::runApplication(config, game) == 1);
    REQUIRE(game.shutdown == 0);
}
