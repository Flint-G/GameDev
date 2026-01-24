# Project Itinerary

## Completed Milestones

### Phase 1: Engine Foundation
- Basic OpenGL Rendering Setup (Window, Context, GLAD).
- Texture Loading system.
- Primitive Rendering (Quads for Sprite).

### Phase 2: Gameplay Basics
- Car Physics (Acceleration, Braking, Steering).
- Scrolling Background (Infinite Road illusion).
- Obstacles (Rocks, Trees) and Collision Detection.
- Road Lines generation.

### Phase 3: Multiplayer Implementation
- **Architecture**: Server-Client model using ENet.
- **Dedicated Server**: Support for `--server` mode.
- **Sync**: Position synchronization with interpolation/smoothing.
- **Lobby**: Unique spawn points (Lanes 0-3).
- **Polish**: Player Names rendered above cars.

## Future Roadmap

### Phase 4: Gameplay Loop (Next Steps)
- [ ] **Race Logic**: Start/Finish lines, Lap counting.
- [ ] **Win Condition**: First to complete N laps.
- [ ] **UI Layer**: Speedometer, Lap timer, Leaderboard.

### Phase 5: Polish & Optimizations
- [ ] **Audio**: Engine sounds, crash effects, background music.
- [ ] **Particles**: Smoke when drifting/braking.
- [ ] **Menu**: Main Menu screen (Host/Join).
