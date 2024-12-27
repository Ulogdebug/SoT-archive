/*
 * File: z_en_heishi1.c
 * Overlay: ovl_En_Heishi1
 * Description: Courtyard Guards
 */
#include <uLib.h>
#include "z_en_heishi1.h"
#include "assets/objects/object_sd/object_sd.h"
#include "vt.h"

#define FLAGS ACTOR_FLAG_4
#define NOTICE_RADIUS 120.0f
void EnHeishi1_Init(Actor* thisx, PlayState* play);
void EnHeishi1_Destroy(Actor* thisx, PlayState* play);
void EnHeishi1_Update(Actor* thisx, PlayState* play);
void EnHeishi1_Draw(Actor* thisx, PlayState* play);

void EnHeishi1_SetupWait(EnHeishi1* this, PlayState* play);
void EnHeishi1_SetupWalk(EnHeishi1* this, PlayState* play);
void EnHeishi1_SetupMoveToLink(EnHeishi1* this, PlayState* play);
void EnHeishi1_SetupTurnTowardLink(EnHeishi1* this, PlayState* play);
void EnHeishi1_SetupKick(EnHeishi1* this, PlayState* play);
void EnHeishi1_SetupWaitNight(EnHeishi1* this, PlayState* play);
void EnHeishi1_CollisionCheck(EnHeishi1* this, PlayState* play);
void EnHeishi1_Wait(EnHeishi1* this, PlayState* play);
void EnHeishi1_Walk(EnHeishi1* this, PlayState* play);
void EnHeishi1_MoveToLink(EnHeishi1* this, PlayState* play);
void EnHeishi1_TurnTowardLink(EnHeishi1* this, PlayState* play);
void EnHeishi1_Kick(EnHeishi1* this, PlayState* play);
void EnHeishi1_WaitNight(EnHeishi1* this, PlayState* play);
void EnHeishi1_ReducePlayerDying(EnHeishi1* this, PlayState* play);

static s16 findPlayerDialogues[] = {0x7028,0x70BB,0x70B9};//see player, hear player, got hit by plaer

static s32 sPlayerIsCaught = false;

const ActorInit En_Heishi1_InitVars = {
    0,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_SD,
    sizeof(EnHeishi1),
    (ActorFunc)EnHeishi1_Init,
    (ActorFunc)EnHeishi1_Destroy,
    (ActorFunc)EnHeishi1_Update,
    (ActorFunc)EnHeishi1_Draw,
};

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

static f32 sAnimParamsInit[][8] = {
    { 1.0f, -10.0f, 3.0f, 0.5f, 1000.0f, 200.0f, 0.3f, 1000.0f },
    { 3.0f, -3.0f, 6.0f, 0.8f, 2000.0f, 400.0f, 0.5f, 2000.0f },
    { 1.0f, -10.0f, 3.0f, 0.5f, 1000.0f, 200.0f, 0.3f, 1000.0f },
    { 3.0f, -3.0f, 6.0f, 0.8f, 2000.0f, 400.0f, 0.5f, 2000.0f },
};

static s16 sBaseHeadTimers[] = { 20, 10, 20, 10, 13, 0 };

static Vec3f sRupeePositions[] = {
    { 0.0f, 0.0f, 90.0f },  { -55.0f, 0.0f, 90.0f }, { -55.0f, 0.0f, 30.0f }, { -55.0f, 0.0f, -30.0f },
    { 0.0f, 0.0f, -30.0f }, { 55.0f, 0.0f, -30.0f }, { 55.0f, 0.0f, 30.0f },  { 55.0f, 0.0f, 90.0f },
};

static s32 sBgCamIndices[] = {
    7, 7, 2, 2, 2, 2, 3, 3, 4, 4, 5, 6, 4, 4, 5, 6,
};

static s16 sWaypoints[] = { 0, 4, 1, 5, 2, 6, 3, 7 };

