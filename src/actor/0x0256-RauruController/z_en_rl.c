/*
 * File: z_en_rl.c
 * Overlay: En_Rl
 * Description: Cutscene Switch Trigger
 */

#include "z_en_rl.h"
#include <uLib.h>

#define FLAGS ACTOR_FLAG_4

void EnRl_Init(Actor* thisx, PlayState* play);
void EnRl_Destroy(Actor* thisx, PlayState* play);
void EnRl_Update(Actor* thisx, PlayState* play);
void EnRl_GetCutsceneCMD(EnRl* this, PlayState* play);
void EnRl_UnsetSwitchFlag(EnRl* this, s16 flag);
void EnRl_SetFlag(EnRl* this, s16 flag);
u8 EnRl_IfGetFlag(EnRl* this, s16 flag);

const ActorInit En_Rl_InitVars = {
    .id           = 0x0256,
    .category     = ACTORCAT_NPC,
    .flags        = FLAGS,
    .objectId     = OBJECT_GAMEPLAY_KEEP,
    .instanceSize = sizeof(EnRl),
    .init         = (ActorFunc)EnRl_Init,
    .destroy      = (ActorFunc)EnRl_Destroy,
    .update       = (ActorFunc)EnRl_Update,
    .draw         = NULL
};

void EnRl_Init(Actor* thisx, PlayState* play) {
    EnRl* this = (EnRl*)thisx;
    this->type = this->actor.home.rot.x; //1 chest, 2 collectible, 3 global, the others are switch
    this->flags = (this->actor.params >> 8) & 0xFF;
    this->actionFunc = EnRl_GetCutsceneCMD;
}

void EnRl_UnsetSwitchFlag(EnRl* this, s16 flag) {
    PlayState* play = Effect_GetPlayState();
    if (this->type > 3 && this->type < 1) {
        Flags_UnsetSwitch(play, flag);
    }
}

u8 EnRl_IfGetFlag(EnRl* this, s16 flag) {
    PlayState* play = Effect_GetPlayState();

    switch (this->type) {
        case 1 : //CHEST
             return Flags_GetTreasure(play, flag)? 1 : 0;
        case 2 : //COLLECTIBLE
             return Flags_GetCollectible(play, flag)? 1 : 0;
        case 3 : //GLOBAL
             return Flags_GetEventChkInf(flag)? 1 : 0;
        case 4 : //Custom Flag
             return CustomFlag_Get(flag)? 1 : 0;    
        default:
            return Flags_GetSwitch(play, flag)? 1 : 0;
    }
    return false;
}

void EnRl_SetFlag(EnRl* this, s16 flag) {
    PlayState* play = Effect_GetPlayState();
    switch (this->type) {
        case 1 : //CHEST
             Flags_SetTreasure(play, flag);
             break;
        case 2 : //COLLECTIBLE
             Flags_SetCollectible(play, flag);
             break;
        case 3 : //GLOBAL
             Flags_SetEventChkInf(flag);
             break;
        case 4 : //Custom Flag
            CustomFlag_Set(flag,1);  
        default:
             Flags_SetSwitch(play, flag);
             break;
    }
}

void EnRl_Destroy(Actor* thisx, PlayState* play) {
    EnRl* this = (EnRl*)thisx;
}

void EnRl_DoNothing(Actor* thisx, PlayState* play) {
    EnRl* this = (EnRl*)thisx;
    Actor_Kill(&this->actor);
}

void EnRl_GetCutsceneCMD(EnRl* this, PlayState* play) {
    CsCmdActorAction* npcAction = play->csCtx.npcActions[4]; //SO slot number -1, '0x1E - ???(Slot 5)'
    CsCmdActorAction* flipNpcAction = play->csCtx.npcActions[5]; //SO slot number -1, '0x54 - ???(Slot 6)'

    if ((npcAction != NULL)){ //npcAction is the order of animation
        if(npcAction->action != 0xFE){
            if(this->flags == 0){
                this->flags = npcAction->action;
            }
            if((EnRl_IfGetFlag(this,npcAction->action) == 0) && (this->flags == npcAction->action) ){
                EnRl_SetFlag(this, this->flags);
                Actor_Kill(&this->actor);
            }else{
                npcAction->action = 0xFE;
            }
        }
    }
    if ((flipNpcAction != NULL)){
        if(flipNpcAction->action != 0xFE){
            if(this->flags == 0){
                this->flags = flipNpcAction->action;
            }
            if((EnRl_IfGetFlag(this,flipNpcAction->action) == 1) && (this->flags == flipNpcAction->action)){
                EnRl_UnsetSwitchFlag(this, this->flags);
                Actor_Kill(&this->actor);
            }else{
                flipNpcAction->action = 0xFE;
            }
        }
    }
}

void EnRl_Update(Actor* thisx, PlayState* play) {
    EnRl* this = (EnRl*)thisx;
    Debug_Text(255,255,255,4,5,"wait for flags");
    this->actionFunc(this, play);
}

