/*
 * File: z_en_takara_man.c
 * Overlay: ovl_En_Takara_Man
 * Description: Treasure Chest Game Man
 */

#include "z_en_takara_man.h"
#include "vt.h"
#include "assets/objects/object_ts/object_ts.h"

#define FLAGS (ACTOR_FLAG_0 | ACTOR_FLAG_3 | ACTOR_FLAG_4 | ACTOR_FLAG_5 | ACTOR_FLAG_27)

void EnTakaraMan_Init(Actor* thisx, PlayState* play);
void EnTakaraMan_Destroy(Actor* thisx, PlayState* play);
void EnTakaraMan_Update(Actor* thisx, PlayState* play);
void EnTakaraMan_Draw(Actor* thisx, PlayState* play);

void func_80B176E0(EnTakaraMan* this, PlayState* play);
void func_80B1778C(EnTakaraMan* this, PlayState* play);
void func_80B17B14(EnTakaraMan* this, PlayState* play);
void func_80B17934(EnTakaraMan* this, PlayState* play);
void func_80B17A6C(EnTakaraMan* this, PlayState* play);
void func_80B17AC4(EnTakaraMan* this, PlayState* play);

const ActorInit En_Takara_Man_InitVars = {
    ACTOR_EN_TAKARA_MAN,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_TS,
    sizeof(EnTakaraMan),
    (ActorFunc)EnTakaraMan_Init,
    (ActorFunc)EnTakaraMan_Destroy,
    (ActorFunc)EnTakaraMan_Update,
    (ActorFunc)EnTakaraMan_Draw,
};

static u8 sTakaraIsInitialized = false;

void EnTakaraMan_Destroy(Actor* thisx, PlayState* play) {
}

void EnTakaraMan_Init(Actor* thisx, PlayState* play) {
    EnTakaraMan* this = (EnTakaraMan*)thisx;
    s32 i;
    if (sTakaraIsInitialized) {
        Actor_Kill(&this->actor);
        osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ もういてる原 ☆☆☆☆☆ \n" VT_RST); // "Already initialized"
        return;
    }
    for(i=0;i<32;i++){
        Flags_UnsetClear(play,i);
    }
    sTakaraIsInitialized = true;
    osSyncPrintf("\n\n");
    // "Bun! %x" (needs a better translation)
    osSyncPrintf(VT_FGCOL(MAGENTA) "☆☆☆☆☆ ばぅん！ ☆☆☆☆☆ %x\n" VT_RST, play->actorCtx.flags.chest);
    play->actorCtx.flags.chest = 0;
    gSaveContext.inventory.dungeonKeys[gSaveContext.mapIndex] = -1;
    SkelAnime_InitFlex(play, &this->skelAnime, &object_ts_Skel_004FE0, &object_ts_Anim_000498, this->jointTable,
                       this->morphTable, 10);
    thisx->focus.pos = thisx->world.pos;
    this->pos = thisx->world.pos;
    thisx->world.pos.x = 133.0f;
    thisx->world.pos.y = -12.0f;
    thisx->world.pos.z = 102.0f;
    Actor_SetScale(&this->actor, 0.013f);
    this->height = 90.0f;
    Flags_SetSwitch(play, 0x5);
    this->originalRoomNum = thisx->room;
    thisx->room = -1;
    thisx->world.rot.y = thisx->shape.rot.y = -0x4E20;
    thisx->targetMode = 1;
    this->actionFunc = func_80B176E0;
}

void func_80B176E0(EnTakaraMan* this, PlayState* play) {
    f32 frameCount = Animation_GetLastFrame(&object_ts_Anim_000498);

    Animation_Change(&this->skelAnime, &object_ts_Anim_000498, 1.0f, 0.0f, (s16)frameCount, ANIMMODE_LOOP, -10.0f);
    if (!this->playingGame) {
        if(Flags_GetSwitch(play, 0x6)){//have beaten once
            this->actor.textId = 0x6030;
        }else{
            this->actor.textId = 0x6D; //First entering, ask you to play the game
        }
        this->dialogState = TEXT_STATE_CHOICE;
    }
    this->actionFunc = func_80B1778C;
}

