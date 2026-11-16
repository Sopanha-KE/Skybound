# Skybound / SkyEngine

SkyEngine is a small code-first C++20 runtime built for Skybound, a flying-ship survival sandbox. See `docs/GAME_BRIEF.md` (game) and `docs/GameEngine.md` (engine rules and roadmap).

## Build and run
See `docs/build.md`. Short version (Windows, VS 2022 x64 prompt):
```bat
cmake --preset windows-msvc-debug
cmake --build --preset windows-msvc-debug
ctest --preset windows-msvc-debug --output-on-failure
build\windows-msvc-debug\game\Skybound.exe
```
Escape or closing the window exits.

## Status
Phase 1 (CMake, SDL3 window, fixed-step loop, input, logging, Catch2, CI): implemented. See `docs/PROGRESS.md` for the verification record and the Phase 2 plan. Verified so far: Built, Unit tested (35 tests) and run on macOS/clang only; Windows/MSVC and CI results are still pending.

## Known limitations
- The window shows nothing yet (no renderer until Phase 2).
- Loop pacing is a 1 ms sleep placeholder until vsync arrives with OpenGL.
- Only a small fixed set of keys is mapped; action maps are built in code (JSON later).
- Windows presets do not yet treat warnings as errors.
- macOS is not a supported platform.
