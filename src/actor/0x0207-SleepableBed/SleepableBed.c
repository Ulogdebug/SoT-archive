#include "SleepableBed.h"
#include "state/state.h"
void SleepableBed_Init(SleepableBed* this, PlayState* play);
void SleepableBed_Destroy(SleepableBed* this, PlayState* play);
void SleepableBed_Update(SleepableBed* this, PlayState* play);
void SleepableBed_Draw(SleepableBed* this, PlayState* play);
void SleepableBed_Talk(SleepableBed* this, PlayState* play);
void SleepableBed_DoNothing(SleepableBed* this, PlayState* play);

const ActorInit sSleepableBed_InitVars = {
    .id = 0x0207,
    .category     = ACTORCAT_BG,
    .flags        = 0x00000031,
    .objectId = OBJECT_GAMEPLAY_KEEP,
    .instanceSize = sizeof(SleepableBed),
    .init         = (ActorFunc)SleepableBed_Init,
    .destroy      = (ActorFunc)SleepableBed_Destroy,
    .update       = (ActorFunc)SleepableBed_Update,
    .draw         = (ActorFunc)SleepableBed_Draw
};

OvlMessage gIntroduceText[] = {
    {//0
        .type = MSGBOX_TYPE_BLACK,
        .pos = MSGBOX_POS_VARIABLE,
        .txt =
            "Current Y-axis control mode: \x05\x01Vanilla\x05\x00"
    },{//1
        .type = MSGBOX_TYPE_BLACK,
        .pos = MSGBOX_POS_VARIABLE,
        .txt =
            "Current Y-axis control mode: \x05\x01Inverted\x05\x00"
    },
};


OvlMessage checkSelect[] = {
    {
        .type = MSGBOX_TYPE_BLACK,
        .txt =
            "Y-axis control mode option\n\n"
            MSG_TWO_CHOICE MSG_COLOR_REG_0 MSG_INSTANT_ON
            "Change to Vanilla Mode\n"
            "cancel"
            MSG_COLOR_DEFAULT MSG_INSTANT_OFF
    },{
        .type = MSGBOX_TYPE_BLACK,
        .txt =
            "Y-axis control mode option\n\n"
            MSG_TWO_CHOICE MSG_COLOR_REG_0 MSG_INSTANT_ON
            "Change to Inverted Mode\n"
            "cancel"
            MSG_COLOR_DEFAULT MSG_INSTANT_OFF        
    }
};


void SleepableBed_Talk(SleepableBed* this, PlayState* play){
    s8 textOrder = 1;
    s8 decision;
    if(CustomFlag_Get(0x7)){
        textOrder = 0;
    }
    if ((OvlMessage_IsClosed(&this->actor))){
        OvlMessage_Start(NULL, &checkSelect[textOrder]);          
    }
    if((decision = OvlMessage_GetChoice(&this->actor))){
        switch (decision){
            case 1:
                if(textOrder == 0){
                    CustomFlag_Set(0x7,0);
                    //ghackSave.modernAimingControl = 0;
                }else{
                    CustomFlag_Set(0x7,1);
                    //ghackSave.modernAimingControl = 1;
                }
                break;
            case 2:
                break;
        }
        this->actionFunc = SleepableBed_DoNothing;
    }  
}
void SleepableBed_DoNothing(SleepableBed* this, PlayState* play) {

}

void SleepableBed_Init(SleepableBed* this, PlayState* play) {
    OvlMessage_Init(&this->actor, false);
    this->actor.targetMode = 0;
    this->actionFunc = SleepableBed_DoNothing;

}

void SleepableBed_Destroy(SleepableBed* this, PlayState* play) {
}

void SleepableBed_Update(SleepableBed* this, PlayState* play) {
    s8 textOrder = 1;
    if(CustomFlag_Get(0x7)){
        textOrder = 0;
    }
    if((ABS(this->actor.yDistToPlayer) < 40.0f)){
        if(OvlMessage_Prompt(&this->actor,&gIntroduceText[textOrder],50,0) > 0){
            OvlMessage_RegisterColor(0,0,255,0);
            this->actionFunc = SleepableBed_Talk;
        }
        
    }
    Actor_SetFocus(&this->actor, 1.0f);
    this->actionFunc(this, play);
}

s32 SleepableBed_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3f* pos, Vec3s* rot, void* thisx) {
    SleepableBed* this = (void*)thisx;
    
    return 0;
}

void SleepableBed_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3s* rot, void* thisx) {
    SleepableBed* this = (void*)thisx;
}

void SleepableBed_Draw(SleepableBed* this, PlayState* play) {
}
