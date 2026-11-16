#include "sky/platform/input.hpp"

namespace sky {
namespace {

void clearEdges(InputSnapshot& s) {
    s.keysPressed.reset();
    s.keysReleased.reset();
    s.buttonsPressed.reset();
    s.buttonsReleased.reset();
    s.mouseDeltaX = 0.0f;
    s.mouseDeltaY = 0.0f;
}

}  // namespace

void Input::beginFrame() { clearEdges(frame_); }

void Input::handle(const InputEvent& e) {
    const auto k = static_cast<std::size_t>(e.key);
    const auto b = static_cast<std::size_t>(e.button);
    switch (e.type) {
    case InputEvent::Type::KeyDown:
        if (k >= kKeyCount) return;
        if (!down_[k]) {
            frame_.keysPressed.set(k);
            fixed_.keysPressed.set(k);
        }
        down_.set(k);
        break;
    case InputEvent::Type::KeyUp:
        if (k >= kKeyCount) return;
        if (down_[k]) {
            frame_.keysReleased.set(k);
            fixed_.keysReleased.set(k);
        }
        down_.reset(k);
        break;
    case InputEvent::Type::MouseButtonDown:
        if (b >= kMouseButtonCount) return;
        if (!buttonsDown_[b]) {
            frame_.buttonsPressed.set(b);
            fixed_.buttonsPressed.set(b);
        }
        buttonsDown_.set(b);
        break;
    case InputEvent::Type::MouseButtonUp:
        if (b >= kMouseButtonCount) return;
        if (buttonsDown_[b]) {
            frame_.buttonsReleased.set(b);
            fixed_.buttonsReleased.set(b);
        }
        buttonsDown_.reset(b);
        break;
    case InputEvent::Type::MouseMotion:
        frame_.mouseDeltaX += e.dx;
        frame_.mouseDeltaY += e.dy;
        fixed_.mouseDeltaX += e.dx;
        fixed_.mouseDeltaY += e.dy;
        break;
    }
}

InputSnapshot Input::frameSnapshot() const {
    InputSnapshot s = frame_;
    s.keysDown = down_;
    s.buttonsDown = buttonsDown_;
    return s;
}

InputSnapshot Input::fixedStepSnapshot() {
    InputSnapshot s = fixed_;
    s.keysDown = down_;
    s.buttonsDown = buttonsDown_;
    clearEdges(fixed_);
    return s;
}

void ActionMap::bind(ActionId action, Key key) { bindings_.push_back({action, key}); }

bool ActionMap::down(const InputSnapshot& in, ActionId action) const {
    for (const auto& b : bindings_)
        if (b.action == action && in.down(b.key)) return true;
    return false;
}

bool ActionMap::pressed(const InputSnapshot& in, ActionId action) const {
    for (const auto& b : bindings_)
        if (b.action == action && in.pressed(b.key)) return true;
    return false;
}

bool ActionMap::released(const InputSnapshot& in, ActionId action) const {
    for (const auto& b : bindings_)
        if (b.action == action && in.released(b.key)) return true;
    return false;
}

float ActionMap::axis(const InputSnapshot& in, ActionId negative, ActionId positive) const {
    return (down(in, positive) ? 1.0f : 0.0f) - (down(in, negative) ? 1.0f : 0.0f);
}

}  // namespace sky
