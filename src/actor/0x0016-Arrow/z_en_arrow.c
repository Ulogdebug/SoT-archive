/*
 * File: z_en_arrow.c
 * Overlay: ovl_En_Arrow
 * Description: Arrow, Deku Seed, and Deku Nut Projectile
 */

#include "z_en_arrow.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h"
#include <uLib.h>

#define AT_TYPES (AT_TYPE_PLAYER | AT_TYPE_OTHER)
#define SPAWN_ON_GROUND (0x0001)
#define SPAWN_ON_WATER (0x0000)
#define FLAGS (ACTOR_FLAG_4 | ACTOR_FLAG_5)

void EnArrow_Init(Actor* thisx, PlayState* play);
void EnArrow_Destroy(Actor* thisx, PlayState* play);
void EnArrow_Update(Actor* thisx, PlayState* play);
void EnArrow_Draw(Actor* thisx, PlayState* play);

void EnArrow_Shoot(EnArrow* this, PlayState* play);
void EnArrow_Fly(EnArrow* this, PlayState* play);
void func_809B45E0(EnArrow* this, PlayState* play);
void func_809B4640(EnArrow* this, PlayState* play);

const ActorInit En_Arrow_InitVars = {
    ACTOR_EN_ARROW,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnArrow),
    (ActorFunc)EnArrow_Init,
    (ActorFunc)EnArrow_Destroy,
    (ActorFunc)EnArrow_Update,
    (ActorFunc)EnArrow_Draw,
};

static ColliderQuadInit sColliderInit = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPES,
        AC_NONE,
        OC1_NONE,
        OC2_TYPE_PLAYER,
        COLSHAPE_QUAD,
    },
    {
        ELEMTYPE_UNK2,
        { 0x00000020, 0x00, 0x01 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_NEAREST | TOUCH_SFX_NONE,
        BUMP_NONE,
        OCELEM_NONE,
    },
    { { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } } },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(minVelocityY, -150, ICHAIN_STOP),
};

void EnArrow_SetupAction(EnArrow* this, EnArrowActionFunc actionFunc) {
    this->actionFunc = actionFunc;
}