void EnHeishi1_Init(Actor* thisx, PlayState* play) {
    s32 pad;
    EnHeishi1* this = (EnHeishi1*)thisx;
    Vec3f rupeePos;
    s32 i;

    Actor_SetScale(&this->actor, 0.01f);
    SkelAnime_Init(play, &this->skelAnime, &gEnHeishiSkel, &gEnHeishiIdleAnim, this->jointTable, this->morphTable, 17);
    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinder(play, &this->collider, thisx, &sCylinderInit);
    this->collider.dim.yShift = 0;
    this->collider.dim.radius = 15;
    this->collider.dim.height = 70;
    this->type = (this->actor.params >> 8) & 0xFF;
    this->path = this->actor.params & 0xFF;
    this->actor.gravity = -1.0f;
    for (i = 0; i < ARRAY_COUNT(sAnimParamsInit[0]); i++) {
        this->animParams[i] = sAnimParamsInit[this->type][i];
    }

    // "type"
    osSyncPrintf(VT_FGCOL(GREEN) " 種類☆☆☆☆☆☆☆☆☆☆☆☆☆ %d\n" VT_RST, this->type);
    // "path data"
    osSyncPrintf(VT_FGCOL(YELLOW) " れえるでぇたぁ☆☆☆☆☆☆☆☆ %d\n" VT_RST, this->path);
    osSyncPrintf(VT_FGCOL(MAGENTA) " anime_frame_speed ☆☆☆☆☆☆ %f\n" VT_RST, this->animSpeed);
    // "interpolation frame"
    osSyncPrintf(VT_FGCOL(MAGENTA) " 補間フレーム☆☆☆☆☆☆☆☆☆ %f\n" VT_RST, this->animMorphFrames);
    // "targeted movement speed value between points"
    osSyncPrintf(VT_FGCOL(MAGENTA) " point間の移動スピード目標値 ☆ %f\n" VT_RST, this->moveSpeedTarget);
    // "maximum movement speed value between points"
    osSyncPrintf(VT_FGCOL(MAGENTA) " point間の移動スピード最大 ☆☆ %f\n" VT_RST, this->moveSpeedMax);
    // "(body) targeted turning angle speed value"
    osSyncPrintf(VT_FGCOL(MAGENTA) " (体)反転アングルスピード目標値 %f\n" VT_RST, this->bodyTurnSpeedTarget);
    // "(body) maximum turning angle speed"
    osSyncPrintf(VT_FGCOL(MAGENTA) " (体)反転アングルスピード最大☆ %f\n" VT_RST, this->bodyTurnSpeedMax);
    // "(head) targeted turning angle speed value"
    osSyncPrintf(VT_FGCOL(MAGENTA) " (頭)反転アングルスピード加算値 %f\n" VT_RST, this->headTurnSpeedScale);
    // "(head) maximum turning angle speed"
    osSyncPrintf(VT_FGCOL(MAGENTA) " (頭)反転アングルスピード最大☆ %f\n" VT_RST, this->headTurnSpeedMax);
    osSyncPrintf(VT_FGCOL(GREEN) " 今時間 %d\n" VT_RST, ((void)0, gSaveContext.dayTime)); // "current time"
    osSyncPrintf(VT_FGCOL(YELLOW) " チェック時間 %d\n" VT_RST, CLOCK_TIME(17, 30) - 1);   // "check time"
    osSyncPrintf("\n\n");

    // if (this->path == 3) {
    //     for (i = 0; i < ARRAY_COUNT(sRupeePositions); i++) {
    //         rupeePos = sRupeePositions[i];
    //         Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, ACTOR_EN_EX_RUPPY, rupeePos.x, rupeePos.y,
    //                            rupeePos.z, 0, 0, 0, 3);
    //     }
    // }

    if (this->type != 5) {
            this->actionFunc = EnHeishi1_SetupWalk;
    } else {
            this->actionFunc = EnHeishi1_SetupWaitNight;
    }
}

