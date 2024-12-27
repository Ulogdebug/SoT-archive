#include "CsGanon.h"
#define FLAGS ACTOR_FLAG_4

void CsGanon_Init(CsGanon* this, PlayState* play);
void CsGanon_Destroy(CsGanon* this, PlayState* play);
void CsGanon_Update(CsGanon* this, PlayState* play);
void CsGanon_Draw(CsGanon* this, PlayState* play);
void CsGano_SetupIdle(CsGanon* this, PlayState* play);
void CsGano_Idle(CsGanon* this, PlayState* play);
void CsGano_SetupSleep(CsGanon* this, PlayState* play);
void CsGano_Sleep(CsGanon* this, PlayState* play);
void CsGano_SetupWakeUp(CsGanon* this, PlayState* play);
void CsGano_SetupPunch(CsGanon* this, PlayState* play);
void CsGano_Punch(CsGanon* this, PlayState* play);
void CsGano_SetupPunchRest(CsGanon* this, PlayState* play);
void CsGano_PunchRest(CsGanon* this, PlayState* play);
void CsGano_WakeUp(CsGanon* this, PlayState* play);

const ActorInit sCsGanon_InitVars = {
    .id = 0x0031,
    .category     = ACTORCAT_NPC,
    .flags        = FLAGS,
    .objectId = 0x0287,
    .instanceSize = sizeof(CsGanon),
    .init         = (ActorFunc)CsGanon_Init,
    .destroy      = (ActorFunc)CsGanon_Destroy,
    .update       = (ActorFunc)CsGanon_Update,
    .draw         = (ActorFunc)CsGanon_Draw
};

void CsGanon_Init(CsGanon* this, PlayState* play) {
    SkelAnime_InitFlex(play, &this->skelAnime, &gCsGanonObj_SkelCsGanonObj, &gCsGanonObj_AnimSleepHeadUp, this->jointTable,
                   this->morphTable, CSGANONOBJ_LIMB_MAX);
    CsGano_SetupIdle(this,play);
}

void CsGanon_Destroy(CsGanon* this, PlayState* play) {
}

void CsGanon_Update(CsGanon* this, PlayState* play) {
    this->actionFunc(this, play);
}

void CsGano_SetupIdle(CsGanon* this, PlayState* play) {
    this->actionFunc = CsGano_Idle;
}

void CsGano_Idle(CsGanon* this, PlayState* play){
    SkelAnime_Update(&this->skelAnime);
    if(Flags_GetSwitch(play, 0x3B)){
        CustomFlag_Set(0x11,1);
        this->actionFunc = CsGano_SetupSleep;
    }
}

void CsGano_SetupSleep(CsGanon* this, PlayState* play) {
    f32 frames = Animation_GetLastFrame(&gCsGanonObj_AnimSleep);
    Animation_Change(&this->skelAnime, &gCsGanonObj_AnimSleep, 1.0f, 0.0f, (s16)frames, ANIMMODE_LOOP, 6.0f);
    this->actionFunc = CsGano_Sleep;
}

void CsGano_Sleep(CsGanon* this, PlayState* play) { 
    SkelAnime_Update(&this->skelAnime);
    if(Flags_GetSwitch(play, 0x3C)){
       CsGano_SetupWakeUp(this,play);
    }
}

void CsGano_SetupWakeUp(CsGanon* this, PlayState* play){
    f32 frames = Animation_GetLastFrame(&gCsGanonObj_AnimWakeUp);
    Animation_Change(&this->skelAnime, &gCsGanonObj_AnimWakeUp, 1.0f, 0.0f, (s16)frames, ANIMMODE_ONCE, 6.0f);
    this->actionFunc = CsGano_WakeUp;
}

void CsGano_WakeUp(CsGanon* this, PlayState* play){
    if(SkelAnime_Update(&this->skelAnime)){
        CsGano_SetupPunch(this,play);
    }
}


void CsGano_SetupPunch(CsGanon* this, PlayState* play){
    f32 frames = Animation_GetLastFrame(&gCsGanonObj_AnimPunch);
    Animation_Change(&this->skelAnime, &gCsGanonObj_AnimPunch, 1.0f, 0.0f, (s16)frames, ANIMMODE_ONCE, 6.0f);
    this->actionFunc = CsGano_Punch;
}

void CsGano_Punch(CsGanon* this, PlayState* play){
    if(SkelAnime_Update(&this->skelAnime)){
        CsGano_SetupPunchRest(this,play);
    }
    if(this->skelAnime.curFrame == 25.0f){
        Audio_PlayActorSfx2(&this->actor, NA_SE_EN_FANTOM_VOICE);
    }
    if(this->skelAnime.curFrame == 35.0f){
        Audio_PlayActorSfx2(&this->actor, NA_SE_EN_FANTOM_SPARK);
    }
}

void CsGano_SetupPunchRest(CsGanon* this, PlayState* play){
    f32 frames = Animation_GetLastFrame(&gCsGanonObj_AnimPunchToSleep);
    Animation_Change(&this->skelAnime, &gCsGanonObj_AnimPunchToSleep, 1.0f, 0.0f, (s16)frames, ANIMMODE_LOOP, 6.0f);
    this->actionFunc = CsGano_PunchRest;
}

void CsGano_PunchRest(CsGanon* this, PlayState* play){
    SkelAnime_Update(&this->skelAnime);
}

s32 CsGanon_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3f* pos, Vec3s* rot, void* thisx) {
    CsGanon* this = (void*)thisx;
    
    return 0;
}

void CsGanon_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3s* rot, void* thisx) {
    CsGanon* this = (void*)thisx;
}

void CsGanon_Draw(CsGanon* this, PlayState* play) {
    if(Flags_GetSwitch(play, 0x3A)){
        if(this->audioFlag == 0){
            Audio_PlayActorSfx2(&this->actor, NA_SE_EN_FANTOM_LAUGH);
            this->audioFlag = 1;
        }
        Gfx_SetupDL_25Opa(play->state.gfxCtx);
        SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->jointTable,CSGANONOBJ_LIMB_MAX,CsGanon_OverrideLimbDraw, CsGanon_PostLimbDraw,
                          this);
    }
}
