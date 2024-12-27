#include <uLib.h>

// z64ram = 0x8008F470
// z64rom = 0xB06610
// z64next = 0x8008F87C

void Player_DrawImpl(PlayState* play, void** skeleton, Vec3s* jointTable, s32 dListCount, s32 lod, s32 tunic, s32 boots,
                     s32 face, OverrideLimbDrawOpa overrideLimbDraw, PostLimbDrawOpa postLimbDraw, void* data) {
    Player_DrawNew(play, skeleton, jointTable, dListCount, lod, tunic, boots,
                     face, overrideLimbDraw, postLimbDraw, data);
}