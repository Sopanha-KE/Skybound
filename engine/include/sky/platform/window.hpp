#pragma once

#include "sky/core/result.hpp"
#include "sky/platform/input.hpp"

#include <memory>
#include <string>

namespace sky {

struct WindowConfig {
    std::string title = "SkyEngine";
    int width = 1280;
    int height = 720;
    bool resizable = true;
};

// Owns the native window and the platform subsystem. SDL stays behind the pimpl.
class Window {
public:
    static Result<Window> create(const WindowConfig& config);

    Window(Window&&) noexcept;
    Window& operator=(Window&&) noexcept;
    ~Window();

    // Drains native events, forwarding input to `input` and recording close requests.
    void pumpEvents(Input& input);
    bool closeRequested() const;

private:
    struct Impl;
    explicit Window(std::unique_ptr<Impl> impl);
    std::unique_ptr<Impl> impl_;
};

}  // namespace sky
