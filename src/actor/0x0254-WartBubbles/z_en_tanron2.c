/*
 * File: z_en_tanron2.c
 * Overlay: ovl_En_Tanron2
 * Description: Wart's Bubbles
 */

#include "../0x0253-Wart/z_boss_04.h"
#include "../0x0254-WartBubbles/z_en_tanron2.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h"
//#include "src/actor/0x0253-Wart/z_boss_04.c"
#include <uLib.h>

#define FLAGS (ACTOR_FLAG_0 | ACTOR_FLAG_2 | ACTOR_FLAG_4 | ACTOR_FLAG_5)

#define THIS ((EnTanron2*)thisx)

void EnTanron2_Init(Actor* thisx, PlayState* play);
void EnTanron2_Destroy(Actor* thisx, PlayState* play);
void EnTanron2_Update(Actor* thisx, PlayState* play);
void EnTanron2_Draw(Actor* thisx, PlayState* play2);

void WartBubble_SetupStickAroundWart(EnTanron2* this);
void WartBubble_StickAroundWart(EnTanron2* this, PlayState* play);
void WartBubble_DetachedAndJump(EnTanron2* this, PlayState* play);
void func_80BB6F64(EnTanron2* this);
void func_80BB6F78(EnTanron2* this, PlayState* play);
void func_80BB7408(EnTanron2* this, PlayState* play);
void func_80BB7B90(Actor* thisx, PlayState* play);

Boss04* D_80BB8450;
f32 D_80BB8454;
EnTanron2* D_80BB8458[82];

ActorInit En_Tanron2_InitVars = {
    /**/ 0x0254,
    /**/ ACTORCAT_BOSS,
    /**/ FLAGS,
    /**/ 0x0227,
    /**/ sizeof(EnTanron2),
    /**/ EnTanron2_Init,
    /**/ EnTanron2_Destroy,
    /**/ EnTanron2_Update,
    /**/ EnTanron2_Draw,
};

static DamageTable sDamageTable_Bubbles = {
    /* Deku Nut       */ DMG_ENTRY(0, 0x0),
    /* Deku Stick     */ DMG_ENTRY(1, 0xF),
    /* Slingshot      */ DMG_ENTRY(1, 0xF),
    /* Explosives     */ DMG_ENTRY(2, 0xF),
    /* Boomerang      */ DMG_ENTRY(2, 0xF),
    /* Normal arrow   */ DMG_ENTRY(2, 0xF),
    /* Hammer swing   */ DMG_ENTRY(2, 0xF),
    /* Hookshot       */ DMG_ENTRY(0, 0xF),
    /* Kokiri Sword   */ DMG_ENTRY(1, 0xF),
    /* Master Sword   */ DMG_ENTRY(2, 0xF),
    /* Giant's Knife  */ DMG_ENTRY(4, 0xF),
    /* Fire arrow     */ DMG_ENTRY(2, 0x2),
    /* Ice arrow      */ DMG_ENTRY(2, 0x3),
    /* Light arrow    */ DMG_ENTRY(2, 0x4),
    /* Unk arrow 1    */ DMG_ENTRY(2, 0xF),
    /* Unk arrow 2    */ DMG_ENTRY(2, 0xF),
    /* Unk arrow 3    */ DMG_ENTRY(2, 0xF),
    /* Fire magic     */ DMG_ENTRY(2, 0x2),
    /* Ice magic      */ DMG_ENTRY(2, 0x3),
    /* Light magic    */ DMG_ENTRY(2, 0x4),
    /* Normal shield  */ DMG_ENTRY(0, 0xF),
    /* Light ray      */ DMG_ENTRY(0, 0xF),
    /* Kokiri spin    */ DMG_ENTRY(2, 0xF),
    /* Giant spin     */ DMG_ENTRY(8, 0xF),
    /* Master spin    */ DMG_ENTRY(4, 0xF),
    /* Kokiri jump    */ DMG_ENTRY(2, 0xF),
    /* Giant jump     */ DMG_ENTRY(8, 0xF),
    /* Master jump    */ DMG_ENTRY(4, 0xF),
    /* Unknown 1      */ DMG_ENTRY(0, 0x0),
    /* Unblockable    */ DMG_ENTRY(0, 0x0),
    /* Hammer jump    */ DMG_ENTRY(4, 0xF),
    /* Powder Keg     */ DMG_ENTRY(4, 0xF),
};

