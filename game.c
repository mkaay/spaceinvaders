#include "game.h"
#include "helpers.h"
#include "menu.h"

#include <unistd.h>

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


int enemysLanded(Game *g)
{
    if (g->enemyContainer.posy + (ENEMYFIELD_HEIGHT * FIELD_HEIGHT) + (ENEMYFIELD_HEIGHT * FIELD_MARGIN * 2) < PLAYER_Y_POS) {
        return false;
    } else {
        int aliverow = -1;
        for (int y = ENEMYFIELD_HEIGHT-1; y >= 0; y--) {
            for (int x = 0; x < ENEMYFIELD_WIDTH; x++) {
                if (!g->enemyContainer.enemys[x][y].dead) {
                    aliverow = y;
                }
            }
        }
        
        if (aliverow == -1) {
            return false;
        }
        
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
            // TODO: Game over
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

// Game Struct initialisieren
void initGame(Game *g)
{
    g->level = 0;
    g->score = 0;
    g->enemyShots = NULL;
    g->player.shot = NULL;
    g->player.lives = 2;
    
    // Spielerposition
    g->player.rect.x = WIDTH/2 - PLAYER_WIDTH/2;
    g->player.rect.y = PLAYER_Y_POS;
    g->player.rect.w = PLAYER_WIDTH;
    g->player.rect.h = PLAYER_HEIGHT;
    
    // Nur zeichen, daher None
    movePlayer(g, None);
}

void startNewLevel(Game *g)
{
    g->level++;
    g->player.lives++;
    
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
    
    // Box Zeichnen
    moveEnemys(g);
}

int killEnemy(Game *g, int x, int y)
{
    if (!g->enemyContainer.enemys[x][y].dead) {
        SDL_Rect p_rect;
        SDL_Rect e_rect;
        
        p_rect.x = g->player.shot->posx;
        p_rect.y = g->player.shot->posy;
        p_rect.w = PLAYER_SHOT_WIDTH;
        p_rect.h = PLAYER_SHOT_HEIGHT;
        
        e_rect.x = g->enemyContainer.posx + (x * FIELD_WIDTH) + (x * FIELD_MARGIN * 2) - FIELD_MARGIN;
        e_rect.y = g->enemyContainer.posy + (y * FIELD_HEIGHT) + (y * FIELD_MARGIN * 2) - FIELD_MARGIN;
        e_rect.w = FIELD_WIDTH;
        e_rect.h = FIELD_HEIGHT;
        
        if (!collides(p_rect, e_rect)) {
            return false;
        }
        
        SDL_FillRect(g->screen, &p_rect, SDL_MapRGB(g->screen->format, 0, 0, 0));
        
        free(g->player.shot);
        g->player.shot = NULL;
        g->enemyContainer.enemys[x][y].dead = true;
        
        SDL_FillRect(g->screen, &e_rect, SDL_MapRGB(g->screen->format, 0, 0, 0));
        
        g->enemyContainer.aliveCount--;
        if (g->enemyContainer.aliveCount == 0) {
            startNewLevel(g);
        }
        
        if (g->enemyContainer.enemys[x][y].type == 3) {
            g->score += 10;
        } else if (g->enemyContainer.enemys[x][y].type == 2) {
            g->score += 20;
        } else if (g->enemyContainer.enemys[x][y].type == 1) {
            g->score += 40;
        }
        updateScore(g);
        
        return true;
    } else {
        return false;
    }
}

void checkCollision(Game *g)
{
    if (g->player.shot != NULL) {
        if (g->enemyContainer.posx < g->player.shot->posx && g->enemyContainer.posy < g->player.shot->posy) {
            int dx = g->player.shot->posx - g->enemyContainer.posx + FIELD_MARGIN;
            int dy = g->player.shot->posy - g->enemyContainer.posy + FIELD_MARGIN;
            
            int x1 = dx / (FIELD_WIDTH + 2*FIELD_MARGIN);
            int y1 = dy / (FIELD_HEIGHT + 2*FIELD_MARGIN);
            int x2 = (dx + PLAYER_SHOT_WIDTH) / (FIELD_WIDTH + 2*FIELD_MARGIN);
            int y2 = (dy + PLAYER_SHOT_HEIGHT) / (FIELD_HEIGHT + 2*FIELD_MARGIN);
            
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
    }
}

void updateShots(Game *g)
{
    if (g->player.shot != NULL) {
        SDL_Rect rect;
        rect.x = g->player.shot->posx;
        rect.y = g->player.shot->posy;
        rect.w = PLAYER_SHOT_WIDTH;
        rect.h = PLAYER_SHOT_HEIGHT;
        
        SDL_FillRect(g->screen, &rect, SDL_MapRGB(g->screen->format, 0, 0, 0));
        
        g->player.shot->posy -= PLAYER_SHOT_SPEED;
        if (g->player.shot->posy < BORDER_TOP) {
            free(g->player.shot);
            g->player.shot = NULL;
            return;
        }
        rect.y = g->player.shot->posy;
        
        drawSprite(g->screen, &rect, "player_shot.bmp");
    }
    
    Shot *s = g->enemyShots;
    while (s != NULL) {
        if (s->type == 1) {
            s->posy += ALIEN_1_SHOT_SPEED;
        } else if (s->type == 2) {
            s->posy += ALIEN_2_SHOT_SPEED;
        } else if (s->type == 3) {
            s->posy += ALIEN_3_SHOT_SPEED;
        }
        s = s->next;
    }
}

void shoot(Game *g)
{
    if (g->player.shot == NULL) {
        g->player.shot = malloc(sizeof(Shot));
        g->player.shot->type = 0;
        g->player.shot->next = NULL;
        g->player.shot->posy = g->player.rect.y;
        g->player.shot->posx = g->player.rect.x + PLAYER_WIDTH/2 - PLAYER_SHOT_WIDTH/2;
    }
}