#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "map.h"
#include "graphics.h"

void drawMap(SDL_Surface *surface, u8 m[15][20])
{
    u32 i = 0;
    u32 j = 0;
    u32 x = 0;
    u32 y = 0;

    while (i < SCREEN_HEIGHT / BOX_Y_SIZE)
    {
        j = 0;
        x = 0;
        while (j < SCREEN_WIDTH / BOX_X_SIZE)
        {
            if (m[i][j])
                drawBox(surface, x, y);
            x += BOX_X_SIZE;
            j++;
        }
        y += BOX_Y_SIZE;
        i++;
    }
}
