#include "CyberGanon.h"
#include "overlays/actors/ovl_Door_Warp1/z_door_warp1.h"

#define FLAGS (ACTOR_FLAG_0 | ACTOR_FLAG_2 | ACTOR_FLAG_NO_UPDATE_CULLING | ACTOR_FLAG_5)
#define GANON_HEALTH_MAX (70) // 70
#define GANON_CLUB_HEALTH (40) //40
#define TOUCH_WALL ((this->actor.bgCheckFlags & BGCHECKFLAG_WALL)&&((ABS(this->actor.wallYaw - this->actor.shape.rot.y) >= 0x4000)))
#define JUMP_BACK_DISTANCE (300.0f)
#define IN_CANNON_MELEE_ATTACK_RANGE (Math_Vec3f_DistXZ(&this->actor.world.pos, &player->actor.world.pos) <= 80.f && (this->actor.yawTowardsPlayer - this->actor.shape.rot.y) < -6000 && (this->actor.yawTowardsPlayer - this->actor.shape.rot.y) > -32768)
#define MELEE_ATTACK_DISTANCE (125.f)
#define LUNGE_DISTANCE (600.f)
#define NORMAL_WALK_SPEED (5.0f)
#define MAD_WALK_SPEED (7.0f)

void CyberGanon_Init(CyberGanon* this, PlayState* play);
void CyberGanon_Destroy(CyberGanon* this, PlayState* play);
void CyberGanon_Update(CyberGanon* this, PlayState* play);
void CyberGanon_Draw(CyberGanon* this, PlayState* play);
void CyberGanon_SetupIdle(CyberGanon* this, PlayState* play);
void CyberGanon_Idle(CyberGanon* this, PlayState* play);
void CyberGanon_CollisionCheck(CyberGanon* this, PlayState* play);
void CyberGanon_FeetDamaged(CyberGanon* this, PlayState* play);
void CyberGanon_SetupHeadDamaged(CyberGanon* this, PlayState* play);
void CyberGanon_HeadDamaged(CyberGanon* this, PlayState* play);
void CyberGanon_SetupDeath(CyberGanon* this, PlayState* play);
void CyberGanon_Death(CyberGanon* this, PlayState* play);
void CyberGanon_SetupActionJudge(CyberGanon* this, PlayState* play);
void CyberGanon_ActionJudge(CyberGanon* this, PlayState* play);
void CyberGanon_InitSphereCollider(CyberGanon* this, PlayState* play);
void CyberGanon_SetupWalk(CyberGanon* this, PlayState* play);
void CyberGanon_Walk(CyberGanon* this, PlayState* play);
void CyberGanon_SetupBlock(CyberGanon* this, PlayState* play);
void CyberGanon_Block(CyberGanon* this, PlayState* play);
void CyberGanon_SetupMeleeAttackPrepare(CyberGanon* this, PlayState* play);
void CyberGanon_MeleeAttackPrepare(CyberGanon* this, PlayState* play);
void CyberGanon_SetupGrabLink(CyberGanon* this, PlayState* play);
void CyberGanon_GrabLink(CyberGanon* this, PlayState* play);
void CyberGanon_SetupGrabAndExecute(CyberGanon* this, PlayState* play);
void CyberGanon_GrabAndExecute(CyberGanon* this, PlayState* play);
void CyberGanon_PinLinkInHand(CyberGanon* this, PlayState* play);
void CyberGanon_SetupJumpBack(CyberGanon* this, PlayState* play);
void CyberGanon_JumpBack(CyberGanon* this, PlayState* play);
void CyberGanon_SetupLandAfterJumpBack(CyberGanon* this, PlayState* play);
void CyberGanon_LandAfterJumpBack(CyberGanon* this, PlayState* play);
void CyberGanon_VerticalAttackPrepare(CyberGanon* this, PlayState* play);
void CyberGanon_Nothing(CyberGanon* this, PlayState* play);
void CyberGanon_SetupFireMissilePrepare(CyberGanon* this,PlayState* play);
void CyberGanon_FireMissilePrepare(CyberGanon* this,PlayState* play);
void CyberGanon_SetupFireMissile(CyberGanon* this,PlayState* play);
void CyberGanon_FireMissile(CyberGanon* this,PlayState* play);

void CyberGanon_SetupFireMissileUpward(CyberGanon* this,PlayState* play);
void CyberGanon_FireMissileUpward(CyberGanon* this,PlayState* play);

void CyberGanon_SetupLunge(CyberGanon* this,PlayState* play);
void CyberGanon_Lunge(CyberGanon* this,PlayState* play);
void CyberGanon_SetupLandAfterLunge(CyberGanon* this,PlayState* play);
void CyberGanon_LandAfterLunge(CyberGanon* this,PlayState* play);
void CyberGanon_SetupIntroCutscene(CyberGanon* this,PlayState* play);
void CyberGanon_IntroCutscene(CyberGanon* this,PlayState* play);
void CyberGanon_SetupIdle(CyberGanon* this,PlayState* play);
void CyberGanon_Idle(CyberGanon* this,PlayState* play);

void CyberGanon_SetupPullOutClub(CyberGanon* this, PlayState* play);
void CyberGanon_PullOutClub(CyberGanon* this, PlayState* play);
void CyberGanon_FireMissileAfterClubAttack(CyberGanon* this, PlayState* play);
void CyberGanon_VerticalClubAttack(CyberGanon* this, PlayState* play);
void CyberGanon_HorizontalAttack(CyberGanon* this, PlayState* play);
void CyberGanon_SetupCannonMeleeAttack(CyberGanon* this,PlayState* play);
void CyberGanon_CannonMeleeAttack(CyberGanon* this,PlayState* play);
s8 CyberGanon_ReactToProjectile(CyberGanon* this, PlayState* play);
void CyberGanon_SpawnSmokeFromGun(CyberGanon* this, PlayState* play);
void CyberGanon_SpawnSmokeFromTail(CyberGanon* this, PlayState* play);
void CyberGanon_UpdateEye(CyberGanon* this);

static ColliderQuadInit sClubColliderInit = {
    {
        COLTYPE_METAL,
        AT_ON | AT_TYPE_ALL,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_QUAD,
    },
    {
        ELEMTYPE_UNK0,
        { 0x20000000, 0x03, 0x30 },
        { 0x00000000, 0x00, 0x00 },
        TOUCH_ON | TOUCH_NEAREST | TOUCH_SFX_NORMAL,
        BUMP_NONE,
        OCELEM_ON,
    },
    { { { 0.0f, 0.0f, 0.0f}, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } } },
};

static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, 0x0),
    /* Deku stick    */ DMG_ENTRY(1, 0x0),
    /* Slingshot     */ DMG_ENTRY(0, 0x0),
    /* Explosive     */ DMG_ENTRY(1, 0x0),
    /* Boomerang     */ DMG_ENTRY(0, 0x0),
    /* Normal arrow  */ DMG_ENTRY(0, 0x0),
    /* Hammer swing  */ DMG_ENTRY(1, 0x0),
    /* Hookshot      */ DMG_ENTRY(0, 0x0),
    /* Kokiri sword  */ DMG_ENTRY(0, 0x0),
    /* Master sword  */ DMG_ENTRY(1, 0x0),
    /* Giant's Knife */ DMG_ENTRY(1, 0x0),
    /* Fire arrow    */ DMG_ENTRY(1, 0x0),
    /* Ice arrow     */ DMG_ENTRY(1, 0x0),
    /* Light arrow   */ DMG_ENTRY(1, 0x0),
    /* Unk arrow 1   */ DMG_ENTRY(1, 0x0),
    /* Unk arrow 2   */ DMG_ENTRY(1, 0x0),
    /* Unk arrow 3   */ DMG_ENTRY(1, 0x0),
    /* Fire magic    */ DMG_ENTRY(2, 0x0),
    /* Ice magic     */ DMG_ENTRY(0, 0x0),
    /* Light magic   */ DMG_ENTRY(3, 0x0),
    /* Shield        */ DMG_ENTRY(0, 0x0),
    /* Mirror Ray    */ DMG_ENTRY(0, 0x0),
    /* Kokiri spin   */ DMG_ENTRY(0, 0x0),
    /* Giant spin    */ DMG_ENTRY(1, 0x0),
    /* Master spin   */ DMG_ENTRY(1, 0x0),
    /* Kokiri jump   */ DMG_ENTRY(2, 0x0),
    /* Giant jump    */ DMG_ENTRY(4, 0x0),
    /* Master jump   */ DMG_ENTRY(2, 0x0),
    /* Unknown 1     */ DMG_ENTRY(0, 0x0),
    /* Unblockable   */ DMG_ENTRY(0, 0x0),
    /* Hammer jump   */ DMG_ENTRY(4, 0x0),
    /* Unknown 2     */ DMG_ENTRY(0, 0x0),
};

const ActorInit sCyberGanon_InitVars = {
    .id = 0x0036,
    .category     = ACTORCAT_BOSS,
    .flags        = FLAGS,
    .objectId = 0x0011,
    .instanceSize = sizeof(CyberGanon),
    .init         = (ActorFunc)CyberGanon_Init,
    .destroy      = (ActorFunc)CyberGanon_Destroy,
    .update       = (ActorFunc)CyberGanon_Update,
    .draw         = (ActorFunc)CyberGanon_Draw
};

