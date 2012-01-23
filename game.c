#include "game.h"
#include "helpers.h"
#include "menu.h"
#include "highscore.h"

#include <unistd.h>
#include <stdlib.h>

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

// Abfrage ob Aliens am unteren Spielfeldrand angekommen sind
int enemysLanded(Game *g)
{
    // Box angekommen?
    if (g->enemyContainer.posy + (ENEMYFIELD_HEIGHT * FIELD_HEIGHT) + (ENEMYFIELD_HEIGHT * FIELD_MARGIN * 2) < PLAYER_Y_POS) {
        // Nicht
        return false;
    } else {
        int aliverow = -1;
        // Welches ist die unterste Reihe in der noch min. 1 Alien lebt
        for (int y = ENEMYFIELD_HEIGHT-1; y >= 0; y--) {
            for (int x = 0; x < ENEMYFIELD_WIDTH; x++) {
                if (!g->enemyContainer.enemys[x][y].dead) {
                    aliverow = y;
                }
            }
        }
        
        // Alle tot (sollte nicht passieren, trotzdem false zurückgeben)
        if (aliverow == -1) {
            return false;
        }
        
        // Letzte Reihe unten?
        if (g->enemyContainer.posy + (aliverow * FIELD_HEIGHT) + (aliverow * FIELD_MARGIN * 2) < PLAYER_Y_POS) {
            return false;
        } else {
            return true;
        }
    }
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
        if (!enemysLanded(g)) {
            g->enemyContainer.moveDirection = (g->enemyContainer.posx <= BORDER) ? Right : Left;
            dx = (g->enemyContainer.posx <= BORDER) ? ENEMY_MOVE_INTERVAL_HORIZONTAL : -ENEMY_MOVE_INTERVAL_HORIZONTAL;
        } else {
            // Unten angekommen
            playerDead(g);
        }
    }
    
    // Alte Position "befreien"
    for (int x = 0; x < ENEMYFIELD_WIDTH; x++) {
        for (int y = 0; y < ENEMYFIELD_HEIGHT; y++) {
            // Alien tot -> überspringen
            if (g->enemyContainer.enemys[x][y].dead) {
                continue;
            }
            
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
            // Alien tot -> überspringen
            if (g->enemyContainer.enemys[x][y].dead) {
                continue;
            }
            
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


// Alien Schuss erstellen
void alienShot(Game *g)
{
    // nicht jedesmal
    if (rand() % 2) {
        return;
    }
    
    Enemy *e;
    int ex;
    int ey;
    
    // Zufälliges Alien auf unterster Stufe
    do {
        e = NULL;
        ex = rand() % ENEMYFIELD_WIDTH;
        ey = -1;
        
        for (int y = ENEMYFIELD_HEIGHT-1; y >= 0; y--) {
            e = &g->enemyContainer.enemys[ex][y];
            if (!e->dead) {
                ey = y;
                break;
            }
        }
    } while (ey == -1);
    
    // noch ein Alien da
    if (e != NULL) {
        // Position berechen
        Shot *s = (Shot*) malloc(sizeof(Shot));
        s->posx = g->enemyContainer.posx + (ex * FIELD_WIDTH) + (ex * FIELD_MARGIN * 2) - FIELD_MARGIN + FIELD_WIDTH/2;
        s->posy = g->enemyContainer.posy + (ey * FIELD_HEIGHT) + (ey * FIELD_MARGIN * 2) - FIELD_MARGIN + FIELD_HEIGHT;
        s->type = e->type;
        s->next = NULL;
        s->prev = NULL;
        
        if (e->type == 1) {
            s->posx -= ALIEN_1_SHOT_WIDTH/2;
        } else if (e->type == 2) {
            s->posx -= ALIEN_2_SHOT_WIDTH/2;
        } else if (e->type == 3) {
            s->posx -= ALIEN_3_SHOT_WIDTH/2;
        }
        
        // Zur Liste hinzufügen
        if (g->enemyShots == NULL) {
            g->enemyShots = s;
        } else {
            Shot *tmp = g->enemyShots;
            for (;;) {
                if (tmp->next == NULL) {
                    tmp->next = s;
                    s->prev = tmp;
                    break;
                }
                tmp = tmp->next;
            }
        }
    }
}


// Game Struct initialisieren
void initGame(Game *g)
{
    // Startwerte
    g->level = 0;
    g->score = 0;
    g->enemyShots = NULL;
    g->player.shot = NULL;
    g->player.lives = 2; // Nur 2, weil danach startNewLevel aufgerufen wird (-> ein Leben mehr)
    g->enemyContainer.ufo.alive = false;
    g->enemyContainer.ufo.posx = 0;
    g->enemyContainer.ufo.lastufo = ms_time();
    
    // Spielerposition
    g->player.rect.x = WIDTH/2 - PLAYER_WIDTH/2;
    g->player.rect.y = PLAYER_Y_POS;
    g->player.rect.w = PLAYER_WIDTH;
    g->player.rect.h = PLAYER_HEIGHT;
    
    g->blocks = malloc(sizeof(Block) * 4);
    for (int i = 0; i < 4; i++) {
        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 3; y++) {
                g->blocks[i].damage[x][y] = 0;
                g->blocks[i].posx[x][y] = WIDTH/4 * i - 40 + x*20 + WIDTH/8;
                g->blocks[i].posy[x][y] = PLAYER_Y_POS - 100 + y*20;
            }
        }
    }
    
    // Nur zeichen, daher None
    movePlayer(g, None);
}


