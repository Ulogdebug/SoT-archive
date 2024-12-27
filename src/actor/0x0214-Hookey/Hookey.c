#include "Hookey.h"
#include "object/0x020A-HookeyObj.h"

#define FLAGS ACTOR_FLAG_4
#define SEARCH_RAIDUS (50.0f)
#define MOVEBG_POINT_ID(params) ((u16)(params)&0xF)
#define MOVEBG_SPEED(params) (((u16)(params) >> 0x4) & 0xF)
#define MOVEBG_FLAGS(params) ((u16)(params)&0x3F)
#define MOVEBG_PATH_ID(params) (((u16)(params) >> 0x8) & 0xF)
#define MOVEBG_POINT_ID(params) ((u16)(params)&0xF)

void Hookey_Init(Actor* thisx, PlayState* play) ;
void func_8089E650(Hookey* this, PlayState* play) ;
void Hookey_Pasue(Hookey* this, PlayState* play);
void Hookey_Destroy(Hookey* this, PlayState* play);
void Hookey_Update(Hookey* this, PlayState* play);
void Hookey_Draw(Hookey* this, PlayState* play);
s32 func_8089E108(Path* pathList, Vec3f* pos, s32 pathId, s32 pointId);

const ActorInit sHookey_InitVars = {
    .id = 0x0214,
    .category     = ACTORCAT_BG,
    .flags        = 0x00000030,
    .objectId = 0x020A,
    .instanceSize = sizeof(Hookey),
    .init         = (ActorFunc)Hookey_Init,
    .destroy      = (ActorFunc)Hookey_Destroy,
    .update       = (ActorFunc)Hookey_Update,
    .draw         = (ActorFunc)Hookey_Draw
};

static u8 D_8089EE40;

void Hookey_Init(Actor* thisx, PlayState* play) {
    Hookey* this = (Hookey*)thisx;
    CollisionHeader* colHeader = NULL;
    s32 waypointId;

    DynaPolyActor_Init(&this->dyna, DPM_PLAYER);
    CollisionHeader_GetVirtual(&gHookeyObj_CollHookeyObj, &colHeader);
    this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, thisx, colHeader);
    this->searchTimer = 60;
    this->reverse = (this->dyna.actor.params >> 12) & 0xFF;  //Variable Form : *000 (hex)
    this->switchFlags = this->dyna.actor.world.rot.z;
    waypointId = MOVEBG_POINT_ID(thisx->params);//Variable Form : 0*00 (hex)
    func_8089E108(play->setupPathList, &thisx->world.pos, MOVEBG_PATH_ID(thisx->params), waypointId);
    this->dyna.actor.world.rot.z = 0;

    this->actionFunc = func_8089E650;

}

void func_8089E650(Hookey* this, PlayState* play) {
    Vec3f waypoint;
    f32 dist;
    f32 dx;
    f32 dy;
    f32 dz;

    this->dyna.actor.speedXZ = MOVEBG_SPEED(this->dyna.actor.params) * 0.3f; //Variable Form : 00*0 (hex)
    func_8089E108(play->setupPathList, &waypoint, MOVEBG_PATH_ID(this->dyna.actor.params), this->waypointId); //Variable Form : 0*00 (hex)
    dist = Actor_WorldDistXYZToPoint(&this->dyna.actor, &waypoint);
    if (dist < this->dyna.actor.speedXZ) {
        this->dyna.actor.speedXZ = dist;
    }
    func_80035844(&this->dyna.actor.world.pos, &waypoint, &this->dyna.actor.world.rot, 1);
    func_8002D97C(&this->dyna.actor);
    dx = waypoint.x - this->dyna.actor.world.pos.x;
    dy = waypoint.y - this->dyna.actor.world.pos.y;
    dz = waypoint.z - this->dyna.actor.world.pos.z;
    if (fabsf(dx) < 2.0f && fabsf(dy) < 2.0f && fabsf(dz) < 2.0f) {
        this->waypointId++;
        if (this->waypointId >= play->setupPathList[MOVEBG_PATH_ID(this->dyna.actor.params)].count) {
            this->waypointId = 0;
            func_8089E108(play->setupPathList, &this->dyna.actor.world.pos, MOVEBG_PATH_ID(this->dyna.actor.params), 0);
        }
    }
    if (!(D_8089EE40 & 1) && MOVEBG_SPEED(this->dyna.actor.params) != 0) {
        D_8089EE40 |= 1;
        this->sfxFlags |= 1;
    }
    if (this->sfxFlags & 1) {
        func_8002F948(&this->dyna.actor, NA_SE_EV_ROLL_STAND_2 - SFX_FLAG);
    }

    if(this->reverse){
        if(!Flags_GetSwitch(play, this->switchFlags)){
            this->actionFunc = Hookey_Pasue;
        }
    }else{
        if(Flags_GetSwitch(play, this->switchFlags)){
            this->actionFunc = Hookey_Pasue;
        }
    }
}

