#include <uLib.h>
#include "object/0x0204-SoliderObj.h"
#include "Solider.h"
#include "vt.h"


#define FLAGS (ACTOR_FLAG_0 | ACTOR_FLAG_3)

void Solider_Init(Actor* thisx, PlayState* play);
void Solider_Destroy(Actor* thisx, PlayState* play);
void Solider_Update(Actor* thisx, PlayState* play);
void Solider_Draw(Actor* thisx, PlayState* play);
void Solider_Idle(Solider* this, PlayState* play);
void Solider_SetupTalk(Solider* this, PlayState* play);
void Solider_CollisionCheck(Solider* this, PlayState* play) ;
void Solider_UpdateAnimation(Actor* thisx, PlayState* play) ;
void Solider_Talk(Actor* thisx, PlayState* play);

OvlMessage gIntroduceText = {
    .type = MSGBOX_TYPE_BLACK,
    .pos = MSGBOX_POS_BOTTOM,
    .txt =
    "I AM ERROR"
};

const ActorInit En_Heishi4_InitVars = {
    0x020E,
    ACTORCAT_NPC,
    FLAGS,
    0x0204,//Object ID
    sizeof(Solider),
    (ActorFunc)Solider_Init,
    (ActorFunc)Solider_Destroy,
    (ActorFunc)Solider_Update,
    (ActorFunc)Solider_Draw,
};

static u32 sFaceReactionSets[] = { 6, 7 };

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

void Solider_Init(Actor* thisx, PlayState* play) {
    Solider* this = (Solider*)thisx;
    OvlMessage_Init(&this->actor, false);
    //Actor_SetScale(thisx, 0.01f);
    this->type = thisx->params & 0xFF;
    thisx->colChkInfo.mass = MASS_IMMOVABLE;
    this->pos = thisx->world.pos;
    thisx->targetMode = 8;
    this->height = 120.0f;
    ActorShape_Init(&thisx->shape, 0.0f, NULL, 30.0f);
    SkelAnime_InitFlex(play, &this->skelAnime, &gSolider_SkelSoliderObj, &gSolider_AnimIdle, this->jointTable,
                   this->morphTable, 18);
    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinder(play, &this->collider, thisx, &sCylinderInit);
    this->collider.dim.yShift = 0;
    this->collider.dim.radius = 15;
    this->collider.dim.height = 70;
    this->cutsceneTimer = 8;
    this->actionFunc = Solider_Idle;
    this->unk_27C = ((thisx->params >> 8) & 0xFF);
}

void Solider_Destroy(Actor* thisx, PlayState* play) {
    Solider* this = (Solider*)thisx;

    Collider_DestroyCylinder(play, &this->collider);
}

void Solider_SetupTalk(Solider* this, PlayState* play){
    f32 frames = Animation_GetLastFrame(&gSolider_AnimTalk);
    Animation_Change(&this->skelAnime, &gSolider_AnimTalk, 0.5f, 0.0f, (s16)frames, ANIMMODE_ONCE, 6.0f);
    this->actionFunc = Solider_Talk;
}


void Solider_Idle(Solider* this, PlayState* play) {
    f32 frames = Animation_GetLastFrame(&gSolider_AnimIdle);
    Animation_Change(&this->skelAnime, &gSolider_AnimIdle, 1.0f, 0.0f, (s16)frames, ANIMMODE_LOOP, 6.0f);
    this->actionFunc = Solider_UpdateAnimation;
}

void Solider_UpdateAnimation(Actor* thisx, PlayState* play) {
    Solider* this = (Solider*)thisx;
    SkelAnime_Update(&this->skelAnime);
}

void Solider_Talk(Actor* thisx, PlayState* play) {
    Solider* this = (Solider*)thisx;
    f32 frames = Animation_GetLastFrame(&gSolider_AnimIdle);
    if(SkelAnime_Update(&this->skelAnime)){
        Animation_Change(&this->skelAnime, &gSolider_AnimIdle, 1.0f, 0.0f, (s16)frames, ANIMMODE_LOOP, 6.0f);
    }
    if(OvlMessage_IsCurrentMessage(&gIntroduceText)){
         if(OvlMessage_IsClosed(&this->actor)){
         this->actor.update = (ActorFunc)Solider_Update;
         }
    }
}

void Solider_Update(Actor* thisx, PlayState* play) {
    Solider* this = (Solider*)thisx;
    s32 pad;
    Player* player = GET_PLAYER(play);

    thisx->world.pos.x = this->pos.x;
    thisx->world.pos.y = this->pos.y;
    thisx->world.pos.z = this->pos.z;
    if((this->actor.xzDistToPlayer) < 150.0f){
        this->unk_28C.unk_18 = player->actor.world.pos;
        func_80034A14(thisx, &this->unk_28C, 2, 4);
        this->headRot = this->unk_28C.unk_08;
        this->torsoRot = this->unk_28C.unk_0E;
     }
     Actor_SetFocus(thisx, this->HeadPos.y);
    if(ABS(this->actor.yDistToPlayer) < 50.0f){
        if(OvlMessage_Prompt(&this->actor,&gIntroduceText,150,0) > 0){
            Actor_TrackPlayer(play, &this->actor, &this->headRot, &this->torsoRot, this->actor.focus.pos);
            OvlMessage_RegisterColor(0,0,255,0);
            this->actor.update = (ActorFunc)Solider_Talk;
        }
     }

    this->actionFunc(this, play);
    //Actor_MoveForward(thisx);
     Actor_UpdateBgCheckInfo(play, thisx, 10.0f, 10.0f, 30.0f,
                             UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2 | UPDBGCHECKINFO_FLAG_3 |
                                 UPDBGCHECKINFO_FLAG_4);
    Collider_UpdateCylinder(&this->actor, &this->collider);
    Solider_CollisionCheck(this, play);

}

void Solider_CollisionCheck(Solider* this, PlayState* play) {
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
    if (this->collider.base.acFlags & AC_HIT){
        this->collider.base.acFlags &= ~AC_HIT;
    }else{
         CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
    }
}




s32 Solider_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    Solider* this = (Solider*)thisx;
    Vec3f NullOffset = { 0.0f, 0.0f, 0.0f };
    if (limbIndex ==  SOLIDEROBJ_LIMB_HEAD) {
        Matrix_MultVec3f(&NullOffset, &this->HeadPos);
        if((this->actor.xzDistToPlayer) < 150.0f){
        rot->x += this->headRot.y;
        rot->z += this->headRot.z;
        }  
    }
    if (limbIndex == SOLIDEROBJ_LIMB_TORSO) {
        if((this->actor.xzDistToPlayer) < 150.0f){
        rot->x += this->torsoRot.y;
        }
    }
    return false;
}


void Solider_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3s* rot, void* thisx) {
}

void Solider_Draw(Actor* thisx, PlayState* play) {
    Solider* this = (Solider*)thisx;

    Gfx_SetupDL_25Opa(play->state.gfxCtx);
    SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->jointTable, 18,Solider_OverrideLimbDraw, Solider_PostLimbDraw,
                      this);
}
