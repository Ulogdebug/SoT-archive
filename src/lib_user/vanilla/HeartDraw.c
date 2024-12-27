#include <uLib.h>
#include <code/z_lifemeter.h>
asm ("sHeartDDTextures = 0x8011FFF0;");
asm ("sHeartTextures = 0x8011FFB0;");
asm ("sHeartsPrimFactors = 0x8011FF38;");
asm ("sHeartsEnvFactors = 0x8011FF4C;");
asm ("sHeartsPrimColors = 0x8011FF10;");
asm ("sHeartsEnvColors = 0x8011FF24;");
asm ("sHeartsDDPrimFactors = 0x8011FF88;");
asm ("sHeartsDDEnvFactors = 0x8011FF9C;");
asm ("sHeartsDDEnvColors = 0x8011FF74;");
asm ("sHeartsDDPrimColors = 0x8011FF60;");
Asm_VanillaHook(Health_DrawMeter);
void Health_DrawMeter(PlayState* play) {
    s32 pad[5];
    void* heartBgImg;
    u32 curColorSet;
    f32 offsetX;
    f32 offsetY;
    s32 heartIndex;
    f32 halfHeartLength;
    f32 heartCenterX;
    f32 heartCenterY;
    f32 heartTexCoordPerPixel;
    InterfaceContext* interfaceCtx = &play->interfaceCtx;
    GraphicsContext* gfxCtx = play->state.gfxCtx;
    Vtx* beatingHeartVtx = interfaceCtx->beatingHeartVtx;
    s32 curHeartFraction = gSaveContext.health % 0x10;
    s16 totalHeartCount = gSaveContext.healthCapacity / 0x10;
    s16 fullHeartCount = gSaveContext.health / 0x10;
    s32 pad2;
    f32 beatingHeartPulsingSize = interfaceCtx->beatingHeartOscillator * 0.1f;
    s32 curCombineModeSet = 0;
    u8* curBgImgLoaded = NULL;
    s32 ddHeartCountMinusOne = gSaveContext.inventory.defenseHearts - 1;

    OPEN_DISPS(gfxCtx, "../z_lifemeter.c", 353);

    if (!(gSaveContext.health % 0x10)) {
        fullHeartCount--;
    }

    curColorSet = -1;
    offsetY = 10.0f;//0.0f
    offsetX = 0.0f;

    for (heartIndex = 0; heartIndex < totalHeartCount; heartIndex++) {
        if ((ddHeartCountMinusOne < 0) || (heartIndex > ddHeartCountMinusOne)) {
            if (heartIndex < fullHeartCount) {
                if (curColorSet != 0) {
                    curColorSet = 0;
                    gDPPipeSync(OVERLAY_DISP++);
                    gDPSetPrimColor(OVERLAY_DISP++, 0, 0, interfaceCtx->heartsPrimR[0], interfaceCtx->heartsPrimG[0],
                                    interfaceCtx->heartsPrimB[0], interfaceCtx->healthAlpha);
                    gDPSetEnvColor(OVERLAY_DISP++, interfaceCtx->heartsEnvR[0], interfaceCtx->heartsEnvG[0],
                                   interfaceCtx->heartsEnvB[0], 255);
                }
            } else if (heartIndex == fullHeartCount) {
                if (curColorSet != 1) {
                    curColorSet = 1;
                    gDPPipeSync(OVERLAY_DISP++);
                    gDPSetPrimColor(OVERLAY_DISP++, 0, 0, interfaceCtx->beatingHeartPrim[0],
                                    interfaceCtx->beatingHeartPrim[1], interfaceCtx->beatingHeartPrim[2],
                                    interfaceCtx->healthAlpha);
                    gDPSetEnvColor(OVERLAY_DISP++, interfaceCtx->beatingHeartEnv[0], interfaceCtx->beatingHeartEnv[1],
                                   interfaceCtx->beatingHeartEnv[2], 255);
                }
            } else if (heartIndex > fullHeartCount) {
                if (curColorSet != 2) {
                    curColorSet = 2;
                    gDPPipeSync(OVERLAY_DISP++);
                    gDPSetPrimColor(OVERLAY_DISP++, 0, 0, interfaceCtx->heartsPrimR[0], interfaceCtx->heartsPrimG[0],
                                    interfaceCtx->heartsPrimB[0], interfaceCtx->healthAlpha);
                    gDPSetEnvColor(OVERLAY_DISP++, interfaceCtx->heartsEnvR[0], interfaceCtx->heartsEnvG[0],
                                   interfaceCtx->heartsEnvB[0], 255);
                }
            } else {
                if (curColorSet != 3) {
                    curColorSet = 3;
                    gDPPipeSync(OVERLAY_DISP++);
                    gDPSetPrimColor(OVERLAY_DISP++, 0, 0, interfaceCtx->heartsPrimR[1], interfaceCtx->heartsPrimG[1],
                                    interfaceCtx->heartsPrimB[1], interfaceCtx->healthAlpha);
                    gDPSetEnvColor(OVERLAY_DISP++, interfaceCtx->heartsEnvR[1], interfaceCtx->heartsEnvG[1],
                                   interfaceCtx->heartsEnvB[1], 255);
                }
            }

            if (heartIndex < fullHeartCount) {
                heartBgImg = gHeartFullTex;
            } else if (heartIndex == fullHeartCount) {
                heartBgImg = sHeartTextures[curHeartFraction];
            } else {
                heartBgImg = gHeartEmptyTex;
            }
        } else {
            if (heartIndex < fullHeartCount) {
                if (curColorSet != 4) {
                    curColorSet = 4;
                    gDPPipeSync(OVERLAY_DISP++);
                    gDPSetPrimColor(OVERLAY_DISP++, 0, 0, sHeartsDDPrim[0][0], sHeartsDDPrim[0][1], sHeartsDDPrim[0][2],
                                    interfaceCtx->healthAlpha);
                    gDPSetEnvColor(OVERLAY_DISP++, sHeartsDDEnv[0][0], sHeartsDDEnv[0][1], sHeartsDDEnv[0][2], 255);
                }
            } else if (heartIndex == fullHeartCount) {
                if (curColorSet != 5) {
                    curColorSet = 5;
                    gDPPipeSync(OVERLAY_DISP++);
                    gDPSetPrimColor(OVERLAY_DISP++, 0, 0, sBeatingHeartsDDPrim[0], sBeatingHeartsDDPrim[1],
                                    sBeatingHeartsDDPrim[2], interfaceCtx->healthAlpha);
                    gDPSetEnvColor(OVERLAY_DISP++, sBeatingHeartsDDEnv[0], sBeatingHeartsDDEnv[1],
                                   sBeatingHeartsDDEnv[2], 255);
                }
            } else if (heartIndex > fullHeartCount) {
                if (curColorSet != 6) {
                    curColorSet = 6;
                    gDPPipeSync(OVERLAY_DISP++);
                    gDPSetPrimColor(OVERLAY_DISP++, 0, 0, sHeartsDDPrim[0][0], sHeartsDDPrim[0][1], sHeartsDDPrim[0][2],
                                    interfaceCtx->healthAlpha);
                    gDPSetEnvColor(OVERLAY_DISP++, sHeartsDDEnv[0][0], sHeartsDDEnv[0][1], sHeartsDDEnv[0][2], 255);
                }
            } else {
                if (curColorSet != 7) {
                    curColorSet = 7;
                    gDPPipeSync(OVERLAY_DISP++);
                    gDPSetPrimColor(OVERLAY_DISP++, 0, 0, sHeartsDDPrim[1][0], sHeartsDDPrim[1][1], sHeartsDDPrim[1][2],
                                    interfaceCtx->healthAlpha);
                    gDPSetEnvColor(OVERLAY_DISP++, sHeartsDDEnv[1][0], sHeartsDDEnv[1][1], sHeartsDDEnv[1][2], 255);
                }
            }

            if (heartIndex < fullHeartCount) {
                heartBgImg = gDefenseHeartFullTex;
            } else if (heartIndex == fullHeartCount) {
                heartBgImg = sHeartDDTextures[curHeartFraction];
            } else {
                heartBgImg = gDefenseHeartEmptyTex;
            }
        }

        if (curBgImgLoaded != heartBgImg) {
            curBgImgLoaded = heartBgImg;
            gDPLoadTextureBlock(OVERLAY_DISP++, heartBgImg, G_IM_FMT_IA, G_IM_SIZ_8b, 16, 16, 0,
                                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                G_TX_NOLOD, G_TX_NOLOD);
        }

        if (heartIndex != fullHeartCount) {
            if ((ddHeartCountMinusOne < 0) || (heartIndex > ddHeartCountMinusOne)) {
                if (curCombineModeSet != 1) {
                    curCombineModeSet = 1;
                    Gfx_SetupDL_39Overlay(gfxCtx);
                    gDPSetCombineLERP(OVERLAY_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE,
                                      0, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0);
                }
            } else {
                if (curCombineModeSet != 3) {
                    curCombineModeSet = 3;
                    Gfx_SetupDL_39Overlay(gfxCtx);
                    gDPSetCombineLERP(OVERLAY_DISP++, ENVIRONMENT, PRIMITIVE, TEXEL0, PRIMITIVE, TEXEL0, 0, PRIMITIVE,
                                      0, ENVIRONMENT, PRIMITIVE, TEXEL0, PRIMITIVE, TEXEL0, 0, PRIMITIVE, 0);
                }
            }

            heartCenterY = 26.0f + offsetY;
            heartCenterX = 30.0f + offsetX;
            heartTexCoordPerPixel = 1.0f;
            heartTexCoordPerPixel /= 0.68f;
            heartTexCoordPerPixel *= 1 << 10;
            halfHeartLength = 8.0f;
            halfHeartLength *= 0.68f;
            gSPTextureRectangle(OVERLAY_DISP++, (s32)((heartCenterX - halfHeartLength) * 4),
                                (s32)((heartCenterY - halfHeartLength) * 4),
                                (s32)((heartCenterX + halfHeartLength) * 4),
                                (s32)((heartCenterY + halfHeartLength) * 4), G_TX_RENDERTILE, 0, 0,
                                (s32)heartTexCoordPerPixel, (s32)heartTexCoordPerPixel);
        } else {
            if ((ddHeartCountMinusOne < 0) || (heartIndex > ddHeartCountMinusOne)) {
                if (curCombineModeSet != 2) {
                    curCombineModeSet = 2;
                    Gfx_SetupDL_42Overlay(gfxCtx);
                    gDPSetCombineLERP(OVERLAY_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE,
                                      0, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0);
                }
            } else {
                if (curCombineModeSet != 4) {
                    curCombineModeSet = 4;
                    Gfx_SetupDL_42Overlay(gfxCtx);
                    gDPSetCombineLERP(OVERLAY_DISP++, ENVIRONMENT, PRIMITIVE, TEXEL0, PRIMITIVE, TEXEL0, 0, PRIMITIVE,
                                      0, ENVIRONMENT, PRIMITIVE, TEXEL0, PRIMITIVE, TEXEL0, 0, PRIMITIVE, 0);
                }
            }

            {
                Mtx* matrix = Graph_Alloc(gfxCtx, sizeof(Mtx));
                Matrix_SetTranslateScaleMtx2(
                    matrix, 1.0f - (0.32f * beatingHeartPulsingSize), 1.0f - (0.32f * beatingHeartPulsingSize),
                    1.0f - (0.32f * beatingHeartPulsingSize), -130.0f + offsetX, 94.5f - offsetY, 0.0f);
                gSPMatrix(OVERLAY_DISP++, matrix, G_MTX_MODELVIEW | G_MTX_LOAD);
                gSPVertex(OVERLAY_DISP++, beatingHeartVtx, 4, 0);
                gSP1Quadrangle(OVERLAY_DISP++, 0, 2, 3, 1, 0);
            }
        }

        // Move offset to next heart
        offsetX += 10.0f;

        //Go down one line after 12 hearts
         if (heartIndex == 11) {
             offsetY += 10.0f;
             offsetX = 0.0f;
         }
    }

    CLOSE_DISPS(gfxCtx, "../z_lifemeter.c", 606);
}

