#include <uLib.h>
#include "bonfire.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h"

void Bonfire_Init(Bonfire* this, PlayState* play);
void Bonfire_Destroy(Bonfire* this, PlayState* play);
void Bonfire_Update(Bonfire* this, PlayState* play);
void Bonfire_Draw(Bonfire* this, PlayState* play);

const ActorInit Bonfire_initvars = {
    1,
    ACTORCAT_MISC,  
    0x00000030,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(Bonfire),
    (ActorFunc)Bonfire_Init,
    (ActorFunc)Bonfire_Destroy,
    (ActorFunc)Bonfire_Update,
    (ActorFunc)Bonfire_Draw,
};

void Bonfire_Init(Bonfire* this, PlayState* play) {

    if(Actor_FindNearby(play, &this->actor, 0x0206, ACTORCAT_BG, 100.0f) != NULL){
        this->superBonfireNearby = 1;
    }

}

void Bonfire_Destroy(Bonfire* this, PlayState* play) {
    
}

void Bonfire_Update(Bonfire* this, PlayState* play) {

    if (this->timer > 0) {
        this->timer--;
    }
    if (this->timer == 0) {
        if (this->actor.xzDistToPlayer < 125.0f && ABS(this->actor.yDistToPlayer) < 30.0f) {
            this->timer = (this->actor.xzDistToPlayer < 80.0f) ? 15 : 30;
            if((gSaveContext.inventory.items[SLOT_BOTTLE_1]) != ITEM_NONE){
                if(this->superBonfireNearby){
                    Bonfire_SearchAndFillRedPotion(this,play);
                }else if((this->actor.params) != -1){
                    Bonfire_SearchAndFillMilk(this,play);
                }
            }
            Magic_Fill(play);
            if(gSaveContext.health < gSaveContext.healthCapacity){
                play->damagePlayer(play, 0x10);
            }
            if((play->sceneId != 0x5) || ((this->actor.params == 1) && (play->sceneId == 0x5))){
                Item_Give(play, ITEM_BOMBS_10);
                Item_Give(play, ITEM_ARROWS_MEDIUM);
            }
        }            
    }
}

void Bonfire_SearchAndFillMilk(Bonfire* this, PlayState* play) {
    s16 bottleSlot,cButtons,order = 0;
    for(bottleSlot = SLOT_BOTTLE_1 ; bottleSlot <= SLOT_BOTTLE_4; bottleSlot++){
        if(gSaveContext.inventory.items[bottleSlot] == ITEM_BOTTLE || gSaveContext.inventory.items[bottleSlot] == ITEM_MILK_HALF){
            gSaveContext.inventory.items[bottleSlot] = ITEM_MILK_BOTTLE;
                Audio_PlaySfxGeneral(
                   NA_SE_SY_CARROT_RECOVER, &gSfxDefaultPos, 4,
                   &gSfxDefaultFreqAndVolScale, &gSfxDefaultFreqAndVolScale,
                   &gSfxDefaultReverb
               );
            break;
        }
    }
    for(cButtons = 0; cButtons < 4; cButtons++){
        if(gSaveContext.equips.buttonItems[cButtons] == ITEM_BOTTLE || gSaveContext.equips.buttonItems[cButtons] == ITEM_MILK_HALF){
            gSaveContext.equips.buttonItems[cButtons] = ITEM_MILK_BOTTLE;
            Interface_LoadItemIcon1(play, cButtons);
            gSaveContext.buttonStatus[cButtons] = BTN_ENABLED;
            break;
        }
    }
}

void Bonfire_SearchAndFillRedPotion(Bonfire* this, PlayState* play) {
    s16 bottleSlot,cButtons,order = 0;
    for(bottleSlot = SLOT_BOTTLE_1 ; bottleSlot <= SLOT_BOTTLE_4; bottleSlot++){
        if(gSaveContext.inventory.items[bottleSlot] == ITEM_BOTTLE){
            gSaveContext.inventory.items[bottleSlot] = ITEM_POTION_RED;
                Audio_PlaySfxGeneral(
                   NA_SE_SY_CARROT_RECOVER, &gSfxDefaultPos, 4,
                   &gSfxDefaultFreqAndVolScale, &gSfxDefaultFreqAndVolScale,
                   &gSfxDefaultReverb
               );
            break;
        }
    }
    for(cButtons = 0; cButtons < 4; cButtons++){
        if(gSaveContext.equips.buttonItems[cButtons] == ITEM_BOTTLE){
            gSaveContext.equips.buttonItems[cButtons] = ITEM_POTION_RED;
            Interface_LoadItemIcon1(play, cButtons);
            gSaveContext.buttonStatus[cButtons] = BTN_ENABLED;
            break;
        }
    }
}

void Bonfire_Draw(Bonfire* this, PlayState* play) {

}
