#ifndef __TEMPLE_BELL_ACTOR_H__
#define __TEMPLE_BELL_ACTOR_H__

#include <uLib.h>

struct TempleBell;

typedef void (* TempleBellFunc)(struct TempleBell*, PlayState*);

typedef struct TempleBell {
    union {
        Actor actor;
        DynaPolyActor dyna;
    };
    TempleBellFunc actionFunc;
    ColliderCylinder collider;
    s16 switchFlags;
    Vec3s randFactor;
    s16 momentum;
    s16 timer;
} TempleBell;

#endif // __TEMPLE_BELL_ACTOR_H__