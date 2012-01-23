#include <sys/time.h>

#include "helpers.h"

// Zeit in Millisekunden
long ms_time()
{
    struct timeval detail_time;
    gettimeofday(&detail_time, NULL);
    return detail_time.tv_usec / 1000 + detail_time.tv_sec * 1000;
}


// BMP Zeichen
void drawSprite(SDL_Surface *screen, SDL_Rect *rect, char *image)
{
    SDL_Surface *tmp;
    SDL_Surface *pic;
    
    // BMP Laden
    if ((tmp = SDL_LoadBMP(image)) == NULL) {
        printf("Sprite konnte nicht geladen werden: %s\n", SDL_GetError());
        exit(1);
    }
    
    // Hintergrundfarbe transparent mappen
    SDL_SetColorKey(tmp, SDL_SRCCOLORKEY, SDL_MapRGB(tmp->format, 0, 0, 0));
    // Anwenden
    pic = SDL_DisplayFormat(tmp);
    SDL_FreeSurface(tmp);
    
    // Zeichen
    SDL_BlitSurface(pic, NULL, screen, rect);
}


// prüft ob 2 SDL_Rects sich überscheiden
int collides(SDL_Rect a, SDL_Rect b)
{
    if ((a.x + a.w >= b.x) && (a.x <= b.x + b.w) && (a.y + a.h >= b.y) && (a.y <= b.y + b.h)) {
        return true;
    } else {
        return false;
    }
}

void freeShotList(Shot *s)
{
    if (s->next != NULL) {
        freeShotList(s->next);
    }
    free(s);
}