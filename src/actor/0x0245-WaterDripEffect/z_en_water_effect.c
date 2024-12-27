/*
 * File: z_en_water_effect.c
 * Overlay: ovl_En_Water_Effect
 * Description: Water/rock drop spawner and Gyorg water splashing effect
 *
 * This actor serves two purposes:
 * - It can act as a "spawner" for either water drops or flaming rocks. This
 *   spawner can be placed in the ceiling to drop these repeatedly.
 * - It is also used to control various water splashing effects for the Gyorg
 *   fight. Specifically, it handles the splashing that is caused when it
 *   enters or exits the water, and it handles the shockwave that is created
 *   when it rams the center platform.
 */

#include "z_en_water_effect.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h"
#include <uLib.h>

#define FLAGS (ACTOR_FLAG_0 | ACTOR_FLAG_2 | ACTOR_FLAG_4 | ACTOR_FLAG_5)
#define ABS_ALT(x) ((x) < 0 ? -(x) : (x))



void Matrix_MultVecZ(f32 z, Vec3f* dest) {
    MtxF* cmf = sCurrentMatrix;

    dest->x = cmf->xw + cmf->xz * z;
    dest->y = cmf->yw + cmf->yz * z;
    dest->z = cmf->zw + cmf->zz * z;
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

/**
 * @brief Right-multiply by a rotation about the x axis.
 *      current * R -> current
 *
 * @note Matrix_RotateXF() with mode APPLY.
 *
 * @param x rotation angle (radians).
 */
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

/**
 * @brief Replace current by a rotation about the x axis.
 *      R -> current
 *
 * @note Matrix_RotateXF() with mode NEW.
 *
 * @param x rotation angle (radians).
 */
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

/**
 * @brief Right-multiply by a rotation about the y axis
 *      - APPLY: current * R -> current
 *      - NEW: R -> current
 *
 * R is given by
 *
 * \f[
 *  \begin{pmatrix}
 *      c & 0 & s & 0 \\
 *      0 & 1 & 0 & 0 \\
 *      -s & 0 & c & 0 \\
 *      0 & 0 & 0 & 1
 *  \end{pmatrix}
 * \f]
 *
 * where \f$ c = \cos y, s = \sin y \f$.
 *
 * @note The same as Matrix_RotateYF(), but uses a binary angle.
 *
 * @param y rotation angle (binary).
 * @param mode APPLY or NEW.
 *
 * @remark original name: "Matrix_RotateY"
 */
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

/**
 * @brief Right-multiply by a rotation about the y axis.
 *      - APPLY: current * R -> current
 *      - NEW: R -> current
 *
 * R is given by
 *
 * \f[
 *  \begin{pmatrix}
 *      c & 0 & s & 0 \\
 *      0 & 1 & 0 & 0 \\
 *      -s & 0 & c & 0 \\
 *      0 & 0 & 0 & 1
 *  \end{pmatrix}
 * \f]
 *
 * where \f$ c = \cos y, s = \sin y \f$.
 *
 * @note The same as Matrix_RotateYS(), but uses a float angle in radians.
 *
 * @param y rotation angle (radians).
 * @param mode APPLY or NEW.
 *
 * @remark original name may have been "Matrix_RotateY", but clashed with the previous function.
 */
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

/**
 * @brief Right-multiply by a rotation about the z axis.
 *      - APPLY: current * R -> current
 *      - NEW: R -> current
 *
 * R is given by
 *
 * \f[
 *  \begin{pmatrix}
 *      c & -s & 0 & 0 \\
 *      s & c & 0 & 0 \\
 *      0 & 0 & 1 & 0 \\
 *      0 & 0 & 0 & 1
 *  \end{pmatrix}
 * \f]
 *
 * where \f$ c = \cos z, s = \sin z \f$.
 *
 * @note The same as Matrix_RotateZF, but uses a binary angle.
 *
 * @param z rotation angle (binary).
 * @param mode APPLY or NEW.
 *
 * @remark original name: "Matrix_RotateZ"
 */
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

/**
 * @brief Right-multiply by a rotation about the z axis.
 *      - APPLY: current * R -> current
 *      - NEW: R -> current
 *
 * R is given by
 *
 * \f[
 *  \begin{pmatrix}
 *      c & -s & 0 & 0 \\
 *      s & c & 0 & 0 \\
 *      0 & 0 & 1 & 0 \\
 *      0 & 0 & 0 & 1
 *  \end{pmatrix}
 * \f]
 *
 * where \f$ c = \cos z, s = \sin z \f$.
 *
 * @note The same as Matrix_RotateYS(), but uses a float angle in radians.
 *
 * @param z rotation angle (radians).
 * @param mode APPLY or NEW.
 *
 * @remark original name may have been "Matrix_RotateZ", but clashed with the previous function.
 */
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

/**
 * @brief Rotate using ZYX Tait-Bryan angles.
 *      - APPLY: current Rz Ry Rx -> current
 *      - NEW: Rz Ry Rx -> current
 *
 * This means a (column) vector is first rotated around X, then around Y, then around Z, then (if `mode` is APPLY) gets
 * transformed by what the matrix was before adding the ZYX rotation.
 *
 * See previous functions for the forms of Rz, Ry, Rx
 *
 * @param x binary angle to rotate about x axis
 * @param y binary angle to rotate about y axis
 * @param z binary angle to rotate about z axis
 * @param mode APPLY or NEW
 *
 * @remark original name: "Matrix_RotateXYZ", changed to reflect rotation order.
 */

/**
 * @brief Translate and rotate using ZYX Tait-Bryan angles.
 *      current T Rz Ry Rx -> current
 *
 * This means a (column) vector is first rotated around X, then around Y, then around Z, then translated, then gets
 * transformed by whatever the matrix was previously.
 *
 * @param translation vector by which to translate.
 * @param rot vector of rotation angles.
 *
 * @remark original name appears to be "Matrix_softcv3_mult"
 */
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


/**
 * Stores a displaylist in the provided segment ID that sets a render mode from the index provided.
 */
void Scene_SetRenderModeXlu(PlayState* play, s32 index, u32 flags) {
    static Gfx renderModeSetNoneDL[] = {
        gsSPEndDisplayList(),
        // These instructions will never get executed
        gsSPEndDisplayList(),
        gsSPEndDisplayList(),
        gsSPEndDisplayList(),
    };
    static Gfx renderModeSetXluSingleCycleDL[] = {
        gsDPSetRenderMode(AA_EN | Z_CMP | IM_RD | CLR_ON_CVG | CVG_DST_WRAP | ZMODE_XLU | FORCE_BL |
                              GBL_c1(G_BL_CLR_IN, G_BL_0, G_BL_CLR_IN, G_BL_1),
                          G_RM_AA_ZB_XLU_SURF2),
        gsSPEndDisplayList(),
        // These instructions will never get executed
        gsDPSetRenderMode(AA_EN | Z_CMP | IM_RD | CLR_ON_CVG | CVG_DST_WRAP | ZMODE_XLU | FORCE_BL |
                              GBL_c1(G_BL_CLR_FOG, G_BL_A_SHADE, G_BL_CLR_IN, G_BL_1MA),
                          G_RM_AA_ZB_XLU_SURF2),
        gsSPEndDisplayList(),
    };
    static Gfx renderModeSetXluTwoCycleDL[] = {
        gsDPSetRenderMode(AA_EN | Z_CMP | Z_UPD | IM_RD | CLR_ON_CVG | CVG_DST_WRAP | ZMODE_XLU | FORCE_BL |
                              GBL_c1(G_BL_CLR_IN, G_BL_0, G_BL_CLR_IN, G_BL_1),
                          AA_EN | Z_CMP | Z_UPD | IM_RD | CLR_ON_CVG | CVG_DST_WRAP | ZMODE_XLU | FORCE_BL |
                              GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)),
        gsSPEndDisplayList(),
        // These instructions will never get executed
        gsDPSetRenderMode(AA_EN | Z_CMP | Z_UPD | IM_RD | CLR_ON_CVG | CVG_DST_WRAP | ZMODE_XLU | FORCE_BL |
                              GBL_c1(G_BL_CLR_FOG, G_BL_A_SHADE, G_BL_CLR_IN, G_BL_1MA),
                          AA_EN | Z_CMP | Z_UPD | IM_RD | CLR_ON_CVG | CVG_DST_WRAP | ZMODE_XLU | FORCE_BL |
                              GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)),
        gsSPEndDisplayList(),
    };
    static Gfx* dLists[] = {
        renderModeSetNoneDL,
        renderModeSetXluSingleCycleDL,
        renderModeSetXluTwoCycleDL,
    };
    Gfx* dList = dLists[index];

    OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

    if (flags & 1) {
        gSPSegment(POLY_OPA_DISP++, 0x0C, dList);
    }

    if (flags & 2) {
        gSPSegment(POLY_XLU_DISP++, 0x0C, dList);
    }

    CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
}


























