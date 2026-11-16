#include "sky/platform/input.hpp"

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace sky;

namespace {
InputEvent keyDown(Key k) { return {InputEvent::Type::KeyDown, k, MouseButton::Left, 0, 0}; }
InputEvent keyUp(Key k) { return {InputEvent::Type::KeyUp, k, MouseButton::Left, 0, 0}; }
InputEvent motion(float dx, float dy) { return {InputEvent::Type::MouseMotion, Key::Escape, MouseButton::Left, dx, dy}; }
InputEvent buttonDown(MouseButton b) { return {InputEvent::Type::MouseButtonDown, Key::Escape, b, 0, 0}; }
}  // namespace

TEST_CASE("Key press produces down and pressed for one frame", "[input]") {
    Input in;
    in.beginFrame();
    in.handle(keyDown(Key::W));
    auto s = in.frameSnapshot();
    REQUIRE(s.down(Key::W));
    REQUIRE(s.pressed(Key::W));
    REQUIRE_FALSE(s.released(Key::W));

    in.beginFrame();
    s = in.frameSnapshot();
    REQUIRE(s.down(Key::W));
    REQUIRE_FALSE(s.pressed(Key::W));
}

TEST_CASE("Key release produces released and clears down", "[input]") {
    Input in;
    in.beginFrame();
    in.handle(keyDown(Key::A));
    in.beginFrame();
    in.handle(keyUp(Key::A));
    auto s = in.frameSnapshot();
    REQUIRE_FALSE(s.down(Key::A));
    REQUIRE(s.released(Key::A));
}

TEST_CASE("Duplicate key-down events do not create extra edges", "[input]") {
    Input in;
    in.beginFrame();
    in.handle(keyDown(Key::S));
    in.beginFrame();
    in.handle(keyDown(Key::S));
    REQUIRE_FALSE(in.frameSnapshot().pressed(Key::S));
}

TEST_CASE("Mouse delta accumulates within a frame and resets", "[input]") {
    Input in;
    in.beginFrame();
    in.handle(motion(2, -1));
    in.handle(motion(3, 4));
    auto s = in.frameSnapshot();
    REQUIRE(s.mouseDeltaX == Catch::Approx(5));
    REQUIRE(s.mouseDeltaY == Catch::Approx(3));
    in.beginFrame();
    REQUIRE(in.frameSnapshot().mouseDeltaX == Catch::Approx(0));
}

TEST_CASE("Mouse buttons have edges", "[input]") {
    Input in;
    in.beginFrame();
    in.handle(buttonDown(MouseButton::Right));
    auto s = in.frameSnapshot();
    REQUIRE(s.down(MouseButton::Right));
    REQUIRE(s.pressed(MouseButton::Right));
}

TEST_CASE("Fixed-step edges are delivered to exactly one step", "[input][fixed]") {
    Input in;
    in.beginFrame();
    in.handle(keyDown(Key::Space));
    in.handle(motion(1, 1));
    auto first = in.fixedStepSnapshot();
    auto second = in.fixedStepSnapshot();  // second step in the same frame
    REQUIRE(first.pressed(Key::Space));
    REQUIRE(first.mouseDeltaX == Catch::Approx(1));
    REQUIRE(second.down(Key::Space));
    REQUIRE_FALSE(second.pressed(Key::Space));
    REQUIRE(second.mouseDeltaX == Catch::Approx(0));
}

TEST_CASE("Edges survive frames that run zero fixed steps", "[input][fixed]") {
    Input in;
    in.beginFrame();
    in.handle(keyDown(Key::F));
    in.beginFrame();  // no fixed step ran this frame
    in.beginFrame();
    REQUIRE_FALSE(in.frameSnapshot().pressed(Key::F));  // frame view is gone...
    REQUIRE(in.fixedStepSnapshot().pressed(Key::F));    // ...but the fixed step still sees it
    REQUIRE_FALSE(in.fixedStepSnapshot().pressed(Key::F));
}

TEST_CASE("A tap within one fixed interval is not lost", "[input][fixed]") {
    Input in;
    in.beginFrame();
    in.handle(keyDown(Key::E));
    in.handle(keyUp(Key::E));
    auto s = in.fixedStepSnapshot();
    REQUIRE(s.pressed(Key::E));
    REQUIRE(s.released(Key::E));
    REQUIRE_FALSE(s.down(Key::E));
}

TEST_CASE("ActionMap resolves actions and axes", "[input][actions]") {
    enum : ActionId { Left, Right };
    ActionMap map;
    map.bind(Left, Key::A);
    map.bind(Left, Key::Left);
    map.bind(Right, Key::D);

    Input in;
    in.beginFrame();
    in.handle(keyDown(Key::Left));
    auto s = in.frameSnapshot();
    REQUIRE(map.down(s, Left));
    REQUIRE(map.pressed(s, Left));
    REQUIRE_FALSE(map.down(s, Right));
    REQUIRE(map.axis(s, Left, Right) == Catch::Approx(-1));

    in.handle(keyDown(Key::D));
    REQUIRE(map.axis(in.frameSnapshot(), Left, Right) == Catch::Approx(0));
}
