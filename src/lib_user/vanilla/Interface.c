#include <uLib.h>
#include <code/z_parameter.h>
#include <code/z_map_exp.h>
#include "state/state.h"

asm ("sSetupDL_80125A60 = 0x80125A60;");
asm ("D_80125A5C = 0x80125A5C;");
asm ("sHBAScoreDigits = 0x80125A5C");
asm ("D_80125A58 = 0x80125A58");
asm ("sHBAScoreTier = 0x80125A04");
asm ("sMagicBorderG = 0x80125A2C");
asm ("sMagicBorderB = 0x80125A30");
asm ("sMagicBorderR = 0x80125A28");
asm ("sCUpInvisible = 0x80125A10");
asm ("sCUpTimer = 0x80125A14");

 u8 sDpadTexture[0x200];
u32 sDpadIcons[4][32 * 32];
int sDPadInit;
const u32 sDPadItemID[] = {
    ITEM_LENS,
    ITEM_BOOTS_HOVER,
    ITEM_BOTTLE,
    ITEM_BOOTS_IRON,
};


static void DPadIcons_Init(PlayState* play) {
    u32 begin = gDmaDataTable[7].vromStart;
    if(ghackSave.quickPotionItem != 0 ){
        DmaMgr_SendRequest1(sDpadIcons[2], begin + ghackSave.quickPotionItem * 0x1000, sizeof(Color_RGBA8[32 * 32]), __FILE__, __LINE__);
        //Debug_Text(255,0,0,2,5,"quickPotionItem : %d",ghackSave.quickPotionItem);
    }else{
        DmaMgr_SendRequest1(sDpadIcons[2], begin + sDPadItemID[2] * 0x1000, sizeof(Color_RGBA8[32 * 32]), __FILE__, __LINE__);
        //Debug_Text(255,0,0,2,5,"Empty Bottle %d",ghackSave.quickPotionItem);
    }
    if (sDPadInit) return;
          DmaMgr_SendRequest1(sDpadTexture, gDmaDataTable[0x30].vromStart, 0x200, __FILE__, __LINE__);
  
    for (int i = 0; i < 4; i++)
        if(i != 2){
            DmaMgr_SendRequest1(sDpadIcons[i], begin + sDPadItemID[i] * 0x1000, sizeof(Color_RGBA8[32 * 32]), __FILE__, __LINE__);
        }
        
    sDPadInit = true;
}

static Vtx* Graph_AllocQuad(s16 x, s16 y, s16 width, s16 height, s16 u, s16 v) {
    Vtx* vtx = Graph_Alloc(__gfxCtx, 4 * sizeof(Vtx));
    
    vtx[0] = gdSPDefVtxC(x,         y + height, 0, 0, 0, 255, 255, 255, 255);
    vtx[1] = gdSPDefVtxC(x + width, y + height, 0, u, 0, 255, 255, 255, 255);
    vtx[2] = gdSPDefVtxC(x,         y,          0, 0, v, 255, 255, 255, 255);
    vtx[3] = gdSPDefVtxC(x + width, y,          0, u, v, 255, 255, 255, 255);
    
    return vtx;
}

static void DPadIcons_DrawIcons(PlayState* play) {
    Player* player = GET_PLAYER(play);
    Vec2f center = { 115, 45 };
    f32 spread = 14;
    f32 img_scale = 0.5f;
    


    Vec2f offset[] = {
        { 0.1f, 1     },
        { 1.1,    0     },
        { 0,    -1   },
        { -1.1,   0     },
    };
    
    static int visible = true;
    static f32 alpha;
    f32 alpha_target = 1.0f;
    
    if (play->frameAdvCtx.enabled == true || Play_InCsMode(play) || (player->stateFlags1 & (/*PLAYER_STATE1_RIDING_HORSE | */PLAYER_STATE1_TALKING | PLAYER_STATE1_EXITING_SCENE | PLAYER_STATE1_INPUT_DISABLED)) || (play->pauseCtx.state != 0) || play->msgCtx.ocarinaMode == 1) {
        alpha_target = 0.0f;
    }else if (player->stateFlags1 & (PLAYER_STATE1_SWINGING_BOTTLE | PLAYER_STATE1_CHARGING_SPIN_ATTACK) || player->stateFlags2 & (PLAYER_STATE2_RESTRAINED_BY_ENEMY| PLAYER_STATE2_FROZEN_IN_ICE | PLAYER_STATE2_MOVING_PUSH_PULL_WALL)){
        alpha_target = 0.5f;
    }

    Math_SmoothStepToF(&alpha, alpha_target, 0.9f, 0.05f, 0.005f);
    
    if (alpha == 0.0f)
        return;
    Vtx* quad = Graph_AllocQuad(-14, -14, 28, 28, 32, 32);

    gDPPipeSync(OVERLAY_DISP++);
    Gfx_SetupDL_42Overlay(play->state.gfxCtx);
    gSPClearGeometryMode(OVERLAY_DISP++, G_CULL_BOTH);
    gDPSetCombineMode(OVERLAY_DISP++, TEXEL0, 0, PRIMITIVE, 0, TEXEL0, 0, PRIMITIVE, 0, TEXEL0, 0, PRIMITIVE, 0, TEXEL0, 0, PRIMITIVE, 0);



    Matrix_Translate(center.x, center.y, 0, MTXMODE_NEW);
    Matrix_Scale(img_scale, img_scale, img_scale, MTXMODE_APPLY);
    gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 128, 128, 128, 0xFF * alpha);
    gSPMatrix(OVERLAY_DISP++, Matrix_NewMtx(__gfxCtx, __FILE__, __LINE__), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_MODELVIEW);
    gDPLoadTextureBlock(OVERLAY_DISP++, sDpadTexture, G_IM_FMT_I, G_IM_SIZ_4b, 32, 32, 0, G_TX_CLAMP, G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);
    gSPVertex(OVERLAY_DISP++, quad, 4, 0);
    gSP1Quadrangle(OVERLAY_DISP++, 0, 2, 3, 1, 0);


        gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, 255*alpha);
    
    for (int i = 0; i < 4; i++) {
        if( i == 0 || i == 2){
            if(i == 0){
                if (INV_CONTENT(sDPadItemID[i]) != sDPadItemID[i]){
                    continue;
                }
            }else if(ghackSave.quickPotionItem == 0 && (gSaveContext.inventory.items[SLOT_BOTTLE_1]) == ITEM_NONE){
                 continue;
            }
        }else{
            if (!CHECK_OWNED_EQUIP(EQUIP_TYPE_BOOTS, sDPadItemID[i] - ITEM_BOOTS_KOKIRI)){
                continue;
            }
        }

        Matrix_Translate(center.x + offset[i].x * spread, center.y + offset[i].y * spread, 0, MTXMODE_NEW);
        Matrix_Scale(img_scale, img_scale, img_scale, MTXMODE_APPLY);
        gSPMatrix(OVERLAY_DISP++, Matrix_NewMtx(__gfxCtx, __FILE__, __LINE__), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_MODELVIEW);
        gDPLoadTextureBlock(OVERLAY_DISP++, sDpadIcons[i], G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, G_TX_CLAMP, G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);
        gSPVertex(OVERLAY_DISP++, quad, 4, 0);
        gSP1Quadrangle(OVERLAY_DISP++, 0, 2, 3, 1, 0);
    }
}

