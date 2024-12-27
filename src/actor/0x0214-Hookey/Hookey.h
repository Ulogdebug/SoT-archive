#ifndef __HOOKEY_ACTOR_H__
#define __HOOKEY_ACTOR_H__

#include <uLib.h>

struct Hookey;

typedef void (* HookeyFunc)(struct Hookey*, PlayState*);

typedef struct Hookey {
    DynaPolyActor dyna;
    Actor* torchPos;
    Actor* switchPos;
    HookeyFunc actionFunc;
    u8 sfxFlags;
    s32 waypointId;
    s16 switchFlags;
    s16 searchTimer;
    s8 switchIsOnTop;
    s8 torchIsOnTop;
    s8 reverse;
} Hookey;

#endif // __HOOKEY_ACTOR_H__