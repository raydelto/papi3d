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
#include "misc.h"

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
                switch (e.key.keysym.sym)
                {
                case SDLK_w:
                    p.xpos += MUL_FP(p.dx, COS[p.angle % 360]);
                    p.ypos += MUL_FP(p.dy, SIN[p.angle % 360]);
                    break;
                case SDLK_s:
                    p.xpos -= MUL_FP(p.dx, COS[p.angle % 360]);
                    p.ypos -= MUL_FP(p.dy, SIN[p.angle % 360]);
                    break;
                case SDLK_a:
                    p.angle -= 1;
                    p.dx = COS[p.angle % 360];
                    p.dy = SIN[p.angle % 360];
                    p.camX = MUL_FP(COS[(p.angle + 90) % 360], FLOAT_TO_FP(0.7));
                    p.camY = MUL_FP(SIN[(p.angle + 90) % 360], FLOAT_TO_FP(0.7));
                    break;
                case SDLK_d:
                    p.angle += 1;
                    p.dx = COS[p.angle % 360];
                    p.dy = SIN[p.angle % 360];
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