#include "TownGate.h"
#include "object/0x0202-TownGateObj.h"
#define FLAGS (ACTOR_FLAG_NO_DRAW_CULLING | ACTOR_FLAG_NO_UPDATE_CULLING | ACTOR_FLAG_SFX_AT_POS | ACTOR_FLAG_OCARINA_NO_FREEZE)

void TownGate_Init(Actor* thisx, PlayState* play);
void TownGate_Destroy(TownGate* this, PlayState* play);
void TownGate_Update(TownGate* this, PlayState* play);
void TownGate_Draw(TownGate* this, PlayState* play);
void TownGate_SwitchJudge(TownGate* this, PlayState* play);
void TownGate_Rotate(TownGate* this, PlayState* play);
void TownGate_SlowDown(TownGate* this, PlayState* play);
void TownGate_Idle(TownGate* this, PlayState* play);
void TownGate_Down(TownGate* this, PlayState* play);
 void TownGate_Nothing(TownGate* this, PlayState* play);




const ActorInit sTownGate_InitVars = {
    .id = 0x020C,
    .category     = ACTORCAT_BG,
    .flags        = FLAGS,
    .objectId = 0x0202,
    .instanceSize = sizeof(TownGate),
    .init         = (ActorFunc)TownGate_Init,
    .destroy      = (ActorFunc)TownGate_Destroy,
    .update       = (ActorFunc)TownGate_Update,
    .draw         = (ActorFunc)TownGate_Draw
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 15, ICHAIN_STOP),
};

void TownGate_Init(Actor* thisx, PlayState* play) {
    TownGate* this = (TownGate*)thisx;
    Player* player = GET_PLAYER(play);
    if(play->sceneId == 0x3F){
        Actor_ProcessInitChain(&this->dyna.actor, sInitChain);
    }
    CollisionHeader* colHeader = NULL;
    DynaPolyActor_Init(&this->dyna, DPM_PLAYER);
    CollisionHeader_GetVirtual(&gTownGateObj_CollTownGateObj001, &colHeader);
    this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, thisx, colHeader);
    this->riseDis  = (this->dyna.actor.params >> 4 & 0xF) * 50; //Variable Form : 00*0 (hex)
    this->switchFlag = (this->dyna.actor.params >> 8) & 0xFF; //Variable Form : **00 (hex) 
    if ((Flags_GetSwitch(play, this->switchFlag))){
        this->dyna.actor.world.pos.y = this->dyna.actor.world.pos.y + this->riseDis;
        this->backHomeFlag = 0;
        this->actionFunc = TownGate_Nothing;
    }
    else{
        this->backHomeFlag = 1;
        this->actionFunc = TownGate_Idle;
    }

}

void TownGate_Rise(TownGate* this, PlayState* play) {
    if (!(Flags_GetSwitch(play, this->switchFlag))) {
        this->actionFunc = TownGate_Down;
    }
    if (Math_StepToF(&this->dyna.actor.world.pos.y, this->dyna.actor.home.pos.y + this->riseDis, 2.0f)){
        this->backHomeFlag = 0;
        this->actionFunc = TownGate_Idle;
    }
    else{
        func_8002F974(&this->dyna.actor, NA_SE_EV_ELEVATOR_MOVE - SFX_FLAG);
    }
}

void TownGate_Down(TownGate* this, PlayState* play) {
    if (Flags_GetSwitch(play, this->switchFlag)) {
        this->actionFunc = TownGate_Rise;
    }
    if (Math_StepToF(&this->dyna.actor.world.pos.y, this->dyna.actor.home.pos.y, 2.0f)){
        this->backHomeFlag = 1;
        this->actionFunc = TownGate_Idle;
    }else{
        func_8002F974(&this->dyna.actor, NA_SE_EV_ELEVATOR_MOVE - SFX_FLAG);
    }
 }

void TownGate_Idle(TownGate* this, PlayState* play) {
    if ((Flags_GetSwitch(play, this->switchFlag)) && this->backHomeFlag) {
        this->actionFunc = TownGate_Rise;
    }
    else if(!(Flags_GetSwitch(play, this->switchFlag)) && this->backHomeFlag != 1){
        this->actionFunc = TownGate_Down;
    }
}

 void TownGate_Nothing(TownGate* this, PlayState* play) {
}

void TownGate_Destroy(TownGate* this, PlayState* play) {
    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

void TownGate_Update(TownGate* this, PlayState* play) {
    this->actionFunc(this, play);
}


s32 TownGate_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3f* pos, Vec3s* rot, void* thisx) {
    TownGate* this = (void*)thisx;
    
    return 0;
}

void TownGate_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3s* rot, void* thisx) {
    TownGate* this = (void*)thisx;
}

void TownGate_Draw(TownGate* this, PlayState* play) {
    Gfx_DrawDListOpa(play, &gTownGateObj_DlTownGateObj);
}
