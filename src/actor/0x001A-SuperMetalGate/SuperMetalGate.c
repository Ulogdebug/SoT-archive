#include "SuperMetalGate.h"
#include "object/0x0010-SuperMetalGateObj.h"
#include "state/state.h"

void SuperMetalGate_Init(SuperMetalGate* this, PlayState* play);
void SuperMetalGate_Destroy(SuperMetalGate* this, PlayState* play);
void SuperMetalGate_Update(SuperMetalGate* this, PlayState* play);
void SuperMetalGate_Draw(SuperMetalGate* this, PlayState* play);
void SuperMetalGate_ResetWorldPos(SuperMetalGate* this);
void SuperMetalGate_OpenSetup(SuperMetalGate* this, PlayState* play);
void SuperMetalGate_Open(SuperMetalGate* this, PlayState* play);
void SuperMetalGate_StopWhenOpen(SuperMetalGate* this, PlayState* play);
void SuperMetalGate_CloseSetup(SuperMetalGate* this, PlayState* play);
void SuperMetalGate_StopWhenClose(SuperMetalGate* this, PlayState* play);
void SuperMetalGate_Close(SuperMetalGate* this, PlayState* play);

#define FLAGS (ACTOR_FLAG_4 | ACTOR_FLAG_5)

const ActorInit sSuperMetalGate_InitVars = {
    .id = 0x001A,
    .category     = ACTORCAT_PROP,
    .flags        = FLAGS,
    .objectId = 0x0010,
    .instanceSize = sizeof(SuperMetalGate),
    .init         = (ActorFunc)SuperMetalGate_Init,
    .destroy      = (ActorFunc)SuperMetalGate_Destroy,
    .update       = (ActorFunc)SuperMetalGate_Update,
    .draw         = (ActorFunc)SuperMetalGate_Draw
};

static f32 DistanceOffsets[] = {240.0f,163.0f,163.0f,120.0f,52.0f };

static CollisionHeader* sMetalFencesCollisions[] = {
    &gSuperMetalGateObj_CollSuperMetalGateHugeObj,
    &gSuperMetalGateObj_CollSuperMetalGateBigObj,
    &gSuperMetalGateObj_CollSuperMetalGateBigObj,
    &gSuperMetalGateObj_CollSuperMetalGateMediumObj,
    &gSuperMetalGateObj_CollSuperMetalGateSmallObj,
};

static s16 RotationOffsets[] = {
    0x4000,
    0xC000,
    0xC000,
    0xC000,
    0xC000
};

static Gfx* sMetalFencesDLs[] = {
    gSuperMetalGateObj_DlSuperMetalGateHugeObj,
    gSuperMetalGateObj_DlSuperMetalGateBigObj,
    gSuperMetalGateObj_DlSuperMetalGateBigRustyObj,
    gSuperMetalGateObj_DlSuperMetalGateMediumObj,
    gSuperMetalGateObj_DlSuperMetalGateSmallObj,
    
};

void SuperMetalGate_SetupAction(SuperMetalGate* this, SuperMetalGateFunc actionFunc) {
    this->actionFunc = actionFunc;
}

void SuperMetalGate_Init(SuperMetalGate* this, PlayState* play) {
    s32 pad;
    CollisionHeader* colHeader = NULL;

    DynaPolyActor_Init(&this->dyna, DPM_UNK);
    /*These two lines of codes give mesh-based collision to this actor*/
    CollisionHeader_GetVirtual(sMetalFencesCollisions[this->actor.params & 0xFF], &colHeader); 
    this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &this->actor, colHeader);

    this->actor.world.rot.y = RotationOffsets[this->dyna.actor.params & 0xFF] + this->actor.shape.rot.y;
    if((!CustomFlag_Get(HAS_SOLD) && play->sceneId == 0xC) || (play->sceneId == 0x1 && (play->roomCtx.curRoom.num == 0xC) && (!CustomFlag_Get(GERUDO_CAPTURE)))){
        Actor_Kill(&this->actor);
    }
    if (Flags_GetSwitch(play, (this->actor.params >> 8) & 0xFF)) {
        SuperMetalGate_ResetWorldPos(this);
        SuperMetalGate_SetupAction(this, SuperMetalGate_CloseSetup);
    }else {
        SuperMetalGate_SetupAction(this, SuperMetalGate_OpenSetup);
    }

}

