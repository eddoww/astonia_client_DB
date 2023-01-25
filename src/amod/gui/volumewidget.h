/*
 * Part of Astonia Mod (c) ModderMcModFace.
 *
 * Says Hello
 *
 */

#ifndef VOLUME_WIDGET_H_
#define VOLUME_WIDGET_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

extern SDL_Window *mod_window;
extern SDL_Renderer *mod_renderer;

void volumeWidgetInit(void);
void volumeWidgetBegin(void);
void volumeWidgetEnd(void);
void volumeWidgetRender();
void volumeWidgetCleanup(void);



#endif /* VOLUME_WIDGET_H_ */