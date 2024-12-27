#include "Crane.h"
#include "object/0x0200-CraneObj.h"

#define SEARCH_RAIDUS (200.0f)

void Crane_Init(Actor* thisx, PlayState* play);
void Crane_Destroy(Crane* this, PlayState* play);
void Crane_Update(Crane* this, PlayState* play);
void Crane_Draw(Crane* this, PlayState* play);
void Crane_SwitchJudge(Crane* this, PlayState* play);
void Crane_Rotate(Crane* this, PlayState* play);
void Crane_SlowDown(Crane* this, PlayState* play);
void Crane_Idle(Crane* this, PlayState* play);




const ActorInit sCrane_InitVars = {
    .id = 0x020B,
    .category     = ACTORCAT_BG,
    .flags        = 0x00000030,
    .objectId = 0x0200,
    .instanceSize = sizeof(Crane),
    .init         = (ActorFunc)Crane_Init,
    .destroy      = (ActorFunc)Crane_Destroy,
    .update       = (ActorFunc)Crane_Update,
    .draw         = (ActorFunc)Crane_Draw
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 70, ICHAIN_STOP),
};

void Crane_Init(Actor* thisx, PlayState* play) {
    Crane* this = (Crane*)thisx;
    Actor_ProcessInitChain(&this->dyna.actor, sInitChain);
    CollisionHeader* colHeader = NULL;
    DynaPolyActor_Init(&this->dyna, DPM_UNK3);
    
    CollisionHeader_GetVirtual(&gCraneObj_CollCraneObj, &colHeader);
    this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, thisx, colHeader);
    this->moveDis  = (this->dyna.actor.params >> 4 & 0xF) * 100; //Variable Form : 00*0 (hex)
    this->switchFlag = (this->dyna.actor.params >> 8) & 0xFF; //Variable Form : **00 (hex)  0XFF used to clear "1" in high byte, which is a double insurance
    //0xFF BIN:0000 0000  1111 1111
    //         HIGH BYTE| LOW BYTE
    if ((Flags_GetSwitch(play, this->switchFlag))){
        Flags_UnsetSwitch(play, this->switchFlag);
    }
    this->actionFunc = Crane_Idle;

}

void Crane_Rotate(Crane* this, PlayState* play) {
    if (!(Flags_GetSwitch(play, this->switchFlag))) {
        this->actionFunc = Crane_SlowDown;
    }
    if (Math_StepToF(&this->vecFactor, 2.0f, 0.02f)){
        this->vecFactor = 2.0f;
    }
    func_8002F974(&this->dyna.actor, NA_SE_EV_ELEVATOR_MOVE2 - SFX_FLAG);
    this->dyna.actor.world.rot.y = this->dyna.actor.shape.rot.y =  this->dyna.actor.shape.rot.y + this->vecFactor * 25.0f;
}

void Crane_SlowDown(Crane* this, PlayState* play) {
    if (Flags_GetSwitch(play, this->switchFlag)) {
        this->actionFunc = Crane_Rotate;
    }
    if (Math_StepToF(&this->vecFactor, 0.0f, 0.02f)){
        Audio_PlayActorSfx2(&this->dyna.actor, NA_SE_EV_METALDOOR_STOP);
       this->actionFunc = Crane_Idle;
    }else{
        if(this->vecFactor> 0.8f ){
            func_8002F974(&this->dyna.actor, NA_SE_EV_ELEVATOR_MOVE2 - SFX_FLAG);
        }else{
            func_8002F974(&this->dyna.actor, NA_SE_EV_METALDOOR_SLIDE - SFX_FLAG);
        }
        this->dyna.actor.world.rot.y = this->dyna.actor.shape.rot.y =  this->dyna.actor.shape.rot.y + this->vecFactor * 25.0f;
    }
   
 }

void Crane_Idle(Crane* this, PlayState* play) {
    this->vecFactor = 0.0f;
    if ((Flags_GetSwitch(play, this->switchFlag))) {
        this->actionFunc = Crane_Rotate;
    }
}
 

void Crane_Destroy(Crane* this, PlayState* play) {
    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

void Crane_Update(Crane* this, PlayState* play) {
    this->actionFunc(this, play);
    if (DynaPolyActor_IsPlayerOnTop(&this->dyna)) {
        Camera_ChangeSetting(play->cameraPtrs[CAM_ID_MAIN], 0x3A);
    }
    // if(!this->switchIsOnTop){
    //     if(Actor_FindNearby(play, &this->dyna.actor, ACTOR_OBJ_SWITCH, ACTORCAT_SWITCH, SEARCH_RAIDUS) != NULL){ 
    //         this->appendedSwitch = Actor_FindNearby(play, &this->dyna.actor, ACTOR_OBJ_SWITCH, ACTORCAT_SWITCH, SEARCH_RAIDUS);
    //             this->switchIsOnTop = 1;
    //     }
    // }
    // if(this->switchIsOnTop){
    //     Crane_SwitchPosUpdate(this,play);
    // }
}


s32 Crane_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3f* pos, Vec3s* rot, void* thisx) {
    Crane* this = (void*)thisx;
    
    return 0;
}

void Crane_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3s* rot, void* thisx) {
    Crane* this = (void*)thisx;
}

void Crane_Draw(Crane* this, PlayState* play) {
    Gfx_DrawDListOpa(play, &gCraneObj_DlCraneObj);


}