void EnHeishi1_Destroy(Actor* thisx, PlayState* play) {
    EnHeishi1* this = (EnHeishi1*)thisx;
    Collider_DestroyCylinder(play, &this->collider);
}

void EnHeishi1_SetupWalk(EnHeishi1* this, PlayState* play) {
    f32 frameCount = Animation_GetLastFrame(&gEnHeishiWalkAnim);

    Animation_Change(&this->skelAnime, &gEnHeishiWalkAnim, this->animSpeed, 0.0f, (s16)frameCount, ANIMMODE_LOOP,
                     this->animMorphFrames);
    this->bodyTurnSpeed = 0.0f;
    this->moveSpeed = 0.0f;
    this->headDirection = Rand_ZeroFloat(1.99f);
    this->actionFunc = EnHeishi1_Walk;
}

void EnHeishi1_ReducePlayerDying(EnHeishi1* this, PlayState* play){
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

}


void EnHeishi1_Walk(EnHeishi1* this, PlayState* play) {
    Path* path;
    Vec3s* pointPos;
    f32 pathDiffX;
    f32 pathDiffZ;
    f32 pathDiffY;
    s16 randOffset;

    SkelAnime_Update(&this->skelAnime);

    if (Animation_OnFrame(&this->skelAnime, 1.0f) || Animation_OnFrame(&this->skelAnime, 17.0f)) {
        Audio_PlayActorSfx2(&this->actor, NA_SE_EV_KNIGHT_WALK);
    }

    if (!sPlayerIsCaught) {
        path = &play->setupPathList[this->path];
        pointPos = SEGMENTED_TO_VIRTUAL(path->points);
        pointPos += this->waypoint;

        Math_ApproachF(&this->actor.world.pos.x, pointPos->x, 1.0f, this->moveSpeed);
        Math_ApproachF(&this->actor.world.pos.z, pointPos->z, 1.0f, this->moveSpeed);
        Math_ApproachF(&this->moveSpeed, this->moveSpeedTarget, 1.0f, this->moveSpeedMax);

        pathDiffX = pointPos->x - this->actor.world.pos.x;
        pathDiffZ = pointPos->z - this->actor.world.pos.z;

        Math_SmoothStepToS(&this->actor.shape.rot.y, RAD_TO_BINANG(Math_FAtan2F(pathDiffX, pathDiffZ)), 3,
                           this->bodyTurnSpeed, 0);

        Math_ApproachF(&this->bodyTurnSpeed, this->bodyTurnSpeedTarget, 1.0f, this->bodyTurnSpeedMax);

        if (this->headTimer == 0) {
            this->headDirection++;
            this->headAngleTarget = 0x2000;
            // if headDirection is odd, face 45 degrees left
            if ((this->headDirection & 1) != 0) {
                this->headAngleTarget *= -1;
            }
            randOffset = Rand_ZeroFloat(30.0f);
            this->headTimer = sBaseHeadTimers[this->type] + randOffset;
        }

        Math_ApproachF(&this->headAngle, this->headAngleTarget, this->headTurnSpeedScale, this->headTurnSpeedMax);

        if ((this->path == BREG(1)) && (BREG(0) != 0)) {
            osSyncPrintf(VT_FGCOL(RED) " 種類  %d\n" VT_RST, this->path);
            osSyncPrintf(VT_FGCOL(RED) " ぱす  %d\n" VT_RST, this->waypoint);
            osSyncPrintf(VT_FGCOL(RED) " 反転  %d\n" VT_RST, this->bodyTurnSpeed);
            osSyncPrintf(VT_FGCOL(RED) " 時間  %d\n" VT_RST, this->waypointTimer);
            osSyncPrintf(VT_FGCOL(RED) " 点座  %d\n" VT_RST, path->count);
            osSyncPrintf("\n\n");
        }

        // when 20 units away from a middle waypoint, decide whether or not to skip it
        if ((fabsf(pathDiffX) < 20.0f) && (fabsf(pathDiffZ) < 20.0f)) {
            if (this->waypointTimer == 0) {
                if (this->type >= 2) {
                    if ((this->waypoint >= 4) && (Rand_ZeroFloat(1.99f) > 1.0f)) {
                        if (this->waypoint == 7) {
                            this->waypoint = 0;
                        }
                        if (this->waypoint >= 4) {
                            this->waypoint -= 3;
                        }
                        this->waypointTimer = 5;
                        return;
                    }
                }
                this->actionFunc = EnHeishi1_SetupWait;
            }
        }
    }
}

