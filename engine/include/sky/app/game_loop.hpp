#pragma once

#include "sky/app/game.hpp"
#include "sky/core/fixed_step.hpp"
#include "sky/core/time.hpp"
#include "sky/platform/input.hpp"

#include <cstdint>
#include <optional>

namespace sky {

struct FrameStats {
    int fixedSteps = 0;
    double alpha = 0.0;
    bool clamped = false;
};

// One iteration of the frame pipeline, separated from the window so it can be tested
// with a fake clock and fake input: accumulate -> fixed steps -> update -> render(alpha).
class GameLoop {
public:
    GameLoop(FixedStepConfig config, const IClock& clock);

    FrameStats runFrame(IGame& game, Input& input);

    std::uint64_t totalFixedSteps() const { return totalFixedSteps_; }
    std::uint64_t totalFrames() const { return totalFrames_; }

private:
    FixedStepAccumulator accumulator_;
    const IClock& clock_;
    std::optional<Nanoseconds> lastTime_;
    std::uint64_t totalFixedSteps_ = 0;
    std::uint64_t totalFrames_ = 0;
};

}  // namespace sky
