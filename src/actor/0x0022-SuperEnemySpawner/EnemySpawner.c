#include "EnemySpawner.h"
#include "overlays/actors/ovl_Door_Warp1/z_door_warp1.h"

void EnemySpawner_Init(EnemySpawner* this, PlayState* play);
void EnemySpawner_Update(EnemySpawner* this, PlayState* play);
s16 EnemySpawner_IfGetFlag(EnemySpawner* this,PlayState* play);
void EnemySpawner_DetectClear(EnemySpawner* this, PlayState* play);
void EnemySpawner_Wait(EnemySpawner* this, PlayState* play);

#define FLAGS (ACTOR_FLAG_FRIENDLY | ACTOR_FLAG_NO_UPDATE_CULLING )

const ActorInit sEnemySpawner_InitVars = {
    .id = 0x0022,
    .category     = ACTORCAT_SWITCH,
    .flags        = FLAGS,
    .objectId = OBJECT_GAMEPLAY_KEEP,
    .instanceSize = sizeof(EnemySpawner),
    .init         = (ActorFunc)EnemySpawner_Init,
    .destroy      = NULL,
    .update       = (ActorFunc)EnemySpawner_Update,
    .draw         = NULL
};

typedef struct {
    s16 enemyID;
    s16 params;
    s16 zRotation; //For special useage
} enemyInfo;

static enemyInfo enemyInfoTable[] = {
    {0x0090,0},         //Redead
    {0x0090,0x0080,0},  //Gibdo
    {0x0220,0,0},       //Iron Redead, screaming
    {0x0220,0x0080,0},  //Iron Redead, cheasing
    {0x008E,0,0},       //Big Floormaster
    {0x008E,0x30,0},    //Small Floormaster
    {0x0011,0x1,0},     //Wallmaster
    {0x00DD,0,0},       //Like Like
    {0x025A,0x0,1},     //Garo, mini Boss music
    {0x025B,0x3F3F,0},  //Dexihand
    {0x0222,0,0},       //Rat
    {0x0035,0xFFFF,0},  //Tailparaisan
    {0x0002,0x0002,0},  //Stalfos, rising from ground
    {0x01AF,0x0,0},  //Grey Wolfo, rising from ground
    {0x01AF,0x0001,0},  //White Wolfo, mini boss music
    {0x0121,0xFFFF,0},  //Freezard,mobile
    {0x0121,0x0,0},     //Freezard,immobile
    {0x0003,0xFF03,0},  //Iron Guard
    {0x00A5,0x0,0},  //Dead Hand's Arm
    {0x0215,0x0,0},  //Super FireLizard
    {0x005D,WARP_DUNGEON_ADULT,0},  //Blue Warp
    {0x0217,0x0,0},  //Teleporter

};

void EnemySpawner_Init(EnemySpawner* this, PlayState* play) {
    this->flagType = this->actor.home.rot.x; //1 chest, 2 collectible, 3 global, the others are switch
    this->flag = (this->actor.params >> 8) & 0x3F;
    this->enemyType = (this->actor.params) & 0xFF;
    if(Flags_GetTempClear(play, this->actor.room) && EnemySpawner_IfGetFlag(this,play)){
        Actor_Kill(&this->actor);
    }
    this->actionFunc = EnemySpawner_Wait;
}

s16 EnemySpawner_IfGetFlag(EnemySpawner* this, PlayState* play) {
    switch (this->flagType) {
        case 1 : //CHEST
             return Flags_GetTreasure(play, this->flag)? 1 : 0;
        case 2 : //COLLECTIBLE
             return Flags_GetCollectible(play, this->flag)? 1 : 0;
        case 3 : //GLOBAL
             return Flags_GetEventChkInf(this->flag)? 1 : 0;
        default:
            return Flags_GetSwitch(play, this->flag)? 1 : 0;
    }
    return false;
}

void EnemySpawner_Wait(EnemySpawner* this, PlayState* play) {
    Debug_Text(255,255,255,1,5," Waiting for %x spawned. Params: %X", this->enemyType,enemyInfoTable[this->enemyType].params);
    if(EnemySpawner_IfGetFlag(this,play) == 1 || (this->flag == 0x3F && this->flagType != 3)){
        Actor_Spawn(&play->actorCtx, play, enemyInfoTable[this->enemyType].enemyID, this->actor.home.pos.x, this->actor.home.pos.y,
        this->actor.home.pos.z, 0, this->actor.shape.rot.y,enemyInfoTable[this->enemyType].zRotation, enemyInfoTable[this->enemyType].params);
        this->actionFunc = EnemySpawner_DetectClear;
    }
}

void EnemySpawner_Update(EnemySpawner* this, PlayState* play) {
    this->actionFunc(this, play);
}

void EnemySpawner_DetectClear(EnemySpawner* this, PlayState* play) {
    Actor_Kill(&this->actor);
}
