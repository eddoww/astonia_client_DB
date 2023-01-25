/*
 * Part of Astonia Mod (c) ModderMcModFace.
 *
 * Says Hello
 *
 */

#include <stdint.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_RENDERER_IMPLEMENTATION

#include "volumewidget.h"
#include "nuklear.h"
#include "nuklear_sdl_renderer.h"
#include "../amod.h"

extern int note();
    /* GUI */
    struct nk_context *ctx;
    struct nk_colorf bg;
    int flags = 0;
    float font_scale = 1;

void volumeWidgetInit(){

    SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");

    /* GUI */
    ctx = nk_sdl_init(mod_window, mod_renderer);
    /* Load Fonts: if none of these are loaded a default font will be used  */
    /* Load Cursor: if you uncomment cursor loading please hide the cursor */
    {
        struct nk_font_atlas *atlas;
        struct nk_font_config config = nk_font_config(0);
        struct nk_font *font;

        /* set up the font atlas and add desired font; note that font sizes are
         * multiplied by font_scale to produce better results at higher DPIs */
        nk_sdl_font_stash_begin(&atlas);
        font = nk_font_atlas_add_default(atlas, 13 * font_scale, &config);
        /*font = nk_font_atlas_add_from_file(atlas, "../../../extra_font/DroidSans.ttf", 14 * font_scale, &config);*/
        /*font = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Roboto-Regular.ttf", 16 * font_scale, &config);*/
        /*font = nk_font_atlas_add_from_file(atlas, "../../../extra_font/kenvector_future_thin.ttf", 13 * font_scale, &config);*/
        /*font = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyClean.ttf", 12 * font_scale, &config);*/
        /*font = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyTiny.ttf", 10 * font_scale, &config);*/
        /*font = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Cousine-Regular.ttf", 13 * font_scale, &config);*/
        nk_sdl_font_stash_end();

        /* this hack makes the font appear to be scaled down to the desired
         * size and is only necessary when font_scale > 1 */
        font->handle.height /= font_scale;
        /*nk_style_load_all_cursors(ctx, atlas->cursors);*/
        nk_style_set_font(ctx, &font->handle);
    }
}

void volumeWidgetBegin(){
        /* Input */
        SDL_Event evt;
        nk_input_begin(ctx);
        while (SDL_PollEvent(&evt)) {
            if (evt.type == SDL_QUIT) return;
            nk_sdl_handle_event(&evt);
        }
        nk_input_end(ctx);

            /* GUI */
        if (nk_begin(ctx, "Volume Adjuster", nk_rect(50, 50, 230, 150),
            NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
            NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
        {


            nk_layout_row_begin(ctx, NK_STATIC, 30, 2);
            {
                nk_layout_row_push(ctx, 50);
                nk_label(ctx, "Volume:", NK_TEXT_LEFT);
                nk_layout_row_push(ctx, 110);
                nk_slider_int(ctx, 0, &sound_volume, 100, 1);
            }
            nk_layout_row_end(ctx);

        nk_layout_row_static(ctx, 30, 80, 1);
        if (nk_button_label(ctx, "Save")) {
            // event handling
            note("storing the value of the volume slider into volume variable %d", sound_volume);
        }
        }
}

void volumeWidgetRender(){
        SDL_SetRenderDrawColor(mod_renderer, bg.r * 255, bg.g * 255, bg.b * 255, bg.a * 255);
        SDL_RenderClear(mod_renderer);

        nk_sdl_render(NK_ANTI_ALIASING_ON);

        SDL_RenderPresent(mod_renderer);
}
void volumeWidgetEnd(){
    nk_end(ctx);
}
void volumeWidgetCleanup(){
    nk_sdl_shutdown();
}