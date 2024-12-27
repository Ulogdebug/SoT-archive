/*
 * File: z_en_rl.c
 * Overlay: En_Rl
 * Description: Rauru
 */

#include "z_en_rl.h"
#include "vt.h"
#include "assets/objects/object_rl/object_rl.h"

#define FLAGS ACTOR_FLAG_4

void EnRl_Init(Actor* thisx, PlayState* play);
void EnRl_Destroy(Actor* thisx, PlayState* play);
void EnRl_Update(Actor* thisx, PlayState* play);
void EnRl_Draw(Actor* thisx, PlayState* play);

void EnRl_SetupAction1(EnRl* this, PlayState* play);
void EnRl_SetupAction2(EnRl* this, PlayState* play);
void EnRl_SetupAction3(EnRl* this, PlayState* play);
void EnRl_SetupGiveLightMedallion(EnRl* this, PlayState* play);
void func_80AE7C64(EnRl* this, PlayState* play);
void func_80AE7C94(EnRl* this, PlayState* play);
void func_80AE7CE8(EnRl* this, PlayState* play);
void func_80AE7D40(EnRl* this, PlayState* play);
void EnRl_DrawNothing(EnRl* this, PlayState* play);
void EnRl_DrawOpa(EnRl* this, PlayState* play);
void EnRl_DrawXlu(EnRl* this, PlayState* play);

static void* eyesTextures[] = { object_rl_Tex_003620, object_rl_Tex_003960, object_rl_Tex_003B60 };

void EnRl_Destroy(Actor* thisx, PlayState* play) {
    EnRl* this = (EnRl*)thisx;

    SkelAnime_Free(&this->skelAnime, play);
}

void EnRl_UpdateBlinking(EnRl* this) {
    s32 pad[3];
    s16* timer = &this->timer;
    s16* eyeTextureIndex = &this->eyeTextureIndex;

    if (DECR(*timer) == 0) {
        *timer = Rand_S16Offset(60, 60);
    }

    *eyeTextureIndex = *timer;
    if (*eyeTextureIndex > 2) {
        *eyeTextureIndex = 0;
    }
}

void func_80AE7358(EnRl* this) {
    Animation_Change(&this->skelAnime, &object_rl_Anim_000A3C, 1.0f, 0.0f,
                     Animation_GetLastFrame(&object_rl_Anim_000A3C), ANIMMODE_LOOP, 0.0f);
    this->action = 4;
    this->drawConfig = 0;
    this->alpha = 0;
    this->lightBallSpawned = 0;
    this->actor.shape.shadowAlpha = 0;
    this->unk_19C = 0.0f;
}

void func_80AE73D8(EnRl* this, PlayState* play) {
    static s32 flag = 0;

    if (play->csCtx.state == CS_STATE_IDLE) {
        if (flag) {
            if (this->actor.params == 2) {
                func_80AE7358(this);
            }
            flag = 0;
        }
    } else if (!flag) {
        flag = 1;
    }
}

void EnRl_UpdateBgCheck(EnRl* this, PlayState* play) {
    Actor_UpdateBgCheckInfo(play, &this->actor, 75.0f, 30.0f, 30.0f, UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2);
}

s32 EnRl_UpdateSkelAnime(EnRl* this) {
    return SkelAnime_Update(&this->skelAnime);
}

s32 EnRl_GetCutsceneCMD(EnRl* this, PlayState* play, u16 csCmdAction, s32 npcAction) {
    CsCmdActorAction* csCmdActorAction;

    if (play->csCtx.state != CS_STATE_IDLE) {
        csCmdActorAction = play->csCtx.npcActions[npcAction];
        if (csCmdActorAction != NULL && csCmdActorAction->action == csCmdAction) {
            return 1;
        }
    }
    return 0;
}


void EnRl_Idle(EnRl* this, PlayState* play) {
    SkelAnime_InitFlex(play, &this->skelAnime, &object_rl_Skel_007B38, &object_rl_Anim_000A3C, NULL, NULL, 0);
}

void EnRl_GiveLightMedallion(EnRl* this, PlayState* play) {
    s32 pad;
    Player* player;
    Vec3f pos;
    s16 sceneId = play->sceneId;

    if (gSaveContext.sceneLayer == 4 && sceneId == SCENE_KENJYANOMA && play->csCtx.state != CS_STATE_IDLE &&
        play->csCtx.npcActions[6] != NULL && play->csCtx.npcActions[6]->action == 2 && !this->lightMedallionGiven) {
        player = GET_PLAYER(play);
        pos.x = player->actor.world.pos.x;
        pos.y = player->actor.world.pos.y + 80.0f;
        pos.z = player->actor.world.pos.z;
        Actor_Spawn(&play->actorCtx, play, ACTOR_DEMO_EFFECT, pos.x, pos.y, pos.z, 0, 0, 0, 0xE);
        Item_Give(play, ITEM_MEDALLION_LIGHT);
        this->lightMedallionGiven = 1;
    }
}

void EnRl_ChangeToAction1(EnRl* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    this->drawConfig = 1;
    this->action = 1;
    player->actor.world.rot.y = player->actor.shape.rot.y = this->actor.world.rot.y + 0x8000;
}

