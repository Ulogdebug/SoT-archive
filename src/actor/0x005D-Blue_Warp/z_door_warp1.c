#include <uLib.h>
#include "z_door_warp1.h"
#include "assets/objects/object_warp1/object_warp1.h"

#define FLAGS (ACTOR_FLAG_NO_UPDATE_CULLING)

void DoorWarp1_Init(Actor* thisx, PlayState* play);
void DoorWarp1_Destroy(Actor* thisx, PlayState* play);
void DoorWarp1_Update(Actor* thisx, PlayState* play);
void DoorWarp1_Draw(Actor* thisx, PlayState* play);

void DoorWarp1_Destination(DoorWarp1* this, PlayState* play);
void func_8099B020(DoorWarp1* this, PlayState* play);
void DoorWarp1_AwaitClearFlag(DoorWarp1* this, PlayState* play);
void func_8099A3A4(DoorWarp1* this, PlayState* play);
void DoorWarp1_BlueCrystal(DoorWarp1* this, PlayState* play);
void DoorWarp1_PurpleCrystal(DoorWarp1* this, PlayState* play);
void func_80999214(DoorWarp1* this, PlayState* play);
void func_80999348(DoorWarp1* this, PlayState* play);
void func_809995D4(DoorWarp1* this, PlayState* play);
void func_809998A4(DoorWarp1* this, PlayState* play);
void DoorWarp1_ChildWarpIdle(DoorWarp1* this, PlayState* play);
void DoorWarp1_RutoWarpIdle(DoorWarp1* this, PlayState* play);
void DoorWarp1_ChildWarpOut(DoorWarp1* this, PlayState* play);
void func_80999EE0(DoorWarp1* this, PlayState* play);
void func_80999FE4(DoorWarp1* this, PlayState* play);
void DoorWarp1_RutoWarpOut(DoorWarp1* this, PlayState* play);
void DoorWarp1_AdultWarpIdle(DoorWarp1* this, PlayState* play);
void func_8099A508(DoorWarp1* this, PlayState* play);
void DoorWarp1_AdultWarpOut(DoorWarp1* this, PlayState* play);
void DoorWarp1_DoNothing(DoorWarp1* this, PlayState* play);
void DoorWarp1_ChooseInitialAction(DoorWarp1* this, PlayState* play);
void DoorWarp1_FloatPlayer(DoorWarp1* this, PlayState* play);
void DoorWarp1_Wait(DoorWarp1* this, PlayState* play);

const ActorInit Door_Warp1_InitVars = {
    ACTOR_DOOR_WARP1,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_WARP1,
    sizeof(DoorWarp1),
    (ActorFunc)DoorWarp1_Init,
    (ActorFunc)DoorWarp1_Destroy,
    (ActorFunc)DoorWarp1_Update,
    (ActorFunc)DoorWarp1_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 1000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 4000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 800, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 4000, ICHAIN_STOP),
};

static s16 sWarpTimerTarget;

void DoorWarp1_SetupAction(DoorWarp1* this, DoorWarp1ActionFunc actionFunc) {
    this->actionFunc = actionFunc;
}

void DoorWarp1_Init(Actor* thisx, PlayState* play) {
    DoorWarp1* this = (DoorWarp1*)thisx;

    this->unk_1B8 = 0;
    this->unk_1B4 = 0.0f;
    Actor_ProcessInitChain(&this->actor, sInitChain);
    ActorShape_Init(&this->actor.shape, 0.0f, NULL, 0.0f);
    this->currentSceneId = play->sceneId;
    DoorWarp1_Wait(this, play);
}

void DoorWarp1_Wait(DoorWarp1* this, PlayState* play){
    if(this->currentSceneId != 0){
        DoorWarp1_ChooseInitialAction(this, play);
    }
}


void DoorWarp1_Destroy(Actor* thisx, PlayState* play) {
    u8 i;
    DoorWarp1* this = (DoorWarp1*)thisx;

    LightContext_RemoveLight(play, &play->lightCtx, this->upperLight);
    LightContext_RemoveLight(play, &play->lightCtx, this->lowerLight);

    for (i = 0; i < 3; i++) {
        play->envCtx.adjAmbientColor[i] = play->envCtx.adjFogColor[i] = play->envCtx.adjLight1Color[i] = 0;
    }
    //! @bug SkelAnime_Free is not called for crystal variants
}