#define THIS ((EnWaterEffect*)thisx)

void EnWaterEffect_Init(Actor* thisx, PlayState* play);
void EnWaterEffect_Destroy(Actor* thisx, PlayState* play);
void EnWaterEffect_Update(Actor* thisx, PlayState* play2);
void EnWaterEffect_Draw(Actor* thisx, PlayState* play2);

void func_80A59C04(Actor* thisx, PlayState* play2);
void func_80A5A184(Actor* thisx, PlayState* play2);
void func_80A5A534(Actor* thisx, PlayState* play);
void func_80A5A6B8(Actor* thisx, PlayState* play2);

ActorInit En_Water_Effect_InitVars = {
    0x0245,
    ACTORCAT_BOSS,
    FLAGS,
    0x0224,
    sizeof(EnWaterEffect),
    (ActorFunc)EnWaterEffect_Init,
    (ActorFunc)EnWaterEffect_Destroy,
    (ActorFunc)EnWaterEffect_Update,
    (ActorFunc)EnWaterEffect_Draw,
};

static Vec3f D_80A5AFB0 = { 0.0f, 0.0f, 0.0f };
static Vec3f D_80A5AFBC = { 0.0f, -1.0f, 0.0f };

void func_80A587A0(EnWaterEffect* this, Vec3f* arg1, u8 arg2) {
    s16 i;
    EnWaterEffectStruct* ptr = &this->unk_144[0];

    for (i = 0; i < ARRAY_COUNT(this->unk_144) / 2; i++, ptr++) {
        if (!ptr->unk_00) {
            ptr->unk_00 = true;
            ptr->unk_04 = *arg1;
            ptr->unk_10 = D_80A5AFB0;
            ptr->unk_1C = D_80A5AFB0;
            ptr->unk_2C.x = 0.1f;
            ptr->unk_2C.y = 0.0f;
            ptr->unk_2C.z = Rand_ZeroFloat(M_PI * 2);
            ptr->unk_01 = Rand_ZeroFloat(100.0f);
            ptr->unk_2A = arg2;
            break;
        }
    }
}

