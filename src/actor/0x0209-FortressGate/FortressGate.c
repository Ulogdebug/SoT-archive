#include "FortressGate.h"
#include "object/0x019D-FortressGateObj.h"
#define FLAGS (ACTOR_FLAG_NO_DRAW_CULLING | ACTOR_FLAG_NO_UPDATE_CULLING | ACTOR_FLAG_SFX_AT_POS | ACTOR_FLAG_OCARINA_NO_FREEZE)
void FortressGate_Init(FortressGate* this, PlayState* play);
void FortressGate_Destroy(FortressGate* this, PlayState* play);
void FortressGate_Update(FortressGate* this, PlayState* play);
void FortressGate_Draw(FortressGate* this, PlayState* play);
void FortressGate_SwitchJudge(FortressGate* this, PlayState* play);
void FortressGate_Rotate(FortressGate* this, PlayState* play);
void FortressGate_SlowDown(FortressGate* this, PlayState* play);
void FortressGate_Idle(FortressGate* this, PlayState* play);
void FortressGate_Down(FortressGate* this, PlayState* play);

const ActorInit sFortressGate_InitVars = {
    .id = 0x0209,
    .category     = ACTORCAT_BG,
    .flags        = FLAGS,
    .objectId = 0x019D,
    .instanceSize = sizeof(FortressGate),
    .init         = (ActorFunc)FortressGate_Init,
    .destroy      = (ActorFunc)FortressGate_Destroy,
    .update       = (ActorFunc)FortressGate_Update,
    .draw         = (ActorFunc)FortressGate_Draw
};

void FortressGate_Init(FortressGate* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    CollisionHeader* colHeader = NULL;
    DynaPolyActor_Init(&this->dyna, DPM_PLAYER);
    CollisionHeader_GetVirtual(&gFortressGateObj_CollGate, &colHeader);
    this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, this, colHeader);
    this->riseDis  = (this->dyna.actor.params >> 4 & 0xF) * 50; //Variable Form : 00*0 (hex)
    this->switchFlag = (this->dyna.actor.params >> 8) & 0xFF; //Variable Form : **00 (hex) 
    if ((Flags_GetSwitch(play, this->switchFlag))){
        this->dyna.actor.world.pos.y = this->dyna.actor.world.pos.y + this->riseDis;
        this->backHomeFlag = 0;
    }
    else{
        this->backHomeFlag = 1;
        this->actionFunc = FortressGate_Idle;
    }
}

void FortressGate_Destroy(FortressGate* this, PlayState* play) {
    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

void FortressGate_Update(FortressGate* this, PlayState* play) {
    this->actionFunc(this, play);
}

void FortressGate_Rise(FortressGate* this, PlayState* play) {
    if (!(Flags_GetSwitch(play, this->switchFlag))) {
        this->actionFunc = FortressGate_Down;
    }
    if (Math_StepToF(&this->dyna.actor.world.pos.y, this->dyna.actor.home.pos.y + this->riseDis, 2.0f)){
        this->backHomeFlag = 0;
        this->actionFunc = FortressGate_Idle;
    }
    else{
        func_8002F974(&this->dyna.actor, NA_SE_EV_ELEVATOR_MOVE - SFX_FLAG);
    }
}

void FortressGate_Down(FortressGate* this, PlayState* play) {
    if (Flags_GetSwitch(play, this->switchFlag)) {
        this->actionFunc = FortressGate_Rise;
    }
    if (Math_StepToF(&this->dyna.actor.world.pos.y, this->dyna.actor.home.pos.y, 2.0f)){
        this->backHomeFlag = 1;
        this->actionFunc = FortressGate_Idle;
    }else{
        func_8002F974(&this->dyna.actor, NA_SE_EV_ELEVATOR_MOVE - SFX_FLAG);
    }
 }

void FortressGate_Idle(FortressGate* this, PlayState* play) {
    if ((Flags_GetSwitch(play, this->switchFlag)) && this->backHomeFlag) {
        
        this->actionFunc = FortressGate_Rise;
    }
    else if(!(Flags_GetSwitch(play, this->switchFlag)) && this->backHomeFlag != 1){
        this->actionFunc = FortressGate_Down;
    }
}

s32 FortressGate_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3f* pos, Vec3s* rot, void* thisx) {
    FortressGate* this = (void*)thisx;
    
    return 0;
}

void FortressGate_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3s* rot, void* thisx) {
    FortressGate* this = (void*)thisx;
}

void FortressGate_Draw(FortressGate* this, PlayState* play) {
    Gfx_DrawDListOpa(play, &gFortressGateObj_DlGate);
}
