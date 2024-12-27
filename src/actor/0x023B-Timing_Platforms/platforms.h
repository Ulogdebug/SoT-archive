#ifndef Z_PLATFORMS_H
#define Z_PLATFORMS_H

#include "ultra64.h"
#include "global.h"

struct Platforms;

typedef void (*PlatformsActionFunc)(struct Platforms*, PlayState*);

typedef struct Platforms {
    union {
        Actor actor;
        DynaPolyActor dyna;
    };
    PlatformsActionFunc actionFunc;
    f32 dipOffset;
    s16 timer;
    s16 alphaTarget;
    s16 alpha;
    u8 playerOn;
    s16 preTimerDuration;
} Platforms;

#endif