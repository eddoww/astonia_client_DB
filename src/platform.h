/*
 * Part of Astonia Client (c) Daniel Brockhaus. Please read license.txt.
 *
 * Platform Compatibility Layer
 *
 * Provides cross-platform abstractions for Windows, Linux, and macOS
 */

#ifndef PLATFORM_H
#define PLATFORM_H

/* Feature test macros for Linux - must come before any system includes */
#if defined(__linux__) && !defined(_GNU_SOURCE)
    #define _GNU_SOURCE
#endif

#include <stdint.h>

/* Platform detection */
#if defined(_WIN32) || defined(_WIN64)
    #ifndef PLATFORM_WINDOWS
        #define PLATFORM_WINDOWS
    #endif
#elif defined(__APPLE__)
    #ifndef PLATFORM_MACOS
        #define PLATFORM_MACOS
    #endif
#elif defined(__linux__)
    #ifndef PLATFORM_LINUX
        #define PLATFORM_LINUX
    #endif
#else
    #ifndef PLATFORM_UNIX
        #define PLATFORM_UNIX
    #endif
#endif

/* Export/Import macros */
#ifdef PLATFORM_WINDOWS
    #define EXPORT __declspec(dllexport)
    #define IMPORT __declspec(dllimport)
#else
    #define EXPORT __attribute__((visibility("default")))
    #define IMPORT
#endif

/* Windows-specific includes and compatibility */
#ifdef PLATFORM_WINDOWS
    #include <winsock2.h>
    #include <windows.h>
    #include <psapi.h>
    #include <shlobj.h>
    #include <direct.h>

    #define PATH_SEPARATOR '\\'
    #define O_BINARY _O_BINARY

    #ifndef bzero
    #define bzero(ptr,size) memset(ptr,0,size)
    #endif

#else
    /* Unix-like platforms */
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <sys/time.h>
    #include <dlfcn.h>
    #include <errno.h>
    #include <sys/socket.h>
    #include <sys/select.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h>

    #define PATH_SEPARATOR '/'
    #define O_BINARY 0

    /* Windows types compatibility */
    typedef void* HANDLE;
    typedef void* HMODULE;
    typedef uint32_t DWORD;
    typedef int BOOL;

    #define INVALID_HANDLE_VALUE ((HANDLE)(long)-1)
    #define TRUE 1
    #define FALSE 0

    /* Socket compatibility */
    typedef int SOCKET;
    #define INVALID_SOCKET (-1)
    #define SOCKET_ERROR (-1)
    #define closesocket(s) close(s)
    #define ioctlsocket ioctl
    #define WSAEWOULDBLOCK EWOULDBLOCK
    #define WSAGetLastError() errno

    /* ioctl compatibility */
    #include <sys/ioctl.h>
    #ifndef FIONBIO
        #define FIONBIO 0x5421
    #endif
#endif

/* PATH_MAX compatibility */
#ifndef MAX_PATH
    #ifdef PATH_MAX
        #define MAX_PATH PATH_MAX
    #else
        #define MAX_PATH 260
    #endif
#endif

/* Memory allocation wrappers */
#ifdef PLATFORM_WINDOWS
    /* Windows heap functions */
    extern HANDLE platform_heap;
#else
    /* Unix heap handle (dummy for compatibility) */
    extern HANDLE platform_heap;
#endif

#ifdef PLATFORM_WINDOWS

    static inline void* platform_heap_alloc(size_t size) {
        extern HANDLE platform_heap;
        return HeapAlloc(platform_heap, HEAP_ZERO_MEMORY, size);
    }

    static inline void* platform_heap_realloc(void* ptr, size_t size) {
        extern HANDLE platform_heap;
        return HeapReAlloc(platform_heap, HEAP_ZERO_MEMORY, ptr, size);
    }

    static inline void platform_heap_free(void* ptr) {
        extern HANDLE platform_heap;
        HeapFree(platform_heap, 0, ptr);
    }

    static inline size_t platform_heap_size(void* ptr) {
        extern HANDLE platform_heap;
        return HeapSize(platform_heap, 0, ptr);
    }

    static inline HANDLE platform_heap_create(void) {
        return HeapCreate(0, 0, 0);
    }

    static inline int platform_heap_validate(HANDLE heap) {
        return HeapValidate(heap, 0, NULL);
    }
#else
    /* Unix: use standard malloc/free */
    #include <stdlib.h>
    #ifdef __linux__
        #include <malloc.h>
    #endif
    #ifdef __APPLE__
        #include <malloc/malloc.h>
    #endif

    static inline void* platform_heap_alloc(size_t size) {
        void* ptr = calloc(1, size);
        return ptr;
    }

    static inline void* platform_heap_realloc(void* ptr, size_t size) {
        void* new_ptr = realloc(ptr, size);
        return new_ptr;
    }

    static inline void platform_heap_free(void* ptr) {
        free(ptr);
    }

    static inline size_t platform_heap_size(void* ptr) {
        #ifdef __GLIBC__
            return malloc_usable_size(ptr);
        #elif defined(__APPLE__)
            return malloc_size(ptr);
        #else
            return 0; /* Not available on all platforms */
        #endif
    }

    static inline HANDLE platform_heap_create(void) {
        return (HANDLE)1; /* Dummy handle for Unix */
    }

    static inline int platform_heap_validate(HANDLE heap) {
        return 1; /* Always valid on Unix */
    }
#endif

/* Dynamic library loading */
#ifdef PLATFORM_WINDOWS
    static inline HMODULE platform_load_library(const char* filename) {
        return LoadLibrary(filename);
    }

    static inline void* platform_get_proc_address(HMODULE module, const char* proc_name) {
        return (void*)GetProcAddress(module, proc_name);
    }

    static inline void platform_free_library(HMODULE module) {
        FreeLibrary(module);
    }
#else
    static inline HMODULE platform_load_library(const char* filename) {
        return dlopen(filename, RTLD_LAZY);
    }

    static inline void* platform_get_proc_address(HMODULE module, const char* proc_name) {
        return dlsym(module, proc_name);
    }

    static inline void platform_free_library(HMODULE module) {
        dlclose(module);
    }
#endif

/* Process information */
#ifdef PLATFORM_WINDOWS
    static inline uint32_t platform_get_process_id(void) {
        return GetProcessId(GetCurrentProcess());
    }

    static inline void* platform_get_module_handle(void) {
        return (void*)GetModuleHandle(NULL);
    }
#else
    static inline uint32_t platform_get_process_id(void) {
        return (uint32_t)getpid();
    }

    static inline void* platform_get_module_handle(void) {
        return NULL; /* Not directly equivalent on Unix */
    }
#endif

/* Directory functions */
#ifdef PLATFORM_WINDOWS
    static inline int platform_mkdir(const char* path) {
        return _mkdir(path);
    }
#else
    static inline int platform_mkdir(const char* path) {
        return mkdir(path, 0755);
    }
#endif

/* Get application data directory */
char* platform_get_pref_path(const char* org, const char* app);

/* Message box */
void platform_show_message_box(const char* title, const char* message, int error);

/* Network initialization */
int platform_net_init(void);
void platform_net_cleanup(void);

/* DPI awareness (Windows only) */
void platform_set_dpi_aware(void);

/* Memory status */
typedef struct {
    uint64_t total_phys;
    uint64_t avail_phys;
} platform_memory_status;

void platform_get_memory_status(platform_memory_status* status);

#endif /* PLATFORM_H */
