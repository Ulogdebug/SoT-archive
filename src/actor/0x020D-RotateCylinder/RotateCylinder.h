#ifndef __ROTATE_CYLINDER_ACTOR_H__
#define __ROTATE_CYLINDER_ACTOR_H__

#include <uLib.h>

struct RotateCylinder;

typedef void (* RotateCylinderFunc)(struct RotateCylinder*, PlayState*);

typedef struct RotateCylinder {
    DynaPolyActor dyna;
    RotateCylinderFunc actionFunc;
    s16 switchFlag;
    u8 backHomeFlag;
} RotateCylinder;

#endif // __ROTATE_CYLINDER_ACTOR_H__