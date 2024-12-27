#include <uLib.h>
#include "include/textureInfo/sandstorm.h"

extern u8 sSandstormColorIndex;
asm ("sSandstormEnvColors = 0x8011FDCC");
extern u8 sNextSandstormColorIndex;
asm ("sNextSandstormColorIndex = 0x8011FDD0");
extern f32 sSandstormLerpScale;
asm ("sSandstormLerpScale = 0x8011FDD4");
extern Color_RGB8 sSandstormPrimColors[];
asm ("sSandstormEnvColors = 0x8011FEC4");
extern Color_RGB8 sSandstormEnvColors[];
asm ("sSandstormEnvColors = 0x8011FED0");
extern u16 sSandstormScroll;
asm ("sSandstormEnvColors = 0x8015FDB0");

static Gfx sandStormDl[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPSetRenderMode(GBL_c1(G_BL_CLR_IN, G_BL_0, G_BL_CLR_IN, G_BL_1), GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA) | IM_RD | 0x10<<5 | ZMODE_OPA | FORCE_BL),
    gsDPSetCombineLERP(TEXEL1, TEXEL0, PRIM_LOD_FRAC, TEXEL0, TEXEL1, TEXEL0, ENVIRONMENT, TEXEL0, PRIMITIVE, ENVIRONMENT, COMBINED, ENVIRONMENT, COMBINED, 0, PRIMITIVE, 0),
    gsSPClearGeometryMode(G_TEXTURE_ENABLE | G_CULL_BACK | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsDPLoadTextureBlock(gFieldSandstorm1Tex, G_IM_FMT_I, G_IM_SIZ_8b, 64, 32, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 6, 5, 1, 0),
    gsDPLoadMultiBlock(gFieldSandstorm2Tex, 0x100, 1, G_IM_FMT_IA, G_IM_SIZ_8b, 64, 32, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 6, 5, 2, 1),
    gsSPDisplayList(0x08000000),
    gsSPTextureRectangle(0, 0, (320<<2), (240<<2), G_TX_RENDERTILE, 0, 0, 140, (1<<15) | (31<<10) | 884),
    gsSPEndDisplayList(),
};

