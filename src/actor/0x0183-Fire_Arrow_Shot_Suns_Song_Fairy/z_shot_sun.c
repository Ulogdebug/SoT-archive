/*
 * File: z_shot_sun.c
 * Overlay: ovl_Shot_Sun
 * Description: Lake Hylia Sun hitbox and Song of Storms Fairy spawner
 */
#include <uLib.h>
#include "z_shot_sun.h"
#include "overlays/actors/ovl_Demo_Kankyo/z_demo_kankyo.h"
#include "overlays/actors/ovl_En_Elf/z_en_elf.h"
#include "assets/scenes/overworld/spot06/spot06_scene.h"
#include "vt.h"

#define FLAGS (ACTOR_FLAG_NO_UPDATE_CULLING)
#define GET_SWITCH_FLAG(this) ((this->actor.params >> 8) & 0x3F)

void ShotSun_Init(Actor* thisx, PlayState* play);
void ShotSun_Destroy(Actor* thisx, PlayState* play);
void ShotSun_Update(Actor* thisx, PlayState* play);
void ShotSun_ChangeEnv(Actor* thisx, PlayState* play);
void ShotSun_SpawnFairy(ShotSun* this, PlayState* play);
void ShotSun_TriggerFairy(ShotSun* this, PlayState* play);
void func_80BADF0C(ShotSun* this, PlayState* play);
void ShotSun_UpdateHyliaSun(ShotSun* this, PlayState* play);
void ShotSun_SetupCountDownAndKill(ShotSun* this, PlayState* play);
void ShotSun_CountDownAndKill(ShotSun* this, PlayState* play);

const ActorInit Shot_Sun_InitVars = {
    ACTOR_SHOT_SUN,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(ShotSun),
    (ActorFunc)ShotSun_Init,
    (ActorFunc)ShotSun_Destroy,
    (ActorFunc)ShotSun_Update,
    NULL,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { DMG_ARROW_ICE | DMG_ARROW_NORMAL, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 30, 60, 0, { 0, 0, 0 } },
};

void ShotSun_Init(Actor* thisx, PlayState* play) {
    ShotSun* this = (ShotSun*)thisx;
    s32 params;
    Player* player = GET_PLAYER(play);
    // "Ocarina secret occurrence"
    osSyncPrintf("%d ---- オカリナの秘密発生!!!!!!!!!!!!!\n", this->actor.params);
    params = this->actor.params & 0xFF;
    if (params == 0x40 || params == 0x41) {
        this->unk_1A4 = 0;
        this->actor.flags |= ACTOR_FLAG_4;
        this->actor.flags |= ACTOR_FLAG_25;
        this->actionFunc = func_80BADF0C;
        this->actor.flags |= ACTOR_FLAG_27;
    } else if(INV_CONTENT(ITEM_ARROW_FIRE) == ITEM_ARROW_FIRE && !CHECK_QUEST_ITEM(QUEST_MEDALLION_SPIRIT)){
        gSaveContext.skyboxTime = gSaveContext.dayTime = CLOCK_TIME(3, 20);
        Actor_Spawn(&play->actorCtx, play, 0x003B, this->actor.home.pos.x, this->actor.home.pos.y,this->actor.home.pos.z, 0, 0, 0, 0x000E);
        //Audio_PlayFanfare(0x68);
        func_800F5ACC(0x68);
        this->actionFunc = ShotSun_ChangeEnv;
    }else{
        Collider_InitCylinder(play, &this->collider);
        Collider_SetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
        this->actionFunc = ShotSun_UpdateHyliaSun;
        this->actor.flags &= ~ACTOR_FLAG_0;
    }
}

void ShotSun_ChangeEnv(Actor* thisx, PlayState* play) {
    ShotSun* this = (ShotSun*)thisx;
    Player* player = GET_PLAYER(play);
    play->envCtx.lightSettingOverride = 1;
    play->envCtx.lightBlendRateOverride = 40;
}

void ShotSun_Destroy(Actor* thisx, PlayState* play) {
    ShotSun* this = (ShotSun*)thisx;
    s32 params = this->actor.params & 0xFF;
    //Audio_QueueSeqCmd(0x1 << 28 | SEQ_PLAYER_FANFARE << 24 | 0xFF);
    if (params != 0x40 && params != 0x41) {
        Collider_DestroyCylinder(play, &this->collider);
    }
}

void ShotSun_SpawnFairy(ShotSun* this, PlayState* play) {
    s32 params = this->actor.params & 0xFF;
    s32 fairyType;

    if (this->timer > 0) {
        this->timer--;
    } else {
        switch (params) {
            case 0x40:
                fairyType = FAIRY_HEAL_BIG;
                break;
            case 0x41:
                fairyType = FAIRY_HEAL_BIG;
                break;
        }

        //! @bug fairyType may be uninitialized
        Actor_Spawn(&play->actorCtx, play, ACTOR_EN_ELF, this->actor.home.pos.x, this->actor.home.pos.y,
                    this->actor.home.pos.z, 0, 0, 0, fairyType);

        Actor_Kill(&this->actor);
    }
}

void ShotSun_TriggerFairy(ShotSun* this, PlayState* play) {
    if ((func_8005B198() == this->actor.category) || (this->timer != 0)) {
        this->actionFunc = ShotSun_SpawnFairy;
        this->timer = 50;

        Actor_Spawn(&play->actorCtx, play, ACTOR_DEMO_KANKYO, this->actor.home.pos.x, this->actor.home.pos.y,
                    this->actor.home.pos.z, 0, 0, 0, DEMOKANKYO_SPARKLES);

        func_80078914(&this->actor.projectedPos, NA_SE_EV_TRE_BOX_APPEAR);
    }
}