Asm_VanillaHook(Interface_Draw);
void Interface_Draw(PlayState* play) {
    static s16 magicArrowEffectsR[] = { 255, 100, 255 };
    static s16 magicArrowEffectsG[] = { 0, 100, 255 };
    static s16 magicArrowEffectsB[] = { 0, 255, 100 };
    static s16 timerDigitLeftPos[] = { 16, 25, 34, 42, 51 };
    static s16 digitWidth[] = { 9, 9, 8, 9, 9 };
    static s16 rupeeDigitsFirst[] = { 1, 0, 0 };
    static s16 rupeeDigitsCount[] = { 2, 3, 3 };
    static s16 spoilingItemEntrances[] = { ENTR_SPOT10_2, ENTR_SPOT07_3, ENTR_SPOT07_3 };
    static s16 D_8015FFE0;
    static s16 D_8015FFE2;
    static s16 D_8015FFE4;
    static s16 D_8015FFE6;
    static s16 timerDigits[5];
    InterfaceContext* interfaceCtx = &play->interfaceCtx;
    PauseContext* pauseCtx = &play->pauseCtx;
    MessageContext* msgCtx = &play->msgCtx;
    Player* player = GET_PLAYER(play);
    s16 svar1;
    s16 svar2;
    s16 svar3;
    s16 svar5;
    s16 svar6;
    s16 AbuttonYoffset;
    s16 mergeHideFlag;
    s16* heartChangeflag = &interfaceCtx->unk_226;
    s16* keyAlpha = &interfaceCtx->unk_228;
    u16* rupeeAlpha = &interfaceCtx->minimapAlpha;

    OPEN_DISPS(play->state.gfxCtx, "../z_parameter.c", 3405);
    
    gSPSegment(OVERLAY_DISP++, 0x02, interfaceCtx->parameterSegment);
    gSPSegment(OVERLAY_DISP++, 0x07, interfaceCtx->doActionSegment);
    gSPSegment(OVERLAY_DISP++, 0x08, interfaceCtx->iconItemSegment);
    gSPSegment(OVERLAY_DISP++, 0x0B, interfaceCtx->mapSegment);
    
#ifdef DEV_BUILD
    
    if (gLibCtx.cinematic && play->pauseCtx.state == 0) {
        Letterbox_SetSizeTarget(0x20);
        
        return;
    }
    
#endif

    if (pauseCtx->debugState == 0) {
        Interface_InitVertices(play);
        func_8008A994(interfaceCtx);
        
        if(play->frameAdvCtx.enabled == true || Play_InCsMode(play) || player->stateFlags1 & PLAYER_STATE1_RIDING_HORSE || (play->pauseCtx.state != 0) || play->msgCtx.ocarinaMode == 1){
            mergeHideFlag = true;
        }else{
            mergeHideFlag = false;
        }

        if(mergeHideFlag == false){
            if(R_MINIMAP_DISABLED == false){//open
                ghackSave.hudFadeTimer = 1;
            }else{
                ghackSave.hudFadeTimer = 0;
            }
            if(ghackSave.hudFadeTimer == 1){
                interfaceCtx->healthAlpha = 255;
                interfaceCtx->magicAlpha = 255;
                *keyAlpha =  255;
                *rupeeAlpha = 255;         
            }

            if(*heartChangeflag){
                interfaceCtx->healthAlpha = 255;
                *heartChangeflag = false;
            }
            if(gSaveContext.healthAccumulator != 0){
                interfaceCtx->healthAlpha = 255;
            }

            if((player->itemActionParam ==PLAYER_AP_BOTTLE_POTION_BLUE) || (player->itemActionParam == PLAYER_AP_BOTTLE_POTION_GREEN)){
                interfaceCtx->magicAlpha = 255;
            }

            if(gSaveContext.health <= gSaveContext.healthCapacity/2 || (play->actorCtx.targetCtx.bgmEnemy != NULL)){
                interfaceCtx->healthAlpha += 5;
            }else if((ghackSave.hudFadeTimer == 0) && (interfaceCtx->healthAlpha > 0) && (gSaveContext.healthAccumulator == 0)){
                interfaceCtx->healthAlpha -= 5;
            }
            
            if(gSaveContext.magicState != MAGIC_STATE_IDLE || ((gSaveContext.magic <= gSaveContext.magicCapacity/2) &&(gSaveContext.magicLevel))){
                interfaceCtx->magicAlpha = 255 ;
            }else if((interfaceCtx->magicAlpha > 0) && (ghackSave.hudFadeTimer == 0)) {
                interfaceCtx->magicAlpha -= 5;
            }            

        }else{
            if(!(player->stateFlags1 & PLAYER_STATE1_RIDING_HORSE)){    //for special scenrios like getting pieces of hearts or acquring extra magic.
                if(gSaveContext.healthAccumulator != 0){
                    interfaceCtx->healthAlpha = 255;
                }else{
                    interfaceCtx->healthAlpha = 0;
                }

                interfaceCtx->magicAlpha = 0;
            
            } 
            *keyAlpha = *rupeeAlpha = 0;
        }
        
        if(*keyAlpha > 255){
            *keyAlpha = 255;
        }else if(*keyAlpha < 0){
            *keyAlpha = 0;
        }if(*rupeeAlpha > 255){
            *rupeeAlpha = 255;
        }else if(*rupeeAlpha < 0){
            *rupeeAlpha = 0;
        }else if(interfaceCtx->healthAlpha > 255){
            interfaceCtx->healthAlpha = 255;
        }else if(interfaceCtx->magicAlpha >255){
            interfaceCtx->magicAlpha = 255;

        }

        if(gSaveContext.rupeeAccumulator!=0){
            *rupeeAlpha = 255;
        }else if(*rupeeAlpha > 0){
            *rupeeAlpha= *rupeeAlpha - 5;
        }

        if(*keyAlpha > 0){
            *keyAlpha= *keyAlpha - 5;
        }

        Health_DrawMeter(play);
        DPadIcons_Init(play);
        DPadIcons_DrawIcons(play);
        

        Gfx_SetupDL_39Overlay(play->state.gfxCtx);
        // Rupee Icon
        static Color_RGB8 const walletColors[] = {
            { 200, 255, 100 },
            { 130, 130, 255 },
            { 255, 100, 100 },
        };
        u8 walletUpg = CUR_UPG_VALUE(UPG_WALLET);
        gDPSetPrimColor(OVERLAY_DISP++, 0, 0, walletColors[walletUpg].r, walletColors[walletUpg].g,
                    walletColors[walletUpg].b, *rupeeAlpha);

        //gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 200, 255, 100, *rupeeAlpha);
        gDPSetEnvColor(OVERLAY_DISP++, 0, 80, 0, 255);
        OVERLAY_DISP = Gfx_TextureIA8(OVERLAY_DISP, gRupeeCounterIconTex, 16, 16, 26, 206, 16, 16, 1 << 10, 1 << 10);


        switch (play->sceneId) {
            case 0x00:
            case 0x01:
            case 0x02:
            case 0x03:
            case SCENE_HIDAN:
            case SCENE_MIZUSIN:
            case SCENE_JYASINZOU:
            case SCENE_HAKADAN:
            case SCENE_HAKADANCH:
            case SCENE_ICE_DOUKUTO:
            case SCENE_GANON:
            case SCENE_MEN:
            case SCENE_GERUDOWAY:
            case SCENE_GANONTIKA:
            case SCENE_GANON_SONOGO:
            case SCENE_GANONTIKA_SONOGO:
            case SCENE_TAKARAYA:
                if (gSaveContext.inventory.dungeonKeys[gSaveContext.mapIndex] >= 0) {
                    // Small Key Icon
                    gDPPipeSync(OVERLAY_DISP++);
                    gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 200, 230, 255, *keyAlpha);
                    gDPSetEnvColor(OVERLAY_DISP++, 0, 0, 20, 255);
                    OVERLAY_DISP = Gfx_TextureIA8(
                        OVERLAY_DISP, gSmallKeyCounterIconTex, 16, 16, 26, 190, 16, 16,
                        1 << 10, 1 << 10
                    );
                    
                    // Small Key Counter
                    gDPPipeSync(OVERLAY_DISP++);
                    gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, *keyAlpha);
                    gDPSetCombineLERP(
                        OVERLAY_DISP++, 0, 0, 0, PRIMITIVE, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, PRIMITIVE,
                        TEXEL0, 0, PRIMITIVE, 0
                    );
                    
                    interfaceCtx->counterDigits[2] = 0;
                    interfaceCtx->counterDigits[3] = gSaveContext.inventory.dungeonKeys[gSaveContext.mapIndex];
                    
                    while (interfaceCtx->counterDigits[3] >= 10) {
                        interfaceCtx->counterDigits[2]++;
                        interfaceCtx->counterDigits[3] -= 10;
                    }
                    
                    svar3 = 42;
                    
                    if (interfaceCtx->counterDigits[2] != 0) {
                        OVERLAY_DISP = Gfx_TextureI8(
                            OVERLAY_DISP, ((u8*)gCounterDigit0Tex + (8 * 16 * interfaceCtx->counterDigits[2])), 8, 16,
                            svar3, 190, 8, 16, 1 << 10, 1 << 10
                        );
                        svar3 += 8;
                    }
                    
                    OVERLAY_DISP = Gfx_TextureI8(
                        OVERLAY_DISP,
                        ((u8*)gCounterDigit0Tex + (8 * 16 * interfaceCtx->counterDigits[3])),
                        8, 16, svar3, 190, 8, 16, 1 << 10, 1 << 10
                    );
                }
                break;
            default:
                // if (gSaveContext.inventory.dungeonKeys[gSaveContext.mapIndex] >= 0) {
                //     gDPPipeSync(OVERLAY_DISP++);
                //     gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 200, 230, 255, *keyAlpha);
                //     gDPSetEnvColor(OVERLAY_DISP++, 0, 0, 20, 255);
                //     OVERLAY_DISP = Gfx_TextureIA8(
                //         OVERLAY_DISP, gSmallKeyCounterIconTex, 16, 16, 26, 190, 16, 16,
                //         1 << 10, 1 << 10
                //     );
                    
                //     gDPPipeSync(OVERLAY_DISP++);
                //     gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, *keyAlpha);
                //     gDPSetCombineLERP(
                //         OVERLAY_DISP++, 0, 0, 0, PRIMITIVE, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, PRIMITIVE,
                //         TEXEL0, 0, PRIMITIVE, 0
                //     );
                    
                //     interfaceCtx->counterDigits[2] = 0;
                //     interfaceCtx->counterDigits[3] = gSaveContext.inventory.dungeonKeys[gSaveContext.mapIndex];
                    
                //     while (interfaceCtx->counterDigits[3] >= 10) {
                //         interfaceCtx->counterDigits[2]++;
                //         interfaceCtx->counterDigits[3] -= 10;
                //     }
                    
                //     svar3 = 42;
                    
                //     if (interfaceCtx->counterDigits[2] != 0) {
                //         OVERLAY_DISP = Gfx_TextureI8(
                //             OVERLAY_DISP, ((u8*)gCounterDigit0Tex + (8 * 16 * interfaceCtx->counterDigits[2])), 8, 16,
                //             svar3, 190, 8, 16, 1 << 10, 1 << 10
                //         );
                //         svar3 += 8;
                //     }
                    
                //     OVERLAY_DISP = Gfx_TextureI8(
                //         OVERLAY_DISP,
                //         ((u8*)gCounterDigit0Tex + (8 * 16 * interfaceCtx->counterDigits[3])),
                //         8, 16, svar3, 190, 8, 16, 1 << 10, 1 << 10
                //     );
                // }
                break;
        }
        

        // Rupee Counter
        gDPPipeSync(OVERLAY_DISP++);
        
        if (gSaveContext.rupees == CUR_CAPACITY(UPG_WALLET)) {
            gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 120, 255, 0, *rupeeAlpha);
        } else if (gSaveContext.rupees != 0) {
            gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, *rupeeAlpha);
        } else {
            gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 100, 100, 100, *rupeeAlpha);
        }
        
        gDPSetCombineLERP(
            OVERLAY_DISP++, 0, 0, 0, PRIMITIVE, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, PRIMITIVE, TEXEL0, 0,
            PRIMITIVE, 0
        );
        
        interfaceCtx->counterDigits[0] = interfaceCtx->counterDigits[1] = 0;
        interfaceCtx->counterDigits[2] = gSaveContext.rupees;
        
        if ((interfaceCtx->counterDigits[2] > 9999) || (interfaceCtx->counterDigits[2] < 0)) {
            interfaceCtx->counterDigits[2] &= 0xDDD;
        }
        
        while (interfaceCtx->counterDigits[2] >= 100) {
            interfaceCtx->counterDigits[0]++;
            interfaceCtx->counterDigits[2] -= 100;
        }
        
        while (interfaceCtx->counterDigits[2] >= 10) {
            interfaceCtx->counterDigits[1]++;
            interfaceCtx->counterDigits[2] -= 10;
        }
        
        svar2 = rupeeDigitsFirst[CUR_UPG_VALUE(UPG_WALLET)];
        svar5 = rupeeDigitsCount[CUR_UPG_VALUE(UPG_WALLET)];
        
        for (svar1 = 0, svar3 = 42; svar1 < svar5; svar1++, svar2++, svar3 += 8) {
            OVERLAY_DISP =
                Gfx_TextureI8(
                OVERLAY_DISP, ((u8*)gCounterDigit0Tex + (8 * 16 * interfaceCtx->counterDigits[svar2])), 8,
                16, svar3, 206, 8, 16, 1 << 10, 1 << 10
                );
        }
        
        Magic_DrawMeter(play);//MAGIC!
        Minimap_Draw(play);
        
        if ((R_PAUSE_MENU_MODE != 2) && (R_PAUSE_MENU_MODE != 3)) {
            func_8002C124(&play->actorCtx.targetCtx, play); // Draw Z-Target
        }
        
        Gfx_SetupDL_39Overlay(play->state.gfxCtx);
        
        Interface_DrawItemButtons(play); //It draw most background button textures, and Navi text.
        
        gDPPipeSync(OVERLAY_DISP++);
        gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, interfaceCtx->bAlpha);
        gDPSetCombineMode(OVERLAY_DISP++, G_CC_MODULATERGBA_PRIM, G_CC_MODULATERGBA_PRIM);
        
        if (!(interfaceCtx->unk_1FA)) {
            // B Button Icon & Ammo Count
            if (gSaveContext.equips.buttonItems[0] != ITEM_NONE) {
                //Interface_DrawItemIconTexture(play, interfaceCtx->iconItemSegment, 0); /Draw Sowrd Icon.
                
                if (
                     (player->stateFlags1 & PLAYER_STATE1_23) || (play->shootingGalleryStatus > 1) ||
                     ((play->sceneId == SCENE_BOWLING) && Flags_GetSwitch(play, 0x38))
                ) {
                    gDPPipeSync(OVERLAY_DISP++);
                    gDPSetCombineLERP(
                        OVERLAY_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE,
                        0, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0
                    );
                    Interface_DrawAmmoCount(play, 0, interfaceCtx->bAlpha);
                }
            }
        } else {
            // B Button Do Action Label
            gDPPipeSync(OVERLAY_DISP++);
            gDPSetCombineLERP(
                OVERLAY_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0,
                PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0
            );
            gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, interfaceCtx->bAlpha);
            
            gDPLoadTextureBlock_4b(
                OVERLAY_DISP++, interfaceCtx->doActionSegment + DO_ACTION_TEX_SIZE, G_IM_FMT_IA,
                DO_ACTION_TEX_WIDTH, DO_ACTION_TEX_HEIGHT, 0, G_TX_NOMIRROR | G_TX_WRAP,
                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD
            );
            
            R_B_LABEL_DD = (1 << 10) / (WREG(37 + gSaveContext.language) / 100.0f);
            gSPTextureRectangle(
                OVERLAY_DISP++, R_B_LABEL_X(gSaveContext.language) << 8,
                    R_B_LABEL_Y(gSaveContext.language) << 8,
                    (R_B_LABEL_X(gSaveContext.language) + DO_ACTION_TEX_WIDTH) << 2,
                    (R_B_LABEL_Y(gSaveContext.language) + DO_ACTION_TEX_HEIGHT) << 2, G_TX_RENDERTILE, 0, 0,
                    R_B_LABEL_DD, R_B_LABEL_DD
            );
        }
        
        gDPPipeSync(OVERLAY_DISP++);
        
        // C-Left Button Icon & Ammo Count
        if (gSaveContext.equips.buttonItems[1] < 0xF0) {
            gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, interfaceCtx->cLeftAlpha);
            gDPSetCombineMode(OVERLAY_DISP++, G_CC_MODULATERGBA_PRIM, G_CC_MODULATERGBA_PRIM);
            Interface_DrawItemIconTexture(play, interfaceCtx->iconItemSegment + 0x1000, 1);
            gDPPipeSync(OVERLAY_DISP++);
            gDPSetCombineLERP(
                OVERLAY_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0,
                PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0
            );
            Interface_DrawAmmoCount(play, 1, interfaceCtx->cLeftAlpha);
        }
        
        gDPPipeSync(OVERLAY_DISP++);
        
        // C-Down Button Icon & Ammo Count
        if (gSaveContext.equips.buttonItems[2] < 0xF0) {
            gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, interfaceCtx->cDownAlpha);
            gDPSetCombineMode(OVERLAY_DISP++, G_CC_MODULATERGBA_PRIM, G_CC_MODULATERGBA_PRIM);
            Interface_DrawItemIconTexture(play, interfaceCtx->iconItemSegment + 0x2000, 2);
            gDPPipeSync(OVERLAY_DISP++);
            gDPSetCombineLERP(
                OVERLAY_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0,
                PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0
            );
            Interface_DrawAmmoCount(play, 2, interfaceCtx->cDownAlpha);
        }
        
        gDPPipeSync(OVERLAY_DISP++);
        
        // C-Right Button Icon & Ammo Count
        if (gSaveContext.equips.buttonItems[3] < 0xF0) {
            gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, interfaceCtx->cRightAlpha);
            gDPSetCombineMode(OVERLAY_DISP++, G_CC_MODULATERGBA_PRIM, G_CC_MODULATERGBA_PRIM);
            Interface_DrawItemIconTexture(play, interfaceCtx->iconItemSegment + 0x3000, 3);
            gDPPipeSync(OVERLAY_DISP++);
            gDPSetCombineLERP(
                OVERLAY_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0,
                PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0
            );
            Interface_DrawAmmoCount(play, 3, interfaceCtx->cRightAlpha);
        }
        
        // A Button
        AbuttonYoffset = XREG(20);//XREG(16);
        s16 AbuttonXoffset =  XREG(24);// XREG(20)

            Gfx_SetupDL_42Overlay(play->state.gfxCtx);
            func_8008A8B8(play, AbuttonYoffset, AbuttonYoffset + 45, AbuttonYoffset, AbuttonYoffset + 45);
            gSPClearGeometryMode(OVERLAY_DISP++, G_CULL_BOTH);
            gDPSetCombineMode(OVERLAY_DISP++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
            gDPSetPrimColor(
                OVERLAY_DISP++, 0, 0, R_A_BTN_COLOR(0), R_A_BTN_COLOR(1), R_A_BTN_COLOR(2),
                interfaceCtx->aAlpha
            );
        
        //Interface_DrawActionButton(play); //Draw Do actin Circle icon
        gDPPipeSync(OVERLAY_DISP++);
        func_8008A8B8(play, AbuttonYoffset, AbuttonYoffset + 45, AbuttonXoffset, AbuttonXoffset + 45);
        gSPSetGeometryMode(OVERLAY_DISP++, G_CULL_BACK);
        gDPSetCombineLERP(
            OVERLAY_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0,
            PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0
        );
        gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, interfaceCtx->aAlpha);
        gDPSetEnvColor(OVERLAY_DISP++, 0, 0, 0, 0);
        Matrix_Translate(0.0f, 0.0f, WREG(46 + gSaveContext.language) / 10.0f, MTXMODE_NEW);
        Matrix_Scale(1.0f, 1.0f, 1.0f, MTXMODE_APPLY);
        Matrix_RotateX(interfaceCtx->unk_1F4 / 10000.0f, MTXMODE_APPLY);
        gSPMatrix(
            OVERLAY_DISP++, Matrix_NewMtx(play->state.gfxCtx, "../z_parameter.c", 3701),
            G_MTX_MODELVIEW | G_MTX_LOAD
        );
        gSPVertex(OVERLAY_DISP++, &interfaceCtx->actionVtx[4], 4, 0);
        
        if(mergeHideFlag == false){
            if ((interfaceCtx->unk_1EC < 2) || (interfaceCtx->unk_1EC == 3)) {
                Interface_DrawActionLabel(play->state.gfxCtx, interfaceCtx->doActionSegment);
            } else {
                Interface_DrawActionLabel(play->state.gfxCtx, interfaceCtx->doActionSegment + DO_ACTION_TEX_SIZE);
            }
        }
        gDPPipeSync(OVERLAY_DISP++);
        
        func_8008A994(interfaceCtx);
        
        if ((pauseCtx->state == 6) && (pauseCtx->unk_1E4 == 3)) {
            // Inventory Equip Effects
            gSPSegment(OVERLAY_DISP++, 0x08, pauseCtx->iconItemSegment);
            Gfx_SetupDL_42Overlay(play->state.gfxCtx);
            gDPSetCombineMode(OVERLAY_DISP++, G_CC_MODULATERGBA_PRIM, G_CC_MODULATERGBA_PRIM);
            gSPMatrix(OVERLAY_DISP++, &gMtxClear, G_MTX_MODELVIEW | G_MTX_LOAD);
            
            pauseCtx->cursorVtx[16].v.ob[0] = pauseCtx->cursorVtx[18].v.ob[0] = pauseCtx->equipAnimX / 10;
            pauseCtx->cursorVtx[17].v.ob[0] = pauseCtx->cursorVtx[19].v.ob[0] =
                pauseCtx->cursorVtx[16].v.ob[0] + WREG(90) / 10;
            pauseCtx->cursorVtx[16].v.ob[1] = pauseCtx->cursorVtx[17].v.ob[1] = pauseCtx->equipAnimY / 10;
            pauseCtx->cursorVtx[18].v.ob[1] = pauseCtx->cursorVtx[19].v.ob[1] =
                pauseCtx->cursorVtx[16].v.ob[1] - WREG(90) / 10;
            
            if (pauseCtx->equipTargetItem < 0xBF) {
                // Normal Equip (icon goes from the inventory slot to the C button when equipping it)
                gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, pauseCtx->equipAnimAlpha);
                gSPVertex(OVERLAY_DISP++, &pauseCtx->cursorVtx[16], 4, 0);
                
                gDPLoadTextureBlock(
                    OVERLAY_DISP++, gItemIcons[pauseCtx->equipTargetItem], G_IM_FMT_RGBA, G_IM_SIZ_32b,
                    32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK,
                    G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD
                );
            } else {
                // Magic Arrow Equip Effect
                svar1 = pauseCtx->equipTargetItem - 0xBF;
                gDPSetPrimColor(
                    OVERLAY_DISP++, 0, 0, magicArrowEffectsR[svar1], magicArrowEffectsG[svar1],
                    magicArrowEffectsB[svar1], pauseCtx->equipAnimAlpha
                );
                
                if ((pauseCtx->equipAnimAlpha > 0) && (pauseCtx->equipAnimAlpha < 255)) {
                    svar1 = (pauseCtx->equipAnimAlpha / 8) / 2;
                    pauseCtx->cursorVtx[16].v.ob[0] = pauseCtx->cursorVtx[18].v.ob[0] =
                        pauseCtx->cursorVtx[16].v.ob[0] - svar1;
                    pauseCtx->cursorVtx[17].v.ob[0] = pauseCtx->cursorVtx[19].v.ob[0] =
                        pauseCtx->cursorVtx[16].v.ob[0] + svar1 * 2 + 32;
                    pauseCtx->cursorVtx[16].v.ob[1] = pauseCtx->cursorVtx[17].v.ob[1] =
                        pauseCtx->cursorVtx[16].v.ob[1] + svar1;
                    pauseCtx->cursorVtx[18].v.ob[1] = pauseCtx->cursorVtx[19].v.ob[1] =
                        pauseCtx->cursorVtx[16].v.ob[1] - svar1 * 2 - 32;
                }
                
                gSPVertex(OVERLAY_DISP++, &pauseCtx->cursorVtx[16], 4, 0);
                gDPLoadTextureBlock(
                    OVERLAY_DISP++, gMagicArrowEquipEffectTex, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0,
                    G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                    G_TX_NOLOD, G_TX_NOLOD
                );
            }
            
            gSP1Quadrangle(OVERLAY_DISP++, 0, 2, 3, 1, 0);
        }
        
        Gfx_SetupDL_39Overlay(play->state.gfxCtx);
        
        if ((play->pauseCtx.state == 0) && (play->pauseCtx.debugState == 0)) {
            if (gSaveContext.minigameState != 1) {
                // Carrots rendering if the action corresponds to riding a horse
                 if (interfaceCtx->unk_1EE == 8) {
                     // Load Carrot Icon
                      gDPLoadTextureBlock(
                          OVERLAY_DISP++, gCarrotIconTex, G_IM_FMT_RGBA, G_IM_SIZ_32b, 16, 16, 0,
                          G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                          G_TX_NOLOD, G_TX_NOLOD
                      );
                  
                     // Draw 6 carrots
                     for (svar1 = 1, svar5 = ZREG(14); svar1 < 7; svar1++, svar5 += 16) {
                         // Carrot Color (based on availability)
                         if ((interfaceCtx->numHorseBoosts == 0) || (interfaceCtx->numHorseBoosts < svar1)) {
                             gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 0, 150, 255, interfaceCtx->aAlpha);
                         } else {
                             gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, interfaceCtx->aAlpha);
                         }
                      
                         gSPTextureRectangle(
                             OVERLAY_DISP++, svar5 << 2, ZREG(15) << 2, (svar5 + 16) << 2,
                                 (ZREG(15) + 16) << 2, G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10
                         );
                     }
                 }
            } else {
                // Score for the Horseback Archery
                svar5 = WREG(32);
                gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, interfaceCtx->bAlpha);
                
                // Target Icon
                gDPLoadTextureBlock(
                    OVERLAY_DISP++, gArcheryScoreIconTex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 24, 16, 0,
                    G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                    G_TX_NOLOD, G_TX_NOLOD
                );
                
                gSPTextureRectangle(
                    OVERLAY_DISP++, (svar5 + 28) << 2, ZREG(15) << 2, (svar5 + 52) << 2,
                        (ZREG(15) + 16) << 2, G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10
                );
                
                // Score Counter
                gDPPipeSync(OVERLAY_DISP++);
                gDPSetCombineLERP(
                    OVERLAY_DISP++, 0, 0, 0, PRIMITIVE, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, PRIMITIVE,
                    TEXEL0, 0, PRIMITIVE, 0
                );
                
                svar5 = WREG(32) + 6 * 9;
                
                for (svar1 = svar2 = 0; svar1 < 4; svar1++) {
                    if (sHBAScoreDigits[svar1] != 0 || (svar2 != 0) || (svar1 >= 3)) {
                        OVERLAY_DISP = Gfx_TextureI8(
                            OVERLAY_DISP, ((u8*)gCounterDigit0Tex + (8 * 16 * sHBAScoreDigits[svar1])), 8, 16, svar5,
                            (ZREG(15) - 2), digitWidth[0], VREG(42), VREG(43) << 1, VREG(43) << 1
                        );
                        svar5 += 9;
                        svar2++;
                    }
                }
                
                gDPPipeSync(OVERLAY_DISP++);
                gDPSetCombineMode(OVERLAY_DISP++, G_CC_MODULATERGBA_PRIM, G_CC_MODULATERGBA_PRIM);
            }
        }
        
        if ((gSaveContext.timer2State == 5) && (Message_GetState(&play->msgCtx) == TEXT_STATE_EVENT)) {
            // Trade quest timer reached 0
            D_8015FFE6 = 40;
            gSaveContext.cutsceneIndex = 0;
            play->transitionTrigger = TRANS_TRIGGER_START;
            play->transitionType = TRANS_TYPE_FADE_WHITE;
            gSaveContext.timer2State = 0;
            
            if (
                (gSaveContext.equips.buttonItems[0] != ITEM_SWORD_KOKIRI) &&
                (gSaveContext.equips.buttonItems[0] != ITEM_SWORD_MASTER) &&
                (gSaveContext.equips.buttonItems[0] != ITEM_SWORD_BGS) &&
                (gSaveContext.equips.buttonItems[0] != ITEM_SWORD_KNIFE)
            ) {
                if (gSaveContext.buttonStatus[0] != BTN_ENABLED) {
                    gSaveContext.equips.buttonItems[0] = gSaveContext.buttonStatus[0];
                } else {
                    gSaveContext.equips.buttonItems[0] = ITEM_NONE;
                }
            }
            
            // Revert any spoiling trade quest items
            for (svar1 = 0; svar1 < ARRAY_COUNT(gSpoilingItems); svar1++) {
                if (INV_CONTENT(ITEM_TRADE_ADULT) == gSpoilingItems[svar1]) {
                    gSaveContext.eventInf[EVENTINF_HORSES_INDEX] &=
                        (u16) ~(EVENTINF_HORSES_STATE_MASK | EVENTINF_HORSES_HORSETYPE_MASK | EVENTINF_HORSES_05_MASK |
                        EVENTINF_HORSES_06_MASK | EVENTINF_HORSES_0F_MASK);
                    osSyncPrintf("EVENT_INF=%x\n", gSaveContext.eventInf[EVENTINF_HORSES_INDEX]);
                    play->nextEntranceIndex = spoilingItemEntrances[svar1];
                    INV_CONTENT(gSpoilingItemReverts[svar1]) = gSpoilingItemReverts[svar1];
                    
                    for (svar2 = 1; svar2 < 4; svar2++) {
                        if (gSaveContext.equips.buttonItems[svar2] == gSpoilingItems[svar1]) {
                            gSaveContext.equips.buttonItems[svar2] = gSpoilingItemReverts[svar1];
                            Interface_LoadItemIcon1(play, svar2);
                        }
                    }
                }
            }
        }
        
        if (
            (play->pauseCtx.state == 0) && (play->pauseCtx.debugState == 0) &&
            (play->gameOverCtx.state == GAMEOVER_INACTIVE) && (msgCtx->msgMode == MSGMODE_NONE) &&
            !(player->stateFlags2 & PLAYER_STATE2_24) && (play->transitionTrigger == TRANS_TRIGGER_OFF) &&
            (play->transitionMode == TRANS_MODE_OFF) && !Play_InCsMode(play) && (gSaveContext.minigameState != 1) &&
            (play->shootingGalleryStatus <= 1) && !((play->sceneId == SCENE_BOWLING) && Flags_GetSwitch(play, 0x38))
        ) {
            svar6 = 0;
            switch (gSaveContext.timer1State) {
                case 1:
                    D_8015FFE2 = 20;
                    D_8015FFE0 = 20;
                    gSaveContext.timer1Value = gSaveContext.health >> 1;
                    gSaveContext.timer1State = 2;
                    break;
                case 2:
                    D_8015FFE2--;
                    if (D_8015FFE2 == 0) {
                        D_8015FFE2 = 20;
                        gSaveContext.timer1State = 3;
                    }
                    break;
                case 5:
                case 11:
                    D_8015FFE2 = 20;
                    D_8015FFE0 = 20;
                    if (gSaveContext.timer1State == 5) {
                        gSaveContext.timer1State = 6;
                    } else {
                        gSaveContext.timer1State = 12;
                    }
                    break;
                case 6:
                case 12:
                    D_8015FFE2--;
                    if (D_8015FFE2 == 0) {
                        D_8015FFE2 = 20;
                        if (gSaveContext.timer1State == 6) {
                            gSaveContext.timer1State = 7;
                        } else {
                            gSaveContext.timer1State = 13;
                        }
                    }
                    break;
                case 3:
                case 7:
                    svar1 = (gSaveContext.timerX[0] - 26) / D_8015FFE2;
                    gSaveContext.timerX[0] -= svar1;
                    
                    if (gSaveContext.healthCapacity > 0xA0) {
                        svar1 = (gSaveContext.timerY[0] - 54) / D_8015FFE2;
                    } else {
                        svar1 = (gSaveContext.timerY[0] - 46) / D_8015FFE2;
                    }
                    gSaveContext.timerY[0] -= svar1;
                    
                    D_8015FFE2--;
                    if (D_8015FFE2 == 0) {
                        D_8015FFE2 = 20;
                        gSaveContext.timerX[0] = 26;
                        
                        if (gSaveContext.healthCapacity > 0xA0) {
                            gSaveContext.timerY[0] = 54;
                        } else {
                            gSaveContext.timerY[0] = 46;
                        }
                        
                        if (gSaveContext.timer1State == 3) {
                            gSaveContext.timer1State = 4;
                        } else {
                            gSaveContext.timer1State = 8;
                        }
                    }
                    FALLTHROUGH;
                case 4:
                case 8:
                    if ((gSaveContext.timer1State == 4) || (gSaveContext.timer1State == 8)) {
                        if (gSaveContext.healthCapacity > 0xA0) {
                            gSaveContext.timerY[0] = 54;
                        } else {
                            gSaveContext.timerY[0] = 46;
                        }
                    }
                    
                    if ((gSaveContext.timer1State >= 3) && (msgCtx->msgLength == 0)) {
                        D_8015FFE0--;
                        if (D_8015FFE0 == 0) {
                            if (gSaveContext.timer1Value != 0) {
                                gSaveContext.timer1Value--;
                            }
                            
                            D_8015FFE0 = 20;
                            
                            if (gSaveContext.timer1Value == 0) {
                                gSaveContext.timer1State = 10;
                                if (D_80125A5C) {
                                    gSaveContext.health = 0;
                                    play->damagePlayer(play, -(gSaveContext.health + 2));
                                }
                                D_80125A5C = false;
                            } else if (gSaveContext.timer1Value > 60) {
                                if (timerDigits[4] == 1) {
                                    Audio_PlaySfxGeneral(
                                        NA_SE_SY_MESSAGE_WOMAN, &gSfxDefaultPos, 4,
                                        &gSfxDefaultFreqAndVolScale, &gSfxDefaultFreqAndVolScale,
                                        &gSfxDefaultReverb
                                    );
                                }
                            } else if (gSaveContext.timer1Value >= 11) {
                                if (timerDigits[4] & 1) {
                                    Audio_PlaySfxGeneral(
                                        NA_SE_SY_WARNING_COUNT_N, &gSfxDefaultPos, 4,
                                        &gSfxDefaultFreqAndVolScale, &gSfxDefaultFreqAndVolScale,
                                        &gSfxDefaultReverb
                                    );
                                }
                            } else {
                                Audio_PlaySfxGeneral(
                                    NA_SE_SY_WARNING_COUNT_E, &gSfxDefaultPos, 4,
                                    &gSfxDefaultFreqAndVolScale, &gSfxDefaultFreqAndVolScale,
                                    &gSfxDefaultReverb
                                );
                            }
                        }
                    }
                    break;
                case 13:
                    svar1 = (gSaveContext.timerX[0] - 26) / D_8015FFE2;
                    gSaveContext.timerX[0] -= svar1;
                    
                    if (gSaveContext.healthCapacity > 0xA0) {
                        svar1 = (gSaveContext.timerY[0] - 54) / D_8015FFE2;
                    } else {
                        svar1 = (gSaveContext.timerY[0] - 46) / D_8015FFE2;
                    }
                    gSaveContext.timerY[0] -= svar1;
                    
                    D_8015FFE2--;
                    if (D_8015FFE2 == 0) {
                        D_8015FFE2 = 20;
                        gSaveContext.timerX[0] = 26;
                        if (gSaveContext.healthCapacity > 0xA0) {
                            gSaveContext.timerY[0] = 54;
                        } else {
                            gSaveContext.timerY[0] = 46;
                        }
                        
                        gSaveContext.timer1State = 14;
                    }
                    FALLTHROUGH;
                case 14:
                    if (gSaveContext.timer1State == 14) {
                        if (gSaveContext.healthCapacity > 0xA0) {
                            gSaveContext.timerY[0] = 54;
                        } else {
                            gSaveContext.timerY[0] = 46;
                        }
                    }
                    
                    if (gSaveContext.timer1State >= 3) {
                        D_8015FFE0--;
                        if (D_8015FFE0 == 0) {
                            gSaveContext.timer1Value++;
                            D_8015FFE0 = 20;
                            
                            if (gSaveContext.timer1Value == 3599) {
                                D_8015FFE2 = 40;
                                gSaveContext.timer1State = 15;
                            } else {
                                Audio_PlaySfxGeneral(
                                    NA_SE_SY_WARNING_COUNT_N, &gSfxDefaultPos, 4,
                                    &gSfxDefaultFreqAndVolScale, &gSfxDefaultFreqAndVolScale,
                                    &gSfxDefaultReverb
                                );
                            }
                        }
                    }
                    break;
                case 10:
                    if (gSaveContext.timer2State != 0) {
                        D_8015FFE6 = 20;
                        D_8015FFE4 = 20;
                        gSaveContext.timerX[1] = 140;
                        gSaveContext.timerY[1] = 80;
                        
                        if (gSaveContext.timer2State < 7) {
                            gSaveContext.timer2State = 2;
                        } else {
                            gSaveContext.timer2State = 8;
                        }
                        
                        gSaveContext.timer1State = 0;
                    } else {
                        gSaveContext.timer1State = 0;
                    }
                case 15:
                    break;
                default:
                    svar6 = 1;
                    switch (gSaveContext.timer2State) {
                        case 1:
                        case 7:
                            D_8015FFE6 = 20;
                            D_8015FFE4 = 20;
                            gSaveContext.timerX[1] = 140;
                            gSaveContext.timerY[1] = 80;
                            if (gSaveContext.timer2State == 1) {
                                gSaveContext.timer2State = 2;
                            } else {
                                gSaveContext.timer2State = 8;
                            }
                            break;
                        case 2:
                        case 8:
                            D_8015FFE6--;
                            if (D_8015FFE6 == 0) {
                                D_8015FFE6 = 20;
                                if (gSaveContext.timer2State == 2) {
                                    gSaveContext.timer2State = 3;
                                } else {
                                    gSaveContext.timer2State = 9;
                                }
                            }
                            break;
                        case 3:
                        case 9:
                            osSyncPrintf(
                                "event_xp[1]=%d,  event_yp[1]=%d  TOTAL_EVENT_TM=%d\n",
                                ((void)0, gSaveContext.timerX[1]), ((void)0, gSaveContext.timerY[1]),
                                gSaveContext.timer2Value
                            );
                            svar1 = (gSaveContext.timerX[1] - 26) / D_8015FFE6;
                            gSaveContext.timerX[1] -= svar1;
                            if (gSaveContext.healthCapacity > 0xA0) {
                                svar1 = (gSaveContext.timerY[1] - 54) / D_8015FFE6;
                            } else {
                                svar1 = (gSaveContext.timerY[1] - 46) / D_8015FFE6;
                            }
                            gSaveContext.timerY[1] -= svar1;
                            
                            D_8015FFE6--;
                            if (D_8015FFE6 == 0) {
                                D_8015FFE6 = 20;
                                gSaveContext.timerX[1] = 26;
                                
                                if (gSaveContext.healthCapacity > 0xA0) {
                                    gSaveContext.timerY[1] = 54;
                                } else {
                                    gSaveContext.timerY[1] = 46;
                                }
                                
                                if (gSaveContext.timer2State == 3) {
                                    gSaveContext.timer2State = 4;
                                } else {
                                    gSaveContext.timer2State = 10;
                                }
                            }
                            FALLTHROUGH;
                        case 4:
                        case 10:
                            if ((gSaveContext.timer2State == 4) || (gSaveContext.timer2State == 10)) {
                                if (gSaveContext.healthCapacity > 0xA0) {
                                    gSaveContext.timerY[1] = 54;
                                } else {
                                    gSaveContext.timerY[1] = 46;
                                }
                            }
                            
                            if (gSaveContext.timer2State >= 3) {
                                D_8015FFE4--;
                                if (D_8015FFE4 == 0) {
                                    D_8015FFE4 = 20;
                                    if (gSaveContext.timer2State == 4) {
                                        gSaveContext.timer2Value--;
                                        osSyncPrintf("TOTAL_EVENT_TM=%d\n", gSaveContext.timer2Value);
                                        
                                        if (gSaveContext.timer2Value <= 0) {
                                            if (
                                                !Flags_GetSwitch(play, 0x37) ||
                                                ((play->sceneId != SCENE_GANON_DEMO) &&
                                                (play->sceneId != SCENE_GANON_FINAL) &&
                                                (play->sceneId != SCENE_GANON_SONOGO) &&
                                                (play->sceneId != SCENE_GANONTIKA_SONOGO))
                                            ) {
                                                D_8015FFE6 = 40;
                                                gSaveContext.timer2State = 5;
                                                gSaveContext.cutsceneIndex = 0;
                                                Message_StartTextbox(play, 0x71B0, NULL);
                                                func_8002DF54(play, NULL, 8);
                                            } else {
                                                D_8015FFE6 = 40;
                                                gSaveContext.timer2State = 6;
                                            }
                                        } else if (gSaveContext.timer2Value > 60) {
                                            if (timerDigits[4] == 1) {
                                                Audio_PlaySfxGeneral(
                                                    NA_SE_SY_MESSAGE_WOMAN, &gSfxDefaultPos, 4,
                                                    &gSfxDefaultFreqAndVolScale,
                                                    &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb
                                                );
                                            }
                                        } else if (gSaveContext.timer2Value > 10) {
                                            if (timerDigits[4] & 1) {
                                                Audio_PlaySfxGeneral(
                                                    NA_SE_SY_WARNING_COUNT_N, &gSfxDefaultPos, 4,
                                                    &gSfxDefaultFreqAndVolScale,
                                                    &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb
                                                );
                                            }
                                        } else {
                                            Audio_PlaySfxGeneral(
                                                NA_SE_SY_WARNING_COUNT_E, &gSfxDefaultPos, 4,
                                                &gSfxDefaultFreqAndVolScale,
                                                &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb
                                            );
                                        }
                                    } else {
                                        gSaveContext.timer2Value++;
                                        if (GET_EVENTINF(EVENTINF_10)) {
                                            if (gSaveContext.timer2Value == 240) {
                                                Message_StartTextbox(play, 0x6083, NULL);
                                                CLEAR_EVENTINF(EVENTINF_10);
                                                gSaveContext.timer2State = 0;
                                            }
                                        }
                                    }
                                    
                                    if ((gSaveContext.timer2Value % 60) == 0) {
                                        Audio_PlaySfxGeneral(
                                            NA_SE_SY_WARNING_COUNT_N, &gSfxDefaultPos, 4,
                                            &gSfxDefaultFreqAndVolScale, &gSfxDefaultFreqAndVolScale,
                                            &gSfxDefaultReverb
                                        );
                                    }
                                }
                            }
                            break;
                        case 6:
                            D_8015FFE6--;
                            if (D_8015FFE6 == 0) {
                                gSaveContext.timer2State = 0;
                            }
                            break;
                    }
                    break;
            }
            
            if (
                ((gSaveContext.timer1State != 0) && (gSaveContext.timer1State != 10)) ||
                (gSaveContext.timer2State != 0)
            ) {
                timerDigits[0] = timerDigits[1] = timerDigits[3] = 0;
                timerDigits[2] = 10; // digit 10 is used as ':' (colon)
                
                if (gSaveContext.timer1State != 0) {
                    timerDigits[4] = gSaveContext.timer1Value;
                } else {
                    timerDigits[4] = gSaveContext.timer2Value;
                }
                
                while (timerDigits[4] >= 60) {
                    timerDigits[1]++;
                    if (timerDigits[1] >= 10) {
                        timerDigits[0]++;
                        timerDigits[1] -= 10;
                    }
                    timerDigits[4] -= 60;
                }
                
                while (timerDigits[4] >= 10) {
                    timerDigits[3]++;
                    timerDigits[4] -= 10;
                }
                
                // Clock Icon
                gDPPipeSync(OVERLAY_DISP++);
                gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, 255);
                gDPSetEnvColor(OVERLAY_DISP++, 0, 0, 0, 0);
                OVERLAY_DISP =
                    Gfx_TextureIA8(
                    OVERLAY_DISP, gClockIconTex, 16, 16, ((void)0, gSaveContext.timerX[svar6]),
                    ((void)0, gSaveContext.timerY[svar6]) + 2, 16, 16, 1 << 10, 1 << 10
                    );
                
                // Timer Counter
                gDPPipeSync(OVERLAY_DISP++);
                gDPSetCombineLERP(
                    OVERLAY_DISP++, 0, 0, 0, PRIMITIVE, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, PRIMITIVE,
                    TEXEL0, 0, PRIMITIVE, 0
                );
                
                if (gSaveContext.timer1State != 0) {
                    if ((gSaveContext.timer1Value < 10) && (gSaveContext.timer1State < 11)) {
                        gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 50, 0, 255);
                    } else {
                        gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, 255);
                    }
                } else {
                    if ((gSaveContext.timer2Value < 10) && (gSaveContext.timer2State < 6)) {
                        gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 50, 0, 255);
                    } else {
                        gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 0, 255);
                    }
                }
                
                for (svar1 = 0; svar1 < 5; svar1++) {
                    OVERLAY_DISP =
                        Gfx_TextureI8(
                        OVERLAY_DISP, ((u8*)gCounterDigit0Tex + (8 * 16 * timerDigits[svar1])), 8, 16,
                        ((void)0, gSaveContext.timerX[svar6]) + timerDigitLeftPos[svar1],
                        ((void)0, gSaveContext.timerY[svar6]), digitWidth[svar1], VREG(42), VREG(43) << 1,
                            VREG(43) << 1
                        );
                }
            }
        }
    }
    
    if (pauseCtx->debugState == 3) {
        FlagSet_Update(play);
    }
    
    if (interfaceCtx->unk_244 != 0) {
        gDPPipeSync(OVERLAY_DISP++);
        gSPDisplayList(OVERLAY_DISP++, sSetupDL_80125A60);
        gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 0, 0, 0, interfaceCtx->unk_244);
        gDPFillRectangle(OVERLAY_DISP++, 0, 0, gScreenWidth - 1, gScreenHeight - 1);
    }
    
    CLOSE_DISPS(play->state.gfxCtx, "../z_parameter.c", 4269);
}

