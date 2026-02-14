cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME papi3d)

project(${PROJECT_NAME}
    VERSION 1.0.0
    DESCRIPTION "Papi3D - A 3D raycasting game engine"
    LANGUAGES CXX
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for IDE support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set default build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

# ============================================================================
# Find SDL2 - Cross-platform approach
# ============================================================================

# Try to find SDL2 using CMake's find_package first
find_package(SDL2 QUIET)

if(NOT SDL2_FOUND)
    # Fallback: Try pkg-config (Linux/macOS with Homebrew)
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(SDL2 QUIET sdl2)
    endif()

    # If still not found, try common installation paths
    if(NOT SDL2_FOUND)
        if(APPLE)
            # Check Homebrew paths (both Intel and Apple Silicon)
            set(SDL2_SEARCH_PATHS
                /opt/homebrew/opt/sdl2    # Apple Silicon
                /usr/local/opt/sdl2       # Intel Mac
            )
        elseif(WIN32)
            # Check common Windows paths
            set(SDL2_SEARCH_PATHS
                "C:/SDL2"
                "C:/Program Files/SDL2"
                "C:/msys64/mingw64"
                "$ENV{PROGRAMFILES}/SDL2"
            )
        else()
            # Linux paths
            set(SDL2_SEARCH_PATHS
                /usr
                /usr/local
            )
        endif()

        find_path(SDL2_INCLUDE_DIR SDL2/SDL.h
            PATHS ${SDL2_SEARCH_PATHS}
            PATH_SUFFIXES include
        )

        find_library(SDL2_LIBRARY
            NAMES SDL2
            PATHS ${SDL2_SEARCH_PATHS}
            PATH_SUFFIXES lib
        )

        if(SDL2_INCLUDE_DIR AND SDL2_LIBRARY)
            set(SDL2_FOUND TRUE)
            set(SDL2_INCLUDE_DIRS ${SDL2_INCLUDE_DIR})
            set(SDL2_LIBRARIES ${SDL2_LIBRARY})
        endif()
    endif()
endif()

if(NOT SDL2_FOUND)
    message(FATAL_ERROR "SDL2 not found! Please install SDL2:\n"
        "  macOS:   brew install sdl2\n"
        "  Ubuntu:  sudo apt-get install libsdl2-dev\n"
        "  Fedora:  sudo dnf install SDL2-devel\n"
        "  Windows: Download from https://www.libsdl.org/download-2.0.php")
endif()

# ============================================================================
# Source files
# ============================================================================

set(SOURCES
    src/main.cpp
    src/graphics.cpp
    src/map.cpp
    src/constants.cpp
    src/player.cpp
)

set(HEADERS
    src/graphics.h
    src/map.h
    src/pfloat.h
    src/player.h
    src/types.h
    src/constants.h
)

# ============================================================================
# Create executable target
# ============================================================================

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Set target properties
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "${PROJECT_NAME}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# ============================================================================
# Include directories
# ============================================================================

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${SDL2_INCLUDE_DIRS}
)

# ============================================================================
# Compiler-specific flags
# ============================================================================

if(MSVC)
    # MSVC (Visual Studio) compiler flags
    target_compile_options(${PROJECT_NAME} PRIVATE
        /W4                     # Warning level 4
        $<$<CONFIG:Release>:/O2>  # Optimize for speed
        /permissive-            # Standards conformance
    )
else()
    # GCC/Clang compiler flags
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall                   # Enable all warnings
        -Wextra                # Enable extra warnings
        -pedantic              # Strict ISO C++ compliance
        $<$<CONFIG:Release>:-O2>  # Optimize for speed in Release
        $<$<CONFIG:Debug>:-g>     # Debug symbols in Debug
    )
endif()

# ============================================================================
# Link libraries
# ============================================================================

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${SDL2_LIBRARIES}
)

# Platform-specific linking
if(WIN32)
    # Windows-specific libraries
    target_link_libraries(${PROJECT_NAME} PRIVATE
        mingw32     # Required for SDL2 on MinGW
    )
    # Set subsystem to windows for GUI app (removes console window)
    # Comment out if you want to keep the console for debugging
    if(MSVC)
        set_target_properties(${PROJECT_NAME} PROPERTIES
            WIN32_EXECUTABLE TRUE
        )
    endif()
elseif(APPLE)
    # macOS-specific frameworks
    target_link_libraries(${PROJECT_NAME} PRIVATE
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreAudio"
        "-framework CoreVideo"
        "-framework ForceFeedback"
    )
elseif(UNIX)
    # Linux-specific libraries
    target_link_libraries(${PROJECT_NAME} PRIVATE
        m           # Math library
        pthread     # POSIX threads
        dl          # Dynamic loading
    )
endif()

# ============================================================================
# Installation rules (optional)
# ============================================================================

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

# ============================================================================
# Print configuration summary
# ============================================================================

message(STATUS "")
message(STATUS "========================================")
message(STATUS "  ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "========================================")
message(STATUS "Build type:       ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard:     C++${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler:         ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "System:           ${CMAKE_SYSTEM_NAME}")
message(STATUS "SDL2 found:       ${SDL2_FOUND}")
message(STATUS "SDL2 include:     ${SDL2_INCLUDE_DIRS}")
message(STATUS "SDL2 libraries:   ${SDL2_LIBRARIES}")
message(STATUS "Install prefix:   ${CMAKE_INSTALL_PREFIX}")
message(STATUS "========================================")
message(STATUS "")
