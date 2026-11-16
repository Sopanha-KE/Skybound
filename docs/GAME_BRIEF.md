# Skybound — Initial Game Design Brief

> **Document purpose:** This file captures the current vision for the flying-ship survival sandbox game and gives Claude Code a clear starting point for helping develop the project.
>
> **Project status:** Early concept / pre-production. This is a design brief, not a final specification. Do not silently turn undecided ideas into fixed requirements.

---

## 1. Project Overview

### Working title
**Skybound** — *A living home above the clouds.*

The title is temporary and can be changed later.

### High-level concept

Skybound is a survival sandbox adventure set in a world of floating islands. The player's flying ship is the heart of the experience: it is their transport, workshop, farm, storage, shelter, and permanent home.

Players begin with a small, basic flying vessel and gradually transform it into a larger, personalized flying home. They explore floating islands, gather resources, discover secrets, craft equipment, grow food, improve their ship, and travel to increasingly distant or dangerous places.

The game should not feel like a simple sky-pirate combat game or a short airship demo. The central fantasy is **building and living in a customizable flying home while exploring a large sky world**.

### Core player fantasy

"I have my own flying home. I can build it, improve it, decorate it, grow food on it, take it anywhere, discover new places, and gradually turn it into something extraordinary."

---

## 2. Design Pillars

The following five pillars should guide design decisions.

### 2.1 Ship building and customization

The ship is the player's main base and should be highly personal.

Players should be able to:
- Expand the ship over time.
- Add rooms, floors, decks, stairs, doors, walls, and useful structures.
- Place furniture, storage, crafting stations, engines, and decorations.
- Create functional spaces such as bedrooms, workshops, kitchens, and farms.
- Improve the ship's capabilities as they progress.

The building system should support creativity while remaining technically achievable for an indie project.

### 2.2 Exploration

The world should encourage players to leave their ship and discover new places.

Potential locations include:
- Floating islands of different sizes and biomes.
- Forests, cliffs, caves, and unusual rock formations.
- Abandoned structures and ancient ruins.
- Resource-rich locations.
- Dangerous regions that require better equipment or ship upgrades.
- Hidden locations, environmental stories, and discoveries.

Exploration should provide useful resources, new crafting opportunities, secrets, and reasons to improve the ship.

### 2.3 Survival

Survival should create meaningful decisions without becoming unnecessarily tedious.

Potential systems include:
- Food gathering, cooking, and farming.
- Resource management.
- Ship maintenance and repair.
- Environmental hazards.
- Threats encountered on islands or during travel.
- Preparation for longer or more dangerous expeditions.

Exact survival meters, penalties, and difficulty are undecided. Avoid adding excessive hunger, thirst, temperature, or durability systems without evaluating whether each one improves the core experience.

### 2.4 Progression

Progression should make the player feel that both their ship and their capabilities are growing.

Possible progression paths:
- Better tools and equipment.
- Improved crafting stations.
- More advanced materials.
- Better engines and ship components.
- New building pieces and room options.
- Access to new biomes and distant regions.
- New farming, cooking, and production capabilities.

Progression should connect exploration, gathering, crafting, and ship improvement.

### 2.5 A living world and home

The ship should feel like a place where the player lives, not merely a vehicle or menu.

Examples:
- Players physically walk between rooms and decks.
- Machines and crafting stations exist in the world and can be interacted with.
- The player can see the world moving around the ship.
- The ship has cozy, useful, and personalized spaces.
- Farming and production happen on the ship.
- Lighting, sound, animation, and environmental details make the ship feel inhabited.

---

## 3. Core Gameplay Loop

The intended repeatable gameplay loop is:

1. **Explore** — Travel to floating islands and discover resources, locations, and secrets.
2. **Gather** — Collect wood, stone, minerals, plants, food, and other materials.
3. **Return to the ship** — Store resources, cook food, rest, and process materials.
4. **Build and upgrade** — Expand the ship, create rooms, improve engines, craft tools, and grow crops.
5. **Reach new destinations** — Use improved equipment and ship capabilities to travel farther and explore more challenging areas.
6. **Repeat** — Discover new resources and opportunities that support further development.