void EnHeishi1_SetupMoveToLink(EnHeishi1* this, PlayState* play) {
    f32 frameCount = Animation_GetLastFrame(&gEnHeishiWalkAnim);
    this->kickTimer = 60;
    func_80078884(NA_SE_SY_FOUND);
    Animation_Change(&this->skelAnime, &gEnHeishiWalkAnim, 3.0f, 0.0f, (s16)frameCount, ANIMMODE_LOOP, -3.0f);
    this->bodyTurnSpeed = 0.0f;
    this->moveSpeed = 0.0f;
    Message_StartTextbox(play, findPlayerDialogues[this->findPlayerState - 1], &this->actor);
    Interface_SetDoAction(play, DO_ACTION_STOP);
    this->actionFunc = EnHeishi1_MoveToLink;
}

void EnHeishi1_MoveToLink(EnHeishi1* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    SkelAnime_Update(&this->skelAnime);
    if((ABS(this->actor.yDistToPlayer) > 45.0f)){
        this->kickTimer = 30;
        this->actionFunc = EnHeishi1_SetupTurnTowardLink;
    }
    Math_ApproachF(&this->actor.world.pos.x, player->actor.world.pos.x, 1.0f, this->moveSpeed);
    Math_ApproachF(&this->actor.world.pos.z, player->actor.world.pos.z, 1.0f, this->moveSpeed);
    Math_ApproachF(&this->moveSpeed, 6.0f, 1.0f, 0.4f);
    Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 3, this->bodyTurnSpeed, 0);
    Math_ApproachF(&this->bodyTurnSpeed, 3000.0f, 1.0f, 300.0f);
    Math_ApproachZeroF(&this->headAngle, 0.5f, 2000.0f);

    if ((this->actor.xzDistToPlayer < 70.0f)&&(ABS(this->actor.yDistToPlayer) <= 45.0f) || (this->kickTimer == 0)) {
        this->actionFunc = EnHeishi1_SetupTurnTowardLink;
    }
}

void EnHeishi1_SetupWait(EnHeishi1* this, PlayState* play) {
    s16 rand;
    f32 frameCount = Animation_GetLastFrame(&gEnHeishiIdleAnim);

    Animation_Change(&this->skelAnime, &gEnHeishiIdleAnim, this->animSpeed, 0.0f, (s16)frameCount, ANIMMODE_LOOP,
                     this->animMorphFrames);
    this->headBehaviorDecided = false;
    this->headDirection = Rand_ZeroFloat(1.99f);
    rand = Rand_ZeroFloat(50.0f);
    this->waitTimer = rand + 50;
    this->actionFunc = EnHeishi1_Wait;
}

