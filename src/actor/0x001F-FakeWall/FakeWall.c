#include "FakeWall.h"
#include <object/0x022B-FakeWallObj.h>
#define FLAGS (ACTOR_FLAG_4 | ACTOR_FLAG_5 | ACTOR_FLAG_7)

void FakeWall_Init(FakeWall* this, PlayState* play);
void FakeWall_Destroy(FakeWall* this, PlayState* play);
void FakeWall_Update(FakeWall* this, PlayState* play);
void FakeWall_Draw(FakeWall* this, PlayState* play);

static Gfx* displayDLs[] = {
    gFakeWallObj_DlFakeWallStoneObj,
    gFakeWallObj_DlFakeWallDemonObj,
    gFakeWallObj_DlLensOfTruthMark,
    gFakeWallObj_DlLensOfTruthMark01,
};

static CollisionHeader* wallCollision[] = {
    gFakeWallObj_CollFakeWallObj,
    gFakeWallObj_CollFakeWall01Obj,
};


const ActorInit sFakeWall_InitVars = {
    .id = 0x001F,
    .category     = ACTORCAT_BG,
    .flags        = FLAGS,
    .objectId = 0x022B,
    .instanceSize = sizeof(FakeWall),
    .init         = (ActorFunc)FakeWall_Init,
    .destroy      = (ActorFunc)FakeWall_Destroy,
    .update       = (ActorFunc)FakeWall_Update,
    .draw         = (ActorFunc)FakeWall_Draw
};

void FakeWall_Init(FakeWall* this, PlayState* play) {
    CollisionHeader* colHeader = NULL;
    DynaPolyActor_Init(&this->dyna, DPM_PLAYER);
    CollisionHeader_GetVirtual(wallCollision[this->actor.params & 0xF], &colHeader);
    this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &this->dyna.actor, colHeader);
}

void FakeWall_Destroy(FakeWall* this, PlayState* play) {
    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

void FakeWall_Update(FakeWall* this, PlayState* play) {
    Actor* thisx = &this->dyna.actor;
    if (play->actorCtx.lensActive) {
        thisx->flags |= ACTOR_FLAG_7;
        DynaPoly_DisableCollision(play, &play->colCtx.dyna, this->dyna.bgId);
    } else {
        thisx->flags &= ~ACTOR_FLAG_7;
        DynaPoly_EnableCollision(play, &play->colCtx.dyna, this->dyna.bgId);
    }
}

s32 FakeWall_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3f* pos, Vec3s* rot, void* thisx) {
    FakeWall* this = (void*)thisx;
    
    return 0;
}

void FakeWall_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3s* rot, void* thisx) {
    FakeWall* this = (void*)thisx;
}

void FakeWall_Draw(FakeWall* this, PlayState* play) {
    if (!CHECK_FLAG_ALL(this->actor.flags, ACTOR_FLAG_7)) {
        Gfx_DrawDListXlu(play, displayDLs[this->actor.params & 0xF]);
    }else{
        Gfx_DrawDListXlu(play, displayDLs[(this->actor.params & 0xF) + 2]);
    }
}