void func_80A58908(EnWaterEffect* this, Vec3f* arg1, Vec3f* arg2, u8 arg3) {
    Vec3f sp2C = D_80A5AFBC;
    EnWaterEffectStruct* ptr = &this->unk_144[0];
    s16 i;

    for (i = 0; i < ARRAY_COUNT(this->unk_144) / 2; i++, ptr++) {
        if (ptr->unk_00 == 0) {
            ptr->unk_00 = 2;
            ptr->unk_04 = *arg1;
            ptr->unk_10 = *arg2;
            ptr->unk_1C = sp2C;
            ptr->unk_2C.y = Rand_ZeroFloat(0.02f) + 0.02f;
            ptr->unk_2C.x = ptr->unk_2C.y;
            ptr->unk_2C.z = Rand_ZeroFloat(M_PI * 2);
            ptr->unk_01 = Rand_ZeroFloat(100.0f);
            ptr->unk_2A = arg3;
            break;
        }
    }
}

void EnWaterEffect_Init(Actor* thisx, PlayState* play) {
    s32 pad;
    EnWaterEffect* this = THIS;

    this->actor.flags &= ~ACTOR_FLAG_0;
    this->unk_DC4 = Rand_ZeroFloat(100.0f);

    if (this->actor.params == ENWATEREFFECT_TYPE_FALLING_ROCK_SPAWNER) {
        this->actor.update = func_80A59C04;
        this->actor.draw = func_80A5A184;
        this->unk_DC6 = Rand_ZeroFloat(100.0f) + 60.0f;
    } else if ((this->actor.params == ENWATEREFFECT_TYPE_GYORG_RIPPLES) ||
               (this->actor.params == ENWATEREFFECT_TYPE_GYORG_PRIMARY_SPRAY) ||
               (this->actor.params == ENWATEREFFECT_TYPE_GYORG_SECONDARY_SPRAY) ||
               (this->actor.params == ENWATEREFFECT_TYPE_GYORG_SHOCKWAVE)) {
        this->actor.update = func_80A5A534;
        this->actor.draw = func_80A5A6B8;
        this->actor.shape.rot.y = Rand_ZeroFloat(0x10000);
        Actor_SetScale(&this->actor, this->actor.shape.rot.z * 0.0002f);

        if (this->actor.params == ENWATEREFFECT_TYPE_GYORG_RIPPLES) {
            Actor_Spawn(&play->actorCtx, play, 0x0245, this->actor.world.pos.x, this->actor.world.pos.y,
                        this->actor.world.pos.z, 0, 0, this->actor.shape.rot.z, ENWATEREFFECT_TYPE_GYORG_PRIMARY_SPRAY);
            Actor_Spawn(&play->actorCtx, play, 0x0245, this->actor.world.pos.x, this->actor.world.pos.y,
                        this->actor.world.pos.z, 0, 0, this->actor.shape.rot.z,
                        ENWATEREFFECT_TYPE_GYORG_SECONDARY_SPRAY);
        } else if (this->actor.params == ENWATEREFFECT_TYPE_GYORG_PRIMARY_SPRAY) {
            this->unk_DC4 = -3;
        } else if (this->actor.params == ENWATEREFFECT_TYPE_GYORG_SECONDARY_SPRAY) {
            this->unk_DC4 = -6;
        } else if (this->actor.params == ENWATEREFFECT_TYPE_GYORG_SHOCKWAVE) {
            this->unk_DC4 = 23;
        }

        this->unk_E08[1] = 0.2f;
        this->unk_E18[1] = -0.017f;
        this->unk_E08[2] = 0.2f;
        this->unk_E18[2] = -0.018f;
        this->unk_E08[3] = 0.2f;
        this->unk_E18[3] = -0.019f;

        this->unk_DC8[1].y = 1.0f;
        this->unk_DC8[2].y = 1.0f;
        this->unk_DC8[3].y = 1.0f;

        this->unk_E2C = 255.0f;
        this->unk_E34 = 255.0f;
        this->unk_E30 = 200.0f;
        this->unk_E38 = 191.25f;
    }
}

void EnWaterEffect_Destroy(Actor* thisx, PlayState* play) {
}

