#include "SuperElevator.h"
#include "object/0x0193-SuperElevatorObj.h"

void SuperElevator_Init(Actor* thisx, PlayState* play);
void SuperElevator_Destroy(SuperElevator* this, PlayState* play);
void SuperElevator_Update(SuperElevator* this, PlayState* play);
void SuperElevator_Draw(SuperElevator* this, PlayState* play);
void SuperElevator_SwitchJudge(SuperElevator* this, PlayState* play);
void SuperElevator_VerticalUp(SuperElevator* this, PlayState* play);
void SuperElevator_VerticalDown(SuperElevator* this, PlayState* play);
void SuperElevator_Idle(SuperElevator* this, PlayState* play);




const ActorInit sSuperElevator_InitVars = {
    .id = 0x01E1,
    .category     = ACTORCAT_BG,
    .flags        = 0x00000030,
    .objectId = 0x0193,
    .instanceSize = sizeof(SuperElevator),
    .init         = (ActorFunc)SuperElevator_Init,
    .destroy      = (ActorFunc)SuperElevator_Destroy,
    .update       = (ActorFunc)SuperElevator_Update,
    .draw         = (ActorFunc)SuperElevator_Draw
};

void SuperElevator_Init(Actor* thisx, PlayState* play) {
    SuperElevator* this = (SuperElevator*)thisx;
    CollisionHeader* colHeader = NULL;
    DynaPolyActor_Init(&this->dyna, DPM_PLAYER);
    CollisionHeader_GetVirtual(&gSuperElevatorObj_CollSuperElevatorObj, &colHeader);
    this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, thisx, colHeader);
    u8 reversed = 0;
    this->moveDis  = (this->dyna.actor.params >> 4 & 0xF) * 100; //Variable Form : 00*0 (hex)
    this->switchFlag = (this->dyna.actor.params >> 8) & 0xFF; //Variable Form : **00 (hex)  0XFF used to clear "1" in high byte, which is a double insurance
    reversed = (this->dyna.actor.params & 0xF); //Variable Form : 000* (hex)
    //0xFF BIN:0000 0000  1111 1111
    //         HIGH BYTE| LOW BYTE
    if(reversed == 1){
        if(!Flags_GetSwitch(play,this->switchFlag)){
            Flags_SetSwitch(play, this->switchFlag);
        }
    }
    if ((Flags_GetSwitch(play, this->switchFlag))) {
        this->dyna.actor.world.pos.y = this->dyna.actor.world.pos.y + this->moveDis;
        this->backHomeFlag = false;
    }
    else {
        this->dyna.actor.world.pos.y = this->dyna.actor.home.pos.y;
        this->backHomeFlag = true;
    }
    
    this->actionFunc = SuperElevator_Idle;

}

void SuperElevator_SwitchJudge(SuperElevator* this, PlayState* play) {

    if ((Flags_GetSwitch(play, this->switchFlag))) {
        this->actionFunc = SuperElevator_VerticalUp;
    }

    if (!(Flags_GetSwitch(play, this->switchFlag))) {
        this->actionFunc = SuperElevator_VerticalDown;
    }
   
}

void SuperElevator_VerticalUp(SuperElevator* this, PlayState* play) {

    if((Math_StepToF(&this->dyna.actor.world.pos.y, this->dyna.actor.home.pos.y + this->moveDis, 4.0f)) ){
        Audio_PlayActorSfx2(&this->dyna.actor, NA_SE_EV_BLOCK_BOUND);
        this->backHomeFlag = false;
        this->actionFunc = SuperElevator_Idle;

    }
    else {
        func_8002F974(&this->dyna.actor, NA_SE_EV_ELEVATOR_MOVE3 - SFX_FLAG);
        if (!(Flags_GetSwitch(play, this->switchFlag))) {
            this->actionFunc = SuperElevator_VerticalDown;
        }
    }
  
}

void SuperElevator_VerticalDown(SuperElevator* this, PlayState* play) {


    if (Math_StepToF(&this->dyna.actor.world.pos.y, this->dyna.actor.home.pos.y, 4.0f)) {
        Audio_PlayActorSfx2(&this->dyna.actor, NA_SE_EV_BLOCK_BOUND);
        this->backHomeFlag = true;    
        this->actionFunc = SuperElevator_Idle;
    }
    else {
        func_8002F974(&this->dyna.actor, NA_SE_EV_ELEVATOR_MOVE3 - SFX_FLAG);
        if ((Flags_GetSwitch(play, this->switchFlag))) {
            this->actionFunc = SuperElevator_VerticalUp;
        }

    }

}


void SuperElevator_Idle(SuperElevator* this, PlayState* play) {


    if ((Flags_GetSwitch(play, this->switchFlag))) {
       
        if (this->backHomeFlag == true) {
            this->actionFunc = SuperElevator_VerticalUp;
        }
        else {
            return;
        
        }
    }
    if (!(Flags_GetSwitch(play, this->switchFlag))) {
        
        if (this->backHomeFlag == false) {
            this->actionFunc = SuperElevator_VerticalDown;
        }
        else {
            return;
        }
    }
}
 

void SuperElevator_Destroy(SuperElevator* this, PlayState* play) {
    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

void SuperElevator_Update(SuperElevator* this, PlayState* play) {
    this->actionFunc(this, play);


}

s32 SuperElevator_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3f* pos, Vec3s* rot, void* thisx) {
    SuperElevator* this = (void*)thisx;
    
    return 0;
}

void SuperElevator_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3s* rot, void* thisx) {
    SuperElevator* this = (void*)thisx;
}

void SuperElevator_Draw(SuperElevator* this, PlayState* play) {
    // GfxPrint printer;
    Gfx_DrawDListOpa(play, &gSuperElevatorObj_DlSuperElevatorObj);

    // Gfx* gfx = play->state.gfxCtx->polyOpa.p + 1;
    // gSPDisplayList(play->state.gfxCtx->overlay.p++, gfx);
    // GfxPrint_Init(&printer);
    // GfxPrint_Open(&printer, gfx);
    // GfxPrint_SetColor(&printer, 255, 0, 0, 255);
    // GfxPrint_SetPos(&printer, 1, 1);
    // if (Flags_GetSwitch(play, this->switchFlag)) {
    //     GfxPrint_Printf(&printer, "flagswicthGet! %x", this->switchFlag);
    // }
    // else {
    //     GfxPrint_Printf(&printer, "nothing! %x", this->switchFlag);
    // }
    // gfx = GfxPrint_Close(&printer);
    // GfxPrint_Destroy(&printer);
    // gSPEndDisplayList(gfx++);
    // gSPBranchList(play->state.gfxCtx->polyOpa.p, gfx);
    // play->state.gfxCtx->polyOpa.p = gfx;

}
