#include <uLib.h>
#include "object/0x020E-TankHeadObj.h"
#include "TankHead.h"
#include "overlays/actors/ovl_En_Bom/z_en_bom.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h"


#define FLAGS (ACTOR_FLAG_0 | ACTOR_FLAG_2 | ACTOR_FLAG_4)
#define PLAYER_IN_FIRE_RANGE ((this->actor.xzDistToPlayer < 1200.0f) && (ABS(this->actor.yDistToPlayer) < 500.0f)&&(this->actor.xzDistToPlayer >= 220.0f))
#define PLAYER_IN_MELEE_RANGE ( (ABS(this->actor.yDistToPlayer) < 150.0f)&&(this->actor.xzDistToPlayer < 220.0f))
#define TANK_ORIGINAL_HEALTH (2)

void TankHead_Init(TankHead* this, PlayState* play);
void TankHead_Destroy(TankHead* this, PlayState* play);
void TankHead_Update(TankHead* this, PlayState* play);
void TankHead_Draw(TankHead* this, PlayState* play);
void TankHead_SetupSeek(TankHead* this, PlayState* play);
void TankHead_Seek(TankHead* this, PlayState* play);
void TankHead_SetupFire(TankHead* this, PlayState* play);
void TankHead_Fire(TankHead* this, PlayState* play);
void TankHead_SetupTurnToPlayer(TankHead* this, PlayState* play);
void TankHead_TurnToPlayer(TankHead* this, PlayState* play);
void TankHead_SetupAlert(TankHead* this, PlayState* play);
void TankHead_Alert(TankHead* this, PlayState* play);
u8 TankHead_UpdateRot(TankHead* this, PlayState* play);
void TankHead_SetupRotateAttack(TankHead* this, PlayState* play);
void TankHead_RotateAttack(TankHead* this, PlayState* play);
void TankHead_SetupStunned(TankHead* this, PlayState* play);
void TankHead_Stunned(TankHead* this, PlayState* play);
void TankHead_SetupDamaged(TankHead* this, PlayState* play);
void TankHead_Damaged(TankHead* this, PlayState* play);
void TankHead_DetectPlayer(TankHead* this, PlayState* play);
void TankHead_SetupDead(TankHead* this, PlayState* play);
void TankHead_Dead(TankHead* this, PlayState* play);
void TankHead_SetupHitGround(TankHead* this, PlayState* play);
void TankHead_HitGround(TankHead* this, PlayState* play);
void TankHead_SetupKillingMode(TankHead* this, PlayState* play);
void TankHead_KillingMode(TankHead* this, PlayState* play);
void TankHead_SpawnShell(TankHead* this, PlayState* play, u8 shellType);
void TankHead_SpawnSmokeFromGun(TankHead* this, PlayState* play);


u8 TankHead_UpdateGunRotOnly(TankHead* this, PlayState* play);
static enum EyeTexlName
{
    ANGRY,
    MAD,
    STARTLE
};