static InitChainEntry sInitChain[] = {
    ICHAIN_U8(targetMode, 5, ICHAIN_CONTINUE),
    ICHAIN_S8(naviEnemyId, NAVI_ENEMY_GANON, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(gravity, -1000, ICHAIN_STOP),
};

void CyberGanon_Init(CyberGanon* this, PlayState* play) {
    EffectBlureInit1 blureInit;
    this->actor.colChkInfo.mass = MASS_IMMOVABLE;
    Actor_ProcessInitChain(&this->actor, sInitChain);
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 50.0f);
    this->actor.colChkInfo.health = GANON_HEALTH_MAX;
    SkelAnime_InitFlex(play, &this->skelAnime, &gCyberGanonObj_SkelCyberGanonObj, &gCyberGanonObj_AnimIdle, this->jointTable,
                       this->morphTable, CYBERGANONOBJ_LIMB_MAX);
    this->actor.colChkInfo.damageTable = &sDamageTable;
    CyberGanon_InitSphereCollider(this,play);            
    Collider_InitQuad(play, &this->clubCollider);
    Collider_SetQuad(play, &this->clubCollider, &this->actor, &sClubColliderInit);
    this->headDamageable = 0;
    blureInit.p1StartColor[0] = blureInit.p1StartColor[1] = blureInit.p1StartColor[2] = blureInit.p1StartColor[3] =
    blureInit.p2StartColor[0] = blureInit.p2StartColor[1] = blureInit.p2StartColor[2] = blureInit.p1EndColor[0] =
        blureInit.p1EndColor[1] = blureInit.p1EndColor[2] = blureInit.p2EndColor[0] = blureInit.p2EndColor[1] =
            blureInit.p2EndColor[2] = 255;

    blureInit.p1EndColor[3] = blureInit.p2EndColor[3] = 0;
    blureInit.p2StartColor[3] = 64;
    blureInit.elemDuration = 8;
    blureInit.unkFlag = false;
    blureInit.calcMode = 2;
    Effect_Add(play, &this->effectIndex, EFFECT_BLURE1, 0, 0, &blureInit);
    this->missileCoolTimer = 80;
    this->fallDownTimer = 140;
    if(CustomFlag_Get(0x10)){
        Actor_Spawn(&play->actorCtx, play, ACTOR_DOOR_WARP1,  this->actor.home.pos.x,  this->actor.home.pos.y,
                     this->actor.home.pos.z, 0, 0, 0,  WARP_DUNGEON_ADULT);
        Actor_Kill(&this->actor);
    }
    if(CustomFlag_Get(0xF)){
        this->actor.flags |= ACTOR_FLAG_TARGETABLE;
        CyberGanon_SetupActionJudge(this,play);
    }else{
        this->actor.flags &= ~ ACTOR_FLAG_TARGETABLE;
        CyberGanon_SetupIdle(this,play);
    }
}

void CyberGanon_InitSphereCollider(CyberGanon* this, PlayState* play){

    Collider_InitJntSph(play, &this->bodyArmorCollider);
    Collider_SetJntSph(play, &this->bodyArmorCollider, &this->actor, &sbodyArmorColliderInit, this->bodyArmorColliderItems);
    Collider_InitJntSph(play, &this->damageCollider);
    Collider_SetJntSph(play, &this->damageCollider, &this->actor, &sdamageColliderInit, this->damageColliderItems);
}

void CyberGanon_SetupIntroCutscene(CyberGanon* this,PlayState* play){
    Flags_SetSwitch(play, 0x20);
    f32 frames = Animation_GetLastFrame(&gCyberGanonObj_AnimWakeUp);
    Animation_Change(&this->skelAnime, &gCyberGanonObj_AnimWakeUp, 1.0f, 0.0f, (s16)frames, ANIMMODE_ONCE, -6.0f);
    this->actionFunc = CyberGanon_IntroCutscene;
}
void CyberGanon_IntroCutscene(CyberGanon* this,PlayState* play){
    if(SkelAnime_Update(&this->skelAnime)){
        CustomFlag_Set(0xF,1);
        this->actor.flags |= ACTOR_FLAG_TARGETABLE;
        this->missileCoolTimer = 80;
        Flags_UnsetSwitch(play, 0x20);
        CyberGanon_SetupWalk(this,play);
    }
    if((s16)this->skelAnime.curFrame == 19){
        func_800F5ACC(0x47);
        Audio_PlayActorSfx2(&this->actor, NA_SE_EN_MGANON_WALK);
        Actor_SpawnFloorDustRing(play, &this->actor, &this->feetPos[FOOT_LEFT], 25.0f, 3, 8.0f, 500, 10, true);
        Actor_SpawnFloorDustRing(play, &this->actor, &this->feetPos[FOOT_RIGHT], 25.0f, 3, 8.0f, 500, 10, true);
        func_80033E88(&this->actor, play, 1, 0x4);
    }
    if((s16)this->skelAnime.curFrame == 70){
        func_8002F7DC(&this->actor, NA_SE_EN_MGANON_UNARI);
    }
    // if((s16)this->skelAnime.curFrame == 99){
    //     Message_StartTextbox(play, 0x018A, NULL);   
    // }
    if((s16)this->skelAnime.curFrame == 31){
        play->envCtx.lightSettingOverride = 1;
    }
    if((s16)this->skelAnime.curFrame == 34){
        TitleCard_InitBossName(play, &play->actorCtx.titleCtx,
                       SEGMENTED_TO_VIRTUAL(&gCyberGanonObj_TexGanonTitleCardIa8), 0xA0, 0xB4, 128,
                       31);
    }
        

}
void CyberGanon_SetupIdle(CyberGanon* this,PlayState* play){
    Animation_MorphToLoop(&this->skelAnime, &gCyberGanonObj_AnimShutdown, 0.0f);
    this->actionFunc = CyberGanon_Idle;
}

void CyberGanon_Idle(CyberGanon* this,PlayState* play){
    SkelAnime_Update(&this->skelAnime);
    if((s32)this->skelAnime.curFrame == 30){
         Audio_PlayActorSfx2(&this->actor, NA_SE_EN_MGANON_BREATH);
    }
    if(this->actor.xzDistToPlayer <= 300.0F && this->actor.yDistToPlayer <= 300.0F){
        CyberGanon_SetupIntroCutscene(this,play);
    }
}


void CyberGanon_SetupActionJudge(CyberGanon* this, PlayState* play){
    this->slowTurnTimer = 20;
    this->clubAttack = 0;
    this->leftHandBlur = 0;
    this->cannonMeeleAttack = 0;
    this->actor.flags |= ACTOR_FLAG_TARGETABLE; 
    if(Rand_ZeroOne() <= 0.2f){
        this->actionTimer = 40;
    }else if(Rand_ZeroOne() <= 0.7f){
        this->actionTimer = 30;
    }else{
        this->actionTimer = 20;
    }
    this->actionFunc = CyberGanon_ActionJudge;
}

void CyberGanon_SetupWalk(CyberGanon* this, PlayState* play){
    this->headDamageable = 0;
    Animation_Change(&this->skelAnime, &gCyberGanonObj_AnimWalkFast, 1.0f, 0.0f, Animation_GetLastFrame(&gCyberGanonObj_AnimWalkFast), ANIMMODE_LOOP, -6.0f);
    this->actionFunc = CyberGanon_Walk;
    
}

void CyberGanon_ActionJudge(CyberGanon* this, PlayState* play){
    Player* player = GET_PLAYER(play);
    s16 yaw = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;
    if(this->shouldBackHome){
        yaw = Actor_WorldYawTowardPoint(&this->actor, &this->actor.home.pos) - this->actor.shape.rot.y;
        if(ABS(yaw) < 0x500){
            CyberGanon_SetupLunge(this,play);
        }else{
            CyberGanon_SetupWalk(this,play);
        }
        return;
    }
    if(!this->hasClub && this->actor.colChkInfo.health <= GANON_CLUB_HEALTH && this->actor.colChkInfo.health > 0 && ((this->actor.xzDistToPlayer < LUNGE_DISTANCE))){
        CyberGanon_SetupPullOutClub(this,play);
        return;
    }
    if(this->actionTimer == 0){
        if(IN_CANNON_MELEE_ATTACK_RANGE && (this->failToHitCount < (3))){
            CyberGanon_SetupCannonMeleeAttack(this,play);
        }else if((this->actor.xzDistToPlayer > 80.0F) && (this->actor.xzDistToPlayer) <= MELEE_ATTACK_DISTANCE && !(this->actor.bgCheckFlags & BGCHECKFLAG_WALL)){
            CyberGanon_SetupJumpBack(this,play);
        }else if (this->actor.xzDistToPlayer >= (MELEE_ATTACK_DISTANCE * 2) && Actor_IsFacingPlayer(&this->actor, 0x1555) && (this->missileCoolTimer == 0) && play->actorCtx.actorLists[ACTORCAT_EXPLOSIVE].length < 3){
             CyberGanon_SetupFireMissilePrepare(this,play);
        }else if (this->actor.xzDistToPlayer >= (LUNGE_DISTANCE) && Actor_IsFacingPlayer(&this->actor, 0x100)&& play->actorCtx.actorLists[ACTORCAT_EXPLOSIVE].length == 0){
            CyberGanon_SetupLunge(this,play);
        }else{
            CyberGanon_SetupWalk(this,play);
        }
    }else{
        CyberGanon_SetupWalk(this,play);
    }

}

void CyberGanon_SetupPullOutClub(CyberGanon* this, PlayState* play){
    this->headDamageable = 0;
    this->actor.naviEnemyId = NAVI_ENEMY_GANONDORF;
    Animation_Change(&this->skelAnime, &gCyberGanonObj_AnimPullOutClub, 1.0f, 0.0f, Animation_GetLastFrame(&gCyberGanonObj_AnimPullOutClub), ANIMMODE_ONCE, -6.0f);
    this->actionFunc = CyberGanon_PullOutClub;
}

void CyberGanon_PullOutClub(CyberGanon* this, PlayState* play){
    if(SkelAnime_Update(&this->skelAnime)){
        this->hasClub = 1;
        play->envCtx.lightSettingOverride = 1;
        CyberGanon_SetupActionJudge(this,play);
    }else{
        if((s32)this->skelAnime.curFrame == 8){
            play->envCtx.lightSettingOverride = 2;
            Actor_SetColorFilter(&this->actor,  -0x8000, 0xFF, 0, 40);
            this->hasClub = 1;
        }
        Audio_PlayActorSfx2(&this->actor, NA_SE_EV_GOD_LIGHTBALL_2 - SFX_FLAG);
    }
}

void CyberGanon_SetupFireMissileAfterClubAttack(CyberGanon* this, PlayState* play){
    this->headDamageable = 1;
    this->clubAttack = 0;
    Animation_Change(&this->skelAnime, &gCyberGanonObj_AnimFireMissileAfterClubAttack, 1.0f, 0.0f, Animation_GetLastFrame(&gCyberGanonObj_AnimFireMissileAfterClubAttack), ANIMMODE_ONCE, -6.0f);
    this->actionFunc = CyberGanon_FireMissileAfterClubAttack;
}

void CyberGanon_FireMissileAfterClubAttack(CyberGanon* this, PlayState* play){
    if(SkelAnime_Update(&this->skelAnime)){
        CyberGanon_SetupActionJudge(this,play);
    }else{
        if((s16)this->skelAnime.curFrame == 15){
            this->missileCoolTimer = 60;
            CyberGanon_SpawnMissile(this,play);
        }
        if((s16)this->skelAnime.curFrame >= 15){
            CyberGanon_SpawnSmokeFromGun(this,play);
        }
        if((s16)this->skelAnime.curFrame == 29) {
            CyberGanon_SpawnMissile(this,play);
        }
    }
}

