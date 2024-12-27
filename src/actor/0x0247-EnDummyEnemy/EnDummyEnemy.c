#include "EnDummyEnemy.h"

void EnDummyEnemy_Init(EnDummyEnemy* this, PlayState* play);
void EnDummyEnemy_Destroy(EnDummyEnemy* this, PlayState* play);
void EnDummyEnemy_Update(EnDummyEnemy* this, PlayState* play);
void EnDummyEnemy_Draw(EnDummyEnemy* this, PlayState* play);

const ActorInit sEnDummyEnemy_InitVars = {
    .id           = DUMMYENEMY_ID,
    .category     = ACTORCAT_ENEMY,
    .flags        = 0x00000000,
    .objectId     = OBJECT_GAMEPLAY_KEEP,
    .instanceSize = sizeof(EnDummyEnemy),
    .init         = (ActorFunc)EnDummyEnemy_Init,
    .destroy      = (ActorFunc)EnDummyEnemy_Destroy,
    .update       = (ActorFunc)EnDummyEnemy_Update,
    .draw         = (ActorFunc)EnDummyEnemy_Draw
};

void EnDummyEnemy_Init(EnDummyEnemy* this, PlayState* play) {
}

void EnDummyEnemy_Destroy(EnDummyEnemy* this, PlayState* play) {
}

void EnDummyEnemy_Update(EnDummyEnemy* this, PlayState* play) {
}

void EnDummyEnemy_Draw(EnDummyEnemy* this, PlayState* play) {
}
