/*
 * File: z_en_baguo.c
 * Overlay: ovl_En_Baguo
 * Description: Nejiron!
 */

#include "z_en_baguo.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h"
#include "vt.h"
#include <uLib.h>

#define FLAGS (ACTOR_FLAG_0 | ACTOR_FLAG_2 | ACTOR_FLAG_4 | ACTOR_FLAG_27)
#define ABS_ALT(x) ((x) < 0 ? -(x) : (x))

void Nejiron_Init(Actor* thisx, PlayState* globalCtx);
void Nejiron_Destroy(Actor* thisx, PlayState* globalCtx);
void Nejiron_Update(Actor* thisx, PlayState* globalCtx);

void Nejiron_UndergroundIdle(Nejiron* this, PlayState* globalCtx);
void Nejiron_EmergeFromUnderground(Nejiron* this, PlayState* globalCtx);
void Nejiron_Idle(Nejiron* this, PlayState* globalCtx);
void Nejiron_Roll(Nejiron* this, PlayState* globalCtx);
void Nejiron_SetupRetreatUnderground(Nejiron* this);
void Nejiron_RetreatUnderground(Nejiron* this, PlayState* globalCtx);
void Nejiron_DrawBody(Actor* thisx, PlayState* globalCtx);
void Nejiron_InitializeParticle(Nejiron* this, Vec3f* position, Vec3f* velocity, Vec3f* acceleration, f32 scale,
                                s16 timer);
void Nejiron_UpdateParticles(Nejiron* this, PlayState* globalCtx);
void Nejiron_DrawRockParticles(Nejiron* this, PlayState* globalCtx);

typedef enum {
    /* 0x0 */ NEJIRON_ACTION_INACTIVE,   // The Nejiron is either underground or emerging from underground
    /* 0x1 */ NEJIRON_ACTION_ACTIVE,     // The Nejiron is above ground and actively chasing the player
    /* 0x2 */ NEJIRON_ACTION_RETREATING, // The Nejiron is burrowing back underground
    /* 0x3 */ NEJIRON_ACTION_EXPLODING   // The Nejiron has detonated
} NejironAction;

/**
 * These directions are relative to the Nejiron.
 */
typedef enum {
    /* 0x0 */ NEJIRON_DIRECTION_RIGHT,
    /* 0x1 */ NEJIRON_DIRECTION_LEFT
} NejironRollDirection;

const ActorInit En_Lightbox_InitVars = {
    0x305,
    ACTORCAT_ENEMY,
    FLAGS,
    0x07A,
    sizeof(Nejiron),
    (ActorFunc)Nejiron_Init,
    (ActorFunc)Nejiron_Destroy,
    (ActorFunc)Nejiron_Update,
    (ActorFunc)NULL,
};

static ColliderJntSphElementInit sJntSphElementsInit[1] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0xFFCFFFFF, 0x04, 0x04 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { 1, { { 0, 0, 0 }, 0 }, 1 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COLTYPE_HARD,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    1,
    sJntSphElementsInit,
};

typedef enum {
    /* 0x0 */ NEJIRON_DMGEFF_NONE,      // Does not interact with the Nejiron at all
    /* 0xE */ NEJIRON_DMGEFF_KILL = 14, // Kills and detonates the Nejiron
    /* 0xF */ NEJIRON_DMGEFF_RECOIL     // Deals no damage, but displays the appropriate hit mark and recoil animation
} NejironDamageEffect;

