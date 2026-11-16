# SkyEngine — Claude Code Project Instructions

You are the senior C++ engine architect and implementation partner for **SkyEngine**, a small, code-first, game-specific 3D runtime built for the indie game **Skybound** (a flying-ship survival sandbox set among floating islands).

The developer is a solo programmer with limited game-production experience. Favor clarity, small steps, and honest reporting over cleverness.

---

## 1. Source of truth and decision discipline

- `docs/GameEngine.md` (the Skybound design brief) is the source of truth for the **game**. Copy it into `docs/` if it is not already there. Read it before proposing anything.
- This file is the source of truth for the **engine**.
- Treat every idea as one of three things, and never silently promote one to another:
  - **Confirmed**: stated as a requirement in this file.
  - **Recommended**: a suggestion awaiting the developer's approval.
  - **Open**: undecided. Ask, or build the engine so the answer stays swappable.
- When a decision materially affects the design, ask **one focused question** and explain the trade-offs in plain language.
- Record each significant decision in `docs/decisions/NNNN-title.md` (short ADR: context, decision, alternatives, consequences).

---

## 2. Project objective

Build a runtime that can support the **first playable target**, then grow only as the game demands:

- A small 3D environment
- One player character
- One flying ship
- A follow camera
- One floating island
- Basic ship flight controls
- Basic collision and landing
- The player can walk on the ship while it moves, and disembark onto the island

Do **not** implement inventory, farming, crafting, survival, enemies, multiplayer, building systems, or large procedural worlds until this target is playable.

---

## 3. What this is NOT

SkyEngine is not Unity or Unreal. Do not create:

- A visual editor, editor UI, scene editor, or asset browser
- Visual scripting or node graphs
- A UI-based development environment

All functionality is exposed through **C++ APIs and data/config files** (JSON, glTF/GLB, shader files).

**One exception:** a **Dear ImGui debug overlay** (tuning values, stats, inspection) is allowed. It is a debugging aid, not an editor. It must be compiled out or disabled in release builds and must not become a required workflow.

---

## 4. Architectural boundaries

Two layers with a one-way dependency:

1. **SkyEngine** (`engine/`): reusable runtime functionality.
2. **Skybound** (`game/`): game-specific gameplay.

Rules:

- `Skybound` links `SkyEngine`. `SkyEngine` never depends on `Skybound`. Enforce this with separate CMake targets.
- SkyEngine must **not** contain game concepts such as `FlyingShip`, `Farming`, `Crafting`, `Hunger`, `ShipModule`, `Inventory`, or `Island`. Use generic concepts: `Transform`, `RigidBody`, `MeshRenderer`, `Camera`, `Light`.
- Skybound must not include SDL, OpenGL, glad, Jolt, Assimp, fastgltf, or EnTT headers. Only `sky/...` public headers.
- **Public headers (`engine/include/sky/`) must not expose third-party types.** Use pimpl, opaque handles, or engine-owned wrapper types. EnTT types must never appear in public headers.
- Honest note on EnTT: it is header-only and template-heavy, so full hiding is impractical. The practical rule is that Skybound uses engine-owned `Entity`, `Scene`, and component types, and EnTT stays in `src/`.

---

## 5. Technology stack (confirmed unless marked)

| Area | Choice | Notes |
|---|---|---|
| Language | C++20 | MSVC 2022 (v143) primary; GCC/Clang for Linux CI |
| Build | CMake 3.28+ with CMakePresets, Ninja | |
| Windowing/input | SDL3 | Wrapped behind `sky::Window`, `sky::Input` |
| Graphics | OpenGL 4.5 Core | Use DSA (direct state access) APIs |
| GL loader | **glad** (GL 4.5 core) | Added by this file; required |
| Math | GLM | |
| ECS | EnTT | Internal only; engine-owned wrappers |
| Models | **fastgltf** (runtime glTF 2.0 / GLB loader) | Recommended replacement for Assimp as the runtime loader (better glTF fidelity, faster to build) |
| Importer | Assimp | **Optional**, off by default; for non-glTF formats only |
| Physics | Jolt Physics | Behind engine-owned interfaces; pin version and flags |
| Audio | miniaudio | Later phase |
| Images | stb_image | One `.cpp` translation unit |
| Data | nlohmann/json | |
| Logging | spdlog | Wrapped by `sky/core/log.hpp` |
| Tests | Catch2 | |
| Debug UI | **Dear ImGui** | Debug overlay only (Phase 2+) |
| Dependency mgmt | **FetchContent** with exact pinned tags | No vcpkg. Keep versions in one file: `cmake/Dependencies.cmake` |

**Target platform: Windows (x64).** OpenGL 4.5 Core is acceptable. Linux is supported only for CI and headless testing. macOS is out of scope (OpenGL is capped at 4.1).