### Gameplay-loop principle

Every major activity should connect to another activity.

Examples:
- Farming provides food for expeditions.
- Mining provides materials for ship upgrades.
- Better engines allow access to distant islands.
- Exploration discovers recipes, blueprints, and rare materials.
- Crafting better tools makes resource gathering more efficient.
- Ship expansion creates room for more production and farming.

Avoid designing features as isolated systems that do not support the main loop.

---

## 4. The Flying Ship

### 4.1 Ship identity

The flying ship is the game's central feature and the player's persistent home base.

It should serve as:
- Transportation through the sky.
- A customizable building space.
- A home and shelter.
- A storage and inventory hub.
- A crafting and production center.
- A farm or greenhouse location.
- A platform for future defensive or combat systems.

The player should develop an emotional attachment to the ship as it grows over the course of the game.

### 4.2 Potential ship spaces

These are examples, not mandatory launch features.

| Space | Potential function |
|---|---|
| Living room | Furniture, decoration, comfort, and storage |
| Bedroom | Resting and possibly saving or time progression |
| Workshop | Crafting tools, equipment, and components |
| Engine room | Engines, fuel, propulsion, and ship upgrades |
| Farm deck | Crops, plants, trees, and possibly livestock |
| Kitchen | Cooking and food preparation |
| Navigation room | Maps, destinations, route planning, and ship controls |
| Storage room | Organized resource and item storage |
| Defence area | Optional protection systems for later development |

### 4.3 Physical interaction

The ship should feel like a physical place:
- The player walks between rooms and decks.
- Stairs, ladders, doors, and passages connect spaces.
- Machines and workstations are placed in the world.
- The player can interact with ship controls and systems.
- The player can see the ship traveling through the environment.

Do not make the ship primarily a flat menu or abstract upgrade screen.

### 4.4 Ship building approaches

Three possible approaches have been identified:

**A. Modular block building**
- Similar to Minecraft-style construction.
- Players place individual blocks, walls, floors, furniture, and engines.
- Offers a high degree of creative freedom.
- Could create significant technical and design complexity.

**B. Room-based building**
- Players construct predefined rooms and connect them.
- Easier to constrain and potentially more achievable for a solo developer.
- Offers less freedom unless room customization is extensive.

**C. Hybrid building — current recommendation, not yet confirmed**
- Players construct rooms or structural modules.
- Players can customize interiors, furniture, decorations, and selected structural details.
- Aims to balance creative freedom with manageable development complexity.

**Important:** The user has not made a final selection. Treat the hybrid approach as the current recommendation only. Help compare the options before locking the system.

---

## 5. World and Exploration

### 5.1 World concept

The setting is a world of floating islands surrounded by open sky and clouds.

The world should make flying meaningful. Players should have interesting destinations to find, reasons to travel, and places worth returning to.

### 5.2 Potential world content

- Different island sizes and shapes.
- Distinct biomes and environmental conditions.
- Forests, cliffs, caves, waterfalls, and unusual natural formations.
- Ruins, abandoned settlements, and points of interest.
- Common and rare resources.
- Hidden areas and environmental storytelling.
- More dangerous regions that require preparation.

### 5.3 Exploration rewards

Exploration may reward players with:
- Building materials.
- Rare resources.
- Food and seeds.
- Crafting recipes and blueprints.
- Equipment and tools.
- Knowledge about the world.
- New destinations and progression opportunities.

### 5.4 World generation

Procedural generation has not been decided.

Claude Code should help evaluate:
- Handcrafted islands versus procedural islands.
- A hybrid approach using handcrafted points of interest within generated regions.
- World streaming and performance implications.
- How to make exploration feel varied without requiring an enormous amount of handcrafted content.

Do not assume that a fully procedural infinite world is required.

---

## 6. Survival, Farming, and Crafting

### 6.1 Survival

Survival should support exploration and make preparation meaningful.

Potential features:
- Gathering food.
- Cooking.
- Repairing the ship.
- Managing expedition supplies.
- Handling environmental hazards.
- Preparing for dangerous islands.

The exact survival model is still open.

### 6.2 Farming