void EnWaterEffect_Update(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    EnWaterEffect* this = THIS;
    Player* player = GET_PLAYER(play);
    EnWaterEffectStruct* ptr = &this->unk_144[0];
    s16 i;
    s16 j;
    s32 phi_v0;
    Vec3f spA4;
    Vec3f sp98;
    u8 phi_v1;
    Vec3f sp88;

    if (!Flags_GetSwitch(play, this->actor.params)) {
        this->unk_DC4++;
        if ((this->unk_DC4 % 32) == 0) {
            if (Rand_ZeroOne() < 0.5f) {
                Actor_UpdateBgCheckInfo(play, &this->actor, 20.0f, 10.0f, 40.0f, 4);
                sp88.x = Rand_CenteredFloat(50.0f) + this->actor.world.pos.x;
                sp88.y = this->actor.world.pos.y;
                sp88.z = Rand_CenteredFloat(50.0f) + this->actor.world.pos.z;
                if ((this->actor.world.pos.y + this->actor.yDistToWater) <= this->actor.floorHeight) {
                    phi_v1 = 0;
                } else {
                    phi_v1 = 1;
                    this->actor.floorHeight = this->actor.world.pos.y + this->actor.yDistToWater;
                }
                func_80A587A0(this, &sp88, phi_v1);
            }
        }
    }

    for (i = 0; i < ARRAY_COUNT(this->unk_144) / 2; i++, ptr++) {
        if (ptr->unk_00 != 0) {
            ptr->unk_01++;

            ptr->unk_04.x += ptr->unk_10.x;
            ptr->unk_04.y += ptr->unk_10.y;
            ptr->unk_04.z += ptr->unk_10.z;
            ptr->unk_10.y += ptr->unk_1C.y;

            Math_ApproachF(&ptr->unk_38, (ptr->unk_01 & 6) ? 80.0f : 200.0f, 1.0f, 80);

            if (ptr->unk_00 == 1) {
                ptr->unk_2C.z += 0.15f;
                Math_ApproachF(&ptr->unk_2C.x, 0.03f, 0.5f, 0.005f);
                Math_ApproachF(&ptr->unk_2C.y, 0.5f, 0.5f, 0.02f);

                if (ptr->unk_2C.y > 0.15f) {
                    ptr->unk_1C.y = -1.0f;
                    if (ptr->unk_10.y < -20.0f) {
                        ptr->unk_10.y = -20.0f;
                    }
                }

                if ((fabsf(ptr->unk_04.x - player->actor.world.pos.x) < 15.0f) &&
                    (fabsf(ptr->unk_04.z - player->actor.world.pos.z) < 15.0f) &&
                    (player->actor.world.pos.y < ptr->unk_04.y) &&
                    ((ptr->unk_04.y - player->actor.world.pos.y) < 40.0f)) {
                    phi_v0 = true;
                } else {
                    phi_v0 = false;
                }

                if ((ptr->unk_04.y <= this->actor.floorHeight) || phi_v0) {

                    if (phi_v0) {
                        ptr->unk_00 = 0;
                        SfxSource_PlaySfxAtFixedWorldPos(play, &ptr->unk_04, 30, NA_SE_EV_BOMB_DROP_WATER);
                    } else {
                        ptr->unk_04.y = this->actor.floorHeight;
                        if (ptr->unk_2A == 0) {
                            //SfxSource_PlaySfxAtFixedWorldPos(play, &ptr->unk_04, 30, NA_SE_EV_WATERDROP_GRD);
                            SoundEffect_PlayOneshot(SOUND_MM_WATER_DRIP_GROUND, 1.0f, 1.0f, &this->actor.world.pos, 1.0f, 1200.0f, &this->actor);
                            ptr->unk_00 = 3;
                            ptr->unk_2C.x = 0.1f;
                            ptr->unk_2C.y = 0.6f;
                            ptr->unk_10 = D_80A5AFB0;
                            ptr->unk_1C = D_80A5AFB0;
                            ptr->unk_3C = 200;
                            ptr->unk_28 = 9;
                            Math_Vec3f_Copy(&sp98, &ptr->unk_04);
                            sp98.y += 3.0f;
                            EffectSsGSplash_Spawn(play, &sp98, NULL, NULL, 1, 100);
                        } else {
                            ptr->unk_00 = 0;
                            //SfxSource_PlaySfxAtFixedWorldPos(play, &ptr->unk_04, 30, NA_SE_EV_WATERDROP);
                            SoundEffect_PlayOneshot(SOUND_MM_WATER_DRIP_WATER, 1.0f, 1.0f, &this->actor.world.pos, 1.0f, 1200.0f, &this->actor);
                            EffectSsGRipple_Spawn(play, &ptr->unk_04, 70, 500, 0);
                            EffectSsGRipple_Spawn(play, &ptr->unk_04, 70, 500, 10);
                            Math_Vec3f_Copy(&sp98, &ptr->unk_04);
                            sp98.y += 10.0f;
                            EffectSsGSplash_Spawn(play, &sp98, NULL, NULL, 1, 300);
                        }
                    }

                    for (j = 0; j < 12; j++) {
                        Matrix_RotateYF((2.0f * (j * M_PI)) / 5.5f, MTXMODE_NEW);
                        Matrix_MultVecZ(Rand_ZeroFloat(1.5f) + 1.5f, &spA4);
                        spA4.y = Rand_ZeroFloat(4.0f) + 2.0f;
                        func_80A58908(this, &ptr->unk_04, &spA4, ptr->unk_2A);
                    }
                }
            } else if (ptr->unk_00 == 2) {
                ptr->unk_2C.z += 0.15f;
                if (ptr->unk_10.y < -8.0f) {
                    ptr->unk_10.y = -8.0f;
                }

                if (ptr->unk_10.y < 0.0f) {
                    if (ptr->unk_04.y <= this->actor.floorHeight) {
                        ptr->unk_04.y = this->actor.floorHeight;
                        if (ptr->unk_2A == 0) {
                            ptr->unk_00 = 3;
                            ptr->unk_2C.x = 0.05f;
                            ptr->unk_2C.y = 0.2f;
                            ptr->unk_10 = D_80A5AFB0;
                            ptr->unk_1C = D_80A5AFB0;
                            ptr->unk_3C = 150;
                            ptr->unk_28 = Rand_ZeroFloat(5.0f) + 7.0f;
                        } else {
                            EffectSsGRipple_Spawn(play, &ptr->unk_04, 0, 200, 0);
                            ptr->unk_00 = 0;
                        }
                    }
                }
            } else if (ptr->unk_00 == 3) {
                Math_ApproachF(&ptr->unk_2C.x, ptr->unk_2C.y, 0.1f, 0.6f);
                ptr->unk_3C -= ptr->unk_28;
                if (ptr->unk_3C <= 0) {
                    ptr->unk_3C = 0;
                    ptr->unk_00 = 0;
                }
            }
        }
    }
}

