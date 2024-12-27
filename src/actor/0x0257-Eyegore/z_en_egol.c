/*
 * File: z_en_egol.c
 * Overlay: ovl_En_Egol
 * Description: Eyegore
 */

#include "z_en_egol.h"
//#include "objects/object_eg/object_eg.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h"
#include "overlays/actors/ovl_En_Arrow/z_en_arrow.h"
//#include "overlays/actors/ovl_En_Clear_Tag/z_en_clear_tag.h"
//#include "overlays/actors/ovl_En_Estone/z_en_estone.h"
#include "overlays/effects/ovl_Effect_Ss_Hitmark/z_eff_ss_hitmark.h"
#include <uLib.h>

#define ACTOR_FLAG_31      (1 << 31)

#define FLAGS (ACTOR_FLAG_0 | ACTOR_FLAG_2 | ACTOR_FLAG_4 | ACTOR_FLAG_5 | ACTOR_FLAG_31)
#define ABS_ALT(x) ((x) < 0 ? -(x) : (x))
#define RAD_TO_BINANG_ALT2(radians) TRUNCF_BINANG(((radians) * 0x8000) / M_PI)
#define TRUNCF_BINANG(f) (s16)(s32)(f)


#define COLORFILTER_COLORFLAG_NONE 0xC000
#define COLORFILTER_COLORFLAG_GRAY 0x8000
#define COLORFILTER_COLORFLAG_RED  0x4000
#define COLORFILTER_COLORFLAG_BLUE 0x0000

#define COLORFILTER_BUFFLAG_XLU    0x2000
#define COLORFILTER_BUFFLAG_OPA    0x0000

#define PATH_INDEX_NONE -1


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






void Actor_RequestQuake(PlayState* play, s16 y, s16 duration) {
    s16 quakeIndex = Quake_Add(Play_GetCamera(play, CAM_ID_MAIN), 3);

    Quake_SetSpeed(quakeIndex, 20000);
    Quake_SetQuakeValues(quakeIndex, y, 0, 0, 0);
    Quake_SetCountdown(quakeIndex, duration);
}

void Actor_RequestQuakeWithSpeed(PlayState* play, s16 y, s16 duration, s16 speed) {
    s16 quakeIndex = Quake_Add(Play_GetCamera(play, CAM_ID_MAIN), 3);

    Quake_SetSpeed(quakeIndex, speed);
    Quake_SetQuakeValues(quakeIndex, y, 0, 0, 0);
    Quake_SetCountdown(quakeIndex, duration);
}

void Actor_RequestQuakeAndRumble(Actor* actor, PlayState* play, s16 quakeY, s16 quakeDuration) {
    if (quakeY >= 5) {
        Rumble_Request(actor->xyzDistToPlayerSq, 255, 20, 150);
    } else {
        Rumble_Request(actor->xyzDistToPlayerSq, 180, 20, 100);
    }
    Actor_RequestQuake(play, quakeY, quakeDuration);
}





void* Lib_SegmentedToVirtual(void* ptr) {
    return SEGMENTED_TO_VIRTUAL(ptr);
}

void* Lib_SegmentedToVirtualNull(void* ptr) {
    if (((uintptr_t)ptr >> 28) == 0) {
        return ptr;
    } else {
        return SEGMENTED_TO_VIRTUAL(ptr);
    }
}


s32 SubS_CopyPointFromPath(Path* path, s32 pointIndex, Vec3f* dst) {
    Vec3s* point;

    if (path == NULL) {
        return false;
    }

    point = Lib_SegmentedToVirtual(path->points);
    point = &point[pointIndex];
    dst->x = point->x;
    dst->y = point->y;
    dst->z = point->z;

    return true;
}





s32 SubS_CopyPointFromPathCheckBounds(Path* path, s32 pointIndex, Vec3f* dst) {
    Vec3s* point;

    if ((path == NULL) || (pointIndex >= path->count) || (pointIndex < 0)) {
        return false;
    }

    point = Lib_SegmentedToVirtual(path->points);
    point = &point[pointIndex];
    dst->x = point->x;
    dst->y = point->y;
    dst->z = point->z;
    return true;
}

Path* SubS_GetPathByIndex(PlayState* play, s16 pathIndex, s16 pathIndexNone) {
    return (pathIndex != pathIndexNone) ? &play->setupPathList[pathIndex] : NULL;
}


Gfx* func_8012CB28(GraphicsContext* gfxCtx, u32 x, u32 y) {
    return Gfx_TexScroll(gfxCtx, x, y, 0, 0);
}


VecGeo* OLib_Vec3fToVecGeo(VecGeo* dest, Vec3f* vec) {
    VecSph_MM sph;

    OLib_Vec3fToVecSph(&sph, vec);
    sph.pitch = 0x4000 - sph.pitch;

    *dest = sph;

    return dest;
}

Vec3f* OLib_VecGeoToVec3f(Vec3f* dest, VecGeo* geo) {
    VecSph sph;

    sph.r = geo->r;
    sph.pitch = 0x4000 - geo->pitch;
    sph.yaw = geo->yaw;

    return OLib_VecSphToVec3f(dest, &sph);
}

Vec3f* OLib_AddVecGeoToVec3f(Vec3f* dest, Vec3f* a, VecGeo* geo) {
    Vec3f sum;
    Vec3f b;

    OLib_VecGeoToVec3f(&b, geo);
    sum.x = a->x + b.x;
    sum.y = a->y + b.y;
    sum.z = a->z + b.z;

    *dest = sum;

    return dest;
}


