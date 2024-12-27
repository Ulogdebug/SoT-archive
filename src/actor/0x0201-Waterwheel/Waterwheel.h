#ifndef __WATERWHEEL_ACTOR_H__
#define __WATERWHEEL_ACTOR_H__

#include <uLib.h>

struct Waterwheel;

typedef void (* WaterwheelFunc)(struct Waterwheel*, PlayState*);
typedef void (* WaterwheelFuncWithPara)(struct Waterwheel*, PlayState*,s16);

typedef enum {
    /* 0 */ WATER_WHEEL,
    /* 1 */ REVERSE_WATER_WHEEL
} wheelType;

typedef struct Waterwheel {
    DynaPolyActor dyna;
    WaterwheelFunc actionFunc;
    s16 spinSpeed;
    s16 switchFlag;
    s16 type;
} Waterwheel;



#endif // __WATERWHEEL_ACTOR_H__