/*
 * File: z_bg_icefloe.c
 * Overlay: ovl_Bg_Icefloe
 * Description: Ice Platform Created by Ice Arrow
 */
#include <uLib.h>
#include "z_bg_icefloe.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h"
#include "object/0x0308-IceArrowPlatform.h"


typedef struct {
    /* 0x00 */ Actor* actor;
    /* 0x04 */ Vec3f pos;
    /* 0x10 */ Vec3f velocity;
    /* 0x1C */ Vec3f accel;
    /* 0x28 */ s16 scale;
} EffectEnIceBlockInitParams; // size = 0x2C

static Gfx* icePlatformDLs[] = {
    gIcePlatformObj_DlWaterIcePlatformObj,
    gIcePlatformObj_DlGroundIcePlatformObj,
};

static CollisionHeader* icePlatformCollisions[] = {
    &gIcePlatformObj_CollWaterIcePlatformObj,
    &gIcePlatformObj_CollGroundIcePlatformObj,
};

#define PARAMS ((EffectEnIceBlockInitParams*)initParamsx)


#define FLAGS (ACTOR_FLAG_4)

#define THIS ((BgIcefloe*)thisx)

void BgIcefloe_Init(Actor* thisx, PlayState* play);
void BgIcefloe_Destroy(Actor* thisx, PlayState* play);
void BgIcefloe_Update(Actor* thisx, PlayState* play);
void BgIcefloe_Draw(Actor* thisx, PlayState* play);

void func_80AC4A80(BgIcefloe* this, PlayState* play);
void BgIcefloe_Grow(BgIcefloe* this, PlayState* play);
void func_80AC4C18(BgIcefloe* this,PlayState* play);
void func_80AC4D2C(BgIcefloe* this, PlayState* play);
void func_80AC4C34(BgIcefloe* this, PlayState* play);
void func_80AC4CF0(BgIcefloe* this);

ActorInit Bg_Icefloe_InitVars = {
    /**/ 0x0240,
    /**/ ACTORCAT_BG,
    /**/ FLAGS,
    /**/ 0x0308,
    /**/ sizeof(BgIcefloe),
    /**/ BgIcefloe_Init,
    /**/ BgIcefloe_Destroy,
    /**/ BgIcefloe_Update,
    /**/ BgIcefloe_Draw,
};
static BgIcefloe* sSpawnedInstances[] = { NULL, NULL, NULL };

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | (AC_TYPE_ALL),
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK2,
        { 0x00000000, 0x00, 0x00 },
        {  DMG_DEFAULT, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_NONE,
    },
    .dim = { .radius = 40, .height = 70 },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 0, ICHAIN_STOP),
};

static s32 numberSpawned;

void BgIcefloe_Init(Actor* thisx, PlayState* play) {
    BgIcefloe* this = THIS;
    Actor_ProcessInitChain(&this->dyna.actor, sInitChain);
	CollisionHeader* colHeader = NULL;
    DynaPolyActor_Init(&this->dyna, 0); 
    u8 platformType = (this->dyna.actor.params & 0xF != 0) ? 1 : 0 ;
    CollisionHeader_GetVirtual(icePlatformCollisions[platformType],&colHeader);		//gameplay_keep
    this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna,&this->dyna.actor, colHeader);
    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinder(play, &this->collider, &this->dyna.actor, &sCylinderInit);
    Collider_UpdateCylinder(&this->dyna.actor, &this->collider);
    if(platformType == 0){
        this->collider.dim.height = 51.0f;
        this->collider.dim.pos.y = this->dyna.actor.world.pos.y - 51.0f;
    }
    if (numberSpawned >= ARRAY_COUNT(sSpawnedInstances)) {
        s32 i;

        if ((sSpawnedInstances[0] != NULL) && (sSpawnedInstances[0]->dyna.actor.update != NULL) &&
            (sSpawnedInstances[0]->actionFunc != func_80AC4D2C)) {
            func_80AC4CF0(sSpawnedInstances[0]);
        }
        for (i = 0; i < ARRAY_COUNT(sSpawnedInstances) - 1; i++) {
            sSpawnedInstances[i] = sSpawnedInstances[i + 1];
        }
        sSpawnedInstances[ARRAY_COUNT(sSpawnedInstances) - 1] = this;
    } else {
        sSpawnedInstances[numberSpawned] = this;
    }
    numberSpawned++;
    this->dyna.actor.world.pos.y = this->dyna.actor.home.pos.y + 10.0f;
    func_80AC4A80(this, play);
}

void BgIcefloe_Destroy(Actor* thisx, PlayState* play) {
    BgIcefloe* this = THIS;
    s32 i;
    Collider_DestroyCylinder(play, &this->collider);
    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
    numberSpawned--;

    for (i = 0; i < 3; i++) {
        if (sSpawnedInstances[i] == this) {
            sSpawnedInstances[i] = NULL;
            break;
        }
    }
}

void func_80AC4A80(BgIcefloe* this, PlayState* play) {
    this->timer = 20;
    SkinMatrix_Vec3fMtxFMultXYZW(&play->viewProjectionMtxF, &this->dyna.actor.world.pos, &this->dyna.actor.projectedPos,
                                 &this->dyna.actor.projectedW);
    //Actor_PlaySfx(&this->dyna.actor, NA_SE_EV_ICE_STAND_APPEAR);	//MISSING SFX
	//Audio_PlayActorSfx2(&this->dyna.actor, NA_SE_EV_ICE_FREEZE);
    SoundEffect_PlayOneshot(SOUND_MM_ICE_PLATFORM, 1.0f, 1.0f, &this->dyna.actor.world.pos, 0.0f, 1000.0f, &this->dyna.actor);
    this->actionFunc = BgIcefloe_Grow;
}