void Matrix_MultZero(Vec3f* dest) {
    MtxF* cmf = sCurrentMatrix;

    dest->x = cmf->xw;
    dest->y = cmf->yw;
    dest->z = cmf->zw;
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






void OLib_Vec3fAdd(PosRot* a, Vec3f* b, Vec3f* dest, s16 mode) {
    VecGeo geo;

    switch (mode) {
        case OLIB_ADD_OFFSET:
            OLib_Vec3fToVecGeo(&geo, b);
            geo.yaw += a->rot.y;
            OLib_AddVecGeoToVec3f(dest, &a->pos, &geo);
            break;

        case OLIB_ADD:
            dest->x = a->pos.x + b->x;
            dest->y = a->pos.y + b->y;
            dest->z = a->pos.z + b->z;
            break;

        default: // OLIB_ADD_COPY
            *dest = *b;
            break;
    }
}




/**
 * Returns a pointer to a single layer texture scroll displaylist.
 */
Gfx* AnimatedMat_TexScroll(PlayState* play, AnimatedMatTexScrollParams* params) {
    return Gfx_TexScroll(play->state.gfxCtx, params->xStep * sMatAnimStep, -(params->yStep * sMatAnimStep),
                         params->width, params->height);
}

/**
 * Animated Material Type 0:
 * Scrolls a single layer texture using the provided `AnimatedMatTexScrollParams`.
 */
void AnimatedMat_DrawTexScroll(PlayState* play, s32 segment, void* params) {
    AnimatedMatTexScrollParams* texScrollParams = (AnimatedMatTexScrollParams*)params;
    Gfx* texScrollDList = AnimatedMat_TexScroll(play, texScrollParams);

    OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

    if (sMatAnimFlags & 1) {
        gSPSegment(POLY_OPA_DISP++, segment, texScrollDList);
    }

    if (sMatAnimFlags & 2) {
        gSPSegment(POLY_XLU_DISP++, segment, texScrollDList);
    }

    CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
}

/**
 * Returns a pointer to a two layer texture scroll displaylist.
 */
Gfx* AnimatedMat_TwoLayerTexScroll(PlayState* play, AnimatedMatTexScrollParams* params) {
    return Gfx_TwoTexScroll(play->state.gfxCtx, 0, params[0].xStep * sMatAnimStep, -(params[0].yStep * sMatAnimStep),
                            params[0].width, params[0].height, 1, params[1].xStep * sMatAnimStep,
                            -(params[1].yStep * sMatAnimStep), params[1].width, params[1].height);
}

/**
 * Animated Material Type 1:
 * Scrolls a two layer texture using the provided `AnimatedMatTexScrollParams`.
 */
void AnimatedMat_DrawTwoTexScroll(PlayState* play, s32 segment, void* params) {
    AnimatedMatTexScrollParams* texScrollParams = (AnimatedMatTexScrollParams*)params;
    Gfx* texScrollDList = AnimatedMat_TwoLayerTexScroll(play, texScrollParams);

    OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

    if (sMatAnimFlags & 1) {
        gSPSegment(POLY_OPA_DISP++, segment, texScrollDList);
    }

    if (sMatAnimFlags & 2) {
        gSPSegment(POLY_XLU_DISP++, segment, texScrollDList);
    }

    CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
}

/**
 * Generates a displaylist that sets the prim and env color, and stores it in the provided segment ID.
 */
void AnimatedMat_SetColor(PlayState* play, s32 segment, F3DPrimColor* primColorResult, F3DEnvColor* envColor) {
    Gfx* gfx = Graph_Alloc(play->state.gfxCtx, 3 * sizeof(Gfx));

    OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

    // clang-format off
    if (sMatAnimFlags & 1) { gSPSegment(POLY_OPA_DISP++, segment, gfx); }
    if (sMatAnimFlags & 2) { gSPSegment(POLY_XLU_DISP++, segment, gfx); }
    // clang-format on

    gDPSetPrimColor(gfx++, 0, primColorResult->lodFrac, primColorResult->r, primColorResult->g, primColorResult->b,
                    (u8)(primColorResult->a * sMatAnimAlphaRatio));

    if (envColor != NULL) {
        gDPSetEnvColor(gfx++, envColor->r, envColor->g, envColor->b, envColor->a);
    }

    gSPEndDisplayList(gfx++);

    CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
}

/**
 * Animated Material Type 2:
 * Color key frame animation without linear interpolation.
 */
void AnimatedMat_DrawColor(PlayState* play, s32 segment, void* params) {
    AnimatedMatColorParams* colorAnimParams = (AnimatedMatColorParams*)params;
    F3DPrimColor* primColor = Lib_SegmentedToVirtual(colorAnimParams->primColors);
    F3DEnvColor* envColor;
    s32 curFrame = sMatAnimStep % colorAnimParams->keyFrameLength;

    primColor += curFrame;
    envColor = (colorAnimParams->envColors != NULL)
                   ? (F3DEnvColor*)Lib_SegmentedToVirtual(colorAnimParams->envColors) + curFrame
                   : NULL;

    AnimatedMat_SetColor(play, segment, primColor, envColor);
}

/**
 * Linear Interpolation
 */
s32 AnimatedMat_Lerp(s32 min, s32 max, f32 norm) {
    return (s32)((max - min) * norm) + min;
}

/**
 * Animated Material Type 3:
 * Color key frame animation with linear interpolation.
 */
void AnimatedMat_DrawColorLerp(PlayState* play, s32 segment, void* params) {
    AnimatedMatColorParams* colorAnimParams = (AnimatedMatColorParams*)params;
    F3DPrimColor* primColorMax = Lib_SegmentedToVirtual(colorAnimParams->primColors);
    F3DEnvColor* envColorMax;
    u16* keyFrames = Lib_SegmentedToVirtual(colorAnimParams->keyFrames);
    s32 curFrame = sMatAnimStep % colorAnimParams->keyFrameLength;
    s32 endFrame;
    s32 relativeFrame; // relative to the start frame
    s32 startFrame;
    f32 norm;
    F3DPrimColor* primColorMin;
    F3DPrimColor primColorResult;
    F3DEnvColor* envColorMin;
    F3DEnvColor envColorResult;
    s32 i;

    keyFrames++;
    i = 1;

    while (colorAnimParams->keyFrameCount > i) {
        if (curFrame < *keyFrames) {
            break;
        }
        i++;
        keyFrames++;
    }

    startFrame = keyFrames[-1];
    endFrame = keyFrames[0] - startFrame;
    relativeFrame = curFrame - startFrame;
    norm = (f32)relativeFrame / (f32)endFrame;

    primColorMax += i;
    primColorMin = primColorMax - 1;
    primColorResult.r = AnimatedMat_Lerp(primColorMin->r, primColorMax->r, norm);
    primColorResult.g = AnimatedMat_Lerp(primColorMin->g, primColorMax->g, norm);
    primColorResult.b = AnimatedMat_Lerp(primColorMin->b, primColorMax->b, norm);
    primColorResult.a = AnimatedMat_Lerp(primColorMin->a, primColorMax->a, norm);
    primColorResult.lodFrac = AnimatedMat_Lerp(primColorMin->lodFrac, primColorMax->lodFrac, norm);

    if (colorAnimParams->envColors) {
        envColorMax = Lib_SegmentedToVirtual(colorAnimParams->envColors);
        envColorMax += i;
        envColorMin = envColorMax - 1;
        envColorResult.r = AnimatedMat_Lerp(envColorMin->r, envColorMax->r, norm);
        envColorResult.g = AnimatedMat_Lerp(envColorMin->g, envColorMax->g, norm);
        envColorResult.b = AnimatedMat_Lerp(envColorMin->b, envColorMax->b, norm);
        envColorResult.a = AnimatedMat_Lerp(envColorMin->a, envColorMax->a, norm);
    } else {
        envColorMax = NULL;
    }

    AnimatedMat_SetColor(play, segment, &primColorResult, (envColorMax != NULL) ? &envColorResult : NULL);
}

/**
 * Lagrange interpolation
 */
f32 Scene_LagrangeInterp(s32 n, f32 x[], f32 fx[], f32 xp) {
    f32 weights[50];
    f32 xVal;
    f32 m;
    f32 intp;
    f32* xPtr1;
    f32* fxPtr;
    f32* weightsPtr;
    f32* xPtr2;
    s32 i;
    s32 j;

    for (i = 0, xPtr1 = x, fxPtr = fx, weightsPtr = weights; i < n; i++) {
        for (xVal = *xPtr1, m = 1.0f, j = 0, xPtr2 = x; j < n; j++) {
            if (j != i) {
                m *= xVal - (*xPtr2);
            }
            xPtr2++;
        }

        xPtr1++;
        *weightsPtr = (*fxPtr) / m;
        fxPtr++;
        weightsPtr++;
    }

    for (intp = 0.0f, i = 0, weightsPtr = weights; i < n; i++) {
        for (m = 1.0f, j = 0, xPtr2 = x; j < n; j++) {
            if (j != i) {
                m *= xp - (*xPtr2);
            }
            xPtr2++;
        }

        intp += (*weightsPtr) * m;
        weightsPtr++;
    }

    return intp;
}

/**
 * Lagrange interpolation specifically for colors
 */
u8 Scene_LagrangeInterpColor(s32 n, f32 x[], f32 fx[], f32 xp) {
    s32 intp = Scene_LagrangeInterp(n, x, fx, xp);

    // Clamp between 0 and 255 to ensure the color value does not overflow in either direction
    return CLAMP(intp, 0, 255);
}

/**
 * Animated Material Type 4:
 * Color key frame animation with non-linear interpolation.
 */
void AnimatedMat_DrawColorNonLinearInterp(PlayState* play, s32 segment, void* params) {
    AnimatedMatColorParams* colorAnimParams = (AnimatedMatColorParams*)params;
    F3DPrimColor* primColorCur = Lib_SegmentedToVirtual(colorAnimParams->primColors);
    F3DEnvColor* envColorCur = Lib_SegmentedToVirtual(colorAnimParams->envColors);
    u16* keyFrames = Lib_SegmentedToVirtual(colorAnimParams->keyFrames);
    f32 curFrame = sMatAnimStep % colorAnimParams->keyFrameLength;
    F3DPrimColor primColorResult;
    F3DEnvColor envColorResult;
    f32 x[50];
    f32 fxPrimR[50];
    f32 fxPrimG[50];
    f32 fxPrimB[50];
    f32 fxPrimA[50];
    f32 fxPrimLodFrac[50];
    f32 fxEnvR[50];
    f32 fxEnvG[50];
    f32 fxEnvB[50];
    f32 fxEnvA[50];
    f32* xPtr = x;
    f32* fxPrimRPtr = fxPrimR;
    f32* fxPrimGPtr = fxPrimG;
    f32* fxPrimBPtr = fxPrimB;
    f32* fxPrimAPtr = fxPrimA;
    f32* fxPrimLodFracPtr = fxPrimLodFrac;
    f32* fxEnvRPtr = fxEnvR;
    f32* fxEnvGPtr = fxEnvG;
    f32* fxEnvBPtr = fxEnvB;
    f32* fxEnvAPtr = fxEnvA;
    s32 i;

    for (i = 0; i < colorAnimParams->keyFrameCount; i++) {
        *xPtr = *keyFrames;
        *fxPrimRPtr = primColorCur->r;
        *fxPrimGPtr = primColorCur->g;
        *fxPrimBPtr = primColorCur->b;
        *fxPrimAPtr = primColorCur->a;
        *fxPrimLodFracPtr = primColorCur->lodFrac;

        primColorCur++;
        fxPrimRPtr++;
        fxPrimGPtr++;
        fxPrimBPtr++;
        fxPrimAPtr++;
        fxPrimLodFracPtr++;

        if (envColorCur != NULL) {
            *fxEnvRPtr = envColorCur->r;
            *fxEnvGPtr = envColorCur->g;
            *fxEnvBPtr = envColorCur->b;
            *fxEnvAPtr = envColorCur->a;

            envColorCur++;
            fxEnvRPtr++;
            fxEnvGPtr++;
            fxEnvBPtr++;
            fxEnvAPtr++;
        }

        keyFrames++;
        xPtr++;
    }

    primColorResult.r = Scene_LagrangeInterpColor(colorAnimParams->keyFrameCount, x, fxPrimR, curFrame);
    primColorResult.g = Scene_LagrangeInterpColor(colorAnimParams->keyFrameCount, x, fxPrimG, curFrame);
    primColorResult.b = Scene_LagrangeInterpColor(colorAnimParams->keyFrameCount, x, fxPrimB, curFrame);
    primColorResult.a = Scene_LagrangeInterpColor(colorAnimParams->keyFrameCount, x, fxPrimA, curFrame);
    primColorResult.lodFrac = Scene_LagrangeInterpColor(colorAnimParams->keyFrameCount, x, fxPrimLodFrac, curFrame);

    if (colorAnimParams->envColors != NULL) {
        envColorCur = Lib_SegmentedToVirtual(colorAnimParams->envColors);
        envColorResult.r = Scene_LagrangeInterpColor(colorAnimParams->keyFrameCount, x, fxEnvR, curFrame);
        envColorResult.g = Scene_LagrangeInterpColor(colorAnimParams->keyFrameCount, x, fxEnvG, curFrame);
        envColorResult.b = Scene_LagrangeInterpColor(colorAnimParams->keyFrameCount, x, fxEnvB, curFrame);
        envColorResult.a = Scene_LagrangeInterpColor(colorAnimParams->keyFrameCount, x, fxEnvA, curFrame);
    } else {
        envColorCur = NULL;
    }

    AnimatedMat_SetColor(play, segment, &primColorResult, (envColorCur != NULL) ? &envColorResult : NULL);
}

/**
 * Animated Material Type 5:
 * Cycles between a list of textures (imagine like a GIF)
 */
void AnimatedMat_DrawTexCycle(PlayState* play, s32 segment, void* params) {
    AnimatedMatTexCycleParams* texAnimParams = params;
    TexturePtr* texList = Lib_SegmentedToVirtual(texAnimParams->textureList);
    u8* texId = Lib_SegmentedToVirtual(texAnimParams->textureIndexList);
    s32 curFrame = sMatAnimStep % texAnimParams->keyFrameLength;
    TexturePtr tex = Lib_SegmentedToVirtual(texList[texId[curFrame]]);

    OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

    if (sMatAnimFlags & 1) {
        gSPSegment(POLY_OPA_DISP++, segment, tex);
    }

    if (sMatAnimFlags & 2) {
        gSPSegment(POLY_XLU_DISP++, segment, tex);
    }

    CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
}

/**
 * This is the main function that handles the animated material system.
 * There are six different animated material types, which should be set in the provided `AnimatedMaterial`.
 */
void AnimatedMat_DrawMain(PlayState* play, AnimatedMaterial* matAnim, f32 alphaRatio, u32 step, u32 flags) {
    static void (*matAnimDrawHandlers[])(PlayState*, s32 segment, void* params) = {
        AnimatedMat_DrawTexScroll, AnimatedMat_DrawTwoTexScroll,         AnimatedMat_DrawColor,
        AnimatedMat_DrawColorLerp, AnimatedMat_DrawColorNonLinearInterp, AnimatedMat_DrawTexCycle,
    };
    s32 segmentAbs;
    s32 segment;

    sMatAnimAlphaRatio = alphaRatio;
    sMatAnimStep = step;
    sMatAnimFlags = flags;

    if ((matAnim != NULL) && (matAnim->segment != 0)) {
        do {
            segment = matAnim->segment;
            segmentAbs = ABS_ALT(segment) + 7;
            matAnimDrawHandlers[matAnim->type](play, segmentAbs, Lib_SegmentedToVirtual(matAnim->params));
            matAnim++;
        } while (segment >= 0);
    }
}

/**
 * Draws an animated material to both OPA and XLU buffers.
 */
void AnimatedMat_Draw(PlayState* play, AnimatedMaterial* matAnim) {
    AnimatedMat_DrawMain(play, matAnim, 1, play->gameplayFrames, 3);
}




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
                Audio_PlayActorSfx2(actor, NA_SE_EN_EXTINCT - SFX_FLAG);
                //SoundEffect_PlayHeld(SOUND_MM_ENEMY_EXTINCT_LOOP, 0.5f, 1.0f, 1.0f, &actor->world.pos, 0.0f, 1000.0f, &actor);
            } else if (type == ACTOR_DRAW_DMGEFF_FROZEN_SFX) {
                Audio_PlayActorSfx2(actor, NA_SE_EV_ICE_FREEZE - SFX_FLAG);
            } else if ((type == ACTOR_DRAW_DMGEFF_LIGHT_ORBS) || (type == ACTOR_DRAW_DMGEFF_BLUE_LIGHT_ORBS)) {
                //Audio_PlayActorSfx2(actor, NA_SE_EN_COMMON_DEADLIGHT - SFX_FLAG);
                //SoundEffect_PlayHeld(SOUND_MM_LIGHT_DAMAGE, 1.0f, 1.0f, 1.0f, &actor->world.pos, 0.0f, 1000.0f, &actor);
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
            //SoundEffect_PlayOneshot(SOUND_RAREWARE_GLASS_SHATTER, 1.0f, 1.5f, &actor->world.pos, 0.0f, 1000.0f, &actor);
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









































#define THIS ((EnEgol*)thisx)

typedef enum {
    /*  0 */ EYEGORE_ACTION_WAIT,
    /*  1 */ EYEGORE_ACTION_STAND,
    /*  2 */ EYEGORE_ACTION_WALK,
    /*  3 */ EYEGORE_ACTION_RETREAT,
    /*  4 */ EYEGORE_ACTION_LASER,
    /*  5 */ EYEGORE_ACTION_SLAM,
    /*  6 */ EYEGORE_ACTION_PUNCH,
    /*  7 */ EYEGORE_ACTION_SLAM_WAIT,
    /*  8 */ EYEGORE_ACTION_STUNNED,
    /*  9 */ EYEGORE_ACTION_STUN_END,
    /* 10 */ EYEGORE_ACTION_SLAM_END,
    /* 11 */ EYEGORE_ACTION_DAMAGED,
    /* 12 */ EYEGORE_ACTION_STOP,
    /* 13 */ EYEGORE_ACTION_DYING,
    /* 14 */ EYEGORE_ACTION_DEAD
} EnEgolAction;

typedef enum {
    /* 0 */ EYEGORE_EFFECT_IMPACT,
    /* 1 */ EYEGORE_EFFECT_PIECE_LARGE,
    /* 2 */ EYEGORE_EFFECT_PIECE_SMALL,
    /* 3 */ EYEGORE_EFFECT_DEBRIS
} EnEgolEffectType;

typedef enum {
    /* 0 */ EYEGORE_LASER_OFF,
    /* 1 */ EYEGORE_LASER_START,
    /* 2 */ EYEGORE_LASER_CHARGING,
    /* 3 */ EYEGORE_LASER_FIRE,
    /* 7 */ EYEGORE_LASER_ON = 7
} EnEgolLaserState;

void EnEgol_Init(Actor* thisx, PlayState* play);
void EnEgol_Destroy(Actor* thisx, PlayState* play);
void EnEgol_Update(Actor* thisx, PlayState* play);
void EnEgol_Draw(Actor* thisx, PlayState* play2);

void EnEgol_SetupWait(EnEgol* this);
void EnEgol_Wait(EnEgol* this, PlayState* play);
void EnEgol_SetupStand(EnEgol* this);
void EnEgol_Stand(EnEgol* this, PlayState* play);
void EnEgol_SetupWalk(EnEgol* this);
void EnEgol_Walk(EnEgol* this, PlayState* play);
void EnEgol_SetupRetreat(EnEgol* this);
void EnEgol_Retreat(EnEgol* this, PlayState* play);
void EnEgol_SetupLaser(EnEgol* this);
void EnEgol_Laser(EnEgol* this, PlayState* play);
void EnEgol_SetupStop(EnEgol* this);
void EnEgol_Stop(EnEgol* this, PlayState* play);
void EnEgol_SetupSlam(EnEgol* this);
void EnEgol_Slam(EnEgol* this, PlayState* play);
void EnEgol_SetupPunch(EnEgol* this);
void EnEgol_Punch(EnEgol* this, PlayState* play);
void EnEgol_SetupSlamWait(EnEgol* this);
void EnEgol_SlamWait(EnEgol* this, PlayState* play);
void EnEgol_SetupStunned(EnEgol* this);
void EnEgol_Stunned(EnEgol* this, PlayState* play);
void EnEgol_SetupStunEnd(EnEgol* this);
void EnEgol_StunEnd(EnEgol* this, PlayState* play);
void EnEgol_SetupSlamEnd(EnEgol* this);
void EnEgol_SlamEnd(EnEgol* this, PlayState* play);
void EnEgol_SetupDamaged(EnEgol* this);
void EnEgol_Damaged(EnEgol* this, PlayState* play);
void EnEgol_StartDeathCutscene(EnEgol* this, PlayState* play);
void EnEgol_Death(EnEgol* this, PlayState* play);

void EnEgol_SpawnEffect(EnEgol* this, Vec3f* pos, Vec3s* rot, s16 lifetime, f32 scale, s16 type);
void EnEgol_UpdateEffects(EnEgol* this, PlayState* play);
void EnEgol_DrawEffects(EnEgol* this, PlayState* play);

static ColliderJntSphElementInit sEyeJntSphElementsInit[1] = {
    {
        {
            ELEMTYPE_UNK2,
            { 0xFFCFFFFF, 0x00, 0x00 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_NONE | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_NONE,
        },
        { EYEGORE_LIMB_EYE_COLLIDER, { { 0, 0, 0 }, 0 }, 1 },
    },
};

static ColliderJntSphElementInit sBodySphElementsInit[6] = {
    {
        {
            ELEMTYPE_UNK2,
            { 0x20000000, 0x04, 0x10 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { EYEGORE_LIMB_LEFT_HAND, { { 0, 0, 0 }, 0 }, 1 },
    },
    {
        {
            ELEMTYPE_UNK2,
            { 0x20000000, 0x04, 0x10 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { EYEGORE_LIMB_RIGHT_HAND, { { 0, 0, 0 }, 0 }, 1 },
    },
    {
        {
            ELEMTYPE_UNK2,
            { 0x20000000, 0x00, 0x00 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_NONE | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { EYEGORE_LIMB_HEAD, { { 0, 0, 0 }, 0 }, 1 },
    },
    {
        {
            ELEMTYPE_UNK2,
            { 0x20000000, 0x00, 0x00 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_NONE | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { EYEGORE_LIMB_LEFT_SHOULDER, { { 0, 0, 0 }, 0 }, 1 },
    },
    {
        {
            ELEMTYPE_UNK2,
            { 0x20000000, 0x00, 0x00 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_NONE | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { EYEGORE_LIMB_RIGHT_SHOULDER, { { 0, 0, 0 }, 0 }, 1 },
    },
    {
        {
            ELEMTYPE_UNK2,
            { 0x20000000, 0x00, 0x00 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_NONE | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { EYEGORE_LIMB_FAULDS, { { 0, 0, 0 }, 0 }, 1 },
    },
};

static ColliderJntSphInit sBodyJntSphInit = {
    {
        COLTYPE_METAL,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    ARRAY_COUNT(sBodySphElementsInit),
    sBodySphElementsInit,
};

static ColliderJntSphInit sEyeJntSphInit = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    ARRAY_COUNT(sEyeJntSphElementsInit),
    sEyeJntSphElementsInit,
};

static ColliderQuadInit sLaserQuadInit = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_NONE,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_QUAD,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x03, 0x10 },
        { 0x00000000, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL | TOUCH_UNK7,
        BUMP_NONE,
        OCELEM_NONE,
    },
    { { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } } },
};

typedef enum {
    /* 0x0 */ EYEGORE_DMGEFF_IMMUNE_0,
    /* 0x4 */ EYEGORE_DMGEFF_LIGHT_ARROW = 4,
    /* 0xE */ EYEGORE_DMGEFF_NONE = 0xE,
    /* 0xF */ EYEGORE_DMGEFF_IMMUNE_F
} EnEgolDamageEffect;

static DamageTable sDamageTable = {
    /* Deku Nut       */ DMG_ENTRY(0, EYEGORE_DMGEFF_IMMUNE_F),
    /* Deku Stick     */ DMG_ENTRY(0, EYEGORE_DMGEFF_IMMUNE_F),
    /* Slingshot      */ DMG_ENTRY(1, EYEGORE_DMGEFF_NONE),
    /* Explosives     */ DMG_ENTRY(2, EYEGORE_DMGEFF_NONE),
    /* Boomerang      */ DMG_ENTRY(1, EYEGORE_DMGEFF_NONE),
    /* Normal arrow   */ DMG_ENTRY(1, EYEGORE_DMGEFF_NONE),
    /* Hammer swing   */ DMG_ENTRY(2, EYEGORE_DMGEFF_NONE),
    /* Hookshot       */ DMG_ENTRY(1, EYEGORE_DMGEFF_NONE),
    /* Kokiri Sword   */ DMG_ENTRY(0, EYEGORE_DMGEFF_IMMUNE_0),
    /* Master Sword   */ DMG_ENTRY(1, EYEGORE_DMGEFF_NONE),
    /* Giant's Knife  */ DMG_ENTRY(2, EYEGORE_DMGEFF_NONE),
    /* Fire arrow     */ DMG_ENTRY(2, EYEGORE_DMGEFF_NONE),
    /* Ice arrow      */ DMG_ENTRY(2, EYEGORE_DMGEFF_LIGHT_ARROW),
    /* Light arrow    */ DMG_ENTRY(2, EYEGORE_DMGEFF_LIGHT_ARROW),
    /* Unk arrow 1    */ DMG_ENTRY(2, EYEGORE_DMGEFF_NONE),
    /* Unk arrow 2    */ DMG_ENTRY(2, EYEGORE_DMGEFF_NONE),
    /* Unk arrow 3    */ DMG_ENTRY(2, EYEGORE_DMGEFF_NONE),
    /* Fire magic     */ DMG_ENTRY(2, EYEGORE_DMGEFF_NONE),
    /* Ice magic      */ DMG_ENTRY(2, EYEGORE_DMGEFF_NONE),
    /* Light magic    */ DMG_ENTRY(0, EYEGORE_DMGEFF_LIGHT_ARROW),
    /* Normal shield  */ DMG_ENTRY(0, EYEGORE_DMGEFF_IMMUNE_0),
    /* Light ray      */ DMG_ENTRY(0, EYEGORE_DMGEFF_IMMUNE_F),
    /* Kokiri spin    */ DMG_ENTRY(0, EYEGORE_DMGEFF_IMMUNE_F),
    /* Giant spin     */ DMG_ENTRY(0, EYEGORE_DMGEFF_IMMUNE_F),
    /* Master spin    */ DMG_ENTRY(0, EYEGORE_DMGEFF_IMMUNE_F),
    /* Kokiri jump    */ DMG_ENTRY(0, EYEGORE_DMGEFF_IMMUNE_0),
    /* Giant jump     */ DMG_ENTRY(4, EYEGORE_DMGEFF_NONE),
    /* Master jump    */ DMG_ENTRY(2, EYEGORE_DMGEFF_NONE),
    /* Unknown 1      */ DMG_ENTRY(0, EYEGORE_DMGEFF_IMMUNE_0),
    /* Unblockable    */ DMG_ENTRY(0, EYEGORE_DMGEFF_IMMUNE_0),
    /* Hammer Jump    */ DMG_ENTRY(4, EYEGORE_DMGEFF_NONE),
    /* Unknown 2      */ DMG_ENTRY(0, EYEGORE_DMGEFF_IMMUNE_F),
};

static Color_RGB8 sEyePrimColors[] = {
    { 175, 255, 255 }, // light cyan
    { 195, 245, 10 },  // lime green
    { 255, 55, 205 },  // magenta
};

static Color_RGB8 sEyeEnvColors[] = {
    { 255, 115, 155 }, // pink
    { 40, 155, 155 },  // dark cyan
    { 215, 255, 255 }, // light cyan
};

static Color_RGB8 sLightOrbColors[] = {
    { 255, 255, 255 }, // white
    { 255, 255, 255 }, // white
};

ActorInit En_Egol_InitVars = {
    0x0257,
    ACTORCAT_ENEMY,
    FLAGS,
    0x022D,
    sizeof(EnEgol),
    (ActorFunc)EnEgol_Init,
    (ActorFunc)EnEgol_Destroy,
    (ActorFunc)EnEgol_Update,
    (ActorFunc)EnEgol_Draw,
};

typedef enum {
    /*  0 */ EYEGORE_ANIM_STAND,
    /*  1 */ EYEGORE_ANIM_WALK,
    /*  2 */ EYEGORE_ANIM_SLAM,
    /*  3 */ EYEGORE_ANIM_SLAM_WAIT,
    /*  4 */ EYEGORE_ANIM_SLAM_END,
    /*  5 */ EYEGORE_ANIM_DAMAGED,
    /*  6 */ EYEGORE_ANIM_DEATH,
    /*  7 */ EYEGORE_ANIM_LASER,
    /*  8 */ EYEGORE_ANIM_LASER_END, // unused
    /*  9 */ EYEGORE_ANIM_STUNNED,
    /* 10 */ EYEGORE_ANIM_STUN_END,
    /* 11 */ EYEGORE_ANIM_RETREAT,
    /* 12 */ EYEGORE_ANIM_SIT,
    /* 13 */ EYEGORE_ANIM_LEFT_PUNCH,
    /* 14 */ EYEGORE_ANIM_RIGHT_PUNCH,
    /* 15 */ EYEGORE_ANIM_MAX
} EnEgolAnimation;

static AnimationHeader* sAnimations[EYEGORE_ANIM_MAX] = {
    0x0600ee4c,          // EYEGORE_ANIM_STAND
    0x060100c0,           // EYEGORE_ANIM_WALK
    0x06002460,           // EYEGORE_ANIM_SLAM
    0x06009d2c,       // EYEGORE_ANIM_SLAM_WAIT
    0x0600a204,        // EYEGORE_ANIM_SLAM_END
    0x0600adf8,        // EYEGORE_ANIM_DAMAGED
    0x0600bc60,          // EYEGORE_ANIM_DEATH
    0x0600a484,          // EYEGORE_ANIM_LASER
    0x0600a73c, // EYEGORE_ANIM_LASER_END
    0x0600cc4c,        // EYEGORE_ANIM_STUNNED
    0x0600ce54,        // EYEGORE_ANIM_STUN_END
    0x0600beb8,        // EYEGORE_ANIM_RETREAT
    0x0600d3d8,            // EYEGORE_ANIM_SIT
    0x0600c434,      // EYEGORE_ANIM_LEFT_PUNCH
    0x0600c9b4,     // EYEGORE_ANIM_RIGHT_PUNCH
};

static u8 sAnimationModes[EYEGORE_ANIM_MAX] = {
    ANIMMODE_ONCE, // EYEGORE_ANIM_STAND
    ANIMMODE_LOOP, // EYEGORE_ANIM_WALK
    ANIMMODE_ONCE, // EYEGORE_ANIM_SLAM
    ANIMMODE_ONCE, // EYEGORE_ANIM_SLAM_WAIT
    ANIMMODE_ONCE, // EYEGORE_ANIM_SLAM_END
    ANIMMODE_ONCE, // EYEGORE_ANIM_DAMAGED
    ANIMMODE_ONCE, // EYEGORE_ANIM_DEATH
    ANIMMODE_ONCE, // EYEGORE_ANIM_LASER
    ANIMMODE_ONCE, // EYEGORE_ANIM_LASER_END
    ANIMMODE_ONCE, // EYEGORE_ANIM_STUNNED
    ANIMMODE_ONCE, // EYEGORE_ANIM_STUN_END
    ANIMMODE_LOOP, // EYEGORE_ANIM_RETREAT
    ANIMMODE_ONCE, // EYEGORE_ANIM_SIT
    ANIMMODE_ONCE, // EYEGORE_ANIM_LEFT_PUNCH
    ANIMMODE_ONCE, // EYEGORE_ANIM_RIGHT_PUNCH
};

void EnEgol_ChangeAnim(EnEgol* this, s32 animIndex) {
    this->animIndex = animIndex;
    this->animEndFrame = Animation_GetLastFrame(sAnimations[this->animIndex]);
    Animation_Change(&this->skelAnime, sAnimations[this->animIndex], 1.0f, 0.0f, this->animEndFrame,
                     sAnimationModes[this->animIndex], 0.0f);
}

void EnEgol_FootstepEffects(EnEgol* this, PlayState* play, f32 leftFootFrame, f32 rightFootFrame) {
    if (Animation_OnFrame(&this->skelAnime, leftFootFrame) || Animation_OnFrame(&this->skelAnime, rightFootFrame)) {
        Vec3f spawnPos;
        Player* player = GET_PLAYER(play);
        s32 quakeYOffset;
        s32 pad;

        //Audio_PlayActorSfx2(&this->actor, NA_SE_EN_EYEGOLE_WALK);
        SoundEffect_PlayOneshot(SOUND_ENEMY_EYEGORE_STEP, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 1000.0f, &this->actor);
        quakeYOffset = 4 - (s32)(fabsf(player->actor.world.pos.y - this->actor.world.pos.y) * 0.02f);
        if (quakeYOffset > 4) {
            quakeYOffset = 4;
        } else if (quakeYOffset < 1) {
            quakeYOffset = 1;
        }

        if (player->stateFlags3) {
            Actor_RequestQuakeAndRumble(&this->actor, play, quakeYOffset, 2);
        }
        if (Animation_OnFrame(&this->skelAnime, leftFootFrame)) {
            Math_Vec3f_Copy(&spawnPos, &this->leftFootPos);
            spawnPos.y = this->actor.floorHeight;
            Actor_SpawnFloorDustRing(play, &this->actor, &spawnPos, 0.0f, 10, 6.0f, 50, 30, true);
        } else {
            Math_Vec3f_Copy(&spawnPos, &this->rightFootPos);
            spawnPos.y = this->actor.floorHeight;
            Actor_SpawnFloorDustRing(play, &this->actor, &spawnPos, 0.0f, 10, 6.0f, 50, 30, true);
        }
    }
}

void EnEgol_DestroyBlocks(EnEgol* this, PlayState* play, Vec3f pos1, Vec3f pos2) {
    Actor* prop = play->actorCtx.actorLists[ACTORCAT_PROP].head;

    while (prop != NULL) {
        if (prop->id != 0x0258) {
            prop = prop->next;
            continue;
        }
        if (prop->colChkInfo.health > 0) {
            Vec3f blockTo1;
            Vec3f blockTo2;
            f32 dist1;
            f32 dist2;

            blockTo1.x = pos1.x - prop->world.pos.x;
            blockTo1.y = pos1.y - prop->world.pos.y;
            blockTo1.z = pos1.z - prop->world.pos.z;

            blockTo2.x = pos2.x - prop->world.pos.x;
            blockTo2.y = pos2.y - prop->world.pos.y;
            blockTo2.z = pos2.z - prop->world.pos.z;

            dist1 = sqrtf(SQXZ(blockTo1));
            dist2 = sqrtf(SQXZ(blockTo2));

            if ((dist1 < 40.0f) || (dist2 < 40.0f)) {
                dist1 = sqrtf(SQ(blockTo1.y));
                dist2 = sqrtf(SQ(blockTo2.y));
                if ((dist1 < 40.0f) || (dist2 < 40.0f)) {
                    prop->colChkInfo.health = 0;
                    Audio_PlayActorSfx2(&this->actor, NA_SE_EV_WALL_BROKEN);
                    break;
                }
            }
        }
        prop = prop->next;
    }
}

void EnEgol_GetWaypoint(EnEgol* this) {
    if ((this->pathIndex != PATH_INDEX_NONE) && (this->path != NULL) &&
        !SubS_CopyPointFromPath(this->path, this->waypoint, &this->waypointPos)) {
        Actor_Kill(&this->actor);
    }
}

#define EYEGORE_SET_SPH_DIM(element, centerX, centerY, centerZ, radiusV, scaleV) \
    element.dim.modelSphere.radius = radiusV;                                    \
    element.dim.scale = scaleV;                                                  \
    element.dim.modelSphere.center.x = centerX;                                  \
    element.dim.modelSphere.center.y = centerY;                                  \
    element.dim.modelSphere.center.z = centerZ

void EnEgol_Init(Actor* thisx, PlayState* play) {
    EnEgol* this = THIS;

    this->actor.gravity = -2.0f;
    Actor_SetScale(&this->actor, 0.015f);
    this->actor.colChkInfo.damageTable = &sDamageTable;
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 60.0f);
    this->actor.targetMode = 5;
    this->actor.colChkInfo.mass = MASS_IMMOVABLE;
    this->actor.naviEnemyId = NAVI_ENEMY_GOHMA;
    this->actor.colChkInfo.health = 8;

    SkelAnime_InitFlex(play, &this->skelAnime, 0x06009664, 0x0600ee4c, this->jointTable, this->morphTable,
                       EYEGORE_LIMB_MAX);

    Collider_InitJntSph(play, &this->bodyCollider);
    Collider_InitJntSph(play, &this->eyeCollider);
    Collider_InitQuad(play, &this->laserCollider);
    Collider_SetJntSph(play, &this->bodyCollider, &this->actor, &sBodyJntSphInit, this->bodyElements);
    Collider_SetJntSph(play, &this->eyeCollider, &this->actor, &sEyeJntSphInit, this->eyeElements);
    Collider_SetQuad(play, &this->laserCollider, &this->actor, &sLaserQuadInit);

    this->pathIndex = EYEGORE_GET_PATH_INDEX(&this->actor);
    // if (this->pathIndex == EYEGORE_PATH_INDEX_NONE) {
    //     this->pathIndex = PATH_INDEX_NONE;
    //     Actor_Kill(&this->actor);
    //     return;
    // }

    this->wakeupRange = this->actor.world.rot.x * 20.0f;
    this->actor.world.rot.x = 0;
    if (this->wakeupRange < 0.0f) {
        this->wakeupRange = 200.0f;
    }

    this->minLaserRange = this->actor.world.rot.z * 20.0f;
    this->actor.world.rot.z = 0;
    if (this->minLaserRange < 0.0f) {
        this->minLaserRange = 200.0f;
    }

    this->path = SubS_GetPathByIndex(play, this->pathIndex, EYEGORE_PATH_INDEX_NONE);

    EYEGORE_SET_SPH_DIM(this->eyeCollider.elements[0], 500, 0, 0, 26, 1.0f);

    EYEGORE_SET_SPH_DIM(this->bodyCollider.elements[0], 1800, 100, 0, 20, 1.0f);
    EYEGORE_SET_SPH_DIM(this->bodyCollider.elements[1], 1800, 100, 0, 20, 1.0f);
    EYEGORE_SET_SPH_DIM(this->bodyCollider.elements[2], 1000, -500, 0, 35, 1.0f);
    EYEGORE_SET_SPH_DIM(this->bodyCollider.elements[3], 200, 200, 0, 25, 1.0f);
    EYEGORE_SET_SPH_DIM(this->bodyCollider.elements[4], 300, 200, 0, 25, 1.0f);
    EYEGORE_SET_SPH_DIM(this->bodyCollider.elements[5], 2100, -300, 0, 37, 1.0f);

    this->switchFlag = EYEGORE_GET_SWITCH(&this->actor);
    if (this->switchFlag == 0x7F) {
        this->switchFlag = -1;
    }
    if ((this->switchFlag > -1) && Flags_GetSwitch(play, this->switchFlag)) {
        Actor_Kill(&this->actor);
        return;
    }
    EnEgol_SetupWait(this);
}

void EnEgol_Destroy(Actor* thisx, PlayState* play) {
    EnEgol* this = THIS;

    Collider_DestroyJntSph(play, &this->bodyCollider);
    Collider_DestroyJntSph(play, &this->eyeCollider);
    Collider_DestroyQuad(play, &this->laserCollider);
}

void EnEgol_SetupWait(EnEgol* this) {
    this->actor.flags |= ACTOR_FLAG_27;
    this->action = EYEGORE_ACTION_WAIT;
    this->actionFunc = EnEgol_Wait;
}

void EnEgol_Wait(EnEgol* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;
    s16 angleToFacing = this->actor.shape.rot.y - this->actor.yawTowardsPlayer;

    if ((curFrame >= this->animEndFrame) && (ABS_ALT(angleToFacing) < 0x3000) &&
        (this->actor.xzDistToPlayer < this->wakeupRange)) {
        func_800F5ACC(NA_BGM_MINI_BOSS);
        EnEgol_SetupStand(this);
    }
}

void EnEgol_SetupStand(EnEgol* this) {
    EnEgol_ChangeAnim(this, EYEGORE_ANIM_STAND);
    this->actor.flags &= ~ACTOR_FLAG_27;
    this->action = EYEGORE_ACTION_STAND;
    //Audio_PlayActorSfx2(&this->actor, NA_SE_EN_EYEGOLE_STAND);
    SoundEffect_PlayOneshot(SOUND_ENEMY_EYEGORE_STAND, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 1000.0f, &this->actor);
    this->actionFunc = EnEgol_Stand;
}

void EnEgol_Stand(EnEgol* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;

    if (curFrame >= this->animEndFrame) {
        EnEgol_SetupWalk(this);
    }
}

void EnEgol_SetupWalk(EnEgol* this) {
    EnEgol_ChangeAnim(this, EYEGORE_ANIM_WALK);
    this->laserCount = 0;
    //EnEgol_GetWaypoint(this);
    this->action = EYEGORE_ACTION_WALK;
    this->actionFunc = EnEgol_Walk;
}

void EnEgol_Walk(EnEgol* this, PlayState* play) {
    s16 angleToFacing = this->actor.world.rot.y - this->actor.yawTowardsPlayer;
    f32 curFrame = this->skelAnime.curFrame;
    Player* player = GET_PLAYER(play);
    Vec3f spawnPos;
    Vec3f gZeroVec3s = {0};
    Vec3f gZeroVec3f = {0};
    s16 temp_a1 = this->actor.wallYaw - this->actor.shape.rot.y;
    Math_SmoothStepToS(&this->headRot.z, 0, 1, 0x3E8, 0);
    Math_Vec3f_Copy(&spawnPos, &gZeroVec3f);
    if (curFrame >= this->animEndFrame) {
        this->laserCount++;
        if (this->laserCount > 0) {
            this->laserCount = 0;
            if (this->actor.xzDistToPlayer > this->minLaserRange) {
                EnEgol_SetupLaser(this);
                return;
            }
        }
    }
    if ((this->actor.world.pos.y - 50.0f) <= player->actor.world.pos.y) {
        if (Animation_OnFrame(&this->skelAnime, 24.0f)) {
            Math_Vec3f_Copy(&spawnPos, &this->rightFootPos);
            spawnPos.y = this->actor.floorHeight + 2.0f;
            EnEgol_SpawnEffect(this, &spawnPos, &gZeroVec3s, 100, 0.005f, EYEGORE_EFFECT_IMPACT);
        }
        if (Animation_OnFrame(&this->skelAnime, 9.0f)) {
            Math_Vec3f_Copy(&spawnPos, &this->leftFootPos);
            spawnPos.y = this->actor.floorHeight + 2.0f;
            EnEgol_SpawnEffect(this, &spawnPos, &gZeroVec3s, 100, 0.005f, EYEGORE_EFFECT_IMPACT);
        }
    }
    if ((ABS_ALT(angleToFacing) < 0x3000) && (this->actor.xzDistToPlayer < 100.0f)) {
        EnEgol_SetupSlam(this);
    } else {
        s16 angleBehind;

        if (!((this->skelAnime.curFrame == 0.0f) ||
              ((9.0f <= this->skelAnime.curFrame) && (this->skelAnime.curFrame <= 15.0f)) ||
              ((24.0f <= this->skelAnime.curFrame) && (this->skelAnime.curFrame <= 29.0f)))) {
            // Math_ApproachF(&this->actor.world.pos.x, this->waypointPos.x, 0.5f,
            //                fabsf(Math_SinS(this->actor.world.rot.y) * 4.0f));
            // Math_ApproachF(&this->actor.world.pos.z, this->waypointPos.z, 0.5f,
            //                fabsf(Math_CosS(this->actor.world.rot.y) * 4.0f));
            Math_ApproachF(&this->actor.world.pos.x, player->actor.world.pos.x, 0.5f,
               fabsf(Math_SinS(this->actor.world.rot.y) * 4.0f));
            Math_ApproachF(&this->actor.world.pos.z, player->actor.world.pos.z, 0.5f,
               fabsf(Math_CosS(this->actor.world.rot.y) * 4.0f));
        }
        if ((this->actor.bgCheckFlags & BGCHECKFLAG_WALL) && (ABS(temp_a1) >= 0x4000)) {
            temp_a1 = (this->actor.yawTowardsPlayer > 0) ? this->actor.wallYaw - 0x4000 : this->actor.wallYaw + 0x4000;
            Math_SmoothStepToS(&this->actor.world.rot.y, temp_a1, 1, 0x4B0, 0);
        }else{
            Math_SmoothStepToS(&this->actor.world.rot.y, Math_Vec3f_Yaw(&this->actor.world.pos, &player->actor.world.pos), 1,
                           0x7D0, 0);
        }
        EnEgol_FootstepEffects(this, play, 9.0f, 24.0f);
        angleBehind = this->actor.world.rot.y - this->actor.yawTowardsPlayer + 0x8000;
        if (Actor_WorldDistXYZToPoint(&player->actor, &this->actor.home.pos) > 1000.0f/*ABS_ALT(angleBehind) < 0x2000*/) {
            // this->waypoint--;
            // if (this->waypoint < 0) {
            //     this->waypoint = 0;
            // }
            EnEgol_SetupStop(this);
        } else {
            // f32 dx = this->actor.world.pos.x - this->waypointPos.x;
            // f32 dz = this->actor.world.pos.z - this->waypointPos.z;

            // if (sqrtf(SQ(dx) + SQ(dz)) < 4.0f) {
            //     if (this->path != NULL) {
            //         this->waypoint++;
            //         if (this->waypoint >= this->path->count) {
            //             this->waypoint -= 2;
            //             if (this->waypoint < 0) {
            //                 this->waypoint = 0;
            //             }
            //             EnEgol_SetupStop(this);
            //         }
            //         EnEgol_GetWaypoint(this);
            //     }
            // }
            if (this->actor.wallBgId != BGCHECK_SCENE) {
                EnEgol_DestroyBlocks(this, play, this->rightFootPos, this->leftFootPos);
            }
        }
    }
}

void EnEgol_SetupRetreat(EnEgol* this) {
    EnEgol_ChangeAnim(this, EYEGORE_ANIM_RETREAT);
    this->laserCount = 0;
    //EnEgol_GetWaypoint(this);
    this->actor.world.rot.y = Math_Vec3f_Yaw(&this->actor.world.pos, &this->actor.home.pos);
    this->actor.shape.rot.y = this->actor.world.rot.y + 0x8000;
    this->action = EYEGORE_ACTION_RETREAT;
    this->actionFunc = EnEgol_Retreat;
}

void EnEgol_Retreat(EnEgol* this, PlayState* play) {
    s16 angleToFacing = this->actor.world.rot.y - this->actor.yawTowardsPlayer;
    Vec3f toWaypoint;
    Vec3f spawnPos;
    Vec3f gZeroVec3s = {0};
    Vec3f gZeroVec3f = {0};
    Player* player = GET_PLAYER(play);
    s16 temp_a1 = this->actor.wallYaw - this->actor.shape.rot.y;

    if ((ABS_ALT(angleToFacing) < 0x3000) && (fabsf(this->actor.world.pos.y - player->actor.world.pos.y) < 50.0f) &&
        (this->actor.xzDistToPlayer < 100.0f) && (player->invincibilityTimer == 0)) {
        func_8002F6D4(play, &this->actor, 2.0f, (s32)Rand_CenteredFloat(0x2000) + this->actor.world.rot.y, 5.0f, 0x10);
    }
    Math_Vec3f_Copy(&spawnPos, &gZeroVec3f);
    if ((this->actor.world.pos.y - 50.0f) <= player->actor.world.pos.y) {
        if (Animation_OnFrame(&this->skelAnime, 5.0f)) {
            Math_Vec3f_Copy(&spawnPos, &this->rightFootPos);
            spawnPos.y = this->actor.floorHeight + 2.0f;
            EnEgol_SpawnEffect(this, &spawnPos, &gZeroVec3s, 100, 0.005f, EYEGORE_EFFECT_IMPACT);
        }
        if (Animation_OnFrame(&this->skelAnime, 0.0f)) {
            Math_Vec3f_Copy(&spawnPos, &this->leftFootPos);
            spawnPos.y = this->actor.floorHeight + 2.0f;
            EnEgol_SpawnEffect(this, &spawnPos, &gZeroVec3s, 100, 0.005f, EYEGORE_EFFECT_IMPACT);
        }
    }
    Math_ApproachF(&this->actor.world.pos.x, this->actor.home.pos.x, 0.5f,
                   fabsf(Math_SinS(this->actor.world.rot.y) * 10.0f));
    Math_ApproachF(&this->actor.world.pos.z, this->actor.home.pos.z, 0.5f,
                   fabsf(Math_CosS(this->actor.world.rot.y) * 10.0f));
    if ((this->actor.bgCheckFlags & BGCHECKFLAG_WALL) && (ABS(temp_a1) >= 0x4000)) {
        temp_a1 = Math_Vec3f_Yaw(&this->actor.world.pos, &this->actor.home.pos) ? this->actor.wallYaw - 0x4000 : this->actor.wallYaw + 0x4000;
        Math_SmoothStepToS(&this->actor.world.rot.y, temp_a1, 1, 0x4B0, 0);
    }else{
        Math_SmoothStepToS(&this->actor.world.rot.y, Math_Vec3f_Yaw(&this->actor.world.pos, &this->actor.home.pos), 1, 0x7D0,
                       0);
    }
    Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.world.rot.y + 0x8000, 1, 0x7D0, 0);

    EnEgol_FootstepEffects(this, play, 5.0f, 0.0f);
    toWaypoint.x = this->actor.world.pos.x - this->actor.home.pos.x;
    toWaypoint.z = this->actor.world.pos.z - this->actor.home.pos.z;
    if (sqrtf(SQXZ(toWaypoint)) < 4.0f) {
            this->waypoint = 0;
            EnEgol_SetupStop(this);
        //EnEgol_GetWaypoint(this);
    }
}

void EnEgol_SetupLaser(EnEgol* this) {
    EnEgol_ChangeAnim(this, EYEGORE_ANIM_LASER);
    this->actionTimer = 0;
    this->laserCount = 0;
    this->action = EYEGORE_ACTION_LASER;
    this->actionFunc = EnEgol_Laser;
}

void EnEgol_Laser(EnEgol* this, PlayState* play) {
    static s16 sLaserAngles[3] = { 0x1F40, 0xBB8, 0x7D0 };
    s32 pad1;
    Vec3f stonePos;
    s32 pad2;
    CollisionPoly* colPoly;
    Vec3f hitPos;
    s32 bgId;
    Vec3f gZeroVec3s = {0};
    Vec3f gZeroVec3f = {0};
    Player* player = GET_PLAYER(play);
    s16 xPitch;
    Vec3f playerPos = player->actor.world.pos;
    playerPos.y = playerPos.y + 40.0f;
    xPitch = Math_Vec3f_Pitch(&this->eyePos, &playerPos);
    if ((ABS_ALT((s16)(this->actor.world.rot.y - this->actor.yawTowardsPlayer)) < 0x3000) &&
        (this->actor.xzDistToPlayer < 100.0f)) {
        this->chargingLaser = false;
        this->chargeLevel = 0;
        this->actionTimer = 0;
        this->laserState = EYEGORE_LASER_OFF;
        Math_Vec3f_Copy(&this->laserScale, &gZeroVec3f);
        Math_Vec3f_Copy(&this->laserScaleTarget, &gZeroVec3f);
        Math_Vec3f_Copy(&this->laserCollider.dim.quad[1], &this->laserBase);
        Math_Vec3f_Copy(&this->laserCollider.dim.quad[3], &this->laserBase);
        Math_Vec3f_Copy(&this->laserCollider.dim.quad[0], &this->laserBase);
        Math_Vec3f_Copy(&this->laserCollider.dim.quad[2], &this->laserBase);
        EnEgol_SetupSlam(this);
    } else /*if (this->actor.xzDistToPlayer < this->minLaserRange) {
        this->chargingLaser = false;
        this->chargeLevel = 0;
        this->actionTimer = 0;
        this->laserState = EYEGORE_LASER_OFF;
        Math_Vec3f_Copy(&this->laserScale, &gZeroVec3f);
        Math_Vec3f_Copy(&this->laserScaleTarget, &gZeroVec3f);
        Math_Vec3f_Copy(&this->laserCollider.dim.quad[1], &this->laserBase);
        Math_Vec3f_Copy(&this->laserCollider.dim.quad[3], &this->laserBase);
        Math_Vec3f_Copy(&this->laserCollider.dim.quad[0], &this->laserBase);
        Math_Vec3f_Copy(&this->laserCollider.dim.quad[2], &this->laserBase);
        EnEgol_SetupWalk(this);
    } else */{
        if (this->chargingLaser) {
            switch (this->chargeLevel) {
                case 0:
                    this->waitTimer = 10;
                    this->chargeLevel++;
                    break;

                case 1:
                    Math_ApproachF(&this->chargeLightScale, 8.0f, 0.5f, 2.5f);
                    if (this->waitTimer == 0) {
                        this->waitTimer = 10;
                        this->chargeLevel++;
                    }
                    break;

                case 2:
                    Math_ApproachF(&this->chargeLightScale, 1.0f, 0.5f, 1.0f);
                    if (this->waitTimer == 0) {
                        this->chargeLevel = 0;
                        this->laserState = EYEGORE_LASER_FIRE;
                        this->chargingLaser = false;
                        this->chargeLightScale = 0.0f;
                    }
                    break;

                default:
                    break;
            }
            this->chargeLightRot += 0x7D0;
        }
        if ((this->laserState != EYEGORE_LASER_OFF) || (this->laserCount != 0)) {

            Math_SmoothStepToS(&this->headRot.z, -0x2710, 5, 0x1F4, 5);
        } else {
            Math_SmoothStepToS(&this->headRot.z, 0, 5, 0x1F4, 5);
        }
        if (this->laserState == EYEGORE_LASER_OFF) {
            if (this->laserCount >= 3) {
                EnEgol_SetupWalk(this);
            } else {
                this->laserRot.x = sLaserAngles[this->laserCount];
                this->laserScaleTarget.z = 0.0f;
                this->laserScale.z = 0.0f;
                if (this->laserCount == 0) {
                    this->laserState = EYEGORE_LASER_START;
                } else {
                    this->laserState = EYEGORE_LASER_FIRE;
                    this->laserScaleTarget.x = 0.03f;
                    this->laserScaleTarget.y = 0.03f;
                }
                this->laserCount++;
            }
        } else if (this->laserState >= EYEGORE_LASER_FIRE) {
            //Audio_PlayActorSfx2(&this->actor, NA_SE_EN_EYEGOLE_BEAM - SFX_FLAG);
                SoundEffect_PlayHeld(SOUND_ENEMY_EYEGORE_LASER, 1.0f, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 1000.0f, &this->actor);
            if (this->laserState != EYEGORE_LASER_OFF) {
                EnEgol_DestroyBlocks(this, play, this->laserCollider.dim.quad[0], this->laserCollider.dim.quad[1]);
            }
            //! @bug this should check walls, too
            if ((this->actionTimer == 0) && BgCheck_EntityLineTest1(&play->colCtx, &this->laserCollider.dim.quad[3],
                                                                    &this->laserCollider.dim.quad[1], &hitPos, &colPoly,
                                                                    true, true, false, true, &bgId)) {
                Vec3s rotToNorm;
                f32 nx;
                f32 ny;
                f32 nz;
                s32 pad3;
                s32 i;
                s32 quakeYOffset;
                Player* player = GET_PLAYER(play);

                nx = COLPOLY_GET_NORMAL(colPoly->normal.x);
                ny = COLPOLY_GET_NORMAL(colPoly->normal.y);
                nz = COLPOLY_GET_NORMAL(colPoly->normal.z);

                /*! @bug The following is supposed to calculate the rotation from vertical to the collision poly normal.
                 * However, the calculation is performed incorrectly. The correct calculation is
                 * rotToNorm.x = Math_FAtan2F(nz, ny) * 0x8000 / M_PI;
                 * rotToNorm.z = Math_FAtan2F(-nx, sqrtf(1.0f - SQ(nx))) * 0x8000 / M_PI;
                 */
                rotToNorm.x = Math_FAtan2F(nz, ny) * 0x8000 / M_PI;
                rotToNorm.z = Math_FAtan2F(-nx, sqrtf(1.0f - SQ(nx))) * 0x8000 / M_PI;

                if ((this->actor.world.pos.y - 50.0f) <= player->actor.world.pos.y) {
                    EnEgol_SpawnEffect(this, &hitPos, &rotToNorm, 100, 0.02f, EYEGORE_EFFECT_IMPACT);
                }
                quakeYOffset = 4 - (s32)(fabsf(player->actor.world.pos.y - this->actor.world.pos.y) * 0.02f);
                if (quakeYOffset > 4) {
                    quakeYOffset = 4;
                } else if (quakeYOffset < 1) {
                    quakeYOffset = 1;
                }
                if (player->stateFlags3) {
                    Actor_RequestQuakeAndRumble(&this->actor, play, quakeYOffset, 2);
                }
                Audio_PlayActorSfx2(&this->actor, NA_SE_EV_EXPLOSION);
                EffectSsDeadDs_SpawnStationary(play, &hitPos, 40, -2, 255, 20);
                func_80033480(play, &hitPos, 6.0f, 2, 120, 20, true);

                if ((this->actor.world.pos.y - 50.0f <= player->actor.world.pos.y) &&
                    (this->actor.floorBgId == BGCHECK_SCENE)) {
                    Math_Vec3f_Copy(&stonePos, &hitPos);
                    stonePos.x += Math_SinS(this->actor.world.rot.y) * 60.0f;
                    stonePos.z += Math_CosS(this->actor.world.rot.y) * 60.0f;
                    for (i = 0; i < 3; i++) {
                        Actor_Spawn(&play->actorCtx, play, 0x0258, stonePos.x, stonePos.y, stonePos.z, 0,
                                    this->actor.world.rot.y, 0, ENESTONE_TYPE_SMALL);
                    }
                }
                if (this->actor.world.pos.y - 50.0f <= player->actor.world.pos.y) {
                    for (i = 0; i < 10; i++) {
                        EnEgol_SpawnEffect(this, &hitPos, &gZeroVec3s, 30, (Rand_ZeroFloat(1.0f) * 0.1f) + 0.2f,
                                           EYEGORE_EFFECT_DEBRIS);
                    }
                }
                this->actionTimer = 1;
            }
            if (this->actionTimer != 0) {
                this->actionTimer++;
                if (this->actionTimer >= 5) {
                    this->laserState = EYEGORE_LASER_OFF;
                    Math_Vec3f_Copy(&this->laserScale, &gZeroVec3f);
                    Math_Vec3f_Copy(&this->laserScaleTarget, &gZeroVec3f);
                    Math_Vec3f_Copy(&this->laserCollider.dim.quad[1], &this->laserBase);
                    Math_Vec3f_Copy(&this->laserCollider.dim.quad[3], &this->laserBase);
                    Math_Vec3f_Copy(&this->laserCollider.dim.quad[0], &this->laserBase);
                    Math_Vec3f_Copy(&this->laserCollider.dim.quad[2], &this->laserBase);
                    this->actionTimer = 0;
                }
            }
            Math_ApproachF(&this->laserScale.x, this->laserScaleTarget.x, 0.5f, 0.5f);
            Math_ApproachF(&this->laserScale.y, this->laserScaleTarget.y, 0.5f, 0.5f);
            Math_ApproachF(&this->laserScale.z, this->laserScaleTarget.z, 0.5f, 0.5f);
        }
    }
}

void EnEgol_SetupStop(EnEgol* this) {
    if (!this->isRetreating) {
        EnEgol_ChangeAnim(this, EYEGORE_ANIM_LASER);
    } else {
        EnEgol_ChangeAnim(this, EYEGORE_ANIM_STUN_END);
    }
    this->action = EYEGORE_ACTION_STOP;
    this->actionFunc = EnEgol_Stop;
}

void EnEgol_Stop(EnEgol* this, PlayState* play) {
    s32 isHome = false;
    f32 curFrame = this->skelAnime.curFrame;
    s16 angleToHome;

    EnEgol_FootstepEffects(this, play, 28.0f, 13.0f);
    if (this->isRetreating) {
        Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.home.rot.y, 5, 0xBB8, 5);
        this->actor.world.rot.y = this->actor.shape.rot.y;
        angleToHome = this->actor.world.rot.y - this->actor.home.rot.y;
    }
    if (!this->isRetreating) {
        isHome = true;
    } else if (ABS_ALT(angleToHome) < 0x64) {
        this->actor.world.rot.y = this->actor.shape.rot.y = this->actor.home.rot.y;
        isHome = true;
    }
    if (isHome && (curFrame >= this->animEndFrame)) {
        this->isRetreating ^= true;
        if (!this->isRetreating) {
            EnEgol_ChangeAnim(this, EYEGORE_ANIM_SIT);
            //Audio_PlayActorSfx2(&this->actor, NA_SE_EN_EYEGOLE_SIT);
            SoundEffect_PlayOneshot(SOUND_ENEMY_EYEGORE_SIT, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 1000.0f, &this->actor);
            this->laserCount = 0;
            func_800F5B58();
            this->actionFunc = EnEgol_Wait;
        } else {
            EnEgol_SetupRetreat(this);
        }
    }
}

void EnEgol_SetupSlam(EnEgol* this) {
    EnEgol_ChangeAnim(this, EYEGORE_ANIM_SLAM);
    this->action = EYEGORE_ACTION_SLAM;
    this->actionFunc = EnEgol_Slam;
}

void EnEgol_Slam(EnEgol* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;
    Vec3f gZeroVec3s = {0};

    Math_SmoothStepToS(&this->headRot.z, 0, 1, 0x3E8, 0);
    if (Animation_OnFrame(&this->skelAnime, 17.0f)) {
        s32 i;
        s32 spawnCount;
        Player* player = GET_PLAYER(play);
        Vec3f spawnPos;
        f32 dyToPlayer = fabsf(player->actor.world.pos.y - this->actor.world.pos.y);
        s32 quakeYOffset;

        Audio_PlayActorSfx2(&this->actor, NA_SE_EV_EXPLOSION);
        SoundEffect_PlayOneshot(SOUND_METAL_FIERCE_HIT, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 1000.0f, &this->actor);
        quakeYOffset = 4 - (s32)(dyToPlayer * 0.02f);
        if (quakeYOffset > 4) {
            quakeYOffset = 4;
        } else if (quakeYOffset < 1) {
            quakeYOffset = 1;
        }
        Actor_RequestQuakeAndRumble(&this->actor, play, quakeYOffset, 2);
        if (this->actor.floorBgId == BGCHECK_SCENE) {
            Math_Vec3f_Copy(&spawnPos, &this->actor.world.pos);
            spawnPos.x += Math_SinS(this->actor.world.rot.y) * 60.0f;
            spawnPos.y = this->actor.floorHeight;
            spawnPos.z += Math_CosS(this->actor.world.rot.y) * 60.0f;
            spawnCount = Rand_S16Offset(1, 3);
            for (i = 0; i < spawnCount; i++) {
                Actor_Spawn(&play->actorCtx, play, 0x0258, spawnPos.x, spawnPos.y, spawnPos.z, 0,
                            this->actor.world.rot.y, 0, ENESTONE_TYPE_LARGE);
            }

            Actor_SpawnFloorDustRing(play, &this->actor, &spawnPos, 30.0f, 30, 10.0f, 100, 30, true);
            Math_Vec3f_Copy(&spawnPos, &this->actor.world.pos);
            spawnPos.x += Math_SinS(this->actor.world.rot.y) * 55.0f;
            spawnPos.y = this->actor.floorHeight + 2.0f;
            spawnPos.z += Math_CosS(this->actor.world.rot.y) * 55.0f;
            EnEgol_SpawnEffect(this, &spawnPos, &gZeroVec3s, 100, 0.03f, EYEGORE_EFFECT_IMPACT);
            if((Actor_WorldDistXYZToPoint(&player->actor, &spawnPos) < 90.0f) && (ABS(player->actor.world.pos.y - spawnPos.y)<20.0f)){
               func_8002F6D4(play, &this->actor, 5.0f, this->actor.world.rot.y, 1.0f, 0);
            }
        }
        EnEgol_DestroyBlocks(this, play, this->rightHandPos, this->leftHandPos);
    }
    if (curFrame >= this->animEndFrame) {
        EnEgol_SetupSlamWait(this);
    } else if ((this->skelAnime.curFrame <= 17.0f) && (this->skelAnime.curFrame >= 10.0f)) {
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->bodyCollider.base);
    }
}

void EnEgol_SetupPunch(EnEgol* this) {
    this->bodyCollider.elements[0].dim.modelSphere.radius = 40;
    this->bodyCollider.elements[1].dim.modelSphere.radius = 40;
    this->hitPlayer = false;
    this->action = EYEGORE_ACTION_PUNCH;
    this->actionFunc = EnEgol_Punch;
}

void EnEgol_Punch(EnEgol* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;

    Math_SmoothStepToS(&this->headRot.z, 0, 1, 0x3E8, 0);
    if (curFrame >= this->animEndFrame) {
        this->bodyCollider.elements[0].dim.modelSphere.radius = 20;
        this->bodyCollider.elements[1].dim.modelSphere.radius = 20;
        EnEgol_SetupWalk(this);
    } else if (!this->hitPlayer) {
        if ((this->bodyCollider.elements[0].info.toucherFlags & TOUCH_HIT) ||
            (this->bodyCollider.elements[1].info.toucherFlags & TOUCH_HIT)) {
            this->hitPlayer = true;
            func_8002F6D4(play, &this->actor, 6.0f, this->actor.world.rot.y, 10.0f, 0);
        }
        if (!(this->bodyCollider.base.atFlags & AT_BOUNCED)) {
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->bodyCollider.base);
        }
    }
}

void EnEgol_SetupSlamWait(EnEgol* this) {
    EnEgol_ChangeAnim(this, EYEGORE_ANIM_SLAM_WAIT);
    this->actionTimer = 0;
    this->eyeCollider.elements[0].info.elemType = ELEMTYPE_UNK1;
    this->action = EYEGORE_ACTION_SLAM_WAIT;
    this->actionFunc = EnEgol_SlamWait;
}

void EnEgol_SlamWait(EnEgol* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;

    if (curFrame >= this->animEndFrame) {
        this->actionTimer++;
        if (this->actionTimer > 5) {
            this->eyeCollider.elements[0].info.elemType = ELEMTYPE_UNK2;
            EnEgol_SetupSlamEnd(this);
        }
    }
}

void EnEgol_SetupStunned(EnEgol* this) {
    EnEgol_ChangeAnim(this, EYEGORE_ANIM_STUNNED);
    this->actionTimer = 0;
    this->eyeCollider.elements[0].info.elemType = ELEMTYPE_UNK1;
    this->bodyCollider.elements[0].dim.modelSphere.radius = 0;
    this->bodyCollider.elements[1].dim.modelSphere.radius = 0;
    this->action = EYEGORE_ACTION_STUNNED;
    this->actionFunc = EnEgol_Stunned;
}

void EnEgol_Stunned(EnEgol* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;

    if (curFrame >= this->animEndFrame) {
        //this->actionTimer++;
        if (this->dmgEffectTimer == 0) {
            this->eyeCollider.elements->info.elemType = ELEMTYPE_UNK2;
            EnEgol_SetupStunEnd(this);
        }
    }
}

void EnEgol_SetupStunEnd(EnEgol* this) {
    this->bodyCollider.elements[0].dim.modelSphere.radius = 20;
    this->bodyCollider.elements[1].dim.modelSphere.radius = 20;
    EnEgol_ChangeAnim(this, EYEGORE_ANIM_STUN_END);
    this->action = EYEGORE_ACTION_STUN_END;
    this->actionFunc = EnEgol_StunEnd;
}

void EnEgol_StunEnd(EnEgol* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;

    if (curFrame >= this->animEndFrame) {
        EnEgol_SetupWalk(this);
    }
}

void EnEgol_SetupSlamEnd(EnEgol* this) {
    EnEgol_ChangeAnim(this, EYEGORE_ANIM_SLAM_END);
    this->actionTimer = 0;
    this->action = EYEGORE_ACTION_SLAM_END;
    this->actionFunc = EnEgol_SlamEnd;
}

void EnEgol_SlamEnd(EnEgol* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;

    if (curFrame >= this->animEndFrame) {
        EnEgol_SetupWalk(this);
    }
}

void EnEgol_SetupDamaged(EnEgol* this) {
    EnEgol_ChangeAnim(this, EYEGORE_ANIM_DAMAGED);
    this->bodyCollider.elements[0].dim.modelSphere.radius = 20;
    this->bodyCollider.elements[1].dim.modelSphere.radius = 20;
    this->action = EYEGORE_ACTION_DAMAGED;
    this->actionFunc = EnEgol_Damaged;
}

void EnEgol_Damaged(EnEgol* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;

    if (curFrame >= this->animEndFrame) {
        if (this->actor.colChkInfo.health > 0) {
            EnEgol_SetupWalk(this);
        } else {
            Enemy_StartFinishingBlow(play, &this->actor);
            //Audio_PlayActorSfx2(&this->actor, NA_SE_EN_AMOS_DEAD);
            SoundEffect_PlayOneshot(SOUND_ENEMY_DEATH_ARMOS_DIE_X, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 1000.0f, &this->actor);
            this->actor.flags |= ACTOR_FLAG_27;
            this->actor.flags &= ~ACTOR_FLAG_0;
            this->actor.flags |= ACTOR_FLAG_20;
            this->actionFunc = EnEgol_StartDeathCutscene;
        }
    }
}

void EnEgol_StartDeathCutscene(EnEgol* this, PlayState* play) {
    //if (!CutsceneManager_IsNext(this->actor.csId)) {
    //    CutsceneManager_Queue(this->actor.csId);
    //} else {
        //CutsceneManager_StartWithPlayerCs(this->actor.csId, &this->actor);
        //this->subCamId = CutsceneManager_GetCurrentSubCamId(this->actor.csId);
        this->subCamFovTarget = 60.0f;
        EnEgol_ChangeAnim(this, EYEGORE_ANIM_DEATH);
        this->action = EYEGORE_ACTION_DYING;
        this->actionFunc = EnEgol_Death;
        func_800F5B58();
        this->actor.shape.shadowScale = 0.0f;
    //}
}

void EnEgol_Death(EnEgol* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;
    Vec3f atOffset;
    Vec3f gZeroVec3s = {0};
    Vec3f effVelocity = { 0.0f, 0.0f, 0.0f };
    Vec3f bomb2Accel = { 0.0f, 0.1f, 0.0f };
    Vec3f effAccel = { 0.0f, 0.0f, 0.0f };
    Vec3f effPos;

    this->subCamAt.x = 0.0f;
    this->subCamAt.y = 60.0f;
    this->subCamAt.z = 260.0f;
    Math_Vec3f_Copy(&atOffset, &this->subCamAt);
    OLib_Vec3fAdd(&this->actor.world, &atOffset, &this->subCamAt, OLIB_ADD_OFFSET);
    this->subCamEye.x = this->actor.world.pos.x;
    this->subCamEye.y = this->actor.world.pos.y + 70.0f;
    this->subCamEye.z = this->actor.world.pos.z;
    Math_ApproachF(&this->subCamFov, this->subCamFovTarget, 0.3f, 10.0f);
    // Play_CameraSetAtEye(play, this->subCamId, &this->subCamEye, &this->subCamAt);
    // Play_CameraSetFov(play, this->subCamId, this->subCamFov);
    if ((this->action == EYEGORE_ACTION_DEAD) && (this->waitTimer == 1)) {
        if (this->switchFlag > -1) {
            Flags_SetSwitch(play, this->switchFlag);
        }

        //CutsceneManager_Stop(this->actor.csId);
        Actor_Kill(&this->actor);
    } else {
        if (Animation_OnFrame(&this->skelAnime, 46.0f)) {
            Audio_PlayActorSfx2(&this->actor, NA_SE_EV_EXPLOSION);
            Actor_RequestQuakeAndRumble(&this->actor, play, 10, 5);
        }
        if (Animation_OnFrame(&this->skelAnime, 30.0f)) {
            Audio_PlayActorSfx2(&this->actor, NA_SE_EN_GOLON_LAND_BIG);
            Actor_RequestQuakeAndRumble(&this->actor, play, 10, 5);
        }
        if ((curFrame >= this->animEndFrame) && (this->action != EYEGORE_ACTION_DEAD)) {
            s32 i;
            effPos = this->actor.world.pos;
            effPos.y += 10.0f;
            //EffectSsBomb2_SpawnLayered(&this->actor, &effPos, &effVelocity, &bomb2Accel, 100,
            //                           (this->actor.shape.rot.z * 6) + 19);
            effPos.y = this->actor.floorHeight;
            if (this->actor.floorHeight > BGCHECK_Y_MIN) {
                EffectSsBlast_SpawnWhiteShockwave(&this->actor, &effPos, &effVelocity, &effAccel);
            }
            Actor_Spawn(&play->actorCtx, play, 0x033B, this->actor.world.pos.x, this->actor.world.pos.y,
                        this->actor.world.pos.z, 0, 0, 0, 0);
            this->waitTimer = 30;
            
            Audio_PlayActorSfx2(&this->actor, NA_SE_IT_BOMB_EXPLOSION);
            this->action = EYEGORE_ACTION_DEAD;
            for (i = 0; i < 20; i++) {
                EnEgol_SpawnEffect(this, &this->actor.world.pos, &gZeroVec3s, 10.0f + Rand_ZeroFloat(20.0f),
                                   0.01f + (0.02f * Rand_ZeroFloat(1.0f)), (i & 1) + EYEGORE_EFFECT_PIECE_LARGE);
            }
        }
    }
}

typedef enum {
    /* 0 */ EYEGORE_HIT_NONE,
    /* 1 */ EYEGORE_HIT_DAMAGE,
    /* 2 */ EYEGORE_HIT_IMMUNE // Makes hitmarks, but no reaction
} EyegoreHitReaction;

void EnEgol_CollisionCheck(EnEgol* this, PlayState* play) {
    s16 angleToFacing = this->actor.world.rot.y - this->actor.yawTowardsPlayer;
    s32 reaction = EYEGORE_HIT_NONE;
    Vec3f gZeroVec3f = {0};

    if ((this->action == EYEGORE_ACTION_LASER) && (this->laserCollider.base.atFlags & AT_BOUNCED)) {
        this->laserCollider.base.atFlags &= ~AT_HIT;
        this->laserCollider.base.atFlags &= ~AT_BOUNCED;
        this->actionTimer = 0;
        this->laserState = EYEGORE_LASER_OFF;
        Math_Vec3f_Copy(&this->laserScale, &gZeroVec3f);
        Math_Vec3f_Copy(&this->laserScaleTarget, &gZeroVec3f);
        Math_Vec3f_Copy(&this->laserCollider.dim.quad[1], &this->laserBase);
        Math_Vec3f_Copy(&this->laserCollider.dim.quad[3], &this->laserBase);
        Math_Vec3f_Copy(&this->laserCollider.dim.quad[0], &this->laserBase);
        Math_Vec3f_Copy(&this->laserCollider.dim.quad[2], &this->laserBase);
        EnEgol_SetupWalk(this);
    }
    if ((this->action == EYEGORE_ACTION_LASER) && (this->laserCollider.base.atFlags & AT_HIT)) {
        this->laserCollider.base.atFlags &= ~AT_HIT;
        this->laserCollider.base.atFlags &= ~AT_BOUNCED;
        func_8002F698(play, &this->actor, 3.0f, this->actor.world.rot.y, 5.0f, 3, 0x10);
    }
    if (this->eyeCollider.elements[0].info.bumperFlags & BUMP_HIT) {
        this->eyeCollider.elements[0].info.bumperFlags  &= ~ BUMP_HIT;
        reaction = EYEGORE_HIT_IMMUNE;
        switch (this->actor.colChkInfo.damageEffect) {
            case EYEGORE_DMGEFF_LIGHT_ARROW:
                if (ABS_ALT(angleToFacing) < 0x3000) {
                    if ((this->action == EYEGORE_ACTION_STUNNED) || (this->action == EYEGORE_ACTION_SLAM_WAIT)) {
                        Actor_Spawn(&play->actorCtx, play, 0x033B, this->actor.focus.pos.x,
                                    this->actor.focus.pos.y, this->actor.focus.pos.z, 0, 0, 0,
                                    4);
                        this->dmgEffectTimer = 30;
                        Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_BLUE, 0xff, COLORFILTER_BUFFLAG_OPA,
                        this->dmgEffectTimer);
                        reaction = EYEGORE_HIT_DAMAGE;
                    } else if ((this->action >= EYEGORE_ACTION_WALK) && (this->action <= EYEGORE_ACTION_STUN_END) &&
                               !this->isRetreating) {
                        reaction = EYEGORE_HIT_NONE;
                        this->chargingLaser = false;
                        this->chargeLevel = 0;
                        this->actionTimer = 0;
                        this->laserState = EYEGORE_LASER_OFF;
                        Math_Vec3f_Copy(&this->laserScale, &gZeroVec3f);
                        Math_Vec3f_Copy(&this->laserScaleTarget, &gZeroVec3f);
                        Math_Vec3f_Copy(&this->laserCollider.dim.quad[1], &this->laserBase);
                        Math_Vec3f_Copy(&this->laserCollider.dim.quad[3], &this->laserBase);
                        Math_Vec3f_Copy(&this->laserCollider.dim.quad[0], &this->laserBase);
                        Math_Vec3f_Copy(&this->laserCollider.dim.quad[2], &this->laserBase);
                        this->dmgEffectTimer = 60;
                        Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_BLUE, 0xff, COLORFILTER_BUFFLAG_OPA,
                                             this->dmgEffectTimer);
                        Audio_PlayActorSfx2(&this->actor, NA_SE_EN_GOMA_JR_FREEZE);
                        EnEgol_SetupStunned(this);
                    }
                }
                break;

            case EYEGORE_DMGEFF_NONE:
                if ((this->action == EYEGORE_ACTION_SLAM_WAIT) || (this->action == EYEGORE_ACTION_STUNNED)) {
                    reaction = EYEGORE_HIT_DAMAGE;
                }
                break;

            default:
                break;
        }
    }
    if (reaction == EYEGORE_HIT_DAMAGE) {
        Actor_ApplyDamage(&this->actor);
        CollisionCheck_RedBlood(play, NULL, &this->eyePos);
        CollisionCheck_RedBlood(play, NULL, &this->eyePos);
        CollisionCheck_RedBlood(play, NULL, &this->eyePos);
        Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 25);
        //Audio_PlayActorSfx2(&this->actor, NA_SE_EN_AMOS_DAMAGE);
        Audio_PlayActorSfx2(&this->actor, NA_SE_EN_FANTOM_DAMAGE);
        EnEgol_SetupDamaged(this);
    } else if (reaction == EYEGORE_HIT_IMMUNE) {
        Vec3f hitPos;

        hitPos.x = this->eyeCollider.elements[0].info.bumper.hitPos.x;
        hitPos.y = this->eyeCollider.elements[0].info.bumper.hitPos.y;
        hitPos.z = this->eyeCollider.elements[0].info.bumper.hitPos.z;
        Audio_PlayActorSfx2(&this->actor, NA_SE_IT_SHIELD_BOUND);
        EffectSsHitMark_SpawnFixedScale(play, 3, &hitPos);
        CollisionCheck_SpawnShieldParticlesMetal(play, &hitPos);
    }
}

void EnEgol_UpdateHeadRot(EnEgol* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    f32 dist;
    dist = 500.0f - this->actor.xzDistToPlayer;
    Vec3f playerPos = player->actor.world.pos;
    playerPos.y = playerPos.y + 50.0f;
    s16 rotateSpeed;
     if((this->actor.xzDistToPlayer > 500.0f || this->actor.xzDistToPlayer < 250.0f)){
        rotateSpeed = 10;
    }else{
        rotateSpeed = CLAMP((this->actor.xzDistToPlayer/ (100.0f))*1.5f ,2.0f,8.0f);
    }
    if (this->headRot.x > 0x2B91) {
        this->headRot.x = 0x2B91;
    }
    this->pitch = Math_Vec3f_Pitch(&this->headPos, &playerPos);
    if (this->pitch > 0x1AAA) {
        this->pitch = 0x1AAA;
    }
    Math_SmoothStepToS(&this->headRot.x, this->pitch, 10, 0xAA0, 0);
    Math_SmoothStepToS(&this->headRot.y, this->actor.yawTowardsPlayer, rotateSpeed, 0XAA0, 0);
}

void EnEgol_Update(Actor* thisx, PlayState* play) {
    EnEgol* this = THIS;
    Player* player = GET_PLAYER(play);
    s32 pad;
    Vec3f gZeroVec3f = {0};

    if (!((this->action == EYEGORE_ACTION_PUNCH) || (this->action == EYEGORE_ACTION_RETREAT) || (this->action == EYEGORE_ACTION_WAIT) ||
          (this->action >= EYEGORE_ACTION_DYING) || ((this->action >= EYEGORE_ACTION_LASER) && (this->action <= EYEGORE_ACTION_DAMAGED)))) {
        s16 angleToFacing = this->actor.shape.rot.y - this->actor.yawTowardsPlayer;
        s16 angleBehind = this->actor.world.rot.y - this->actor.yawTowardsPlayer + 0x8000;

        if ((ABS_ALT(angleToFacing) > 0x1888) && (ABS_ALT(angleBehind) > 0x2000) &&
            (this->actor.xzDistToPlayer < 100.0f)) {
            if (angleToFacing < 0) {
                if (this->animIndex != EYEGORE_ANIM_LEFT_PUNCH) {
                    EnEgol_ChangeAnim(this, EYEGORE_ANIM_LEFT_PUNCH);
                }
            } else if (this->animIndex != EYEGORE_ANIM_RIGHT_PUNCH) {
                EnEgol_ChangeAnim(this, EYEGORE_ANIM_RIGHT_PUNCH);
            }
            this->chargingLaser = false;
            this->chargeLevel = 0;
            this->actionTimer = 0;
            this->laserState = EYEGORE_LASER_OFF;
            Math_Vec3f_Copy(&this->laserScale, &gZeroVec3f);
            Math_Vec3f_Copy(&this->laserScaleTarget, &gZeroVec3f);
            Math_Vec3f_Copy(&this->laserCollider.dim.quad[1], &this->laserBase);
            Math_Vec3f_Copy(&this->laserCollider.dim.quad[3], &this->laserBase);
            Math_Vec3f_Copy(&this->laserCollider.dim.quad[0], &this->laserBase);
            Math_Vec3f_Copy(&this->laserCollider.dim.quad[2], &this->laserBase);
            EnEgol_SetupPunch(this);
        }
    }
    if (this->action != EYEGORE_ACTION_WAIT) {
        SkelAnime_Update(&this->skelAnime);
    }
    if (this->laserState == EYEGORE_LASER_OFF) {
        Math_ApproachZeroF(&this->laserLightScale, 0.5f, 0.5f);
    } else if (this->laserState >= EYEGORE_LASER_FIRE) {
        Math_ApproachF(&this->laserLightScale, 1.0f, 0.5f, 0.5f);
    }
    Actor_SetScale(&this->actor, 0.015f);

    if (!((this->action == EYEGORE_ACTION_STOP) || (this->action == EYEGORE_ACTION_RETREAT))) {
        this->actor.shape.rot.y = this->actor.world.rot.y;
    }
    this->texScroll += 12;
    DECR(this->waitTimer);
    DECR(this->eyeShutTimer);
    DECR(this->dmgEffectTimer);

    EnEgol_UpdateEffects(this, play);
    this->actionFunc(this, play);
    Math_Vec3f_Copy(&this->actor.focus.pos, &this->eyePos);
    this->actor.focus.rot.x = this->actor.world.rot.x;
    this->actor.focus.rot.y = this->actor.world.rot.y;
    this->actor.focus.rot.z = this->actor.world.rot.z;
    if (!((this->action == EYEGORE_ACTION_SLAM_WAIT) || (this->action == EYEGORE_ACTION_WAIT) ||
          (this->action == EYEGORE_ACTION_RETREAT) || (this->action == EYEGORE_ACTION_STAND) ||
          (this->action == EYEGORE_ACTION_LASER) || (this->action == EYEGORE_ACTION_STUNNED) ||
          (this->action == EYEGORE_ACTION_DAMAGED) || (this->action == EYEGORE_ACTION_DYING))) {

        this->pupilRot.x = Math_Vec3f_Pitch(&this->eyePos, &player->actor.world.pos);
        this->pupilRot.y = -Math_Vec3f_Yaw(&this->eyePos, &player->actor.world.pos);

        this->pupilRot.y += this->actor.world.rot.y;

        if (this->pupilRot.x > 0x7D0) {
            this->pupilRot.x = 0x7D0;
        } else if (this->pupilRot.x < -0x7D0) {
            this->pupilRot.x = -0x7D0;
        }
        if (this->pupilRot.y > 0x1770) {
            this->pupilRot.y = 0x1770;
        } else if (this->pupilRot.y < -0x1770) {
            this->pupilRot.y = -0x1770;
        }
    } else {
        this->pupilRot.x = this->pupilRot.y = 0;
    }
    if (this->eyeShutTimer == 0) {
        if (((this->action == EYEGORE_ACTION_WALK) || (this->action == EYEGORE_ACTION_LASER)) && !this->isRetreating) {
            Actor* projectile = Actor_GetProjectileActor(play, &this->actor, 80.0f);
            // last argument is DMG_ZORA_BOOMERANG | DMG_HOOKSHOT | DMG_NORMAL_ARROW | DMG_FIRE_ARROW | DMG_ICE_ARROW |
            // DMG_LIGHT_ARROW | DMG_DEKU_BUBBLE

            if ((projectile != NULL) &&
                !((projectile->id == ACTOR_EN_ARROW) && (projectile->params == ARROW_LIGHT))) {
                this->eyelidRotTarget = 0xFA0;
                this->eyeShutTimer = 20;
                if (this->action == EYEGORE_ACTION_LASER) {
                    this->chargingLaser = false;
                    this->chargeLevel = 0;
                    this->actionTimer = 0;
                    this->laserState = EYEGORE_LASER_OFF;
                    Math_Vec3f_Copy(&this->laserScale, &gZeroVec3f);
                    Math_Vec3f_Copy(&this->laserScaleTarget, &gZeroVec3f);
                    Math_Vec3f_Copy(&this->laserCollider.dim.quad[1], &this->laserBase);
                    Math_Vec3f_Copy(&this->laserCollider.dim.quad[3], &this->laserBase);
                    Math_Vec3f_Copy(&this->laserCollider.dim.quad[0], &this->laserBase);
                    Math_Vec3f_Copy(&this->laserCollider.dim.quad[2], &this->laserBase);
                    EnEgol_SetupWalk(this);
                }
            }
        } else {
            this->eyelidRotTarget = 0;
        }
    } else if (this->eyeShutTimer == 1) {
        this->eyelidRotTarget = 0;
    }
    Math_SmoothStepToS(&this->eyelidRot, this->eyelidRotTarget, 1, 0x7D0, 0);
    EnEgol_CollisionCheck(this, play);
    Actor_MoveForward(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 20.0f, 50.0f, 50.0f,
                            UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2 | UPDBGCHECKINFO_FLAG_3 |
                                UPDBGCHECKINFO_FLAG_4);
    if (this->action != EYEGORE_ACTION_DEAD) {
        //! @bug This should be ||, not &&. As is, the check always succeeds.
        if (!((this->action == EYEGORE_ACTION_DAMAGED) || (this->action == EYEGORE_ACTION_DYING))) {
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->eyeCollider.base);
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->bodyCollider.base);
        }
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->bodyCollider.base);
    }
    if (this->laserState >= EYEGORE_LASER_ON) {
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->laserCollider.base);
    }
}