void EnArrow_Init(Actor* thisx, PlayState* play) {
    static EffectBlureInit2 blureNormal = {
        0, 4, 0, { 0, 255, 200, 255 },   { 0, 255, 255, 255 }, { 0, 255, 200, 0 }, { 0, 255, 255, 0 }, 16,
        0, 1, 0, { 255, 255, 170, 255 }, { 0, 150, 0, 0 },
    };
    static EffectBlureInit2 blureFire = {
        0, 4, 0, { 0, 255, 200, 255 }, { 0, 255, 255, 255 }, { 0, 255, 200, 0 }, { 0, 255, 255, 0 }, 16,
        0, 1, 0, { 255, 200, 0, 255 }, { 255, 0, 0, 0 },
    };
    static EffectBlureInit2 blureIce = {
        0, 4, 0, { 0, 255, 200, 255 },   { 0, 255, 255, 255 }, { 0, 255, 200, 0 }, { 0, 255, 255, 0 }, 16,
        0, 1, 0, { 170, 255, 255, 255 }, { 0, 100, 255, 0 },
    };
    static EffectBlureInit2 blureLight = {
        0, 4, 0, { 0, 255, 200, 255 },   { 0, 255, 255, 255 }, { 0, 255, 200, 0 }, { 0, 255, 255, 0 }, 16,
        0, 1, 0, { 255, 255, 170, 255 }, { 255, 255, 0, 0 },
    };
    static u32 dmgFlags[] = {
        //DMG_ARROW_FIRE,  DMG_ARROW_NORMAL, DMG_ARROW_NORMAL, DMG_ARROW_FIRE, DMG_ARROW_ICE,
        //DMG_ARROW_LIGHT, DMG_ARROW_UNK3,   DMG_ARROW_UNK1,   DMG_ARROW_UNK2, DMG_SLINGSHOT,
        DMG_ARROW_FIRE,
        DMG_ARROW_NORMAL,
        DMG_ARROW_NORMAL,
        DMG_ARROW_FIRE,
        DMG_ARROW_ICE,
        DMG_ARROW_LIGHT,
        DMG_ARROW_UNK3,
        DMG_ARROW_UNK1,
        DMG_ARROW_UNK2,
        DMG_SLINGSHOT,
        DMG_ARROW_ICE,
    };
    EnArrow* this = (EnArrow*)thisx;

    Actor_ProcessInitChain(&this->actor, sInitChain);

    if (this->actor.params == ARROW_CS_NUT) {
        this->isCsNut = true;
        this->actor.params = ARROW_NUT;
    }

    if (this->actor.params <= ARROW_SEED) {

        if (this->actor.params <= ARROW_0E) {
            SkelAnime_Init(play, &this->skelAnime, &gArrowSkel, &gArrow2Anim, NULL, NULL, 0);
        }

        if ((this->actor.params <= ARROW_NORMAL) || (this->actor.params == ARROW_BLUE_FIRE_LIT)) {
            if (this->actor.params == ARROW_NORMAL_HORSE) {
                blureNormal.elemDuration = 4;
            } else {
                blureNormal.elemDuration = 16;
            }

            Effect_Add(play, &this->effectIndex, EFFECT_BLURE2, 0, 0, &blureNormal);

        } else if (this->actor.params == ARROW_FIRE) {

            Effect_Add(play, &this->effectIndex, EFFECT_BLURE2, 0, 0, &blureFire);

        } else if (this->actor.params == ARROW_ICE) {

            Effect_Add(play, &this->effectIndex, EFFECT_BLURE2, 0, 0, &blureIce);

        } else if (this->actor.params == ARROW_LIGHT) {

            Effect_Add(play, &this->effectIndex, EFFECT_BLURE2, 0, 0, &blureLight);
        }

        Collider_InitQuad(play, &this->collider);
        Collider_SetQuad(play, &this->collider, &this->actor, &sColliderInit);

        if ((this->actor.params <= ARROW_NORMAL) || (this->actor.params == ARROW_BLUE_FIRE_LIT)) {
            this->collider.info.toucherFlags &= ~TOUCH_SFX_MASK;
            this->collider.info.toucherFlags |= TOUCH_SFX_NORMAL;
        }

        if (this->actor.params < 0) {
            this->collider.base.atFlags = (AT_ON | AT_TYPE_ENEMY);
        } else if ((this->actor.params <= ARROW_SEED) || (this->actor.params == ARROW_BLUE_FIRE_LIT)) {
            this->collider.info.toucher.dmgFlags = dmgFlags[this->actor.params];
            LOG_HEX("this->at_info.cl_elem.at_btl_info.at_type", this->collider.info.toucher.dmgFlags,
                    "../z_en_arrow.c", 707);
        }
    }

    EnArrow_SetupAction(this, EnArrow_Shoot);
}

void EnArrow_Destroy(Actor* thisx, PlayState* play) {
    EnArrow* this = (EnArrow*)thisx;

    if ((this->actor.params <= ARROW_LIGHT) || (this->actor.params == ARROW_BLUE_FIRE_LIT)) {
        Effect_Delete(play, this->effectIndex);
    }

    SkelAnime_Free(&this->skelAnime, play);
    Collider_DestroyQuad(play, &this->collider);

    if ((this->hitActor != NULL) && (this->hitActor->update != NULL)) {
        this->hitActor->flags &= ~ACTOR_FLAG_15;
    }
}