Asm_VanillaHook(Health_UpdateMeter);
void Health_UpdateMeter(PlayState* play) {
    InterfaceContext* interfaceCtx = &play->interfaceCtx;
    f32 factor = interfaceCtx->heartColorOscillator * 0.1f;
    f32 ddFactor;
    s32 type = 0;
    s32 ddType;
    s16 rFactor;
    s16 gFactor;
    s16 bFactor;


    u8 heart_DEN_R = 0;
    u8 heart_DEN_G = 0;
    u8 heart_DEN_B = 0;

    u8 heart_EN_R = 0; //50
    u8 heart_EN_G = 0; //40
    u8 heart_EN_B = 0; //60
    
    u8 heart_DD_PRIM_R = 0; //255
    u8 heart_DD_PRIM_G = 0; //255
    u8 heart_DD_PRIM_B = 0; //255

    u8 heart_PRIM_R = 128; //255
    u8 heart_PRIM_G = 0; //70
    u8 heart_PRIM_B = 0; //50

    if(gSaveContext.healthAccumulator < 0 || gSaveContext.health <=0){
        heart_PRIM_R = 255;
        heart_PRIM_G = 255; 
        heart_PRIM_B = 255;  

        heart_EN_R = 20;
        heart_EN_G = 20;
        heart_EN_B = 20; 
    }else {
        if(gSaveContext.healthAccumulator > 0 || gSaveContext.health == gSaveContext.healthCapacity){
            heart_PRIM_R = 230;
        }else if (gSaveContext.health <= gSaveContext.healthCapacity * (1.0/3.0)){
            heart_PRIM_R = 128;
        }else{
             heart_PRIM_R = 200;
        }
        heart_PRIM_G = 0; 
        heart_PRIM_B = 0;

        heart_EN_R = 0;
        heart_EN_G = 0;
        heart_EN_B = 0; 
    }

    if (interfaceCtx) {}

    if (interfaceCtx->heartColorOscillatorDirection != 0) {
        interfaceCtx->heartColorOscillator--;
        if (interfaceCtx->heartColorOscillator <= 0) {
            interfaceCtx->heartColorOscillator = 0;
            interfaceCtx->heartColorOscillatorDirection = 0;
        }
    } else {
        interfaceCtx->heartColorOscillator++;
        if (interfaceCtx->heartColorOscillator >= 10) {
            interfaceCtx->heartColorOscillator = 10;
            interfaceCtx->heartColorOscillatorDirection = 1;
        }
    }

    ddFactor = factor;

    interfaceCtx->heartsPrimR[0] = heart_PRIM_R;
    interfaceCtx->heartsPrimG[0] = heart_PRIM_G;
    interfaceCtx->heartsPrimB[0] = heart_PRIM_B;

    interfaceCtx->heartsEnvR[0] = heart_EN_R;
    interfaceCtx->heartsEnvG[0] = heart_EN_G;
    interfaceCtx->heartsEnvB[0] = heart_EN_B;

    interfaceCtx->heartsPrimR[1] = sHeartsPrimColors[type][0];
    interfaceCtx->heartsPrimG[1] = sHeartsPrimColors[type][1];
    interfaceCtx->heartsPrimB[1] = sHeartsPrimColors[type][2];

    interfaceCtx->heartsEnvR[1] = sHeartsEnvColors[type][0];
    interfaceCtx->heartsEnvG[1] = sHeartsEnvColors[type][1];
    interfaceCtx->heartsEnvB[1] = sHeartsEnvColors[type][2];

    rFactor = sHeartsPrimFactors[0][0] * factor;
    gFactor = sHeartsPrimFactors[0][1] * factor;
    bFactor = sHeartsPrimFactors[0][2] * factor;

    interfaceCtx->beatingHeartPrim[0] = (u8)(rFactor + heart_PRIM_R) & 0xFF;
    interfaceCtx->beatingHeartPrim[1] = (u8)(gFactor + heart_PRIM_G) & 0xFF;
    interfaceCtx->beatingHeartPrim[2] = (u8)(bFactor + heart_PRIM_B) & 0xFF;

    rFactor = sHeartsEnvFactors[0][0] * factor;
    gFactor = sHeartsEnvFactors[0][1] * factor;
    bFactor = sHeartsEnvFactors[0][2] * factor;

    ddType = type;

    interfaceCtx->beatingHeartEnv[0] = (u8)(rFactor + heart_EN_R) & 0xFF;
    interfaceCtx->beatingHeartEnv[1] = (u8)(gFactor + heart_EN_G) & 0xFF;
    interfaceCtx->beatingHeartEnv[2] = (u8)(bFactor + heart_EN_B) & 0xFF;

    sHeartsDDPrim[0][0] = heart_DD_PRIM_R;
    sHeartsDDPrim[0][1] = heart_DD_PRIM_G;
    sHeartsDDPrim[0][2] = heart_DD_PRIM_B;

    sHeartsDDEnv[0][0] = heart_DEN_R;
    sHeartsDDEnv[0][1] = heart_DEN_G;
    sHeartsDDEnv[0][2] = heart_DEN_B;

    sHeartsDDPrim[1][0] = sHeartsDDPrimColors[ddType][0];
    sHeartsDDPrim[1][1] = sHeartsDDPrimColors[ddType][1];
    sHeartsDDPrim[1][2] = sHeartsDDPrimColors[ddType][2];

    sHeartsDDEnv[1][0] = sHeartsDDEnvColors[ddType][0];
    sHeartsDDEnv[1][1] = sHeartsDDEnvColors[ddType][1];
    sHeartsDDEnv[1][2] = sHeartsDDEnvColors[ddType][2];

    rFactor = sHeartsDDPrimFactors[ddType][0] * ddFactor;
    gFactor = sHeartsDDPrimFactors[ddType][1] * ddFactor;
    bFactor = sHeartsDDPrimFactors[ddType][2] * ddFactor;

    sBeatingHeartsDDPrim[0] = (u8)(rFactor + heart_DD_PRIM_R) & 0xFF;
    sBeatingHeartsDDPrim[1] = (u8)(gFactor + heart_DD_PRIM_G) & 0xFF;
    sBeatingHeartsDDPrim[2] = (u8)(bFactor + heart_DD_PRIM_B) & 0xFF;

    rFactor = sHeartsDDEnvFactors[ddType][0] * ddFactor;
    gFactor = sHeartsDDEnvFactors[ddType][1] * ddFactor;
    bFactor = sHeartsDDEnvFactors[ddType][2] * ddFactor;

    sBeatingHeartsDDEnv[0] = (u8)(rFactor + heart_DEN_R) & 0xFF;
    sBeatingHeartsDDEnv[1] = (u8)(gFactor + heart_DEN_G) & 0xFF;
    sBeatingHeartsDDEnv[2] = (u8)(bFactor + heart_DEN_B) & 0xFF;
}
