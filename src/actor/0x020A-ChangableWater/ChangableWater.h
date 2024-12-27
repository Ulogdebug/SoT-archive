#ifndef Z_BG_HAKA_WATER_H
#define Z_BG_HAKA_WATER_H

#include "ultra64.h"
#include "global.h"

struct ChangableWater;

typedef void (*ChangableWaterActionFunc)(struct ChangableWater*, PlayState*);

typedef struct ChangableWater {
    /* 0x0000 */ Actor actor;
    /* 0x014C */ ChangableWaterActionFunc actionFunc;
    /* 0x0150 */ u8 isLowered;
                 u8 waterBoxOrder;
                 u8 reverse;
                 Vec2f scroll[2];
                 s16 switchFlags;
                 f32 waterDepth;
} ChangableWater; // size = 0x0154

#endif
