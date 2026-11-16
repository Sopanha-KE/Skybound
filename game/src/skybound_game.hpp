#pragma once

#include "sky/app/game.hpp"

#include <cstdint>

class SkyboundGame final : public sky::IGame {
public:
    sky::Result<void> onInit() override;
    void onFixedUpdate(double dt, const sky::InputSnapshot& input) override;
    void onUpdate(double dt, const sky::InputSnapshot& input) override;
    void onRender(double alpha) override;
    void onShutdown() override;

private:
    std::uint64_t fixedTicks_ = 0;
    std::uint64_t frames_ = 0;
};
