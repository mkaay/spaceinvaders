#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>
#include <SDL.h>

#include "types.h"

//Marius ist der Hammer

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


// Spieler bewegen
void movePlayer(Game *g, Direction direction)
{
    if (direction != None) {
        // Alte Position überschreiben
        SDL_FillRect(g->screen, &g->player.rect, SDL_MapRGB(g->screen->format, 0, 0, 0));
        // Neue Position Berechnen
        g->player.rect.x += (direction == Left)?-PLAYER_MOVE_INTERVAL:PLAYER_MOVE_INTERVAL;
        
        // Prüfen ob außerhalb des Spielfeldes
        // Links
        if (g->player.rect.x < BORDER) {
            g->player.rect.x = BORDER;
        }
        // Rechts
        if (g->player.rect.x > WIDTH-BORDER-PLAYER_WIDTH) {
            g->player.rect.x = WIDTH-BORDER-PLAYER_WIDTH;
        }
    }
    
    // Zeichnen
    drawSprite(g->screen, &g->player.rect, "player.bmp");
}


// Alien Box bewegen
void moveEnemys(Game *g)
{
    // Umriss und Position des Aliens
    SDL_Rect rect;
    // Positionsdelta
    int dx = 0;
    int dy = 0;
    
    // Aktuelle Bewegungsrichtung abfragen und Positionsdelta berechnen
    if (g->enemyContainer.moveDirection == Right) {
        // Kollision mit Rand abfragen
        if (g->enemyContainer.posx + (ENEMYFIELD_WIDTH * FIELD_WIDTH) + (ENEMYFIELD_WIDTH * FIELD_MARGIN * 2) >= WIDTH - BORDER) {
            // Richtung ändern
            g->enemyContainer.moveDirection = Down;
            // Eine Spalte nach unten
            dy = ENEMY_MOVE_INTERVAL_VERTICAL;
        } else {
            // Position nach rechts verschieben
            dx = ENEMY_MOVE_INTERVAL_HORIZONTAL;
        }
    } else if (g->enemyContainer.moveDirection == Left) {
        // Kollision mit Rand abfragen
        if (g->enemyContainer.posx <= BORDER) {
            // Richtung ändern
            g->enemyContainer.moveDirection = Down;
            // Eine Spalte nach unten
            dy = ENEMY_MOVE_INTERVAL_VERTICAL;
        } else {
            // Position nach links verschieben
            dx = -ENEMY_MOVE_INTERVAL_HORIZONTAL;
        }
    } else if (g->enemyContainer.moveDirection == Down) {
        // Unteren Rand getroffen noch nicht getroffen?
        if (g->enemyContainer.posy + (ENEMYFIELD_HEIGHT * FIELD_HEIGHT) + (ENEMYFIELD_HEIGHT * FIELD_MARGIN * 2) < PLAYER_Y_POS) {
            g->enemyContainer.moveDirection = (g->enemyContainer.posx <= BORDER) ? Right : Left;
            dx = (g->enemyContainer.posx <= BORDER) ? ENEMY_MOVE_INTERVAL_HORIZONTAL : -ENEMY_MOVE_INTERVAL_HORIZONTAL;
        } else {
            // Unten angekommen?
            // TODO: Schon gestorbene Aliens berücksichtigen
            // TODO: Game over
        }
    }
    
    // Alte Position "befreien"
    for (int x = 0; x < ENEMYFIELD_WIDTH; x++) {
        for (int y = 0; y < ENEMYFIELD_HEIGHT; y++) {
            // Bereich berechnen
            rect.x = g->enemyContainer.posx + (x * FIELD_WIDTH) + (x * FIELD_MARGIN * 2) - FIELD_MARGIN;
            rect.y = g->enemyContainer.posy + (y * FIELD_HEIGHT) + (y * FIELD_MARGIN * 2) - FIELD_MARGIN;
            rect.w = FIELD_WIDTH;
            rect.h = FIELD_HEIGHT;
            
            // Bereich überschreiben
            SDL_FillRect(g->screen, &rect, SDL_MapRGB(g->screen->format, 0, 0, 0));
        }
    }
    
    // Box verschieben
    g->enemyContainer.posx += dx;
    g->enemyContainer.posy += dy;
    
    // Neu zeichnen
    for (int x = 0; x < ENEMYFIELD_WIDTH; x++) {
        for (int y = 0; y < ENEMYFIELD_HEIGHT; y++) {
            // Bereich berechnen
            rect.x = g->enemyContainer.posx + (x * FIELD_WIDTH) + (x * FIELD_MARGIN * 2) - FIELD_MARGIN;
            rect.y = g->enemyContainer.posy + (y * FIELD_HEIGHT) + (y * FIELD_MARGIN * 2) - FIELD_MARGIN;
            rect.w = FIELD_WIDTH;
            rect.h = FIELD_HEIGHT;
            
            // Alientyp Abfrage
            if (g->enemyContainer.enemys[x][y].type == 1) {
                // Sprite in Alienbox zentrieren
                rect.x += (FIELD_WIDTH - ALIEN1_WIDTH) / 2;
                rect.y += (FIELD_HEIGHT - ALIEN1_HEIGHT) / 2;
                rect.w = ALIEN1_WIDTH;
                rect.h = ALIEN1_HEIGHT;
                
                // Animationsstatus abfragen und zeichnen
                if (g->enemyContainer.enemys[x][y].animationState == 0) {
                    drawSprite(g->screen, &rect, "alien1_1.bmp");
                } else {
                    drawSprite(g->screen, &rect, "alien1_2.bmp");
                }
            } else if (g->enemyContainer.enemys[x][y].type == 2) {
                // Sprite in Alienbox zentrieren
                rect.x += (FIELD_WIDTH - ALIEN2_WIDTH) / 2;
                rect.y += (FIELD_HEIGHT - ALIEN2_HEIGHT) / 2;
                rect.w = ALIEN2_WIDTH;
                rect.h = ALIEN2_HEIGHT;
                
                // Animationsstatus abfragen und zeichnen
                if (g->enemyContainer.enemys[x][y].animationState == 0) {
                    drawSprite(g->screen, &rect, "alien2_1.bmp");
                } else {
                    drawSprite(g->screen, &rect, "alien2_2.bmp");
                }
            } else if (g->enemyContainer.enemys[x][y].type == 3) {
                // Sprite in Alienbox zentrieren
                rect.x += (FIELD_WIDTH - ALIEN3_WIDTH) / 2;
                rect.y += (FIELD_HEIGHT - ALIEN3_HEIGHT) / 2;
                rect.w = ALIEN3_WIDTH;
                rect.h = ALIEN3_HEIGHT;
                
                // Animationsstatus abfragen und zeichnen
                if (g->enemyContainer.enemys[x][y].animationState == 0) {
                    drawSprite(g->screen, &rect, "alien3_1.bmp");
                } else {
                    drawSprite(g->screen, &rect, "alien3_2.bmp");
                }
            }
            
            // Animation ändern
            g->enemyContainer.enemys[x][y].animationState = (g->enemyContainer.enemys[x][y].animationState == 0) ? 1 : 0;
        }
    }
}

