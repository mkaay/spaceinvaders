#include "menu.h"
#include "helpers.h"

#include <SDL_ttf.h>

void updateScore(Game *g)
{
    char score_fmt[50];
    sprintf(score_fmt, "SCORE: %d", g->score);
    TTF_Font *font = TTF_OpenFont("font.ttf", 20);
    SDL_Color white = {255, 255, 255, 0};
    SDL_Surface *text = TTF_RenderText_Solid(font, score_fmt, white);
    SDL_Rect rect = {WIDTH/3, 14, 200, 20};
    SDL_FillRect(g->screen, &rect, SDL_MapRGB(g->screen->format, 0, 0, 0));
    SDL_BlitSurface(text, NULL, g->screen, &rect);
}

void updateLives(Game *g)
{
    char lives_fmt[50];
    sprintf(lives_fmt, "LIVES: %d", g->player.lives);
    TTF_Font *font = TTF_OpenFont("font.ttf", 20);
    SDL_Color white = {255, 255, 255, 0};
    SDL_Surface *text = TTF_RenderText_Solid(font, lives_fmt, white);
    SDL_Rect rect = {BORDER, 14, 100, 20};
    SDL_FillRect(g->screen, &rect, SDL_MapRGB(g->screen->format, 0, 0, 0));
    SDL_BlitSurface(text, NULL, g->screen, &rect);
    
    rect.x = 100 + 2*BORDER;
    rect.y = 4;
    rect.w = PLAYER_WIDTH;
    rect.h = PLAYER_HEIGHT;
    SDL_FillRect(g->screen, &rect, SDL_MapRGB(g->screen->format, 0, 0, 0));
    if (g->player.lives > 0) {
        drawSprite(g->screen, &rect, "player.bmp");
    }
}