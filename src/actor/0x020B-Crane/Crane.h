#ifndef __CRANE_ACTOR_H__
#define __CRANE_ACTOR_H__

#include <uLib.h>

struct Crane;

typedef void (* CraneFunc)(struct Crane*, PlayState*);

typedef struct Crane {
    DynaPolyActor dyna;
    CraneFunc actionFunc;
    s16 switchFlag;
    s16 waitTimer;
    s16 moveDis;
    u8 backHomeFlag;
    f32 vecFactor;
    u8 switchIsOnTop;
} Crane;

#endif // __CRANE_ACTOR_H__