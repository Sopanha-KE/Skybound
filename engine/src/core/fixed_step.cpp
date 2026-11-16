#include "sky/core/fixed_step.hpp"

#include <algorithm>

namespace sky {

FixedStepAccumulator::FixedStepAccumulator(FixedStepConfig config) : config_(config) {
    config_.step = std::max<Nanoseconds>(config_.step, 1);
    config_.maxStepsPerFrame = std::max(config_.maxStepsPerFrame, 1);
}

FrameSteps FixedStepAccumulator::advance(Nanoseconds frameDelta) {
    accumulator_ += std::max<Nanoseconds>(frameDelta, 0);

    FrameSteps result;
    while (accumulator_ >= config_.step && result.steps < config_.maxStepsPerFrame) {
        accumulator_ -= config_.step;
        ++result.steps;
    }
    if (accumulator_ >= config_.step) {
        result.clamped = true;
        accumulator_ %= config_.step;
    }
    result.alpha = static_cast<double>(accumulator_) / static_cast<double>(config_.step);
    return result;
}

double FixedStepAccumulator::stepSeconds() const {
    return static_cast<double>(config_.step) * 1e-9;
}

}  // namespace sky
