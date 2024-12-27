#include <uLib.h>
#include "assets/objects/gameplay_keep/gameplay_keep.h"

#define BODYPART_NONE -1

MtxF* sCurrentMatrix; //!< original name: "Matrix_now"

Mtx sActorHiliteMtx;


Gfx* Hilite_Draw(Vec3f* object, Vec3f* eye, Vec3f* lightDir, GraphicsContext* gfxCtx, Gfx* dl, Hilite** hiliteP) {
    LookAt* lookAt = Graph_Alloc(gfxCtx, sizeof(LookAt));
    f32 correctedEyeX = (eye->x == object->x) && (eye->z == object->z) ? eye->x + 0.001f : eye->x;

    *hiliteP = Graph_Alloc(gfxCtx, sizeof(Hilite));

    guLookAtHilite(&sActorHiliteMtx, lookAt, *hiliteP, correctedEyeX, eye->y, eye->z, object->x, object->y, object->z,
                   0.0f, 1.0f, 0.0f, lightDir->x, lightDir->y, lightDir->z, lightDir->x, lightDir->y, lightDir->z, 0x10,
                   0x10);

    gSPLookAt(dl++, lookAt);
    gDPSetHilite1Tile(dl++, 1, *hiliteP, 0x10, 0x10);

    return dl;
}


Hilite* Hilite_DrawXlu(Vec3f* object, Vec3f* eye, Vec3f* lightDir, GraphicsContext* gfxCtx) {
    Hilite* hilite;

    OPEN_DISPS(gfxCtx, __FILE__, __LINE__);

    POLY_XLU_DISP = Hilite_Draw(object, eye, lightDir, gfxCtx, POLY_XLU_DISP, &hilite);

    CLOSE_DISPS(gfxCtx, __FILE__, __LINE__);

    return hilite;
}


void* Lib_SegmentedToVirtual(void* ptr) {
    return SEGMENTED_TO_VIRTUAL(ptr);
}

void Matrix_RotateXS(s16 x, MatrixMode mode) {
    MtxF* cmf;
    f32 sin;
    f32 cos;
    f32 tempY;
    f32 tempZ;

    if (mode == MTXMODE_APPLY) {
        if (x != 0) {
            cmf = sCurrentMatrix;

            sin = Math_SinS(x);
            cos = Math_CosS(x);

            tempY = cmf->xy;
            tempZ = cmf->xz;
            cmf->xy = tempY * cos + tempZ * sin;
            cmf->xz = tempZ * cos - tempY * sin;

            tempY = cmf->yy;
            tempZ = cmf->yz;
            cmf->yy = tempY * cos + tempZ * sin;
            cmf->yz = tempZ * cos - tempY * sin;

            tempY = cmf->zy;
            tempZ = cmf->zz;
            cmf->zy = tempY * cos + tempZ * sin;
            cmf->zz = tempZ * cos - tempY * sin;

            tempY = cmf->wy;
            tempZ = cmf->wz;
            cmf->wy = tempY * cos + tempZ * sin;
            cmf->wz = tempZ * cos - tempY * sin;
        }
    } else {
        cmf = sCurrentMatrix;

        if (x != 0) {
            sin = Math_SinS(x);
            cos = Math_CosS(x);
        } else {
            sin = 0.0f;
            cos = 1.0f;
        }

        cmf->yx = 0.0f;
        cmf->zx = 0.0f;
        cmf->wx = 0.0f;
        cmf->xy = 0.0f;
        cmf->wy = 0.0f;
        cmf->xz = 0.0f;
        cmf->wz = 0.0f;
        cmf->xw = 0.0f;
        cmf->yw = 0.0f;
        cmf->zw = 0.0f;
        cmf->xx = 1.0f;
        cmf->ww = 1.0f;
        cmf->yy = cos;
        cmf->zz = cos;
        cmf->zy = sin;
        cmf->yz = -sin;
    }
}


void Matrix_RotateXF(f32 x, MatrixMode mode) {
    MtxF* cmf;
    f32 sin;
    f32 cos;
    f32 tempY;
    f32 tempZ;
    f32 zero = 0.0;
    f32 one = 1.0;

    if (mode == MTXMODE_APPLY) {
        if (x != 0) {
            cmf = sCurrentMatrix;

            sin = sinf(x);
            cos = cosf(x);

            tempY = cmf->xy;
            tempZ = cmf->xz;
            cmf->xy = tempY * cos + tempZ * sin;
            cmf->xz = tempZ * cos - tempY * sin;

            tempY = cmf->yy;
            tempZ = cmf->yz;
            cmf->yy = tempY * cos + tempZ * sin;
            cmf->yz = tempZ * cos - tempY * sin;

            tempY = cmf->zy;
            tempZ = cmf->zz;
            cmf->zy = tempY * cos + tempZ * sin;
            cmf->zz = tempZ * cos - tempY * sin;

            tempY = cmf->wy;
            tempZ = cmf->wz;
            cmf->wy = tempY * cos + tempZ * sin;
            cmf->wz = tempZ * cos - tempY * sin;
        }
    } else {
        cmf = sCurrentMatrix;

        if (x != 0) {
            sin = sinf(x);
            cos = cosf(x);
        } else {
            sin = zero;
            cos = one;
        }

        cmf->xx = one;
        cmf->yx = zero;
        cmf->zx = zero;
        cmf->wx = zero;
        cmf->xy = zero;
        cmf->yy = cos;
        cmf->zy = sin;
        cmf->wy = zero;
        cmf->xz = zero;
        cmf->yz = -sin;
        cmf->zz = cos;
        cmf->wz = zero;
        cmf->xw = zero;
        cmf->yw = zero;
        cmf->zw = zero;
        cmf->ww = one;
    }
}


