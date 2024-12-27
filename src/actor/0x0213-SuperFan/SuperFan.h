#ifndef __SUPER_FAN_ACTOR_H__
#define __SUPER_FAN_ACTOR_H__

#include <uLib.h>

struct SuperFan;

typedef void (* SuperFanFunc)(struct SuperFan*, PlayState*);

typedef struct SuperFan {
    DynaPolyActor dyna;
    Actor* blowActor;
    SuperFanFunc actionFunc;
    s16 switchFlag;
    u8 backHomeFlag;
    s16 timer;
    u8 reverse;
} SuperFan;

#endif // __SUPER_FAN_ACTOR_H__