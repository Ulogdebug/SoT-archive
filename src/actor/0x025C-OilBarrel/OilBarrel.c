#include "OilBarrel.h"
#include "object/0x007B-OilBarrelObj.h"
#include "overlays/actors/ovl_En_Bom/z_en_bom.h"
#define FLAGS ( ACTOR_FLAG_2 | ACTOR_FLAG_4 | ACTOR_FLAG_26 )

void OilBarrel_Init(OilBarrel* this, PlayState* play);
void OilBarrel_Destroy(OilBarrel* this, PlayState* play);
void OilBarrel_Update(OilBarrel* this, PlayState* play);
void OilBarrel_Draw(OilBarrel* this, PlayState* play);
void OilBarrel_SetupWaitToPush(OilBarrel* this, PlayState* play);
void OilBarrel_WaitToPush(OilBarrel* this, PlayState* play);
void OilBarrel_SetupWaitToExplode(OilBarrel* this, PlayState* play);
void OilBarrel_WaitToExplode(OilBarrel* this, PlayState* play);
void OilBarrel_SpawnFragments(OilBarrel* this, PlayState* play);
void OilBarrel_ChangeCollision(OilBarrel* this, PlayState* play, void* colHeader);
void OilBarrel_Explode(OilBarrel* this, PlayState* play);

static ColliderCylinderInit hitboxCylinderInit = {
    {
        COLTYPE_METAL,
        AT_NONE,
        AC_ON | AC_TYPE_ALL,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { ~(DMG_SHIELD | DMG_MIR_RAY | DMG_DEKU_NUT), 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 30, 78, 0, { 0, 0, 0 } },
};

const ActorInit sOilBarrel_InitVars = {
    .id = 0x025C,
    .category     = ACTORCAT_BG,
    .flags        = FLAGS,
    .objectId = 0x007B,
    .instanceSize = sizeof(OilBarrel),
    .init         = (ActorFunc)OilBarrel_Init,
    .destroy      = (ActorFunc)OilBarrel_Destroy,
    .update       = (ActorFunc)OilBarrel_Update,
    .draw         = (ActorFunc)OilBarrel_Draw
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32_DIV1000(gravity, -4000, ICHAIN_CONTINUE),
};


void OilBarrel_Init(OilBarrel* this, PlayState* play) {
    CollisionHeader* colHeader = NULL;
    Actor_ProcessInitChain(&this->dyna.actor, sInitChain);
    ActorShape_Init(&this->dyna.actor.shape, 0.0f, ActorShadow_DrawCircle, 48.0f);
    DynaPolyActor_Init(&this->dyna, DPM_UNK);
    Collider_InitCylinder(play, &this->hitCollider);
    Collider_SetCylinder(play, &this->hitCollider, &this->dyna.actor, &hitboxCylinderInit);
    this->hitCollider.base.ocFlags1 = (OC1_ON | OC1_NO_PUSH | OC1_TYPE_1 | OC1_TYPE_2);
    CollisionHeader_GetVirtual(gOilBarrelObj_CollOilBarrelObj, &colHeader);
    this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &this->dyna.actor, colHeader);
    this->dyna.actor.colChkInfo.mass = MASS_HEAVY;
    this->explodeTimer = -1;
    OilBarrel_SetupWaitToPush(this,play);
}

void OilBarrel_SetupWaitToPush(OilBarrel* this, PlayState* play){
    this->dyna.actor.speedXZ = 0.0f;
    this->actionFunc = OilBarrel_WaitToPush;
}

void OilBarrel_WaitToPush(OilBarrel* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    f32 temp158f = this->dyna.unk_158;
    s16 moveDir = 0;

    if (this->tempArg == 0) {
        if (this->dyna.unk_150 != 0.0f) {
            this->tempArg = 0x8000;
        }
    } else {
        this->tempArg -= 0x800;
        Audio_PlayActorSfx2(&this->dyna.actor, NA_SE_EV_WALL_SLIDE - SFX_FLAG);

        if (this->dyna.unk_150 < 0.0f) {
            temp158f = this->dyna.unk_158 + 0x8000;
        }

        if (this->hitCollider.base.ocFlags1 & OC1_HIT) {
            moveDir = Math_Vec3f_Yaw(&this->dyna.actor.world.pos, &this->hitCollider.base.oc->world.pos) - temp158f;
        }

        if ((this->dyna.unk_150 == 0.0f) || (this->tempArg == 0) ||
            !(this->dyna.actor.bgCheckFlags & BGCHECKFLAG_GROUND) ||
            !func_800435D8(play, &this->dyna, 0x14, (Math_SinS(this->tempArg) * (this->dyna.unk_150 * 0.5f)) + 40.0f,
                           0xA) ||
            ((this->hitCollider.base.ocFlags1 & OC1_HIT) && (ABS(moveDir) <= 0x2000)) || 
            (this->hitCollider.base.acFlags & AC_HIT)) {

            this->tempArg = 0;
            player->stateFlags2 &= ~(PLAYER_STATE2_0 | PLAYER_STATE2_4 | PLAYER_STATE2_6 | PLAYER_STATE2_8);
            player->actor.speedXZ = 0.0f;
            this->dyna.unk_150 = this->dyna.unk_154 = 0.0f;
        }

        this->dyna.actor.world.rot.y = this->dyna.unk_158;
        this->dyna.actor.speedXZ = (Math_SinS(this->tempArg) * (this->dyna.unk_150 * 0.5f)) * 3;
    }

    if (this->dyna.actor.bgCheckFlags & BGCHECKFLAG_GROUND_TOUCH) {
        Audio_PlayActorSfx2(&this->dyna.actor, NA_SE_EV_ELEVATOR_STOP);
    }

    this->dyna.unk_150 = this->dyna.unk_154 = 0.0f;
}

