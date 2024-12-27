#include <uLib.h>
#include <code/z_actor.h>

Asm_VanillaHook(func_8002C124);
void func_8002C124(TargetContext* targetCtx, PlayState* play) {
    Actor* actor = targetCtx->targetedActor;
    NaviColor hackNaviColorList[] = {
/* 0x00 */{ { 0, 255, 0, 255 }, { 0, 255, 0, 0 } },  //ACTORCAT_SWITCH
/* 0x01 */{ { 0, 255, 0, 255 }, { 0, 255, 0, 0 } },     //ACTORCAT_BG
/* 0x02 */{ { 255, 255, 255, 255 }, { 0, 0, 255, 0 } },     //ACTORCAT_PLAYER
/* 0x03 */{ { 255, 195, 0, 255 }, { 0, 255, 0, 0 } },     //ACTORCAT_EXPLOSIVE
/* 0x04 */{ { 150, 150, 255, 255 }, { 150, 150, 255, 0 } }, //ACTORCAT_NPC
/* 0x05 */{ { 255, 195, 0, 255 }, { 255, 195, 0, 0 } }, //ENEMY
/* 0x06 */{ { 0, 255, 0, 255 }, { 0, 255, 0, 0 } },    //ACTORCAT_PROP
          { { 0, 255, 0, 255 }, { 0, 255, 0, 0 } },     //ACTORCAT_ITEMACTION
          { { 0, 255, 0, 255 }, { 0, 255, 0, 0 } },   //ACTORCAT_MISC      
          { { 255, 195, 0, 255 }, { 200, 195, 0, 0 } }, //ACTORCAT_BOSS
          { { 0, 255, 0, 255 }, { 0, 255, 0, 0 } },         
          { { 0, 255, 0, 255 }, { 0, 255, 0, 0 } },
          { { 0, 255, 0, 255 }, { 0, 255, 0, 0 } },
    };

    OPEN_DISPS(play->state.gfxCtx, "../z_actor.c", 2029);

    if (targetCtx->unk_48 != 0) {
        TargetContextEntry* entry;
        Player* player;
        s16 spCE;
        f32 temp1;
        Vec3f projTargetCenter;
        s32 spB8;
        f32 projTargetCappedInvW;
        s32 spB0;
        s32 spAC;
        f32 var1;
        f32 var2;
        s32 i;

        player = GET_PLAYER(play);

        spCE = 0xFF;
        var1 = 1.0f;

        if (targetCtx->unk_4B != 0) {
            spB8 = 1;
        } else {
            spB8 = 3;
        }

        if (actor != NULL) {
            Math_Vec3f_Copy(&targetCtx->targetCenterPos, &actor->focus.pos);
            var1 = (500.0f - targetCtx->unk_44) / 420.0f;
        } else {
            targetCtx->unk_48 -= 120;
            if (targetCtx->unk_48 < 0) {
                targetCtx->unk_48 = 0;
            }
            spCE = targetCtx->unk_48;
        }

        Actor_ProjectPos(play, &targetCtx->targetCenterPos, &projTargetCenter, &projTargetCappedInvW);

        projTargetCenter.x = (160 * (projTargetCenter.x * projTargetCappedInvW)) * var1;
        projTargetCenter.x = CLAMP(projTargetCenter.x, -320.0f, 320.0f);

        projTargetCenter.y = (120 * (projTargetCenter.y * projTargetCappedInvW)) * var1;
        projTargetCenter.y = CLAMP(projTargetCenter.y, -240.0f, 240.0f);

        projTargetCenter.z = projTargetCenter.z * var1;

        targetCtx->unk_4C--;
        if (targetCtx->unk_4C < 0) {
            targetCtx->unk_4C = 2;
        }

        func_8002BE64(targetCtx, targetCtx->unk_4C, projTargetCenter.x, projTargetCenter.y, projTargetCenter.z);

        if (!(player->stateFlags1 & PLAYER_STATE1_6) || (actor != player->unk_664)) {

            OVERLAY_DISP = Gfx_SetupDL(OVERLAY_DISP, SETUPDL_57);
             for (spB0 = 0, spAC = targetCtx->unk_4C; spB0 < spB8; spB0++, spAC = (spAC + 1) % 3) {
                 entry = &targetCtx->arr_50[spAC];

                 if (entry->unk_0C < 500.0f) {
                     if (entry->unk_0C <= 120.0f) {
                         var2 = 0.15f;
                     } else {
                         var2 = ((entry->unk_0C - 120.0f) * 0.001f) + 0.15f;
                     }

                    Matrix_Translate(entry->pos.x, entry->pos.y + 8.0f, 0.0f, MTXMODE_NEW);
                    Matrix_Scale(var2, 0.15f, 1.0f, MTXMODE_APPLY);
                    //gDPSetPrimColor(OVERLAY_DISP++, 0, 0, entry->color.r, entry->color.g, entry->color.b, (u8)spCE);
                    if (actor != NULL){
                        NaviColor* naviColor1 = &hackNaviColorList[actor->category];
                        gDPSetPrimColor(OVERLAY_DISP++, 0, 0, naviColor1->inner.r, naviColor1->inner.g, naviColor1->inner.b, (u8)spCE);
                    }

                    //Matrix_RotateZ((targetCtx->unk_4B & 0x7F) * (M_PI / 64), MTXMODE_APPLY);

                     //for (i = 0; i < 3; i++) {
                         Matrix_RotateZ(M_PI / 4, MTXMODE_APPLY);
                         Matrix_Push();
                         Matrix_Translate(entry->unk_0C, entry->unk_0C, 0.0f, MTXMODE_APPLY);
                         gSPMatrix(OVERLAY_DISP++, Matrix_NewMtx(play->state.gfxCtx, "../z_actor.c", 2116),
                                   G_MTX_MODELVIEW | G_MTX_LOAD);
                         gSPDisplayList(OVERLAY_DISP++, gZTargetLockOnTriangleDL);
                         Matrix_Pop();
                     //}
                 }

                 spCE -= 0xFF / 3;
                 if (spCE < 0) {
                     spCE = 0;
                 }
             }
        }
    }

    actor = targetCtx->unk_94;
    if ((actor != NULL) && !(actor->flags & ACTOR_FLAG_27)) {
        NaviColor* naviColor = &hackNaviColorList[actor->category];
        POLY_XLU_DISP = Gfx_SetupDL(POLY_XLU_DISP, SETUPDL_7);

        Matrix_Translate(actor->focus.pos.x, actor->focus.pos.y + (actor->targetArrowOffset * actor->scale.y) + 17.0f,
                         actor->focus.pos.z, MTXMODE_NEW);
        Matrix_RotateY(BINANG_TO_RAD((u16)(play->gameplayFrames * 3000)), MTXMODE_APPLY);
        Matrix_Scale((iREG(27) + 35) / 1000.0f, (iREG(28) + 60) / 1000.0f, (iREG(29) + 50) / 1000.0f, MTXMODE_APPLY);

        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, naviColor->inner.r, naviColor->inner.g, naviColor->inner.b, 255);
        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, "../z_actor.c", 2153),
                  G_MTX_MODELVIEW | G_MTX_LOAD);
        gSPDisplayList(POLY_XLU_DISP++, gZTargetArrowDL);
    }

    CLOSE_DISPS(play->state.gfxCtx, "../z_actor.c", 2158);
}