static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, NEJIRON_DMGEFF_RECOIL),
    /* Deku stick    */ DMG_ENTRY(0, NEJIRON_DMGEFF_RECOIL),
    /* Slingshot     */ DMG_ENTRY(0, NEJIRON_DMGEFF_RECOIL),
    /* Explosive     */ DMG_ENTRY(1, NEJIRON_DMGEFF_KILL),
    /* Boomerang     */ DMG_ENTRY(3, NEJIRON_DMGEFF_KILL),
    /* Normal arrow  */ DMG_ENTRY(0, NEJIRON_DMGEFF_RECOIL),
    /* Hammer swing  */ DMG_ENTRY(2, NEJIRON_DMGEFF_KILL),
    /* Hookshot      */ DMG_ENTRY(3, NEJIRON_DMGEFF_KILL),
    /* Kokiri sword  */ DMG_ENTRY(1, NEJIRON_DMGEFF_KILL),
    /* Master sword  */ DMG_ENTRY(2, NEJIRON_DMGEFF_KILL),
    /* Giant's Knife */ DMG_ENTRY(4, NEJIRON_DMGEFF_KILL),
    /* Fire arrow    */ DMG_ENTRY(0, NEJIRON_DMGEFF_RECOIL),
    /* Ice arrow     */ DMG_ENTRY(0, NEJIRON_DMGEFF_RECOIL),
    /* Light arrow   */ DMG_ENTRY(1, NEJIRON_DMGEFF_KILL),
    /* Unk arrow 1   */ DMG_ENTRY(0, NEJIRON_DMGEFF_NONE),
    /* Unk arrow 2   */ DMG_ENTRY(0, NEJIRON_DMGEFF_NONE),
    /* Unk arrow 3   */ DMG_ENTRY(0, NEJIRON_DMGEFF_NONE),
    /* Fire magic    */ DMG_ENTRY(0, NEJIRON_DMGEFF_RECOIL),
    /* Ice magic     */ DMG_ENTRY(0, NEJIRON_DMGEFF_RECOIL),
    /* Light magic   */ DMG_ENTRY(1, NEJIRON_DMGEFF_KILL),
    /* Shield        */ DMG_ENTRY(0, NEJIRON_DMGEFF_NONE),
    /* Mirror Ray    */ DMG_ENTRY(0, NEJIRON_DMGEFF_NONE),
    /* Kokiri spin   */ DMG_ENTRY(1, NEJIRON_DMGEFF_KILL),
    /* Giant spin    */ DMG_ENTRY(4, NEJIRON_DMGEFF_KILL),
    /* Master spin   */ DMG_ENTRY(2, NEJIRON_DMGEFF_KILL),
    /* Kokiri jump   */ DMG_ENTRY(2, NEJIRON_DMGEFF_KILL),
    /* Giant jump    */ DMG_ENTRY(8, NEJIRON_DMGEFF_KILL),
    /* Master jump   */ DMG_ENTRY(4, NEJIRON_DMGEFF_KILL),
    /* Unknown 1     */ DMG_ENTRY(0, NEJIRON_DMGEFF_NONE),
    /* Unblockable   */ DMG_ENTRY(0, NEJIRON_DMGEFF_NONE),
    /* Hammer jump   */ DMG_ENTRY(4, NEJIRON_DMGEFF_KILL),
    /* Unknown 2     */ DMG_ENTRY(0, NEJIRON_DMGEFF_NONE),
};

void Nejiron_Init(Actor* thisx, PlayState* globalCtx) {
    Nejiron* this = (Nejiron*)thisx;

    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 0.0f);
    SkelAnime_Init(globalCtx, &this->skelAnime, 0x060020e8, NULL, this->jointTable, this->morphTable, 3);
    this->actor.naviEnemyId = NAVI_ENEMY_UNUSED_38;
    this->maxDistanceFromHome = 240.0f;
    this->maxDistanceFromHome += this->actor.world.rot.z * 40.0f;
    this->actor.world.rot.z = 0;
    Actor_SetScale(&this->actor, 0.01f);
    this->actor.colChkInfo.mass = MASS_IMMOVABLE;
    this->actor.targetMode = 3;
    this->actor.targetArrowOffset = 2000;
    Collider_InitJntSph(globalCtx, &this->collider);
    Collider_SetJntSph(globalCtx, &this->collider, &this->actor, &sJntSphInit, this->colliderElements);
    this->collider.elements[0].dim.modelSphere.radius = 30;
    this->collider.elements[0].dim.scale = 1.0f;
    this->collider.elements[0].dim.modelSphere.center.x = 80;
    this->collider.elements[0].dim.modelSphere.center.y = 80;
    this->collider.elements[0].dim.modelSphere.center.z = 0;
    this->actor.shape.yOffset = -3000.0f;
    this->actor.gravity = -3.0f;
    this->actor.colChkInfo.damageTable = &sDamageTable;
    this->actor.flags |= ACTOR_FLAG_27;
    this->actor.flags &= ~(ACTOR_FLAG_0 | ACTOR_FLAG_2);
    this->collider.base.acFlags |= AC_HARD;
    this->actionFunc = Nejiron_UndergroundIdle;
}

void Nejiron_Destroy(Actor* thisx, PlayState* globalCtx) {
    Nejiron* this = (Nejiron*)thisx;

    Collider_DestroyJntSph(globalCtx, &this->collider);
}

