#include "Missile.h"
#include "object/0x003A-MissileObj.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h"
#include "overlays/actors/ovl_En_Bom/z_en_bom.h"
#define HIGH_SPEED (1)
void Missile_Init(Missile* this, PlayState* play);
void Missile_Destroy(Missile* this, PlayState* play);
void Missile_Update(Missile* this, PlayState* play);
void Missile_Draw(Missile* this, PlayState* play);

const ActorInit sMissile_InitVars = {
    .id = 0x0246,
    .category     = ACTORCAT_EXPLOSIVE,
    .flags        = 0x00000030,
    .objectId = 0x003A,
    .instanceSize = sizeof(Missile),
    .init         = (ActorFunc)Missile_Init,
    .destroy      = (ActorFunc)Missile_Destroy,
    .update       = (ActorFunc)Missile_Update,
    .draw         = (ActorFunc)Missile_Draw
};

static ColliderQuadInit sHitboxInit = {
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
        { 0x00000008, 0x00, 0x08 },
        { ~(DMG_SHIELD | DMG_MIR_RAY | DMG_EXPLOSIVE), 0x00, 0x00 },
        TOUCH_ON | TOUCH_NEAREST | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_ON,
    },
    { { { 0.0f, 0.0f, 0.0f}, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } } },
};

void Missile_Init(Missile* this, PlayState* play) {
    EffectBlureInit1 blure;
    this->actor.gravity = 0.0f;
    if(this->actor.params != HIGH_SPEED){
        this->actor.speedXZ = 5.0f;
        this->beginChaseTimer = 10;
        this->timer = 600;
    }else{
        this->actor.speedXZ = 4.0f;
        this->beginChaseTimer = 10;
        ActorShape_Init(&this->actor.shape, 1100.0f, ActorShadow_DrawCircle, 25.0f);
        this->timer = 200;
    }
    Collider_InitQuad(play, &this->hitbox);
    Collider_SetQuad(play, &this->hitbox, &this->actor, &sHitboxInit);
    blure.p1StartColor[0] = 255;
    blure.p1StartColor[1] = 0;
    blure.p1StartColor[2] = 50;
    blure.p1StartColor[3] = 255;

    blure.p2StartColor[0] = 255;
    blure.p2StartColor[1] = 0;
    blure.p2StartColor[2] = 50;
    blure.p2StartColor[3] = 64;

    blure.p1EndColor[0] = 255;
    blure.p1EndColor[1] = 0;
    blure.p1EndColor[2] = 50;
    blure.p1EndColor[3] = 0;

    blure.p2EndColor[0] = 255;
    blure.p2EndColor[1] = 0;
    blure.p2EndColor[2] = 50;
    blure.p2EndColor[3] = 0;

    blure.elemDuration = 8;
    blure.unkFlag = 0;
    blure.calcMode = 0;

    Effect_Add(play, &this->effectIndex, EFFECT_BLURE1, 0, 0, &blure);
}


void Missile_Explode(Missile* this, PlayState* play) {
    EnBom* bomb;
    Vec3f sp2C;
    Vec3f effAccel = { 0.0f, 0.0f, 0.0f };
    Vec3f effVelocity = { 0.0f, 0.0f, 0.0f };
    Vec3f effectPos = this->actor.world.pos;
    Camera_AddQuake(&play->mainCamera, 2, 0x19, 5);
    EffectSsBlast_SpawnWhiteShockwave(play, &effectPos, &effVelocity, &effAccel);
    Audio_PlayActorSfx2(&this->actor, NA_SE_EN_FANTOM_HIT_THUNDER);
    bomb = (EnBom*)Actor_Spawn(&play->actorCtx, play, ACTOR_EN_BOM, this->actor.world.pos.x, this->actor.world.pos.y,
                        this->actor.world.pos.z, 0, 0, 0, BOMB_BODY);
    if (bomb != NULL) {
            bomb->timer = 0;
    }
            
    Actor_Kill(&this->actor);
}

void Missile_Destroy(Missile* this, PlayState* play) {
    Effect_Delete(play, this->effectIndex);
    Collider_DestroyQuad(play, &this->hitbox);
}

