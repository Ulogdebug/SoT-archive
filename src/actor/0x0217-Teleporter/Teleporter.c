#include "Teleporter.h"
#include "object/0x020B-TeleporterObj.h" 
#define FLAGS (ACTOR_FLAG_FRIENDLY | ACTOR_FLAG_NO_UPDATE_CULLING /*| ACTOR_FLAG_NO_DRAW_CULLING */| ACTOR_FLAG_SFX_CENTERED)
#define PLAYER_IS_TELEPORTABLE (DynaPolyActor_IsPlayerOnTop(&this->dyna) &&/* (player->actor.parent == NULL) &&*/ (player->currentBoots != PLAYER_BOOTS_IRON))
void Teleporter_Init(Teleporter* this, PlayState* play);
void Teleporter_Destroy(Teleporter* this, PlayState* play);
void Teleporter_Update(Teleporter* this, PlayState* play);
void Teleporter_Draw(Teleporter* this, PlayState* play);
void Teleporter_Detect(Teleporter* this, PlayState* play);
void Teleporter_WarpAndFreezePlayer(Teleporter* this, PlayState* play);
void Teleporter_Wait(Teleporter* this, PlayState* play);
Actor* Actor_FindNearbyWithParams(PlayState* play, Actor* refActor, s16 params);
void Teleporter_WaitForRelease(Teleporter* this, PlayState* play);
void Teleporter_ResetPlayerAttributes(Teleporter* this, PlayState* play);
void Teleporter_WarpEnemy(Teleporter* this, PlayState* play);
void Teleporter_SpawnWarpEffect(Teleporter* this, PlayState* play,Vec3f Pos);
void Teleporter_DoNothing(Teleporter* this, PlayState* play);
u8 Teleporter_UpdateTargetPosAndRot(Teleporter* this, PlayState* play);

const ActorInit sTeleporter_InitVars = {
    .id = 0x0217,
    .category     = ACTORCAT_BG,
    .flags        = FLAGS,
    .objectId = 0x020B,
    .instanceSize = sizeof(Teleporter),
    .init         = (ActorFunc)Teleporter_Init,
    .destroy      = (ActorFunc)Teleporter_Destroy,
    .update       = (ActorFunc)Teleporter_Update,
    .draw         = (ActorFunc)Teleporter_Draw
};

void Teleporter_Init(Teleporter* this, PlayState* play) {
    CollisionHeader* colHeader = NULL;
    DynaPolyActor_Init(&this->dyna, DPM_PLAYER);
    CollisionHeader_GetVirtual(&gTeleporterObj_CollTeleporterObj, &colHeader);
    this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &this->dyna.actor, colHeader);
    this->freezePlayerTimer = 0;
    this->targetPos.x = 0;
    this->targetPos.y = 0;
    this->targetPos.z = 0;
    this->TeleporterOrder = (this->dyna.actor.params & 0xF);  //Variable Form : 000* (hex)
    this->switchFlag = (this->dyna.actor.params >> 8) & 0xFF;  //Variable Form : **00 (hex)
    this->WarppedEnemy = NULL;
    //   Lights_PointNoGlowSetInfo(&this->lightInfo, this->actor.world.pos.x, this->actor.world.pos.y + 30.0f,
    //                     this->actor.world.pos.z, 0, 0, 255, -1);
    //this->lightNode = LightContext_InsertLight(play, &play->lightCtx, &this->lightInfo);
    this->actionFunc = Teleporter_Wait;
}

u8 Teleporter_UpdateTargetPosAndRot(Teleporter* this, PlayState* play){
    if((this->targetTeleporter = Actor_FindNearbyWithParams(play, &this->dyna.actor,this->TeleporterOrder)) == NULL){
        return 0;
    }else{
        Math_Vec3f_Copy(&this->targetPos, &this->targetTeleporter->world.pos);
        this->targetPos.y = this->targetTeleporter->world.pos.y + 10.0f;
        Math_Vec3f_Copy(&this->targetRot, &this->targetTeleporter->world.rot);
    }
    return 1;
}

