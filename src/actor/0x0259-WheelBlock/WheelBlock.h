#ifndef __WHEEL_BLOCK_H__
#define __WHEEL_BLOCK_H__

#include <uLib.h>

struct WheelBlock;

typedef void (* WheelBlockFunc)(struct WheelBlock*, PlayState*);

typedef struct WheelBlock {
   DynaPolyActor dyna;
   s16 switchFlags;
   s16 timer;
    WheelBlockFunc actionFunc;
    Vec3s randFactor;
    Vec3f fragmentPos;
} WheelBlock;

#endif // __WHEEL_BLOCK_H__