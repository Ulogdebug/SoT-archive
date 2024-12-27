#include "ultra64.h"
#include "z64.h"
#include "macros.h"
#include "ovl_En_Sth.h"
static u64 sNoseTex[16];
static u64 sHair1Tex[32];
static u64 sEar1Tex[16];
static u64 sEyeTex[256];
static u64 sMouthTex[128];
static u64 sEar2Tex[16];
static u64 sHair2Tex[32];
static Vtx ovl_En_SthVtx_001E70[202];
static Gfx D_80B0A050[110];
static Gfx D_80B0A3C0[51];
static s16 sChildDanceAnimFrameData[724];
static JointIndex sChildDanceAnimJointIndices[16];
static AnimationHeader sChildDanceAnim;
static s16 sParentDanceAnimFrameData[1042];
static JointIndex sParentDanceAnimJointIndices[16];
static AnimationHeader sParentDanceAnim;

static u64 sNoseTex[16] = {
#include "oot/ovl_En_Sth/nose.rgba16.inc.c"
};

static u64 sHair1Tex[32] = {
#include "oot/ovl_En_Sth/hair_1.rgba16.inc.c"
};

static u64 sEar1Tex[16] = {
#include "oot/ovl_En_Sth/ear_1.rgba16.inc.c"
};

static u64 sEyeTex[256] = {
#include "oot/ovl_En_Sth/eye.rgba16.inc.c"
};

static u64 sMouthTex[128] = {
#include "oot/ovl_En_Sth/mouth.rgba16.inc.c"
};

static u64 sEar2Tex[16] = {
#include "oot/ovl_En_Sth/ear_2.rgba16.inc.c"
};

static u64 sHair2Tex[32] = {
#include "oot/ovl_En_Sth/hair_2.rgba16.inc.c"
};

static Vtx ovl_En_SthVtx_001E70[202] = {
#include "oot/ovl_En_Sth/ovl_En_SthVtx_001E70.vtx.inc"
};

