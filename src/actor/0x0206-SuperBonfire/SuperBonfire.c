#include "SuperBonfire.h"
#include "object/0x019C-SuperBonfireObj.h"
#include "overlays/actors/ovl_En_Ice_Hono/z_en_ice_hono.h"
#define FLAGS (ACTOR_FLAG_FRIENDLY | ACTOR_FLAG_TARGETABLE | ACTOR_FLAG_NO_UPDATE_CULLING | ACTOR_FLAG_NO_DRAW_CULLING | ACTOR_FLAG_SFX_AT_POS| ACTOR_FLAG_OCARINA_NO_FREEZE )
void SuperBonfire_Init(Actor* thisx, PlayState* play);
void SuperBonfire_Destroy(SuperBonfire* this, PlayState* play);
void SuperBonfire_Update(SuperBonfire* this, PlayState* play);
void SuperBonfire_Draw(SuperBonfire* this, PlayState* play);
void SuperBonfire_InitCollision(Actor* thisx, PlayState* play);
void SuperBonfire_Talk(SuperBonfire* this, PlayState* play);
void SuperBonfire_Transport(SuperBonfire* this, PlayState* play,s16 sEntryID);
void SuperBonfire_Heal(SuperBonfire* this, PlayState* play);
void SuperBonfire_InitCollision(Actor* thisx, PlayState* play);
void SuperBonfire_collisionCheck(SuperBonfire* this, PlayState* play);
void SuperBonfire_SetLightFlag(SuperBonfire* this, PlayState* play);
u8 SuperBonfire_ShouldLightCheck(SuperBonfire* this, PlayState* play);
void SuperBonfire_DoNothing(Actor* this, PlayState* play);

const ActorInit sSuperBonfire_InitVars = {
    .id = 0x0206,
    .category     = ACTORCAT_BG,
    .flags        = FLAGS,
    .objectId = 0x019C,
    .instanceSize = sizeof(SuperBonfire),
    .init         = (ActorFunc)SuperBonfire_Init,
    .destroy      = (ActorFunc)SuperBonfire_Destroy,
    .update       = (ActorFunc)SuperBonfire_Update,
    .draw         = (ActorFunc)SuperBonfire_Draw
};

typedef enum {
    TEMPLE_OF_MEMORIES = 0x36,
    MOBLIN_MINE = 0x68,
    FOREST_VALLEY = 0x09,
    GANON_TOMB = 0x67,
}gEntryName;

typedef enum {
    ACCEPT = 1,
    REJECT = 2,
} choiceText;


OvlMessage gIntroduceText[] = {
    {//0
        .type = MSGBOX_TYPE_BLACK,
        .pos = MSGBOX_POS_BOTTOM,
        .txt =
            "You found a Traveller's Bonfire!"
    },{//1
        .type = MSGBOX_TYPE_BLACK,
        .pos = MSGBOX_POS_VARIABLE,
        .txt =
            "Traveller's Bonfire is lit,\n" 
            "Playing \x05\x01Song of Flame\x05\x00 allows\n"
            "you to warp here instantly."
    },{//2
        .type = MSGBOX_TYPE_BLACK,
        .pos = MSGBOX_POS_BOTTOM,
        .txt =
            "Gazing at it,\n"
            "You feel your soul drift\n" 
            "like smoke in the breeze..."
    },

};

OvlMessage inReality_Select = {
    .type = MSGBOX_TYPE_BLACK,
    .txt =
        "Do you want to light it?\n\n"
        MSG_TWO_CHOICE MSG_COLOR_REG_0 MSG_INSTANT_ON
        "Yes\n"
        "Not yet"
        MSG_COLOR_DEFAULT MSG_INSTANT_OFF
};



static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_METAL,
        AT_NONE,
        AC_ON | AC_TYPE_ALL,
        OC1_ON | OC1_TYPE_PLAYER,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 45, 107, 0, {  0, 0, 0  } },//radius,height
};

static CollisionCheckInfoInit sColChkInfoInit = { 0, 12, 60, MASS_IMMOVABLE};

void SuperBonfire_Init(Actor* thisx, PlayState* play) {
    SuperBonfire* this = (SuperBonfire*)thisx;
    SuperBonfire_InitCollision(thisx,play);
    OvlMessage_Init(&this->actor, false);
    this->shouldLight = 0;
    this->switchFlags = (this->actor.params >> 8) & 0xFF; //Variable Form : **00 (hex)
    this->actor.targetMode = 0;
    this->cutsceneTimer = 5;
    this->healTimer = 20;
    this->toSecFlag = 0;
    if(SuperBonfire_ShouldLightCheck(this,play)){
        this->shouldLight = 1;
    }
    this->actionFunc = SuperBonfire_DoNothing;
}

