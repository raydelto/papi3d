cmake_minimum_required(VERSION 3.10)

project(fps)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2")

# Find SDL2
if(APPLE)
    # macOS with Homebrew
    set(SDL2_INCLUDE_DIRS /opt/homebrew/opt/sdl2/include)
    set(SDL2_LIBRARIES /opt/homebrew/opt/sdl2/lib/libSDL2.dylib)
    include_directories(/usr/local/include)
elseif(WIN32)
    # Windows with MSYS2
    set(SDL2_INCLUDE_DIRS C:/msys64/mingw64/include)
    set(SDL2_LIBRARIES C:/msys64/mingw64/lib/libSDL2.a gdi32)
else()
    # Linux
    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS})
endif()

# Include directories
include_directories(${SDL2_INCLUDE_DIRS})

# Source files
set(SOURCES
    src/main.cpp
    src/graphics.cpp
    src/map.cpp
    src/constants.cpp
    src/player.cpp
)

# Create executable
add_executable(main ${SOURCES})

# Link SDL2
target_link_libraries(main ${SDL2_LIBRARIES})

# Set output directory
set_target_properties(main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Copy executable to bin directory for in-source builds
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    set_target_properties(main PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    )
endif()
