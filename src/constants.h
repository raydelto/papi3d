#pragma once
#include "pfloat.h"

constexpr int MAP_HEIGHT = 15;
constexpr int MAP_WIDTH = 20;

constexpr int PER_FRAME_MILLIS_IN_60_FPS = 16;

// Field of view
constexpr float FOV = 0.7f;

constexpr int ANGLE_STEP = 3;

// Player collision radius in pixels - must be large enough to keep player away from walls
constexpr int PLAYER_RADIUS = 12;

extern u8 gMap[MAP_HEIGHT][MAP_WIDTH];
extern FP gCOS[360];
extern FP gSIN[360];

void populateTables();
