# Progress and handoff

Living status file. Update it at the end of every phase. A fresh Claude session (for example on a Windows PC) should read this first, then `docs/GameEngine.md` (engine rules and roadmap) and `docs/GAME_BRIEF.md` (game brief).

Last updated: 2026-11-16.

## Repository and environment

- GitHub: https://github.com/Sopanha-KE/Skybound (branch `master`). An older copy exists at `CambodiaTrade-DEV/SkyBound` (public, unused).
- Primary target: Windows x64, MSVC 2022. Linux is CI only. macOS is not a supported target.
- Developer machine so far: macOS. CMake 4.4.5 and Ninja came from pip (`python3 -m venv ~/.skytools && ~/.skytools/bin/pip install cmake ninja`) because Homebrew refused to install them (Command Line Tools too old). On Windows, use Visual Studio 2022's bundled CMake/Ninja.

## Phase status

| Phase | Scope | Status |
|---|---|---|
| 0 | Architecture review, repo structure, deps, interfaces, build commands, plan | Done, approved |
| 1 | CMake, SDL3, lifecycle, fixed-step loop, input, logging, Catch2, CI | Implemented. **Windows verification pending** (see below) |
| 2 | OpenGL 4.5 context, shaders, buffers, meshes, textures, camera, depth test, ImGui overlay, debug lines | **Plan written, not approved, no code yet** |
| 3-8 | See `docs/GameEngine.md` section 11 | Not started |

### Phase 1 verification record (labels from GameEngine.md section 13)

| Check | Status |
|---|---|
| macOS (AppleClang 17, `macos-dev` preset, warnings as errors) build | **Built** |
| 35 tests (34 Catch2 cases plus `Skybound.SmokeRun`) on macOS | **Unit tested**, all pass |
| macOS real window: opened, 60 fixed ticks/s in the log, Escape exits cleanly | **Run manually** (reported by developer) |
| GitHub Actions CI, `windows` and `linux` jobs | **Not verified**: pushed, result not yet reported |
| Windows MSVC build, tests, real window, Escape exit | **Not verified** |

### Windows verification checklist (do this first on a Windows PC)

From an "x64 Native Tools Command Prompt for VS 2022" in the repo root:
```bat
cmake --preset windows-msvc-debug
cmake --build --preset windows-msvc-debug
ctest --preset windows-msvc-debug --output-on-failure
build\windows-msvc-debug\game\Skybound.exe
```
Expected: configure and build succeed; 35/35 tests pass; a blank 1280x720 "Skybound" window opens; the console logs "N fixed ticks" about once a second (60 ticks per second); Space logs "Space pressed"; Escape and the close button both exit cleanly with a "Shut down after ..." line.

Then:
1. Fix anything that fails. MSVC `/W4` warnings have never been seen; fix them.
2. Set `SKY_WARNINGS_AS_ERRORS` to `ON` in the Windows presets in `CMakePresets.json`.
3. Update the table above and `README.md` status, and commit.

## Decisions made (Phase 0)

- Dependencies via FetchContent at exact tags in `cmake/Dependencies.cmake` (ADR 0001): SDL3 `release-3.4.20`, spdlog `v1.17.0`, Catch2 `v3.16.1`.
- Own `sky::Result<T>` instead of `std::expected` (ADR 0002).
- One process-wide logger installed by RAII `LogScope`, the only global state (ADR 0003).
- `IGame::onFixedUpdate(dt, const InputSnapshot&)`: input is applied in the fixed step (differs from the literal signature in GameEngine.md 7.1, which had no input).
- Input edges are latched until a fixed step consumes them (never lost, never duplicated).
- JSON config and JSON action maps are deferred to a later config phase (not yet in the roadmap; propose adding it before the first playable).
- Public headers must not include SDL, spdlog, fmt or any other third-party library; enforced by `engine/tests/public_header_tests.cpp`.

## Phase 1 layout (what exists)

```
CMakeLists.txt  CMakePresets.json  cmake/{Dependencies,CompilerWarnings}.cmake
engine/include/sky/core/      result.hpp log.hpp time.hpp fixed_step.hpp
engine/include/sky/platform/  input.hpp window.hpp
engine/include/sky/app/       game.hpp game_loop.hpp application.hpp
engine/src/...                SDL and spdlog live here only
engine/tests/                 Catch2 tests + Skybound.SmokeRun
game/src/                     main.cpp, skybound_game.{hpp,cpp}
.github/workflows/ci.yml      windows-latest + ubuntu-latest
```
`Skybound --frames N` exits after N frames (smoke test). Tests run with `SDL_VIDEODRIVER=dummy`.

## Known limitations (carry forward)

- Window is blank; no renderer yet.
- Loop pacing is a 1 ms sleep placeholder (about 880 fps). Phase 2 replaces it with vsync.
- Fixed set of 18 keys; action maps built in code.
- Windows presets do not yet treat warnings as errors.

