# Architecture (Phase 1)

## Layers
`Skybound` (`game/`) links `SkyEngine` (`engine/`); never the reverse. Game code includes only `sky/...` public headers. SDL and spdlog are PRIVATE dependencies of the engine, and a test (`public_header_tests.cpp`) fails if a public header includes a third-party library.

## Modules
- `sky/core`: `Result`, logging (`LogScope`), `IClock`, `FixedStepAccumulator`.
- `sky/platform`: `Window` (SDL pimpl), `Input` / `InputSnapshot` / `ActionMap`.
- `sky/app`: `IGame`, `GameLoop` (one testable frame), `runApplication`.

## Frame pipeline
```
input.beginFrame -> window.pumpEvents -> (Escape / close? exit)
-> GameLoop::runFrame: delta = clock.now - last
   -> accumulator.advance(delta) -> N x onFixedUpdate(dt, input.fixedStepSnapshot())
   -> onUpdate(dt, frameSnapshot) -> onRender(alpha)
```
- Fixed step defaults to 60 Hz; at most 5 steps per frame, excess time is dropped (spiral-of-death clamp).
- Simulation code receives only the constant `dt`, never the clock. The clock is injected into `GameLoop`.
- `alpha` in [0,1) is passed to `onRender` for interpolation (used from Phase 2+).

## Input edges
`pressed`/`released`/mouse delta are latched until a fixed step consumes them, so a tap is never lost when a frame runs zero steps and never duplicated when a frame runs several. `frameSnapshot()` gives per-frame edges for `onUpdate`.

## Multiplayer
Shared ships, world sync and multi-player persistence would be a major cost and are **not** being paid now. Only the cheap rules are followed (fixed step, input snapshots, no wall-clock in simulation, no global state except the logger).
