#include <uLib.h>
#include "z_en_dh.h"
#include "overlays/actors/ovl_En_Dha/z_en_dha.h"
#include "assets/objects/object_dh/object_dh.h"

#define FLAGS (ACTOR_FLAG_0 | ACTOR_FLAG_2 | ACTOR_FLAG_4 | ACTOR_FLAG_10)


typedef enum {
    /* 0 */ DH_WAIT,
    /* 1 */ DH_RETREAT,
    /* 2 */ DH_BURROW,
    /* 3 */ DH_WALK,
    /* 4 */ DH_ATTACK,
    /* 5 */ DH_DEATH,
    /* 6 */ DH_DAMAGE,
    /* 7 */ DH_KILLARMS
} EnDhAction;

typedef enum {
    ENDHA_SEPERATE,
    ENDHA_BACKLEFT,
    ENDHA_DEAD,
    ENDHA_BACKRIGHT
} EnDhaParams;

void EnDh_Init(Actor* thisx, PlayState* play);
void EnDh_Destroy(Actor* thisx, PlayState* play);
void EnDh_Update(Actor* thisx, PlayState* play);
void EnDh_Draw(Actor* thisx, PlayState* play);

void EnDh_SetupWait(EnDh* this, PlayState* play);
void EnDh_SetupWalk(EnDh* this);
void EnDh_SetupAttack(EnDh* this, PlayState* play);
void EnDh_SetupBurrow(EnDh* this, PlayState* play);
Actor* Actor_FindBackArms(PlayState* play, Actor* refActor, s16 armParams) ;
void EnDh_Wait(EnDh* this, PlayState* play);
void EnDh_Walk(EnDh* this, PlayState* play);
void EnDh_Retreat(EnDh* this, PlayState* play);
void EnDh_Attack(EnDh* this, PlayState* play);
void EnDh_Burrow(EnDh* this, PlayState* play);
void EnDh_Damage(EnDh* this, PlayState* play);
void EnDh_Death(EnDh* this, PlayState* play);
void EnDh_DirtShield(EnDh* this, PlayState* play);
void EnDh_SetupDirtShield(EnDh* this, PlayState* play);
void EnDh_CloseDirtShield(EnDh* this, PlayState* play);
u8 EnDh_checkBothBackArms(EnDh* this, PlayState* play);
const ActorInit En_Dh_InitVars = {
    ACTOR_EN_DH,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_DH,
    sizeof(EnDh),
    (ActorFunc)EnDh_Init,
    (ActorFunc)EnDh_Destroy,
    (ActorFunc)EnDh_Update,
    (ActorFunc)EnDh_Draw,
};

static ColliderCylinderInit deadCylinderInit = {
    {
        COLTYPE_HIT0,
        AT_NONE,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_PLAYER,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x00000008, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 35, 70, 0, { 0, 0, 0 } },
};

static ColliderJntSphElementInit deadElementsInit[1] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_ON | OCELEM_UNK3,
        },
        { 1, { { 0, 0, 0 }, 20 }, 100 },
    },
};