void func_80BADF0C(ShotSun* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 pad;
    s32 params = this->actor.params & 0xFF;

    if (Math3D_Vec3fDistSq(&this->actor.world.pos, &player->actor.world.pos) > SQ(150.0f)) {
        this->unk_1A4 = 0;
    } else {
        if (this->unk_1A4 == 0) {
            if (!(player->stateFlags2 & PLAYER_STATE2_24)) {
                player->stateFlags2 |= PLAYER_STATE2_23;
                return;
            } else {
                this->unk_1A4 = 1;
            }
        }
        if (this->unk_1A4 == 1) {
            func_8010BD58(play, OCARINA_ACTION_FREE_PLAY);
            this->unk_1A4 = 2;
        } else if (this->unk_1A4 == 2 && play->msgCtx.ocarinaMode == OCARINA_MODE_04) {
            if ((params == 0x40 && play->msgCtx.lastPlayedSong == OCARINA_SONG_SUNS) ||
                (params == 0x41 && play->msgCtx.lastPlayedSong == OCARINA_SONG_STORMS)) {
                this->actionFunc = ShotSun_TriggerFairy;
                OnePointCutscene_Attention(play, &this->actor);
                this->timer = 0;
            } else {
                this->unk_1A4 = 0;
            }
            this->unk_1A4 = 0;
        }
    }
}


void ShotSun_CountDownAndKill(ShotSun* this, PlayState* play) {
    if (this->timer > 0) {
        Rumble_Request(this->actor.xyzDistToPlayerSq, this->timer * 3, 10, 100);
        this->timer--;
    } else {
        Flags_UnsetSwitch(play, (this->actor.params >> 8) & 0x3F);
        Actor_Kill(&this->actor);
    }
    
}

void ShotSun_SetupCountDownAndKill(ShotSun* this, PlayState* play) {
    this->timer = 120;

    s32 quakeIndex;
    quakeIndex = Quake_Add(GET_ACTIVE_CAM(play), 1);
    Quake_SetSpeed(quakeIndex, 50000);
    Quake_SetQuakeValues(quakeIndex, 2, 0, 0, 0);
    Quake_SetCountdown(quakeIndex, this->timer);
    Flags_SetSwitch(play, (this->actor.params >> 8) & 0x3F);
    this->actionFunc = ShotSun_CountDownAndKill;
    
}

void ShotSun_UpdateHyliaSun(ShotSun* this, PlayState* play) {
    Vec3s cylinderPos;
    Player* player = GET_PLAYER(play);
    EnItem00* collectible;
    s32 pad;
    Vec3f spawnPos;
    s32 iceArrowHit = (this->collider.info.acHitInfo->toucher.dmgFlags & DMG_ARROW_ICE);
    if (this->collider.base.acFlags & AC_HIT) {
        this->collider.base.acFlags &= ~AC_HIT;
        spawnPos.x = this->actor.home.pos.x;
        spawnPos.y = this->actor.home.pos.y + 150.0f;
        spawnPos.z = this->actor.home.pos.z;
        if(iceArrowHit){
            func_80078884(NA_SE_SY_CORRECT_CHIME);
            osSyncPrintf(VT_FGCOL(CYAN) "SHOT_SUN HIT!!!!!!!\n" VT_RST);
            if (INV_CONTENT(ITEM_ARROW_FIRE) == ITEM_NONE) {
                Actor_Spawn(&play->actorCtx, play, ACTOR_ITEM_ETCETERA, spawnPos.x, spawnPos.y, spawnPos.z, 0, 0, 0, 7);
                Actor_Spawn(&play->actorCtx, play, 0x003B, this->actor.home.pos.x, this->actor.home.pos.y,
                    this->actor.home.pos.z, 0, 0, 0, 0x000E);
                //play->csCtx.segment = SEGMENTED_TO_VIRTUAL(gLakeHyliaFireArrowsCS);
                //gSaveContext.cutsceneTrigger = 1;
            } 
            ShotSun_SetupCountDownAndKill(this,play);
        }else {
            Rumble_Request(this->actor.xyzDistToPlayerSq, 120, 10, 120);
            if(this->collectibleCount < 3){
                collectible = Item_DropCollectible(play, &spawnPos, ITEM00_MAGIC_LARGE);
                if (collectible != NULL) {
                    this->collectibleCount++;
                    collectible->despawnTimer = 6000;
                    collectible->actor.speedXZ = 0.0f;
                }
            }else{
                func_80078884(NA_SE_SY_ERROR);
            }

        }
         
    } else {
        if ((this->actor.xzDistToPlayer < 300.0f) && (this->actor.yDistToPlayer < 200.0f) /*&& gSaveContext.dayTime >= CLOCK_TIME(6, 30) &&
            gSaveContext.dayTime < CLOCK_TIME(7, 30)*/) {
            cylinderPos.x = player->bodyPartsPos[PLAYER_BODYPART_HEAD].x + play->envCtx.sunPos.x * (1.0f / 6.0f);
            cylinderPos.y =
                player->bodyPartsPos[PLAYER_BODYPART_HEAD].y - 30.0f + play->envCtx.sunPos.y * (1.0f / 6.0f);
            cylinderPos.z = player->bodyPartsPos[PLAYER_BODYPART_HEAD].z + play->envCtx.sunPos.z * (1.0f / 6.0f);

            this->hitboxPos = cylinderPos;

            Collider_SetCylinderPosition(&this->collider, &cylinderPos);
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
        }
    }
}

void ShotSun_Update(Actor* thisx, PlayState* play) {
    ShotSun* this = (ShotSun*)thisx;

    this->actionFunc(this, play);
}