void CyberGanon_SetupVerticalClubAttack(CyberGanon* this, PlayState* play){
    this->headDamageable = 1;
    this->failToHitCount++;
    Animation_Change(&this->skelAnime, &gCyberGanonObj_AnimClubVerticalAttack, 1.0f, 0.0f, Animation_GetLastFrame(&gCyberGanonObj_AnimClubVerticalAttack), ANIMMODE_ONCE, -4.0f);
    this->actionFunc = CyberGanon_VerticalClubAttack;
}

void CyberGanon_VerticalClubAttack(CyberGanon* this, PlayState* play){
    Vec3f sp2C;
    Vec3f effAccel = { 0.0f, 0.0f, 0.0f };
    Vec3f effVelocity = { 0.0f, 0.0f, 0.0f };
    Vec3f effectPos = this->actor.world.pos;
    if(SkelAnime_Update(&this->skelAnime)){
        CyberGanon_SetupActionJudge(this,play);
    }else{
        if((s16)this->skelAnime.curFrame == 15){
            Audio_PlayActorSfx2(&this->actor, NA_SE_EN_MGANON_SWORD);
            SoundEffect_PlayDefaultOneshot(SOUND_LYNEL_ATTACK_SPEAR2, &this->grabPosRot.pos, this);
        }
        if((s16)this->skelAnime.curFrame >= 15 && (s16)this->skelAnime.curFrame <= 22){
            this->clubAttack = 1;
        }else{
            this->clubAttack = 0;
        }
        if((s16)this->skelAnime.curFrame >= 7 && (s16)this->skelAnime.curFrame <= 15){
            Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 0xF, 0x150, 0x10);
        }
        if((s16)this->skelAnime.curFrame >= 30 && (s16)this->skelAnime.curFrame <= 37){
            Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 0xF, 0x150, 0x10);
        }
        if((s16)this->skelAnime.curFrame == 20 ){
            func_80033E88(&this->actor, play, 2, 0xA);
            Audio_PlayActorSfx2(&this->actor, NA_SE_EV_STONE_BOUND);
            EffectSsBlast_SpawnWhiteShockwave(play, &this->outterClubPos, &effVelocity, &effAccel);
        }
        if((s16)this->skelAnime.curFrame == 17 ){
            Actor_SpawnFloorDustRing(play, &this->actor, &this->feetPos[FOOT_LEFT], 25.0f, 3, 8.0f, 500, 10, true);
        }
    }
}

void CyberGanon_SetupHorizontalAttack(CyberGanon* this, PlayState* play){
    this->headDamageable = 1;
    this->failToHitCount++;
    Animation_Change(&this->skelAnime, &gCyberGanonObj_AnimClubHorizontalAttack, 1.0f, 0.0f, Animation_GetLastFrame(&gCyberGanonObj_AnimClubHorizontalAttack), ANIMMODE_ONCE, -4.0f);
    this->actionFunc = CyberGanon_HorizontalAttack;
}

void CyberGanon_HorizontalAttack(CyberGanon* this, PlayState* play){
    if(SkelAnime_Update(&this->skelAnime)){
        CyberGanon_SetupActionJudge(this,play);
    }else{
        if((s16)this->skelAnime.curFrame == 17 && this->actor.xzDistToPlayer >= (MELEE_ATTACK_DISTANCE * 2) && Actor_IsFacingPlayer(&this->actor, 0x1555) && /*(this->missileCoolTimer == 0) &&*/ play->actorCtx.actorLists[ACTORCAT_EXPLOSIVE].length < 3){
            CyberGanon_SetupFireMissileAfterClubAttack(this,play);
            return;
        }
        if((s16)this->skelAnime.curFrame == 8){
            Audio_PlayActorSfx2(&this->actor, NA_SE_EN_MGANON_SWORD);
            SoundEffect_PlayDefaultOneshot(SOUND_LYNEL_ATTACK_SPEAR2, &this->grabPosRot.pos, this);
        }
        if((s16)this->skelAnime.curFrame >= 8 && (s16)this->skelAnime.curFrame <= 20){
            this->clubAttack = 1;
            Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 0xF, 0x150, 0x10);
        }else if((s16)this->skelAnime.curFrame >= 28 && (s16)this->skelAnime.curFrame <= 36){
            this->clubAttack = 1;
            Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 0xF, 0x150, 0x10);
        }else{
            this->clubAttack = 0;
        }
        if((s16)this->skelAnime.curFrame == 28){
            Audio_PlayActorSfx2(&this->actor, NA_SE_EN_MGANON_SWORD);
            SoundEffect_PlayDefaultOneshot(SOUND_LYNEL_ATTACK_SPEAR2, &this->grabPosRot.pos, this);
        }
    }
    this->actor.world.rot.y = this->actor.shape.rot.y;
}

s8 CyberGanon_ReactToProjectile(CyberGanon* this, PlayState* play) {
    Actor* projectileActor;
    projectileActor = Actor_GetProjectileActor(play, &this->actor, 300.0f);
    s16 yawToProjectile;
    s16 wallYawDiff;
    s16 touchingWall;
    s16 directionFlag;

    if (projectileActor != NULL) {
        yawToProjectile = Actor_WorldYawTowardActor(&this->actor, projectileActor) - (u16)this->actor.shape.rot.y;
        if (Math_Vec3f_DistXYZ(&this->actor.world.pos, &projectileActor->world.pos) < 200.0f) {
            if (Actor_IsTargeted(play, &this->actor) && (projectileActor->id == ACTOR_ARMS_HOOK)) {
                return false;
            }else if(ABS(yawToProjectile) < 0x6000){
                CyberGanon_SetupBlock(this, play);
            }
            return true;
        }
    }
    return false;
}

void CyberGanon_Walk(CyberGanon* this, PlayState* play){
    Player* player = GET_PLAYER(play);
    s16 footOrder = -1;
    s16 yawTarget;
    s16 yawTemp;
    s16 faceYaw;
    f32 disToPlayer = Math_Vec3f_DistXZ(&this->actor.world.pos, &player->actor.world.pos);
    SkelAnime_Update(&this->skelAnime);
    if(this->hasClub){
        this->walkSpeed = MAD_WALK_SPEED;
        faceYaw = 0x800;
    }else{
        this->walkSpeed = NORMAL_WALK_SPEED;
        faceYaw = 0x1555;
    }
    if(this->shouldBackHome){
        yawTarget = Actor_WorldYawTowardPoint(&this->actor, &this->actor.home.pos);
        yawTemp = Actor_WorldYawTowardPoint(&this->actor, &this->actor.home.pos) - this->actor.shape.rot.y;
        if(ABS(yawTemp) < 0X500){
            CyberGanon_SetupLunge(this,play);
            return;
        }
    }else{
        yawTarget = this->actor.yawTowardsPlayer;
    }
    DECR(this->missileCoolTimer);
    if(CyberGanon_ReactToProjectile(this,play) || (player->unk_A73 != 0)){
        CyberGanon_SetupBlock(this,play);
        return;
    }
    if(Actor_IsFacingPlayer(&this->actor, faceYaw)){
        this->actor.speedXZ =  this->walkSpeed;
        if(disToPlayer <= MELEE_ATTACK_DISTANCE){
            if(this->failToHitCount < 3){
                CyberGanon_SetupMeleeAttackPrepare(this, play);
            }else{
                CyberGanon_SetupJumpBack(this, play);
            }
        }else if (this->actor.xzDistToPlayer > (MELEE_ATTACK_DISTANCE * 2) && (this->missileCoolTimer == 0) && play->actorCtx.actorLists[ACTORCAT_EXPLOSIVE].length < 3){
            CyberGanon_SetupFireMissilePrepare(this,play);
        }else if(this->actor.xzDistToPlayer >= (LUNGE_DISTANCE) && (!this->shouldBackHome)&& play->actorCtx.actorLists[ACTORCAT_EXPLOSIVE].length == 0 && Actor_IsFacingPlayer(&this->actor, 0x100)){
            CyberGanon_SetupLunge(this,play);
        }
        if(this->hasClub){
            Math_SmoothStepToS(&this->actor.shape.rot.y, yawTarget, 1, 0x600, 0x25);
        }else{
            Math_SmoothStepToS(&this->actor.shape.rot.y, yawTarget, 1, 0x35A, 0x25);
        }
    }else{
        this->actor.speedXZ =  this->walkSpeed - 0.5F;
        if(IN_CANNON_MELEE_ATTACK_RANGE){
            CyberGanon_SetupCannonMeleeAttack(this,play);
        }
        if(this->slowTurnTimer > 0){
            if(this->hasClub){
                Math_SmoothStepToS(&this->actor.shape.rot.y, yawTarget, 1, 0x600, 0x25);
            }else{
                Math_SmoothStepToS(&this->actor.shape.rot.y, yawTarget, 1, 0x35A, 0x25);
            }      
        }else{
            Math_SmoothStepToS(&this->actor.shape.rot.y, yawTarget, 1, 0xC5A, 0xAA);
        }
    }
    if (((s32)this->skelAnime.curFrame == 8)){
        footOrder = FOOT_LEFT;
    }else if((s32)this->skelAnime.curFrame == 17){
        footOrder = FOOT_RIGHT;
    }
    if (footOrder >= 0 ) {
        Audio_PlayActorSfx2(&this->actor, NA_SE_EN_MGANON_WALK);
        Actor_SpawnFloorDustRing(play, &this->actor, &this->feetPos[footOrder], 25.0f, 3, 8.0f, 500, 10, true);
        func_80033E88(&this->actor, play, 1, 0x4);
    }
    this->actor.world.rot.y = this->actor.shape.rot.y;

}


void CyberGanon_SetupBlock(CyberGanon* this, PlayState* play){
    this->headDamageable = 0;
    this->actor.speedXZ = 0.0f;
    f32 endframes1 = Animation_GetLastFrame(&gCyberGanonObj_AnimBlockWithClub);
    f32 endframes2 = Animation_GetLastFrame(&gCyberGanonObj_AnimBlockWithCannon);
    Animation_Change(&this->skelAnime, &gCyberGanonObj_AnimBlockWithCannon, 1.5f, 0.0f, endframes2, ANIMMODE_ONCE, -6.0f);
    this->actionFunc = CyberGanon_Block;

}

