#ifndef __FAKE_WALL_H__
#define __FAKE_WALL_H__

#include <uLib.h>

struct FakeWall;

typedef void (* FakeWallFunc)(struct FakeWall*, PlayState*);

typedef struct FakeWall {
    union {
        Actor actor;
        DynaPolyActor dyna;
    };
} FakeWall;

#endif // __FAKE_WALL_H__