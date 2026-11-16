#pragma once

#include <bitset>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace sky {

enum class Key : std::uint8_t {
    Escape, Space, Enter, Tab, LeftShift, LeftCtrl,
    W, A, S, D, Q, E, R, F,
    Up, Down, Left, Right,
    Count
};
inline constexpr std::size_t kKeyCount = static_cast<std::size_t>(Key::Count);

enum class MouseButton : std::uint8_t { Left, Right, Middle, Count };
inline constexpr std::size_t kMouseButtonCount = static_cast<std::size_t>(MouseButton::Count);

// Engine-owned, SDL-free event. The platform layer translates native events into these.
struct InputEvent {
    enum class Type : std::uint8_t { KeyDown, KeyUp, MouseButtonDown, MouseButtonUp, MouseMotion };
    Type type = Type::KeyDown;
    Key key = Key::Escape;
    MouseButton button = MouseButton::Left;
    float dx = 0.0f;
    float dy = 0.0f;
};

// Plain data handed to gameplay. Edges (pressed/released) and mouse delta are only
// reported once per frame and once per fixed step (see Input::fixedStepSnapshot).
struct InputSnapshot {
    std::bitset<kKeyCount> keysDown, keysPressed, keysReleased;
    std::bitset<kMouseButtonCount> buttonsDown, buttonsPressed, buttonsReleased;
    float mouseDeltaX = 0.0f;
    float mouseDeltaY = 0.0f;

    bool down(Key k) const { return keysDown[static_cast<std::size_t>(k)]; }
    bool pressed(Key k) const { return keysPressed[static_cast<std::size_t>(k)]; }
    bool released(Key k) const { return keysReleased[static_cast<std::size_t>(k)]; }
    bool down(MouseButton b) const { return buttonsDown[static_cast<std::size_t>(b)]; }
    bool pressed(MouseButton b) const { return buttonsPressed[static_cast<std::size_t>(b)]; }
    bool released(MouseButton b) const { return buttonsReleased[static_cast<std::size_t>(b)]; }
};

class Input {
public:
    // Call once at the start of each rendered frame, before feeding that frame's events.
    void beginFrame();
    void handle(const InputEvent& event);

    // Edges and mouse delta for the whole frame (for onUpdate and the app itself).
    InputSnapshot frameSnapshot() const;

    // Edges accumulated since the previous fixed step are delivered exactly once, to the
    // next fixed step, even if that happens in a later frame or several steps run in one
    // frame. Later steps in the same frame see only the held state. Non-const: consumes.
    InputSnapshot fixedStepSnapshot();

private:
    std::bitset<kKeyCount> down_;
    std::bitset<kMouseButtonCount> buttonsDown_;
    InputSnapshot frame_;  // edges reset every beginFrame()
    InputSnapshot fixed_;  // edges reset when a fixed step consumes them
};

// Maps gameplay actions to keys. Built in code for now; JSON loading comes later.
using ActionId = std::uint32_t;

class ActionMap {
public:
    void bind(ActionId action, Key key);
    bool down(const InputSnapshot& in, ActionId action) const;
    bool pressed(const InputSnapshot& in, ActionId action) const;
    bool released(const InputSnapshot& in, ActionId action) const;
    // +1 when `positive` is held, -1 when `negative` is held, 0 if both or neither.
    float axis(const InputSnapshot& in, ActionId negative, ActionId positive) const;

private:
    struct Binding {
        ActionId action;
        Key key;
    };
    std::vector<Binding> bindings_;
};

}  // namespace sky
