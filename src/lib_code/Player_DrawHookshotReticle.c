#include <uLib.h>
#include "code/z_player_lib.h"
#include "playas/playas.h"

// z64ram = 0x80090AFC
// z64rom = 0xB07C9C
// z64next = 0x80090D20

void Player_DrawHookshotReticle(PlayState* play, Player* this, f32 arg2) {
    Player_DrawHookshotReticleNew(play,this,arg2);
};