void EnArrow_Shoot(EnArrow* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (this->actor.parent == NULL) {
        if ((this->actor.params != ARROW_NUT) && (player->unk_A73 == 0)) {
            Actor_Kill(&this->actor);
            return;
        }

        switch (this->actor.params) {
            case ARROW_SEED:
                func_8002F7DC(&player->actor, NA_SE_IT_SLING_SHOT);
                //SoundEffect_PlayOneshot(SOUND_DOOM_PISTOL, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 2000.0f, &this->actor);
                break;

            case ARROW_NORMAL_LIT:
            case ARROW_NORMAL_HORSE:
            case ARROW_NORMAL:
            case ARROW_BLUE_FIRE_LIT:
                func_8002F7DC(&player->actor, NA_SE_IT_ARROW_SHOT);
                //SoundEffect_PlayOneshot(SOUND_DOOM_SHOTGUN, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 2000.0f, &this->actor);
                //SoundEffect_PlayOneshot(SOUND_HL2_CROSSBOW_FIRE, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 2000.0f, &this->actor);
                break;

            case ARROW_FIRE:
            case ARROW_ICE:
            case ARROW_LIGHT:
                func_8002F7DC(&player->actor, NA_SE_IT_MAGIC_ARROW_SHOT);
                // SoundEffect_PlayOneshot(SOUND_DOOM_SHOTGUN, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 2000.0f, &this->actor);
                // SoundEffect_PlayOneshot(SOUND_DOOM_ROCKET_LAUNCH, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 2000.0f, &this->actor);
                break;
        }

        EnArrow_SetupAction(this, EnArrow_Fly);
        Math_Vec3f_Copy(&this->unk_210, &this->actor.world.pos);

        if (this->actor.params >= ARROW_SEED && this->actor.params != ARROW_BLUE_FIRE_LIT) {
            func_8002D9A4(&this->actor, 80.0f);
            this->timer = 15;
            this->actor.shape.rot.x = this->actor.shape.rot.y = this->actor.shape.rot.z = 0;
        } else {
            func_8002D9A4(&this->actor, 150.0f);
            this->timer = 12;
        }
    }
}

void func_809B3CEC(PlayState* play, EnArrow* this) {
    EnArrow_SetupAction(this, func_809B4640);
    Animation_PlayOnce(&this->skelAnime, &gArrow1Anim);
    this->actor.world.rot.y += (s32)(24576.0f * (Rand_ZeroOne() - 0.5f)) + 0x8000;
    this->actor.velocity.y += (this->actor.speedXZ * (0.4f + (0.4f * Rand_ZeroOne())));
    this->actor.speedXZ *= (0.04f + 0.3f * Rand_ZeroOne());
    this->timer = 50;
    this->actor.gravity = -1.5f;
}

void EnArrow_CarryActor(EnArrow* this, PlayState* play) {
    CollisionPoly* hitPoly;
    Vec3f posDiffLastFrame;
    Vec3f actorNextPos;
    Vec3f hitPos;
    f32 temp_f12;
    f32 scale;
    s32 bgId;

    Math_Vec3f_Diff(&this->actor.world.pos, &this->unk_210, &posDiffLastFrame);

    temp_f12 = ((this->actor.world.pos.x - this->hitActor->world.pos.x) * posDiffLastFrame.x) +
               ((this->actor.world.pos.y - this->hitActor->world.pos.y) * posDiffLastFrame.y) +
               ((this->actor.world.pos.z - this->hitActor->world.pos.z) * posDiffLastFrame.z);

    if (!(temp_f12 < 0.0f)) {
        scale = Math3D_Vec3fMagnitudeSq(&posDiffLastFrame);

        if (!(scale < 1.0f)) {
            scale = temp_f12 / scale;
            Math_Vec3f_Scale(&posDiffLastFrame, scale);
            Math_Vec3f_Sum(&this->hitActor->world.pos, &posDiffLastFrame, &actorNextPos);

            if (BgCheck_EntityLineTest1(&play->colCtx, &this->hitActor->world.pos, &actorNextPos, &hitPos, &hitPoly,
                                        true, true, true, true, &bgId)) {
                this->hitActor->world.pos.x = hitPos.x + ((actorNextPos.x <= hitPos.x) ? 1.0f : -1.0f);
                this->hitActor->world.pos.y = hitPos.y + ((actorNextPos.y <= hitPos.y) ? 1.0f : -1.0f);
                this->hitActor->world.pos.z = hitPos.z + ((actorNextPos.z <= hitPos.z) ? 1.0f : -1.0f);
            } else {
                Math_Vec3f_Copy(&this->hitActor->world.pos, &actorNextPos);
            }
        }
    }
}



