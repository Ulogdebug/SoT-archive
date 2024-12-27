#include "z_en_heishi4.h"
#include <uLib.h>
#include "assets/objects/object_sd/object_sd.h"
#include "vt.h"

#define FLAGS (ACTOR_FLAG_0 | ACTOR_FLAG_3)

void EnHeishi4_Init(Actor* thisx, PlayState* play);
void EnHeishi4_Destroy(Actor* thisx, PlayState* play);
void EnHeishi4_Update(Actor* thisx, PlayState* play);
void EnHeishi4_Draw(Actor* thisx, PlayState* play);

void func_80A56544(EnHeishi4* this, PlayState* play);
void EnHeishi4_DyingDialogue(EnHeishi4* this, PlayState* play);
void EnHeishi4_SetupTextJudge(EnHeishi4* this, PlayState* play);
void EnHeishi_CollisionCheck(EnHeishi4* this, PlayState* play) ;
void EnHeishi4_TextJudge(EnHeishi4* this, PlayState* play);
void EnHeishi_KickPlayer(EnHeishi4* this, PlayState* play) ;
void EnHeishi_SetupKickPlayer(EnHeishi4* this, PlayState* play) ;
void func_80A56B40(EnHeishi4* this, PlayState* play);
void func_80A56614(EnHeishi4* this, PlayState* play);
void func_80A56874(EnHeishi4* this, PlayState* play);
void func_80A56900(EnHeishi4* this, PlayState* play);
void func_80A56994(EnHeishi4* this, PlayState* play);
void func_80A56A50(EnHeishi4* this, PlayState* play);
void func_80A56ACC(EnHeishi4* this, PlayState* play);

const ActorInit En_Heishi4_InitVars = {
    ACTOR_EN_HEISHI4,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_SD,
    sizeof(EnHeishi4),
    (ActorFunc)EnHeishi4_Init,
    (ActorFunc)EnHeishi4_Destroy,
    (ActorFunc)EnHeishi4_Update,
    (ActorFunc)EnHeishi4_Draw,
};

static u32 sFaceReactionSets[] = { 6, 7 };

static s16 randomTextIDs[] = {
0x5064,0x7104,0x7105,0x7102,0x7103,
0x7101,0x7100,0x70FA,0x7168,0x7152,
0x710F,0x7112,0x710F,0X7113,0x710F,
0x7116,0x7114,0x7117,0x7118,0x70C4,
0x70C9,0x70C8,0x70C7,0x70C6,0x70CA,};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_PLAYER,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_ON,
    },
    { 33, 40, 0, { 0, 0, 0 } },
};

