#include "TempleBell.h"
#include "object/0x019B-TempleBellObj.h"
#include "overlays/effects/ovl_Effect_Ss_Kakera/z_eff_ss_kakera.h"

void TempleBell_Init(Actor* thisx, PlayState* play);
void TempleBell_InitCollision(Actor* thisx, PlayState* play);
void TempleBell_Destroy(TempleBell* this, PlayState* play);
void TempleBell_collisionCheck(TempleBell* this, PlayState* play);
void TempleBell_Update(TempleBell* this, PlayState* play);
void TempleBell_Draw(TempleBell* this, PlayState* play);
void TempleBell_SmallYawn(TempleBell* this, PlayState* play);
void TempleBell_SetupSmallYawn(TempleBell* this, PlayState* play);
void TempleBell_BigYawn(TempleBell* this, PlayState* play);
void TempleBell_SetupBigYawn(TempleBell* this, PlayState* play);

const ActorInit sTempleBell_InitVars = {
    .id = 0x0203,
    .category     = ACTORCAT_PROP,
    .flags        = 0x00000030,
    .objectId = 0x019B,
    .instanceSize = sizeof(TempleBell),
    .init         = (ActorFunc)TempleBell_Init,
    .destroy      = (ActorFunc)TempleBell_Destroy,
    .update       = (ActorFunc)TempleBell_Update,
    .draw         = (ActorFunc)TempleBell_Draw
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_METAL,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { DMG_DEFAULT, 0x00, 0x00 },
        { DMG_DEFAULT, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 76, 155, 0, {  0, 0, 0  } },//height 25
};

static InitChainEntry InMinerScale[] = {
    ICHAIN_VEC3F_DIV1000(scale, 48, ICHAIN_STOP),
};

static InitChainEntry InCastleRoom0Scale[] = {
    ICHAIN_VEC3F_DIV1000(scale, 48, ICHAIN_STOP),
};

static InitChainEntry InCastleRoom16Scale[] = {
    ICHAIN_VEC3F_DIV1000(scale, 20, ICHAIN_STOP),
};

static CollisionCheckInfoInit sColChkInfoInit = { 0, 12, 60, MASS_IMMOVABLE};

void TempleBell_Init(Actor* thisx, PlayState* play) {
    TempleBell* this = (TempleBell*)thisx;
    Player* player = GET_PLAYER(play);
    // CollisionHeader* colHeader = NULL;
    // DynaPolyActor_Init(&this->dyna, DPM_PLAYER);
    // CollisionHeader_GetVirtual(&gTempleBellObj_CollTempleBellObj, &colHeader);
    // this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, thisx, colHeader);
    TempleBell_InitCollision(thisx,play);
    if(play->sceneId == 0x68){
        Actor_ProcessInitChain(&this->actor, InMinerScale);
        this->collider.dim.height = 500.0f;  
        this->collider.dim.radius = 270.0f;
        this->collider.dim.pos.y = this->actor.world.pos.y - 500.0f;  
    }else if(play->sceneId == 0x4){
        if((play->roomCtx.curRoom.num == 0)){
            Actor_ProcessInitChain(&this->actor, InCastleRoom0Scale);
            this->collider.dim.height = 450.2f;  
            this->collider.dim.radius = 270.8f;
            this->collider.dim.pos.y = this->actor.home.pos.y - 450.0f; 
        }else{
            Actor_ProcessInitChain(&this->actor, InCastleRoom16Scale);
            this->collider.dim.height = 193.0f; 
            this->collider.dim.radius = 85.0f;
            this->collider.dim.pos.y = this->actor.home.pos.y - 193.0f;  
        }
    }else{
        Actor_ProcessInitChain(&this->actor, InCastleRoom16Scale);
        this->collider.dim.height = 193.0f; 
        this->collider.dim.radius = 85.0f;
        this->collider.dim.pos.y = this->actor.home.pos.y - 193.0f;  
    }


    this->switchFlags = (this->actor.params >> 8) & 0xFF; //Variable Form : **00 (hex)
    this->momentum = 0;
    this->timer = 0;
    this->randFactor.x = Rand_ZeroOne() * 65535.5f;
    this->randFactor.y = Rand_ZeroOne() * 65535.5f;
    this->randFactor.z = Rand_ZeroOne() * 65535.5f;

    this->actionFunc = TempleBell_collisionCheck;
}