void EnArrow_SpawnIce(EnArrow* this, PlayState* play) { // added ice arrow spawn platform function
    // WaterBox* waterBox;
    // f32 waterY = this->actor.world.pos.y;
    // Vec3f pos;
    // f32 temp_f0;
    // s32 params = -1;
    // if (WaterBox_GetSurface1(play, &play->colCtx, this->actor.world.pos.x, this->actor.world.pos.z, &waterY, &waterBox)) {
    //         if ((this->actor.world.pos.y <= waterY) && !(this->underWater)) { // checks if arrow is underneath water
    //         this->underWater = 1  ; // set this to not cause multiple detections, in water
    //         Math_Vec3f_Diff(&this->actor.world.pos, &this->actor.home.pos, &pos);
    //         if (pos.y != 0.0f) {
    //             temp_f0 = sqrtf(SQ(pos.x) + SQ(pos.z));
    //             if (temp_f0 != 0.0f) {
    //                 temp_f0 = (((waterY - this->actor.home.pos.y) / pos.y) * temp_f0) / temp_f0;
    //             }
    //             pos.x = this->actor.home.pos.x + (pos.x * temp_f0);
    //             pos.y = waterY;
    //             pos.z = this->actor.home.pos.z + (pos.z * temp_f0);
    //             EffectSsGSplash_Spawn(play, &this->hitWaterPos, NULL, NULL, 0, 300);
    //             this->hitWaterPos.x = pos.x;
    //             this->hitWaterPos.y = pos.y;
    //             this->hitWaterPos.z = pos.z;
    //         }  
    //         SfxSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 40, NA_SE_EV_DIVE_INTO_WATER_L);

    //         EffectSsGRipple_Spawn(play, &this->hitWaterPos.x, 100, 500, 0);
    //         EffectSsGRipple_Spawn(play, &this->hitWaterPos.y, 100, 500, 4);
    //         EffectSsGRipple_Spawn(play, &this->hitWaterPos.z, 100, 500, 8);
    //     } 

    //     if((this->underWater) ){
    //         Actor_UpdateBgCheckInfo(play, &this->actor, 1.0f, 1.0f, 0.0f, ACTOR_BGCHECK_WALL | ACTOR_BGCHECK_FLOOR);        
    //         if(((waterY - this->actor.world.pos.y) < 15.f) && (this->actor.bgCheckFlags | BGCHECKFLAG_GROUND) ){
    //             params = SPAWN_ON_GROUND;
    //         }else if((waterY - this->actor.world.pos.y) >= 15.f){
    //             params = SPAWN_ON_WATER;
    //         }
    //         this->underWater = 0;
    //     }
    //     if(params != -1){
    //         if ((this->actor.params == ARROW_ICE) && (func_809B4640 != this->actionFunc) && (Actor_FindNearby(play, &this->actor, 0x020A, ACTORCAT_PROP, 1000.0f) == NULL)
    //         ) {
    //             Actor_Spawn(&play->actorCtx, play, 0x0240, this->hitWaterPos.x, this->hitWaterPos.y, this->hitWaterPos.z, 0, 0, 0, params);
    //             //Item_DropCollectible(play, &pos, ITEM00_HEART_PIECE);
    //             Actor_Kill(&this->actor);
    //             return;
    //         }
    //         this->actor.params = ARROW_NORMAL;
    //         this->collider.info.toucher.dmgFlags = 0x20;

    //         if (this->actor.child != NULL) {
    //             Actor_Kill(this->actor.child);
    //             return;
    //         }

    //     }
        
    // }
    WaterBox* waterBox;
    f32 arrowHeight = this->actor.world.pos.y;
    Vec3f arrowToHomeVector;
    f32 distanceToHome;
    Vec3f distanceHomeOffset;

    if (WaterBox_GetSurface1(play, &play->colCtx, this->actor.world.pos.x, this->actor.world.pos.z, &arrowHeight, &waterBox) &&
        (this->actor.world.pos.y < arrowHeight) && !(this->actor.bgCheckFlags & BGCHECKFLAG_WATER)) {
        this->actor.bgCheckFlags |= BGCHECKFLAG_WATER;

        distanceHomeOffset.x = this->actor.home.pos.x;
        distanceHomeOffset.z = this->actor.home.pos.z;
        distanceHomeOffset.y = this->actor.home.pos.y + 10;

        Math_Vec3f_Diff(&this->actor.world.pos, &this->actor.home.pos, &arrowToHomeVector);

        if (arrowToHomeVector.y != 0.0f) {
            distanceToHome = sqrtf(SQ(arrowToHomeVector.x) + SQ(arrowToHomeVector.z));
            if (distanceToHome != 0.0f) {
                distanceToHome = (((arrowHeight - distanceHomeOffset.y) / arrowToHomeVector.y) * distanceToHome) / distanceToHome;
            }
            arrowToHomeVector.x = this->actor.home.pos.x + (arrowToHomeVector.x * distanceToHome);
            arrowToHomeVector.y = arrowHeight;
            arrowToHomeVector.z = this->actor.home.pos.z + (arrowToHomeVector.z * distanceToHome);
            EffectSsGSplash_Spawn(play, &arrowToHomeVector, NULL, NULL, 0, 300);
        }

        Audio_PlayActorSfx2(&this->actor, NA_SE_EV_DIVE_INTO_WATER_L);

        EffectSsGRipple_Spawn(play, &arrowToHomeVector, 100, 500, 0);
        EffectSsGRipple_Spawn(play, &arrowToHomeVector, 100, 500, 4);
        EffectSsGRipple_Spawn(play, &arrowToHomeVector, 100, 500, 8);

        if ((this->actor.params == ARROW_ICE) || (this->actor.params == ARROW_FIRE)) {
            if ((this->actor.params == ARROW_ICE) && (func_809B45E0 != this->actionFunc) && (Actor_FindNearby(play, &this->actor, 0x020A, ACTORCAT_PROP, 1000.0f) == NULL)) {
                Actor_Spawn(&play->actorCtx, play, 0x0240, arrowToHomeVector.x, arrowToHomeVector.y, arrowToHomeVector.z, 0, 0, 0, SPAWN_ON_GROUND);
                Actor_Kill(&this->actor);
                return;
            }

            this->actor.params = ARROW_NORMAL;
            this->collider.info.toucher.dmgFlags = 0x20;

            if (this->actor.child != NULL) {
                Actor_Kill(this->actor.child);
                return;
            }

            Magic_Reset(play);
        }
    }

}

