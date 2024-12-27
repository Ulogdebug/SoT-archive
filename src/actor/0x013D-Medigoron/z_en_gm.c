/*
 * File: z_en_gm.c
 * Overlay: ovl_En_Gm
 * Description: Medi-Goron
 */
#include <uLib.h>
#include "z_en_gm.h"
#include "assets/objects/object_oF1d_map/object_oF1d_map.h"
#include "assets/objects/object_gm/object_gm.h"
#include "vt.h"

#define FLAGS (ACTOR_FLAG_0 | ACTOR_FLAG_3 | ACTOR_FLAG_4)

void EnGm_Init(Actor* thisx, PlayState* play);
void EnGm_Destroy(Actor* thisx, PlayState* play);
void EnGm_Update(Actor* thisx, PlayState* play);
void EnGm_Draw(Actor* thisx, PlayState* play);

void EnGm_IntSetup(EnGm* this, PlayState* play);
void func_80A3DFBC(EnGm* this, PlayState* play);
void EnGm_SwitchJudge(EnGm* this, PlayState* play);
void EnGm_DialogueJudge(EnGm* this, PlayState* play);
void func_80A3DBF4(EnGm* this, PlayState* play);
void EnGm_DialogueEnd(EnGm* this, PlayState* play);
void EnGm_ProcessBuyChoice(EnGm* this, PlayState* play);
void EnGm_TradeSetup(EnGm* this, PlayState* play);
void EnGm_Trade(EnGm* this, PlayState* play);
void EnGm_ProcessFixChoice(EnGm* this, PlayState* play);
void EnGm_GiveKinfe(EnGm* this,PlayState* play) ;

const ActorInit En_Gm_InitVars = {
    ACTOR_EN_GM,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_OF1D_MAP,
    sizeof(EnGm),
    (ActorFunc)EnGm_Init,
    (ActorFunc)EnGm_Destroy,
    (ActorFunc)EnGm_Update,
    NULL,
};

static ColliderCylinderInitType1 sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_TYPE_ALL,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x00000000, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_NONE,
        OCELEM_ON,
    },
    { 100, 120, 0, { 0, 0, 0 } },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_U8(targetMode, 5, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 30, ICHAIN_STOP),
};

void EnGm_Init(Actor* thisx, PlayState* play) {
    EnGm* this = (EnGm*)thisx;

    Actor_ProcessInitChain(&this->actor, sInitChain);

    // "Medi Goron"
    osSyncPrintf(VT_FGCOL(GREEN) "%s[%d] : 中ゴロン[%d]" VT_RST "\n", "../z_en_gm.c", 133, this->actor.params);

    this->objGmBankIndex = Object_GetIndex(&play->objectCtx, OBJECT_GM);

    if (this->objGmBankIndex < 0) {
        osSyncPrintf(VT_COL(RED, WHITE));
        // "There is no model bank! !! (Medi Goron)"
        osSyncPrintf("モデル バンクが無いよ！！（中ゴロン）\n");
        osSyncPrintf(VT_RST);
        ASSERT(0, "0", "../z_en_gm.c", 145);
    }

    this->updateFunc = EnGm_IntSetup;
}

void EnGm_Destroy(Actor* thisx, PlayState* play) {
    EnGm* this = (EnGm*)thisx;

    Collider_DestroyCylinder(play, &this->collider);
}

s32 EnGm_playerStateJudge(void) {
    if (LINK_AGE_IN_YEARS == YEARS_CHILD) {
        return 0;
    } else if (!CHECK_OWNED_EQUIP_ALT(EQUIP_TYPE_SWORD, EQUIP_INV_SWORD_BGS)) {//if your third slot is empty
        return 1;
    } else if (CHECK_OWNED_EQUIP_ALT(EQUIP_TYPE_SWORD, EQUIP_INV_SWORD_BROKENGIANTKNIFE)) {//if you have BrokenGiantKnife
        return 2;
    } else {
        return 3;//3, if you have GiantKnife
    }
}