void EnWaterEffect_Draw(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    GraphicsContext* gfxCtx = play->state.gfxCtx;
    EnWaterEffect* this = THIS;
    s32 pad;
    EnWaterEffectStruct* backupPtr = &this->unk_144[0];
    EnWaterEffectStruct* ptr = backupPtr;
    u8 phi_s4 = false;
    s16 i;

    OPEN_DISPS(gfxCtx, __FILE__, __LINE__);

    Gfx_SetupDL_25Xlu(play->state.gfxCtx);

    for (i = 0; i < ARRAY_COUNT(this->unk_144) / 2; i++, ptr++) {
        if ((ptr->unk_00 == 1) || (ptr->unk_00 == 2)) {
            if (!phi_s4) {
                POLY_XLU_DISP = Gfx_SetupDL(POLY_XLU_DISP, 0);

                gSPSegment(POLY_XLU_DISP++, 0x08, Lib_SegmentedToVirtual(gDust1Tex));
                gSPDisplayList(POLY_XLU_DISP++, 0x06004260);
                gDPSetEnvColor(POLY_XLU_DISP++, 250, 250, 255, 0);
                phi_s4++;
            }

            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, (u8)ptr->unk_38, (u8)(((void)0, ptr->unk_38) + 55.0f), 225, 150);

            Matrix_Translate(ptr->unk_04.x, ptr->unk_04.y, ptr->unk_04.z, MTXMODE_NEW);

            if (ptr->unk_00 == 1) {
                Matrix_RotateYS(Camera_GetInputDirYaw(GET_ACTIVE_CAM(play)), MTXMODE_APPLY);
            } else {
                Matrix_ReplaceRotation(&play->billboardMtxF);
            }

            Matrix_Scale(ptr->unk_2C.x, ptr->unk_2C.y, 1.0f, MTXMODE_APPLY);
            Matrix_RotateZF(ptr->unk_2C.z, MTXMODE_APPLY);

            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(gfxCtx, __FILE__, __LINE__), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, 0x060042b0);
        }
    }

    phi_s4 = false;
    ptr = backupPtr;

    for (i = 0; i < ARRAY_COUNT(this->unk_144) / 2; i++, ptr++) {
        if (ptr->unk_00 == 3) {
            if (!phi_s4) {
                Gfx_SetupDL_44Xlu(gfxCtx);

                gSPSegment(POLY_XLU_DISP++, 0x08, Lib_SegmentedToVirtual(gDust1Tex));
                gDPSetEnvColor(POLY_XLU_DISP++, 250, 250, 255, 0);
                gSPDisplayList(POLY_XLU_DISP++, 0x06004260);
                phi_s4++;
            }

            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, (u8)ptr->unk_38, (u8)(((void)0, ptr->unk_38) + 55.0f), 225,
                            ptr->unk_3C);

            Matrix_Translate(ptr->unk_04.x, ptr->unk_04.y, ptr->unk_04.z, MTXMODE_NEW);
            Matrix_Scale(ptr->unk_2C.x, 1.0f, ptr->unk_2C.x, MTXMODE_APPLY);
            Matrix_RotateYF(ptr->unk_2C.z, MTXMODE_APPLY);

            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(gfxCtx, __FILE__, __LINE__), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, 0x060042f8);
        }
    }

    CLOSE_DISPS(gfxCtx, __FILE__, __LINE__);
}