void EnHeishi1_Wait(EnHeishi1* this, PlayState* play) {
    s16 randOffset;
    s32 i;

    SkelAnime_Update(&this->skelAnime);
    if (!sPlayerIsCaught) {
        switch (this->headBehaviorDecided) {
            case false:
                this->headDirection++;
                // if headDirection is odd, face 52 degrees left
                this->headAngleTarget = (this->headDirection & 1) ? 0x2500 : -0x2500;
                randOffset = Rand_ZeroFloat(30.0f);
                this->headTimer = sBaseHeadTimers[this->type] + randOffset;
                this->headBehaviorDecided = true;
                break;
            case true:
                if (this->headTimer == 0) {
                    if (this->waitTimer == 0) {
                        if ((this->type == 0) || (this->type == 1)) {
                            this->waypoint++;
                            if (this->waypoint >= 4) {
                                this->waypoint = 0;
                            }
                        } else {
                            // waypoints are defined with corners as 0-3 and middle points as 4-7
                            // to choose the next waypoint, the order "04152637" is hardcoded in an array
                            for (i = 0; i < ARRAY_COUNT(sWaypoints); i++) {
                                if (this->waypoint == sWaypoints[i]) {
                                    i++;
                                    if (i >= ARRAY_COUNT(sWaypoints)) {
                                        i = 0;
                                    }
                                    this->waypoint = sWaypoints[i];
                                    break;
                                }
                            }
                            this->waypointTimer = 5;
                        }
                        this->actionFunc = EnHeishi1_SetupWalk;
                    } else {
                        this->headBehaviorDecided = false;
                    }
                }
                break;
        }
        Math_ApproachF(&this->headAngle, this->headAngleTarget, this->headTurnSpeedScale,
                       this->headTurnSpeedMax + this->headTurnSpeedMax);

        if ((this->path == BREG(1)) && (BREG(0) != 0)) {
            osSyncPrintf(VT_FGCOL(GREEN) " 種類  %d\n" VT_RST, this->path);
            osSyncPrintf(VT_FGCOL(GREEN) " ぱす  %d\n" VT_RST, this->waypoint);
            osSyncPrintf(VT_FGCOL(GREEN) " 反転  %d\n" VT_RST, this->bodyTurnSpeed);
            osSyncPrintf(VT_FGCOL(GREEN) " 時間  %d\n" VT_RST, this->waypointTimer);
            osSyncPrintf("\n\n");
        }
    }
}

void EnHeishi1_SetupTurnTowardLink(EnHeishi1* this, PlayState* play) {
    f32 frameCount = Animation_GetLastFrame(&gEnHeishiIdleAnim);
    func_8002DF54(play, &this->actor, 0x5F);
    Animation_Change(&this->skelAnime, &gEnHeishiIdleAnim, 1.0f, 0.0f, (s16)frameCount, ANIMMODE_LOOP, -10.0f);
    this->actionFunc = EnHeishi1_TurnTowardLink;
}

void EnHeishi1_TurnTowardLink(EnHeishi1* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime);

        Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 3, this->bodyTurnSpeed, 0);
        Math_ApproachF(&this->bodyTurnSpeed, 3000.0f, 1.0f, 300.0f);
        Math_ApproachZeroF(&this->headAngle, 0.5f, 2000.0f);

    if (this->kickTimer == 0) {
        this->actionFunc = EnHeishi1_SetupKick;
    }
}

void EnHeishi1_SetupKick(EnHeishi1* this, PlayState* play) {
    f32 frameCount = Animation_GetLastFrame(&gEnHeishiIdleAnim);

    Animation_Change(&this->skelAnime, &gEnHeishiIdleAnim, 1.0f, 0.0f, (s16)frameCount, ANIMMODE_LOOP, -10.0f);
    this->actionFunc = EnHeishi1_Kick;
}