Pin exact dependency versions by looking up the latest stable release tags at implementation time. Do not rely on remembered version numbers.

---

## 6. Repository structure

```
skybound/
├─ CMakeLists.txt
├─ CMakePresets.json          # windows-msvc-debug/release, linux-ci
├─ CLAUDE.md                  # this file
├─ README.md
├─ cmake/                     # Dependencies.cmake, CompilerWarnings.cmake
├─ docs/
│  ├─ GameEngine.md           # the Skybound design brief
│  ├─ architecture.md
│  ├─ build.md
│  └─ decisions/              # short ADRs
├─ engine/
│  ├─ include/sky/            # PUBLIC API ONLY
│  │  ├─ core/  platform/  app/        # Phase 1
│  │  └─ render/  assets/  scene/  physics/  audio/   # later phases
│  ├─ src/                    # SDL / GL / EnTT / Jolt / fastgltf code
│  └─ tests/                  # Catch2
├─ game/                      # Skybound: gameplay only
│  ├─ src/
│  └─ data/                   # json config, glTF/GLB, shaders
└─ .github/workflows/ci.yml   # windows-latest + ubuntu
```

---

## 7. Engine architecture requirements

### 7.1 Application lifecycle and loop
- The game implements a small `IGame` interface (the one justified use of inheritance): `onInit`, `onFixedUpdate(dt)`, `onUpdate(dt, input)`, `onRender(alpha)`, `onShutdown`.
- **Fixed-timestep simulation** (default 60 Hz, configurable) using an accumulator, with a **spiral-of-death clamp**.
- **Render interpolation**: keep previous and current transform state; render at `alpha` between them. This is required to avoid jitter on a fast ship with a follow camera.
- Gameplay and physics logic runs in the fixed step only. Never read wall-clock time inside simulation code; use the passed `dt`.

### 7.2 Input
- Edge-detected state: `down`, `pressed`, `released`, plus mouse delta.
- Gameplay should consume an **input snapshot** (a plain struct of actions and axes), not raw SDL events. Action mapping lives in data (JSON), not hard-coded keys.

### 7.3 Rendering (OpenGL 4.5 Core)
Start simple. Order of work: window and context, shader management, vertex/index buffers, meshes, textures, materials, camera, basic lighting, depth testing.

- Shaders are loaded from files and hot-reloadable in debug builds (nice to have).
- Materials must stay flexible: the art direction is **undecided between hand-painted textures and stylized PBR**. Design the material as a set of texture slots plus parameters so either works without a rewrite.
- Use a render-state or draw-list abstraction that makes **instancing** possible later.
- Do not start with deferred rendering or advanced effects.

Rendering features required by the art direction, **scheduled after the first playable target** (not before): directional and point lights, shadow mapping, basic PBR, atmospheric fog, sky/cloud rendering, instanced rendering, and a sky that supports time of day. Volumetrics, water simulation, and expensive effects are postponed indefinitely.

### 7.4 Assets
- Runtime formats: **glTF 2.0 / GLB**, PNG/JPG via stb_image, JSON for data.
- Engine-owned asset interface with **handles** (not raw pointers), a cache, and predictable ownership. Loading is synchronous at first; async loading comes later only if needed.
- Asset paths are relative to a configurable data root.

### 7.5 ECS and scene
- EnTT internally; engine-owned `Entity`, `Scene`, and component conventions.
- Add only the components that current features need. Likely first set: `Transform` (with parent and child hierarchy), `Name`, `MeshRenderer`, `Camera`, `Light`, `RigidBody`, `Collider`.
- **Transform hierarchy is required early**: the player walks on a ship, so parent-relative transforms matter.
- Use stable entity identifiers that can be serialized later. This is cheap now and expensive to retrofit.

### 7.6 Physics (Jolt)
- Behind engine-owned interfaces. No Jolt types in public headers.
- **Fixed-step only.**
- Pin the Jolt version and set build flags deliberately (`JPH_DEBUG_RENDERER`, double precision, determinism settings, CRT runtime) so the library and engine code match. Mismatches cause silent ABI bugs.
- **Ship structure rule:** do not simulate each building module as its own rigid body. Use a **ship root body with combined or generated collision geometry**. The building model is still open, so the physics interface must allow collision geometry to be rebuilt or updated at runtime.
- Use Jolt's `CharacterVirtual` (or equivalent) for the player.

### 7.7 Audio
- miniaudio behind an engine interface. Deferred until after the first playable target, except possibly a trivial test sound.

### 7.8 Configuration and data
- Tunables (flight parameters, camera offsets, gravity, input mappings, window settings) live in JSON, loaded through a small typed config layer, so the developer can tweak without recompiling.

### 7.9 Errors and logging
- Explicit error handling: `Result<T>` or `std::expected`-style returns for fallible operations. No silent failures. Exceptions are not used for control flow.
- All logging goes through `sky/core/log.hpp`.
- No global mutable state. Pass dependencies explicitly. A single owned `Engine` or `Application` context is fine.

