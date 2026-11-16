#include "sky/app/game_loop.hpp"

#include <algorithm>

namespace sky {

GameLoop::GameLoop(FixedStepConfig config, const IClock& clock) : accumulator_(config), clock_(clock) {}

FrameStats GameLoop::runFrame(IGame& game, Input& input) {
    const Nanoseconds now = clock_.now();
    const Nanoseconds delta = lastTime_ ? now - *lastTime_ : 0;
    lastTime_ = now;

    const FrameSteps steps = accumulator_.advance(delta);
    const double fixedDt = accumulator_.stepSeconds();
    for (int i = 0; i < steps.steps; ++i) game.onFixedUpdate(fixedDt, input.fixedStepSnapshot());
    totalFixedSteps_ += static_cast<std::uint64_t>(steps.steps);

    // Cap the variable dt as well so a long stall can't feed a huge value to onUpdate.
    const Nanoseconds maxDelta = accumulator_.step() * accumulator_.maxStepsPerFrame();
    game.onUpdate(static_cast<double>(std::clamp<Nanoseconds>(delta, 0, maxDelta)) * 1e-9,
                  input.frameSnapshot());
    game.onRender(steps.alpha);
    ++totalFrames_;

    return {steps.steps, steps.alpha, steps.clamped};
}

}  // namespace sky
