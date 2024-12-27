/**
 * by Nokaubure

Variable

& 0x4000 : 0 Use Rotation as waypoint, 1 Use pathway
& 0x8000 : 0 Stop after reaching last point, return on off, 1 Continue till turned off
& 0x3000 : 0 Never stop, 1 Stop 1 second at each extreme of the pathway
& 0x0200 : 0 never stops when moving back and forth, 1 stops 1 second
& 0x01C0 : 0 red, 1 blue (reverse), 2 green, 3 yellow (reverse), 4 purple, 5 orange (reverse), 6 white, 7 white
& 0x003F : switch flag

Zrot

& 0x0F00 : pathway ID (in pathway mode)
& 0x00FF : starting waypoint (in pathway mode)

in pathway mode, Xrot and Yrot are usable


 **/


#define AVAL(base,type,offset)  (*(type*)((u8*)(base)+(offset)))
#define SEARCH_RAIDUS (30.0f)

#include "z64math.h"

#include "ultra64.h"
#include "global.h"

#include "macros.h"

#include "object/0x0080-SwitchPlatform.h"

#define OBJ_ID         0x80      
#define ACT_ID         0xCE     
#define PAD         0x0     
#define STATE_OFF  0
#define STATE_ON   1
#define TYPE_ROT 0
#define TYPE_PATH 1
#define BEH_STOP 0
#define BEH_CONTINUE 1

const u32 Colors[] = 
{
	0xFF2B00FF, // Red
	0x6A00FFFF, // Blue
	0x00FF26FF, // Green
	0xFFEC00FF, // Yellow
    0xFFFFFFFF, // White
    0xFFFFFFFF, // White
    0x90A9C0FF, // Blueish
    0x90A9C0FF, // Blueish
};

const f32 SpeedArray[] = {0.5f, 1.0f, 4.0f, 8.0f};


typedef struct {
	DynaPolyActor dyna;
    Actor* torchPos;
    Actor* switchPos;
    Actor* teleporterPos;
    s16 searchTimer;
    s8 switchIsOnTop;
    s8 torchIsOnTop;
    s8 teleporterIsOnTop;
	u8 type;
    u8 behaviour;
	u8 pathwayID;
	u8 speed;
	u8 color;
	u8 inverted;
    s8 waypointID;
	u16 switchflag;
	Vec3f targetpos;
	u8 orientation;
	u8 moving;
	s8 state;
    s8 endtimer;
    Path* pathpointer;
    Path localpath;
    Vec3s localpoints[2];



} SwitchPlatform;





s32 CheckSwitch(SwitchPlatform *en, PlayState *play)
{

    s32 flag = Flags_GetSwitch(play, en->switchflag);
    return ((!flag && en->state != STATE_OFF && !en->inverted) ||
        (flag && en->state == STATE_OFF && !en->inverted) ||
        (!flag && en->state == STATE_OFF && en->inverted) ||
        (flag && en->state != STATE_OFF && en->inverted));
}

void GetPathpointPos(Path* pathList, Vec3f* pos, s32 pathId, s32 pointId) {
    Path* path = pathList;
    Vec3s* point;

    path += pathId;
    point = &((Vec3s*)SEGMENTED_TO_VIRTUAL(path->points))[pointId];

    pos->x = point->x;
    pos->y = point->y;
    pos->z = point->z;

}

