# Architecture Overview

## Core Systems

### 1. Engine (`Engine.cpp`)
The central hub of the application.
- **Responsibilities**:
    - Manages the game loop (Update/Render).
    - Handles Input (GLFW).
    - Updates Physics (`Car`).
    - Manages Network (`Client`).
    - Manages Rendering (`Renderer`, `TextRenderer`).

### 2. Networking (`NetworkManager`, `Server`, `Client`)
Built on top of **ENet** (Reliable UDP).
- **Protocol**: Defined in `Packet.h`. Uses packed structs for efficiency.
- **Server**:
    - Authoritative for Client IDs and Spawn Points.
    - Relays `PacketPlayerUpdate` messages to all other clients.
- **Client**:
    - Sends inputs/position to Server.
    - Interpolates remote player positions for smooth movement.

### 3. Rendering (`Renderer`, `TextRenderer`)
Uses Modern OpenGL (Shader-based).
- **Renderer**: Handles sprites (Car, Rocks, Trees) using a batched or instanced approach (conceptually, currently simple draw calls).
- **TextRenderer**: Uses `stb_truetype` to bake font bitmaps and render text quads.

### 4. Physics (`Collision`, `Car`)
- **Car**: Simple rigid body physics with velocity, drag, and steering angle.
- **Collision**: AABB (Axis-Aligned Bounding Box) or Circle-based collision detection.

## Directory Structure
- `src/`: Source code.
- `include/`: Header files.
- `assets/`: Game assets (Images, Fonts).
- `shaders/`: GLSL shader code.
- `docs/`: This documentation.
