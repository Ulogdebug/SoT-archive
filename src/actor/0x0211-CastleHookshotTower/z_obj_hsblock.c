/*
 * File: z_obj_hsblock.c
 * Overlay: ovl_Obj_Hsblock
 * Description: Stone Hookshot Target in Castle
 */

#include "z_obj_hsblock.h"
#include "object/0x0207-CastleHookshotTowerObj.h"

#define FLAGS 0

void ObjHsblock_Init(Actor* thisx, PlayState* play);
void ObjHsblock_Destroy(Actor* thisx, PlayState* play);
void ObjHsblock_Update(Actor* thisx, PlayState* play);
void ObjHsblock_Draw(Actor* thisx, PlayState* play);

void ObjHsblock_CheckSwitch(ObjHsblock* this, PlayState* play);
void ObjHsblock_Rise(ObjHsblock* this, PlayState* play);

void ObjHsblock_Idle(ObjHsblock* this);
void ObjHsblock_Lurk(ObjHsblock* this);
void ObjHsblock_SetupRise(ObjHsblock* this);

const ActorInit Obj_Hsblock_InitVars = {
    0x0211, //Actor ID
    ACTORCAT_BG,
    FLAGS,
    0x0207,// Object ID
    sizeof(ObjHsblock),
    (ActorFunc)ObjHsblock_Init,
    (ActorFunc)ObjHsblock_Destroy,
    (ActorFunc)ObjHsblock_Update,
    (ActorFunc)ObjHsblock_Draw,
};

void ObjHsblock_SetupAction(ObjHsblock* this, ObjHsblockActionFunc actionFunc) {
    this->actionFunc = actionFunc;
}

void ObjHsblock_Freeze(ObjHsblock* this, PlayState* play) {
    if ((this->dyna.actor.params >> 5) & 1) {
        Actor_SpawnAsChild(&play->actorCtx, &this->dyna.actor, play, ACTOR_OBJ_ICE_POLY, this->dyna.actor.world.pos.x,
                           this->dyna.actor.world.pos.y, this->dyna.actor.world.pos.z, this->dyna.actor.world.rot.x,
                           this->dyna.actor.world.rot.y, this->dyna.actor.world.rot.z, 1);
    }
}

void ObjHsblock_Init(Actor* thisx, PlayState* play) {
    ObjHsblock* this = (ObjHsblock*)thisx;

    CollisionHeader* colHeader = NULL;
    DynaPolyActor_Init(&this->dyna, DPM_PLAYER);
    CollisionHeader_GetVirtual(&gCastleHookshotTowerOBj_CollCastleHookshotTowerObj, &colHeader);
    this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, thisx, colHeader);
    ObjHsblock_Freeze(this, play);
    if (Flags_GetSwitch(play, (this->dyna.actor.params >> 8) & 0x3F)||((this->dyna.actor.params >> 8) & 0x3F == 0x00)||((this->dyna.actor.params >> 8) & 0x3F == 0x3F)) {//Variable Form : **00 (hex)
        ObjHsblock_Idle(this);
    } else {
        ObjHsblock_Lurk(this);
    }

}

void ObjHsblock_Destroy(Actor* thisx, PlayState* play) {
    ObjHsblock* this = (ObjHsblock*)thisx;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

void ObjHsblock_Idle(ObjHsblock* this) {
    ObjHsblock_SetupAction(this, NULL);
}

void ObjHsblock_Lurk(ObjHsblock* this) {
    this->dyna.actor.flags |= ACTOR_FLAG_4;
    this->dyna.actor.world.pos.y = this->dyna.actor.home.pos.y - 145.0f;
    ObjHsblock_SetupAction(this, ObjHsblock_CheckSwitch);
}

void ObjHsblock_CheckSwitch(ObjHsblock* this, PlayState* play) {
    if (Flags_GetSwitch(play, (this->dyna.actor.params >> 8) & 0x3F)) {
        ObjHsblock_SetupRise(this);
    }
}

void ObjHsblock_SetupRise(ObjHsblock* this) {
    ObjHsblock_SetupAction(this, ObjHsblock_Rise);
}

void ObjHsblock_Rise(ObjHsblock* this, PlayState* play) {
    Math_SmoothStepToF(&this->dyna.actor.velocity.y, 16.0f, 0.1f, 0.8f, 0.0f);
    if (fabsf(Math_SmoothStepToF(&this->dyna.actor.world.pos.y, this->dyna.actor.home.pos.y, 0.3f,
                                 this->dyna.actor.velocity.y, 0.3f)) < 0.001f) {
        this->dyna.actor.world.pos.y = this->dyna.actor.home.pos.y;
        ObjHsblock_Idle(this);
        this->dyna.actor.flags &= ~ACTOR_FLAG_4;
    }
}

void ObjHsblock_Update(Actor* thisx, PlayState* play) {
    ObjHsblock* this = (ObjHsblock*)thisx;

    if (this->actionFunc != NULL) {
        this->actionFunc(this, play);
    }
}

void ObjHsblock_Draw(Actor* thisx, PlayState* play) {

    Gfx_DrawDListOpa(play, &gCastleHookshotTowerOBj_DlCastleHookshotTowerObj);

}