static ColliderJntSphElementInit ssphereElems[] = {
    /* InnerGun.l */ {
        .info.bumperFlags = BUMP_ON,

        .info.elemType = ELEMTYPE_UNK1,
        .info.toucher = { 0xFFCFFFFF, 0x00, 0x10 },
        .info.toucherFlags = TOUCH_ON | TOUCH_SFX_NORMAL, //AT
        .info.bumper = { 0xFFCFFFFF, 0x00, 0x00 },
        .info.bumperFlags = BUMP_ON, //AC
        .info.ocElemFlags = OCELEM_ON,  //OC
        
        .dim.limb = 6,
        .dim.modelSphere ={
            { 0, 0, -34 },
            34,
        },
        .dim.scale = 100,
    },
    /* InnerGun.r */ {
        .info.bumperFlags = BUMP_ON,
        .info.elemType = ELEMTYPE_UNK1,
        .info.toucher = { 0xFFCFFFFF, 0x00, 0x10 },
        .info.toucherFlags = TOUCH_ON | TOUCH_SFX_NORMAL, //AT
        .info.bumper = { 0xFFCFFFFF, 0x00, 0x00 },
        .info.bumperFlags = BUMP_ON, //AC
        .info.ocElemFlags = OCELEM_ON,  //OC
        .dim.limb = 3,
        .dim.modelSphere ={
            { 0, 0, -34 },
            34,
        },
        .dim.scale = 100,
    },
    /* outGun.l */ {
        .info.bumperFlags = BUMP_ON,
        
        .info.elemType = ELEMTYPE_UNK1,
        .info.toucher = { 0xFFCFFFFF, 0x00, 0x10 },
        .info.toucherFlags = TOUCH_ON | TOUCH_SFX_NORMAL, //AT
        .info.bumper = { 0xFFCFFFFF, 0x00, 0x00 },
        .info.bumperFlags = BUMP_ON, //AC
        .info.ocElemFlags = OCELEM_ON,  //OC
        .dim.limb = 7,
        .dim.modelSphere ={
            { 0, 0, -34 },
            34,
        },
        .dim.scale = 100,
    },
    /* outGun.r */ {
        .info.bumperFlags = BUMP_ON,
        .info.elemType = ELEMTYPE_UNK1,
        .info.toucher = { 0xFFCFFFFF, 0x00, 0x10 },
        .info.toucherFlags = TOUCH_ON | TOUCH_SFX_NORMAL, //AT
        .info.bumper = { 0xFFCFFFFF, 0x00, 0x00 },
        .info.bumperFlags = BUMP_ON, //AC
        .info.ocElemFlags = OCELEM_ON,  //OC
        .dim.limb = 4,
        .dim.modelSphere ={
            { 0, 0, -34 },
            34,
        },
        .dim.scale = 100,
    },    /* shellEntry.l */ {
        .info.bumperFlags = BUMP_ON,
        .info.elemType = ELEMTYPE_UNK1,
        .info.toucher = { 0xFFCFFFFF, 0x00, 0x10 },
        .info.toucherFlags = TOUCH_ON | TOUCH_SFX_NORMAL, //AT
        .info.bumper = { 0xFFCFFFFF, 0x00, 0x00 },
        .info.bumperFlags = BUMP_ON, //AC
        .info.ocElemFlags = OCELEM_ON,  //OC
        .dim.limb = 8,
        .dim.modelSphere ={
            { 16, 1, -3 },
            32,
        },
        .dim.scale = 100,
    },
    /* shellEntry.r */ {
        .info.bumperFlags = BUMP_ON,
        .info.elemType = ELEMTYPE_UNK1,
        .info.toucher = { 0xFFCFFFFF, 0x00, 0x10 },
        .info.toucherFlags = TOUCH_ON | TOUCH_SFX_NORMAL, //AT
        .info.bumper = { 0xFFCFFFFF, 0x00, 0x00 },
        .info.bumperFlags = BUMP_ON, //AC
        .info.ocElemFlags = OCELEM_ON,  //OC
        .dim.limb = 5,
        .dim.modelSphere ={
            { 16, 1, -3 },
            32,
        },
        .dim.scale = 100,
    },
};

static ColliderJntSphInit ssphereInit = {
    .base.shape   = COLSHAPE_JNTSPH,
    .base.colType = COLTYPE_METAL,
    .base.acFlags = AC_ON | AC_TYPE_PLAYER,
    .base.atFlags = AT_ON | AT_TYPE_ENEMY,
    .base.ocFlags1 = OC1_ON | OC1_TYPE_PLAYER,
    .base.ocFlags2 = OC2_TYPE_1,
    .count        = ARRAY_COUNT(ssphereElems),
    .elements     = ssphereElems,
};

const ActorInit sTankHead_InitVars = {
    .id = 0x0218,
    .category     = ACTORCAT_ENEMY,
    .flags        = FLAGS,
    .objectId = 0x020E,
    .instanceSize = sizeof(TankHead),
    .init         = (ActorFunc)TankHead_Init,
    .destroy      = (ActorFunc)TankHead_Destroy,
    .update       = (ActorFunc)TankHead_Update,
    .draw         = (ActorFunc)TankHead_Draw
};

static ColliderCylinderInit sHitboxInit = {
    {
        COLTYPE_METAL,
        AT_NONE,
        AC_ON | (AC_TYPE_PLAYER | AC_TYPE_OTHER),
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK1,
        { 0x00000000, 0x00, 0x00 }, 
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_ON,
        BUMP_ON,
        OCELEM_ON,
    },
    { 90, 64, 0, { 0, 0, 0 } },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(naviEnemyId, NAVI_ENEMY_TWINROVA, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 5500, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(gravity, -1000, ICHAIN_STOP),
};

void TankHead_Init(TankHead* this, PlayState* play) {
    Actor_ProcessInitChain(&this->actor, sInitChain);
   this->actor.uncullZoneForward = 2000.0f;
   this->actor.uncullZoneDownward = 1000.0f;
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 48.0f);
    this->actor.colChkInfo.health = TANK_ORIGINAL_HEALTH;
    Rand_Seed((u32)osGetTime());
    SkelAnime_InitFlex(play, &this->skelAnime, &gTankHeadObj_SkelTankHeadObj, &gTankHeadObj_AnimSeek, this->jointTable,
                       this->morphTable, TANKHEADOBJ_LIMB_MAX);
    this->actor.colChkInfo.mass = MASS_IMMOVABLE;
    this->switchFlags = (this->actor.params >> 8) & 0xFF;//params : **00 (hex).
    if(Flags_GetSwitch(play, this->switchFlags) && this->switchFlags != 0x0 && this->switchFlags != 0x3F){
        Actor_Kill(this);
    }
    Collider_InitCylinder(play, &this->hitbox);
    Collider_SetCylinder(play, &this->hitbox, &this->actor, &sHitboxInit);
    Collider_InitJntSph(play, &this->gunCollider);
    Collider_SetJntSph(play, &this->gunCollider, &this->actor, &ssphereInit, this->gunColliderItems);    
    TankHead_SetupSeek(this,play);
}

