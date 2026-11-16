#include "sky/platform/window.hpp"

#include <SDL3/SDL.h>

#include <optional>

namespace sky {
namespace {

std::optional<Key> toKey(SDL_Scancode code) {
    switch (code) {
    case SDL_SCANCODE_ESCAPE: return Key::Escape;
    case SDL_SCANCODE_SPACE: return Key::Space;
    case SDL_SCANCODE_RETURN: return Key::Enter;
    case SDL_SCANCODE_TAB: return Key::Tab;
    case SDL_SCANCODE_LSHIFT: return Key::LeftShift;
    case SDL_SCANCODE_LCTRL: return Key::LeftCtrl;
    case SDL_SCANCODE_W: return Key::W;
    case SDL_SCANCODE_A: return Key::A;
    case SDL_SCANCODE_S: return Key::S;
    case SDL_SCANCODE_D: return Key::D;
    case SDL_SCANCODE_Q: return Key::Q;
    case SDL_SCANCODE_E: return Key::E;
    case SDL_SCANCODE_R: return Key::R;
    case SDL_SCANCODE_F: return Key::F;
    case SDL_SCANCODE_UP: return Key::Up;
    case SDL_SCANCODE_DOWN: return Key::Down;
    case SDL_SCANCODE_LEFT: return Key::Left;
    case SDL_SCANCODE_RIGHT: return Key::Right;
    default: return std::nullopt;
    }
}

std::optional<MouseButton> toButton(Uint8 button) {
    switch (button) {
    case SDL_BUTTON_LEFT: return MouseButton::Left;
    case SDL_BUTTON_RIGHT: return MouseButton::Right;
    case SDL_BUTTON_MIDDLE: return MouseButton::Middle;
    default: return std::nullopt;
    }
}

}  // namespace

struct Window::Impl {
    SDL_Window* window = nullptr;
    bool closeRequested = false;

    ~Impl() {
        if (window) SDL_DestroyWindow(window);
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
    }
};

Window::Window(std::unique_ptr<Impl> impl) : impl_(std::move(impl)) {}
Window::Window(Window&&) noexcept = default;
Window& Window::operator=(Window&&) noexcept = default;
Window::~Window() = default;

Result<Window> Window::create(const WindowConfig& config) {
    if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) return Error{std::string("SDL video init failed: ") + SDL_GetError()};

    auto impl = std::make_unique<Impl>();  // from here on, Impl's destructor undoes the init
    SDL_WindowFlags flags = config.resizable ? SDL_WINDOW_RESIZABLE : 0;
    impl->window = SDL_CreateWindow(config.title.c_str(), config.width, config.height, flags);
    if (!impl->window) return Error{std::string("SDL window creation failed: ") + SDL_GetError()};
    return Window(std::move(impl));
}

void Window::pumpEvents(Input& input) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
        case SDL_EVENT_QUIT:
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            impl_->closeRequested = true;
            break;
        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP:
            if (e.key.repeat) break;
            if (auto key = toKey(e.key.scancode)) {
                InputEvent ev;
                ev.type = e.key.down ? InputEvent::Type::KeyDown : InputEvent::Type::KeyUp;
                ev.key = *key;
                input.handle(ev);
            }
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
        case SDL_EVENT_MOUSE_BUTTON_UP:
            if (auto button = toButton(e.button.button)) {
                InputEvent ev;
                ev.type = e.button.down ? InputEvent::Type::MouseButtonDown : InputEvent::Type::MouseButtonUp;
                ev.button = *button;
                input.handle(ev);
            }
            break;
        case SDL_EVENT_MOUSE_MOTION: {
            InputEvent ev;
            ev.type = InputEvent::Type::MouseMotion;
            ev.dx = e.motion.xrel;
            ev.dy = e.motion.yrel;
            input.handle(ev);
            break;
        }
        default: break;
        }
    }
}

bool Window::closeRequested() const { return impl_->closeRequested; }

}  // namespace sky
