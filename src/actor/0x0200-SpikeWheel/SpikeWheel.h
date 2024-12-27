#ifndef __SPIKE_WHEEL_ACTOR_H__
#define __SPIKE_WHEEL_ACTOR_H__

#include <uLib.h>

struct SpikeWheel;

typedef void (* SpikeWheelFunc)(struct SpikeWheel*, PlayState*);

typedef struct SpikeWheel {
    DynaPolyActor dyna;
} SpikeWheel;

#endif // __SPIKE_WHEEL_ACTOR_H__