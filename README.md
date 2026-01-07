# 🎮 C++ Game Development Project

This repository is a beginner-friendly C++ game development project created to learn and practice:
- Modern C++ (C++17+)
- Game engine structure
- Build systems using CMake
- Clean project organization
- Core game development concepts

The project is intentionally kept simple and scalable so it can grow over time.

---

## 📁 Project Structure

```
GameDev/
├── build/              # Build output (auto-generated, ignored by Git)
├── src/                # C++ source files (.cpp)
├── include/            # C++ header files (.h)
├── assets/             # Game assets (textures, audio, fonts)
├── shaders/            # GPU shader files
├── resources/          # Game data / config files
├── libs/               # External libraries
├── config/             # Project configuration
├── docs/               # Detailed documentation
├── CMakeLists.txt      # CMake build configuration
├── .gitignore          # Git ignore rules
└── README.md           # Project overview
```

---

## 🛠 Requirements

- C++ compiler (GCC / Clang)
- CMake (3.10 or higher)
- Git (optional, recommended)

Check installed versions:
```bash
g++ --version
cmake --version
```

---

## ⚙️ Build Instructions (Linux)

```bash
git clone <repository-url>
cd GameDev
mkdir build
cd build
cmake ..
make
```

---

## ▶️ Run the Program

```bash
./GameDev
```

(Executable name depends on `CMakeLists.txt`.)

---

## 🎯 Project Goals

* Learn game engine basics
* Understand memory and performance
* Practice clean C++ architecture
* Build a foundation for graphics (OpenGL / SDL / SFML)

---

## 🚀 Future Plans

* [ ] Game loop implementation
* [ ] Input handling
* [ ] Entity system
* [ ] Rendering system
* [ ] Asset loading
* [ ] Cross-platform support

---

## 📚 Documentation

Detailed explanations and design notes can be found in the [`docs/`](docs/) folder, including:

- **architecture.md** - Project structure and engine design
- **build.md** - Detailed CMake usage and platform notes
- **game_loop.md** - Game loop fundamentals and timestep management
- **roadmap.md** - Features, milestones, and learning goals

The documentation serves as learning notes and helps understand design decisions as the project evolves.

---

## 📜 License

This project is intended for educational purposes.
Add a license if you plan to distribute or open-source it.
# GameDev

Trying fun new things

# F1 Simulation Engine

A C++ based Formula 1 racing simulation engine.

## Build
```bash
mkdir build && cd build
cmake ..
make
./f1sim
```