static ColliderJntSphInit deadCollider = {
    {
        COLTYPE_HIT6,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_PLAYER,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    1,
    deadElementsInit,
};

static DamageTable deadDamageTablet = {
    /* Deku nut      */ DMG_ENTRY(0, 0x0),
    /* Deku stick    */ DMG_ENTRY(0, 0x0),
    /* Slingshot     */ DMG_ENTRY(0, 0x0),
    /* Explosive     */ DMG_ENTRY(0, 0x0),
    /* Boomerang     */ DMG_ENTRY(0, 0x0),
    /* Normal arrow  */ DMG_ENTRY(0, 0x0),
    /* Hammer swing  */ DMG_ENTRY(6, 0xF),
    /* Hookshot      */ DMG_ENTRY(0, 0x0),
    /* Kokiri sword  */ DMG_ENTRY(2, 0xF),
    /* Master sword  */ DMG_ENTRY(2, 0xF),
    /* Giant's Knife */ DMG_ENTRY(4, 0xF),
    /* Fire arrow    */ DMG_ENTRY(4, 0xF),
    /* Ice arrow     */ DMG_ENTRY(0, 0x0),
    /* Light arrow   */ DMG_ENTRY(0, 0x0),
    /* Unk arrow 1   */ DMG_ENTRY(0, 0x0),
    /* Unk arrow 2   */ DMG_ENTRY(0, 0x0),
    /* Unk arrow 3   */ DMG_ENTRY(0, 0x0),
    /* Fire magic    */ DMG_ENTRY(4, 0xF),
    /* Ice magic     */ DMG_ENTRY(0, 0x0),
    /* Light magic   */ DMG_ENTRY(0, 0x0),
    /* Shield        */ DMG_ENTRY(0, 0x0),
    /* Mirror Ray    */ DMG_ENTRY(0, 0x0),
    /* Kokiri spin   */ DMG_ENTRY(2, 0xF),
    /* Giant spin    */ DMG_ENTRY(2, 0xF),
    /* Master spin   */ DMG_ENTRY(1, 0xF),
    /* Kokiri jump   */ DMG_ENTRY(4, 0xF),
    /* Giant jump    */ DMG_ENTRY(8, 0xF),
    /* Master jump   */ DMG_ENTRY(4, 0xF),
    /* Unknown 1     */ DMG_ENTRY(0, 0x0),
    /* Unblockable   */ DMG_ENTRY(0, 0x0),
    /* Hammer jump   */ DMG_ENTRY(12, 0xF),
    /* Unknown 2     */ DMG_ENTRY(0, 0x0),
};

static InitChainEntry deadInitChain[] = {
    ICHAIN_S8(naviEnemyId, NAVI_ENEMY_DEAD_HAND, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 2000, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 10, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(gravity, -3500, ICHAIN_STOP),
};

void EnDh_SetupAction(EnDh* this, EnDhActionFunc actionFunc) {
    this->actionFunc = actionFunc;
}

void EnDh_Init(Actor* thisx, PlayState* play) {
    EnDh* this = (EnDh*)thisx;

    Actor_ProcessInitChain(&this->actor, deadInitChain);
    this->actor.colChkInfo.damageTable = &deadDamageTablet;
    SkelAnime_InitFlex(play, &this->skelAnime, &object_dh_Skel_007E88, &object_dh_Anim_005880, this->jointTable,
                       this->limbRotTable, 16);
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 64.0f);
    this->actor.params = ENDH_WAIT_UNDERGROUND;
    this->actor.colChkInfo.mass = MASS_HEAVY;

    this->actor.colChkInfo.health = (play->sceneId == 0x48) ? 12 :20;
    this->alpha = this->unk_258 = 255;
    this->actor.flags &= ~ACTOR_FLAG_0;
    Collider_InitCylinder(play, &this->collider1);
    Collider_SetCylinder(play, &this->collider1, &this->actor, &deadCylinderInit);
    Collider_InitJntSph(play, &this->collider2);
    Collider_SetJntSph(play, &this->collider2, &this->actor, &deadCollider, this->elements);
    EnDh_SetupWait(this,play);

}

void EnDh_Destroy(Actor* thisx, PlayState* play) {
    s32 pad;
    EnDh* this = (EnDh*)thisx;

    func_800F5B58();
    Collider_DestroyCylinder(play, &this->collider1);
    Collider_DestroyJntSph(play, &this->collider2);
}

void EnDh_SpawnDebris(PlayState* play, EnDh* this, Vec3f* spawnPos, f32 spread, s32 arg4, f32 accelXZ, f32 scale) {
    Vec3f pos;
    Vec3f vel = { 0.0f, 8.0f, 0.0f };
    Vec3f accel = { 0.0f, -1.5f, 0.0f };
    f32 spreadAngle;
    f32 scaleMod;

    spreadAngle = (Rand_ZeroOne() - 0.5f) * 6.28f;
    pos.y = this->actor.floorHeight;
    pos.x = (Math_SinF(spreadAngle) * spread) + spawnPos->x;
    pos.z = (Math_CosF(spreadAngle) * spread) + spawnPos->z;
    accel.x = (Rand_ZeroOne() - 0.5f) * accelXZ;
    accel.z = (Rand_ZeroOne() - 0.5f) * accelXZ;
    vel.y += (Rand_ZeroOne() - 0.5f) * 4.0f;
    scaleMod = (Rand_ZeroOne() * 5.0f) + 12.0f;
    EffectSsHahen_Spawn(play, &pos, &vel, &accel, arg4, scaleMod * scale, -1, 10, NULL);

}

void EnDh_SetupWait(EnDh* this, PlayState* play) {
    Animation_PlayLoop(&this->skelAnime, &object_dh_Anim_003A8C);
    this->curAction = DH_WAIT;
    this->actor.world.pos.x = Rand_CenteredFloat(600.0f) + this->actor.home.pos.x;
    this->actor.world.pos.z = Rand_CenteredFloat(600.0f) + this->actor.home.pos.z;
    this->actor.shape.yOffset = -15000.0f;
    this->dirtWaveSpread = this->actor.speedXZ = 0.0f;
    this->actor.world.rot.y = this->actor.shape.rot.y;
    this->actor.flags |= ACTOR_FLAG_7;
    this->dirtWavePhase = this->actionState = this->actor.params = ENDH_WAIT_UNDERGROUND;
    this->dirtShieldFlag = 0;
    this->dirtShieldTimer = 120;
    if(Actor_FindBackArms(play,&this->actor,ENDHA_BACKLEFT) != NULL){//kill its arms on back
        Actor_Kill(Actor_FindBackArms(play,&this->actor,ENDHA_BACKLEFT));
    }
    if(Actor_FindBackArms(play,&this->actor,ENDHA_BACKRIGHT) != NULL){
        Actor_Kill(Actor_FindBackArms(play,&this->actor,ENDHA_BACKRIGHT));
    }
    if (this->attachedActor[0] != NULL) {
        this->attachedActor[0] = NULL;
    }
    if ((this->attachedActor[1] != NULL)){
        this->attachedActor[1] = NULL;
    }
    Actor_Spawn(&play->actorCtx, play, ACTOR_EN_DHA, this->actor.world.pos.x, this->actor.world.pos.y,
    this->actor.world.pos.z, 0, 0, 0, ENDHA_SEPERATE);
    this->ArmspawnCounter++;

    EnDh_SetupAction(this, EnDh_Wait);
}

void EnDh_Wait(EnDh* this, PlayState* play) {
    if ((this->ArmspawnCounter >= 10)) {
        this->actor.params = ENDH_KILLTHEMALL;
        if ((Actor_FindNearby(play, &this->actor, ACTOR_EN_DHA, ACTORCAT_ENEMY, 5000.0f) != NULL)) {
            Actor_Kill(Actor_FindNearby(play, &this->actor, ACTOR_EN_DHA, ACTORCAT_ENEMY, 5000.0f));    
        }
        if ((Actor_FindNearby(play, &this->actor, ACTOR_EN_DHA, ACTORCAT_ENEMY, 5000.0f) == NULL)) {
            this->ArmspawnCounter = 0;
            EnDh_SetupAction(this, EnDh_SetupWait);
        }

    }
    if ((s32)this->skelAnime.curFrame == 5) {
        func_800F5ACC(0x1B);
    }
    if (Actor_GetCollidedExplosive(play, &this->collider1.base)|| (this->BurrowFlag == true)) {
        this->actor.params = ENDH_START_ATTACK_BOMB;
    }
    if ((this->actor.params >= ENDH_START_ATTACK_GRAB) || (this->actor.params <= ENDH_HANDS_KILLED_4)) {
        switch (this->actionState) {
            case DH_WAIT:
                this->actor.shape.rot.y = this->actor.yawTowardsPlayer;
                this->actor.flags &= ~ACTOR_FLAG_7;
                this->actionState++;
                this->drawDirtWave++;
                Audio_PlayActorSfx2(&this->actor, NA_SE_EN_DEADHAND_HIDE);             
                FALLTHROUGH;
            case DH_RETREAT:
                this->dirtWavePhase += 0x3A7;
                Math_SmoothStepToF(&this->dirtWaveSpread, 300.0f, 1.0f, 5.0f, 0.0f);
                this->dirtWaveHeight = Math_SinS(this->dirtWavePhase) * 55.0f;
                this->dirtWaveAlpha = (s16)(Math_SinS(this->dirtWavePhase) * 255.0f);
                EnDh_SpawnDebris(play, this, &this->actor.world.pos, this->dirtWaveSpread, 4, 2.05f, 1.2f);

                if (this->actor.shape.yOffset == 0.0f) {
                    this->drawDirtWave = false;
                    this->actionState++;
                } else if (this->dirtWavePhase > 0x12C0) {
                    this->actor.shape.yOffset += 500.0f;
                }
                break;
            case DH_BURROW:
            
                EnDh_SetupWalk(this);
                break;
        }
        Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 0x7D0, 0);
        SkelAnime_Update(&this->skelAnime);
        if (this->actor.params != ENDH_START_ATTACK_BOMB) {
            func_8008EEAC(play, &this->actor);
        }
    }
}

