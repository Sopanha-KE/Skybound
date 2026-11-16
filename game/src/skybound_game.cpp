#include "skybound_game.hpp"

#include "sky/core/log.hpp"

sky::Result<void> SkyboundGame::onInit() {
    sky::logInfo("Skybound starting. Escape quits.");
    return {};
}

void SkyboundGame::onFixedUpdate(double dt, const sky::InputSnapshot& input) {
    ++fixedTicks_;
    if (input.pressed(sky::Key::Space)) sky::logInfo("Space pressed (fixed tick {})", fixedTicks_);
    // Once a second of simulated time, derived from the tick count rather than a clock.
    if (fixedTicks_ % static_cast<std::uint64_t>(1.0 / dt + 0.5) == 0)
        sky::logInfo("{} fixed ticks, {} frames", fixedTicks_, frames_);
}

void SkyboundGame::onUpdate(double, const sky::InputSnapshot&) {}

void SkyboundGame::onRender(double) { ++frames_; }

void SkyboundGame::onShutdown() { sky::logInfo("Skybound shutting down"); }
