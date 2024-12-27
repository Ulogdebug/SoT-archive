#include <uLib.h>
#include "object/0x0210-CringeyRedeadObj.h"
#include "z_en_rd.h"

#define FLAGS (ACTOR_FLAG_0 | ACTOR_FLAG_2 | ACTOR_FLAG_4 | ACTOR_FLAG_10 | ACTOR_FLAG_CAN_HOLD_SWITCH)
#define IN_CHASE_RANGE_GIBDO ((this->actor.xzDistToPlayer <= 400.0f)&&(ABS(this->actor.yDistToPlayer) <= 45.0f)&&(this->actor.params <= REDEAD_TYPE_GIBDO))
#define OUT_CHASE_RANGE_GIBDO (((this->actor.xzDistToPlayer > 400.0f)||(ABS(this->actor.yDistToPlayer) > 45.0f))&&(this->actor.params <= REDEAD_TYPE_GIBDO))

void EnRd_Init(Actor* thisx, PlayState* play);
void EnRd_Destroy(Actor* thisx, PlayState* play);
void EnRd_Update(Actor* thisx, PlayState* play);
void EnRd_Draw(Actor* thisx, PlayState* play);

void EnRd_SetupIdle(EnRd* this);
void EnRd_SetupRiseFromCoffin(EnRd* this);
void EnRd_SetupWalkToHome(EnRd* this, PlayState* play);
void EnRd_SetupWalkToParent(EnRd* this);
void EnRd_SetupGrab(EnRd* this, PlayState* play);
void EnRd_SetupAttemptPlayerFreeze(EnRd* this);
void EnRd_SetupStandUp(EnRd* this);
void EnRd_SetupCrouch(EnRd* this);
void EnRd_SetupDead(EnRd* this);
void EnRd_Idle(EnRd* this, PlayState* play);
void EnRd_RiseFromCoffin(EnRd* this, PlayState* play);
void EnRd_WalkToPlayer(EnRd* this, PlayState* play);
void EnRd_WalkToHome(EnRd* this, PlayState* play);
void EnRd_WalkToParent(EnRd* this, PlayState* play);
void EnRd_Grab(EnRd* this, PlayState* play);
void EnRd_AttemptPlayerFreeze(EnRd* this, PlayState* play);
void EnRd_StandUp(EnRd* this, PlayState* play);
void EnRd_Crouch(EnRd* this, PlayState* play);
void EnRd_Damaged(EnRd* this, PlayState* play);
void EnRd_Dead(EnRd* this, PlayState* play);
void EnRd_Stunned(EnRd* this, PlayState* play);
void EnRd_SetupResurrect(EnRd* this);
void EnRd_Resurrect(EnRd* this, PlayState* play);
     
typedef enum {
    /*  0 */ REDEAD_ACTION_IDLE,
    /*  1 */ REDEAD_ACTION_STUNNED,
    /*  2 */ REDEAD_ACTION_WALK_TO_HOME,
    /*  3 */ REDEAD_ACTION_WALK_TO_PARENT,
    /*  4 */ REDEAD_ACTION_WALK_TO_PLAYER_OR_RELEASE_GRAB,
    /*  5 */ REDEAD_ACTION_STAND_UP,
    /*  6 */ REDEAD_ACTION_CROUCH,
    /*  7 */ REDEAD_ACTION_ATTEMPT_PLAYER_FREEZE,
    /*  8 */ REDEAD_ACTION_GRAB,
    /*  9 */ REDEAD_ACTION_DAMAGED,
    /* 10 */ REDEAD_ACTION_DEAD,
    /* 11 */ REDEAD_ACTION_RISE_FROM_COFFIN
} EnRdAction;

typedef enum {
    /* 0 */ REDEAD_GRAB_START,
    /* 1 */ REDEAD_GRAB_INITIAL_DAMAGE,
    /* 2 */ REDEAD_GRAB_ATTACK,
    /* 3 */ REDEAD_GRAB_RELEASE,
    /* 4 */ REDEAD_GRAB_END
} EnRdGrabState;

const ActorInit En_Rd_InitVars = {
    0x0220, // ID
    ACTORCAT_ENEMY,
    FLAGS,
    0x0210, //Object
    sizeof(EnRd),
    (ActorFunc)EnRd_Init,
    (ActorFunc)EnRd_Destroy,
    (ActorFunc)EnRd_Update,
    (ActorFunc)EnRd_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_HIT7,
        AT_NONE,
        AC_ON | AC_TYPE_ALL,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK1,
        { 0x00000000, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON | BUMP_HOOKABLE,
        OCELEM_ON,
    },
    { 20, 76, 0, { 0, 0, 0 } },
};

typedef enum {
    /* 0x0 */ REDEAD_DMGEFF_NONE,              // Does not interact with the Gibdo/Redead at all
    /* 0x1 */ REDEAD_DMGEFF_HOOKSHOT,          // Stuns the Gibdo/Redead
    /* 0x6 */ REDEAD_DMGEFF_ICE_MAGIC = 0x6,   // Does not interact with the Gibdo/Redead at all
    /* 0xD */ REDEAD_DMGEFF_LIGHT_MAGIC = 0xD, // Stuns the Gibdo/Redead
    /* 0xE */ REDEAD_DMGEFF_FIRE_MAGIC,        // Applies a fire effect
    /* 0xF */ REDEAD_DMGEFF_DAMAGE,             // Deals damage without stunning or applying an effect
} EnRdDamageEffect;

static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, REDEAD_DMGEFF_NONE),
    /* Deku stick    */ DMG_ENTRY(0, REDEAD_DMGEFF_NONE),
    /* Slingshot     */ DMG_ENTRY(0, REDEAD_DMGEFF_NONE),
    /* Explosive     */ DMG_ENTRY(1, REDEAD_DMGEFF_DAMAGE),
    /* Boomerang     */ DMG_ENTRY(0, REDEAD_DMGEFF_NONE),
    /* Normal arrow  */ DMG_ENTRY(0, REDEAD_DMGEFF_NONE),
    /* Hammer swing  */ DMG_ENTRY(4, REDEAD_DMGEFF_DAMAGE),
    /* Hookshot      */ DMG_ENTRY(0, REDEAD_DMGEFF_HOOKSHOT),
    /* Kokiri sword  */ DMG_ENTRY(0, REDEAD_DMGEFF_NONE),
    /* Master sword  */ DMG_ENTRY(0, REDEAD_DMGEFF_NONE),
    /* Giant's Knife */ DMG_ENTRY(2, REDEAD_DMGEFF_DAMAGE),
    /* Fire arrow    */ DMG_ENTRY(6, REDEAD_DMGEFF_FIRE_MAGIC),
    /* Ice arrow     */ DMG_ENTRY(0, REDEAD_DMGEFF_NONE),
    /* Light arrow   */ DMG_ENTRY(4, REDEAD_DMGEFF_FIRE_MAGIC),
    /* Unk arrow 1   */ DMG_ENTRY(0, REDEAD_DMGEFF_NONE),
    /* Unk arrow 2   */ DMG_ENTRY(0, REDEAD_DMGEFF_NONE),
    /* Unk arrow 3   */ DMG_ENTRY(0, REDEAD_DMGEFF_NONE),
    /* Fire magic    */ DMG_ENTRY(8, REDEAD_DMGEFF_FIRE_MAGIC),
    /* Ice magic     */ DMG_ENTRY(0, REDEAD_DMGEFF_ICE_MAGIC),
    /* Light magic   */ DMG_ENTRY(3, REDEAD_DMGEFF_LIGHT_MAGIC),
    /* Shield        */ DMG_ENTRY(0, REDEAD_DMGEFF_NONE),
    /* Mirror Ray    */ DMG_ENTRY(2, REDEAD_DMGEFF_FIRE_MAGIC),
    /* Kokiri spin   */ DMG_ENTRY(0, REDEAD_DMGEFF_NONE),
    /* Giant spin    */ DMG_ENTRY(0, REDEAD_DMGEFF_NONE),
    /* Master spin   */ DMG_ENTRY(0, REDEAD_DMGEFF_NONE),
    /* Kokiri jump   */ DMG_ENTRY(0, REDEAD_DMGEFF_NONE),
    /* Giant jump    */ DMG_ENTRY(4, REDEAD_DMGEFF_DAMAGE),
    /* Master jump   */ DMG_ENTRY(2, REDEAD_DMGEFF_DAMAGE),
    /* Unknown 1     */ DMG_ENTRY(0, REDEAD_DMGEFF_NONE),
    /* Unblockable   */ DMG_ENTRY(4, REDEAD_DMGEFF_DAMAGE),
    /* Hammer jump   */ DMG_ENTRY(8, REDEAD_DMGEFF_DAMAGE),
    /* Unknown 2     */ DMG_ENTRY(0, REDEAD_DMGEFF_NONE),
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(targetArrowOffset, 2000, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 11, ICHAIN_CONTINUE),//10
    ICHAIN_F32_DIV1000(gravity, -3500, ICHAIN_STOP),
};

