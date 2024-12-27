#ifndef __EN_PLANE_H__
#define __EN_PLANE_H__

#include <uLib.h>

struct EnPlane;

typedef enum {
    ROOM_SIDE_NONE = -1,
    ROOM_SIDE_FRONT,
    ROOM_SIDE_BACK,
    ROOM_SIDE_MAX
} RoomSide;

typedef void (* EnPlaneFunc)(struct EnPlane*, PlayState*, RoomSide side, bool cameraOnSameSide);

typedef struct EnPlane {
    Actor actor;
    EnPlaneFunc action;
    Vec3f corners[2];
    Vec3f checkPos;

    u8 roomIndices[ROOM_SIDE_MAX];

    f32 height;

    Vtx dbgVertices[32];
    Vtx planeVertices[4];
} EnPlane;

#endif // __EN_PLANE_H__