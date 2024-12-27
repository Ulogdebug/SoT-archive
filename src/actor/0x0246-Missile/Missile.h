#ifndef __MISSILE_H__
#define __MISSILE_H__

#include <uLib.h>

struct Missile;

typedef void (* MissileFunc)(struct Missile*, PlayState*);

typedef struct Missile {
    Actor actor;
    ColliderQuad hitbox;
    MissileFunc actionFunc;
    WeaponInfo missileInfo;
    s16 timer;
    s16 beginChaseTimer;
    s16 yaw;
    s16 yawSpeed;
    s32 effectIndex;
    Vec3f targetRot;
    s16 pitch;
} Missile;

#endif // __MISSILE_H__