void EnRd_SetupAction(EnRd* this, EnRdActionFunc actionFunc) {
    this->actionFunc = actionFunc;
}

void EnRd_Init(Actor* thisx, PlayState* play) {
    EnRd* this = (EnRd*)thisx;

    Actor_ProcessInitChain(thisx, sInitChain);
    this->actor.colChkInfo.damageTable = &sDamageTable;
    ActorShape_Init(&thisx->shape, 0.0f, NULL, 0.0f);
    this->upperBodyYRotation = this->headYRotation = 0;
    this->actor.focus.pos = thisx->world.pos;
    this->actor.focus.pos.y += 50.0f;
    this->actor.colChkInfo.mass = MASS_HEAVY;
    this->actor.colChkInfo.health = 8;
    this->alpha = this->unk_31D = 255;
    this->rdFlags = REDEAD_GET_FLAGS(thisx);
    this->gibdoSightDistance = 400.0f;
    this->fireDamageTimer = 30;
    if (this->actor.params & 0x80) {
        this->actor.params |= 0xFF00;
    } else {
        this->actor.params &= 0xFF;
    }

    if (this->actor.params >= REDEAD_TYPE_DOES_NOT_MOURN) {
        SkelAnime_InitFlex(play, &this->skelAnime, &gRedeadObj_SkelRedeadObj, &gRedeadObj_AnimIdle, this->jointTable,
                           this->morphTable, REDEADOBJ_LIMB_MAX);
        this->actor.naviEnemyId = NAVI_ENEMY_POE_SISTER_MEG;//normal redead
    } else {
        SkelAnime_InitFlex(play, &this->skelAnime, &gRedeadObj_SkelRedeadObj, &gRedeadObj_AnimIdle, this->jointTable,
                           this->morphTable, REDEADOBJ_LIMB_MAX);
        this->actor.naviEnemyId = NAVI_ENEMY_POE_SISTER_MEG;//gibido
    }

    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinder(play, &this->collider, thisx, &sCylinderInit);

     this->actor.targetMode = 8;

    if (this->actor.params >= REDEAD_TYPE_GIBDO) {
        EnRd_SetupIdle(this);
    } else {
        EnRd_SetupIdle(this);
        //EnRd_SetupRiseFromCoffin(this);
    }

    SkelAnime_Update(&this->skelAnime);

    if (this->actor.params == REDEAD_TYPE_INVISIBLE || this->actor.home.rot.z != 0) {
        this->actor.flags |= ACTOR_FLAG_7;
        this->actor.world.rot.z = this->actor.shape.rot.z = 0;
    }

}

void EnRd_Destroy(Actor* thisx, PlayState* play) {
    EnRd* this = (EnRd*)thisx;

    if (gSaveContext.sunsSongState != SUNSSONG_INACTIVE) {
        gSaveContext.sunsSongState = SUNSSONG_INACTIVE;
    }

    Collider_DestroyCylinder(play, &this->collider);
}

/**
 * The `parent` pointer is updated for all currently loaded Redeads depending
 * on the `shouldMourn` variable.
 *
 * If `shouldMourn` is true, the parent of all other Redeads is set to this
 * instance so they will mourn this Redead.
 *
 * If `shouldMourn` is false, the parent of all other Redeads is cleared so that
 * they stop mourning. This is done when the dead Redead starts fading away.
 */
void EnRd_UpdateMourningTarget(PlayState* play, Actor* thisx, s32 shouldMourn) {
    Actor* enemyIterator = play->actorCtx.actorLists[ACTORCAT_ENEMY].head;

    while (enemyIterator != NULL) {
        if ((enemyIterator->id != ACTOR_EN_RD) || (enemyIterator == thisx) ||
            (enemyIterator->params < REDEAD_TYPE_DOES_NOT_MOURN_IF_WALKING)) {
            enemyIterator = enemyIterator->next;
            continue;
        }

        if (shouldMourn) {
            enemyIterator->parent = thisx;
        } else if (enemyIterator->parent == thisx) {
            enemyIterator->parent = NULL;
        }

        enemyIterator = enemyIterator->next;
    }
}

void EnRd_SetupIdle(EnRd* this) {
    if (this->actor.params != REDEAD_TYPE_CRYING) {
        Animation_MorphToLoop(&this->skelAnime, &gRedeadObj_AnimIdle, -6.0f);
    } else {
        Animation_PlayLoop(&this->skelAnime, &gRedeadObj_AnimCrouching);
    }

    this->action = REDEAD_ACTION_IDLE;
    this->timer = (Rand_ZeroOne() * 10.0f) + 5.0f;
    this->actor.speedXZ = 0.0f;
    this->actor.world.rot.y = this->actor.shape.rot.y;
    EnRd_SetupAction(this, EnRd_Idle);
}