// Leben verloren
void playerDead(Game *g)
{
    freeShotList(g->enemyShots);
    g->enemyShots = NULL;
    if (g->player.shot != NULL) {
        free(g->player.shot);
        g->player.shot = NULL;
    }
    
    g->player.lives -= 2; // startNewLevel erhöht um 1
    g->level--;
    g->enemyContainer.ufo.alive = false;
    g->enemyContainer.ufo.lastufo = ms_time();
    
    // Spielerposition
    g->player.rect.x = WIDTH/2 - PLAYER_WIDTH/2;
    g->player.rect.y = PLAYER_Y_POS;
    g->player.rect.w = PLAYER_WIDTH;
    g->player.rect.h = PLAYER_HEIGHT;
    
    SDL_Rect area = {0, BORDER_TOP, WIDTH, HEIGHT - BORDER_TOP};
    SDL_FillRect(g->screen, &area, SDL_MapRGB(g->screen->format, 0, 0, 0));
    
    if (g->player.lives+2 == 0) {
        free(g->blocks);
        
        showGameOver(g);
        saveHighscore(g->score);
        showHighscore(g);
        
        SDL_Flip(g->screen);
        
        // Warten auf Tastendruck
        SDL_Event e;
        SDL_WaitEvent(&e);
        
        SDL_FillRect(g->screen, &area, SDL_MapRGB(g->screen->format, 0, 0, 0));
        
        initGame(g);
        startNewLevel(g);
        
        return;
    }
    
    SDL_Flip(g->screen);
    usleep(100000);
    
    // Nur zeichen, daher None
    movePlayer(g, None);
    
    startNewLevel(g);
}


// Neues Level starten
void startNewLevel(Game *g)
{
    g->level++;
    g->player.lives++;
    
    updateLives(g);
    
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
    g->enemyContainer.posy = BORDER_TOP + 50;
    g->enemyContainer.aliveCount = ENEMY_COUNT;
    g->enemyContainer.moveDirection = Right;
    
    SDL_Flip(g->screen);
    usleep(500000);
    
    // Box Zeichnen
    moveEnemys(g);
}


// 'Versuchen' Alien zu töten (wenn schon tot, false zurückgeben)
int killEnemy(Game *g, int x, int y)
{
    if (!g->enemyContainer.enemys[x][y].dead) {
        SDL_Rect p_rect = {g->player.shot->posx, g->player.shot->posy, PLAYER_SHOT_WIDTH, PLAYER_SHOT_HEIGHT};
        SDL_Rect e_rect;
        
        e_rect.x = g->enemyContainer.posx + (x * FIELD_WIDTH) + (x * FIELD_MARGIN * 2) - FIELD_MARGIN;
        e_rect.y = g->enemyContainer.posy + (y * FIELD_HEIGHT) + (y * FIELD_MARGIN * 2) - FIELD_MARGIN;
        e_rect.w = FIELD_WIDTH;
        e_rect.h = FIELD_HEIGHT;
        
        // Überscheiden die Umrisse sich?
        if (!collides(p_rect, e_rect)) {
            return false;
        }
        
        // Schuss überschreiben
        SDL_FillRect(g->screen, &p_rect, SDL_MapRGB(g->screen->format, 0, 0, 0));
        
        // Speicher freimachen
        free(g->player.shot);
        // Pointer auf NULL setzen
        g->player.shot = NULL;
        // Alien tot
        g->enemyContainer.enemys[x][y].dead = true;
        
        // Alien löschen
        SDL_FillRect(g->screen, &e_rect, SDL_MapRGB(g->screen->format, 0, 0, 0));
        
        // Zähler dekrementieren
        g->enemyContainer.aliveCount--;
        
        // Punkte gutschreiben
        if (g->enemyContainer.enemys[x][y].type == 3) {
            g->score += ALIEN_3_POINTS;
        } else if (g->enemyContainer.enemys[x][y].type == 2) {
            g->score += ALIEN_2_POINTS;
        } else if (g->enemyContainer.enemys[x][y].type == 1) {
            g->score += ALIEN_1_POINTS;
        }
        // Anzeigen
        updateScore(g);
        
        // Alle tot -> neues Level
        if (g->enemyContainer.aliveCount == 0) {
            startNewLevel(g);
        }
        
        return true;
    } else {
        return false;
    }
}