void func_80B1778C(EnTakaraMan* this, PlayState* play) {
    s16 absYawDiff;
    s16 yawDiff;

    
    if (Actor_ProcessTalkRequest(&this->actor, play) && this->dialogState != TEXT_STATE_DONE) {
        if (!this->playingGame) {
            this->actionFunc = func_80B17934;
        } else {
            this->actionFunc = func_80B17B14;
        }
    } else {
        yawDiff = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;
        if (play->roomCtx.curRoom.num == 0x0 && !this->sucessfullyBeat && Flags_GetSwitch(play, 0x38)) {
            this->actor.textId = 0x6E; //You are a real gambler!
            this->sucessfullyBeat = 1;
            this->dialogState = TEXT_STATE_DONE;
        }

        if (!this->sucessfullyBeat && this->playingGame) {
            if (Flags_GetSwitch(play, 0x32)) { //Trigger the fake chest
                this->actor.textId = 0x84; //"Thanks a lot!"
                this->dialogState = TEXT_STATE_EVENT;
            } else {
                this->actor.textId = 0x704C; //"With that key, proceed to the room ahead."
                this->dialogState = TEXT_STATE_DONE;
            }
        }

        absYawDiff = ABS(yawDiff);
        if (absYawDiff < 0x4300) {
            if (play->roomCtx.curRoom.num != this->originalRoomNum) {
                this->actor.flags &= ~ACTOR_FLAG_0;
                this->unk_218 = 0;
            } else {
                if (!this->unk_218) {
                    this->actor.flags |= ACTOR_FLAG_0;
                    this->unk_218 = 1;
                }
                func_8002F2CC(&this->actor, play, 100.0f);
            }
        }
    }
}

void func_80B17934(EnTakaraMan* this, PlayState* play) {
    if (this->dialogState == Message_GetState(&play->msgCtx) && Message_ShouldAdvance(play)) {
        switch (play->msgCtx.choiceIndex) {
            case 0: // Yes
                if (gSaveContext.rupees >= 50) {
                    Message_CloseTextbox(play);
                    Rupees_ChangeBy(-50);
                    this->playingGame = 1;
                    this->actor.parent = NULL;
                    func_8002F434(&this->actor, play, GI_DOOR_KEY, 2000.0f, 1000.0f);
                    this->actionFunc = func_80B17A6C;
                } else {
                    Message_CloseTextbox(play);
                    this->actor.textId = 0x85; //You don't have enough Rupees!
                    Message_ContinueTextbox(play, this->actor.textId);
                    this->dialogState = TEXT_STATE_EVENT;
                    this->actionFunc = func_80B17B14;
                }
                break;
            case 1: // No
                Message_CloseTextbox(play);
                this->actor.textId = 0x2D; //You don't have to play if you don't want to.
                Message_ContinueTextbox(play, this->actor.textId);
                this->dialogState = TEXT_STATE_EVENT;
                this->actionFunc = func_80B17B14;
                break;
        }
    }
}

void func_80B17A6C(EnTakaraMan* this, PlayState* play) {
    if (Actor_HasParent(&this->actor, play)) {
        this->actionFunc = func_80B17AC4;
    } else {
        func_8002F434(&this->actor, play, GI_DOOR_KEY, 2000.0f, 1000.0f);
    }
}

void func_80B17AC4(EnTakaraMan* this, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == TEXT_STATE_DONE && Message_ShouldAdvance(play)) {
        this->actionFunc = func_80B176E0;
    }
}

void func_80B17B14(EnTakaraMan* this, PlayState* play) {
    if (this->dialogState == Message_GetState(&play->msgCtx) && Message_ShouldAdvance(play)) {
        Message_CloseTextbox(play);
        this->actionFunc = func_80B176E0;
    }
}

void EnTakaraMan_Update(Actor* thisx, PlayState* play) {
    EnTakaraMan* this = (EnTakaraMan*)thisx;

    if (this->eyeTimer != 0) {
        this->eyeTimer--;
    }

    Actor_SetFocus(&this->actor, this->height);
    Actor_TrackPlayer(play, &this->actor, &this->unk_22C, &this->unk_232, this->actor.focus.pos);
    if (this->eyeTimer == 0) {
        this->eyeTextureIdx++;
        if (this->eyeTextureIdx >= 2) {
            this->eyeTextureIdx = 0;
            this->eyeTimer = (s16)Rand_ZeroFloat(60.0f) + 20;
        }
    }
    this->unk_212++;
    SkelAnime_Update(&this->skelAnime);
    this->actionFunc(this, play);
}

s32 EnTakaraMan_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnTakaraMan* this = (EnTakaraMan*)thisx;

    if (limbIndex == 1) {
        rot->x += this->unk_232.y;
    }
    if (limbIndex == 8) {
        rot->x += this->unk_22C.y;
        rot->z += this->unk_22C.z;
    }
    return false;
}

void EnTakaraMan_Draw(Actor* thisx, PlayState* play) {
    static void* eyeTextures[] = {
        object_ts_Tex_000970,
        object_ts_Tex_000D70,
    };
    EnTakaraMan* this = (EnTakaraMan*)thisx;

    OPEN_DISPS(play->state.gfxCtx, "../z_en_takara_man.c", 528);

    Gfx_SetupDL_25Opa(play->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(eyeTextures[this->eyeTextureIdx]));
    SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount,
                          EnTakaraMan_OverrideLimbDraw, NULL, this);

    CLOSE_DISPS(play->state.gfxCtx, "../z_en_takara_man.c", 544);
}