static ColliderCylinderInit sCylinderInit1 = {
    {
        COLTYPE_HIT3,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK3,
        { 0xFFCFFFFF, 0x00, 0x04 },
        { 0xFFFFFFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_ON | BUMP_HOOKABLE,
        OCELEM_ON,
    },
    { 30, 50, -25, { 0, 0, 0 } },
};

static ColliderCylinderInit sCylinderInit2 = {
    {
        COLTYPE_HIT3,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK3,
        { 0xFFCFFFFF, 0x00, 0x04 },
        { 0xFFFFFFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_ON | BUMP_HOOKABLE,
        OCELEM_ON,
    },
    { 22, 42, -21, { 0, 0, 0 } },
};

Color_RGBA8 D_80BB81E8 = { 255, 255, 255, 255 };
Color_RGBA8 D_80BB81EC = { 255, 100, 100, 255 };

void EnTanron2_Init(Actor* thisx, PlayState* play) {
//return;
    EnTanron2* this = THIS;

    D_80BB8450 = (Boss04*)this->actor.parent;
    Assert(D_80BB8450 != NULL);
    this->actor.flags &= ~ACTOR_FLAG_0;
    this->actor.naviEnemyId = 0x25;

    if (this->actor.params == 100) {
        this->actor.update = func_80BB7B90;
        Actor_ChangeCategory(play, &play->actorCtx, &this->actor, 5);
        return;
    }

    this->actor.flags |= ACTOR_FLAG_9;
    Actor_SetScale(&this->actor, 1.0f);

    this->actor.draw = NULL;
    this->actor.colChkInfo.health = 1;
    this->actor.colChkInfo.damageTable = &sDamageTable_Bubbles;
    this->actor.targetMode = 5;

    Collider_InitCylinder(play, &this->collider1);
    Collider_InitCylinder(play, &this->collider2);
    Collider_SetCylinder(play, &this->collider1, &this->actor, &sCylinderInit1);
    Collider_SetCylinder(play, &this->collider2, &this->actor, &sCylinderInit2);

    if ((KREG(64) != 0) || CHECK_EVENTINF(EVENTINF_60)) {
        WartBubble_SetupStickAroundWart(this);
    } else {
        func_80BB6F64(this);
    }

    this->unk_14A = Rand_ZeroFloat(0x10000);
    this->unk_14C = Rand_ZeroFloat(800.0f) + 1400.0f;
    if (Rand_ZeroOne() < 0.5f) {
        this->unk_14C = -this->unk_14C;
    }

    Actor_UpdateBgCheckInfo(play, &this->actor, 35.0f, 60.0f, 60.0f, UPDBGCHECKINFO_FLAG_4);
    this->actor.floorHeight += 20.0f;
    this->unk_148 = Rand_ZeroFloat(32.0f);
}

void EnTanron2_Destroy(Actor* thisx, PlayState* play) {
}

void WartBubble_SetupStickAroundWart(EnTanron2* this) {
    this->actionFunc = WartBubble_StickAroundWart;
    this->unk_160 = 0.0f;
    this->unk_15C = 0.0f;
    this->collider1.dim.radius = 25;
    this->collider1.dim.height = 46;
    this->collider1.dim.yShift = -23;
    this->unk_158 = 0;
}

void WartBubble_StickAroundWart(EnTanron2* this, PlayState* play) {
    f32 sp34;
    f32 sp30;
    f32 sp2C;
    Vec3f* temp_s1 = &D_80BB8450->unk_2E4[this->actor.params];

    if (this->unk_15C == 1.0f) {
        Math_Vec3f_Copy(&this->actor.world.pos, temp_s1);
    } else {
        sp34 = Math_SmoothStepToF(&this->actor.world.pos.x, temp_s1->x, this->unk_15C, this->unk_160, 0.0f);
        sp30 = Math_SmoothStepToF(&this->actor.world.pos.y, temp_s1->y, this->unk_15C, this->unk_160, 0.0f);
        sp2C = Math_SmoothStepToF(&this->actor.world.pos.z, temp_s1->z, this->unk_15C, this->unk_160, 0.0f);

        if ((this->unk_158 == 0) && ((sp34 + sp30 + sp2C) < 2.0f)) {
            this->unk_158 = 1;
            //Actor_PlaySfx(&this->actor, NA_SE_EN_B_SLIME_EAT);
            SoundEffect_PlayOneshot(SOUND_ENEMY_MAD_JELLY_GRAB, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 1000.0f, &this->actor);
        }

        Math_ApproachF(&this->unk_15C, 1.0f, 1.0f, 0.02f);
        Math_ApproachF(&this->unk_160, 20.0f, 1.0f, 1.0f);
    }

    Math_ApproachF(&this->actor.scale.x, 1.0f, 0.1f, 0.2f);
    Math_ApproachF(&this->actor.scale.y, 1.0f, 0.1f, 0.2f);
}

void WartBubble_SetupDetachedAndJump(EnTanron2* this) {
    this->actionFunc = WartBubble_DetachedAndJump;
    this->actor.speedXZ = 0.0f;
    this->actor.velocity.z = 0.0f;
    this->actor.velocity.y = 0.0f;
    this->actor.velocity.x = 0.0f;
    this->unk_158 = 0;
    this->unk_159 = 1;
    this->actor.flags |= ACTOR_FLAG_0;
    this->collider1.dim.radius = 30;
    this->collider1.dim.height = 50;
    this->collider1.dim.yShift = -25;
}

void WartBubble_DetachedAndJump(EnTanron2* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 sp32;
    f32 sp2C;
    f32 sp28;

    if (this->unk_14E == 0) {
        this->actor.world.pos.x += this->actor.velocity.x;
        this->actor.world.pos.y += this->actor.velocity.y;
        this->actor.world.pos.z += this->actor.velocity.z;
        this->actor.velocity.y = this->actor.velocity.y - 2.0f;

        if (this->actor.world.pos.y <= this->actor.floorHeight) {
            this->actor.world.pos.y = this->actor.floorHeight;

            //Actor_PlaySfx(&this->actor, NA_SE_EN_IKURA_JUMP2);
            switch (Rand_S16Offset(0,2))
            {
                case 0:
                    //Audio_PlayActorSfx2(&this->actor, NA_SE_EN_DODO_M_GND);
                break;
                case 1:
                default:
                    //SoundEffect_PlayOneshot(SOUND_ENEMY_WART_BUBBLE_JUMP2, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 1000.0f, &this->actor);
                break;
            }

            sp2C = D_80BB8450->unk_6BC.x - this->actor.world.pos.x;
            sp28 = D_80BB8450->unk_6BC.z - this->actor.world.pos.z;

            switch (this->unk_158) {
                case 0:
                    if (Rand_ZeroOne() > 0.2f) {
                        sp32 = Rand_ZeroFloat(0x10000);
                    } else {
                        //sp32 = Math_Atan2S(sp2C, sp28);
                        sp32 = Math_Atan2S(sp28, sp2C);
                    }
                    this->actor.speedXZ = Rand_ZeroFloat(5.0f) + 5.0f;
                    break;

                case 1:
                    sp32 = Math_Atan2S(sp28, sp2C);
                    this->actor.speedXZ += 2.0f;
                    if (this->actor.speedXZ > 10.0f) {
                        this->actor.speedXZ = 10.0f;
                    }
                    break;

                case 2:
                    sp32 = Math_Atan2S(player->actor.world.pos.z - this->actor.world.pos.z,
                                       player->actor.world.pos.x - this->actor.world.pos.x) +
                           (s16)Rand_ZeroFloat(20000.0f);
                    this->actor.speedXZ = Rand_ZeroFloat(7.0f) + 7.0f;
                    if ((this->unk_152 == 0) && (D_80BB8450->unk_1F6 == 0)) {
                        this->unk_158 = 1;
                    }
                    break;
            }
            Matrix_RotateYS(sp32, MTXMODE_NEW);
            Matrix_MultVecZ(this->actor.speedXZ, &this->actor.velocity);
            this->actor.velocity.y = Rand_ZeroFloat(5.0f) + 12.0f;
            this->unk_14E = 5;
        }

        if (this->unk_150 != 0) {
            Math_ApproachF(&this->actor.scale.x, 0.75f, 0.5f, 1.0f);
            Math_ApproachF(&this->actor.scale.y, 1.2f, 0.5f, 1.0f);
        } else {
            Math_ApproachF(&this->actor.scale.x, 1.0f, 0.1f, 0.2f);
            Math_ApproachF(&this->actor.scale.y, 1.0f, 0.1f, 0.2f);
        }
    } else {
        Math_ApproachF(&this->actor.scale.x, 1.2f, 0.75f, 1.5f);
        Math_ApproachF(&this->actor.scale.y, 0.75f, 0.75f, 1.5f);
        if (this->unk_14E == 1) {
            sp2C = D_80BB8450->unk_6BC.x - this->actor.world.pos.x;
            sp28 = D_80BB8450->unk_6BC.z - this->actor.world.pos.z;

            if ((this->unk_158 != 0) && ((SQ(sp2C) + SQ(sp28)) < 14400.0f)) {
                WartBubble_SetupStickAroundWart(this);
            } else {
                this->unk_150 = 10;
                //Actor_PlaySfx(&this->actor, NA_SE_EN_IKURA_JUMP1);
                //SoundEffect_PlayOneshot(SOUND_ENEMY_WART_BUBBLE_JUMP1, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 1000.0f, &this->actor);
                switch (Rand_S16Offset(0,2))
                {
                    case 0:
                        //Audio_PlayActorSfx2(&this->actor, NA_SE_EN_STAL_JUMP);
                    break;
                    case 1:
                    default:
                        //SoundEffect_PlayOneshot(SOUND_ENEMY_WART_BUBBLE_JUMP1, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 1000.0f, &this->actor);
                    break;
            }
            }
        }
    }
}

void func_80BB6F64(EnTanron2* this) {
    this->actionFunc = func_80BB6F78;
}

void func_80BB6F78(EnTanron2* this, PlayState* play) {
//return;
    switch (this->unk_158) {
        case 0:
            if (D_80BB8450->unk_708 == 13) {
                this->unk_158 = 10;
                this->unk_14E = Rand_ZeroFloat(50.0f);
            }
            break;

        case 10:
            if (this->unk_14E == 0) {
                this->unk_150 = 10;
                this->unk_158 = 1;
            }
            break;

        case 1:
            if (this->unk_14E == 0) {
                this->actor.world.pos.y += this->actor.velocity.y;
                this->actor.velocity.y = this->actor.velocity.y - 2.0f;

                if (this->actor.world.pos.y <= this->actor.floorHeight) {
                    this->actor.world.pos.y = this->actor.floorHeight;
                    //Actor_PlaySfx(&this->actor, NA_SE_EN_IKURA_JUMP2);
                    //SoundEffect_PlayOneshot(SOUND_ENEMY_WART_BUBBLE_JUMP2, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 1000.0f, &this->actor);
                    switch (Rand_S16Offset(0,2))
                    {
                        case 0:
                            //Audio_PlayActorSfx2(&this->actor, NA_SE_EN_DODO_M_GND);
                        break;
                        case 1:
                            //SoundEffect_PlayOneshot(SOUND_ENEMY_WART_BUBBLE_JUMP2, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 1000.0f, &this->actor);
                        break;
                        case 2:
                            //SoundEffect_PlayOneshot(SOUND_ENEMY_WART_BUBBLE_JUMP2, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 1000.0f, &this->actor);
                        break;
                        case 3:
                        default:
                            SoundEffect_PlayOneshot(SOUND_ENEMY_WART_BUBBLE_JUMP2, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 1000.0f, &this->actor);
                        break;
                    }
                    if (D_80BB8450->unk_6F8 > 0.1f) {
                        Audio_PlayActorSfx2(&this->actor, NA_SE_EV_OUT_OF_WATER_L);
                    }
                    this->actor.velocity.y = Rand_ZeroFloat(5.0f) + 12.0f;
                    this->unk_14E = 5;
                }

                if (this->unk_150 != 0) {
                    Math_ApproachF(&this->actor.scale.x, 0.75f, 0.5f, 1.0f);
                    Math_ApproachF(&this->actor.scale.y, 1.2f, 0.5f, 1.0f);
                } else {
                    Math_ApproachF(&this->actor.scale.x, 1.0f, 0.1f, 0.2f);
                    Math_ApproachF(&this->actor.scale.y, 1.0f, 0.1f, 0.2f);
                }
            } else {
                Math_ApproachF(&this->actor.scale.x, 1.2f, 0.75f, 1.5f);
                Math_ApproachF(&this->actor.scale.y, 0.75f, 0.75f, 1.5f);

                if (this->unk_14E == 1) {
                    //return;
                    if (D_80BB8450->unk_708 == 3) {
                        WartBubble_SetupDetachedAndJump(this);
                        this->unk_158 = 1;
                        this->unk_159 = 0;
                    } else {
                        this->unk_150 = 0xA;
                        //Actor_PlaySfx(&this->actor, NA_SE_EN_IKURA_JUMP1);
                        //SoundEffect_PlayOneshot(SOUND_ENEMY_WART_BUBBLE_JUMP1, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 1000.0f, &this->actor);
                        switch (Rand_S16Offset(0,4))
                        {
                            case 0:
                                //Audio_PlayActorSfx2(&this->actor, NA_SE_EN_STAL_JUMP);
                            break;
                            case 1:
                                //SoundEffect_PlayOneshot(SOUND_ENEMY_WART_BUBBLE_JUMP1, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 1000.0f, &this->actor);
                            break;
                            case 2:
                                //SoundEffect_PlayOneshot(SOUND_ENEMY_WART_BUBBLE_JUMP1, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 1000.0f, &this->actor);
                            break;
                            case 3:
                            default:
                                SoundEffect_PlayOneshot(SOUND_ENEMY_WART_BUBBLE_JUMP1, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 1000.0f, &this->actor);
                            break;
                        }
                    }
                }
            }
            break;
    }
}

void func_80BB71C8(EnTanron2* this, PlayState* play) {
    s32 i;
    Vec3f spA8;
    Vec3f sp9C;
    Vec3f sp90;
    SfxSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 60, NA_SE_EN_AWA_BREAK);
    for (i = 0; i < 15; i++) {
        Matrix_RotateYF(Rand_ZeroFloat(6.2831855f), MTXMODE_NEW);
        Matrix_RotateXFApply(Rand_ZeroFloat(6.2831855f));
        Matrix_MultVecZ(Rand_ZeroFloat(10.0f) + 5.0f, &spA8);
        sp90.x = this->actor.world.pos.x + spA8.x;
        sp90.y = this->actor.world.pos.y + spA8.y;
        sp90.z = this->actor.world.pos.z + spA8.z;

        sp9C.x = spA8.x * -0.03f;
        sp9C.y = spA8.y * -0.03f;
        sp9C.z = spA8.z * -0.03f;

        EffectSsDtBubble_SpawnCustomColor(play, &sp90, &spA8, &sp9C, &D_80BB81E8, &D_80BB81EC,
                                          Rand_ZeroFloat(100.0f) + 200.0f, Rand_ZeroFloat(5.0f) + 15.0f, false);
    }
}

void func_80BB7398(EnTanron2* this, PlayState* play) {
    this->actionFunc = func_80BB7408;
    if ((s8)this->actor.colChkInfo.health <= 0) {
        Enemy_StartFinishingBlow(play, &this->actor);
        //SoundSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 30, NA_SE_EN_IKURA_DEAD);
        SoundEffect_PlayOneshot(SOUND_ENEMY_WART_BUBBLE_DEAD, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 1000.0f, &this->actor);
    } else {
        //Actor_PlaySfx(&this->actor, NA_SE_EN_IKURA_DAMAGE);
        SoundEffect_PlayOneshot(SOUND_ENEMY_WART_BUBBLE_DAMAGE, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 1000.0f, &this->actor);
    }
}

void func_80BB7408(EnTanron2* this, PlayState* play) {
    this->actor.world.pos.x += this->actor.velocity.x;
    this->actor.world.pos.y += this->actor.velocity.y;
    this->actor.world.pos.z += this->actor.velocity.z;
    this->actor.velocity.y -= 2.0f;

    if (this->actor.world.pos.y <= this->actor.floorHeight) {
        this->actor.world.pos.y = this->actor.floorHeight;
        if ((s8)this->actor.colChkInfo.health <= 0) {
            if (Rand_ZeroOne() < 0.4f) {
                Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos, 0xA0);
            }
            if (Rand_ZeroOne() > 0.75f) {
                Item_DropCollectible(play, &this->actor.world.pos, ITEM00_RECOVERY_HEART);
            }
            if (Rand_ZeroOne() > 0.9f) {
                Item_DropCollectible(play, &this->actor.world.pos, ITEM00_RECOVERY_HEART);
            }
            Actor_Kill(&this->actor);
            func_80BB71C8(this, play);
            D_80BB8450->unk_6F6--;
            D_80BB8450->unk_2E2 += 4;
            D_80BB8450->unk_2E0 += 4;
        } else {
            this->actor.velocity.x *= 0.5f;
            this->actor.velocity.z *= 0.5f;
            Math_ApproachF(&this->actor.scale.x, 1.2f, 0.75f, 1.5f);
            Math_ApproachF(&this->actor.scale.y, 0.75f, 0.75f, 1.5f);
            if ((this->actor.velocity.x < 0.1f) && (this->actor.velocity.z < 0.1f)) {
                WartBubble_SetupDetachedAndJump(this);
                this->unk_158 = 1;
            }
        }
    }
}

