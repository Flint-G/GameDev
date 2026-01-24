# Usage Guide

## Prerequisites
- **CMake** (3.20+)
- **Compiler** (GCC/Clang)
- **Linux Dependency**: `libgl1-mesa-dev`, `libxrandr-dev`, `libxi-dev`, `libxcursor-dev` (Usually standard on desktop linux).

## Building
```bash
mkdir build
cd build
cmake ..
make
```

## Running

### 1. Dedicated Server
Run the server first to host the game.
```bash
./bin/f1sim --server
```
*Output: server listening on port 1234.*

### 2. Client(s)
Run one or more clients.
```bash
./bin/f1sim
```
*Behavior*:
- The client attempts to connect to localhost (`127.0.0.1`).
- Upon connection, you are assigned a **Lane** and a **Player ID**.
- Use **WASD** to drive:
    - `W`: Accelerate
    - `S`: Brake/Reverse
    - `A/D`: Steer
    - `SPACE`: Emergency Brake

### Troubleshooting
- **No Connection**: Ensure server is running. If server is on another machine, update IP in `Engine.cpp` (currently hardcoded to localhost).
- **Compilation Errors**: Ensure submodules/fetch content (ENet) downloaded correctly. Check internet connection during CMake config.
