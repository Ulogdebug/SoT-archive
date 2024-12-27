/**
 * by Nokaubure

Variable

& 0x4000 : Shape: 0 normal, 1 triple
& 0x8000 : Texture: 0 normal, 1 triforce
& 0x3C00 : size 0.1f * value
& 0x03C0 : 0 red, 1 blue (reverse), 2 green, 3 yellow (reverse), 4 purple, 5 orange (reverse), 6 white, 7 white
& 0x003F : switch flag

 **/

#define AVAL(base,type,offset)  (*(type*)((u8*)(base)+(offset)))

#include "z64math.h"

#include "ultra64.h"
#include "global.h"

#include "macros.h"

#include "object/0x007F-SwitchPillar.h"

#define OBJ_ID         0x7F      
#define ACT_ID         0xB2     
#define PAD         0x0     
#define STATE_OFF  -1
#define STATE_ON   1

const Gfx* DListShape[] = 
{gRedblueblock_DlSinglepillar, 
gRedblueblock_DlTriplepillar};

const CollisionHeader* CollisionShape[] = 
{gRedblueblock_CollSinglepillar, 
gRedblueblock_CollTriplepillar};

const void* Textures[] = 
{gRedblueblock_TexNormalpillar, 
gRedblueblock_TexTriforcepillar};

const u32 Colors[] = 
{
	0xFF2B00FF, // Red
	0x6A00FFFF, // Blue
	0x00FF26FF, // Green
	0xFFEC00FF, // Yellow
    0xAA329BFF, // Purple
    0xFF973CFF, // Orange
    0xFFFFFFFF, // White
    0xFFFFFFFF, // White
};



typedef struct {
	DynaPolyActor dyna;
	u8 shape;
	u8 texture;
	u8 size;
	u8 color;
	u8 inverted;
	u16 switchflag;
	Vec3f targetpos;
	s8 increment;
	u8 timer;
	s8 state;



} SwitchPillar;


void SetTranslateRotateYXZ(f32 translateX, f32 translateY, f32 translateZ, Vec3s* rot, MtxF* cmf) {

    f32 temp1 = Math_SinS(rot->y);
    f32 temp2 = Math_CosS(rot->y);
    f32 cos;
    f32 sin;

    cmf->xx = temp2;
    cmf->zx = -temp1;
    cmf->xw = translateX;
    cmf->yw = translateY;
    cmf->zw = translateZ;
    cmf->wx = 0.0f;
    cmf->wy = 0.0f;
    cmf->wz = 0.0f;
    cmf->ww = 1.0f;

    if (rot->x != 0) {
        sin = Math_SinS(rot->x);
        cos = Math_CosS(rot->x);

        cmf->zz = temp2 * cos;
        cmf->zy = temp2 * sin;
        cmf->xz = temp1 * cos;
        cmf->xy = temp1 * sin;
        cmf->yz = -sin;
        cmf->yy = cos;
    } else {
        cmf->zz = temp2;
        cmf->xz = temp1;
        cmf->yz = 0.0f;
        cmf->zy = 0.0f;
        cmf->xy = 0.0f;
        cmf->yy = 1.0f;
    }

    if (rot->z != 0) {
        sin = Math_SinS(rot->z);
        cos = Math_CosS(rot->z);

        temp1 = cmf->xx;
        temp2 = cmf->xy;
        cmf->xx = temp1 * cos + temp2 * sin;
        cmf->xy = temp2 * cos - temp1 * sin;

        temp1 = cmf->zx;
        temp2 = cmf->zy;
        cmf->zx = temp1 * cos + temp2 * sin;
        cmf->zy = temp2 * cos - temp1 * sin;

        temp2 = cmf->yy;
        cmf->yx = temp2 * sin;
        cmf->yy = temp2 * cos;
    } else {
        cmf->yx = 0.0f;
    }
}

void Translate(f32 x, f32 y, f32 z, MtxF* cmf) {
    f32 tx;
    f32 ty;
    tx = cmf->xx;
    ty = cmf->xy;
    cmf->xw += tx * x + ty * y + cmf->xz * z;
    tx = cmf->yx;
    ty = cmf->yy;
    cmf->yw += tx * x + ty * y + cmf->yz * z;
    tx = cmf->zx;
    ty = cmf->zy;
    cmf->zw += tx * x + ty * y + cmf->zz * z;
    tx = cmf->wx;
    ty = cmf->wy;
    cmf->ww += tx * x + ty * y + cmf->wz * z;
   
}