void CyberGanon_Block(CyberGanon* this, PlayState* play){
    this->actor.speedXZ = 0.0f;
    if(SkelAnime_Update(&this->skelAnime)){
        CyberGanon_SetupActionJudge(this,play);
    }
}

void CyberGanon_SetupMeleeAttackPrepare(CyberGanon* this, PlayState* play){
    this->headDamageable = 1;
    this->actor.speedXZ = 0.0f;
    s16 speed;
    if(this->hasClub){
        speed = 1.8f;
    }else{
        speed = 1.5f;
    }

    if(this->hasClub && Rand_ZeroOne() <= 0.5f && this->actor.xzDistToPlayer >= MELEE_ATTACK_DISTANCE * 0.5F ){
        f32 endframes = Animation_GetLastFrame(&gCyberGanonObj_AnimPreprareVerticalAttack);
        Animation_Change(&this->skelAnime, &gCyberGanonObj_AnimPreprareVerticalAttack, speed, 0.0f, endframes, ANIMMODE_ONCE, -6.0f);
        this->actionFunc = CyberGanon_VerticalAttackPrepare;
    }else{
        f32 endframes = Animation_GetLastFrame(&gCyberGanonObj_AnimPreprareMeeleAttack);
        Animation_Change(&this->skelAnime, &gCyberGanonObj_AnimPreprareMeeleAttack, speed, 0.0f, endframes, ANIMMODE_ONCE, -6.0f);
        this->actionFunc = CyberGanon_MeleeAttackPrepare;
    }
}

void CyberGanon_VerticalAttackPrepare(CyberGanon* this, PlayState* play){
    if(SkelAnime_Update(&this->skelAnime)){
        this->actionFunc = CyberGanon_SetupVerticalClubAttack;
    }else{
        if((s16)this->skelAnime.curFrame == 10){
            Actor_SpawnFloorDustRing(play, &this->actor, &this->feetPos[FOOT_RIGHT], 25.0f, 3, 8.0f, 500, 10, true);
            Audio_PlayActorSfx2(&this->actor, NA_SE_EN_MGANON_WALK);
            func_80033E88(&this->actor, play, 2, 0x4);
        }        
    }
}


void CyberGanon_MeleeAttackPrepare(CyberGanon* this, PlayState* play){
    if(SkelAnime_Update(&this->skelAnime)){
        if(!this->hasClub){
            this->actionFunc = CyberGanon_SetupGrabLink;
        }else{
            this->actionFunc = CyberGanon_SetupHorizontalAttack;
        }
    }else{
        if((s16)this->skelAnime.curFrame == 10){
            Actor_SpawnFloorDustRing(play, &this->actor, &this->feetPos[FOOT_RIGHT], 25.0f, 3, 8.0f, 500, 10, true);
            Audio_PlayActorSfx2(&this->actor, NA_SE_EN_MGANON_WALK);
            func_80033E88(&this->actor, play, 2, 0x4);
        }
    }
}


void CyberGanon_SetupGrabLink(CyberGanon* this, PlayState* play){
    this->leftHandBlur = 1;
    f32 endframes = Animation_GetLastFrame(&gCyberGanonObj_AnimGrabLink);
    Animation_Change(&this->skelAnime, &gCyberGanonObj_AnimGrabLink, 1.0f, 0.0f, endframes, ANIMMODE_ONCE, 0.0f);
    this->actionFunc = CyberGanon_GrabLink;

}

void CyberGanon_PinLinkInHand(CyberGanon* this, PlayState* play){
    Player* player = GET_PLAYER(play);
    player->unk_6AD = 0;
    this->actor.flags &= ~ ACTOR_FLAG_TARGETABLE;
    player->actor.speedXZ = player->actor.velocity.y = 0;
    player->actor.world.pos.x = this->grabPosRot.pos.x;
    player->actor.world.pos.y = this->grabPosRot.pos.y;
    player->actor.world.pos.z = this->grabPosRot.pos.z;
    // Math_ApproachF(&player->actor.world.pos.x, this->grabPosRot.pos.x, 0.5f, 20.0f);
    // Math_ApproachF(&player->actor.world.pos.y, this->grabPosRot.pos.y, 0.5f, 20.0f);
    // Math_ApproachF(&player->actor.world.pos.z, this->grabPosRot.pos.z, 0.5f, 20.0f);
    Math_ApproachS(&player->actor.shape.rot.x, this->grabPosRot.rot.x, 2, 0x7D0);
    Math_ApproachS(&player->actor.shape.rot.y, this->grabPosRot.rot.y, 2, 0x7D0);
    Math_ApproachS(&player->actor.shape.rot.z, this->grabPosRot.rot.z, 2, 0x7D0);
}



void CyberGanon_GrabLink(CyberGanon* this, PlayState* play){
    Player* player = GET_PLAYER(play);
    s16 i;
    s16 colliderCount = ARRAY_COUNT(sbodyArmorColliderElems);
    this->actor.world.rot.y = this->actor.shape.rot.y;
    Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 0xBA, 0x2A);
    if(this->hasGrabPlayer == 1){
        //player->unk_850 = 0xA;
        Audio_PlayActorSfx2(&this->actor, NA_SE_PL_BODY_HIT);
        Audio_PlayActorSfx2(&this->actor, NA_SE_EN_MGANON_UNARI);
        Rumble_Request(this->actor.xyzDistToPlayerSq, 250, 2, 120);
        Audio_PlaySfxGeneral(NA_SE_VO_LI_DAMAGE_S, &player->actor.projectedPos, 4,
                     &gSfxDefaultFreqAndVolScale, &gSfxDefaultFreqAndVolScale,
                     &gSfxDefaultReverb);
        if (play->grabPlayer(play, player)) {
            player->actor.parent = &this->actor;
        }
        this->hasGrabPlayer = 2;
    }
    if(this->hasGrabPlayer){
        this->leftHandBlur = 0;
        CyberGanon_PinLinkInHand(this,play);
    }else if((s16)this->skelAnime.curFrame == 12){
        this->failToHitCount++;
        CyberGanon_SetupActionJudge(this,play);
        return;
    }else if((s16)this->skelAnime.curFrame == 3){
        SoundEffect_PlayDefaultOneshot(SOUND_LYNEL_ATTACK_SPEAR2, &this->grabPosRot.pos, this);
    }
    if(SkelAnime_Update(&this->skelAnime)){
        CyberGanon_SetupGrabAndExecute(this,play);
    }
}

void CyberGanon_SetupGrabAndExecute(CyberGanon* this, PlayState* play){
    this->leftHandBlur = 0;
    f32 endframes = Animation_GetLastFrame(&gCyberGanonObj_AnimGrabAndExecute);
    Animation_Change(&this->skelAnime, &gCyberGanonObj_AnimGrabAndExecute, 1.0f, 0.0f, endframes, ANIMMODE_ONCE, 0.0f);
    this->actionFunc = CyberGanon_GrabAndExecute;
}

void CyberGanon_GrabAndExecute(CyberGanon* this, PlayState* play){
    Player* player = GET_PLAYER(play);
    EnBom* bomb;
    if((s16)this->skelAnime.curFrame == 20){
        this->hasGrabPlayer = 0;
        player->actor.parent = NULL;
        player->csMode = 0;
        player->invincibilityTimer = 0;
        play->damagePlayer(play, -0x20);
        bomb = (EnBom*)Actor_Spawn(&play->actorCtx, play, ACTOR_EN_BOM, player->actor.world.pos.x, player->actor.world.pos.y + 30.0f,
                        player->actor.world.pos.z, 0, 0, 0, BOMB_BODY);
        if (bomb != NULL) {
                bomb->timer = 0;
        }
        CollisionCheck_RedBlood(play, NULL, &this->grabPosRot.pos); 
        Camera_AddQuake(&play->mainCamera, 2, 0x19, 5);
        Audio_PlayActorSfx2(&this->actor, NA_SE_EN_GANON_HIT_THUNDER);
        CyberGanon_SpawnSmokeFromGun(this,play);
        //SoundEffect_PlayDefaultOneshot(SOUND_SHELL_FIRE, &this->grabPosRot.pos, this);
        func_8002F6D4(play, &this->actor, 10.0f, this->actor.shape.rot.y, 8.0f, 0);
    }else if ((s16)this->skelAnime.curFrame < 20){
        if(this->hasGrabPlayer){
            if((s16)this->skelAnime.curFrame == 10){
                CollisionCheck_RedBlood(play, NULL, &this->grabPosRot.pos); 
                player->invincibilityTimer = 0;
                play->damagePlayer(play, -0x10);
                func_8002F7DC(&player->actor, NA_SE_VO_LI_FREEZE + player->ageProperties->unk_92);
                Rumble_Request(this->actor.xyzDistToPlayerSq, 500, 2, 120);
            }
            CyberGanon_PinLinkInHand(this,play);
        }
    }else{
        if(this->hasGrabPlayer){
            Math_ApproachS(&player->actor.shape.rot.x, 0, 5, 0x3E8);
            Math_ApproachS(&player->actor.shape.rot.z, 0, 5, 0x3E8);
        }
    }
    if(SkelAnime_Update(&this->skelAnime)){
        player->actor.shape.rot.x = 0;
        player->actor.shape.rot.z = 0;
        this->actor.flags |= ACTOR_FLAG_TARGETABLE;
        CyberGanon_SetupActionJudge(this,play);
    }
}

void CyberGanon_SetupJumpBack(CyberGanon* this, PlayState* play){
    if(this->failToHitCount >= 2){
        this->failToHitCount = this->failToHitCount - 2;
    }else{
        this->failToHitCount  = 0;
    }
    f32 endframes = Animation_GetLastFrame(&gCyberGanonObj_AnimJumpBack);
    this->actor.speedXZ = 0;
    this->leftHandBlur = 0;
    this->headDamageable = 0;
    Animation_Change(&this->skelAnime, &gCyberGanonObj_AnimJumpBack, 1.0f, 0.0f, endframes, ANIMMODE_ONCE, -6.0f);
    this->actionFunc = CyberGanon_JumpBack;
}