void EnRd_Idle(EnRd* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime);
    Math_SmoothStepToS(&this->headYRotation, 0, 1, 0x64, 0);
    Math_SmoothStepToS(&this->upperBodyYRotation, 0, 1, 0x64, 0);

    if ((this->actor.params == REDEAD_TYPE_CRYING) && (this->skelAnime.curFrame == 0.0f)) {
        if (Rand_ZeroOne() >= 0.5f) {
            Animation_PlayLoop(&this->skelAnime, &gRedeadObj_AnimCrouching);
        } else {
            Animation_PlayLoop(&this->skelAnime, &gRedeadObj_AnimCroucharmswipe);
        }
    } else {
        this->timer--;
        if (this->timer == 0) {
            // This resets the idle animation back to its first frame, making the
            // Redead/Gibdo appear to "judder" in place.
            this->timer = (Rand_ZeroOne() * 10.0f) + 10.0f;
            this->skelAnime.curFrame = 0.0f;
        }
    }

    if (this->actor.parent != NULL) {
        if (!this->isMourning) {
            if (this->actor.params != REDEAD_TYPE_CRYING) {
                EnRd_SetupWalkToParent(this);
            } else {
                EnRd_SetupStandUp(this);
            }
        }
    } else {
        if (this->isMourning) {
            if (this->actor.params != REDEAD_TYPE_CRYING) {
                EnRd_SetupAttemptPlayerFreeze(this);
            } else {
                EnRd_SetupStandUp(this);
            }
        }

        this->isMourning = false;

        if (this->actor.params <= REDEAD_TYPE_GIBDO) {
            if (Actor_IsFacingPlayer(&this->actor, 60 * 0x10000 / 360)){
                if (IN_CHASE_RANGE_GIBDO) {
                    if ((this->actor.params != REDEAD_TYPE_CRYING) && !this->isMourning) {
                        EnRd_SetupAttemptPlayerFreeze(this);
                    } else {
                        EnRd_SetupStandUp(this);
                    }
                }
            }else{
                if ((this->actor.xzDistToPlayer <= 100.0f) && func_8002DDE4(play)&& (this->actor.yDistToPlayer <= 45.0f)) {
                    if ((this->actor.params != REDEAD_TYPE_CRYING) && !this->isMourning) {
                        EnRd_SetupAttemptPlayerFreeze(this);
                    } else {
                        EnRd_SetupStandUp(this);
                    }
                }

            }
        }else if ((this->actor.xzDistToPlayer <= 150.0f) && func_8002DDE4(play)&& (ABS(this->actor.yDistToPlayer) <= 45.0f)) {
              if ((this->actor.params != REDEAD_TYPE_CRYING) && !this->isMourning) {
                  EnRd_SetupAttemptPlayerFreeze(this);
              } else {
                  EnRd_SetupStandUp(this);
              }
        }
        
    }
    

    if ((play->gameplayFrames & 0x5F) == 0) {
        Audio_PlayActorSfx2(&this->actor, NA_SE_EN_REDEAD_CRY);
    }
}

void EnRd_SetupRiseFromCoffin(EnRd* this) {
    Animation_Change(&this->skelAnime, &gRedeadObj_AnimIdle, 0, 0, Animation_GetLastFrame(&gRedeadObj_AnimIdle),
                     ANIMMODE_LOOP, -6.0f);
    this->action = REDEAD_ACTION_RISE_FROM_COFFIN;
    this->timer = 6;
    this->actor.shape.rot.x = -0x4000;
    this->actor.gravity = 0.0f;
    this->actor.shape.yOffset = 0.0f;
    this->actor.speedXZ = 0.0f;
    EnRd_SetupAction(this, EnRd_RiseFromCoffin);
}

void EnRd_RiseFromCoffin(EnRd* this, PlayState* play) {
    if (this->actor.shape.rot.x != -0x4000) {
        Math_SmoothStepToS(&this->actor.shape.rot.x, 0, 1, 0x7D0, 0);
        if (Math_SmoothStepToF(&this->actor.world.pos.y, this->actor.home.pos.y, 0.3f, 2.0f, 0.3f) == 0.0f) {
            this->actor.gravity = -3.5f;
            EnRd_SetupIdle(this);
        }
    } else {
        if (this->actor.world.pos.y == this->actor.home.pos.y) {
            Audio_PlayActorSfx2(&this->actor, NA_SE_EN_REDEAD_CRY);
        }

        if (Math_SmoothStepToF(&this->actor.world.pos.y, this->actor.home.pos.y + 50.0f, 0.3f, 2.0f, 0.3f) == 0.0f) {
            if (this->timer != 0) {
                this->timer--;
                Math_SmoothStepToF(&this->actor.speedXZ, 6.0f, 0.3f, 1.0f, 0.3f);
            } else if (Math_SmoothStepToF(&this->actor.speedXZ, 0.0f, 0.3f, 1.0f, 0.3f) == 0.0f) {
                Math_SmoothStepToS(&this->actor.shape.rot.x, 0, 1, 0x7D0, 0);
            }
        }
    }
}

void EnRd_SetupWalkToPlayer(EnRd* this, PlayState* play) {
    Animation_Change(&this->skelAnime, &gRedeadObj_AnimSlowWalk, 1.0f, 4.0f, Animation_GetLastFrame(&gRedeadObj_AnimSlowWalk),
                     ANIMMODE_LOOP_INTERP, -4.0f);
    if(this->actor.params <= REDEAD_TYPE_GIBDO){
        this->actor.speedXZ = 1.0f;
    }else{
        this->actor.speedXZ = 0.4f;
    }
    this->action = REDEAD_ACTION_WALK_TO_PLAYER_OR_RELEASE_GRAB;
    EnRd_SetupAction(this, EnRd_WalkToPlayer);
}

