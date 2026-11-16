# 0001: FetchContent with exact pinned tags

- **Context:** Solo developer, Windows primary, Linux CI. Need reproducible builds without a package manager.
- **Decision:** All third-party libraries come from CMake FetchContent at exact release tags, all declared in `cmake/Dependencies.cmake`. No vcpkg. Phase 1 pins: SDL3 `release-3.4.20`, spdlog `v1.17.0`, Catch2 `v3.16.1`.
- **Alternatives:** vcpkg, Conan, vendored sources.
- **Consequences:** First configure downloads and builds everything (SDL is slow); CI caches `_deps`. Upgrading is a one-line change per library.