void EnHeishi4_Init(Actor* thisx, PlayState* play) {
    EnHeishi4* this = (EnHeishi4*)thisx;

    Actor_SetScale(thisx, 0.01f);
    this->type = thisx->params & 0xFF;
    thisx->colChkInfo.mass = MASS_IMMOVABLE;
    this->pos = thisx->world.pos;
    thisx->targetMode = 6;
    if (this->type == HEISHI4_AT_MARKET_DYING) {
        this->height = 30.0f;
        ActorShape_Init(&thisx->shape, 0.0f, NULL, 30.0f);
        SkelAnime_Init(play, &this->skelAnime, &gEnHeishiSkel, &gEnHeishiDyingGuardAnim_00C444, this->jointTable,
                       this->morphTable, 17);
    } else {
        this->height = 60.0f;
        ActorShape_Init(&thisx->shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
        SkelAnime_Init(play, &this->skelAnime, &gEnHeishiSkel, &gEnHeishiIdleAnim, this->jointTable, this->morphTable,
                       17);
    }
    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinder(play, &this->collider, thisx, &sCylinderInit);
    this->collider.dim.yShift = 0;
    this->collider.dim.radius = 15;
    this->collider.dim.height = 70;
    this->cutsceneTimer = 8;
    switch (this->type) {
        case HEISHI4_AT_KAKRIKO_ENTRANCE:
        case HEISHI4_AT_IMPAS_HOUSE:
        case HEISHI4_AT_MARKET_NIGHT:
            this->actionFunc = EnHeishi4_SetupTextJudge;
            break;
        case HEISHI4_AT_MARKET_DYING:
            this->collider.dim.radius = 28;
            this->collider.dim.height = 5;
            this->actionFunc = EnHeishi4_DyingDialogue;
            break;
        // case HEISHI4_AT_MARKET_NIGHT:
        //     this->actionFunc = func_80A56544;
        //     break;
    }
    this->unk_27C = ((thisx->params >> 8) & 0xFF);
    osSyncPrintf("\n\n");
    osSyncPrintf(VT_FGCOL(GREEN) " ☆☆☆☆☆ 兵士２セット完了！ ☆☆☆☆☆ %d\n" VT_RST, thisx->params);
    osSyncPrintf(VT_FGCOL(YELLOW) " ☆☆☆☆☆ 識別完了！\t    ☆☆☆☆☆ %d\n" VT_RST, this->type);
    osSyncPrintf(VT_FGCOL(MAGENTA) " ☆☆☆☆☆ メッセージ完了！   ☆☆☆☆☆ %x\n\n" VT_RST, (thisx->params >> 8) & 0xF);
    osSyncPrintf("\n\n");
}

void EnHeishi4_Destroy(Actor* thisx, PlayState* play) {
    EnHeishi4* this = (EnHeishi4*)thisx;

    Collider_DestroyCylinder(play, &this->collider);
}

void EnHeishi4_SetupTextJudge(EnHeishi4* this, PlayState* play) {
    f32 frames = Animation_GetLastFrame(&gEnHeishiIdleAnim);

    Animation_Change(&this->skelAnime, &gEnHeishiIdleAnim, 1.0f, 0.0f, (s16)frames, ANIMMODE_LOOP, -10.0f);
    this->actionFunc = EnHeishi4_TextJudge;
}

void EnHeishi4_TextJudge(EnHeishi4* this, PlayState* play) {
    s16 reactionOffset;
    s16 i;
    this->unk_2B4 = 0;
    reactionOffset = this->type - 4;
    if (reactionOffset < 0) {
        reactionOffset = 0;
    }
    if (reactionOffset >= 3) {
        reactionOffset = 1;
    }
    if (Text_GetFaceReaction(play, sFaceReactionSets[reactionOffset]) != 0) {
        this->actor.textId = Text_GetFaceReaction(play, sFaceReactionSets[reactionOffset]);
        this->unk_2B4 = 1;
        this->actionFunc = func_80A56B40;
    } else {
        // if (GET_EVENTCHKINF(EVENTCHKINF_80)) {
        //     this->actor.textId = 0x5065;
        //     this->actionFunc = func_80A56B40;
        //     return;
        // }
        // if (GET_EVENTCHKINF(EVENTCHKINF_45)) {
        //     this->actor.textId = 0x5068;
        //     this->actionFunc = func_80A56B40;
        //     return;
        // }
        if (this->type == HEISHI4_AT_IMPAS_HOUSE) {
            this->actor.textId = 0x5079;
        } else if(this->type == HEISHI4_AT_MARKET_NIGHT){
             this->actor.textId = 0x70E4;
        }else{
            if (CustomFlag_Get(0x4)){
                if(CHECK_QUEST_ITEM(QUEST_MEDALLION_SPIRIT)){
                    i = Rand_S16Offset(0,25);
                }else{
                    i = Rand_S16Offset(0,20);
                }
                this->actor.textId = randomTextIDs[i];  // random dialogues
            }else{
                this->actor.textId = 0x7119;
            }

        }
        this->actionFunc = func_80A56B40;
    }
}

void func_80A56544(EnHeishi4* this, PlayState* play) {
    f32 frames = Animation_GetLastFrame(&gEnHeishiIdleAnim);

    Animation_Change(&this->skelAnime, &gEnHeishiIdleAnim, 1.0f, 0.0f, (s16)frames, ANIMMODE_LOOP, -10.0f);
    if (LINK_AGE_IN_YEARS != YEARS_CHILD) {
        osSyncPrintf(VT_FGCOL(GREEN) " ☆☆☆☆☆ ぎゃぁ！オトナだー ☆☆☆☆☆ \n" VT_RST);
        Actor_Kill(&this->actor);
    } else {
        this->actionFunc = func_80A56614;
    }
}

void func_80A56614(EnHeishi4* this, PlayState* play) {
    s16 reactionOffset;

    reactionOffset = this->type - 4;
    this->unk_2B4 = 0;
    if (reactionOffset < 0) {
        reactionOffset = 0;
    }
    if (reactionOffset >= 3) {
        reactionOffset = 1;
    }
    if (Text_GetFaceReaction(play, sFaceReactionSets[reactionOffset]) != 0) {
        this->actor.textId = Text_GetFaceReaction(play, sFaceReactionSets[reactionOffset]);
        this->unk_2B4 = 1;
        this->actionFunc = func_80A56B40;
        return;
    }
    if (play->sceneId == SCENE_MIHARIGOYA) {
        if (IS_DAY) {
            this->actor.textId = 0x7004;
        } else {
            this->actor.textId = 0x709A;
        }
    } else if (play->sceneId != SCENE_MARKET_NIGHT) {
        if (IS_DAY) {
            this->actor.textId = 0x7002;
        } else {
            this->actor.textId = 0x7003;
        }
    } else {
        this->actor.textId = 0x7003;
    }

    this->actionFunc = func_80A56B40;
}

void EnHeishi4_DyingDialogue(EnHeishi4* this, PlayState* play) {
    /*if (GET_EVENTCHKINF(EVENTCHKINF_45)) {
        Actor_Kill(&this->actor);
        return;
    }*/
    this->unk_284 = 0;
    if (1/*GET_EVENTCHKINF(EVENTCHKINF_80)*/) {
        if (0/*!GET_INFTABLE(INFTABLE_6C)*/) {
            f32 frames = Animation_GetLastFrame(&gEnHeishiDyingGuardAnim_00C444);
            Animation_Change(&this->skelAnime, &gEnHeishiDyingGuardAnim_00C444, 1.0f, 0.0f, (s16)frames, ANIMMODE_LOOP,
                             -10.0f);
            this->actor.textId = 0x7007;
            this->unk_282 = TEXT_STATE_EVENT;
            this->unk_284 = 1;
        } else {
            this->actor.textId = 0x7008;
            this->unk_282 = TEXT_STATE_DONE;
        }
        this->actionFunc = func_80A56874;
    } else {
        Actor_Kill(&this->actor);
    }
}

void func_80A56874(EnHeishi4* this, PlayState* play) {
    if (this->unk_284 != 0) {
        SkelAnime_Update(&this->skelAnime);
    }
    if (Actor_ProcessTalkRequest(&this->actor, play)) {
        if (this->unk_284 == 0) {
            this->actionFunc = EnHeishi4_DyingDialogue;

        } else {
            this->actionFunc = func_80A56900;
        }
    } else {
        func_8002F2CC(&this->actor, play, 100.0f);
    }
}

void func_80A56900(EnHeishi4* this, PlayState* play) {
    f32 frames = Animation_GetLastFrame(&gEnHeishiDyingGuardTalkAnim);

    Animation_Change(&this->skelAnime, &gEnHeishiDyingGuardTalkAnim, 1.0f, 0.0f, (s16)frames, ANIMMODE_LOOP, -10.0f);
    this->actionFunc = func_80A56994;
}

void func_80A56994(EnHeishi4* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime);
    Actor_TrackPlayer(play, &this->actor, &this->unk_260, &this->unk_266, this->actor.focus.pos);
    if ((this->unk_282 == Message_GetState(&play->msgCtx)) && Message_ShouldAdvance(play)) {
        Message_CloseTextbox(play);
        SET_INFTABLE(INFTABLE_6C);
        func_8002DF54(play, NULL, 0x5F);
        this->actionFunc = func_80A56A50;
    }
}

