
#include "SpikeWheel.h"
#include "object/0x0196-SpikeWheelObj.h"

void SpikeWheel_Init(Actor* thisx, PlayState* play);
void SpikeWheel_Destroy(SpikeWheel* this, PlayState* play);
void SpikeWheel_Update(SpikeWheel* this, PlayState* play);
void SpikeWheel_Draw(SpikeWheel* this, PlayState* play);

const ActorInit sSpikeWheel_InitVars = {
    .id = 0x0200,
    .category     = ACTORCAT_BG,
    .flags        = 0x00000030,
    .objectId = 0x0196,
    .instanceSize = sizeof(SpikeWheel),
    .init         = (ActorFunc)SpikeWheel_Init,
    .destroy      = (ActorFunc)SpikeWheel_Destroy,
    .update       = (ActorFunc)SpikeWheel_Update,
    .draw         = (ActorFunc)SpikeWheel_Draw
};

void SpikeWheel_Init(Actor* thisx, PlayState* play) {
    SpikeWheel* this = (SpikeWheel*)thisx;
    CollisionHeader* colHeader = NULL;
    DynaPolyActor_Init(&this->dyna, DPM_PLAYER);
    CollisionHeader_GetVirtual(&gSpikeWheelObj_CollSpikeWheelObj, &colHeader);
     this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, thisx, colHeader);
     Actor_SetScale(&this->dyna.actor, 0.03f);


}

void SpikeWheel_Destroy(SpikeWheel* this, PlayState* play) {
        DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);

}

void SpikeWheel_Update(SpikeWheel* this, PlayState* play) {
    this->dyna.actor.shape.rot.x += 0x120;
      this->dyna.actor.world.rot.x = this->dyna.actor.shape.rot.x;
      func_8002F974(&this->dyna.actor, NA_SE_EV_TOGE_STICK_ROLLING - SFX_FLAG);

}

s32 SpikeWheel_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3f* pos, Vec3s* rot, void* thisx) {
    SpikeWheel* this = (void*)thisx;
    
    return 0;
}

void SpikeWheel_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3s* rot, void* thisx) {
    SpikeWheel* this = (void*)thisx;
}

void SpikeWheel_Draw(SpikeWheel* this, PlayState* play) {
       Gfx_SetupDL_25Opa(play->state.gfxCtx);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, gSpikeWheelObj_DlSpikeWheelObj);

}