void Teleporter_Wait(Teleporter* this, PlayState* play){
    u8 endPoint = (this->dyna.actor.params >> 4 & 0xF);  //Variable Form : 00*0 (hex) 
    if (!Teleporter_UpdateTargetPosAndRot(this,play)) { 
        Actor_Kill(&this->dyna.actor);
    }else{
        if(endPoint == 1){
            Actor_Kill(&this->dyna.actor);
            return;
        } 
        this->actionFunc = Teleporter_Detect;
    }
}

void Teleporter_SpawnWarpEffect(Teleporter* this, PlayState* play,Vec3f Pos){
    static Color_RGBA8 primColor = { 255, 255, 255, 200 };
    static Color_RGBA8 Y = { 0, 0, 200, 200 };
    Vec3f effAccel = { 0.0f, 0.0f, 0.0f };
    Vec3f effVelocity = { 0.0f, 5.0f, 0.0f };
    Vec3f effectPos;
    s32 i;
    s16 effectYaw;
    s16 effectXZ;
    s64 random;
    effectXZ = 40;
    random = 1.0f;
    for (i = 0; i < 4; i++) {
        if (!((this->timer + (i << 1)) % 4)) {
            effectYaw = (s16)Rand_CenteredFloat(12288.0f) + (i * 0x4000) + 0x2000;
            effectPos.x = Rand_CenteredFloat(5.0f) + Pos.x;
            effectPos.y = (Rand_ZeroOne() * 5.0f) +  Pos.y + 30.0f;
            effectPos.z = Rand_CenteredFloat(5.0f) + Pos.z;         
            EffectSsLightning_Spawn(play, &effectPos, &primColor, &Y, effectXZ, effectYaw, 6, random);
        }
    }

}

Actor* Actor_FindNearbyWithParams(PlayState* play, Actor* refActor, s16 params) {
    Actor* actor = play->actorCtx.actorLists[ACTORCAT_BG].head;
    while (actor != NULL) {
        s16 targetOrder = (actor->params & 0xF);
        if (actor == refActor || ((actor->id != 0x0217) || (targetOrder != params))) {
            actor = actor->next;
        } else {
            // if (Actor_WorldDistXYZToActor(refActor, actor) <= 5000.0f) {
                return actor;
            // } else {
            //     actor = actor->next;
            // }
        }
    }
    return NULL;
}

void Teleporter_WaitForRelease(Teleporter* this, PlayState* play){
    Player* player = GET_PLAYER(play);

    Teleporter_ResetPlayerAttributes(play,this);
}

void Teleporter_Detect(Teleporter* this, PlayState* play){
    Player* player = GET_PLAYER(play);
    if((this->targetTeleporter != NULL) && (!Teleporter_UpdateTargetPosAndRot(this,play))){
        this->targetTeleporter = NULL;
    };
    if((this->dyna.actor.colorFilterTimer != 0)){//when player is teleported to the target teleporter
        if(DynaPolyActor_IsActorOnTop(&this->dyna)){ //don't teleport players if they linger above it
            if(PLAYER_IS_TELEPORTABLE){
                if(this->dyna.actor.colorFilterTimer >= 5){
                    Teleporter_ResetPlayerAttributes(this,play);
                }else{
                    player->stateFlags2 &= ~PLAYER_STATE2_7;
                    this->dyna.actor.colorFilterTimer++;
                }
            }else{
                this->dyna.actor.colorFilterTimer++;
            }
            if(this->dyna.actor.colorFilterTimer >= 5){
                func_8002F974(this, NA_SE_PL_FREEZE);
            }
        }
    }else if(DynaPolyActor_IsActorOnTop(&this->dyna)){ //fresh teleporting
        if(PLAYER_IS_TELEPORTABLE){
            player->actor.freezeTimer = 5;
            this->actionFunc = Teleporter_WarpAndFreezePlayer;
        }else if((this->WarppedEnemy = Actor_FindNearby(play, &this->dyna.actor, -1, ACTORCAT_ENEMY, 50.0f)) != NULL){
            this->WarppedEnemy->freezeTimer = 5;
            this->actionFunc = Teleporter_WarpEnemy;
        }
    }
}