s32 EnEgol_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx) {
    EnEgol* this = THIS;

    if (limbIndex == EYEGORE_LIMB_HEAD) {
        rot->z += this->headRot.z;
        // rot->x += this->headRot.x;
        // rot->y += this->headRot.y;
    }
    if (limbIndex == EYEGORE_LIMB_UPPER_EYELID) {
        rot->z += this->eyelidRot;
    }
    if (limbIndex == EYEGORE_LIMB_LOWER_EYELID) {
        rot->z += -1 * this->eyelidRot;
    }
    if (limbIndex == EYEGORE_LIMB_PUPIL) {
        rot->y += this->pupilRot.y;
        rot->z += this->pupilRot.x;
    }
    if (limbIndex == EYEGORE_LIMB_EYEBALL) {
        s32 eyeColorIndex = 0;
        
        OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

        if ((this->action == EYEGORE_ACTION_DAMAGED) || (this->action == EYEGORE_ACTION_DYING)) {
            eyeColorIndex = 2;
        } else if ((this->action == EYEGORE_ACTION_STUNNED) || (this->action == EYEGORE_ACTION_SLAM_WAIT)) {
            eyeColorIndex = 1;
        }
        this->eyePrimColor.r = sEyePrimColors[eyeColorIndex].r;
        this->eyePrimColor.g = sEyePrimColors[eyeColorIndex].g;
        this->eyePrimColor.b = sEyePrimColors[eyeColorIndex].b;
        this->eyeEnvColor.r = sEyeEnvColors[eyeColorIndex].r;
        this->eyeEnvColor.g = sEyeEnvColors[eyeColorIndex].g;
        this->eyeEnvColor.b = sEyeEnvColors[eyeColorIndex].b;

        gDPPipeSync(POLY_OPA_DISP++);
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0xFF, (u8)this->eyePrimColor.r, (u8)this->eyePrimColor.g,
                        (u8)this->eyePrimColor.b, 255);
        gDPSetEnvColor(POLY_OPA_DISP++, (u8)this->eyeEnvColor.r, (u8)this->eyeEnvColor.g, (u8)this->eyeEnvColor.b, 255);
        gSPSegment(POLY_OPA_DISP++, 0x09, func_8012CB28(play->state.gfxCtx, 0, this->texScroll));

        CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
    }
    return false;
}

