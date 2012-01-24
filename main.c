#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <SDL.h>
#include <SDL_ttf.h>

#include "game.h"
#include "helpers.h"
#include "menu.h"
#include "types.h"
#include "highscore.h"

int main(int argc, char *argv[])
{
    Game *g = (Game*) malloc(sizeof(Game));
    SDL_Event event;
    Uint8 *keystates;
    Uint8 quit = false;
    long lastplayerupdate = ms_time();
    long lastenemyupdate = ms_time();
    long lastshotupdate = ms_time();
    long lastufoupdate = ms_time();
    
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
    
    TTF_Init();
    
    
    // Game initialisieren
    initGame(g);
    startNewLevel(g);
    updateScore(g);
    updateLives(g);
    showHighscore(g);
    updateBlocks(g);
    
    // Nächster Grafikzustand
    SDL_Flip(g->screen);
    
    // Loop
    while (!quit) {
        // SDL Events abfragen
        SDL_PollEvent(&event);
        
        // Tastenstatus laden
        keystates = SDL_GetKeyState(NULL);
        
        // Escape gedrückt -> beenden
        // TODO: Menü aufrufen statt beenden
        if (keystates[SDLK_ESCAPE]) {
            saveHighscore(g->score);
            quit = true;
        }
        
        // Nur wenn entweder Links oder Rechts, nicht beide zur selben Zeit
        if (keystates[SDLK_LEFT] != keystates[SDLK_RIGHT] && lastplayerupdate >= 100) {
            lastplayerupdate = ms_time();
            
            // Links
            if (keystates[SDLK_LEFT]) {
                movePlayer(g, Left);
            }
            // Rechts
            if (keystates[SDLK_RIGHT]) {
                movePlayer(g, Right);
            }
        }
        
        if (keystates[SDLK_SPACE]) {
            shoot(g);
        }
        
        // UFO
        if (ms_time() - lastufoupdate >= UFO_UPDATE) {
            lastufoupdate = ms_time();
            ufo(g);
        }
        
        // Schüsse aktualisieren
        if (ms_time() - lastshotupdate >= SHOT_UPDATE) {
            lastshotupdate = ms_time();
            checkCollision(g);
            updateShots(g);
            movePlayer(g, None);
        }
        
        // Alienposition aktualisieren?
        // Exponentialfunktion, die Level und Alienanzahl berücksichtigt
        if (ms_time() - lastenemyupdate >= ENEMY_UPDATE_BASE * pow(0.95, g->level * 3 + (ENEMY_COUNT - g->enemyContainer.aliveCount) / 4)) {
            lastenemyupdate = ms_time();
            moveEnemys(g);
            alienShot(g);
        }
        
        usleep(20000); // begrenzt CPU Last
        // Nächster Grafikzustand
        SDL_Flip(g->screen);
    }
    
    SDL_Quit();
    return 0;
}