void func_80A56A50(EnHeishi4* this, PlayState* play) {
    f32 frames = Animation_GetLastFrame(&gEnHeishiDyingGuardDieAnim);

    this->unk_288 = frames;
    Animation_Change(&this->skelAnime, &gEnHeishiDyingGuardDieAnim, 1.0f, 0.0f, frames, ANIMMODE_ONCE, -10.0f);
    this->actionFunc = func_80A56ACC;
}

void func_80A56ACC(EnHeishi4* this, PlayState* play) {
    f32 currentFrame = this->skelAnime.curFrame;

    SkelAnime_Update(&this->skelAnime);
    if (this->unk_288 <= currentFrame) {
        func_8002DF54(play, NULL, 0x7);
        this->actionFunc = EnHeishi4_DyingDialogue;
    }
}

void func_80A56B40(EnHeishi4* this, PlayState* play) {
    s16 reactionOffset;

    SkelAnime_Update(&this->skelAnime);
    reactionOffset = (this->type - 4);
    if (reactionOffset < 0) {
        reactionOffset = 0;
    }
    if (reactionOffset >= 3) {
        reactionOffset = 1;
    }
    if (Text_GetFaceReaction(play, sFaceReactionSets[reactionOffset]) != 0) {
        if (this->unk_2B4 == 0) {
            if ((this->type == HEISHI4_AT_KAKRIKO_ENTRANCE) || (this->type == HEISHI4_AT_IMPAS_HOUSE)) {
                this->actionFunc = EnHeishi4_TextJudge;
                return;
            }
            if (this->type == HEISHI4_AT_MARKET_NIGHT) {
                this->actionFunc = EnHeishi4_TextJudge;
                return;
            }
        }
    } else {
        if (this->unk_2B4 != 0) {
            if ((this->type == HEISHI4_AT_KAKRIKO_ENTRANCE) || (this->type == HEISHI4_AT_IMPAS_HOUSE)) {
                this->actionFunc = EnHeishi4_TextJudge;
                return;
            }
            if (this->type == HEISHI4_AT_MARKET_NIGHT) {
                this->actionFunc = EnHeishi4_TextJudge;
                return;
            }
        }
    }
    if (Actor_ProcessTalkRequest(&this->actor, play)) {
        if ((this->type == HEISHI4_AT_KAKRIKO_ENTRANCE) || (this->type == HEISHI4_AT_IMPAS_HOUSE)) {
            if (this->type == HEISHI4_AT_KAKRIKO_ENTRANCE){
                if(!CustomFlag_Get(0x4)){
                    CustomFlag_Set(0x4,true);
                }
            }
            this->unk_284 = 1;
            this->actionFunc = EnHeishi4_TextJudge;
            return;
        }
        if (this->type == HEISHI4_AT_MARKET_NIGHT) {
            this->unk_284 = 1;
            this->actionFunc = EnHeishi4_TextJudge;
            return;
        }
    }
    
    func_8002F2F4(&this->actor, play);
}