void WartBubble_DamageCheck(EnTanron2* this, PlayState* play) {
    ColliderInfo* acHitInfo;
    s32 pad;
    Player* player = GET_PLAYER(play);
    s32 pad2;
    s32 pad3;
    u8 damage;

    if (this->unk_154 == 0) {
        if (this->collider1.base.acFlags & AC_HIT) {
            this->collider1.base.acFlags &= ~AC_HIT;
            acHitInfo = this->collider1.info.acHitInfo;
            if(this->actor.colChkInfo.damageEffect == 0x0){
                return;
            }
            if (acHitInfo->toucher.dmgFlags & 0x80) {
                WartBubble_SetupDetachedAndJump(this);
                this->unk_158 = 1;
                //Actor_PlaySfx(&this->actor, NA_SE_EN_IKURA_DAMAGE);
                //SoundEffect_PlayOneshot(SOUND_ENEMY_WART_BUBBLE_DAMAGE, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 1000.0f, &this->actor);
                if ((player->targetActor != NULL) && (&this->actor != player->targetActor)) {
                    player->targetActor = &this->actor;
                    play->actorCtx.targetCtx.arrowPointedActor = &this->actor;
                    play->actorCtx.targetCtx.targetedActor = &this->actor;
                }
            } else {
                this->unk_154 = 15;
                if (this->actionFunc != WartBubble_StickAroundWart) {
                    Matrix_RotateYS(this->actor.yawTowardsPlayer, MTXMODE_NEW);
                    if ((acHitInfo->toucher.dmgFlags & 0x300000) != 0) {
                        this->unk_154 = 10;
                        Matrix_MultVecZ(-10.0f, &this->actor.velocity);
                    } else {
                        this->unk_156 = 15;
                        Matrix_MultVecZ(-20.0f, &this->actor.velocity);
                        damage = this->actor.colChkInfo.damage;
                        this->actor.colChkInfo.health -= damage;
                        func_80BB7398(this, play);
                    }
                } else {
                    //Actor_PlaySfx(&this->actor, NA_SE_EN_IKURA_DAMAGE);
                    //SoundEffect_PlayOneshot(SOUND_ENEMY_WART_BUBBLE_DAMAGE, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 1000.0f, &this->actor);
                    goto block_18;
                }
            }
        }
    }

    if ((this->actionFunc == WartBubble_StickAroundWart) &&
        ((D_80BB8450->unk_1F6 == 1) || ((D_80BB8450->unk_1F7 != 0) && (Rand_ZeroOne() < 0.1f)))) {
        if (D_80BB8450->unk_1F7 != 0) {
            D_80BB8450->unk_1F7--;
        }

    block_18:
        WartBubble_SetupDetachedAndJump(this);
        this->unk_158 = 2;
        Matrix_RotateYS(Math_Atan2S(this->actor.world.pos.z - D_80BB8450->unk_6BC.z,
                                    this->actor.world.pos.x - D_80BB8450->unk_6BC.x),
                        MTXMODE_NEW);
        Matrix_MultVecZ(10.0f, &this->actor.velocity);
        this->unk_152 = Rand_ZeroFloat(100.0f) + 200.0f;
    } else if (D_80BB8450->unk_1F6 == 10) {
        if (Rand_ZeroOne() < 0.4f) {
            Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos, 0xA0);
        }
        Actor_Kill(&this->actor);
        func_80BB71C8(this, play);
    }
}