void TankHead_Destroy(TankHead* this, PlayState* play) {
    Collider_DestroyJntSph(play, &this->gunCollider);
    Collider_DestroyCylinder(play, &this->hitbox);

}

void TankHead_SetupSeek(TankHead* this, PlayState* play){
    this->whichEyes = STARTLE;
    this->actor.speedXZ = 0.0f;
    Animation_MorphToLoop(&this->skelAnime, &gTankHeadObj_AnimSeek, -2.0f);
    this->actionFunc = TankHead_Seek;
}

void TankHead_SetupAlert(TankHead* this, PlayState* play){
    this->whichEyes = ANGRY;
    Animation_MorphToPlayOnce(&this->skelAnime, &gTankHeadObj_AnimAlert, 4.0f);
    this->actionFunc = TankHead_Alert;
}

void TankHead_Alert(TankHead* this, PlayState* play){
     Player* player = GET_PLAYER(play);
    TankHead_UpdateRot(this,play);
    if(SkelAnime_Update(&this->skelAnime)){
        TankHead_SetupTurnToPlayer(this,play);
    }else if(PLAYER_IN_MELEE_RANGE){
        if((ABS(this->headYawToPlayer) <= 0x1500) && ((!(player->stateFlags1 & PLAYER_STATE1_26)))){
            TankHead_SetupHitGround(this,play);
        }else{
            TankHead_SetupRotateAttack(this,play);
        }
    }
    if((this->skelAnime.curFrame == 5.0f)){
            SoundEffect_PlayDefaultOneshot(SOUND_TANK_STRUCK, &this->actor.world.pos, this);
    }
}

void TankHead_SetupTurnToPlayer(TankHead* this, PlayState* play){
    this->whichEyes = ANGRY;
    this->actionFunc = TankHead_TurnToPlayer;
}

void TankHead_TurnToPlayer(TankHead* this, PlayState* play){
    Player* player = GET_PLAYER(play);
    if(this->findPlayerTimer && PLAYER_IN_FIRE_RANGE ){
        if(Rand_ZeroOne() >0.7 && (!(player->stateFlags1 & PLAYER_STATE1_26)) && (this->actor.xzDistToPlayer < 350.0f)){
            TankHead_SetupKillingMode(this,play);
        }else if(this->fireTimer == 0 && (!(player->stateFlags1 & PLAYER_STATE1_26))){
            TankHead_UpdateRot(this,play);
             TankHead_SetupFire(this,play);
         }
    }else if(PLAYER_IN_MELEE_RANGE){
        if((ABS(this->headYawToPlayer) <= 0x2710) &&(!(player->stateFlags1 & PLAYER_STATE1_26))){
            TankHead_SetupHitGround(this,play);
        }else{
            TankHead_SetupRotateAttack(this,play);
        }
    }else{
        TankHead_SetupSeek(this,play);
    }
    
}


void TankHead_SetupHitGround(TankHead* this,PlayState* play){
    this->whichEyes = MAD;
    Animation_Change(&this->skelAnime, gTankHeadObj_AnimHitGround, 1.0f, 0.0f,
                     Animation_GetLastFrame(gTankHeadObj_AnimHitGround), ANIMMODE_ONCE, 4.0f);
    this->actionFunc = TankHead_HitGround;

}
void TankHead_HitGround(TankHead* this,PlayState* play){
    if(SkelAnime_Update(&this->skelAnime)){
        TankHead_SetupAlert(this,play);
    }else{
        if((this->skelAnime.curFrame > 18.0f) && (this->skelAnime.curFrame < 28.0f)){
            this->meleeAttacking = 1;
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->gunCollider.base);
        }else if(this->skelAnime.curFrame < 15.0f){
            Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer + this->randOffset, 10, 0XAA0, 0);
        }
        if(this->skelAnime.curFrame == 8.0f){
            SoundEffect_PlayDefaultOneshot(SOUND_TANK_STRUCK, &this->actor.world.pos, this);
        }
        if(this->skelAnime.curFrame == 22.0f){
             SoundEffect_PlayDefaultOneshot(SOUND_METAL_FIERCE_HIT, &this->actor.world.pos, this);
        }

    }
}

void TankHead_SetupKillingMode(TankHead* this,PlayState* play){
    this->whichEyes = MAD;
    SoundEffect_PlayDefaultOneshot(SOUND_TANK_TURNING, &this->actor.world.pos, this);
    Animation_Change(&this->skelAnime, gTankHeadObj_AnimFireKillingMode, 1.0f, 0.0f,
                     Animation_GetLastFrame(gTankHeadObj_AnimFireKillingMode), ANIMMODE_ONCE, 4.0f);
    this->actionFunc = TankHead_KillingMode;
}