void EnDh_SetupWalk(EnDh* this) {
    this->actor.flags |= ACTOR_FLAG_0;
    Animation_Change(&this->skelAnime, &object_dh_Anim_003A8C, 1.0f, 0.0f,
                     Animation_GetLastFrame(&object_dh_Anim_003A8C) - 3.0f, ANIMMODE_LOOP, -6.0f);
    this->curAction = DH_WALK;
    this->timer = 450;
   
    this->actor.speedXZ = 1.0f;

    EnDh_SetupAction(this, EnDh_Walk);

}

void EnDh_Walk(EnDh* this, PlayState* play) {
   
    if (this->dirtShieldFlag == 0) {
        Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 0x15E/*0xFA*/, 0);
    }
    else {
        Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 0xBB8, 0);
    }
    this->actor.world.rot.y = this->actor.shape.rot.y;
    SkelAnime_Update(&this->skelAnime);
    if (this->spawnBackArms == 0 )
    {
        this->attachedActor[0]  = Actor_Spawn(&play->actorCtx, play, ACTOR_EN_DHA, this->actor.world.pos.x, this->actor.world.pos.y,
            this->actor.world.pos.z, 0, 0, 0, ENDHA_BACKLEFT);//0:Left

        this->attachedActor[1]  = Actor_Spawn(&play->actorCtx, play, ACTOR_EN_DHA, this->actor.world.pos.x, this->actor.world.pos.y,
            this->actor.world.pos.z, 0, 0, 0, ENDHA_BACKRIGHT);//1:Right*/
        this->spawnBackArms = 1;
    }

    if (((this->spawnBackArms == 1) && (this->attachedActor[0] == NULL) && (this->attachedActor[1] == NULL)) || (this->BurrowFlag == true)) {
        this->skelAnime.playSpeed = 2.0f;
        this->actor.speedXZ = 2.0f; 
        if (this->dirtShieldFlag == 0){
        
            EnDh_SetupDirtShield(this, play);
         }
        else {
            EnDh_DirtShield(this, play);
        }
        if (this->BurrowFlag == false) {
            this->BurrowFlag = true;
            EnDh_SetupAction(this, EnDh_SetupBurrow);

        }

    }
    else {
        this->skelAnime.playSpeed = 1.5f;
        this->actor.speedXZ = 1.5f;
    }

    if (((s32)this->skelAnime.curFrame % 8) == 0) {
        Audio_PlayActorSfx2(&this->actor, NA_SE_EN_DEADHAND_WALK);
    }
    if ((play->gameplayFrames & 0x5F) == 0) {
        Audio_PlayActorSfx2(&this->actor, NA_SE_EN_DEADHAND_LAUGH);
    }
    if (this->dirtShieldFlag != 0) {
        

        if (this->dirtShieldTimer <= 0) {
            if (this->actor.xzDistToPlayer >= 300.0f|| (this->actor.xzDistToPlayer <=100.0f ) ) {
                EnDh_SetupBurrow(this, play);
            }
        }
        else if ((this->actor.xzDistToPlayer <= 100.0f) && (this->actor.xzDistToPlayer > 80.0f)){
         
                if (Actor_IsFacingPlayer(&this->actor, 60 * 0x10000 / 360)) {
                    EnDh_SetupAttack(this, play);
                }
        }
        
   }
    else if (this->actor.xzDistToPlayer <= 100.0f) {
            this->actor.speedXZ = 0.0f;
            if (Actor_IsFacingPlayer(&this->actor, 60 * 0x10000 / 360)) {
                EnDh_SetupAttack(this, play);
            }
    }
     else if (--this->timer == 0) {
            EnDh_SetupBurrow(this, play);
    }
        
    
}

