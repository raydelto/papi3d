#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "types.h"

struct pl {
    FP xpos, ypos;
    FP dx, dy;
    s16 angle;
    s8 fov;
};

typedef struct pl PLAYER;

void initPlayer(PLAYER *p);
void drawPlayer(SDL_Surface *surface, PLAYER *p);
void drawRays(SDL_Surface *surface, PLAYER *p, u8 m[15][20], u8 view);
void drawFloorCeiling(SDL_Surface *surface);