void EnArrow_Fly(EnArrow* this, PlayState* play) {
    CollisionPoly* hitPoly;
    s32 bgId;
    Vec3f hitPoint;
    Vec3f posCopy;
    s32 atTouched;
    u16 sfxId;
    Actor* hitActor;
    Vec3f sp60;
    Vec3f sp54;

    if (DECR(this->timer) == 0) {
        Actor_Kill(&this->actor);
        return;
    }

    if (this->timer < 7.2000003f) {
        this->actor.gravity = -0.4f;
    }

    atTouched = (this->actor.params != ARROW_NORMAL_LIT) && (this->actor.params <= ARROW_SEED) &&
                (this->collider.base.atFlags & AT_HIT);


    atTouched = this->actor.params != ARROW_BLUE_FIRE_LIT && atTouched;
    if (atTouched || this->touchedPoly) {
        EnArrow_SpawnIce(this,play);
        if (this->actor.params >= ARROW_SEED && this->actor.params != ARROW_BLUE_FIRE_LIT) {
            if (atTouched) {
                this->actor.world.pos.x = (this->actor.world.pos.x + this->actor.prevPos.x) * 0.5f;
                this->actor.world.pos.y = (this->actor.world.pos.y + this->actor.prevPos.y) * 0.5f;
                this->actor.world.pos.z = (this->actor.world.pos.z + this->actor.prevPos.z) * 0.5f;
            }

            if (this->actor.params == ARROW_NUT) {
                iREG(50) = -1;
                Actor_Spawn(&play->actorCtx, play, ACTOR_EN_M_FIRE1, this->actor.world.pos.x, this->actor.world.pos.y,
                            this->actor.world.pos.z, 0, 0, 0, 0);
                sfxId = NA_SE_IT_DEKU;
            } else {
                sfxId = NA_SE_IT_SLING_REFLECT;
            }

            EffectSsStone1_Spawn(play, &this->actor.world.pos, 0);
            SfxSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 20, sfxId);
            Actor_Kill(&this->actor);
        } else {
            EffectSsHitMark_SpawnCustomScale(play, 0, 150, &this->actor.world.pos);

            if (atTouched && (this->collider.info.atHitInfo->elemType != ELEMTYPE_UNK4)) {
                hitActor = this->collider.base.at;

                if ((hitActor->update != NULL) && !(this->collider.base.atFlags & AT_BOUNCED) &&
                    (hitActor->flags & ACTOR_FLAG_14)) {
                    this->hitActor = hitActor;
                    EnArrow_CarryActor(this, play);
                    Math_Vec3f_Diff(&hitActor->world.pos, &this->actor.world.pos, &this->unk_250);
                    hitActor->flags |= ACTOR_FLAG_15;
                    this->collider.base.atFlags &= ~AT_HIT;
                    this->actor.speedXZ /= 2.0f;
                    this->actor.velocity.y /= 2.0f;
                } else {
                    this->hitFlags |= 1;
                    this->hitFlags |= 2;

                    if (this->collider.info.atHitInfo->bumperFlags & BUMP_HIT) {
                        this->actor.world.pos.x = this->collider.info.atHitInfo->bumper.hitPos.x;
                        this->actor.world.pos.y = this->collider.info.atHitInfo->bumper.hitPos.y;
                        this->actor.world.pos.z = this->collider.info.atHitInfo->bumper.hitPos.z;
                    }

                    func_809B3CEC(play, this);
                    Audio_PlayActorSfx2(&this->actor, NA_SE_IT_ARROW_STICK_CRE);
                }
            } else if (this->touchedPoly) {
                EnArrow_SetupAction(this, func_809B45E0);
                Animation_PlayOnce(&this->skelAnime, &gArrow2Anim);

                if (this->actor.params >= ARROW_NORMAL_LIT) {
                    this->timer = 60;
                } else {
                    this->timer = 20;
                }

                Audio_PlayActorSfx2(&this->actor, NA_SE_IT_ARROW_STICK_OBJ);
                this->hitFlags |= 1;
            }
        }
    } else {
        Math_Vec3f_Copy(&this->unk_210, &this->actor.world.pos);
        Actor_MoveForward(&this->actor);

        if ((this->touchedPoly =
                 BgCheck_ProjectileLineTest(&play->colCtx, &this->actor.prevPos, &this->actor.world.pos, &hitPoint,
                                            &this->actor.wallPoly, true, true, true, true, &bgId))) {
            func_8002F9EC(play, &this->actor, this->actor.wallPoly, bgId, &hitPoint);
            Math_Vec3f_Copy(&posCopy, &this->actor.world.pos);
            Math_Vec3f_Copy(&this->actor.world.pos, &hitPoint);
        }

        if ((this->actor.params <= ARROW_0E) || (this->actor.params == ARROW_BLUE_FIRE_LIT)) {
            this->actor.shape.rot.x = Math_Atan2S(this->actor.speedXZ, -this->actor.velocity.y);
        }
    }

    if (this->hitActor != NULL) {
        if (this->hitActor->update != NULL) {
            Math_Vec3f_Sum(&this->unk_210, &this->unk_250, &sp60);
            Math_Vec3f_Sum(&this->actor.world.pos, &this->unk_250, &sp54);

            if (BgCheck_EntityLineTest1(&play->colCtx, &sp60, &sp54, &hitPoint, &hitPoly, true, true, true, true,
                                        &bgId)) {
                this->hitActor->world.pos.x = hitPoint.x + ((sp54.x <= hitPoint.x) ? 1.0f : -1.0f);
                this->hitActor->world.pos.y = hitPoint.y + ((sp54.y <= hitPoint.y) ? 1.0f : -1.0f);
                this->hitActor->world.pos.z = hitPoint.z + ((sp54.z <= hitPoint.z) ? 1.0f : -1.0f);
                Math_Vec3f_Diff(&this->hitActor->world.pos, &this->actor.world.pos, &this->unk_250);
                this->hitActor->flags &= ~ACTOR_FLAG_15;
                this->hitActor = NULL;
            } else {
                Math_Vec3f_Sum(&this->actor.world.pos, &this->unk_250, &this->hitActor->world.pos);
            }

            if (this->touchedPoly && (this->hitActor != NULL)) {
                this->hitActor->flags &= ~ACTOR_FLAG_15;
                this->hitActor = NULL;
            }
        } else {
            this->hitActor = NULL;
        }
    }
}

