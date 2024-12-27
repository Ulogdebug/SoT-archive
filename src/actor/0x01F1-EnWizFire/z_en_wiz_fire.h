#ifndef Z_EN_WIZ_FIRE_H
#define Z_EN_WIZ_FIRE_H

#include "ultra64.h"
#include "global.h"

typedef enum {
    /* 0 */ EN_WIZ_FIRE_TYPE_MAGIC_PROJECTILE,
    /* 1 */ EN_WIZ_FIRE_TYPE_ARCING_MAGIC_PROJECTILE,
    /* 2 */ EN_WIZ_FIRE_TYPE_SMALL_FLAME,
    /* 3 */ EN_WIZ_FIRE_TYPE_REFLECTED_MAGIC_PROJECTILE,
    /* 4 */ EN_WIZ_FIRE_TYPE_ICE_MAGIC_PROJECTILE,
    /* 5 */ EN_WIZ_FIRE_TYPE_ELEC_MAGIC_PROJECTILE,
} EnWizFireType;

#define EN_WIZ_FIRE_GET_TYPE(thisx) ((thisx)->params)

struct EnWizFire;


typedef void (*EnWizActionFunc)(struct EnWiz*, PlayState*);

struct EnWiz;

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

typedef void (*EnWizFireActionFunc)(struct EnWizFire*, PlayState*);

typedef struct {
    /* 0x00 */ u8 isEnabled;
    /* 0x01 */ u8 smokeScroll;
    /* 0x06 */ s16 alpha;
    /* 0x08 */ s16 shouldDecreaseAlpha;
    /* 0x0C */ f32 scale;
    /* 0x10 */ Vec3f pos;
    /* 0x1C */ Vec3f velocity;
    /* 0x28 */ Vec3f accel;
} EnWizFireEffect; // size = 0x34

typedef struct EnWizFire {
    /* 0x0000 */ Actor actor;
    /* 0x0144 */ EnWizFireActionFunc actionFunc;
    /* 0x0148 */ u8 hitByIceArrow;
    /* 0x014C */ f32 scale;
    /* 0x0150 */ f32 targetScale;
    /* 0x0154 */ Vec3f scaleMod;
    /* 0x0160 */ s16 action;
    /* 0x0162 */ s16 type;
    /* 0x0164 */ union {
                    u8 shouldPoolFadeOut;
                    u8 hasSpawnedSmallFlame;
                };
    /* 0x0166 */ s16 isIceType;
    /* 0x0168 */ s16 timer;
    /* 0x016A */ union {
                    s16 poolTimer;
                    s16 increaseLowestUsedIndexTimer;
                };
    /* 0x016C */ s16 steamSpawnTimer;
    /* 0x016E */ s16 lowestUsedIndex;
    /* 0x0170 */ s16 smallFlameScroll;
    /* 0x0172 */ s16 wallCheckTimer; // The projectile won't check for impacts with walls until this is zero
    /* 0x0174 */ s8 playerHitByIceProjectile; // Set, but never used
    /* 0x0178 */ Vec3f magicProjectilePos[10];
    /* 0x01F0 */ f32 poolScale;
    /* 0x01F4 */ f32 bigFlameScale;
    /* 0x01F8 */ f32 fireSmokeScale;
    /* 0x01FC */ f32 alpha;
    /* 0x0200 */ f32 blendScale;
    /* 0x0204 */ f32 blendScaleFrac;
    /* 0x0208 */ ColliderCylinder collider;
    /* 0x0254 */ EnWizFireEffect effects[50];
                 Vec3f ZeroVec3f;
                 s16 isElectricType;
                 s16 isBlueType;
} EnWizFire; // size = 0x2AF4

#endif // Z_EN_WIZ_FIRE_H
