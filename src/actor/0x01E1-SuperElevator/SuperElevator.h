#ifndef __SUPER_ELEVATOR_ACTOR_H__
#define __SUPER_ELEVATOR_ACTOR_H__

#include <uLib.h>

struct SuperElevator;

typedef void (* SuperElevatorFunc)(struct SuperElevator*, PlayState*);

typedef struct SuperElevator {
    DynaPolyActor dyna;
    SuperElevatorFunc actionFunc;
    s16 switchFlag;
    s16 waitTimer;
    s16 moveDis;
    u8 backHomeFlag;
} SuperElevator;

#endif // __SUPER_ELEVATOR_ACTOR_H__