void Nejiron_UndergroundIdle(Nejiron* this, PlayState* globalCtx) {
    this->action = NEJIRON_ACTION_INACTIVE;
    if (this->actor.xzDistToPlayer < 200.0f && (ABS(this->actor.yDistToPlayer) <= 50.0f)) {
        this->actor.draw = Nejiron_DrawBody;
        SoundEffect_PlayOneshot(SOUND_ENEMY_NEJIRON_STAND, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 1000.0f, &this->actor);
        this->actor.world.rot.z = 0;
        this->actor.world.rot.x = this->actor.world.rot.z;
        this->actor.flags &= ~ACTOR_FLAG_27;
        this->actor.flags |= (ACTOR_FLAG_0 | ACTOR_FLAG_2);
        this->actionFunc = Nejiron_EmergeFromUnderground;
    }
    this->actor.shape.rot.y = this->actor.world.rot.y;
}

void Nejiron_EmergeFromUnderground(Nejiron* this, PlayState* globalCtx) {
    this->actor.world.rot.y += 0x1518;
    this->actor.shape.rot.y = this->actor.world.rot.y;
    if ((globalCtx->gameplayFrames % 8) == 0) {
        Actor_SpawnFloorDustRing(globalCtx, &this->actor, &this->actor.world.pos, this->actor.shape.shadowScale - 20.0f, 10, 8.0f,
                      500, 10, 1);
    }
    Math_ApproachF(&this->actor.shape.shadowScale, 50.0f, 0.3f, 5.0f);
    Math_ApproachF(&this->actor.shape.yOffset, 2700.0f, 100.0f, 500.0f);
    if (this->actor.shape.yOffset > 2650.0f) {
        this->action = NEJIRON_ACTION_ACTIVE;
        this->actor.shape.yOffset = 2700.0f;
        this->timer = 60;
        this->actionFunc = Nejiron_Idle;
    }
}

void Nejiron_Idle(Nejiron* this, PlayState* globalCtx) {
    Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };
    s16 absoluteYaw;
    s16 yaw;

    if (this->timer != 0) {
        // Depending on how the last roll ended, this actor may be "sitting" on
        // something other than its legs. This slowly corrects that.
        Math_SmoothStepToS(&this->actor.world.rot.x, 0, 10, 100, 1000);
        Math_SmoothStepToS(&this->actor.world.rot.z, 0, 10, 100, 1000);

        // If this actor isn't mostly facing the player, do a discrete turn towards
        // them. It takes 8 frames to turn, and we must wait 8 frames to do another.
        if ((this->timer & 8) != 0) {
            if (fabsf(this->actor.world.rot.y - this->actor.yawTowardsPlayer) > 200.0f) {
                Math_SmoothStepToS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 30, 300, 1000);
                if ((globalCtx->gameplayFrames % 8) == 0) {
                    Actor_SpawnFloorDustRing(globalCtx, &this->actor, &this->actor.world.pos,
                                  this->actor.shape.shadowScale - 20.0f, 10, 8.0f, 500, 10, 1);
                    SoundEffect_PlayOneshot(SOUND_ENEMY_NEJIRON_VOICE, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 1000.0f, &this->actor);
                }
            }
        }
        this->actor.shape.rot.y = this->actor.world.rot.y;
        return;
    }

    yaw = this->actor.yawTowardsPlayer - this->actor.world.rot.y;
    absoluteYaw = ABS_ALT(yaw);
    Math_Vec3f_Copy(&this->targetRotation, &zeroVec);
    Math_Vec3f_Copy(&this->currentRotation, &zeroVec);
    if (absoluteYaw < 0x2000) {
        this->targetRotation.x = 2000.0f;
    } else {
        this->zRollDirection = NEJIRON_DIRECTION_RIGHT;
        this->targetRotation.z = 2000.0f;
        if ((s16)(this->actor.yawTowardsPlayer - this->actor.world.rot.y) > 0) {
            this->zRollDirection = NEJIRON_DIRECTION_LEFT;
        }
    }
    this->timer = 38;
    this->actor.world.rot.y = this->actor.yawTowardsPlayer;
    this->hardHitFlag = 0;
    this->actionFunc = Nejiron_Roll;
}