void TankHead_KillingMode(TankHead* this,PlayState* play){
    if(SkelAnime_Update(&this->skelAnime)){
        TankHead_SetupTurnToPlayer(this,play);
    }else{
        if((this->skelAnime.curFrame == 20.0f) || (this->skelAnime.curFrame == 32.0f)||(this->skelAnime.curFrame == 44.0f)){
            TankHead_SpawnSmokeFromGun(this,play);
            TankHead_SpawnShell(this,play,2);
        }else if((this->skelAnime.curFrame > 20.0f)){
            Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer + this->randOffset, 3, 0XAA0, 0);
            
        }
        if((this->skelAnime.curFrame == 5.0f) || (this->skelAnime.curFrame == 52.0f) ){
            SoundEffect_PlayDefaultOneshot(SOUND_TANK_STRUCK, &this->actor.world.pos, this);
        }
    }
    
}

void TankHead_SetupRotateAttack(TankHead* this,PlayState* play){
    AnimationHeader* animation;
    this->whichEyes = MAD;
    SoundEffect_PlayDefaultOneshot(SOUND_TANK_TURNING, &this->actor.world.pos, this);
    if ((s16)(this->actor.yawTowardsPlayer - this->actor.shape.rot.y) >= 0) {
        animation = &gTankHeadObj_AnimRotateL;
    } else {
        animation = &gTankHeadObj_AnimRotateR;
    }
    Animation_Change(&this->skelAnime, animation, 1.0f, 0.0f,
                     Animation_GetLastFrame(animation), ANIMMODE_ONCE, 4.0f);
    this->actionFunc = TankHead_RotateAttack;
}

void TankHead_RotateAttack(TankHead* this,PlayState* play){
    TankHead_UpdateRot(this, play);
    if(SkelAnime_Update(&this->skelAnime)){
        TankHead_SetupAlert(this,play);
    }else{
        if((this->skelAnime.curFrame > 26.0f) && (this->skelAnime.curFrame < 43.0f)){
            this->meleeAttacking = 1;
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->gunCollider.base);
        }else if(this->skelAnime.curFrame == 7){
            SoundEffect_PlayDefaultOneshot(SOUND_TANK_STRUCK, &this->actor.world.pos, this);
            this->meleeAttacking = 0;
        }
        if(this->skelAnime.curFrame == 26.0f){
            SoundEffect_PlayDefaultOneshot(SOUND_LYNEL_ATTACK_SPEAR2, &this->actor.world.pos, this);
        }
    }
}

void TankHead_SetupFire(TankHead* this, PlayState* play){
    this->whichEyes = ANGRY;
    Animation_MorphToPlayOnce(&this->skelAnime, &gTankHeadObj_AnimFire, 4.0f);
    this->actionFunc = TankHead_Fire;
}

void TankHead_Fire(TankHead* this, PlayState* play){
    TankHead_UpdateRot(this, play);
    if(SkelAnime_Update(&this->skelAnime)){
        TankHead_SetupAlert(this,play);
    }else{
        if(this->skelAnime.curFrame == 1.0f){
            TankHead_SpawnShell(this,play,0);
            this->whichEyes = MAD;  
        }
        if((this->skelAnime.curFrame > 1.0f) && (this->skelAnime.curFrame < 6.0f)){
            TankHead_SpawnSmokeFromGun(this,play);
        }
    }

}

void TankHead_SpawnShell(TankHead* this, PlayState* play, u8 shellType) {
    s16 PitchL;
    s16 PitchR;
    PitchL = Math_Vec3f_Pitch(&this->twinInnerGunPos[0], &this->twinOutGunPos[0]);
    PitchR = Math_Vec3f_Pitch(&this->twinInnerGunPos[1], &this->twinOutGunPos[1]);
    SoundEffect_PlayDefaultOneshot(SOUND_SHELL_FIRE, &this->actor.world.pos, this);
    Actor_Spawn(&play->actorCtx, play, 0x0219, this->twinOutGunPos[0].x, this->twinOutGunPos[0].y,
           this->twinOutGunPos[0].z, PitchL, this->actor.shape.rot.y, 0, shellType);
    Actor_Spawn(&play->actorCtx, play, 0x0219, this->twinOutGunPos[1].x, this->twinOutGunPos[1].y,
           this->twinOutGunPos[1].z, PitchR, this->actor.shape.rot.y, 0, shellType);
}


