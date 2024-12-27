#include <uLib.h>
#include "Facamera.h"
#include "object/0x0005-FacameraObj.h"

void Facamera_Init(Facamera* this, PlayState* play);
void Facamera_Destroy(Facamera* this, PlayState* play);
void Facamera_Update(Facamera* this, PlayState* play);
void Facamera_Draw(Facamera* this, PlayState* play);
void Facamera_Rotate(Facamera* this, PlayState* play);

const ActorInit Facamera_initvars = {
    0x1AA,
    ACTORCAT_MISC,  //ACTORCAT_MISC is the actor category.See the ActorCategory enum in z64actor.h for a list of categories.
    0x00000030,
    5,
    sizeof(Facamera),
    (ActorFunc)Facamera_Init,
    (ActorFunc)Facamera_Destroy,
    (ActorFunc)Facamera_Update,
    (ActorFunc)Facamera_Draw,
};

void Facamera_Init(Facamera* this, PlayState* play) {

   
}

void Facamera_Destroy(Facamera* this, PlayState* play) {
    
}


void Facamera_Rotate(Facamera* this, PlayState* play) {

    Math_ApproachS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer + 0x3800, 3, 0x11C);
    if ((ABS(this->actor.shape.rot.y - this->actor.yawTowardsPlayer + 0x3800) < 0x8000)) {
        
        func_8002F974(&this->actor, NA_SE_EV_ELEVATOR_MOVE3 - SFX_FLAG);
    }
    
}

void Facamera_Update(Facamera* this, PlayState* play) {
    Facamera_Rotate(this, play);


}

void Facamera_Draw(Facamera* this, PlayState* play) {

   Gfx_DrawDListOpa(play, &gFacameraObj_DlFacamera);
 
   
}
