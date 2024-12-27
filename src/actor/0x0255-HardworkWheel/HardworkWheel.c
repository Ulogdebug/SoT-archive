#include "HardworkWheel.h"
#include "object/0x0078-HardworkWheel.h"
#include "state/state.h"

#define FLAGS (ACTOR_FLAG_4 | ACTOR_FLAG_NO_DRAW_CULLING)

void HardworkWheel_Init(HardworkWheel* this, PlayState* play);
void HardworkWheel_Destroy(HardworkWheel* this, PlayState* play);
void HardworkWheel_Update(HardworkWheel* this, PlayState* play);
void HardworkWheel_Draw(HardworkWheel* this, PlayState* play);
s16 HardworkWheel_shouldMove(HardworkWheel* this, PlayState* play);
void HardworkWheel_Moving(HardworkWheel* this, PlayState* play);
void HardworkWheel_MoveJudge(HardworkWheel* this, PlayState* play);
void HardworkWheel_Shake(HardworkWheel* this, PlayState* play);

const ActorInit sHardworkWheel_InitVars = {
    .id = 0x0255,
    .category     = ACTORCAT_BG,
    .flags        = FLAGS,
    .objectId = 0x0078,
    .instanceSize = sizeof(HardworkWheel),
    .init         = (ActorFunc)HardworkWheel_Init,
    .destroy      = (ActorFunc)HardworkWheel_Destroy,
    .update       = (ActorFunc)HardworkWheel_Update,
    .draw         = (ActorFunc)HardworkWheel_Draw
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
};

void HardworkWheel_Init(HardworkWheel* this, PlayState* play) {
    CollisionHeader* colHeader = NULL;
    Actor_ProcessInitChain(&this->dyna.actor, sInitChain);
    DynaPolyActor_Init(&this->dyna, DPM_UNK3);
    CollisionHeader_GetVirtual(gHardworkWheelObj_CollHardworkWheel, &colHeader); 
    this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &this->dyna.actor, colHeader);   
    this->switchFlags = (this->dyna.actor.params >> 8) & 0x3F;
    this->moving = (this->dyna.actor.params) & 0xF;
    this->spawnBlock = (this->dyna.actor.params >> 4) & 0xF;
    this->blockDie = 0;
    this->slackTimer = 400;
    if(this->spawnBlock && !Flags_GetSwitch(play,this->switchFlags + 1)){
        if ((this->blockRot = Actor_Spawn(&play->actorCtx, play, 0x0259, this->dyna.actor.world.pos.x,
                               this->dyna.actor.world.pos.y, this->dyna.actor.world.pos.z, this->dyna.actor.world.rot.x,
                               this->dyna.actor.world.rot.y, this->dyna.actor.world.rot.z, this->switchFlags + 1)) == NULL) {
            Actor_Kill(&this->dyna.actor);
             this->spawnBlock = 0;
        }else{
            this->spawnBlock = 2;
        }
    }else{
         this->spawnBlock = 0;
    }
    if(this->switchFlags != 0 && this->switchFlags != 0x3F){
        if(Flags_GetSwitch(play,this->switchFlags)){
            Flags_UnsetSwitch(play,this->switchFlags);
        }
    }
    if(this->moving){
        this->actionFunc = HardworkWheel_Moving;
    }else{
        this->actionFunc = HardworkWheel_MoveJudge;
    }
}

void HardworkWheel_Moving(HardworkWheel* this, PlayState* play) {
    func_8002F974(&this->dyna.actor, NA_SE_EV_TOGE_STICK_ROLLING - SFX_FLAG);
    this->dyna.actor.shape.rot.x = this->dyna.actor.shape.rot.x + 200;
}

void HardworkWheel_Shake(HardworkWheel* this, PlayState* play){
    s32 quakeIndex;
    s16 shakeTimer = 40;
    quakeIndex = Quake_Add(GET_ACTIVE_CAM(play), 1);
    Quake_SetSpeed(quakeIndex, 80000);
    Quake_SetQuakeValues(quakeIndex, 2, 0, 0, 0);
    Quake_SetCountdown(quakeIndex, shakeTimer);
}

