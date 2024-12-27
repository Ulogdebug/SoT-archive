#ifndef __OIL_BARREL_H__
#define __OIL_BARREL_H__

#include <uLib.h>

struct OilBarrel;

typedef void (* OilBarrelFunc)(struct OilBarrel*, PlayState*);

typedef struct OilBarrel {
    DynaPolyActor dyna;
    OilBarrelFunc actionFunc;
    ColliderCylinder hitCollider;
    s16 tempArg;
    s16 explodeTimer;
    s16 breakTimer;
} OilBarrel;

#endif // __OIL_BARREL_H__