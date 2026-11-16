#pragma once

#include "sky/core/result.hpp"
#include "sky/platform/input.hpp"

namespace sky {

// The one justified use of inheritance: the game plugs into the engine loop here.
class IGame {
public:
    virtual ~IGame() = default;

    virtual Result<void> onInit() = 0;
    // Simulation and physics only. `dt` is the constant step in seconds; never read the clock here.
    virtual void onFixedUpdate(double dt, const InputSnapshot& input) = 0;
    // Once per rendered frame: camera look, UI, anything not part of the simulation.
    virtual void onUpdate(double dt, const InputSnapshot& input) = 0;
    // `alpha` in [0, 1) blends previous and current simulation state.
    virtual void onRender(double alpha) = 0;
    virtual void onShutdown() = 0;
};

}  // namespace sky
