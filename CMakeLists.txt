cmake_minimum_required(VERSION 3.10)
project(AstoniaClient C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add custom CMake module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# Build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-pointer-sign -Wno-char-subscripts")
set(CMAKE_C_FLAGS_DEBUG "-g -O0 -DDEVELOPER")
set(CMAKE_C_FLAGS_RELEASE "-O3 -fomit-frame-pointer")

# Find required packages - try pkg-config first, fallback to find_package
find_package(PkgConfig)

if(PKG_CONFIG_FOUND)
    pkg_check_modules(SDL2 sdl2)
    pkg_check_modules(SDL2_NET SDL2_net)
    pkg_check_modules(SDL2_MIXER SDL2_mixer)
    pkg_check_modules(ZLIB zlib)
    pkg_check_modules(PNG libpng)
    pkg_check_modules(LIBZIP libzip)
endif()

# Fallback to find_package if pkg-config didn't find the packages
if(NOT SDL2_FOUND)
    find_package(SDL2 REQUIRED)
endif()
if(NOT ZLIB_FOUND)
    find_package(ZLIB REQUIRED)
endif()
if(NOT PNG_FOUND)
    find_package(PNG REQUIRED)
endif()
if(NOT LIBZIP_FOUND)
    find_package(libzip QUIET)
    if(NOT libzip_FOUND)
        # Try pkg-config with different case
        if(PKG_CONFIG_FOUND)
            pkg_search_module(LIBZIP libzip zip)
        endif()
    else()
        set(LIBZIP_LIBRARIES libzip::zip)
    endif()
endif()

# Platform detection
if(WIN32)
    add_definitions(-DPLATFORM_WINDOWS)
    # Windows-specific libraries
    set(PLATFORM_LIBS wsock32 ws2_32)
elseif(APPLE)
    add_definitions(-DPLATFORM_MACOS)
    # macOS-specific settings
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mmacosx-version-min=10.12")
    set(PLATFORM_LIBS)
elseif(UNIX)
    add_definitions(-DPLATFORM_LINUX)
    # Linux-specific libraries
    set(PLATFORM_LIBS m dl)
endif()

# Include directories
include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_NET_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIRS}
    ${PNG_INCLUDE_DIRS}
    ${LIBZIP_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src
)

# Source files
set(CLIENT_SOURCES
    src/platform.c
    src/gui/gui.c
    src/client/client.c
    src/client/skill.c
    src/game/dd.c
    src/game/font.c
    src/game/main.c
    src/game/sprite.c
    src/game/game.c
    src/game/version.c
    src/modder/modder.c
    src/sdl/sound.c
    src/sdl/sdl.c
    src/helper/helper.c
    src/gui/dots.c
    src/gui/display.c
    src/gui/teleport.c
    src/gui/color.c
    src/gui/cmd.c
    src/gui/questlog.c
    src/gui/context.c
    src/gui/hover.c
    src/gui/minimap.c
    src/modder/sharedmem.c
)

# Amod sources
set(AMOD_SOURCES
    src/amod/amod.c
    src/amod/ranks.c
    src/amod/quests.c
    src/amod/strings.c
    src/amod/skills.c
)

# Main executable
add_executable(moac ${CLIENT_SOURCES})

# Link libraries
target_link_libraries(moac
    ${SDL2_LIBRARIES}
    ${SDL2_NET_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
    ${ZLIB_LIBRARIES}
    ${PNG_LIBRARIES}
    ${LIBZIP_LIBRARIES}
    ${PLATFORM_LIBS}
)

# Add library directories
target_link_directories(moac PRIVATE
    ${SDL2_LIBRARY_DIRS}
    ${SDL2_NET_LIBRARY_DIRS}
    ${SDL2_MIXER_LIBRARY_DIRS}
    ${ZLIB_LIBRARY_DIRS}
    ${PNG_LIBRARY_DIRS}
    ${LIBZIP_LIBRARY_DIRS}
)

# Set output directory
set_target_properties(moac PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)

# Amod library (optional modding support)
# Skipping amod build on all platforms due to complex linking requirements
# The amod sources have multiple definition errors that would require significant refactoring
message(STATUS "Skipping amod build (optional modding support - requires linking fixes)")

# Helper tools
add_executable(convert src/helper/convert.c)
if(PNG_LIBRARY_DIRS)
    target_link_directories(convert PRIVATE ${PNG_LIBRARY_DIRS})
endif()
if(LIBZIP_LIBRARY_DIRS)
    target_link_directories(convert PRIVATE ${LIBZIP_LIBRARY_DIRS})
endif()
target_link_libraries(convert ${PNG_LIBRARIES} ${ZLIB_LIBRARIES} ${LIBZIP_LIBRARIES})
set_target_properties(convert PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    COMPILE_DEFINITIONS "STANDALONE"
)

add_executable(anicopy src/helper/anicopy.c)
set_target_properties(anicopy PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)

# Install targets
install(TARGETS moac DESTINATION bin)
install(DIRECTORY res DESTINATION share/astonia)
install(DIRECTORY bin/data DESTINATION share/astonia OPTIONAL)

# Print build configuration
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "Platform: ${CMAKE_SYSTEM_NAME}")
