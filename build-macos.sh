#!/bin/bash
#
# Build script for macOS
#

set -e

echo "========================================="
echo "Astonia Client - macOS Build Script"
echo "========================================="

# Check for Homebrew
if ! command -v brew &> /dev/null; then
    echo "ERROR: Homebrew is not installed"
    echo "Please install Homebrew from https://brew.sh"
    exit 1
fi

# Check for required dependencies
echo "Checking dependencies..."

check_formula() {
    if ! brew list $1 &> /dev/null; then
        echo "Installing $1..."
        brew install $1
    fi
}

check_formula cmake
check_formula sdl2
check_formula sdl2_net
check_formula sdl2_mixer
check_formula libpng
check_formula zlib

echo "All dependencies installed!"
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
make -j$(sysctl -n hw.ncpu)

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
