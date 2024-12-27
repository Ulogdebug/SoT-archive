#include <z64hdr.h>
#include "code/z_player_lib.h"
#include "functions.h"
#include "playas/playas.h"

#define COLOR_TUNIC_KOKIRI {109,165,0}
#define COLOR_TUNIC_GORON {224,47,28}
#define COLOR_TUNIC_ZORA {37,103,237}

void Player_DrawNew(PlayState* play, void** skeleton, Vec3s* jointTable, s32 dListCount, s32 lod, s32 tunic, s32 boots,
                     s32 face, OverrideLimbDrawOpa overrideLimbDraw, PostLimbDrawOpa postLimbDraw, void* data) {
    Color_RGB8* color;
    s32 eyeIndex = (jointTable[22].x & 0xF) - 1;
    s32 mouthIndex = (jointTable[22].x >> 4) - 1;

    Color_RGB8 sHackTunicColors[PLAYER_TUNIC_MAX] = {
        COLOR_TUNIC_KOKIRI, // PLAYER_TUNIC_KOKIRI
        COLOR_TUNIC_GORON,  // PLAYER_TUNIC_GORON
        COLOR_TUNIC_ZORA,  // PLAYER_TUNIC_ZORA
    };

        //Adult
    Gfx* hackAdultBootDLists[] = {
         gPlayasAdult_DlFoot2L,
        gPlayasAdult_DlFoot3L, 
        gPlayasAdult_DlFoot2R,
        gPlayasAdult_DlFoot3R,
    };

    OPEN_DISPS(play->state.gfxCtx, "../z_player_lib.c", 1721);

    if (eyeIndex < 0) {
        eyeIndex = sEyeMouthIndices[face][0];
    }

#ifndef AVOID_UB
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sEyeTextures[eyeIndex]));
#else
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sEyeTextures[gSaveContext.linkAge][eyeIndex]));
#endif

    if (mouthIndex < 0) {
        mouthIndex = sEyeMouthIndices[face][1];
    }

#ifndef AVOID_UB
    gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(sMouthTextures[mouthIndex]));
#else
    gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(sMouthTextures[gSaveContext.linkAge][mouthIndex]));
#endif
    color = &sHackTunicColors[tunic];
    gDPSetEnvColor(POLY_OPA_DISP++, color->r, color->g, color->b, 0);

    sDListsLodOffset = lod * 2;
    SkelAnime_DrawFlexLod(play, skeleton, jointTable, dListCount, overrideLimbDraw, postLimbDraw, data, lod);

    if ((overrideLimbDraw != Player_OverrideLimbDrawGameplayFirstPerson) &&
        (overrideLimbDraw != Player_OverrideLimbDrawGameplay_80090440) &&
        (gSaveContext.gameMode != GAMEMODE_END_CREDITS)) {

        if (boots != PLAYER_BOOTS_KOKIRI) {
            Gfx** bootDListsToUse = hackAdultBootDLists;
            gSPDisplayList(POLY_OPA_DISP++, bootDListsToUse[boots - 1]);
            gSPDisplayList(POLY_OPA_DISP++, bootDListsToUse[boots + 1]);
        }
    }
    CLOSE_DISPS(play->state.gfxCtx, "../z_player_lib.c", 1803);
}