void EnHeishi1_Kick(EnHeishi1* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    SkelAnime_Update(&this->skelAnime);
    if (!this->loadStarted) {
        // if dialog state is 5 and textbox has been advanced, kick player out
        if ((Message_GetState(&play->msgCtx) == TEXT_STATE_EVENT) && Message_ShouldAdvance(play)) {
            Message_CloseTextbox(play);
            if (!this->loadStarted) {
                if(this->findPlayerState !=3 ){
                    if((gSaveContext.health - 16) > 0 ){
                       gSaveContext.health = gSaveContext.health - 16;
                    }
                }else{
                    EnHeishi1_ReducePlayerDying(this,play); //Beat up player if they get hit
                }
                //SET_EVENTCHKINF(EVENTCHKINF_4E);
                //play->nextEntranceIndex = 0x0033;
                play->transitionTrigger = TRANS_TRIGGER_START;
                gExitParam.exit.value = 0;
                gExitParam.exit.sceneIndex = 0xB;
                gExitParam.exit.__isExit = true;
                // gExitParam.exit.fadeOut = 5;
                // gExitParam.exit.fadeIn = 5;

                play->transitionType = TRANS_TYPE_CIRCLE(TCA_STARBURST, TCC_WHITE, TCS_FAST);
                gSaveContext.nextTransitionType = TRANS_TYPE_CIRCLE(TCA_STARBURST, TCC_WHITE, TCS_FAST);
                if(this->findPlayerState == 3){
                    gExitParam.exit.spawnIndex = 0X4;
                }else{
                    gExitParam.exit.spawnIndex = 0X1;
                }
                this->loadStarted = true;
                sPlayerIsCaught = false;
            }
        }
    }
}

void EnHeishi1_SetupWaitNight(EnHeishi1* this, PlayState* play) {
    f32 frameCount = Animation_GetLastFrame(&gEnHeishiIdleAnim);

    Animation_Change(&this->skelAnime, &gEnHeishiIdleAnim, 1.0f, 0.0f, (s16)frameCount, ANIMMODE_LOOP, -10.0f);
    this->actionFunc = EnHeishi1_WaitNight;
}

void EnHeishi1_WaitNight(EnHeishi1* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime);
    if ( ABS(this->actor.yDistToPlayer) <= 60.0f) {
        if((this->actor.xzDistToPlayer < NOTICE_RADIUS) && (Actor_IsFacingPlayer(&this->actor, DEG_TO_BINANG(120.0f)))){
            this->findPlayerState = 1; //see player
        }
        if(((this->actor.xzDistToPlayer < (NOTICE_RADIUS + 30.0f))&&(func_8002DDE4(play)))){
            this->findPlayerState = 2; // hear player
        }
    }
    if(this->findPlayerState){
        this->actionFunc = EnHeishi1_SetupMoveToLink;
    }

}

void EnHeishi1_CollisionCheck(EnHeishi1* this, PlayState* play) {
    Actor_MoveForward(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 60.0f, 10.0f, 60.0f, ACTOR_BGCHECK_FLOOR);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
    if (this->collider.base.acFlags & AC_HIT){
        this->collider.base.acFlags &= ~AC_HIT;
        func_8002DF54(play, &this->actor, 0x5F);
        this->findPlayerState = 3;
        sPlayerIsCaught = true;
        this->actionFunc = EnHeishi1_SetupMoveToLink;      
        
    }else{
         CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
    }
}

