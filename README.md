# OpenGL Games Collection

A collection of 3D and 2D games built from scratch using OpenGL and C++. This project showcases game development fundamentals including 3D rendering, physics simulation, camera systems, and real-time graphics programming.

![C++](https://img.shields.io/badge/C++-11-blue.svg)
![OpenGL](https://img.shields.io/badge/OpenGL-3.3+-green.svg)
![Platform](https://img.shields.io/badge/Platform-Windows%20|%20Linux%20|%20macOS-lightgrey.svg)

---

## :video_game: Games

### Drone Delivery
A 3D drone simulation game where you pilot a delivery drone through an open environment.

**Features:**
- Third-person camera system with smooth following
- Physics-based flight mechanics with realistic drone movement
- Package delivery gameplay - pick up and deliver packages to drop zones
- Procedurally placed obstacles including trees and platforms
- Minimap HUD for navigation
- Score tracking system

### Tank Wars
A 2D artillery game inspired by classics like Scorched Earth and Worms.

**Features:**
- Destructible terrain with realistic landslide physics
- Turn-based local multiplayer
- Projectile physics with trajectory prediction
- Health system with visual life bars
- Procedurally generated terrain using sine wave combinations

---

## :hammer_and_wrench: Technical Highlights

- Custom 3D transformation and rendering pipeline
- Shader-based rendering with GLSL
- Multiple camera systems (third-person, minimap view)
- Real-time collision detection
- Physics simulation for projectiles and terrain
- Text rendering for UI elements
- Cross-platform support (Windows, Linux, macOS)

---

## :clipboard: Prerequisites

### Compiler
| Platform | Requirement |
|----------|-------------|
| Windows | Visual Studio 2019+ with "Desktop development with C++" |
| Linux | GCC 5+ (`g++`) |
| macOS | Clang 4+ (Xcode or `brew install llvm`) |

### Graphics
- OpenGL 3.3+ capable GPU
- Updated graphics drivers ([NVIDIA](https://www.nvidia.com/Download/index.aspx) / [AMD](https://www.amd.com/en/support))

### Dependencies

**Windows:** All dependencies included in `gfx-framework-master/deps/` folder.

**Linux:**
```bash
cd gfx-framework-master
./tools/deps-ubuntu.sh   # Debian/Ubuntu
./tools/deps-fedora.sh   # Fedora
./tools/deps-arch.sh     # Arch
```

**macOS:**
```bash
brew install cmake
cd gfx-framework-master && ./tools/deps-macos.sh
```

---

## :gear: Building

```bash
cd gfx-framework-master

# Create and enter build directory
mkdir build && cd build

# Configure
cmake ..

# Build
cmake --build .
```

**IDE Support:**
- **Visual Studio:** Open `gfx-framework-master` folder → auto-detects CMake → Build with `Ctrl+Shift+B`
- **CLion:** Open `gfx-framework-master` folder → auto-configures → Build and run

---

## :rocket: Running

```bash
# From the build directory
./bin/Debug/OpenGLGames
```

### Switching Games

Edit `gfx-framework-master/src/main.cpp` to select which game to run:

```cpp
// Drone Delivery (3D)
World* world = new m1::DroneDelivery();

// Tank Wars (2D)
World* world = new m1::TankWars();
```

---

## :joystick: Controls

### Drone Delivery
| Key | Action |
|-----|--------|
| `W/A/S/D` | Move drone horizontally |
| `Q/E` | Rotate drone |
| `Space` | Ascend |
| `Ctrl` | Descend |
| `F` | Drop package |
| `Mouse` | Camera control |

### Tank Wars
| Key | Action |
|-----|--------|
| `A/D` | Move tank |
| `W/S` | Adjust cannon angle |
| `Space` | Fire projectile |

---

## :file_folder: Project Structure

```
opengl-games/
└── gfx-framework-master/
    ├── src/
    │   ├── components/          # Core rendering components
    │   ├── core/                # Engine (window, GPU, managers)
    │   ├── lab_m1/
    │   │   ├── DroneDelivery/   # 3D drone simulation game
    │   │   └── TankWars/        # 2D artillery game
    │   └── utils/               # Helper utilities
    ├── assets/
    │   ├── models/              # 3D models (.obj, .fbx)
    │   ├── shaders/             # GLSL vertex/fragment shaders
    │   └── textures/            # Texture images
    └── deps/                    # Third-party dependencies
```

---

## :books: Dependencies

| Library | Purpose |
|---------|---------|
| [GLFW](https://www.glfw.org/) | Window & input management |
| [GLEW](http://glew.sourceforge.net/) | OpenGL extension loading |
| [GLM](https://github.com/g-truc/glm) | Mathematics (vectors, matrices) |
| [Assimp](https://github.com/assimp/assimp) | 3D model importing |
| [FreeType](https://freetype.org/) | Font rendering |
| [spdlog](https://github.com/gabime/spdlog) | Logging |

---

## :page_facing_up: License

This project is available under the MIT License. See [gfx-framework-master/LICENSE.md](gfx-framework-master/LICENSE.md) for details.

External libraries are available under their respective licenses. See [gfx-framework-master/LEGAL.txt](gfx-framework-master/LEGAL.txt) for full details.
