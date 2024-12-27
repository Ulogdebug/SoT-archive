#ifndef __S_F_XMANAGER_ACTOR_H__
#define __S_F_XMANAGER_ACTOR_H__

#include <uLib.h>

struct SFXmanager;

typedef void (* SFXmanagerFunc)(struct SFXmanager*, PlayState*);

typedef struct SFXmanager {
    Actor actor;
    SFXmanagerFunc actionFunc;
    s16 switchFlags;
    s8 whichSFX;

} SFXmanager;

#endif // __S_F_XMANAGER_ACTOR_H__