void EnRl_ChangeToAction2(EnRl* this, PlayState* play) {
    CsCmdActorAction* csCmdActorAction;

    if (play->csCtx.state != CS_STATE_IDLE) {
        csCmdActorAction = play->csCtx.npcActions[0];
        if (csCmdActorAction != NULL && csCmdActorAction->action == 3) {
            Animation_Change(&this->skelAnime, &object_rl_Anim_00040C, 1.0f, 0.0f,
                             Animation_GetLastFrame(&object_rl_Anim_00040C), ANIMMODE_ONCE, 0.0f);
            this->action = 2;
        }
    }
}

void EnRl_ChangeToAction3(EnRl* this, s32 arg1) {
    if (arg1) {
        Animation_Change(&this->skelAnime, &object_rl_Anim_000830, 1.0f, 0.0f,
                         Animation_GetLastFrame(&object_rl_Anim_000830), ANIMMODE_LOOP, 0.0f);
        this->action = 3;
    }
}

void EnRl_SetupAction1(EnRl* this, PlayState* play) {
    EnRl_ChangeToAction1(this, play);
}

void EnRl_SetupAction2(EnRl* this, PlayState* play) {
    EnRl_UpdateBgCheck(this, play);
    EnRl_UpdateSkelAnime(this);
    EnRl_UpdateBlinking(this);
    EnRl_ChangeToAction2(this, play);
}

void EnRl_SetupAction3(EnRl* this, PlayState* play) {
    s32 temp;

    EnRl_UpdateBgCheck(this, play);
    temp = EnRl_UpdateSkelAnime(this);
    EnRl_UpdateBlinking(this);
    EnRl_ChangeToAction3(this, temp);
}

void EnRl_SetupGiveLightMedallion(EnRl* this, PlayState* play) {
    EnRl_UpdateBgCheck(this, play);
    EnRl_UpdateSkelAnime(this);
    EnRl_UpdateBlinking(this);
    EnRl_GiveLightMedallion(this, play);
}

void EnRl_IdleForCutscene(EnRl* this, PlayState* play) {
    SkelAnime_InitFlex(play, &this->skelAnime, &object_rl_Skel_007B38, &object_rl_Anim_000A3C, NULL, NULL, 0);
    this->action = 4;
    this->actor.shape.shadowAlpha = 0;
}

void EnRl_SpawnLightball(EnRl* this, PlayState* play) {
    // Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, ACTOR_DEMO_6K, this->actor.world.pos.x,
    //                    kREG(18) + 22.0f + this->actor.world.pos.y, this->actor.world.pos.z, 0, 0, 0, 5);
}

void func_80AE7954(EnRl* this, PlayState* play) {
    if (EnRl_GetCutsceneCMD(this, play, 4, 0)) { //Rauru Appear
        this->action = 5;
        this->drawConfig = 2;
        this->alpha = 0;
        this->actor.shape.shadowAlpha = 0;
        this->unk_19C = 0.0f;
    }
}

void func_80AE79A4(EnRl* this, PlayState* play) {
    f32* unk_19C = &this->unk_19C;
    s32 alpha = 255;

    if (EnRl_GetCutsceneCMD(this, play, 4, 0)) {
        *unk_19C += 1.0f;
        if (*unk_19C >= kREG(5) + 10.0f) {
            this->action = 7;
            this->drawConfig = 1;
            *unk_19C = kREG(5) + 10.0f;
            this->alpha = alpha;
            this->actor.shape.shadowAlpha = alpha;
            return;
        }
    } else {
        *unk_19C -= 1.0f;
        if (*unk_19C <= 0.0f) {
            this->action = 4;
            this->drawConfig = 0;
            *unk_19C = 0.0f;
            this->alpha = 0;
            this->actor.shape.shadowAlpha = 0;
            return;
        }
    }
    alpha = (*unk_19C / (kREG(5) + 10.0f)) * 255.0f;
    this->alpha = alpha;
    this->actor.shape.shadowAlpha = alpha;
}

void func_80AE7AF8(EnRl* this, PlayState* play) {
    if (EnRl_GetCutsceneCMD(this, play, 3, 0)) { //Rauru Raise his hand
        Animation_Change(&this->skelAnime, &object_rl_Anim_00040C, 1.0f, 0.0f,
                         Animation_GetLastFrame(&object_rl_Anim_00040C), ANIMMODE_ONCE, -8.0f);
        this->action = 6;
    } else if (EnRl_GetCutsceneCMD(this, play, 4, 0)) {
        this->action = 5;
        this->drawConfig = 2;
        this->unk_19C = kREG(5) + 10.0f;
        this->alpha = 255;
        if (!this->lightBallSpawned) {
            EnRl_SpawnLightball(this, play);
            this->lightBallSpawned = 1;
        }
        this->actor.shape.shadowAlpha = 0xFF;
    }
}

void func_80AE7BF8(EnRl* this, s32 arg1) {
    if (arg1 != 0) {
        Animation_Change(&this->skelAnime, &object_rl_Anim_000830, 1.0f, 0.0f,
                         Animation_GetLastFrame(&object_rl_Anim_000830), ANIMMODE_LOOP, 0.0f);
        this->action = 7;
    }
}