void Teleporter_WarpAndFreezePlayer(Teleporter* this, PlayState* play){
    Player* player = GET_PLAYER(play);
    f32 pitch;
    pitch = CLAMP((this->TeleporterOrder * 0.1f) + 0.7f ,0.7f, 1.5f);
    if(player->actor.freezeTimer ==0){
        Teleporter_ResetPlayerAttributes(this,play);
        if(this->targetTeleporter != NULL){
            this->targetTeleporter->colorFilterTimer = 10;
        }
        player->actor.shape.rot.y = player->actor.world.rot.y =  this->targetRot.y;
        Math_Vec3f_Copy(&player->actor.world.pos, &this->targetPos);
        Math_Vec3f_Copy(&player->actor.home.pos, &this->targetPos);
        SoundEffect_PlayOneshot(SOUND_TELEPORTING, 1.0f, pitch, &this->targetPos, DEFAULT_REVERB, 8000.0f, this);
        Rumble_Request(this->dyna.actor.xzDistToPlayer,0x55 , 0x14, 0x96);
        if(this->switchFlag != 0x0 && this->switchFlag != 0xFF){
            Flags_SetSwitch(play, this->switchFlag);
        }
        this->actionFunc = Teleporter_Detect;
    }
}

void Teleporter_WarpEnemy(Teleporter* this, PlayState* play){
    f32 pitch;
    pitch = CLAMP((this->TeleporterOrder * 0.1f) + 0.6f ,0.6f, 1.5f);
    if(this->WarppedEnemy->freezeTimer == 0){
        if(this->targetTeleporter != NULL){
            this->targetTeleporter->colorFilterTimer = 10;
        }  
        Math_Vec3f_Copy(&this->WarppedEnemy->world.pos, &this->targetPos);
        SoundEffect_PlayOneshot(SOUND_TELEPORTING, 1.0f, pitch, &this->targetPos, DEFAULT_REVERB, 8000.0f, this);
        this->WarppedEnemy = NULL;
        this->actionFunc = Teleporter_Detect;
    }

}


void Teleporter_ResetPlayerAttributes(Teleporter* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    player->unk_6AD = 0; // clear attention mode
    player->actor.speedXZ = 0.0f;
    player->linearVelocity = 0.0f;
    player->actor.parent = NULL;
    //player->stateFlags2 |= (PLAYER_STATE2_RESTRAINED_BY_ENEMY);
    player->stateFlags2 |= PLAYER_STATE2_5 | PLAYER_STATE2_6;
}


void Teleporter_Destroy(Teleporter* this, PlayState* play) {
    //LightContext_RemoveLight(play, &play->lightCtx, this->lightNode);
    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

void Teleporter_Update(Teleporter* this, PlayState* play) {
    u8 brightness = 0;
    s32 lightRadius = -1;
    // if(this->actor.colorFilterTimer == 0){
    //     lightRadius = 200;
    //     brightness = (u8)(Rand_ZeroOne() * 127.0f) + 128;
    // }else{
    //     lightRadius = 200 - (this->actor.colorFilterTimer * 40);
    //     brightness = (u8)(Rand_ZeroOne() * 128.0f);
    // }
    this->actionFunc(this, play);
    //Lights_PointSetColorAndRadius(&this->lightInfo, brightness / 2, brightness / 2, brightness, lightRadius);
            
}

s32 Teleporter_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3f* pos, Vec3s* rot, void* thisx) {
    Teleporter* this = (void*)thisx;
    
    return 0;
}

void Teleporter_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3s* rot, void* thisx) {
    Teleporter* this = (void*)thisx;
}

void Teleporter_Draw(Teleporter* this, PlayState* play) {
    Gfx_DrawDListOpa(play, &gTeleporterObj_DlTeleporterObj);
    // if(this->dyna.actor.colorFilterTimer == 0 && this->dyna.actor.xzDistToPlayer <= 1200.0f && ABS(this->dyna.actor.yDistToPlayer) <= 600.0f){
    //     Teleporter_SpawnWarpEffect(this,play, this->dyna.actor.world.pos);
    //     //func_8002F974(this, NA_SE_PL_SLIP_ICE_LELEL	 - SFX_FLAG);
    // }
    //Debug_Text(255, 0, 0, 1, 1, "! %x", this->dyna.actor.colorFilterTimer);

}
