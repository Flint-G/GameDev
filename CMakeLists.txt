cmake_minimum_required(VERSION 3.20)
project(F1SimulationEngine VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =========================
# Sources
# =========================

set(CORE_SOURCES
    src/engine/Engine.cpp
    src/engine/Car.cpp
    src/engine/Collision.cpp
)

set(RENDER_SOURCES
    src/render/Window.cpp
    src/render/Renderer.cpp
    src/render/TextRenderer.cpp
    shaders/Shader.cpp 
)

set(APP_SOURCES
    src/main.cpp
)

# GLAD source (C file)
set(GLAD_SOURCES
    libs/glad/glad.c
)

# Texture loader
set(TEXTURE_SOURCES
    resources/vendors/stb_image/stb_image.cpp
    src/texture/Texture.cpp
)

# Combine all sources
set(ALL_SOURCES
    ${CORE_SOURCES}
    ${RENDER_SOURCES}
    ${APP_SOURCES}
    ${GLAD_SOURCES}
    ${TEXTURE_SOURCES}
)

# =========================
# Create executable
# =========================
add_executable(f1sim ${ALL_SOURCES})

# =========================
# Include directories
# =========================
target_include_directories(f1sim PRIVATE
    include
    src
    libs/glad            # glad.h & khrplatform.h
    shaders              # Shader.h
    resources/vendors/stb_image
)

# =========================
# Link system libraries (Linux)
# =========================
# =========================
# System Dependencies (FetchContent)
# =========================
include(FetchContent)

FetchContent_Declare(
  enet
  GIT_REPOSITORY https://github.com/zpl-c/enet.git
  GIT_TAG        master
)
FetchContent_MakeAvailable(enet)

# =========================
# Link system libraries (Linux)
# =========================
find_package(OpenGL REQUIRED)
target_link_libraries(f1sim PRIVATE OpenGL::GL glfw enet_static)

# =========================
# Compiler warnings
# =========================
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # Enable warnings for all your C++ code
    target_compile_options(f1sim PRIVATE -Wall -Wextra -Wpedantic)

    # Suppress pedantic warnings ONLY for glad.c
    set_source_files_properties(libs/glad/glad.c PROPERTIES COMPILE_FLAGS -Wno-pedantic)
endif()

# =========================
# Output directory
# =========================
set_target_properties(f1sim PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