void EnHeishi4_Update(Actor* thisx, PlayState* play) {
    EnHeishi4* this = (EnHeishi4*)thisx;
    s32 pad;
    Player* player = GET_PLAYER(play);
   if( CustomFlag_Get(0x4)){
        Debug_Text(255,255,255,3,3,"custom flag enable");
    }
    thisx->world.pos.x = this->pos.x;
    thisx->world.pos.y = this->pos.y;
    thisx->world.pos.z = this->pos.z;
    Actor_SetFocus(thisx, this->height);
    if (this->type != HEISHI4_AT_MARKET_DYING) {
        this->unk_28C.unk_18 = player->actor.world.pos;
        if (!LINK_IS_ADULT) {
            this->unk_28C.unk_18.y = (player->actor.world.pos.y - 10.0f);
        }
        func_80034A14(thisx, &this->unk_28C, 2, 4);
        this->unk_260 = this->unk_28C.unk_08;
        this->unk_266 = this->unk_28C.unk_0E;
    }
    this->unk_27E += 1;
    this->actionFunc(this, play);
    Actor_MoveForward(thisx);
    Actor_UpdateBgCheckInfo(play, thisx, 10.0f, 10.0f, 30.0f,
                            UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2 | UPDBGCHECKINFO_FLAG_3 |
                                UPDBGCHECKINFO_FLAG_4);
    Collider_UpdateCylinder(&this->actor, &this->collider);
    EnHeishi_CollisionCheck(this, play);

}

