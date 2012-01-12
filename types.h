#ifndef _types_h
#define _types_h

#include <SDL.h>

#define true 1
#define false 0

typedef enum Direction {
    NoMove,
    Left,
    Right
} Direction;

typedef struct Shot {
    int posx;
    int posy;
    struct Shot *next;
} Shot;

typedef struct Player {
    SDL_Rect rect;
    int lifes;
    int scores;
    Shot *shot;
} Player;

typedef struct Ufo {
    int posx;
    int alive;
} Ufo;

typedef struct Enemy {
    int type;
    int dead;
} Enemy;

typedef struct EnemyContainer {
    Enemy **enemys;
    Ufo *ufo;
    int posx;
    int posy;
    int aliveCount;
    int moveDirection;
} EnemyContainer;

typedef struct Game {
    SDL_Surface *screen;
    Player *player;
    EnemyContainer *enemyContainer;
    Shot *enemyShots;
    int level;
} Game;

#endif