void CyberGanon_JumpBack(CyberGanon* this, PlayState* play){
    Player* player = GET_PLAYER(play);
    EnBom* bomb;
    if(!SkelAnime_Update(&this->skelAnime)){
        if (this->skelAnime.curFrame > 14.0f) {
            Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 0xF, 0xFF, 0x2A);
            if(ABS(this->actor.xzDistToPlayer) >= JUMP_BACK_DISTANCE){
                Math_SmoothStepToF(&this->actor.velocity.y, -5.0f, 0.1f, 1.0f, 0.2f);
            }
            CyberGanon_SpawnSmokeFromGun(this,play);
        } 
        if (Animation_OnFrame(&this->skelAnime, 14.0f)) {   
            this->actor.speedXZ = -15.0f;
            this->actor.velocity.y = 8.5f;
            bomb = (EnBom*)Actor_Spawn(&play->actorCtx, play, ACTOR_EN_BOM, this->cannonGunPos.x, this->cannonGunPos.y,
                        this->cannonGunPos.z, 0, 0, 0, BOMB_BODY);
            if (bomb != NULL) {
                    bomb->timer = 0;
            }
            Camera_AddQuake(&play->mainCamera, 2, 0x19, 5);
            Audio_PlayActorSfx2(&this->actor, NA_SE_EN_GANON_HIT_THUNDER);
        }
    }
    if(this->skelAnime.curFrame > 15.0f && this->actor.bgCheckFlags & BGCHECKFLAG_GROUND){
        this->actor.speedXZ = 0.0f;
        Audio_PlayActorSfx2(&this->actor, NA_SE_EN_MGANON_WALK);
        func_80033E88(&this->actor, play, 2, 0xA);
        CyberGanon_SetupLandAfterJumpBack(this,play);
    }
    this->actor.world.rot.y = this->actor.shape.rot.y;
}

void CyberGanon_SetupLandAfterJumpBack(CyberGanon* this,PlayState* play){

    Audio_PlayActorSfx2(&this->actor, NA_SE_EN_MGANON_WALK);
    Actor_SpawnFloorDustRing(play, &this->actor, &this->feetPos[FOOT_LEFT], 25.0f, 3, 8.0f, 500, 10, true);
    Actor_SpawnFloorDustRing(play, &this->actor, &this->feetPos[FOOT_RIGHT], 25.0f, 3, 8.0f, 500, 10, true);
    func_80033E88(&this->actor, play, 1, 0xA);

    Animation_Change(&this->skelAnime, &gCyberGanonObj_AnimLand, 1.0f, 0.0f, 13.0f, ANIMMODE_ONCE, 0.0f);
    this->actionFunc = CyberGanon_LandAfterJumpBack;
}

void CyberGanon_LandAfterJumpBack(CyberGanon* this,PlayState* play){
    s16 isOnGround = this->actor.bgCheckFlags & BGCHECKFLAG_GROUND;
   if(SkelAnime_Update(&this->skelAnime)){
        CyberGanon_SetupActionJudge(this,play);
    }else if(this->skelAnime.curFrame >= 5.0f){
            this->headDamageable = 1;
    }
}

void CyberGanon_SetupCannonMeleeAttack(CyberGanon* this,PlayState* play){
    this->headDamageable = 1;
    this->actor.speedXZ = 0.0f;
    f32 endframes1 = Animation_GetLastFrame(&gCyberGanonObj_AnimCannonMeeleAttackFront);
    f32 endframes2 = Animation_GetLastFrame(&gCyberGanonObj_AnimCannonMeeleAttackBack);
    s16 yaw = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;
    if(yaw < 0 && yaw > -0x4000){
        Animation_Change(&this->skelAnime, &gCyberGanonObj_AnimCannonMeeleAttackFront, 1.0f, 0.0f, endframes1, ANIMMODE_ONCE, -4.0f);
    }else{
        Animation_Change(&this->skelAnime, &gCyberGanonObj_AnimCannonMeeleAttackBack, 1.0f, 0.0f, endframes2, ANIMMODE_ONCE, -4.0f);
    }
    this->actionFunc = CyberGanon_CannonMeleeAttack;
}

void CyberGanon_CannonMeleeAttack(CyberGanon* this,PlayState* play){
   if(SkelAnime_Update(&this->skelAnime)){
        CyberGanon_SetupActionJudge(this,play);
    }else{
        if(this->skelAnime.curFrame >= 12.0f){
            this->cannonMeeleAttack = 1;
        }
        if((s16)this->skelAnime.curFrame == 12){
            SoundEffect_PlayDefaultOneshot(SOUND_LYNEL_ATTACK_SPEAR2, &this->cannonGunPos, this);
        }
    }
}

void CyberGanon_SpawnSmokeFromGun(CyberGanon* this, PlayState* play) {
     s32 pad;
    s16 i;
    Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    Vec3f accel = { 0.0f, 0.3f, 0.0f };
    Vec3f pos;
    pos = this->cannonGunPos;
    s16 randAngle = Rand_CenteredFloat(0x4000);
    f32 randCos;
    f32 randSin;

    randCos = Math_CosS(this->actor.shape.rot.y + randAngle);
    randSin = Math_SinS(this->actor.shape.rot.y + randAngle);
    this->smokeEnvColor.r =100;
    this->smokeEnvColor.g =100;
    this->smokeEnvColor.b =100;
    this->smokeEnvColor.a =150;
    this->smokePrimColor.r =20;
    this->smokePrimColor.g =20;
    this->smokePrimColor.b =20;
    this->smokePrimColor.a =100;
    if (this->smokeEnvColor.g != 0) {
        this->smokeEnvColor.g++;
    }
    if (this->smokeEnvColor.b != 0) {
        this->smokeEnvColor.b += 15;
    }
    if (this->smokeEnvColor.r != 0) {
        this->smokeEnvColor.r++;
    }
    velocity.x = randSin * 3.5f;
    velocity.y = this->smokeEnvColor.r * 0.02f;
    velocity.z = randCos * 3.5f;
    accel.x = ((Rand_ZeroOne() * 0.1f) + 0.15f) * -randSin;
    accel.z = ((Rand_ZeroOne() * 0.1f) + 0.15f) * -randCos;
    func_8002836C(play, &pos, &velocity, &accel, &this->smokePrimColor, &this->smokeEnvColor, 100, 25, 20);
    func_8002836C(play, &pos, &velocity, &accel, &this->smokePrimColor, &this->smokeEnvColor, 100, 25, 20);
    
}

void CyberGanon_SpawnSmokeFromTail(CyberGanon* this, PlayState* play) {
     s32 pad;
    s16 i;
    Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    Vec3f accel = { 0.0f, 0.3f, 0.0f };
    Vec3f pos;
    pos = this->tailVentPos;
    s16 randAngle = Rand_CenteredFloat(0x4000);
    f32 randCos;
    f32 randSin;

    randCos = Math_CosS(this->actor.shape.rot.y + randAngle);
    randSin = Math_SinS(this->actor.shape.rot.y + randAngle);
    this->smokeEnvColor2.r =200;
    this->smokeEnvColor2.g =200;
    this->smokeEnvColor2.b =200;
    this->smokeEnvColor2.a =50;
    this->smokePrimColor2.r =200;
    this->smokePrimColor2.g =200;
    this->smokePrimColor2.b =200;
    this->smokePrimColor2.a =50;
    if (this->smokeEnvColor2.g != 0) {
        this->smokeEnvColor2.g++;
    }
    if (this->smokeEnvColor2.b != 0) {
        this->smokeEnvColor2.b += 15;
    }
    if (this->smokeEnvColor2.r != 0) {
        this->smokeEnvColor2.r++;
    }
    velocity.x = randSin * 3.5f;
    velocity.y = this->smokeEnvColor.r * 0.02f;
    velocity.z = randCos * 3.5f;
    accel.x = ((Rand_ZeroOne() * 0.1f) + 0.15f) * -randSin;
    accel.z = ((Rand_ZeroOne() * 0.1f) + 0.15f) * -randCos;
    func_8002836C(play, &pos, &velocity, &accel, &this->smokePrimColor2, &this->smokeEnvColor2, 100, 25, 20);
    SoundEffect_PlayDefaultHeld(SOUND_TANK_RUNNING,1.0,&this->tailVentPos,this);
}


void CyberGanon_SpawnMissile(CyberGanon* this,PlayState* play){
    s16 rotX;
    s16 param;
    Audio_PlayActorSfx2(&this->actor, NA_SE_EN_GANON_HIT_THUNDER);
    func_80033E88(&this->actor, play, 2, 0xA);
    if(this->upwardMode){
        
        param = 1; //High Speed
        rotX = -16319;
    }else{
        if(this->actionFunc == CyberGanon_FireMissileAfterClubAttack){
            param = 3;
        }else{
            param = 0;
        }
        rotX = Math_Vec3f_Pitch(&this->cannonGunPos, &this->outterArmRPos);
    }
    Actor_Spawn(&play->actorCtx, play, 0x0246, this->cannonGunPos.x, this->cannonGunPos.y,
           this->cannonGunPos.z, rotX, this->actor.shape.rot.y, 0, param);
}

void CyberGanon_SetupFireMissilePrepare(CyberGanon* this,PlayState* play){
    this->leftHandBlur = 0;
    this->headDamageable = 1;
    this->actor.speedXZ = 0.0f;
    this->upwardMode = 0;
    if(Rand_ZeroOne() <= 0.65f){
        this->upwardMode = 1;
    }
    if(this->upwardMode){
        Animation_Change(&this->skelAnime, &gCyberGanonObj_AnimPrepareFireMssileUpward, 1.0f, 0.0f, Animation_GetLastFrame(&gCyberGanonObj_AnimPrepareFireMssileUpward), ANIMMODE_ONCE, -6.0f);
    }else{
        Animation_Change(&this->skelAnime, &gCyberGanonObj_AnimPrepareFireMssile, 1.0f, 0.0f, Animation_GetLastFrame(&gCyberGanonObj_AnimPrepareFireMssile), ANIMMODE_ONCE, -6.0f);
    }

    this->actionFunc = CyberGanon_FireMissilePrepare;
}

void CyberGanon_FireMissilePrepare(CyberGanon* this,PlayState* play){
    Player* player = GET_PLAYER(play);
    f32 disToPlayer = Math_Vec3f_DistXZ(&this->actor.world.pos, &player->actor.world.pos);
    if(SkelAnime_Update(&this->skelAnime)){
        if(Actor_IsFacingPlayer(&this->actor, 0x1555) && disToPlayer > MELEE_ATTACK_DISTANCE){
            if(this->upwardMode){
                CyberGanon_SetupFireMissileUpward(this,play);
            }else{
                CyberGanon_SetupFireMissile(this,play);
            }
        }else{
            this->missileCoolTimer = 20;
            CyberGanon_SetupActionJudge(this,play);
        }
    }else{
        if (((s32)this->skelAnime.curFrame == 10)){
            Audio_PlayActorSfx2(&this->actor, NA_SE_EN_MGANON_WALK);
            Actor_SpawnFloorDustRing(play, &this->actor, &this->feetPos[FOOT_LEFT], 25.0f, 3, 8.0f, 500, 10, true);
            func_80033E88(&this->actor, play, 1, 0x4);
        }
        if((s32)this->skelAnime.curFrame < 8){
            Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 0xF, 0xBA, 0x2A);
        }
    }
    this->actor.world.rot.y = this->actor.shape.rot.y;
}

