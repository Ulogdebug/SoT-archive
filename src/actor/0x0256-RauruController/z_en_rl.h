#ifndef Z_EN_RL_H
#define Z_EN_RL_H

#include "ultra64.h"
#include "global.h"

struct EnRl;

typedef void (*EnRlActionFunc)(struct EnRl*, PlayState*);

typedef struct EnRl {
    /* 0x0000 */ Actor actor;
    /* 0x025C */ EnRlActionFunc actionFunc;
                 u8 flip;
                 u8 type;
                 u16 flags;
} EnRl; // size = 0x01AC

#endif
