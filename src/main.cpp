#ifdef _WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "types.h"
#include "graphics.h"
#include "map.h"
#include "player.h"
#include "pfloat.h"
#include "constants.h"

int GameMain()
{
    u8 view = 0;
    bool quit = false;
    SDL_Window *window = nullptr;
    SDL_Surface *surface;
    SDL_Event e;
    PLAYER p;
    u32 start, end;

    populateTables();

    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Papi3D", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    surface = SDL_GetWindowSurface(window);

    initPlayer(&p);

    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }

            if (e.type == SDL_KEYDOWN)
            {
                FP newX, newY;
                FP moveSpeed = FLOAT_TO_FP(5); // Movement speed
                
                switch (e.key.keysym.sym)
                {
                case SDLK_w: // Move forward (in direction player is facing)
                    newX = p.xpos + MUL_FP(p.dx, moveSpeed);
                    newY = p.ypos + MUL_FP(p.dy, moveSpeed);
                    // Try full movement first
                    if (canMoveTo(FP_TO_INT(newX), FP_TO_INT(newY)))
                    {
                        p.xpos = newX;
                        p.ypos = newY;
                    }
                    // Try sliding along X axis only
                    else if (canMoveTo(FP_TO_INT(newX), FP_TO_INT(p.ypos)))
                    {
                        p.xpos = newX;
                    }
                    // Try sliding along Y axis only
                    else if (canMoveTo(FP_TO_INT(p.xpos), FP_TO_INT(newY)))
                    {
                        p.ypos = newY;
                    }
                    break;
                case SDLK_s: // Move backward (opposite of facing direction)
                    newX = p.xpos - MUL_FP(p.dx, moveSpeed);
                    newY = p.ypos - MUL_FP(p.dy, moveSpeed);
                    if (canMoveTo(FP_TO_INT(newX), FP_TO_INT(newY)))
                    {
                        p.xpos = newX;
                        p.ypos = newY;
                    }
                    else if (canMoveTo(FP_TO_INT(newX), FP_TO_INT(p.ypos)))
                    {
                        p.xpos = newX;
                    }
                    else if (canMoveTo(FP_TO_INT(p.xpos), FP_TO_INT(newY)))
                    {
                        p.ypos = newY;
                    }
                    break;
                case SDLK_a: // Strafe left (perpendicular to facing direction)
                    newX = p.xpos + MUL_FP(p.dy, moveSpeed);
                    newY = p.ypos - MUL_FP(p.dx, moveSpeed);
                    if (canMoveTo(FP_TO_INT(newX), FP_TO_INT(newY)))
                    {
                        p.xpos = newX;
                        p.ypos = newY;
                    }
                    else if (canMoveTo(FP_TO_INT(newX), FP_TO_INT(p.ypos)))
                    {
                        p.xpos = newX;
                    }
                    else if (canMoveTo(FP_TO_INT(p.xpos), FP_TO_INT(newY)))
                    {
                        p.ypos = newY;
                    }
                    break;
                case SDLK_d: // Strafe right (perpendicular to facing direction)
                    newX = p.xpos - MUL_FP(p.dy, moveSpeed);
                    newY = p.ypos + MUL_FP(p.dx, moveSpeed);
                    if (canMoveTo(FP_TO_INT(newX), FP_TO_INT(newY)))
                    {
                        p.xpos = newX;
                        p.ypos = newY;
                    }
                    else if (canMoveTo(FP_TO_INT(newX), FP_TO_INT(p.ypos)))
                    {
                        p.xpos = newX;
                    }
                    else if (canMoveTo(FP_TO_INT(p.xpos), FP_TO_INT(newY)))
                    {
                        p.ypos = newY;
                    }
                    break;
                case SDLK_LEFT: // Rotate left
                    p.angle = (p.angle - 3 + 360) % 360;
                    p.dx = gCOS[p.angle];
                    p.dy = gSIN[p.angle];
                    p.camX = MUL_FP(gCOS[(p.angle + 90) % 360], FLOAT_TO_FP(0.7));
                    p.camY = MUL_FP(gSIN[(p.angle + 90) % 360], FLOAT_TO_FP(0.7));
                    break;
                case SDLK_RIGHT: // Rotate right
                    p.angle = (p.angle + 3) % 360;
                    p.dx = gCOS[p.angle];
                    p.dy = gSIN[p.angle];
                    p.camX = MUL_FP(gCOS[(p.angle + 90) % 360], FLOAT_TO_FP(0.7));
                    p.camY = MUL_FP(gSIN[(p.angle + 90) % 360], FLOAT_TO_FP(0.7));
                    break;
                case SDLK_e:
                    view ^= 0x01;
                    break;
                case SDLK_ESCAPE:
                case SDLK_q:
                    quit = true;
                    break;
                default:
                    break;
                }
            }
        }

        clearSreen(surface);

        start = SDL_GetTicks();
        if (view)
        {
            drawMap(surface, gMap);
            drawPlayer(surface, &p);
        }
        else
        {
            drawFloorCeiling(surface);
        }
        drawRays(surface, &p, gMap, view);

        end = SDL_GetTicks();
        SDL_UpdateWindowSurface(window);

        // Limit to a max of 60 Frames per seconds (FPS)
        SDL_Delay(PER_FRAME_MILLIS_IN_60_FPS - (start - end));
    }

    SDL_Quit();

    return 0;
}

int main(int argc, char *argv[])
{
    return GameMain();
}

#ifdef _WIN32
int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nShowCmd)
{
    return GameMain();
}

#endif