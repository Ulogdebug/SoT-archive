#include <z64hdr.h>
#include "code/z_player_lib.h"
#include "playas/playas.h"
#include "uLib.h"
#include "z64camera.h"
#include "z64player.h"
#include "../system/kaleido/0x01-Player/playas_adult.h"
#include "../system/kaleido/0x01-Player/playas_child.h"
#include "state/state.h"
static s16 scroll = 0;
void Player_PostLimbDrawGameplayNew(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    Player* this = (Player*)thisx;
    f32 intensity = (Rand_ZeroOne() * 0.5f) + 0.5f;
    static Gfx* sAdultMaskDlists[PLAYER_MASK_MAX - 1] = {
        gPlayasAdult_DlMaskTruth, gPlayasAdult_DlMaskTruth, gPlayasAdult_DlMaskTruth,  gPlayasAdult_DlMaskTruth,
        gPlayasAdult_DlMaskTruth,  gPlayasAdult_DlMaskTruth,  gPlayasAdult_DlMaskTruth,  gPlayasAdult_DlMaskTruth,
    };


    Camera cam = *Play_GetCamera(play,CAM_ID_MAIN);

    if (*dList != NULL) {
        Matrix_MultVec3f(&sZeroVec, sCurBodyPartPos);
    }

    //hack gauntlet/bracelets not in first person view
    if (CUR_UPG_VALUE(UPG_STRENGTH) != 0 && cam.mode != 6 && cam.mode != 7 && cam.mode != 11){
        DrawGauntletDList(play,this,limbIndex);
    }

    
    if (limbIndex == PLAYER_LIMB_L_HAND) {
        MtxF sp14C;
        Actor* hookedActor;

        Math_Vec3f_Copy(&this->leftHandPos, sCurBodyPartPos);
        
        if (this->itemActionParam == PLAYER_AP_STICK) {

            Vec3f sp124[3];
            OPEN_DISPS(play->state.gfxCtx, "../z_player_lib.c", 2633);

            if (this->actor.scale.y >= 0.0f) {
                D_80126080.x = this->unk_85C * 5000.0f;
                func_80090A28(this, sp124);
                if (this->meleeWeaponState != 0) {
                    func_800906D4(play, this, sp124);
                } else {
                    Math_Vec3f_Copy(&this->meleeWeaponInfo[0].tip, &sp124[0]);
                }
                // if(this->unk_860!=0 && this->unk_85C > 0.5f){
                //     Lights_PointGlowSetInfo(&ghackSave.lightInfo, sp124[0].x, sp124[0].y,
                //                     sp124[0].z, 255, 255, 180, 60);
                //     Lights_PointSetColorAndRadius(&ghackSave.lightInfo, 255 * intensity, 200 * intensity, 0, 180);
                // }
            }

            //Shrinking of stick when burning down
            if (this->unk_85C > 0.5f) {
                Matrix_Scale(this->unk_85C, 1.0f, 1.0f, MTXMODE_APPLY);
            }

            gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx, "../z_player_lib.c", 2653),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

            //Stick Display Lists
            //D_80126080.x = 2500.0f; //Used to assign length manually if changed
            if (LINK_IS_ADULT) {
                if (this->unk_85C > 0.5f) {
                    gSPDisplayList(POLY_OPA_DISP++, gPlayasAdult_DlDekuStick);
                } else {
                    gSPDisplayList(POLY_OPA_DISP++, gPlayasAdult_DlDekuStickBrokenHeld);
                }

            } else {
                if (this->unk_85C > 0.5f) {
                    //gSPDisplayList(POLY_OPA_DISP++, gPlayasChild_DlDekuStick);
                } else {
                    //gSPDisplayList(POLY_OPA_DISP++, gPlayasChild_DlDekuStickBrokenHeld);
                }
            }

            CLOSE_DISPS(play->state.gfxCtx, "../z_player_lib.c", 2656);
        } else if ((this->actor.scale.y >= 0.0f) && (this->meleeWeaponState != 0)) {
            Vec3f spE4[3];

            if (this->heldItemActionParam != PLAYER_AP_STICK){
                if (Player_HoldsBrokenKnife(this)){
                    D_80126080.x = 1500.0f;
                } else {
                    D_80126080.x = sMeleeWeaponLengths[Player_GetMeleeWeaponHeld(this)];
                }
            }

            func_80090A28(this, spE4);
            func_800906D4(play, this, spE4);
        } else if ((*dList != NULL) && (this->leftHandType == PLAYER_MODELTYPE_LH_BOTTLE)) {

            // fixes a vanilla bug for shielding with an empty bottle showing a bottle color out of range
            s8 bottlemap = Player_ActionToBottle(this, this->itemActionParam);
            
            if (bottlemap < 0) {
                bottlemap = 0;
            }

            Color_RGB8* bottleColor = &sBottleColors[bottlemap];
            OPEN_DISPS(play->state.gfxCtx, "../z_player_lib.c", 2710);
            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, "../z_player_lib.c", 2712),
                    G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gsDPSetPrimColor(0x00, 0x00, 255, 255, 255, 178);
            gDPSetEnvColor(POLY_XLU_DISP++, bottleColor->r, bottleColor->g, bottleColor->b, 0);
            gSPDisplayList(POLY_XLU_DISP++, sBottleDLists[((void)0, gSaveContext.linkAge)]);

            CLOSE_DISPS(play->state.gfxCtx, "../z_player_lib.c", 2717);
        }

        if (this->actor.scale.y >= 0.0f) {
            if (!Player_HoldsHookshot(this) && ((hookedActor = this->heldActor) != NULL)) {
                if (this->stateFlags1 & PLAYER_STATE1_READY_TO_SHOOT) {
                    Matrix_MultVec3f(&D_80126128, &hookedActor->world.pos);
                    Matrix_RotateZYX(0x69E8, -0x5708, 0x458E, MTXMODE_APPLY);
                    Matrix_Get(&sp14C);
                    Matrix_MtxFToYXZRotS(&sp14C, &hookedActor->world.rot, 0);
                    hookedActor->shape.rot = hookedActor->world.rot;
                } else if (this->stateFlags1 & PLAYER_STATE1_HOLDING_ACTOR) {
                    Vec3s spB8;

                    Matrix_Get(&sp14C);
                    Matrix_MtxFToYXZRotS(&sp14C, &spB8, 0);

                    if (hookedActor->flags & ACTOR_FLAG_17) {
                        hookedActor->world.rot.x = hookedActor->shape.rot.x = spB8.x - this->unk_3BC.x;
                    } else {
                        hookedActor->world.rot.y = hookedActor->shape.rot.y = this->actor.shape.rot.y + this->unk_3BC.y;
                    }
                }
            } else {
                Matrix_Get(&this->mf_9E0);
                Matrix_MtxFToYXZRotS(&this->mf_9E0, &this->unk_3BC, 0);
            }
        }
        Actor* heldActor = this->heldActor;
        if (this->actor.scale.y >= 0.0f) {
            if((this->rightHandType == PLAYER_MODELTYPE_RH_BOW_SLINGSHOT)){
                if ((heldActor != NULL) ) {
                    //Debug_Text(255,255,255,10,10,"Bow Dot Spawned");
                    DrawGeneralDot(play,this);
                }
                
            }     
        }   

    } else if (limbIndex == PLAYER_LIMB_R_HAND) {
        Actor* heldActor = this->heldActor;
        if (this->rightHandType == PLAYER_MODELTYPE_RH_FF) {
            Matrix_Get(&this->shieldMf);
        } else if ((this->rightHandType == PLAYER_MODELTYPE_RH_BOW_SLINGSHOT) ||
                   (this->rightHandType == PLAYER_MODELTYPE_RH_BOW_SLINGSHOT_2)) {
            BowSlingshotStringData* stringData = &sBowSlingshotStringData[gSaveContext.linkAge];

            OPEN_DISPS(play->state.gfxCtx, "../z_player_lib.c", 2783);

            Matrix_Push();
            Matrix_Translate(stringData->pos.x, stringData->pos.y, stringData->pos.z, MTXMODE_APPLY);

            if ((this->stateFlags1 & PLAYER_STATE1_READY_TO_SHOOT) && (this->unk_860 >= 0) && (this->unk_834 <= 10)) {
                Vec3f sp90;
                f32 distXYZ;

                Matrix_MultVec3f(&sZeroVec, &sp90);
                distXYZ = Math_Vec3f_DistXYZ(sCurBodyPartPos, &sp90);

                this->unk_858 = distXYZ - 3.0f;
                if (distXYZ < 3.0f) {
                    this->unk_858 = 0.0f;
                } else {
                    this->unk_858 *= 1.6f;
                    if (this->unk_858 > 1.0f) {
                        this->unk_858 = 1.0f;
                    }
                }

                this->unk_85C = -0.5f;
            }

            Matrix_Scale(1.0f, this->unk_858, 1.0f, MTXMODE_APPLY);

            if (!LINK_IS_ADULT) {
                Matrix_RotateZ(this->unk_858 * -0.2f, MTXMODE_APPLY);
            }

            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, "../z_player_lib.c", 2804),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, stringData->dList);

            Matrix_Pop();

            CLOSE_DISPS(play->state.gfxCtx, "../z_player_lib.c", 2809);
        } else if ((this->actor.scale.y >= 0.0f) && (this->rightHandType == PLAYER_MODELTYPE_RH_SHIELD)) {
            Matrix_Get(&this->shieldMf);
            Player_UpdateShieldCollider(play, this, &this->shieldQuad, sRightHandLimbModelShieldQuadVertices);
        }

        if (this->actor.scale.y >= 0.0f) {
            if ((this->heldItemActionParam == PLAYER_AP_HOOKSHOT) ||
                (this->heldItemActionParam == PLAYER_AP_LONGSHOT) ) {
                Matrix_MultVec3f(&D_80126184, &this->unk_3C8);
                if (heldActor != NULL) {
                    MtxF sp44;
                    s32 pad;

                    Matrix_MultVec3f(&D_80126190, &heldActor->world.pos);
                    Matrix_RotateZYX(0, -0x4000, -0x4000, MTXMODE_APPLY);
                    Matrix_Get(&sp44);
                    Matrix_MtxFToYXZRotS(&sp44, &heldActor->world.rot, 0);
                    heldActor->shape.rot = heldActor->world.rot;

                    if (func_8002DD78(this) != 0) {
                        Matrix_Translate(500.0f, 300.0f, 0.0f, MTXMODE_APPLY);
                        Player_DrawHookshotReticle(
                            play, this, (this->heldItemActionParam == PLAYER_AP_HOOKSHOT) ? 38600.0f : 77600.0f);
                    }
                }
            }


            if ((this->unk_862 != 0) || ((func_8002DD6C(this) == 0) && (heldActor != NULL))) {
                if (!(this->stateFlags1 & PLAYER_STATE1_10) && (this->unk_862 != 0) &&
                    (this->exchangeItemId != EXCH_ITEM_NONE)) {
                    Math_Vec3f_Copy(&sGetItemRefPos, &this->leftHandPos);
                } else {
                    sGetItemRefPos.x = (this->bodyPartsPos[PLAYER_BODYPART_R_HAND].x + this->leftHandPos.x) * 0.5f;
                    sGetItemRefPos.y = (this->bodyPartsPos[PLAYER_BODYPART_R_HAND].y + this->leftHandPos.y) * 0.5f;
                    sGetItemRefPos.z = (this->bodyPartsPos[PLAYER_BODYPART_R_HAND].z + this->leftHandPos.z) * 0.5f;
                }

                if (this->unk_862 == 0) {
                    Math_Vec3f_Copy(&heldActor->world.pos, &sGetItemRefPos);
                }
            }
        }        
    } else if (this->actor.scale.y >= 0.0f) {
        if (limbIndex == PLAYER_LIMB_SHEATH) {
            if ((this->rightHandType != PLAYER_MODELTYPE_RH_SHIELD) &&
                (this->rightHandType != PLAYER_MODELTYPE_RH_FF)) {

                Matrix_TranslateRotateZYX(&sSheathLimbModelShieldOnBackPos, &sSheathLimbModelShieldOnBackZyxRot);
                Matrix_Get(&this->shieldMf);
            }
        } else if (limbIndex == PLAYER_LIMB_HEAD) {
        Matrix_MultVec3f(&sPlayerFocusHeadLimbModelPos, &this->actor.focus.pos);
        // Hack Masks excluding bunny for physics
        if(!(this->stateFlags1 & PLAYER_STATE1_IN_FIRST_PERSON_MODE)){ //If FPS mode is true
            if(this->currentMask == PLAYER_MASK_NONE){
                if(play->actorCtx.lensActive == true){
                    gSPSegment(POLY_OPA_DISP++, 0x08, Gfx_TexScroll(play->state.gfxCtx, 0, scroll, 32, 32));
                    scroll += 4;
                    gSPDisplayList(POLY_OPA_DISP++, gPlayasAdult_DlMaskTruth);
                }
            }
            if (this->currentMask != PLAYER_MASK_NONE && this->currentMask != PLAYER_MASK_BUNNY) {
                //Select correct mask based on age
                Gfx** maskDListsToUse = sAdultMaskDlists;
                gSPDisplayList(POLY_OPA_DISP++, maskDListsToUse[this->currentMask - 1]);
            }
        }

        } else if (limbIndex == PLAYER_LIMB_L_FOOT || limbIndex == PLAYER_LIMB_R_FOOT) {
            Vec3f footpos = {200.0f, 300.0f, 0.0f};
            Actor_SetFeetPos(&this->actor, limbIndex, PLAYER_LIMB_L_FOOT, &footpos, PLAYER_LIMB_R_FOOT, &footpos);
        }
    }
}