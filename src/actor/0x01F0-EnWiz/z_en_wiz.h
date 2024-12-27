#ifndef Z_EN_WIZ_H
#define Z_EN_WIZ_H

#include "ultra64.h"
#include "global.h"

struct EnWiz;

#define ARMATURE_LOWERBODY_POS_LIMB 0
#define ARMATURE_BONE_ROT_LIMB 1
#define ARMATURE_UPPERBODY_LIMB 2
#define ARMATURE_HEAD_LIMB 3
#define ARMATURE_LOWERJAW_LIMB 4
#define ARMATURE_COLLAR_L_LIMB 5
#define ARMATURE_UPPERARM_L_LIMB 6
#define ARMATURE_FOREARM_L_LIMB 7
#define ARMATURE_COLLAR_R_LIMB 8
#define ARMATURE_UPPERARM_R_LIMB 9
#define ARMATURE_FOREARM_R_LIMB 10
#define ARMATURE_STAFF_R_LIMB 11
#define ARMATURE_NUM_LIMBS 12


typedef void (*EnWizActionFunc)(struct EnWiz*, PlayState*);

typedef enum {
    /* 0 */ ENWIZ_TYPE_MASTER,    // master wizzrobe
    /* 1 */ ENWIZ_TYPE_FIRE,      // fire wizzrobe
    /* 2 */ ENWIZ_TYPE_ICE,       // ice wizzrobe
    /* 3 */ ENWIZ_TYPE_ELECTRIC,  // electric wizzrobe
    /* 4 */ ENWIZ_TYPE_NORMAL,    // normal wizzrobe
} EnWizType;

typedef struct EnWiz {
    Actor actor;
    SkelAnime skelAnime;
    EnWizActionFunc actionFunc;
    ColliderCylinder collider;
    s16 type;
    s16 alpha;
    s16 timer;
    Actor* spawnedActor[2];
    u8 spawnCount[2];
    u8 enemySpawnSet;
    s16 hasActiveProjectile;
    s8 dropboomerang;
    Vec3f staffFlamePos;
    u8 castMagicType;
    Color_RGBA8 castColor;
    Color_RGBA8 typeColor;
    u8 EnemyAnim;
    u8 spawnIceTimer;
    s16 teleportTimer;
    f32 floatPosY;
    Vec3s bodyPartsPos[10];
    Vec3s jointTable[12];
    Vec3s morphTable[12];
} EnWiz;

#endif
