/*
 * File: z_en_wdhand.c
 * Overlay: ovl_En_Wdhand
 * Description: Dexihand
 */

#include "z_en_wdhand.h"
#include <uLib.h>

#define FLAGS (ACTOR_FLAG_0 | ACTOR_FLAG_2 | ACTOR_FLAG_ARROW_CAN_CARRY)

#define ABS_ALT(x) ((x) < 0 ? -(x) : (x))




static Gfx SetupDL25[] = {

        /* SETUPDL_25 */
        gsDPPipeSync(),
        gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
        gsDPSetCombineMode(G_CC_MODULATEIDECALA, G_CC_MODULATEIA_PRIM2),
        gsDPSetOtherMode(G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE |
                             G_TD_CLAMP | G_TP_PERSP | G_CYC_2CYCLE | G_PM_NPRIMITIVE,
                         G_AC_NONE | G_ZS_PIXEL | G_RM_FOG_SHADE_A | G_RM_AA_ZB_OPA_SURF2),
        gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
        gsSPEndDisplayList(),
};


void Matrix_MultVecY(f32 y, Vec3f* dest) {
    MtxF* cmf = sCurrentMatrix;

    dest->x = cmf->xw + cmf->xy * y;
    dest->y = cmf->yw + cmf->yy * y;
    dest->z = cmf->zw + cmf->zy * y;
}

void Matrix_MtxFToYXZRot(MtxF* src, Vec3s* dest, s32 nonUniformScale) {
    f32 temp;
    f32 temp2;
    f32 temp3;
    f32 temp4;

    temp = src->xz;
    temp *= temp;
    temp += SQ(src->zz);
    //dest->x = Math_Atan2S(-src->yz, sqrtf(temp));
    dest->x = Math_Atan2S(sqrtf(temp), -src->yz);

    if ((dest->x == 0x4000) || (dest->x == -0x4000)) {
        // cos(x) = 0 if either of these is true, and we get gimbal locking
        // (https://en.wikipedia.org/wiki/Gimbal_lock#Loss_of_a_degree_of_freedom_with_Euler_angles); fix z to make y
        // well-defined.
        dest->z = 0;

        //dest->y = Math_Atan2S(-src->zx, src->xx);
        dest->y = Math_Atan2S(-src->xx, src->zx);
    } else {
        //dest->y = Math_Atan2S(src->xz, src->zz);
        dest->y = Math_Atan2S(src->zz, src->xz);

        if (!nonUniformScale) {
            // assume the columns have the same normalisation
            //dest->z = Math_Atan2S(src->yx, src->yy);
            dest->z = Math_Atan2S(src->yy, src->yx);
        } else {
            temp = src->xx;
            temp2 = src->zx;
            temp3 = src->zy;

            // find norm of the first column
            temp *= temp;
            temp += SQ(temp2);
            temp2 = src->yx;
            temp += SQ(temp2);
            // temp = xx^2+zx^2+yx^2 == 1 for a rotation matrix
            temp = sqrtf(temp);
            temp = temp2 / temp; // yx in normalised column

            // find norm of the second column
            temp2 = src->xy;
            temp2 *= temp2;
            temp2 += SQ(temp3);
            temp3 = src->yy;
            temp2 += SQ(temp3);
            // temp2 = xy^2+zy^2+yy^2 == 1 for a rotation matrix
            temp2 = sqrtf(temp2);
            temp2 = temp3 / temp2; // yy in normalised column

            // for a rotation matrix, temp == yx and temp2 == yy which is the same as in the !nonUniformScale branch
            //dest->z = Math_Atan2S(temp, temp2);
            dest->z = Math_Atan2S(temp2, temp);
        }
    }
}


#define THIS ((EnWdhand*)thisx)

void EnWdhand_Init(Actor* thisx, PlayState* play);
void EnWdhand_Destroy(Actor* thisx, PlayState* play);
void EnWdhand_Update(Actor* thisx, PlayState* play);
void EnWdhand_Draw(Actor* thisx, PlayState* play);

void func_80AF4FF8(EnWdhand* this, PlayState* play);
void func_80AF4A88(EnWdhand* this, PlayState* play);
void EnWdhand_ReachPlayer(EnWdhand* this, PlayState* play);
void func_80AF4F30(EnWdhand* this, PlayState* play);
void func_80AF4FF8(EnWdhand* this, PlayState* play);
void EnWdhand_GrabPlayer(EnWdhand* this, PlayState* play);
void EnWdhand_Death(EnWdhand* this, PlayState* play);

void func_80AF4608(EnWdhand* this, Vec3f* dst);
void func_80AF4964(EnWdhand* this);
void func_80AF4ED0(EnWdhand* this);
void EnWdhand_SetupReachPlayer(EnWdhand* this);
void func_80AF4F6C(EnWdhand* this);
void EnWdhand_SetupGrabPlayer(EnWdhand* this, PlayState* play);
void func_80AF488C(Vec3s* dst, Vec3f* src);

