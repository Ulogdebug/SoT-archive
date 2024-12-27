#include "WheelBlock.h"
#include "object/0x0079-WheelBlock.h"
#include "assets/objects/gameplay_dangeon_keep/gameplay_dangeon_keep.h"
#include "overlays/effects/ovl_Effect_Ss_Kakera/z_eff_ss_kakera.h"
#define FLAGS (ACTOR_FLAG_4 )

void WheelBlock_Init(WheelBlock* this, PlayState* play);
void WheelBlock_Destroy(WheelBlock* this, PlayState* play);
void WheelBlock_Update(WheelBlock* this, PlayState* play);
void WheelBlock_Draw(WheelBlock* this, PlayState* play);
void WheelBlock_Detect(WheelBlock* this, PlayState* play) ;
void WheelBlock_Break(WheelBlock* this, PlayState* play);
void WheelBlock_SetupShake(WheelBlock* this, PlayState* play);
void WheelBlock_Shake(WheelBlock* this, PlayState* play);
void WheelBlock_SpawnDust(WheelBlock* this, PlayState* play);

const ActorInit sWheelBlock_InitVars = {
    .id = 0x0259,
    .category     = ACTORCAT_BG,
    .flags        = FLAGS,
    .objectId = 0x0079,
    .instanceSize = sizeof(WheelBlock),
    .init         = (ActorFunc)WheelBlock_Init,
    .destroy      = (ActorFunc)WheelBlock_Destroy,
    .update       = (ActorFunc)WheelBlock_Update,
    .draw         = (ActorFunc)WheelBlock_Draw
};

void WheelBlock_Init(WheelBlock* this, PlayState* play) {
    CollisionHeader* colHeader = NULL;
    DynaPolyActor_Init(&this->dyna, DPM_UNK3);
    CollisionHeader_GetVirtual(gWheelBlockj_CollWheelBlock, &colHeader); 
    this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &this->dyna.actor, colHeader);   
    this->switchFlags = (this->dyna.actor.params) & 0x3F;
    this->dyna.actor.targetArrowOffset = 0.0f;
    this->actionFunc = WheelBlock_Shake;
}

void WheelBlock_SpawnDust(WheelBlock* this, PlayState* play) {
    s32 pad;
    Vec3f pos;

    pos = this->dyna.actor.world.pos;
    pos.y += 10.0f;

    func_80033480(play, &pos, 0.0f, 0, 600, 300, 1);

    pos.x = ((Rand_ZeroOne() - 0.5f) * 80.0f) + this->fragmentPos.x;
    pos.y = (Rand_ZeroOne() * 100.0f) +         this->fragmentPos.y;
    pos.z = ((Rand_ZeroOne() - 0.5f) * 80.0f) + this->fragmentPos.z;

    func_80033480(play, &pos, 100.0f, 4, 200, 250, 1);
}

void WheelBlock_WaitToDie(WheelBlock* this, PlayState* play) {
    DECR(this->timer);
    if(this->timer == 30){
        Message_StartTextbox(play,0x6087,&this->dyna.actor);
    }
    if(this->timer==0){
        Actor_Kill(&this->dyna.actor);
    }
}

void WheelBlock_Break(WheelBlock* this, PlayState* play) {
    // s32 i;
    // s32 j;
    // Vec3f velocity;
    // Vec3f pos;
    // s16 arg5;
    // Actor* thisx = &this->dyna.actor;
    // f32 sin = Math_SinS(thisx->shape.rot.y);
    // f32 cos = Math_CosS(thisx->shape.rot.y);
    // f32 tmp1;
    // f32 tmp2;
    // s16 arg9;
    // for (i = 0; i < 5; i++) {
    //     pos.y = (20 * i) + this->fragmentPos.y;
    //     for (j = 0; j < 5; j++) {
    //         tmp1 = 16 * (j - 2);

    //         pos.x = (tmp1 * cos) +  this->fragmentPos.x;
    //         pos.z = -(tmp1 * sin) + this->fragmentPos.z;

    //         tmp1 = 3.0f * Rand_ZeroOne() * (j - 2);
    //         tmp2 = 6.0f * Rand_ZeroOne();

    //         velocity.x = (tmp2 * sin) + (tmp1 * cos);
    //         velocity.y = 18.0f * Rand_ZeroOne();
    //         velocity.z = (tmp2 * cos) - (tmp1 * sin);

    //         arg9 = ((Rand_ZeroOne() - 0.5f) * 11.0f * 1.4f) + 11.0f;
    //         arg5 = (arg9 >= 15) ? 32 : 64;

    //         if (Rand_ZeroOne() < 5.0f) {
    //             arg5 |= 1;
    //         }

    //          EffectSsKakera_Spawn(play, &pos, &velocity, &this->fragmentPos, -540, arg5, 20, 20, 0, arg9, 2, 32, 100,
    //                               KAKERA_COLOR_NONE, OBJECT_GAMEPLAY_DANGEON_KEEP, gBrownFragmentDL);
    //     }
    // }
    Audio_PlayActorSfx2(&this->dyna.actor, NA_SE_EN_IRONNACK_ARMOR_OFF_DEMO);
    SfxSource_PlaySfxAtFixedWorldPos(play, &this->dyna.actor.world.pos, 20, NA_SE_EV_BRIDGE_CLOSE_STOP);
    Flags_SetSwitch(play,this->switchFlags);
    WheelBlock_SpawnDust(this,play);
    this->dyna.actor.draw = NULL;
    DynaPoly_DisableCollision(play, &play->colCtx.dyna, this->dyna.bgId);
    this->actionFunc = WheelBlock_WaitToDie;
    return;
}