void CyberGanon_SetupFireMissile(CyberGanon* this,PlayState* play){
    f32 endframes = Animation_GetLastFrame(&gCyberGanonObj_AnimFireMssile);
    Animation_Change(&this->skelAnime, &gCyberGanonObj_AnimFireMssile, 1.0f, 0.0f, endframes, ANIMMODE_ONCE, -6.0f);
    this->actionFunc = CyberGanon_FireMissile;
}

void CyberGanon_FireMissile(CyberGanon* this,PlayState* play){
    if(SkelAnime_Update(&this->skelAnime)){
        CyberGanon_SetupActionJudge(this,play);
    }else{
        if((s16)this->skelAnime.curFrame == 1){
            this->missileCoolTimer = 60;
            CyberGanon_SpawnMissile(this,play);
        }
        if((s16)this->skelAnime.curFrame >= 1) {
            CyberGanon_SpawnSmokeFromGun(this,play);
        }
    }
}

void CyberGanon_SetupFireMissileUpward(CyberGanon* this,PlayState* play){
    f32 endframes = Animation_GetLastFrame(&gCyberGanonObj_AnimFireMssileUpward);
    play->envCtx.lightSettingOverride = 3;
    play->envCtx.lightBlendRateOverride = 20;
    Animation_Change(&this->skelAnime, &gCyberGanonObj_AnimFireMssileUpward, 1.0f, 0.0f, endframes, ANIMMODE_ONCE, -6.0f);
    this->actionFunc = CyberGanon_FireMissileUpward;
}

void CyberGanon_FireMissileUpward(CyberGanon* this,PlayState* play){
    if(SkelAnime_Update(&this->skelAnime)){
        play->envCtx.lightSettingOverride = 1;
        
        if(this->actor.xzDistToPlayer <= MELEE_ATTACK_DISTANCE && (!TOUCH_WALL)){
            CyberGanon_SetupJumpBack(this,play);
        }else{
            CyberGanon_SetupActionJudge(this,play);
        }
    }else{
        if((s16)this->skelAnime.curFrame == 1 || (s16)this->skelAnime.curFrame == 14 || (s16)this->skelAnime.curFrame == 27){
            this->missileCoolTimer = 80;
            CyberGanon_SpawnMissile(this,play);
        }
        if((s16)this->skelAnime.curFrame >= 1) {
            CyberGanon_SpawnSmokeFromGun(this,play);
        }
    }
}


void CyberGanon_SetupLunge(CyberGanon* this,PlayState* play){
    this->headDamageable = 0;
    this->actor.speedXZ = 0;
    if(this->failToHitCount >= 2){
        this->failToHitCount = this->failToHitCount - 2;
    }else{
        this->failToHitCount  = 0;
    }
    f32 endframes = Animation_GetLastFrame(&gCyberGanonObj_AnimLunge);
    Animation_Change(&this->skelAnime, &gCyberGanonObj_AnimLunge, 1.0f, 0.0f, endframes, ANIMMODE_ONCE, -6.0f);
    this->actionFunc = CyberGanon_Lunge;
}

void CyberGanon_Lunge(CyberGanon* this,PlayState* play){
    Player* player = GET_PLAYER(play);
    f32 dis;
    s16 yawTarget;
    s16 yawTemp;
    if(this->shouldBackHome){
        yawTarget = Actor_WorldYawTowardPoint(&this->actor, &this->actor.home.pos);
        yawTemp = Actor_WorldYawTowardPoint(&this->actor, &this->actor.home.pos) - this->actor.shape.rot.y;
        dis = Math_Vec3f_DistXZ(&this->actor.world.pos, &this->actor.home.pos);
    }else{
        yawTarget = this->actor.yawTowardsPlayer;
        dis = Math_Vec3f_DistXZ(&this->actor.world.pos, &player->actor.world.pos);

    }
    if(!SkelAnime_Update(&this->skelAnime)){
        if (this->skelAnime.curFrame > 19.0f) {
            Math_SmoothStepToS(&this->actor.shape.rot.y, yawTarget, 1, 0x50, 0x0);
            if(ABS(dis) < MELEE_ATTACK_DISTANCE * 2){
                Math_SmoothStepToF(&this->actor.velocity.y, -5.0f, 0.1f, 1.0f, 0.2f);
            }
            CyberGanon_SpawnSmokeFromGun(this,play);
        } 
        if (Animation_OnFrame(&this->skelAnime, 17.0f)) {  
            if(this->shouldBackHome){ 
                this->actor.speedXZ = 30.0f;
                this->actor.velocity.y = 16.0f;
            }else{
                this->actor.speedXZ = 27.0f;
                this->actor.velocity.y = 10.0f;
            }
        }
    }
    if(this->skelAnime.curFrame > 19.0f && this->actor.bgCheckFlags & BGCHECKFLAG_GROUND){
        CyberGanon_SetupLandAfterLunge(this,play);
    }
    this->actor.world.rot.y = this->actor.shape.rot.y;
}

void CyberGanon_SetupLandAfterLunge(CyberGanon* this,PlayState* play){
    this->headDamageable = 0;
    Audio_PlayActorSfx2(&this->actor, NA_SE_EN_MGANON_WALK);
    Actor_SpawnFloorDustRing(play, &this->actor, &this->feetPos[FOOT_LEFT], 25.0f, 3, 8.0f, 500, 10, true);
    Actor_SpawnFloorDustRing(play, &this->actor, &this->feetPos[FOOT_RIGHT], 25.0f, 3, 8.0f, 500, 10, true);
    func_80033E88(&this->actor, play, 1, 0xA);

    this->actor.speedXZ = 0;
    f32 endframes = Animation_GetLastFrame(&gCyberGanonObj_AnimLandAfterLunge);
    Animation_Change(&this->skelAnime, &gCyberGanonObj_AnimLandAfterLunge, 1.5f, 0.0f, 34.0F, ANIMMODE_ONCE, -6.0f);
    this->actionFunc = CyberGanon_LandAfterLunge;
}

void CyberGanon_LandAfterLunge(CyberGanon* this,PlayState* play){
    if(SkelAnime_Update(&this->skelAnime)){
        this->actionFunc = CyberGanon_SetupActionJudge;
    }else{
        if (this->skelAnime.curFrame > 19.0f) {
            this->headDamageable = 1;
        }
    }
}


void CyberGanon_Update(CyberGanon* this, PlayState* play) {
    s16 yaw = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;
    f32 disToHome = ABS(Math_Vec3f_DistXZ(&this->actor.world.pos, &this->actor.home.pos));
    CyberGanon_CollisionCheck(this,play);

    if(this->actor.colorFilterTimer == 0){
        DECR(this->actionTimer);
        DECR(this->slowTurnTimer);
        if(this->headHitcCounter > 0){
            this->eyeTexIndex = this->headHitcCounter ;
            if(this->fallDownTimer != 0){
                this->fallDownTimer--;
            }else{
                this->headHitcCounter--;
                if(!this->hasClub){
                    this->fallDownTimer = 120;
                }else{
                    this->fallDownTimer = 80;
                }
            } 
        }else{
            CyberGanon_UpdateEye(this);
        }
        if(this->hasClub && this->clubAlpha < 255){
            this->clubAlpha = this->clubAlpha + 5;
            //Debug_Text(255,0,0,2,5,"clubAlpha : %d",this->clubAlpha);
        }
        // if(this->hasClub){
        //     func_8002F974(&this->actor, NA_SE_EV_DUMMY45 - SFX_FLAG);
        // }
    }
    if((!this->shouldBackHome) && (disToHome >= JUMP_BACK_DISTANCE * 4)){//1200
        this->shouldBackHome = 1;
    }else if(this->shouldBackHome && (disToHome < JUMP_BACK_DISTANCE * 2)){
        this->shouldBackHome = 0;
    }
    
    //Debug_Text(255,0,0,1,6,"health : %d",this->actor.colChkInfo.health);
    Actor_MoveForward(this);
    this->actionFunc(this, play);
}


void CyberGanon_Destroy(CyberGanon* this, PlayState* play) {
    Effect_Delete(play, this->effectIndex);
    Collider_DestroyJntSph(play, &this->bodyArmorCollider);
    Collider_DestroyJntSph(play, &this->damageCollider);
    Collider_DestroyQuad(play, &this->clubCollider);

}

void CyberGanon_SetupDeath(CyberGanon* this, PlayState* play){
    this->actor.speedXZ = 0.0f;
    this->clubAttack = 0;
    this->headDamageable = 0;
    play->envCtx.lightSettingOverride = 1;
    this->actor.flags &= ~ ACTOR_FLAG_TARGETABLE;
    Audio_QueueSeqCmd(0x1 << 28 | SEQ_PLAYER_BGM_MAIN << 24 | 0x100FF);
    f32 endframes = Animation_GetLastFrame(&gCyberGanonObj_AnimDie);
    Audio_PlayActorSfx2(&this->actor, NA_SE_EN_MGANON_DEAD1);
    Animation_Change(&this->skelAnime, &gCyberGanonObj_AnimDie, 1.0f, 0.0f, endframes, ANIMMODE_ONCE, -6.0f);
    this->actionFunc = CyberGanon_Death;

}

