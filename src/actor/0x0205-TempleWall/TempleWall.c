#include "TempleWall.h"
#include "object/0x0197-TempleWallObj.h"
#include "assets/objects/gameplay_dangeon_keep/gameplay_dangeon_keep.h"
#include "overlays/effects/ovl_Effect_Ss_Kakera/z_eff_ss_kakera.h"

void TempleWall_Init(Actor* thisx, PlayState* play);
void TempleWall_Destroy(TempleWall* this, PlayState* play);
void TempleWall_Update(TempleWall* this, PlayState* play);
void TempleWall_Draw(TempleWall* this, PlayState* play);
void TempleWall_FlagCheck(TempleWall* this, PlayState* play);
void TempleWall_SetupShake(TempleWall* this, PlayState* play);
void TempleWall_Break(TempleWall* this, PlayState* play);
void TempleWall_SpawnDust(TempleWall* this, PlayState* play);
void TempleWall_SpawnBreakEffect(TempleWall* this, PlayState* play);
void TempleWall_Shake(TempleWall* this, PlayState* play);



const ActorInit sTempleWall_InitVars = {
    .id = 0x0205,
    .category     = ACTORCAT_BG,
    .flags        = 0x00000030,
    .objectId = 0x0197,
    .instanceSize = sizeof(TempleWall),
    .init         = (ActorFunc)TempleWall_Init,
    .destroy      = (ActorFunc)TempleWall_Destroy,
    .update       = (ActorFunc)TempleWall_Update,
    .draw         = (ActorFunc)TempleWall_Draw
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 16, ICHAIN_STOP),
};

void TempleWall_Init(Actor* thisx, PlayState* play) {
    TempleWall* this = (TempleWall*)thisx;
    Player* player = GET_PLAYER(play);
    if(play->sceneId == 0x68){
        Actor_ProcessInitChain(&this->dyna.actor, sInitChain);
    }
    CollisionHeader* colHeader = NULL;
    DynaPolyActor_Init(&this->dyna, DPM_PLAYER);
    CollisionHeader_GetVirtual(&gTempleWallObj_CollTempleWalObj, &colHeader);
    this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, thisx, colHeader);
    this->shakeTimer = 0;
    this->randFactor.x = Rand_ZeroOne() * 65535.5f;
    this->randFactor.y = Rand_ZeroOne() * 65535.5f;
    this->randFactor.z = Rand_ZeroOne() * 65535.5f;

    this->switchFlags = (this->dyna.actor.params >> 8) & 0xFF;  //Variable Form : **00 (hex) 
   if (Flags_GetSwitch(play, this->switchFlags) && (this->switchFlags!= 0x3F) ) {
        Actor_Kill(&this->dyna.actor);
        return;
    }  
    this->actionFunc = TempleWall_FlagCheck;



}

void TempleWall_FlagCheck(TempleWall* this, PlayState* play){

 if (Flags_GetSwitch(play, this->switchFlags)) {
        TempleWall_SetupShake(this, play);
    }

}

void TempleWall_SetupShake(TempleWall* this, PlayState* play){
    this->shakeTimer = 60;
    this->actionFunc = TempleWall_Shake;
}


void TempleWall_Shake(TempleWall* this, PlayState* play){
     s32 quakeIndex;
     if(this->shakeTimer == 30 ){
     OnePointCutscene_AttentionSetSfx(play, &this->dyna.actor, NA_SE_SY_CORRECT_CHIME);
     }
    if(this->shakeTimer > 0){
        SfxSource_PlaySfxAtFixedWorldPos(play, &this->dyna.actor.world.pos, 16, NA_SE_EV_BLOCK_SHAKE);
        this->randFactor.x += 10000;
        this->dyna.actor.world.rot.x = (s16)(Math_SinS(this->randFactor.x) * 300.0f) + this->dyna.actor.home.rot.x;
        this->dyna.actor.world.rot.z = (s16)(Math_CosS(this->randFactor.x) * 300.0f) + this->dyna.actor.home.rot.z;
        this->dyna.actor.shape.rot.x = this->dyna.actor.world.rot.x;
        this->dyna.actor.shape.rot.z = this->dyna.actor.world.rot.z;
        this->randFactor.y += 18000;
        this->dyna.actor.world.pos.y = Math_SinS(this->randFactor.y) + this->dyna.actor.home.pos.y;
        this->randFactor.z += 18000;
        this->dyna.actor.world.pos.x = Math_SinS(this->randFactor.z) * 3.0f + this->dyna.actor.home.pos.x;
        this->dyna.actor.world.pos.z = Math_CosS(this->randFactor.z) * 3.0f + this->dyna.actor.home.pos.z;
        this->shakeTimer--;
    }else{
        this->actionFunc = TempleWall_Break;
    }

}