void OilBarrel_Destroy(OilBarrel* this, PlayState* play) {
    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
    Collider_DestroyCylinder(play, &this->hitCollider);
}

void OilBarrel_ChangeCollision(OilBarrel* this, PlayState* play, void* colHeader) {
    CollisionHeader* colHeaderVirtual = NULL;
    CollisionHeader_GetVirtual(colHeader, &colHeaderVirtual);
    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
    this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &this->dyna.actor, colHeaderVirtual);
}

void OilBarrel_SetupWaitToExplode(OilBarrel* this, PlayState* play) {
    this->dyna.actor.world.rot.y = this->dyna.actor.yawTowardsPlayer;
    this->dyna.actor.speedXZ = -5.0f;
    this->dyna.actor.velocity.y = 10.0f;
    this->explodeTimer = 60;
    Player* player = GET_PLAYER(play);
    this->dyna.unk_150 = 0.0f;
    player->stateFlags2 &= ~PLAYER_STATE2_4;
    OilBarrel_ChangeCollision(this, play, gOilBarrelObj_CollOilBarrelObjNoPush);
    this->actionFunc = OilBarrel_WaitToExplode;
}

void OilBarrel_Explode(OilBarrel* this, PlayState* play) {
    this->dyna.actor.draw = NULL;
    Audio_PlayActorSfx2(&this->dyna.actor, NA_SE_EN_IRONNACK_ARMOR_OFF_DEMO);
    SfxSource_PlaySfxAtFixedWorldPos(play, &this->dyna.actor.world.pos, 20, NA_SE_EV_BRIDGE_CLOSE_STOP);
    EnBom* bomb;
    bomb =
        (EnBom*)Actor_Spawn(&play->actorCtx, play, ACTOR_EN_BOM, this->dyna.actor.world.pos.x,
                            this->dyna.actor.world.pos.y + 35.0f, this->dyna.actor.world.pos.z, 0, 0, 2, BOMB_BODY);
    if (bomb != NULL) {
        bomb->timer = 0;
    }

    Vec3f dustPos;
    s32 i;
    f32 dustPosScale;
    s32 pad1;
    static Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };
    static Color_RGBA8 dustPrimColor = { 150, 150, 150, 255 };
    static Color_RGBA8 dustEnvColor = { 100, 100, 100, 150 };
    dustPosScale = play->gameplayFrames * 10;
    for (i = 9; i >= 0; i--) {
        dustPos.x = (sinf(dustPosScale) * 7.0f) + this->dyna.actor.world.pos.x;
        dustPos.y = (Rand_CenteredFloat(10.0f) * 6.0f) + (this->dyna.actor.world.pos.y + 40.0f);
        dustPos.z = (cosf(dustPosScale) * 7.0f) + this->dyna.actor.world.pos.z;

        func_8002836C(play, &dustPos, &zeroVec, &zeroVec, &dustPrimColor, &dustEnvColor, 200, 45, 12);
        dustPosScale += 60.0f;
    }
    OilBarrel_SpawnFragments(this, play);
    Enemy_StartFinishingBlow(play, &this->dyna.actor);
    Actor_Kill(&this->dyna.actor);
}

void OilBarrel_WaitToExplode(OilBarrel* this, PlayState* play) {
    Vec3f effPos;
    s16 frame;
    Vec3f dustAccel = { 0.0f, 0.6f, 0.0f };
    Color_RGBA8 dustColor = { 0, 0, 0, 0 };
    effPos = this->dyna.actor.world.pos;
    effPos.y = this->dyna.actor.world.pos.y + 74.2f;
    Vec3f effVelocity = { 0.0f, 0.0f, 0.0f };
    
    if ((this->dyna.actor.velocity.y <= 0.0f) ) {
        this->dyna.actor.speedXZ = 0.0f;
    }

    DECR(this->explodeTimer);
    DECR(this->breakTimer);
    if(this->explodeTimer > 40){
        frame = 6;
    }else if (this->explodeTimer <= 40 && (this->explodeTimer > 20)){
        frame = 4;;
    }else{
        frame = 2;;
    }
    if ((this->explodeTimer % frame) == 0) {
        Actor_SetColorFilter(&this->dyna.actor, 0x4000, 255, 0, 4);
    }
    Audio_PlayActorSfx2(&this->dyna.actor, NA_SE_IT_BOMB_IGNIT - SFX_FLAG);
    func_8002829C(play, &effPos, &effVelocity, &dustAccel, &dustColor, &dustColor, 50, 5);
    if(this->explodeTimer == 0){
        this->actionFunc = OilBarrel_Explode;
        return;
    }
}