void CyberGanon_Death(CyberGanon* this, PlayState* play){
    Vec3f effPos;
    Vec3f effPosBase;
    Vec3f effZeroVec = { 0.0f, 0.0f, 0.0f };
    effPos = this->actor.world.pos;
    effPos.x += Math_SinS(this->actor.shape.rot.y) * 60.0f;
    effPos.z += Math_CosS(this->actor.shape.rot.y) * 60.0f;
    effPosBase = effPos;
    f32 disToHome = Math_Vec3f_DistXZ(&this->actor.world.pos, &this->actor.home.pos);
    if(SkelAnime_Update(&this->skelAnime)){
        Audio_QueueSeqCmd(SEQ_PLAYER_BGM_MAIN << 24 | NA_BGM_BOSS_CLEAR);
        CustomFlag_Set(0x10,1);
        if(disToHome > 150.0f){
            Actor_Spawn(&play->actorCtx, play, ACTOR_DOOR_WARP1,  this->actor.home.pos.x,  this->actor.home.pos.y,
            this->actor.home.pos.z, 0, 0, 0, WARP_DUNGEON_ADULT);
        }else{
            Actor_Spawn(&play->actorCtx, play, ACTOR_DOOR_WARP1,  this->actor.world.pos.x + 250.0f,  this->actor.home.pos.y,
            this->actor.world.pos.z + 250.0f, 0, 0, 0, WARP_DUNGEON_ADULT);  
        }        
        this->actor.update = CyberGanon_Nothing;
            //Actor_Kill(&this->actor);
    }else{
        if((s16)this->skelAnime.curFrame == 34 || (s16)this->skelAnime.curFrame == 65){
            Actor_SpawnFloorDustRing(play, &this->actor, &this->tailVentPos, 25.0f, 3, 8.0f, 500, 10, true);
            func_80033E88(&this->actor, play, 1, 0xa);
            Audio_PlayActorSfx2(&this->actor, NA_SE_EN_MGANON_WALK);
        }
        if((s16)this->skelAnime.curFrame == 65){
            Audio_PlayActorSfx2(&this->actor, NA_SE_EN_MGANON_SWDIMP);

        }
        if((s16)this->skelAnime.curFrame == 40){
            play->envCtx.lightSettingOverride = 2;
            this->actionTimer = 60;
        }
        if((s16)this->skelAnime.curFrame >= 40){
            s32 i;
            Vec3f pos;
            Vec3f velAndAccel = { 0.0f, 0.5f, 0.0f };
            for (i = ((s32)this->skelAnime.animLength - this->actionTimer) >> 1; i >= 0; i--) {
                pos.x = this->actor.world.pos.x + Rand_CenteredFloat(120.0f);
                pos.z = this->actor.world.pos.z + Rand_CenteredFloat(120.0f);
                pos.y = this->actor.world.pos.y + 20.0f + Rand_CenteredFloat(50.0f);
                EffectSsDeadDb_Spawn(play, &pos, &velAndAccel, &velAndAccel, 100, 0, 255, 255, 255, 255, 0, 0, 255, 1, 9, true);

            }            
        }
    }
}

void CyberGanon_Nothing(CyberGanon* this, PlayState* play){
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->bodyArmorCollider.base);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->damageCollider.base);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->clubCollider.base);
    this->eyeTexIndex = 3;

}

void CyberGanon_CollisionCheck(CyberGanon* this, PlayState* play) {
    s8 feetGotValidHit = 0;
    s8 headGotValidHit = 0;
    s16 damage = 1;
    s16 i,j ;
    u8 attackLimb;
    s16 colliderCount = ARRAY_COUNT(sdamageColliderElems);
    Player* player = GET_PLAYER(play);
    s16 yaw = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;

    if(this->clubCollider.base.atFlags & AT_HIT){
        this->clubCollider.base.acFlags &= ~AC_HIT;
        func_8002F71C(play, &this->actor, 8.0f, this->actor.yawTowardsPlayer, 8.0f);
    }else if(!this->clubAttack){
        this->clubCollider.info.toucherFlags &= ~ TOUCH_ON;
    }else{
         this->clubCollider.info.toucherFlags |= TOUCH_ON;
    }
    for(i =0; i < (colliderCount); i++){
        if(!(headGotValidHit || feetGotValidHit)){
            if (this->damageCollider.elements[i].info.bumperFlags & BUMP_HIT) {
                //Debug_Text(255,0,0,1,5,"Hit collider : %d",i);
                this->damageCollider.elements[i].info.bumperFlags &= ~ BUMP_HIT;  
                if(this->damageCollider.elements[i].dim.limb == CYBERGANONOBJ_LIMB_HEAD){
                    headGotValidHit = 1;
                }else{
                    feetGotValidHit = 1;
                    if(this->damageCollider.elements[i].info.acHitInfo->toucher.dmgFlags & DMG_JUMP_SLASH){
                        damage = 2;
                    }
                }
            }
        }
        if (this->damageCollider.elements[i].info.toucherFlags & TOUCH_HIT) {
            this->damageCollider.elements[i].info.toucherFlags &= ~ TOUCH_HIT;  
            if(ABS(this->actor.speedXZ) > this->walkSpeed){
                if((this->damageCollider.elements[i].dim.limb == CYBERGANONOBJ_LIMB_LEG_L) || this->damageCollider.elements[i].dim.limb == CYBERGANONOBJ_LIMB_FOOT_L){
                    if (ABS(this->actor.speedXZ) >= 20.0F){
                        func_8002F6D4(play, &this->actor, 10.0f, this->actor.shape.rot.y + 0x8000, 10.0f, 0);
                    }else{
                        func_8002F6D4(play, &this->actor, 4.0f, this->actor.shape.rot.y + 0x6000, 4.0f, 0);
                    }
                }else{
                    if (ABS(this->actor.speedXZ) >= 20.0F){
                        func_8002F6D4(play, &this->actor, 8.0f, this->actor.shape.rot.y - 0x6000, 8.0f, 0);
                    }else{
                         func_8002F6D4(play, &this->actor, 4.0f, this->actor.shape.rot.y - 0x6000, 4.0f, 0);
                    }
                }
            }
        }
    }

    if(this->actor.colorFilterTimer == 0 && this->actionFunc != CyberGanon_Death ){
        s16 curHealth;
        curHealth = (s16)this->actor.colChkInfo.health;
        if(headGotValidHit){
            this->leftHandBlur = 0;
            if(this->headDamageable){
                this->headHitcCounter++;
                if(this->headHitcCounter >= 2){
                    damage = 6;
                }else{
                    damage = 2;
                }
                if ((curHealth =  curHealth - damage) > 0 ){
                    this->actor.colChkInfo.health = curHealth;
                    Actor_SetColorFilter(&this->actor, 0x4000, 0xFF, 0, 0x10);
                    CyberGanon_SetupHeadDamaged(this, play);
                }else{
                    this->actor.colChkInfo.health = 0;
                    Actor_SetColorFilter(&this->actor, 0x4000, 0xFF, 0, 0x20);
                    CyberGanon_SetupDeath(this,play);
                }
            }else{
                 if(this->actionFunc != CyberGanon_SetupBlock && this->actionFunc != CyberGanon_Block){
                    CyberGanon_SetupBlock(this, play);
                }
            }
        }else if(feetGotValidHit){
            Audio_PlayActorSfx2(&this->actor, NA_SE_EN_GANON_CUTBODY);
            if ((curHealth =  curHealth - damage) > 0 ){
                this->actor.colChkInfo.health = curHealth;
                Actor_SetColorFilter(&this->actor, 0x4000, 0xFF, 0, 0xC);

            }else{
                this->actor.colChkInfo.health = 0;
                Actor_SetColorFilter(&this->actor, 0x4000, 0xFF, 0, 0x20);
                CyberGanon_SetupDeath(this,play);
            }
        }

        if (this->bodyArmorCollider.base.acFlags & AC_HIT) {
            this->bodyArmorCollider.base.acFlags &= ~AC_HIT;
        }else{
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->bodyArmorCollider.base);
        }
        if (this->bodyArmorCollider.base.acFlags & AC_HIT) {
            this->bodyArmorCollider.base.acFlags &= ~AC_HIT;
        }else if((!(headGotValidHit || feetGotValidHit)) && this->actionFunc != CyberGanon_Block){
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->damageCollider.base);
        }
        if(ABS(this->actor.speedXZ) <= this->walkSpeed && this->actionFunc != CyberGanon_FireMissileUpward && this->actionFunc != CyberGanon_Idle)
            CyberGanon_SpawnSmokeFromTail(this,play);
    }

    if(this->hasGrabPlayer){
        if ((this->actor.colChkInfo.health <= 0 || headGotValidHit)) {
            this->hasGrabPlayer = 0;
            player->actor.shape.rot.x = 0;
            player->actor.shape.rot.z = 0;
            player->actor.parent = NULL;
        }
    }else{
        if(gSaveContext.health <= 0){
            player->actor.shape.rot.x = 0;
            player->actor.shape.rot.z = 0;
        }
    }



    if(ABS(this->actor.speedXZ) > 0.0f){
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->damageCollider.base);
    }

    colliderCount = ARRAY_COUNT(sbodyArmorColliderElems);
    for(i =0; i < (colliderCount); i++){
        attackLimb = this->bodyArmorCollider.elements[i].dim.limb;
        if(this->cannonMeeleAttack ){
            if (attackLimb == CYBERGANONOBJ_LIMB_OUTTERARM_R){
                this->bodyArmorCollider.elements[i].info.toucherFlags |= TOUCH_ON;
            }else{
                this->bodyArmorCollider.elements[i].info.toucherFlags &= ~ TOUCH_ON;
            }
        }else{
          if(attackLimb == CYBERGANONOBJ_LIMB_HAND_L || attackLimb == CYBERGANONOBJ_LIMB_OUTTERARM_L){
                this->bodyArmorCollider.elements[i].info.toucherFlags |= TOUCH_ON;
            }else{
                this->bodyArmorCollider.elements[i].info.toucherFlags &= ~ TOUCH_ON;
            }
        }
        if (this->bodyArmorCollider.elements[i].info.toucherFlags & TOUCH_HIT) {
            //Debug_Text(255,0,0,1,5,"Hit collider : %d",i);
            if(attackLimb == CYBERGANONOBJ_LIMB_HAND_L || attackLimb == CYBERGANONOBJ_LIMB_OUTTERARM_L){
                if(!this->clubAttack){
                    this->hasGrabPlayer = 1;
                }
                if(this->hasClub){
                    this->bodyArmorCollider.elements[i].info.toucher.damage = 0x10;
                    func_8002F71C(play, &this->actor, 8.0f, this->actor.yawTowardsPlayer, 8.0f);
                }
                this->failToHitCount = 0;
            }
            if(attackLimb == CYBERGANONOBJ_LIMB_OUTTERARM_R){
                s16 yaw;
                //yaw = Actor_WorldYawTowardPoint(&player->actor, &this->cannonGunPos);
                func_8002F6D4(play, &this->actor, 7.0f, this->actor.yawTowardsPlayer, 6.0f, 0);
                this->failToHitCount = 0;
            }
            if(ABS(this->actor.speedXZ) > this->walkSpeed && ABS(this->actor.speedXZ) < 20.0F){
                func_8002F6D4(play, &this->actor, 4.0f, this->actor.shape.rot.y + 0x8000, 4.0f, 0);
            }else if (ABS(this->actor.speedXZ) >= 20.0F){
                func_8002F6D4(play, &this->actor, 8.0f, this->actor.shape.rot.y + 0x8000, 8.0f, 0);
            }

            this->bodyArmorCollider.elements[i].info.toucherFlags &= ~TOUCH_HIT;
        }
    }
    if((!this->hasGrabPlayer && this->actionFunc == CyberGanon_GrabLink) || ABS(this->actor.speedXZ) > this->walkSpeed || this->cannonMeeleAttack || this->clubAttack){
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->bodyArmorCollider.base);
    }
    Actor_UpdateBgCheckInfo(play, &this->actor, 60.0f, 60.0f, 100.0f,UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2);

    CollisionCheck_SetOC(play, &play->colChkCtx, &this->bodyArmorCollider.base);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->damageCollider.base);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->clubCollider.base);
}