void EnDh_SetupRetreat(EnDh* this, PlayState* play) {
    Animation_MorphToLoop(&this->skelAnime, &object_dh_Anim_005880, -4.0f);
    this->curAction = DH_RETREAT;
    this->timer = 35;
    this->skelAnime.playSpeed = 2.5f;
    this->actor.speedXZ = 3.0f;
    this->BurrowFlag = false;
    EnDh_SetupAction(this, EnDh_Retreat);
}

void EnDh_Retreat(EnDh* this, PlayState* play) {
    this->timer--;
    if (this->timer == 0) {
        this->retreat = false;
        EnDh_SetupBurrow(this,play);
    } else {
        Math_SmoothStepToS(&this->actor.shape.rot.y, (s16)(this->actor.yawTowardsPlayer + 0x8000), 1, 0xBB8, 0);
    }
    this->actor.world.rot.y = this->actor.shape.rot.y;
    SkelAnime_Update(&this->skelAnime);
}

void EnDh_SetupAttack(EnDh* this, PlayState* play) {

   
    if (this->actor.params == ENDH_START_ATTACK_GRAB)
    {
        Animation_Change(&this->skelAnime, &object_dh_Anim_004658, 0.7f, 0.0f, Animation_GetLastFrame(&object_dh_Anim_004658),ANIMMODE_ONCE, -6.0f);
        Audio_PlayActorSfx2(&this->actor, NA_SE_EN_DEADHAND_LAUGH);
    }
    else {
        Animation_MorphToPlayOnce(&this->skelAnime, &object_dh_Anim_004658, -6.0f);
    }

    if (EnDh_checkBothBackArms(this,play) == 0) {
        this->skelAnime.playSpeed = 2.0f;
        this->actor.speedXZ = 2.0f;
        if (this->BurrowFlag == false) {
            this->BurrowFlag = true;
            EnDh_SetupAction(this, EnDh_SetupBurrow);

        }

    }
    else {
        this->skelAnime.playSpeed = 1.0f;
        this->actor.speedXZ = 1.0f;
    }
    this->timer = this->actionState = DH_WAIT;
    this->curAction = DH_ATTACK;
    this->actor.speedXZ = 0.0f;
   
    EnDh_SetupAction(this, EnDh_Attack);
}

