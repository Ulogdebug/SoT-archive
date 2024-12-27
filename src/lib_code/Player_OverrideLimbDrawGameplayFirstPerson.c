#include <uLib.h>
#include "code/z_player_lib.h"

// z64ram = 0x800902F0
// z64rom = 0xB07490
// z64next = 0x80090440

s32 Player_OverrideLimbDrawGameplayFirstPerson(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                                           void* thisx) {
    Player_OverrideLimbDrawGameplayFirstPersonNew(play,limbIndex,dList,pos,rot,thisx);
}