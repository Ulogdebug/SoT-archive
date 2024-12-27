#include "SuperFan.h"
#include "object/0x0209-SuperFanObj.h"

void SuperFan_Init(Actor* thisx, PlayState* play);
void SuperFan_Destroy(SuperFan* this, PlayState* play);
void SuperFan_Update(SuperFan* this, PlayState* play);
void SuperFan_Draw(SuperFan* this, PlayState* play);
void SuperFan_CheckTimer(SuperFan* this, PlayState* play);
void SuperFan_ModeCheck(SuperFan* this, PlayState* play) ;
void SuperFan_FullSpeedMode(SuperFan* this, PlayState* play) ;
void SuperFan_SetupFullSpeed(SuperFan* this, PlayState* play) ;
void SuperFan_SetupStop(SuperFan* this, PlayState* play) ;
void SuperFan_Idle(SuperFan* this, PlayState* play) ;
void SuperFan_BlowPlayerAwayHeavy(SuperFan* this, PlayState* play, s16 arg2);
void SuperFan_BlowActorAway(SuperFan* this, PlayState* play, s16 arg2);
void SuperFan_BlowPlayerAwaySoft(SuperFan* this, PlayState* play, s16 arg2) ;

const ActorInit sSuperFan_InitVars = {
    .id = 0x0213,
    .category     = ACTORCAT_BG,
    .flags        = 0x00000030,
    .objectId = 0x0209,
    .instanceSize = sizeof(SuperFan),
    .init         = (ActorFunc)SuperFan_Init,
    .destroy      = (ActorFunc)SuperFan_Destroy,
    .update       = (ActorFunc)SuperFan_Update,
    .draw         = (ActorFunc)SuperFan_Draw
};

void SuperFan_Init(Actor* thisx, PlayState* play) {
    SuperFan* this = (SuperFan*)thisx;
    this->blowActor = NULL;
    u8 isSwitchDrive = 0;
    this->timer = 40;
    this->switchFlag = (this->dyna.actor.params >> 8) & 0xFF; //Variable Form : **00 (hex)  0XFF used to clear "1" in high byte, which is a double insurance
    this->reverse = (this->dyna.actor.params & 0xF); //Variable Form : 000* (hex)
    isSwitchDrive = (this->dyna.actor.params >> 4 & 0xF);  //Variable Form : 00*0 (hex)
    if(isSwitchDrive == 1){
        if(this->reverse == 0){
            if ((Flags_GetSwitch(play, this->switchFlag))) {
                this->backHomeFlag == false;
            }else {
                this->backHomeFlag == true;
            }
        }else{
            if ((Flags_GetSwitch(play, this->switchFlag))) {
                this->backHomeFlag == true;
            }else {
               this->backHomeFlag == false;
            }                
        }
        this->actionFunc = SuperFan_Idle;
    }else{
        this->actionFunc = SuperFan_CheckTimer;
    }
}

void SuperFan_CheckTimer(SuperFan* this, PlayState* play) {
    if (this->timer != 0) {
        this->timer -= 1;
    }
    if (this->timer == 0) {
        this->actionFunc = SuperFan_ModeCheck;
    }
}


void SuperFan_ModeCheck(SuperFan* this, PlayState* play) {
    if (this->timer != 0) { 
        if (Math_ScaledStepToS(&this->dyna.actor.world.rot.z, 0, this->dyna.actor.world.rot.z * 0.03f + 5.0f)) {
            this->timer = 40;
            this->actionFunc = SuperFan_CheckTimer;
        }
    } else {
        if (Math_ScaledStepToS(&this->dyna.actor.world.rot.z, 0x3A00, this->dyna.actor.world.rot.z * 0.03f + 5.0f)) {
            this->timer = 100;
            this->actionFunc = SuperFan_FullSpeedMode;
        }
    }
    this->dyna.actor.shape.rot.z += this->dyna.actor.world.rot.z;
    if (this->dyna.actor.world.rot.z >= 0x1801) {
        func_8002F974(&this->dyna.actor, NA_SE_EV_WIND_TRAP - SFX_FLAG);
    }
    SuperFan_BlowPlayerAwaySoft(this, play, this->dyna.actor.world.rot.z);
    SuperFan_BlowActorAway(this, play, this->dyna.actor.world.rot.z);

}

