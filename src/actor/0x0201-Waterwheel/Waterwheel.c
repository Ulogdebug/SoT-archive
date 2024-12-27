#include "Waterwheel.h"
#include "object/0x0195-WaterwheelObj.h"


void Waterwheel_Init(Actor* thisx, PlayState* play);
void Waterwheel_Destroy(Waterwheel* this, PlayState* play);
void Waterwheel_Update(Waterwheel* this, PlayState* play);
void Waterwheel_Draw(Waterwheel* this, PlayState* play);
void Waterwheel_Spin(Waterwheel* this, PlayState* play);
void Waterwheel_Idle(Waterwheel* this, PlayState* play);
void Waterwheel_Reverse_Spin(Waterwheel* this, PlayState* play);


const ActorInit sWaterwheel_InitVars = {
    .id = 0x0201,
    .category     = ACTORCAT_BG,
    .flags        = 0x00000030,
    .objectId = 0x0195,
    .instanceSize = sizeof(Waterwheel),
    .init         = (ActorFunc)Waterwheel_Init,
    .destroy      = (ActorFunc)Waterwheel_Destroy,
    .update       = (ActorFunc)Waterwheel_Update,
    .draw         = (ActorFunc)Waterwheel_Draw
};


void Waterwheel_Init(Actor* thisx, PlayState* play) {
    Waterwheel* this = (Waterwheel*)thisx;
    this->type = (this->dyna.actor.params & 7); //Variable Form : 000* (hex)
    CollisionHeader* colHeader = NULL;
    DynaPolyActor_Init(&this->dyna, DPM_PLAYER);
        CollisionHeader_GetVirtual(&gWaterwheelObj_CollWaterwheelObj[0], &colHeader);
    this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, thisx, colHeader);
     this->spinSpeed  = (this->dyna.actor.params >> 4 & 0XF) * 50; //Variable Form : 00*0 (hex)
    this->switchFlag = (this->dyna.actor.params >> 8) & 0xFF; //Variable Form : **00 (hex)
    if(this->type == WATER_WHEEL){
            this->actionFunc = Waterwheel_Idle;
        }
    else if(this->type == REVERSE_WATER_WHEEL){
           this->actionFunc = Waterwheel_Reverse_Spin;
    }


}

void Waterwheel_Reverse_Spin(Waterwheel* this, PlayState* play){
    if(!Flags_GetSwitch(play, this->switchFlag)){
        this->dyna.actor.shape.rot.x += this->spinSpeed;
        this->dyna.actor.world.rot.x = this->dyna.actor.shape.rot.x;
        func_8002F974(&this->dyna.actor, NA_SE_EV_TOGE_STICK_ROLLING - SFX_FLAG);
    }
    else{
        return;
    }
}

void Waterwheel_Spin(Waterwheel* this, PlayState* play){
    if ((Flags_GetSwitch(play, this->switchFlag))||(this->switchFlag == 0X3F)) {
        this->dyna.actor.shape.rot.x += this->spinSpeed;
        this->dyna.actor.world.rot.x = this->dyna.actor.shape.rot.x;
        func_8002F974(&this->dyna.actor, NA_SE_EV_TOGE_STICK_ROLLING - SFX_FLAG);
    }
    else{
        this->actionFunc = Waterwheel_Idle;
    }

}

void Waterwheel_Idle(Waterwheel* this, PlayState* play){
    if ((Flags_GetSwitch(play, this->switchFlag))||(this->switchFlag == 0X3F)) {
        this->actionFunc = Waterwheel_Spin;
    }
    else{
        return;
    }

}

void Waterwheel_Destroy(Waterwheel* this, PlayState* play) {
        DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);

}


void Waterwheel_Update(Waterwheel* this, PlayState* play) {
        this->actionFunc(this, play);

   
}

s32 Waterwheel_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3f* pos, Vec3s* rot, void* thisx) {
    Waterwheel* this = (void*)thisx;
    
    return 0;
}

void Waterwheel_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3s* rot, void* thisx) {
    Waterwheel* this = (void*)thisx;
}

void Waterwheel_Draw(Waterwheel* this, PlayState* play) {
    
    Gfx_DrawDListOpa(play, &gWaterwheelObj_DlWaterwheelObj[0]); 
   
}
