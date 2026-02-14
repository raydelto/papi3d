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

extern FP gCOS[360];
extern FP gSIN[360];

// Player collision radius in pixels - must be large enough to keep player away from walls
#define PLAYER_RADIUS 12

// Check if a position collides with a wall (with radius buffer)
bool canMoveTo(int pixelX, int pixelY)
{
    // Check all four corners of player bounding box
    int minX = (pixelX - PLAYER_RADIUS) / BOX_X_SIZE;
    int maxX = (pixelX + PLAYER_RADIUS) / BOX_X_SIZE;
    int minY = (pixelY - PLAYER_RADIUS) / BOX_Y_SIZE;
    int maxY = (pixelY + PLAYER_RADIUS) / BOX_Y_SIZE;
    
    // Bounds check - must be fully inside map
    if (minX < 0 || maxX >= MAP_WIDTH || minY < 0 || maxY >= MAP_HEIGHT)
        return false;
    
    // Also check pixel-level bounds to stay away from edges
    if (pixelX - PLAYER_RADIUS < BOX_X_SIZE || pixelX + PLAYER_RADIUS >= (MAP_WIDTH - 1) * BOX_X_SIZE)
        return false;
    if (pixelY - PLAYER_RADIUS < BOX_Y_SIZE || pixelY + PLAYER_RADIUS >= (MAP_HEIGHT - 1) * BOX_Y_SIZE)
        return false;
    
    // Check all tiles the player's bounding box overlaps
    for (int ty = minY; ty <= maxY; ty++)
    {
        for (int tx = minX; tx <= maxX; tx++)
        {
            if (gMap[ty][tx] != 0)
                return false; // Wall found
        }
    }
    return true; // No collision
}

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
        // printf("Camera Plane: %d %f %f\n", p.angle, FP_TO_FLOAT(p.camX), FP_TO_FLOAT(p.camY));
        // printf("Player Direction %d %f %f\n", p.angle, FP_TO_FLOAT(p.dx), FP_TO_FLOAT(p.dy));

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
        SDL_Delay(16 - (start - end));
    }

    SDL_Quit();

    return 0;
}

int main(int argc, char *argv[])
{
    GameMain();
    return 0;
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