cmake_minimum_required(VERSION 3.20)
project(F1SimulationEngine VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =========================
# Sources
# =========================
set(CORE_SOURCES
    src/engine/Engine.cpp
    src/engine/Car.cpp        # <--- added
)

set(RENDER_SOURCES
    src/render/Window.cpp
    src/render/Renderer.cpp
    shaders/Shader.cpp 
)

set(APP_SOURCES
    src/main.cpp
)

# GLAD source (must compile)
set(GLAD_SOURCES
    libs/glad/glad.c
)

# Combine all sources
set(ALL_SOURCES
    ${CORE_SOURCES}
    ${RENDER_SOURCES}
    ${APP_SOURCES}
    ${GLAD_SOURCES}
)

# =========================
# Create executable
# =========================
add_executable(f1sim ${ALL_SOURCES})

# =========================
# Include directories (folders, NOT files)
target_include_directories(f1sim PRIVATE
    include
    src
    libs/glad          # folder containing glad.h & khrplatform.h
    shaders            # folder containing Shader.h
)

# =========================
# Link system libraries (Linux)
find_package(OpenGL REQUIRED)
target_link_libraries(f1sim PRIVATE OpenGL::GL glfw)

# =========================
# Compiler warnings
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(f1sim PRIVATE -Wall -Wextra -Wpedantic)
endif()

# =========================
# Output directory
set_target_properties(f1sim PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