void EnRd_WalkToPlayer(EnRd* this, PlayState* play) {
    Vec3f D_80AE4918 = { 0.0f, 0.0f, 0.0f };
    Color_RGBA8 D_80AE4924 = { 200, 200, 255, 255 };
    Color_RGBA8 D_80AE4928 = { 0, 0, 255, 0 };
    Player* player = GET_PLAYER(play);
    s16 gibdoRotateRate = 0xE4; 
    s32 pad;
    s16 yaw = this->actor.yawTowardsPlayer - this->actor.shape.rot.y - this->headYRotation - this->upperBodyYRotation;

    this->skelAnime.playSpeed = this->actor.speedXZ;
     if(this->actor.params <= REDEAD_TYPE_GIBDO){
        Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, (2 * gibdoRotateRate), 0);
        Math_SmoothStepToS(&this->headYRotation, 0, 1, gibdoRotateRate, 0);
        Math_SmoothStepToS(&this->upperBodyYRotation, 0, 1, gibdoRotateRate, 0);
    }else{
        Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 0xFA, 0);
        Math_SmoothStepToS(&this->headYRotation, 0, 1, 0x64, 0);
        Math_SmoothStepToS(&this->upperBodyYRotation, 0, 1, 0x64, 0);
    }
    this->actor.world.rot.y = this->actor.shape.rot.y;
    SkelAnime_Update(&this->skelAnime);

     if(this->actor.params <= REDEAD_TYPE_GIBDO){
        if(Actor_WorldDistXYZToActor(&this->actor, &player->actor) <= 200.0f && (this->actor.speedXZ < 2.5f) && 
        (Actor_WorldDistXYZToActor(&this->actor, &player->actor) > 100.0f) && Actor_IsFacingPlayer(&this->actor, 60 * 0x10000 / 360)){
            this->actor.speedXZ += 0.05f;
        }
        else if(this->actor.xzDistToPlayer <= 100.0f && this->actor.yDistToPlayer < 45.0f){
            SoundEffect_PlayHeld(SOUND_VIOLIN_TENSE, 1.0f,1.1f,1.0,&this->actor.world.pos,1.0f,1000.0f, &this->actor);
            if(Actor_IsFacingPlayer(&this->actor, 60 * 0x10000 / 360)&& (this->actor.speedXZ < 2.5f)){
            this->actor.speedXZ += 0.1f;
            }
        }
        else if(((Actor_WorldDistXYZToActor(&this->actor, &player->actor) > 200.0f && this->actor.speedXZ >= 1.0f))
        ||((!Actor_IsFacingPlayer(&this->actor, 60 * 0x10000 / 360)) && this->actor.speedXZ >= 1.0f) ){
            this->actor.speedXZ -= 0.1f;

        }
     }

    if(OUT_CHASE_RANGE_GIBDO){
        EnRd_SetupWalkToHome(this, play);
    }else if((Actor_WorldDistXYZToPoint(&player->actor, &this->actor.home.pos) >= 150.0f) && (this->actor.params >= REDEAD_TYPE_DOES_NOT_MOURN)){
        EnRd_SetupWalkToHome(this, play);
    }

    if ((ABS(yaw) < 0x1554) && (this->actor.xzDistToPlayer <= 150.0f)&&(this->actor.yDistToPlayer <= 45.0f)) {
        if (!(player->stateFlags1 & (PLAYER_STATE1_7 | PLAYER_STATE1_13 | PLAYER_STATE1_14 | PLAYER_STATE1_18 |
                                     PLAYER_STATE1_19 | PLAYER_STATE1_21)) &&
            !(player->stateFlags2 & PLAYER_STATE2_7)) {

            if(this->actor.params >= REDEAD_TYPE_DOES_NOT_MOURN){
                if (this->playerStunWaitTimer == 0) {
                    if (!(this->rdFlags & 0x80)) {
                        player->actor.freezeTimer = 20;
                        func_8008EEAC(play, &this->actor);
                        GET_PLAYER(play)->unk_684 = &this->actor;
                        Rumble_Request(this->actor.xzDistToPlayer, 0xFF, 0x14, 0x96);
                        this->playerStunWaitTimer = 90;
                        Audio_PlayActorSfx2(&this->actor, NA_SE_EN_REDEAD_AIM);
                    }
                
                }
            }
            else if(Actor_WorldDistXYZToActor(&this->actor, &player->actor) <= 50.0f){//GIBDO
                // if (this->playerStunWaitTimer == 0) {
                //     if (!(this->rdFlags & 0x80)) {
                //         player->actor.freezeTimer = 40;
                //         func_8008EEAC(play, &this->actor);
                //         GET_PLAYER(play)->unk_684 = &this->actor;
                //         Rumble_Request(this->actor.xzDistToPlayer, 0xFF, 0x14, 0x96);
                //     }
                
                //     this->playerStunWaitTimer = 60;
                //     Audio_PlayActorSfx2(&this->actor, NA_SE_EN_REDEAD_AIM);
                // }

            }
    } else {
            EnRd_SetupWalkToHome(this, play);
        }
    }

    if (this->grabWaitTimer != 0) {
        this->grabWaitTimer--;
    }

    if (!this->grabWaitTimer && (Actor_WorldDistXYZToActor(&this->actor, &player->actor) <= 45.0f) &&
        Actor_IsFacingPlayer(&this->actor, 0x38E3)) {
        player->actor.freezeTimer = 0;
        if (play->grabPlayer(play, player)) {
            this->actor.flags &= ~ACTOR_FLAG_0;
            EnRd_SetupGrab(this,play);
        }
    } else if (this->actor.params > REDEAD_TYPE_DOES_NOT_MOURN_IF_WALKING) {
        if (this->actor.parent != NULL) {
            EnRd_SetupWalkToParent(this);
        } else {
            this->isMourning = false;
        }
    }

    if ((this->skelAnime.curFrame == 10.0f) || (this->skelAnime.curFrame == 22.0f)) {
        Audio_PlayActorSfx2(&this->actor, NA_SE_EN_RIZA_WALK);
    } else if ((play->gameplayFrames & 0x5F) == 0) {
        if(this->actor.params >= REDEAD_TYPE_DOES_NOT_MOURN){
            Audio_PlayActorSfx2(&this->actor, NA_SE_EN_REDEAD_CRY);
        }else if(OUT_CHASE_RANGE_GIBDO){
            Audio_PlayActorSfx2(&this->actor, NA_SE_EN_REDEAD_CRY);
        }
    }
}

void EnRd_SetupWalkToHome(EnRd* this, PlayState* play) {
    Animation_Change(&this->skelAnime, &gRedeadObj_AnimSlowWalk, 0.5f, 0, Animation_GetLastFrame(&gRedeadObj_AnimSlowWalk),
                     ANIMMODE_LOOP_INTERP, -4.0f);
    this->action = REDEAD_ACTION_WALK_TO_HOME;
    this->actor.speedXZ = 0.4f;
    EnRd_SetupAction(this, EnRd_WalkToHome);
}

void EnRd_WalkToHome(EnRd* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 pad;
    s16 targetY = Actor_WorldYawTowardPoint(&this->actor, &this->actor.home.pos);
    this->skelAnime.playSpeed = this->actor.speedXZ;
    if (Actor_WorldDistXYZToPoint(&this->actor, &this->actor.home.pos) >= 5.0f) {
        Math_SmoothStepToS(&this->actor.shape.rot.y, targetY, 1, 0x1C2, 0);
    } else {
        this->actor.speedXZ = 0.0f;
        if (Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.home.rot.y, 1, 0x1C2, 0) == 0) {
            if (this->actor.params != REDEAD_TYPE_CRYING) {
                EnRd_SetupIdle(this);
            } else {
                EnRd_SetupCrouch(this);
            }
        }
    }

    Math_SmoothStepToS(&this->headYRotation, 0, 1, 0x64, 0);
    Math_SmoothStepToS(&this->upperBodyYRotation, 0, 1, 0x64, 0);
    this->actor.world.rot.y = this->actor.shape.rot.y;
    SkelAnime_Update(&this->skelAnime);

    if (!(player->stateFlags1 & (PLAYER_STATE1_7 | PLAYER_STATE1_13 | PLAYER_STATE1_14 | PLAYER_STATE1_18 |
                                 PLAYER_STATE1_19 | PLAYER_STATE1_21)) &&
        !(player->stateFlags2 & PLAYER_STATE2_7)) {
        if(Actor_WorldDistXYZToPoint(&player->actor, &this->actor.home.pos) < 150.0f){
            // if (this->actor.params <= REDEAD_TYPE_GIBDO) {
            //     this->actor.targetMode = 8;
            //  } else {
            //     this->actor.targetMode = 0;
            //  }
            EnRd_SetupWalkToPlayer(this, play);
        }else if(IN_CHASE_RANGE_GIBDO){
            // this->actor.targetMode = 8;
            EnRd_SetupWalkToPlayer(this, play);
        }
    } else if (this->actor.params > REDEAD_TYPE_DOES_NOT_MOURN_IF_WALKING) {
        if (this->actor.parent != NULL) {
            EnRd_SetupWalkToParent(this);
        } else {
            this->isMourning = false;
        }
    }

    if (this->skelAnime.curFrame == 10.0f || this->skelAnime.curFrame == 22.0f) {
        Audio_PlayActorSfx2(&this->actor, NA_SE_EN_RIZA_WALK);
    } else if ((play->gameplayFrames & 0x5F) == 0) {
        Audio_PlayActorSfx2(&this->actor, NA_SE_EN_REDEAD_CRY);
    }
}

void EnRd_SetupWalkToParent(EnRd* this) {
    Animation_Change(&this->skelAnime, &gRedeadObj_AnimSlowWalk, 0.5f, 0, Animation_GetLastFrame(&gRedeadObj_AnimSlowWalk),
                     ANIMMODE_LOOP_INTERP, -4.0f);
    this->action = REDEAD_ACTION_WALK_TO_PARENT;
    this->isMourning = true;
    EnRd_SetupAction(this, EnRd_WalkToParent);
}

/**
 * When a Redead or Gibdo dies, it sets itself to be the parent for all other
 * Redeads in the area that are capable of mourning. This function will make
 * these Redeads walk over to the corpse and stand near until it begins to
 * fade away.
 */