Asm_VanillaHook(Interface_LoadActionLabel);
void Interface_LoadActionLabel(InterfaceContext* interfaceCtx, u16 action, s16 loadOffset) {
    static void* sDoActionTextures[] = { gAttackDoActionENGTex, gCheckDoActionENGTex };
    
    if (action >= DO_ACTION_MAX) {
        action = DO_ACTION_NONE;
    }
    
#if 0                          // RIP other languages
    if (gSaveContext.language != LANGUAGE_ENG) {
        action += DO_ACTION_MAX;
    }
    
    if (gSaveContext.language == LANGUAGE_FRA) {
        action += DO_ACTION_MAX;
    }
#endif
    
    if ((action != DO_ACTION_NONE) && (action != DO_ACTION_MAX + DO_ACTION_NONE) && (action != 2 * DO_ACTION_MAX + DO_ACTION_NONE)) {
        osCreateMesgQueue(&interfaceCtx->loadQueue, &interfaceCtx->loadMsg, OS_MESG_BLOCK);
        DmaMgr_SendRequest2(
            &interfaceCtx->dmaRequest_160,
            (u32)interfaceCtx->doActionSegment + (loadOffset * DO_ACTION_TEX_SIZE),
            gDmaDataTable[17].vromStart + (action * DO_ACTION_TEX_SIZE),
            DO_ACTION_TEX_SIZE,
            0,
            &interfaceCtx->loadQueue,
            NULL,
            NULL,
            0
        );
        osRecvMesg(&interfaceCtx->loadQueue, NULL, OS_MESG_BLOCK);
    } else {
        gSegments[7] = VIRTUAL_TO_PHYSICAL(interfaceCtx->doActionSegment);
        func_80086D5C(SEGMENTED_TO_VIRTUAL(sDoActionTextures[loadOffset]), DO_ACTION_TEX_SIZE / 4);
    }
}