void Matrix_RotateXFApply(f32 x) {
    MtxF* cmf;
    f32 sin;
    f32 cos;
    f32 tempY;
    f32 tempZ;
    s32 pad;

    if (x != 0.0f) {
        cmf = sCurrentMatrix;

        sin = sins(RAD_TO_BINANG(x)) * SHT_MINV;
        cos = coss(RAD_TO_BINANG(x)) * SHT_MINV;

        tempY = cmf->xy;
        tempZ = cmf->xz;
        cmf->xy = (tempY * cos) + (tempZ * sin);
        cmf->xz = (tempZ * cos) - (tempY * sin);

        tempY = cmf->yy;
        tempZ = cmf->yz;
        cmf->yy = (tempY * cos) + (tempZ * sin);
        cmf->yz = (tempZ * cos) - (tempY * sin);

        tempY = cmf->zy;
        tempZ = cmf->zz;
        cmf->zy = (tempY * cos) + (tempZ * sin);
        cmf->zz = (tempZ * cos) - (tempY * sin);

        tempY = cmf->wy;
        tempZ = cmf->wz;
        cmf->wy = (tempY * cos) + (tempZ * sin);
        cmf->wz = (tempZ * cos) - (tempY * sin);
    }
}
void Matrix_RotateYS(s16 y, MatrixMode mode) {
    MtxF* cmf;
    f32 sin;
    f32 cos;
    f32 tempX;
    f32 tempZ;

    if (mode == MTXMODE_APPLY) {
        if (y != 0) {
            cmf = sCurrentMatrix;

            sin = Math_SinS(y);
            cos = Math_CosS(y);

            tempX = cmf->xx;
            tempZ = cmf->xz;
            cmf->xx = tempX * cos - tempZ * sin;
            cmf->xz = tempX * sin + tempZ * cos;

            tempX = cmf->yx;
            tempZ = cmf->yz;
            cmf->yx = tempX * cos - tempZ * sin;
            cmf->yz = tempX * sin + tempZ * cos;

            tempX = cmf->zx;
            tempZ = cmf->zz;
            cmf->zx = tempX * cos - tempZ * sin;
            cmf->zz = tempX * sin + tempZ * cos;

            tempX = cmf->wx;
            tempZ = cmf->wz;
            cmf->wx = tempX * cos - tempZ * sin;
            cmf->wz = tempX * sin + tempZ * cos;
        }
    } else {
        cmf = sCurrentMatrix;

        if (y != 0) {
            sin = Math_SinS(y);
            cos = Math_CosS(y);
        } else {
            sin = 0.0f;
            cos = 1.0f;
        }

        cmf->yx = 0.0f;
        cmf->wx = 0.0f;
        cmf->xy = 0.0f;
        cmf->zy = 0.0f;
        cmf->wy = 0.0f;
        cmf->yz = 0.0f;
        cmf->wz = 0.0f;
        cmf->xw = 0.0f;
        cmf->yw = 0.0f;
        cmf->zw = 0.0f;
        cmf->yy = 1.0f;
        cmf->ww = 1.0f;
        cmf->xx = cos;
        cmf->zz = cos;
        cmf->zx = -sin;
        cmf->xz = sin;
    }
}

void Matrix_RotateYF(f32 y, MatrixMode mode) {
    MtxF* cmf;
    f32 sin;
    f32 cos;
    f32 tempX;
    f32 tempZ;
    f32 zero = 0.0;
    f32 one = 1.0;

    if (mode == MTXMODE_APPLY) {
        if (y != 0.0f) {
            cmf = sCurrentMatrix;

            sin = sinf(y);
            cos = cosf(y);

            tempX = cmf->xx;
            tempZ = cmf->xz;
            cmf->xx = tempX * cos - tempZ * sin;
            cmf->xz = tempX * sin + tempZ * cos;

            tempX = cmf->yx;
            tempZ = cmf->yz;
            cmf->yx = tempX * cos - tempZ * sin;
            cmf->yz = tempX * sin + tempZ * cos;

            tempX = cmf->zx;
            tempZ = cmf->zz;
            cmf->zx = tempX * cos - tempZ * sin;
            cmf->zz = tempX * sin + tempZ * cos;

            tempX = cmf->wx;
            tempZ = cmf->wz;
            cmf->wx = tempX * cos - tempZ * sin;
            cmf->wz = tempX * sin + tempZ * cos;
        }
    } else {
        cmf = sCurrentMatrix;

        if (y != 0.0f) {
            sin = sinf(y);
            cos = cosf(y);
        } else {
            cos = one;
            sin = zero;
        }

        cmf->yx = zero;
        cmf->wx = zero;
        cmf->xy = zero;
        cmf->zy = zero;
        cmf->wy = zero;
        cmf->yz = zero;
        cmf->wz = zero;
        cmf->xw = zero;
        cmf->yw = zero;
        cmf->zw = zero;
        cmf->yy = one;
        cmf->ww = one;
        cmf->xx = cos;
        cmf->zz = cos;
        cmf->zx = -sin;
        cmf->xz = sin;
    }
}
void Matrix_RotateZS(s16 z, MatrixMode mode) {
    MtxF* cmf;
    f32 sin;
    f32 cos;
    f32 tempX;
    f32 tempY;
    f32 zero = 0.0;
    f32 one = 1.0;

    if (mode == MTXMODE_APPLY) {
        if (z != 0) {
            cmf = sCurrentMatrix;

            sin = Math_SinS(z);
            cos = Math_CosS(z);

            tempX = cmf->xx;
            tempY = cmf->xy;
            cmf->xx = tempX * cos + tempY * sin;
            cmf->xy = tempY * cos - tempX * sin;

            tempX = cmf->yx;
            tempY = cmf->yy;
            cmf->yx = tempX * cos + tempY * sin;
            cmf->yy = tempY * cos - tempX * sin;

            tempX = cmf->zx;
            tempY = cmf->zy;
            cmf->zx = tempX * cos + tempY * sin;
            cmf->zy = tempY * cos - tempX * sin;

            tempX = cmf->wx;
            tempY = cmf->wy;
            cmf->wx = tempX * cos + tempY * sin;
            cmf->wy = tempY * cos - tempX * sin;
        }
    } else {
        cmf = sCurrentMatrix;

        if (z != 0) {
            sin = Math_SinS(z);
            cos = Math_CosS(z);
        } else {
            sin = zero;
            cos = one;
        }

        cmf->zx = zero;
        cmf->wx = zero;
        cmf->zy = zero;
        cmf->wy = zero;
        cmf->xz = zero;
        cmf->yz = zero;
        cmf->wz = zero;
        cmf->xw = zero;
        cmf->yw = zero;
        cmf->zw = zero;
        cmf->zz = one;
        cmf->ww = one;
        cmf->xx = cos;
        cmf->yy = cos;
        cmf->yx = sin;
        cmf->xy = -sin;
    }
}

void Matrix_RotateZF(f32 z, MatrixMode mode) {
    MtxF* cmf;
    f32 sin;
    f32 cos;
    f32 tempX;
    f32 tempY;

    if (mode == MTXMODE_APPLY) {
        if (z != 0) {
            cmf = sCurrentMatrix;

            sin = sinf(z);
            cos = cosf(z);

            tempX = cmf->xx;
            tempY = cmf->xy;
            cmf->xx = tempX * cos + tempY * sin;
            cmf->xy = tempY * cos - tempX * sin;

            tempX = cmf->yx;
            tempY = cmf->yy;
            cmf->yx = tempX * cos + tempY * sin;
            cmf->yy = tempY * cos - tempX * sin;

            tempX = cmf->zx;
            tempY = cmf->zy;
            cmf->zx = tempX * cos + tempY * sin;
            cmf->zy = tempY * cos - tempX * sin;

            tempX = cmf->wx;
            tempY = cmf->wy;
            cmf->wx = tempX * cos + tempY * sin;
            cmf->wy = tempY * cos - tempX * sin;
        }
    } else {
        cmf = sCurrentMatrix;

        if (z != 0) {
            sin = sinf(z);
            cos = cosf(z);
        } else {
            sin = 0.0f;
            cos = 1.0f;
        }

        cmf->zx = 0.0f;
        cmf->wx = 0.0f;
        cmf->zy = 0.0f;
        cmf->wy = 0.0f;
        cmf->xz = 0.0f;
        cmf->yz = 0.0f;
        cmf->wz = 0.0f;
        cmf->xw = 0.0f;
        cmf->yw = 0.0f;
        cmf->zw = 0.0f;
        cmf->zz = 1.0f;
        cmf->ww = 1.0f;
        cmf->xx = cos;
        cmf->yy = cos;
        cmf->yx = sin;
        cmf->xy = -sin;
    }
}

