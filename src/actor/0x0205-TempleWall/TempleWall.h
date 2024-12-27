#ifndef __TEMPLE_WALL_ACTOR_H__
#define __TEMPLE_WALL_ACTOR_H__

#include <uLib.h>

struct TempleWall;

typedef void (* TempleWallFunc)(struct TempleWall*, PlayState*);

typedef struct TempleWall {
    DynaPolyActor dyna;
    TempleWallFunc actionFunc;
    s16 switchFlags;
    s16 shakeTimer;
    Vec3s randFactor;
} TempleWall;

#endif // __TEMPLE_WALL_ACTOR_H__