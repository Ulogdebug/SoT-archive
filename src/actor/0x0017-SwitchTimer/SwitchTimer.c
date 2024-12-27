#include <uLib.h>
#include "SwitchTimer.h"

#define FLAGS (ACTOR_FLAG_NO_UPDATE_CULLING | ACTOR_FLAG_SFX_CENTERED | ACTOR_FLAG_SFX_TIMER)
#define SECONDS_TO_FRAMES(sec) (sec * (60 / R_UPDATE_RATE))

void SwitchTimer_Init(SwitchTimer* this, PlayState* play);
void SwitchTimer_Destroy(SwitchTimer* this, PlayState* play);
void SwitchTimer_Update(SwitchTimer* this, PlayState* play);
void SwitchTimer_Draw(SwitchTimer* this, PlayState* play);
void SwitchTimer_WaitForSetSwitch(SwitchTimer* this, PlayState* play);
void SwitchTimer_WaitForUnsetSwitch(SwitchTimer* this, PlayState* play);
void SwitchTimer_ProcessTimer(SwitchTimer* this, PlayState* play);

 
const ActorInit sSwitchTimer_InitVars = {
    .id = 0x0017,
    .category     = ACTORCAT_BG,
    .flags        = FLAGS,
    .objectId = OBJECT_GAMEPLAY_KEEP,
    .instanceSize = sizeof(SwitchTimer),
    .init         = (ActorFunc)SwitchTimer_Init,
    .destroy      = (ActorFunc)SwitchTimer_Destroy,
    .update       = (ActorFunc)SwitchTimer_Update,
    .draw         = (ActorFunc)SwitchTimer_Draw
};

void SwitchTimer_Init(SwitchTimer* this, PlayState* play) {
    this->preTimerDuration = CLAMP_MAX(this->actor.home.rot.x, 600); //10 mins at maxiunm
    this->preTimerDuration = this->timer = SECONDS_TO_FRAMES(this->preTimerDuration);
    this->switchFlag = ((this->actor.params >> 8) & 0xFF); //**00;
    this->reverse = (this->actor.params >> 4 & 0xF); //00*0.
    if(!this->reverse){
        this->actionFunc = SwitchTimer_WaitForSetSwitch;
    }else{
        this->actionFunc = SwitchTimer_WaitForUnsetSwitch;
    }
}

void SwitchTimer_WaitForSetSwitch(SwitchTimer* this, PlayState* play) {
    if (Flags_GetSwitch(play, this->switchFlag)) {
        this->actionFunc = SwitchTimer_ProcessTimer;
    }
}

void SwitchTimer_WaitForUnsetSwitch(SwitchTimer* this, PlayState* play) {
    if (!Flags_GetSwitch(play, this->switchFlag)) {
        this->actionFunc = SwitchTimer_ProcessTimer;
    }
}

void SwitchTimer_SetupWaitforSwitch(SwitchTimer* this, PlayState* play) {
    this->timer = this->preTimerDuration;
    if(!this->reverse){
        Flags_UnsetSwitch(play, this->switchFlag);
        this->actionFunc = SwitchTimer_WaitForSetSwitch;
    }else{
        Flags_SetSwitch(play, this->switchFlag);
        this->actionFunc = SwitchTimer_WaitForUnsetSwitch;
    }
}


void SwitchTimer_ProcessTimer(SwitchTimer* this, PlayState* play) {
    if(this->preTimerDuration != 0){
        func_8002F994(&this->actor, this->timer);
        if (DECR(this->timer) == 0) {
            SwitchTimer_SetupWaitforSwitch(this,play);
            return;
        }
    }else{
        if(!this->reverse){
            Flags_UnsetSwitch(play, this->switchFlag);
        }else{
            Flags_SetSwitch(play, this->switchFlag);
        }
        Actor_Kill(&this->actor);
    }
}

void SwitchTimer_Destroy(SwitchTimer* this, PlayState* play) {
    if(this->preTimerDuration != 0){
        if(!this->reverse){
            Flags_UnsetSwitch(play, this->switchFlag);
        }else{
            Flags_SetSwitch(play, this->switchFlag);
        }
    }
}

void SwitchTimer_Update(SwitchTimer* this, PlayState* play) {
    Debug_Text(255,255,255,3,3,"timer = %d", this->timer);
    this->actionFunc(this, play);
}

s32 SwitchTimer_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3f* pos, Vec3s* rot, void* thisx) {
    SwitchTimer* this = (void*)thisx;
    
    return 0;
}

void SwitchTimer_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3s* rot, void* thisx) {
    SwitchTimer* this = (void*)thisx;
}

void SwitchTimer_Draw(SwitchTimer* this, PlayState* play) {
}
