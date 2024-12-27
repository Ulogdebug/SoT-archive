#ifndef __SUPER_METAL_GATE_H__
#define __SUPER_METAL_GATE_H__

#include <uLib.h>

struct SuperMetalGate;

typedef void (* SuperMetalGateFunc)(struct SuperMetalGate*, PlayState*);

typedef struct SuperMetalGate {
    union {
        Actor actor;
        DynaPolyActor dyna;
    };
    SuperMetalGateFunc actionFunc;
    s16 state;
} SuperMetalGate;

#endif // __SUPER_METAL_GATE_H__