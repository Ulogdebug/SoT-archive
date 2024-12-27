#include "global.h"


typedef void* TexturePtr;



typedef struct {
    /* 0x0 */ s8 segment;
    /* 0x2 */ s16 type;
    /* 0x4 */ void* params;
} AnimatedMaterial; // size = 0x8

typedef struct {
    /* 0x0 */ u8 r;
    /* 0x1 */ u8 g;
    /* 0x2 */ u8 b;
    /* 0x3 */ u8 a;
    /* 0x4 */ u8 lodFrac;
} F3DPrimColor; // size = 0x5

typedef struct {
    /* 0x0 */ u8 r;
    /* 0x1 */ u8 g;
    /* 0x2 */ u8 b;
    /* 0x3 */ u8 a;
} F3DEnvColor; // size = 0x4

typedef struct {
    /* 0x0 */ u16 keyFrameLength;
    /* 0x2 */ u16 keyFrameCount;
    /* 0x4 */ F3DPrimColor* primColors;
    /* 0x8 */ F3DEnvColor* envColors;
    /* 0xC */ u16* keyFrames;
} AnimatedMatColorParams; // size = 0x10

typedef struct {
    /* 0x0 */ s8 xStep;
    /* 0x1 */ s8 yStep;
    /* 0x2 */ u8 width;
    /* 0x3 */ u8 height;
} AnimatedMatTexScrollParams; // size = 0x4

typedef struct {
    /* 0x0 */ u16 keyFrameLength;
    /* 0x4 */ TexturePtr* textureList;
    /* 0x8 */ u8* textureIndexList;
} AnimatedMatTexCycleParams; // size = 0xC

MtxF* sCurrentMatrix; //!< original name: "Matrix_now"



s32 sMatAnimStep;
u32 sMatAnimFlags;
f32 sMatAnimAlphaRatio;