void func_809B45E0(EnArrow* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime);

    if (DECR(this->timer) == 0) {
        Actor_Kill(&this->actor);
    }
}

void func_809B4640(EnArrow* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime);
    Actor_MoveForward(&this->actor);
    if (DECR(this->timer) == 0) {
        Actor_Kill(&this->actor);
    }
}

void EnArrow_Update(Actor* thisx, PlayState* play) {
    s32 pad;
    EnArrow* this = (EnArrow*)thisx;
    Player* player = GET_PLAYER(play);

    if (this->isCsNut || ((this->actor.params >= ARROW_NORMAL_LIT) && (player->unk_A73 != 0)) ||
        !Player_InBlockingCsMode(play, player)) {
        this->actionFunc(this, play);
    }

    if ((this->actor.params >= ARROW_FIRE) && (this->actor.params <= ARROW_0E)) {
        s16 elementalActorIds[] = { ACTOR_ARROW_FIRE, ACTOR_ARROW_ICE,  ACTOR_ARROW_LIGHT,
                                    ACTOR_ARROW_FIRE, ACTOR_ARROW_FIRE, ACTOR_ARROW_FIRE };

        if (this->actor.child == NULL) {
            Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, elementalActorIds[this->actor.params - 3],
                               this->actor.world.pos.x, this->actor.world.pos.y, this->actor.world.pos.z, 0, 0, 0, 0);
        }
    } else if ((this->actor.params == ARROW_NORMAL_LIT) || (this->actor.params == ARROW_BLUE_FIRE_LIT)) {
        static Vec3f velocity = { 0.0f, 0.5f, 0.0f };
        static Vec3f accel = { 0.0f, 0.5f, 0.0f };
        if (this->actor.params == ARROW_NORMAL_LIT) {
            static Color_RGBA8 primColor = { 255, 255, 100, 255 };
            static Color_RGBA8 envColor = { 255, 50, 0, 0 };
            // spawn dust for the flame
            func_8002836C(play, &this->unk_21C, &velocity, &accel, &primColor, &envColor, 100, 0, 8);
        } else {
            static Color_RGBA8 primColorBlueFire = { 100, 255, 255, 255 };
            static Color_RGBA8 envColorBlueFire = { 0, 50, 255, 0 };
            // spawn dust for the flame
            func_8002836C(play, &this->unk_21C, &velocity, &accel, &primColorBlueFire, &envColorBlueFire, 100, 0, 8);
        }

    }
	Math_Vec3f_Copy(&this->actor.home.pos, &this->actor.prevPos); // added from MM for water surface detection
}