void func_80AE7C64(EnRl* this, PlayState* play) {
    func_80AE7954(this, play);
    func_80AE73D8(this, play);
}

void func_80AE7C94(EnRl* this, PlayState* play) {
    EnRl_UpdateBgCheck(this, play);
    EnRl_UpdateSkelAnime(this);
    EnRl_UpdateBlinking(this);
    func_80AE79A4(this, play);
    func_80AE73D8(this, play);
}

void func_80AE7CE8(EnRl* this, PlayState* play) {
    s32 temp;

    EnRl_UpdateBgCheck(this, play);
    temp = EnRl_UpdateSkelAnime(this);
    EnRl_UpdateBlinking(this);
    func_80AE7BF8(this, temp);
    func_80AE73D8(this, play);
}

void func_80AE7D40(EnRl* this, PlayState* play) {
    EnRl_UpdateBgCheck(this, play);
    EnRl_UpdateSkelAnime(this);
    EnRl_UpdateBlinking(this);
    func_80AE7AF8(this, play);
    func_80AE73D8(this, play);
}

void EnRl_DrawXlu(EnRl* this, PlayState* play) {
    s32 pad[2];
    s16 temp = this->eyeTextureIndex;
    void* tex = eyesTextures[temp];
    SkelAnime* skelAnime = &this->skelAnime;

    OPEN_DISPS(play->state.gfxCtx, "../z_en_rl_inKenjyanomaDemo02.c", 304);

    Gfx_SetupDL_25Xlu(play->state.gfxCtx);

    gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(tex));
    gSPSegment(POLY_XLU_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(tex));
    gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, this->alpha);
    gSPSegment(POLY_XLU_DISP++, 0x0C, D_80116280);

    POLY_XLU_DISP = SkelAnime_DrawFlex(play, skelAnime->skeleton, skelAnime->jointTable, skelAnime->dListCount, NULL,
                                       NULL, NULL, POLY_XLU_DISP);

    CLOSE_DISPS(play->state.gfxCtx, "../z_en_rl_inKenjyanomaDemo02.c", 331);
}

static EnRlActionFunc sActionFuncs[] = {
    EnRl_SetupAction1, EnRl_SetupAction2, EnRl_SetupAction3, EnRl_SetupGiveLightMedallion,
    func_80AE7C64, func_80AE7C94, func_80AE7CE8, func_80AE7D40,
};

void EnRl_Update(Actor* thisx, PlayState* play) {
    EnRl* this = (EnRl*)thisx;

    if ((this->action < 0) || (this->action > 7) || (sActionFuncs[this->action] == NULL)) {
        osSyncPrintf(VT_FGCOL(RED) "メインモードがおかしい!!!!!!!!!!!!!!!!!!!!!!!!!\n" VT_RST);
        return;
    }
    sActionFuncs[this->action](this, play); //execute the functions according to the actions
}

void EnRl_Init(Actor* thisx, PlayState* play) {
    EnRl* this = (EnRl*)thisx;

    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 50.0f);
    if (this->actor.params == 2) { 
        EnRl_IdleForCutscene(this, play);
    } else {
        EnRl_Idle(this, play);
    }
}
void EnRl_DrawNothing(EnRl* this, PlayState* play) {
}

void EnRl_DrawOpa(EnRl* this, PlayState* play) {
    s32 pad[2];
    s16 temp = this->eyeTextureIndex;
    void* tex = eyesTextures[temp];
    SkelAnime* skelAnime = &this->skelAnime;

    OPEN_DISPS(play->state.gfxCtx, "../z_en_rl.c", 416);

    Gfx_SetupDL_25Opa(play->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(tex));
    gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(tex));
    gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, 255);
    gSPSegment(POLY_OPA_DISP++, 0x0C, &D_80116280[2]);

    SkelAnime_DrawFlexOpa(play, skelAnime->skeleton, skelAnime->jointTable, skelAnime->dListCount, NULL, NULL,
                          &this->actor);
    CLOSE_DISPS(play->state.gfxCtx, "../z_en_rl.c", 437);
}

static EnRlDrawFunc sDrawFuncs[] = {
    EnRl_DrawNothing,
    EnRl_DrawOpa,
    EnRl_DrawXlu,
};

void EnRl_Draw(Actor* thisx, PlayState* play) {
    EnRl* this = (EnRl*)thisx;

    if (this->drawConfig < 0 || this->drawConfig >= 3 || sDrawFuncs[this->drawConfig] == NULL) {
        osSyncPrintf(VT_FGCOL(RED) "描画モードがおかしい!!!!!!!!!!!!!!!!!!!!!!!!!\n" VT_RST);
        return;
    }
    sDrawFuncs[this->drawConfig](this, play);
}

const ActorInit En_Rl_InitVars = {
    ACTOR_EN_RL,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_RL,
    sizeof(EnRl),
    (ActorFunc)EnRl_Init,
    (ActorFunc)EnRl_Destroy,
    (ActorFunc)EnRl_Update,
    (ActorFunc)EnRl_Draw,
};
