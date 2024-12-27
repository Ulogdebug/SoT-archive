#include <uLib.h>
#include "code/z_scene_table.h"

Asm_VanillaHook(Scene_SetTransitionForNextEntrance);
void Scene_SetTransitionForNextEntrance(PlayState* play) {
    s16 entranceIndex;
    entranceIndex = play->nextEntranceIndex;
    play->transitionType = ENTRANCE_INFO_START_TRANS_TYPE(gEntranceTable[entranceIndex].field);
}