u8 TankHead_UpdateRot(TankHead* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Vec3f playerPos = player->actor.world.pos;
    playerPos.y = playerPos.y + 50.0f;
    s16 rotateSpeed;
    s16 targetHeadRotation = this->actor.yawTowardsPlayer + this->randOffset;
    if((this->actor.xzDistToPlayer > 500.0f || this->actor.xzDistToPlayer < 250.0f)){
        rotateSpeed = 10;
    }else{
        rotateSpeed = CLAMP((this->actor.xzDistToPlayer/ (100.0f))*1.5f ,2.0f,8.0f);
    }
    Vec3f sumtwinInnerGunPos;
    sumtwinInnerGunPos.x = (this->twinInnerGunPos[0].x + this->twinInnerGunPos[1].x)/2;
    sumtwinInnerGunPos.y = (this->twinInnerGunPos[0].y + this->twinInnerGunPos[1].y)/2;
    sumtwinInnerGunPos.z = (this->twinInnerGunPos[0].z + this->twinInnerGunPos[1].z)/2;
    if (this->twinInnerGunRot.x > 0x2B91) {
        this->twinInnerGunRot.x = 0x2B91;
    }
    this->pitch = Math_Vec3f_Pitch(&sumtwinInnerGunPos, &playerPos);
    if (this->pitch > 0x1AAA) {
        this->pitch = 0x1AAA;
    }
    if(Math_SmoothStepToS(&this->twinInnerGunRot.x, this->pitch, 10, 0xAA0, 0) && Math_SmoothStepToS(&this->actor.shape.rot.y, targetHeadRotation, rotateSpeed, 0XAA0, 0)
/*&& 
        (Math_SmoothStepToS(&this->headRot.y, -this->actor.shape.rot.y + this->actor.yawTowardsPlayer, 10, 0x600, 0))*/){
        return 1;
    }else{
        return 0;
    }
    
}

u8 TankHead_UpdateGunRotOnly(TankHead* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Vec3f playerPos = player->actor.world.pos;
    playerPos.y = playerPos.y + 50.0f;
    Vec3f sumtwinInnerGunPos;
    sumtwinInnerGunPos.x = (this->twinInnerGunPos[0].x + this->twinInnerGunPos[1].x)/2;
    sumtwinInnerGunPos.y = (this->twinInnerGunPos[0].y + this->twinInnerGunPos[1].y)/2;
    sumtwinInnerGunPos.z = (this->twinInnerGunPos[0].z + this->twinInnerGunPos[1].z)/2;
    if (this->twinInnerGunRot.x > 0x2B91) {
        this->twinInnerGunRot.x = 0x2B91;
    }
    this->pitch = Math_Vec3f_Pitch(&sumtwinInnerGunPos, &playerPos);
    if(Math_SmoothStepToS(&this->twinInnerGunRot.x, this->pitch, 10, 0xAA0, 0)){
        return 1;
    }else{
        return 0;
    }
    
}

void TankHead_SpawnSmokeFromLauncher(TankHead* this, PlayState* play) {
    s32 pad;
    s16 i;
    Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    Vec3f accel = { 0.0f, 0.3f, 0.0f };
    Vec3f posL[3];
    Vec3f posR[3];
    for(i = 0; i<3; i++){
        posL[i] = this->smokeLauncherPosL[i];
        posR[i] = this->smokeLauncherPosR[i];
    }
    s16 randAngle = Rand_CenteredFloat(0x4000);
    f32 randCos;
    f32 randSin;

    randCos = Math_CosS(this->actor.shape.rot.y + randAngle);
    randSin = Math_SinS(this->actor.shape.rot.y + randAngle);
    this->bombSmokeEnvColor.r =200;
    this->bombSmokeEnvColor.g =200;
    this->bombSmokeEnvColor.b =200;
    this->bombSmokeEnvColor.a =50;
    this->bombSmokePrimColor.r =200;
    this->bombSmokePrimColor.g =200;
    this->bombSmokePrimColor.b =200;
    this->bombSmokePrimColor.a =50;
    if (this->bombSmokeEnvColor.g != 0) {
        this->bombSmokeEnvColor.g++;
    }
    if (this->bombSmokeEnvColor.b != 0) {
        this->bombSmokeEnvColor.b += 15;
    }
    if (this->bombSmokeEnvColor.r != 0) {
        this->bombSmokeEnvColor.r++;
    }
    velocity.x = randSin * 3.5f;
    velocity.y = this->bombSmokeEnvColor.r * 0.02f;
    velocity.z = randCos * 3.5f;
    accel.x = ((Rand_ZeroOne() * 0.1f) + 0.15f) * -randSin;
    accel.z = ((Rand_ZeroOne() * 0.1f) + 0.15f) * -randCos;
    for(i = 0; i<3; i++){
        func_8002836C(play, &posL[i], &velocity, &accel, &this->bombSmokePrimColor, &this->bombSmokeEnvColor, 100, 25, 20);
        func_8002836C(play, &posR[i], &velocity, &accel, &this->bombSmokePrimColor, &this->bombSmokeEnvColor, 100, 25, 20);
    }
    SoundEffect_PlayDefaultHeld(SOUND_TANK_RUNNING,1.0,&this->actor.world.pos,this);

}