void SuperBonfire_DoNothing(Actor* this, PlayState* play) {
}

void SuperBonfire_InitCollision(Actor* thisx, PlayState* play){
    SuperBonfire* this = (SuperBonfire*)thisx;
    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
    Collider_UpdateCylinder(&this->actor, &this->collider); 
    CollisionCheck_SetInfo(&thisx->colChkInfo, NULL, &sColChkInfoInit);

}

void SuperBonfire_collisionCheck(SuperBonfire* this, PlayState* play){

    if (this->actor.xzDistToPlayer < 400.0f && this->actor.yDistToPlayer < 400.0f) {
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
    }
    
}

u8 SuperBonfire_ShouldLightCheck(SuperBonfire* this, PlayState* play){
    switch(play->sceneId){
        case MOBLIN_MINE:
            return (CustomFlag_Get(0x1) ? 1 : 0);
        case FOREST_VALLEY:
            return (CustomFlag_Get(0x2) ? 1 : 0);
        case GANON_TOMB:
            return (CustomFlag_Get(0x3) ? 1 : 0);
        default:
            Actor_Kill(&this->actor);
            return;
    }

}


void SuperBonfire_SetLightFlag(SuperBonfire* this,PlayState* play){
    switch(play->sceneId){
        case MOBLIN_MINE:
            CustomFlag_Set(0x1,true);
            break;
        case FOREST_VALLEY:
            CustomFlag_Set(0x2,true);
            break;
        case GANON_TOMB:
            CustomFlag_Set(0x3,true);
            CustomFlag_Set(0x1,true);
            CustomFlag_Set(0x2,true);
            break;
    }
}


void SuperBonfire_Talk(SuperBonfire* this, PlayState* play){
    s8 decision;
    if ((OvlMessage_IsClosed(&this->actor))){
        if(this->shouldLight == 0){
            SuperBonfire_SetLightFlag(this,play);
            OvlMessage_Start(NULL, &gIntroduceText[1]);
        }else{
            this->actionFunc = SuperBonfire_DoNothing;
        }
    }
    // if((decision = OvlMessage_GetChoice(&this->actor))){
    //         switch (decision){
    //             case ACCEPT:
    //                 SuperBonfire_SetLightFlag(this,play);
    //                 OvlMessage_Continue(&this->actor,&gIntroduceText[1]);
    //                 break;
    //             case REJECT:
    //                 this->actionFunc = SuperBonfire_DoNothing;
    //                 break;
    //         }
    // }

}



void SuperBonfire_Update(SuperBonfire* this, PlayState* play) {
    OvlMessage* textToTalk;
    s8 textOrder;
    SuperBonfire_collisionCheck(this,play);
    if(this->shouldLight){
        textOrder = 1;
        if(!this->spawnFlameFlag){
            EnIceHono* Flame = (EnIceHono*)Actor_Spawn(&play->actorCtx, play, ACTOR_EN_ICE_HONO, 
            this->actor.world.pos.x, this->actor.world.pos.y, this->actor.world.pos.z, 0, 0, 0, 0xFFFF);
            this->spawnFlameFlag = 1;
        }
    }else{
        this->shouldLight = SuperBonfire_ShouldLightCheck(this,play);
        textOrder = 0;
    }

    if((ABS(this->actor.yDistToPlayer) < 40.0f)){
        if(OvlMessage_Prompt(&this->actor,&gIntroduceText[textOrder],120,0) > 0){
            OvlMessage_RegisterColor(0,0,255,0);
            this->actionFunc = SuperBonfire_Talk;
        }
        
    }
    this->actionFunc(this, play);
}

void SuperBonfire_Destroy(SuperBonfire* this, PlayState* play){
    //DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
    Collider_DestroyCylinder(play, &this->collider);

}


s32 SuperBonfire_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3f* pos, Vec3s* rot, void* thisx) {
    SuperBonfire* this = (void*)thisx;
    
    return 0;
}

void SuperBonfire_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3s* rot, void* thisx) {
    SuperBonfire* this = (void*)thisx;
}

void SuperBonfire_Draw(SuperBonfire* this, PlayState* play) {
    Gfx_DrawDListOpa(play, &gSuperBonfireObj_DlSuperBonfireObj);
}
