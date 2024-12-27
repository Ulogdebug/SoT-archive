#include "uLib.h"
#include "code/z_player_lib.h"
#include "playas/playas.h"
#include "../system/kaleido/0x01-Player/playas_adult.h"
#include "../system/kaleido/0x01-Player/playas_child.h"

s32 Player_OverrideLimbDrawGameplayFirstPersonNew(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                                               void* thisx) {
    Player* this = (Player*)thisx;

    if (!Player_OverrideLimbDrawGameplayCommon(play, limbIndex, dList, pos, rot, thisx)) {
        if (this->unk_6AD != 2) {
            *dList = NULL;

        } else if (limbIndex == PLAYER_LIMB_L_FOREARM) {
            *dList = sFirstPersonLeftForearmDLs[(void)0, gSaveContext.linkAge];
        
        } else if (limbIndex == PLAYER_LIMB_L_HAND) {
            *dList = sFirstPersonLeftHandDLs[(void)0, gSaveContext.linkAge];

        //All Ages/ equips for flashing piece of tunic when pausing
        } else if (limbIndex == PLAYER_LIMB_R_SHOULDER) {
            *dList = sFirstPersonRightShoulderDLs[(void)0, gSaveContext.linkAge];
        
        //Bow Right Forearm Adult
        } else if (limbIndex == PLAYER_LIMB_R_FOREARM) {
            *dList = sFirstPersonForearmDLs[(void)0, gSaveContext.linkAge];
        
        //bow/slingshot/hookshot
        } else if (limbIndex == PLAYER_LIMB_R_HAND) {
            
            if (Player_HoldsHookshot(this)){
                    *dList = gPlayAsLUT_Adult_HooshkotFpsHandR; 
            }else if (this->itemActionParam == PLAYER_AP_BOOMERANG){
                if (LINK_IS_ADULT){
                    *dList = gPlayAsLUT_Adult_BoomerangFistL;
                }else{
                }
            }else if(this->itemActionParam == PLAYER_AP_BOW) {
                 *dList = gPlayAsLUT_Adult_BowFpsHandR;
                //Debug_Text(255,255,255,10,20,"Bow bruh");

            }else{
                *dList = sFirstPersonRightHandHoldingWeaponDLs[(void)0, gSaveContext.linkAge];
            }

        } else {
            *dList = NULL;
        }
    }

    return false;
}