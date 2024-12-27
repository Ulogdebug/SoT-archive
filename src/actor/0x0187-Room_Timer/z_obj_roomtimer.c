/*
 * File: z_obj_roomtimer.c
 * Overlay: ovl_Obj_Roomtimer
 * Description: Starts Timer 1 with a value specified in params
 */
#include <uLib.h>
#include "z_obj_roomtimer.h"

#define FLAGS ACTOR_FLAG_4

void ObjRoomtimer_Init(Actor* thisx, PlayState* play);
void ObjRoomtimer_Destroy(Actor* thisx, PlayState* play);
void ObjRoomtimer_Update(Actor* thisx, PlayState* play);

void ObjRoomtimer_SetupProceedTimer(ObjRoomtimer* this, PlayState* play);
void ObjRoomtimer_ProceedTimer(ObjRoomtimer* this, PlayState* play);

const ActorInit Obj_Roomtimer_InitVars = {
    ACTOR_OBJ_ROOMTIMER,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(ObjRoomtimer),
    (ActorFunc)ObjRoomtimer_Init,
    (ActorFunc)ObjRoomtimer_Destroy,
    (ActorFunc)ObjRoomtimer_Update,
    (ActorFunc)NULL,
};

void ObjRoomtimer_Init(Actor* thisx, PlayState* play) {
    ObjRoomtimer* this = (ObjRoomtimer*)thisx;
    s16 params = this->actor.params;
    
    this->flipFlag = 0;
    this->switchFlag = (params >> 10) & 0x3F;
    this->actor.params = params & 0x3FF;
    params = this->actor.params;
    if(this->actor.world.rot.z ==1){
        this->flipFlag = 1;
    }
    if (params != 0x3FF) {
        if (params > 600) {
            this->actor.params = 600;
        } else {
            this->actor.params = params;
        }
    }

    this->actionFunc = ObjRoomtimer_SetupProceedTimer;
    
}
        

void ObjRoomtimer_Destroy(Actor* thisx, PlayState* play) {
    ObjRoomtimer* this = (ObjRoomtimer*)thisx;

    if ((this->actor.params != 0x3FF) && (gSaveContext.timer1Value > 0)) {
        gSaveContext.timer1State = 10;
    }
}

void ObjRoomtimer_SetupProceedTimer(ObjRoomtimer* this, PlayState* play) {

    Actor_ChangeCategory(play, &play->actorCtx, &this->actor, ACTORCAT_PROP);
    if((!this->flipFlag && !Flags_GetSwitch(play, this->switchFlag)) || (this->flipFlag && Flags_GetSwitch(play, this->switchFlag))){    
        if (this->actor.params != 0x3FF) {
            func_80088B34(this->actor.params); //Setting up timer, params is the time
        }
        this->actionFunc = ObjRoomtimer_ProceedTimer;
    }
}

void ObjRoomtimer_ProceedTimer(ObjRoomtimer* this, PlayState* play) {

    if (Flags_GetTempClear(play, this->actor.room) && (!this->flipFlag)) {
        if (this->actor.params != 0x3FF) {
            gSaveContext.timer1State = 10;
        }
        Flags_SetClear(play, this->actor.room);
        Flags_SetSwitch(play, this->switchFlag);
        func_80078884(NA_SE_SY_CORRECT_CHIME);
        Actor_Kill(&this->actor);
    } else {
        if ((this->actor.params != 0x3FF) && (gSaveContext.timer1Value == 0)) {
            if(play->sceneId == 0xB){//castle scene
                play->transitionTrigger = TRANS_TRIGGER_START;
                gExitParam.exit.value = 0;
                gExitParam.exit.sceneIndex = 0xB;
                gExitParam.exit.spawnIndex = 0X1;
                gExitParam.exit.__isExit = true;
                play->transitionType = TRANS_TYPE_CIRCLE(TCA_STARBURST, TCC_WHITE, TCS_FAST);
                gSaveContext.nextTransitionType = TRANS_TYPE_CIRCLE(TCA_STARBURST, TCC_WHITE, TCS_FAST);
            }else{
                Audio_PlaySfxGeneral(NA_SE_OC_ABYSS, &gSfxDefaultPos, 4, &gSfxDefaultFreqAndVolScale,
                                     &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
                Play_TriggerVoidOut(play);
                Actor_Kill(&this->actor);
            }
        }
    }
}

void ObjRoomtimer_Update(Actor* thisx, PlayState* play) {
    ObjRoomtimer* this = (ObjRoomtimer*)thisx;
    Debug_Text(255,255,255,1,5," %d ", this->flipFlag);
    this->actionFunc(this, play);
}