void EnDh_Attack(EnDh* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 pad;  
    this->actor.speedXZ = 0.0f;
    if (EnDh_checkBothBackArms(this,play) == 0) {     
        if (this->BurrowFlag == false) {
            this->BurrowFlag = true;
            EnDh_SetupAction(this, EnDh_SetupBurrow);

        }
    }
    if (SkelAnime_Update(&this->skelAnime)) {

            this->actionState++;    
       
    }
    /*else if ((this->actor.params == ENDH_START_ATTACK_GRAB) && (this->actor.xzDistToPlayer <= 150.0f)) {
        Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 0x5DC, 0);
        /*Animation_Change(&this->skelAnime, &object_dh_Anim_003D6C, -1.0f, this->skelAnime.curFrame, 0.0F, ANIMMODE_ONCE,
            -6.0f);
        this->actionState = DH_ATTACK;
        EnDh_SetupAction(this, EnDh_Attack);
    } */
    else if (((this->actor.xzDistToPlayer > 140.0f) || (!Actor_IsFacingPlayer(&this->actor, 60 * 0x10000 / 360))))
     {//lift its head

        Animation_Change(&this->skelAnime, &object_dh_Anim_004658, -1.0f, this->skelAnime.curFrame, 0.0f, ANIMMODE_ONCE,
                         -4.0f);
        this->actionState = DH_ATTACK;
        this->collider2.base.atFlags = this->collider2.elements[0].info.toucherFlags = AT_NONE; // also TOUCH_NONE
        this->collider2.elements[0].info.toucher.dmgFlags = this->collider2.elements[0].info.toucher.damage = 0;

    }

    if (this->dirtShieldFlag != 0) {
        if (this->dirtShieldTimer <= 0) {
            this->actionState = DH_DEATH;
        }
    }

    if ((EnDh_checkBothBackArms(this,play) == 0) || (this->BurrowFlag == true)) {
        if (this->dirtShieldFlag == 0)
        {
            EnDh_SetupDirtShield(this, play);
        }
        else {
            EnDh_DirtShield(this, play);
        }
    }
    else {
        EnDh_CloseDirtShield(this, play);
    }
  

    switch (this->actionState) {
        case DH_RETREAT:
            if ((this->actor.params == ENDH_START_ATTACK_GRAB )&&(EnDh_checkBothBackArms(this,play) != 0)&& (this->actor.xzDistToPlayer >= 60.0f)) {
                Animation_Change(&this->skelAnime, &object_dh_Anim_003A8C /* & object_dh_Anim_004658*/, -1.6f, this->skelAnime.curFrame, 0.0f, ANIMMODE_LOOP,
                    -6.0f);
                Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 0x5DC, 0);
                Audio_PlayActorSfx2(&this->actor, NA_SE_EN_DEADHAND_LAUGH);
                
                //Animation_GetLastFrame(&object_dh_Anim_004658)
                /*if (this->actor.xzDistToPlayer >= 10.0f) {
                    Math_SmoothStepToS(&this->actor.shape.rot.y, -this->actor.yawTowardsPlayer, 1, 0x5DC, 0);
                    this->actor.speedXZ = 3.0f;
                }
                else{ this->actor.speedXZ = 0.0f; }*/
                
                     //Math_SmoothStepToS(&this->actor.shape.rot.y, (s16)(this->actor.yawTowardsPlayer + 0x8000), 1, 0xBB8, 0);
                    //this->dragFlag = 1;
                if (this->actor.xzDistToPlayer >= 60.0f) {
                    
                    this->actor.speedXZ = 1.5f;
                    this->actionState = DH_RETREAT;
                    EnDh_SetupAction(this, EnDh_Attack);
                }
                else {
                    this->actor.speedXZ = 0.5f;
                    EnDh_SetupAction(this, EnDh_SetupAttack);
                }
                   
                
              
                
            }
            else {
                Animation_PlayOnce(&this->skelAnime, &object_dh_Anim_001A3C);
                this->actionState++;
                Audio_PlayActorSfx2(&this->actor, NA_SE_EN_DEADHAND_BITE);
            }
               
                FALLTHROUGH;
        case DH_WAIT:
            Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 0x5DC, 0);
            if (((this->actor.xzDistToPlayer > 100.0f) || (!Actor_IsFacingPlayer(&this->actor, 60 * 0x10000 / 360))))
            {//lift its head

                Animation_Change(&this->skelAnime, &object_dh_Anim_004658, -1.0f, this->skelAnime.curFrame, 0.0f, ANIMMODE_ONCE,
                    -4.0f);
                this->actionState = DH_ATTACK;
                this->collider2.base.atFlags = this->collider2.elements[0].info.toucherFlags = AT_NONE; // also TOUCH_NONE
                this->collider2.elements[0].info.toucher.dmgFlags = this->collider2.elements[0].info.toucher.damage = 0;

            }
            break;
        case DH_BURROW:
            if (this->skelAnime.curFrame >= 4.0f) {
                this->collider2.base.atFlags = this->collider2.elements[0].info.toucherFlags =
                    AT_ON | AT_TYPE_ENEMY; // also TOUCH_ON | TOUCH_SFX_WOOD
                this->collider2.elements[0].info.toucher.dmgFlags = DMG_FIRE;
                this->collider2.elements[0].info.toucher.damage = 16;
            }
            if (this->collider2.base.atFlags & AT_BOUNCED) {
                this->collider2.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
                this->collider2.base.atFlags = this->collider2.elements[0].info.toucherFlags =
                    AT_NONE; // also TOUCH_NONE
                this->collider2.elements[0].info.toucher.dmgFlags = this->collider2.elements[0].info.toucher.damage = 0;
                this->actionState++;
            } else if (this->collider2.base.atFlags & AT_HIT) {
                this->collider2.base.atFlags &= ~AT_HIT;
                func_8002F71C(play, &this->actor, 8.0f, this->actor.shape.rot.y, 8.0f);
            }
            break;
        case DH_WALK:
            if ((this->actor.xzDistToPlayer <= 150.0f) && (Actor_IsFacingPlayer(&this->actor, 60 * 0x10000 / 360))) {
                Animation_Change(&this->skelAnime, &object_dh_Anim_004658, 1.0f, 20.0f,
                                 Animation_GetLastFrame(&object_dh_Anim_004658), ANIMMODE_ONCE, -6.0f);
                this->actionState = DH_WAIT;
            } else {
                Animation_Change(&this->skelAnime, &object_dh_Anim_004658, -1.0f,
                                 Animation_GetLastFrame(&object_dh_Anim_004658), 0.0f, ANIMMODE_ONCE, -4.0f);
                this->actionState++;
                this->collider2.base.atFlags = this->collider2.elements[0].info.toucherFlags =
                    AT_NONE; // also TOUCH_NONE
                this->collider2.elements[0].info.toucher.dmgFlags = this->collider2.elements[0].info.toucher.damage = 0;
            }
            break;
        case DH_DEATH:
            EnDh_SetupWalk(this);
            break;
        case DH_ATTACK:
            break;
    }
    this->actor.world.rot.y = this->actor.shape.rot.y;
}