void EnHeishi_CollisionCheck(EnHeishi4* this, PlayState* play) {
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
    if(this->type != HEISHI4_AT_MARKET_NIGHT && this->type != HEISHI4_AT_MARKET_DYING){
        if (this->collider.base.acFlags & AC_HIT){
            this->collider.base.acFlags &= ~AC_HIT;
            this->actionFunc = EnHeishi_SetupKickPlayer;
        }else{
             CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
        }
    }
}

void EnHeishi_SetupKickPlayer(EnHeishi4* this, PlayState* play) {
    f32 frameCount;
    if(this->type == HEISHI4_AT_MARKET_DYING){
        frameCount = Animation_GetLastFrame(&gEnHeishiDyingGuardTalkAnim);
        Animation_Change(&this->skelAnime, &gEnHeishiDyingGuardTalkAnim, 1.0f, 0.0f, (s16)frameCount, ANIMMODE_ONCE,
                             6.0f);
    }else{
        frameCount = Animation_GetLastFrame(&gEnHeishiSlamSpearAnim);
        Animation_Change(&this->skelAnime, &gEnHeishiSlamSpearAnim, 1.0f, 0.0f, (s16)frameCount, ANIMMODE_ONCE, 6.0f);
    }
    if(this->cutsceneTimer <=0){
        func_8002DF54(play, &this->actor, 0x5F);
        Message_StartTextbox(play, 0x70B9, &this->actor);
        func_80078884(NA_SE_SY_FOUND);
        this->actionFunc = EnHeishi_KickPlayer;

    }
    else{
        this->cutsceneTimer--;
    }
}

void EnHeishi_KickPlayer(EnHeishi4* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    if (!this->loadStarted && SkelAnime_Update(&this->skelAnime)) {
        // if dialog state is 5 and textbox has been advanced, kick player out
        if ((Message_GetState(&play->msgCtx) == TEXT_STATE_EVENT) && Message_ShouldAdvance(play)) {
            Message_CloseTextbox(play);
            if (!this->loadStarted) {

                if (gSaveContext.healthCapacity <= 0x50 && gSaveContext.health > 0x10) {
                    gSaveContext.health = 0x10;
                } else if (gSaveContext.healthCapacity <= 0xA0 && gSaveContext.health > 0x18) {
                    gSaveContext.health = 0x18;
                } else if (gSaveContext.healthCapacity <= 0xF0 && gSaveContext.health > 0x20) {
                    gSaveContext.health = 0x20;
                } else if (gSaveContext.health > 0xF0 && gSaveContext.health > 0x2C){
                    gSaveContext.health = 0x2C;
                }else{
                    gSaveContext.health = 4;
                }             
                   
                gExitParam.exit.value = 0;
                gExitParam.exit.sceneIndex = 0x5A;
                gExitParam.exit.__isExit = true;
                play->transitionType = TRANS_TYPE_CIRCLE(TCA_STARBURST, TCC_WHITE, TCS_FAST);
                gSaveContext.nextTransitionType = TRANS_TYPE_CIRCLE(TCA_STARBURST, TCC_WHITE, TCS_FAST);
                gExitParam.exit.spawnIndex = 0x3;
                play->transitionTrigger = TRANS_TRIGGER_START;
            }
        }
    }
}

s32 EnHeishi_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnHeishi4* this = (EnHeishi4*)thisx;

    if (limbIndex == 9) {
        rot->x += this->unk_266.y;
    }
    if (limbIndex == 16) {
        rot->x += this->unk_260.y;
        rot->z += this->unk_260.z;
    }
    return false;
}

void EnHeishi4_Draw(Actor* thisx, PlayState* play) {
    EnHeishi4* this = (EnHeishi4*)thisx;

    Gfx_SetupDL_25Opa(play->state.gfxCtx);
    SkelAnime_DrawOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, EnHeishi_OverrideLimbDraw, NULL,
                      this);
}