void EnEgol_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx) {
    EnEgol* this = THIS;
    Vec3f footOffset = { 1000.0f, 0.0f, 0.0f };
    Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };
    Vec3f gZeroVec3f = {0};
    Vec3f bodyPartPos;
    s32 bodyPart = -1;

    if (limbIndex == EYEGORE_LIMB_LEFT_HAND) {
        Matrix_MultVec3f(&gZeroVec3f, &this->leftHandPos);
    }
    if (limbIndex == EYEGORE_LIMB_RIGHT_HAND) {
        Matrix_MultVec3f(&gZeroVec3f, &this->rightHandPos);
    }
    if (limbIndex == EYEGORE_LIMB_LASER_ATTACH) {
        Matrix_MultVec3f(&gZeroVec3f, &this->eyePos);
    }
    if (limbIndex == EYEGORE_LIMB_HEAD) {
        Matrix_MultVec3f(&gZeroVec3f, &this->headPos);
    }
    if ((limbIndex == EYEGORE_LIMB_LASER_ATTACH) && (this->laserState >= EYEGORE_LASER_FIRE)) {
        Vec3f laserLengthZ = { 0.0f, 0.0f, 0.0f };
        Vec3f laserVec;
        f32 targetScale;
        f32 targetHeight;

        Matrix_MultVec3f(&zeroVec, &this->laserBase);
        this->laserCollider.dim.quad[3].x =
            this->laserBase.x + Math_SinS(this->actor.world.rot.y + 0x4000) * this->laserScale.x * 77.0f;
        this->laserCollider.dim.quad[3].y = this->laserBase.y;
        this->laserCollider.dim.quad[3].z =
            this->laserBase.z + Math_CosS(this->actor.world.rot.y + 0x4000) * this->laserScale.x * 77.0f;
        this->laserCollider.dim.quad[2].x =
            this->laserBase.x + Math_SinS(this->actor.world.rot.y + 0xC000) * this->laserScale.x * 77.0f;
        this->laserCollider.dim.quad[2].y = this->laserBase.y;
        this->laserCollider.dim.quad[2].z =
            this->laserBase.z + Math_CosS(this->actor.world.rot.y + 0xC000) * this->laserScale.x * 77.0f;

        targetScale = this->laserScale.z;
        targetHeight = this->laserCollider.dim.quad[3].y;

        while ((targetHeight + 20.0f > this->actor.floorHeight) && (targetScale < 10.0f)) {
            Matrix_Push();
            Matrix_RotateYS(this->actor.world.rot.y, MTXMODE_NEW);
            Matrix_RotateXS(this->laserRot.x, MTXMODE_APPLY);
            laserLengthZ.z = targetScale * 700.0f;
            Matrix_MultVec3f(&laserLengthZ, &laserVec);
            Matrix_Pop();
            targetHeight = this->laserCollider.dim.quad[3].y + laserVec.y;
            targetScale += 0.01f;
        }
        this->laserScaleTarget.z = targetScale;
        Matrix_Push();
        Matrix_RotateYS(this->actor.world.rot.y, MTXMODE_NEW);
        Matrix_RotateXS(this->laserRot.x, MTXMODE_APPLY);
        laserLengthZ.z = this->laserScale.z * 700.0f;
        Matrix_MultVec3f(&laserLengthZ, &laserVec);
        Matrix_Pop();
        this->laserCollider.dim.quad[1].x = this->laserCollider.dim.quad[3].x + laserVec.x;
        this->laserCollider.dim.quad[1].y = this->laserCollider.dim.quad[3].y + laserVec.y;
        this->laserCollider.dim.quad[1].z = this->laserCollider.dim.quad[3].z + laserVec.z;
        this->laserCollider.dim.quad[0].x = this->laserCollider.dim.quad[2].x + laserVec.x;
        this->laserCollider.dim.quad[0].y = this->laserCollider.dim.quad[2].y + laserVec.y;
        this->laserCollider.dim.quad[0].z = this->laserCollider.dim.quad[2].z + laserVec.z;
        Collider_SetQuadVertices(&this->laserCollider, &this->laserCollider.dim.quad[0],
                                 &this->laserCollider.dim.quad[1], &this->laserCollider.dim.quad[2],
                                 &this->laserCollider.dim.quad[3]);
        this->laserState = EYEGORE_LASER_ON;
    }
    if ((limbIndex == EYEGORE_LIMB_LASER_ATTACH) && (this->laserState == EYEGORE_LASER_START)) {
        this->chargingLaser = true;
        this->laserState = EYEGORE_LASER_CHARGING;
        this->laserScaleTarget.x = 0.03f;
        this->laserScaleTarget.y = 0.03f;
    }
    if (limbIndex == EYEGORE_LIMB_LEFT_FOOT) {
        Matrix_MultVec3f(&footOffset, &this->leftFootPos);
    }
    if (limbIndex == EYEGORE_LIMB_RIGHT_FOOT) {
        Matrix_MultVec3f(&footOffset, &this->rightFootPos);
    }

    if ((limbIndex == EYEGORE_LIMB_HEAD) || (limbIndex == EYEGORE_LIMB_LEFT_SHOULDER) ||
        (limbIndex == EYEGORE_LIMB_LEFT_ARM) || (limbIndex == EYEGORE_LIMB_LEFT_HAND) ||
        (limbIndex == EYEGORE_LIMB_RIGHT_SHOULDER) || (limbIndex == EYEGORE_LIMB_RIGHT_ARM) ||
        (limbIndex == EYEGORE_LIMB_RIGHT_HAND) || (limbIndex == EYEGORE_LIMB_UPPER_EYELID) ||
        (limbIndex == EYEGORE_LIMB_LOWER_EYELID) || (limbIndex == EYEGORE_LIMB_HIPS) ||
        (limbIndex == EYEGORE_LIMB_LEFT_SHIN) || (limbIndex == EYEGORE_LIMB_RIGHT_SHIN)) {
        Matrix_MultZero(&this->bodyPartsPos[this->bodyPartIndex]);
        this->bodyPartIndex++;
        if (this->bodyPartIndex >= EYEGORE_BODYPART_MAX) {
            this->bodyPartIndex = 0;
        }
    }
    // if (this->dmgEffectTimer != 0) {
    //     switch (limbIndex) {
    //         case EYEGORE_LIMB_HEAD:
    //             bodyPart = 0;
    //             break;
    //         case EYEGORE_LIMB_LEFT_SHOULDER:
    //             bodyPart = 1;
    //             break;
    //         case EYEGORE_LIMB_LEFT_ARM:
    //             bodyPart = 2;
    //             break;
    //         case EYEGORE_LIMB_LEFT_HAND:
    //             bodyPart = 3;
    //             break;
    //         case EYEGORE_LIMB_RIGHT_SHOULDER:
    //             bodyPart = 4;
    //             break;
    //         case EYEGORE_LIMB_RIGHT_ARM:
    //             bodyPart = 5;
    //             break;
    //         case EYEGORE_LIMB_RIGHT_HAND:
    //             bodyPart = 6;
    //             break;
    //         case EYEGORE_LIMB_UPPER_EYELID:
    //             bodyPart = 7;
    //             break;
    //         case EYEGORE_LIMB_LOWER_EYELID:
    //             bodyPart = 8;
    //             break;
    //         case EYEGORE_LIMB_HIPS:
    //             bodyPart = 9;
    //             break;
    //         case EYEGORE_LIMB_LEFT_SHIN:
    //             bodyPart = 10;
    //             break;
    //         case EYEGORE_LIMB_RIGHT_SHIN:
    //             bodyPart = 11;
    //             break;                
    //     }
    // }
    //     if (bodyPart >= 0) {
    //         Matrix_MultVec3f(&gZeroVec3f, &bodyPartPos);
    //         this->freezeBodyPartsPos[bodyPart].x = bodyPartPos.x;
    //         this->freezeBodyPartsPos[bodyPart].y = bodyPartPos.y;
    //         this->freezeBodyPartsPos[bodyPart].z = bodyPartPos.z;
    //     }
    Collider_UpdateSpheres(limbIndex, &this->bodyCollider);
    Collider_UpdateSpheres(limbIndex, &this->eyeCollider);
}

