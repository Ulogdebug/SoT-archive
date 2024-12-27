#ifndef JUMPTOGGLE_PLATFORM_H
#define JUMPTOGGLE_PLATFORM_H

#include "ultra64.h"
#include "global.h"

#define GET_JUMPTOGGLE_TYPE(this) (this->dyna.actor.params)

typedef enum {
    JUMPTOGGLE_TYPE_START_RED = 0x0000,
    JUMPTOGGLE_TYPE_START_BLUE = 0x0001,
    JUMPTOGGLE_TYPE_BORDER = 0x0002
} JumpTogglePlatformType;

struct JumptogglePlatform;

typedef void (*JumptogglePlatformActionFunc)(struct JumptogglePlatform*, PlayState*);

typedef struct JumptogglePlatform {
    DynaPolyActor dyna;
    JumptogglePlatformActionFunc actionFunc;
    u8 prevPlayerOnGround;
    s16 currentRot;
    s16 targetRot;
} JumptogglePlatform;

#endif