Vec3f move_distance_by_3angles(float distance, Vec3s angle){

    Vec3f result;
    MtxF cmf;

    SetTranslateRotateYXZ(0,0,0,&angle,&cmf);

    Translate(0,-distance,0,&cmf);


    result.x = cmf.xw;
    result.y = cmf.yw;
    result.z = cmf.zw;


    return result;

}



void init(SwitchPillar *en, PlayState *play) {



	en->shape = (en->dyna.actor.params & 0x4000) >> 14;
	en->texture = (en->dyna.actor.params & 0x8000) >> 15;
	en->size = (en->dyna.actor.params & 0x3C00) >> 10;
	en->color = (en->dyna.actor.params & 0x03C0) >> 6;
	en->switchflag = (en->dyna.actor.params & 0x003F);
	en->inverted = (en->color%2 == 0) ? 0 : 1;
    en->timer = 0;
    en->state = 0;

	Actor_SetScale(&en->dyna.actor,en->size == 0 ? 0.5f : (en->size * 0.1f));

    en->dyna.actor.uncullZoneForward = 3500.0f;




	CollisionHeader* colHeader = NULL;
    DynaPolyActor_Init(&en->dyna, DYNA_INTERACT_ACTOR_ON_TOP);
    CollisionHeader_GetVirtual((void*)CollisionShape[en->shape],&colHeader);
	en->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna,&en->dyna.actor, colHeader);

    Vec3f ret = move_distance_by_3angles(20.0f * en->dyna.actor.scale.x ,en->dyna.actor.world.rot);

    en->targetpos.x = ret.x;
    en->targetpos.y = ret.y;
    en->targetpos.z = ret.z;

    s32 flag = Flags_GetSwitch(play, en->switchflag);
    if ((flag != 0 && en->inverted == 0) ||
        (flag == 0 && en->inverted == 1)) {
        en->dyna.actor.world.pos.x = (en->dyna.actor.world.pos.x + (en->targetpos.x * 10.0f));
        en->dyna.actor.world.pos.y = (en->dyna.actor.world.pos.y + (en->targetpos.y * 10.0f));
        en->dyna.actor.world.pos.z = (en->dyna.actor.world.pos.z + (en->targetpos.z * 10.0f));
        en->state = STATE_ON;
    }
    else
        en->state = STATE_OFF;
    

}



void update(SwitchPillar *en, PlayState *play) {


    if (en->timer == 0)
    {
        s32 flag = Flags_GetSwitch(play, en->switchflag);
        if ((flag == 0 && en->state == STATE_ON && en->inverted == 0) ||
            (flag != 0 && en->state == STATE_OFF && en->inverted == 0) ||
            (flag == 0 && en->state == STATE_OFF && en->inverted == 1) ||
            (flag != 0 && en->state == STATE_ON && en->inverted == 1))
        {
            en->increment = -en->state;
            en->timer = 0xA;
            SfxSource_PlaySfxAtFixedWorldPos(play, &en->dyna.actor.world.pos, 0x1E, 0x2859);
        }
    }
    else
    {
        en->dyna.actor.world.pos.x += en->targetpos.x * en->increment;
        en->dyna.actor.world.pos.y += en->targetpos.y * en->increment;
        en->dyna.actor.world.pos.z += en->targetpos.z * en->increment;
        en->timer--;
        if (en->timer == 0) {
            en->state = en->increment;
            en->increment = 0;
        }
    }


  



}

static void dest(SwitchPillar *en, PlayState *play) {

	 DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, en->dyna.bgId);

}


static void draw(SwitchPillar *en, PlayState *play) {



        OPEN_DISPS(play->state.gfxCtx, "", 1);


        Gfx_SetupDL_25Opa(play->state.gfxCtx);

        gDPSetEnvColor(POLY_OPA_DISP++, (Colors[en->color] & 0xFF000000) >> 24, (Colors[en->color] & 0x00FF0000) >> 16, (Colors[en->color] & 0x0000FF00)>> 8, 0xFF);

        gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(Textures[en->texture]));

        

        Gfx_DrawDListOpa(play,(Gfx*)DListShape[en->shape]);

        gDPSetEnvColor(POLY_OPA_DISP++, 0,0,0,0);

        CLOSE_DISPS(play->state.gfxCtx, "", 2);


	
}





/* .data */
const ActorInit init_vars = {
	ACT_ID, 
	ACTORCAT_BG, 
	0x00000010,
	OBJ_ID,
	sizeof(SwitchPillar),
	(ActorFunc)init,
	(ActorFunc)dest,
	(ActorFunc)update,
	(ActorFunc)draw
};