void EnTanron2_Update(Actor* thisx, PlayState* play) {
//return;
    s32 pad;
    EnTanron2* this = THIS;
    s32 pad2[2];
    Input* input;

    this->unk_148++;
    Actor_SetFocus(&this->actor, 0.0f);

    if (this->unk_14E != 0) {
        this->unk_14E--;
    }

    if (this->unk_150 != 0) {
        this->unk_150--;
    }

    if (this->unk_152 != 0) {
        this->unk_152--;
    }

    if (this->unk_154 != 0) {
        this->unk_154--;
    }

    if (this->unk_156 != 0) {
        this->unk_156--;
    }

    this->actionFunc(this, play);


    if (this->actionFunc != WartBubble_StickAroundWart) {
        if ((this->actor.world.pos.x < (D_80BB8450->unk_6D8 + 20.0f)) ||
            ((D_80BB8450->unk_6DC - 20.0f) < this->actor.world.pos.x)) {
            this->actor.world.pos.x = this->actor.prevPos.x;
            this->actor.velocity.x *= -1.0f;
        }

        if ((this->actor.world.pos.z < (D_80BB8450->unk_6E0 + 20.0f)) ||
            ((D_80BB8450->unk_6E4 - 20.0f) < this->actor.world.pos.z)) {
            this->actor.world.pos.z = this->actor.prevPos.z;
            this->actor.velocity.z *= -1.0f;
        }
    }

    this->unk_14A += this->unk_14C;
    WartBubble_DamageCheck(this, play);

    if (this->unk_159 != 0) {
        if (this->actor.xzDistToPlayer < 100.0f) {
            if (this->unk_15A == 0) {
                Collider_UpdateCylinder(&this->actor, &this->collider2);
                CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider2.base);
            } else {
                this->unk_15A--;
            }
        }

        Collider_UpdateCylinder(&this->actor, &this->collider1);
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider1.base);
    }

    if (!(this->unk_148 & 0x1F)) {
        if ((this->actionFunc != WartBubble_StickAroundWart) && (this->actor.xyzDistToPlayerSq < SQ(200.0f))) {
            this->unk_15B = 1;
        } else {
            this->unk_15B = 0;
        }

        if (this->actionFunc == WartBubble_StickAroundWart) {
            s16 atan = Math_Atan2S(this->actor.world.pos.x - D_80BB8450->unk_6BC.x,
                                   this->actor.world.pos.z - D_80BB8450->unk_6BC.z);

            if (ABS_ALT(BINANG_SUB(D_80BB8450->actor.yawTowardsPlayer, atan)) > 0x3000) {
                this->unk_159 = 0;
                this->actor.flags &= ~ACTOR_FLAG_0;
            } else {
                this->unk_159 = 1;
                this->actor.flags |= ACTOR_FLAG_0;
            }
        }
    }

    if (CHECK_FLAG_ALL(this->actor.flags, ACTOR_FLAG_13) && (this->actor.xzDistToPlayer < 80.0f)) {
        this->actor.flags &= ~ACTOR_FLAG_13;
        this->unk_15A = 25;
        this->unk_159 = 1;
    }


    input = CONTROLLER3(&play->state);
    if (CHECK_BTN_ALL(input->press.button, BTN_L)) {
        this->unk_158 = 1;
    }

    if (CHECK_BTN_ALL(input->press.button, BTN_R)) {
        WartBubble_SetupDetachedAndJump(this);
    }

}

