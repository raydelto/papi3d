#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "types.h"
#include "graphics.h"
#include <string.h>

void drawPixel(SDL_Surface *surface, s32 x, s32 y, s32 color)
{
    u32 offset = SCREEN_WIDTH * y + x;
    if (offset >= TOTAL_PIXELS)
    {
        return;
    }

    u32 *pixels;
    pixels = (u32 *)surface->pixels;
    pixels[offset] = color;
}

void clearSreen(SDL_Surface *surface)
{
    u32 *pixels;
    pixels = (u32 *)surface->pixels;
    memset(pixels, 0, SCREEN_WIDTH * SCREEN_HEIGHT * 4);
}

void drawVertLine(SDL_Surface *surface, s32 x, s32 y1, s32 y2, s32 color)
{
    s32 yi = 0;
    s32 yf = 0;

    if (y1 < y2)
    {
        yi = y1;
        yf = y2;
    }
    else if (y2 < y1)
    {
        yi = y2;
        yf = y1;
    }

    while (yi <= yf)
    {
        drawPixel(surface, x, yi, color);
        yi++;
    }
}

void drawHorLine(SDL_Surface *surface, s32 x1, s32 x2, s32 y, s32 color)
{
    s32 xi = 0;
    s32 xf = 0;

    if (x1 < x2)
    {
        xi = x1;
        xf = x2;
    }
    else if (x2 < x1)
    {
        xi = x2;
        xf = x1;
    }

    while (xi <= xf)
    {
        drawPixel(surface, xi, y, color);
        xi++;
    }
}

void drawSlopedLine(SDL_Surface *surface, s32 x1, s32 y1, s32 x2, s32 y2, s32 color)
{
    s32 xi = 0;
    s32 xf = 0;
    s32 rise = 0, run = 0;
    s32 y = 0, b = 0;
    float m;

    rise = y2 - y1;
    run = x2 - x1;
    m = (float)rise / (float)run;
    b = -m * x2 + y2;

    if (x1 < x2)
    {
        xi = x1;
        xf = x2;
    }
    else if (x2 < x1)
    {
        xi = x2;
        xf = x1;
    }

    while (xi <= xf)
    {
        y = m * xi + b;
        drawPixel(surface, xi, y, color);
        xi++;
    }
}

void drawLine(SDL_Surface *surface, s32 x1, s32 y1, s32 x2, s32 y2, s32 color)
{
    if (x1 == x2)
    { // Draw Vertical Line
        drawVertLine(surface, x1, y1, y2, color);
    }
    else if (y1 == y2)
    { // Draw Horizontal Line
        drawHorLine(surface, x1, x2, y1, color);
    }
    else
    { // Draw sloped Line
        drawSlopedLine(surface, x1, y1, x2, y2, color);
    }
}

void drawBox(SDL_Surface *surface, s32 x, s32 y)
{
    drawHorLine(surface, x, x + BOX_X_SIZE, y, 0x00FFFFFF);
    drawHorLine(surface, x, x + BOX_X_SIZE, y + BOX_Y_SIZE, 0x00FFFFFF);
    drawVertLine(surface, x, y, y + BOX_Y_SIZE, 0x00FFFFFF);
    drawVertLine(surface, x + BOX_X_SIZE, y, y + BOX_Y_SIZE, 0x00FFFFFF);
}
