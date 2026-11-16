#include "sky/core/fixed_step.hpp"

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

using sky::FixedStepAccumulator;
using sky::FixedStepConfig;

namespace {
constexpr sky::Nanoseconds kStep = 10'000'000;  // 10 ms keeps the arithmetic obvious
FixedStepAccumulator make(int maxSteps = 5) { return FixedStepAccumulator({kStep, maxSteps}); }
}  // namespace

TEST_CASE("A delta below one step runs nothing and reports alpha", "[fixed_step]") {
    auto acc = make();
    auto f = acc.advance(kStep / 4);
    REQUIRE(f.steps == 0);
    REQUIRE(f.alpha == Catch::Approx(0.25));
    REQUIRE_FALSE(f.clamped);
}

TEST_CASE("Exact multiples produce exact step counts", "[fixed_step]") {
    auto acc = make();
    auto f = acc.advance(kStep * 3);
    REQUIRE(f.steps == 3);
    REQUIRE(f.alpha == Catch::Approx(0.0));
}

TEST_CASE("Remainders carry over between frames", "[fixed_step]") {
    auto acc = make();
    REQUIRE(acc.advance(kStep * 3 / 2).steps == 1);  // 1.5 steps -> 1, 0.5 left
    auto f = acc.advance(kStep * 3 / 2);             // 0.5 + 1.5 = 2.0
    REQUIRE(f.steps == 2);
    REQUIRE(f.alpha == Catch::Approx(0.0));
}

TEST_CASE("Total steps match elapsed time over many small frames", "[fixed_step]") {
    auto acc = make();
    int total = 0;
    for (int i = 0; i < 1000; ++i) total += acc.advance(kStep / 3).steps;  
    REQUIRE(total == 333);  // 1000 * (1/3 step) = 333.33 steps
}

TEST_CASE("Spiral-of-death clamp caps steps and drops the excess", "[fixed_step]") {
    auto acc = make(5);
    auto f = acc.advance(kStep * 100);
    REQUIRE(f.steps == 5);
    REQUIRE(f.clamped);
    // The backlog is dropped: the next normal frame is not still catching up.
    auto g = acc.advance(kStep);
    REQUIRE(g.steps == 1);
    REQUIRE_FALSE(g.clamped);
}

TEST_CASE("Alpha is always in [0, 1)", "[fixed_step]") {
    auto acc = make();
    for (sky::Nanoseconds d : {0LL, 1LL, 3'333'333LL, 9'999'999LL, 10'000'000LL, 123'456'789LL, 5'000'000'000LL}) {
        auto f = acc.advance(d);
        REQUIRE(f.alpha >= 0.0);
        REQUIRE(f.alpha < 1.0);
    }
}

TEST_CASE("Negative deltas are treated as zero", "[fixed_step]") {
    auto acc = make();
    auto f = acc.advance(-kStep);
    REQUIRE(f.steps == 0);
    REQUIRE(f.alpha == Catch::Approx(0.0));
}

TEST_CASE("Invalid config is sanitized", "[fixed_step]") {
    FixedStepAccumulator acc({0, 0});
    REQUIRE(acc.step() >= 1);
    REQUIRE(acc.maxStepsPerFrame() >= 1);
    REQUIRE(acc.advance(10).steps >= 1);
}

TEST_CASE("stepSeconds converts nanoseconds", "[fixed_step]") {
    REQUIRE(make().stepSeconds() == Catch::Approx(0.01));
}