void EnDh_SetupBurrow(EnDh* this,PlayState* play) {
    this->skelAnime.playSpeed = 1.0f;
    Animation_MorphToPlayOnce(&this->skelAnime, &object_dh_Anim_002148, -6.0f);
    this->curAction = DH_BURROW;
    this->dirtWaveSpread = this->actor.speedXZ = 0.0f;
    this->actor.world.rot.y = this->actor.shape.rot.y;
    this->dirtWavePhase = 0;
    this->actionState = DH_WAIT;
    this->actor.flags &= ~ACTOR_FLAG_0;
    Audio_PlayActorSfx2(&this->actor, NA_SE_EN_DEADHAND_HIDE);
    EnDh_SetupAction(this, EnDh_Burrow);
}

void EnDh_Burrow(EnDh* this, PlayState* play) {

    if (Actor_FindNearby(play, &this->actor, ACTOR_EN_DHA, ACTORCAT_ENEMY, 100.0f) != NULL) {
        
        Actor_FindNearby(play, &this->actor, ACTOR_EN_DHA, ACTORCAT_ENEMY, 100.0f)->params = ENDHA_DEAD;

    }
    switch (this->actionState) {
        case DH_WAIT:
            if (this->BurrowFlag == false) {
                this->spawnBackArms = 0;
           }     
           
            this->actionState++;
            this->drawDirtWave++;
            this->collider1.base.atFlags = this->collider1.info.toucherFlags =
                AT_ON | AT_TYPE_ENEMY; // also TOUCH_ON | TOUCH_SFX_WOOD
            this->collider1.info.toucher.dmgFlags = DMG_DEFAULT;
            this->collider1.info.toucher.damage = 4;
            FALLTHROUGH;
        case DH_RETREAT:
           
            this->dirtWavePhase += 0x47E;
            Math_SmoothStepToF(&this->dirtWaveSpread, 300.0f, 1.0f, 8.0f, 0.0f);
            this->dirtWaveHeight = Math_SinS(this->dirtWavePhase) * 55.0f;
            this->dirtWaveAlpha = (s16)(Math_SinS(this->dirtWavePhase) * 255.0f);
            EnDh_SpawnDebris(play, this, &this->actor.world.pos, this->dirtWaveSpread, 4, 2.05f, 1.2f);
            this->collider1.dim.radius = this->dirtWaveSpread * 0.6f;
            if (SkelAnime_Update(&this->skelAnime)) {
                this->actionState++;            
            }
            break;
        case DH_BURROW:
            this->drawDirtWave = false;
            this->collider1.dim.radius = 35;
            this->collider1.base.atFlags = this->collider1.info.toucherFlags = AT_NONE; // Also TOUCH_NONE
            this->collider1.info.toucher.dmgFlags = this->collider1.info.toucher.damage = 0;

            Actor_Spawn(&play->actorCtx, play, ACTOR_EN_DHA, this->actor.world.pos.x, this->actor.world.pos.y,
            this->actor.world.pos.z, 0, 0, 0, ENDHA_SEPERATE);
            this->ArmspawnCounter++;
           
            EnDh_SetupWait(this,play);
            break;
    }
}
void EnDh_SetupDirtShield(EnDh* this, PlayState* play) {
    
    this->dirtWaveSpread = 0.0f;
    this->actor.world.rot.y = this->actor.shape.rot.y;
    this->dirtWavePhase = 0;
    this->drawDirtWave++;

    this->collider1.base.atFlags = this->collider1.info.toucherFlags =
        AT_ON | AT_TYPE_ENEMY; 
    this->collider1.info.toucher.dmgFlags = DMG_DEFAULT;
    this->collider1.info.toucher.damage = 4;

    this->dirtShieldFlag = 1;
    EnDh_DirtShield(this, play);

}

void EnDh_DirtShield(EnDh* this, PlayState* play) {
    f32 dirtWaveSpreadTarget = 0;
    Audio_PlayActorSfx2(&this->actor, NA_SE_EV_STONE_LAUNCH - SFX_FLAG);
    if (this->dirtShieldTimer <= 0) {
        this->collider1.dim.radius = 120;
        dirtWaveSpreadTarget = 120;
    }
    else {
        this->collider1.dim.radius = 60;
        dirtWaveSpreadTarget = 60;
    }
    Math_SmoothStepToF(&this->dirtWaveSpread, dirtWaveSpreadTarget, 1.0f, 8.0f, 0.0f);
    this->dirtWaveHeight = 80.0f;//Math_SinS(this->dirtWavePhase) * 55.0f;
    this->dirtWaveAlpha = 200;//(s16)(Math_SinS(this->dirtWavePhase) * 255.0f);
    EnDh_SpawnDebris(play, this, &this->actor.world.pos, 1/*this->dirtWaveSpread*/, 4, 2.05f, 1.2f);
   

    
    
}
void EnDh_CloseDirtShield(EnDh* this, PlayState* play) {
    Math_SmoothStepToF(&this->dirtWaveSpread, 0.0f, 1.0f, 8.0f, 0.0f);
    if (this->dirtWaveSpread == 0.0f) {
        this->drawDirtWave = false;
    }
    this->collider1.base.atFlags = this->collider1.info.toucherFlags = AT_NONE; 
    this->collider1.info.toucher.dmgFlags = this->collider1.info.toucher.damage = 0;
    this->dirtShieldFlag = 0;


}