// Game Struct initialisieren
void initGame(Game *g)
{
    // Anfangslevel setzten
    g->level = 1;
    
    // Spielerposition
    g->player.rect.x = BORDER;
    g->player.rect.y = PLAYER_Y_POS;
    g->player.rect.w = PLAYER_WIDTH;
    g->player.rect.h = PLAYER_HEIGHT;
    
    // Nur zeichen, daher None
    movePlayer(g, None);
    
    // Erste Reihe: Typ 1
    for (int x = 0; x < ENEMYFIELD_WIDTH; x++) {
        g->enemyContainer.enemys[x][0].dead = false;
        g->enemyContainer.enemys[x][0].type = 1;
        g->enemyContainer.enemys[x][0].animationState = 0;
    }
    
    // Reihe 2+3: Typ 2
    for (int y = 1; y < 3; y++) {
        for (int x = 0; x < ENEMYFIELD_WIDTH; x++) {
            g->enemyContainer.enemys[x][y].dead = false;
            g->enemyContainer.enemys[x][y].type = 2;
            g->enemyContainer.enemys[x][y].animationState = 0;
        }
    }
    
    // Reihe 4+5: Typ 3
    for (int y = 3; y < ENEMYFIELD_HEIGHT; y++) {
        for (int x = 0; x < ENEMYFIELD_WIDTH; x++) {
            g->enemyContainer.enemys[x][y].dead = false;
            g->enemyContainer.enemys[x][y].type = 3;
            g->enemyContainer.enemys[x][y].animationState = 0;
        }
    }
    
    // Alien Box Positionieren
    g->enemyContainer.posx = BORDER;
    g->enemyContainer.posy = BORDER;
    g->enemyContainer.aliveCount = ENEMY_COUNT;
    g->enemyContainer.moveDirection = Right;
    
    // Box Zeichnen
    moveEnemys(g);
    
    // Nächster Grafikzustand
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
    
    // Game initialisieren
    initGame(g);
    
    // Loop
    while (!quit) {
        // SDL Events abfragen
        SDL_PollEvent(&event);
        
        // Tastenstatus laden
        keystates = SDL_GetKeyState(NULL);
        
        // Escape gedrückt -> beenden
        // TODO: Menü aufrufen statt beenden
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
            // Loop verzögern, um Geschwindigkeit zu verlangsamen
            usleep(20000);
        }
        
        // Alienposition aktualisieren?
        // Exponentialfunktion, die Level und Alienanzahl berücksichtigt
        if (ms_time() - lastupdate >= 1000 * pow(0.95, g->level * 2 + (ENEMY_COUNT - g->enemyContainer.aliveCount) / 10)) {
            lastupdate = ms_time();
            moveEnemys(g);
        }
        
        // Nächster Grafikzustand
        SDL_Flip(g->screen);
    }
    
    SDL_Quit();
    return 0;
}
