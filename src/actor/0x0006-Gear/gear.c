#include <uLib.h>
#include "gear.h"
#include "object/0x0004-GearObj.h"

void Gear_Init(Actor* thisx, PlayState* play);
void Gear_Destroy(Actor* thisx, PlayState* play);
void Gear_Update(Actor* thisx, PlayState* play);
void Gear_Draw(Actor* thisx, PlayState* play);
void Gear_Rotate(Gear* this, PlayState* play);

const ActorInit Gear_initvars = {
    6,
    ACTORCAT_BG,  //ACTORCAT_MISC is the actor category.See the ActorCategory enum in z64actor.h for a list of categories.
    0x00000030,
    4,
    sizeof(Gear),
    (ActorFunc)Gear_Init,
    (ActorFunc)Gear_Destroy,
    (ActorFunc)Gear_Update,
    (ActorFunc)Gear_Draw,
};

void Gear_Init(Actor* thisx, PlayState* play) {
    Gear* this = (Gear*)thisx;
    CollisionHeader* colHeader = NULL;
    DynaPolyActor_Init(&this->dyna, DPM_PLAYER);
    CollisionHeader_GetVirtual(&gGearObj_CollGearObj, &colHeader);
    this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, thisx, colHeader);
    this->timer = 10;
   
}

void Gear_Destroy(Actor* thisx, PlayState* play) {
    Gear* this = (Gear*)thisx;
    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}


void Gear_Rotate(Gear* this, PlayState* play) {

    if (this->timer == 0) {

        this->dyna.actor.shape.rot.z += 0x1000;
        this->timer = 20;
        Audio_PlayActorSfx2(&this->dyna.actor, NA_SE_EV_BLOCK_BOUND);
    }
    else {
        this->timer--;
    }

}

void Gear_Update(Actor* thisx, PlayState* play) {
    Gear* this = (Gear*)thisx;
    Gear_Rotate(this, play);
    

}

void Gear_Draw(Actor* thisx, PlayState* play) {
    Gear* this = (Gear*)thisx;
   Gfx_DrawDListOpa(play, &gGearObj_DlGearObj); 
 
   
}
