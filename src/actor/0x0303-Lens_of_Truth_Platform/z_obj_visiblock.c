/*
 * File: z_obj_visiblock.c
 * Overlay: ovl_Obj_Visiblock
 * Description: Lens of Truth Platform
 */

#include "z_obj_visiblock.h"

#define FLAGS (ACTOR_FLAG_7)

#define THIS ((ObjVisiblock*)thisx)

void ObjVisiblock_Init(Actor* thisx, PlayState* play);
void ObjVisiblock_Destroy(Actor* thisx, PlayState* play);
void ObjVisiblock_Draw(Actor* thisx, PlayState* play);

ActorInit Obj_Visiblock_InitVars = {
    /**/ 0x0303,
    /**/ ACTORCAT_BG,
    /**/ FLAGS,
    /**/ 0x0223,
    /**/ sizeof(ObjVisiblock),
    /**/ ObjVisiblock_Init,
    /**/ ObjVisiblock_Destroy,
    /**/ Actor_Noop,
    /**/ ObjVisiblock_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneForward, 4000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 200, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 150, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void ObjVisiblock_InitDynaPoly(ObjVisiblock* this, PlayState* play, CollisionHeader* collision, s32 flags) {
    s32 pad;
    CollisionHeader* colHeader = NULL;
    s32 pad2;

    DynaPolyActor_Init(&this->dyna, flags);
    CollisionHeader_GetVirtual(collision, &colHeader);
    this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &this->dyna.actor, colHeader);
}

void ObjVisiblock_Init(Actor* thisx, PlayState* play) {
    ObjVisiblock* this = THIS;

    Actor_ProcessInitChain(&this->dyna.actor, sInitChain);
    DynaPolyActor_Init(&this->dyna, 0);
    ObjVisiblock_InitDynaPoly(this, play, 0x06000ad0, DPM_PLAYER);
}

void ObjVisiblock_Destroy(Actor* thisx, PlayState* play) {
    ObjVisiblock* this = THIS;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

void ObjVisiblock_Draw(Actor* thisx, PlayState* play) {
    Gfx_DrawDListXlu(play, 0x06000140);
}