void Missile_Update(Missile* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    u8 Explode = 0;
    u8 kickPlayer = 0;
    f32 disToPlayer = Math_Vec3f_DistXZ(&this->actor.world.pos, &player->actor.world.pos);
    Vec3f playerPos = player->actor.world.pos;
    if(this->actor.params != HIGH_SPEED){
      playerPos.y = playerPos.y + 50.0F;
    }else{
      playerPos.y = playerPos.y - 30.0F;
      if(this->beginChaseTimer != 0){
        Math_SmoothStepToS(&this->actor.world.rot.x, this->pitch, 30, 0xFA0, 0);
        this->actor.world.rot.y = this->actor.yawTowardsPlayer;
      }
    }

    Actor_MoveForward(&this->actor);
    //Debug_Text(255,255,255,5,2,"chase timer : %d",this->beginChaseTimer);

    if(this->beginChaseTimer == 0){
        this->pitch = Math_Vec3f_Pitch(&this->actor.world.pos, &playerPos);
        if(this->actor.params == HIGH_SPEED){
            Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 0xBA, 0x2A);
                Math_SmoothStepToS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 0xA, 0x9FF, 0XA);
             
            this->actor.speedXZ = 12.0f;
            this->actor.world.rot.x = this->pitch;
        }else{
            Math_SmoothStepToS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 1, 0x35A, 0x25);
            Math_SmoothStepToS(&this->actor.world.rot.x, this->pitch, 30, 0xFA0, 0);
        }
    }

    this->actor.shape.rot.y = this->actor.world.rot.y;
    this->actor.shape.rot.z = this->actor.world.rot.z;
    this->actor.shape.rot.x = this->actor.world.rot.x;


    this->actor.velocity.y = -Math_SinS(this->actor.world.rot.x) * this->actor.speedXZ;

    if (this->actor.bgCheckFlags & BGCHECKFLAG_WALL || (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) || (this->actor.bgCheckFlags & BGCHECKFLAG_CEILING)) {
        Explode = 1;
    }
    if (this->hitbox.base.ocFlags1 & OC1_HIT) {  
        if(this->hitbox.base.oc->id != 0x0036){
            this->actor.speedXZ = 0;
            if(&player->actor == this->hitbox.base.oc){
                kickPlayer = 1;
            }
            Explode = 1;      
        }     
    }
    if(this->hitbox.base.atFlags & AT_HIT){
        if(&player->actor == this->hitbox.base.at){
            kickPlayer = 1;
        }
        Explode = 1;
    }
    if(this->hitbox.base.acFlags & AC_HIT){
        Explode = 1;
    }
    if(this->timer == 0 ){
        Explode = 1;
    }
    if(kickPlayer){
        func_8002F71C(play, &this->actor, 8.0f, this->actor.world.rot.y, 8.0f);
    }
    if(Explode){
        if(!kickPlayer){
            if (Rand_ZeroOne() < 0.4f) {
                Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos, 0xA0);
            }
            if(this->actor.params == 0){
                Item_DropCollectible(play, &this->actor.world.pos, ITEM00_RECOVERY_HEART);
                Item_DropCollectible(play, &this->actor.world.pos, ITEM00_RECOVERY_HEART);
            }
        }
        Missile_Explode(this,play);
    
    }
    DECR(this->beginChaseTimer);
    DECR(this->timer);
    Actor_UpdateBgCheckInfo(play, &this->actor, 10.0f, 15.0f, 30.0f, UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2 | UPDBGCHECKINFO_FLAG_1);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->hitbox.base);
    CollisionCheck_SetAT(play, &play->colChkCtx, &this->hitbox.base);
    CollisionCheck_SetAC(play, &play->colChkCtx, &this->hitbox.base);

}

s32 Missile_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3f* pos, Vec3s* rot, void* thisx) {
    Missile* this = (void*)thisx;
    
    return 0;
}

void Missile_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3s* rot, void* thisx) {
    Missile* this = (void*)thisx;
}

void Missile_Draw(Missile* this, PlayState* play) {
    Vec3f vec1;
    Vec3f vec2;
    Vec3f sMultVec1 = { 400.0f, -400.0f, 0.0f };
    Vec3f sMultVec2 = { -400.0f, 400.0f, 2800.0f };
    this->yawSpeed = this->yawSpeed + 0x300;
    this->yaw = this->yaw + this->yawSpeed;
    Matrix_MultVec3f(&sMultVec1, &vec1);
    Matrix_MultVec3f(&sMultVec2, &vec2);
    if (func_80090480(play, &this->hitbox, &this->missileInfo, &vec1, &vec2)) {
         Matrix_RotateZ(this->yaw, MTXMODE_APPLY) ;
        EffectBlure_AddVertex(Effect_GetByIndex(this->effectIndex), &vec1, &vec2);
    }
    Gfx_DrawDListOpa(play, gMissileObj_DlMissileObj);
}
