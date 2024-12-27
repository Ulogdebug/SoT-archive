#ifndef __HOOK_SPOT_ACTOR_H__
#define __HOOK_SPOT_ACTOR_H__

#include <uLib.h>

struct HookSpot;

typedef void (* HookSpotFunc)(struct HookSpot*, PlayState*);

typedef enum {
    /* 0 */ HAS_SWITCH,
    /* 1 */ NO_SWITCH
} hookSpotType;

typedef struct HookSpot {
    /* 0x0000 */ Actor actor;
                 DynaPolyActor dyna;
                 HookSpotFunc actionFunc;
    /* 0x014C */ ColliderCylinder collider;
                 s16 switchFlags; 
} HookSpot;

#endif // __HOOK_SPOT_ACTOR_H__