void TankHead_SpawnSmokeFromGun(TankHead* this, PlayState* play) {
     s32 pad;
    s16 i;
    Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    Vec3f accel = { 0.0f, 0.3f, 0.0f };
    Vec3f posL;
    Vec3f posR;
    posL = this->twinOutGunPos[0];
    posR = this->twinOutGunPos[1];
    s16 randAngle = Rand_CenteredFloat(0x4000);
    f32 randCos;
    f32 randSin;

    randCos = Math_CosS(this->actor.shape.rot.y + randAngle);
    randSin = Math_SinS(this->actor.shape.rot.y + randAngle);
    this->bombSmokeEnvColor.r =100;
    this->bombSmokeEnvColor.g =100;
    this->bombSmokeEnvColor.b =100;
    this->bombSmokeEnvColor.a =150;
    this->bombSmokePrimColor.r =20;
    this->bombSmokePrimColor.g =20;
    this->bombSmokePrimColor.b =20;
    this->bombSmokePrimColor.a =100;
    if (this->bombSmokeEnvColor.g != 0) {
        this->bombSmokeEnvColor.g++;
    }
    if (this->bombSmokeEnvColor.b != 0) {
        this->bombSmokeEnvColor.b += 15;
    }
    if (this->bombSmokeEnvColor.r != 0) {
        this->bombSmokeEnvColor.r++;
    }
    velocity.x = randSin * 3.5f;
    velocity.y = this->bombSmokeEnvColor.r * 0.02f;
    velocity.z = randCos * 3.5f;
    accel.x = ((Rand_ZeroOne() * 0.1f) + 0.15f) * -randSin;
    accel.z = ((Rand_ZeroOne() * 0.1f) + 0.15f) * -randCos;
    func_8002836C(play, &posL, &velocity, &accel, &this->bombSmokePrimColor, &this->bombSmokeEnvColor, 100, 25, 20);
    func_8002836C(play, &posR, &velocity, &accel, &this->bombSmokePrimColor, &this->bombSmokeEnvColor, 100, 25, 20);
    
}


void TankHead_Seek(TankHead* this, PlayState* play){
    //TankHead_ResetRotation(this,play);
    SkelAnime_Update(&this->skelAnime);
    if(this->findPlayerTimer){
        TankHead_SetupAlert(this,play);
    }
}

void TankHead_Update(TankHead* this, PlayState* play) {
    u8 randFactor = (play->gameplayFrames & 1) ? 1 :-1;
    TankHead_CollisionCheck(this,play);
    if(this->madState){
            this->whichEyes = MAD;
    }else{
        if(this->findPlayerTimer > 0){
            this->findPlayerTimer --;
        }
    }
    if(!PLAYER_IN_MELEE_RANGE){
        this->randOffset = (s16)(Rand_Centered() * 10)*(0x1) * (randFactor);
    }
    if(this->fireTimer > 0){
        this->fireTimer--;
    }


    if(this->actor.colorFilterTimer == 0){
        TankHead_DetectPlayer(this,play);
        if(this->findPlayerTimer){
            TankHead_SpawnSmokeFromLauncher(this,play);
        }
    }
    this->actor.world.rot.y = this->actor.shape.rot.y;
    this->headYawToPlayer = (this->actor.yawTowardsPlayer - this->headRot.y - this->actor.shape.rot.y);
    this->actionFunc(this, play);

}

void TankHead_DetectPlayer(TankHead* this, PlayState* play) {
    if((PLAYER_IN_FIRE_RANGE && (ABS(this->headYawToPlayer) <= 0x2710)) || (PLAYER_IN_MELEE_RANGE && (ABS(this->headYawToPlayer) <= 0x5420))){
        if(this->findPlayerTimer >= 20 ){
            this->findPlayerTimer++ ;
        }else{
             this->findPlayerTimer = 20 ;
        }
    }
}

void TankHead_HealthCheck(TankHead* this, PlayState* play) {
    if (this->actor.colChkInfo.health <= 0){
        TankHead_SetupDead(this,play);
    }else{
        TankHead_SetupDamaged(this,play); 
    }

}

