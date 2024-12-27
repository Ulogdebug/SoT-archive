#ifndef __ROTATE_PLATFORM_ACTOR_H__
#define __ROTATE_PLATFORM_ACTOR_H__

#include <uLib.h>

struct RotatePlatform;

typedef void (* RotatePlatformFunc)(struct RotatePlatform*, PlayState*);

typedef enum {
    /* 0 */ CLOCKWISE,
    /* 1 */ COUNTERWISE,
            BACKHOME_CLOCKWISE,
            BACKHOME_COUNTERWISE,
            NONE
} direction;

typedef struct RotatePlatform {
    DynaPolyActor dyna;
    RotatePlatformFunc actionFunc;
    s16 ClockFlag;
    s16 CounterFlag;
    s16 targetDegree;
    s16 directionState;
} RotatePlatform;

#endif // __ROTATE_PLATFORM_ACTOR_H__