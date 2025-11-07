# Astonia Client - Cross-Platform Build Guide

This document describes how to build the Astonia client on Linux, macOS, and Windows.

## Overview

The Astonia client has been converted from Windows-only to cross-platform using:
- SDL2 for graphics, input, and windowing
- SDL2_net for networking
- SDL2_mixer for sound
- CMake for cross-platform building

## Platform-Specific Instructions

### Linux

#### Dependencies

On Ubuntu/Debian:
```bash
sudo apt-get update
sudo apt-get install build-essential cmake libsdl2-dev libsdl2-net-dev libsdl2-mixer-dev zlib1g-dev libpng-dev
```

On Fedora/RHEL:
```bash
sudo dnf install gcc cmake SDL2-devel SDL2_net-devel SDL2_mixer-devel zlib-devel libpng-devel
```

On Arch Linux:
```bash
sudo pacman -S base-devel cmake sdl2 sdl2_net sdl2_mixer zlib libpng
```

#### Building

Use the provided build script:
```bash
./build-linux.sh
```

Or manually:
```bash
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
```

### macOS

#### Dependencies

Install Homebrew from https://brew.sh, then:
```bash
brew install cmake sdl2 sdl2_net sdl2_mixer libpng zlib
```

#### Building

Use the provided build script:
```bash
./build-macos.sh
```

Or manually:
```bash
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(sysctl -n hw.ncpu)
```

### Windows

#### Dependencies

1. Install MSYS2 from https://www.msys2.org/
2. Open MSYS2 MinGW 64-bit terminal
3. Install dependencies:
```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_net mingw-w64-x86_64-SDL2_mixer mingw-w64-x86_64-zlib mingw-w64-x86_64-libpng
```

#### Building

```bash
mkdir -p build
cd build
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..
mingw32-make
```

## Running the Client

After building, the executable will be in `bin/moac` (or `bin/moac.exe` on Windows).

Example usage:
```bash
./bin/moac -u YourUsername -p YourPassword -d server.astonia.com -w 1920 -h 1080
```

### Command Line Options

- `-u <username>` - Your character name
- `-p <password>` - Your password
- `-d <server>` - Server address (hostname or IP)
- `-w <width>` - Window width in pixels
- `-h <height>` - Window height in pixels
- `-o <options>` - Game options bitfield (see usage)
- `-m <threads>` - Number of background threads (default: 4)
- `-c <cachesize>` - Texture cache size (default: 8000)
- `-k <fps>` - Frames per second limit

## Platform-Specific Changes

### What was converted:

1. **Memory Management**: Windows HeapAlloc/HeapFree → cross-platform malloc/free
2. **Networking**: Windows Sockets (winsock2) → SDL_net + BSD sockets
3. **Dynamic Libraries**: Windows LoadLibrary → dlopen on Unix
4. **File Paths**: Windows backslashes → platform-aware path separators
5. **Message Boxes**: Windows MessageBox → SDL_ShowSimpleMessageBox
6. **Configuration Storage**: Windows %APPDATA% → SDL_GetPrefPath
7. **DPI Awareness**: Windows-specific → conditional compilation
8. **Exception Handling**: Windows SEH → conditional (Windows-only)

### What is platform-specific:

1. **Shared Memory** (modder/sharedmem.c): Windows-only feature, disabled on other platforms
2. **Crash Reporting** (dwarfstack): Windows-only, conditionally compiled
3. **Mods**: Support .dll (Windows), .dylib (macOS), .so (Linux)

## Troubleshooting

### Linux

- **Missing SDL2**: Make sure you have both runtime and development packages
- **OpenGL errors**: Install mesa-utils and proper graphics drivers

### macOS

- **Permission denied**: Run `chmod +x build-macos.sh`
- **Code signing**: You may need to right-click and "Open" the first time

### Windows

- **CMake not found**: Make sure MSYS2 bin directory is in PATH
- **DLL errors**: Run from MSYS2 MinGW terminal, not Windows cmd

## Development

For development builds with debug symbols:
```bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
```

The DEVELOPER define enables additional debug output and features.

## License

See license.txt for licensing information.