void TankHead_CollisionCheck(TankHead* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    u8 gotValidAttack = 0;
    u8 gotStunned = 0;
    if(this->actor.colChkInfo.health <= (s16)(TANK_ORIGINAL_HEALTH/2) && (this->actor.colChkInfo.health == 0)){
        this->madState = 1;
    }
    if (this->hitbox.base.acFlags & AC_HIT) {
        this->hitbox.base.acFlags &= ~AC_HIT;
        this->findPlayerTimer = this->findPlayerTimer + 20;
        if((this->hitbox.info.acHitInfo->toucher.dmgFlags & DMG_EXPLOSIVE) || (this->hitbox.info.acHitInfo->toucher.dmgFlags & DMG_HAMMER)){
            gotValidAttack = 1;
        }else if(this->hitbox.info.acHitInfo->toucher.dmgFlags & DMG_DEKU_NUT){
            gotStunned = 1;
        }
    }else if(this->gunCollider.base.acFlags & AC_HIT){
        this->gunCollider.base.acFlags &= ~AC_HIT;
    }else if (this->gunCollider.base.atFlags & AT_HIT) {
        this->gunCollider.base.atFlags &= ~AT_HIT;
        if (this->gunCollider.base.at == &player->actor) {
            Audio_PlayActorSfx2(&player->actor, NA_SE_PL_BODY_HIT);
            func_8002F71C(play, &this->actor, 6.0f, this->actor.yawTowardsPlayer, 6.0f);
        }
    }
    if(gotValidAttack){
        Actor_SetColorFilter(&this->actor, 0x4000, 0xFF, 0, 0x20);
        gotValidAttack = 0;
        this->actor.colChkInfo.health--;
        if(this->actor.colChkInfo.health<=0){
            TankHead_SetupDead(this,play);
        }else{
            TankHead_SetupDamaged(this,play);
        }
    }else if(gotStunned){
        Actor_SetColorFilter(&this->actor, 0, 0xFF, 0, 8);
        gotStunned =0;
        this->findPlayerTimer = this->findPlayerTimer + 20;
        TankHead_SetupStunned(this,play);
    }

    Collider_UpdateCylinder(&this->actor, &this->hitbox);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->hitbox.base);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->gunCollider.base);
    if(this->actor.colorFilterTimer == 0){
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->hitbox.base);
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->gunCollider.base);
    }

}

void TankHead_SetupDamaged(TankHead* this, PlayState* play) {
    this->whichEyes = STARTLE;

    SoundEffect_PlayDefaultOneshot(SOUND_METAL_FIERCE_HIT, &this->actor.world.pos, this);
    Animation_MorphToPlayOnce(&this->skelAnime, &gTankHeadObj_AnimDamaged, 4.0f);
    this->actionFunc = TankHead_Damaged;

}

void TankHead_Damaged(TankHead* this, PlayState* play) {
    if(SkelAnime_Update(&this->skelAnime)){
        if(this->findPlayerTimer){
            TankHead_SetupAlert(this,play);
        }else{
            TankHead_SetupSeek(this,play);
        }
    }else{
        if((this->skelAnime.curFrame == 13.0f)){
            SoundEffect_PlayDefaultOneshot(SOUND_MACHINE_SHUT_DOWN, &this->actor.world.pos, this);
        }
    }

}

void TankHead_SetupDead(TankHead* this, PlayState* play) {
    this->whichEyes = MAD;
    if(this->switchFlags != 0x0 && this->switchFlags != 0x3F){
        Flags_SetSwitch(play, this->switchFlags);
    }
    SoundEffect_PlayDefaultOneshot(SOUND_METAL_FIERCE_HIT, &this->actor.world.pos, this);
    Animation_MorphToPlayOnce(&this->skelAnime, &gTankHeadObj_AnimDead, 4.0f);
    this->actor.flags &= ~ACTOR_FLAG_0;
    this->actionFunc = TankHead_Dead;

}

void TankHead_Dead(TankHead* this, PlayState* play) {
    EnBom* bomb;

    if(SkelAnime_Update(&this->skelAnime)){
        bomb = (EnBom*)Actor_Spawn(&play->actorCtx, play, ACTOR_EN_BOM, this->actor.world.pos.x,
        this->actor.world.pos.y + 20.0f, this->actor.world.pos.z, 0, 0, 0x601, BOMB_BODY);
        if (bomb != NULL) {
            bomb->timer = 0;
        }
        Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos, 0xA0);
        Actor_Kill(&this->actor);
    
    }else{
        if(((this->skelAnime.curFrame > 15.0f) && (this->skelAnime.curFrame < 20.0f)) || ((this->skelAnime.curFrame > 26.0f) && (this->skelAnime.curFrame < 30.0f))){
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->gunCollider.base);
        }else if((this->skelAnime.curFrame == 45.0f)){
            SoundEffect_PlayDefaultOneshot(SOUND_MACHINE_SHUT_DOWN, &this->actor.world.pos, this);
            Enemy_StartFinishingBlow(play, &this->actor);
            bomb = (EnBom*)Actor_Spawn(&play->actorCtx, play, ACTOR_EN_BOM, this->actor.world.pos.x,
                this->actor.world.pos.y + 20.0f, this->actor.world.pos.z, 0, 0, 0x601, BOMB_BODY);
            if (bomb != NULL) {
                bomb->timer = 0;
            }
        }
        if((this->skelAnime.curFrame == 15.0f)||(this->skelAnime.curFrame == 26.0f)||(this->skelAnime.curFrame == 48.0f)){
            SoundEffect_PlayDefaultOneshot(SOUND_METAL_FIERCE_HIT, &this->actor.world.pos, this);
        }
        if((this->skelAnime.curFrame == 15.0f)||(this->skelAnime.curFrame == 26.0f)){
            TankHead_SpawnSmokeFromGun(this,play);
            TankHead_SpawnShell(this,play,0);
        }
        this->actor.colorFilterTimer ++;
    }

}