void EnGm_IntSetup(EnGm* this, PlayState* play) {
    if (Object_IsLoaded(&play->objectCtx, this->objGmBankIndex)) {
        this->actor.flags &= ~ACTOR_FLAG_4;
        SkelAnime_InitFlex(play, &this->skelAnime, &gGoronSkel, NULL, this->jointTable, this->morphTable, 18);
        gSegments[6] = VIRTUAL_TO_PHYSICAL(play->objectCtx.status[this->objGmBankIndex].segment);
        Animation_Change(&this->skelAnime, &object_gm_Anim_0002B8, 1.0f, 0.0f,
                         Animation_GetLastFrame(&object_gm_Anim_0002B8), ANIMMODE_LOOP, 0.0f);
        this->actor.draw = EnGm_Draw;
        Collider_InitCylinder(play, &this->collider);
        Collider_SetCylinderType1(play, &this->collider, &this->actor, &sCylinderInit);
        ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 35.0f);
        Actor_SetScale(&this->actor, 0.05f);
        this->actor.colChkInfo.mass = MASS_IMMOVABLE;
        this->eyeTexIndex = 0;
        this->blinkTimer = 20;
        this->actor.textId = 0x3049;
        this->updateFunc = func_80A3DFBC;
        this->actionFunc = EnGm_SwitchJudge;
        this->actor.speedXZ = 0.0f;
        this->actor.gravity = -1.0f;
        this->actor.velocity.y = 0.0f;
    }
}

void EnGm_UpdateEye(EnGm* this) {
    if (this->blinkTimer != 0) {
        this->blinkTimer--;
    } else {
        this->eyeTexIndex++;

        if (this->eyeTexIndex >= 3) {
            this->eyeTexIndex = 0;
            this->blinkTimer = Rand_ZeroFloat(60.0f) + 20.0f;
        }
    }
}

void EnGm_SetTextID(EnGm* this) {
    switch (EnGm_playerStateJudge()) {
        case 0:
            if (GET_INFTABLE(INFTABLE_B0)) {
                this->actor.textId = 0x304B;
            } else {
                this->actor.textId = 0x304A;
            }
            break;
        case 1:
            if (GET_INFTABLE(INFTABLE_B1)) {
                this->actor.textId = 0x304F;//ask you buy
            } else {
                this->actor.textId = 0x304C;
            }
            break;
        case 2:
            this->actor.textId = 0x304E;
            break;
        case 3:
            this->actor.textId = 0x304D;
            break;
    }
}

void EnGm_SwitchJudge(EnGm* this, PlayState* play) {
    f32 dx;
    f32 dz;
    Player* player = GET_PLAYER(play);

    dx = this->talkPos.x - player->actor.world.pos.x;
    dz = this->talkPos.z - player->actor.world.pos.z;

    if (Flags_GetSwitch(play, this->actor.params)) {
        EnGm_SetTextID(this);
        this->actionFunc = EnGm_DialogueJudge;
    } else if (Actor_ProcessTalkRequest(&this->actor, play)) {
        this->actionFunc = func_80A3DBF4;
    } else if ((this->collider.base.ocFlags1 & OC1_HIT) || (SQ(dx) + SQ(dz)) < SQ(100.0f)) {
        this->collider.base.acFlags &= ~AC_HIT;
        func_8002F2CC(&this->actor, play, 415.0f);
    }
}

void func_80A3DBF4(EnGm* this, PlayState* play) {
    if ((Message_GetState(&play->msgCtx) == TEXT_STATE_DONE) && Message_ShouldAdvance(play)) {
        this->actionFunc = EnGm_SwitchJudge;
    }
}

