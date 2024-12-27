#include "SFXmanager.h"

void SFXmanager_Init(SFXmanager* this, PlayState* play);
void SFXmanager_Destroy(SFXmanager* this, PlayState* play);
void SFXmanager_Update(SFXmanager* this, PlayState* play);
void SFXmanager_DoNothing(SFXmanager* this, PlayState* play);
void SFXmanager_Draw(SFXmanager* this, PlayState* play);

const ActorInit sSFXmanager_InitVars = {
    .id = 0x0208,
    .category     = ACTORCAT_BG,
    .flags        = (ACTOR_FLAG_4 | ACTOR_FLAG_5),
    .objectId = OBJECT_GAMEPLAY_KEEP,
    .instanceSize = sizeof(SFXmanager),
    .init         = (ActorFunc)SFXmanager_Init,
    .destroy      = (ActorFunc)SFXmanager_Destroy,
    .update       = (ActorFunc)SFXmanager_Update,
    .draw         = (ActorFunc)SFXmanager_Draw
};

static s16 SFXselect[] = {
    SOUND_WORM_VOMIT,   //    0
    SOUND_STEAM,      //    1
    SOUND_TRAIN_RUNNING, // 2
    };

void SFXmanager_Init(SFXmanager* this, PlayState* play) {
    this->switchFlags = (this->actor.params >> 8) & 0xFF; //Variable Form : **00 (hex)
    this->whichSFX  = (this->actor.params & 0xFF); //Variable Form : 00** (hex)
    if(Flags_GetSwitch(play,this->switchFlags) && (this->switchFlags != 0x3F) && (this->switchFlags != 0)){
        Actor_Kill(&this->actor);
        return; 
    }
    this->actionFunc = SFXmanager_DoNothing;
}

void SFXmanager_Destroy(SFXmanager* this, PlayState* play) {
}

void SFXmanager_Update(SFXmanager* this, PlayState* play) {
    this->actionFunc(this, play);
}

void SFXmanager_DoNothing(SFXmanager* this, PlayState* play){
}

s32 SFXmanager_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3f* pos, Vec3s* rot, void* thisx) {
    SFXmanager* this = (void*)thisx;
    
    return 0;
}

void SFXmanager_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3s* rot, void* thisx) {
    SFXmanager* this = (void*)thisx;
}

void SFXmanager_Draw(SFXmanager* this, PlayState* play) {
    if(SFXselect[this->whichSFX]!=SOUND_TRAIN_RUNNING){
        SoundEffect_PlayHeld(SFXselect[this->whichSFX], 0.7f, 0.8f, 1.0f, &this->actor.world.pos, 0.2f, 1000.0f, &this->actor);
    }else{
        SoundEffect_PlayHeld(SFXselect[this->whichSFX], 0.5f, 1.0f, 1.0f, NULL, 0.1f, 20000.0f, &this->actor);
    }
}