void func_80A599E8(EnWaterEffect* this, Vec3f* arg1, u8 arg2) {
    s16 i;
    EnWaterEffectStruct* ptr = &this->unk_144[0];

    for (i = 0; i < ARRAY_COUNT(this->unk_144); i++, ptr++) {
        if (ptr->unk_00 == 0) {
            ptr->unk_00 = 4;
            ptr->unk_04 = *arg1;

            ptr->unk_1C = D_80A5AFB0;
            ptr->unk_10 = D_80A5AFB0;

            if ((arg2 == 0) || (arg2 == 2)) {
                ptr->unk_1C.y = -1.0f;
            }

            if (arg2 >= 2) {
                if (arg2 == 2) {
                    ptr->unk_10.x = Rand_CenteredFloat(10.0f);
                    ptr->unk_10.y = Rand_ZeroFloat(3.0f) + 5.0f;
                    ptr->unk_10.z = Rand_CenteredFloat(10.0f);
                }
                ptr->unk_2C.z = 0.0017f;
                ptr->unk_2C.x = 0.003f;
                ptr->unk_2C.y = 0.0018f;
            } else {
                ptr->unk_2C.z = 0.003f;
                ptr->unk_2C.x = 0.003f;
                ptr->unk_2C.y = 0.003f;
            }
            ptr->unk_38 = 255.0f;
            ptr->unk_28 = Rand_ZeroFloat(0x10000);
            ptr->unk_3C = 255;
            ptr->unk_01 = Rand_ZeroFloat(200.0f);
            ptr->unk_2A = arg2;
            break;
        }
    }
}

void func_80A59C04(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    EnWaterEffect* this = THIS;
    s16 i;
    s16 j;
    f32 temp_f0_2;
    Player* player = GET_PLAYER(play);
    Vec3f sp90;
//    Actor* rotaryRoom = play->actorCtx.actorLists[ACTORCAT_BG].first;
    CollisionPoly* sp88;
    EnWaterEffectStruct* ptr = &this->unk_144[0];
    u8 phi_s5;
    Vec3f sp74;

//    while (rotaryRoom != NULL) {
//        if (rotaryRoom->id == ACTOR_BG_IKANA_ROTARYROOM) {
//            break;
//        }
//        rotaryRoom = rotaryRoom->next;
//    }

    if (this->unk_DC6 != 0) {
        this->unk_DC6--;
    }

//    if ((rotaryRoom != NULL) && Flags_GetSwitch(play, (rotaryRoom->params >> 1) & 0x7F)) {
//        this->unk_DC6 = Rand_ZeroFloat(150.0f) + 100.0f;
//    } else if (!Play_InCsMode(play)) {
//        this->unk_DC4++;
//        if (this->unk_DC6 == 0) {
//            this->unk_DC6 = Rand_ZeroFloat(150.0f) + 100.0f;
//            Actor_UpdateBgCheckInfo(play, &this->actor, 20.0f, 10.0f, 40.0f, 4);
//            sp74.x = Rand_CenteredFloat(50.0f) + this->actor.world.pos.x;
//            sp74.y = this->actor.world.pos.y;
//            sp74.z = Rand_CenteredFloat(50.0f) + this->actor.world.pos.z;
//            func_80A599E8(this, &sp74, 0);
//        }
//    }

    for (i = 0; i < ARRAY_COUNT(this->unk_144); i++, ptr++) {
        phi_s5 = false;

        if (ptr->unk_00 != 0) {
            ptr->unk_01++;

            ptr->unk_04.x += ptr->unk_10.x;
            ptr->unk_04.y += ptr->unk_10.y;
            ptr->unk_04.z += ptr->unk_10.z;
            ptr->unk_10.y = ptr->unk_10.y + ptr->unk_1C.y;

            if (ptr->unk_00 == 4) {
                if (ptr->unk_2A > 0) {
                    if (ptr->unk_2A == 1) {
                        Math_ApproachF(&ptr->unk_2C.x, 0.001f, 0.5f, 0.0002f);
                    }

                    if (ptr->unk_2A & 1) {
                        Math_ApproachZeroF(&ptr->unk_38, 1.0f, 30.0f);
                        if (ptr->unk_2A == 1) {
                            ptr->unk_3C -= 20;
                        } else {
                            ptr->unk_3C -= 10;
                        }
                        if (ptr->unk_3C <= 0) {
                            ptr->unk_00 = 0;
                        }
                        Math_ApproachZeroF(&ptr->unk_10.x, 1.0f, 1.0f);
                        Math_ApproachZeroF(&ptr->unk_10.z, 1.0f, 1.0f);
                    }
                }

                if ((ptr->unk_2A % 2) == 0) {
                    if (ptr->unk_2A == 0) {
                        func_80A599E8(this, &ptr->unk_04, 1);
                    } else if ((ptr->unk_01 & 1) == 0) {
                        func_80A599E8(this, &ptr->unk_04, 3);
                    }

                    if (ptr->unk_10.y < -20.0f) {
                        ptr->unk_10.y = -20.0f;
                    }

                    ptr->unk_28 += 0x800;
                    if ((fabsf(ptr->unk_04.x - player->actor.world.pos.x) < 20.0f) &&
                        (fabsf(ptr->unk_04.z - player->actor.world.pos.z) < 20.0f) &&
                        (fabsf(ptr->unk_04.y - (player->actor.world.pos.y + 25.0f)) < 30.0f)) {
                        phi_s5 = true;
                        //if (!player->isBurning) {
                        //    func_800B8D50(play, &this->actor, 2.0f, Rand_ZeroFloat(0x10000), 0.0f, 0x10);
                        //    for (j = 0; j < ARRAY_COUNT(player->flameTimers); j++) {
                        //        player->flameTimers[j] = Rand_S16Offset(0, 200);
                        //    }
                        //    player->isBurning = true;
                        //    //Player_PlaySfx(player, player->ageProperties->voiceSfxIdOffset + NA_SE_VO_LI_DEMO_DAMAGE);
                        //}
                    }

                    Math_Vec3f_Copy(&sp90, &ptr->unk_04);
                    sp90.y += 30.0f;

                    temp_f0_2 = BgCheck_EntityRaycastDown1(&play->colCtx, &sp88, &sp90);
                    if (fabsf(temp_f0_2 - this->actor.floorHeight) > 200.0f) {
                        phi_s5 = true;
                    }

                    if ((ptr->unk_04.y <= temp_f0_2) || (phi_s5 != 0)) {
                        if (phi_s5 == 0) {
                            ptr->unk_04.y = temp_f0_2;
                        }
                        ptr->unk_10.y = 0.0f;
                        ptr->unk_1C.y = 0.0f;
                        if (ptr->unk_2A == 0) {
                            for (j = 0; j < 5; j++) {
                                func_80A599E8(this, &ptr->unk_04, 2);
                            }
                            SfxSource_PlaySfxAtFixedWorldPos(play, &ptr->unk_04, 30, NA_SE_EV_PLANT_BROKEN);
                        }
                        ptr->unk_2A++;
                    }
                }
            }
        }
    }
}