## Phase 2 plan (PROPOSED: requires developer approval before any code)

**Goal / exit criteria (from the roadmap):** a textured, camera-controlled mesh renders on Windows, with depth test, ImGui debug overlay and debug lines.

### Important constraint
OpenGL 4.5 DSA cannot run on macOS (capped at 4.1). Phase 2 can be written and compiled on a Mac, but it can only be **Run manually** on Windows. Do Phase 2 verification on a Windows PC (or Windows VM / cloud PC). GPU-less CI runners generally cannot create a 4.5 context, so GL tests must skip when no context is available.

### Proposed new dependencies (versions looked up 2026-11-16; re-verify at implementation time)
| Library | Proposed pin | Notes |
|---|---|---|
| GLM | `1.0.3` (2025-12-31) | Header-only. Use `GLM_FORCE_DEPTH_ZERO_TO_ONE` only if clip control is adopted (decide in ADR). |
| glad (GL 4.5 core) | `v2.0.8` | glad2 normally needs Python at configure time. Recommended: generate once and commit the loader under `engine/third_party/glad`, to avoid a Python build dependency on Windows and CI. |
| Dear ImGui | `v1.93.0` (2026-10-14) | Plain (non-docking) branch is enough. SDL3 and OpenGL3 backends. Compiled out in Release via `SKY_ENABLE_IMGUI`. |
| stb_image | single header, v2.32 | stb has no release tags. 2.32 was published 2026-11-12 right after regressions, so pin a commit SHA that is at least 7 days old when implementing. One `.cpp` translation unit. |

### Proposed work, in order
1. ADR for clip-control/depth convention and for how glad is vendored.
2. `Window`: request an OpenGL 4.5 core context (`SDL_WINDOW_OPENGL`, GL attributes, `SDL_GL_CreateContext`), load glad with `SDL_GL_GetProcAddress`, `swapBuffers()`, vsync via `SDL_GL_SetSwapInterval`, framebuffer size and resize events. Remove the 1 ms sleep. Return an `Error` with a clear message if 4.5 is not available.
3. `render/` public API (engine-owned opaque types, no GL in public headers): `Shader` (load from files, compile/link errors as `Result`), `Mesh` (vertex/index buffers via DSA), `Texture` (stb_image, mipmaps), `Material` as texture slots plus parameters (keeps hand-painted vs stylized PBR open), `Camera` (view/projection, no hard-wired mode), `Renderer` (clear, depth test, draw-list that can gain instancing later).
4. `DebugDraw` lines, and an ImGui overlay (frame time, fixed-step stats, tunables), compiled out in Release.
5. Render interpolation hook: `onRender(alpha)` already exists; demonstrate it with a spinning mesh whose previous/current state is interpolated.
6. Game side: a small test scene in `game/` (textured cube plus a free fly camera with WASD and mouse). No game concepts in the engine.
7. Tests: camera matrices and any pure math helpers; shader-source preprocessing and file loading with injected data; texture decode from an embedded PNG; GL-dependent tests skip without a context.
8. Docs: update README, `docs/architecture.md`, `docs/build.md`, add ADRs; update this file.

### Main risks
- GL 4.5 context creation and driver differences on Windows (Intel/AMD/NVIDIA); mitigate with an explicit version check and clear error text.
- Cannot run rendering on the Mac; every visual claim needs a Windows run.
- glad2 generation and CRT/linking settings on MSVC.
- ImGui backend input handling versus the engine's own `Input` (events must reach both without double handling).
- Debug-only code paths must be compiled out of Release.

### Manual verification checklist for Phase 2 (run on Windows)
1. Window opens and logs the GL vendor, renderer and version (4.5 core).
2. A textured cube renders with correct depth (nearer faces occlude farther ones).
3. WASD and mouse move the camera; Escape exits.
4. The overlay shows frame time and fixed-step stats; debug lines draw; resizing the window keeps the aspect ratio correct.
5. The cube rotation stays smooth when the frame rate varies (interpolation).
6. Shader errors (introduce a typo) are reported in the log, not a crash.
7. A Release build has no overlay.

### Questions to ask the developer before starting Phase 2
1. Vendor generated glad files in the repo (recommended) or run the glad generator at configure time?
2. Is a Windows PC or VM available for Phase 2 manual verification?
3. Should the ImGui overlay be a Debug-only compile option (recommended) or a runtime toggle in all builds?

## How to continue with Claude

Start the session in the repo root and say, for example: "Read docs/PROGRESS.md, docs/GameEngine.md and docs/GAME_BRIEF.md. Run the Windows verification checklist, report results using the status labels, then ask me before starting Phase 2." Do not skip the workflow in GameEngine.md section 12, and do not start a phase without approval.
