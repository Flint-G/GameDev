# Car Simulation

A C++ multiplayer F1 racing simulation using OpenGL and ENet.

## Features
- **Multiplayer**: Server-Client architecture with dedicated server support (`--server`).
- **Gameplay**: Infinite scrolling road, obstacles, and simple car physics.
- **Graphics**: Modern OpenGL rendering, sprite handling, and text rendering.
- **Polish**: Unique spawn points, movement smoothing, and player name tags.

## Documentation
Detailed documentation is available in the [`docs/`](docs/) directory:

- **[Usage Guide](docs/USAGE.md)**: How to build and run the game.
- **[Architecture](docs/ARCHITECTURE.md)**: Overview of the engine, networking, and rendering systems.
- **[Itinerary](docs/ITINERARY.md)**: Project roadmap and completed milestones.

## Quick Start (Linux)
```bash
mkdir build && cd build
cmake ..
make

# Run Server
./bin/f1sim --server

# Run Client (in new terminal)
./bin/f1sim
```

## License
MIT License