void Nejiron_Roll(Nejiron* this, PlayState* globalCtx) {
    f32 xDistanceFromHome = this->actor.home.pos.x - this->actor.world.pos.x;
    f32 zDistanceFromHome = this->actor.home.pos.z - this->actor.world.pos.z;

    if ((sqrtf(SQ(xDistanceFromHome) + SQ(zDistanceFromHome)) > this->maxDistanceFromHome)) {
        Nejiron_SetupRetreatUnderground(this);
        return;
    }

    if (this->timer == 0) {
        this->timer = 100;
        this->actor.world.rot.y = this->actor.shape.rot.y;
        this->actionFunc = Nejiron_Idle;
        this->actor.speedXZ = 0.0f;
        return;
    }

    if (!this->hardHitFlag && this->collider.base.atFlags & AT_BOUNCED) {
        this->zRollDirection ^= 1;
        this->hardHitFlag = 1;
        this->actor.speedXZ = -7.0f;
    }

    Math_ApproachF(&this->currentRotation.x, this->targetRotation.x, 0.2f, 1000.0f);
    Math_ApproachF(&this->currentRotation.z, this->targetRotation.z, 0.2f, 1000.0f);
    Math_ApproachF(&this->actor.speedXZ, 5.0f, 0.3f, 0.5f);
    this->actor.world.rot.x += (s16)this->currentRotation.x;
    if (this->currentRotation.z != 0.0f) {
        if (this->zRollDirection == NEJIRON_DIRECTION_RIGHT) {
            this->actor.world.rot.z += (s16)this->currentRotation.z;
        } else {
            this->actor.world.rot.z -= (s16)this->currentRotation.z;
        }
    }
    Audio_PlayActorSfx2(&this->actor, NA_SE_EN_DODO_K_ROLL - SFX_FLAG);
}

void Nejiron_SetupRetreatUnderground(Nejiron* this) {
    this->action = NEJIRON_ACTION_RETREATING;
    this->actionFunc = Nejiron_RetreatUnderground;
    this->actor.speedXZ = 0.0f;
}

void Nejiron_RetreatUnderground(Nejiron* this, PlayState* globalCtx) {
    this->actor.world.rot.y -= 0x1518;
    this->actor.shape.rot.y = this->actor.world.rot.y;
    if ((globalCtx->gameplayFrames % 8) == 0) {
        Actor_SpawnFloorDustRing(globalCtx, &this->actor, &this->actor.world.pos, this->actor.shape.shadowScale - 20.0f, 10, 8.0f,
                      500, 10, 1);
    }
    Math_ApproachF(&this->actor.shape.yOffset, -3000.0f, 100.0f, 500.0f);
    Math_ApproachZeroF(&this->actor.shape.shadowScale, 0.3f, 5.0f);
    if (this->actor.shape.yOffset < -2970.0f) {
        this->actor.shape.yOffset = -3000.0f;
        this->actor.draw = Nejiron_DrawBody;
        Math_Vec3f_Copy(&this->actor.world.pos, &this->actor.home.pos);
        SoundEffect_PlayOneshot(SOUND_ENEMY_NEJIRON_STAND, 1.0f, 1.0f, &this->actor.world.pos, 1.0f, 1000.0f, &this->actor);
        this->actor.flags |= ACTOR_FLAG_27;
        this->actor.flags &= ~(ACTOR_FLAG_0 | ACTOR_FLAG_2);
        this->actionFunc = Nejiron_UndergroundIdle;
    }
}

void Nejiron_PostDetonation(Nejiron* this, PlayState* globalCtx) {
    if (this->timer == 0) {
        Actor_Kill(&this->actor);
    }
    if (this->timer >= 26) {
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &this->collider.base);
    }
}

