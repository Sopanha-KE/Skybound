#pragma once

#include "sky/app/game.hpp"
#include "sky/core/fixed_step.hpp"
#include "sky/core/log.hpp"
#include "sky/platform/window.hpp"

#include <cstdint>

namespace sky {

struct ApplicationConfig {
    WindowConfig window;
    FixedStepConfig fixedStep;
    LogLevel logLevel = LogLevel::Info;
    std::uint64_t maxFrames = 0;  // 0 = run until quit; nonzero is for headless smoke tests
};

// Creates the window, runs the loop until Escape / window close / maxFrames, then shuts down.
// Returns a process exit code: 0 on a clean exit.
int runApplication(const ApplicationConfig& config, IGame& game);

}  // namespace sky
