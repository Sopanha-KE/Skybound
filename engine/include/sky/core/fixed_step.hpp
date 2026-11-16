#pragma once

#include "sky/core/time.hpp"

namespace sky {

struct FixedStepConfig {
    Nanoseconds step = 16'666'667;  // ~60 Hz
    int maxStepsPerFrame = 5;       // spiral-of-death clamp
};

struct FrameSteps {
    int steps = 0;
    double alpha = 0.0;  // in [0, 1): how far the render time is into the next step
    bool clamped = false;
};

// Integer nanoseconds avoid float drift in the accumulator.
class FixedStepAccumulator {
public:
    explicit FixedStepAccumulator(FixedStepConfig config = {});

    // Negative deltas count as zero. If more than maxStepsPerFrame would be needed, the
    // excess whole steps are dropped (simulation slows down rather than spiralling).
    FrameSteps advance(Nanoseconds frameDelta);

    double stepSeconds() const;
    Nanoseconds step() const { return config_.step; }
    int maxStepsPerFrame() const { return config_.maxStepsPerFrame; }

private:
    FixedStepConfig config_;
    Nanoseconds accumulator_ = 0;
};

}  // namespace sky