void EnRd_WalkToParent(EnRd* this, PlayState* play) {
    if (this->actor.parent != NULL) {
        s32 pad;
        s16 targetY;
        Vec3f parentPos = this->actor.parent->world.pos;

        targetY = Actor_WorldYawTowardPoint(&this->actor, &parentPos);

        Math_SmoothStepToS(&this->actor.shape.rot.y, targetY, 1, 0xFA, 0);

        if (Actor_WorldDistXYZToPoint(&this->actor, &parentPos) >= 45.0f) {
            this->actor.speedXZ = 0.4f;
        } else {
            this->actor.speedXZ = 0.0f;
            if (this->actor.params != REDEAD_TYPE_CRYING) {
                EnRd_SetupIdle(this);
            } else {
                EnRd_SetupCrouch(this);
            }
        }

        Math_SmoothStepToS(&this->headYRotation, 0, 1, 0x64, 0);
        Math_SmoothStepToS(&this->upperBodyYRotation, 0, 1, 0x64, 0);
    } else {
        EnRd_SetupWalkToPlayer(this, play);
    }

    this->actor.world.rot.y = this->actor.shape.rot.y;
    SkelAnime_Update(&this->skelAnime);

    if (this->skelAnime.curFrame == 10.0f || this->skelAnime.curFrame == 22.0f) {
        Audio_PlayActorSfx2(&this->actor, NA_SE_EN_RIZA_WALK);
    } else if ((play->gameplayFrames & 0x5F) == 0) {
        Audio_PlayActorSfx2(&this->actor, NA_SE_EN_REDEAD_CRY);
    }
}

void EnRd_SetupGrab(EnRd* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Animation_PlayOnce(&this->skelAnime, &gRedeadObj_AnimGrabPlayer);
    f32 rand01 = Rand_ZeroOne();
    if (rand01 < 0.4f){ //Hug Link face to face
        this->targetRotY = player->actor.shape.rot.y + 0x8000;
        this->forwardOffset = -22.0f;//original 25.0f, bigger value means it closer to Link
    }else{
        this->targetRotY = player->actor.shape.rot.y;
        this->forwardOffset = -25.0f;
    }
    this->timer = this->grabState = 0;
    this->grabDamageTimer = 200;
    this->action = REDEAD_ACTION_GRAB;
    this->actor.speedXZ = 0.0f;
    EnRd_SetupAction(this, EnRd_Grab);
}

void EnRd_Grab(EnRd* this, PlayState* play) {
    s32 pad;
    Player* player = GET_PLAYER(play);
    f32 rand01 = Rand_ZeroOne();

    if (SkelAnime_Update(&this->skelAnime)) {
        this->grabState++;
    }

    switch (this->grabState) {
        case REDEAD_GRAB_INITIAL_DAMAGE:
            Animation_PlayLoop(&this->skelAnime, &gRedeadObj_AnimChokePlayer);
            this->grabState++;
            play->damagePlayer(play, -8);
            Rumble_Request(this->actor.xzDistToPlayer, 0xFF, 1, 0xC);
            this->grabDamageTimer = 20;
            FALLTHROUGH;
        case REDEAD_GRAB_START:
            Math_SmoothStepToS(&this->headYRotation, 0, 1, 0x5DC, 0);
            Math_SmoothStepToS(&this->upperBodyYRotation, 0, 1, 0x5DC, 0);
            FALLTHROUGH;
        case REDEAD_GRAB_ATTACK:
            if (!(player->stateFlags2 & PLAYER_STATE2_7)) {
                Animation_Change(&this->skelAnime, &gRedeadObj_AnimReleasePlayer, 0.5f, 0.0f,
                                 Animation_GetLastFrame(&gRedeadObj_AnimReleasePlayer), ANIMMODE_ONCE_INTERP, 0.0f);
                this->grabState++;
                this->action = REDEAD_ACTION_WALK_TO_PLAYER_OR_RELEASE_GRAB;
                return;
            }

            if (!LINK_IS_ADULT) {
                Math_SmoothStepToF(&this->actor.shape.yOffset, -1500.0f, 1.0f, 150.0f, 0.0f);
            }else{
                Math_SmoothStepToF(&this->actor.shape.yOffset, -700.0f, 1.0f, 150.0f, 0.0f);                
            }

            Math_SmoothStepToF(&this->actor.world.pos.x,
                               (Math_SinS(this->targetRotY) * this->forwardOffset) + player->actor.world.pos.x, 1.0f, 10.0f,
                               0.0f);
            Math_SmoothStepToF(&this->actor.world.pos.y, player->actor.world.pos.y, 1.0f, 10.0f, 0.0f);
            Math_SmoothStepToF(&this->actor.world.pos.z,
                               (Math_CosS(this->targetRotY) * this->forwardOffset) + player->actor.world.pos.z, 1.0f, 10.0f,
                               0.0f);
            Math_SmoothStepToS(&this->actor.shape.rot.y, this->targetRotY, 1, 0x1770, 0);

            if (this->skelAnime.curFrame == 0.0f) {
                Audio_PlayActorSfx2(&this->actor, NA_SE_EN_REDEAD_ATTACK);
            }

            this->grabDamageTimer--;
            if (this->grabDamageTimer == 0) {
                play->damagePlayer(play, -8);
                if (this->actor.colChkInfo.health < 16) {
                    this->actor.colChkInfo.health++;
                   
                }
                Rumble_Request(this->actor.xzDistToPlayer, 0xF0, 1, 0xC);
                this->grabDamageTimer = 20;
                func_8002F7DC(&player->actor, NA_SE_VO_LI_DAMAGE_S + player->ageProperties->unk_92);
            }
            break;

        case REDEAD_GRAB_RELEASE:
            Math_SmoothStepToF(&this->actor.shape.yOffset, 0, 1.0f, 400.0f, 0.0f);
            
            break;

        case REDEAD_GRAB_END:
            Math_SmoothStepToF(&this->actor.shape.yOffset, 0, 1.0f, 400.0f, 0.0f);
            //  if (this->actor.params <= REDEAD_TYPE_GIBDO) {
            //     this->actor.targetMode = 8;
            //  } else {
            //     this->actor.targetMode = 0;
            //  }
            this->actor.flags |= ACTOR_FLAG_0;
            this->playerStunWaitTimer = 90;
            this->grabWaitTimer = 0xF;

            EnRd_SetupWalkToPlayer(this, play);
            break;
    }
}

void EnRd_SetupAttemptPlayerFreeze(EnRd* this) {
    Animation_Change(&this->skelAnime, &gRedeadObj_AnimTurn90Deg, 0.0f, 0.0f,
                     Animation_GetLastFrame(&gRedeadObj_AnimTurn90Deg), ANIMMODE_ONCE, 0.0f);
    this->action = REDEAD_ACTION_ATTEMPT_PLAYER_FREEZE;
    EnRd_SetupAction(this, EnRd_AttemptPlayerFreeze);
}

