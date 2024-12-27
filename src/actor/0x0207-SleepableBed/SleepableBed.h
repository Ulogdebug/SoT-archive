#ifndef __SLEEPABLE_BED_ACTOR_H__
#define __SLEEPABLE_BED_ACTOR_H__

#include <uLib.h>

struct SleepableBed;

typedef void (* SleepableBedFunc)(struct SleepableBed*, PlayState*);

typedef struct SleepableBed {
    Actor actor;
    s16 cutsceneTimer;
    SleepableBedFunc actionFunc;
    s8 talkDecision;
} SleepableBed;

#endif // __SLEEPABLE_BED_ACTOR_H__