void DoorWarp1_SetupAdultDungeonWarp(DoorWarp1* this, PlayState* play) {
    SkelAnime_Init(play, &this->skelAnime, &gWarpCrystalSkel, &gWarpCrystalAnim, NULL, NULL, 0);
    Animation_ChangeImpl(&this->skelAnime, &gWarpCrystalAnim, 1.0f, 1.0f, 1.0f, ANIMMODE_ONCE, 40.0f, 1);

    this->scale = 0;
    this->unk_1AE = -140;
    this->unk_1B0 = -80;
    sWarpTimerTarget = 160;
    this->actor.shape.yOffset = -400.0f;
    this->warpTimer = 0;
    this->unk_1BC = 1.0f;
    this->unk_194 = 0.3f;
    this->unk_198 = 0.3f;
    this->lightRayAlpha = 0.0f;
    this->warpAlpha = 0.0f;
    this->crystalAlpha = 0.0f;
    this->unk_19C = 0.0f;

    Lights_PointNoGlowSetInfo(&this->upperLightInfo, this->actor.world.pos.x, this->actor.world.pos.y,
                              this->actor.world.pos.z, 200, 255, 255, 255);
    Lights_PointNoGlowSetInfo(&this->lowerLightInfo, this->actor.world.pos.x, this->actor.world.pos.y,
                              this->actor.world.pos.z, 200, 255, 255, 255);

    DoorWarp1_SetupAction(this, func_8099A3A4);
}

void DoorWarp1_SetupBlueCrystal(DoorWarp1* this, PlayState* play) {
    s16 i;

    SkelAnime_Init(play, &this->skelAnime, &gWarpCrystalSkel, &gWarpCrystalAnim, NULL, NULL, 0);
    Animation_ChangeImpl(&this->skelAnime, &gWarpCrystalAnim, 0, Animation_GetLastFrame(&gWarpCrystalAnim),
                         Animation_GetLastFrame(&gWarpCrystalAnim), ANIMMODE_ONCE, 0.0f, 1);

    this->skelAnime.curFrame = Animation_GetLastFrame(&gWarpCrystalAnim);
    this->scale = 10;
    this->unk_1AE = 120;
    this->unk_1B0 = 230;
    this->warpTimer = 0;
    this->unk_194 = 0.3f;
    this->unk_198 = 0.3f;
    this->lightRayAlpha = 0.0f;
    this->warpAlpha = 0.0f;
    this->crystalAlpha = 0.0f;
    this->unk_19C = 0.0f;
    this->unk_1BC = 1.0f;
    this->actor.shape.yOffset = 800.0f;

    for (i = 0; i < 3; i++) {
        play->envCtx.adjAmbientColor[i] = play->envCtx.adjFogColor[i] = play->envCtx.adjLight1Color[i] = -255;
    }

    play->envCtx.adjFogNear = -500;
    this->warpTimer = 30;
    this->unk_1B8 = 4000;
    DoorWarp1_SetupAction(this, DoorWarp1_BlueCrystal);
}

void DoorWarp1_SetupPurpleCrystal(DoorWarp1* this, PlayState* play) {
    SkelAnime_Init(play, &this->skelAnime, &gWarpCrystalSkel, &gWarpCrystalAnim, NULL, NULL, 0);
    Animation_ChangeImpl(&this->skelAnime, &gWarpCrystalAnim, 0, Animation_GetLastFrame(&gWarpCrystalAnim),
                         Animation_GetLastFrame(&gWarpCrystalAnim), ANIMMODE_ONCE, 0.0f, 1);

    this->skelAnime.curFrame = Animation_GetLastFrame(&gWarpCrystalAnim);
    this->unk_1AE = 120;
    this->unk_1B0 = 230;
    this->warpTimer = 200;
    this->unk_1B8 = 4000;
    this->actor.scale.x = this->actor.scale.y = this->actor.scale.z = 1.0f;
    this->unk_194 = 0.3f;
    this->unk_198 = 0.3f;
    this->lightRayAlpha = 0.0f;
    this->warpAlpha = 0.0f;
    this->crystalAlpha = 0.0f;
    this->unk_19C = 0.0f;
    this->unk_1BC = 1.f;
    this->actor.shape.yOffset = 800.0f;

    if (gSaveContext.entranceIndex != ENTR_TOKINOMA_0) {
        this->actor.scale.x = 0.0499f;
        this->actor.scale.y = 0.077f;
        this->actor.scale.z = 0.09f;
        this->crystalAlpha = 255.0f;
    } else {
        Audio_PlayActorSfx2(&this->actor, NA_SE_EV_SHUT_BY_CRYSTAL);
    }
    DoorWarp1_SetupAction(this, DoorWarp1_PurpleCrystal);
}