void EnRd_AttemptPlayerFreeze(EnRd* this, PlayState* play) {
    Vec3f D_80AE492C = { 0.0f, 0.0f, 0.0f };
    Color_RGBA8 D_80AE4938 = { 200, 200, 255, 255 };
    Color_RGBA8 D_80AE493C = { 0, 0, 255, 0 };
    Player* player = GET_PLAYER(play);
    s16 yaw = this->actor.yawTowardsPlayer - this->actor.shape.rot.y - this->headYRotation - this->upperBodyYRotation;
    
    if (ABS(yaw) < 0x2008) {
        
         if ((!(this->rdFlags & 0x80) && (this->actor.params >= REDEAD_TYPE_DOES_NOT_MOURN)) 
         || ((this->actor.params <= REDEAD_TYPE_GIBDO) && (Actor_WorldDistXYZToActor(&this->actor, &player->actor) <= 50.0f)) ) {
            if (this->playerStunWaitTimer == 0) {
                player->actor.freezeTimer = 20;
                Rumble_Request(this->actor.xzDistToPlayer, 0xFF, 0x14, 0x96);
                func_8008EEAC(play, &this->actor);
                Audio_PlayActorSfx2(&this->actor, NA_SE_EN_REDEAD_AIM);
                this->playerStunWaitTimer = 90;
            }
         }

        EnRd_SetupWalkToPlayer(this, play);
    }
}

void EnRd_SetupStandUp(EnRd* this) {
    Animation_MorphToPlayOnce(&this->skelAnime, &gRedeadObj_AnimStandUp, -4.0f);
    this->action = REDEAD_ACTION_STAND_UP;
    EnRd_SetupAction(this, EnRd_StandUp);
}

void EnRd_StandUp(EnRd* this, PlayState* play) {
    if (SkelAnime_Update(&this->skelAnime)) {
        if (this->actor.parent != NULL) {
            EnRd_SetupWalkToParent(this);
        } else {
            EnRd_SetupAttemptPlayerFreeze(this);
        }
    }
}

void EnRd_SetupCrouch(EnRd* this) {
    Animation_Change(&this->skelAnime, &gRedeadObj_AnimStandUp, -1.0f,
                     Animation_GetLastFrame(&gRedeadObj_AnimStandUp), 0.0f, ANIMMODE_ONCE, -4.0f);
    this->action = REDEAD_ACTION_CROUCH;
    EnRd_SetupAction(this, EnRd_Crouch);
}

void EnRd_Crouch(EnRd* this, PlayState* play) {
    if (SkelAnime_Update(&this->skelAnime)) {
        EnRd_SetupIdle(this);
    }
}

void EnRd_SetupDamaged(EnRd* this) {
    Animation_MorphToPlayOnce(&this->skelAnime, &gRedeadObj_AnimDamaged, -6.0f);

    if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
        this->actor.speedXZ = -2.0f;
    }

    this->actor.flags |= ACTOR_FLAG_0;
    Audio_PlayActorSfx2(&this->actor, NA_SE_EN_REDEAD_DAMAGE);
    this->action = REDEAD_ACTION_DAMAGED;
    EnRd_SetupAction(this, EnRd_Damaged);
}

void EnRd_Damaged(EnRd* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (this->actor.speedXZ < 0.0f) {
        this->actor.speedXZ += 0.15f;
    }

    this->actor.world.rot.y = this->actor.yawTowardsPlayer;
    Math_SmoothStepToS(&this->headYRotation, 0, 1, 0x12C, 0);
    Math_SmoothStepToS(&this->upperBodyYRotation, 0, 1, 0x12C, 0);
    if (SkelAnime_Update(&this->skelAnime)) {
        this->actor.world.rot.y = this->actor.shape.rot.y;

        if (this->actor.parent != NULL) {
            EnRd_SetupWalkToParent(this);
        } else if (Actor_WorldDistXYZToPoint(&player->actor, &this->actor.home.pos) >= 150.0f && (this->actor.params >= REDEAD_TYPE_DOES_NOT_MOURN) ) { 
            EnRd_SetupWalkToHome(this, play);
        }
        else if (OUT_CHASE_RANGE_GIBDO){
            EnRd_SetupWalkToHome(this, play);
        } else{
            EnRd_SetupWalkToPlayer(this, play);
        }

        this->unk_31D = 0xFF;
    }
}
void EnRd_SetupResurrect(EnRd* this) {
    this->actor.flags |= ACTOR_FLAG_0;
    this->actor.colChkInfo.health = 4;
    Audio_PlayActorSfx2(&this->actor, NA_SE_EN_REDEAD_CRY);
    Animation_Change(&this->skelAnime, &gRedeadObj_AnimDying,  -0.8f, this->skelAnime.curFrame, 0.0f, ANIMMODE_ONCE, -6.0f);

    EnRd_SetupAction(this, EnRd_Resurrect);
}

void EnRd_Resurrect(EnRd* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    if (SkelAnime_Update(&this->skelAnime)) {       
        if ((this->actor.xzDistToPlayer <= 150.0f) && (this->actor.yDistToPlayer <= 45.0f)) {
            EnRd_SetupWalkToPlayer(this,play);
        }else if (this->actor.params == REDEAD_TYPE_CRYING) {
            EnRd_SetupCrouch(this);
        }else{
            EnRd_SetupIdle(this);
        }
    }
}

void EnRd_SetupDead(EnRd* this) {
    Animation_MorphToPlayOnce(&this->skelAnime, &gRedeadObj_AnimDying, -1.0f);
    this->action = REDEAD_ACTION_DEAD;
    if(this->realDead == 1){
        this->fireTimer = this->timer = 150; //original was 300
    }
    else {
        this->timer = 600;
    }
    this->actor.flags &= ~ACTOR_FLAG_0;
    this->actor.speedXZ = 0.0f;
    if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
        this->actor.home.pos = this->actor.world.pos;
    }
    Audio_PlayActorSfx2(&this->actor, NA_SE_EN_REDEAD_DEAD);
    EnRd_SetupAction(this, EnRd_Dead);
}


void EnRd_Dead(EnRd* this, PlayState* play) {
    /*if (this->actor.category != ACTORCAT_PROP) {
        Actor_ChangeCategory(play, &play->actorCtx, &this->actor, ACTORCAT_PROP);
    }*/
    Math_SmoothStepToS(&this->headYRotation, 0, 1, 0x7D0, 0);
    Math_SmoothStepToS(&this->upperBodyYRotation, 0, 1, 0x7D0, 0);
    if (this->realDead == 1) {
        if (this->actor.category != ACTORCAT_PROP) {
            Actor_ChangeCategory(play, &play->actorCtx, &this->actor, ACTORCAT_PROP);
        }
        if (SkelAnime_Update(&this->skelAnime)) {
                if (!Flags_GetSwitch(play, this->rdFlags & 0x7F)&& ((this->rdFlags & 0x7F) != 0x0)) {
                    Flags_SetSwitch(play, this->rdFlags & 0x7F);
                }
            if (this->timer == 0) {
                if (this->alpha != 0) {
                    // if(this->alpha == 180) {
                    //     EnRd_UpdateMourningTarget(play, &this->actor, false);
                    // }
                    this->actor.scale.y -= 0.000075f;
                    this->alpha -= 5;
                }else{
                    Actor_Kill(&this->actor);
                }
            }else {
                this->timer--;
            }
        }
        else if (((s32)this->skelAnime.curFrame == 33) || ((s32)this->skelAnime.curFrame == 40)) {
            Audio_PlayActorSfx2(&this->actor, NA_SE_EN_RIZA_DOWN);
        }
    }
    else {
        if(SkelAnime_Update(&this->skelAnime)){
            if (this->timer == 0) {
                EnRd_SetupResurrect(this);
                /*if (!Flags_GetSwitch(play, this->rdFlags & 0x7F)) 
                    Flags_SetSwitch(play, this->rdFlags & 0x7F);
                }*/
            }else {
                this->timer--;
            }
        }else if(((s32)this->skelAnime.curFrame == 33) || ((s32)this->skelAnime.curFrame == 40)) {
            Audio_PlayActorSfx2(&this->actor, NA_SE_EN_RIZA_DOWN);
        }
    }
}