void Matrix_RotateXFNew(f32 x) {
    MtxF* cmf = sCurrentMatrix;
    s32 pad[2];
    f32 sin;
    f32 cos;

    cmf->xx = 1.0f;
    cmf->yx = 0.0f;
    cmf->zx = 0.0f;
    cmf->wx = 0.0f;
    cmf->xy = 0.0f;
    cmf->wy = 0.0f;
    cmf->xz = 0.0f;
    cmf->wz = 0.0f;
    cmf->xw = 0.0f;
    cmf->yw = 0.0f;
    cmf->zw = 0.0f;
    cmf->ww = 1.0f;

    if (x != 0.0f) {
        sin = sinf(x);
        cos = cosf(x);

        cmf->yy = cos;
        cmf->zz = cos;
        cmf->yz = -sin;
        cmf->zy = sin;
    } else {
        cmf->yy = 1.0f;
        cmf->zy = 0.0f;
        cmf->yz = 0.0f;
        cmf->zz = 1.0f;
    }
}

void Matrix_TranslateRotateZYX(Vec3f* translation, Vec3s* rot) {
    MtxF* cmf = sCurrentMatrix;
    f32 sin = Math_SinS(rot->z);
    f32 cos = Math_CosS(rot->z);
    f32 temp1;
    f32 temp2;

    // No check for z != 0, presumably since translation is interleaved.
    temp1 = cmf->xx;
    temp2 = cmf->xy;
    cmf->xw += temp1 * translation->x + temp2 * translation->y + cmf->xz * translation->z;
    cmf->xx = temp1 * cos + temp2 * sin;
    cmf->xy = temp2 * cos - temp1 * sin;

    temp1 = cmf->yx;
    temp2 = cmf->yy;
    cmf->yw += temp1 * translation->x + temp2 * translation->y + cmf->yz * translation->z;
    cmf->yx = temp1 * cos + temp2 * sin;
    cmf->yy = temp2 * cos - temp1 * sin;

    temp1 = cmf->zx;
    temp2 = cmf->zy;
    cmf->zw += temp1 * translation->x + temp2 * translation->y + cmf->zz * translation->z;
    cmf->zx = temp1 * cos + temp2 * sin;
    cmf->zy = temp2 * cos - temp1 * sin;

    temp1 = cmf->wx;
    temp2 = cmf->wy;
    cmf->ww += temp1 * translation->x + temp2 * translation->y + cmf->wz * translation->z;
    cmf->wx = temp1 * cos + temp2 * sin;
    cmf->wy = temp2 * cos - temp1 * sin;

    if (rot->y != 0) {
        sin = Math_SinS(rot->y);
        cos = Math_CosS(rot->y);

        temp1 = cmf->xx;
        temp2 = cmf->xz;
        cmf->xx = temp1 * cos - temp2 * sin;
        cmf->xz = temp1 * sin + temp2 * cos;

        temp1 = cmf->yx;
        temp2 = cmf->yz;
        cmf->yx = temp1 * cos - temp2 * sin;
        cmf->yz = temp1 * sin + temp2 * cos;

        temp1 = cmf->zx;
        temp2 = cmf->zz;
        cmf->zx = temp1 * cos - temp2 * sin;
        cmf->zz = temp1 * sin + temp2 * cos;

        temp1 = cmf->wx;
        temp2 = cmf->wz;
        cmf->wx = temp1 * cos - temp2 * sin;
        cmf->wz = temp1 * sin + temp2 * cos;
    }

    if (rot->x != 0) {
        sin = Math_SinS(rot->x);
        cos = Math_CosS(rot->x);

        temp1 = cmf->xy;
        temp2 = cmf->xz;
        cmf->xy = temp1 * cos + temp2 * sin;
        cmf->xz = temp2 * cos - temp1 * sin;

        temp1 = cmf->yy;
        temp2 = cmf->yz;
        cmf->yy = temp1 * cos + temp2 * sin;
        cmf->yz = temp2 * cos - temp1 * sin;

        temp1 = cmf->zy;
        temp2 = cmf->zz;
        cmf->zy = temp1 * cos + temp2 * sin;
        cmf->zz = temp2 * cos - temp1 * sin;

        temp1 = cmf->wy;
        temp2 = cmf->wz;
        cmf->wy = temp1 * cos + temp2 * sin;
        cmf->wz = temp2 * cos - temp1 * sin;
    }
}

/**
 * @brief Set current to a general translation and rotation using YXZ Tait-Bryan angles: T Ry Rx Rz -> current
 *
 * This means a (column) vector is first rotated around Y, then around X, then around Z, then translated, then gets
 * transformed by whatever the matrix was previously.
 *
 * @param x amount to translate in X direction.
 * @param y amount to translate in Y direction.
 * @param z amount to translate in Z direction.
 * @param rot vector of rotation angles.
 *
 * @remark original name appears to be "Matrix_softcv3_load"
 */
void Matrix_SetTranslateRotateYXZ(f32 x, f32 y, f32 z, Vec3s* rot) {
    MtxF* cmf = sCurrentMatrix;
    f32 sinY = Math_SinS(rot->y);
    f32 cosY = Math_CosS(rot->y);
    f32 cosTemp;
    f32 sinTemp;

    cmf->xx = cosY;
    cmf->zx = -sinY;
    cmf->xw = x;
    cmf->yw = y;
    cmf->zw = z;
    cmf->wx = 0.0f;
    cmf->wy = 0.0f;
    cmf->wz = 0.0f;
    cmf->ww = 1.0f;

    if (rot->x != 0) {
        sinTemp = Math_SinS(rot->x);
        cosTemp = Math_CosS(rot->x);

        cmf->zz = cosY * cosTemp;
        cmf->zy = cosY * sinTemp;
        cmf->xz = sinY * cosTemp;
        cmf->xy = sinY * sinTemp;
        cmf->yz = -sinTemp;
        cmf->yy = cosTemp;
    } else {
        cmf->zz = cosY;
        cmf->xz = sinY;
        cmf->yz = 0.0f;
        cmf->zy = 0.0f;
        cmf->xy = 0.0f;
        cmf->yy = 1.0f;
    }

    if (rot->z != 0) {
        sinTemp = Math_SinS(rot->z);
        cosTemp = Math_CosS(rot->z);

        sinY = cmf->xx;
        cosY = cmf->xy;
        cmf->xx = sinY * cosTemp + cosY * sinTemp;
        cmf->xy = cosY * cosTemp - sinY * sinTemp;

        sinY = cmf->zx;
        cosY = cmf->zy;
        cmf->zx = sinY * cosTemp + cosY * sinTemp;
        cmf->zy = cosY * cosTemp - sinY * sinTemp;

        cosY = cmf->yy;
        cmf->yx = cosY * sinTemp;
        cmf->yy = cosY * cosTemp;
    } else {
        cmf->yx = 0.0f;
    }
}




