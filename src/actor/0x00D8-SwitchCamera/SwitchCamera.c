/**
 * by Nokaubure

Variable

& 0x0100 : 0 instant, 1 smooth transition (unfinished!)
& 0x1000 : 1 discovery sound
& 0x003F : switch flag

Zrot = time in seconds / 10 (1 unit = 0.1 seconds), defaults at 1 second

 **/


#define AVAL(base,type,offset)  (*(type*)((u8*)(base)+(offset)))

#include "z64math.h"

#include "ultra64.h"
#include "global.h"

#include "macros.h"


#define OBJ_ID         0x01      
#define ACT_ID         0xD8    
#define PAD         0x0     

#define SMOOTH 1



typedef struct {
    Actor actor;
    u16 switchflag;
    s16 camera;
    u16 timer;
    u8 transition;
    u8 enabled;
    u8 discoverysound;
    Vec3f targetpos;
    Vec3f currentpos;
    Vec3f currenttarget;
    Vec3f posincr;
    Vec3f targetincr;

} entity_t;


Vec3f move_distance_by_XYrot(float distance, Vec3s angle){

    Vec3f result;


    angle.x = -angle.x;

    //pitch yaw
    f32 sp24 = Math_CosS(angle.x) * distance;
    result.x = Math_SinS(angle.y) * sp24;
    result.y = Math_SinS(angle.x) * distance;
    result.z = Math_CosS(angle.y) * sp24;

    return result;

}

float distance(Vec3f p1, Vec3f p2) {
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    float dz = p2.z - p1.z;
    return sqrt(dx * dx + dy * dy + dz * dz);
}



void approximateToPoint(Vec3f* pointA, Vec3f* pointB, float targetDistance) {
    // Calculate the vector from A to B
    float vectorAB_x = pointB->x - pointA->x;
    float vectorAB_y = pointB->y - pointA->y;
    float vectorAB_z = pointB->z - pointA->z;

    // Calculate the current distance
    float currentDistance = distance(*pointA, *pointB);

    // Scale the vector to the desired distance
    float scale = targetDistance / currentDistance;
    vectorAB_x *= scale;
    vectorAB_y *= scale;
    vectorAB_z *= scale;

    // Set the new position of point B
    pointB->x = pointA->x + vectorAB_x;
    pointB->y = pointA->y + vectorAB_y;
    pointB->z = pointA->z + vectorAB_z;
}



void init(entity_t *en, PlayState *global) {

    en->transition = (en->actor.params & 0x0100) >> 8;
    en->switchflag = (en->actor.params & 0x003F);
    en->discoverysound = ((en->actor.params & 0x1000) != 0);
    en->timer = en->actor.world.rot.z * 2;
    if (en->timer == 0) en->timer = 10;
    
    if (Flags_GetSwitch(global, en->switchflag) != 0)
    {
        Actor_Kill(&en->actor);
    }

}



void update(entity_t *en, PlayState *global) {


    if (en->enabled == 0 && Flags_GetSwitch(global, en->switchflag) != 0)
    {
        en->enabled = 1;

        if (en->discoverysound)
        {
            func_80078884((u16)NA_SE_SY_CORRECT_CHIME);
        }

        en->camera = Play_CreateSubCamera(global);
        Play_ChangeCameraStatus(global, CAM_ID_MAIN, CAM_STAT_WAIT);
        Play_ChangeCameraStatus(global, en->camera, CAM_STAT_ACTIVE);
        en->targetpos = move_distance_by_XYrot(50.0f, en->actor.rot);
        en->targetpos.x += en->actor.world.pos.x;
        en->targetpos.y += en->actor.world.pos.y;
        en->targetpos.z += en->actor.world.pos.z;
        if (en->transition != SMOOTH )
        {
        Play_CameraSetAtEye(global, en->camera, &en->targetpos, &en->actor.pos);
        }
        else
        {
            en->currentpos = global->mainCamera.at;
            en->currenttarget = global->mainCamera.eye;
            
            en->posincr.x = ABS((en->actor.world.pos.x - global->mainCamera.at.x) / 10.0f);
            en->posincr.y = ABS((en->actor.world.pos.y - global->mainCamera.at.y) / 10.0f);
            en->posincr.z = ABS((en->actor.world.pos.z - global->mainCamera.at.z) / 10.0f);
            en->targetincr.x = ABS((en->targetpos.x - en->currenttarget.x) / 10.0f);
            en->targetincr.y = ABS((en->targetpos.y - en->currenttarget.y) / 10.0f);
            en->targetincr.z = ABS((en->targetpos.z - en->currenttarget.z) / 10.0f);



            Play_CameraSetAtEye(global, en->camera, &en->currenttarget, &en->currentpos);

            func_800C0808(global, en->camera, GET_PLAYER(global), CAM_SET_FREE0);

        }
        func_80064520(global, &global->csCtx);


    }
    else if (en->enabled == 1)
    {
        en->timer--;
        if (en->transition == SMOOTH)
        {


            Math_SmoothStepToF(&en->currentpos.x, en->actor.world.pos.x, 0.5f, en->posincr.x, 5.0f);
            Math_SmoothStepToF(&en->currentpos.y, en->actor.world.pos.y, 0.5f, en->posincr.y, 5.0f);
            Math_SmoothStepToF(&en->currentpos.z, en->actor.world.pos.z, 0.5f, en->posincr.z, 5.0f);

            Math_SmoothStepToF(&en->currenttarget.x, en->targetpos.x, 0.5f, en->targetincr.x, 5.0f);
            Math_SmoothStepToF(&en->currenttarget.y, en->targetpos.y, 0.5f, en->targetincr.y, 5.0f);
            Math_SmoothStepToF(&en->currenttarget.z, en->targetpos.z, 0.5f, en->targetincr.z, 5.0f);

             Play_CameraSetAtEye(global, en->camera, &en->currenttarget, &en->currentpos);
        }
        if (en->timer == 0)
        {
            func_80064534(global, &global->csCtx); 
            func_800C08AC(global, en->camera,0);
            Actor_Kill(&en->actor);            
        }

    }


}

static void dest(entity_t *en, PlayState *global) {

}


static void draw(entity_t *en, PlayState *global) {




    
}





/* .data */
const ActorInit init_vars = {
    ACT_ID, 
    ACTORCAT_BG, 
    0x00000010,
    OBJ_ID,
    sizeof(entity_t),
    (ActorFunc)init,
    (ActorFunc)dest,
    (ActorFunc)update,
    (ActorFunc)draw
};