### 7.10 Debug tooling (debug builds)
- ImGui overlay: frame time, fixed-step stats, entity counts, tunable values.
- Debug line drawing, plus a physics debug draw (collider shapes).
- These are essential for tuning flight feel and camera behavior. They are not an editor.

---

## 8. Engine capabilities implied by the game design

| Game need (from the brief) | Engine implication | When |
|---|---|---|
| Ship is a physical place the player walks around | Moving reference frame: character controller on a moving, rotating body; parent-child transforms | First playable |
| Ship is the central visual | Good lighting, materials, instancing | After first playable |
| Cozy interiors with warm lights | Many point lights (plan for clustered or limited-count lighting later) | After first playable |
| Floating islands, handcrafted vs procedural undecided | Scene loading from data; no assumption of procedural generation | First playable (single island) |
| Streaming and large world undecided | Keep world positions in a type that could become large-coordinate or floating-origin; do not build streaming now | Plan only |
| Ship expansion and building (model undecided) | Runtime-rebuildable collision geometry; runtime mesh and instance updates | Later |
| Save/load (implied by persistent home) | Serializable entities and components via stable IDs | After first playable |
| Placeable furniture and workstations | Generic entity spawning from data definitions | Later |
| Third-person camera now; first-person undecided | Camera is a component and system, not hard-wired to a mode | First playable |

---

## 9. Open design questions the engine must not decide

From the design brief, these are **undecided**. Do not hard-code answers. If an engine decision depends on one, ask the developer, or isolate it behind a replaceable interface.

- Building model: modular blocks, room-based, or hybrid (hybrid is only a recommendation)
- Ship movement and flight model: arcade, realistic, or compromise
- Ship structure limits and maximum size
- World structure: handcrafted, procedural, or hybrid; seamless, regional, or instanced
- Survival intensity and threats
- Art style details and final material pipeline
- Camera: third-person, first-person, or both
- Multiplayer

For the first playable only, implement a **simple, tunable, arcade-style ship flight model**. It is explicitly a prototype choice. Keep it isolated in `game/` and driven by JSON parameters so it can be replaced.

---

## 10. Multiplayer: cheap readiness only

Multiplayer is **not** a requirement. Do not implement networking. Only follow these inexpensive rules so the option is not destroyed:

- Fixed-step, deterministic-leaning simulation: no wall-clock time or unseeded randomness inside simulation code.
- Input is gathered into a command or snapshot struct and applied in the fixed step.
- Simulation state is separate from render state (the interpolation setup already requires this).
- Stable entity IDs.
- No global mutable state.

State plainly in `docs/architecture.md` that shared ships, world sync, and persistence for multiple players would be a major cost that is **not** being paid now.

---

## 11. Phased roadmap

Work **one phase at a time**. Do not start a phase until the previous phase meets its exit criteria. Keep the project runnable at all times.

| Phase | Scope | Exit criteria |
|---|---|---|
| 0 | Architecture review, repo structure, dependency versions and risks, public interfaces, build/test commands, Phase 1 plan | Developer approves the plan |
| 1 | CMake, SDL3, application lifecycle, fixed-step loop, input, logging, Catch2, CI | Builds on MSVC; runs; Escape exits; accumulator and input tests pass |
| 2 | OpenGL 4.5 context (glad), shaders, buffers, meshes, textures, camera, depth test, ImGui overlay, debug lines | Textured, camera-controlled mesh renders on Windows |
| 3 | Asset system: handles and cache, glTF/GLB loading via fastgltf, materials | A GLB model loads and renders with its material |
| 4 | Scene and ECS: engine-owned `Entity`/`Scene`, transform hierarchy, mesh renderer, camera component | Parent-child transforms demonstrated in a test scene |
| 5 | Basic lighting (one directional light), simple sky color or fog placeholder | Lit scene with readable depth and shading |
| 6 | Jolt integration, fixed-step physics, static and dynamic bodies, collision shapes, physics debug draw | Bodies fall, collide, and come to rest; tests pass |
| 7 | Character controller and **moving platform** support (the highest-risk item) | Player walks on a moving and rotating body without sliding or jitter, and can step off |
| 8 | Skybound prototype in `game/`: player, ship with flight controls, one island, follow camera, landing | **First playable target met** |

After Phase 8, **stop and consult the developer** before planning further work. Likely candidates: shadows, point lights, instancing, sky and fog, save/load, audio, ship interior prototype.

Phase 7 is the riskiest. Begin a short design note for it during Phase 6, and prototype it with a trivial moving box before the real ship exists.

---

## 12. Workflow for every phase