#define COLORFILTER_COLORFLAG_NONE 0xC000
#define COLORFILTER_COLORFLAG_GRAY 0x8000
#define COLORFILTER_COLORFLAG_RED  0x4000
#define COLORFILTER_COLORFLAG_BLUE 0x0000

#define COLORFILTER_BUFFLAG_XLU    0x2000
#define COLORFILTER_BUFFLAG_OPA    0x0000

#define COLORFILTER_INTENSITY_FLAG 0x8000

#define ABS_ALT(x) ((x) < 0 ? -(x) : (x))

#define BINANG_ADD(a, b) ((s16)(a + b))
#define BINANG_SUB(a, b) ((s16)(a - b))

#define SUBS_SHADOW_TEX_WIDTH 64
#define SUBS_SHADOW_TEX_HEIGHT 64
#define SUBS_SHADOW_TEX_SIZE ((s32)sizeof(u8[SUBS_SHADOW_TEX_HEIGHT][SUBS_SHADOW_TEX_WIDTH]))
typedef void* TexturePtr;

typedef enum {
    /*  0 */ ACTOR_DRAW_DMGEFF_FIRE,
    /*  1 */ ACTOR_DRAW_DMGEFF_BLUE_FIRE,
    /* 10 */ ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX = 10,
    /* 11 */ ACTOR_DRAW_DMGEFF_FROZEN_SFX,
    /* 20 */ ACTOR_DRAW_DMGEFF_LIGHT_ORBS = 20,
    /* 21 */ ACTOR_DRAW_DMGEFF_BLUE_LIGHT_ORBS,
    /* 30 */ ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_SMALL = 30,
    /* 31 */ ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_MEDIUM,
    /* 32 */ ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_LARGE
} ActorDrawDamageEffectType;



u64 gEffFireMask1Tex[] = {
#include "eff_fire_mask_1.i4.inc.h"
};

u64 gEffFireMask2Tex[] = {
#include "eff_fire_mask_2.i4.inc.h"
};

u64 gEffIceFragmentTex[] = {
#include "eff_ice_fragment.i8.inc.h"
};

u64 gElectricSpark1Tex[] = {
#include "electric_spark_1.i4.inc.h"
};

u64 gElectricSpark2Tex[] = {
#include "electric_spark_2.i4.inc.h"
};

u64 gElectricSpark3Tex[] = {
#include "electric_spark_3.i4.inc.h"
};

u64 gElectricSpark4Tex[] = {
#include "electric_spark_4.i4.inc.h"
};

u64 gFlashTex[] = {
#include "flash.i8.inc.h"
};

Vtx gameplay_keepVtx_0502C0[] = {
#include "gameplay_keepVtx_0502C0.vtx.inc"
};

Vtx gameplay_keepVtx_050740[] = {
#include "gameplay_keepVtx_050740.vtx.inc"
};

u64 gEffIceSmokeTex[] = {
#include "eff_ice_smoke.i4.inc.h"
};

Vtx gameplay_keepVtx_051080[] = {
#include "gameplay_keepVtx_051080.vtx.inc"
};

Vtx gameplay_keepVtx_023440[] = {
#include "gameplay_keepVtx_023440.vtx.inc"
};

Vtx gameplay_keepVtx_023308[] = {
#include "gameplay_keepVtx_023308.vtx.inc"
};

