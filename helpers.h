#ifndef _helpers_h
#define _helpers_h

#include <SDL.h>
#include "types.h"

long ms_time();
void drawSprite(SDL_Surface *screen, SDL_Rect *rect, char *image);
int collides(SDL_Rect a, SDL_Rect b);
void freeShotList(Shot *s);

#endif