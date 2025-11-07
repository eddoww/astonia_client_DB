/*
 * Part of Astonia Client (c) Daniel Brockhaus. Please read license.txt.
 *
 * Platform Compatibility Layer Implementation
 */

#include "platform.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

/* Platform heap handle - used by Windows, dummy for Unix */
HANDLE platform_heap = NULL;

char* platform_get_pref_path(const char* org, const char* app) {
#ifdef PLATFORM_WINDOWS
    static char path[MAX_PATH];
    if (SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path) == S_OK) {
        return path;
    }
    return ".";
#else
    /* Use SDL's cross-platform preference path */
    char* sdl_path = SDL_GetPrefPath(org, app);
    if (sdl_path) {
        static char path[MAX_PATH];
        strncpy(path, sdl_path, MAX_PATH - 1);
        path[MAX_PATH - 1] = '\0';
        SDL_free(sdl_path);

        /* Remove trailing path separator */
        size_t len = strlen(path);
        if (len > 0 && (path[len-1] == '/' || path[len-1] == '\\')) {
            path[len-1] = '\0';
        }
        return path;
    }
    return ".";
#endif
}

void platform_show_message_box(const char* title, const char* message, int error) {
#ifdef PLATFORM_WINDOWS
    MessageBox(NULL, message, title, MB_APPLMODAL | MB_OK |
               (error ? MB_ICONSTOP : MB_ICONEXCLAMATION));
#else
    SDL_ShowSimpleMessageBox(
        error ? SDL_MESSAGEBOX_ERROR : SDL_MESSAGEBOX_WARNING,
        title,
        message,
        NULL
    );
#endif
}

int platform_net_init(void) {
#ifdef PLATFORM_WINDOWS
    WSADATA wsadata;
    if (WSAStartup(0x0002, &wsadata)) {
        return -1;
    }
#endif
    /* Initialize SDL_net */
    if (SDLNet_Init() < 0) {
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        return -1;
    }
    return 0;
}

void platform_net_cleanup(void) {
    SDLNet_Quit();
#ifdef PLATFORM_WINDOWS
    WSACleanup();
#endif
}

void platform_set_dpi_aware(void) {
#ifdef PLATFORM_WINDOWS
    SetProcessDPIAware();
#endif
    /* No equivalent needed on Unix-like systems */
}

void platform_get_memory_status(platform_memory_status* status) {
#ifdef PLATFORM_WINDOWS
    MEMORYSTATUS ms;
    bzero(&ms, sizeof(ms));
    ms.dwLength = sizeof(ms);
    GlobalMemoryStatus(&ms);
    status->total_phys = ms.dwTotalPhys;
    status->avail_phys = ms.dwAvailPhys;
#elif defined(__linux__)
    /* Read from /proc/meminfo */
    FILE* fp = fopen("/proc/meminfo", "r");
    if (fp) {
        char line[256];
        uint64_t memtotal = 0, memavailable = 0;
        while (fgets(line, sizeof(line), fp)) {
            if (sscanf(line, "MemTotal: %" SCNu64 " kB", &memtotal) == 1) {
                status->total_phys = memtotal * 1024;
            } else if (sscanf(line, "MemAvailable: %" SCNu64 " kB", &memavailable) == 1) {
                status->avail_phys = memavailable * 1024;
            }
        }
        fclose(fp);
    }
#elif defined(__APPLE__)
    /* Use sysctl on macOS */
    #include <sys/sysctl.h>
    int mib[2];
    int64_t physical_memory;
    size_t length;

    mib[0] = CTL_HW;
    mib[1] = HW_MEMSIZE;
    length = sizeof(int64_t);
    sysctl(mib, 2, &physical_memory, &length, NULL, 0);
    status->total_phys = physical_memory;
    status->avail_phys = physical_memory / 2; /* Approximation */
#else
    status->total_phys = 0;
    status->avail_phys = 0;
#endif
}
