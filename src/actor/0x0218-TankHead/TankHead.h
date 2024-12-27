#ifndef __TANK_HEAD_H__
#define __TANK_HEAD_H__

#include <uLib.h>

struct TankHead;

typedef void (* TankHeadFunc)(struct TankHead*, PlayState*);

typedef struct TankHead {
    Actor actor;
    SkelAnime skelAnime;
    Vec3s jointTable[TANKHEADOBJ_LIMB_MAX];
    Vec3s morphTable[TANKHEADOBJ_LIMB_MAX];
    TankHeadFunc actionFunc;
    ColliderCylinder hitbox;
    Vec3s headRot;
    Vec3s twinInnerGunRot;
    Vec3f twinInnerGunPos[2];
    Vec3f twinOutGunPos[2];
    Vec3f smokeLauncherPosL[3];
    Vec3f smokeLauncherPosR[3];
    u8 meleeAttacking;
    ColliderJntSph gunCollider;
    ColliderJntSphElement gunColliderItems[13];
    s16 effectIndex;
    s16 pitch;
    s16 switchFlags;
    u8 madState;
    s16 fireTimer;
    u8 whichEyes;
    s16 randOffset;
    s16 headYawToPlayer;
    u8 findPlayer;
    s16 findPlayerTimer;
    Color_RGBA8 bombSmokePrimColor;
    Color_RGBA8 bombSmokeEnvColor;
} TankHead;

#endif // __TANK_HEAD_H__