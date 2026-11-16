# 0003: One process-wide logger (the only global state)

- **Context:** "No global mutable state" conflicts with free-function logging that any module can call.
- **Decision:** A single logger lives in `engine/src/core/log.cpp`, installed and removed by the RAII `LogScope` (scopes nest). Public API (`sky/core/log.hpp`) uses `std::format` and engine types only; spdlog stays private. With no scope installed, messages go to stderr.
- **Alternatives:** Pass a logger reference everywhere (noisy for a solo project); expose spdlog directly (leaks a third-party type).
- **Consequences:** Logging is the sole documented global. Not intended for use from multiple threads installing scopes concurrently.
