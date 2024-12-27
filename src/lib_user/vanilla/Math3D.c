
#include <uLib.h>

static u8 __emptyFileForNow_Math3D = 0;

#if 0
// Made by Tharo

f32 fabsf(f32);

#define ALWAYS_INLINE __attribute__((always_inline)) inline
#define PURE          __attribute__((pure))

// Bitwise int to float
static ALWAYS_INLINE PURE f32 i2f(u32 i) {
    return *(f32*)&i;
}

// Bitwise float to int
static ALWAYS_INLINE PURE u32 f2i(f32 f) {
    return *(u32*)&f;
}

/**
 *  Get the sign of a 32-bit big-endian IEEE f32ing-point number.
 *  Returns 1.0f if positive, -1.0f if negative.
 */
static ALWAYS_INLINE PURE f32 sgn(f32 f) {
    u32 sgn_bit = f2i(-0.0f);
    u32 r1 = f2i(1.0f);
    u32 rf = f2i(f);
    
    return i2f(r1 | (rf & sgn_bit));
}

/**
 *  Check if `p` is inside the triangle with vertices (p0, p1, p2)
 */
PURE s32 Math3D_PointVsTri2D(f32 px, f32 py, f32 p0x, f32 p0y, f32 p1x, f32 p1y, f32 p2x, f32 p2y) {
    f32 dX = px - p2x;
    f32 dY = py - p2y;
    f32 dX21 = p2x - p1x;
    f32 dY21 = p2y - p1y;
    f32 dX02 = p0x - p2x;
    f32 dY02 = p0y - p2y;
    
    f32 D =          (dX21 * dY02 - dY21 * dX02);
    f32 s = sgn(D) * (dX21 * dY   - dY21 * dX  );
    f32 t = sgn(D) * (dX02 * dY   - dY02 * dX  );
    
    return s >= 0.0f && t >= 0.0f && s + t <= fabsf(D);
}

/**
 *  Check if point `p` is inside the circle centered at `c` with radius `radius`.
 */
PURE s32 Math3D_PointInCircle(f32 px, f32 py, f32 cx, f32 cy, f32 radius) {
    return SQ(px - cx) + SQ(py - cy) <= SQ(radius);
}

/**
 *  Check if point `p` is within `dst` from the line segment [`a`,`b`].
 *  If there is no line perpendicular to [`a`,`b`] that intersects `p`, fail the check.
 */
PURE s32 Math3D_PointNearLine2D(f32 px, f32 py, f32 ax, f32 ay, f32 bx, f32 by, f32 dst) {
    f32 dx = bx - ax;
    f32 dy = by - ay;
    
    f32 dxp = px - ax;
    f32 dyp = py - ay;
    
    f32 lenSq = SQ(dx) + SQ(dy);
    
    f32 lambda = dxp * dx + dyp * dy;
    f32 r = dxp * dy - dyp * dx;
    
    if (lambda < 0.0f || lambda > lenSq)
        return false;
    
    return SQ(r) <= SQ(dst) * lenSq;
}

PURE s32 Math3D_TriCheckPoint(f32 px, f32 py, f32 v1x, f32 v1y, f32 v2x, f32 v2y, f32 v3x, f32 v3y, f32 n, f32 dst) {
    return Math3D_PointVsTri2D(px, py, v1x, v1y, v2x, v2y, v3x, v3y) ||
           Math3D_PointInCircle(px, py, v1x, v1y, dst) ||
           Math3D_PointInCircle(px, py, v2x, v2y, dst) ||
           Math3D_PointInCircle(px, py, v3x, v3y, dst) ||
           (fabsf(n) > 0.5f && (
               Math3D_PointNearLine2D(px, py, v1x, v1y, v2x, v2y, dst) ||
               Math3D_PointNearLine2D(px, py, v2x, v2y, v3x, v3y, dst) ||
               Math3D_PointNearLine2D(px, py, v3x, v3y, v1x, v1y, dst)));
}

// These are existing sys_math3d functions, replace them

PURE s32 NewMath3D_TriChkPointParaYImpl(Vec3f* restrict v0, Vec3f* restrict v1, Vec3f* restrict v2, f32 z, f32 x, f32 detMax, f32 chkDist, f32 ny) {
    return Math3D_TriCheckPoint(x, z, v0->x, v0->z, v1->x, v1->z, v2->x, v2->z, ny, chkDist);
}

PURE s32 NewMath3D_TriChkPointParaXImpl(Vec3f* restrict v0, Vec3f* restrict v1, Vec3f* restrict v2, f32 y, f32 z, f32 detMax, f32 chkDist, f32 nx) {
    return Math3D_TriCheckPoint(y, z, v0->y, v0->z, v1->y, v1->z, v2->y, v2->z, nx, chkDist);
}

PURE s32 NewMath3D_TriChkPointParaZImpl(Vec3f* restrict v0, Vec3f* restrict v1, Vec3f* restrict v2, f32 x, f32 y, f32 detMax, f32 chkDist, f32 nz) {
    return Math3D_TriCheckPoint(x, y, v0->x, v0->y, v1->x, v1->y, v2->x, v2->y, nz, chkDist);
}

PURE f32 Math3D_DotProduct(Vec3f* restrict a, Vec3f* restrict b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

void Math3D_ProjectPointToPlane(Vec3f* restrict dst, Vec3f* restrict p, Plane* restrict plane) {
    f32 PdotN = Math3D_DotProduct(p, &plane->normal);
    f32 lambda = plane->originDist - PdotN;
    
    dst->x = p->x + lambda * plane->normal.x;
    dst->y = p->y + lambda * plane->normal.y;
    dst->z = p->z + lambda * plane->normal.z;
}

void Math3D_PlaneOrigin(Vec3f* restrict dst, Plane* restrict plane) {
    dst->x = plane->originDist * plane->normal.x;
    dst->y = plane->originDist * plane->normal.y;
    dst->z = plane->originDist * plane->normal.z;
}
#endif