// Kollisionen auf dem Spielfeld überprüfen
void checkCollision(Game *g)
{
    if (g->player.shot != NULL) {
        if (g->enemyContainer.posx < g->player.shot->posx && g->enemyContainer.posy < g->player.shot->posy) {
            // Überlappende Felder in der Alienbox berechnen
            int dx = g->player.shot->posx - g->enemyContainer.posx + FIELD_MARGIN;
            int dy = g->player.shot->posy - g->enemyContainer.posy + FIELD_MARGIN;
            
            int x1 = dx / (FIELD_WIDTH + 2*FIELD_MARGIN);
            int y1 = dy / (FIELD_HEIGHT + 2*FIELD_MARGIN);
            int x2 = (dx + PLAYER_SHOT_WIDTH) / (FIELD_WIDTH + 2*FIELD_MARGIN);
            int y2 = (dy + PLAYER_SHOT_HEIGHT) / (FIELD_HEIGHT + 2*FIELD_MARGIN);
            
            // Felder durchgehen
            if (x1 < ENEMYFIELD_WIDTH && y1 < ENEMYFIELD_HEIGHT) {
                if (killEnemy(g, x1, y1)) {
                    return;
                }
            }
            if (x1 < ENEMYFIELD_WIDTH && y2 < ENEMYFIELD_HEIGHT) {
                if (killEnemy(g, x1, y2)) {
                    return;
                }
            }
            if (x2 < ENEMYFIELD_WIDTH && y1 < ENEMYFIELD_HEIGHT) {
                if (killEnemy(g, x2, y1)) {
                    return;
                }
            }
            if (x2 < ENEMYFIELD_WIDTH && y2 < ENEMYFIELD_HEIGHT) {
                if (killEnemy(g, x2, y2)) {
                    return;
                }
            }
        }
        
        SDL_Rect p_rect = {g->player.shot->posx, g->player.shot->posy, PLAYER_SHOT_WIDTH, PLAYER_SHOT_HEIGHT};
        
        // UFO Kollision abfragen
        if (g->enemyContainer.ufo.alive) {
            SDL_Rect ufo_rect = {g->enemyContainer.ufo.posx, BORDER_TOP + 5, UFO_WIDTH, UFO_HEIGHT};
            if (collides(ufo_rect, p_rect)) {
                g->score += ((rand() % 12)+1) * 25;
                g->enemyContainer.ufo.alive = false;
                SDL_FillRect(g->screen, &p_rect, SDL_MapRGB(g->screen->format, 0, 0, 0));
                SDL_FillRect(g->screen, &ufo_rect, SDL_MapRGB(g->screen->format, 0, 0, 0));
                updateScore(g);
                free(g->player.shot);
                g->player.shot = NULL;
            }
        }
        
        
        SDL_Rect rect;
        rect.h = 20;
        rect.w = 20;
        
        for (int i = 0; i < 4; i++) {
            for (int x = 0; x < 4; x++) {
                for (int y = 0; y < 3; y++) {
                    rect.x = g->blocks[i].posx[x][y];
                    rect.y = g->blocks[i].posy[x][y];
                    
                    if (collides(rect, p_rect) && g->blocks[i].damage[x][y] < MAX_BLOCK_DAMAGE) {
                        g->blocks[i].damage[x][y]++;
                        SDL_FillRect(g->screen, &p_rect, SDL_MapRGB(g->screen->format, 0, 0, 0));
                        free(g->player.shot);
                        g->player.shot = NULL;
                    }
                }
            }
        }
        
    }
    
    SDL_Rect s_rect;
    Shot *s = g->enemyShots;
    while (s != NULL) {
        s_rect.x = s->posx;
        s_rect.y = s->posy;
        
        if (s->type == 1) {
            s_rect.w = ALIEN_1_SHOT_WIDTH;
            s_rect.h = ALIEN_1_SHOT_HEIGHT;
        } else if (s->type == 2) {
            s_rect.w = ALIEN_2_SHOT_WIDTH;
            s_rect.h = ALIEN_2_SHOT_HEIGHT;
        } else if (s->type == 3) {
            s_rect.w = ALIEN_3_SHOT_WIDTH;
            s_rect.h = ALIEN_3_SHOT_HEIGHT;
        }
        
        if (collides(s_rect, g->player.rect)) {
            playerDead(g);
            break;
        }
        
        SDL_Rect rect;
        rect.h = 20;
        rect.w = 20;
        
        for (int i = 0; i < 4; i++) {
            for (int x = 0; x < 4; x++) {
                for (int y = 0; y < 3; y++) {
                    rect.x = g->blocks[i].posx[x][y];
                    rect.y = g->blocks[i].posy[x][y];
                    
                    if (collides(rect, s_rect) && g->blocks[i].damage[x][y] < MAX_BLOCK_DAMAGE) {
                        g->blocks[i].damage[x][y]++;
                        SDL_FillRect(g->screen, &s_rect, SDL_MapRGB(g->screen->format, 0, 0, 0));
                        
                        Shot *tmp = s;
                        if (s->prev == NULL) {
                            g->enemyShots = s->next;
                        } else {
                            s->prev->next = s->next;
                        }
                        if (s->next != NULL) {
                            s->next->prev = s->prev;
                        }
                        s = s->next;
                        free(tmp);
                        goto block_loop_break;
                    }
                }
            }
        }
        
        s = s->next;
        
        block_loop_break:
        continue;
    }
    
    updateBlocks(g);
}


