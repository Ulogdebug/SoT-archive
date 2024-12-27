#include <z64hdr.h>
#include "code/z_player_lib.h"
#include "functions.h"
#include "playas/playas.h"

Asm_VanillaHook(Player_HasMirrorShieldSetToDraw);
s32 Player_HasMirrorShieldSetToDraw(PlayState* play) {
    Player* this = GET_PLAYER(play);

    return (this->currentShield == PLAYER_SHIELD_MIRROR);
}