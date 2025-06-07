#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "types.h"
#include "player.h"
#include "graphics.h"
#include "misc.h"

extern FP COS[360];
extern FP SIN[360];

void initPlayer(PLAYER *p)
{
    p->dx = FLOAT_TO_FP(1);
    p->dy = FLOAT_TO_FP(0);
    p->xpos = FLOAT_TO_FP(100);
    p->ypos = FLOAT_TO_FP(300);
    p->camX = 0;
    p->camY = FLOAT_TO_FP(0.7);
    p->angle = 0;
}

void drawPlayer(SDL_Surface *surface, PLAYER *p)
{
    s8 i = 0;

    while (i < 10)
    {
        drawHorLine(surface, FP_TO_INT(p->xpos) - 5, FP_TO_INT(p->xpos) + 5, FP_TO_INT(p->ypos) + (5 - i), 0x00FF0000);
        i++;
    }
}

void drawRays(SDL_Surface *surface, PLAYER *p, u8 m[15][20], u8 view)
{
    u8 hit = 0, side = 0;
    FP stepX, stepY;
    FP tileDeltaX, tileDeltaY;
    FP lengthX, lengthY;
    FP tileX = 0, tileY = 0;                             // Tile coordinate of player position
    FP pPosX = DIV_FP(p->xpos, FLOAT_TO_FP(BOX_X_SIZE)); // Exact position within tile
    FP pPosY = DIV_FP(p->ypos, FLOAT_TO_FP(BOX_Y_SIZE));
    FP cPlaneX = p->camX, cPlaneY = p->camY;
    FP rDirX, pDirX = p->dx;
    FP rDirY, pDirY = p->dy;
    FP rayDistance;
    FP cDist;
    FP lHeight; // Line height for 3D rendering
    s32 lStart, lEnd;
    FP R, G, B;

    u16 i = 0;
    while (i < SCREEN_WIDTH)
    {
        tileX = FLOAT_TO_FP(FP_TO_INT(p->xpos) / BOX_X_SIZE);
        tileY = FLOAT_TO_FP(FP_TO_INT(p->ypos) / BOX_Y_SIZE);

        cDist = 2 * DIV_FP(FLOAT_TO_FP(i), FLOAT_TO_FP(SCREEN_WIDTH - 1)) - FLOAT_TO_FP(1); // Calculate length along the camera plane for ray intersection

        rDirX = pDirX + MUL_FP(cPlaneX, cDist);
        rDirY = pDirY + MUL_FP(cPlaneY, cDist);

        // Prevent division by Zero when calculating the deltas.
        rDirX = rDirX == 0 ? FLOAT_TO_FP(0.001) : rDirX;
        rDirY = rDirY == 0 ? FLOAT_TO_FP(0.001) : rDirY;

        tileDeltaX = abs(DIV_FP(FLOAT_TO_FP(1), rDirX));
        tileDeltaY = abs(DIV_FP(FLOAT_TO_FP(1), rDirY));

        if (rDirX < 0)
        {
            stepX = FLOAT_TO_FP(-1);
            lengthX = MUL_FP(pPosX - tileX, tileDeltaX);
        }
        else
        {
            stepX = FLOAT_TO_FP(1);
            lengthX = MUL_FP(FLOAT_TO_FP(1) + tileX - pPosX, tileDeltaX);
        }

        if (rDirY < 0)
        {
            stepY = FLOAT_TO_FP(-1);
            lengthY = MUL_FP(pPosY - tileY, tileDeltaY);
        }
        else
        {
            stepY = FLOAT_TO_FP(1);
            lengthY = MUL_FP(FLOAT_TO_FP(1) + tileY - pPosY, tileDeltaY);
        }

        hit = 0;
        while (!hit)
        {
            if (lengthX < lengthY)
            {
                rayDistance = lengthX;
                lengthX += tileDeltaX;
                tileX += stepX;
                side = 0;
            }
            else
            {
                rayDistance = lengthY;
                lengthY += tileDeltaY;
                tileY += stepY;
                side = 1;
            }

            if (m[FP_TO_INT(tileY)][FP_TO_INT(tileX)] != 0)
                hit = 1;
        }

        if(rayDistance == 0)
        {
            rayDistance = FLOAT_TO_FP(0.001); // Prevent division by zero
        }

        if (view)
        {
            drawLine(surface, FP_TO_INT(p->xpos), FP_TO_INT(p->ypos),
                     (FP_TO_INT(p->xpos) + FP_TO_INT(MUL_FP(rDirX, rayDistance * BOX_X_SIZE))),
                     (FP_TO_INT(p->ypos) + FP_TO_INT(MUL_FP(rDirY, rayDistance * BOX_Y_SIZE))),
                     0x00FFFF00);
        }
        else
        {
            lHeight = DIV_FP(FLOAT_TO_FP(SCREEN_HEIGHT), rayDistance);
            lStart = FP_TO_INT(DIV_FP(-lHeight, FLOAT_TO_FP(2)) + DIV_FP(FLOAT_TO_FP(SCREEN_HEIGHT), FLOAT_TO_FP(2)));
            lEnd = FP_TO_INT(DIV_FP(lHeight, FLOAT_TO_FP(2)) + DIV_FP(FLOAT_TO_FP(SCREEN_HEIGHT), FLOAT_TO_FP(2)));
            if (lStart < 0)
                lStart = 0;
            if (lEnd >= SCREEN_HEIGHT - 1)
                lEnd = SCREEN_HEIGHT - 1;

            R = FLOAT_TO_FP(0x1FF);
            G = FLOAT_TO_FP(0x1FF);
            B = FLOAT_TO_FP(0x1FF);
            R = DIV_FP(R, rayDistance);
            G = DIV_FP(G, rayDistance);
            B = DIV_FP(B, rayDistance);

            if (FP_TO_INT(R) > 0xFF)
                R = FLOAT_TO_FP(0xFF);
            if (FP_TO_INT(G) > 0xFF)
                G = FLOAT_TO_FP(0xFF);
            if (FP_TO_INT(B) > 0xFF)
                B = FLOAT_TO_FP(0xFF);

            drawVertLine(surface, i, lStart, lEnd, (FP_TO_INT(R) << 16) + (FP_TO_INT(G) << 8) + FP_TO_INT(B));
        }
        i++;
    }
}

void drawFloorCeiling(SDL_Surface *surface)
{
    u32 y = 0;

    while (y < SCREEN_HEIGHT / 2)
    {
        drawHorLine(surface, 0, SCREEN_WIDTH - 1, y, 0x007F7F7F);
        y++;
    }

    while (y < SCREEN_HEIGHT)
    {
        drawHorLine(surface, 0, SCREEN_WIDTH - 1, y, 0x003F3F3F);
        y++;
    }
}