void init(SwitchPlatform *en, PlayState *play) {

    //Actor_SetScale(&en->dyna.actor, 0.1f);

    en->dyna.actor.uncullZoneForward = 3500.0f;



	en->type = (en->dyna.actor.params & 0x4000) >> 14;
	en->behaviour = (en->dyna.actor.params & 0x8000) >> 15;
    en->speed = (en->dyna.actor.params & 0x3000) >> 12;
    en->endtimer = ((en->dyna.actor.params & 0x0200) >> 9) ? 0 : -1;
	en->pathwayID = (en->type == TYPE_ROT) ? 0 : ((en->dyna.actor.home.rot.z & 0x0F00) >> 8);
    en->waypointID = (en->type == TYPE_ROT) ? 0 : (en->dyna.actor.home.rot.z & 0x00FF);
	en->color = (en->dyna.actor.params & 0x01C0) >> 6;
	en->switchflag = (en->dyna.actor.params & 0x003F);
	en->inverted = (en->color%2 == 0) ? 0 : 1;
    en->moving = 0;
    en->state = 0;
    en->orientation = 1;
    en->searchTimer = 60;



    CollisionHeader* colHeader = NULL;
    DynaPolyActor_Init(&en->dyna, DYNA_INTERACT_ACTOR_ON_TOP);
    CollisionHeader_GetVirtual((void*)gSwitchPlatformObj_CollSwitchPlatformObj,&colHeader);
    en->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna,&en->dyna.actor, colHeader);



    if (en->type != TYPE_PATH)
    {
        en->pathpointer = &en->localpath;
        en->pathpointer->count = 2;
        en->pathpointer->points = en->localpoints;

        en->localpoints[0].x = en->dyna.actor.home.pos.x;
        en->localpoints[0].y = en->dyna.actor.home.pos.y;
        en->localpoints[0].z = en->dyna.actor.home.pos.z;

        en->localpoints[1].x = en->dyna.actor.home.rot.x;
        en->localpoints[1].y = en->dyna.actor.home.rot.y;
        en->localpoints[1].z = en->dyna.actor.home.rot.z;


    }
    else
    {

        en->pathpointer = play->setupPathList;

    }

    GetPathpointPos(en->pathpointer, &en->targetpos, en->pathwayID, (en->type == TYPE_ROT) ? 0 : en->waypointID);


    en->dyna.actor.world.rot.z = 0;
    en->dyna.actor.shape.rot.z = 0;

    if (en->type == TYPE_ROT)
    {
        en->dyna.actor.shape.rot.x = 0;
        en->dyna.actor.shape.rot.y = 0;
        en->dyna.actor.world.rot.x = 0;
        en->dyna.actor.world.rot.y = 0;
    }

    if (en->inverted && en->behaviour == BEH_STOP) {en->orientation = -1; en->waypointID = 1;}


    if (en->behaviour == BEH_STOP)
    {
    s32 flag = Flags_GetSwitch(play, en->switchflag);
    if (((flag && !en->inverted) ||
        (!flag && en->inverted))) {


        if (en->type == TYPE_ROT) GetPathpointPos(en->pathpointer, &en->targetpos, en->pathwayID, 1);
        else GetPathpointPos(en->pathpointer, &en->targetpos, en->pathwayID, en->pathpointer[en->pathwayID].count-1);

        en->dyna.actor.world.pos.x = en->targetpos.x;
        en->dyna.actor.world.pos.y = en->targetpos.y;
        en->dyna.actor.world.pos.z = en->targetpos.z;
    

        en->state = STATE_ON;
    }
    else
        en->state = STATE_OFF;

    }



}



void update(SwitchPlatform *en, PlayState *play) {

    if(en->searchTimer > 0){
        if(!en->switchIsOnTop){
            if(Actor_FindNearby(play, &en->dyna.actor, ACTOR_OBJ_SWITCH, ACTORCAT_SWITCH, SEARCH_RAIDUS) != NULL){ 
                en->switchPos = Actor_FindNearby(play, &en->dyna.actor, ACTOR_OBJ_SWITCH, ACTORCAT_SWITCH, SEARCH_RAIDUS);
                en->switchIsOnTop = 1;
            }
        }
        if(!en->torchIsOnTop){
            if(Actor_FindNearby(play, &en->dyna.actor, ACTOR_OBJ_SYOKUDAI, ACTORCAT_PROP, SEARCH_RAIDUS) != NULL){ 
                en->torchPos = Actor_FindNearby(play, &en->dyna.actor, ACTOR_OBJ_SYOKUDAI, ACTORCAT_PROP, SEARCH_RAIDUS);
                en->torchIsOnTop = 1;
            }
        }
        if(!en->teleporterIsOnTop){
            if(Actor_FindNearby(play, &en->dyna.actor, 0x0217, ACTORCAT_BG, SEARCH_RAIDUS) != NULL){ 
                en->teleporterPos = Actor_FindNearby(play, &en->dyna.actor, 0x0217, ACTORCAT_BG, SEARCH_RAIDUS);
                en->teleporterIsOnTop = 1;
            }
        }
        en->searchTimer--;
    }else{
        en->searchTimer = 0;
    }

    if(en->torchIsOnTop){
        TorchPosUpdate(en,play);
    }
    if(en->switchIsOnTop){
        SwitchPosUpdate(en,play);
    }
    if(en->teleporterIsOnTop){
        TeleporterPosUpdate(en,play);
    }
    if (en->moving == 0)
    {
        s32 flag = CheckSwitch(en, play);
        if (flag)
        {

            if (en->behaviour == BEH_STOP) en->state = (en->state == STATE_ON) ? STATE_OFF : STATE_ON;
            en->moving = 1;

        }
    }
    else
    {
        func_8002F948(&en->dyna.actor, NA_SE_EV_ROLL_STAND_2 - SFX_FLAG);
        if (en->endtimer < 1)
        {
        en->dyna.actor.speedXZ = SpeedArray[en->speed];
        
        f32 dist = Actor_WorldDistXYZToPoint(&en->dyna.actor, &en->targetpos);
        if (dist < en->dyna.actor.speedXZ) {
           // en->dyna.actor.speedXZ = dist;
            en->dyna.actor.world.pos =  en->targetpos;
        }
        else
        {
            func_80035844(&en->dyna.actor.world.pos, &en->targetpos, &en->dyna.actor.world.rot, 1);
            func_8002D97C(&en->dyna.actor);
        }

        
        f32 dx = en->targetpos.x - en->dyna.actor.world.pos.x;
        f32 dy = en->targetpos.y - en->dyna.actor.world.pos.y;
        f32 dz = en->targetpos.z - en->dyna.actor.world.pos.z;
        if (fabsf(dx) < 2.0f && fabsf(dy) < 2.0f && fabsf(dz) < 2.0f) {
            en->waypointID += en->orientation;
            if (en->waypointID >= en->pathpointer[en->pathwayID].count) {
                en->waypointID -= 2;
                en->orientation = -en->orientation;
                if (en->behaviour == BEH_STOP) en->moving = 0;
                if (en->endtimer != -1) en->endtimer = 20;
                
            }
            else if (en->waypointID < 0) {
                en->waypointID = 1;
                en->orientation = -en->orientation;
                if (en->behaviour == BEH_STOP) en->moving = 0;
                if (en->endtimer != -1) en->endtimer = 20;
                
            }

            GetPathpointPos(en->pathpointer, &en->targetpos, en->pathwayID, en->waypointID);

        }


        }
        else en->endtimer--;

        if (!CheckSwitch(en, play) && en->behaviour != BEH_STOP)
        {
            en->moving = 0;
        }
}


}