Asm_VanillaHook(Interface_LoadActionLabelB);
void Interface_LoadActionLabelB(PlayState* playState, u16 action) {
    InterfaceContext* interfaceCtx = &playState->interfaceCtx;
    
#if 0
    if (gSaveContext.language != LANGUAGE_ENG) {
        action += DO_ACTION_MAX;
    }
    
    if (gSaveContext.language == LANGUAGE_FRA) {
        action += DO_ACTION_MAX;
    }
#endif
    
    interfaceCtx->unk_1FC = action;
    
    osCreateMesgQueue(&interfaceCtx->loadQueue, &interfaceCtx->loadMsg, OS_MESG_BLOCK);
    DmaMgr_SendRequest2(
        &interfaceCtx->dmaRequest_160,
        (u32)interfaceCtx->doActionSegment + DO_ACTION_TEX_SIZE,
        gDmaDataTable[17].vromStart + (action * DO_ACTION_TEX_SIZE),
        DO_ACTION_TEX_SIZE,
        0,
        &interfaceCtx->loadQueue,
        NULL,
        NULL,
        0
    );
    osRecvMesg(&interfaceCtx->loadQueue, NULL, OS_MESG_BLOCK);
    
    interfaceCtx->unk_1FA = 1;
}

Asm_VanillaHook(Interface_LoadItemIcon1);
void Interface_LoadItemIcon1(PlayState* playState, u16 button) {
    InterfaceContext* interfaceCtx = &playState->interfaceCtx;
    
    osCreateMesgQueue(&interfaceCtx->loadQueue, &interfaceCtx->loadMsg, OS_MESG_BLOCK);
    DmaMgr_SendRequest2(
        &interfaceCtx->dmaRequest_160,
        (u32)interfaceCtx->iconItemSegment + button * 0x1000,
        gDmaDataTable[7].vromStart + (gSaveContext.equips.buttonItems[button] * 0x1000),
        0x1000,
        0,
        &interfaceCtx->loadQueue,
        NULL,
        NULL,
        0
    );
    osRecvMesg(&interfaceCtx->loadQueue, NULL, OS_MESG_BLOCK);
}

