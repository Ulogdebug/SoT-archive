#ifndef Z_METAL_CRATE_H
#define Z_METAL_CRATE_H

#include <uLib.h>


struct MetalCrate;

typedef void (*MetalCrateActionFunc)(struct MetalCrate*, PlayState*);

typedef struct MetalCrate {
    union {
        Actor actor;
        DynaPolyActor dyna;
    };
    MetalCrateActionFunc actionFunc;
    ColliderCylinder collider;
    s16 collectibleFlag;
} MetalCrate;

#endif
