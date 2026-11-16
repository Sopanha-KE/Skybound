# Build

## Toolchain
- Windows (primary): Visual Studio 2022 (v143), CMake 3.28+, Ninja. Run from an "x64 Native Tools Command Prompt" so `cl` is on PATH.
- Linux (CI): GCC 13+ (needs `std::format`), CMake 3.28+, Ninja.
- macOS: unsupported target; `macos-dev` preset exists for local smoke builds only.

## Commands
```bat
cmake --preset windows-msvc-debug
cmake --build --preset windows-msvc-debug
ctest --preset windows-msvc-debug --output-on-failure
build\windows-msvc-debug\game\Skybound.exe
```
Replace the preset with `linux-ci` or `macos-dev` elsewhere. Tests set `SDL_VIDEODRIVER=dummy` themselves.
`Skybound --frames N` exits after N frames (used by the smoke test).

## Dependencies
Pinned in `cmake/Dependencies.cmake` (FetchContent): SDL3 release-3.4.20, spdlog v1.17.0, Catch2 v3.16.1. The first configure is slow (SDL builds from source).

## Flags
`/W4 /permissive-` on MSVC, `-Wall -Wextra` elsewhere. `SKY_WARNINGS_AS_ERRORS` is ON in `linux-ci` and `macos-dev`, OFF for the Windows presets until a Windows build has been verified.

## Troubleshooting
- `cl` not found: use the VS developer prompt.
- No display on Linux: set `SDL_VIDEODRIVER=dummy`.
