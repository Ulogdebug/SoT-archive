#ifndef __SUPER_BONFIRE_ACTOR_H__
#define __SUPER_BONFIRE_ACTOR_H__

#include <uLib.h>

struct SuperBonfire;

typedef void (* SuperBonfireFunc)(struct SuperBonfire*, PlayState*);

typedef struct SuperBonfire {
    Actor actor;
    DynaPolyActor dyna;
    ColliderCylinder collider;
    SuperBonfireFunc actionFunc;
    s16 switchFlags;
    s16 healTimer;
    s16 cutsceneTimer;
    s16 whichBonfire;
    s16 textState;
    u8 shouldLight;
    s8 spawnFlameFlag;
    s8 toSecFlag;
} SuperBonfire;

#endif // __SUPER_BONFIRE_ACTOR_H__