void EnEgol_Draw(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    EnEgol* this = THIS;

    OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

    Gfx_SetupDL_25Opa(play->state.gfxCtx);
    if (this->action != EYEGORE_ACTION_DEAD) {
        AnimatedMat_Draw(play, Lib_SegmentedToVirtual(0x060094e4));
        SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount,
                              EnEgol_OverrideLimbDraw, EnEgol_PostLimbDraw, &this->actor);
        POLY_OPA_DISP = Play_SetFog(play, POLY_OPA_DISP);
    }
    if (this->dmgEffectTimer != 0) {
        s32 iceIndex = this->dmgEffectTimer >> 2;
        f32 drawDmgEffAlpha = 0.05f * this->dmgEffectTimer;
         Actor_DrawDamageEffects(play, &this->actor, this->bodyPartsPos, EYEGORE_BODYPART_MAX, 0.8f, 0.8f,
                                  drawDmgEffAlpha, ACTOR_DRAW_DMGEFF_FROZEN_SFX);
        // if( this->actor.colChkInfo.damageEffect == EYEGORE_DMGEFF_LIGHT_ARROW){
        //     if ((this->dmgEffectTimer % 4) == 0) {
        //         EffectSsEnIce_SpawnFlyingVec3s(play, thisx, &this->freezeBodyPartsPos[iceIndex], 150, 150, 150, 250, 235, 245,
        //                                        255, 1.5f);
        //     }
        // }
    }
    if (this->laserState >= EYEGORE_LASER_FIRE) {
        this->laserRot.y = this->actor.world.rot.y;
        gSPSegment(POLY_OPA_DISP++, 0x08, func_8012CB28(play->state.gfxCtx, 0, this->texScroll));
        gDPPipeSync(POLY_OPA_DISP++);
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, 255);
        Matrix_Translate(this->laserBase.x, this->laserBase.y, this->laserBase.z, MTXMODE_NEW);
        Matrix_RotateZYX(this->laserRot.x, this->laserRot.y, 0, MTXMODE_APPLY);
        Matrix_Scale(this->laserScale.x, this->laserScale.y, this->laserScale.z, MTXMODE_APPLY);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, 0x060041f0);
    }
    if (((this->action == EYEGORE_ACTION_LASER) && (this->laserState >= EYEGORE_LASER_FIRE)) || this->chargingLaser) {
        Gfx_SetupDL_25Xlu(play->state.gfxCtx);
        gSPDisplayList(POLY_XLU_DISP++, gLightOrbMaterial1DL);
        if (!this->chargingLaser) {
            s32 i;
            f32 laserLightScaleMod;
            f32 laserLightAlpha;

            gDPSetEnvColor(POLY_XLU_DISP++, 155, 255, 255, 128);
            Matrix_Translate(this->eyePos.x, this->eyePos.y, this->eyePos.z, MTXMODE_NEW);
            Matrix_Scale(this->laserLightScale, this->laserLightScale, this->laserLightScale, MTXMODE_APPLY);

            laserLightScaleMod = 10.0f;
            laserLightAlpha = 80.0f;
            for (i = 0; i < ARRAY_COUNT(sLightOrbColors); i++) {
                Matrix_Push();
                Matrix_Scale(laserLightScaleMod, laserLightScaleMod, laserLightScaleMod, MTXMODE_APPLY);
                Matrix_ReplaceRotation(&play->billboardMtxF);
                gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, sLightOrbColors[i].r, sLightOrbColors[i].g, sLightOrbColors[i].b,
                                laserLightAlpha);
                gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__),
                          G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPDisplayList(POLY_XLU_DISP++, gLightOrbModelDL);
                Matrix_Pop();
                laserLightScaleMod = 3.0f;
                laserLightAlpha = 200.0f;
            }
        } else {
            gDPSetEnvColor(POLY_XLU_DISP++, 155, 255, 255, 128);
            Matrix_Translate(this->eyePos.x, this->eyePos.y, this->eyePos.z, MTXMODE_NEW);
            Matrix_Scale(this->chargeLightScale, this->chargeLightScale, this->chargeLightScale, MTXMODE_APPLY);
            Matrix_Push();
            Matrix_Scale(1.0f, 1.0f, 1.0f, MTXMODE_APPLY);
            Matrix_ReplaceRotation(&play->billboardMtxF);
            Matrix_RotateZS(this->chargeLightRot, MTXMODE_APPLY);
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, 255);
            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, gLightOrbModelDL);
            Matrix_Pop();
        }
    }
    EnEgol_DrawEffects(this, play);

    CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
}

