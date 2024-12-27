/*
 * File: hammer_peg.c
 * Overlay: ovl_Hammer_Peg
 * Description: Hammer Peg actor from the Hylian Modding actor pack.
 *  * Params: YY00
 *      -- YY: Switch flag (0x00 thru 0x3F)
 */

#include "hammer_peg.h"
#include "overlays/effects/ovl_Effect_Ss_Kakera/z_eff_ss_kakera.h"

#define FLAGS (ACTOR_FLAG_4 | ACTOR_FLAG_5)

void HammerPeg_Init(Actor* thisx, PlayState* play);
void HammerPeg_Destroy(Actor* thisx, PlayState* play);
void HammerPeg_Update(Actor* thisx, PlayState* play);
void HammerPeg_Draw(Actor* thisx, PlayState* play);

void HammerPeg_IdleUp(HammerPeg* this, PlayState* play);
void HammerPeg_Lowering(HammerPeg* this, PlayState* play);
void HammerPeg_IdleDown(HammerPeg* this, PlayState* play);

const ActorInit Hammer_Peg_InitVars = {
    0x0237,
    ACTORCAT_BG,
    FLAGS,
    0x0228,
    sizeof(HammerPeg),
    (ActorFunc)HammerPeg_Init,
    (ActorFunc)HammerPeg_Destroy,
    (ActorFunc)HammerPeg_Update,
    (ActorFunc)HammerPeg_Draw,
};

static ColliderCylinderInit sCylinderBaseInit = {
    {
        COLTYPE_TREE,
        AT_NONE,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00100000, 0x00, 0x00 },
        { 0xEE01FFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 20, 60, 0, { 0, 0, 0 } },
};

static ColliderCylinderInit sCylinderTopInit = {
    {
        COLTYPE_TREE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { DMG_HAMMER, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON | BUMP_NO_AT_INFO | BUMP_NO_DAMAGE | BUMP_NO_SWORD_SFX | BUMP_NO_HITMARK,
        OCELEM_NONE,
    },
    { 35, 20, 30, { 0, 0, 0 } },
};

#define HAMMER_PEG_GET_SWITCH_FLAG(this) ((this->dyna.actor.params >> 8) & 0xFF)

void HammerPeg_Init(Actor* thisx, PlayState* play) {
    HammerPeg* this = (HammerPeg*)thisx;
    CollisionHeader* colHeader = NULL;
    u8 pegDown = Flags_GetSwitch(play, HAMMER_PEG_GET_SWITCH_FLAG(this));

    Actor_SetScale(&this->dyna.actor, 0.01f);

    this->dyna.actor.colChkInfo.mass = MASS_HEAVY;

    DynaPolyActor_Init(&this->dyna, 0);
    CollisionHeader_GetVirtual(0x06000ed8, &colHeader);
    this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &this->dyna.actor, colHeader);

    Collider_InitCylinder(play, &this->colliderBase);
    Collider_InitCylinder(play, &this->colliderTop);

    Collider_SetCylinder(play, &this->colliderBase, &this->dyna.actor, &sCylinderBaseInit);
    Collider_SetCylinder(play, &this->colliderTop, &this->dyna.actor, &sCylinderTopInit);

    if (!pegDown) {
        this->percentUp = 100;
        this->actionFunc = HammerPeg_IdleUp;
    } else {
        this->percentUp = 0;
        this->actionFunc = HammerPeg_IdleDown;
    }
}

void HammerPeg_Destroy(Actor* thisx, PlayState* play) {
    HammerPeg* this = (HammerPeg*)thisx;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

// MAIN

void HammerPeg_SpawnParticles(HammerPeg* this, PlayState* play) {
    static Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    static Color_RGBA8 sPrimColor = { 200, 200, 200, 255 };
    static Color_RGBA8 sEnvColor = { 100, 100, 100, 255 };
    static Vec3f accel = { 0.0f, 0.3f, 0.0f };
    s16 scale;
    Vec3f* thisPos;
    Vec3f pos;
    s32 i;

    thisPos = &this->dyna.actor.world.pos;

    for (i = 6; i >= 0; i--) {
        pos.x = thisPos->x + ((Rand_ZeroOne() - 0.5f) * 65.0f);
        pos.y = thisPos->y + 5.0f;
        pos.z = thisPos->z + ((Rand_ZeroOne() - 0.5f) * 65.0f);

        scale = (s16)((Rand_ZeroOne() * 10) * 0.02f) + 10;
        EffectSsDust_Spawn(play, 4, &pos, &velocity, &accel, &sPrimColor, &sEnvColor, scale, 80, 8, 0);
    }
}

void HammerPeg_IdleUp(HammerPeg* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (this->colliderTop.base.acFlags & AC_HIT && !Player_InCsMode(play)) {
        if (player->meleeWeaponAnimation != PLAYER_MWA_STAB_1H &&
            this->colliderTop.info.acHitInfo->toucher.dmgFlags & DMG_HAMMER) {
            this->colliderTop.base.acFlags &= ~AC_HIT;

            Audio_PlayActorSfx2(&this->dyna.actor, NA_SE_EV_WOOD_BOUND);
            HammerPeg_SpawnParticles(this, play);
            this->actionFunc = HammerPeg_Lowering;
        }
    }
}

void HammerPeg_Lowering(HammerPeg* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (this->percentUp <= 0) {
        Flags_SetSwitch(play, HAMMER_PEG_GET_SWITCH_FLAG(this));
        this->percentUp = 0;
        Audio_PlayActorSfx2(&this->dyna.actor, NA_SE_SY_CORRECT_CHIME);
        this->actionFunc = HammerPeg_IdleDown;
        return;
    }

    this->percentUp -= 30;
}

void HammerPeg_IdleDown(HammerPeg* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
}

void HammerPeg_Update(Actor* thisx, PlayState* play) {
    HammerPeg* this = (HammerPeg*)thisx;

    this->actionFunc(this, play);

    if (this->percentUp != 0) {
        Collider_UpdateCylinder(&this->dyna.actor, &this->colliderTop);
        Collider_UpdateCylinder(&this->dyna.actor, &this->colliderBase);

        if (this->dyna.actor.xzDistToPlayer < 600.0f) {
            CollisionCheck_SetOC(play, &play->colChkCtx, &this->colliderBase.base);
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->colliderBase.base);
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->colliderTop.base);
        }
    }
}

void HammerPeg_Draw(Actor* thisx, PlayState* play) {
    HammerPeg* this = (HammerPeg*)thisx;

    OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

    Gfx_DrawDListOpa(play, 0x060004d0);

    Matrix_Translate(0.0f, (-2100.0f / 100.0f) * (100 - this->percentUp), 0.0f, MTXMODE_APPLY);
    Matrix_Scale(1.0f, ((0.25f / 100.0f) * this->percentUp) + 0.75f, 1.0f, MTXMODE_APPLY);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    Gfx_DrawDListOpa(play, 0x06000db0);

    CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
}