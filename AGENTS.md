# SkyBound / SkyEngine

Engine rules: `docs/GameEngine.md`. Game brief: `docs/GAME_BRIEF.md`. Work one phase at a time.

## Build and test
- Windows (primary, VS 2022 x64 prompt): `cmake --preset windows-msvc-debug`, `cmake --build --preset windows-msvc-debug`, `ctest --preset windows-msvc-debug --output-on-failure`
- Linux CI: same with `linux-ci`
- macOS local smoke only (unsupported platform): same with `macos-dev`
- Run: `build/<preset>/game/Skybound` (`.exe` on Windows)

## Conventions
- Public headers in `engine/include/sky/` must not include SDL, spdlog or fmt.
- Report status with the labels Built / Unit tested / Run manually / Not verified.