static void dest(SwitchPlatform *en, PlayState *play) {

	 DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, en->dyna.bgId);

}


static void draw(SwitchPlatform *en, PlayState *play) {

        u32 colorDL;

        OPEN_DISPS(play->state.gfxCtx, "", 1);


        Gfx_SetupDL_25Opa(play->state.gfxCtx);

        gDPSetEnvColor(POLY_OPA_DISP++, (Colors[en->color] & 0xFF000000) >> 24, (Colors[en->color] & 0x00FF0000) >> 16, (Colors[en->color] & 0x0000FF00)>> 8, 0xFF);


        if(play->sceneId != 0x6){
            Gfx_DrawDListOpa(play,(Gfx*)gSwitchPlatformObj_DlSwitchPlatformDemonObj);
        }else{
            Gfx_DrawDListOpa(play,(Gfx*)gSwitchPlatformObj_DlSwitchPlatformObj);
        }

        gDPSetEnvColor(POLY_OPA_DISP++, 0,0,0,0);

        CLOSE_DISPS(play->state.gfxCtx, "", 2);


	
}

void TorchPosUpdate(SwitchPlatform *en, PlayState* play){
    en->torchPos->world.pos.x = en->dyna.actor.world.pos.x;
    en->torchPos->world.pos.z = en->dyna.actor.world.pos.z;
    en->torchPos->world.pos.y = en->dyna.actor.world.pos.y + 34.0f;

}

void SwitchPosUpdate(SwitchPlatform *en, PlayState* play){
    en->switchPos->world.pos.x = en->dyna.actor.world.pos.x;
    en->switchPos->world.pos.z = en->dyna.actor.world.pos.z;
    en->switchPos->world.pos.y = en->dyna.actor.world.pos.y + 34.0f;
}

void TeleporterPosUpdate(SwitchPlatform *en, PlayState* play){
    en->teleporterPos->world.pos.x = en->dyna.actor.world.pos.x;
    en->teleporterPos->world.pos.z = en->dyna.actor.world.pos.z;
    en->teleporterPos->world.pos.y = en->dyna.actor.world.pos.y + 34.0f;
    en->teleporterPos->home.pos.y = en->dyna.actor.world.pos.y;
}

/* .data */
const ActorInit init_vars = {
	ACT_ID, 
	ACTORCAT_BG, 
	0x00000010,
	OBJ_ID,
	sizeof(SwitchPlatform),
	(ActorFunc)init,
	(ActorFunc)dest,
	(ActorFunc)update,
	(ActorFunc)draw
};