Gfx gEffIceFragment1DL[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0x0FA0, 0x0FA0, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(gEffIceFragmentTex, G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP,
                         G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD, 15),
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_I, G_IM_SIZ_8b, 4, 0x0000, 1, 0, G_TX_NOMIRROR | G_TX_WRAP, 5, G_TX_NOLOD, G_TX_NOMIRROR |
                G_TX_WRAP, 5, 1),
    gsDPSetTileSize(1, 0, 0, 0x007C, 0x007C),
    gsDPSetCombineLERP(TEXEL1, PRIMITIVE, PRIM_LOD_FRAC, TEXEL0, TEXEL1, TEXEL0, PRIM_LOD_FRAC, TEXEL0, PRIMITIVE,
                       ENVIRONMENT, COMBINED, ENVIRONMENT, COMBINED, 0, ENVIRONMENT, 0),
    gsDPSetRenderMode(G_RM_PASS, G_RM_AA_ZB_XLU_SURF2),
    gsSPClearGeometryMode(G_CULL_BACK | G_FOG),
    gsSPSetGeometryMode(G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsDPSetPrimColor(0, 0x80, 170, 255, 255, 255),
    gsSPDisplayList(0x08000000),
    gsSPVertex(gameplay_keepVtx_0502C0, 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSPVertex(&gameplay_keepVtx_0502C0[3], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(6, 7, 8, 0, 9, 10, 11, 0),
    gsSP2Triangles(12, 13, 14, 0, 15, 16, 17, 0),
    gsSP2Triangles(18, 19, 20, 0, 21, 22, 23, 0),
    gsSP2Triangles(24, 25, 26, 0, 27, 28, 29, 0),
    gsSPVertex(&gameplay_keepVtx_0502C0[33], 8, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 0, 4, 0),
    gsSP1Triangle(5, 6, 7, 0),
    gsSPEndDisplayList(),
};

Gfx gEffIceFragment2MaterialDL[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0x0FA0, 0x0FA0, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(gEffIceFragmentTex, G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP,
                         G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD, 15),
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_I, G_IM_SIZ_8b, 4, 0x0000, 1, 0, G_TX_NOMIRROR | G_TX_WRAP, 5, G_TX_NOLOD, G_TX_NOMIRROR |
                G_TX_WRAP, 5, 1),
    gsDPSetTileSize(1, 0, 0, 0x007C, 0x007C),
    gsDPSetCombineLERP(TEXEL1, PRIMITIVE, PRIM_LOD_FRAC, TEXEL0, TEXEL1, TEXEL0, PRIM_LOD_FRAC, TEXEL0, PRIMITIVE,
                       ENVIRONMENT, COMBINED, ENVIRONMENT, COMBINED, 0, ENVIRONMENT, 0),
    gsDPSetRenderMode(G_RM_PASS, G_RM_AA_ZB_XLU_SURF2),
    gsSPClearGeometryMode(G_CULL_BACK | G_FOG),
    gsSPSetGeometryMode(G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPDisplayList(0x08000000),
    gsSPEndDisplayList(),
};

Gfx gEffIceFragment2ModelDL[] = {
    gsSPVertex(gameplay_keepVtx_0502C0, 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSPVertex(&gameplay_keepVtx_0502C0[3], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(6, 7, 8, 0, 9, 10, 11, 0),
    gsSP2Triangles(12, 13, 14, 0, 15, 16, 17, 0),
    gsSP2Triangles(18, 19, 20, 0, 21, 22, 23, 0),
    gsSP2Triangles(24, 25, 26, 0, 27, 28, 29, 0),
    gsSPVertex(&gameplay_keepVtx_0502C0[33], 8, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 0, 4, 0),
    gsSP1Triangle(5, 6, 7, 0),
    gsSPEndDisplayList(),
};

Gfx gEffIceFragment3DL[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0x0FA0, 0x0FA0, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(gEffIceFragmentTex, G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP,
                         G_TX_NOMIRROR | G_TX_WRAP, 5, 5, 1, G_TX_NOLOD),
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_I, G_IM_SIZ_8b, 4, 0x0000, 1, 0, G_TX_NOMIRROR | G_TX_WRAP, 5, 1, G_TX_NOMIRROR | G_TX_WRAP, 5,
                2),
    gsDPSetTileSize(1, 0, 0, 0x007C, 0x007C),
    gsDPSetCombineLERP(TEXEL1, PRIMITIVE, PRIM_LOD_FRAC, TEXEL0, TEXEL1, TEXEL0, PRIM_LOD_FRAC, TEXEL0, PRIMITIVE,
                       ENVIRONMENT, COMBINED, ENVIRONMENT, COMBINED, 0, ENVIRONMENT, 0),
    gsDPSetRenderMode(G_RM_PASS, G_RM_AA_ZB_XLU_SURF2),
    gsSPClearGeometryMode(G_CULL_BACK | G_FOG),
    gsSPSetGeometryMode(G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsDPSetPrimColor(0x80, 0x80, 170, 255, 255, 255),
    gsSPDisplayList(0x08000000),
    gsSPVertex(gameplay_keepVtx_050740, 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSPVertex(&gameplay_keepVtx_050740[3], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(3, 5, 6, 0, 7, 8, 9, 0),
    gsSP2Triangles(10, 11, 12, 0, 13, 14, 15, 0),
    gsSP2Triangles(13, 15, 16, 0, 17, 18, 19, 0),
    gsSP2Triangles(17, 19, 20, 0, 21, 22, 23, 0),
    gsSP2Triangles(24, 25, 26, 0, 24, 2, 1, 0),
    gsSP2Triangles(27, 28, 29, 0, 27, 29, 30, 0),
    gsSPVertex(&gameplay_keepVtx_050740[34], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0),
    gsSP2Triangles(8, 9, 10, 0, 8, 10, 11, 0),
    gsSP2Triangles(12, 13, 14, 0, 12, 14, 15, 0),
    gsSP2Triangles(16, 17, 18, 0, 16, 18, 19, 0),
    gsSP2Triangles(20, 17, 21, 0, 22, 23, 24, 0),
    gsSP2Triangles(25, 24, 26, 0, 27, 28, 29, 0),
    gsSP1Triangle(27, 29, 30, 0),
    gsSPVertex(&gameplay_keepVtx_050740[65], 28, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 1, 4, 0),
    gsSP2Triangles(5, 6, 7, 0, 5, 8, 9, 0),
    gsSP2Triangles(10, 11, 12, 0, 13, 14, 15, 0),
    gsSP2Triangles(4, 16, 3, 0, 17, 18, 19, 0),
    gsSP2Triangles(9, 20, 5, 0, 21, 22, 23, 0),
    gsSP2Triangles(24, 25, 26, 0, 0, 2, 27, 0),
    gsSPEndDisplayList(),
};

Gfx gEffIceSmokeDL[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock_4b(gEffFireMask1Tex, G_IM_FMT_I, 32, 64, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR |
                            G_TX_WRAP, 5, 6, G_TX_NOLOD, 15),
    gsDPLoadMultiBlock_4b(gEffIceSmokeTex, 0x0100, 1, G_IM_FMT_I, 32, 32, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR |
                          G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineLERP(TEXEL0, 0, ENV_ALPHA, TEXEL1, TEXEL0, 1, ENVIRONMENT, TEXEL1, PRIMITIVE, ENVIRONMENT, COMBINED,
                       ENVIRONMENT, COMBINED, 0, PRIMITIVE, 0),
    gsDPSetRenderMode(G_RM_PASS, G_RM_ZB_CLD_SURF2),
    gsSPClearGeometryMode(G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPSetGeometryMode(G_CULL_BACK),
    gsDPSetEnvColor(255, 255, 255, 128),
    gsSPDisplayList(0x08000000),
    gsSPVertex(gameplay_keepVtx_051080, 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSPEndDisplayList(),
};

Gfx gFrozenSteamMaterialDL[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock_4b(gEffFireMask1Tex, G_IM_FMT_I, 32, 64, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR |
                            G_TX_WRAP, 5, 6, G_TX_NOLOD, 15),
    gsDPLoadMultiBlock_4b(gEffIceSmokeTex, 0x0100, 1, G_IM_FMT_I, 32, 32, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR |
                          G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineLERP(TEXEL0, 0, ENV_ALPHA, TEXEL1, TEXEL0, 1, ENVIRONMENT, TEXEL1, PRIMITIVE, ENVIRONMENT, COMBINED,
                       ENVIRONMENT, COMBINED, 0, PRIMITIVE, 0),
    gsDPSetRenderMode(G_RM_PASS, G_RM_ZB_CLD_SURF2),
    gsSPClearGeometryMode(G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPSetGeometryMode(G_CULL_BACK),
    gsDPSetEnvColor(255, 255, 255, 128),
    gsSPEndDisplayList(),
};

Gfx gFrozenSteamModelDL[] = {
    gsSPDisplayList(0x08000000),
    gsSPVertex(gameplay_keepVtx_051080, 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSPEndDisplayList(),
};

Gfx gElectricSparkMaterialDL[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock_4b(0x08000000, G_IM_FMT_I, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP,
                            G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineLERP(PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED, 0, 0, 0,
                       COMBINED),
    gsDPSetRenderMode(G_RM_PASS, G_RM_ZB_OVL_SURF2),
    gsSPClearGeometryMode(G_CULL_BACK | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPEndDisplayList(),
};

Gfx gElectricSparkModelDL[] = {
    gsSPVertex(gameplay_keepVtx_023440, 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 1, 3, 2, 0),
    gsSPEndDisplayList(),
};

Gfx gLightOrbMaterial1DL[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(gFlashTex, G_IM_FMT_I, G_IM_SIZ_8b, 64, 64, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR |
                         G_TX_CLAMP, 6, 6, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineLERP(PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED, 0, 0, 0,
                       COMBINED),
    gsDPSetRenderMode(G_RM_PASS, G_RM_ZB_CLD_SURF2),
    gsSPClearGeometryMode(G_CULL_BACK | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPEndDisplayList(),
};

Gfx gLightOrbMaterial2DL[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(gFlashTex, G_IM_FMT_I, G_IM_SIZ_8b, 64, 64, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR |
                         G_TX_CLAMP, 6, 6, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineLERP(PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED, 0, 0, 0,
                       COMBINED),
    gsDPSetRenderMode(G_RM_PASS, G_RM_AA_ZB_XLU_DECAL2),
    gsSPClearGeometryMode(G_CULL_BACK | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPEndDisplayList(),
};

Gfx gLightOrbModelDL[] = {
    gsSPVertex(gameplay_keepVtx_023308, 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSPEndDisplayList(),
};


MtxF* sCurrentMatrix; //!< original name: "Matrix_now"


Hilite* func_800BCC68(Vec3f* arg0, PlayState* play) {
    Vec3f lightDir;

    lightDir.x = play->envCtx.dirLight1.params.dir.x;
    lightDir.y = play->envCtx.dirLight1.params.dir.y;
    lightDir.z = play->envCtx.dirLight1.params.dir.z;

    return Hilite_DrawXlu(arg0, &play->view.eye, &lightDir, play->state.gfxCtx);
}


TexturePtr sElectricSparkTextures[] = {
    gElectricSpark1Tex,
    gElectricSpark2Tex,
    gElectricSpark3Tex,
    gElectricSpark4Tex,
};

/**
 * Draw common damage effects applied to each body part provided in bodyPartsPos
 */
void Actor_DrawDamageEffects(PlayState* play, Actor* actor, Vec3f bodyPartsPos[], s16 bodyPartsCount, f32 effectScale,
                             f32 frozenSteamScale, f32 effectAlpha, u8 type) {
    if (effectAlpha > 0.001f) {
        s32 twoTexScrollParam;
        s16 bodyPartIndex;
        MtxF* currentMatrix;
        f32 alpha;
        f32 frozenScale;
        f32 lightOrbsScale;
        f32 electricSparksScale;
        f32 steamScale;
        Vec3f* bodyPartsPosStart = bodyPartsPos;
        u32 gameplayFrames = play->gameplayFrames;
        f32 effectAlphaScaled;

        currentMatrix = Matrix_GetCurrent();

        // Apply sfx along with damage effect
        if ((actor != NULL) && (effectAlpha > 0.05f) && (play->gameOverCtx.state == GAMEOVER_INACTIVE)) {
            if (type == ACTOR_DRAW_DMGEFF_FIRE) {
                Audio_PlayActorSfx2(actor, NA_SE_EV_BURN_OUT - SFX_FLAG);
            } else if (type == ACTOR_DRAW_DMGEFF_BLUE_FIRE) {
                //Audio_PlayActorSfx2(actor, NA_SE_EN_EXTINCT_LOOP - SFX_FLAG);
                SoundEffect_PlayHeld(SOUND_MM_ENEMY_EXTINCT_LOOP, 0.5f, 1.0f, 1.0f, &actor->world.pos, 0.0f, 1000.0f, &actor);
            } else if (type == ACTOR_DRAW_DMGEFF_FROZEN_SFX) {
                Audio_PlayActorSfx2(actor, NA_SE_EV_ICE_FREEZE - SFX_FLAG);
            } else if ((type == ACTOR_DRAW_DMGEFF_LIGHT_ORBS) || (type == ACTOR_DRAW_DMGEFF_BLUE_LIGHT_ORBS)) {
                //Audio_PlayActorSfx2(actor, NA_SE_EN_COMMON_DEADLIGHT - SFX_FLAG);
                SoundEffect_PlayHeld(SOUND_MM_LIGHT_DAMAGE, 1.0f, 1.0f, 1.0f, &actor->world.pos, 0.0f, 1000.0f, &actor);
            }
        }

        OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

        Gfx_SetupDL_25Xlu(play->state.gfxCtx);

        switch (type) {
            case ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX:
            case ACTOR_DRAW_DMGEFF_FROZEN_SFX:
                frozenScale = ((KREG(19) * 0.01f) + 2.3f) * effectScale;
                steamScale = ((KREG(28) * 0.0001f) + 0.035f) * frozenSteamScale;
                func_800BCC68(bodyPartsPos, play);

                // Setup to draw ice over frozen actor

                gSPSegment(POLY_XLU_DISP++, 0x08,
                           Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, gameplayFrames & 0xFF, 32, 16, 1, 0,
                                            (gameplayFrames * 2) & 0xFF, 64, 32));
                gDPSetPrimColor(POLY_XLU_DISP++, 0, 0x80, 170, 255, 255, 255);
                gSPDisplayList(POLY_XLU_DISP++, gEffIceFragment2MaterialDL);

                effectAlphaScaled = effectAlpha * 255.0f;

                // Apply and draw ice over each body part of frozen actor
                for (bodyPartIndex = 0; bodyPartIndex < bodyPartsCount; bodyPartIndex++, bodyPartsPos++) {
                    alpha = bodyPartIndex & 3;
                    alpha = effectAlphaScaled - (30.0f * alpha);
                    if (effectAlphaScaled < (30.0f * (bodyPartIndex & 3))) {
                        alpha = 0.0f;
                    }
                    if (alpha > 255.0f) {
                        alpha = 255.0f;
                    }

                    gDPSetEnvColor(POLY_XLU_DISP++, KREG(20) + 200, KREG(21) + 200, KREG(22) + 255, (u8)alpha);

                    Matrix_Translate(bodyPartsPos->x, bodyPartsPos->y, bodyPartsPos->z, MTXMODE_NEW);
                    Matrix_Scale(frozenScale, frozenScale, frozenScale, MTXMODE_APPLY);

                    if (bodyPartIndex & 1) {
                        Matrix_RotateYF(M_PI, MTXMODE_APPLY);
                    }

                    if (bodyPartIndex & 2) {
                        Matrix_RotateZF(M_PI, MTXMODE_APPLY);
                    }

                    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__),
                              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

                    gSPDisplayList(POLY_XLU_DISP++, gEffIceFragment2ModelDL);
                }

                bodyPartsPos = bodyPartsPosStart; // reset bodyPartsPos

                // Setup to draw steam over frozen actor

                gDPSetColorDither(POLY_XLU_DISP++, G_CD_BAYER);
                gDPSetAlphaDither(POLY_XLU_DISP++, G_AD_PATTERN);
                gSPDisplayList(POLY_XLU_DISP++, gFrozenSteamMaterialDL);

                alpha = effectAlpha * 100.0f;
                if (alpha > 100.0f) {
                    alpha = 100.0f;
                }

                gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 195, 225, 235, (u8)alpha);

                // Apply and draw steam over each body part of frozen actor
                for (bodyPartIndex = 0; bodyPartIndex < bodyPartsCount; bodyPartIndex++, bodyPartsPos++) {
                    twoTexScrollParam = ((bodyPartIndex * 3) + gameplayFrames);
                    gSPSegment(POLY_XLU_DISP++, 0x08,
                               Gfx_TwoTexScroll(play->state.gfxCtx, 0, twoTexScrollParam * 3, twoTexScrollParam * -12,
                                                32, 64, 1, 0, 0, 32, 32));

                    Matrix_Translate(bodyPartsPos->x, bodyPartsPos->y, bodyPartsPos->z, MTXMODE_NEW);
                    Matrix_ReplaceRotation(&play->billboardMtxF);
                    Matrix_Scale(steamScale, steamScale, 1.0f, MTXMODE_APPLY);

                    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__),
                              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

                    gSPDisplayList(POLY_XLU_DISP++, gFrozenSteamModelDL);
                }
                break;

            case ACTOR_DRAW_DMGEFF_FIRE:
            case ACTOR_DRAW_DMGEFF_BLUE_FIRE:
                if (type == ACTOR_DRAW_DMGEFF_FIRE) {
                    gDPSetEnvColor(POLY_XLU_DISP++, 255, 10, 0, 0);
                } else {
                    gDPSetEnvColor(POLY_XLU_DISP++, 0, 255, 255, 0);
                    // Reuse type for blue primitive color
                    type = 255;
                }

                Matrix_Put(&play->billboardMtxF);
                Matrix_Scale((effectScale * 0.005f) * 1.35f, (effectScale * 0.005f), (effectScale * 0.005f) * 1.35f,
                             MTXMODE_APPLY);

                effectAlphaScaled = effectAlpha * 255.0f;

                // Apply and draw fire on every body part
                for (bodyPartIndex = 0; bodyPartIndex < bodyPartsCount; bodyPartIndex++, bodyPartsPos++) {
                    alpha = bodyPartIndex & 3;
                    alpha = effectAlphaScaled - 30.0f * alpha;
                    if (effectAlphaScaled < 30.0f * (bodyPartIndex & 3)) {
                        alpha = 0.0f;
                    }
                    if (alpha > 255.0f) {
                        alpha = 255.0f;
                    }

                    // Use type for blue primitive color
                    // = 0 for ACTOR_DRAW_DMGEFF_FIRE
                    // = 255 for ACTOR_DRAW_DMGEFF_BLUE_FIRE
                    gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, 255, 255, type, (u8)alpha);

                    gSPSegment(POLY_XLU_DISP++, 0x08,
                               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, 0, 32, 64, 1, 0,
                                                ((bodyPartIndex * 10 + gameplayFrames) * -20) & 0x1FF, 32, 128));

                    Matrix_RotateYF(M_PI, MTXMODE_APPLY);
                    currentMatrix->mf[3][0] = bodyPartsPos->x;
                    currentMatrix->mf[3][1] = bodyPartsPos->y;
                    currentMatrix->mf[3][2] = bodyPartsPos->z;

                    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__),
                              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

                    gSPDisplayList(POLY_XLU_DISP++, gEffFire1DL);
                }
                break;

            case ACTOR_DRAW_DMGEFF_LIGHT_ORBS:
            case ACTOR_DRAW_DMGEFF_BLUE_LIGHT_ORBS:

                // Setup to draw light orbs on actor

                lightOrbsScale = ((KREG(19) * 0.01f) + 4.0f) * effectScale;

                gSPDisplayList(POLY_XLU_DISP++, gLightOrbMaterial1DL);

                alpha = effectAlpha * 255.0f;
                if (alpha > 255.0f) {
                    alpha = 255.0f;
                }

                if (type == ACTOR_DRAW_DMGEFF_BLUE_LIGHT_ORBS) {
                    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, (u8)(sREG(16) + 255), (u8)(sREG(17) + 255),
                                    (u8)(sREG(18) + 255), (u8)alpha);

                    gDPSetEnvColor(POLY_XLU_DISP++, (u8)sREG(19), (u8)(sREG(20) + 255), (u8)(sREG(21) + 255), 128);
                } else {
                    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 200, (u8)alpha);

                    gDPSetEnvColor(POLY_XLU_DISP++, 255, 255, 100, 128);
                }

                Matrix_Put(&play->billboardMtxF);
                Matrix_Scale(lightOrbsScale, lightOrbsScale, 1.0f, MTXMODE_APPLY);

                // Apply and draw a light orb over each body part of frozen actor
                for (bodyPartIndex = 0; bodyPartIndex < bodyPartsCount; bodyPartIndex++, bodyPartsPos++) {
                    Matrix_RotateZF(Rand_CenteredFloat(2 * M_PI), MTXMODE_APPLY);
                    currentMatrix->mf[3][0] = bodyPartsPos->x;
                    currentMatrix->mf[3][1] = bodyPartsPos->y;
                    currentMatrix->mf[3][2] = bodyPartsPos->z;

                    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__),
                              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

                    gSPDisplayList(POLY_XLU_DISP++, gLightOrbModelDL);
                }
                break;

            case ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_SMALL:
            case ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_MEDIUM:
            case ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_LARGE:
                if (type == ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_SMALL) {
                    electricSparksScale = (KREG(19) * 0.01f + 1.0f) * effectScale;
                } else if (type == ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_MEDIUM) {
                    electricSparksScale = (KREG(19) * 0.01f + 1.5f) * effectScale;
                } else {
                    electricSparksScale = (KREG(19) * 0.01f + 2.0f) * effectScale;
                }

                gSPSegment(POLY_XLU_DISP++, 0x08,
                           Lib_SegmentedToVirtual(sElectricSparkTextures[play->gameplayFrames % 4]));

                gSPDisplayList(POLY_XLU_DISP++, gElectricSparkMaterialDL);

                gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, (u8)(sREG(16) + 255), (u8)(sREG(17) + 255), (u8)(sREG(18) + 150),
                                (u8)(sREG(19) + 255));

                gDPSetEnvColor(POLY_XLU_DISP++, (u8)(sREG(20) + 255), (u8)(sREG(21) + 255), (u8)sREG(22), (u8)sREG(23));

                Matrix_Put(&play->billboardMtxF);
                Matrix_Scale(electricSparksScale, electricSparksScale, electricSparksScale, MTXMODE_APPLY);

                // Every body part draws two electric sparks at random orientations
                for (bodyPartIndex = 0; bodyPartIndex < bodyPartsCount; bodyPartIndex++, bodyPartsPos++) {
                    // first electric spark
                    Matrix_RotateXFApply(Rand_ZeroFloat(2 * M_PI));
                    Matrix_RotateZF(Rand_ZeroFloat(2 * M_PI), MTXMODE_APPLY);
                    currentMatrix->mf[3][0] = Rand_CenteredFloat((f32)sREG(24) + 30.0f) + bodyPartsPos->x;
                    currentMatrix->mf[3][1] = Rand_CenteredFloat((f32)sREG(24) + 30.0f) + bodyPartsPos->y;
                    currentMatrix->mf[3][2] = Rand_CenteredFloat((f32)sREG(24) + 30.0f) + bodyPartsPos->z;

                    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__),
                              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

                    gSPDisplayList(POLY_XLU_DISP++, gElectricSparkModelDL);

                    // second electric spark
                    Matrix_RotateXFApply(Rand_ZeroFloat(2 * M_PI));
                    Matrix_RotateZF(Rand_ZeroFloat(2 * M_PI), MTXMODE_APPLY);
                    currentMatrix->mf[3][0] = Rand_CenteredFloat((f32)sREG(24) + 30.0f) + bodyPartsPos->x;
                    currentMatrix->mf[3][1] = Rand_CenteredFloat((f32)sREG(24) + 30.0f) + bodyPartsPos->y;
                    currentMatrix->mf[3][2] = Rand_CenteredFloat((f32)sREG(24) + 30.0f) + bodyPartsPos->z;

                    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__),
                              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

                    gSPDisplayList(POLY_XLU_DISP++, gElectricSparkModelDL);
                }

                break;
        }

        CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
    }
}


void Actor_SpawnIceEffects(PlayState* play, Actor* actor, Vec3f limbPos[], s32 limbPosCount, s32 effectsPerLimb,
                           f32 scale, f32 scaleRange) {
    static Color_RGBA8 primColor = { 170, 255, 255, 255 };
    static Color_RGBA8 envColor = { 200, 200, 255, 255 };
    static Vec3f accel = { 0.0f, -1.0f, 0.0f };
    s32 i;
    s32 pad;
    Vec3f velocity;
    s16 randomYaw;
    s16 yaw;
    s32 j;

    SfxSource_PlaySfxAtFixedWorldPos(play, &actor->world.pos, 30, NA_SE_EV_ICE_BROKEN);
    switch (Rand_S16Offset(0,2))
    {
        case 0:
            SoundEffect_PlayOneshot(SOUND_MM_GLASS_SHATTER, 1.0f, 1.5f, &actor->world.pos, 0.0f, 1000.0f, &actor);
        break;
        case 1:
        default:
            SoundEffect_PlayOneshot(SOUND_RAREWARE_GLASS_SHATTER, 1.0f, 1.5f, &actor->world.pos, 0.0f, 1000.0f, &actor);
        break;
    }

    for (i = 0; i < limbPosCount; i++) {
        yaw = Actor_WorldYawTowardPoint(actor, limbPos);

        for (j = 0; j < effectsPerLimb; j++) {
            randomYaw = ((s32)Rand_Next() >> 0x13) + yaw;

            velocity.z = Rand_ZeroFloat(5.0f);

            velocity.x = Math_SinS(randomYaw) * velocity.z;
            velocity.y = Rand_ZeroFloat(4.0f) + 8.0f;
            velocity.z *= Math_CosS(randomYaw);

            EffectSsEnIce_Spawn(play, limbPos, Rand_ZeroFloat(scaleRange) + scale, &velocity, &accel, &primColor,
                                &envColor, 30);
        }

        limbPos++;
    }
}

MtxF* sCurrentMatrix; //!< original name: "Matrix_now"



void Matrix_MultZero(Vec3f* dest) {
    MtxF* cmf = sCurrentMatrix;

    dest->x = cmf->xw;
    dest->y = cmf->yw;
    dest->z = cmf->zw;
}

void Matrix_MultVecZ(f32 z, Vec3f* dest) {
    MtxF* cmf = sCurrentMatrix;

    dest->x = cmf->xw + cmf->xz * z;
    dest->y = cmf->yw + cmf->yz * z;
    dest->z = cmf->zw + cmf->zz * z;
}

const u8 sShadowMaps[4][12][12] = {
    {
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },
        { 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0 },
        { 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0 },
        { 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0 },
        { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    },
    {
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },
        { 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0 },
        { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
        { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
        { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
        { 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0 },
        { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    },
    {
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },
        { 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0 },
        { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
        { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
        { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
        { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
        { 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0 },
        { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    },
    {
        { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },
        { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
        { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
        { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
        { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
        { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
        { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },
    },
};


void SubS_FillShadowTex(s32 startCol, s32 startRow, u8* tex, s32 size) {
    s32 i;
    s32 j;
    s32 start;

    for (i = 0; i < 12; i++) {
        start = ((startRow + i) * 64) + startCol - 390;
        for (j = 0; j < 12; j++) {
            if (sShadowMaps[size][i][j] != 0) {
                if ((start + j >= 0) && (start + j < SUBS_SHADOW_TEX_SIZE)) {
                    tex[start + j] = 255;
                }
            }
        }
    }
}


void SubS_GenShadowTex(Vec3f bodyPartsPos[], Vec3f* worldPos, u8* tex, f32 tween, u8 bodyPartsNum, u8 sizes[],
                       s8 parentBodyParts[]) {
    Vec3f pos;
    Vec3f startVec;
    s32 i;
    s32 parentBodyPart;
    Vec3f* bodyPartPos;
    s32 startCol;
    s32 startRow;

    for (i = 0; i < bodyPartsNum; i++) {
        if (parentBodyParts[i] >= 0) {
            parentBodyPart = parentBodyParts[i];
            bodyPartPos = &bodyPartsPos[i];

            pos.x = (bodyPartsPos[parentBodyPart].x - bodyPartPos->x) * tween + (bodyPartPos->x - worldPos->x);
            pos.y = (bodyPartsPos[parentBodyPart].y - bodyPartPos->y) * tween + (bodyPartPos->y - worldPos->y);
            pos.z = (bodyPartsPos[parentBodyPart].z - bodyPartPos->z) * tween + (bodyPartPos->z - worldPos->z);
        } else {
            bodyPartPos = &bodyPartsPos[i];

            pos.x = bodyPartPos->x - worldPos->x;
            pos.y = bodyPartPos->y - worldPos->y;
            pos.z = bodyPartPos->z - worldPos->z;
        }

        Matrix_MultVec3f(&pos, &startVec);
        startCol = 64.0f + startVec.x;
        startRow = 64.0f - startVec.z;
        SubS_FillShadowTex(startCol >> 1, startRow >> 1, tex, sizes[i]);
    }
}


void SubS_DrawShadowTex(Actor* actor, GameState* gameState, u8* tex) {
    s32 pad;
    GraphicsContext* gfxCtx = gameState->gfxCtx;

    OPEN_DISPS(gfxCtx, __FILE__, __LINE__);

    Gfx_SetupDL_25Opa(gfxCtx);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 0, 0, 0, 100);
    gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, 0);
    Matrix_Translate(actor->world.pos.x, 0.0f, actor->world.pos.z, MTXMODE_NEW);
    Matrix_Scale(0.6f, 1.0f, 0.6f, MTXMODE_APPLY);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(gfxCtx, __FILE__, __LINE__), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    //gSPDisplayList(POLY_OPA_DISP++, gShadowMaterialDL);
    gDPLoadTextureBlock(POLY_OPA_DISP++, tex, G_IM_FMT_I, G_IM_SIZ_8b, SUBS_SHADOW_TEX_WIDTH, SUBS_SHADOW_TEX_HEIGHT, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, 6, 6, G_TX_NOLOD, G_TX_NOLOD);
    //gSPDisplayList(POLY_OPA_DISP++, gShadowModelDL);

    CLOSE_DISPS(gfxCtx, __FILE__, __LINE__);
}

