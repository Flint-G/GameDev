# Minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Project name and language
project(GameDev VERSION 1.0 LANGUAGES CXX)

# Use C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =========================
# Include directories
# =========================
include_directories(
    include
)

# =========================
# Source files
# =========================
# Automatically include all .cpp files in src/ and subdirectories
file(GLOB_RECURSE SOURCES "src/*.cpp")

# =========================
# Create executable
# =========================
add_executable(GameDev ${SOURCES})

# =========================
# Optional: compiler warnings
# =========================
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(GameDev PRIVATE -Wall -Wextra -Wpedantic)
endif()

# =========================
# Output directory
# =========================
set_target_properties(GameDev PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