Farming is an important part of the intended ship-as-home fantasy.

Potential features:
- Placeable farm plots or planters.
- Growing crops on a deck or in a greenhouse.
- Harvesting food and seeds.
- Using harvested crops in cooking.
- Improving farming capacity as the ship expands.

Livestock and advanced agriculture are optional future possibilities, not initial requirements.

### 6.3 Crafting

Crafting should connect gathered resources to useful progression.

Potential crafting categories:
- Tools and equipment.
- Building materials and ship components.
- Furniture and decorations.
- Cooking stations and food.
- Engines and mechanical parts.
- Farming equipment.
- Advanced technology.

Crafting stations should be physical objects placed on the ship where practical.

---

## 7. Progression and Long-Term Goals

The player should begin with limited capabilities and gradually build a capable, personalized flying home.

Possible progression stages:

1. **Starting vessel** — A small ship with basic shelter, storage, and limited functionality.
2. **Functional home** — Add essential rooms, a workshop, cooking, and basic farming.
3. **Improved vessel** — Upgrade propulsion, storage, tools, and building options.
4. **Advanced exploration** — Reach distant islands and more dangerous regions.
5. **Personalized flying base** — Build a larger ship with specialized rooms and production areas.
6. **Late-game expansion** — Discover rare materials, advanced technologies, and ambitious building possibilities.

These are conceptual stages, not a finalized progression tree. Costs, recipes, unlock requirements, and pacing need further design.

---

## 8. Visual Direction and Art Style

### 8.1 Visual Identity

The current recommended art direction for Skybound is **Stylized 3D Low-Poly with Hand-Painted Textures and Atmospheric Lighting**.

This is a recommendation, not a finalized production decision.

The game should have a colorful, painterly fantasy appearance that combines simplified 3D geometry, expressive silhouettes, natural colors, and atmospheric environments.

The visual identity should communicate three feelings:

- **Cozy:** The flying ship should feel like a comfortable, personal home.
- **Adventurous:** Floating islands and distant environments should encourage exploration.
- **Magical:** The sky world should feel unusual, beautiful, and full of discoveries without necessarily relying on excessive magical effects.

Avoid extremely minimalist low-poly visuals that make the world feel empty. The goal is simplified geometry with enough environmental and material detail to make the world feel alive.

### 8.2 Core Art Style Characteristics

**Geometry**
- Stylized, simplified 3D models.
- Readable silhouettes and recognizable object shapes.
- Slightly exaggerated proportions where appropriate.
- Low-to-medium polygon complexity.
- Consistent visual detail across different asset categories.

**Textures and Materials**
- Hand-painted or painterly-looking textures.
- Subtle surface variation and painted highlights.
- Natural wood, stone, metal, fabric, and vegetation materials.
- Controlled material roughness rather than excessive reflectivity.
- Avoid overly realistic textures that conflict with the stylized geometry.

**Color Palette**
- Rich natural greens for forests and vegetation.
- Warm browns for wooden ships and architecture.
- Soft blues for the sky and distant environments.
- Warm orange and yellow for sunlight and interior lighting.
- Distinctive palettes for different biomes and environmental regions.

**Lighting and Atmosphere**
- Soft directional sunlight.
- Atmospheric fog and distance haze.
- Clouds that create depth and environmental scale.
- Warm interior lighting aboard the ship.
- Distinct lighting conditions for different times of day.
- Carefully controlled shadows and ambient lighting.

### 8.3 Environment Art Direction

Floating islands should have distinctive silhouettes and recognizable environmental features.

Potential visual elements include:
- Grass-covered island surfaces.
- Exposed rocky undersides.
- Hanging roots and vegetation.
- Waterfalls falling into clouds.
- Distinctive trees and plants.
- Natural rock formations.
- Caves, ruins, and abandoned structures.

Biomes should share the same overall artistic language while having different color palettes, vegetation, architecture, and atmospheric characteristics.

The environment should remain visually interesting both from the flying ship and when exploring an island on foot.

### 8.4 Flying Ship Art Direction

The flying ship is the most important visual element in Skybound.

Its appearance should communicate the player's progression and personal investment.