// Schüsse bewegen
void updateShots(Game *g)
{
    SDL_Rect rect;
    
    if (g->player.shot != NULL) {
        // Spieler Schuss
        
        // Umriss setzen
        rect.x = g->player.shot->posx;
        rect.y = g->player.shot->posy;
        rect.w = PLAYER_SHOT_WIDTH;
        rect.h = PLAYER_SHOT_HEIGHT;
        
        // Überschreiben
        SDL_FillRect(g->screen, &rect, SDL_MapRGB(g->screen->format, 0, 0, 0));
        
        // Neue Position
        g->player.shot->posy -= PLAYER_SHOT_SPEED;
        if (g->player.shot->posy < BORDER_TOP) {
            // Oben angekommen
            free(g->player.shot);
            g->player.shot = NULL;
            return;
        }
        rect.y = g->player.shot->posy;
        
        // Zeichen
        drawSprite(g->screen, &rect, "player_shot.bmp");
    }
    
    // durch Schüsse iterieren
    Shot *s = g->enemyShots;
    while (s != NULL) {
        // Position einlesen
        rect.x = s->posx;
        rect.y = s->posy;
        
        if (s->type == 1) {
            rect.w = ALIEN_1_SHOT_WIDTH;
            rect.h = ALIEN_1_SHOT_HEIGHT;
        } else if (s->type == 2) {
            rect.w = ALIEN_2_SHOT_WIDTH;
            rect.h = ALIEN_2_SHOT_HEIGHT;
        } else if (s->type == 3) {
            rect.w = ALIEN_3_SHOT_WIDTH;
            rect.h = ALIEN_3_SHOT_HEIGHT;
        }
        
        // Überschreiben
        SDL_FillRect(g->screen, &rect, SDL_MapRGB(g->screen->format, 0, 0, 0));
        
        // Am Rand -> löschen
        if (s->posy >= HEIGHT - BORDER) {
            Shot *tmp = s;
            if (s->prev == NULL) {
                g->enemyShots = s->next;
            } else {
                s->prev->next = s->next;
            }
            if (s->next != NULL) {
                s->next->prev = s->prev;
            }
            s = s->next;
            free(tmp);
            continue;
        }
        
        // Neu Zeichnen
        if (s->type == 1) {
            s->posy += ALIEN_1_SHOT_SPEED;
            rect.y = s->posy;
            drawSprite(g->screen, &rect, "alien_shot_1.bmp");
        } else if (s->type == 2) {
            s->posy += ALIEN_2_SHOT_SPEED;
            rect.y = s->posy;
            drawSprite(g->screen, &rect, "alien_shot_2.bmp");
        } else if (s->type == 3) {
            s->posy += ALIEN_3_SHOT_SPEED;
            rect.y = s->posy;
            drawSprite(g->screen, &rect, "alien_shot_3.bmp");
        }
        
        s = s->next;
    }
}