void DoorWarp1_SetPlayerPos(DoorWarp1* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    player->actor.velocity.y = 0.0f;
    player->actor.world.pos.x = this->actor.world.pos.x;
    player->actor.world.pos.y = this->actor.world.pos.y + 55.0f;
    player->actor.world.pos.z = this->actor.world.pos.z;
}

void DoorWarp1_BlueCrystal(DoorWarp1* this, PlayState* play) {
    if (this->warpTimer != 0) {
        this->warpTimer--;
    } else {
        DoorWarp1_SetupAction(this, func_80999214);
    }
    DoorWarp1_SetPlayerPos(this, play);
}

void func_80999214(DoorWarp1* this, PlayState* play) {
    s32 temp_f4;
    f32 darkness;
    s16 i;

    Math_SmoothStepToF(&this->crystalAlpha, 255.0f, 0.2f, 5.0f, 0.1f);

    darkness = (f32)(40 - this->warpTimer) / 40.0f;
    darkness = CLAMP_MIN(darkness, 0);

    for (i = 0; i < 3; i++) {
        play->envCtx.adjAmbientColor[i] = play->envCtx.adjFogColor[i] = play->envCtx.adjLight1Color[i] =
            -255.0f * darkness;
    }
    play->envCtx.adjFogNear = -500.0f * darkness;

    this->warpTimer++;
    if (darkness <= 0) {
        DoorWarp1_SetupAction(this, func_80999348);
    }
    this->actor.shape.rot.y += 0x320;
    DoorWarp1_SetPlayerPos(this, play);
}

void func_80999348(DoorWarp1* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    DoorWarp1_SetPlayerPos(this, play);

    if (this->warpTimer == 0) {
        Math_SmoothStepToF(&this->crystalAlpha, 0.0f, 0.1f, 4.0f, 1.0f);
        if (this->crystalAlpha <= 150.0f) {
            player->actor.gravity = -0.1f;
        }
        if (this->crystalAlpha <= 0.0f) {
            DoorWarp1_SetupAction(this, DoorWarp1_FloatPlayer);
        }
    } else {
        this->warpTimer--;
    }
    this->actor.shape.rot.y += 0x320;
}

void DoorWarp1_FloatPlayer(DoorWarp1* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    player->actor.gravity = -0.1f;
}

void DoorWarp1_PurpleCrystal(DoorWarp1* this, PlayState* play) {
    if (this->warpTimer != 0) {
        this->warpTimer--;
        Math_SmoothStepToF(&this->actor.scale.x, 0.0499f, 0.2f, 0.05f, 0.001f);
        Math_SmoothStepToF(&this->actor.scale.y, 0.077f, 0.2f, 0.05f, 0.001f);
        Math_SmoothStepToF(&this->actor.scale.z, 0.09f, 0.2f, 0.05f, 0.001f);
        Math_SmoothStepToF(&this->crystalAlpha, 255.0f, 0.2f, 5.0f, 0.1f);
    }
}