static Gfx D_80B0A050[110] = {
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPLoadTextureBlock(sNoseTex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 8, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR |
                         G_TX_CLAMP, 3, 3, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetTileSize(G_TX_RENDERTILE, 0, 0, 0x001C, 0x001C),
    gsDPSetPrimColor(0, 0x80, 255, 255, 255, 255),
    gsSPSetGeometryMode(G_LIGHTING),
    gsSPSetGeometryMode(G_CULL_BACK),
    gsSPClearGeometryMode(G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPVertex(ovl_En_SthVtx_001E70, 30, 0),
    gsSP2Triangles(0, 1, 2, 0, 2, 3, 4, 0),
    gsSP2Triangles(5, 6, 7, 0, 8, 9, 10, 0),
    gsSP2Triangles(11, 12, 13, 0, 14, 15, 16, 0),
    gsSP2Triangles(17, 18, 19, 0, 20, 21, 22, 0),
    gsSP2Triangles(16, 15, 23, 0, 15, 24, 23, 0),
    gsSP2Triangles(24, 15, 25, 0, 14, 25, 15, 0),
    gsSP2Triangles(26, 27, 28, 0, 29, 9, 8, 0),
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPLoadTextureBlock(sHair1Tex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 16, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR |
                         G_TX_CLAMP, 3, 4, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetTileSize(G_TX_RENDERTILE, 0, 0, 0x001C, 0x003C),
    gsSPVertex(&ovl_En_SthVtx_001E70[30], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 0, 2, 0),
    gsSP2Triangles(4, 5, 6, 0, 4, 7, 5, 0),
    gsSP2Triangles(8, 9, 10, 0, 11, 12, 13, 0),
    gsSP2Triangles(14, 15, 16, 0, 16, 17, 14, 0),
    gsSP2Triangles(18, 19, 20, 0, 21, 22, 18, 0),
    gsSP2Triangles(18, 23, 21, 0, 24, 25, 14, 0),
    gsSP2Triangles(19, 26, 20, 0, 24, 27, 25, 0),
    gsSP2Triangles(20, 26, 28, 0, 28, 27, 24, 0),
    gsSP1Triangle(29, 30, 31, 0),
    gsSPVertex(&ovl_En_SthVtx_001E70[62], 29, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(4, 6, 5, 0, 7, 8, 9, 0),
    gsSP2Triangles(7, 10, 8, 0, 11, 12, 13, 0),
    gsSP2Triangles(14, 15, 13, 0, 15, 11, 13, 0),
    gsSP2Triangles(16, 12, 17, 0, 17, 12, 11, 0),
    gsSP2Triangles(18, 11, 15, 0, 18, 17, 11, 0),
    gsSP2Triangles(19, 16, 17, 0, 20, 19, 18, 0),
    gsSP2Triangles(19, 17, 18, 0, 16, 19, 21, 0),
    gsSP2Triangles(19, 20, 21, 0, 20, 22, 21, 0),
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPLoadTextureBlock(sEar1Tex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 8, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR |
                         G_TX_CLAMP, 3, 3, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetTileSize(G_TX_RENDERTILE, 0, 0, 0x001C, 0x001C),
    gsSP2Triangles(23, 24, 25, 0, 26, 27, 28, 0),
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPLoadTextureBlock(sEyeTex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR |
                         G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetTileSize(G_TX_RENDERTILE, 0, 0, 0x007C, 0x007C),
    gsSPVertex(&ovl_En_SthVtx_001E70[91], 30, 0),
    gsSP2Triangles(0, 1, 2, 0, 2, 3, 4, 0),
    gsSP2Triangles(5, 6, 7, 0, 8, 6, 5, 0),
    gsSP2Triangles(9, 1, 10, 0, 11, 9, 10, 0),
    gsSP2Triangles(12, 13, 14, 0, 14, 13, 15, 0),
    gsSP2Triangles(10, 1, 16, 0, 16, 1, 17, 0),
    gsSP2Triangles(17, 1, 0, 0, 2, 1, 3, 0),
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPLoadTextureBlock(sMouthTex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 16, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR |
                         G_TX_CLAMP, 5, 4, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetTileSize(G_TX_RENDERTILE, 0, 0, 0x007C, 0x003C),
    gsSP2Triangles(18, 19, 20, 0, 19, 21, 20, 0),
    gsSP2Triangles(20, 22, 18, 0, 22, 23, 18, 0),
    gsSP2Triangles(22, 24, 23, 0, 18, 23, 25, 0),
    gsSP2Triangles(23, 24, 25, 0, 26, 19, 18, 0),
    gsSP1Triangle(26, 21, 19, 0),
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPLoadTextureBlock(sEar2Tex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 8, 0, G_TX_MIRROR | G_TX_WRAP, G_TX_NOMIRROR |
                         G_TX_CLAMP, 3, 3, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetTileSize(G_TX_RENDERTILE, 0, 0, 0x001C, 0x001C),
    gsSP1Triangle(27, 28, 29, 0),
    gsSPEndDisplayList(),
};

static Gfx D_80B0A3C0[51] = {
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPLoadTextureBlock(sHair2Tex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 16, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR |
                         G_TX_WRAP, 3, 4, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetTileSize(G_TX_RENDERTILE, 0, 0, 0x001C, 0x003C),
    gsDPSetPrimColor(0, 0x80, 255, 255, 255, 255),
    gsSPSetGeometryMode(G_LIGHTING),
    gsSPSetGeometryMode(G_CULL_BACK),
    gsSPClearGeometryMode(G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPVertex(&ovl_En_SthVtx_001E70[121], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(6, 7, 8, 0, 9, 7, 6, 0),
    gsSP2Triangles(10, 11, 3, 0, 12, 13, 14, 0),
    gsSP2Triangles(15, 12, 14, 0, 16, 17, 18, 0),
    gsSP2Triangles(16, 18, 19, 0, 20, 21, 22, 0),
    gsSP2Triangles(23, 24, 25, 0, 25, 24, 26, 0),
    gsSP2Triangles(27, 28, 29, 0, 30, 21, 20, 0),
    gsSPVertex(&ovl_En_SthVtx_001E70[152], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(5, 4, 6, 0, 1, 7, 2, 0),
    gsSP2Triangles(8, 9, 10, 0, 11, 8, 10, 0),
    gsSP2Triangles(12, 10, 13, 0, 13, 14, 12, 0),
    gsSP2Triangles(15, 16, 11, 0, 16, 8, 11, 0),
    gsSP2Triangles(16, 17, 8, 0, 10, 18, 19, 0),
    gsSP2Triangles(20, 14, 13, 0, 10, 21, 13, 0),
    gsSP2Triangles(19, 21, 10, 0, 22, 23, 24, 0),
    gsSP2Triangles(24, 23, 25, 0, 17, 25, 26, 0),
    gsSP2Triangles(25, 23, 26, 0, 25, 17, 16, 0),
    gsSP2Triangles(16, 15, 25, 0, 10, 9, 18, 0),
    gsSP2Triangles(27, 18, 28, 0, 18, 9, 28, 0),
    gsSP2Triangles(19, 27, 29, 0, 18, 27, 19, 0),
    gsSP2Triangles(22, 24, 30, 0, 31, 19, 29, 0),
    gsSPVertex(&ovl_En_SthVtx_001E70[184], 18, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(6, 5, 4, 0, 7, 1, 0, 0),
    gsSP2Triangles(8, 9, 10, 0, 9, 11, 10, 0),
    gsSP2Triangles(9, 12, 13, 0, 9, 8, 12, 0),
    gsSP2Triangles(8, 10, 14, 0, 10, 11, 14, 0),
    gsSP2Triangles(15, 12, 8, 0, 15, 13, 12, 0),
    gsSP2Triangles(14, 16, 8, 0, 8, 17, 15, 0),
    gsSP2Triangles(5, 6, 14, 0, 16, 14, 6, 0),
    gsSP2Triangles(7, 15, 17, 0, 15, 7, 0, 0),
    gsSPEndDisplayList(),
};

static s16 sChildDanceAnimFrameData[724] = {
    0x0000, 0x08FF, 0xC000, 0x08FF, 0x09DD, 0x0B7C, 0x0C62, 0x0B95, 0x0A10, 0x094C, 0x09E6, 0x0B1E, 0x0C60, 0x0D1C, 
    0x0D53, 0x0D6D, 0x0D6E, 0x0D5C, 0x0D3A, 0x0D0C, 0x0CD7, 0x0CA0, 0x0C5C, 0x0C05, 0x0B9E, 0x0B2F, 0x0ABC, 0x0A4B, 
    0x09E3, 0x0988, 0x093F, 0x0910, 0x08FF, 0x0000, 0x002F, 0x0087, 0x00B7, 0x0087, 0x002F, 0x0000, 0x001D, 0x005B, 
    0x009A, 0x00B7, 0x00B0, 0x009E, 0x0084, 0x0065, 0x0045, 0x0027, 0x000F, 0x0000, 0xFFF8, 0xFFF3, 0xFFF1, 0xFFF0, 
    0xFFF2, 0xFFF4, 0xFFF7, 0xFFFA, 0xFFFD, 0xFFFF, 0x0000, 0x0C81, 0x0506, 0xF8FC, 0xF308, 0xF854, 0x0374, 0x09DD, 
    0x04CE, 0xFB9E, 0xF32F, 0xEE40, 0xEC9C, 0xEBBE, 0xEBC7, 0xEC8C, 0xEDBB, 0xEF1E, 0xF095, 0xF20A, 0xF3C1, 0xF5F6, 
    0xF891, 0xFB7D, 0xFE9F, 0x01DA, 0x0504, 0x07E9, 0x0A4C, 0x0BEA, 0x0C81, 0xDD2B, 0xDE1C, 0xE1B2, 0xE4E4, 0xE200, 
    0xDE75, 0xDD7B, 0xDE3A, 0xE0B3, 0xE4D8, 0xE8AC, 0xEA53, 0xEB50, 0xEB53, 0xEA8A, 0xE966, 0xE82F, 0xE706, 0xE5F8, 
    0xE4D8, 0xE391, 0xE245, 0xE10D, 0xDFF7, 0xDF0D, 0xDE54, 0xDDCB, 0xDD70, 0xDD3C, 0xDD2B, 0xCC69, 0xD5FA, 0xE662, 
    0xEF82, 0xE741, 0xD7CF, 0xCF65, 0xD5D7, 0xE233, 0xEED7, 0xF75C, 0xFA5C, 0xFBDB, 0xFB74, 0xF97F, 0xF6CB, 0xF3DD, 
    0xF0FE, 0xEE5E, 0xEB8E, 0xE82C, 0xE470, 0xE07F, 0xDC7C, 0xD889, 0xD4D0, 0xD181, 0xCED6, 0xCD0E, 0xCC69, 0x471F, 
    0x4037, 0x303A, 0x2450, 0x2F13, 0x3E80, 0x44D4, 0x3FED, 0x345D, 0x248D, 0x16E4, 0x110F, 0x0D94, 0x0D8D, 0x1062, 
    0x147C, 0x18DD, 0x1D16, 0x20EF, 0x2515, 0x29DF, 0x2EDA, 0x33BB, 0x3850, 0x3C74, 0x400B, 0x42FE, 0x4536, 0x469F, 
    0x471F, 0xE9BF, 0xEABE, 0xECA4, 0xEDD2, 0xED52, 0xEC19, 0xEB60, 0xEBE3, 0xECE6, 0xED69, 0xED52, 0xED32, 0xED09, 
    0xECD9, 0xECA4, 0xEC69, 0xEC2B, 0xEBEB, 0xEBA9, 0xEB67, 0xEB25, 0xEAE6, 0xEAAA, 0xEA73, 0xEA40, 0xEA15, 0xE9F1, 
    0xE9D6, 0xE9C5, 0xE9BF, 0xFD3D, 0xFB20, 0xF869, 0xF761, 0xF84E, 0xFAC2, 0xFC81, 0xFB22, 0xF903, 0xF77C, 0xF6D7, 
    0xF6B8, 0xF6B4, 0xF6C6, 0xF6E6, 0xF712, 0xF746, 0xF77D, 0xF7B4, 0xF7F5, 0xF84D, 0xF8BD, 0xF944, 0xF9E1, 0xFA91, 
    0xFB4B, 0xFC01, 0xFCA1, 0xFD12, 0xFD3D, 0xB14B, 0xB21B, 0xB631, 0xBA68, 0xB6A1, 0xB293, 0xB1BB, 0xB275, 0xB53A, 
    0xBA9F, 0xC043, 0xC2FF, 0xC4CD, 0xC514, 0xC413, 0xC288, 0xC0E3, 0xBF4F, 0xBDD8, 0xBC3B, 0xBA63, 0xB88A, 0xB6D0, 
    0xB549, 0xB401, 0xB2FC, 0xB239, 0xB1B3, 0xB165, 0xB14B, 0xF374, 0xFAF2, 0x0701, 0x0CF6, 0x07A9, 0xFC85, 0xF61A, 
    0xFB2B, 0x045E, 0x0CCF, 0x11C0, 0x1366, 0x1443, 0x143B, 0x1375, 0x1245, 0x10E2, 0x0F6B, 0x0DF5, 0x0C3D, 0x0A07, 
    0x076B, 0x047F, 0x015C, 0xFE20, 0xFAF5, 0xF80E, 0xF5AB, 0xF40C, 0xF374, 0x22D8, 0x21E7, 0x1E52, 0x1B1F, 0x1E04, 
    0x218F, 0x2288, 0x21C9, 0x1F51, 0x1B2B, 0x1756, 0x15AE, 0x14B2, 0x14AE, 0x1578, 0x169D, 0x17D4, 0x18FD, 0x1A0B, 
    0x1B2B, 0x1C73, 0x1DBE, 0x1EF7, 0x200D, 0x20F6, 0x21B0, 0x2238, 0x2293, 0x22C7, 0x22D8, 0xCC5E, 0xD5F3, 0xE660, 
    0xEF82, 0xE73E, 0xD7C8, 0xCF5B, 0xD5CF, 0xE230, 0xEED6, 0xF75E, 0xFA5E, 0xFBDE, 0xFB77, 0xF981, 0xF6CC, 0xF3DE, 
    0xF0FE, 0xEE5E, 0xEB8C, 0xE82A, 0xE46C, 0xE07B, 0xDC77, 0xD883, 0xD4C8, 0xD178, 0xCECC, 0xCD04, 0xCC5E, 0x4721, 
    0x4039, 0x303B, 0x2450, 0x2F13, 0x3E82, 0x44D7, 0x3FEF, 0x345E, 0x248D, 0x16E2, 0x110B, 0x0D8E, 0x0D87, 0x105E, 
    0x1479, 0x18DB, 0x1D14, 0x20EF, 0x2515, 0x29DF, 0x2EDB, 0x33BD, 0x3852, 0x3C76, 0x400D, 0x4300, 0x4539, 0x46A2, 
    0x4721, 0x1525, 0x1449, 0x12B3, 0x11E3, 0x12DA, 0x1496, 0x158D, 0x14BE, 0x1329, 0x124B, 0x124C, 0x1258, 0x126D, 
    0x128B, 0x12B0, 0x12DB, 0x130B, 0x133F, 0x1375, 0x13AD, 0x13E4, 0x141B, 0x1450, 0x1482, 0x14AF, 0x14D6, 0x14F7, 
    0x1510, 0x1520, 0x1525, 0x0133, 0x0349, 0x05F3, 0x06F4, 0x060D, 0x03A4, 0x01EB, 0x0345, 0x055A, 0x06D7, 0x0777, 
    0x0794, 0x0795, 0x0782, 0x0760, 0x0733, 0x06FF, 0x06C8, 0x0692, 0x0653, 0x05FD, 0x0591, 0x050E, 0x0476, 0x03CC, 
    0x0318, 0x0266, 0x01CB, 0x015D, 0x0133, 0xAFF7, 0xB0CE, 0xB4F3, 0xB933, 0xB565, 0xB14A, 0xB06C, 0xB12C, 0xB3FC, 
    0xB96E, 0xBF1A, 0xC1DA, 0xC3AC, 0xC3F6, 0xC2F7, 0xC16D, 0xBFC8, 0xBE34, 0xBCBB, 0xBB1B, 0xB940, 0xB761, 0xB5A1, 
    0xB413, 0xB2C4, 0xB1B8, 0xB0EE, 0xB064, 0xB012, 0xAFF7, 0x85B4, 0x82A4, 0x7CF4, 0x79E3, 0x7CE3, 0x8283, 0x85B4, 
    0x858E, 0x84AE, 0x834F, 0x81AC, 0x8000, 0x7E86, 0x7D78, 0x7D11, 0x7D2D, 0x7D7C, 0x7DF7, 0x7E94, 0x7F4E, 0x801B, 
    0x80F4, 0x81D1, 0x82AA, 0x8377, 0x8430, 0x84CE, 0x8549, 0x8598, 0x85B4, 0x0000, 0x0000, 0x8000, 0x8000, 0x8000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 
    0x8000, 0x8000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xE4AB, 0xD131, 0xD321, 
    0xE71D, 0xD4D8, 0xCDC2, 0xE021, 0xD7CC, 0xC4D1, 0xD19B, 0xE444, 0xEBF5, 0xEB10, 0xE8E1, 0xE593, 0xE14E, 0xDC3C, 
    0xD687, 0xD058, 0xC9D9, 0xC333, 0xC370, 0xC9E7, 0xD008, 0xD5A9, 0xDAA1, 0xDEC7, 0xE1F1, 0xE3F6, 0xE4AB, 0x8000, 
    0x8000, 0x0000, 0x0000, 0x0000, 0x8000, 0x8000, 0x8000, 0x8000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 
    0x8000, 0x0000, 0x0000, 0x8000, 0x8000, 0x8000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8000, 0x8000, 0x8000, 0x8000, 
    0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x1B55, 0x2ECF, 0x2CDF, 0x18E3, 0x2B28, 0x323E, 0x1FDF, 0x2834, 0x3B2F, 0x2E65, 0x1BBC, 
    0x140B, 0x14F0, 0x171F, 0x1A6D, 0x1EB2, 0x23C4, 0x2979, 0x2FA8, 0x3627, 0x3CCD, 0x3C90, 0x3619, 0x2FF8, 0x2A57, 
    0x255F, 0x2139, 0x1E0F, 0x1C0A, 0x1B55, 0x8000, 0x8000, 0x0000, 0x0000, 0x0000, 0x8000, 0x8000, 0x8000, 0x8000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8000, 0x8000, 
    0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x0000, 0x0289, 0x073E, 0x09C7, 0x089B, 0x05D5, 0x0297, 
    0x0000, 0xFE01, 0xFBE4, 0xF9D2, 0xF7F1, 0xF669, 0xF561, 0xF500, 0xF524, 0xF589, 0xF625, 0xF6EE, 0xF7DA, 0xF8DF, 
    0xF9F3, 0xFB0D, 0xFC21, 0xFD26, 0xFE12, 0xFEDB, 0xFF77, 0xFFDC, 0x0000, 0x0000, 
};

static JointIndex sChildDanceAnimJointIndices[16] = {
    { 0x0000, 0x0003, 0x0021 },
    { 0x0002, 0x0000, 0x0002 },
    { 0x003F, 0x005D, 0x007B },
    { 0x0000, 0x0000, 0x0099 },
    { 0x00B7, 0x00D5, 0x00F3 },
    { 0x0111, 0x012F, 0x014D },
    { 0x0000, 0x0000, 0x016B },
    { 0x0189, 0x01A7, 0x01C5 },
    { 0x0000, 0x0000, 0x01E3 },
    { 0x0201, 0x021F, 0x023D },
    { 0x0000, 0x0000, 0x0000 },
    { 0x0000, 0x0000, 0x0000 },
    { 0x025B, 0x0279, 0x0297 },
    { 0x0000, 0x0000, 0x0000 },
    { 0x0000, 0x0000, 0x0000 },
    { 0x0000, 0x0000, 0x02B5 },
};

static AnimationHeader sChildDanceAnim = { 
    { 30 }, sChildDanceAnimFrameData,
    sChildDanceAnimJointIndices, 3
};

static s16 sParentDanceAnimFrameData[1042] = {
    0xFF73, 0x0E5D, 0x0032, 0xC000, 0x0000, 0x8000, 0xFF73, 0xFF47, 0xFF1D, 0xFEF6, 0xFED6, 0xFEBD, 0xFEAF, 0xFEAE, 
    0xFEBB, 0xFEE8, 0xFF39, 0xFF9E, 0x0006, 0x0062, 0x00A2, 0x00CA, 0x00ED, 0x010A, 0x011F, 0x012C, 0x0130, 0x012B, 
    0x011B, 0x00F2, 0x00AD, 0x0057, 0xFFFE, 0xFFAD, 0x0E5D, 0x0E59, 0x0E4D, 0x0E3D, 0x0E2B, 0x0E18, 0x0E08, 0x0DFD, 
    0x0DF8, 0x0E00, 0x0E12, 0x0E2B, 0x0E43, 0x0E56, 0x0E5D, 0x0E59, 0x0E4D, 0x0E3D, 0x0E2B, 0x0E18, 0x0E08, 0x0DFD, 
    0x0DF8, 0x0E00, 0x0E12, 0x0E2B, 0x0E43, 0x0E56, 0x024A, 0x023C, 0x023E, 0x024E, 0x0266, 0x027E, 0x028C, 0x0285, 
    0x0260, 0x020F, 0x0196, 0x0107, 0x007D, 0x0000, 0x0001, 0x0000, 0x000D, 0x0017, 0x0014, 0x0010, 0x000F, 0x0015, 
    0x0024, 0x0043, 0x00A7, 0x013F, 0x01D9, 0x023B, 0xF514, 0xF47F, 0xF407, 0xF3B0, 0xF381, 0xF37E, 0xF3AC, 0xF412, 
    0xF4B3, 0xF5C9, 0xF75F, 0xF934, 0xFB08, 0xFC9F, 0xFDBE, 0xFE4F, 0xFECC, 0xFF32, 0xFF7E, 0xFFAD, 0xFFBC, 0xFFA8, 
    0xFF6E, 0xFEDC, 0xFD3E, 0xFAF2, 0xF878, 0xF655, 0xFDD5, 0xFE67, 0xFE9C, 0xFE7F, 0xFE26, 0xFDAB, 0xFD2A, 0xFCC0, 
    0xFC8C, 0xFC9F, 0xFD13, 0xFE1B, 0x0007, 0x03D2, 0x0393, 0x03AF, 0x020B, 0xFF2B, 0xFD5B, 0xFBFA, 0xFAF5, 0xFA4E, 
    0xFA15, 0xFA8A, 0xFAB0, 0xFAED, 0xFBA5, 0xFCC8, 0x1105, 0x0FC4, 0x0F2B, 0x0F23, 0x0F83, 0x1018, 0x10AE, 0x1113, 
    0x1108, 0x1071, 0x0F1A, 0x0C91, 0x0834, 0x0000, 0x0037, 0x0000, 0x036E, 0x096C, 0x0D3C, 0x1026, 0x124F, 0x13B0, 
    0x142B, 0x1331, 0x139A, 0x1450, 0x141B, 0x12C8, 0x0225, 0x0131, 0xFF48, 0xFD50, 0xFC34, 0xFBE4, 0xFBB5, 0xFBA2, 
    0xFBA6, 0xFBBF, 0xFBE8, 0xFC1C, 0xFC59, 0xFC99, 0xFCD9, 0xFD4E, 0xFE0D, 0xFEDE, 0xFF8B, 0xFFDD, 0xFF9F, 0xFEF6, 
    0xFE31, 0xFD9C, 0xFD84, 0xFE6A, 0x000A, 0x018F, 0xE79B, 0xE7A3, 0xE7B7, 0xE7CC, 0xE7D5, 0xE7D4, 0xE7CF, 0xE7C9, 
    0xE7C1, 0xE7B8, 0xE7B0, 0xE7A8, 0xE7A1, 0xE79D, 0xE79B, 0xE7A1, 0xE7AD, 0xE7BD, 0xE7CC, 0xE7D7, 0xE7DE, 0xE7E4, 
    0xE7E8, 0xE7E8, 0xE7E4, 0xE7D5, 0xE7BD, 0xE7A7, 0xBE3E, 0xC0F2, 0xC400, 0xC67F, 0xC787, 0xC762, 0xC6F9, 0xC654, 
    0xC57C, 0xC47B, 0xC359, 0xC220, 0xC0D7, 0xBF89, 0xBE3E, 0xBC8A, 0xBA44, 0xB7DD, 0xB5C2, 0xB465, 0xB3D0, 0xB3B9, 
    0xB40F, 0xB4BF, 0xB5B9, 0xB740, 0xB96A, 0xBBDD, 0xFFE6, 0xFFDA, 0xFFD6, 0xFFDF, 0xFFEE, 0x0000, 0x000C, 0x000F, 
    0x0002, 0xFFD7, 0xFF5F, 0xFEB4, 0xFE13, 0xFDC0, 0xFDC2, 0xFDD2, 0xFDC8, 0xFDA8, 0xFD7D, 0xFD55, 0xFD3E, 0xFD40, 
    0xFD64, 0xFDB8, 0xFE35, 0xFEC2, 0xFF46, 0xFFAB, 0x028E, 0x01F2, 0x015A, 0x00CF, 0x005A, 0x0001, 0xFFCE, 0xFFC9, 
    0xFFF9, 0x009B, 0x0263, 0x04E6, 0x0797, 0x09E4, 0x0B37, 0x0BC2, 0x0C20, 0x0C51, 0x0C56, 0x0C2E, 0x0BDA, 0x0B5A, 
    0x0AB0, 0x09AC, 0x0841, 0x06A4, 0x0507, 0x039A, 0x020F, 0x0091, 0xFED6, 0xFD40, 0xFBDE, 0xFAB8, 0xF9D5, 0xF941, 
    0xF90A, 0xF96C, 0xF9B4, 0xFA3E, 0xFB5A, 0xFCE5, 0xFE33, 0xFEC0, 0xFEC6, 0xFE5E, 0xFDB0, 0xFCE7, 0xFC27, 0xFB90, 
    0xFB3F, 0xFB4C, 0xFBC4, 0xFCB6, 0xFE2C, 0x001D, 0x0366, 0x0682, 0x0A1F, 0x0D75, 0x1061, 0x12D0, 0x14B2, 0x15EE, 
    0x1663, 0x1591, 0x15B4, 0x15C6, 0x14BA, 0x1288, 0x1041, 0x0F0D, 0x0ED4, 0x0F66, 0x1073, 0x11AA, 0x12C6, 0x138C, 
    0x13C0, 0x1336, 0x11D5, 0x0F7A, 0x0C12, 0x07B3, 0x034A, 0x032F, 0x02F6, 0x02B2, 0x0273, 0x024C, 0x023F, 0x0241, 
    0x024D, 0x025D, 0x026E, 0x0283, 0x02A0, 0x02BD, 0x02D1, 0x02DE, 0x02EB, 0x02F9, 0x0307, 0x0315, 0x0323, 0x032F, 
    0x033A, 0x0343, 0x034A, 0x034F, 0x0351, 0x034F, 0x102E, 0x1025, 0x1017, 0x1008, 0x0FFC, 0x0FF4, 0x0FF0, 0x0FEF, 
    0x0FF0, 0x0FF6, 0x1001, 0x1019, 0x103D, 0x105E, 0x106E, 0x1070, 0x1070, 0x106E, 0x106C, 0x1068, 0x1063, 0x105E, 
    0x1058, 0x1051, 0x104A, 0x1043, 0x103C, 0x1035, 0xBAF6, 0xB9F8, 0xB852, 0xB66C, 0xB4AF, 0xB383, 0xB2D9, 0xB26C, 
    0xB251, 0xB2A2, 0xB376, 0xB595, 0xB8D8, 0xBBEE, 0xBD88, 0xBDE3, 0xBE21, 0xBE44, 0xBE4D, 0xBE3F, 0xBE1C, 0xBDE6, 
    0xBD9E, 0xBD47, 0xBCE3, 0xBC74, 0xBBFB, 0xBB7B, 0xFBED, 0xFB2F, 0xFA9C, 0xFA56, 0xFA7D, 0xFB30, 0xFC80, 0xFE4C, 
    0x0068, 0x02A8, 0x04E2, 0x06EA, 0x0896, 0x09B9, 0x0A2A, 0x0A09, 0x099D, 0x08F1, 0x080D, 0x06FC, 0x05C8, 0x047B, 
    0x031F, 0x01BE, 0x0062, 0xFF15, 0xFDE1, 0xFCD1, 0x0342, 0x0349, 0x0310, 0x02B0, 0x0242, 0x01DE, 0x017A, 0x0102, 
    0x007F, 0xFFFA, 0xFF7C, 0xFF0E, 0xFEB9, 0xFE86, 0xFE7D, 0xFE99, 0xFECC, 0xFF13, 0xFF69, 0xFFCD, 0x0039, 0x00AB, 
    0x011F, 0x0192, 0x0200, 0x0266, 0x02C0, 0x030A, 0x8000, 0x8001, 0x7FF5, 0x7FDF, 0x7FC7, 0x7FB0, 0x7F9A, 0x7F7F, 
    0x7F62, 0x7F44, 0x7F28, 0x7F10, 0x7EFD, 0x7EF1, 0x7EEF, 0x7EF6, 0x7F01, 0x7F11, 0x7F24, 0x7F3A, 0x7F52, 0x7F6C, 
    0x7F86, 0x7F9F, 0x7FB8, 0x7FCF, 0x7FE3, 0x7FF3, 0x0000, 0xFFF2, 0xFFCB, 0xFF8E, 0xFF3D, 0xFEDB, 0xFE6A, 0xFDEF, 
    0xFD6B, 0xFCE2, 0xFC55, 0xFBC9, 0xFB3F, 0xFABC, 0xFA40, 0xF9D0, 0xF96E, 0xF91D, 0xF8DF, 0xF8B8, 0xF8AB, 0xF8FB, 
    0xF9D0, 0xFAFD, 0xFC55, 0xFDAE, 0xFEDB, 0xFFAF, 0xF3BF, 0xF3AF, 0xF384, 0xF340, 0xF2E6, 0xF279, 0xF1FC, 0xF173, 
    0xF0E0, 0xF047, 0xEFAC, 0xEF10, 0xEE77, 0xEDE4, 0xED5B, 0xECDE, 0xEC71, 0xEC17, 0xEBD3, 0xEBA8, 0xEB99, 0xEBF2, 
    0xECDE, 0xEE2D, 0xEFAC, 0xF12B, 0xF279, 0xF365, 0xF3E6, 0xF40E, 0xF4EB, 0xF650, 0xF813, 0xFA09, 0xFC06, 0xFDE0, 
    0xFF6A, 0x00F5, 0x02AE, 0x0436, 0x052B, 0x0530, 0x047C, 0x038B, 0x0268, 0x011D, 0xFFB5, 0xFE39, 0xFCB3, 0xFB2D, 
    0xF9B3, 0xF84D, 0xF705, 0xF5E7, 0xF4FC, 0xF44D, 0x0D48, 0x0E07, 0x0F6B, 0x113C, 0x1342, 0x1546, 0x1710, 0x1868, 
    0x1917, 0x1883, 0x16B2, 0x145B, 0x1231, 0x10EB, 0x1056, 0x0FC6, 0x0F3C, 0x0EBB, 0x0E45, 0x0DDA, 0x0D7D, 0x0D2F, 
    0x0CF2, 0x0CC7, 0x0CB1, 0x0CB1, 0x0CC9, 0x0CFB, 0xBAAA, 0xBAA8, 0xBAAA, 0xBAAE, 0xBAB5, 0xBABD, 0xBAC6, 0xBAD0, 
    0xBAD9, 0xBAE8, 0xBAFD, 0xBB12, 0xBB23, 0xBB28, 0xBB24, 0xBB1F, 0xBB18, 0xBB0F, 0xBB06, 0xBAFB, 0xBAF0, 0xBAE5, 
    0xBADA, 0xBAD0, 0xBAC6, 0xBABD, 0xBAB5, 0xBAAE, 0x0F8A, 0x0F68, 0x0F09, 0x0E72, 0x0DAD, 0x0CC1, 0x0BB7, 0x0A94, 
    0x0963, 0x0829, 0x06EF, 0x05BE, 0x049B, 0x0390, 0x02A5, 0x01E0, 0x0149, 0x00E9, 0x00C8, 0x0132, 0x0251, 0x03F8, 
    0x05FA, 0x0829, 0x0A58, 0x0C5A, 0x0E01, 0x0F20, 0x0368, 0x03AC, 0x046E, 0x059E, 0x072C, 0x090A, 0x0B26, 0x0D71, 
    0x0FDC, 0x1256, 0x14D1, 0x173B, 0x1986, 0x1BA3, 0x1D80, 0x1F0E, 0x203E, 0x2100, 0x2145, 0x206E, 0x1E29, 0x1AD1, 
    0x16C1, 0x1256, 0x0DEB, 0x09DB, 0x0683, 0x043E, 0x015D, 0x0134, 0x00BE, 0x0006, 0xFF14, 0xFDF3, 0xFCAC, 0xFB48, 
    0xF9D1, 0xF850, 0xF6CF, 0xF558, 0xF3F4, 0xF2AD, 0xF18C, 0xF09A, 0xEFE2, 0xEF6C, 0xEF43, 0xEFC5, 0xF125, 0xF32C, 
    0xF5A2, 0xF850, 0xFAFE, 0xFD74, 0xFF7B, 0x00DB, 0x0AA0, 0x0B8D, 0x0CEA, 0x0E7D, 0x100C, 0x115D, 0x1236, 0x125E, 
    0x11C8, 0x10A8, 0x0F25, 0x0D65, 0x0B8F, 0x09C9, 0x083A, 0x0709, 0x065B, 0x0615, 0x05FC, 0x060C, 0x063F, 0x0691, 
    0x06FD, 0x077F, 0x0811, 0x08AF, 0x0954, 0x09FC, 0xFCE2, 0xFCDA, 0xFCCE, 0xFCC1, 0xFCB2, 0xFCA3, 0xFC95, 0xFC8A, 
    0xFC82, 0xFC7F, 0xFC80, 0xFC84, 0xFC8B, 0xFC93, 0xFC9C, 0xFCA6, 0xFCB1, 0xFCBC, 0xFCC6, 0xFCCF, 0xFCD7, 0xFCDE, 
    0xFCE2, 0xFCE6, 0xFCE9, 0xFCEA, 0xFCE9, 0xFCE7, 0x022F, 0x01BA, 0x0107, 0x002A, 0xFF37, 0xFE41, 0xFD5C, 0xFC9C, 
    0xFC13, 0xFBD7, 0xFBDC, 0xFC07, 0xFC53, 0xFCBA, 0xFD35, 0xFDBE, 0xFE50, 0xFEE5, 0xFF76, 0xFFFD, 0x0075, 0x00D7, 
    0x0133, 0x0193, 0x01ED, 0x0235, 0x025F, 0x0261, 0xCC20, 0xCB32, 0xCA17, 0xC8E4, 0xC7AF, 0xC68D, 0xC595, 0xC4DB, 
    0xC477, 0xC47C, 0xC4EC, 0xC5B1, 0xC6B9, 0xC7F4, 0xC953, 0xCAC5, 0xCC3A, 0xCDA3, 0xCEEE, 0xD00B, 0xD0EC, 0xD17F, 
    0xD198, 0xD12C, 0xD060, 0xCF55, 0xCE2F, 0xCD12, 0x06B3, 0x0681, 0x05F5, 0x051B, 0x0401, 0x02B3, 0x013F, 0xFFB0, 
    0xFE15, 0xFC79, 0xFAEB, 0xF976, 0xF828, 0xF70E, 0xF634, 0xF5A8, 0xF577, 0xF5CE, 0xF6BE, 0xF828, 0xF9EF, 0xFBF2, 
    0xFE15, 0x0037, 0x023B, 0x0401, 0x056C, 0x065C, 0x01C6, 0x018B, 0x00E4, 0xFFE1, 0xFE91, 0xFD04, 0xFB48, 0xF96D, 
    0xF783, 0xF59A, 0xF3BF, 0xF203, 0xF076, 0xEF26, 0xEE23, 0xED7C, 0xED41, 0xEDA8, 0xEEC6, 0xF076, 0xF293, 0xF4F9, 
    0xF783, 0xFA0E, 0xFC74, 0xFE91, 0x0041, 0x015E, 0xF58D, 0xF579, 0xF541, 0xF4E9, 0xF477, 0xF3F0, 0xF35A, 0xF2B9, 
    0xF213, 0xF16D, 0xF0CC, 0xF035, 0xEFAE, 0xEF3C, 0xEEE4, 0xEEAC, 0xEE98, 0xEEBB, 0xEF1C, 0xEFAE, 0xF066, 0xF136, 
    0xF213, 0xF2EF, 0xF3C0, 0xF477, 0xF50A, 0xF56A, 0x0000, 0xFFD3, 0xFF57, 0xFE97, 0xFDA2, 0xFC85, 0xFB4D, 0xFA07, 
    0xF8C2, 0xF78A, 0xF66D, 0xF578, 0xF4B8, 0xF43B, 0xF40F, 0xF43B, 0xF4B8, 0xF578, 0xF66D, 0xF78A, 0xF8C2, 0xFA07, 
    0xFB4D, 0xFC85, 0xFDA2, 0xFE97, 0xFF57, 0xFFD3, 0x0000, 0x0016, 0x0053, 0x00B1, 0x0129, 0x01B5, 0x024E, 0x02ED, 
    0x038D, 0x0426, 0x04B2, 0x052A, 0x0588, 0x05C5, 0x05DB, 0x05C5, 0x0588, 0x052A, 0x04B2, 0x0426, 0x038D, 0x02ED, 
    0x024E, 0x01B5, 0x0129, 0x00B1, 0x0053, 0x0016, 0x0000, 0xFFF9, 0xFFE7, 0xFFCB, 0xFFA7, 0xFF7D, 0xFF4F, 0xFF1F, 
    0xFEEF, 0xFEC1, 0xFE97, 0xFE73, 0xFE56, 0xFE44, 0xFE3E, 0xFE44, 0xFE56, 0xFE73, 0xFE97, 0xFEC1, 0xFEEF, 0xFF1F, 
    0xFF4F, 0xFF7D, 0xFFA7, 0xFFCB, 0xFFE7, 0xFFF9, 
};

static JointIndex sParentDanceAnimJointIndices[16] = {
    { 0x0006, 0x0022, 0x0002 },
    { 0x0003, 0x0004, 0x0003 },
    { 0x003E, 0x005A, 0x0076 },
    { 0x0004, 0x0004, 0x0092 },
    { 0x00AE, 0x00CA, 0x00E6 },
    { 0x0102, 0x011E, 0x013A },
    { 0x0004, 0x0004, 0x0156 },
    { 0x0172, 0x018E, 0x01AA },
    { 0x01C6, 0x01E2, 0x01FE },
    { 0x021A, 0x0236, 0x0005 },
    { 0x0252, 0x026E, 0x028A },
    { 0x02A6, 0x02C2, 0x02DE },
    { 0x0004, 0x02FA, 0x0005 },
    { 0x0316, 0x0332, 0x034E },
    { 0x036A, 0x0386, 0x03A2 },
    { 0x03BE, 0x03DA, 0x03F6 },
};

static AnimationHeader sParentDanceAnim = { 
    { 28 }, sParentDanceAnimFrameData,
    sParentDanceAnimJointIndices, 6
};
