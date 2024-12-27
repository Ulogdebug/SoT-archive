#ifndef __CS_GANON_H__
#define __CS_GANON_H__

#include <uLib.h>
#include "object/0x0287-CsGanonObj.h"

struct CsGanon;

typedef void (* CsGanonFunc)(struct CsGanon*, PlayState*);

typedef struct CsGanon {
    Actor actor;
    SkelAnime skelAnime;
    Vec3s jointTable[CSGANONOBJ_LIMB_MAX];
    Vec3s morphTable[CSGANONOBJ_LIMB_MAX];
    s16 switchFlag;
    s8 audioFlag;
    CsGanonFunc actionFunc;
} CsGanon;

#endif // __CS_GANON_H__