static Vec3f sIceBlockAccel = { 0.0f, -0.5f, 0.0f };



void EffectSsIceBlock_Spawn(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale) {
    EffectEnIceBlockInitParams initParams;

    Math_Vec3f_Copy(&initParams.pos, pos);
    Math_Vec3f_Copy(&initParams.velocity, velocity);
    Math_Vec3f_Copy(&initParams.accel, accel);
    initParams.scale = scale;

    EffectSs_Spawn(play, 0x0028, 128, &initParams);
}



void BgIcefloe_Grow(BgIcefloe* this, PlayState* play) {
    Vec3f velocity;
    Vec3f position;

    velocity.x = Rand_CenteredFloat(6.0f);
    velocity.z = Rand_CenteredFloat(6.0f);
    velocity.y = Rand_ZeroFloat(4.0f) + 4.0f;
    this->dyna.actor.scale.x += (0.65f * 0.001f);
    this->dyna.actor.scale.z += (0.65f * 0.001f);
    this->dyna.actor.scale.y += (0.65f * 0.001f);
    position.x = this->dyna.actor.world.pos.x + (velocity.x * this->dyna.actor.scale.x * 75.0f);
    position.z = this->dyna.actor.world.pos.z + (velocity.z * this->dyna.actor.scale.z * 75.0f);
    position.y = this->dyna.actor.world.pos.y + (300.0f * this->dyna.actor.scale.y);
    EffectSsIceBlock_Spawn(play, &position, &velocity, &sIceBlockAccel, Rand_S16Offset(10, 10));	//MISSING EFFECT
    this->timer--;
    if (this->timer == 0) {
        func_80AC4C18(this,play);
    }
}

void func_80AC4C18(BgIcefloe* this,PlayState* play) {
    if(Actor_FindNearby(play, &this->dyna.actor, 0x0240, ACTORCAT_BG, 80.0f) != NULL){
        this->timer = 0;
    }else{
        this->timer = 0xE10;//3 mins
    }
    this->actionFunc = func_80AC4C34;
}

void func_80AC4C34(BgIcefloe* this, PlayState* play) {
    WaterBox* water;
    this->timer--;
    if (this->collider.base.acFlags & AC_HIT) {
        this->collider.base.acFlags &= ~AC_HIT;
        if(this->collider.info.acHitInfo->toucher.dmgFlags & (DMG_FIRE)){
            this->timer = 0;
        }
    }else if (this->dyna.actor.xzDistToPlayer < 1200.0f) {
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
    }

    if ((this->timer <= 0) ||
        !WaterBox_GetSurface1(play, &play->colCtx, this->dyna.actor.world.pos.x, this->dyna.actor.world.pos.z,
                                &this->dyna.actor.home.pos.y, &water)) {
        func_80AC4CF0(this);
    } else {
        this->dyna.actor.world.pos.y =
            (Math_SinF(this->timer * (M_PI / 30.0f)) * 3.0f) + (this->dyna.actor.home.pos.y + 10.0f);
    }
}

void func_80AC4CF0(BgIcefloe* this) {
    this->timer = 50;
    //Actor_PlaySfx(&this->dyna.actor, NA_SE_EV_ICE_MELT_LEVEL);	//MISSING SFX
	Audio_PlayActorSfx2(&this->dyna.actor, NA_SE_EV_ICE_MELT);
    this->actionFunc = func_80AC4D2C;
}

void func_80AC4D2C(BgIcefloe* this, PlayState* play) {
    this->timer--;
    if ((this->timer >= 38) && !(this->timer % 2)) {
        Vec3f velocity;
        Vec3f position;

        velocity.y = (this->timer - 38) * (1 / 12.0f);
        velocity.x = Rand_CenteredFloat(1.5f) * velocity.y;
        velocity.z = Rand_CenteredFloat(1.5f) * velocity.y;
        velocity.y += 0.8f;
        position.x = this->dyna.actor.world.pos.x + (2.0f * velocity.x);
        position.z = this->dyna.actor.world.pos.z + (2.0f * velocity.z);
        position.y = this->dyna.actor.world.pos.y + 3.0f;
		Vec3f gZeroVec3f = {0.0f,0.0f,0.0f};
        EffectSsIceSmoke_Spawn(play, &position, &velocity, &gZeroVec3f, 200);
    }
    if (this->timer < 25) {
        this->dyna.actor.scale.x -= 0.00052f;
        this->dyna.actor.scale.z -= 0.00052f;
    }
    this->dyna.actor.scale.y -= 0.0026f;
    if (this->dyna.actor.scale.y <= 0.0f) {
        Actor_Kill(&this->dyna.actor);
    }
}

void BgIcefloe_Update(Actor* thisx, PlayState* play) {
    BgIcefloe* this = THIS;

    if (!Play_InCsMode(play)) {
        this->actionFunc(this, play);
    }
}

void BgIcefloe_Draw(Actor* thisx, PlayState* play) {
    BgIcefloe* this = THIS;

    Gfx_DrawDListOpa(play, icePlatformDLs[this->dyna.actor.params & 0xF]);
}