s32 func_8089E108(Path* pathList, Vec3f* pos, s32 pathId, s32 pointId) {
    Path* path = pathList;
    Vec3s* point;

    path += pathId;
    point = &((Vec3s*)SEGMENTED_TO_VIRTUAL(path->points))[pointId];

    pos->x = point->x;
    pos->y = point->y;
    pos->z = point->z;

    return 0;
}

void Hookey_Pasue(Hookey* this, PlayState* play){
    if(this->reverse){
        if(Flags_GetSwitch(play, this->switchFlags)){
            this->actionFunc = func_8089E650;
        }
    }else{
        if(!Flags_GetSwitch(play, this->switchFlags)){
            this->actionFunc = func_8089E650;
        }
    }
    
}


void Hookey_Destroy(Hookey* this, PlayState* play) {
    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
    if (this->sfxFlags & 1) {
        D_8089EE40 &= ~1;
    }

}

void Hookey_Update(Hookey* this, PlayState* play) {
    // if(this->searchTimer > 0){
    //     if(!this->switchIsOnTop){
    //         if(Actor_FindNearby(play, &this->dyna.actor, ACTOR_OBJ_SWITCH, ACTORCAT_SWITCH, SEARCH_RAIDUS) != NULL){ 
    //             this->switchPos = Actor_FindNearby(play, &this->dyna.actor, ACTOR_OBJ_SWITCH, ACTORCAT_SWITCH, SEARCH_RAIDUS);
    //             this->switchIsOnTop = 1;
    //         }
    //     }
    //     if(!this->torchIsOnTop){
    //         if(Actor_FindNearby(play, &this->dyna.actor, ACTOR_OBJ_SYOKUDAI, ACTORCAT_PROP, SEARCH_RAIDUS) != NULL){ 
    //             this->torchPos = Actor_FindNearby(play, &this->dyna.actor, ACTOR_OBJ_SYOKUDAI, ACTORCAT_PROP, SEARCH_RAIDUS);
    //             this->torchIsOnTop = 1;
    //         }
    //     }
    //     this->searchTimer--;
    // }else{
    //     this->searchTimer = 0;
    // }

    // if(this->torchIsOnTop){
    //     Hookey_TorchPosUpdate(this,play);
    // }
    // if(this->switchIsOnTop){
    //     Hookey_SwitchPosUpdate(this,play);
    // }

    this->actionFunc(this, play);

}

s32 Hookey_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3f* pos, Vec3s* rot, void* thisx) {
    Hookey* this = (void*)thisx;
    
    return 0;
}

void Hookey_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3s* rot, void* thisx) {
    Hookey* this = (void*)thisx;
}

void Hookey_Draw(Hookey* this, PlayState* play) {

    Gfx_DrawDListOpa(play, &gHookeyObj_DlHookeyObj);

}