void SuperMetalGate_Destroy(SuperMetalGate* this, PlayState* play) {
    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

void SuperMetalGate_ResetWorldPos(SuperMetalGate* this) {
    Vec3s* rot = &this->dyna.actor.world.rot;
    f32 temp1 = DistanceOffsets[this->dyna.actor.params & 0xFF] * Math_SinS(rot->y);
    f32 temp2 = DistanceOffsets[this->dyna.actor.params & 0xFF] * Math_CosS(rot->y);
    this->dyna.actor.world.pos.x = this->dyna.actor.home.pos.x + temp1;
    this->dyna.actor.world.pos.z = this->dyna.actor.home.pos.z + temp2;
}

void SuperMetalGate_OpenSetup(SuperMetalGate* this, PlayState* play) {
    if (Flags_GetSwitch(play, (this->actor.params >> 8) & 0xFF)) {
        SuperMetalGate_SetupAction(this, SuperMetalGate_Open);
    }
}

void SuperMetalGate_CloseSetup(SuperMetalGate* this, PlayState* play) {
    if (!Flags_GetSwitch(play, (this->actor.params >> 8) & 0xFF)) {
        SuperMetalGate_SetupAction(this, SuperMetalGate_Close);
    }
}

void SuperMetalGate_Close(SuperMetalGate* this, PlayState* play) {
    this->dyna.actor.speedXZ -= 1.0f;
    if (this->dyna.actor.speedXZ <- 3.0f) {
        this->dyna.actor.speedXZ = -3.0f;
        SuperMetalGate_SetupAction(this, SuperMetalGate_StopWhenClose);
    }
    Actor_MoveForward(&this->dyna.actor);
    func_8002F974(&this->dyna.actor, NA_SE_EV_METALDOOR_SLIDE - SFX_FLAG);
}

void SuperMetalGate_StopWhenClose(SuperMetalGate* this, PlayState* play) {
    func_8002D7EC(&this->dyna.actor);
    SuperMetalGate_OpenSetup(this,play);
    if (Math_Vec3f_DistXYZ(&this->dyna.actor.home.pos, &this->dyna.actor.world.pos) < 5.0f) {
        this->dyna.actor.world.pos.x = this->dyna.actor.home.pos.x ;
        this->dyna.actor.world.pos.z = this->dyna.actor.home.pos.z ;
        Audio_PlayActorSfx2(&this->dyna.actor, NA_SE_EV_METALDOOR_STOP);
        SuperMetalGate_SetupAction(this, SuperMetalGate_OpenSetup);
        return;
    }else {
        func_8002F974(&this->dyna.actor, NA_SE_EV_METALDOOR_SLIDE - SFX_FLAG);//opening SFX
    }
}

void SuperMetalGate_Open(SuperMetalGate* this, PlayState* play) {
    this->dyna.actor.speedXZ += 1.0f;
    if (this->dyna.actor.speedXZ > 3.0f) {
        this->dyna.actor.speedXZ = 3.0f;
        SuperMetalGate_SetupAction(this, SuperMetalGate_StopWhenOpen);
    }
    Actor_MoveForward(&this->dyna.actor);
    func_8002F974(&this->dyna.actor, NA_SE_EV_METALDOOR_SLIDE - SFX_FLAG);
}

void SuperMetalGate_StopWhenOpen(SuperMetalGate* this, PlayState* play) {
    func_8002D7EC(&this->dyna.actor);//Actor_UpdateWorld
    SuperMetalGate_CloseSetup(this,play);
    if (DistanceOffsets[this->dyna.actor.params & 0xFF] < Math_Vec3f_DistXYZ(&this->dyna.actor.home.pos, &this->dyna.actor.world.pos)) { 
        SuperMetalGate_ResetWorldPos(this);
        Audio_PlayActorSfx2(&this->dyna.actor, NA_SE_EV_METALDOOR_STOP);
        SuperMetalGate_SetupAction(this, SuperMetalGate_CloseSetup);
        return;
    } else {
        func_8002F974(&this->dyna.actor, NA_SE_EV_METALDOOR_SLIDE - SFX_FLAG);//opening SFX
    }
}



void SuperMetalGate_Update(SuperMetalGate* this, PlayState* play) {
    this->actionFunc(this, play);
}

s32 SuperMetalGate_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3f* pos, Vec3s* rot, void* thisx) {
    SuperMetalGate* this = (void*)thisx;
    
    return 0;
}

void SuperMetalGate_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3s* rot, void* thisx) {
    SuperMetalGate* this = (void*)thisx;
}

void SuperMetalGate_Draw(SuperMetalGate* this, PlayState* play) {
    OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
    Gfx_SetupDL_25Opa(play->state.gfxCtx);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, sMetalFencesDLs[this->actor.params & 0xFF]);

    CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
}