void CyberGanon_SetupHeadDamaged(CyberGanon* this, PlayState* play) {
    this->actor.speedXZ = 0.0f;
    this->clubAttack = 0;
    this->headDamageable = 0;
    play->envCtx.lightSettingOverride = 2;
    play->envCtx.lightBlendRateOverride = 20;
    if(!this->hasClub){
        this->fallDownTimer = 120;
    }else{
        this->fallDownTimer = 80;
    }
    if(this->headHitcCounter < 2 ){
        Audio_PlayActorSfx2(&this->actor, NA_SE_EN_MGANON_DAMAGE);
        Animation_MorphToPlayOnce(&this->skelAnime, &gCyberGanonObj_AnimHeadDamaged, 4.0f);
    }else{
        Audio_PlayActorSfx2(&this->actor, NA_SE_EN_MGANON_ROAR);
        Animation_MorphToPlayOnce(&this->skelAnime, &gCyberGanonObj_AnimHeadDamagedAndFallDown, 4.0f);
    }
    this->actionFunc =  CyberGanon_HeadDamaged;

}

void CyberGanon_HeadDamaged(CyberGanon* this, PlayState* play) {
    if(SkelAnime_Update(&this->skelAnime)){
        this->actor.colorFilterTimer = 0;
        play->envCtx.lightSettingOverride = 1;
        CyberGanon_SetupActionJudge(this,play);
    }else{
        if(this->headHitcCounter >= 2 ){
            this->headHitcCounter = 0;
            if((s16)this->skelAnime.curFrame == 11){
                Actor_SpawnFloorDustRing(play, &this->actor, &this->tailVentPos, 25.0f, 3, 8.0f, 500, 10, true);
                func_80033E88(&this->actor, play, 1, 0xa);
                Audio_PlayActorSfx2(&this->actor, NA_SE_EV_STONE_BOUND);
                Audio_PlayActorSfx2(&this->actor, NA_SE_EN_MGANON_WALK);
            }
            if((s16)this->skelAnime.curFrame == 46){
                Actor_SpawnFloorDustRing(play, &this->actor, &this->feetPos[FOOT_LEFT], 25.0f, 3, 8.0f, 500, 10, true);
                Actor_SpawnFloorDustRing(play, &this->actor, &this->feetPos[FOOT_RIGHT], 25.0f, 3, 8.0f, 500, 10, true);
                func_80033E88(&this->actor, play, 1, 0x4);
                Audio_PlayActorSfx2(&this->actor, NA_SE_EN_MGANON_WALK);
            }
        }
    }
}

s32 CyberGanon_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3f* pos, Vec3s* rot, void* thisx) {
    CyberGanon* this = (void*)thisx;
    if(this->actionFunc == CyberGanon_FireMissilePrepare){
        if (limbIndex == CYBERGANONOBJ_LIMB_OUTTERARM_R) {
            rot->x += this->targetGunPos.x;
        }
    }
    return 0;
}

void CyberGanon_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3s* rot, void* thisx) {
    CyberGanon* this = (void*)thisx;
    Vec3f focusOffest = {0,-25.0f,0};
    Vec3f footOffset = {0,-9.0f,0};
    Vec3f handOffset = {2000.0f,-1000.0f,-3000.0f};
    Vec3f cannonGunffset = {0,0,-70.0f};
    Vec3f zeroOffset = {0,0,0};
    Vec3f handOffsetInner = {0.0f, 100.0f, 300.0f};
    Vec3f handOffsetOutter = { 800.0f, -300.0f, -300.0f};
    Vec3f handPosInner;
    Vec3f handPosOutter;
    Vec3f clubPosInner;
    Vec3f clubPosOutter;
    Vec3f clubOffsetInner = {-700.0f, 0.0f, 0.0f};
    Vec3f clubOffsetOutter = { 700.0f, 0.0f, 12300.0f};
    
    if (limbIndex == CYBERGANONOBJ_LIMB_NECK) {
        Matrix_MultVec3f(&focusOffest, &this->actor.focus.pos);
    }
    if (limbIndex == CYBERGANONOBJ_LIMB_HEAD) {
        Matrix_MultVec3f(&zeroOffset, &this->headPos);
    }
    if (limbIndex == CYBERGANONOBJ_LIMB_CHEST) {
        Matrix_MultVec3f(&zeroOffset, &this->chestPos);
    }
    if (limbIndex == CYBERGANONOBJ_LIMB_FOOT_L) {
        Matrix_MultVec3f(&footOffset, &this->feetPos[FOOT_LEFT]);
    }
    if (limbIndex == CYBERGANONOBJ_LIMB_FOOT_R) {
        Matrix_MultVec3f(&footOffset, &this->feetPos[FOOT_RIGHT]);
    }
    if (limbIndex == CYBERGANONOBJ_LIMB_INNERCLUB) {
        if(this->hasClub){
                Matrix_MultVec3f(&clubOffsetInner, &clubPosInner);
                Matrix_MultVec3f(&clubOffsetOutter, &this->outterClubPos);
            if (func_80090480(play, &this->clubCollider, &this->clubInfo, &clubPosInner, &this->outterClubPos) && this->clubAttack) {
                EffectBlure_AddVertex(Effect_GetByIndex(this->effectIndex), &clubPosInner, &this->outterClubPos);
            }
        }
    }
    if (limbIndex == CYBERGANONOBJ_LIMB_GUNHOLE) {
        MtxF temp0;
        Vec3s temp1;
        Matrix_Push();
        Matrix_MultVec3f(&cannonGunffset, &this->cannonGunRot);
        Matrix_RotateX(35 * M_PI / 64, MTXMODE_APPLY);
        Matrix_Get(&temp0);
        Matrix_MtxFToYXZRotS(&temp0, &temp1, 0);
        this->cannonGunRot.x = temp1.x;
        this->cannonGunRot.y = temp1.y;
        this->cannonGunRot.z = temp1.z;
        Matrix_Pop();

        Matrix_MultVec3f(&cannonGunffset, &this->cannonGunPos);
    }
    if (limbIndex == CYBERGANONOBJ_LIMB_TAIL) {
        Matrix_MultVec3f(&zeroOffset, &this->tailVentPos);
    }
    if (limbIndex == CYBERGANONOBJ_LIMB_OUTTERARM_R) {
        Matrix_MultVec3f(&cannonGunffset, &this->outterArmRPos);
    }
    if (limbIndex == CYBERGANONOBJ_LIMB_HAND_L) {
        MtxF temp0;
        Vec3s temp1;
        Matrix_Push();
        Matrix_MultVec3f(&handOffset, &this->grabPosRot.pos);
        Matrix_RotateX(35 * M_PI / 64, MTXMODE_APPLY);
        Matrix_Get(&temp0);
        Matrix_MtxFToYXZRotS(&temp0, &temp1, 0);
        this->grabPosRot.rot.x = temp1.x;
        this->grabPosRot.rot.y = temp1.y;
        this->grabPosRot.rot.z = temp1.z;
        Matrix_Pop();
        if (this->leftHandBlur) {
            Matrix_MultVec3f(&handOffsetInner, &handPosInner);
            Matrix_MultVec3f(&handOffsetOutter, &handPosOutter);
            EffectBlure_AddVertex(Effect_GetByIndex(this->effectIndex), &handPosInner, &handPosOutter);
        }
    }

    if (limbIndex == CYBERGANONOBJ_LIMB_OUTTERARM_L) {
        Matrix_MultVec3f(&zeroOffset, &this->outterArmLPos);
    }
     Collider_UpdateSpheres(limbIndex, &this->bodyArmorCollider);
     Collider_UpdateSpheres(limbIndex, &this->damageCollider);
}

void CyberGanon_UpdateEye(CyberGanon* this) {
    if (this->blinkTimer != 0) {
        this->blinkTimer--;
    } else {
        this->eyeTexIndex++;
        if (this->eyeTexIndex >= 3) {
            this->eyeTexIndex = 0;
            this->blinkTimer = Rand_ZeroFloat(40.0f) + 20.0f;
        }
    }
}

void CyberGanon_Draw(CyberGanon* this, PlayState* play) {
    void* EyeTexel[] = {
        gCyberGanonObj_TexGanonEyeOpenCi43, //Open
        gCyberGanonObj_TexGanonEyeOpenCi42, //Half Open
        gCyberGanonObj_TexGanonEyeOpenCi41, //Squeeze
        gCyberGanonObj_TexGanonEyeOpenCi40, //Shut 
    };

    OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(EyeTexel[this->eyeTexIndex]));
    Gfx_SetupDL_25Opa(play->state.gfxCtx);
    //gDPSetPrimColor(POLY_OPA_DISP++, 0x80, 0x80, this->bodyPrimColor.r, this->bodyPrimColor.g, this->bodyPrimColor.b, 255);
    SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->jointTable, CYBERGANONOBJ_LIMB_MAX,CyberGanon_OverrideLimbDraw, CyberGanon_PostLimbDraw,this);
    if(this->hasClub){
        if(this->hasClub){
            gSPSegment(POLY_OPA_DISP++, 0x08, Gfx_TexScroll(play->state.gfxCtx, 0, this->scrollY, 32, 64));
            if(this->actionFunc!= CyberGanon_Death){
                this->scrollY += 5;
            }
            Gfx_SetupDL_25Xlu(play->state.gfxCtx);
            gSPDisplayList(POLY_OPA_DISP++, gCyberGanonObj_DlClubObj);
        }       
     }
    CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
}