void Nejiron_CheckForDetonation(Nejiron* this, PlayState* globalCtx) {
    Vec3f effVelocity = { 0.0f, 0.0f, 0.0f };
    Vec3f bomb2Accel = { 0.0f, 0.1f, 0.0f };
    Vec3f effAccel = { 0.0f, 0.0f, 0.0f };
    Vec3f effPos;
    Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    Vec3f acceleration = { 0.0f, 0.0f, 0.0f };
    s32 i;

    // In order to match, this variable must act as both a boolean to check if
    // the Nejiron should forcibly explode and as a loop index.
    i = false;
    if (this->action != NEJIRON_ACTION_EXPLODING && this->action != NEJIRON_ACTION_RETREATING) {
        if (!(this->actor.bgCheckFlags & 1) && this->actor.world.pos.y < (this->actor.home.pos.y - 100.0f)) {
            // Force a detonation if we're off the ground and have fallen
            // below our home position (e.g., we rolled off a ledge).
            i = true;
        }
        if (this->actor.bgCheckFlags & 0x60 && this->actor.yDistToWater >= 40.0f) {
            // Force a detonation if we're too far below the water's surface.
            i = true;
        }
        if ((this->collider.base.acFlags & AC_HIT || i)) {
            this->collider.base.acFlags &= ~AC_HIT;
            if (i || this->actor.colChkInfo.damageEffect == NEJIRON_DMGEFF_KILL) {
                Actor_SetColorFilter(&this->actor, 0x4000, 0xFF, 0, 8);
                this->action = NEJIRON_ACTION_EXPLODING;
                this->actor.speedXZ = 0.0f;
                this->actor.shape.shadowScale = 0.0f;

                for (i = 0; i < ARRAY_COUNT(this->particles); i++) {
                    acceleration.x = (Rand_ZeroOne() - 0.5f) * 8.0f;
                    acceleration.y = -1.0f;
                    acceleration.z = (Rand_ZeroOne() - 0.5f) * 8.0f;
                    velocity.x = (Rand_ZeroOne() - 0.5f) * 14.0f;
                    velocity.y = Rand_ZeroOne() * 30.0f;
                    velocity.z = (Rand_ZeroOne() - 0.5f) * 14.0f;
                    Nejiron_InitializeParticle(this, &this->actor.focus.pos, &velocity, &acceleration,
                                               (Rand_ZeroFloat(1.0f) * 0.01f) + 0.003f, 90);
                }
                effPos = this->actor.world.pos;
                effPos.y += 10.0f;
                EffectSsBomb2_SpawnLayered(globalCtx, &effPos, &effVelocity, &bomb2Accel, 100,
                                       (this->actor.shape.rot.z * 6) + 19);
                effPos.y = this->actor.floorHeight;
                if (this->actor.floorHeight > BGCHECK_Y_MIN) {
                    EffectSsBlast_SpawnWhiteShockwave(globalCtx, &effPos, &effVelocity, &effAccel);
                }
                Audio_PlayActorSfx2(&this->actor, NA_SE_IT_BOMB_EXPLOSION);
                this->timer = 30;
                this->actor.flags |= ACTOR_FLAG_27;
                this->actor.flags &= ~(ACTOR_FLAG_0 | ACTOR_FLAG_2);
                Actor_SetScale(&this->actor, 0.0f);
                this->collider.elements->dim.scale = 3.0f;
                this->collider.elements->info.toucher.damage = 8;
                Item_DropCollectibleRandom(globalCtx, NULL, &this->actor.world.pos, 0xB0);
                this->actionFunc = Nejiron_PostDetonation;
            }
        }
    }
}

void Actor_SetHeight(Actor* actor, f32 height) {
    actor->focus.pos.x = actor->world.pos.x;
    actor->focus.pos.y = actor->world.pos.y + height;
    actor->focus.pos.z = actor->world.pos.z;
    actor->focus.rot.x = actor->world.rot.x;
    actor->focus.rot.y = actor->world.rot.y;
    actor->focus.rot.z = actor->world.rot.z;
}

void Nejiron_Update(Actor* thisx, PlayState* globalCtx) {
    Nejiron* this = (Nejiron*)thisx;

    Actor_SetHeight(&this->actor, 30.0f);
    Nejiron_UpdateParticles(this, globalCtx);
    Nejiron_CheckForDetonation(this, globalCtx);
    this->actionFunc(this, globalCtx);

    DECR(this->blinkTimer);
    DECR(this->timer);

    if (this->action != NEJIRON_ACTION_EXPLODING && this->action != NEJIRON_ACTION_INACTIVE) {
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &this->collider.base);
    }

    if (this->action != NEJIRON_ACTION_EXPLODING) {
        this->actor.shape.rot.x = this->actor.world.rot.x;
        this->actor.shape.rot.z = this->actor.world.rot.z;
        if (this->blinkTimer == 0) {
            this->eyeIndex++;
            if (this->eyeIndex >= 3) {
                this->eyeIndex = 0;
                this->blinkTimer = Rand_ZeroFloat(60.0f) + 20.0f;
            }
        }
        Actor_MoveForward(&this->actor);
        Actor_UpdateBgCheckInfo(globalCtx, &this->actor, 20.0f, 20.0f, 60.0f, 0x1D);
        if (this->action != NEJIRON_ACTION_INACTIVE) {
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &this->collider.base);
        }
        if (this->action != NEJIRON_ACTION_EXPLODING) {
            CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &this->collider.base);
        }
    }
}