void func_809B4800(EnArrow* this, PlayState* play) {
    static Vec3f D_809B4E88 = { 0.0f, 400.0f, 1500.0f };
    static Vec3f D_809B4E94 = { 0.0f, -400.0f, 1500.0f };
    static Vec3f D_809B4EA0 = { 0.0f, 0.0f, -300.0f };
    Vec3f sp44;
    Vec3f sp38;
    s32 addBlureVertex;

    Matrix_MultVec3f(&D_809B4EA0, &this->unk_21C);

    if (EnArrow_Fly == this->actionFunc) {
        Matrix_MultVec3f(&D_809B4E88, &sp44);
        Matrix_MultVec3f(&D_809B4E94, &sp38);

        if ((this->actor.params <= ARROW_SEED) || (this->actor.params == ARROW_BLUE_FIRE_LIT)) {
            addBlureVertex = this->actor.params <= ARROW_LIGHT;
            addBlureVertex = this->actor.params == ARROW_BLUE_FIRE_LIT || addBlureVertex;

            if (this->hitActor == NULL) {
                addBlureVertex &= func_80090480(play, &this->collider, &this->weaponInfo, &sp44, &sp38);
            } else {
                if (addBlureVertex) {
                    if ((sp44.x == this->weaponInfo.tip.x) && (sp44.y == this->weaponInfo.tip.y) &&
                        (sp44.z == this->weaponInfo.tip.z) && (sp38.x == this->weaponInfo.base.x) &&
                        (sp38.y == this->weaponInfo.base.y) && (sp38.z == this->weaponInfo.base.z)) {
                        addBlureVertex = false;
                    }
                }
            }

            if (addBlureVertex) {
                EffectBlure_AddVertex(Effect_GetByIndex(this->effectIndex), &sp44, &sp38);
            }
        }
    }
}