void SuperFan_FullSpeedMode(SuperFan* this, PlayState* play) {
    if((this->dyna.actor.params >> 4 & 0xF) == 0){
        if (this->timer != 0) {
            this->timer--;
        }
        func_8002F974(&this->dyna.actor, NA_SE_EV_WIND_TRAP - SFX_FLAG);
        if (this->timer == 0) {
            this->timer = 1;
            this->actionFunc = SuperFan_ModeCheck;
        }
    }else{
        func_8002F974(&this->dyna.actor, NA_SE_EV_WIND_TRAP - SFX_FLAG);
        if(this->reverse == 0){
            if (!(Flags_GetSwitch(play, this->switchFlag))) {
                this->actionFunc = SuperFan_SetupStop;
            }
        }else{
            if ((Flags_GetSwitch(play, this->switchFlag))) {
                this->actionFunc = SuperFan_SetupStop;
            }            
        }
    }
    this->dyna.actor.shape.rot.z += this->dyna.actor.world.rot.z;
    SuperFan_BlowPlayerAwayHeavy(this, play, this->dyna.actor.world.rot.z);
    SuperFan_BlowActorAway(this, play, this->dyna.actor.world.rot.z);

}

void SuperFan_BlowPlayerAwayHeavy(SuperFan* this, PlayState* play, s16 arg2) {
    Player* player = GET_PLAYER(play);
    Vec3f playerToFanPos;
    func_8002DBD0(&this->dyna.actor, &playerToFanPos, &player->actor.world.pos);
    if ((fabsf(playerToFanPos.x) < 44.0f) && (playerToFanPos.y > -100.0f) && (playerToFanPos.y < 30.0f) && (playerToFanPos.z < 500.0f) &&
        (GET_PLAYER(play)->currentBoots != PLAYER_BOOTS_IRON)) {
        player->pushedSpeed = ((500.0f - playerToFanPos.z) * 0.06f + 5.0f) * arg2 * (1.0f / 0x3A00) * (2.0f / 3.0f);
        player->currentYaw = this->dyna.actor.shape.rot.y;
        player->targetYaw = this->dyna.actor.shape.rot.y;
        player->pushedYaw = this->dyna.actor.shape.rot.y;
    }
}


void SuperFan_BlowPlayerAwaySoft(SuperFan* this, PlayState* play, s16 arg2) {
    Player* player = GET_PLAYER(play);
    Vec3f playerToFanPos;
    func_8002DBD0(&this->dyna.actor, &playerToFanPos, &player->actor.world.pos);
    if ((fabsf(playerToFanPos.x) < 44.0f) && (playerToFanPos.y > -100.0f) && (playerToFanPos.y < 30.0f) && (playerToFanPos.z < 500.0f) &&
        (GET_PLAYER(play)->currentBoots != PLAYER_BOOTS_IRON)) {
        player->pushedSpeed = ((500.0f - playerToFanPos.z) * 0.05f + 5.0f) * arg2 * (1.0f / 0x3A00) * (1.0f / 10.0f);
        player->pushedYaw = this->dyna.actor.shape.rot.y;
    }
}

void SuperFan_BlowActorAway(SuperFan* this, PlayState* play, s16 arg2) {
    Vec3f blowActorToFanPos;
    if((Actor_FindNearby(play, &this->dyna.actor, -1, ACTORCAT_MISC, 500.0f))!= NULL){
        this->blowActor = Actor_FindNearby(play, &this->dyna.actor, -1, ACTORCAT_PROP, 500.0f);

    }
    if((Actor_FindNearby(play, &this->dyna.actor, -1, ACTORCAT_ENEMY, 500.0f))!= NULL){
        this->blowActor = Actor_FindNearby(play, &this->dyna.actor, -1, ACTORCAT_ENEMY, 500.0f);
    }
    if(this->blowActor != NULL){
        f32 pushSpeed = ((500.0f - blowActorToFanPos.z) * 0.06f + 5.0f) * arg2 * (1.0f / 0x3A00) * (2.0f / 3.0f);
        func_8002DBD0(&this->dyna.actor, &blowActorToFanPos, &this->blowActor->world.pos);
        if ((fabsf(blowActorToFanPos.x) < 80.0f) && (blowActorToFanPos.y > -100.0f) && (blowActorToFanPos.y < 30.0f) && (blowActorToFanPos.z < 500.0f)) {
            this->blowActor->world.rot.y = this->dyna.actor.shape.rot.y;
            if(this->blowActor->speedXZ >= 2.5f ){
                this->blowActor->speedXZ = pushSpeed;
            }else{
                this->blowActor->speedXZ = this->blowActor->speedXZ  + pushSpeed;
            }
        }else{
            this->blowActor = NULL;
        }
    }
}

