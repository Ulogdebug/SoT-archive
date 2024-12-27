#include "RotateCylinder.h"
#include "object/0x0201-RotateCylinderObj.h"

void RotateCylinder_Init(Actor* thisx, PlayState* play);
void RotateCylinder_Destroy(RotateCylinder* this, PlayState* play);
void RotateCylinder_Update(RotateCylinder* this, PlayState* play);
void RotateCylinder_Draw(RotateCylinder* this, PlayState* play);
void RotateCylinder_SwitchJudge(RotateCylinder* this, PlayState* play);
void RotateCylinder_Rotate(RotateCylinder* this, PlayState* play);
void RotateCylinder_RotateBack(RotateCylinder* this, PlayState* play);
void RotateCylinder_SlowDown(RotateCylinder* this, PlayState* play);
void RotateCylinder_Idle(RotateCylinder* this, PlayState* play);


const ActorInit sRotateCylinder_InitVars = {
    .id = 0x020D,
    .category     = ACTORCAT_BG,
    .flags        = 0x00000030,
    .objectId = 0x0201,
    .instanceSize = sizeof(RotateCylinder),
    .init         = (ActorFunc)RotateCylinder_Init,
    .destroy      = (ActorFunc)RotateCylinder_Destroy,
    .update       = (ActorFunc)RotateCylinder_Update,
    .draw         = (ActorFunc)RotateCylinder_Draw
};


void RotateCylinder_Init(Actor* thisx, PlayState* play) {
    RotateCylinder* this = (RotateCylinder*)thisx;
    CollisionHeader* colHeader = NULL;
    DynaPolyActor_Init(&this->dyna, DPM_PLAYER);
    CollisionHeader_GetVirtual(&gRotateCylinderObj_CollRotateCylinder, &colHeader);
    this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, thisx, colHeader);
    this->switchFlag = (this->dyna.actor.params >> 8) & 0xFF; //Variable Form : **00 (hex)  0XFF used to clear "1" in high byte, which is a double insurance
    u8 reverse = (this->dyna.actor.params >> 4 & 0xF); //Variable Form : 00*0 (hex)
    if(reverse){
        Flags_SetSwitch(play, this->switchFlag);
    }
    //0xFF BIN:0000 0000  1111 1111
    //         HIGH BYTE| LOW BYTE
     if ((Flags_GetSwitch(play, this->switchFlag))) {
         this->backHomeFlag = false;
         this->dyna.actor.shape.rot.y = this->dyna.actor.home.rot.y + 0x8000;
     }else{
         this->backHomeFlag = true;
     }
     this->actionFunc = RotateCylinder_Idle;

}

void RotateCylinder_Rotate(RotateCylinder* this, PlayState* play) {
    if (!(Flags_GetSwitch(play, this->switchFlag))) {
        this->actionFunc = RotateCylinder_RotateBack;
    }
    if(ABS(this->dyna.actor.shape.rot.y - this->dyna.actor.home.rot.y + 0x8000) <= 0x400){
        this->dyna.actor.shape.rot.y = this->dyna.actor.home.rot.y + 0x8000;
        Audio_PlayActorSfx2(&this->dyna.actor, NA_SE_EV_BLOCK_BOUND);
        this->backHomeFlag = false;
        this->actionFunc = RotateCylinder_Idle;
     }else{
        this->dyna.actor.shape.rot.y = this->dyna.actor.shape.rot.y + 0x200;
        this->dyna.actor.world.rot.y = this->dyna.actor.shape.rot.y;
        func_8002F974(&this->dyna.actor, NA_SE_EV_ELEVATOR_MOVE3 - SFX_FLAG);
     }
}

void RotateCylinder_RotateBack(RotateCylinder* this, PlayState* play){
    if ((Flags_GetSwitch(play, this->switchFlag))) {
        this->actionFunc = RotateCylinder_Rotate;
    }
    if (ABS(this->dyna.actor.shape.rot.y - this->dyna.actor.home.rot.y) <= 0x400){
        this->dyna.actor.shape.rot.y = this->dyna.actor.home.rot.y;
        Audio_PlayActorSfx2(&this->dyna.actor, NA_SE_EV_BLOCK_BOUND);
        this->backHomeFlag = true;
        this->actionFunc = RotateCylinder_Idle;
    }else{
        this->dyna.actor.shape.rot.y = this->dyna.actor.shape.rot.y + 0x200;
        this->dyna.actor.world.rot.y = this->dyna.actor.shape.rot.y;
        func_8002F974(&this->dyna.actor, NA_SE_EV_ELEVATOR_MOVE3 - SFX_FLAG);
    }
}

void RotateCylinder_Idle(RotateCylinder* this, PlayState* play) {
    if ((Flags_GetSwitch(play, this->switchFlag)) && (this->backHomeFlag == true)) {
        this->actionFunc = RotateCylinder_Rotate;
    }else if((!Flags_GetSwitch(play, this->switchFlag)) && (this->backHomeFlag == false)){
        this->actionFunc = RotateCylinder_RotateBack;
    }
} 

void RotateCylinder_Destroy(RotateCylinder* this, PlayState* play) {
    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

void RotateCylinder_Update(RotateCylinder* this, PlayState* play) {
    this->actionFunc(this, play);
}


s32 RotateCylinder_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3f* pos, Vec3s* rot, void* thisx) {
    RotateCylinder* this = (void*)thisx;
    
    return 0;
}

void RotateCylinder_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3s* rot, void* thisx) {
    RotateCylinder* this = (void*)thisx;
}

void RotateCylinder_Draw(RotateCylinder* this, PlayState* play) {
    Gfx_DrawDListOpa(play, &gRotateCylinderObj_DlRotateCylinder);
}
