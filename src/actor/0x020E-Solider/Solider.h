#ifndef Z_SOLIDER_H
#define Z_SOLIDER_H
#include "ultra64.h"
#include "global.h"

struct Solider;

typedef void (*SoliderActionFunc)(struct Solider*, PlayState*);

typedef struct Solider {
    /* 0x0000 */ Actor actor;
    /* 0x014C */ SkelAnime skelAnime;
    /* 0x0190 */ Vec3s jointTable[18];
    /* 0x01F6 */ Vec3s morphTable[18];
    /* 0x025C */ SoliderActionFunc actionFunc;
    /* 0x0260 */ Vec3s headRot;
    /* 0x0266 */ Vec3s torsoRot;
    /* 0x026C */ Vec3f pos;
                Vec3f HeadPos;
    /* 0x0278 */ f32 height;
    /* 0x027C */ s16 unk_27C;
    /* 0x027E */ s16 unk_27E;
    /* 0x0280 */ s16 type;
    /* 0x0282 */ s16 unk_282;
    /* 0x0284 */ s16 unk_284;
    /* 0x0288 */ f32 unk_288;
    /* 0x028C */ struct_80034A14_arg1 unk_28C;
    /* 0x02B4 */ u8 unk_2B4;
                u8 loadStarted;
                 s16 cutsceneTimer;
    /* 0x02B6 */ char unk_2B6[7];
    /* 0x02BC */ ColliderCylinder collider;
} Solider; // size = 0x0308

#endif