void TankHead_SetupStunned(TankHead* this, PlayState* play){
    this->whichEyes = STARTLE;
    Audio_PlayActorSfx2(&this->actor, NA_SE_EN_GOMA_JR_FREEZE);
    SoundEffect_PlayDefaultOneshot(SOUND_MACHINE_SHUT_DOWN, &this->actor.world.pos, this);
    Animation_Change(&this->skelAnime, gTankHeadObj_AnimStun, 1.0f, 0.0f,
                     Animation_GetLastFrame(gTankHeadObj_AnimStun), ANIMMODE_ONCE, 4.0f);
    this->actionFunc = TankHead_Stunned;

}

void TankHead_Stunned(TankHead* this, PlayState* play) {
    if(SkelAnime_Update(&this->skelAnime) && this->actor.colorFilterTimer == 0){
        if (this->actor.colChkInfo.health <= 0) {
            TankHead_SetupDead(this,play);
        }else if(this->actor.colorFilterTimer == 0){
            TankHead_SetupSeek(this,play);
        }
    }
}


void TankHead_ResetRotation(TankHead* this, PlayState* play) {
    this->twinInnerGunRot.x = 0;
    this->headRot.y = 0;
}


s32 TankHead_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3f* pos, Vec3s* rot, void* thisx) {
    TankHead* this = (void*)thisx;
    if (limbIndex == TANKHEADOBJ_LIMB_INNERGUN_R) {
        rot->x += this->twinInnerGunRot.x;
    }
    if (limbIndex == TANKHEADOBJ_LIMB_INNERGUN_L) {
        rot->x += this->twinInnerGunRot.x;
    }
    if (limbIndex == TANKHEADOBJ_LIMB_HEAD) {
        rot->y += this->headRot.y;
    }
    return 0;
}

void TankHead_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3s* rot, void* thisx) {
    TankHead* this = (void*)thisx;
    static Vec3f Offest = {0,0,0};
    static Vec3f focusOffest = {0.0f,42.0f,0.0f};
    if (limbIndex == TANKHEADOBJ_LIMB_INNERGUN_L) {
        Matrix_MultVec3f(&Offest, &this->twinInnerGunPos[0]);

    }
    if (limbIndex == TANKHEADOBJ_LIMB_INNERGUN_R) {
        Matrix_MultVec3f(&Offest, &this->twinInnerGunPos[1]);

    }
    if (limbIndex == TANKHEADOBJ_LIMB_SHELLENTRY_L) {
        Matrix_MultVec3f(&Offest, &this->twinOutGunPos[0]);
    }
    if (limbIndex == TANKHEADOBJ_LIMB_SHELLENTRY_R) {
        Matrix_MultVec3f(&Offest, &this->twinOutGunPos[1]);
    }
    if (limbIndex == TANKHEADOBJ_LIMB_SMOKELAUNCHERUP_L) {
        Matrix_MultVec3f(&Offest, &this->smokeLauncherPosL[0]);
    }
    if (limbIndex == TANKHEADOBJ_LIMB_SMOKELAUNCHERDOWN0_L) {
        Matrix_MultVec3f(&Offest, &this->smokeLauncherPosL[1]);
    }
    if (limbIndex == TANKHEADOBJ_LIMB_SMOKELAUNCHERDOWN1_L) {
        Matrix_MultVec3f(&Offest, &this->smokeLauncherPosL[2]);
    }
    if (limbIndex == TANKHEADOBJ_LIMB_SMOKELAUNCHERUP_R) {
        Matrix_MultVec3f(&Offest, &this->smokeLauncherPosR[0]);
    }
    if (limbIndex == TANKHEADOBJ_LIMB_SMOKELAUNCHERDOWN0_R) {
        Matrix_MultVec3f(&Offest, &this->smokeLauncherPosR[1]);
    }
    if (limbIndex == TANKHEADOBJ_LIMB_SMOKELAUNCHERDOWN1_R) {
        Matrix_MultVec3f(&Offest, &this->smokeLauncherPosR[2]);
    }
    if (limbIndex == TANKHEADOBJ_LIMB_NECK) {
        Matrix_MultVec3f(&focusOffest, &this->actor.focus.pos);
    }
    Collider_UpdateSpheres(limbIndex, &this->gunCollider);

}

void TankHead_Draw(TankHead* this, PlayState* play) {
   void* EyeTexel[] = {
        gTankHeadObj_Tex0x7E500,
        gTankHeadObj_Tex0x7E501,
        gTankHeadObj_Tex0x7E502
    };

     gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(EyeTexel[this->whichEyes]));
     Gfx_SetupDL_25Opa(play->state.gfxCtx);
     SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->jointTable, TANKHEADOBJ_LIMB_MAX,TankHead_OverrideLimbDraw, TankHead_PostLimbDraw,this);
}
