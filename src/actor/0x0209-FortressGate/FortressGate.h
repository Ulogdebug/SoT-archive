#ifndef __FORTRESS_GATE_ACTOR_H__
#define __FORTRESS_GATE_ACTOR_H__

#include <uLib.h>

struct FortressGate;

typedef void (* FortressGateFunc)(struct FortressGate*, PlayState*);

typedef struct FortressGate {
    union {
        Actor actor;
        DynaPolyActor dyna;
    };
    FortressGateFunc actionFunc;
    s16 switchFlag;
    s16 riseDis;
    u8 backHomeFlag;
} FortressGate;

#endif // __FORTRESS_GATE_ACTOR_H__