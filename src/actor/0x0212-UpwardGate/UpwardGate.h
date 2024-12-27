#ifndef __UPWARD_GATE_ACTOR_H__
#define __UPWARD_GATE_ACTOR_H__

#include <uLib.h>

struct UpwardGate;

typedef void (* UpwardGateFunc)(struct UpwardGate*, PlayState*);

typedef struct UpwardGate {
    DynaPolyActor dyna;
    UpwardGateFunc actionFunc;
    s8 reverse;
    s16 backHomeFlag;
    s16 switchFlag;
} UpwardGate;

#endif // __UPWARD_GATE_ACTOR_H__