#ifndef Z_HAMMER_PEG_H
#define Z_HAMMER_PEG_H

#include "ultra64.h"
#include "global.h"

struct Ladder;

typedef void (*LadderActionFunc)(struct Ladder*, PlayState*);

typedef enum {
    LADDER_SMALL_WOOD,
    LADDER_MEDIUM_WOOD,
    LADDER_LARGE_WOOD,
    LADDER_SMALL_METAL,
    LADDER_MEDIUM_METAL,
    LADDER_LARGE_METAL
} LadderModel;

typedef enum {
    LADDER_FADING,
    LADDER_FALLING
} LadderType;

typedef struct Ladder {
    DynaPolyActor dyna;
    LadderActionFunc actionFunc;
    ColliderCylinder collider;
    u8 isActivated;
    u8 bounceCount;
    u8 model;
    u8 type;
    s16 timer;
} Ladder;

#endif
