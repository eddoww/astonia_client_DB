/*
 * Part of Astonia Mod (c) ModderMcModFace.
 *
 * Says Hello
 *
 */

#include <stdint.h>
#include <stdio.h>

//#include "strings.c"
//#include "ranks.c"
//#include "skills.c"

#include "amod.h"
#include "gui/volumewidget.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

SDL_Window *mod_window;
SDL_Renderer *mod_renderer;
int initial;
void sdl_init_mod(){
    // Create window
    mod_window = SDL_CreateWindow("Yet another SDL window! Loaded through DLL magic!",
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  800, 600,
                                  SDL_WINDOW_SHOWN|SDL_WINDOW_ALLOW_HIGHDPI);
    if(!mod_window)
    {
        fprintf(stderr, "Window could not be created!\n"
                        "SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
        // Create renderer
        mod_renderer = SDL_CreateRenderer(mod_window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
        if(!mod_renderer)
        {
            fprintf(stderr, "Renderer could not be created!\n"
                            "SDL_Error: %s\n", SDL_GetError());
        }
        volumeWidgetInit();
    }
}

__declspec(dllexport) void amod_init(void) {
    initial++;
    note("A Mod by ModderMcModFace in init function");
    sdl_init_mod();
}

__declspec(dllexport) void amod_gamestart(void) {
    note("A Mod by ModderMcModFace loaded (%d).",initial);
    addline("Mod loaded");
}
__declspec(dllexport) void amod_frame(void){
    volumeWidgetBegin();
    volumeWidgetEnd();
    volumeWidgetRender();
    //addline("I am run every single frame!");
    //note("I am run every single frame!");
}
__declspec(dllexport) void amod_tick(void){
    //note("I am run every tick");
}
__declspec(dllexport) void amod_exit(void) {
    //note("I am run at exit!");
    volumeWidgetCleanup();
    SDL_DestroyRenderer(mod_renderer);
    SDL_DestroyWindow(mod_window);
    SDL_Quit();
}
