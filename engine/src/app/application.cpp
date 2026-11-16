#include "sky/app/application.hpp"

#include "sky/app/game_loop.hpp"
#include "sky/core/log.hpp"
#include "sky/core/time.hpp"

#include <chrono>
#include <thread>

namespace sky {

int runApplication(const ApplicationConfig& config, IGame& game) {
    LogScope logScope(config.logLevel);

    auto window = Window::create(config.window);
    if (!window) {
        logError("Window creation failed: {}", window.error().message);
        return 1;
    }

    if (auto init = game.onInit(); !init) {
        logError("Game init failed: {}", init.error().message);
        return 1;
    }

    const auto clock = makeSystemClock();
    GameLoop loop(config.fixedStep, *clock);
    Input input;
    logInfo("Entering main loop ({:.1f} Hz fixed step)", 1e9 / static_cast<double>(config.fixedStep.step));

    while (!window.value().closeRequested()) {
        input.beginFrame();
        window.value().pumpEvents(input);
        if (window.value().closeRequested() || input.frameSnapshot().pressed(Key::Escape)) break;

        loop.runFrame(game, input);
        if (config.maxFrames != 0 && loop.totalFrames() >= config.maxFrames) break;

        // Placeholder pacing until Phase 2 adds a swap interval; avoids spinning a core.
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    game.onShutdown();
    logInfo("Shut down after {} frames, {} fixed steps", loop.totalFrames(), loop.totalFixedSteps());
    return 0;
}

}  // namespace sky
