#include <uLib.h>

// z64ram = 0x80090D20
// z64rom = 0xB07EC0
// z64next = 0x80091738

 void Player_PostLimbDrawGameplay(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx){
 	Player_PostLimbDrawGameplayNew(play,limbIndex,dList,rot,thisx);
 }