void EnEgol_SpawnEffect(EnEgol* this, Vec3f* pos, Vec3s* rot, s16 lifetime, f32 scale, s16 type) {
    EnEgolEffect* effect = this->effects;
    s16 i;

    for (i = 0; i < ARRAY_COUNT(this->effects); i++, effect++) {
        if (!effect->isActive) {
            effect->isActive = true;
            effect->scale = scale;
            effect->pos = *pos;
            effect->timer = lifetime;
            effect->alpha = 255;
            effect->type = type;
            effect->rot.x = Rand_CenteredFloat(0x7530);
            effect->rot.y = Rand_CenteredFloat(0x7530);
            effect->rot.z = Rand_CenteredFloat(0x7530);
            if ((effect->type == EYEGORE_EFFECT_PIECE_LARGE) || (effect->type == EYEGORE_EFFECT_PIECE_SMALL)) {
                effect->accel.y = -1.0f;
                effect->velocity.x = 4.0f * (Rand_ZeroOne() - 0.5f);
                effect->velocity.y = 10.0f + (10.0f * Rand_ZeroOne());
                effect->velocity.z = 4.0f * (Rand_ZeroOne() - 0.5f);
                break;
            } else if (effect->type == EYEGORE_EFFECT_DEBRIS) {
                effect->accel.y = -1.0f;
                effect->velocity.x = 0.5f * (Rand_ZeroOne() - 0.5f);
                effect->velocity.y = 5.0f + (5.0f * Rand_ZeroOne());
                effect->velocity.z = 0.5f * (Rand_ZeroOne() - 0.5f);
                effect->timer = 20.0f + Rand_ZeroFloat(10.0f);
                break;
            } else if (effect->type == EYEGORE_EFFECT_IMPACT) {
                effect->rot.x = rot->x;
                effect->rot.y = 0;
                effect->rot.z = rot->z;
                break;
            }
        }
    }
}