void updateBlocks(Game *g)
{
    SDL_Rect rect;
    rect.h = 20;
    rect.w = 20;
    for (int i = 0; i < 4; i++) {
        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 3; y++) {
                rect.x = g->blocks[i].posx[x][y];
                rect.y = g->blocks[i].posy[x][y];
                
                if (g->blocks[i].damage[x][y] < MAX_BLOCK_DAMAGE) {
                    if (x == 0 && y == 0) {
                        drawSprite(g->screen, &rect, "block_topleft.bmp");
                    } else if (x == 3 && y == 0) {
                        drawSprite(g->screen, &rect, "block_topright.bmp");
                    } else if (x == 0 && y == 2) {
                        drawSprite(g->screen, &rect, "block_bottomleft.bmp");
                    } else if (x == 1 && y == 2) {
                        drawSprite(g->screen, &rect, "block_bottommiddleleft.bmp");
                    } else if (x == 2 && y == 2) {
                        drawSprite(g->screen, &rect, "block_bottommiddleright.bmp");
                    } else if (x == 3 && y == 2) {
                        drawSprite(g->screen, &rect, "block_bottomright.bmp");
                    } else {
                        drawSprite(g->screen, &rect, "block_fill.bmp");
                    }
                } else {
                    SDL_FillRect(g->screen, &rect, SDL_MapRGB(g->screen->format, 0, 0, 0));
                }
            }
        }
    }
}


// Leertaste gedrückt -> Schussversuch
void shoot(Game *g)
{
    // Aktiver Schuss?
    if (g->player.shot == NULL) {
        // Speicher belegen
        g->player.shot = malloc(sizeof(Shot));
        // Initialisieren
        g->player.shot->type = 0;
        g->player.shot->next = NULL;
        g->player.shot->posy = g->player.rect.y;
        g->player.shot->posx = g->player.rect.x + PLAYER_WIDTH/2 - PLAYER_SHOT_WIDTH/2;
    }
}


// entscheidet ob UFO erstellt oder aktualisert wird
void ufo(Game *g)
{
    // 1:1 Chance nach MIN_UFO_PAUSE
    if (!g->enemyContainer.ufo.alive && (MIN_UFO_PAUSE + g->enemyContainer.ufo.lastufo) < ms_time() && (rand() % 2)) {
        g->enemyContainer.ufo.lastufo = ms_time();
        g->enemyContainer.ufo.alive = true;
        g->enemyContainer.ufo.posx = -UFO_WIDTH;
        SDL_Rect rect = {g->enemyContainer.ufo.posx, BORDER_TOP + 5, UFO_WIDTH, UFO_HEIGHT};
        drawSprite(g->screen, &rect, "ufo.bmp");
    } else if (g->enemyContainer.ufo.alive) {
        // UFO lebt noch, Position Aktualisieren
        SDL_Rect rect = {g->enemyContainer.ufo.posx, BORDER_TOP + 5, UFO_WIDTH, UFO_HEIGHT};
        SDL_FillRect(g->screen, &rect, SDL_MapRGB(g->screen->format, 0, 0, 0));
        g->enemyContainer.ufo.posx += UFO_SPEED;
        rect.x = g->enemyContainer.ufo.posx;
        
        if (g->enemyContainer.ufo.posx < WIDTH) {
            // Zeichnen
            drawSprite(g->screen, &rect, "ufo.bmp");
        } else {
            // Außerhalb
            g->enemyContainer.ufo.lastufo = ms_time();
            g->enemyContainer.ufo.alive = false;
        }
    } else if ((MIN_UFO_PAUSE + g->enemyContainer.ufo.lastufo) < ms_time()) {
        // UFO Start verschieben
        g->enemyContainer.ufo.lastufo += ((rand() % 11) * 1000);
    }
}