void Nejiron_PostLimbDraw(PlayState* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    Nejiron* this = (Nejiron*)thisx;

    Collider_UpdateSpheres(limbIndex, &this->collider);
}

void Nejiron_DrawBody(Actor* thisx, PlayState* globalCtx) {
    static void* sEyeTextures[] = { 0x060014C8, 0x060018C8, 0x06001CC8 };
    Nejiron* this = (Nejiron*)thisx;
    Gfx* gfx;
    s32 eyeIndex;
    void* virtualAddress;

    OPEN_DISPS(globalCtx->state.gfxCtx, __FILE__, __LINE__);

    Gfx_SetupDL_25Xlu(globalCtx->state.gfxCtx);

    gfx = POLY_OPA_DISP;

    eyeIndex = this->eyeIndex;
    virtualAddress = SEGMENTED_TO_VIRTUAL(sEyeTextures[eyeIndex]);
    gSPSegment(&gfx[0], 0x08, virtualAddress);

    POLY_OPA_DISP = &gfx[1];

    SkelAnime_DrawOpa(globalCtx, this->skelAnime.skeleton, this->skelAnime.jointTable, NULL, Nejiron_PostLimbDraw,
                      this);

    CLOSE_DISPS(globalCtx->state.gfxCtx, __FILE__, __LINE__);

    Nejiron_DrawRockParticles(this, globalCtx);
}

void Nejiron_InitializeParticle(Nejiron* this, Vec3f* position, Vec3f* velocity, Vec3f* acceleration, f32 scale,
                                s16 timer) {
    s16 i;
    NejironParticle* particle = this->particles;

    for (i = 0; i < ARRAY_COUNT(this->particles); i++, particle++) {
        if (!particle->isVisible) {
            particle->isVisible = true;
            particle->position = *position;
            particle->velocity = *velocity;
            particle->acceleration = *acceleration;
            particle->scale = scale;
            particle->timer = timer;
            particle->rotation.x = (s16)Rand_CenteredFloat(30000.0f);
            particle->rotation.y = (s16)Rand_CenteredFloat(30000.0f);
            particle->rotation.z = (s16)Rand_CenteredFloat(30000.0f);
            return;
        }
    }
}

void Nejiron_UpdateParticles(Nejiron* this, PlayState* globalCtx) {
    s32 i;
    NejironParticle* particle = this->particles;

    for (i = 0; i < ARRAY_COUNT(this->particles); i++, particle++) {
        if (particle->isVisible) {
            particle->position.x += particle->velocity.x;
            particle->position.y += particle->velocity.y;
            particle->position.z += particle->velocity.z;
            particle->rotation.x += 0xBB8;
            particle->rotation.y += 0xBB8;
            particle->rotation.z += 0xBB8;
            particle->velocity.x += particle->acceleration.x;
            particle->velocity.y += particle->acceleration.y;
            particle->velocity.z += particle->acceleration.z;
            if (particle->position.y < (this->actor.world.pos.y - 10.0f)) {
                Math_ApproachZeroF(&particle->scale, 0.2f, 0.001f);
                if (particle->scale <= 0.0001f) {
                    particle->timer = 0;
                }
            }
            if (particle->timer != 0) {
                particle->timer--;
            } else {
                particle->isVisible = false;
            }
        }
    }
}

void Nejiron_DrawRockParticles(Nejiron* this, PlayState* globalCtx) {
    s16 i;
    NejironParticle* particle = this->particles;
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;

    OPEN_DISPS(globalCtx->state.gfxCtx, __FILE__, __LINE__);
    Gfx_SetupDL_25Xlu(globalCtx->state.gfxCtx);
    for (i = 0; i < ARRAY_COUNT(this->particles); i++, particle++) {
        if (particle->isVisible) {
            Matrix_Translate(particle->position.x, particle->position.y, particle->position.z, MTXMODE_NEW);
            Matrix_RotateX(particle->rotation.x, MTXMODE_APPLY);
            Matrix_RotateY(particle->rotation.y, MTXMODE_APPLY);
            Matrix_RotateZ(particle->rotation.z, MTXMODE_APPLY);
            Matrix_Scale(particle->scale, particle->scale, particle->scale, MTXMODE_APPLY);
            gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(gfxCtx, "../z_en_lightbox.c", __LINE__), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 1, 255, 255, 255, 255);
            gSPDisplayList(POLY_OPA_DISP++, gBoulderFragmentsDL);
        }
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, __FILE__, __LINE__);
}