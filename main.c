#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>
#include <SDL.h>

#include "types.h"

long ms_time()
{
    struct timeval detail_time;
    gettimeofday(&detail_time, NULL);
    return detail_time.tv_usec / 1000 + detail_time.tv_sec * 1000;
}

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
    if (direction != None) {
        SDL_FillRect(g->screen, &g->player.rect, SDL_MapRGB(g->screen->format, 0, 0, 0));
        g->player.rect.x += (direction == Left)?-PLAYER_MOVE_INTERVAL:PLAYER_MOVE_INTERVAL;
        if (g->player.rect.x < BORDER) {
            g->player.rect.x = BORDER;
        }
        if (g->player.rect.x > WIDTH-BORDER-PLAYER_WIDTH) {
            g->player.rect.x = WIDTH-BORDER-PLAYER_WIDTH;
        }
    }
    drawSprite(g->screen, &g->player.rect, "player.bmp");
}

// TODO: bisher wird nur die box geprüft, aber nicht welche reihe tatsächlich noch lebt
void moveEnemys(Game *g)
{
    SDL_Rect rect;
    int dx = 0;
    int dy = 0;
    
    if (g->enemyContainer.moveDirection == Right) {
        if (g->enemyContainer.posx + (ENEMYFIELD_WIDTH * FIELD_WIDTH) + (ENEMYFIELD_WIDTH * FIELD_MARGIN * 2) >= WIDTH - BORDER) {
            g->enemyContainer.moveDirection = Down;
            dy = 30;
        } else {
            dx = 10;
        }
    } else if (g->enemyContainer.moveDirection == Left) {
        if (g->enemyContainer.posx <= BORDER) {
            g->enemyContainer.moveDirection = Down;
            dy = 30;
        } else {
            dx = -10;
        }
    } else if (g->enemyContainer.moveDirection == Down) {
        if (g->enemyContainer.posy + (ENEMYFIELD_HEIGHT * FIELD_HEIGHT) + (ENEMYFIELD_HEIGHT * FIELD_MARGIN * 2) < PLAYER_Y_POS) {
            g->enemyContainer.moveDirection = (g->enemyContainer.posx <= BORDER) ? Right : Left;
            dx = (g->enemyContainer.posx <= BORDER) ? 10 : -10;
        } else {
            // Game over
        }
    }
    
    for (int x = 0; x < ENEMYFIELD_WIDTH; x++) {
        for (int y = 0; y < ENEMYFIELD_HEIGHT; y++) {
            rect.x = g->enemyContainer.posx + (x * FIELD_WIDTH) + (x * FIELD_MARGIN * 2) - FIELD_MARGIN;
            rect.y = g->enemyContainer.posy + (y * FIELD_HEIGHT) + (y * FIELD_MARGIN * 2) - FIELD_MARGIN;
            rect.w = FIELD_WIDTH;
            rect.h = FIELD_HEIGHT;
            
            SDL_FillRect(g->screen, &rect, SDL_MapRGB(g->screen->format, 0, 0, 0));
        }
    }
    
    g->enemyContainer.posx += dx;
    g->enemyContainer.posy += dy;
    
    
    for (int x = 0; x < ENEMYFIELD_WIDTH; x++) {
        for (int y = 0; y < ENEMYFIELD_HEIGHT; y++) {
            rect.x = g->enemyContainer.posx + (x * FIELD_WIDTH) + (x * FIELD_MARGIN * 2) - FIELD_MARGIN;
            rect.y = g->enemyContainer.posy + (y * FIELD_HEIGHT) + (y * FIELD_MARGIN * 2) - FIELD_MARGIN;
            rect.w = FIELD_WIDTH;
            rect.h = FIELD_HEIGHT;
            
            if (g->enemyContainer.enemys[x][y].type == 1) {
                rect.x += (FIELD_WIDTH - ALIEN1_WIDTH) / 2;
                rect.y += (FIELD_HEIGHT - ALIEN1_HEIGHT) / 2;
                rect.w = ALIEN1_WIDTH;
                rect.h = ALIEN1_HEIGHT;
                
                if (g->enemyContainer.enemys[x][y].animationState == 0) {
                    drawSprite(g->screen, &rect, "alien1_1.bmp");
                } else {
                    drawSprite(g->screen, &rect, "alien1_2.bmp");
                }
            } else if (g->enemyContainer.enemys[x][y].type == 2) {
                rect.x += (FIELD_WIDTH - ALIEN2_WIDTH) / 2;
                rect.y += (FIELD_HEIGHT - ALIEN2_HEIGHT) / 2;
                rect.w = ALIEN2_WIDTH;
                rect.h = ALIEN2_HEIGHT;
                
                if (g->enemyContainer.enemys[x][y].animationState == 0) {
                    drawSprite(g->screen, &rect, "alien2_1.bmp");
                } else {
                    drawSprite(g->screen, &rect, "alien2_2.bmp");
                }
            } else if (g->enemyContainer.enemys[x][y].type == 3) {
                rect.x += (FIELD_WIDTH - ALIEN3_WIDTH) / 2;
                rect.y += (FIELD_HEIGHT - ALIEN3_HEIGHT) / 2;
                rect.w = ALIEN3_WIDTH;
                rect.h = ALIEN3_HEIGHT;
                
                if (g->enemyContainer.enemys[x][y].animationState == 0) {
                    drawSprite(g->screen, &rect, "alien3_1.bmp");
                } else {
                    drawSprite(g->screen, &rect, "alien3_2.bmp");
                }
            }
            
            g->enemyContainer.enemys[x][y].animationState = (g->enemyContainer.enemys[x][y].animationState == 0) ? 1 : 0;
        }
    }
}