void EnRd_SetupStunned(EnRd* this) {
    this->action = REDEAD_ACTION_STUNNED;
    this->actor.speedXZ = 0.0f;
    this->actor.world.rot.y = this->actor.shape.rot.y;
    if (gSaveContext.sunsSongState != SUNSSONG_INACTIVE) {
        this->stunnedBySunsSong = true;
        this->sunsSongStunTimer = 300;
        //Audio_PlayActorSfx2(&this->actor, NA_SE_EN_LIGHT_ARROW_HIT - SFX_FLAG);
        if(gSaveContext.sunsSongState == SUNSSONG_SPECIAL){
             Audio_PlayActorSfx2(&this->actor, NA_SE_EN_LIGHT_ARROW_HIT - SFX_FLAG);
        }
        Actor_SetColorFilter(&this->actor, -0x8000, -0x7F38, 0, 0xFF);
        //gSaveContext.sunsSongState = SUNSSONG_INACTIVE;
    } else if (this->damageEffect == REDEAD_DMGEFF_HOOKSHOT) {
        Actor_SetColorFilter(&this->actor, 0, 0xC8, 0, 0x50);
    } else {
        Audio_PlayActorSfx2(&this->actor, NA_SE_EN_LIGHT_ARROW_HIT);
        Actor_SetColorFilter(&this->actor, -0x8000, 0xC8, 0, 0x50);
    }
    EnRd_SetupAction(this, EnRd_Stunned);
}


void EnRd_Stunned(EnRd* this, PlayState* play) {
    if(gSaveContext.sunsSongState == SUNSSONG_SPECIAL){
        EnRd_SetupStunned(this);
    }
    if (this->stunnedBySunsSong && (this->sunsSongStunTimer != 0)) {
        this->sunsSongStunTimer--;
        if (this->sunsSongStunTimer >= 150 ) {
            Actor_SetColorFilter(&this->actor, -0x8000, 0xC8, 0, 0xFF);
        }
    }
    if (this->actor.colorFilterTimer == 0) {
        this->stunnedBySunsSong = false;
        if (this->actor.colChkInfo.health == 0) {
            //EnRd_UpdateMourningTarget(play, &this->actor, true);
            EnRd_SetupDead(this);
            //Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos, 0x90);
        } else {
            EnRd_SetupDamaged(this);
        }
    }
}

void EnRd_TurnTowardsPlayer(EnRd* this, PlayState* play) {
    s16 headAngleTemp = this->actor.yawTowardsPlayer - (s16)(this->actor.shape.rot.y + this->upperBodyYRotation);
    s16 upperBodyAngle = CLAMP(headAngleTemp, -500, 500);
    s16 headAngle;

    headAngleTemp -= this->headYRotation;
    headAngle = CLAMP(headAngleTemp, -500, 500);

    if ((s16)(this->actor.yawTowardsPlayer - this->actor.shape.rot.y) >= 0) {
        this->upperBodyYRotation += ABS(upperBodyAngle);
        this->headYRotation += ABS(headAngle);
    } else {
        this->upperBodyYRotation -= ABS(upperBodyAngle);
        this->headYRotation -= ABS(headAngle);
    }

    this->upperBodyYRotation = CLAMP(this->upperBodyYRotation, -0x495F, 0x495F);
    this->headYRotation = CLAMP(this->headYRotation, -0x256F, 0x256F);
}

void EnRd_UpdateDamage(EnRd* this, PlayState* play) {
    s32 pad;
    Actor* actor;
    Vec3f hitPos;
    Player* player = GET_PLAYER(play);
    if ((gSaveContext.sunsSongState != SUNSSONG_INACTIVE) && (this->actor.shape.rot.x == 0) &&
        !this->stunnedBySunsSong && (this->action != REDEAD_ACTION_DAMAGED) && (this->action != REDEAD_ACTION_DEAD) &&
        (this->action != REDEAD_ACTION_STUNNED)) {
        EnRd_SetupStunned(this);
        return;
    }

    if(this->fireTimer>0){
        if(this->fireDamageTimer > 0){
            this->fireDamageTimer--;
        }else{
            if(this->actor.colChkInfo.health >= 2 ){
                this->actor.colChkInfo.health--; 
                EnRd_SetupDamaged(this);
                
            }else if(this->action != REDEAD_ACTION_DEAD){
                 //EnRd_UpdateMourningTarget(play, &this->actor, true);
                    this->realDead = 1;
                    EnRd_SetupDead(this);
                    Item_DropCollectibleRandom(play, 0, &this->actor.world.pos, 0x90);
                    
            }
            this->fireDamageTimer = 30;
        }
    }else{
        this->fireDamageTimer = 30;
    }

    if (this->collider.base.acFlags & AC_HIT) {
        this->collider.base.acFlags &= ~AC_HIT;
        this->damageEffect = this->actor.colChkInfo.damageEffect;
        if (this->action != REDEAD_ACTION_RISE_FROM_COFFIN) {
            Actor_SetDropFlag(&this->actor, &this->collider.info, true);
            if (player->unk_844 != 0) {
                this->unk_31D = player->unk_845;
            }
            if(this->damageEffect ==  REDEAD_DMGEFF_NONE){
                hitPos.x = this->collider.info.bumper.hitPos.x;
                hitPos.y = this->collider.info.bumper.hitPos.y;
                hitPos.z = this->collider.info.bumper.hitPos.z;
                CollisionCheck_SpawnShieldParticlesMetal2(play, &hitPos);
            }else if (this->damageEffect != REDEAD_DMGEFF_ICE_MAGIC) {
                if (((this->damageEffect == REDEAD_DMGEFF_HOOKSHOT) ||
                     (this->damageEffect == REDEAD_DMGEFF_LIGHT_MAGIC)) &&
                    (this->action != REDEAD_ACTION_STUNNED)) {
                    Actor_ApplyDamage(&this->actor);
                    EnRd_SetupStunned(this);
                    return;
                }

                this->stunnedBySunsSong = false;
                this->sunsSongStunTimer = 0;

                if (this->damageEffect == REDEAD_DMGEFF_FIRE_MAGIC) {
                    Actor_SetColorFilter(&this->actor, 0x4000, 0xFF, 0, 0x50);
                    if (this->actor.params <= REDEAD_TYPE_GIBDO) {
                         this->fireTimer = 80;
                    }else{
                        this->fireTimer = 80;//original was 40;
                    }
                } else {
                    Actor_SetColorFilter(&this->actor, 0x4000, 0xFF, 0, 8);
                }

                Actor_ApplyDamage(&this->actor);
                if ((this->actor.colChkInfo.health == 0 )&& (this->fireTimer > 0)) {
                    //EnRd_UpdateMourningTarget(play, &this->actor, true);
                    this->realDead = 1;
                    EnRd_SetupDead(this);
                    Item_DropCollectibleRandom(play, 0, &this->actor.world.pos, 0x90);
                } else if(this->actor.colChkInfo.health == 0){
                    this->realDead = 0;
                    EnRd_SetupDead(this); 
                    
                }
                else {
                    EnRd_SetupDamaged(this);
                }

            }

        }
    }
    

}

