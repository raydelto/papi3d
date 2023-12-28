#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "types.h"
#include "graphics.h"
#include "map.h"
#include "player.h"
#include "misc.h"

u8 map [15][20] = {
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
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

extern FP COS[360];
extern FP SIN[360];

s16 main(void)
{
	u8 frameComplete=0;
	s16 quit=0;
	SDL_Window *window=NULL;
	SDL_Surface *surface;
	SDL_Event e;
    PLAYER p;
    u32 start, end;

    populateTables();

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Papi's Wolf3D", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	surface = SDL_GetWindowSurface(window);

    initPlayer(&p);

	while(!quit) {
        while(SDL_PollEvent(&e)) {		
            if(e.type==SDL_QUIT) {
                quit=1;
            }

			if(e.type==SDL_KEYDOWN) {
                switch(e.key.keysym.sym) {
                    case SDLK_w:

                        break;
                    case SDLK_s:

                        break;
                    case SDLK_a:
                        p.angle+=1;
                        if(p.angle>=360)
                            p.angle=0;
                        break;
                    case SDLK_d:
                        p.angle-=1;
                        if(p.angle>=360)
                            p.angle=359;
                        break;
                    default:
                        break;
                }			
			}
        }

        clearSreen(surface);
 
        start=SDL_GetTicks();
        //drawMap(surface, map);
        //drawPlayer(surface, &p);  
        drawFloorCeiling(surface); 
        drawRays(surface, &p, map);
        end=SDL_GetTicks();
        SDL_UpdateWindowSurface(window);
        SDL_Delay(16-(start-end));
        //printf("%d\n", end-start);        
	}

	SDL_Quit();

	return 0;
}