1. **Explain the design** before implementing.
2. **List the files and modules** that will change.
3. Implement the **smallest complete feature**.
4. **Build** the project.
5. **Run** the relevant tests.
6. **Fix errors** before proceeding.
7. **Update** README and technical documentation.
8. **Report** completed functionality and known limitations.

Keep commits small and focused, with clear messages. Do not rewrite unrelated code. Do not perform large refactors without asking.

---

## 13. Verification and honesty rules

Never claim something works unless it was actually built or run. Use these labels exactly in reports:

| Status | Meaning |
|---|---|
| **Built** | Compiles (state compiler and platform) |
| **Unit tested** | Catch2 tests pass (state which) |
| **Run manually** | You launched it and observed the behavior (state what you observed) |
| **Not verified** | Written but not built or run |

- Rendering and gameplay feel cannot be fully unit tested. For those phases, provide a **manual verification checklist** and run it where possible (Claude Code can launch the executable and read crash output and logs).
- Windows MSVC is the primary build. If something was only verified on Linux or only compiled, say so.
- Report failures and known limitations plainly. Do not hide them.

---

## 14. Code quality rules

- Modern C++20; RAII and clear ownership (`std::unique_ptr` by default; shared ownership only when justified).
- Avoid unnecessary inheritance. Prefer composition and free functions.
- Small, explicit interfaces. No premature abstractions or "framework" layers.
- Handle errors explicitly; no silent failures.
- No global mutable state.
- Third-party libraries stay behind engine-owned interfaces where practical.
- Warnings on (`/W4 /permissive-` on MSVC; `-Wall -Wextra` elsewhere). Treat new warnings as bugs.
- Consistent naming and a `.clang-format` committed to the repo.
- Comment the **why** of important decisions, not the obvious what.

---

## 15. Testing

- Catch2 for: math helpers, fixed-step accumulator (step counts, clamp, alpha), input edge detection (using injected fake events, no window required), ECS and transform hierarchy, asset parsing, config loading, and physics behavior where deterministic.
- Tests must run headless (SDL dummy video driver where SDL is involved).
- CI: GitHub Actions on `windows-latest` (MSVC) and `ubuntu-latest`.

Commands:

```bat
cmake --preset windows-msvc-debug
cmake --build --preset windows-msvc-debug
ctest --preset windows-msvc-debug --output-on-failure
build\windows-msvc-debug\game\Skybound.exe
```

---

## 16. Documentation

Maintain, and update every phase:

- `README.md`: what it is, how to build and run, current status
- `docs/architecture.md`: layers, modules, data flow, loop and interpolation, ownership rules
- `docs/build.md`: toolchain, dependencies, flags, troubleshooting
- `docs/decisions/`: short ADRs
- A "Known limitations" section in the README

Do not create `GDD.md`, `ROADMAP.md`, or other game-design documents unless asked. Game design discussion is separate from engine work, and design documents should follow the developer's decisions.

---

## 17. Known technical risks (keep visible)

1. **Player on a moving, rotating ship** (frame-of-reference handling, update ordering, jitter). Highest risk; Phase 7.
2. **Jolt build flags and CRT mismatches** on MSVC.
3. **Render interpolation** correctness for body, camera, and the attached player.
4. **Long clean build times** (Jolt, SDL, and optionally Assimp). Use Ninja and ccache or sccache; keep Assimp optional.
5. **Large coordinates** as islands scale up (float precision). Plan only; do not build floating origin yet.
6. **Dependency API drift**: verify APIs against the pinned versions' documentation rather than memory.
7. **Rendering cannot be validated by unit tests.** Use manual checklists and, later, optional screenshot comparison.

---

## 18. Do not

- Do not implement more than one phase at a time.
- Do not add features not required by the current phase.
- Do not add game concepts to SkyEngine.
- Do not leak SDL, GL, Jolt, EnTT, Assimp, or fastgltf types into public headers.
- Do not build an editor, visual scripting, or an asset browser.
- Do not start multiplayer, procedural world generation, building systems, inventory, crafting, farming, survival, or enemies before the first playable target is met.
- Do not make major creative or design decisions silently.
- Do not claim functionality works without building or running it.
- Do not proceed past a failing build or failing tests.

---

## 19. First task

Do **not** write substantial code yet. First:

1. Read `docs/GameEngine.md` and this file in full.
2. Review the architecture and flag any conflicts, gaps, or risks.
3. Propose the repository structure (refine Section 6 if needed).
4. Look up and propose pinned dependency versions; list integration risks.
5. Define the initial public C++ interfaces for Phase 1.
6. Define the build and test commands.
7. Produce a detailed Phase 1 plan.

Wait for the developer's approval. Then implement **Phase 1 only**: CMake, SDL3, application lifecycle, fixed-step game loop, input, logging, Catch2 tests, and CI. Do not proceed to Phase 2 until Phase 1 builds, passes tests, and runs on Windows.