#ifndef __SWITCH_TIMER_H__
#define __SWITCH_TIMER_H__

#include <uLib.h>

struct SwitchTimer;

typedef void (* SwitchTimerFunc)(struct SwitchTimer*, PlayState*);

typedef struct SwitchTimer {
    Actor actor;
    SwitchTimerFunc actionFunc;
    s16 timer;
    s16 switchFlag;
    u8 reverse;
    s16 preTimerDuration;
} SwitchTimer;

#endif // __SWITCH_TIMER_H__