void WheelBlock_Shake(WheelBlock* this, PlayState* play){
     s32 quakeIndex;
    //  if(this->timer == 30 ){
    //  OnePointCutscene_AttentionSetSfx(play, &this->dyna.actor, NA_SE_SY_CORRECT_CHIME);
    //  }
    if(this->dyna.actor.targetArrowOffset > 0.0f && this->dyna.actor.targetArrowOffset <= 400.0f){
        if(this->dyna.actor.targetArrowOffset <= 300.0f){
            if((play->gameplayFrames & 95) == 0){
                SfxSource_PlaySfxAtFixedWorldPos(play, &this->dyna.actor.world.pos, 16, NA_SE_EV_BLOCK_SHAKE);
            }
        }else{
            if((play->gameplayFrames & 8) == 0){
                SfxSource_PlaySfxAtFixedWorldPos(play, &this->dyna.actor.world.pos, 16, NA_SE_EV_BLOCK_SHAKE);
            }
        }

        this->randFactor.x += 10 * (s32)this->dyna.actor.targetArrowOffset;
        this->dyna.actor.world.rot.x = (s16)(Math_SinS(this->randFactor.x) * 300.0f) + this->dyna.actor.home.rot.x;
        this->dyna.actor.world.rot.z = (s16)(Math_CosS(this->randFactor.x) * 300.0f) + this->dyna.actor.home.rot.z;
        this->dyna.actor.shape.rot.x = this->dyna.actor.world.rot.x;
        this->dyna.actor.shape.rot.z = this->dyna.actor.world.rot.z;
        this->randFactor.y += (50 * (s32)this->dyna.actor.targetArrowOffset) ;
        this->dyna.actor.world.pos.y = Math_SinS(this->randFactor.y) + this->dyna.actor.home.pos.y;
        this->randFactor.z += (50 * (s32)this->dyna.actor.targetArrowOffset) ;
        this->dyna.actor.world.pos.x = Math_SinS(this->randFactor.z) * 2.0f + this->dyna.actor.home.pos.x;
        this->dyna.actor.world.pos.z = Math_CosS(this->randFactor.z) * 2.0f + this->dyna.actor.home.pos.z;
        if(this->dyna.actor.targetArrowOffset >= 390.0f){
            this->timer = 60;
            OnePointCutscene_AttentionSetSfx(play, &this->dyna.actor, NA_SE_SY_CORRECT_CHIME);
            this->actionFunc = WheelBlock_Break;
            return;
        }
    }

}



void WheelBlock_Destroy(WheelBlock* this, PlayState* play) {
    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);

}



void WheelBlock_Update(WheelBlock* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Debug_Text(255,255,0,4,3,"targetArrowOffset = %f.1",this->dyna.actor.targetArrowOffset);
    this->fragmentPos.x = 73.0f;
    this->fragmentPos.y = 30.0f * Math_CosS(this->dyna.actor.shape.rot.x);
    this->fragmentPos.z = 30.0f * Math_SinS(this->dyna.actor.shape.rot.x);

    DECR(this->dyna.actor.targetArrowOffset);
    this->actionFunc(this, play);
    this->dyna.actor.world.rot.x = this->dyna.actor.shape.rot.x ;


}

s32 WheelBlock_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3f* pos, Vec3s* rot, void* thisx) {
    WheelBlock* this = (void*)thisx;
    
    return 0;
}

void WheelBlock_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3s* rot, void* thisx) {
    WheelBlock* this = (void*)thisx;
}

void WheelBlock_Draw(WheelBlock* this, PlayState* play) {
    Gfx_DrawDListOpa(play, gWheelBlockj_DlWheelBlock);
}