void func_80A5A184(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    EnWaterEffect* this = THIS;
    GraphicsContext* gfxCtx = play->state.gfxCtx;
    EnWaterEffectStruct* ptr = &this->unk_144[0];
    u8 flag = false;
    s16 i;

    OPEN_DISPS(gfxCtx, __FILE__, __LINE__);

    Gfx_SetupDL_25Opa(play->state.gfxCtx);
    Gfx_SetupDL_25Xlu(play->state.gfxCtx);

    for (i = 0; i < ARRAY_COUNT(this->unk_144); i++, ptr++) {
        if (ptr->unk_00 == 4) {
            if (!flag) {
                gSPDisplayList(POLY_XLU_DISP++, 0x06004340);
                gDPSetEnvColor(POLY_XLU_DISP++, 255, 10, 0, 0);
                POLY_OPA_DISP = Gfx_SetFog(POLY_OPA_DISP, 255, 0, 0, 255, 500, 3600);
                flag++;
            }

            gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, (u8)ptr->unk_38, 0, 0, (u8)ptr->unk_3C);
            gSPSegment(POLY_XLU_DISP++, 0x08,
                       Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, 0, 0x20, 0x40, 1, 0, (ptr->unk_01 * -20) & 0x1FF,
                                        0x20, 0x80));

            Matrix_Translate(ptr->unk_04.x, ptr->unk_04.y, ptr->unk_04.z, MTXMODE_NEW);

            if (ptr->unk_2A >= 2) {
                Matrix_ReplaceRotation(&play->billboardMtxF);
            } else {
                Matrix_RotateYS(Camera_GetInputDirYaw(GET_ACTIVE_CAM(play)), MTXMODE_APPLY);
            }

            Matrix_Scale(ptr->unk_2C.x, ptr->unk_2C.y, 1.0f, MTXMODE_APPLY);

            if ((i & 1) != 0) {
                Matrix_RotateYF(M_PI, MTXMODE_APPLY);
            }

            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, 0x060043e8);

            if ((ptr->unk_2A & 1) == 0) {
                Matrix_Translate(ptr->unk_04.x, ptr->unk_04.y + 5.0f, ptr->unk_04.z, MTXMODE_NEW);
                Matrix_RotateXS(ptr->unk_28, MTXMODE_APPLY);
                Matrix_Scale(ptr->unk_2C.z, ptr->unk_2C.z, ptr->unk_2C.z, MTXMODE_APPLY);

                gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__),
                          G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                //gSPDisplayList(POLY_OPA_DISP++, gameplay_keep_DL_06AB30);
            }
        }
    }

    POLY_OPA_DISP = Play_SetFog(play, POLY_OPA_DISP);

    CLOSE_DISPS(gfxCtx, __FILE__, __LINE__);
}

void func_80A5A534(Actor* thisx, PlayState* play) {
    EnWaterEffect* this = THIS;
    s32 i;

    if (this->unk_E38 < 1.0f) {
        Actor_Kill(&this->actor);
    }

    this->unk_DC4++;

    if (this->unk_DC4 >= 0) {
        for (i = 1; i < 4; i++) {
            this->unk_DC8[i].z += this->unk_E08[i];
            this->unk_E08[i] += this->unk_E18[i];
            if (this->unk_DC8[i].z <= 0.0f) {
                this->unk_DC8[i].z = 0.0f;
                this->unk_E08[i] = 0.0f;
            }
        }

        Math_ApproachF(&this->unk_DC8[1].y, 20.0f, 1.0f, 0.025f);
        Math_ApproachF(&this->unk_DC8[2].y, 20.0f, 1.0f, 0.075f);
        Math_ApproachF(&this->unk_DC8[3].y, 20.0f, 1.0f, 0.1f);

        if (this->unk_DC4 >= 24) {
            Math_ApproachF(&this->unk_DC8[4].y, 2.0f, 0.1f, 0.2f);
            Math_ApproachZeroF(&this->unk_E2C, 1.0f, 26.0f);
            Math_ApproachZeroF(&this->unk_E30, 1.0f, 26.0f);
            Math_ApproachZeroF(&this->unk_E34, 1.0f, 24.0f);
            if (this->unk_DC4 >= 50) {
                Math_ApproachZeroF(&this->unk_E38, 1.0f, 3.75f);
            }
        }
    }
}

