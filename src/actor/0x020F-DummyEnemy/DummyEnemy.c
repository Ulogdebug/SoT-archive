#include "DummyEnemy.h"

void DummyEnemy_Init(DummyEnemy* this, PlayState* play);
void DummyEnemy_Destroy(DummyEnemy* this, PlayState* play);
void DummyEnemy_Update(DummyEnemy* this, PlayState* play);
void DummyEnemy_Draw(DummyEnemy* this, PlayState* play);

const ActorInit sDummyEnemy_InitVars = {
    .id = 0x020F,
    .category     = ACTORCAT_BG,
    .flags        = 0x00000000,
    .objectId = OBJECT_GAMEPLAY_KEEP,
    .instanceSize = sizeof(DummyEnemy),
    .init         = (ActorFunc)DummyEnemy_Init,
    .destroy      = (ActorFunc)DummyEnemy_Destroy,
    .update       = (ActorFunc)DummyEnemy_Update,
    .draw         = (ActorFunc)DummyEnemy_Draw
};

void DummyEnemy_Init(DummyEnemy* this, PlayState* play) {
}

void DummyEnemy_Destroy(DummyEnemy* this, PlayState* play) {
}

void DummyEnemy_Update(DummyEnemy* this, PlayState* play) {
}

s32 DummyEnemy_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3f* pos, Vec3s* rot, void* thisx) {
    DummyEnemy* this = (void*)thisx;
    
    return 0;
}

void DummyEnemy_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3s* rot, void* thisx) {
    DummyEnemy* this = (void*)thisx;
}

void DummyEnemy_Draw(DummyEnemy* this, PlayState* play) {
}