void EnArrow_Draw(Actor* thisx, PlayState* play) {
    s32 pad;
    EnArrow* this = (EnArrow*)thisx;
    u8 alpha;
    f32 scale;

    if ((this->actor.params <= ARROW_0E) || (this->actor.params == ARROW_BLUE_FIRE_LIT)) {
        Gfx_SetupDL_25Opa(play->state.gfxCtx);
        SkelAnime_DrawLod(play, this->skelAnime.skeleton, this->skelAnime.jointTable, NULL, NULL, this,
                          (this->actor.projectedPos.z < MREG(95)) ? 0 : 1);
    } else if (this->actor.speedXZ != 0.0f) {
        alpha = (Math_CosS(this->timer * 5000) * 127.5f) + 127.5f;

        OPEN_DISPS(play->state.gfxCtx, "../z_en_arrow.c", 1346);

        Gfx_SetupDL_25Xlu2(play->state.gfxCtx);

        if (this->actor.params == ARROW_SEED) {
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, 255);
            gDPSetEnvColor(POLY_XLU_DISP++, 0, 255, 255, alpha);
            scale = 50.0f;
        } else {
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 12, 0, 0, 255);
            gDPSetEnvColor(POLY_XLU_DISP++, 250, 250, 0, alpha);
            scale = 150.0f;
        }

        Matrix_Push();
        Matrix_Mult(&play->billboardMtxF, MTXMODE_APPLY);
        // redundant check because this is contained in an if block for non-zero speed
        Matrix_RotateZ((this->actor.speedXZ == 0.0f) ? 0.0f : BINANG_TO_RAD((play->gameplayFrames & 0xFF) * 4000),
                       MTXMODE_APPLY);
        Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, "../z_en_arrow.c", 1374),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, gEffSparklesDL);
        Matrix_Pop();
        Matrix_RotateY(BINANG_TO_RAD(this->actor.world.rot.y), MTXMODE_APPLY);

        CLOSE_DISPS(play->state.gfxCtx, "../z_en_arrow.c", 1381);
    }

    func_809B4800(this, play);
}