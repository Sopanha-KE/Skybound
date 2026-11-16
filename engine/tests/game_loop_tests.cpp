#include "sky/app/game_loop.hpp"

#include <catch2/catch_test_macros.hpp>

using namespace sky;

namespace {

struct FakeClock final : IClock {
    Nanoseconds t = 0;
    Nanoseconds now() const override { return t; }
};

struct RecordingGame final : IGame {
    int fixed = 0, updates = 0, renders = 0;
    double lastAlpha = -1, lastFixedDt = 0;
    bool sawSpacePressedInFixed = false;
    int fixedStepsWithSpace = 0;

    Result<void> onInit() override { return {}; }
    void onFixedUpdate(double dt, const InputSnapshot& in) override {
        ++fixed;
        lastFixedDt = dt;
        if (in.pressed(Key::Space)) ++fixedStepsWithSpace;
    }
    void onUpdate(double, const InputSnapshot&) override { ++updates; }
    void onRender(double alpha) override {
        ++renders;
        lastAlpha = alpha;
    }
    void onShutdown() override {}
};

constexpr Nanoseconds kStep = 10'000'000;

}  // namespace

TEST_CASE("First frame runs no fixed steps", "[loop]") {
    FakeClock clock;
    clock.t = 5'000'000'000;
    GameLoop loop({kStep, 5}, clock);
    RecordingGame game;
    Input input;
    auto stats = loop.runFrame(game, input);
    REQUIRE(stats.fixedSteps == 0);
    REQUIRE(game.updates == 1);
    REQUIRE(game.renders == 1);
}

TEST_CASE("Fixed steps follow the fake clock, update/render run once per frame", "[loop]") {
    FakeClock clock;
    GameLoop loop({kStep, 5}, clock);
    RecordingGame game;
    Input input;
    loop.runFrame(game, input);
    clock.t += kStep * 2 + kStep / 2;
    loop.runFrame(game, input);
    REQUIRE(game.fixed == 2);
    REQUIRE(game.updates == 2);
    REQUIRE(game.renders == 2);
    REQUIRE(game.lastAlpha == 0.5);
    REQUIRE(game.lastFixedDt == 0.01);
    REQUIRE(loop.totalFixedSteps() == 2);
    REQUIRE(loop.totalFrames() == 2);
}

TEST_CASE("A long stall is clamped", "[loop]") {
    FakeClock clock;
    GameLoop loop({kStep, 5}, clock);
    RecordingGame game;
    Input input;
    loop.runFrame(game, input);
    clock.t += kStep * 1000;
    auto stats = loop.runFrame(game, input);
    REQUIRE(stats.clamped);
    REQUIRE(game.fixed == 5);
}

TEST_CASE("A press is seen by exactly one fixed step even with several per frame", "[loop][input]") {
    FakeClock clock;
    GameLoop loop({kStep, 5}, clock);
    RecordingGame game;
    Input input;
    loop.runFrame(game, input);

    input.beginFrame();
    input.handle({InputEvent::Type::KeyDown, Key::Space, MouseButton::Left, 0, 0});
    clock.t += kStep * 3;
    loop.runFrame(game, input);
    REQUIRE(game.fixed == 3);
    REQUIRE(game.fixedStepsWithSpace == 1);
}

TEST_CASE("A press in a zero-step frame reaches the next fixed step", "[loop][input]") {
    FakeClock clock;
    GameLoop loop({kStep, 5}, clock);
    RecordingGame game;
    Input input;
    loop.runFrame(game, input);

    input.beginFrame();
    input.handle({InputEvent::Type::KeyDown, Key::Space, MouseButton::Left, 0, 0});
    clock.t += kStep / 4;
    loop.runFrame(game, input);
    REQUIRE(game.fixed == 0);

    input.beginFrame();
    clock.t += kStep;
    loop.runFrame(game, input);
    REQUIRE(game.fixedStepsWithSpace == 1);
}