void EnGm_DialogueJudge(EnGm* this, PlayState* play) {
    f32 dx;
    f32 dz;
    s32 pad;
    Player* player = GET_PLAYER(play);

    EnGm_SetTextID(this);

    dx = this->talkPos.x - player->actor.world.pos.x;
    dz = this->talkPos.z - player->actor.world.pos.z;

    if (Actor_ProcessTalkRequest(&this->actor, play)) {
        switch (EnGm_playerStateJudge()) {
            case 0:
                SET_INFTABLE(INFTABLE_B0);
                FALLTHROUGH;
            case 3:
                this->actionFunc = EnGm_DialogueEnd;
                return;
            case 1://if you don't have the third slot sowrd
                SET_INFTABLE(INFTABLE_B1);
                if (GET_INFTABLE(INFTABLE_B1)){
                    this->actionFunc = EnGm_ProcessBuyChoice;
                }
                return;
            case 2: //ask you fix the sword
                this->actionFunc = EnGm_ProcessFixChoice;
                FALLTHROUGH;
            default:
                return;
        }

       // this->actionFunc = EnGm_ProcessBuyChoice;
    }
    if ((this->collider.base.ocFlags1 & OC1_HIT) || (SQ(dx) + SQ(dz)) < SQ(100.0f)) {
        this->collider.base.acFlags &= ~AC_HIT;
        func_8002F2CC(&this->actor, play, 415.0f);
    }
}

void EnGm_DialogueEnd(EnGm* this, PlayState* play) {
    u8 dialogState = Message_GetState(&play->msgCtx);

    if ((dialogState == TEXT_STATE_DONE || dialogState == TEXT_STATE_EVENT) && Message_ShouldAdvance(play)) {
        this->actionFunc = EnGm_DialogueJudge;
        if (dialogState == TEXT_STATE_EVENT) {
            play->msgCtx.msgMode = MSGMODE_TEXT_CLOSING;
            play->msgCtx.stateTimer = 4;
        }
    }
}

void EnGm_ProcessBuyChoice(EnGm* this, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == TEXT_STATE_CHOICE && Message_ShouldAdvance(play)) {
        switch (play->msgCtx.choiceIndex) {
            case 0: // yes
                if (gSaveContext.rupees < 200) {
                    Message_ContinueTextbox(play, 0xC8);
                    this->actionFunc = EnGm_DialogueEnd;
                } else {
                    EnGm_GiveKinfe(this,play);
                    gSaveContext.swordHealth = KNIFLE_HEALTH;
                    Rupees_ChangeBy(-200);                  
                    this->actionFunc = EnGm_TradeSetup;
                }
                break;
            case 1: // no
                Message_ContinueTextbox(play, 0x3050);
                this->actionFunc = EnGm_DialogueEnd;
                break; 
               
                
        }
    }
}
void EnGm_ProcessFixChoice(EnGm* this, PlayState* play) {

    if (Message_GetState(&play->msgCtx) == TEXT_STATE_CHOICE && Message_ShouldAdvance(play)) {
        switch (play->msgCtx.choiceIndex) {
        case 0: // yes
            if (gSaveContext.rupees < 50) {
                Message_ContinueTextbox(play, 0xC8);
                this->actionFunc = EnGm_DialogueEnd;
            }
            else {               
                EnGm_GiveKinfe(this,play);
                gSaveContext.swordHealth = KNIFLE_HEALTH;
                Rupees_ChangeBy(-50);                
                this->actionFunc = EnGm_TradeSetup;
            }
            break;
        case 1: // no
            Message_ContinueTextbox(play, 0x3050);
            this->actionFunc = EnGm_DialogueEnd;
            break;


        }
    }

}

void EnGm_GiveKinfe(EnGm* this,PlayState* play) {
    Player* player = GET_PLAYER(play);
    gSaveContext.inventory.equipment |= OWNED_EQUIP_FLAG(EQUIP_TYPE_SWORD, EQUIP_INV_SWORD_BGS);
    if (CHECK_OWNED_EQUIP_ALT(EQUIP_TYPE_SWORD, EQUIP_INV_SWORD_BROKENGIANTKNIFE)) {
        gSaveContext.inventory.equipment ^= OWNED_EQUIP_FLAG_ALT(EQUIP_TYPE_SWORD, EQUIP_INV_SWORD_BROKENGIANTKNIFE);
    }

    if (CHECK_OWNED_EQUIP_ALT(EQUIP_TYPE_SWORD, EQUIP_INV_SWORD_BROKENGIANTKNIFE)) {
        gSaveContext.equips.buttonItems[0] = ITEM_SWORD_KNIFE;
    } else {
        gSaveContext.equips.buttonItems[0] = ITEM_SWORD_KNIFE;
    }

    Interface_LoadItemIcon1(play, 0);
}

