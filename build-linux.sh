#!/bin/bash
#
# Build script for Linux
#

set -e

echo "========================================="
echo "Astonia Client - Linux Build Script"
echo "========================================="

# Check for required dependencies
echo "Checking dependencies..."

check_command() {
    if ! command -v $1 &> /dev/null; then
        echo "ERROR: $1 is not installed"
        echo "Please install it using your package manager:"
        echo "  Ubuntu/Debian: sudo apt-get install $2"
        echo "  Fedora/RHEL: sudo dnf install $2"
        echo "  Arch: sudo pacman -S $2"
        exit 1
    fi
}

check_pkg() {
    if ! pkg-config --exists $1; then
        echo "ERROR: $1 development package is not installed"
        echo "Please install it using your package manager:"
        echo "  Ubuntu/Debian: sudo apt-get install $2"
        echo "  Fedora/RHEL: sudo dnf install $2"
        echo "  Arch: sudo pacman -S $2"
        exit 1
    fi
}

check_command cmake cmake
check_command make make
check_command gcc gcc
check_pkg sdl2 libsdl2-dev
check_pkg SDL2_net libsdl2-net-dev
check_pkg SDL2_mixer libsdl2-mixer-dev
check_pkg zlib zlib1g-dev
check_pkg libpng libpng-dev

echo "All dependencies found!"
echo ""

# Create build directory
echo "Creating build directory..."
mkdir -p build
cd build

# Configure with CMake
echo "Configuring build with CMake..."
cmake -DCMAKE_BUILD_TYPE=Release ..

# Build
echo "Building..."
make -j$(nproc)

echo ""
echo "========================================="
echo "Build complete!"
echo "========================================="
echo ""
echo "The executable is located at: bin/moac"
echo ""
echo "To run the client:"
echo "  cd .."
echo "  ./bin/moac -u <username> -p <password> -d <server> -w 1920 -h 1080"
echo ""
