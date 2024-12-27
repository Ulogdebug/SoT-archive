#include <z64hdr.h>
#include "code/z_player_lib.h"
#include "hackplayer/hackplayer.h"
#include "uLib.h"


void Player_UpdateBottleForInventory(PlayState* play, Player* this, s32 newItem, s16* quickPotionSlot, s16* quickPotionItem, s32 actionParam) {
    s16 a = 0; 
    if(*quickPotionSlot == 0){ //when player don't ulitize quick potion function to use potion
        if((gSaveContext.inventory.items[gSaveContext.equips.cButtonSlots[this->heldItemButton - 1]] == ITEM_MILK_BOTTLE && (newItem == ITEM_BOTTLE))){
            newItem = ITEM_MILK_HALF;
        }
        gSaveContext.inventory.items[gSaveContext.equips.cButtonSlots[this->heldItemButton - 1]] = newItem;
        gSaveContext.equips.buttonItems[this->heldItemButton] = newItem;
        Interface_LoadItemIcon1(play, this->heldItemButton);
        play->pauseCtx.cursorItem[PAUSE_ITEM] = newItem;
        gSaveContext.buttonStatus[this->heldItemButton] = BTN_ENABLED;

         if (newItem != ITEM_BOTTLE) {
            this->heldItemId = newItem;
            this->heldItemActionParam = actionParam;
        }
    }else{
        if(((*quickPotionItem == ITEM_MILK_BOTTLE) && (newItem == ITEM_BOTTLE))){
            newItem = ITEM_MILK_HALF;
        }
        for(a=0;a<4;a++){
            if(gSaveContext.equips.buttonItems[a] == *quickPotionItem){
                gSaveContext.inventory.items[*quickPotionSlot] = newItem;
                gSaveContext.equips.buttonItems[a] = newItem;
                Interface_LoadItemIcon1(play, a);
                gSaveContext.buttonStatus[a] = BTN_ENABLED;
                break;
            }
        }
        gSaveContext.inventory.items[*quickPotionSlot] = newItem;
        
    }
    *quickPotionSlot = 0;
    *quickPotionItem = 0;
    this->itemActionParam = actionParam;
}