void EnEgol_UpdateEffects(EnEgol* this, PlayState* play) {
    EnEgolEffect* effect = this->effects;
    s32 i;

    for (i = 0; i < ARRAY_COUNT(this->effects); i++, effect++) {
        if (effect->isActive) {
            if (this->action == EYEGORE_ACTION_DYING) {
                effect->timer = 0;
            }
            if (effect->type != EYEGORE_EFFECT_IMPACT) {
                effect->rot.x += 0x100;
                effect->rot.z += 0x130;

                effect->pos.x += effect->velocity.x;
                effect->pos.y += effect->velocity.y;
                effect->pos.z += effect->velocity.z;

                effect->velocity.y += effect->accel.y;
            }
            if (effect->timer != 0) {
                effect->timer--;
            } else if (effect->type == EYEGORE_EFFECT_IMPACT) {
                effect->alpha -= 10;
                if (effect->alpha < 10) {
                    effect->isActive = false;
                }
            } else {
                effect->isActive = false;
            }
        }
    }
}

void EnEgol_DrawEffects(EnEgol* this, PlayState* play) {
    GraphicsContext* gfxCtx = play->state.gfxCtx;
    EnEgolEffect* effect = this->effects;
    s16 i;

    OPEN_DISPS(gfxCtx, __FILE__, __LINE__);

    Gfx_SetupDL_25Opa(play->state.gfxCtx);
    for (i = 0; i < ARRAY_COUNT(this->effects); i++, effect++) {
        if (effect->isActive) {
            Matrix_Push();

            Matrix_Translate(effect->pos.x, effect->pos.y, effect->pos.z, MTXMODE_NEW);
            Matrix_RotateXS(effect->rot.x, MTXMODE_APPLY);
            Matrix_RotateYS(effect->rot.y, MTXMODE_APPLY);
            Matrix_RotateZS(effect->rot.z, MTXMODE_APPLY);
            Matrix_Scale(effect->scale, effect->scale, effect->scale, MTXMODE_APPLY);

            switch (effect->type) {
                case EYEGORE_EFFECT_IMPACT:
                    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0x80, 155, 155, 155, 255);
                    Matrix_Translate(0.0f, 50.0f, 0.0f, MTXMODE_APPLY);
                    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(gfxCtx, __FILE__, __LINE__), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                    gDPSetEnvColor(POLY_OPA_DISP++, 255, 255, 255, effect->alpha);
                    gSPDisplayList(POLY_OPA_DISP++, 0x06000040);
                    break;

                case EYEGORE_EFFECT_PIECE_LARGE:
                case EYEGORE_EFFECT_PIECE_SMALL:
                    gDPPipeSync(POLY_OPA_DISP++);
                    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0x80, 255, 255, 255, 255);
                    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(gfxCtx, __FILE__, __LINE__), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                    gDPSetEnvColor(POLY_OPA_DISP++, 255, 255, 255, effect->alpha);
                    if (effect->type == EYEGORE_EFFECT_PIECE_LARGE) {
                        gSPDisplayList(POLY_OPA_DISP++, 0x06001170);
                    } else {
                        gSPDisplayList(POLY_OPA_DISP++, 0x060013b0);
                    }
                    break;

                case EYEGORE_EFFECT_DEBRIS:
                    gDPPipeSync(POLY_OPA_DISP++);
                    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0x80, 255, 255, 255, 255);
                    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(gfxCtx, __FILE__, __LINE__), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                    gDPSetEnvColor(POLY_OPA_DISP++, 255, 255, 255, 255);
                    gSPDisplayList(POLY_OPA_DISP++, 0x06001918);
                    break;

                default:
                    break;
            }
            Matrix_Pop();
        }
    }

    CLOSE_DISPS(gfxCtx, __FILE__, __LINE__);
}
