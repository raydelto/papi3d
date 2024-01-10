#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "types.h"
#include "misc.h"

FP COS[360];
FP SIN[360];

void populateTables(void)
{
    u16 i=0;
    while(i<360) {
        COS[i]=FLOAT_TO_FP(cos(i*0.0174533));
        SIN[i]=FLOAT_TO_FP(sin(i*0.0174533));
        i++;
    }
}
