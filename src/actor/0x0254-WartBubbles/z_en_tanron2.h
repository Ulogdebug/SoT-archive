#ifndef Z_EN_TANRON2_H
#define Z_EN_TANRON2_H

#include "global.h"
//#include "mm/requirements.h"

struct EnTanron2;

#define ABS_ALT(x) ((x) < 0 ? -(x) : (x))
#define BINANG_SUB(a, b) ((s16)(a - b))

#define SWAP(type, a, b)    \
    {                       \
        type _temp = (a);   \
        (a) = (b);          \
        (b) = _temp;        \
    }                       \
    (void)0


#define CHECK_EVENTINF(flag) (gSaveContext.eventInf[(flag) >> 4] & (1 << ((flag) & 0xF)))
#define EVENTINF_60 0x60
#define CONTROLLER3(gameState) (&(gameState)->input[2])

MtxF* sCurrentMatrix; //!< original name: "Matrix_now"

typedef void (*EnTanron2ActionFunc)(struct EnTanron2*, PlayState*);

typedef struct EnTanron2 {
    /* 0x000 */ Actor actor;
    /* 0x144 */ EnTanron2ActionFunc actionFunc;
    /* 0x148 */ s16 unk_148;
    /* 0x14A */ s16 unk_14A;
    /* 0x14C */ s16 unk_14C;
    /* 0x14E */ s16 unk_14E;
    /* 0x150 */ s16 unk_150;
    /* 0x152 */ s16 unk_152;
    /* 0x154 */ s16 unk_154;
    /* 0x156 */ s16 unk_156;
    /* 0x158 */ u8 unk_158;
    /* 0x159 */ u8 unk_159;
    /* 0x15A */ u8 unk_15A;
    /* 0x15B */ u8 unk_15B;
    /* 0x15C */ f32 unk_15C;
    /* 0x160 */ f32 unk_160;
    /* 0x164 */ ColliderCylinder collider1;
    /* 0x1B0 */ ColliderCylinder collider2;
} EnTanron2; // size = 0x1FC


#endif // Z_EN_TANRON2_H
