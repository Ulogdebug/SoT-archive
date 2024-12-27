#include "uLib.h"
#include "z64player.h"
#include <z64hdr.h>
#include <playas/playas.h>
#include "code/z_player_lib.h"

void DrawGauntletDList(PlayState* play, Player* this, s32 limbIndex) {

    Gfx* hackAdultGauntletsDLists[] = {
        gPlayasAdult_DlGauntletForearmL,//gPlayasAdult_DlBracelet, 			// PLAYER_LIMB_L_HAND
        gPlayasAdult_DlGauntletForearmL,	// PLAYER_LIMB_L_FOREARM
        gPlayasAdult_DlGauntletForearmR,	// PLAYER_LIMB_R_FOREARM
        gPlayasAdult_DlGauntletHandL,		// PLAYER_LIMB_L_HAND
        gPlayasAdult_DlGauntletHandR,		// PLAYER_LIMB_R_HAND
        gPlayasAdult_DlGauntletFistL,		// PLAYER_LIMB_L_HAND
        gPlayasAdult_DlGauntletFistR,		// PLAYER_LIMB_R_HAND
    };

    Gfx** gauntletDListsToUse = hackAdultGauntletsDLists;
	s32 strengthUpgrade = CUR_UPG_VALUE(UPG_STRENGTH);
	Color_RGB8* tuniccolor = &sTunicColors[this->currentTunic];

	if (strengthUpgrade > 1 && ((limbIndex == PLAYER_LIMB_L_HAND) || (limbIndex == PLAYER_LIMB_R_HAND) || 
         (limbIndex == PLAYER_LIMB_L_FOREARM)|| (limbIndex == PLAYER_LIMB_R_FOREARM))) {

		OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

		Color_RGB8* color = &sGauntletColors[strengthUpgrade - 2];
		gDPSetEnvColor(POLY_OPA_DISP++, color->r, color->g, color->b, 0);

		if (limbIndex == PLAYER_LIMB_L_HAND && strengthUpgrade != 1){
			gSPDisplayList(POLY_OPA_DISP++, (sLeftHandType == PLAYER_MODELTYPE_LH_OPEN)
		                                        ? gauntletDListsToUse[3]
		                                        : gauntletDListsToUse[5]);
		}

		if (limbIndex == PLAYER_LIMB_R_HAND && strengthUpgrade != 1){
			gSPDisplayList(POLY_OPA_DISP++, (sRightHandType == PLAYER_MODELTYPE_RH_OPEN)
			                                        ? gauntletDListsToUse[4]
			                                        : gauntletDListsToUse[6]);
		}

	    if (limbIndex == PLAYER_LIMB_L_FOREARM){
		
		// Goron Bracelet
			if (strengthUpgrade == 1){
				gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__),
	        		G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
				gSPDisplayList(POLY_OPA_DISP++, gauntletDListsToUse[0]);
			}else{
				gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__),
	        		G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
				gSPDisplayList(POLY_OPA_DISP++, gauntletDListsToUse[1]);
			}
		}

		// Gauntlet Forearms
		if (limbIndex == PLAYER_LIMB_R_FOREARM && strengthUpgrade != 1){
			gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__),
	        	G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
			gSPDisplayList(POLY_OPA_DISP++, gauntletDListsToUse[2]);
		}

		gDPSetEnvColor(POLY_OPA_DISP++, tuniccolor->r, tuniccolor->g, tuniccolor->b, 0);
		CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
	}	
}