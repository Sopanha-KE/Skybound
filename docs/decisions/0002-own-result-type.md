# 0002: Engine-owned Result type

- **Context:** Fallible operations must return explicit results, not exceptions. The project targets C++20, and `std::expected` is C++23 (MSVC needs `/std:c++latest`).
- **Decision:** `sky::Result<T>` / `Result<void>` with a string-message `sky::Error` in `sky/core/result.hpp`.
- **Alternatives:** `tl::expected` (extra dependency), switching to C++23.
- **Consequences:** Minimal API (`ok`, `value`, `error`). Can be swapped for `std::expected` once C++23 is adopted.