void DoorWarp1_ChooseInitialAction(DoorWarp1* this, PlayState* play) {
    
    if(this->currentSceneId == 0x8 && play->roomCtx.curRoom.num == 0x3){
        if (!CHECK_QUEST_ITEM(QUEST_MEDALLION_SPIRIT) || !Flags_GetSwitch(play, 0x1) || !CHECK_QUEST_ITEM(QUEST_MEDALLION_FIRE) || 
        !CHECK_QUEST_ITEM(QUEST_MEDALLION_FOREST)){/*If you haven't beaten 3 of temples or killed Dark Link*/
            Actor_Kill(this);
            return;
        }
    }
    if( this->currentSceneId == 0X69 && !CustomFlag_Get(0x11)){
            Actor_Kill(this);
            return;
    }
    DoorWarp1_SetupAdultDungeonWarp(this, play);

}





void func_809998A4(DoorWarp1* this, PlayState* play) {
    if (this->lightRayAlpha != 0.0f) {
        Audio_PlayActorSfx2(&this->actor, NA_SE_EV_WARP_HOLE - SFX_FLAG);
    }
    Math_SmoothStepToF(&this->lightRayAlpha, 0.0f, 0.1f, 2.0f, 0.01f);
    Math_SmoothStepToF(&this->warpAlpha, 0.0f, 0.1f, 2.0f, 0.01f);
}

s32 DoorWarp1_PlayerInRange(DoorWarp1* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 ret = false;

    if (fabsf(this->actor.xzDistToPlayer) < 60.0f) {
        if ((player->actor.world.pos.y - 20.0f) < this->actor.world.pos.y) {
            if (this->actor.world.pos.y < (player->actor.world.pos.y + 20.0f)) {
                ret = true;
            }
        }
    }
    return ret;
}


void func_8099A3A4(DoorWarp1* this, PlayState* play) {
    Audio_PlayActorSfx2(&this->actor, NA_SE_EV_WARP_HOLE - SFX_FLAG);
    Math_SmoothStepToF(&this->lightRayAlpha, 255.0f, 0.2f, 2.0f, 0.1f);
    Math_SmoothStepToF(&this->warpAlpha, 255.0f, 0.2f, 2.0f, 0.1f);

    if (this->scale < 10) {
        this->scale += 2;
    }
    if (this->unk_1AE < 120) {
        this->unk_1AE += 4;
    }
    if (this->unk_1B0 < 230) {
        this->unk_1B0 += 4;
    } else {
        DoorWarp1_SetupAction(this, DoorWarp1_AdultWarpIdle);
    }
}

void DoorWarp1_AdultWarpIdle(DoorWarp1* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Audio_PlayActorSfx2(&this->actor, NA_SE_EV_WARP_HOLE - SFX_FLAG);
    if (DoorWarp1_PlayerInRange(this, play)) {

        OnePointCutscene_Init(play, 0x25E8, 999, &this->actor, CAM_ID_MAIN);
        func_8002DF54(play, &this->actor, 10);
        player->unk_450.x = this->actor.world.pos.x;
        player->unk_450.z = this->actor.world.pos.z;
        this->unk_1B2 = 20;
        DoorWarp1_SetupAction(this, func_8099A508);
    }
}