Asm_VanillaHook(Interface_LoadItemIcon2);
void Interface_LoadItemIcon2(PlayState* playState, u16 button) {
    osCreateMesgQueue(&playState->interfaceCtx.loadQueue, &playState->interfaceCtx.loadMsg, OS_MESG_BLOCK);
    DmaMgr_SendRequest2(
        &playState->interfaceCtx.dmaRequest_180,
        (u32)playState->interfaceCtx.iconItemSegment + button * 0x1000,
        gDmaDataTable[7].vromStart + (gSaveContext.equips.buttonItems[button] * 0x1000),
        0x1000,
        0,
        &playState->interfaceCtx.loadQueue,
        NULL,
        NULL,
        0
    );
    osRecvMesg(&playState->interfaceCtx.loadQueue, NULL, OS_MESG_BLOCK);
}

Asm_VanillaHook(Minimap_Draw);
void Minimap_Draw(PlayState* play) {
    InterfaceContext* interfaceCtx = &play->interfaceCtx;
    s32 mapIndex = gSaveContext.mapIndex;

    OPEN_DISPS(play->state.gfxCtx, "../z_map_exp.c", 626);
    
    if (play->pauseCtx.state < 4) {

        if (CHECK_BTN_ALL(play->state.input[0].press.button, BTN_L) /*&& 
        play->frameAdvCtx.enabled == false && !Play_InCsMode(play)*/) {
           if (R_MINIMAP_DISABLED == false) {//is Opening
                Audio_PlaySfxGeneral(
                    NA_SE_SY_CAMERA_ZOOM_UP, &gSfxDefaultPos, 4,
                    &gSfxDefaultFreqAndVolScale, &gSfxDefaultFreqAndVolScale,
                    &gSfxDefaultReverb
                );
           } else {//is Closeing
               Audio_PlaySfxGeneral(
                   NA_SE_SY_CAMERA_ZOOM_DOWN, &gSfxDefaultPos, 4,
                   &gSfxDefaultFreqAndVolScale, &gSfxDefaultFreqAndVolScale,
                   &gSfxDefaultReverb
               );
           }
           
           R_MINIMAP_DISABLED ^= 1;
        }

        switch (play->sceneId) {
            /*Dungeons*/
            case SCENE_YDAN:
            case SCENE_DDAN:
            case SCENE_BDAN:
            case SCENE_BMORI1:
            case SCENE_HIDAN:
            case SCENE_MIZUSIN:
            case SCENE_JYASINZOU:
            case SCENE_HAKADAN:
            case SCENE_HAKADANCH:
            case SCENE_ICE_DOUKUTO:
                if (/*!R_MINIMAP_DISABLED*/0) {
                    Gfx_SetupDL_39Overlay(play->state.gfxCtx);
                    gDPSetCombineLERP(
                        OVERLAY_DISP++, 1, 0, PRIMITIVE, 0, TEXEL0, 0, PRIMITIVE, 0, 1, 0, PRIMITIVE, 0,
                        TEXEL0, 0, PRIMITIVE, 0
                    );
                    
                    if (CHECK_DUNGEON_ITEM(DUNGEON_COMPASS, mapIndex)) {
                        Minimap_DrawCompassIcons(play); // Draw icons for the player spawn and current position
                        Gfx_SetupDL_39Overlay(play->state.gfxCtx);
                        MapMark_Draw(play);
                    }
                }
      
                break;
            /*Minimaps*/
            case SCENE_SPOT00:
            case SCENE_SPOT01:
            case SCENE_SPOT02:
            case SCENE_SPOT03:
            case SCENE_SPOT04:
            case SCENE_SPOT05:
            case SCENE_SPOT06:
            case SCENE_SPOT07:
            case SCENE_SPOT08:
            case SCENE_SPOT09:
            case SCENE_SPOT10:
            case SCENE_SPOT11:
            case SCENE_SPOT12:
            case SCENE_SPOT13:
            case SCENE_SPOT15:
            case SCENE_SPOT16:
            case SCENE_SPOT17:
            case SCENE_SPOT18:
            case SCENE_SPOT20:
            case SCENE_GANON_TOU:
                if (/*!R_MINIMAP_DISABLED*/0) {
                    Gfx_SetupDL_39Overlay(play->state.gfxCtx);
                    
                    gDPSetCombineMode(OVERLAY_DISP++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
                    gDPSetPrimColor(
                        OVERLAY_DISP++, 0, 0, R_MINIMAP_COLOR(0), R_MINIMAP_COLOR(1), R_MINIMAP_COLOR(2),
                        interfaceCtx->minimapAlpha
                    );
                    
                    gDPLoadTextureBlock_4b(
                        OVERLAY_DISP++, interfaceCtx->mapSegment, G_IM_FMT_IA,
                        gMapData->owMinimapWidth[mapIndex], gMapData->owMinimapHeight[mapIndex], 0,
                        G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK,
                        G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD
                    );
                    
                    gSPTextureRectangle(
                        OVERLAY_DISP++, R_OW_MINIMAP_X << 2, R_OW_MINIMAP_Y << 2,
                            (R_OW_MINIMAP_X + gMapData->owMinimapWidth[mapIndex]) << 2,
                            (R_OW_MINIMAP_Y + gMapData->owMinimapHeight[mapIndex]) << 2, G_TX_RENDERTILE, 0,
                            0, 1 << 10, 1 << 10
                    );
                    
                    if (
                        ((play->sceneId != SCENE_SPOT01) && (play->sceneId != SCENE_SPOT04) &&
                        (play->sceneId != SCENE_SPOT08)) ||
                        (LINK_AGE_IN_YEARS != YEARS_ADULT)
                    ) {
                        if (
                            (gMapData->owEntranceFlag[sEntranceIconMapIndex] == 0xFFFF) ||
                            ((gMapData->owEntranceFlag[sEntranceIconMapIndex] != 0xFFFF) &&
                            (gSaveContext.infTable[INFTABLE_1AX_INDEX] &
                            gBitFlags[gMapData->owEntranceFlag[mapIndex]]))
                        ) {
                            
                            gDPLoadTextureBlock(
                                OVERLAY_DISP++, gMapDungeonEntranceIconTex, G_IM_FMT_RGBA, G_IM_SIZ_16b,
                                8, 8, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP,
                                G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD
                            );
                            
                            gSPTextureRectangle(
                                OVERLAY_DISP++,
                                gMapData->owEntranceIconPosX[sEntranceIconMapIndex] << 2,
                                    gMapData->owEntranceIconPosY[sEntranceIconMapIndex] << 2,
                                    (gMapData->owEntranceIconPosX[sEntranceIconMapIndex] + 8) << 2,
                                    (gMapData->owEntranceIconPosY[sEntranceIconMapIndex] + 8) << 2,
                                    G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10
                            );
                        }
                    }
                    
                    if (
                        (play->sceneId == SCENE_SPOT08) &&
                        (gSaveContext.infTable[INFTABLE_1AX_INDEX] & gBitFlags[INFTABLE_1A9_SHIFT])
                    ) {
                        gDPLoadTextureBlock(
                            OVERLAY_DISP++, gMapDungeonEntranceIconTex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 8,
                            8, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK,
                            G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD
                        );
                        
                        gSPTextureRectangle(
                            OVERLAY_DISP++, 270 << 2, 154 << 2, 278 << 2, 162 << 2, G_TX_RENDERTILE, 0,
                                0, 1 << 10, 1 << 10
                        );
                    }
                    
                    Minimap_DrawCompassIcons(play); // Draw icons for the player spawn and current position
                }
                break;
        }
    }
    
    CLOSE_DISPS(play->state.gfxCtx, "../z_map_exp.c", 782);
}

Asm_VanillaHook(Interface_Update);
void Interface_Update(PlayState* play) {
    static u8 D_80125B60 = false;
    static s16 sPrevTimeSpeed = 0;
    MessageContext* msgCtx = &play->msgCtx;
    InterfaceContext* interfaceCtx = &play->interfaceCtx;
    Player* player = GET_PLAYER(play);
    s16 alpha;
    s16 alpha1;
    u16 action;
    Input* debugInput = &play->state.input[2];

    if (CHECK_BTN_ALL(debugInput->press.button, BTN_DLEFT)) {
        gSaveContext.language = LANGUAGE_ENG;
        osSyncPrintf("J_N=%x J_N=%x\n", gSaveContext.language, &gSaveContext.language);
    } else if (CHECK_BTN_ALL(debugInput->press.button, BTN_DUP)) {
        gSaveContext.language = LANGUAGE_GER;
        osSyncPrintf("J_N=%x J_N=%x\n", gSaveContext.language, &gSaveContext.language);
    } else if (CHECK_BTN_ALL(debugInput->press.button, BTN_DRIGHT)) {
        gSaveContext.language = LANGUAGE_FRA;
        osSyncPrintf("J_N=%x J_N=%x\n", gSaveContext.language, &gSaveContext.language);
    }

    if ((play->pauseCtx.state == 0) && (play->pauseCtx.debugState == 0)) {
        if ((gSaveContext.minigameState == 1) || !IS_CUTSCENE_LAYER ||
            ((play->sceneId == SCENE_SPOT20) && (gSaveContext.sceneLayer == 4))) {
            if ((msgCtx->msgMode == MSGMODE_NONE) ||
                ((msgCtx->msgMode != MSGMODE_NONE) && (play->sceneId == SCENE_BOWLING))) {
                if (play->gameOverCtx.state == GAMEOVER_INACTIVE) {
                    func_80083108(play);
                }
            }
        }
    }

    switch (gSaveContext.unk_13E8) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
            alpha = 255 - (gSaveContext.unk_13EC << 5);
            if (alpha < 0) {
                alpha = 0;
            }

            func_80082850(play, alpha);
            gSaveContext.unk_13EC++;

            if (alpha == 0) {
                gSaveContext.unk_13E8 = 0;
            }
            break;
        case 50:
            alpha = 255 - (gSaveContext.unk_13EC << 5);
            if (alpha < 0) {
                alpha = 0;
            }

            alpha1 = 255 - alpha;
            if (alpha1 >= 255) {
                alpha1 = 255;
            }

            osSyncPrintf("case 50 : alpha=%d  alpha1=%d\n", alpha, alpha1);
            func_80082644(play, alpha1);

            switch (play->sceneId) {
                case SCENE_SPOT00:
                case SCENE_SPOT01:
                case SCENE_SPOT02:
                case SCENE_SPOT03:
                case SCENE_SPOT04:
                case SCENE_SPOT05:
                case SCENE_SPOT06:
                case SCENE_SPOT07:
                case SCENE_SPOT08:
                case SCENE_SPOT09:
                case SCENE_SPOT10:
                case SCENE_SPOT11:
                case SCENE_SPOT12:
                case SCENE_SPOT13:
                case SCENE_SPOT15:
                case SCENE_SPOT16:
                case SCENE_SPOT17:
                case SCENE_SPOT18:
                case SCENE_SPOT20:
                case SCENE_GANON_TOU:
                    break;
                default:
                    break;
            }

            gSaveContext.unk_13EC++;
            if (alpha1 == 255) {
                gSaveContext.unk_13E8 = 0;
            }

            break;
        case 52:
            gSaveContext.unk_13E8 = 1;
            func_80082850(play, 0);
            gSaveContext.unk_13E8 = 0;
        default:
            break;
    }

    Map_Update(play);

     if (gSaveContext.healthAccumulator > 0) {
         if((gSaveContext.healthAccumulator - 4) < 0){
            gSaveContext.healthAccumulator = 0;
         }else{
            gSaveContext.healthAccumulator -= 4;
         }
         if((gSaveContext.health + 4) < gSaveContext.healthCapacity){
            gSaveContext.health +=4;
         }else{
            gSaveContext.health = gSaveContext.healthCapacity;
            gSaveContext.healthAccumulator = 0;
         }
         if ((gSaveContext.health & 0xF) < 4) {
            Audio_PlaySfxGeneral(NA_SE_SY_HP_RECOVER, &gSfxDefaultPos, 4, &gSfxDefaultFreqAndVolScale,
                                  &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
         }
    

     }else if(gSaveContext.healthAccumulator < 0){
        if((gSaveContext.healthAccumulator + 4) > 0){
            gSaveContext.healthAccumulator = 0;
        }else{
            gSaveContext.healthAccumulator += 4;
        }
        if(gSaveContext.health > 0 && (gSaveContext.health-4) > 0 ){
           gSaveContext.health -=4;
        }else{
           gSaveContext.health = 0;
           gSaveContext.healthAccumulator = 0;
        }
     }




    Health_UpdateBeatingHeart(play);
    D_80125A58 = func_8008F2F8(play);

    if (D_80125A58 == 1) {
        if (CUR_EQUIP_VALUE(EQUIP_TYPE_TUNIC) == EQUIP_VALUE_TUNIC_GORON) {
            D_80125A58 = 0;
        }
    } else if ((func_8008F2F8(play) >= 2) && (func_8008F2F8(play) < 5)) {
        if (CUR_EQUIP_VALUE(EQUIP_TYPE_TUNIC) == EQUIP_VALUE_TUNIC_ZORA) {
            D_80125A58 = 0;
        }
    }

    Health_UpdateMeter(play);

    if ((gSaveContext.timer1State >= 3) && (play->pauseCtx.state == 0) && (play->pauseCtx.debugState == 0) &&
        (msgCtx->msgMode == MSGMODE_NONE) && !(player->stateFlags2 & PLAYER_STATE2_24) &&
        (play->transitionTrigger == TRANS_TRIGGER_OFF) && (play->transitionMode == TRANS_MODE_OFF) &&
        !Play_InCsMode(play)) {}

    if (gSaveContext.rupeeAccumulator != 0) {
        if (gSaveContext.rupeeAccumulator > 0) {
            if (gSaveContext.rupees < CUR_CAPACITY(UPG_WALLET)) {
                gSaveContext.rupeeAccumulator--;
                gSaveContext.rupees++;
                Audio_PlaySfxGeneral(NA_SE_SY_RUPY_COUNT, &gSfxDefaultPos, 4, &gSfxDefaultFreqAndVolScale,
                                     &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
            } else {
                // "Rupee Amount MAX = %d"
                osSyncPrintf("ルピー数ＭＡＸ = %d\n", CUR_CAPACITY(UPG_WALLET));
                gSaveContext.rupees = CUR_CAPACITY(UPG_WALLET);
                gSaveContext.rupeeAccumulator = 0;
            }
        } else if (gSaveContext.rupees != 0) {
            if (gSaveContext.rupeeAccumulator <= -50) {
                gSaveContext.rupeeAccumulator += 10;
                gSaveContext.rupees -= 10;

                if (gSaveContext.rupees < 0) {
                    gSaveContext.rupees = 0;
                }

                Audio_PlaySfxGeneral(NA_SE_SY_RUPY_COUNT, &gSfxDefaultPos, 4, &gSfxDefaultFreqAndVolScale,
                                     &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
            } else {
                gSaveContext.rupeeAccumulator++;
                gSaveContext.rupees--;
                Audio_PlaySfxGeneral(NA_SE_SY_RUPY_COUNT, &gSfxDefaultPos, 4, &gSfxDefaultFreqAndVolScale,
                                     &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
            }
        } else {
            gSaveContext.rupeeAccumulator = 0;
        }
    }

    switch (interfaceCtx->unk_1EC) {
        case 1:
            interfaceCtx->unk_1F4 += 31400.0f / WREG(5);
            if (interfaceCtx->unk_1F4 >= 15700.0f) {
                interfaceCtx->unk_1F4 = -15700.0f;
                interfaceCtx->unk_1EC = 2;
            }
            break;
        case 2:
            interfaceCtx->unk_1F4 += 31400.0f / WREG(5);
            if (interfaceCtx->unk_1F4 >= 0.0f) {
                interfaceCtx->unk_1F4 = 0.0f;
                interfaceCtx->unk_1EC = 0;
                interfaceCtx->unk_1EE = interfaceCtx->unk_1F0;
                action = interfaceCtx->unk_1EE;
                if ((action == DO_ACTION_MAX) || (action == DO_ACTION_MAX + 1)) {
                    action = DO_ACTION_NONE;
                }
                Interface_LoadActionLabel(interfaceCtx, action, 0);
            }
            break;
        case 3:
            interfaceCtx->unk_1F4 += 31400.0f / WREG(5);
            if (interfaceCtx->unk_1F4 >= 15700.0f) {
                interfaceCtx->unk_1F4 = -15700.0f;
                interfaceCtx->unk_1EC = 2;
            }
            break;
        case 4:
            interfaceCtx->unk_1F4 += 31400.0f / WREG(5);
            if (interfaceCtx->unk_1F4 >= 0.0f) {
                interfaceCtx->unk_1F4 = 0.0f;
                interfaceCtx->unk_1EC = 0;
                interfaceCtx->unk_1EE = interfaceCtx->unk_1F0;
                action = interfaceCtx->unk_1EE;
                if ((action == DO_ACTION_MAX) || (action == DO_ACTION_MAX + 1)) {
                    action = DO_ACTION_NONE;
                }
                Interface_LoadActionLabel(interfaceCtx, action, 0);
            }
            break;
    }

    WREG(7) = interfaceCtx->unk_1F4;

    // Update Magic
    if ((play->pauseCtx.state == 0) && (play->pauseCtx.debugState == 0) && (msgCtx->msgMode == MSGMODE_NONE) &&
        (play->transitionTrigger == TRANS_TRIGGER_OFF) && (play->gameOverCtx.state == GAMEOVER_INACTIVE) &&
        (play->transitionMode == TRANS_MODE_OFF) && ((play->csCtx.state == CS_STATE_IDLE) || !Player_InCsMode(play))) {

        if (gSaveContext.isMagicAcquired && (gSaveContext.magicLevel == 0)) {
            gSaveContext.magicLevel = gSaveContext.isDoubleMagicAcquired + 1;
            gSaveContext.magicState = MAGIC_STATE_STEP_CAPACITY;
            osSyncPrintf(VT_FGCOL(YELLOW));
            osSyncPrintf("魔法スター─────ト！！！！！！！！！\n"); // "Magic Start!!!!!!!!!"
            osSyncPrintf("MAGIC_MAX=%d\n", gSaveContext.magicLevel);
            osSyncPrintf("MAGIC_NOW=%d\n", gSaveContext.magic);
            osSyncPrintf("Z_MAGIC_NOW_NOW=%d\n", gSaveContext.magicFillTarget);
            osSyncPrintf("Z_MAGIC_NOW_MAX=%d\n", gSaveContext.magicCapacity);
            osSyncPrintf(VT_RST);
        }

        Magic_Update(play);
    }

    if (gSaveContext.timer1State == 0) {
        if (((D_80125A58 == 1) || (D_80125A58 == 2) || (D_80125A58 == 4)) && ((gSaveContext.health >> 1) != 0)) {
            gSaveContext.timer1State = 1;
            gSaveContext.timerX[0] = 140;
            gSaveContext.timerY[0] = 80;
            D_80125A5C = true;
        }
    } else {
        if (((D_80125A58 == 0) || (D_80125A58 == 3)) && (gSaveContext.timer1State < 5)) {
            gSaveContext.timer1State = 0;
        }
    }

    if (gSaveContext.minigameState == 1) {
        gSaveContext.minigameScore += interfaceCtx->unk_23C;
        interfaceCtx->unk_23C = 0;

        if (sHBAScoreTier == 0) {
            if (gSaveContext.minigameScore >= 1000) {
                sHBAScoreTier++;
            }
        } else if (sHBAScoreTier == 1) {
            if (gSaveContext.minigameScore >= 1500) {
                sHBAScoreTier++;
            }
        }

        sHBAScoreDigits[0] = sHBAScoreDigits[1] = 0;
        sHBAScoreDigits[2] = 0;
        sHBAScoreDigits[3] = gSaveContext.minigameScore;

        while (sHBAScoreDigits[3] >= 1000) {
            sHBAScoreDigits[0]++;
            sHBAScoreDigits[3] -= 1000;
        }

        while (sHBAScoreDigits[3] >= 100) {
            sHBAScoreDigits[1]++;
            sHBAScoreDigits[3] -= 100;
        }

        while (sHBAScoreDigits[3] >= 10) {
            sHBAScoreDigits[2]++;
            sHBAScoreDigits[3] -= 10;
        }
    }

    if (gSaveContext.sunsSongState != SUNSSONG_INACTIVE) {
        // exit out of ocarina mode after suns song finishes playing
        if ((msgCtx->ocarinaAction != OCARINA_ACTION_CHECK_NOWARP_DONE) &&
            (gSaveContext.sunsSongState == SUNSSONG_START)) {
            play->msgCtx.ocarinaMode = OCARINA_MODE_04;
        }

        // handle suns song in areas where time moves
        if (play->envCtx.sceneTimeSpeed != 0) {
            if (gSaveContext.sunsSongState != SUNSSONG_SPEED_TIME) {
                D_80125B60 = false;
                if ((gSaveContext.dayTime >= CLOCK_TIME(6, 30)) && (gSaveContext.dayTime <= CLOCK_TIME(18, 0) + 1)) {
                    D_80125B60 = true;
                }

                gSaveContext.sunsSongState = SUNSSONG_SPEED_TIME;
                sPrevTimeSpeed = gTimeSpeed;
                gTimeSpeed = 400;
            } else if (!D_80125B60) {
                if ((gSaveContext.dayTime >= CLOCK_TIME(6, 30)) && (gSaveContext.dayTime <= CLOCK_TIME(18, 0) + 1)) {
                    gSaveContext.sunsSongState = SUNSSONG_INACTIVE;
                    gTimeSpeed = sPrevTimeSpeed;
                    play->msgCtx.ocarinaMode = OCARINA_MODE_04;
                }
            } else if (gSaveContext.dayTime > CLOCK_TIME(18, 0) + 1) {
                gSaveContext.sunsSongState = SUNSSONG_INACTIVE;
                gTimeSpeed = sPrevTimeSpeed;
                play->msgCtx.ocarinaMode = OCARINA_MODE_04;
            }
        } else if ((play->roomCtx.curRoom.behaviorType1 != ROOM_BEHAVIOR_TYPE1_1) &&
                   (interfaceCtx->restrictions.sunsSong != 3)) {
            if ((gSaveContext.dayTime >= CLOCK_TIME(6, 30)) && (gSaveContext.dayTime < CLOCK_TIME(18, 0) + 1)) {
                gSaveContext.nextDayTime = NEXT_TIME_NIGHT;
                play->transitionType = TRANS_TYPE_FADE_BLACK_FAST;
                gSaveContext.nextTransitionType = TRANS_TYPE_FADE_BLACK;
                play->unk_11DE9 = true;
            } else {
                gSaveContext.nextDayTime = NEXT_TIME_DAY;
                play->transitionType = TRANS_TYPE_FADE_WHITE_FAST;
                gSaveContext.nextTransitionType = TRANS_TYPE_FADE_WHITE;
                play->unk_11DE9 = true;
            }

            if (play->sceneId == SCENE_SPOT13) {
                play->transitionType = TRANS_TYPE_SANDSTORM_PERSIST;
                gSaveContext.nextTransitionType = TRANS_TYPE_SANDSTORM_PERSIST;
            }

            gSaveContext.respawnFlag = -2;
            play->nextEntranceIndex = gSaveContext.entranceIndex;
            play->transitionTrigger = TRANS_TRIGGER_START;
            gSaveContext.sunsSongState = SUNSSONG_INACTIVE;
            func_800F6964(30);
            gSaveContext.seqId = (u8)NA_BGM_DISABLED;
            gSaveContext.natureAmbienceId = NATURE_ID_DISABLED;
        } else {
            gSaveContext.sunsSongState = SUNSSONG_SPECIAL;
        }
    }
}

Asm_VanillaHook(Interface_DrawItemIconTexture);
void Interface_DrawItemIconTexture(PlayState* play, void* texture, s16 button) {
    OPEN_DISPS(play->state.gfxCtx, "../z_parameter.c", 3079);
    gDPLoadTextureBlock(OVERLAY_DISP++, texture, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP,
                        G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

    gSPTextureRectangle(OVERLAY_DISP++, R_ITEM_ICON_X(button) << 2, R_ITEM_ICON_Y(button) << 2,
                        (R_ITEM_ICON_X(button) + R_ITEM_ICON_WIDTH(button)) << 2,
                        (R_ITEM_ICON_Y(button) + R_ITEM_ICON_WIDTH(button)) << 2, G_TX_RENDERTILE, 0, 0,
                        R_ITEM_ICON_DD(button) << 1, R_ITEM_ICON_DD(button) << 1);

    CLOSE_DISPS(play->state.gfxCtx, "../z_parameter.c", 3094);
}

Asm_VanillaHook(Magic_DrawMeter);
void Magic_DrawMeter(PlayState* play) {
    InterfaceContext* interfaceCtx = &play->interfaceCtx;
    s16 magicMeterY;
    s16 magicMeterX;
    OPEN_DISPS(play->state.gfxCtx, "../z_parameter.c", 2650);

    if (gSaveContext.magicLevel != 0) {
        // if (gSaveContext.healthCapacity > 0xA0) {
        //     magicMeterY = R_MAGIC_METER_Y_LOWER; // two rows of hearts
        // } else {
        //     magicMeterY =  R_MAGIC_METER_Y_HIGHER; // one row of hearts
        // }
        magicMeterY = XREG(24);//first :24,21,20
        magicMeterX = XREG(49);//49
        Gfx_SetupDL_39Overlay(play->state.gfxCtx);

        gDPSetPrimColor(OVERLAY_DISP++, 0, 0, sMagicBorderR, sMagicBorderG, sMagicBorderB, interfaceCtx->magicAlpha);
        gDPSetEnvColor(OVERLAY_DISP++, 100, 50, 50, 255);

        OVERLAY_DISP = Gfx_TextureIA8(OVERLAY_DISP, gMagicMeterEndTex, 8, 16, magicMeterX, magicMeterY, 8, 16,
                                      1 << 10, 1 << 10);

        OVERLAY_DISP = Gfx_TextureIA8(OVERLAY_DISP, gMagicMeterMidTex, 24, 16, magicMeterX + 8, magicMeterY,
                                      gSaveContext.magicCapacity, 16, 1 << 10, 1 << 10);

        gDPLoadTextureBlock(OVERLAY_DISP++, gMagicMeterEndTex, G_IM_FMT_IA, G_IM_SIZ_8b, 8, 16, 0,
                            G_TX_MIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 3, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

        gSPTextureRectangle(OVERLAY_DISP++, (magicMeterX + gSaveContext.magicCapacity + 8) << 2, magicMeterY << 2,
                            (magicMeterX + gSaveContext.magicCapacity + 16) << 2, (magicMeterY + 16) << 2,
                            G_TX_RENDERTILE, 256, 0, 1 << 10, 1 << 10);

        gDPPipeSync(OVERLAY_DISP++);
        gDPSetCombineLERP(OVERLAY_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, 0, 0, 0, PRIMITIVE, PRIMITIVE,
                          ENVIRONMENT, TEXEL0, ENVIRONMENT, 0, 0, 0, PRIMITIVE);
        gDPSetEnvColor(OVERLAY_DISP++, 0, 0, 0, 255);

        if (gSaveContext.magicState == MAGIC_STATE_METER_FLASH_2) {
            // Yellow part of the meter indicating the amount of magic to be subtracted
            gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 250, 250, 0, interfaceCtx->magicAlpha);

            gDPLoadMultiBlock_4b(OVERLAY_DISP++, gMagicMeterFillTex, 0x0000, G_TX_RENDERTILE, G_IM_FMT_I, 16, 16, 0,
                                 G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                 G_TX_NOLOD, G_TX_NOLOD);

            gSPTextureRectangle(OVERLAY_DISP++, R_MAGIC_FILL_X << 2, (magicMeterY + 3) << 2,
                                (R_MAGIC_FILL_X + gSaveContext.magic) << 2, (magicMeterY + 10) << 2, G_TX_RENDERTILE, 0,
                                0, 1 << 10, 1 << 10);

            // Fill the rest of the meter with the normal magic color
            gDPPipeSync(OVERLAY_DISP++);
            gDPSetPrimColor(OVERLAY_DISP++, 0, 0, R_MAGIC_FILL_COLOR(0), R_MAGIC_FILL_COLOR(1), R_MAGIC_FILL_COLOR(2),
                            interfaceCtx->magicAlpha);

            gSPTextureRectangle(OVERLAY_DISP++, R_MAGIC_FILL_X << 2, (magicMeterY + 3) << 2,
                                (R_MAGIC_FILL_X + gSaveContext.magicTarget) << 2, (magicMeterY + 10) << 2,
                                G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10);
        } else {
            // Fill the whole meter with the normal magic color
            gDPSetPrimColor(OVERLAY_DISP++, 0, 0, R_MAGIC_FILL_COLOR(0), R_MAGIC_FILL_COLOR(1), R_MAGIC_FILL_COLOR(2),
                            interfaceCtx->magicAlpha);

            gDPLoadMultiBlock_4b(OVERLAY_DISP++, gMagicMeterFillTex, 0x0000, G_TX_RENDERTILE, G_IM_FMT_I, 16, 16, 0,
                                 G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                 G_TX_NOLOD, G_TX_NOLOD);

            gSPTextureRectangle(OVERLAY_DISP++, R_MAGIC_FILL_X << 2, (magicMeterY + 3) << 2,
                                (R_MAGIC_FILL_X + gSaveContext.magic) << 2, (magicMeterY + 10) << 2, G_TX_RENDERTILE, 0,
                                0, 1 << 10, 1 << 10);
        }
    }

    CLOSE_DISPS(play->state.gfxCtx, "../z_parameter.c", 2731);
}

Asm_VanillaHook(Interface_DrawItemButtons);
void Interface_DrawItemButtons(PlayState* play) {
    static void* cUpLabelTextures[] = { gNaviCUpENGTex, gNaviCUpENGTex, gNaviCUpENGTex };
    static s16 startButtonLeftPos[] = { 132, 130, 130 };
    InterfaceContext* interfaceCtx = &play->interfaceCtx;
    Player* player = GET_PLAYER(play);
    PauseContext* pauseCtx = &play->pauseCtx;
    s16 temp; // Used as both an alpha value and a button index
    s16 texCoordScale;
    s16 width;
    s16 height;

    OPEN_DISPS(play->state.gfxCtx, "../z_parameter.c", 2900);

    // // B Button Color & Texture
    // // Also loads the Item Button Texture reused by other buttons afterwards
    // gDPPipeSync(OVERLAY_DISP++);
    // gDPSetCombineMode(OVERLAY_DISP++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
    // gDPSetPrimColor(OVERLAY_DISP++, 0, 0, R_B_BTN_COLOR(0), R_B_BTN_COLOR(1), R_B_BTN_COLOR(2), interfaceCtx->bAlpha);
    // gDPSetEnvColor(OVERLAY_DISP++, 0, 0, 0, 255);
    // OVERLAY_DISP =
    //     Gfx_TextureIA8(OVERLAY_DISP, gButtonBackgroundTex, 32, 32, R_ITEM_BTN_X(0), R_ITEM_BTN_Y(0),
    //                    R_ITEM_BTN_WIDTH(0), R_ITEM_BTN_WIDTH(0), R_ITEM_BTN_DD(0) << 1, R_ITEM_BTN_DD(0) << 1);

    // // C-Left Button Color & Texture
    // gDPPipeSync(OVERLAY_DISP++);
    // gDPSetPrimColor(OVERLAY_DISP++, 0, 0, R_C_BTN_COLOR(0), R_C_BTN_COLOR(1), R_C_BTN_COLOR(2),
    //                 interfaceCtx->cLeftAlpha);
    // gSPTextureRectangle(OVERLAY_DISP++, R_ITEM_BTN_X(1) << 2, R_ITEM_BTN_Y(1) << 2,
    //                     (R_ITEM_BTN_X(1) + R_ITEM_BTN_WIDTH(1)) << 2, (R_ITEM_BTN_Y(1) + R_ITEM_BTN_WIDTH(1)) << 2,
    //                     G_TX_RENDERTILE, 0, 0, R_ITEM_BTN_DD(1) << 1, R_ITEM_BTN_DD(1) << 1);

    // // C-Down Button Color & Texture
    // gDPSetPrimColor(OVERLAY_DISP++, 0, 0, R_C_BTN_COLOR(0), R_C_BTN_COLOR(1), R_C_BTN_COLOR(2),
    //                 interfaceCtx->cDownAlpha);
    // gSPTextureRectangle(OVERLAY_DISP++, R_ITEM_BTN_X(2) << 2, R_ITEM_BTN_Y(2) << 2,
    //                     (R_ITEM_BTN_X(2) + R_ITEM_BTN_WIDTH(2)) << 2, (R_ITEM_BTN_Y(2) + R_ITEM_BTN_WIDTH(2)) << 2,
    //                     G_TX_RENDERTILE, 0, 0, R_ITEM_BTN_DD(2) << 1, R_ITEM_BTN_DD(2) << 1);

    // // C-Right Button Color & Texture
    // gDPSetPrimColor(OVERLAY_DISP++, 0, 0, R_C_BTN_COLOR(0), R_C_BTN_COLOR(1), R_C_BTN_COLOR(2),
    //                 interfaceCtx->cRightAlpha);
    // gSPTextureRectangle(OVERLAY_DISP++, R_ITEM_BTN_X(3) << 2, R_ITEM_BTN_Y(3) << 2,
    //                     (R_ITEM_BTN_X(3) + R_ITEM_BTN_WIDTH(3)) << 2, (R_ITEM_BTN_Y(3) + R_ITEM_BTN_WIDTH(3)) << 2,
    //                     G_TX_RENDERTILE, 0, 0, R_ITEM_BTN_DD(3) << 1, R_ITEM_BTN_DD(3) << 1);

    //  if ((pauseCtx->state < 8) || (pauseCtx->state >= 18)) {
    //      if ((play->pauseCtx.state != 0) || (play->pauseCtx.debugState != 0)) {
    //          // Start Button Texture, Color & Label
    //          gDPPipeSync(OVERLAY_DISP++);
    //          gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 120, 120, 120, interfaceCtx->startAlpha);
    //          gSPTextureRectangle(OVERLAY_DISP++, startButtonLeftPos[gSaveContext.language] << 2, 17 << 2,
    //                              (startButtonLeftPos[gSaveContext.language] + 22) << 2, 39 << 2, G_TX_RENDERTILE, 0, 0,
    //                              (s32)(1.4277344 * (1 << 10)), (s32)(1.4277344 * (1 << 10)));
    //          gDPPipeSync(OVERLAY_DISP++);
    //          gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, interfaceCtx->startAlpha);
    //          gDPSetEnvColor(OVERLAY_DISP++, 0, 0, 0, 0);
    //          gDPSetCombineLERP(OVERLAY_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0,
    //                            PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0);

    //          gDPLoadTextureBlock_4b(OVERLAY_DISP++, interfaceCtx->doActionSegment + DO_ACTION_TEX_SIZE * 2, G_IM_FMT_IA,
    //                                 DO_ACTION_TEX_WIDTH, DO_ACTION_TEX_HEIGHT, 0, G_TX_NOMIRROR | G_TX_WRAP,
    //                                 G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

    //          texCoordScale = (1 << 10) / (R_START_LABEL_DD(gSaveContext.language) / 100.0f);
    //          width = DO_ACTION_TEX_WIDTH / (R_START_LABEL_DD(gSaveContext.language) / 100.0f);
    //          height = DO_ACTION_TEX_HEIGHT / (R_START_LABEL_DD(gSaveContext.language) / 100.0f);
    //          gSPTextureRectangle(OVERLAY_DISP++, R_START_LABEL_X(gSaveContext.language) << 2,
    //                              R_START_LABEL_Y(gSaveContext.language) << 2,
    //                              (R_START_LABEL_X(gSaveContext.language) + width) << 2,
    //                              (R_START_LABEL_Y(gSaveContext.language) + height) << 2, G_TX_RENDERTILE, 0, 0,
    //                              texCoordScale, texCoordScale);
    //      }
    //  }

    if (interfaceCtx->naviCalling && (play->pauseCtx.state == 0) && (play->pauseCtx.debugState == 0) &&
        (play->csCtx.state == CS_STATE_IDLE)) {
        if (!sCUpInvisible) {
            // C-Up Button Texture, Color & Label (Navi Text)
            gDPPipeSync(OVERLAY_DISP++);

            if ((gSaveContext.unk_13EA == 1) || (gSaveContext.unk_13EA == 2) || (gSaveContext.unk_13EA == 5)) {
                temp = 0;
            } else if ((player->stateFlags1 & PLAYER_STATE1_21) || (func_8008F2F8(play) == 4) ||
                       (player->stateFlags2 & PLAYER_STATE2_18)) {
                temp = 70;
            } else {
                temp = 255;//interfaceCtx->healthAlpha;
            }

            // gDPSetPrimColor(OVERLAY_DISP++, 0, 0, R_C_BTN_COLOR(0), R_C_BTN_COLOR(1), R_C_BTN_COLOR(2), temp);
            // gDPSetCombineMode(OVERLAY_DISP++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
            // gSPTextureRectangle(OVERLAY_DISP++, R_C_UP_BTN_X << 2, R_C_UP_BTN_Y << 2, (R_C_UP_BTN_X + 16) << 2,
            //                     (R_C_UP_BTN_Y + 16) << 2, G_TX_RENDERTILE, 0, 0, 2 << 10, 2 << 10);
            gDPPipeSync(OVERLAY_DISP++);
            gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, temp);
            gDPSetEnvColor(OVERLAY_DISP++, 0, 0, 0, 0);
            gDPSetCombineLERP(OVERLAY_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0,
                              PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0);

            gDPLoadTextureBlock_4b(OVERLAY_DISP++, cUpLabelTextures[gSaveContext.language], G_IM_FMT_IA, 32, 8, 0,
                                   G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                   G_TX_NOLOD, G_TX_NOLOD);

            gSPTextureRectangle(OVERLAY_DISP++, R_C_UP_ICON_X << 2, R_C_UP_ICON_Y << 2, (R_C_UP_ICON_X + 32) << 2,
                                (R_C_UP_ICON_Y + 8) << 2, G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10);
        }

        sCUpTimer--;
        if (sCUpTimer == 0) {
            sCUpInvisible ^= 1;
            sCUpTimer = 10;
        }
    }

    gDPPipeSync(OVERLAY_DISP++);

    // Empty C Button Arrows
    for (temp = 1; temp < 4; temp++) {
        if (gSaveContext.equips.buttonItems[temp] > 0xF0) {
            if (temp == 1) {
                gDPSetPrimColor(OVERLAY_DISP++, 0, 0, R_C_BTN_COLOR(0), R_C_BTN_COLOR(1), R_C_BTN_COLOR(2),
                                interfaceCtx->cLeftAlpha);
            } else if (temp == 2) {
                gDPSetPrimColor(OVERLAY_DISP++, 0, 0, R_C_BTN_COLOR(0), R_C_BTN_COLOR(1), R_C_BTN_COLOR(2),
                                interfaceCtx->cDownAlpha);
            } else {
                gDPSetPrimColor(OVERLAY_DISP++, 0, 0, R_C_BTN_COLOR(0), R_C_BTN_COLOR(1), R_C_BTN_COLOR(2),
                                interfaceCtx->cRightAlpha);
            }

            OVERLAY_DISP = Gfx_TextureIA8(OVERLAY_DISP, ((u8*)gButtonBackgroundTex + ((32 * 32) * (temp + 1))), 32, 32,
                                          R_ITEM_BTN_X(temp), R_ITEM_BTN_Y(temp), R_ITEM_BTN_WIDTH(temp),
                                          R_ITEM_BTN_WIDTH(temp), R_ITEM_BTN_DD(temp) << 1, R_ITEM_BTN_DD(temp) << 1);
        }
    }

    CLOSE_DISPS(play->state.gfxCtx, "../z_parameter.c", 3071);
}



Asm_VanillaHook(func_80082850);
void func_80082850(PlayState* play, s16 maxAlpha) {
    InterfaceContext* interfaceCtx = &play->interfaceCtx;
    s16 alpha = 255 - maxAlpha;

    switch (gSaveContext.unk_13E8) {
        case 1:
        case 2:
        case 8:
            osSyncPrintf("a_alpha=%d, c_alpha=%d   →   ", interfaceCtx->aAlpha, interfaceCtx->cLeftAlpha);

            if (gSaveContext.unk_13E8 == 8) {
                if (interfaceCtx->bAlpha != 255) {
                    interfaceCtx->bAlpha = alpha;
                }
            } else {
                if ((interfaceCtx->bAlpha != 0) && (interfaceCtx->bAlpha > maxAlpha)) {
                    interfaceCtx->bAlpha = maxAlpha;
                }
            }

            if ((interfaceCtx->aAlpha != 0) && (interfaceCtx->aAlpha > maxAlpha)) {
                interfaceCtx->aAlpha = maxAlpha;
            }

            if ((interfaceCtx->cLeftAlpha != 0) && (interfaceCtx->cLeftAlpha > maxAlpha)) {
                interfaceCtx->cLeftAlpha = maxAlpha;
            }

            if ((interfaceCtx->cDownAlpha != 0) && (interfaceCtx->cDownAlpha > maxAlpha)) {
                interfaceCtx->cDownAlpha = maxAlpha;
            }

            if ((interfaceCtx->cRightAlpha != 0) && (interfaceCtx->cRightAlpha > maxAlpha)) {
                interfaceCtx->cRightAlpha = maxAlpha;
            }

            if ((interfaceCtx->healthAlpha != 0) && (interfaceCtx->healthAlpha > maxAlpha)) {
                interfaceCtx->healthAlpha = maxAlpha;
            }

            if ((interfaceCtx->magicAlpha != 0) && (interfaceCtx->magicAlpha > maxAlpha)) {
                interfaceCtx->magicAlpha = maxAlpha;
            }



            osSyncPrintf("a_alpha=%d, c_alpha=%d\n", interfaceCtx->aAlpha, interfaceCtx->cLeftAlpha);

            break;
        case 3:
            if ((interfaceCtx->aAlpha != 0) && (interfaceCtx->aAlpha > maxAlpha)) {
                interfaceCtx->aAlpha = maxAlpha;
            }

            func_8008277C(play, maxAlpha, alpha);

            if ((interfaceCtx->magicAlpha != 0) && (interfaceCtx->magicAlpha > maxAlpha)) {
                interfaceCtx->magicAlpha = maxAlpha;
            }


            break;
        case 4:
            if ((interfaceCtx->bAlpha != 0) && (interfaceCtx->bAlpha > maxAlpha)) {
                interfaceCtx->bAlpha = maxAlpha;
            }

            if ((interfaceCtx->aAlpha != 0) && (interfaceCtx->aAlpha > maxAlpha)) {
                interfaceCtx->aAlpha = maxAlpha;
            }

            if ((interfaceCtx->cLeftAlpha != 0) && (interfaceCtx->cLeftAlpha > maxAlpha)) {
                interfaceCtx->cLeftAlpha = maxAlpha;
            }

            if ((interfaceCtx->cDownAlpha != 0) && (interfaceCtx->cDownAlpha > maxAlpha)) {
                interfaceCtx->cDownAlpha = maxAlpha;
            }

            if ((interfaceCtx->cRightAlpha != 0) && (interfaceCtx->cRightAlpha > maxAlpha)) {
                interfaceCtx->cRightAlpha = maxAlpha;
            }

            if ((interfaceCtx->healthAlpha != 0) && (interfaceCtx->healthAlpha > maxAlpha)) {
                interfaceCtx->healthAlpha = maxAlpha;
            }

            if ((interfaceCtx->magicAlpha != 0) && (interfaceCtx->magicAlpha > maxAlpha)) {
                interfaceCtx->magicAlpha = maxAlpha;
            }


            if (interfaceCtx->aAlpha != 255) {
                interfaceCtx->aAlpha = alpha;
            }

            break;
        case 5:
            func_8008277C(play, maxAlpha, alpha);

            if (interfaceCtx->aAlpha != 255) {
                interfaceCtx->aAlpha = alpha;
            }

            break;
        case 6:
            func_8008277C(play, maxAlpha, alpha);

            if (interfaceCtx->aAlpha != 255) {
                interfaceCtx->aAlpha = alpha;
            }

            switch (play->sceneId) {
                case SCENE_SPOT00:
                case SCENE_SPOT01:
                case SCENE_SPOT02:
                case SCENE_SPOT03:
                case SCENE_SPOT04:
                case SCENE_SPOT05:
                case SCENE_SPOT06:
                case SCENE_SPOT07:
                case SCENE_SPOT08:
                case SCENE_SPOT09:
                case SCENE_SPOT10:
                case SCENE_SPOT11:
                case SCENE_SPOT12:
                case SCENE_SPOT13:
                case SCENE_SPOT15:
                case SCENE_SPOT16:
                case SCENE_SPOT17:
                case SCENE_SPOT18:
                case SCENE_SPOT20:
                case SCENE_GANON_TOU:
                    break;
                default:
                
                    break;
            }
            break;
        case 7:

            func_80082644(play, alpha);

            break;
        case 9:
            if ((interfaceCtx->bAlpha != 0) && (interfaceCtx->bAlpha > maxAlpha)) {
                interfaceCtx->bAlpha = maxAlpha;
            }

            if ((interfaceCtx->aAlpha != 0) && (interfaceCtx->aAlpha > maxAlpha)) {
                interfaceCtx->aAlpha = maxAlpha;
            }

            if ((interfaceCtx->cLeftAlpha != 0) && (interfaceCtx->cLeftAlpha > maxAlpha)) {
                interfaceCtx->cLeftAlpha = maxAlpha;
            }

            if ((interfaceCtx->cDownAlpha != 0) && (interfaceCtx->cDownAlpha > maxAlpha)) {
                interfaceCtx->cDownAlpha = maxAlpha;
            }

            if ((interfaceCtx->cRightAlpha != 0) && (interfaceCtx->cRightAlpha > maxAlpha)) {
                interfaceCtx->cRightAlpha = maxAlpha;
            }

            break;
        case 10:
            if ((interfaceCtx->aAlpha != 0) && (interfaceCtx->aAlpha > maxAlpha)) {
                interfaceCtx->aAlpha = maxAlpha;
            }

            if ((interfaceCtx->cLeftAlpha != 0) && (interfaceCtx->cLeftAlpha > maxAlpha)) {
                interfaceCtx->cLeftAlpha = maxAlpha;
            }

            if ((interfaceCtx->cDownAlpha != 0) && (interfaceCtx->cDownAlpha > maxAlpha)) {
                interfaceCtx->cDownAlpha = maxAlpha;
            }

            if ((interfaceCtx->cRightAlpha != 0) && (interfaceCtx->cRightAlpha > maxAlpha)) {
                interfaceCtx->cRightAlpha = maxAlpha;
            }

            if ((interfaceCtx->healthAlpha != 0) && (interfaceCtx->healthAlpha > maxAlpha)) {
                interfaceCtx->healthAlpha = maxAlpha;
            }

            if ((interfaceCtx->magicAlpha != 0) && (interfaceCtx->magicAlpha > maxAlpha)) {
                interfaceCtx->magicAlpha = maxAlpha;
            }


            if (interfaceCtx->bAlpha != 255) {
                interfaceCtx->bAlpha = alpha;
            }

            break;
        case 11:
            if ((interfaceCtx->bAlpha != 0) && (interfaceCtx->bAlpha > maxAlpha)) {
                interfaceCtx->bAlpha = maxAlpha;
            }

            if ((interfaceCtx->aAlpha != 0) && (interfaceCtx->aAlpha > maxAlpha)) {
                interfaceCtx->aAlpha = maxAlpha;
            }

            if ((interfaceCtx->cLeftAlpha != 0) && (interfaceCtx->cLeftAlpha > maxAlpha)) {
                interfaceCtx->cLeftAlpha = maxAlpha;
            }

            if ((interfaceCtx->cDownAlpha != 0) && (interfaceCtx->cDownAlpha > maxAlpha)) {
                interfaceCtx->cDownAlpha = maxAlpha;
            }

            if ((interfaceCtx->cRightAlpha != 0) && (interfaceCtx->cRightAlpha > maxAlpha)) {
                interfaceCtx->cRightAlpha = maxAlpha;
            }


            if ((interfaceCtx->magicAlpha != 0) && (interfaceCtx->magicAlpha > maxAlpha)) {
                interfaceCtx->magicAlpha = maxAlpha;
            }

            break;
        case 12:
            if (interfaceCtx->aAlpha != 255) {
                interfaceCtx->aAlpha = alpha;
            }

            if (interfaceCtx->bAlpha != 255) {
                interfaceCtx->bAlpha = alpha;
            }


            if ((interfaceCtx->cLeftAlpha != 0) && (interfaceCtx->cLeftAlpha > maxAlpha)) {
                interfaceCtx->cLeftAlpha = maxAlpha;
            }

            if ((interfaceCtx->cDownAlpha != 0) && (interfaceCtx->cDownAlpha > maxAlpha)) {
                interfaceCtx->cDownAlpha = maxAlpha;
            }

            if ((interfaceCtx->cRightAlpha != 0) && (interfaceCtx->cRightAlpha > maxAlpha)) {
                interfaceCtx->cRightAlpha = maxAlpha;
            }

            if ((interfaceCtx->magicAlpha != 0) && (interfaceCtx->magicAlpha > maxAlpha)) {
                interfaceCtx->magicAlpha = maxAlpha;
            }

            if ((interfaceCtx->healthAlpha != 0) && (interfaceCtx->healthAlpha > maxAlpha)) {
                interfaceCtx->healthAlpha = maxAlpha;
            }

            break;
        case 13:
            func_8008277C(play, maxAlpha, alpha);

            if ((interfaceCtx->aAlpha != 0) && (interfaceCtx->aAlpha > maxAlpha)) {
                interfaceCtx->aAlpha = maxAlpha;
            }


            break;
    }
}


