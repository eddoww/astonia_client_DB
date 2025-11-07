# Locate SDL2 library
# This module defines
# SDL2_LIBRARY, the name of the library to link against
# SDL2_FOUND, if false, do not try to link to SDL2
# SDL2_INCLUDE_DIR, where to find SDL.h
#
# This module responds to the the flag:
# SDL2_BUILDING_LIBRARY
# If this is defined, then no SDL2main will be linked in because
# only applications need main().
# Otherwise, it is assumed you are building an application and this
# module will attempt to locate and set the the proper link flags
# as part of the returned SDL2_LIBRARY variable.

# Try pkg-config first
find_package(PkgConfig)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(PC_SDL2 QUIET sdl2)
endif()

find_path(SDL2_INCLUDE_DIR SDL.h
    HINTS
    ${PC_SDL2_INCLUDEDIR}
    ${PC_SDL2_INCLUDE_DIRS}
    PATH_SUFFIXES SDL2
    PATHS
    /usr/local/include
    /usr/include
    /opt/local/include
    /sw/include
)

find_library(SDL2_LIBRARY
    NAMES SDL2
    HINTS
    ${PC_SDL2_LIBDIR}
    ${PC_SDL2_LIBRARY_DIRS}
    PATHS
    /usr/local/lib
    /usr/lib
    /opt/local/lib
    /sw/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SDL2 DEFAULT_MSG SDL2_LIBRARY SDL2_INCLUDE_DIR)

if(SDL2_FOUND)
    set(SDL2_LIBRARIES ${SDL2_LIBRARY})
    set(SDL2_INCLUDE_DIRS ${SDL2_INCLUDE_DIR})
endif()

mark_as_advanced(SDL2_INCLUDE_DIR SDL2_LIBRARY)