void EnRd_Update(Actor* thisx, PlayState* play) {
    s32 pad;
    EnRd* this = (EnRd*)thisx;
    Player* player = GET_PLAYER(play);
    s32 pad2;

    EnRd_UpdateDamage(this, play);

    // if (gSaveContext.sunsSongState != SUNSSONG_INACTIVE && !this->stunnedBySunsSong) {
    //     gSaveContext.sunsSongState = SUNSSONG_INACTIVE;
    // }

    if (this->damageEffect != REDEAD_DMGEFF_ICE_MAGIC &&
        ((this->action != REDEAD_ACTION_RISE_FROM_COFFIN) || (this->damageEffect != REDEAD_DMGEFF_FIRE_MAGIC))) {
        if (this->playerStunWaitTimer != 0) {
            this->playerStunWaitTimer--;
        }

        this->actionFunc(this, play);
        if (this->action != REDEAD_ACTION_GRAB && this->actor.speedXZ != 0.0f) {
            Actor_MoveForward(&this->actor);
        }

        if ((this->actor.shape.rot.x == 0) && (this->action != REDEAD_ACTION_GRAB) /*&& (this->actor.speedXZ != 0.0f)*/) {
            Actor_UpdateBgCheckInfo(play, &this->actor, 30.0f, 20.0f, 35.0f,
                                    UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2 | UPDBGCHECKINFO_FLAG_3 |
                                        UPDBGCHECKINFO_FLAG_4);
        }

        if (this->action == REDEAD_ACTION_ATTEMPT_PLAYER_FREEZE) {
            EnRd_TurnTowardsPlayer(this, play);
        }
        if (this->actor.params == REDEAD_TYPE_INVISIBLE || this->actor.home.rot.z != 0) {
            if (play->actorCtx.lensActive && (this->action != REDEAD_ACTION_DEAD)) {
                this->actor.flags |= ACTOR_FLAG_0 ;
                this->actor.shape.shadowDraw = ActorShadow_DrawFeet;
            } else {
                this->actor.flags &= ~(ACTOR_FLAG_0);
                this->actor.shape.shadowDraw = NULL;
            }
        }
    }

    this->actor.focus.pos = this->actor.world.pos;
    this->actor.focus.pos.y += 50.0f;

    if ((this->actor.colChkInfo.health > 0) && (this->action != REDEAD_ACTION_GRAB)) {
        Collider_UpdateCylinder(&this->actor, &this->collider);
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
        if ((this->action != REDEAD_ACTION_DAMAGED) || ((player->unk_844 != 0) && (player->unk_845 != this->unk_31D))) {
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
        }
    }

    if((player->actor.freezeTimer > 0) || (this->action == REDEAD_ACTION_GRAB) ){
        SoundEffect_PlayHeld(SOUND_VIOLIN_TENSE, 1.0f,1.1f,1.0,&this->actor.world.pos,1.0f,1000.0f, &this->actor);
    }
}

s32 EnRd_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx, Gfx** gfx) {
    EnRd* this = (EnRd*)thisx;

    if (limbIndex == REDEAD_GIBDO_LIMB_HEAD_ROOT) {
        rot->y += this->headYRotation;
    } else if (limbIndex == REDEAD_GIBDO_LIMB_UPPER_BODY_ROOT) {
        rot->y += this->upperBodyYRotation;
    }

    return false;
}

void EnRd_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx, Gfx** gfx) {
    Vec3f D_80AE4940 = { 300.0f, 0.0f, 0.0f };
    EnRd* this = (EnRd*)thisx;
    s32 idx = -1;
    Vec3f destPos;

    if ((this->fireTimer != 0) || ((this->actor.colorFilterTimer != 0) && (this->actor.colorFilterParams & 0x4000))) {
        switch (limbIndex) {
            case REDEAD_GIBDO_LIMB_HEAD:
                idx = 0;
                break;

            case REDEAD_GIBDO_LIMB_ROOT:
                idx = 1;
                break;

            case REDEAD_GIBDO_LIMB_RIGHT_HAND:
                idx = 2;
                break;

            case REDEAD_GIBDO_LIMB_LEFT_HAND:
                idx = 3;
                break;

            case REDEAD_GIBDO_LIMB_TORSO:
                idx = 4;
                break;

            case REDEAD_GIBDO_LIMB_PELVIS:
                idx = 5;
                break;

            case REDEAD_GIBDO_LIMB_RIGHT_SHIN:
                idx = 6;
                break;

            case REDEAD_GIBDO_LIMB_LEFT_SHIN:
                idx = 7;
                break;

            case REDEAD_GIBDO_LIMB_RIGHT_FOOT:
                idx = 8;
                break;

            case REDEAD_GIBDO_LIMB_LEFT_FOOT:
                idx = 9;
                break;
        }

        if (idx >= 0) {
            Matrix_MultVec3f(&D_80AE4940, &destPos);
            this->firePos[idx].x = destPos.x;
            this->firePos[idx].y = destPos.y;
            this->firePos[idx].z = destPos.z;
        }
    }
}

void EnRd_Draw(Actor* thisx, PlayState* play) {
    static Vec3f D_80AE494C = { 300.0f, 0.0f, 0.0f };
    static Vec3f sShadowScale = { 0.25f, 0.25f, 0.25f };
    Player* player = GET_PLAYER(play);
    s32 pad;
    EnRd* this = (EnRd*)thisx;
    Vec3f thisPos = thisx->world.pos;
    OPEN_DISPS(play->state.gfxCtx, "../z_en_rd.c", 1679);

    if ((this->alpha == 255 && (this->actor.params != REDEAD_TYPE_INVISIBLE && this->actor.home.rot.z == 0))|| (this->stunnedBySunsSong)) {
        Gfx_SetupDL_25Opa(play->state.gfxCtx);
        gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, this->alpha);
        gSPSegment(POLY_OPA_DISP++, 8, &D_80116280[2]);
        gSPSegment(POLY_OPA_DISP++, 0x08, Gfx_TexScroll(play->state.gfxCtx,/* scroll */ 0, this->scrollY,/* dim*/ 32, 32));    
        if(!(this->stunnedBySunsSong)){
            this->scrollY += -4;
         }
        POLY_OPA_DISP =
            SkelAnime_DrawFlex(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount,
                               EnRd_OverrideLimbDraw, EnRd_PostLimbDraw, this, POLY_OPA_DISP);
                               
        func_80033C30(&thisPos, &sShadowScale, 255, play);

        if (this->fireTimer != 0) {
            thisx->colorFilterTimer = this->fireTimer;
            this->fireTimer--;
            if (this->fireTimer % 4 == 0) {
                EffectSsEnFire_SpawnVec3s(play, thisx, &this->firePos[this->fireTimer >> 2], 0x4B, 0, 0,
                                          (this->fireTimer >> 2));
            }
        }
    } else {
        Gfx_SetupDL_25Xlu(play->state.gfxCtx);
        gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, this->alpha);
        gSPSegment(POLY_XLU_DISP++, 8, &D_80116280[0]);
        gSPSegment(POLY_XLU_DISP++, 0x08, Gfx_TexScroll(play->state.gfxCtx,/* scroll */ 0, this->scrollY,/* dim*/ 32, 32));    
        if(!(this->stunnedBySunsSong)){
            this->scrollY += -4;
         }
        POLY_XLU_DISP =
            SkelAnime_DrawFlex(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount,
                               EnRd_OverrideLimbDraw, NULL, this, POLY_XLU_DISP);

        func_80033C30(&thisPos, &sShadowScale, this->alpha, play);
    }
    CLOSE_DISPS(play->state.gfxCtx, "../z_en_rd.c", 1735);
}
