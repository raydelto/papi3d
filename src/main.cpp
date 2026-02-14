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

u8 map[15][20] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

extern FP COS[360];
extern FP SIN[360];

// Player collision radius in pixels - must be large enough to keep player away from walls
#define PLAYER_RADIUS 12

// Check if a position collides with a wall (with radius buffer)
int canMoveTo(int pixelX, int pixelY, u8 m[15][20])
{
    // Check all four corners of player bounding box
    int minX = (pixelX - PLAYER_RADIUS) / BOX_X_SIZE;
    int maxX = (pixelX + PLAYER_RADIUS) / BOX_X_SIZE;
    int minY = (pixelY - PLAYER_RADIUS) / BOX_Y_SIZE;
    int maxY = (pixelY + PLAYER_RADIUS) / BOX_Y_SIZE;
    
    // Bounds check - must be fully inside map
    if (minX < 0 || maxX >= 20 || minY < 0 || maxY >= 15)
        return 0;
    
    // Also check pixel-level bounds to stay away from edges
    if (pixelX - PLAYER_RADIUS < BOX_X_SIZE || pixelX + PLAYER_RADIUS >= 19 * BOX_X_SIZE)
        return 0;
    if (pixelY - PLAYER_RADIUS < BOX_Y_SIZE || pixelY + PLAYER_RADIUS >= 14 * BOX_Y_SIZE)
        return 0;
    
    // Check all tiles the player's bounding box overlaps
    for (int ty = minY; ty <= maxY; ty++)
    {
        for (int tx = minX; tx <= maxX; tx++)
        {
            if (m[ty][tx] != 0)
                return 0; // Wall found
        }
    }
    return 1; // No collision
}

int GameMain()
{
    u8 view = 0;
    s16 quit = 0;
    SDL_Window *window = nullptr;
    SDL_Surface *surface;
    SDL_Event e;
    PLAYER p;
    u32 start, end;

    populateTables();

    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Papi's Wolf3D", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    surface = SDL_GetWindowSurface(window);

    initPlayer(&p);

    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = 1;
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
                    if (canMoveTo(FP_TO_INT(newX), FP_TO_INT(newY), map))
                    {
                        p.xpos = newX;
                        p.ypos = newY;
                    }
                    // Try sliding along X axis only
                    else if (canMoveTo(FP_TO_INT(newX), FP_TO_INT(p.ypos), map))
                    {
                        p.xpos = newX;
                    }
                    // Try sliding along Y axis only
                    else if (canMoveTo(FP_TO_INT(p.xpos), FP_TO_INT(newY), map))
                    {
                        p.ypos = newY;
                    }
                    break;
                case SDLK_s: // Move backward (opposite of facing direction)
                    newX = p.xpos - MUL_FP(p.dx, moveSpeed);
                    newY = p.ypos - MUL_FP(p.dy, moveSpeed);
                    if (canMoveTo(FP_TO_INT(newX), FP_TO_INT(newY), map))
                    {
                        p.xpos = newX;
                        p.ypos = newY;
                    }
                    else if (canMoveTo(FP_TO_INT(newX), FP_TO_INT(p.ypos), map))
                    {
                        p.xpos = newX;
                    }
                    else if (canMoveTo(FP_TO_INT(p.xpos), FP_TO_INT(newY), map))
                    {
                        p.ypos = newY;
                    }
                    break;
                case SDLK_a: // Strafe left (perpendicular to facing direction)
                    newX = p.xpos + MUL_FP(p.dy, moveSpeed);
                    newY = p.ypos - MUL_FP(p.dx, moveSpeed);
                    if (canMoveTo(FP_TO_INT(newX), FP_TO_INT(newY), map))
                    {
                        p.xpos = newX;
                        p.ypos = newY;
                    }
                    else if (canMoveTo(FP_TO_INT(newX), FP_TO_INT(p.ypos), map))
                    {
                        p.xpos = newX;
                    }
                    else if (canMoveTo(FP_TO_INT(p.xpos), FP_TO_INT(newY), map))
                    {
                        p.ypos = newY;
                    }
                    break;
                case SDLK_d: // Strafe right (perpendicular to facing direction)
                    newX = p.xpos - MUL_FP(p.dy, moveSpeed);
                    newY = p.ypos + MUL_FP(p.dx, moveSpeed);
                    if (canMoveTo(FP_TO_INT(newX), FP_TO_INT(newY), map))
                    {
                        p.xpos = newX;
                        p.ypos = newY;
                    }
                    else if (canMoveTo(FP_TO_INT(newX), FP_TO_INT(p.ypos), map))
                    {
                        p.xpos = newX;
                    }
                    else if (canMoveTo(FP_TO_INT(p.xpos), FP_TO_INT(newY), map))
                    {
                        p.ypos = newY;
                    }
                    break;
                case SDLK_LEFT: // Rotate left
                    p.angle = (p.angle - 3 + 360) % 360;
                    p.dx = COS[p.angle];
                    p.dy = SIN[p.angle];
                    p.camX = MUL_FP(COS[(p.angle + 90) % 360], FLOAT_TO_FP(0.7));
                    p.camY = MUL_FP(SIN[(p.angle + 90) % 360], FLOAT_TO_FP(0.7));
                    break;
                case SDLK_RIGHT: // Rotate right
                    p.angle = (p.angle + 3) % 360;
                    p.dx = COS[p.angle];
                    p.dy = SIN[p.angle];
                    p.camX = MUL_FP(COS[(p.angle + 90) % 360], FLOAT_TO_FP(0.7));
                    p.camY = MUL_FP(SIN[(p.angle + 90) % 360], FLOAT_TO_FP(0.7));
                    break;
                case SDLK_e:
                    view ^= 0x01;
                    break;
                case SDLK_q:
                    quit = 1;
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
            drawMap(surface, map);
            drawPlayer(surface, &p);
        }
        else
        {
            drawFloorCeiling(surface);
        }
        drawRays(surface, &p, map, view);

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