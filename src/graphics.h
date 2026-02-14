#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "types.h"

constexpr int SCREEN_WIDTH = 640;
constexpr int SCREEN_HEIGHT = 480;
constexpr int TOTAL_PIXELS = SCREEN_WIDTH * SCREEN_HEIGHT;
constexpr int BOX_X_SIZE = 32;
constexpr int BOX_Y_SIZE = 32;

void drawPixel(SDL_Surface *surface, s32 x, s32 y, s32 color);
void clearSreen(SDL_Surface *surface);
void drawVertLine(SDL_Surface *surface, s32 x, s32 y1, s32 y2, s32 color);
void drawLine(SDL_Surface *surface, s32 x1, s32 y1, s32 x2, s32 y2, s32 color);
void drawHorLine(SDL_Surface *surface, s32 x1, s32 x2, s32 y, s32 color);
void drawSlopedLine(SDL_Surface *surface, s32 x1, s32 y1, s32 x2, s32 y2, s32 color);
void drawBox(SDL_Surface *surface, s32 x, s32 y);