void EnHeishi1_Update(Actor* thisx, PlayState* play) {
    EnHeishi1* this = (EnHeishi1*)thisx;
    s16 path;
    u8 i;
    s32 pad;
    Player* player = GET_PLAYER(play);
    s32 pad2;
    Camera* activeCam;

    this->activeTimer++;

    for (i = 0; i < ARRAY_COUNT(this->timers); i++) {
        if (this->timers[i] != 0) {
            this->timers[i]--;
        }
    }

    if (this->waypointTimer != 0) {
        this->waypointTimer--;
    }

    activeCam = GET_ACTIVE_CAM(play);
    Collider_UpdateCylinder(&this->actor, &this->collider);
    EnHeishi1_CollisionCheck(this, play);
    if (player->actor.freezeTimer == 0) {

        this->actionFunc(this, play);

        this->actor.uncullZoneForward = 550.0f;
        this->actor.uncullZoneScale = 350.0f;
        this->actor.uncullZoneDownward = 700.0f;

        if (this->type != 5) {
            path = this->path * 2;
            if (1/*(sBgCamIndices[path] == activeCam->bgCamIndex) || (sBgCamIndices[path + 1] == activeCam->bgCamIndex)*/) {
                if (!sPlayerIsCaught) {
                    if ((this->actionFunc == EnHeishi1_Walk) || (this->actionFunc == EnHeishi1_Wait)) {
                        Vec3f searchBallVel;
                        Vec3f searchBallAccel = { 0.0f, 0.0f, 0.0f };
                        Vec3f searchBallMult = { 0.0f, 0.0f, 20.0f };
                        Vec3f searchBallPos;

                        searchBallPos.x = this->actor.world.pos.x;
                        searchBallPos.y = this->actor.world.pos.y + 60.0f;
                        searchBallPos.z = this->actor.world.pos.z;

                        Matrix_Push();
                        Matrix_RotateY(BINANG_TO_RAD_ALT(this->actor.shape.rot.y + this->headAngle), MTXMODE_NEW);
                        searchBallMult.z = 30.0f;
                        Matrix_MultVec3f(&searchBallMult, &searchBallVel);
                        Matrix_Pop();

                        EffectSsSolderSrchBall_Spawn(play, &searchBallPos, &searchBallVel, &searchBallAccel, 2,
                                                     &this->linkDetected);

                        if ((this->actor.xzDistToPlayer < 80.0f) && (ABS(this->actor.yDistToPlayer) < 60.0f)) {
                            this->linkDetected = true;
                            this->findPlayerState = 1;
                        } else if ((this->actor.xzDistToPlayer < 80.0f) && (ABS(this->actor.yDistToPlayer) < 60.0f)) {
                            // this case probably exists to detect link making a jump sound
                            // from slightly further away than the previous 60 unit check
                            if (player->actor.velocity.y > -4.0f || func_8002DDE4(play)) {
                                this->linkDetected = true;
                                this->findPlayerState = 2;
                            }
                        }

                        if (this->linkDetected) {
                            if(!this->findPlayerState){
                                this->findPlayerState = 1;
                            }
                            //! @bug This appears to be a check to make sure that link is standing on the ground
                            // before getting caught. However this is an issue for two reasons:
                            // 1: When doing a backflip or falling from the upper path, links y velocity will reach
                            // less than -4.0 before even touching the ground.
                            // 2: There is one frame when landing from a sidehop where you can sidehop again without
                            // letting y velocity reach -4.0 or less. This enables the player to do frame perfect
                            // sidehops onto the next screen and prevent getting caught.
                            if (!(player->actor.velocity.y > -3.9f)) {
                                this->linkDetected = false;
                                // this 60 unit height check is so the player doesnt get caught when on the upper path
                                if (fabsf(player->actor.world.pos.y - this->actor.world.pos.y) < 60.0f) {
                                    // "Discovered!"
                                    sPlayerIsCaught = true;
                                    this->actionFunc = EnHeishi1_SetupMoveToLink;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

s32 EnHeishi1_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnHeishi1* this = (EnHeishi1*)thisx;

    // turn the guards head to match the direction he is looking
    if (limbIndex == 16) {
        rot->x += (s16)this->headAngle;
    }

    return false;
}

void EnHeishi1_Draw(Actor* thisx, PlayState* play) {
    s32 pad;
    EnHeishi1* this = (EnHeishi1*)thisx;
    Vec3f matrixScale = { 0.3f, 0.3f, 0.3f };

    Gfx_SetupDL_25Opa(play->state.gfxCtx);
    SkelAnime_DrawOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, EnHeishi1_OverrideLimbDraw, NULL,
                      this);
    func_80033C30(&this->actor.world.pos, &matrixScale, 0xFF, play);

    if ((this->path == BREG(1)) && (BREG(0) != 0)) {
        DebugDisplay_AddObject(this->actor.world.pos.x, this->actor.world.pos.y + 100.0f, this->actor.world.pos.z,
                               17000, this->actor.world.rot.y, this->actor.world.rot.z, 1.0f, 1.0f, 1.0f, 255, 0, 0,
                               255, 4, play->state.gfxCtx);
    }
}