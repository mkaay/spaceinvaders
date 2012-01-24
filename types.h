#ifndef _types_h
#define _types_h

#include <SDL.h>

// Einstellungen Anfang

// Spielfeld Dimension
#define WIDTH 800
#define HEIGHT 700
#define BORDER 20
#define BORDER_TOP 50

// Spieler
#define PLAYER_WIDTH 52
#define PLAYER_HEIGHT 30
#define PLAYER_MOVE_INTERVAL 5

// Aliens
#define ENEMYFIELD_WIDTH 11
#define ENEMYFIELD_HEIGHT 6
#define ENEMY_MOVE_INTERVAL_HORIZONTAL 10
#define ENEMY_MOVE_INTERVAL_VERTICAL 30
#define UFO_SPEED 10

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

// Schüsse Gewschindigkeit
#define PLAYER_SHOT_SPEED 20
#define ALIEN_1_SHOT_SPEED 9
#define ALIEN_2_SHOT_SPEED 6
#define ALIEN_3_SHOT_SPEED 3

// Schüsse Dimensionen
#define PLAYER_SHOT_WIDTH 8
#define PLAYER_SHOT_HEIGHT 16
#define ALIEN_1_SHOT_WIDTH 14
#define ALIEN_2_SHOT_WIDTH 12
#define ALIEN_3_SHOT_WIDTH 12
#define ALIEN_1_SHOT_HEIGHT 24
#define ALIEN_2_SHOT_HEIGHT 24
#define ALIEN_3_SHOT_HEIGHT 24

// Punkte
#define ALIEN_1_POINTS 40
#define ALIEN_2_POINTS 20
#define ALIEN_3_POINTS 10

// Zeiten
#define ENEMY_UPDATE_BASE 1000
#define SHOT_UPDATE 20
#define UFO_UPDATE 50
#define MIN_UFO_PAUSE 15000

// Blöcke
#define MAX_BLOCK_DAMAGE 4
#define BLOCK_COUNT 5
#define BLOCK_WIDTH 80
#define BLOCK_HEIGHT 60
#define BLOCK_TILE_WIDTH 20
#define BLOCK_TILE_HEIGHT 20

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
    int type;
    struct Shot *next;
    struct Shot *prev;
} Shot;

typedef struct Block {
    int damage[BLOCK_WIDTH/BLOCK_TILE_WIDTH][BLOCK_HEIGHT/BLOCK_TILE_HEIGHT];
    int posx[BLOCK_WIDTH/BLOCK_TILE_WIDTH][BLOCK_HEIGHT/BLOCK_TILE_HEIGHT];
    int posy[BLOCK_WIDTH/BLOCK_TILE_WIDTH][BLOCK_HEIGHT/BLOCK_TILE_HEIGHT];
} Block;

typedef struct Player {
    SDL_Rect rect;
    int lives;
    Shot *shot;
} Player;

typedef struct Ufo {
    int posx;
    int alive;
    long lastufo;
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
    int score;
    Block *blocks;
} Game;

#endif
