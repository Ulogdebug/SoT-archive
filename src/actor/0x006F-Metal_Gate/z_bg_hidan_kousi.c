/*
 * File: z_bg_hidan_kousi.c
 * Overlay: ovl_Bg_Hidan_Kousi
 * Description: Switchable Gate
 */
#include <uLib.h>
#include "z_bg_hidan_kousi.h"
#include "assets/objects/object_hidan_objects/object_hidan_objects.h"

#define FLAGS (ACTOR_FLAG_4 | ACTOR_FLAG_5)

void BgHidanKousi_Init(Actor* thisx, PlayState* play);
void BgHidanKousi_Destroy(Actor* thisx, PlayState* play);
void BgHidanKousi_Update(Actor* thisx, PlayState* play);
void BgHidanKousi_Draw(Actor* thisx, PlayState* play);

void BgHidanKousi_ResetWorldPos(BgHidanKousi* this);
void BgHidanKousi_OpenSetup(BgHidanKousi* this, PlayState* play);
void BgHidanKousi_Open(BgHidanKousi* this, PlayState* play);
void BgHidanKousi_StopWhenOpen(BgHidanKousi* this, PlayState* play);
void BgHidanKousi_CloseSetup(BgHidanKousi* this, PlayState* play);
void BgHidanKousi_StopWhenClose(BgHidanKousi* this, PlayState* play);
void BgHidanKousi_Close(BgHidanKousi* this, PlayState* play);

static f32 DistanceOffsets[] = { 120.0f, 150.0f, 150.0f };

const ActorInit Bg_Hidan_Kousi_InitVars = {
    ACTOR_BG_HIDAN_KOUSI,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_HIDAN_OBJECTS,
    sizeof(BgHidanKousi),
    (ActorFunc)BgHidanKousi_Init,
    (ActorFunc)BgHidanKousi_Destroy,
    (ActorFunc)BgHidanKousi_Update,
    (ActorFunc)BgHidanKousi_Draw,
};

static InitChainEntry sInitChain[] = {//Set scale
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

static CollisionHeader* sMetalFencesCollisions[] = {
    &gFireTempleMetalFenceWithSlantCol,
    &gFireTempleMetalFenceCol,
    &gFireTempleMetalFence2Col,
};

static s16 RotationOffsets[] = {
    0x4000,
    0xC000,
    0xC000,
    0x0000,
};

static Gfx* sMetalFencesDLs[] = {
    gFireTempleMetalFenceWithSlantDL,
    gFireTempleMetalFenceDL,
    gFireTempleMetalFence2DL,
};

void BgHidanKousi_SetupAction(BgHidanKousi* this, BgHidanKousiActionFunc actionFunc) {
    this->actionFunc = actionFunc;
}

void BgHidanKousi_Init(Actor* thisx, PlayState* play) {
    BgHidanKousi* this = (BgHidanKousi*)thisx;
    s32 pad;
    CollisionHeader* colHeader = NULL;

    DynaPolyActor_Init(&this->dyna, DPM_UNK);
    Actor_SetFocus(thisx, 50.0f);
    Actor_ProcessInitChain(thisx, sInitChain);

    /*These two lines of codes give mesh-based collision to this actor*/
    CollisionHeader_GetVirtual(sMetalFencesCollisions[thisx->params & 0xFF], &colHeader); 
    this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, thisx, colHeader);

    thisx->world.rot.y = RotationOffsets[this->dyna.actor.params & 0xFF] + thisx->shape.rot.y;
    if (Flags_GetSwitch(play, (thisx->params >> 8) & 0xFF)) {
        BgHidanKousi_ResetWorldPos(this);
        BgHidanKousi_SetupAction(this, BgHidanKousi_CloseSetup);
    }else {
        BgHidanKousi_SetupAction(this, BgHidanKousi_OpenSetup);
    }
}