Recommended characteristics:
- Wooden structures with visible material details.
- Distinctive engines and propulsion components.
- Modular architectural shapes.
- Warm windows and interior lighting.
- Furniture, plants, lanterns, and decorations.
- Visible crafting equipment and production stations.
- Different materials and architectural details as the ship improves.

The starting vessel should look small and functional. Advanced ships should have the potential to resemble personalized flying homes, workshops, greenhouses, or small floating settlements.

### 8.5 Interior Art Direction

Ship interiors should feel warmer and more comfortable than the outside environment.

Recommended visual elements:
- Wooden floors and walls.
- Warm light sources.
- Furniture and decorative objects.
- Plants and small gardens.
- Windows showing the surrounding sky.
- Functional crafting and cooking equipment.
- Small environmental animations and ambient details.

The goal is to make players enjoy spending time aboard their ship, rather than treating it as an inventory interface or transportation vehicle.

### 8.6 Character and Creature Direction

Characters and creatures should follow the same stylized visual language.

Potential characteristics:
- Readable silhouettes.
- Simplified anatomy and proportions.
- Expressive animations.
- Consistent material and texture treatment.
- Distinctive designs for different environments and creature types.

Detailed character designs, enemy ecosystems, and extensive animation sets are not initial production priorities.

### 8.7 Technical Art Constraints

The art direction must remain achievable for a solo developer.

Initial technical guidelines:
- Prefer modular and reusable 3D assets.
- Use a consistent texture and material pipeline.
- Reuse vegetation and environmental assets where appropriate.
- Avoid unnecessary high-polygon models.
- Use instancing for repeated objects.
- Keep texture resolutions and material complexity controlled.
- Prioritize visual readability and composition over raw graphical fidelity.

The rendering system should initially support directional lighting, point lighting, shadows, basic PBR materials, atmospheric fog, sky rendering, and instanced objects.

Advanced volumetric effects, complex water simulation, and expensive rendering features should be postponed until the core game is playable.

### 8.8 Visual References

The following games may be studied for individual visual characteristics:

- Windbound: Stylized natural environments and exploration.
- The Legend of Zelda: Breath of the Wild: Environmental composition, lighting, and atmospheric depth.
- Valheim: Modular construction and practical building systems.
- Palia: Cozy environments, furniture, and welcoming home aesthetics.

These are references for studying visual principles, not templates to copy.

### 8.9 Current Art Direction Status

**Current recommendation:** Stylized painterly 3D with simplified geometry, hand-painted-looking materials, and atmospheric lighting.

**Still undecided:**
- Exact polygon density and geometric complexity.
- Whether textures should be primarily hand-painted or stylized PBR.
- Character proportions and visual design.
- The balance between whimsical and realistic fantasy.
- Final color palette.
- Shader and lighting implementation.
- Whether the world should lean more toward cozy fantasy or mysterious adventure.

Validate the direction through a small visual prototype containing one flying ship, one floating island, a few trees, and a basic interior before producing a large asset library.

---

## 9. Multiplayer

Multiplayer is a possible future feature, but it is not a confirmed initial requirement.

The first priority should be a strong single-player experience.

When considering architecture, identify decisions that could make future multiplayer extremely difficult to add. However:
- Do not implement multiplayer prematurely.
- Do not let hypothetical multiplayer requirements delay a playable single-player prototype.
- Clearly separate essential early architecture from optional future-proofing.
- Explain the cost of supporting shared ships, world synchronization, persistence, and multiple players.

---

## 10. Solo-Developer Constraints

### Current developer context

- Team size: Solo developer.
- Game-development experience: Primarily programming/software development; limited or no established game-production experience.
- Goal: Build an achievable indie game.
- The developer is willing to learn game-development concepts and tools.

### Scope principle

The full vision is ambitious. The project must be developed in small, testable milestones.

Prioritize:
1. A fun and reliable movement/exploration prototype.
2. A small working flying ship.
3. A basic ship-building interaction.
4. A small test environment with one or more islands.
5. Resource gathering and inventory.
6. A simple crafting loop.
7. One useful ship upgrade.
8. A basic farming loop.
9. A polished vertical slice.