void func_80BB7B90(Actor* thisx, PlayState* play) {
    EnTanron2* this = THIS;

    D_80BB8454 = (Math_SinS(play->gameplayFrames * 0x3000) * 0.1f) + 1.0f;
    if (D_80BB8450->unk_1F6 == 11) {
        if (Rand_ZeroOne() < 0.4f) {
            Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos, 0xA0);
        }
        Actor_Kill(&this->actor);
    }
}

void EnTanron2_Draw(Actor* thisx, PlayState* play2) {

    PlayState* play = play2;
    s32 i;
    s32 j;
    s32 found;
    Actor* tanron2;

    OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

    Gfx_SetupDL_25Xlu(play->state.gfxCtx);

    gSPDisplayList(POLY_XLU_DISP++, 0x06003450);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, 150);

    tanron2 = play->actorCtx.actorLists[ACTORCAT_BOSS].head;

    for (i = 0; i < ARRAY_COUNT(D_80BB8458); i++) {
        D_80BB8458[i] = NULL;
    }

    found = 0;
    while (tanron2 != NULL) {
        if (tanron2->params < 100) {
            D_80BB8458[found] = (EnTanron2*)tanron2;
            found++;
        }
        tanron2 = tanron2->next;
    }
    
    for (j = 0; j < found - 1; j++) {
        for (i = 0; i < found - 1; i++) {
            if (D_80BB8458[i + 1] != NULL) {
                if (D_80BB8458[i]->actor.projectedPos.z < D_80BB8458[i + 1]->actor.projectedPos.z) {
                    SWAP(EnTanron2*, D_80BB8458[i], D_80BB8458[i + 1]);
                }
            }
        }
    }
    
    for (i = 0; i < ARRAY_COUNT(D_80BB8458); i++) {
        if (D_80BB8458[i] != NULL) {
            Matrix_Translate(D_80BB8458[i]->actor.world.pos.x, D_80BB8458[i]->actor.world.pos.y,
                             D_80BB8458[i]->actor.world.pos.z, MTXMODE_NEW);
            Matrix_ReplaceRotation(&play->billboardMtxF);
            Matrix_Scale(D_80BB8458[i]->actor.scale.x, D_80BB8458[i]->actor.scale.y, 0.0f, MTXMODE_APPLY);
            Matrix_RotateZS(D_80BB8458[i]->unk_14A, MTXMODE_APPLY);
            Matrix_Scale(0.13f, 0.14299999f, 0.13f, MTXMODE_APPLY);
            Matrix_RotateZS(-D_80BB8458[i]->unk_14A, MTXMODE_APPLY);

            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, 0x060034c8);
        }
    }

    Gfx_SetupDL_44Xlu(play->state.gfxCtx);

    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 0, 0, 0, 150);
    gSPDisplayList(POLY_XLU_DISP++, 0x06004510);

    tanron2 = play->actorCtx.actorLists[ACTORCAT_BOSS].head;
    while (tanron2 != NULL) {
        if ((tanron2->params < 100) && (((EnTanron2*)tanron2)->unk_15B != 0)) {
            Matrix_Translate(tanron2->world.pos.x, D_80BB8450->actor.floorHeight, tanron2->world.pos.z, MTXMODE_NEW);
            Matrix_Scale(0.6f, 0.0f, 0.6f, MTXMODE_APPLY);

            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, 0x06004550);
        }
        tanron2 = tanron2->next;
    }

    Gfx_SetupDL_60NoCDXlu(play->state.gfxCtx);

    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, 255);
    gDPSetEnvColor(POLY_XLU_DISP++, 255, 255, 255, 255);
    gDPSetAlphaDither(POLY_XLU_DISP++, G_AD_NOISE);
    gDPSetColorDither(POLY_XLU_DISP++, G_CD_NOISE);

    tanron2 = play->actorCtx.actorLists[ACTORCAT_BOSS].head;
    while (tanron2 != NULL) {
        /*if ((tanron2->params < 100) && (((EnTanron2*)tanron2)->unk_15B != 0) &&
            (tanron2->world.pos.y <= tanron2->floorHeight)) {
            Matrix_Translate(tanron2->world.pos.x, D_80BB8450->actor.floorHeight + 2.0f, tanron2->world.pos.z,
                             MTXMODE_NEW);
            Matrix_Scale(D_80BB8454, 0.0f, D_80BB8454, MTXMODE_APPLY);

            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            SPDisplayList(POLY_XLU_DISP++, gEffWaterRippleDL);
        }*/
        /*if ((tanron2->params < 100) && (((EnTanron2*)tanron2)->unk_15B != 0) &&
            (tanron2->world.pos.y <= tanron2->floorHeight)) {
        }*/
        /*if ((tanron2->params < 100) && (((EnTanron2*)tanron2)->unk_15B != 0) &&
            (tanron2->world.pos.y <= tanron2->floorHeight)) {
            Matrix_Translate(tanron2->world.pos.x, D_80BB8450->actor.floorHeight + 2.0f, tanron2->world.pos.z,
                             MTXMODE_NEW);
        }*/
        float test = (Math_SinS(play->gameplayFrames * 0x3000) * 0.1f) + 1.0f;
        if ((tanron2->params < 100) && (((EnTanron2*)tanron2)->unk_15B != 0) &&
            (tanron2->world.pos.y <= tanron2->floorHeight)) {
            Matrix_Translate(tanron2->world.pos.x, D_80BB8450->actor.floorHeight + 2.0f, tanron2->world.pos.z,
                             MTXMODE_NEW);
            Matrix_Scale(test, 0.0f, test, MTXMODE_APPLY);

            
            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, gEffWaterRippleDL);
        }
        tanron2 = tanron2->next;
    }

    CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

}