void EnDh_SetupDamage(EnDh* this) {
    Animation_MorphToPlayOnce(&this->skelAnime, &object_dh_Anim_003D6C, -6.0f);
    if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
        this->actor.speedXZ = -1.0f;
    }
    Audio_PlayActorSfx2(&this->actor, NA_SE_EN_DEADHAND_DAMAGE);
    this->curAction = DH_DAMAGE;
    
    EnDh_SetupAction(this, EnDh_Damage);
}

void EnDh_Damage(EnDh* this, PlayState* play) {
    
    if (this->actor.speedXZ < 0.0f) {
        this->actor.speedXZ += 0.15f;
    }
    this->actor.world.rot.y = this->actor.yawTowardsPlayer;
    EnDh_CloseDirtShield(this, play);
    if (SkelAnime_Update(&this->skelAnime)) {
        this->actor.world.rot.y = this->actor.shape.rot.y;
        if (this->retreat) {
            EnDh_SetupRetreat(this, play);
        } else if ((this->actor.xzDistToPlayer <= 105.0f) && Actor_IsFacingPlayer(&this->actor, 60 * 0x10000 / 360)) {
            f32 frames = Animation_GetLastFrame(&object_dh_Anim_004658);

            EnDh_SetupAttack(this,play);
            Animation_Change(&this->skelAnime, &object_dh_Anim_004658, 1.0f, 20.0f, frames, ANIMMODE_ONCE, -6.0f);
        } else {
            EnDh_SetupWalk(this);
        }
        this->unk_258 = 255;
    }
}

void EnDh_SetupDeath(EnDh* this) {
    Animation_MorphToPlayOnce(&this->skelAnime, &object_dh_Anim_0032BC, -1.0f);
    this->curAction = DH_DEATH;
    this->timer = 300;
    this->actor.flags &= ~ACTOR_FLAG_0;
    this->actor.speedXZ = 0.0f;
    func_800F5B58();
    this->actor.params = ENDH_DEATH;
    Audio_PlayActorSfx2(&this->actor, NA_SE_EN_DEADHAND_DEAD);
    EnDh_SetupAction(this, EnDh_Death);
}

void EnDh_Death(EnDh* this, PlayState* play) {
    EnDh_CloseDirtShield(this, play);
    if (SkelAnime_Update(&this->skelAnime) || (this->timer != 300)) {
        if (this->timer == 300) {
            Animation_PlayLoop(&this->skelAnime, &object_dh_Anim_00375C);
        }
        this->timer--;
        if (this->timer < 150) {
            if (this->alpha != 0) {
                this->actor.scale.y -= 0.000075f;
                this->actor.shape.shadowAlpha = this->alpha -= 5;
            } else {
                Actor_Kill(&this->actor);
                return;
            }
        }
    } else {
        if (((s32)this->skelAnime.curFrame == 53) || ((s32)this->skelAnime.curFrame == 56) ||
            ((s32)this->skelAnime.curFrame == 61)) {
            Audio_PlayActorSfx2(&this->actor, NA_SE_EN_RIZA_DOWN);
        }
        if ((s32)this->skelAnime.curFrame == 61) {
            Actor_ChangeCategory(play, &play->actorCtx, &this->actor, ACTORCAT_PROP);
        }
    }
}

void EnDh_CollisionCheck(EnDh* this, PlayState* play) {
    s32 pad;
    Player* player = GET_PLAYER(play);
    s32 lastHealth;

    if (this->dirtShieldFlag != 0) {
        if (this->dirtShieldTimer > 0) {
            this->dirtShieldTimer--;
        }

        if ((this->collider2.base.acFlags & AC_HIT) && !this->retreat) {
            this->collider2.base.acFlags &= ~AC_HIT;
            if ((this->actor.colChkInfo.damageEffect != 0) && (this->actor.colChkInfo.damageEffect != 6)) {
                this->collider2.base.atFlags = this->collider2.elements[0].info.toucherFlags = AT_NONE; // also TOUCH_NONE
                this->collider2.elements[0].info.toucher.dmgFlags = this->collider2.elements[0].info.toucher.damage = 0;
                if (player->unk_844 != 0) {
                    this->unk_258 = player->unk_845;
                }
                Actor_SetColorFilter(&this->actor, 0x4000, 0xFF, 0, 8);
                lastHealth = this->actor.colChkInfo.health;
                if (Actor_ApplyDamage(&this->actor) == 0) {
                    EnDh_SetupDeath(this);
                    Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos, 0x90);
                }
                else {
                    /*if  (((lastHealth >= 15) && (this->actor.colChkInfo.health < 15)) ||
                        ((lastHealth >= 9) && (this->actor.colChkInfo.health < 9)) ||
                        ((lastHealth >= 3) && (this->actor.colChkInfo.health < 3))) */
                    if (this->dirtShieldTimer <= 40) {
                        this->retreat++;
                    }
                    EnDh_SetupDamage(this);
                }
            }
        }
    }
}

u8 EnDh_checkBothBackArms(EnDh* this, PlayState* play) {
    if(Actor_FindBackArms(play,&this->actor,ENDHA_BACKLEFT) != NULL){
        return 1;
    }
    if(Actor_FindBackArms(play,&this->actor,ENDHA_BACKRIGHT) != NULL){
        return 1;
    }
     return 0;
}

