#ifndef __CASTLE_PLATFORM_ACTOR_H__
#define __CASTLE_PLATFORM_ACTOR_H__

#include <uLib.h>

struct CastlePlatform;

typedef void (* CastlePlatformFunc)(struct CastlePlatform*, PlayState*);

typedef struct CastlePlatform {
    DynaPolyActor dyna;
    CastlePlatformFunc actionFunc;
    Actor* torchPos;
    Actor* switchPos;
    u8 sfxFlags;
    s32 waypointId;
    s16 switchFlags;
    s16 searchTimer;
    s8 switchIsOnTop;
    s8 torchIsOnTop;
    s8 reverse;
} CastlePlatform;

#endif // __CASTLE_PLATFORM_ACTOR_H__