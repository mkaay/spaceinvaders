#ifndef _types_h
#define _types_h

#include <SDL.h>

// Einstellungen Anfang

// Spielfeld Dimension
#define WIDTH 800
#define HEIGHT 600
#define BORDER 20

// Spieler
#define PLAYER_WIDTH 52
#define PLAYER_HEIGHT 30
#define PLAYER_MOVE_INTERVAL 5

// Aliens
#define ENEMYFIELD_WIDTH 11
#define ENEMYFIELD_HEIGHT 6
#define ENEMY_MOVE_INTERVAL_HORIZONTAL 10
#define ENEMY_MOVE_INTERVAL_VERTICAL 30

// Alien Platzhalter
#define FIELD_WIDTH 48
#define FIELD_HEIGHT 36
#define FIELD_MARGIN 5

// Alien Dimension
#define ALIEN1_WIDTH 32
#define ALIEN2_WIDTH 44
#define ALIEN3_WIDTH 48
#define ALIEN1_HEIGHT 32
#define ALIEN2_HEIGHT 32
#define ALIEN3_HEIGHT 36
#define UFO_WIDTH 64
#define UFO_HEIGHT 28

// Einstellungen Ende

// Automatisch berechnet
#define ENEMY_COUNT ENEMYFIELD_WIDTH*ENEMYFIELD_HEIGHT
#define PLAYER_Y_POS HEIGHT-PLAYER_HEIGHT-BORDER

// Konstanten
#define true 1
#define false 0

// Structs

typedef enum Direction {
    None,
    Left,
    Right,
    Up,
    Down
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
    int animationState;
} Enemy;

typedef struct EnemyContainer {
    Enemy enemys[ENEMYFIELD_WIDTH][ENEMYFIELD_HEIGHT];
    Ufo ufo;
    int posx;
    int posy;
    int aliveCount;
    Direction moveDirection;
} EnemyContainer;

typedef struct Game {
    SDL_Surface *screen;
    Player player;
    EnemyContainer enemyContainer;
    Shot *enemyShots;
    int level;
} Game;

#endif