Actor* Actor_FindBackArms(PlayState* play, Actor* refActor, s16 armParams) {
    Actor* actor = play->actorCtx.actorLists[ACTORCAT_ENEMY].head;
    while (actor != NULL) {
        s16 params = (actor->params);
        if (actor == refActor || ((actor->id != ACTOR_EN_DHA) || (params != armParams) )) {
            actor = actor->next;
        } else {
                return actor;
        }
    }
    return NULL;
}


void EnDh_Update(Actor* thisx, PlayState* play) {
    s32 pad;
    EnDh* this = (EnDh*)thisx;
    Player* player = GET_PLAYER(play);
    s32 pad40;
    Actor* backArms;
    EnDh_CollisionCheck(this, play);
    this->actionFunc(this, play);
    Actor_MoveForward(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 20.0f, 45.0f, 45.0f,
                            UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2 | UPDBGCHECKINFO_FLAG_3 |
                                UPDBGCHECKINFO_FLAG_4);
    this->actor.focus.pos = this->headPos;
    Collider_UpdateCylinder(&this->actor, &this->collider1);
    if(this->spawnBackArms == 1){
        if(Actor_FindBackArms(play,&this->actor,ENDHA_BACKLEFT) == NULL){
            this->attachedActor[0] = NULL;
        }
        if(Actor_FindBackArms(play,&this->actor,ENDHA_BACKRIGHT) == NULL){
            this->attachedActor[1] = NULL;
        }
    }
    if (this->actor.colChkInfo.health > 0) {
        if (this->curAction == DH_WAIT) {
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider1.base);
        } else {
            CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider1.base);
        }
        if (((this->curAction != DH_DAMAGE) && (this->actor.shape.yOffset == 0.0f)) ||
            ((player->unk_844 != 0) && (player->unk_845 != this->unk_258))) {

            CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider2.base);
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider2.base);
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider1.base);
        }
    } else {
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider1.base);
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider2.base);
    }
   
       
      
    
}

void EnDh_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx, Gfx** gfx) {
    Vec3f headOffset = { 2000.0f, 1000.0f, 0.0f };//2000.0f, 1000.0f, 0.0f
    Vec3f shoulderOffset = {700.0f, 0.0f, 0.0f };
    EnDh* this = (EnDh*)thisx;

    if (limbIndex == 13) {
        Matrix_MultVec3f(&headOffset, &this->headPos);
        Matrix_Push();
        Matrix_Translate(headOffset.x, headOffset.y, headOffset.z, MTXMODE_APPLY);
        Collider_UpdateSpheres(1, &this->collider2);
        Matrix_Pop();
    }
    
    if (limbIndex == 5) { //5-right shoulder
        if(this->attachedActor[1] != NULL){
            Matrix_MultVec3f(&shoulderOffset, &this->attachedActor[1]->world.pos);
        }
    }

    if (limbIndex == 8) { // 8-left shoulder
        if(this->attachedActor[0] != NULL){
            Matrix_MultVec3f(&shoulderOffset, &this->attachedActor[0]->world.pos);
        }
    }
     
}


void EnDh_Draw(Actor* thisx, PlayState* play) {
    s32 pad;
    EnDh* this = (EnDh*)thisx;

    OPEN_DISPS(play->state.gfxCtx, "../z_en_dh.c", 1099);
    if (this->alpha == 255) {
        Gfx_SetupDL_25Opa(play->state.gfxCtx);
        gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, this->alpha);
        gSPSegment(POLY_OPA_DISP++, 0x08, &D_80116280[2]);
        POLY_OPA_DISP =
            SkelAnime_DrawFlex(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount,
                               NULL, EnDh_PostLimbDraw, &this->actor, POLY_OPA_DISP);
    } else {
        Gfx_SetupDL_25Xlu(play->state.gfxCtx);
        gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, this->alpha);
        gSPSegment(POLY_XLU_DISP++, 0x08, &D_80116280[0]);
        POLY_XLU_DISP = SkelAnime_DrawFlex(play, this->skelAnime.skeleton, this->skelAnime.jointTable,
                                           this->skelAnime.dListCount, NULL, NULL, &this->actor, POLY_XLU_DISP);
    }
    if (this->drawDirtWave) {
        Gfx_SetupDL_25Xlu(play->state.gfxCtx);
        gDPSetEnvColor(POLY_XLU_DISP++, 85, 55, 0, 130);
        gSPSegment(POLY_XLU_DISP++, 0x08,
                   Gfx_TwoTexScroll(play->state.gfxCtx, G_TX_RENDERTILE, (play->state.frames * -3) % 0x80, 0, 0x20,
                                    0x40, 1, (play->state.frames * -10) % 0x80, (play->state.frames * -20) % 0x100,
                                    0x20, 0x40));
        gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, 0, 0, 0, this->dirtWaveAlpha);

        Matrix_Translate(0.0f, -this->actor.shape.yOffset, 0.0f, MTXMODE_APPLY);
        Matrix_Scale(this->dirtWaveSpread * 0.01f, this->dirtWaveHeight * 0.01f, this->dirtWaveSpread * 0.01f,
                     MTXMODE_APPLY);
        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, "../z_en_dh.c", 1160),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, object_dh_DL_007FC0);
    }
    CLOSE_DISPS(play->state.gfxCtx, "../z_en_dh.c", 1166);
}
