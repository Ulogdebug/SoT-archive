#ifndef __TOWN_GATE_ACTOR_H__
#define __TOWN_GATE_ACTOR_H__

#include <uLib.h>

struct TownGate;

typedef void (* TownGateFunc)(struct TownGate*, PlayState*);

typedef struct TownGate {
    DynaPolyActor dyna;
    TownGateFunc actionFunc;
    s16 switchFlag;
    s16 riseDis;
    u8 backHomeFlag;
} TownGate;

#endif // __TOWN_GATE_ACTOR_H__