ActorInit En_Wdhand_InitVars = {
    /**/ 0x025B,
    /**/ ACTORCAT_ENEMY,
    /**/ FLAGS,
    /**/ 0x021D,
    /**/ sizeof(EnWdhand),
    /**/ EnWdhand_Init,
    /**/ EnWdhand_Destroy,
    /**/ EnWdhand_Update,
    /**/ EnWdhand_Draw,
};

// static ColliderJntSphElementInit sJntSphElementsInit[7] = {
static ColliderJntSphElementInit D_80AF63E0[7] = {
    {
        {
            ELEMTYPE_UNK1,
            { 0xFFCFFFFF, 0x00, 0x00 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NONE,
            BUMP_ON,
            OCELEM_ON,
        },
        { 50, { { 0, 575, 0 }, 10 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK1,
            { 0xFFCFFFFF, 0x00, 0x00 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NONE,
            BUMP_ON,
            OCELEM_ON,
        },
        { 50, { { 0, 1725, 0 }, 10 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK1,
            { 0xFFCFFFFF, 0x00, 0x00 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NONE,
            BUMP_ON,
            OCELEM_ON,
        },
        { 50, { { 0, 575, 0 }, 10 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK1,
            { 0xFFCFFFFF, 0x00, 0x00 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NONE,
            BUMP_ON,
            OCELEM_ON,
        },
        { 50, { { 0, 1725, 0 }, 10 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK1,
            { 0xFFCFFFFF, 0x00, 0x00 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NONE,
            BUMP_ON,
            OCELEM_ON,
        },
        { 50, { { 0, 575, 0 }, 10 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK1,
            { 0xFFCFFFFF, 0x00, 0x00 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NONE,
            BUMP_ON,
            OCELEM_ON,
        },
        { 50, { { 0, 1725, 0 }, 10 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK1,
            { 0xFFCFFFFF, 0x00, 0x00 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NONE,
            BUMP_ON,
            OCELEM_ON,
        },
        { 50, { { 0, 1000, 0 }, 15 }, 100 },
    },
};

// static ColliderJntSphInit sJntSphInit = {
static ColliderJntSphInit D_80AF64DC = {
    {
        COLTYPE_HIT0,
        AT_NONE | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    ARRAY_COUNT(D_80AF63E0),
    D_80AF63E0, // sJntSphElementsInit,
};

// sColChkInfoInit
static CollisionCheckInfoInit D_80AF64EC = { 1, 25, 25, MASS_HEAVY };

// static DamageTable sDamageTable = {
static DamageTable D_80AF64F4 = {
    /* Deku Nut       */ DMG_ENTRY(0, 0x0),
    /* Deku Stick     */ DMG_ENTRY(0, 0x0),
    /* Horse trample  */ DMG_ENTRY(0, 0x0),
    /* Explosives     */ DMG_ENTRY(1, 0x0),
    /* Zora boomerang */ DMG_ENTRY(1, 0x0),
    /* Normal arrow   */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x06   */ DMG_ENTRY(0, 0x0),
    /* Hookshot       */ DMG_ENTRY(1, 0x0),
    /* Goron punch    */ DMG_ENTRY(0, 0x0),
    /* Sword          */ DMG_ENTRY(0, 0x0),
    /* Goron pound    */ DMG_ENTRY(0, 0x0),
    /* Fire arrow     */ DMG_ENTRY(0, 0x0),
    /* Ice arrow      */ DMG_ENTRY(0, 0x0),
    /* Light arrow    */ DMG_ENTRY(0, 0x0),
    /* Goron spikes   */ DMG_ENTRY(0, 0x0),
    /* Deku spin      */ DMG_ENTRY(0, 0x0),
    /* Deku bubble    */ DMG_ENTRY(0, 0x0),
    /* Deku launch    */ DMG_ENTRY(1, 0x0),
    /* UNK_DMG_0x12   */ DMG_ENTRY(1, 0x0),
    /* Zora barrier   */ DMG_ENTRY(1, 0x0),
    /* Normal shield  */ DMG_ENTRY(0, 0x0),
    /* Light ray      */ DMG_ENTRY(0, 0x0),
    /* Thrown object  */ DMG_ENTRY(0, 0x0),
    /* Zora punch     */ DMG_ENTRY(0, 0x0),
    /* Spin attack    */ DMG_ENTRY(0, 0x0),
    /* Sword beam     */ DMG_ENTRY(0, 0x0),
    /* Normal Roll    */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1B   */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1C   */ DMG_ENTRY(0, 0x0),
    /* Unblockable    */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1E   */ DMG_ENTRY(0, 0x0),
    /* Powder Keg     */ DMG_ENTRY(0, 0x0),
};

void EnWdhand_Init(Actor* thisx, PlayState* play) {
    EnWdhand* this = THIS;
    s32 i;
    Vec3f sp4C;

    SkelAnime_InitFlex(play, &this->unk144, 0x06001e20, 0x06000854, this->unk188,
                       this->unk1B8, 8);
    CollisionCheck_SetInfo(&this->actor.colChkInfo, &D_80AF64F4, &D_80AF64EC);
    Matrix_Translate(this->actor.world.pos.x, this->actor.world.pos.y, this->actor.world.pos.z, MTXMODE_NEW);
    Matrix_RotateY(this->actor.shape.rot.y, MTXMODE_APPLY);
    Matrix_RotateX(this->actor.shape.rot.x, MTXMODE_APPLY);
    Matrix_RotateZ(this->actor.shape.rot.z, MTXMODE_APPLY);
    Matrix_MultVecY(3.0f, &this->unk25C[1]);
    Matrix_MtxFCopy(&this->unk21C, Matrix_GetCurrent());
    Matrix_MtxFToYXZRot(&this->unk21C, &this->actor.shape.rot, 0);
    Matrix_Transpose(&this->unk21C);
    this->actor.world.rot.y = this->actor.shape.rot.y;
    func_80AF4608(this, &sp4C);
    //this->actor.gravity = -1.0f
    this->actor.speedXZ = sp4C.z;
    this->actor.velocity.y = sp4C.y;
    this->actor.velocity.x = Math_SinS(this->actor.world.rot.y) * this->actor.speedXZ;
    this->actor.velocity.z = Math_CosS(this->actor.world.rot.y) * this->actor.speedXZ;
    Collider_InitJntSph(play, &this->unk274);
    Collider_SetJntSph(play, &this->unk274, &this->actor, &D_80AF64DC, this->unk294);

    for (i = 0; i < ARRAY_COUNT(this->unk294); i++) {
        ColliderJntSphElement* elem = &this->unk274.elements[i];
        elem->dim.worldSphere.radius = elem->dim.modelSphere.radius;
        func_80AF488C(&elem->dim.worldSphere.center, &this->actor.world.pos);
    }

    for (i = 0; i < 3; i++) {
        this->unk210[i] = 1.0f;
    }

    this->unk1EC[0].x = -1;
    this->unk1EC[0].y = 0;
    this->actor.world.rot.x = 0;
    //this->actor.world.rot.z = Math_Atan2S(this->unk21C.mf[2][1], this->unk21C.mf[0][1]);
    this->actor.world.rot.z = Math_Atan2S(this->unk21C.mf[0][1], this->unk21C.mf[2][1]);
    this->actor.naviEnemyId = NAVI_ENEMY_POE_SISTER_AMY;
    func_80AF4964(this);
}

void EnWdhand_Destroy(Actor* thisx, PlayState* play) {
    EnWdhand* this = THIS;

    Collider_DestroyJntSph(play, &this->unk274);
}

void func_80AF4608(EnWdhand* this, Vec3f* dst) {
    s32 var_v0;

    var_v0 = this->actor.params & 0x7F;
    if (var_v0 == 0x7F) {
        var_v0 = 0x28;
    }
    dst->y = var_v0 * 0.2f;

    var_v0 = (this->actor.params >> 8) & 0x7F;
    if (var_v0 == 0x7F) {
        var_v0 = 0x28;
    }
    dst->z = var_v0 * 0.2f;

    dst->x = 0.0f;
}

void func_80AF4670(EnWdhand* this, Player* player, s16* arg2, s16* arg3) {
    Vec3f sp24;
    Vec3f sp18;

    Math_Vec3f_Diff(&player->bodyPartsPos[3], &this->actor.world.pos, &sp24);
    Matrix_Put(&this->unk21C);
    Matrix_MultVec3f(&sp24, &sp18);
    *arg3 = Actor_WorldYawTowardPoint(&this->actor, &sp18);
    *arg2 = Actor_WorldPitchTowardPoint(&this->actor, &sp18) + 0x4000;
}

void func_80AF46F0(EnWdhand* this, s32 arg1, s32 arg2, Vec3f* arg3) {
    MtxF* sp24 = Matrix_GetCurrent();
    f32 scale;

    Matrix_RotateZYX(this->unk1EC[arg1 + 1].x, this->unk1EC[arg1 + 1].y, 0, MTXMODE_APPLY);
    if (arg2 != 0) {
        Matrix_RotateY((s16)(this->unk1EC[arg1 + 1].y * -1), MTXMODE_APPLY);
    }
    if (arg1 == 3) {
        if (this->unk20C > 1.0f) {
            if (this->unk20C > 1.5f) {
                scale = 1.5f;
            } else {
                scale = this->unk20C;
            }
            Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
        }
    } else {
        if ((this->unk210[arg1] < 1.0f) || (this->unk20C > 1.0f)) {
            if (this->unk210[arg1] < 0.1f) {
                Matrix_Scale(0.0f, this->unk20C * this->unk210[arg1], 0.0f, MTXMODE_APPLY);
            } else {
                Matrix_Scale(1.0f, this->unk20C * this->unk210[arg1], 1.0f, MTXMODE_APPLY);
            }
        }
    }
    sp24->mf[3][0] = arg3->x;
    sp24->mf[3][1] = arg3->y;
    sp24->mf[3][2] = arg3->z;
}

void func_80AF488C(Vec3s* dst, Vec3f* src) {
    dst->x = src->x;
    dst->y = src->y;
    dst->z = src->z;
}

s16 func_80AF48D0(EnWdhand* this, s32 arg1) {
    return this->unk1EC[arg1 + 1].z * Math_SinF((this->unk1EC[0].z - (arg1 * 0x14)) * 0.07853982f);
}

void func_80AF4964(EnWdhand* this) {
    s32 var_s0;

    Animation_MorphToLoop(&this->unk144, 0x06000854, -5.0f);
    if (this->actionFunc != func_80AF4FF8) {
        this->unk1EC[0].z = 0x50;
        this->unk1EC[1].y = Rand_Next() >> 0x10;

        for (var_s0 = 1; var_s0 < 4; var_s0++) {
            this->unk1EC[var_s0 + 1].y = this->unk1EC[var_s0].y + (s32)Rand_ZeroFloat((f32)0x2000) + 0x8000;
        }

        for (var_s0 = 0; var_s0 < 4; var_s0++) {
            this->unk1EC[var_s0 + 1].z = Rand_S16Offset(0x1800, 0x1000);
        }
    }

    for (var_s0 = 0; var_s0 < 4; var_s0++) {
        this->unk1EC[var_s0 + 1].x = func_80AF48D0(this, var_s0);
    }

    this->actionFunc = func_80AF4A88;
    this->unk20C = 1.0f;
}

void func_80AF4A88(EnWdhand* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 var_s1;
    s16 sp4E;
    s16 sp4C;

    SkelAnime_Update(&this->unk144);

    this->unk1EC[0].z--;

    for (var_s1 = 0; var_s1 < 4; var_s1++) {
        this->unk1EC[var_s1 + 1].x = func_80AF48D0(this, var_s1);
        if (this->unk1EC[0].z == 0x14 * var_s1) {
            if (var_s1 != 0) {
                this->unk1EC[var_s1 + 1].y = this->unk1EC[var_s1].y + (s32)Rand_CenteredFloat(16384.0f);
                this->unk1EC[var_s1 + 1].y -= 0x8000;
            } else {
                this->unk1EC[1].y += (s16)(s32)Rand_CenteredFloat(16384.0f);
            }
            this->unk1EC[var_s1 + 1].z = Rand_S16Offset(0x1800, 0x1000);
        }
    }

    if (this->unk1EC[0].z == 0) {
        this->unk1EC[0].z = 0x50;
    }

    if (!(player->stateFlags2 & PLAYER_STATE2_7) && (this->actor.xyzDistToPlayerSq < SQ(150.00f)) && (!(player->stateFlags1 & PLAYER_STATE1_26))) {
         Audio_PlayActorSfx2(&this->actor, NA_SE_EN_DEADHAND_HAND_AT);
        //Audio_PlayActorSfx2(&this->actor, NA_SE_EN_DEADHAND_GRIP);
        func_80AF4670(this, player, &sp4E, &sp4C);
        if (ABS_ALT(sp4E) <= 0x4000) {
            EnWdhand_SetupReachPlayer(this);
        }
    }
}

void EnWdhand_SetupReachPlayer(EnWdhand* this) {
    Animation_MorphToPlayOnce(&this->unk144, 0x06000534, -5.0f);
    this->unk1EC[0].z = 5;
    this->actionFunc = EnWdhand_ReachPlayer;
}

void EnWdhand_ReachPlayer(EnWdhand* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 var_s0 = 1;
    s32 var_s1;
    s16 sp6A;
    s16 sp68;

    SkelAnime_Update(&this->unk144);
    if (this->unk1EC[0].z > 0) {
        this->unk1EC[0].z--;
        if (this->unk1EC[0].z == 0) {
            this->unk274.base.atFlags |= 1;
        }
    } else {
        Math_StepToF(&this->unk20C, 1.5f, 0.05f);
        func_80AF4670(this, player, &sp6A, &sp68);

        for (var_s1 = 0; var_s1 < 4; var_s1++) {
            s16 var_v1 = sp68 - this->unk1EC[1 + var_s1].y;

            if (ABS_ALT(var_v1) < 0x4000) {
                var_s0 &=
                    Math_ScaledStepToS(&this->unk1EC[1 + var_s1].x, sp6A, (s16)(s32)((var_s1 * 0.1f + 1.0f) * 1920.0f));
                var_s0 &= Math_ScaledStepToS(&this->unk1EC[1 + var_s1].y, sp68, 0x800);
            } else {
                var_s0 &= Math_ScaledStepToS(&this->unk1EC[1 + var_s1].x, -sp6A,
                                             (s16)(s32)((var_s1 * 0.1f + 1.0f) * 1920.0f));
                var_s0 &= Math_ScaledStepToS(&this->unk1EC[1 + var_s1].y, sp68 + 0x8000, 0x800);
            }
        }

        if ((this->unk274.base.atFlags & 2) && (play->grabPlayer(play, player) != 0)) {
            EnWdhand_SetupGrabPlayer(this, play);
        } else if (var_s0 != 0) {
            func_80AF4ED0(this);
        }

        if (this->unk274.base.atFlags & 2) {
            this->unk274.base.atFlags &= ~2;
        }
    }
}

void func_80AF4ED0(EnWdhand* this) {
    Animation_PlayOnce(&this->unk144, 0x060000f4);
    this->unk274.base.atFlags &= ~1;
    this->unk274.base.atFlags &= ~2;
    this->actionFunc = func_80AF4F30;
    this->unk20C = 1.5f;
}

void func_80AF4F30(EnWdhand* this, PlayState* play) {
    if (SkelAnime_Update(&this->unk144) != 0) {
        func_80AF4F6C(this);
    }
}

void func_80AF4F6C(EnWdhand* this) {
    s32 var_s0;

    Animation_MorphToLoop(&this->unk144, 0x06000854, -10.0f);

    for (var_s0 = 0; var_s0 < 4; var_s0++) {
        this->unk1EC[1 + var_s0].z = Rand_S16Offset(0x1800, 0x1000);
    }
    this->unk1EC[0].z = 0x50;
    this->actionFunc = func_80AF4FF8;
}

void func_80AF4FF8(EnWdhand* this, PlayState* play) {
    s32 var_s0;
    s32 var_s5;

    SkelAnime_Update(&this->unk144);

    this->unk1EC[0].z--;

    var_s5 = Math_ScaledStepToS(&this->unk1EC[1].x, func_80AF48D0(this, 0), 0x200);

    for (var_s0 = 1; var_s0 < 4; var_s0++) {
        var_s5 &= Math_ScaledStepToS(&this->unk1EC[1 + var_s0].x, func_80AF48D0(this, var_s0), 0x300);
        if (0x14 * var_s0 == this->unk1EC[0].z) {
            this->unk1EC[1 + var_s0].z = Rand_S16Offset(0x1800, 0x1000);
        }
    }

    if (this->unk1EC[0].z == 0) {
        this->unk1EC[0].z = 0x50;
        this->unk1EC[1].z = Rand_S16Offset(0x1800, 0x1000);
    }

    Math_StepToF(&this->unk20C, 1.0f, 0.05f);

    if (var_s5 != 0) {
        func_80AF4964(this);
    }
}

void EnWdhand_SetupGrabPlayer(EnWdhand* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 var_s0;

    this->unk274.base.atFlags &= ~1;
    this->unk274.base.atFlags &= ~2;
    this->unk274.base.ocFlags1 &= ~1;
    Animation_PlayOnce(&this->unk144, 0x060000f4);

    for (var_s0 = 0; var_s0 < 4; var_s0++) {
        this->unk1EC[1 + var_s0].z = Rand_S16Offset(0x2000, 0x1000);
    }
    this->unk1EC[0].z = Rand_S16Offset(0x10,0x10); //0x50
    player->actor.parent = &this->actor;
    Matrix_Transpose(&this->unk21C);
    this->unk20C = 1.5f;

    //Actor_PlaySfx(&this->actor, NA_SE_EN_HANDW_GET);
    Audio_PlayActorSfx2(&this->actor, NA_SE_EN_DEADHAND_GRIP);
    Audio_PlayActorSfx2(&this->actor, NA_SE_EN_SHADEST_CATCH);
    this->actionFunc = EnWdhand_GrabPlayer;
}

void EnWdhand_GrabPlayer(EnWdhand* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 var_s1;
    s32 var_s2;
    Vec3f sp68;
    f32 var_fv1;

    SkelAnime_Update(&this->unk144);
    this->unk1EC[0].z--;
    player->unk_850 = 0;
    var_s2 = this->unk1EC[0].z;
    for (var_s1 = 0; var_s1 < 4; var_s1++, var_s2 += 2) {
        if (this->unk1EC[0].z < 0x4C) {
            this->unk1EC[1 + var_s1].x = (s16)(s32)(this->unk1EC[1 + var_s1].z * Math_SinF(var_s2 * 0.3926991f));
        } else {
            Math_ScaledStepToS(&this->unk1EC[1 + var_s1].x,
                               (s16)(s32)(this->unk1EC[1 + var_s1].z * Math_SinF(var_s2 * 0.3926991f)), 0x400);
        }
        if (!(var_s2 & 0xF)) {
            if (var_s2 == 0x10) {
                this->unk1EC[1 + var_s1].y = 0;
            } else if (var_s1 != 0) {
                this->unk1EC[1 + var_s1].y = this->unk1EC[var_s1].y + (s32)Rand_CenteredFloat(12288.0f);
            } else {
                this->unk1EC[1].y += (s16)Rand_CenteredFloat(12288.0f);
            }
            this->unk1EC[1 + var_s1].z = Rand_S16Offset(0x2000, 0x1000);
        }
    }

    if (this->unk1EC[0].z < 4) {
        Math_StepToF(&this->unk20C, 1.5f, 0.125f);
    } else if (this->unk1EC[0].z < 0x10) {
        Math_StepToF(&this->unk20C, 2.0f, 0.041666668f);
    }

    if (this->unk1EC[0].z == 0) {
        this->unk274.base.ocFlags1 |= 1;
        func_80AF4F6C(this);
    } else if (this->unk1EC[0].z >= 4) {
        Matrix_Put(&this->unk21C);
        Matrix_Scale(0.01f, 0.01f, 0.01f, MTXMODE_APPLY);
        Matrix_MultVecY(300.0f, &sp68);

        for (var_s1 = 0; var_s1 < 3; var_s1++) {
            Matrix_Push();
            func_80AF46F0(this, var_s1, 0, &sp68);
            Matrix_MultVecY(2300.0f, &sp68);
            Matrix_Pop();
        }

        func_80AF46F0(this, 3, 1, &sp68);
        Matrix_MtxFToYXZRot(Matrix_GetCurrent(), &player->actor.shape.rot, 0);

        if (this->unk20C > 1.0f) {
            if (this->unk20C > 1.5f) {
                var_fv1 = 1.5f;
            } else {
                var_fv1 = this->unk20C;
            }
            Matrix_RotateZ((s16)(s32)((var_fv1 - 1.0f) * -32768.0f), MTXMODE_APPLY);
        }
        Matrix_MultVecY(1000.0f, &player->actor.world.pos);

        if ((this->unk1EC[0].z == 4) && (&this->actor == player->actor.parent)) {
            player->unk_850 = 100;
            player->actor.parent = NULL;
            Matrix_Transpose(&this->unk21C);
            player->actor.shape.rot.x = 0;
            player->actor.shape.rot.z = 0;
            player->actor.world.pos.x += 2.0f * this->actor.velocity.x;
            player->actor.world.pos.y += 2.0f * this->actor.velocity.y;
            player->actor.world.pos.z += 2.0f * this->actor.velocity.z;
            func_8002F6D4(play, &this->actor, this->actor.speedXZ + 4.0f , this->actor.world.rot.y, this->actor.velocity.y + 1.0f, 0);
            //Actor_PlaySfx(&this->actor, NA_SE_EN_HANDW_RELEASE);
            Audio_PlayActorSfx2(&this->actor, NA_SE_EN_DEADHAND_HAND_AT);
            //SoundEffect_PlayOneshot(SOUND_ENEMY_DEXIHAND_THROW, 1.0f, 1.0f, &this->actor.world.pos, 0.1f, 1000.0f, this);
        } else if (this->unk1EC[0].z == 2) {
            Animation_PlayOnce(&this->unk144, 0x06000364);
        }
    }
}

s32 func_80AF5650(EnWdhand* this, s32 arg1) {
    this->unk210[arg1] -= 0.1f;
    if (this->unk210[arg1] <= 0.05f) {
        this->unk210[arg1] = 0.05f;
        return true;
    }
    return false;
}

void EnWdhand_SetupDeath(EnWdhand* this) {
    s32 var_s0;

    this->actor.flags &= ~1;
    this->actor.flags |= 0x10;

    for (var_s0 = 0; var_s0 < ARRAY_COUNT(this->unk294); var_s0++) {
        if (this->unk274.elements[var_s0].info.bumperFlags & 2) { //get the number of palm collider
            break;
        }
    }

    this->unk1EC[0].y = (s16)((s32)((s32)(var_s0 + 1) / 2) % 4);
    this->unk1EC[0].x = this->unk1EC[0].y - 1;
    Matrix_Put(&this->unk21C);
    Matrix_Scale(0.01f, 0.01f, 0.01f, MTXMODE_APPLY);
    Matrix_MultVecY(300.0f, &this->unk25C[1]);

    for (var_s0 = 0; var_s0 < this->unk1EC[0].y; var_s0++) {
        Matrix_Push();
        func_80AF46F0(this, var_s0, 0, &this->unk25C[1]);
        Matrix_MultVecY(2300.0f, &this->unk25C[1]);
        Matrix_Pop();
    }
    // if(this->actor.bgCheckFlags & BGCHECKFLAG_WATER){
    //this->actor.velocity.y = -3.0f;
    
    // this->actor.velocity.x = 2.0f * Math_SinS(this->actor.world.rot.z);
    // this->actor.velocity.z = 2.0f * Math_CosS(this->actor.world.rot.z);
    this->unk1EC[0].z = 5;
    this->actionFunc = EnWdhand_Death;
}

void EnWdhand_Death(EnWdhand* this, PlayState* play) {
    static Vec3f D_80AF6514 = { 0.0f, 0.0f, 0.0f };
    static Vec3f D_80AF6520 = { 0.0f, 0.05f, 0.0f };
    Vec3s* temp;
    Vec3f spA0;
    Vec3f sp94;
    s16 temp_s0_6;
    s32 var_s0;
    s32 var_s2;
    s32 var_s3;
    Vec3f* temp_s1;
    s32 tmp;
    Vec3f sp70;

    temp = &this->unk274.elements[6].dim.worldSphere.center;
    temp_s1 = &this->unk25C[1];

    spA0.x = (temp_s1->x - temp->x) * 0.5f;
    spA0.y = (temp_s1->y - temp->y) * 0.5f;
    spA0.z = (temp_s1->z - temp->z) * 0.5f;
    Matrix_RotateX(0x100, MTXMODE_NEW);
    Matrix_MultVec3f(&spA0, &sp94);
    temp_s1->x = spA0.x + sp94.x + temp->x;
    temp_s1->y = spA0.y + sp94.y + temp->y;
    temp_s1->z = spA0.z + sp94.z + temp->z;

    //Math_Vec3f_Sum(temp_s1, &this->actor.velocity, temp_s1);

    this->actor.world.rot.x += 0x100;

    if (this->unk1EC[0].x >= 0 && func_80AF5650(this, this->unk1EC[0].x)) {
        this->unk1EC[0].x--;
    }


    this->unk1EC[0].z--;

    tmp = this->unk1EC[0].x + 1;
    var_s3 = this->unk1EC[0].z;
    for (var_s2 = 0; var_s2 < tmp; var_s2++) {
        if (this->unk1EC[0].z > 0) {
            Math_ScaledStepToS(&this->unk1EC[1 + var_s2].x,
                               this->unk1EC[1 + var_s2].z * (Math_SinF(var_s3 * 0.8975979f)), 0x200);
        } else if (Rand_ZeroOne() < 0.65f) {
            this->unk1EC[1 + var_s2].x = this->unk1EC[1 + var_s2].z * Math_SinF(var_s3 * 0.8975979f);
        }
        if ((var_s3 % 7) == 0) {
            if (var_s2 != 0) {
                this->unk1EC[1 + var_s2].y = this->unk1EC[var_s2].y + (s32)Rand_CenteredFloat(12288.0f);
            } else {
                this->unk1EC[1].y += (s16)Rand_CenteredFloat(12288.0f);
            }
            this->unk1EC[1 + var_s2].z = Rand_S16Offset((s16)((var_s2 * 0x100) + 0xC00), 0x800);
        }
        var_s3 += 2;
    }

    if ((this->unk1EC[0].x < 0) ) {
        if (Math_StepToF(&this->actor.scale.x, 0.0f, 0.001f) != 0) {
            for (var_s0 = 0; var_s0 < 2; var_s0++) {
                for (var_s2 = 0; var_s2 < 5; var_s2++) {
                    temp_s1 = &this->unk25C[var_s0];
                    D_80AF6514.y = Rand_ZeroOne() + 1.0f;
                    temp_s0_6 = Rand_S16Offset(0x28, 0x28);
                    sp70.x = temp_s1->x + Rand_CenteredFloat(12.0f);
                    sp70.y = temp_s1->y + Rand_CenteredFloat(12.0f);
                    sp70.z = temp_s1->z + Rand_CenteredFloat(12.0f);
                    //EffectSsDtBubble_SpawnColorProfile(play, &sp70, &D_80AF6514, &D_80AF6520, temp_s0_6, 25, 2, 1);
                }
            }

            Actor_Kill(&this->actor);
        } else {
            this->actor.scale.y = this->actor.scale.x;
            this->actor.scale.z = this->actor.scale.x;
        }
    }

    for (var_s2 = 0; var_s2 < 2; var_s2++) {
        D_80AF6514.y = Rand_ZeroOne() + 1.0f;
        // EffectSsDtBubble_SpawnColorProfile(play, &this->unk25C[var_s2], &D_80AF6514, &D_80AF6520,
        //                                    Rand_S16Offset(40, 40), 25, 2, 1);
    }
}

void EnWdhand_DamageCheck(EnWdhand* this, PlayState* play) {

    if (this->unk274.base.acFlags & 2) {
        Player* player = GET_PLAYER(play);
        s32 temp = ~1;
        this->unk274.base.acFlags &= temp;
        this->unk274.base.acFlags &= ~2;
        this->unk274.base.atFlags &= temp;
        this->unk274.base.ocFlags1 &= temp;
        Actor_SetDropFlagJntSph(&this->actor, &this->unk274, true);
        Enemy_StartFinishingBlow(play, &this->actor);
        if ((player->stateFlags2 & PLAYER_STATE2_7) && (&this->actor == player->actor.parent)) {
            player->unk_850 = 100;
            player->actor.parent = NULL;
            player->actor.shape.rot.x = 0;
            player->actor.shape.rot.z = 0;
            func_8002F6D4(play, &this->actor, this->actor.speedXZ, this->actor.world.rot.y, this->actor.velocity.y, 0U);
        } else {
            //Matrix_Transpose(&this->unk21C);
        }
        EnWdhand_SetupDeath(this);
    }
}

void EnWdhand_Update(Actor* thisx, PlayState* play) {
    EnWdhand* this = THIS;

    EnWdhand_DamageCheck(this, play);

    this->actionFunc(this, play);

    if (this->unk274.base.atFlags & 1) {
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->unk274.base);
    }
    if (this->unk274.base.acFlags & 1) {
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->unk274.base);
    }
    if (this->unk274.base.ocFlags1 & 1) {
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->unk274.base);
    }
}

void func_80AF5FE4(EnWdhand* this, s32 arg1, Vec3f* arg2) {
    func_80AF46F0(this, arg1, 1, arg2);
    Matrix_MultVecY(575.0f, arg2);
    func_80AF488C(&this->unk274.elements[2 * arg1].dim.worldSphere.center, arg2);
    Matrix_MultVecY(1725.0f, arg2);
    func_80AF488C(&this->unk274.elements[2 * arg1 + 1].dim.worldSphere.center, arg2);
    Matrix_MultVecY(2300.0f, arg2);
}

void EnWdhand_Draw(Actor* thisx, PlayState* play) {
    EnWdhand* this = THIS;
    Vec3f sp90;
    Gfx* dl;
    s32 var;
    s32 var_s1;

    OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

    dl = POLY_OPA_DISP;

    gSPDisplayList(&dl[0], SetupDL25); // same problem as ovl_En_Knight. TODO: This may be an issue
    gSPMatrix(&dl[1], Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(&dl[2], 0x060014c0);

    Matrix_MultVecY(300.0f, &sp90);

    dl = &dl[3];

    var = this->unk1EC[0].x + 1;
    for (var_s1 = 0; var_s1 < var; var_s1++) {
        Matrix_Push();
        func_80AF5FE4(this, var_s1, &sp90);
        gSPMatrix(dl++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(dl++, 0x060015b0);
        Matrix_Pop();
    }

    Math_Vec3f_Copy(&this->unk25C[0], &sp90);
    Math_Vec3f_Copy(&sp90, &this->unk25C[1]);
    if (this->actor.world.rot.x != 0) {
        Matrix_RotateZYX(this->actor.world.rot.x, this->actor.world.rot.z, 0, MTXMODE_NEW);
        Matrix_RotateY(-this->actor.world.rot.z, MTXMODE_APPLY);
        Matrix_Mult(&this->unk21C, MTXMODE_APPLY);
        Matrix_Scale(this->actor.scale.x, this->actor.scale.y, this->actor.scale.z, MTXMODE_APPLY);
    }

    for (var_s1 = this->unk1EC[0].y; var_s1 < 3; var_s1++) {
        Matrix_Push();
        func_80AF5FE4(this, var_s1, &sp90);
        gSPMatrix(dl++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(dl++, 0x060015b0);
        Matrix_Pop();
    }

    func_80AF46F0(this, 3, 1, &sp90);
    if (this->unk20C > 1.0f) {
        s32 pad;
        f32 var_fv1;

        if (this->unk20C > 1.5f) {
            var_fv1 = 1.5f;
        } else {
            var_fv1 = this->unk20C;
        }
        Matrix_RotateZ((s16)(s32)((var_fv1 - 1.0f) * -32768.0f), MTXMODE_APPLY);
    }
    if (this->actor.scale.x < 0.01f) {
        Matrix_Translate(0.0f, (10.0f / this->actor.scale.x) - 1000.0f, 0.0f, MTXMODE_APPLY);
    }
    Math_Vec3f_Copy(&this->actor.focus.pos, &sp90);
    Matrix_MultVecY(1000.0f, &sp90);
    func_80AF488C(&this->unk274.elements[6].dim.worldSphere.center, &sp90);

    POLY_OPA_DISP = dl;
    SkelAnime_DrawFlexOpa(play, this->unk144.skeleton, this->unk144.jointTable, this->unk144.dListCount, NULL, NULL,
                          &this->actor);

    CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
}
