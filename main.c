#include <stdio.h>

#include <SDL.h>
#include <SDL_gfxPrimitives.h>

#define WIDTH 800
#define HEIGHT 600

int main(int argc, char *argv[])
{
    SDL_Surface *screen;
    SDL_Surface *tmp;
    SDL_Surface *pic;
    SDL_Event event;
    SDL_Rect rect;
    Uint8 *keystates;
    int quit = 0;
    int movedist = 0;
    
    // SDL initialisieren
    if (SDL_Init(SDL_INIT_VIDEO) == -1) {
        printf("Kann Video nicht initialisieren: %s\n", SDL_GetError());
        exit(1);
    }
    
    atexit(SDL_Quit);
    
    screen = SDL_SetVideoMode(WIDTH, HEIGHT, 16, SDL_HWSURFACE);
    
    if (screen == NULL) {
        printf("Kann Video-Modus nicht festlegen: %s\n", SDL_GetError());
        exit(1);
    }
    
    if ((tmp = SDL_LoadBMP("player.bmp")) == NULL) {
        printf("Bild konnte nicht geladen werden: %s\n", SDL_GetError());
        exit(1);
    }
    
    SDL_SetColorKey(tmp, SDL_SRCCOLORKEY, SDL_MapRGB(tmp->format, 0, 0, 0));
    pic = SDL_DisplayFormat(tmp);
    SDL_FreeSurface (tmp);
    
    rect.x = 10;
    rect.y = 550;
    rect.w = 21;
    rect.h = 15;
    
    SDL_BlitSurface(pic, NULL, screen, &rect);
    SDL_UpdateRect(screen, rect.x, rect.y, rect.w, rect.h);
    
    while (!quit) {
        SDL_PollEvent(&event);
        
        // Tastenstatus laden
        keystates = SDL_GetKeyState(NULL);
        
        if (keystates[SDLK_ESCAPE]) {
            quit = 1;
        }
        
        // Nur wenn entweder Links oder Rechts, nicht beide zur selben Zeit
        if (keystates[SDLK_LEFT] != keystates[SDLK_RIGHT]) {
            SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, 0, 0, 0));
            
            // Links
            if (keystates[SDLK_LEFT]) {
                rect.x -= 5;
            }
            // Rechts
            if (keystates[SDLK_RIGHT]) {
                rect.x += 5;
            }
            
            rect.x += movedist;
            
            SDL_BlitSurface(pic, NULL, screen, &rect);
            SDL_Flip(screen);
        }
    }
    
    SDL_Quit();
    return 0;
}