Avoid starting with:
- A huge open world.
- Hundreds of building pieces.
- Complex procedural generation.
- Full multiplayer.
- Large enemy ecosystems.
- Advanced ship combat.
- Many biomes and extensive quest content.

These can be evaluated after the core loop is proven.

---

## 11. Important Open Design Questions

Claude Code should help the developer resolve these questions through discussion and documented trade-offs. Do not invent final answers.

1. **Building model:** Modular blocks, room-based construction, or hybrid?
2. **Ship movement:** Is the ship continuously moving, manually piloted, or able to switch between travel and parked modes?
3. **Flight model:** Arcade-style controls, realistic physics, or a compromise?
4. **Ship structure:** Can players build any shape, or are there structural limits and predefined attachment points?
5. **Ship size:** What prevents unlimited ship expansion?
6. **World structure:** Handcrafted islands, procedural islands, or a hybrid?
7. **Exploration format:** Seamless open world, separate regions, or instanced islands?
8. **Survival intensity:** Cozy survival, moderate survival, or challenging survival?
9. **Threats:** Are there hostile creatures, environmental hazards, ship attacks, or a combination?
10. **Ship damage:** Can engines and structures be damaged? How does repair work?
11. **Farming depth:** Simple crop plots or a more involved agricultural system?
12. **Progression:** Technology tree, discovered blueprints, crafting progression, or a combination?
13. **Art direction:** Stylized low-poly, voxel, hand-painted, or another direction?
14. **Camera:** First-person, third-person, or both?
15. **Multiplayer:** Is it strictly single-player initially, with multiplayer considered later?

For each major decision, provide:
- Recommended option.
- Alternatives.
- Advantages and disadvantages.
- Technical complexity.
- Impact on solo-development scope.
- A small prototype or test that could validate the decision.

---

## 12. Instructions for Claude Code

You are assisting with the design and eventual development of **Skybound**, a solo indie flying-ship survival sandbox game.

### Your role

Act as a combination of:
- Game designer.
- Technical game-development advisor.
- Systems designer.
- Software architect.
- Practical production planner.

Help the developer turn the concept into a coherent, achievable game.

### How to work with the developer

1. Read this entire document before proposing implementation.
2. Treat this document as the current source of truth for the concept.
3. Distinguish confirmed ideas, recommendations, and undecided ideas.
4. Do not silently make major creative decisions on the developer's behalf.
5. Ask focused questions when a decision materially affects the design.
6. Explain trade-offs in plain language, including technical and production costs.
7. Prefer small prototypes that test the riskiest mechanics.
8. Keep the solo-developer constraint central to recommendations.
9. Avoid overengineering and premature abstractions.
10. Keep the design document updated when decisions are made.
11. Do not implement the entire game at once.
12. Do not generate large amounts of code before the engine, core mechanics, and first milestone are agreed.

### First task

Do not immediately start coding.

First:
1. Summarize the game vision in your own words.
2. Identify the three biggest design or technical risks for a solo developer.
3. Compare the three ship-building approaches in Section 4.4.
4. Give a recommendation for a first playable prototype.
5. Ask the developer the single most important unresolved design question.

After the developer answers, continue iteratively. Help create a more detailed Game Design Document (GDD), technical design, milestone plan, and implementation tasks.

### Documentation expectations

As the project evolves, maintain clear documents such as:
- `GDD.md` — Core game vision, mechanics, systems, and design decisions.
- `TECHNICAL_DESIGN.md` — Engine architecture, systems, data models, and technical decisions.
- `ROADMAP.md` — Milestones, priorities, and scope.
- `DECISIONS.md` — Important decisions, alternatives, and reasons.

Do not create all of these documents blindly at the beginning. Propose them when useful and keep them consistent with the developer's decisions.

---

## 13. Current One-Sentence Pitch

**Skybound is a flying-ship survival sandbox where players build and live aboard a customizable airborne home, explore mysterious floating islands, gather resources, farm, craft, and upgrade their vessel to reach new parts of a vast sky world.**

---

*End of initial design brief. This document is intentionally a starting point: the next step is to validate the core fantasy and ship-building approach before expanding the design or beginning full production.*