void HardworkWheel_MoveJudge(HardworkWheel* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 counterRequired;
    f32 temp;
    if(this->spawnBlock == 2){
        counterRequired = 2;
    }else {
        counterRequired = 0;
    }
    this->lastRotX = this->dyna.actor.shape.rot.x;
    if(HardworkWheel_shouldMove(this,play)){
        this->dyna.actor.shape.rot.x = this->dyna.actor.shape.rot.x + this->accumulation * 2;
        if(this->dyna.actor.shape.rot.x < this->lastRotX){
            if(this->turnCounter >= 200){
                this->turnCounter = 0;
            }
            if(this->spawnBlock == 2){
                if(this->turnCounter == 1){
                    Message_StartTextbox(play,0x702d,&this->dyna.actor);
                }else if(this->turnCounter == 2){
                    Message_StartTextbox(play,0x0196,&this->dyna.actor);
                }
            }
            this->turnCounter ++;
        }
        if(this->spawnBlock == 2){
            temp = this->blockRot->targetArrowOffset + 2.0f;
            if(this->turnCounter < counterRequired + 3){
                if(temp < 80.0f){
                    this->blockRot->targetArrowOffset = (f32)temp;
                }else{
                   this->blockRot->targetArrowOffset = 80.0f;
                }
            }else{
                if(temp >= 400.0f){
                    this->blockRot->targetArrowOffset = 400.0f;
                }else{
                    this->blockRot->targetArrowOffset = (f32)temp;
                }
            }
        }    
        if(this->accumulation >= 100 && this->turnCounter >= counterRequired){
            if(this->switchFlags != 0 && this->switchFlags != 0x3F){
                if(!Flags_GetSwitch(play,this->switchFlags)){
                    Rumble_Request(this->dyna.actor.xyzDistToPlayerSq, 120, 2, 120);
                    Audio_PlayActorSfx2(&this->dyna.actor, NA_SE_EV_ELEVATOR_STOP);
                    Flags_SetSwitch(play,this->switchFlags);
                    HardworkWheel_Shake(this,play);
                }
            }
        }
    }else {
        if(this->switchFlags != 0 && this->switchFlags != 0x3F){
            if(Flags_GetSwitch(play,this->switchFlags)){
                Rumble_Request(this->dyna.actor.xyzDistToPlayerSq, 120, 2, 120);
                func_8002F974(&this->dyna.actor, NA_SE_EV_METALDOOR_STOP);
                Flags_UnsetSwitch(play,this->switchFlags);
            }
        }
    }
}


s16 HardworkWheel_shouldMove(HardworkWheel* this, PlayState* play) {
    Debug_Text(255,255,0,4,5,"turn Counter = %d",this->turnCounter);
    Player* player = GET_PLAYER(play);
    s16 yawDiff = this->dyna.actor.shape.rot.y - player->actor.shape.rot.y;
    s16 working = 0;
    if((ABS(player->actor.speedXZ) > 0) && (ABS(yawDiff) < 0X4000) && DynaPolyActor_IsPlayerOnTop(&this->dyna)){
        working = 1;
        if(this->accumulation < 200){
            this->accumulation = (s32)player->actor.speedXZ + this->accumulation;
        }
    }
    if(this->spawnBlock == 2){
        if(working){
            if(this->slackTimer < 200){
                this->slackTimer ++;
            }
        }else{
            DECR(this->slackTimer);
            if(this->slackTimer == 0 ){
                ghackSave.gotManuallyShocked = 1;
                //func_8002F698(play, &this->dyna.actor, 3.0f, player->actor.world.rot.y, 0.0f, 3, 4);
                 Message_StartTextbox(play,0x6073,&this->dyna.actor);
                play->damagePlayer(play, -8);
                this->slackTimer = 200;
            }
        }
    }
    if(this->accumulation > 0){
        if(this->accumulation > 100)
            this->accumulation = this->accumulation - 2;
        else{
            this->accumulation--;
        }
    }else{
        if(this->spawnBlock == 2){
            this->blockRot->targetArrowOffset  = 0;
         }
        this->accumulation = 0;
    }
    
    if(this->accumulation > 20){
        if(working){
            func_8002F974(&this->dyna.actor, NA_SE_EV_TOGE_STICK_ROLLING - SFX_FLAG);
        }else{
            func_8002F974(&this->dyna.actor, NA_SE_EV_METALDOOR_SLIDE - SFX_FLAG);
        }
        return 1;
    }
    return 0;
}

void HardworkWheel_Update(HardworkWheel* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    if(this->spawnBlock ==2 ){
        if(Actor_FindNearby(play, &this->dyna.actor, 0x0259, ACTORCAT_BG, 3000.0f) == NULL){ 
            this->spawnBlock = -1;
            this->blockRot = NULL;
        }else{
            this->blockRot->shape.rot.x =  this->dyna.actor.shape.rot.x;
        }
    }
    this->dyna.actor.world.rot.x = this->dyna.actor.shape.rot.x ;
    this->dyna.actor.world.rot.y = this->dyna.actor.shape.rot.y ;
    this->dyna.actor.world.rot.z = this->dyna.actor.shape.rot.z ;
    this->actionFunc(this, play);

}

void HardworkWheel_Destroy(HardworkWheel* this, PlayState* play) {
    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);

}


s32 HardworkWheel_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3f* pos, Vec3s* rot, void* thisx) {
    HardworkWheel* this = (void*)thisx;
    
    return 0;
}

void HardworkWheel_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3s* rot, void* thisx) {
    HardworkWheel* this = (void*)thisx;
}

void HardworkWheel_Draw(HardworkWheel* this, PlayState* play) {
     Gfx_DrawDListOpa(play, gHardworkWheelObj_DlHardworkWheel);
}
