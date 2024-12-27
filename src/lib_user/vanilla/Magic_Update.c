#include "global.h"
#include "uLib_macros.h"
#include "vt.h"
#include "assets/textures/parameter_static/parameter_static.h"
#include "assets/textures/do_action_static/do_action_static.h"
#include "assets/textures/icon_item_static/icon_item_static.h"
#include <z64hdr.h>
#include "uLib.h"
#include <code/z_parameter.h>

// z64ram = 0x800879BC
// z64rom = 0xAFEB5C
asm("sMagicBorderR=0x80125A28");
asm("sMagicBorderG=0x80125A2C");
asm("sMagicBorderB=0x80125A30");

Asm_VanillaHook(Magic_Update);

void Magic_Update(PlayState* play) {
    static s16 sMagicBorderColors[][3] = {
        { 255, 255, 255 },
        { 150, 150, 150 },
        { 255, 255, 150 }, // unused
        { 255, 255, 50 },  // unused
    };
    static s16 sMagicBorderIndices[] = { 0, 1, 1, 0 };
    static s16 sMagicBorderRatio = 2;
    static s16 sMagicBorderStep = 1;
    MessageContext* msgCtx = &play->msgCtx;
    InterfaceContext* interfaceCtx = &play->interfaceCtx;
    s16 borderChangeR;
    s16 borderChangeG;
    s16 borderChangeB;
    s16 temp; // target for magicCapacity, or magicBorderIndex

    switch (gSaveContext.magicState) {
        case MAGIC_STATE_STEP_CAPACITY:
            // Step magicCapacity to the capacity determined by magicLevel
            // This changes the width of the magic meter drawn
            temp = gSaveContext.magicLevel * MAGIC_NORMAL_METER;
            if (gSaveContext.magicCapacity != temp) {
                if (gSaveContext.magicCapacity < temp) {
                    gSaveContext.magicCapacity += 8;
                    if (gSaveContext.magicCapacity > temp) {
                        gSaveContext.magicCapacity = temp;
                    }
                } else {
                    gSaveContext.magicCapacity -= 8;
                    if (gSaveContext.magicCapacity <= temp) {
                        gSaveContext.magicCapacity = temp;
                    }
                }
            } else {
                // Once the capacity has reached its target,
                // follow up by filling magic to magicFillTarget
                gSaveContext.magicState = MAGIC_STATE_FILL;
            }
            break;

        case MAGIC_STATE_FILL:
            // Add magic until magicFillTarget is reached
            gSaveContext.magic += 4;

            if (gSaveContext.gameMode == GAMEMODE_NORMAL && !IS_CUTSCENE_LAYER) {
                Audio_PlaySfxGeneral(NA_SE_SY_GAUGE_UP - SFX_FLAG, &gSfxDefaultPos, 4, &gSfxDefaultFreqAndVolScale,
                                     &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
            }

            // "Storage  MAGIC_NOW=%d (%d)"
            osSyncPrintf("蓄電  MAGIC_NOW=%d (%d)\n", gSaveContext.magic, gSaveContext.magicFillTarget);

            if (gSaveContext.magic >= gSaveContext.magicFillTarget) {
                gSaveContext.magic = gSaveContext.magicFillTarget;
                gSaveContext.magicState = gSaveContext.prevMagicState;
                gSaveContext.prevMagicState = MAGIC_STATE_IDLE;
            }
            break;

        case MAGIC_STATE_CONSUME_SETUP:
            // Sets the speed at which magic border flashes
            sMagicBorderRatio = 2;
            gSaveContext.magicState = MAGIC_STATE_CONSUME;
            break;

        case MAGIC_STATE_CONSUME:
            // Consume magic until target is reached or no more magic is available
            gSaveContext.magic -= 2;
            if (gSaveContext.magic <= 0) {
                gSaveContext.magic = 0;
                gSaveContext.magicState = MAGIC_STATE_METER_FLASH_1;
                sMagicBorderR = sMagicBorderG = sMagicBorderB = 255;
            } else if (gSaveContext.magic == gSaveContext.magicTarget) {
                gSaveContext.magicState = MAGIC_STATE_METER_FLASH_1;
                sMagicBorderR = sMagicBorderG = sMagicBorderB = 255;
            }
            FALLTHROUGH; // Flash border while magic is being consumed
        case MAGIC_STATE_METER_FLASH_1:
        case MAGIC_STATE_METER_FLASH_2:
        case MAGIC_STATE_METER_FLASH_3:
            temp = sMagicBorderIndices[sMagicBorderStep];
            borderChangeR = ABS(sMagicBorderR - sMagicBorderColors[temp][0]) / sMagicBorderRatio;
            borderChangeG = ABS(sMagicBorderG - sMagicBorderColors[temp][1]) / sMagicBorderRatio;
            borderChangeB = ABS(sMagicBorderB - sMagicBorderColors[temp][2]) / sMagicBorderRatio;

            if (sMagicBorderR >= sMagicBorderColors[temp][0]) {
                sMagicBorderR -= borderChangeR;
            } else {
                sMagicBorderR += borderChangeR;
            }

            if (sMagicBorderG >= sMagicBorderColors[temp][1]) {
                sMagicBorderG -= borderChangeG;
            } else {
                sMagicBorderG += borderChangeG;
            }

            if (sMagicBorderB >= sMagicBorderColors[temp][2]) {
                sMagicBorderB -= borderChangeB;
            } else {
                sMagicBorderB += borderChangeB;
            }

            sMagicBorderRatio--;
            if (sMagicBorderRatio == 0) {
                sMagicBorderR = sMagicBorderColors[temp][0];
                sMagicBorderG = sMagicBorderColors[temp][1];
                sMagicBorderB = sMagicBorderColors[temp][2];
                sMagicBorderRatio = YREG(40 + sMagicBorderStep);
                sMagicBorderStep++;
                if (sMagicBorderStep >= 4) {
                    sMagicBorderStep = 0;
                }
            }
            break;

        case MAGIC_STATE_RESET:
            sMagicBorderR = sMagicBorderG = sMagicBorderB = 255;
            gSaveContext.magicState = MAGIC_STATE_IDLE;
            break;

        case MAGIC_STATE_CONSUME_LENS:
            // Slowly consume magic while lens is on
            if ((play->pauseCtx.state == 0) && (play->pauseCtx.debugState == 0) && (msgCtx->msgMode == MSGMODE_NONE) &&
                (play->gameOverCtx.state == GAMEOVER_INACTIVE) && (play->transitionTrigger == TRANS_TRIGGER_OFF) &&
                (play->transitionMode == TRANS_MODE_OFF) && !Play_InCsMode(play)) {
                if ((gSaveContext.magic == 0) //|| ((func_8008F2F8(play) >= 2) && (func_8008F2F8(play) < 5)) 
                    ||
                    !play->actorCtx.lensActive) {
                    // Force lens off and set magic meter state to idle
                    play->actorCtx.lensActive = false;
                    Audio_PlaySfxGeneral(NA_SE_SY_GLASSMODE_OFF, &gSfxDefaultPos, 4, &gSfxDefaultFreqAndVolScale,
                                         &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
                    gSaveContext.magicState = MAGIC_STATE_IDLE;
                    sMagicBorderR = sMagicBorderG = sMagicBorderB = 255;
                    break;
                }

                interfaceCtx->lensMagicConsumptionTimer--;
                if (interfaceCtx->lensMagicConsumptionTimer == 0) {
                    gSaveContext.magic--;
                    interfaceCtx->lensMagicConsumptionTimer = 80;
                }
            }

            temp = sMagicBorderIndices[sMagicBorderStep];
            borderChangeR = ABS(sMagicBorderR - sMagicBorderColors[temp][0]) / sMagicBorderRatio;
            borderChangeG = ABS(sMagicBorderG - sMagicBorderColors[temp][1]) / sMagicBorderRatio;
            borderChangeB = ABS(sMagicBorderB - sMagicBorderColors[temp][2]) / sMagicBorderRatio;

            if (sMagicBorderR >= sMagicBorderColors[temp][0]) {
                sMagicBorderR -= borderChangeR;
            } else {
                sMagicBorderR += borderChangeR;
            }

            if (sMagicBorderG >= sMagicBorderColors[temp][1]) {
                sMagicBorderG -= borderChangeG;
            } else {
                sMagicBorderG += borderChangeG;
            }

            if (sMagicBorderB >= sMagicBorderColors[temp][2]) {
                sMagicBorderB -= borderChangeB;
            } else {
                sMagicBorderB += borderChangeB;
            }

            sMagicBorderRatio--;
            if (sMagicBorderRatio == 0) {
                sMagicBorderR = sMagicBorderColors[temp][0];
                sMagicBorderG = sMagicBorderColors[temp][1];
                sMagicBorderB = sMagicBorderColors[temp][2];
                sMagicBorderRatio = YREG(40 + sMagicBorderStep);
                sMagicBorderStep++;
                if (sMagicBorderStep >= 4) {
                    sMagicBorderStep = 0;
                }
            }
            break;

        case MAGIC_STATE_ADD:
            // Add magic until target is reached
            gSaveContext.magic += 4;
            Audio_PlaySfxGeneral(NA_SE_SY_GAUGE_UP - SFX_FLAG, &gSfxDefaultPos, 4, &gSfxDefaultFreqAndVolScale,
                                 &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
            if (gSaveContext.magic >= gSaveContext.magicTarget) {
                gSaveContext.magic = gSaveContext.magicTarget;
                gSaveContext.magicState = gSaveContext.prevMagicState;
                gSaveContext.prevMagicState = MAGIC_STATE_IDLE;
            }
            break;

        default:
            gSaveContext.magicState = MAGIC_STATE_IDLE;
            break;
    }
}