void TempleWall_Break(TempleWall* this, PlayState* play){

    SfxSource_PlaySfxAtFixedWorldPos(play, &this->dyna.actor.world.pos, 40, NA_SE_EV_WALL_BROKEN);
    TempleWall_SpawnBreakEffect(this,play);
    TempleWall_SpawnDust(this,play);
    Actor_Kill(&this->dyna.actor);
    return;
}

void TempleWall_Destroy(TempleWall* this, PlayState* play) {
    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);

}

void TempleWall_Update(TempleWall* this, PlayState* play) {
    this->actionFunc(this, play);
}

void TempleWall_SpawnBreakEffect(TempleWall* this, PlayState* play) {
    s32 i;
    s32 j;
    Vec3f velocity;
    Vec3f pos;
    s16 arg5;
    Actor* thisx = &this->dyna.actor;
    f32 sin = Math_SinS(thisx->shape.rot.y);
    f32 cos = Math_CosS(thisx->shape.rot.y);
    f32 tmp1;
    f32 tmp2;
    s16 arg9;

    for (i = 0; i < 5; i++) {
        pos.y = (20 * i) + thisx->world.pos.y;
        for (j = 0; j < 5; j++) {
            tmp1 = 16 * (j - 2);

            pos.x = (tmp1 * cos) + thisx->world.pos.x;
            pos.z = -(tmp1 * sin) + thisx->world.pos.z;

            tmp1 = 3.0f * Rand_ZeroOne() * (j - 2);
            tmp2 = 6.0f * Rand_ZeroOne();

            velocity.x = (tmp2 * sin) + (tmp1 * cos);
            velocity.y = 18.0f * Rand_ZeroOne();
            velocity.z = (tmp2 * cos) - (tmp1 * sin);

            arg9 = ((Rand_ZeroOne() - 0.5f) * 11.0f * 1.4f) + 11.0f;
            arg5 = (arg9 >= 15) ? 32 : 64;

            if (Rand_ZeroOne() < 5.0f) {
                arg5 |= 1;
            }

             EffectSsKakera_Spawn(play, &pos, &velocity, &thisx->world.pos, -540, arg5, 20, 20, 0, arg9, 2, 32, 100,
                                  KAKERA_COLOR_NONE, OBJECT_GAMEPLAY_DANGEON_KEEP, gBrownFragmentDL);
        }
    }
}

void TempleWall_SpawnDust(TempleWall* this, PlayState* play) {
    s32 pad;
    Vec3f pos;

    pos = this->dyna.actor.world.pos;
    pos.y += 10.0f;

    func_80033480(play, &pos, 0.0f, 0, 600, 300, 1);

    pos.x = ((Rand_ZeroOne() - 0.5f) * 80.0f) + this->dyna.actor.world.pos.x;
    pos.y = (Rand_ZeroOne() * 100.0f) + this->dyna.actor.world.pos.y;
    pos.z = ((Rand_ZeroOne() - 0.5f) * 80.0f) + this->dyna.actor.world.pos.z;

    func_80033480(play, &pos, 100.0f, 4, 200, 250, 1);
}

s32 TempleWall_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3f* pos, Vec3s* rot, void* thisx) {
    TempleWall* this = (void*)thisx;
    
    return 0;
}

void TempleWall_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3s* rot, void* thisx) {
    TempleWall* this = (void*)thisx;
}

void TempleWall_Draw(TempleWall* this, PlayState* play) {
    Gfx_DrawDListOpa(play, &gTempleWallObj_DlTempleWalObj);

}