void OilBarrel_CollisionCheck(OilBarrel* this, PlayState* play) {
    // if(this->explodeTimer > 0 ){
    //     OilBarrel_ChangeCollision(this, play, gOilBarrelObj_CollOilBarrelObjNoPush);
    // }
    if (this->hitCollider.base.acFlags & AC_HIT) {
        this->hitCollider.base.acFlags &= ~AC_HIT;
        this->breakTimer = 20;
        if(this->explodeTimer > 0){
            this->explodeTimer = 0;
        }else if(this->explodeTimer == -1){
            OilBarrel_SetupWaitToExplode(this,play);
        }
    }else if (this->dyna.actor.xzDistToPlayer < 600.0f) {
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->hitCollider.base);
    }
}

void OilBarrel_Update(OilBarrel* this, PlayState* play) {
    this->dyna.actor.home.pos = this->dyna.actor.world.pos;
    OilBarrel_CollisionCheck(this,play);
    Actor_MoveForward(&this->dyna.actor);
    this->actionFunc(this, play);
    Actor_UpdateBgCheckInfo(play, &this->dyna.actor, 20.0f, 30.0f, 80.0f,
                                UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2 | UPDBGCHECKINFO_FLAG_3 |
                                    UPDBGCHECKINFO_FLAG_4);
    Collider_UpdateCylinder(&this->dyna.actor, &this->hitCollider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->hitCollider.base);
    if(this->breakTimer == 0){
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->hitCollider.base);                           
    }

}

void OilBarrel_SpawnFragments(OilBarrel* this, PlayState* play) {
    Vec3f thisPos;
    Vec3f pos;
    Vec3f velocity;
    s16 angle;
    s32 i;    
    // spawn fragments
    thisPos = this->dyna.actor.world.pos;
    thisPos.y = this->dyna.actor.world.pos.y + 40.0f;
    for (i = 0, angle = 0; i < 0x8; i++, angle += 0x4E20) {
        f32 sn = Math_SinS(angle);
        f32 cs = Math_CosS(angle);
        f32 temp_rand;
        s32 phi_s0;

        temp_rand = Rand_ZeroOne() * 30.0f;
        pos.x = sn * temp_rand;
        pos.y = (Rand_ZeroOne() * 10.0f) + 2.0f;
        pos.z = cs * temp_rand;
        velocity.x = pos.x * 0.2f;
        velocity.y = (Rand_ZeroOne() * 10.0f) + 2.0f;
        velocity.z = pos.z * 0.2f;
        pos.x += thisPos.x;
        pos.y += thisPos.y;
        pos.z += thisPos.z;
        temp_rand = Rand_ZeroOne();
        if (temp_rand < 0.05f) {
            phi_s0 = 0x60;
        } else if (temp_rand < 0.7f) {
            phi_s0 = 0x40;
        } else {
            phi_s0 = 0x20;
        }
        EffectSsKakera_Spawn(play, &pos, &velocity, &pos, -200, phi_s0, 28, 2, 0, (Rand_ZeroOne() * 30.0f) + 5.0f, 0, 0,
                             70, -1, 0x007B, gOilBarrelObj_DlOilBarrelFragmentObj);
    }


}

s32 OilBarrel_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3f* pos, Vec3s* rot, void* thisx) {
    OilBarrel* this = (void*)thisx;
    
    return 0;
}

void OilBarrel_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3s* rot, void* thisx) {
    OilBarrel* this = (void*)thisx;
}

static Vec3f EffectVel = { 0.0f, 0.0f, 0.0f };

void OilBarrel_Draw(OilBarrel* this, PlayState* play) {
    Vec3f pos  = this->dyna.actor.world.pos;
    pos.y = pos.y + 70.0f;
    Vec3f effVelocity = { 0.0f, 0.0f, 0.0f };
    Vec3f effAccel = { 0.0f, 0.0f, 0.0f };
    Gfx_DrawDListOpa(play, gOilBarrelObj_DlOilBarrelObj);
    if(this->explodeTimer >0 ){
        s16 fireTimer;
        fireTimer = 60 - this->explodeTimer;
        if (fireTimer != 0) {
            if (fireTimer % 4 == 0) {
                Audio_PlayActorSfx2(&this->dyna.actor, NA_SE_EV_FLAME_IGNITION);
                EffectSsGSpk_SpawnFuse(play, &this->dyna.actor, &pos, &effVelocity, &effAccel);                
            }
        }
    }
}
