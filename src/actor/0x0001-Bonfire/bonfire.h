#ifndef _Z_CUSTOMACTOR_H_
#define _Z_CUSTOMACTOR_H_

#include "ultra64.h"
#include "global.h"

typedef struct Bonfire {
    Actor actor;
    s32 timer;
    s8 superBonfireNearby;
} Bonfire;

#endif