void TempleBell_Destroy(TempleBell* this, PlayState* play) {
    Collider_DestroyCylinder(play, &this->collider);
    // DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

void TempleBell_InitCollision(Actor* thisx, PlayState* play) {
    TempleBell* this = (TempleBell*)thisx;
    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
    Collider_UpdateCylinder(&this->actor, &this->collider); 
    CollisionCheck_SetInfo(&thisx->colChkInfo, NULL, &sColChkInfoInit);

}

void TempleBell_collisionCheck(TempleBell* this, PlayState* play){
    Actor* actor;
    s16 hammerHit;
    s16 explosiveHit;
    if (this->collider.base.acFlags & AC_HIT){
        this->collider.base.acFlags &= ~AC_HIT;
        actor = this->collider.base.ac;
        hammerHit = (this->collider.info.acHitInfo->toucher.dmgFlags & DMG_HAMMER);
        explosiveHit = (this->collider.info.acHitInfo->toucher.dmgFlags & DMG_EXPLOSIVE);
        if ((hammerHit) || (explosiveHit)) {
            if((!Flags_GetSwitch(play,this->switchFlags)) && (this->switchFlags != 0x3F)){
                Flags_SetSwitch(play, this->switchFlags);
                OnePointCutscene_Attention(play, &this->actor);
            
            }
            TempleBell_SetupBigYawn(this,play);
        }else{
            TempleBell_SetupSmallYawn(this,play);
        }
     } else if (this->actor.xzDistToPlayer < 1200.0f) {
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
    }
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
}

void TempleBell_SetupBigYawn(TempleBell* this, PlayState* play){
    s32 quakeIndex;
    this->timer = 40;

    SoundEffect_PlayOneshot(SOUND_BELL_BIG, 1.0f, 1.0f,&this->actor.world.pos,1.0f,1000.0f,&this->actor);
    SoundEffect_PlayOneshot(SOUND_BELL_SMALL, 1.0f, 1.0f,&this->actor.world.pos,1.0f,1000.0f,&this->actor);
    SfxSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 16, NA_SE_EV_BLOCK_SHAKE);
    
    quakeIndex = Quake_Add(GET_ACTIVE_CAM(play), 1);
    Quake_SetSpeed(quakeIndex, 80000);
    Quake_SetQuakeValues(quakeIndex, 2, 0, 0, 0);
    Quake_SetCountdown(quakeIndex, this->timer);

    gSaveContext.sunsSongState = SUNSSONG_SPECIAL;

    this->actionFunc = TempleBell_BigYawn;
}

void TempleBell_BigYawn(TempleBell* this, PlayState* play){
    if(this->timer > 0){
        Rumble_Request(this->actor.xyzDistToPlayerSq, 120, 10, 120);
        this->randFactor.x += 10000;
        this->actor.world.rot.x = (s16)(Math_SinS(this->randFactor.x) * 500.0f) + this->actor.home.rot.x;
        this->actor.world.rot.z = (s16)(Math_CosS(this->randFactor.x) * 500.0f) + this->actor.home.rot.z;
        this->actor.shape.rot.x = this->actor.world.rot.x;
        this->actor.shape.rot.z = this->actor.world.rot.z;
        this->randFactor.y += 18000;
        this->actor.world.pos.y = Math_SinS(this->randFactor.y) + this->actor.home.pos.y;
        this->randFactor.z += 18000;
        this->actor.world.pos.x = Math_SinS(this->randFactor.z) * 4.0f + this->actor.home.pos.x;
        this->actor.world.pos.z = Math_CosS(this->randFactor.z) * 4.0f + this->actor.home.pos.z;
        TempleBell_collisionCheck(this,play);
         this->timer--;
    }else{
        gSaveContext.sunsSongState = SUNSSONG_INACTIVE;
        this->actionFunc = TempleBell_collisionCheck;
    }

}

void TempleBell_SetupSmallYawn(TempleBell* this, PlayState* play){
    s32 quakeIndex;
    this->timer = 5;

    SoundEffect_PlayOneshot(SOUND_BELL_SMALL, 1.0f, 1.0f,&this->actor.world.pos,1.0f,1000.0f,&this->actor);
    SfxSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 16, NA_SE_EV_BLOCK_SHAKE);

    quakeIndex = Quake_Add(GET_ACTIVE_CAM(play), 1);
    Quake_SetSpeed(quakeIndex, 50000);
    Quake_SetQuakeValues(quakeIndex, 2, 0, 0, 0);
    Quake_SetCountdown(quakeIndex, this->timer);

    this->actionFunc = TempleBell_SmallYawn;
}



void TempleBell_SmallYawn(TempleBell* this, PlayState* play){
    s16 ywanFactor;
     s32 quakeIndex;
    if(this->timer > 0){
        Rumble_Request(this->actor.xyzDistToPlayerSq, 120, 10, 120);
        this->randFactor.x += 10000;
        this->actor.world.rot.x = (s16)(Math_SinS(this->randFactor.x) * 300.0f) + this->actor.home.rot.x;
        this->actor.world.rot.z = (s16)(Math_CosS(this->randFactor.x) * 300.0f) + this->actor.home.rot.z;
        this->actor.shape.rot.x = this->actor.world.rot.x;
        this->actor.shape.rot.z = this->actor.world.rot.z;
        this->randFactor.y += 18000;
        this->actor.world.pos.y = Math_SinS(this->randFactor.y) + this->actor.home.pos.y;
        this->randFactor.z += 18000;
        this->actor.world.pos.x = Math_SinS(this->randFactor.z) * 3.0f + this->actor.home.pos.x;
        this->actor.world.pos.z = Math_CosS(this->randFactor.z) * 3.0f + this->actor.home.pos.z;
        TempleBell_collisionCheck(this,play);
        this->timer--;
    }else{
        this->actionFunc = TempleBell_collisionCheck;
    }

}


void TempleBell_Update(TempleBell* this, PlayState* play) {
        this->actionFunc(this, play);

}

s32 TempleBell_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3f* pos, Vec3s* rot, void* thisx) {
    TempleBell* this = (void*)thisx;
    
    return 0;
}

void TempleBell_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3s* rot, void* thisx) {
    TempleBell* this = (void*)thisx;
}

void TempleBell_Draw(TempleBell* this, PlayState* play) {
     Gfx_DrawDListOpa(play, &gTempleBellObj_DlTempleBellObj);
}