void EnGm_TradeSetup(EnGm* this, PlayState* play) {
    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        this->actionFunc = EnGm_Trade;
    } else {
        gSaveContext.swordHealth = KNIFLE_HEALTH;
        EnGm_GiveKinfe(this,play);
    }
}

void EnGm_Trade(EnGm* this, PlayState* play) {
    if ((Message_GetState(&play->msgCtx) == TEXT_STATE_DONE) && Message_ShouldAdvance(play)) {
        
        this->actionFunc = EnGm_DialogueJudge;
    }
}

void func_80A3DFBC(EnGm* this, PlayState* play) {
    gSegments[6] = VIRTUAL_TO_PHYSICAL(play->objectCtx.status[this->objGmBankIndex].segment);
    this->timer++;
    this->actionFunc(this, play);
    this->actor.focus.rot.x = this->actor.world.rot.x;
    this->actor.focus.rot.y = this->actor.world.rot.y;
    this->actor.focus.rot.z = this->actor.world.rot.z;
    EnGm_UpdateEye(this);
    SkelAnime_Update(&this->skelAnime);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
}

void EnGm_Update(Actor* thisx, PlayState* play) {
    EnGm* this = (EnGm*)thisx;

    this->updateFunc(this, play);
}

void func_80A3E090(EnGm* this) {
    Vec3f vec1;
    Vec3f vec2;

    Matrix_Push();
    Matrix_Translate(0.0f, 0.0f, 2600.0f, MTXMODE_APPLY);
    Matrix_RotateZYX(this->actor.world.rot.x, this->actor.world.rot.y, this->actor.world.rot.z, MTXMODE_APPLY);
    vec1.x = vec1.y = vec1.z = 0.0f;
    Matrix_MultVec3f(&vec1, &vec2);
    this->collider.dim.pos.x = vec2.x;
    this->collider.dim.pos.y = vec2.y;
    this->collider.dim.pos.z = vec2.z;
    Matrix_Pop();
    Matrix_Push();
    Matrix_Translate(0.0f, 0.0f, 4300.0f, MTXMODE_APPLY);
    Matrix_RotateZYX(this->actor.world.rot.x, this->actor.world.rot.y, this->actor.world.rot.z, MTXMODE_APPLY);
    vec1.x = vec1.y = vec1.z = 0.0f;
    Matrix_MultVec3f(&vec1, &this->talkPos);
    Matrix_Pop();
    Matrix_Translate(0.0f, 0.0f, 3800.0f, MTXMODE_APPLY);
    Matrix_RotateZYX(this->actor.world.rot.x, this->actor.world.rot.y, this->actor.world.rot.z, MTXMODE_APPLY);
    vec1.x = vec1.y = vec1.z = 0.0f;
    Matrix_MultVec3f(&vec1, &this->actor.focus.pos);
    this->actor.focus.pos.y += 100.0f;
}

void EnGm_Draw(Actor* thisx, PlayState* play) {
    static void* eyeTextures[] = { gGoronCsEyeOpenTex, gGoronCsEyeHalfTex, gGoronCsEyeClosedTex };
    EnGm* this = (EnGm*)thisx;
    s32 pad;

    OPEN_DISPS(play->state.gfxCtx, "../z_en_gm.c", 613);

    Gfx_SetupDL_25Opa(play->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(eyeTextures[this->eyeTexIndex]));
    gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(gGoronCsMouthNeutralTex));
    SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount, NULL,
                          NULL, &this->actor);

    CLOSE_DISPS(play->state.gfxCtx, "../z_en_gm.c", 629);

    func_80A3E090(this);
}