Asm_VanillaHook(Environment_DrawSandstorm);
void Environment_DrawSandstorm(PlayState* play, u8 sandstormState) {
    s32 primA1;
    s32 envA1;
    s32 primA = play->envCtx.sandstormPrimA;
    s32 envA = play->envCtx.sandstormEnvA;
    Color_RGBA8 primColor;
    Color_RGBA8 envColor;
    f32 sp98;
    u16 sp96;
    u16 sp94;
    u16 sp92;


    switch (sandstormState) {
        case SANDSTORM_ACTIVE:
            if ((play->sceneId == SCENE_SPOT13) && (play->roomCtx.curRoom.num == 0)) {
                envA1 = 0;
                primA1 = (play->envCtx.sandstormEnvA > 128) ? 255 : play->envCtx.sandstormEnvA >> 1;
            } else {
                primA1 = play->state.frames % 128;
                if (primA1 > 64) {
                    primA1 = 128 - primA1;
                }
                primA1 += 73;
                envA1 = 128;
            }
            break;

        case SANDSTORM_FILL:
            primA1 = 255;
            envA1 = (play->envCtx.sandstormPrimA >= 255) ? 255 : 128;
            break;

        case SANDSTORM_UNFILL:
            envA1 = 128;
            if (play->envCtx.sandstormEnvA > 128) {
                primA1 = 255;
            } else {
                primA1 = play->state.frames % 128;
                if (primA1 > 64) {
                    primA1 = 128 - primA1;
                }
                primA1 += 73;
            }
            if ((primA1 >= primA) && (primA1 != 255)) {
                play->envCtx.sandstormState = SANDSTORM_ACTIVE;
            }
            break;

        case SANDSTORM_DISSIPATE:
            envA1 = 0;
            primA1 = (play->envCtx.sandstormEnvA > 128) ? 255 : play->envCtx.sandstormEnvA >> 1;

            if (primA == 0) {
                play->envCtx.sandstormState = SANDSTORM_OFF;
            }
            break;
    }

    if (ABS(primA - primA1) < 9) {
        primA = primA1;
    } else if (primA1 < primA) {
        primA = primA - 9;
    } else {
        primA = primA + 9;
    }

    if (ABS(envA - envA1) < 9) {
        envA = envA1;
    } else if (envA1 < envA) {
        envA = envA - 9;
    } else {
        envA = envA + 9;
    }

    play->envCtx.sandstormPrimA = primA;
    play->envCtx.sandstormEnvA = envA;

    sp98 = (512.0f - (primA + envA)) * (3.0f / 128.0f);

    if (sp98 > 6.0f) {
        sp98 = 6.0f;
    }

    if ((play->envCtx.lightMode != LIGHT_MODE_TIME) ||
        (play->envCtx.lightSettingOverride != LIGHT_SETTING_OVERRIDE_NONE)) {
        primColor.r = sSandstormPrimColors[1].r;
        primColor.g = sSandstormPrimColors[1].g;
        primColor.b = sSandstormPrimColors[1].b;
        envColor.r = sSandstormEnvColors[1].r;
        envColor.g = sSandstormEnvColors[1].g;
        envColor.b = sSandstormEnvColors[1].b;
    } else if (sSandstormColorIndex == sNextSandstormColorIndex) {
        primColor.r = sSandstormPrimColors[sSandstormColorIndex].r;
        primColor.g = sSandstormPrimColors[sSandstormColorIndex].g;
        primColor.b = sSandstormPrimColors[sSandstormColorIndex].b;
        envColor.r = sSandstormEnvColors[sSandstormColorIndex].r;
        envColor.g = sSandstormEnvColors[sSandstormColorIndex].g;
        envColor.b = sSandstormEnvColors[sSandstormColorIndex].b;
    } else {
        primColor.r = (s32)F32_LERP(sSandstormPrimColors[sSandstormColorIndex].r,
                                    sSandstormPrimColors[sNextSandstormColorIndex].r, sSandstormLerpScale);
        primColor.g = (s32)F32_LERP(sSandstormPrimColors[sSandstormColorIndex].g,
                                    sSandstormPrimColors[sNextSandstormColorIndex].g, sSandstormLerpScale);
        primColor.b = (s32)F32_LERP(sSandstormPrimColors[sSandstormColorIndex].b,
                                    sSandstormPrimColors[sNextSandstormColorIndex].b, sSandstormLerpScale);
        envColor.r = (s32)F32_LERP(sSandstormEnvColors[sSandstormColorIndex].r,
                                   sSandstormEnvColors[sNextSandstormColorIndex].r, sSandstormLerpScale);
        envColor.g = (s32)F32_LERP(sSandstormEnvColors[sSandstormColorIndex].g,
                                   sSandstormEnvColors[sNextSandstormColorIndex].g, sSandstormLerpScale);
        envColor.b = (s32)F32_LERP(sSandstormEnvColors[sSandstormColorIndex].b,
                                   sSandstormEnvColors[sNextSandstormColorIndex].b, sSandstormLerpScale);
    }

    envColor.r = ((envColor.r * sp98) + ((6.0f - sp98) * primColor.r)) * (1.0f / 6.0f);
    envColor.g = ((envColor.g * sp98) + ((6.0f - sp98) * primColor.g)) * (1.0f / 6.0f);
    envColor.b = ((envColor.b * sp98) + ((6.0f - sp98) * primColor.b)) * (1.0f / 6.0f);

    sp96 = (s32)(sSandstormScroll * (11.0f / 6.0f));
    sp94 = (s32)(sSandstormScroll * (9.0f / 6.0f));
    sp92 = (s32)(sSandstormScroll * (6.0f / 6.0f));

    OPEN_DISPS(play->state.gfxCtx,  __FILE__, __LINE__);

    POLY_XLU_DISP = Gfx_SetupDL_64(POLY_XLU_DISP);

    gDPSetAlphaDither(POLY_XLU_DISP++, G_AD_NOISE);
    gDPSetColorDither(POLY_XLU_DISP++, G_CD_NOISE);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0x80, primColor.r, primColor.g, primColor.b, play->envCtx.sandstormPrimA);
    gDPSetEnvColor(POLY_XLU_DISP++, envColor.r, envColor.g, envColor.b, play->envCtx.sandstormEnvA);
    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(play->state.gfxCtx, G_TX_RENDERTILE, (u32)sp96 % 4096, 0, 512, 32, 1, (u32)sp94 % 4096,
                                4095 - ((u32)sp92 % 4096), 256, 64));
    gDPSetTextureLUT(POLY_XLU_DISP++, G_TT_NONE);
    gSPDisplayList(POLY_XLU_DISP++, sandStormDl);

    CLOSE_DISPS(play->state.gfxCtx,  __FILE__, __LINE__);

    sSandstormScroll += (s32)sp98;
}