void func_80A5A6B8(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    EnWaterEffect* this = THIS;
    EnWaterEffectStruct* ptr = &this->unk_144[0];
    u8 phi_s4 = false;
    s16 i;

    OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

    Matrix_Translate(this->actor.world.pos.x, this->actor.world.pos.y, this->actor.world.pos.z, MTXMODE_NEW);
    Matrix_RotateYS(this->actor.shape.rot.y, MTXMODE_APPLY);
    Matrix_Scale(this->actor.scale.x, this->actor.scale.y, this->actor.scale.z, MTXMODE_APPLY);
    Gfx_SetupDL_25Xlu(play->state.gfxCtx);

    gDPSetEnvColor(POLY_XLU_DISP++, 165, 235, 255, 128);

    Matrix_Push();
    Matrix_Push();
    Matrix_Push();

    if ((this->actor.params == ENWATEREFFECT_TYPE_GYORG_RIPPLES) ||
        (this->actor.params == ENWATEREFFECT_TYPE_GYORG_PRIMARY_SPRAY)) {
        if (this->unk_E2C > 1.0f) {
            Gfx_SetupDL_25Xlu(play->state.gfxCtx);
            AnimatedMat_Draw(play, Lib_SegmentedToVirtual(0x06000de0));
            Matrix_Scale(this->unk_DC8[1].y, this->unk_DC8[1].z, this->unk_DC8[1].y, MTXMODE_APPLY);

            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, (u8)this->unk_E2C);
            gSPDisplayList(POLY_XLU_DISP++, 0x06000420);
        }

        Matrix_Pop();

        if (this->unk_E30 > 1.0f) {
            Gfx_SetupDL_25Xlu(play->state.gfxCtx);
            AnimatedMat_Draw(play, Lib_SegmentedToVirtual(0x06000e0c));
            Matrix_Scale(this->unk_DC8[2].y, this->unk_DC8[2].z, this->unk_DC8[2].y, MTXMODE_APPLY);

            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, (u8)this->unk_E30);
            gSPDisplayList(POLY_XLU_DISP++, 0x06000730);
        }
        Matrix_Pop();
    } else {
        Matrix_Pop();
        Matrix_Pop();
    }

    if ((this->unk_E34 > 1.0f) && (this->actor.params != ENWATEREFFECT_TYPE_GYORG_SHOCKWAVE)) {
        Gfx_SetupDL_25Xlu(play->state.gfxCtx);
        AnimatedMat_Draw(play, Lib_SegmentedToVirtual(0x06000e40));
        Matrix_Scale(this->unk_DC8[3].y, this->unk_DC8[3].z, this->unk_DC8[3].y, MTXMODE_APPLY);

        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, (u8)this->unk_E34);
        gSPDisplayList(POLY_XLU_DISP++, 0x06000a48);
    }

    Matrix_Pop();

    if ((this->actor.params == ENWATEREFFECT_TYPE_GYORG_RIPPLES) ||
        (this->actor.params == ENWATEREFFECT_TYPE_GYORG_SHOCKWAVE)) {
        Gfx_SetupDL_25Xlu(play->state.gfxCtx);
        AnimatedMat_Draw(play, Lib_SegmentedToVirtual(0x06000e58));
        Matrix_Scale(this->unk_DC8[4].y, this->unk_DC8[4].z, this->unk_DC8[4].y, MTXMODE_APPLY);

        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, (u8)this->unk_E38);
        gSPDisplayList(POLY_XLU_DISP++, 0x06000cd8);
    }

    if (this->actor.params == ENWATEREFFECT_TYPE_GYORG_RIPPLES) {
        Gfx_SetupDL_25Xlu(play->state.gfxCtx);

        for (i = 0; i < ARRAY_COUNT(this->unk_144) / 2; i++, ptr++) {
            if (ptr->unk_00 == 3) {
                if (!phi_s4) {
                    Gfx_SetupDL_44Xlu(play->state.gfxCtx);

                    gSPSegment(POLY_XLU_DISP++, 0x08, Lib_SegmentedToVirtual(gDust1Tex));
                    gDPSetEnvColor(POLY_XLU_DISP++, 250, 250, 255, 0);
                    gSPDisplayList(POLY_XLU_DISP++, 0x06004260);
                    phi_s4++;
                }

                gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, (u8)ptr->unk_38, (u8)(((void)0, ptr->unk_38) + 55.0f), 225,
                                ptr->unk_3C);

                Matrix_Translate(ptr->unk_04.x, ptr->unk_04.y, ptr->unk_04.z, MTXMODE_NEW);
                Matrix_Scale(ptr->unk_2C.x, 1.0f, ptr->unk_2C.x, MTXMODE_APPLY);
                Matrix_RotateYF(ptr->unk_2C.z, MTXMODE_APPLY);

                gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__),
                          G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPDisplayList(POLY_XLU_DISP++, 0x060042f8);
            }
        }
    }

    CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
}
