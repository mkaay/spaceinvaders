#ifndef _game_h
#define _game_h

#include "types.h"

void movePlayer(Game *g, Direction direction);
int enemysLanded(Game *g);
void moveEnemys(Game *g);
void initGame(Game *g);
void startNewLevel(Game *g);
int killEnemy(Game *g, int x, int y);
void checkCollision(Game *g);
void updateShots(Game *g);
void shoot(Game *g);
void ufo(Game *g);
void alienShot(Game *g);
void playerDead(Game *g);
void updateBlocks(Game *g);

#endif