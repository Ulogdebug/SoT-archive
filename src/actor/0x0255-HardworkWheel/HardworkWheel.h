#ifndef __HARDWORK_WHEEL_H__
#define __HARDWORK_WHEEL_H__

#include <uLib.h>

struct HardworkWheel;

typedef void (* HardworkWheelFunc)(struct HardworkWheel*, PlayState*);

typedef struct HardworkWheel {
    DynaPolyActor dyna;
    Actor* blockRot;
    HardworkWheelFunc actionFunc;
    s32 accumulation;
    s16 switchFlags;
    s16 lastRotX;
    s16 turnCounter;
    s8  moving;
    s16 spawnBlock;
    s16 blockDie;
    s16 slackTimer;
    s16 shakeTimer;
} HardworkWheel;

#endif // __HARDWORK_WHEEL_H__