void initGame(Game *g)
{
    g->level = 10;
    
    g->player.rect.x = BORDER;
    g->player.rect.y = PLAYER_Y_POS;
    g->player.rect.w = PLAYER_WIDTH;
    g->player.rect.h = PLAYER_HEIGHT;
    
    movePlayer(g, None);
    
    for (int x = 0; x < ENEMYFIELD_WIDTH; x++) {
        g->enemyContainer.enemys[x][0].dead = false;
        g->enemyContainer.enemys[x][0].type = 1;
        g->enemyContainer.enemys[x][0].animationState = 0;
    }
    
    for (int y = 1; y < 3; y++) {
        for (int x = 0; x < ENEMYFIELD_WIDTH; x++) {
            g->enemyContainer.enemys[x][y].dead = false;
            g->enemyContainer.enemys[x][y].type = 2;
            g->enemyContainer.enemys[x][y].animationState = 0;
        }
    }
    
    for (int y = 3; y < ENEMYFIELD_HEIGHT; y++) {
        for (int x = 0; x < ENEMYFIELD_WIDTH; x++) {
            g->enemyContainer.enemys[x][y].dead = false;
            g->enemyContainer.enemys[x][y].type = 3;
            g->enemyContainer.enemys[x][y].animationState = 0;
        }
    }
    
    g->enemyContainer.posx = BORDER;
    g->enemyContainer.posy = BORDER;
    g->enemyContainer.aliveCount = ENEMY_COUNT;
    g->enemyContainer.moveDirection = Right;
    
    moveEnemys(g);
    
    SDL_Flip(g->screen);
}

int main(int argc, char *argv[])
{
    Game *g = (Game*) malloc(sizeof(Game));
    SDL_Event event;
    Uint8 *keystates;
    Uint8 quit = false;
    long lastupdate = ms_time();
    
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
            usleep(20000);
        }
        
        if (ms_time() - lastupdate >= 1000 * pow(0.95, g->level * 2 + (ENEMY_COUNT - g->enemyContainer.aliveCount) / 10)) {
            lastupdate = ms_time();
            moveEnemys(g);
        }
        SDL_Flip(g->screen);
    }
    
    SDL_Quit();
    return 0;
}