void func_8099A508(DoorWarp1* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (this->unk_1B2 != 0) {
        this->unk_1B2--;
        return;
    }
    Audio_PlaySfxGeneral(NA_SE_EV_LINK_WARP, &player->actor.projectedPos, 4, &gSfxDefaultFreqAndVolScale,
                         &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
    Animation_ChangeImpl(&this->skelAnime, &gWarpCrystalAnim, 1.0f, Animation_GetLastFrame(&gWarpCrystalAnim),
                         Animation_GetLastFrame(&gWarpCrystalAnim), ANIMMODE_ONCE, 40.0f, 1);

    this->unk_1B2 = 0x32;

    DoorWarp1_SetupAction(this, DoorWarp1_AdultWarpOut);
}

void DoorWarp1_AdultWarpOut(DoorWarp1* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    f32 temp_f0_2;
    if (this->unk_1B2 != 0) {
        this->unk_1B2--;
    }
    if (this->unk_1B2 < 31) {
        u32 phi_v0 = (LINK_IS_ADULT) ? 35 : 45;

        if ((player->actor.world.pos.y - this->actor.world.pos.y) <= phi_v0) {
            player->actor.gravity = 0.0139999995f;
        } else {
            player->actor.gravity = 0.0f;
            player->actor.velocity.y = 0.0f;
        }
        if (this->unk_1B2 <= 0) {
            if (this->unk_1B8 < 4000) {
                this->unk_1B8 += 40;
            }
            player->actor.world.rot.y -= this->unk_1B8;
            player->actor.shape.rot.y -= this->unk_1B8;
        }
        Math_SmoothStepToF(&player->actor.world.pos.x, this->actor.world.pos.x, 0.5f, 0.1f, 0.01f);
        Math_SmoothStepToF(&player->actor.world.pos.z, this->actor.world.pos.z, 0.5f, 0.1f, 0.01f);
    }
    this->warpTimer++;

    if (this->warpTimer > sWarpTimerTarget /*&& gSaveContext.nextCutsceneIndex == 0xFFEF*/) {
        switch (this->currentSceneId){
            case 0x15: //Sundown Castle
                if (!CHECK_QUEST_ITEM(QUEST_MEDALLION_FIRE)) {
                      Item_Give(play, ITEM_MEDALLION_FIRE);
                }
                this->sceneIndex = 0x68;
                this->spawnIndex = 0x6;
                break;
            case 0x14: //Factory
                if (!CHECK_QUEST_ITEM(QUEST_MEDALLION_FOREST)) {
                      Item_Give(play, ITEM_MEDALLION_FOREST);           
                } 
                this->sceneIndex = 0x09;
                this->spawnIndex = 0x3;     
                break;
            case 0x1E:
                if (play->sceneId == 0x1E) {/*Ganon's Tomb Boss room*/
                    if (!CHECK_QUEST_ITEM(QUEST_MEDALLION_SPIRIT)) {
                        Item_Give(play, ITEM_MEDALLION_SPIRIT);
                        this->sceneIndex = 0x67;
                        this->spawnIndex = 0x5;
                    }         
                }
                break;
            case 0x08:
                if(play->roomCtx.curRoom.num == 0x11){
                    this->sceneIndex = 0x25;
                    this->spawnIndex = 0x2; 
                }else{ /*Abyss warp room*/
                    if (!CHECK_QUEST_ITEM(QUEST_MEDALLION_SPIRIT)){
                        this->sceneIndex = 0x08; 
                        this->spawnIndex = 0x0;         
                    }else{
                        this->sceneIndex = 0x07; //Go to Death Temple
                        this->spawnIndex = 0x0;         
                    }       
                }
                break;
            case 0x18: //Death Temple Boss Room
                if (!CHECK_QUEST_ITEM(QUEST_MEDALLION_SHADOW)) {
                    Item_Give(play, ITEM_MEDALLION_SHADOW);
                }
                this->sceneIndex = 0x36; //ENDING
                this->spawnIndex = 0x3; 
                break;
            case 0x69: //Death Temple
                this->sceneIndex = 0x18; 
                this->spawnIndex = 0x0; 
                break;
            default:
                this->sceneIndex = 0x36; 
                this->spawnIndex = 0x2;  
                break;
        }
        gSaveContext.entranceIndex = 0x8000;
        gExitParam.nextEntranceIndex = 0x8000;
        gExitParam.exit = (NewExit) {
            .sceneIndex = this->sceneIndex,
            .spawnIndex = this->spawnIndex,
            .headerIndex = 0x0,
            .fadeIn = 0x2C,
            .fadeOut = 0x2C,
        };
        play->transitionTrigger = TRANS_TRIGGER_START;
    }
    if (this->warpTimer >= 141) {
        f32 screenFillAlpha;

        play->envCtx.fillScreen = true;
        screenFillAlpha = (f32)(this->warpTimer - 140) / 20.0f;

        if (screenFillAlpha > 1.0f) {
            screenFillAlpha = 1.0f;
        }
        play->envCtx.screenFillColor[0] = 160;
        play->envCtx.screenFillColor[1] = 160;
        play->envCtx.screenFillColor[2] = 160;
        play->envCtx.screenFillColor[3] = (u32)(255.0f * screenFillAlpha);

        osSyncPrintf("\nparcent=[%f]", screenFillAlpha);
    }
    Lights_PointNoGlowSetInfo(&this->upperLightInfo, (s16)player->actor.world.pos.x + 10.0f,
                              (s16)player->actor.world.pos.y + 10.0f, (s16)player->actor.world.pos.z + 10.0f, 235, 255,
                              255, 255);
    Lights_PointNoGlowSetInfo(&this->lowerLightInfo, (s16)player->actor.world.pos.x - 10.0f,
                              (s16)player->actor.world.pos.y - 10.0f, (s16)player->actor.world.pos.z - 10.0f, 235, 255,
                              255, 255);

    Math_SmoothStepToF(&this->actor.shape.yOffset, 800.0f, 0.5f, 15.0f, 0.1f);
    this->actor.shape.rot.y += 0x320;

    Math_SmoothStepToF(&this->unk_1BC, 1.13f, 0.2f, 0.1f, 0.01f);
    Math_StepToF(&this->unk_194, 2.0f, 0.003f);
    Math_StepToF(&this->unk_198, 10.0f, 0.006f);
    Math_SmoothStepToF(&this->lightRayAlpha, 0.0f, 0.2f, 3.0f, 0.01f);
    Math_SmoothStepToF(&this->warpAlpha, 0.0f, 0.2f, 2.0f, 0.01f);
    Math_SmoothStepToF(&this->crystalAlpha, 255.0f, 0.1f, 1.0f, 0.01f);

    temp_f0_2 = 1.0f - (f32)(sWarpTimerTarget - this->warpTimer) / (sWarpTimerTarget - (sWarpTimerTarget - 100));
    if (temp_f0_2 > 0.0f) {
        s16 i;

        for (i = 0; i < 3; i++) {
            play->envCtx.adjAmbientColor[i] = play->envCtx.adjFogColor[i] = play->envCtx.adjLight1Color[i] =
                -255.0f * temp_f0_2;
        }

        play->envCtx.adjFogNear = -500.0f * temp_f0_2;
        if (play->envCtx.adjFogNear < -300) {
            play->roomCtx.curRoom.segment = NULL;
        }
    }
}

void DoorWarp1_DoNothing(DoorWarp1* this, PlayState* play) {
}

void func_8099B020(DoorWarp1* this, PlayState* play) {
    Math_SmoothStepToF(&this->lightRayAlpha, 128.0f, 0.2f, 2.0f, 0.1f);
    Math_SmoothStepToF(&this->warpAlpha, 128.0f, 0.2f, 2.0f, 0.1f);

    if (this->lightRayAlpha >= 128.0f) {
        Math_StepToF(&this->unk_194, 2.0f, 0.01f);
        Math_StepToF(&this->unk_198, 10.0f, 0.02f);
    }
    Audio_PlayActorSfx2(&this->actor, NA_SE_EV_WARP_HOLE - SFX_FLAG);
}

void DoorWarp1_Update(Actor* thisx, PlayState* play) {
    DoorWarp1* this = (DoorWarp1*)thisx;
    this->currentSceneId = play->sceneId; //((this->actor.params) & 0xFF);
    Debug_Text(255,0,0,1,5,"omg  scene ID : %x",this->currentSceneId);
    this->actionFunc(this, play);
    Actor_SetScale(&this->actor, this->scale / 100.0f);
    
}

void DoorWarp1_DrawBlueCrystal(DoorWarp1* this, PlayState* play) {
    s32 pad;

    OPEN_DISPS(play->state.gfxCtx, "../z_door_warp1.c", 2078);

    Gfx_SetupDL_25Xlu(play->state.gfxCtx);

    gDPSetPrimColor(POLY_XLU_DISP++, 0xFF, 0xFF, 200, 255, 255, (u8)this->crystalAlpha);
    gDPSetEnvColor(POLY_XLU_DISP++, 0, 100, 255, (u8)this->crystalAlpha);

    POLY_XLU_DISP = SkelAnime_Draw(play, this->skelAnime.skeleton, this->skelAnime.jointTable, NULL, NULL, &this->actor,
                                   POLY_XLU_DISP);

    CLOSE_DISPS(play->state.gfxCtx, "../z_door_warp1.c", 2098);

    SkelAnime_Update(&this->skelAnime);
}



void DoorWarp1_DrawWarp(DoorWarp1* this, PlayState* play) {
    s32 pad;
    u32 pad1;
    u32 spEC = play->state.frames * 10;
    f32 spE8 = (this->unk_194 >= 1.0f) ? 0.0f : 1.0f - this->unk_194;
    f32 spE4 = (this->unk_198 >= 1.0f) ? 0.0f : 1.0f - this->unk_198;
    f32 xzScale;
    f32 temp_f0;

    OPEN_DISPS(play->state.gfxCtx, "../z_door_warp1.c", 2173);

    temp_f0 = 1.0f - (2.0f - this->unk_194) / 1.7f;
    this->unk_19C += (s16)(temp_f0 * 15.0f);
    Gfx_SetupDL_25Xlu(play->state.gfxCtx);


    gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x80, 255 * temp_f0, 255, 255, (u8)this->warpAlpha);
    gDPSetEnvColor(POLY_XLU_DISP++, 0, 255 * temp_f0, 255, 255);

    gDPSetColorDither(POLY_XLU_DISP++, G_CD_DISABLE);
    gDPSetColorDither(POLY_XLU_DISP++, G_AD_NOTPATTERN | G_CD_MAGICSQ);

    Matrix_Translate(this->actor.world.pos.x, this->actor.world.pos.y + 1.0f, this->actor.world.pos.z, MTXMODE_NEW);
    gSPSegment(POLY_XLU_DISP++, 0x0A, Matrix_NewMtx(play->state.gfxCtx, "../z_door_warp1.c", 2247));
    Matrix_Push();

    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(play->state.gfxCtx, G_TX_RENDERTILE, spEC & 0xFF,
                                -((s16)(this->unk_19C + this->unk_19C) & 511), 0x100, 0x100, 1, spEC & 0xFF,
                                -((s16)(this->unk_19C + this->unk_19C) & 511), 0x100, 0x100));

    Matrix_Translate(0.0f, this->unk_194 * 230.0f, 0.0f, MTXMODE_APPLY);
    xzScale = (((f32)this->unk_1AE * spE8) / 100.0f) + 1.0f;
    Matrix_Scale(xzScale, 1.0f, xzScale, MTXMODE_APPLY);
    gSPSegment(POLY_XLU_DISP++, 0x09, Matrix_NewMtx(play->state.gfxCtx, "../z_door_warp1.c", 2267));
    gSPDisplayList(POLY_XLU_DISP++, gWarpPortalDL);
    Matrix_Pop();

    if (this->lightRayAlpha > 0.0f) {

        gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x80, 255 * temp_f0, 255, 255, (u8)this->lightRayAlpha);
        gDPSetEnvColor(POLY_XLU_DISP++, 0, 255 * temp_f0, 255, 255);
        
        spEC *= 2;

        gSPSegment(POLY_XLU_DISP++, 0x08,
                   Gfx_TwoTexScroll(play->state.gfxCtx, G_TX_RENDERTILE, spEC & 0xFF, -((s16)this->unk_19C & 511),
                                    0x100, 0x100, 1, spEC & 0xFF, -((s16)this->unk_19C & 511), 0x100, 0x100));

        Matrix_Translate(0.0f, this->unk_198 * 60.0f, 0.0f, MTXMODE_APPLY);

        xzScale = (((f32)this->unk_1B0 * spE4) / 100.0f) + 1.0f;
        Matrix_Scale(xzScale, 1.0f, xzScale, MTXMODE_APPLY);

        gSPSegment(POLY_XLU_DISP++, 0x09, Matrix_NewMtx(play->state.gfxCtx, "../z_door_warp1.c", 2336));
        gSPDisplayList(POLY_XLU_DISP++, gWarpPortalDL);
    }

    CLOSE_DISPS(play->state.gfxCtx, "../z_door_warp1.c", 2340);
}

void DoorWarp1_Draw(Actor* thisx, PlayState* play) {
    DoorWarp1* this = (DoorWarp1*)thisx;
    DoorWarp1_DrawBlueCrystal(this, play);
    DoorWarp1_DrawWarp(this, play);
     

}