void SuperFan_SetupFullSpeed(SuperFan* this, PlayState* play) {
    this->backHomeFlag = false;
    if(Math_ScaledStepToS(&this->dyna.actor.world.rot.z, 0x3A00, this->dyna.actor.world.rot.z * 0.03f + 5.0f)) {
        this->actionFunc = SuperFan_FullSpeedMode;
    }else {
        if(this->reverse == 0){
            if (!(Flags_GetSwitch(play, this->switchFlag))) {
                this->actionFunc = SuperFan_SetupStop;
            }
        }else{
            if ((Flags_GetSwitch(play, this->switchFlag))) {
                this->actionFunc = SuperFan_SetupStop;            
            }
        }
    }
    if (this->dyna.actor.world.rot.z >= 0x1801) {
        func_8002F974(&this->dyna.actor, NA_SE_EV_WIND_TRAP - SFX_FLAG);
    }
    this->dyna.actor.shape.rot.z += this->dyna.actor.world.rot.z;
    SuperFan_BlowPlayerAwaySoft(this, play, this->dyna.actor.world.rot.z);
    SuperFan_BlowActorAway(this, play, this->dyna.actor.world.rot.z);

}

void SuperFan_SetupStop(SuperFan* this, PlayState* play) {
    if(Math_ScaledStepToS(&this->dyna.actor.world.rot.z, 0, this->dyna.actor.world.rot.z * 0.03f + 5.0f)) {
        this->backHomeFlag = true;
        this->actionFunc = SuperFan_Idle;
        
    }else {
        if(this->reverse == 0){
            if ((Flags_GetSwitch(play, this->switchFlag))) {
                this->actionFunc = SuperFan_SetupFullSpeed;
            }
        }else{
            if (!(Flags_GetSwitch(play, this->switchFlag))) {
                this->actionFunc = SuperFan_SetupFullSpeed;            
            }
        }        
    }
    if (this->dyna.actor.world.rot.z >= 0x1801) {
        func_8002F974(&this->dyna.actor, NA_SE_EV_WIND_TRAP - SFX_FLAG);
    }
    this->dyna.actor.shape.rot.z += this->dyna.actor.world.rot.z;
    SuperFan_BlowPlayerAwaySoft(this, play, this->dyna.actor.world.rot.z);
    SuperFan_BlowActorAway(this, play, this->dyna.actor.world.rot.z);

}


void SuperFan_Idle(SuperFan* this, PlayState* play) {
    if(this->reverse ==0){
        if ((Flags_GetSwitch(play, this->switchFlag))) {
            this->actionFunc = SuperFan_SetupFullSpeed;
        }
        if (!(Flags_GetSwitch(play, this->switchFlag))) {
            if (this->backHomeFlag == false) {
                this->actionFunc = SuperFan_SetupStop;
            }
        }
    }else{
        if ((Flags_GetSwitch(play, this->switchFlag))) {
            if (this->backHomeFlag == false) {
                this->actionFunc = SuperFan_SetupStop;
            }
        }
        if (!(Flags_GetSwitch(play, this->switchFlag))) {
                this->actionFunc = SuperFan_SetupFullSpeed;
        }
    }
}
 

void SuperFan_Destroy(SuperFan* this, PlayState* play) {
}

void SuperFan_Update(SuperFan* this, PlayState* play) {
    this->actionFunc(this, play);
}

s32 SuperFan_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3f* pos, Vec3s* rot, void* thisx) {
    SuperFan* this = (void*)thisx;
    return 0;
}

void SuperFan_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3s* rot, void* thisx) {
    SuperFan* this = (void*)thisx;
}

void SuperFan_Draw(SuperFan* this, PlayState* play) {
    if((this->dyna.actor.params >> 4 & 0xF) == 0 )
        Gfx_DrawDListOpa(play, &gSuperFanObj_DlSuperFanObj);
    else{
        Gfx_DrawDListOpa(play, &gSuperFanObj_DlCastleSuperFanObj);
    }

    /*void Debug_Text(u8 r, u8 g, u8 b, s32 x, s32 y, char* fmt, ...)*/

}
