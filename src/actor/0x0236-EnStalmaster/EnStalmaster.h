#ifndef __EN_STALMASTER_H__
#define __EN_STALMASTER_H__

#include <uLib.h>
#include "object\0x0235-Stalmaster.h"
#include "../0x0247-EnDummyEnemy/EnDummyEnemy.h"
#include "overlays/actors/ovl_Door_Warp1/z_door_warp1.h"

#define SFX_REVERB 1.25f
#define SFX_DIST 1500.0f

#define STALMASTER_HEALTH 55
#define STALMASTER_HEALTH_RATIO 0.7 //STALMASTER_HEALTH multiplied by this is the minimum health for the first phase

struct EnStalmaster;

typedef void (* EnStalmasterFunc)(struct EnStalmaster*, PlayState*);

#define BONE_SPREAD 120.0f //This is used in the setup of the limbs, its used for the spread of bones spawn

#define STALMASTER_MAX_EFFECT 13
#define TAIL_NUMBER 10
#define TAIL_LENGTH 5.0f

typedef enum {
    STALMASTER_CUTSCENE_INTRO_WAIT,
    STALMASTER_CUTSCENE_INTRO_OPENING,
} StalmasterIntroCutsceneStates;

typedef enum {
    STALMASTER_CUTSCENE_THREAT_WAIT,
    STALMASTER_CUTSCENE_THREAT_DO,
} StalmasterThreatCutsceneStates;

typedef enum {
    STALMASTER_CUTSCENE_DIE_WAIT,
    STALMASTER_CUTSCENE_DIE_DO,
} StalmasterDeathCutsceneStates;

typedef enum {
    STALMASTER_EFFECT_NONE,
    STALMASTER_EFFECT_HELMET,
    STALMASTER_EFFECT_TAIL,
    STALMASTER_EFFECT_TIP,
    STALMASTER_EFFECT_PLATE,
} StalmasterEffectType;

typedef struct {
    u8 gravity;
    u8 onGround;
    u8 bounceCounter;
    u8 type;
    u8 csHandled;
    u8 invisible;
    Vec3f velocity;
    Vec3f skeletonPos;
    Vec3f pos;
    Vec3s rot;
    Vec3s skeletonRot;
} StalmasterParts;

typedef struct EnStalmaster {
    Actor actor;
    EnStalmasterFunc actionFunc;

    SkelAnime skelAnime;
    Vec3s jointTable[STALMASTER_LIMB_MAX];
    Vec3s morphTable[STALMASTER_LIMB_MAX];

    u8 helmetOff;
    u8 plateOff;
    EnDummyEnemy* enemyDummy;

    u8 hasCS;
    u8 csState;
    s32 csTimer;
    s16 subCamId;
    Vec3f subCamEye;
    Vec3f subCamAt;
    f32 subCamFov;
    s16 switchFlags;
    Vec3f headHandPos;
    Vec3f tailPos[4];
    Vec3s headRot;
    f32 torsoRotX;
    f32 torsoRotZ;
    u8 shouldUpdateHead;
    u8 commonTimer;
    u8 aggressionTimer;
    u8 dmgFogEffectTimer;
    
    u8 swordAActive;
    u8 swordBActive;
    u8 swordCActive;
    u8 swordDActive;
    u8 armBlowed;

    u8 inSecondPhase;
    u8 hasTakenWeaponry;

    s32 effectIndex;
    s32 effectIndex1;
    s32 effectIndex2;
    s32 effectIndex3;

    ColliderJntSph colliderSwordUpLeft;
	ColliderJntSphElement collidersSwordUpLeft[2];

	ColliderJntSph colliderSwordUpRight;
	ColliderJntSphElement collidersSwordUpRight[2];

    ColliderJntSph colliderSwordDownLeft;
	ColliderJntSphElement collidersSwordDownLeft[2];

	ColliderJntSph colliderSwordDownRight;
	ColliderJntSphElement collidersSwordDownRight[2];

    ColliderCylinder bodyCollider;

    Particle* particleTail;
	Chain*    chainTail;
    int       numParticleTail;
	int       numTail;

    StalmasterParts sStalmasterParts[STALMASTER_LIMB_MAX];
    StalmasterParts sStalmasterEffects[STALMASTER_MAX_EFFECT];
} EnStalmaster;

#endif // __EN_STALMASTER_H__