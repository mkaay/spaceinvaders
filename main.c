#include <stdio.h>
#include <SDL.h>

#include "types.h"

#define WIDTH 800
#define HEIGHT 600
#define BORDER 10

#define PLAYER_WIDTH 42
#define PLAYER_HEIGHT 30
#define PLAYER_MOVE_INTERVAL 5
#define PLAYER_Y_POS HEIGHT-PLAYER_HEIGHT-5

void drawSprite(SDL_Surface *screen, SDL_Rect *rect, char *image)
{
    SDL_Surface *tmp;
    SDL_Surface *pic;
    
    if ((tmp = SDL_LoadBMP(image)) == NULL) {
        printf("Sprite konnte nicht geladen werden: %s\n", SDL_GetError());
        exit(1);
    }
    
    SDL_SetColorKey(tmp, SDL_SRCCOLORKEY, SDL_MapRGB(tmp->format, 0, 0, 0));
    pic = SDL_DisplayFormat(tmp);
    SDL_FreeSurface(tmp);
    
    SDL_BlitSurface(pic, NULL, screen, rect);
}

void movePlayer(Game *g, Direction direction)
{
    if (direction != NoMove) {
        SDL_FillRect(g->screen, &g->player->rect, SDL_MapRGB(g->screen->format, 0, 0, 0));
        g->player->rect.x += (direction == Left)?-PLAYER_MOVE_INTERVAL:PLAYER_MOVE_INTERVAL;
        if (g->player->rect.x < BORDER) {
            g->player->rect.x = BORDER;
        }
        if (g->player->rect.x > WIDTH-BORDER-PLAYER_WIDTH) {
            g->player->rect.x = WIDTH-BORDER-PLAYER_WIDTH;
        }
    }
    drawSprite(g->screen, &g->player->rect, "player.bmp");
}

void initGame(Game *g)
{
    g->player = (Player*) malloc(sizeof(Player));
    
    g->player->rect.x = BORDER;
    g->player->rect.y = PLAYER_Y_POS;
    g->player->rect.w = PLAYER_WIDTH;
    g->player->rect.h = PLAYER_HEIGHT;
    
    movePlayer(g, NoMove);
    
    SDL_Flip(g->screen);
}

int main(int argc, char *argv[])
{
    Game *g = (Game*) malloc(sizeof(Game));
    SDL_Event event;
    Uint8 *keystates;
    Uint8 quit = false;
    
    // SDL initialisieren
    if (SDL_Init(SDL_INIT_VIDEO) == -1) {
        printf("Kann Video nicht initialisieren: %s\n", SDL_GetError());
        exit(1);
    }
    
    atexit(SDL_Quit);
    
    g->screen = SDL_SetVideoMode(WIDTH, HEIGHT, 16, SDL_HWSURFACE);
    
    if (g->screen == NULL) {
        printf("Kann Video-Modus nicht festlegen: %s\n", SDL_GetError());
        exit(1);
    }
    
    initGame(g);
    
    while (!quit) {
        SDL_PollEvent(&event);
        
        // Tastenstatus laden
        keystates = SDL_GetKeyState(NULL);
        
        if (keystates[SDLK_ESCAPE]) {
            quit = true;
        }
        
        // Nur wenn entweder Links oder Rechts, nicht beide zur selben Zeit
        if (keystates[SDLK_LEFT] != keystates[SDLK_RIGHT]) {
            // Links
            if (keystates[SDLK_LEFT]) {
                movePlayer(g, Left);
            }
            // Rechts
            if (keystates[SDLK_RIGHT]) {
                movePlayer(g, Right);
            }
        }
        SDL_Flip(g->screen);
    }
    
    SDL_Quit();
    return 0;
}