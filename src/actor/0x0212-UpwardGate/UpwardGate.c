#include "UpwardGate.h"
#include "object/0x0208-UpwardGateObj.h"

void UpwardGate_Init(Actor* thisx, PlayState* play);
void UpwardGate_Draw(UpwardGate* this, PlayState* play);
void UpwardGate_Lurk(UpwardGate* this, PlayState* play);
void UpwardGate_Destroy(UpwardGate* this, PlayState* play);
void UpwardGate_Update(UpwardGate* this, PlayState* play) ;
void UpwardGate_CheckSwitch(UpwardGate* this, PlayState* play);
void UpwardGate_Rise(UpwardGate* this, PlayState* play);
void UpwardGate_Down(UpwardGate* this, PlayState* play) ;
void UpwardGate_Idle(UpwardGate* this, PlayState* play) ;
void UpwardGate_Nothing(UpwardGate* this, PlayState* play) ;



const ActorInit sUpwardGate_InitVars = {
    .id = 0x0212,
    .category     = ACTORCAT_BG,
    .flags        = 0x00000030,
    .objectId = 0x0208,
    .instanceSize = sizeof(UpwardGate),
    .init         = (ActorFunc)UpwardGate_Init,
    .destroy      = (ActorFunc)UpwardGate_Destroy,
    .update       = (ActorFunc)UpwardGate_Update,
    .draw         = (ActorFunc)UpwardGate_Draw
};

void UpwardGate_Init(Actor* thisx, PlayState* play) {
    UpwardGate* this = (UpwardGate*)thisx;
    CollisionHeader* colHeader = NULL;
    DynaPolyActor_Init(&this->dyna, DPM_PLAYER);
    CollisionHeader_GetVirtual(&gUpwardGateObj_CollUpwardGateObj, &colHeader);
    this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, thisx, colHeader);
    this->reverse =   (this->dyna.actor.params >> 4 & 0xF); //Variable Form : 00*0 (hex)
    this->switchFlag = (this->dyna.actor.params >> 8) & 0x3F; //Variable Form : **00 (hex)
    if(this->reverse == 1){
        if(!Flags_GetSwitch(play,this->switchFlag)){
            Flags_SetSwitch(play, this->switchFlag);
        }
    }
    if ((Flags_GetSwitch(play, this->switchFlag))) {
        this->dyna.actor.world.pos.y = this->dyna.actor.home.pos.y - 150.0f;
        this->backHomeFlag = false;
    }else {
        this->backHomeFlag = true;
    }
    this->actionFunc = UpwardGate_Idle;
}


void UpwardGate_Destroy(UpwardGate* this, PlayState* play) {
     DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

void UpwardGate_Update(UpwardGate* this, PlayState* play) {
        this->actionFunc(this, play);
}

void UpwardGate_CheckSwitch(UpwardGate* this, PlayState* play) {
    if(this->reverse){
        if (Flags_GetSwitch(play, (this->dyna.actor.params >> 8) & 0x3F)) {
            this->actionFunc = UpwardGate_Rise;
        }
    }else{
        if (Flags_GetSwitch(play, (this->dyna.actor.params >> 8) & 0x3F)) {
            this->actionFunc = UpwardGate_Down;
        }        
    }
}

void UpwardGate_Rise(UpwardGate* this, PlayState* play) {
    Math_SmoothStepToF(&this->dyna.actor.velocity.y, 4.0f, 0.1f, 0.8f, 0.0f);
    if (fabsf(Math_SmoothStepToF(&this->dyna.actor.world.pos.y, this->dyna.actor.home.pos.y, 0.3f,
                                 this->dyna.actor.velocity.y, 0.3f)) < 0.001f) {
        this->dyna.actor.world.pos.y = this->dyna.actor.home.pos.y;
         Audio_PlayActorSfx2(&this->dyna.actor, NA_SE_EV_METALDOOR_STOP);
         this->backHomeFlag = true;
        this->actionFunc = UpwardGate_Idle;
    }else{
        if ((Flags_GetSwitch(play, this->switchFlag))) {
            this->actionFunc = UpwardGate_Down;
        }
        func_8002F974(&this->dyna.actor, NA_SE_EV_METALDOOR_SLIDE - SFX_FLAG);
    }
}

void UpwardGate_Down(UpwardGate* this, PlayState* play) {
    Math_SmoothStepToF(&this->dyna.actor.velocity.y, 4.0f, 0.1f, 0.8f, 0.0f);
     this->backHomeFlag = false;
    if (fabsf(Math_SmoothStepToF(&this->dyna.actor.world.pos.y, this->dyna.actor.home.pos.y - 150.f, 0.3f,
                                 this->dyna.actor.velocity.y, 0.3f)) < 0.001f) {
        this->dyna.actor.world.pos.y = this->dyna.actor.home.pos.y - 150.f;
        Audio_PlayActorSfx2(&this->dyna.actor, NA_SE_EV_METALDOOR_STOP);
        this->actionFunc = UpwardGate_Idle;
    }else{
        if (!(Flags_GetSwitch(play, this->switchFlag))) {
            this->actionFunc = UpwardGate_Rise;
        }
        func_8002F974(&this->dyna.actor, NA_SE_EV_METALDOOR_SLIDE - SFX_FLAG);
    }
}



void UpwardGate_Idle(UpwardGate* this, PlayState* play) {
    if ((Flags_GetSwitch(play, this->switchFlag))) {
        if (this->backHomeFlag == true) {
            this->actionFunc = UpwardGate_Down;
        }
        else {
            return;
        }
    }
    if (!(Flags_GetSwitch(play, this->switchFlag))) {
        if (this->backHomeFlag == false) {
            this->actionFunc = UpwardGate_Rise;
        }
        else {
            return;
        }
    }
}
void UpwardGate_Nothing(UpwardGate* this, PlayState* play){

}


s32 UpwardGate_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3f* pos, Vec3s* rot, void* thisx) {
    UpwardGate* this = (void*)thisx;
    
    return 0;
}

void UpwardGate_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3s* rot, void* thisx) {
    UpwardGate* this = (void*)thisx;
}

void UpwardGate_Draw(UpwardGate* this, PlayState* play) {
        Gfx_DrawDListOpa(play, &gUpwardGateObj_DlUpwardGateObj);
}