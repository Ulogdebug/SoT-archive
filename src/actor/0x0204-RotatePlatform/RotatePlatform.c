#include "RotatePlatform.h"
#include "object/0x019A-RotatePlatformObj.h"

void RotatePlatform_Init(Actor* thisx, PlayState* play);
void RotatePlatform_Destroy(RotatePlatform* this, PlayState* play);
void RotatePlatform_Update(RotatePlatform* this, PlayState* play);
void RotatePlatform_Draw(RotatePlatform* this, PlayState* play);
void RotatePlatform_SetupClockwise(RotatePlatform* this, PlayState* play);
void RotatePlatform_SetupCounterwise(RotatePlatform* this, PlayState* play);
void RotatePlatform_SetupBackHome(RotatePlatform* this, PlayState* play);
void RotatePlatform_Rotate(RotatePlatform* this, PlayState* play);
void RotatePlatform_Idle(RotatePlatform* this, PlayState* play);
void RotatePlatform_DirectionStateSet(RotatePlatform* this, PlayState* play);



const ActorInit sRotatePlatform_InitVars = {
    .id = 0x0204,
    .category     = ACTORCAT_BG,
    .flags        = 0x00000000,
    .objectId = 0x019A,
    .instanceSize = sizeof(RotatePlatform),
    .init         = (ActorFunc)RotatePlatform_Init,
    .destroy      = (ActorFunc)RotatePlatform_Destroy,
    .update       = (ActorFunc)RotatePlatform_Update,
    .draw         = (ActorFunc)RotatePlatform_Draw
};

void RotatePlatform_Init(Actor* thisx, PlayState* play) {
    RotatePlatform* this = (RotatePlatform*)thisx;
    CollisionHeader* colHeader = NULL;
    DynaPolyActor_Init(&this->dyna, DPM_PLAYER);
    CollisionHeader_GetVirtual(&gRotatePlatformObj_CollRotatePlatformObj, &colHeader);
    this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, thisx, colHeader);
    this->directionState = NONE;
    this->ClockFlag = (this->dyna.actor.params >> 8) & 0xFF;  //Variable Form : **00 (hex) 
    this->CounterFlag = (this->dyna.actor.params & 0xFF);  //Variable Form : 00** (hex) 
    this->targetDegree = 0x0;
    this->actionFunc = RotatePlatform_DirectionStateSet;

}

void RotatePlatform_DirectionStateSet(RotatePlatform* this, PlayState* play){
     if(Flags_GetSwitch(play, this->ClockFlag)){
         Flags_UnsetSwitch(play, this->CounterFlag);
         this->directionState = CLOCKWISE;
         RotatePlatform_SetupClockwise(this,play);
     }
     else if(Flags_GetSwitch(play, this->CounterFlag)){
         Flags_UnsetSwitch(play, this->ClockFlag);
         this->directionState = COUNTERWISE;
         RotatePlatform_SetupCounterwise(this,play);
     }
     else if((!Flags_GetSwitch(play, this->CounterFlag)) && (!Flags_GetSwitch(play, this->ClockFlag)) && (this->directionState != NONE) ){
        if(this->directionState == CLOCKWISE){
            this->directionState = BACKHOME_CLOCKWISE;
        }
        else if (this->directionState == COUNTERWISE){
            this->directionState = BACKHOME_COUNTERWISE;
        }
        RotatePlatform_SetupBackHome(this,play);
    }
}
    



void RotatePlatform_Destroy(RotatePlatform* this, PlayState* play) {
    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);

}

void RotatePlatform_SetupClockwise(RotatePlatform* this, PlayState* play){
    this->targetDegree = this->dyna.actor.home.rot.y + 0x4000;
    this->actionFunc = RotatePlatform_Rotate;
}


void RotatePlatform_SetupCounterwise(RotatePlatform* this, PlayState* play){
    this->targetDegree = this->dyna.actor.home.rot.y - 0x4000;
    this->actionFunc = RotatePlatform_Rotate;
}

void RotatePlatform_SetupBackHome(RotatePlatform* this, PlayState* play){
    this->targetDegree = this->dyna.actor.home.rot.y;
    this->actionFunc = RotatePlatform_Rotate;
}


void RotatePlatform_Rotate(RotatePlatform* this, PlayState* play){
    s16 step;
    step = 0x200;
    //func_8002D7EC(&this->dyna.actor);
    if(abs(this->dyna.actor.shape.rot.y - this->targetDegree) <= step/*Math_StepToF(&this->dyna.actor.shape.rot.y, this->targetDegree, 0x100))*/ ){
       this->dyna.actor.shape.rot.y = this->targetDegree;
        Audio_PlayActorSfx2(&this->dyna.actor, NA_SE_EV_BLOCK_BOUND);
        this->dyna.actor.world.rot.y = this->dyna.actor.shape.rot.y;
        this->actionFunc = RotatePlatform_Idle;
    }else {
        if((this->directionState == CLOCKWISE) || (this->directionState == BACKHOME_COUNTERWISE)){
            this->dyna.actor.shape.rot.y = this->dyna.actor.shape.rot.y + step;
        }
        if((this->directionState == COUNTERWISE) || (this->directionState == BACKHOME_CLOCKWISE)){
            this->dyna.actor.shape.rot.y = this->dyna.actor.shape.rot.y - step;
        }
        func_8002F974(&this->dyna.actor, NA_SE_EV_ELEVATOR_MOVE3 - SFX_FLAG);
        this->dyna.actor.world.rot.y = this->dyna.actor.shape.rot.y;
        RotatePlatform_Idle(this,play);
    }
}

void RotatePlatform_Idle(RotatePlatform* this, PlayState* play){
    
    switch(this->directionState){
        case NONE:
            break;
        case CLOCKWISE:
            if(Flags_GetSwitch(play, this->CounterFlag)){
                Flags_UnsetSwitch(play, this->ClockFlag);
            }
            break;
        case BACKHOME_CLOCKWISE:
        case BACKHOME_COUNTERWISE:
            if(Flags_GetSwitch(play, this->ClockFlag)){
                Flags_UnsetSwitch(play, this->CounterFlag);
            }
            else if(Flags_GetSwitch(play, this->CounterFlag)){
                Flags_UnsetSwitch(play, this->ClockFlag);
            }
            break;
        case COUNTERWISE:
            if(Flags_GetSwitch(play, this->ClockFlag)){
                Flags_UnsetSwitch(play, this->CounterFlag);
            }
            break;
        default:
            break;
    }

    if(this->directionState != NONE){
        RotatePlatform_DirectionStateSet(this,play);
    }
}

void RotatePlatform_Update(RotatePlatform* this, PlayState* play) {
     this->actionFunc(this, play);


}

s32 RotatePlatform_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3f* pos, Vec3s* rot, void* thisx) {
    RotatePlatform* this = (void*)thisx;
    
    return 0;
}

void RotatePlatform_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3s* rot, void* thisx) {
    RotatePlatform* this = (void*)thisx;
}

void RotatePlatform_Draw(RotatePlatform* this, PlayState* play) {
        Gfx_DrawDListOpa(play, &gRotatePlatformObj_DlRotatePlatformObj);
        // if(Flags_GetSwitch(play, this->ClockFlag))
        // Debug_Text(255, 0, 0, 1, 1, "ClockFlag! %x", this->ClockFlag);
        // else{
        //     Debug_Text(255, 0, 0, 1, 1, "Nothing! %x", this->ClockFlag);
        // }
}