void BgHidanKousi_Destroy(Actor* thisx, PlayState* play) {
    BgHidanKousi* this = (BgHidanKousi*)thisx;
    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

void BgHidanKousi_ResetWorldPos(BgHidanKousi* this) {
    Vec3s* rot = &this->dyna.actor.world.rot;
    f32 temp1 = DistanceOffsets[this->dyna.actor.params & 0xFF] * Math_SinS(rot->y);
    f32 temp2 = DistanceOffsets[this->dyna.actor.params & 0xFF] * Math_CosS(rot->y);
    this->dyna.actor.world.pos.x = this->dyna.actor.home.pos.x + temp1;
    this->dyna.actor.world.pos.z = this->dyna.actor.home.pos.z + temp2;
}

void BgHidanKousi_OpenSetup(BgHidanKousi* this, PlayState* play) {
    if (Flags_GetSwitch(play, (this->dyna.actor.params >> 8) & 0xFF)) {
        BgHidanKousi_SetupAction(this, BgHidanKousi_Open);
    }
}

void BgHidanKousi_CloseSetup(BgHidanKousi* this, PlayState* play) {
    if (!Flags_GetSwitch(play, (this->dyna.actor.params >> 8) & 0xFF)) {
        BgHidanKousi_SetupAction(this, BgHidanKousi_Close);
    }
}

void BgHidanKousi_Close(BgHidanKousi* this, PlayState* play) {
    this->dyna.actor.speedXZ -= 1.0f;
    if (this->dyna.actor.speedXZ <- 3.0f) {
        this->dyna.actor.speedXZ = -3.0f;
        BgHidanKousi_SetupAction(this, BgHidanKousi_StopWhenClose);
    }
    Actor_MoveForward(&this->dyna.actor);
    func_8002F974(&this->dyna.actor, NA_SE_EV_METALDOOR_SLIDE - SFX_FLAG);
}

void BgHidanKousi_StopWhenClose(BgHidanKousi* this, PlayState* play) {
    func_8002D7EC(&this->dyna.actor);
    BgHidanKousi_OpenSetup(this,play);
    if (Math_Vec3f_DistXYZ(&this->dyna.actor.home.pos, &this->dyna.actor.world.pos) < 10.0f) {
        this->dyna.actor.world.pos.x = this->dyna.actor.home.pos.x ;
        this->dyna.actor.world.pos.z = this->dyna.actor.home.pos.z ;
        Audio_PlayActorSfx2(&this->dyna.actor, NA_SE_EV_METALDOOR_STOP);
        BgHidanKousi_SetupAction(this, BgHidanKousi_OpenSetup);
        return;
    }else {
        func_8002F974(&this->dyna.actor, NA_SE_EV_METALDOOR_SLIDE - SFX_FLAG);//opening SFX
    }
}

void BgHidanKousi_Open(BgHidanKousi* this, PlayState* play) {
    this->dyna.actor.speedXZ += 1.0f;
    if (this->dyna.actor.speedXZ > 3.0f) {
        this->dyna.actor.speedXZ = 3.0f;
        BgHidanKousi_SetupAction(this, BgHidanKousi_StopWhenOpen);
    }
    Actor_MoveForward(&this->dyna.actor);
    func_8002F974(&this->dyna.actor, NA_SE_EV_METALDOOR_SLIDE - SFX_FLAG);
}

void BgHidanKousi_StopWhenOpen(BgHidanKousi* this, PlayState* play) {
    func_8002D7EC(&this->dyna.actor);//Actor_UpdateWorld
    BgHidanKousi_CloseSetup(this,play);
    if (DistanceOffsets[this->dyna.actor.params & 0xFF] < Math_Vec3f_DistXYZ(&this->dyna.actor.home.pos, &this->dyna.actor.world.pos)) { 
        BgHidanKousi_ResetWorldPos(this);
        Audio_PlayActorSfx2(&this->dyna.actor, NA_SE_EV_METALDOOR_STOP);
        BgHidanKousi_SetupAction(this, BgHidanKousi_CloseSetup);
        return;
    } else {
        func_8002F974(&this->dyna.actor, NA_SE_EV_METALDOOR_SLIDE - SFX_FLAG);//opening SFX
    }
}

void BgHidanKousi_Update(Actor* thisx, PlayState* play) {
    BgHidanKousi* this = (BgHidanKousi*)thisx;
    this->actionFunc(this, play);
}

void BgHidanKousi_Draw(Actor* thisx, PlayState* play) {
    OPEN_DISPS(play->state.gfxCtx, "../z_bg_hidan_kousi.c", 350);

    Gfx_SetupDL_25Opa(play->state.gfxCtx);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx, "../z_bg_hidan_kousi.c", 354),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, sMetalFencesDLs[thisx->params & 0xFF]);

    CLOSE_DISPS(play->state.gfxCtx, "../z_bg_hidan_kousi.c", 359);
}
