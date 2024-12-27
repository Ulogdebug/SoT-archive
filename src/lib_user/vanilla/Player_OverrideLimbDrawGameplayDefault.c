#include <z64hdr.h>
#include "code/z_player_lib.h"
#include "playas/playas.h"
#include "state/state.h"
#include "uLib.h"

Asm_VanillaHook(Player_OverrideLimbDrawGameplayDefault);
s32 Player_OverrideLimbDrawGameplayDefault(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                                           void* thisx) {
    Player* this = (Player*)thisx;

    if (!Player_OverrideLimbDrawGameplayCommon(play, limbIndex, dList, pos, rot, thisx)) {
        if (limbIndex == PLAYER_LIMB_L_HAND) {
            Gfx** dLists = this->leftHandDLists;
            if(ghackSave.disArmed && Play_InCsMode(play)){
              dLists = gPlayerLeftHandOpenDLs + gSaveContext.linkAge;
              sLeftHandType = PLAYER_MODELTYPE_LH_OPEN;
            }else if ((sLeftHandType == PLAYER_MODELTYPE_LH_BGS) && (gSaveContext.swordHealth <= 0.0f)) {
                dLists += 4;
            } else if ((sLeftHandType == PLAYER_MODELTYPE_LH_BOOMERANG) && (this->stateFlags1 & PLAYER_STATE1_25)) {
                dLists = gPlayerLeftHandOpenDLs + gSaveContext.linkAge;
                sLeftHandType = PLAYER_MODELTYPE_LH_OPEN;
            } else if ((this->leftHandType == PLAYER_MODELTYPE_LH_OPEN) && (this->actor.speedXZ > 2.0f) &&
                       !(this->stateFlags1 & PLAYER_STATE1_27)) {
                dLists = gPlayerLeftHandClosedDLs + gSaveContext.linkAge;
                sLeftHandType = PLAYER_MODELTYPE_LH_CLOSED;
            }

            *dList = *(dLists + sDListsLodOffset);
        } else if (limbIndex == PLAYER_LIMB_R_HAND) {
            Gfx** dLists = this->rightHandDLists;
            if(ghackSave.disArmed && Play_InCsMode(play)){
              dLists = sPlayerRightHandOpenDLs + gSaveContext.linkAge;
              sRightHandType = PLAYER_MODELTYPE_RH_OPEN;
            }else if (sRightHandType == PLAYER_MODELTYPE_RH_SHIELD) {
                dLists += this->currentShield * 4;
            } else if ((this->rightHandType == PLAYER_MODELTYPE_RH_OPEN) && (this->actor.speedXZ > 2.0f) &&
                       !(this->stateFlags1 & PLAYER_STATE1_27)) {
                dLists = sPlayerRightHandClosedDLs + gSaveContext.linkAge;
                sRightHandType = PLAYER_MODELTYPE_RH_CLOSED;
            }

            *dList = *(dLists + sDListsLodOffset);
        } else if (limbIndex == PLAYER_LIMB_SHEATH) {
          if(!ghackSave.disArmed){
              Gfx** dLists = this->sheathDLists;
              
              if ((this->sheathType == PLAYER_MODELTYPE_SHEATH_18) || (this->sheathType == PLAYER_MODELTYPE_SHEATH_19)) {
                  dLists += this->currentShield * 4;
                  if (!LINK_IS_ADULT && (this->currentShield < PLAYER_SHIELD_HYLIAN) &&
                      (gSaveContext.equips.buttonItems[0] != ITEM_SWORD_KOKIRI)) { 
                      dLists += PLAYER_SHIELD_MAX * 4;
                  }
              } else if (!LINK_IS_ADULT && ((this->sheathType == PLAYER_MODELTYPE_SHEATH_16) ||
                          (this->sheathType == PLAYER_MODELTYPE_SHEATH_17)) &&
                         (gSaveContext.equips.buttonItems[0] != ITEM_SWORD_KOKIRI)) {
                  dLists = D_80125D28 + PLAYER_SHIELD_MAX * 4;
              }
  
              *dList = *(dLists + sDListsLodOffset);
          }
        } else if (limbIndex == PLAYER_LIMB_WAIST) {
            *dList = *(this->waistDLists + sDListsLodOffset);
        }
    }
    return false;
}
