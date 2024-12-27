#include "EnStalmaster.h"


void EnStalmaster_Init(EnStalmaster* this, PlayState* play);
void EnStalmaster_Destroy(EnStalmaster* this, PlayState* play);
void EnStalmaster_Update(EnStalmaster* this, PlayState* play);
void EnStalmaster_Draw(EnStalmaster* this, PlayState* play);
void EnStalmaster_DrawLimbs(EnStalmaster* this, PlayState* play);
void EnStalmaster_DrawEffects(EnStalmaster* this, PlayState* play);

void EnStalmaster_BlowupPlate(EnStalmaster* this, PlayState* play);
void EnStalmaster_BlowupHelmet(EnStalmaster* this, PlayState* play);
void EnStalmaster_BlowupDeath(EnStalmaster* this, PlayState* play);
void EnStalmaster_BlowupArms(EnStalmaster* this, PlayState* play);
void EnStalmaster_GatherArms(EnStalmaster* this, PlayState* play);
void EnStalmaster_UpdateHeadRotation(EnStalmaster* this, PlayState* play);
void EnStalmaster_UpdateTail(EnStalmaster* this, PlayState* play);
void EnStalmaster_Gather(EnStalmaster* this, PlayState* play);
void EnStalmaster_UpdateTorsoRot(EnStalmaster* this, PlayState* play);
void EnStalmaster_UpdateCollisions(EnStalmaster* this, PlayState* play);
void EnStalmaster_UpdateColliderDamage(EnStalmaster* this, PlayState* play, ColliderJntSph collider);
void EnStalmaster_UpdateParts(EnStalmaster* this, PlayState* play);
void EnStalmaster_UpdateEffects(EnStalmaster* this, PlayState* play);

void EnStalmaster_SetupLimbs(EnStalmaster* this, PlayState* play);
void EnStalmaster_SetupIntroduction(EnStalmaster* this, PlayState* play);
void EnStalmaster_Introduction(EnStalmaster* this, PlayState* play);
void EnStalmaster_SetupWalk(EnStalmaster* this, PlayState* play);
void EnStalmaster_Walk(EnStalmaster* this, PlayState* play);
void EnStalmaster_SetupAttack(EnStalmaster* this, PlayState* play);
void EnStalmaster_Attack(EnStalmaster* this, PlayState* play);
void EnStalmaster_SetupGuard(EnStalmaster* this, PlayState* play);
void EnStalmaster_Guard(EnStalmaster* this, PlayState* play);
void EnStalmaster_SetupCounter(EnStalmaster* this, PlayState* play);
void EnStalmaster_Counter(EnStalmaster* this, PlayState* play);
void EnStalmaster_SetupSecondPhase(EnStalmaster* this, PlayState* play);
void EnStalmaster_SecondPhase(EnStalmaster* this, PlayState* play);
void EnStalmaster_SetupDeath(EnStalmaster* this, PlayState* play);
void EnStalmaster_Death(EnStalmaster* this, PlayState* play);
void EnStalmaster_SetupBlowArms(EnStalmaster* this, PlayState* play);
void EnStalmaster_BlowArms(EnStalmaster* this, PlayState* play);

#define FLAGS (ACTORFLAG_ENEMY | ACTORFLAG_ALWAYS_UPDATE | ACTORFLAG_ALWAYS_DRAW)

const ActorInit sEnStalmaster_InitVars = {
    .id = 0x0236,
    .category     = ACTORCAT_ENEMY,
    .flags        = FLAGS,
    .objectId = 0x0235,
    .instanceSize = sizeof(EnStalmaster),
    .init         = (ActorFunc)EnStalmaster_Init,
    .destroy      = (ActorFunc)EnStalmaster_Destroy,
    .update       = (ActorFunc)EnStalmaster_Update,
    .draw         = (ActorFunc)EnStalmaster_Draw
};

static ColliderJntSphElementInit sColliderJntSphElementInitSwordUpLeft[2] = {
    {
        { ELEMTYPE_UNK0, { 0xFFCFFFFF, 0x00, 0X10 }, { 0xFFCFFFFF, 0x00, 0x00 }, TOUCH_ON | TOUCH_SFX_HARD, BUMP_ON, OCELEM_ON, },
        { STALMASTER_LIMB_HANDLU, { { 10.0f, 0.0f, 55.0f }, 21 }, 100 },
    },
    {
        { ELEMTYPE_UNK0, { 0xFFCFFFFF, 0x00, 0X10  }, { 0xFFCFFFFF, 0x00, 0x00 }, TOUCH_ON | TOUCH_SFX_HARD, BUMP_ON, OCELEM_ON, },
        { STALMASTER_LIMB_HANDLU, { { 25.0f, 0.0f, 130.0f }, 23 }, 100 },
    },
};

static ColliderJntSphInit sColliderJntSphInitSwordUpLeft = {
    { COLTYPE_METAL, AT_ON | AT_TYPE_ENEMY, AC_ON | AC_HARD | AC_TYPE_PLAYER, OC1_ON | OC1_TYPE_PLAYER, OC2_TYPE_1, COLSHAPE_JNTSPH, },
    2,
    sColliderJntSphElementInitSwordUpLeft,
};

static ColliderJntSphElementInit sColliderJntSphElementInitSwordUpRight[2] = {
    {
        { ELEMTYPE_UNK0, { 0xFFCFFFFF, 0x00, 0X10 }, { 0xFFCFFFFF, 0x00, 0x00 }, TOUCH_ON | TOUCH_SFX_HARD, BUMP_ON, OCELEM_ON, },
        { STALMASTER_LIMB_HANDRU, { { -10.0f, 0.0f, 20.0f }, 21 }, 100 },
    },
    {
        { ELEMTYPE_UNK0, { 0xFFCFFFFF, 0x00, 0X10 }, { 0xFFCFFFFF, 0x00, 0x00 }, TOUCH_ON | TOUCH_SFX_HARD, BUMP_ON, OCELEM_ON, },
        { STALMASTER_LIMB_HANDRU, { { -25.0f, 0.0f, 95.0f }, 21 }, 100 },
    },
};

static ColliderJntSphInit sColliderJntSphInitSwordUpRight = {
    { COLTYPE_METAL, AT_ON | AT_TYPE_ENEMY, AC_ON | AC_HARD | AC_TYPE_PLAYER, OC1_ON | OC1_TYPE_PLAYER, OC2_TYPE_1, COLSHAPE_JNTSPH, },
    2,
    sColliderJntSphElementInitSwordUpRight,
};

static ColliderJntSphElementInit sColliderJntSphElementInitSwordDownLeft[2] = {
    {
        { ELEMTYPE_UNK0, { 0xFFCFFFFF, 0x00, 0X20 }, { 0xFFCFFFFF, 0x00, 0x00 }, TOUCH_ON | TOUCH_SFX_HARD, BUMP_ON, OCELEM_ON, },
        { STALMASTER_LIMB_HANDLD, { { 10.0f, 0.0f, 55.0f }, 21 }, 100 },
    },
    {
        { ELEMTYPE_UNK0, { 0xFFCFFFFF, 0x00, 0X20  }, { 0xFFCFFFFF, 0x00, 0x00 }, TOUCH_ON | TOUCH_SFX_HARD, BUMP_ON, OCELEM_ON, },
        { STALMASTER_LIMB_HANDLD, { { 25.0f, 0.0f, 130.0f }, 23 }, 100 },
    },
};

static ColliderJntSphInit sColliderJntSphInitSwordDownLeft = {
    { COLTYPE_METAL, AT_ON | AT_TYPE_ENEMY, AC_ON | AC_HARD | AC_TYPE_PLAYER, OC1_ON | OC1_TYPE_PLAYER, OC2_TYPE_1, COLSHAPE_JNTSPH, },
    2,
    sColliderJntSphElementInitSwordDownLeft,
};

static ColliderJntSphElementInit sColliderJntSphElementInitSwordDownRight[2] = {
    {
        { ELEMTYPE_UNK0, { 0xFFCFFFFF, 0x00, 0X10 }, { 0xFFCFFFFF, 0x00, 0x00 }, TOUCH_ON | TOUCH_SFX_HARD, BUMP_ON, OCELEM_ON, },
        { STALMASTER_LIMB_HANDRD, { { -10.0f, 0.0f, 20.0f }, 21 }, 100 },
    },
    {
        { ELEMTYPE_UNK0, { 0xFFCFFFFF, 0x00, 0X10 }, { 0xFFCFFFFF, 0x00, 0x00 }, TOUCH_ON | TOUCH_SFX_HARD, BUMP_ON, OCELEM_ON, },
        { STALMASTER_LIMB_HANDRD, { { -25.0f, 0.0f, 95.0f }, 21 }, 100 },
    },
};

static ColliderJntSphInit sColliderJntSphInitSwordDownRight = {
    { COLTYPE_METAL, AT_ON | AT_TYPE_ENEMY, AC_ON | AC_HARD | AC_TYPE_PLAYER, OC1_ON | OC1_TYPE_PLAYER, OC2_TYPE_1, COLSHAPE_JNTSPH, },
    2,
    sColliderJntSphElementInitSwordDownRight,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON | BUMP_HOOKABLE,
        OCELEM_ON,
    },
    { 30, 90, 0, { 0, 0, 0 } },
};

typedef enum {
    STALMASTER_DMGEFF_NONE,
    STALMASTER_DMGEFF_NORMAL,
    STALMASTER_DMGEFF_PARRY,
    STALMASTER_DMGEFF_EXPLOSIVE,
    STALMASTER_DMGEFF_SPIN,
} STALMASTERDamageEffect;

static DamageTable sStalmasterDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, STALMASTER_DMGEFF_PARRY),
    /* Deku stick    */ DMG_ENTRY(0, STALMASTER_DMGEFF_PARRY),
    /* Slingshot     */ DMG_ENTRY(1, STALMASTER_DMGEFF_PARRY),
    /* Explosive     */ DMG_ENTRY(3, STALMASTER_DMGEFF_EXPLOSIVE),
    /* Boomerang     */ DMG_ENTRY(0, STALMASTER_DMGEFF_PARRY),
    /* Normal arrow  */ DMG_ENTRY(0, STALMASTER_DMGEFF_PARRY),
    /* Hammer swing  */ DMG_ENTRY(4, STALMASTER_DMGEFF_NORMAL),
    /* Hookshot      */ DMG_ENTRY(0, STALMASTER_DMGEFF_PARRY),
    /* Kokiri sword  */ DMG_ENTRY(1, STALMASTER_DMGEFF_NORMAL),
    /* Master sword  */ DMG_ENTRY(2, STALMASTER_DMGEFF_NORMAL),
    /* Giant's Knife */ DMG_ENTRY(4, STALMASTER_DMGEFF_NORMAL),
    /* Fire arrow    */ DMG_ENTRY(0, STALMASTER_DMGEFF_PARRY),
    /* Ice arrow     */ DMG_ENTRY(0, STALMASTER_DMGEFF_PARRY),
    /* Light arrow   */ DMG_ENTRY(0, STALMASTER_DMGEFF_PARRY),
    /* Unk arrow 1   */ DMG_ENTRY(2, STALMASTER_DMGEFF_NONE),
    /* Unk arrow 2   */ DMG_ENTRY(2, STALMASTER_DMGEFF_NONE),
    /* Unk arrow 3   */ DMG_ENTRY(2, STALMASTER_DMGEFF_NONE),
    /* Fire magic    */ DMG_ENTRY(0, STALMASTER_DMGEFF_NONE),
    /* Ice magic     */ DMG_ENTRY(3, STALMASTER_DMGEFF_NONE),
    /* Light magic   */ DMG_ENTRY(0, STALMASTER_DMGEFF_NONE),
    /* Shield        */ DMG_ENTRY(0, STALMASTER_DMGEFF_NONE),
    /* Mirror Ray    */ DMG_ENTRY(0, STALMASTER_DMGEFF_NONE),
    /* Kokiri spin   */ DMG_ENTRY(1, STALMASTER_DMGEFF_SPIN),
    /* Giant spin    */ DMG_ENTRY(4, STALMASTER_DMGEFF_SPIN),
    /* Master spin   */ DMG_ENTRY(2, STALMASTER_DMGEFF_SPIN),
    /* Kokiri jump   */ DMG_ENTRY(2, STALMASTER_DMGEFF_NORMAL),
    /* Giant jump    */ DMG_ENTRY(8, STALMASTER_DMGEFF_NORMAL),
    /* Master jump   */ DMG_ENTRY(4, STALMASTER_DMGEFF_NORMAL),
    /* Unknown 1     */ DMG_ENTRY(0, STALMASTER_DMGEFF_NONE),
    /* Unblockable   */ DMG_ENTRY(0, STALMASTER_DMGEFF_NONE),
    /* Hammer jump   */ DMG_ENTRY(8, STALMASTER_DMGEFF_NORMAL),
    /* Unknown 2     */ DMG_ENTRY(0, STALMASTER_DMGEFF_NONE),
};

void EnStalmaster_Init(EnStalmaster* this, PlayState* play) {
    Actor_SetScale(&this->actor, 0.55f);

    this->hasCS = true;
     this->actor.naviEnemyId = NAVI_ENEMY_BONGO_BONGO;
    this->actor.targetMode = 5;
    this->actor.colChkInfo.mass = MASS_IMMOVABLE;
    this->actor.colChkInfo.damageTable = &sStalmasterDamageTable;
    this->actor.colChkInfo.health = STALMASTER_HEALTH;
    this->switchFlags = (this->actor.params >> 8) & 0xFF;
    if (this->switchFlags != 0x3F && this->switchFlags != 0x0) {
        if (Flags_GetSwitch(play, this->switchFlags)) {
            Actor_Spawn(&play->actorCtx, play, ACTOR_DOOR_WARP1,  this->actor.home.pos.x,  this->actor.home.pos.y,
            this->actor.home.pos.z, 0, 0, 0, 0x53);
           Actor_Kill(&this->actor);
           return;
        }
    } 
    SkelAnime_InitFlex(play, &this->skelAnime, gStalmaster_SkelStalmaster, gStalmaster_AnimOp, this->jointTable, this->morphTable, STALMASTER_LIMB_MAX);
    
    if(0){
        EnStalmaster_SetupWalk(this, play);
        //AudioStream_ChangeSong(3);

        for (int i = 1; i < ARRAY_COUNT(this->sStalmasterParts); i++) {
            this->sStalmasterParts[i].invisible = true;
        }

        this->actor.flags |= ACTORFLAG_CAN_TARGET;
    }else
        EnStalmaster_SetupIntroduction(this, play);

    this->numTail = TAIL_NUMBER;
    this->numParticleTail = this->numTail + 1;

    this->chainTail = new(Chain[this->numTail]);
	this->particleTail = new(Particle[this->numParticleTail]);

    for (int i = 0; i < this->numParticleTail; i++) {
		this->particleTail[i] = Particle_New(this->actor.pos, 1.25f);

		this->particleTail[i].prevPos.y -= Rand_CenteredFloat(1.0f);
		this->particleTail[i].prevPos.x -= Rand_CenteredFloat(1.0f);
		this->particleTail[i].prevPos.z -= Rand_CenteredFloat(1.0f);
	}
	
	for (int i = 0; i < this->numTail; i++)
		this->chainTail[i] = Chain_New(&this->particleTail[i], &this->particleTail[i + 1], TAIL_LENGTH);

    this->actor.gravity = -2.0f;

    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawFeet, 3.0f);

    //init all blure
    EffectBlureInit1 blureInit;
    blureInit.p1StartColor[0] = blureInit.p1StartColor[1] = blureInit.p1StartColor[2] = blureInit.p1StartColor[3] =
        blureInit.p2StartColor[0] = blureInit.p2StartColor[1] = blureInit.p2StartColor[2] = blureInit.p1EndColor[0] =
            blureInit.p1EndColor[1] = blureInit.p1EndColor[2] = blureInit.p2EndColor[0] = blureInit.p2EndColor[1] =
                blureInit.p2EndColor[2] = 100;

    blureInit.p1EndColor[3] = blureInit.p2EndColor[3] = 0;
    blureInit.p2StartColor[3] = 64;
    blureInit.elemDuration = 8;
    blureInit.unkFlag = false;
    blureInit.calcMode = 2;

    Effect_Add(play, &this->effectIndex, EFFECT_BLURE1, 0, 0, &blureInit);
    Effect_Add(play, &this->effectIndex1, EFFECT_BLURE1, 0, 0, &blureInit);
    Effect_Add(play, &this->effectIndex2, EFFECT_BLURE1, 0, 0, &blureInit);
    Effect_Add(play, &this->effectIndex3, EFFECT_BLURE1, 0, 0, &blureInit);

    //collisions
    Collider_InitJntSph(play, &this->colliderSwordUpLeft);
	Collider_SetJntSph(play, &this->colliderSwordUpLeft, &this->actor, &sColliderJntSphInitSwordUpLeft, this->collidersSwordUpLeft);

	Collider_InitJntSph(play, &this->colliderSwordUpRight);
	Collider_SetJntSph(play, &this->colliderSwordUpRight, &this->actor, &sColliderJntSphInitSwordUpRight, this->collidersSwordUpRight);

    Collider_InitJntSph(play, &this->colliderSwordDownRight);
	Collider_SetJntSph(play, &this->colliderSwordDownRight, &this->actor, &sColliderJntSphInitSwordDownRight, this->collidersSwordDownRight);

    Collider_InitJntSph(play, &this->colliderSwordDownLeft);
	Collider_SetJntSph(play, &this->colliderSwordDownLeft, &this->actor, &sColliderJntSphInitSwordDownLeft, this->collidersSwordDownLeft);

    Collider_InitCylinder(play, &this->bodyCollider);
    Collider_SetCylinder(play, &this->bodyCollider, &this->actor, &sCylinderInit);
}

void QuakeAndRumble(PlayState* play, s16 y, s16 countdown, s16 camId) {
    s16 quakeId = Quake_Add(Play_GetCamera(play, camId), 3);

    Rumble_Override(0.0f, 180, 20, 100);
    Quake_SetSpeed(quakeId, 20000);
    Quake_SetQuakeValues(quakeId, y, 0, 0, 0);
    Quake_SetCountdown(quakeId, countdown);
}

void EnStalmaster_UpdateHeadRotation(EnStalmaster* this, PlayState* play) {
    Vec3s vec;

    if(this->shouldUpdateHead)
        Actor_TrackPlayer(play, &this->actor, &this->headRot, &vec, this->actor.focus.pos);
    else{
        this->headRot.x = 0;
        this->headRot.y = 0;
        this->headRot.z = 0;
    }
}

//Those are the ones who only need to get visibles
const int limbIndices[] = { 
                            STALMASTER_LIMB_BACKBONE02, STALMASTER_LIMB_ARMRU01, STALMASTER_LIMB_ARMRU02, STALMASTER_LIMB_PADLD,
                            STALMASTER_LIMB_PADLU, STALMASTER_LIMB_PADRD, STALMASTER_LIMB_PADRU, STALMASTER_LIMB_ARMRD01, STALMASTER_LIMB_ARMRD02,
                            STALMASTER_LIMB_ARMLD01, STALMASTER_LIMB_ARMLD02, STALMASTER_LIMB_ARMLU01, STALMASTER_LIMB_ARMLU02,
                            STALMASTER_LIMB_LOC_SWORDA, STALMASTER_LIMB_LOC_SWORDB, STALMASTER_LIMB_LOC_SWORDC2, STALMASTER_LIMB_LOC_SWORDD2
                            };
bool isInLimbIndices(int index) {
    for (int i = 0; i < sizeof(limbIndices) / sizeof(limbIndices[0]); i++) {
        if (limbIndices[i] == index) {
            return true;
        }
    }
    return false;
}

void EnStalmaster_UpdateParts(EnStalmaster* this, PlayState* play) {
    StalmasterParts* effect = this->sStalmasterParts;

    for (int i = 1; i < ARRAY_COUNT(this->sStalmasterParts); i++) {
        if(!effect[i].csHandled){
            if(effect[i].gravity && !effect[i].onGround){
                effect[i].pos.x += effect[i].velocity.x;
                effect[i].pos.y += effect[i].velocity.y;
                effect[i].pos.z += effect[i].velocity.z;
                effect[i].velocity.y += -4.0f;
                if (effect[i].pos.y < this->actor.floorHeight+5) {
                    if (effect[i].bounceCounter >= 2) {
                        effect[i].onGround = true;
                    }

                    effect[i].velocity.y *= -0.35;
                    effect[i].velocity.x *= 0.8;
                    effect[i].velocity.z *= 0.8;
                    effect[i].pos.y = this->actor.floorHeight+5;
                    effect[i].bounceCounter++;
                }
                effect[i].rot.x += effect[i].velocity.y*35;
                effect[i].rot.y += effect[i].velocity.x*180;
                effect[i].rot.z += effect[i].velocity.z*180;
            }else{
                
                if(i == STALMASTER_LIMB_LOC_SWORDA || i == STALMASTER_LIMB_LOC_SWORDB ||
                i == STALMASTER_LIMB_LOC_SWORDC || i == STALMASTER_LIMB_LOC_SWORDD){
                    effect[i].rot.x = 0;
                }
                if(i == STALMASTER_LIMB_BACKBONE02){
                    effect[i].rot.x = DEG_TO_BINANG(90);
                }
                effect[i].rot.z = 0;
            }
        }
    }
}

void EnStalmaster_UpdateEffects(EnStalmaster* this, PlayState* play) {
    StalmasterParts* effect = this->sStalmasterEffects;

    for (int i = 1; i < ARRAY_COUNT(this->sStalmasterEffects); i++) {
        if(effect[i].gravity && !effect[i].onGround){
            effect[i].pos.x += effect[i].velocity.x;
            effect[i].pos.y += effect[i].velocity.y;
            effect[i].pos.z += effect[i].velocity.z;
            effect[i].velocity.y += -4.0f;
            if (effect[i].pos.y < this->actor.floorHeight+5) {

                if (effect[i].bounceCounter >= 2) {
                    effect[i].onGround = true;
                }

                effect[i].velocity.y *= -0.35;
                effect[i].velocity.x *= 0.8;
                effect[i].velocity.z *= 0.8;
                effect[i].pos.y = this->actor.floorHeight+5;
                effect[i].bounceCounter++;
            }
            effect[i].rot.x += effect[i].velocity.y*35;
            effect[i].rot.y += effect[i].velocity.x*180;
            effect[i].rot.z += effect[i].velocity.z*180;
        }else{
            effect[i].rot.z = 0;
        }
    }

}

void EnStalmaster_BlowupPlate(EnStalmaster* this, PlayState* play) {
    StalmasterParts* effect = this->sStalmasterEffects;

    for (size_t i = 1; i < STALMASTER_MAX_EFFECT; i++) {
        if (effect[i].type == STALMASTER_EFFECT_NONE) {
            effect[i].pos.x = this->sStalmasterParts[STALMASTER_LIMB_BACKBONE02].skeletonPos.x;
            effect[i].pos.y = this->sStalmasterParts[STALMASTER_LIMB_BACKBONE02].skeletonPos.y;
            effect[i].pos.z = this->sStalmasterParts[STALMASTER_LIMB_BACKBONE02].skeletonPos.z;
            effect[i].rot.x = this->sStalmasterParts[STALMASTER_LIMB_BACKBONE02].skeletonRot.x;
            effect[i].rot.y = this->sStalmasterParts[STALMASTER_LIMB_BACKBONE02].skeletonRot.y;
            effect[i].rot.z = this->sStalmasterParts[STALMASTER_LIMB_BACKBONE02].skeletonRot.z;
            effect[i].gravity = true;
            effect[i].velocity.x = Rand_CenteredFloat(15.0f);
            effect[i].velocity.y = Rand_ZeroFloat(15.0f) + 7.0f;
            effect[i].velocity.z = Rand_CenteredFloat(15.0f);
            effect[i].type = STALMASTER_EFFECT_PLATE;
            this->plateOff = true;
            break;
        }
    }
}

void EnStalmaster_BlowupHelmet(EnStalmaster* this, PlayState* play) {
    StalmasterParts* effect = this->sStalmasterEffects;

    for (size_t i = 1; i < STALMASTER_MAX_EFFECT; i++) {
        if (effect[i].type == STALMASTER_EFFECT_NONE) {
            effect[i].pos.x = this->sStalmasterParts[STALMASTER_LIMB_HEAD].skeletonPos.x;
            effect[i].pos.y = this->sStalmasterParts[STALMASTER_LIMB_HEAD].skeletonPos.y;
            effect[i].pos.z = this->sStalmasterParts[STALMASTER_LIMB_HEAD].skeletonPos.z;
            effect[i].rot.x = this->sStalmasterParts[STALMASTER_LIMB_HEAD].skeletonRot.x;
            effect[i].rot.y = this->sStalmasterParts[STALMASTER_LIMB_HEAD].skeletonRot.y;
            effect[i].rot.z = this->sStalmasterParts[STALMASTER_LIMB_HEAD].skeletonRot.z;
            effect[i].gravity = true;
            effect[i].velocity.x = Rand_CenteredFloat(15.0f);
            effect[i].velocity.y = Rand_ZeroFloat(15.0f) + 7.0f;
            effect[i].velocity.z = Rand_CenteredFloat(15.0f);
            effect[i].type = STALMASTER_EFFECT_HELMET;
            this->helmetOff = true;
            break;
        }
    }

    for (size_t j = 0; j < this->numTail; j++)
    {
        for (size_t i = 1; i < STALMASTER_MAX_EFFECT; i++) 
        {
            if (effect[i].type == STALMASTER_EFFECT_NONE) {
                effect[i].pos.x = this->particleTail[j].pos.x;
                effect[i].pos.y = this->particleTail[j].pos.y;
                effect[i].pos.z = this->particleTail[j].pos.z;
                effect[i].rot.x = this->sStalmasterParts[STALMASTER_LIMB_HEAD].skeletonRot.x;
                effect[i].rot.y = this->sStalmasterParts[STALMASTER_LIMB_HEAD].skeletonRot.y;
                effect[i].rot.z = this->sStalmasterParts[STALMASTER_LIMB_HEAD].skeletonRot.z;
                effect[i].gravity = true;
                effect[i].velocity.x = Rand_CenteredFloat(15.0f);
                effect[i].velocity.y = Rand_ZeroFloat(10.0f) + 7.0f;
                effect[i].velocity.z = Rand_CenteredFloat(15.0f);
                if(j == this->numTail-1)
                    effect[i].type = STALMASTER_EFFECT_TIP;
                else
                    effect[i].type = STALMASTER_EFFECT_TAIL;
                break;
            }
        }
    }
}

void EnStalmaster_BlowupDeath(EnStalmaster* this, PlayState* play) {
    StalmasterParts* effect = this->sStalmasterParts;

    static const int limbIds[] = {
        STALMASTER_LIMB_HEAD,
        STALMASTER_LIMB_ARMRU01,
        STALMASTER_LIMB_ARMRU02,
        STALMASTER_LIMB_ARMLU01,
        STALMASTER_LIMB_ARMLU02,
        STALMASTER_LIMB_ARMRD01,
        STALMASTER_LIMB_ARMRD02,
        STALMASTER_LIMB_ARMLD01,
        STALMASTER_LIMB_ARMLD02,
        STALMASTER_LIMB_PADLD,
        STALMASTER_LIMB_PADLU,
        STALMASTER_LIMB_PADRD,
        STALMASTER_LIMB_PADRU,
        STALMASTER_LIMB_NECK,
        STALMASTER_LIMB_BACKBONE01,
        STALMASTER_LIMB_BACKBONE02,
        STALMASTER_LIMB_LOC_SWORDA,
        STALMASTER_LIMB_LOC_SWORDB,
        STALMASTER_LIMB_LOC_SWORDC,
        STALMASTER_LIMB_LOC_SWORDD,
    };
    size_t numLimbs = sizeof(limbIds) / sizeof(limbIds[0]);

    for (size_t i = 0; i < numLimbs; ++i) {
        for (size_t j = 0; j < ARRAY_COUNT(this->sStalmasterParts); ++j) {
            if (j == limbIds[i] && effect[j].invisible) {
                effect[j].invisible = false;
                effect[j].csHandled = false;
                effect[j].onGround = false;
                effect[j].gravity = true;
                if(i != STALMASTER_LIMB_HEAD){
                    effect[j].velocity.x = Rand_CenteredFloat(15.0f);
                    effect[j].velocity.z = Rand_CenteredFloat(15.0f);
                    effect[j].velocity.y = Rand_ZeroFloat(15.0f) + 7.0f;
                }else{
                    effect[j].velocity.x = Rand_CenteredFloat(1.0f);
                    effect[j].velocity.z = Rand_CenteredFloat(1.0f);
                    effect[j].velocity.y = Rand_ZeroFloat(4.0f) + 7.0f;
                }
                effect[j].pos = effect[j].skeletonPos;
                effect[j].rot = effect[j].skeletonRot;
            }
        }
    }
}

void EnStalmaster_BlowupArms(EnStalmaster* this, PlayState* play) {
    StalmasterParts* effect = this->sStalmasterParts;

    static const int limbIds[] = {
        STALMASTER_LIMB_ARMRU01,
        STALMASTER_LIMB_ARMRU02,
        STALMASTER_LIMB_ARMLU01,
        STALMASTER_LIMB_ARMLU02,
        STALMASTER_LIMB_ARMRD01,
        STALMASTER_LIMB_ARMRD02,
        STALMASTER_LIMB_ARMLD01,
        STALMASTER_LIMB_ARMLD02,
        STALMASTER_LIMB_PADLD,
        STALMASTER_LIMB_PADLU,
        STALMASTER_LIMB_PADRD,
        STALMASTER_LIMB_PADRU,
        STALMASTER_LIMB_LOC_SWORDA,
        STALMASTER_LIMB_LOC_SWORDB,
        STALMASTER_LIMB_LOC_SWORDC,
        STALMASTER_LIMB_LOC_SWORDD,
    };
    size_t numLimbs = sizeof(limbIds) / sizeof(limbIds[0]);

    for (size_t i = 0; i < numLimbs; ++i) {
        for (size_t j = 0; j < ARRAY_COUNT(this->sStalmasterParts); ++j) {
            if( !(!this->inSecondPhase && i == STALMASTER_LIMB_LOC_SWORDC) && !(!this->inSecondPhase && i == STALMASTER_LIMB_LOC_SWORDD) 
                && !(!this->inSecondPhase && i == STALMASTER_LIMB_PADRD) && !(!this->inSecondPhase && i == STALMASTER_LIMB_PADLD)){
                if (j == limbIds[i] && effect[j].invisible) {
                    effect[j].invisible = false;
                    effect[j].csHandled = false;
                    effect[j].onGround = false;
                    effect[j].gravity = true;
                    effect[j].velocity.x = Rand_CenteredFloat(14.0f);
                    effect[j].velocity.z = Rand_CenteredFloat(14.0f);
                    effect[j].velocity.y = Rand_ZeroFloat(6.0f) + 34.0f;
                    effect[j].pos = effect[j].skeletonPos;
                    effect[j].rot = effect[j].skeletonRot;
                }
            }
        }
    }
}

void EnStalmaster_GatherArms(EnStalmaster* this, PlayState* play) {
    StalmasterParts* effect = this->sStalmasterParts;
    this->armBlowed = false;
    static const int limbIds[] = {
        STALMASTER_LIMB_ARMRU01,
        STALMASTER_LIMB_ARMRU02,
        STALMASTER_LIMB_ARMLU01,
        STALMASTER_LIMB_ARMLU02,
        STALMASTER_LIMB_ARMRD01,
        STALMASTER_LIMB_ARMRD02,
        STALMASTER_LIMB_ARMLD01,
        STALMASTER_LIMB_ARMLD02,
        STALMASTER_LIMB_PADLD,
        STALMASTER_LIMB_PADLU,
        STALMASTER_LIMB_PADRD,
        STALMASTER_LIMB_PADRU,
        STALMASTER_LIMB_LOC_SWORDA,
        STALMASTER_LIMB_LOC_SWORDB,
        STALMASTER_LIMB_LOC_SWORDC,
        STALMASTER_LIMB_LOC_SWORDD,
    };
    size_t numLimbs = sizeof(limbIds) / sizeof(limbIds[0]);

    f32 value1 = 2.0f;
    f32 value2 = 24.0f;
    f32 value3 = 0.4f;

    for (size_t i = 0; i < numLimbs; ++i) {
        for (size_t j = 0; j < ARRAY_COUNT(this->sStalmasterParts); ++j) {
            if( !(!this->inSecondPhase && i == STALMASTER_LIMB_LOC_SWORDC) && !(!this->inSecondPhase && i == STALMASTER_LIMB_LOC_SWORDD) 
                && !(!this->inSecondPhase && i == STALMASTER_LIMB_PADRD) && !(!this->inSecondPhase && i == STALMASTER_LIMB_PADLD)){
                if (j == limbIds[i]) {
                    f32 dist_x;
                    f32 dist_y;
                    f32 dist_z;

                    dist_x = effect[j].skeletonPos.x - effect[j].pos.x;
                    dist_y = effect[j].skeletonPos.y - effect[j].pos.y;
                    dist_z = effect[j].skeletonPos.z - effect[j].pos.z;

                    dist_y *= 1.75f;

                    //Make the real limb appear if close enough
                    if (sqrtf(SQ(dist_x) + SQ(dist_y) + SQ(dist_z)) < 30.0f) {
                        effect[j].invisible = true;
                        effect[j].onGround = false;
                    }
                    
                    Math_SmoothStepToF(&effect[j].pos.x, effect[j].skeletonPos.x, value1, value2, value3);
                    Math_SmoothStepToF(&effect[j].pos.y, effect[j].skeletonPos.y, value1, value2, value3);
                    Math_SmoothStepToF(&effect[j].pos.z, effect[j].skeletonPos.z, value1, value2, value3);
                    Math_SmoothStepToS(&effect[j].rot.x, effect[j].skeletonRot.x, 0xA*1.5, 0x1F40*1.5, 0x14*1.5);
                    Math_SmoothStepToS(&effect[j].rot.y, effect[j].skeletonRot.y, 0xA*1.5, 0x1F40*1.5, 0x14*1.5);
                    Math_SmoothStepToS(&effect[j].rot.z, effect[j].skeletonRot.z, 0xA*1.5, 0x1F40*1.5, 0x14*1.5);
                }
            }
        }
    }
}

void EnStalmaster_SetupLimbs(EnStalmaster* this, PlayState* play) {
    for (int i = 1; i < ARRAY_COUNT(this->sStalmasterParts); i++) {
        this->sStalmasterParts[i].pos.y = this->actor.floorHeight;
        this->sStalmasterParts[i].pos.x = this->actor.pos.x + Rand_CenteredFloat(BONE_SPREAD);
        this->sStalmasterParts[i].pos.z = this->actor.pos.z + Rand_CenteredFloat(BONE_SPREAD);
        if(i == STALMASTER_LIMB_HEAD){
            this->sStalmasterParts[i].invisible = false;
            this->sStalmasterParts[i].pos.x = (Math_SinS(this->actor.world.rot.y) * 67.0f) + this->actor.world.pos.x;;
            this->sStalmasterParts[i].pos.y = this->actor.world.pos.y;
            this->sStalmasterParts[i].pos.z = (Math_CosS(this->actor.world.rot.y) * 67.0f) + this->actor.world.pos.z;
        }else if(isInLimbIndices(i)){
            this->sStalmasterParts[i].invisible = false;
            //Do that so its flat on the ground
            if(i == STALMASTER_LIMB_BACKBONE02){
                this->sStalmasterParts[i].rot.x = -DEG_TO_BINANG(90);
            }
            this->sStalmasterParts[i].rot.y = Rand_CenteredFloat(0x8000);
        }else{
            this->sStalmasterParts[i].invisible = true;
        }
        this->sStalmasterParts[i].csHandled = true;
    }
}

void EnStalmaster_Gather(EnStalmaster* this, PlayState* play) {
    s16 i;
    f32 value1 = 1.0f;
    f32 value2 = 12.0f;
    f32 value3 = 0.2f;

    for (i = 1; i < ARRAY_COUNT(this->sStalmasterParts); i++) {
        if(i == STALMASTER_LIMB_HEAD) //The head is mostly cutscene managed
            continue;

        if(this->sStalmasterParts[i].invisible)
            continue;
            
        f32 dist_x;
        f32 dist_y;
        f32 dist_z;

        dist_x = this->sStalmasterParts[i].skeletonPos.x - this->sStalmasterParts[i].pos.x;
        dist_y = this->sStalmasterParts[i].skeletonPos.y - this->sStalmasterParts[i].pos.y;
        dist_z = this->sStalmasterParts[i].skeletonPos.z - this->sStalmasterParts[i].pos.z;

        dist_y *= 1.75f;

        //Make the real limb appear if close enough
        if (sqrtf(SQ(dist_x) + SQ(dist_y) + SQ(dist_z)) < 1.0f) {
            this->sStalmasterParts[i].invisible = true;
        }
        
        //The swords are faster
        if(i == STALMASTER_LIMB_LOC_SWORDA || i == STALMASTER_LIMB_LOC_SWORDB){
            Math_SmoothStepToF(&this->sStalmasterParts[i].pos.x, this->sStalmasterParts[i].skeletonPos.x, 1.0f, 20.0f, 0.0f);
            Math_SmoothStepToF(&this->sStalmasterParts[i].pos.y, this->sStalmasterParts[i].skeletonPos.y, 1.0f, 20.0f, 0.0f);
            Math_SmoothStepToF(&this->sStalmasterParts[i].pos.z, this->sStalmasterParts[i].skeletonPos.z, 1.0f, 20.0f, 0.0f);
            Math_SmoothStepToS(&this->sStalmasterParts[i].rot.x, this->sStalmasterParts[i].skeletonRot.x, 0xA, 0x3E80, 0x14);
            Math_SmoothStepToS(&this->sStalmasterParts[i].rot.y, this->sStalmasterParts[i].skeletonRot.y, 0xA, 0x3E80, 0x14);
            Math_SmoothStepToS(&this->sStalmasterParts[i].rot.z, this->sStalmasterParts[i].skeletonRot.z, 0xA, 0x3E80, 0x14);
        }else{
            Math_SmoothStepToF(&this->sStalmasterParts[i].pos.x, this->sStalmasterParts[i].skeletonPos.x, value1, value2, value3);
            Math_SmoothStepToF(&this->sStalmasterParts[i].pos.y, this->sStalmasterParts[i].skeletonPos.y, value1, value2, value3);
            Math_SmoothStepToF(&this->sStalmasterParts[i].pos.z, this->sStalmasterParts[i].skeletonPos.z, value1, value2, value3);
            Math_SmoothStepToS(&this->sStalmasterParts[i].rot.x, this->sStalmasterParts[i].skeletonRot.x, 0xA, 0x1F40, 0x14);
            Math_SmoothStepToS(&this->sStalmasterParts[i].rot.y, this->sStalmasterParts[i].skeletonRot.y, 0xA, 0x1F40, 0x14);
            Math_SmoothStepToS(&this->sStalmasterParts[i].rot.z, this->sStalmasterParts[i].skeletonRot.z, 0xA, 0x1F40, 0x14);
        }
    }

}

void EnStalmaster_UpdateTail(EnStalmaster* this, PlayState* play) {
    f32 dampen;

    dampen = -0.25f;
    static Vec3f force = { 0.0f, 0.01f, 0.0f };

    if(this->sStalmasterParts[STALMASTER_LIMB_HEAD].invisible){
        this->particleTail[0].pos = this->tailPos[0];

        //Simulate the bending, only if the head is "on the body", PS: its trash, it get the job done but the result is meh
        if(this->actor.focus.pos.y >= this->actor.floorHeight + 30){
            this->particleTail[1].pos = this->tailPos[1];
            this->particleTail[2].pos = this->tailPos[2];
            this->particleTail[3].pos = this->tailPos[3];
        }
    }else{
        //Todo use rot
        this->particleTail[0].pos = this->sStalmasterParts[STALMASTER_LIMB_HEAD].pos;
    }

	for (int i = 0; i < this->numParticleTail; i++) {
		Particle* particle = &this->particleTail[i];
		
        Particle_Update(particle, -3.0f, force, dampen);
		
		particle->pos.y = CLAMP_MIN(particle->pos.y, this->actor.floorHeight);
		particle->prevPos.y = CLAMP_MIN(particle->prevPos.y, this->actor.floorHeight);
	}

    for (int i = 0; i < this->numTail; i++)
		Chain_Update(&this->chainTail[i]);

}

void EnStalmaster_SetupIntroduction(EnStalmaster* this, PlayState* play) {
    this->actionFunc = EnStalmaster_Introduction;
    EnStalmaster_SetupLimbs(this,play);
}

void EnStalmaster_Introduction(EnStalmaster* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    this->csTimer++;
    Camera* mainCam;

    switch (this->csState) {
        case STALMASTER_CUTSCENE_INTRO_WAIT:
            //Make sure all parts are on the floors.
            for (s16 i = 1; i < ARRAY_COUNT(this->sStalmasterParts); i++) {
                this->sStalmasterParts[i].pos.y = this->actor.floorHeight;
            }
            this->sStalmasterParts[STALMASTER_LIMB_HEAD].rot = this->sStalmasterParts[STALMASTER_LIMB_HEAD].skeletonRot;

            Animation_MorphToPlayOnce(&this->skelAnime, gStalmaster_AnimOp, 0.0f);
            if(this->actor.xzDistToPlayer <= 180.0f){
                this->subCamEye.x = (Math_SinS(this->actor.world.rot.y - DEG_TO_BINANG(20)) * 200.0f) + this->actor.world.pos.x;
                this->subCamEye.y = this->actor.world.pos.y + 25;
                this->subCamEye.z = (Math_CosS(this->actor.world.rot.y - DEG_TO_BINANG(20)) * 200.0f) + this->actor.world.pos.z;

                this->subCamAt.x = this->actor.world.pos.x;
                this->subCamAt.y = this->actor.world.pos.y + 55;
                this->subCamAt.z = this->actor.world.pos.z;

                this->csState = STALMASTER_CUTSCENE_INTRO_OPENING;
                
                if(this->hasCS){
                    func_80064520(play, &play->csCtx);
                    func_8002DF54(play, &this->actor, 1);
                    this->subCamId = Play_CreateSubCamera(play);
                    Play_ChangeCameraStatus(play, CAM_ID_MAIN, CAM_STAT_WAIT);
                    Play_ChangeCameraStatus(play, this->subCamId, CAM_STAT_ACTIVE);
                    this->subCamFov = 85;
                    this->csTimer = 0;
                    Audio_QueueSeqCmd(NA_BGM_STOP); //I used streamed music for it myself, feel free to do so and reactivate this.
                    
                    this->enemyDummy = (EnDummyEnemy*)Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, DUMMYENEMY_ID,
                                                        this->actor.world.pos.x, this->actor.world.pos.y, this->actor.world.pos.z, 0, 0, 0, 0); //Used for clear flag, f oot
                    Actor_ChangeCategory(play, &play->actorCtx, &this->actor, ACTORCAT_BOSS);
                }

                SoundEffect_PlayOneshot(SOUND_STALMASTER_TELEGRAPH, 1.0f, 0.6f, &this->actor.world.pos, SFX_REVERB, SFX_DIST, this);
                SoundEffect_PlayOneshot(SOUND_STALMASTER_GATHER, 1.0f, 1.0f, &this->actor.world.pos, SFX_REVERB, SFX_DIST, this);
            }
        break;

        case STALMASTER_CUTSCENE_INTRO_OPENING:
            if(this->hasCS){
                player->actor.world.pos.x = (Math_SinS(this->actor.world.rot.y) * 155.0f) + this->actor.world.pos.x;;
                player->actor.world.pos.y = this->actor.world.pos.y;
                player->actor.world.pos.z = (Math_CosS(this->actor.world.rot.y) * 155.0f) + this->actor.world.pos.z;
            }

            if(this->skelAnime.curFrame >= 165)
                this->subCamFov += 0.35f;
            else
                this->subCamFov -= 0.1f;

            if(this->skelAnime.curFrame >= 79 && this->skelAnime.curFrame < 110){
                this->sStalmasterParts[STALMASTER_LIMB_HEAD].pos = this->headHandPos;
                this->sStalmasterParts[STALMASTER_LIMB_HEAD].rot = this->sStalmasterParts[STALMASTER_LIMB_HEAD].skeletonRot;
            }

            EnStalmaster_Gather(this,play);

            if(Animation_OnFrame(&this->skelAnime, 79.0f))
            {
                SoundEffect_PlayOneshot(SOUND_STALMASTER_PICK, 1.0f, 1.0f, &this->actor.focus.pos, SFX_REVERB, SFX_DIST, this);
            }
            else if(Animation_OnFrame(&this->skelAnime, 149.0f))
            {
                SoundEffect_PlayOneshot(SOUND_STALMASTER_PICK, 1.0f, 0.98f, &this->actor.focus.pos, SFX_REVERB, SFX_DIST, this);
            }
            else if(Animation_OnFrame(&this->skelAnime, 153.0f))
            {
                SoundEffect_PlayOneshot(SOUND_STALMASTER_PICK, 1.0f, 1.00f, &this->actor.focus.pos, SFX_REVERB, SFX_DIST, this);
            }
            else if(Animation_OnFrame(&this->skelAnime, 110.0f))
            {
                this->sStalmasterParts[STALMASTER_LIMB_HEAD].invisible = true;
            }
            else if(Animation_OnFrame(&this->skelAnime, 115.0f))
            {
                SoundEffect_PlayOneshot(SOUND_STALMASTER_REPLACE, 1.0f, 1.0f, &this->actor.focus.pos, SFX_REVERB, SFX_DIST, this);
            }
            else if(Animation_OnFrame(&this->skelAnime, 130.0f))
            {
                SoundEffect_PlayOneshot(SOUND_STALMASTER_TELEGRAPH, 1.0f, 1.0f, &this->actor.focus.pos, SFX_REVERB, SFX_DIST, this);
            }
            else if(Animation_OnFrame(&this->skelAnime, 165.0f))
            {
                SoundEffect_PlayOneshot(SOUND_STALMASTER_SCREECH, 1.0f, 1.0f, &this->actor.focus.pos, SFX_REVERB, SFX_DIST, this);
                QuakeAndRumble(play, 8, 20, this->subCamId);
                if(this->hasCS)
                    func_8002DF54(play, &this->actor, 5);
            }
            else if(Animation_OnFrame(&this->skelAnime, 180.0f))
            {
            
                //AudioStream_ChangeSong(3); //I used streamed music for it myself, feel free to do so and reactivate this.
                func_800F5ACC(NA_BGM_MINI_BOSS);
                if(this->hasCS){
                    mainCam = Play_GetCamera(play, CAM_ID_MAIN);
                    mainCam->eye = this->subCamEye;
                    mainCam->eyeNext = this->subCamEye;
                    mainCam->at = this->subCamAt;
                    func_800C08AC(play, this->subCamId, 0);
                    this->subCamId = SUB_CAM_ID_DONE;
                    func_80064534(play, &play->csCtx);
                    func_8002DF54(play, &this->actor, 7);
                    Actor_ChangeCategory(play, &play->actorCtx, &this->actor, ACTORCAT_ENEMY);
                    if(this->enemyDummy != NULL)
                        Actor_Kill(&this->enemyDummy->actor);
                }
                this->actor.flags |= ACTORFLAG_CAN_TARGET;
                EnStalmaster_SetupWalk(this,play);
            }
        break;
    }

    if (this->subCamId != SUB_CAM_ID_DONE){
        Play_CameraSetAtEye(play, this->subCamId, &this->subCamAt, &this->subCamEye);
        Play_CameraSetFov(play, this->subCamId, this->subCamFov);
    }
}

void EnStalmaster_SetupWalk(EnStalmaster* this, PlayState* play) {
    this->actionFunc = EnStalmaster_Walk;
    this->shouldUpdateHead = true;
    if(this->inSecondPhase)
        Animation_MorphToLoop(&this->skelAnime, gStalmaster_Anim4Walk, -4.0f);
    else
        Animation_MorphToLoop(&this->skelAnime, gStalmaster_Anim2Walk, -4.0f);

    this->aggressionTimer = 0;
    this->bodyCollider.base.acFlags |= AC_HARD;
}

void EnStalmaster_Walk(EnStalmaster* this, PlayState* play) {
    if(Animation_OnFrame(&this->skelAnime, 0.0f)){
		SoundEffect_PlayOneshot(SOUND_STALMASTER_STEP, 0.2f, Rand_CenteredFloat(0.2f) + 1.0f, &this->actor.shape.feetPos[0], SFX_REVERB, SFX_DIST, this);
	}else if(!this->inSecondPhase && Animation_OnFrame(&this->skelAnime, 16.0f)){
        SoundEffect_PlayOneshot(SOUND_STALMASTER_STEP, 0.2f, Rand_CenteredFloat(0.2f) + 1.0f, &this->actor.shape.feetPos[1], SFX_REVERB, SFX_DIST, this);
    }else if(this->inSecondPhase && Animation_OnFrame(&this->skelAnime, 15.0f)){
        SoundEffect_PlayOneshot(SOUND_STALMASTER_STEP, 0.2f, Rand_CenteredFloat(0.2f) + 1.0f, &this->actor.shape.feetPos[1], SFX_REVERB, SFX_DIST, this);
    }

    if(this->actor.xzDistToPlayer > 240.0f){
        this->skelAnime.playSpeed = 1.5f;
        this->commonTimer = 0;
    }else{
        if(this->actor.xzDistToPlayer < 120.0f){
            if(this->commonTimer == 0 || this->actor.xzDistToPlayer < 100.0f){
                this->commonTimer = 31;
                this->skelAnime.playSpeed = -1.0f;
            }
        }

        if(this->commonTimer == 0 || this->actor.xzDistToPlayer > 200.0f){
            this->commonTimer = 30;
            this->skelAnime.playSpeed = 1.0f;
        }   
    }
    
    if(this->skelAnime.playSpeed == 1.0f && this->actor.xzDistToPlayer < 165.0f){
        this->aggressionTimer++;

        if(this->aggressionTimer >= 13){
            EnStalmaster_SetupAttack(this,play);
        }
    }else{
        this->aggressionTimer = 0;
    }

    this->actor.speedXZ = 1.75f * this->skelAnime.playSpeed;

    Math_SmoothStepToS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 5, 1000.0f, 0x80);
}

void EnStalmaster_SetupAttack(EnStalmaster* this, PlayState* play) {
    this->actionFunc = EnStalmaster_Attack;
    this->shouldUpdateHead = true;

    this->bodyCollider.base.acFlags &= ~AC_HARD;
    
    if(!this->inSecondPhase){
        if(Rand_ZeroOne() < 0.5f)
            Animation_MorphToLoop(&this->skelAnime, gStalmaster_AnimAttackA, -4.0f);
        else
            Animation_MorphToLoop(&this->skelAnime, gStalmaster_AnimAttackB, -4.0f);
    }else{
        switch ((u32)(Rand_ZeroOne() * 3.0f)) {
			case 0:
                Animation_MorphToLoop(&this->skelAnime, gStalmaster_AnimAttackC, -4.0f);
				break;
            case 1:
                Animation_MorphToLoop(&this->skelAnime, gStalmaster_AnimAttackD, -4.0f);
				break;
            case 2:
                Animation_MorphToLoop(&this->skelAnime, gStalmaster_Anim4PoseC, -4.0f);
				break;
		}
    }

    SoundEffect_PlayOneshot(SOUND_STALMASTER_TELEGRAPH, 1.0f, 0.8f, &this->actor.focus.pos, SFX_REVERB, SFX_DIST, this);
    SoundEffect_PlayOneshot(SOUND_STALMASTER_GATHER, 1.0f, 1.0f, &this->actor.focus.pos, SFX_REVERB, SFX_DIST, this);
}

void EnStalmaster_Attack(EnStalmaster* this, PlayState* play) {
    if(this->skelAnime.animation == gStalmaster_AnimAttackA){
        if(Animation_OnFrame(&this->skelAnime, 44.0f)){
            SoundEffect_PlayOneshot(SOUND_STALMASTER_SWING, 1.0f, 1.0f, &this->actor.focus.pos, SFX_REVERB, SFX_DIST, this);
        }else if(Animation_OnFrame(&this->skelAnime, 57.0f)){
            SoundEffect_PlayOneshot(SOUND_STALMASTER_SWING, 1.0f, 1.0f, &this->actor.focus.pos, SFX_REVERB, SFX_DIST, this);
            this->actor.speedXZ = 14.0f;
        }else if(Animation_OnFrame(&this->skelAnime, 42.0f)){
            this->swordAActive = true;
        }else if(Animation_OnFrame(&this->skelAnime, 50.0f)){
            this->swordAActive = false;
        }else if(Animation_OnFrame(&this->skelAnime, 54.0f)){
            this->swordBActive = true;
        }else if(Animation_OnFrame(&this->skelAnime, 63.0f)){
            this->swordBActive = false;
        }

        if(this->skelAnime.curFrame <= 40 || (this->skelAnime.curFrame >= 50 && this->skelAnime.curFrame <= 60)){
            Math_SmoothStepToS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 5, 1000.0f, 0x80);
        }
    }else if(this->skelAnime.animation == gStalmaster_AnimAttackB){
        if(this->skelAnime.curFrame <= 10){
            Math_SmoothStepToS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 5, 1000.0f, 0x80);
        }

        if(Animation_OnFrame(&this->skelAnime, 14.0f)){
            SoundEffect_PlayOneshot(SOUND_STALMASTER_SWING, 1.0f, 1.0f, &this->actor.focus.pos, SFX_REVERB, SFX_DIST, this);
            SoundEffect_StopOneshot(SOUND_STALMASTER_TELEGRAPH, &this->actor.focus.pos, this);
            SoundEffect_StopOneshot(SOUND_STALMASTER_GATHER, &this->actor.focus.pos, this);
            this->swordAActive = true;
        }else if(Animation_OnFrame(&this->skelAnime, 20.0f)){
            this->swordAActive = false;
        }
    }else if(this->skelAnime.animation == gStalmaster_AnimAttackC){
        Math_SmoothStepToS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 5, 900.0f, 0x80);

        if(Animation_OnFrame(&this->skelAnime, 42.0f)){
            SoundEffect_PlayOneshot(SOUND_STALMASTER_SWING, 1.0f, 1.0f, &this->actor.focus.pos, SFX_REVERB, SFX_DIST, this);
            this->swordBActive = true;
            this->actor.speedXZ = 15.5f;
        }else if(Animation_OnFrame(&this->skelAnime, 45.0f)){
            this->swordBActive = false;
        }else if(Animation_OnFrame(&this->skelAnime, 56.0f)){
            SoundEffect_PlayOneshot(SOUND_STALMASTER_SWING, 1.0f, 1.0f, &this->actor.focus.pos, SFX_REVERB, SFX_DIST, this);
            this->swordAActive = true;
            this->actor.speedXZ = 15.5f;
        }else if(Animation_OnFrame(&this->skelAnime, 60.0f)){
            this->swordAActive = false;
        }else if(Animation_OnFrame(&this->skelAnime, 70.0f)){
            SoundEffect_PlayOneshot(SOUND_STALMASTER_SWING, 1.0f, 1.0f, &this->actor.focus.pos, SFX_REVERB, SFX_DIST, this);
            this->swordDActive = true;
            this->actor.speedXZ = 15.5f;
        }else if(Animation_OnFrame(&this->skelAnime, 73.0f)){
            this->swordDActive = false;
        }else if(Animation_OnFrame(&this->skelAnime, 95.0f)){
            SoundEffect_PlayOneshot(SOUND_STALMASTER_SWING, 1.0f, 1.0f, &this->actor.focus.pos, SFX_REVERB, SFX_DIST, this);
            this->swordCActive = true;
            this->actor.speedXZ = 16.5f;
        }else if(Animation_OnFrame(&this->skelAnime, 98.0f)){
            this->swordCActive = false;
        }
    }else if(this->skelAnime.animation == gStalmaster_AnimAttackD){
        Math_SmoothStepToS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 5, 900.0f, 0x80);

        if(Animation_OnFrame(&this->skelAnime, 14.0f)){
            SoundEffect_StopOneshot(SOUND_STALMASTER_TELEGRAPH, &this->actor.focus.pos, this);
            SoundEffect_StopOneshot(SOUND_STALMASTER_GATHER, &this->actor.focus.pos, this);
            SoundEffect_PlayOneshot(SOUND_STALMASTER_SWING, 1.0f, 1.0f, &this->actor.focus.pos, SFX_REVERB, SFX_DIST, this);
            this->swordAActive = true;
            this->swordBActive = true;
            this->actor.speedXZ = 16.5f;
        }else if(Animation_OnFrame(&this->skelAnime, 17.0f)){
            this->swordAActive = false;
            this->swordBActive = false;
        }else if(Animation_OnFrame(&this->skelAnime, 32.0f)){
            SoundEffect_PlayOneshot(SOUND_STALMASTER_SWING, 1.0f, 1.0f, &this->actor.focus.pos, SFX_REVERB, SFX_DIST, this);
            this->swordCActive = true;
            this->swordDActive = true;
            this->actor.speedXZ = 16.5f;
        }else if(Animation_OnFrame(&this->skelAnime, 35.0f)){
            this->swordCActive = false;
            this->swordDActive = false;
        }
    }else if(this->skelAnime.animation == gStalmaster_Anim4PoseC){
        Math_SmoothStepToS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 5, 100.0f, 0x80);

        if(Animation_OnFrame(&this->skelAnime, 51.0f)){
            SoundEffect_PlayOneshot(SOUND_STALMASTER_SWING, 1.0f, 1.0f, &this->actor.focus.pos, SFX_REVERB, SFX_DIST, this);
            this->swordAActive = true;
            this->swordBActive = true;
            this->swordCActive = true;
            this->swordDActive = true;
            this->actor.speedXZ = 18.5f;
        }else if(Animation_OnFrame(&this->skelAnime, 55.0f)){
            this->swordAActive = false;
            this->swordBActive = false;
            this->swordCActive = false;
            this->swordDActive = false;
        }
    }

    if(Animation_OnFrame(&this->skelAnime, Animation_GetLastFrame(this->skelAnime.animation)))
		EnStalmaster_SetupWalk(this,play);

    Math_ApproachF(&this->actor.speedXZ, 0.0f, 0.7f, 2.5f);
}

void EnStalmaster_SetupGuard(EnStalmaster* this, PlayState* play) {
    this->actionFunc = EnStalmaster_Guard;
    this->shouldUpdateHead = true;

    if(this->inSecondPhase)
        Animation_MorphToPlayOnce(&this->skelAnime, gStalmaster_Anim4Guard, -4.0f);
    else
        Animation_MorphToPlayOnce(&this->skelAnime, gStalmaster_Anim2Guard, -4.0f);

    this->actor.speedXZ = -15.0f;
    this->actor.world.rot.y = this->actor.yawTowardsPlayer;
    func_8003424C(play, &this->sStalmasterParts[STALMASTER_LIMB_HANDLU].skeletonPos);
    SoundEffect_StopOneshot(SOUND_STALMASTER_TELEGRAPH, &this->actor.focus.pos, this);
    SoundEffect_StopOneshot(SOUND_STALMASTER_GATHER, &this->actor.focus.pos, this);
    this->swordAActive = this->swordBActive = this->swordCActive = this->swordDActive = false;
}

void EnStalmaster_Guard(EnStalmaster* this, PlayState* play) {
    if(Animation_OnFrame(&this->skelAnime, Animation_GetLastFrame(this->skelAnime.animation)))
		EnStalmaster_SetupWalk(this,play);

    Math_ApproachF(&this->actor.speedXZ, 0.0f, 0.7f, 2.5f);
}

void EnStalmaster_SetupBlowArms(EnStalmaster* this, PlayState* play) {
    this->actionFunc = EnStalmaster_BlowArms;
    this->shouldUpdateHead = true;
    this->armBlowed = true;
    this->bodyCollider.base.acFlags &= ~AC_HARD;

    if(this->inSecondPhase)
        Animation_MorphToPlayOnce(&this->skelAnime, gStalmaster_Anim4GuardJust, 0.0f);
    else
        Animation_MorphToPlayOnce(&this->skelAnime, gStalmaster_Anim2GuardJust, 0.0f);

    this->actor.speedXZ = 0.0f;
    this->actor.world.rot.y = this->actor.yawTowardsPlayer;
    EnStalmaster_BlowupArms(this, play);
    SoundEffect_StopOneshot(SOUND_STALMASTER_TELEGRAPH, &this->actor.focus.pos, this);
    SoundEffect_StopOneshot(SOUND_STALMASTER_GATHER, &this->actor.focus.pos, this);
    this->swordAActive = this->swordBActive = this->swordCActive = this->swordDActive = false;
}

void EnStalmaster_BlowArms(EnStalmaster* this, PlayState* play) {
    if(Animation_OnFrame(&this->skelAnime, Animation_GetLastFrame(this->skelAnime.animation)))
		EnStalmaster_SetupWalk(this,play);

    if(this->skelAnime.curFrame >= 45)
        EnStalmaster_GatherArms(this,play);
}

void EnStalmaster_SetupCounter(EnStalmaster* this, PlayState* play) {
    this->actionFunc = EnStalmaster_Counter;
    this->shouldUpdateHead = true;

    this->bodyCollider.base.acFlags &= ~AC_HARD;

    if(this->inSecondPhase)
        Animation_MorphToPlayOnce(&this->skelAnime, gStalmaster_Anim4Counter, -4.0f);
    else
        Animation_MorphToPlayOnce(&this->skelAnime, gStalmaster_Anim2Counter, -4.0f);

    this->actor.speedXZ = 0.0f;
    this->actor.world.rot.y = this->actor.yawTowardsPlayer;
    func_8003424C(play, &this->sStalmasterParts[STALMASTER_LIMB_HANDLU].skeletonPos);
    this->swordAActive = this->swordBActive = this->swordCActive = this->swordDActive = false;

    SoundEffect_StopOneshot(SOUND_STALMASTER_TELEGRAPH, &this->actor.focus.pos, this);
    SoundEffect_StopOneshot(SOUND_STALMASTER_GATHER, &this->actor.focus.pos, this);
    //This may look weird but its to stop the old attack sfx to "replace" them
    SoundEffect_PlayOneshot(SOUND_STALMASTER_TELEGRAPH, 1.0f, 0.8f, &this->actor.focus.pos, SFX_REVERB, SFX_DIST, this);
    SoundEffect_PlayOneshot(SOUND_STALMASTER_GATHER, 1.0f, 1.0f, &this->actor.focus.pos, SFX_REVERB, SFX_DIST, this);
}

void EnStalmaster_Counter(EnStalmaster* this, PlayState* play) {
    if(Animation_OnFrame(&this->skelAnime, Animation_GetLastFrame(this->skelAnime.animation)))
		EnStalmaster_SetupWalk(this,play);

    if(Animation_OnFrame(&this->skelAnime, 14.0f)){
        SoundEffect_PlayOneshot(SOUND_STALMASTER_SWING, 1.0f, 1.0f, &this->actor.focus.pos, SFX_REVERB, SFX_DIST, this);
        SoundEffect_StopOneshot(SOUND_STALMASTER_TELEGRAPH, &this->actor.focus.pos, this);
        SoundEffect_StopOneshot(SOUND_STALMASTER_GATHER, &this->actor.focus.pos, this);
        this->swordAActive = true;
        this->swordBActive = true;
        this->actor.speedXZ = 13.5f;
        if(this->inSecondPhase){
            this->swordCActive = true;
            this->swordDActive = true;
            this->actor.speedXZ = 16.5f;
        }
    }else if(Animation_OnFrame(&this->skelAnime, 20.0f)){
        this->swordAActive = false;
        this->swordBActive = false;
        this->swordCActive = false;
        this->swordDActive = false;
    }
    Math_ApproachF(&this->actor.speedXZ, 0.0f, 0.7f, 2.5f);
}

void EnStalmaster_SetupSecondPhase(EnStalmaster* this, PlayState* play) {
    this->actionFunc = EnStalmaster_SecondPhase;
    this->csState = STALMASTER_CUTSCENE_THREAT_WAIT;
    SoundEffect_StopOneshot(SOUND_STALMASTER_TELEGRAPH, &this->actor.focus.pos, this);
    SoundEffect_StopOneshot(SOUND_STALMASTER_GATHER, &this->actor.focus.pos, this);
    this->inSecondPhase = true;
    this->actor.speedXZ = 0.0f;
    this->swordAActive = this->swordBActive = this->swordCActive = this->swordDActive = false;
}

void EnStalmaster_SecondPhase(EnStalmaster* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    this->csTimer++;
    Camera* mainCam;

    EnStalmaster_GatherArms(this,play);

    switch (this->csState) {
        case STALMASTER_CUTSCENE_THREAT_WAIT:
            Animation_MorphToPlayOnce(&this->skelAnime, gStalmaster_AnimSwordPose, 0.0f);
            this->subCamEye.x = (Math_SinS(this->actor.world.rot.y + DEG_TO_BINANG(20)) * 200.0f) + this->actor.world.pos.x;
            this->subCamEye.y = this->actor.world.pos.y + 25;
            this->subCamEye.z = (Math_CosS(this->actor.world.rot.y + DEG_TO_BINANG(20)) * 200.0f) + this->actor.world.pos.z;

            this->subCamAt.x = this->actor.world.pos.x;
            this->subCamAt.y = this->actor.world.pos.y + 55;
            this->subCamAt.z = this->actor.world.pos.z;

            this->csState = STALMASTER_CUTSCENE_THREAT_DO;
            if(this->hasCS){
                func_80064520(play, &play->csCtx);
                func_8002DF54(play, &this->actor, 1);
                this->subCamId = Play_CreateSubCamera(play);
                Play_ChangeCameraStatus(play, CAM_ID_MAIN, CAM_STAT_WAIT);
                Play_ChangeCameraStatus(play, this->subCamId, CAM_STAT_ACTIVE);
                this->subCamFov = 70;
                this->csTimer = 0;

                this->enemyDummy = (EnDummyEnemy*)Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, DUMMYENEMY_ID,
                                                        this->actor.world.pos.x, this->actor.world.pos.y, this->actor.world.pos.z, 0, 0, 0, 0); //Used for clear flag, f oot
                Actor_ChangeCategory(play, &play->actorCtx, &this->actor, ACTORCAT_BOSS);
            }
        break;

        case STALMASTER_CUTSCENE_THREAT_DO:
            if(this->hasCS){
                player->actor.world.pos.x = (Math_SinS(this->actor.world.rot.y) * 155.0f) + this->actor.world.pos.x;;
                player->actor.world.pos.y = this->actor.world.pos.y;
                player->actor.world.pos.z = (Math_CosS(this->actor.world.rot.y) * 155.0f) + this->actor.world.pos.z;
            }

            if(this->skelAnime.curFrame >= 43)
                this->subCamFov += 0.25f;
            else
                this->subCamFov -= 0.15f;

            if(Animation_OnFrame(&this->skelAnime, 27.0f))
            {
                this->hasTakenWeaponry = true;
            }
            else if(Animation_OnFrame(&this->skelAnime, 28.0f))
            {
                Audio_PlayActorSfx2(&this->actor, NA_SE_IT_SWORD_PICKOUT);
            }
            else if(Animation_OnFrame(&this->skelAnime, 43.0f))
            {
                SoundEffect_PlayOneshot(SOUND_STALMASTER_SCREECH, 1.0f, 1.03f, &this->actor.focus.pos, SFX_REVERB, SFX_DIST, this);
                QuakeAndRumble(play, 8, 20, this->subCamId);
                if(this->hasCS)
                    func_8002DF54(play, &this->actor, 5);
            }
            else if(Animation_OnFrame(&this->skelAnime, 64.0f))
            {
                if(this->hasCS){
                    mainCam = Play_GetCamera(play, CAM_ID_MAIN);
                    mainCam->eye = this->subCamEye;
                    mainCam->eyeNext = this->subCamEye;
                    mainCam->at = this->subCamAt;
                    func_800C08AC(play, this->subCamId, 0);
                    this->subCamId = SUB_CAM_ID_DONE;
                    func_80064534(play, &play->csCtx);
                    this->actor.flags |= ACTORFLAG_CAN_TARGET;
                    func_8002DF54(play, &this->actor, 7);
                    Actor_ChangeCategory(play, &play->actorCtx, &this->actor, ACTORCAT_ENEMY);
                    if(this->enemyDummy != NULL)
                        Actor_Kill(&this->enemyDummy->actor);
                }
                EnStalmaster_SetupWalk(this,play);
            }
        break;
    }

    if (this->subCamId != SUB_CAM_ID_DONE){
        Play_CameraSetAtEye(play, this->subCamId, &this->subCamAt, &this->subCamEye);
        Play_CameraSetFov(play, this->subCamId, this->subCamFov);
    }
}

void EnStalmaster_SetupDeath(EnStalmaster* this, PlayState* play) {
    this->actionFunc = EnStalmaster_Death;
    this->csState = STALMASTER_CUTSCENE_DIE_WAIT;
    SoundEffect_StopOneshot(SOUND_STALMASTER_TELEGRAPH, &this->actor.focus.pos, this);
    SoundEffect_StopOneshot(SOUND_STALMASTER_GATHER, &this->actor.focus.pos, this);
    this->actor.speedXZ = 0.0f;
    func_800F5B58();
    Audio_QueueSeqCmd(NA_BGM_STOP);
    Flags_SetSwitch(play, this->switchFlags);
    this->swordAActive = this->swordBActive = this->swordCActive = this->swordDActive = false;
    this->actor.flags &= ~ACTORFLAG_CAN_TARGET;

    SoundEffect_PlayOneshot(SOUND_STALMASTER_DIE, 1.0f, 1.0f, &this->actor.focus.pos, SFX_REVERB, SFX_DIST, this);
}

void EnStalmaster_Death(EnStalmaster* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    static const float stepFrames[] = {26.0f, 30.0f, 34.0f, 37.0f, 41.0f, 45.0f, 49.0f, 53.0f, 57.0f};
            size_t i;

    this->csTimer++;
    Camera* mainCam;

    switch (this->csState) {
        case STALMASTER_CUTSCENE_DIE_WAIT:
            Animation_MorphToPlayOnce(&this->skelAnime, gStalmaster_AnimEnd, 0.0f);
            this->subCamEye.x = (Math_SinS(this->actor.world.rot.y + DEG_TO_BINANG(10)) * 270.0f) + this->actor.world.pos.x;
            this->subCamEye.y = this->actor.world.pos.y + 40;
            this->subCamEye.z = (Math_CosS(this->actor.world.rot.y + DEG_TO_BINANG(10)) * 270.0f) + this->actor.world.pos.z;

            this->subCamAt.x = this->actor.world.pos.x;
            this->subCamAt.y = this->actor.world.pos.y + 80;
            this->subCamAt.z = this->actor.world.pos.z;

            this->csState = STALMASTER_CUTSCENE_DIE_DO;
            if(this->hasCS){
                func_80064520(play, &play->csCtx);
                func_8002DF54(play, &this->actor, 1);
                this->subCamId = Play_CreateSubCamera(play);
                Play_ChangeCameraStatus(play, CAM_ID_MAIN, CAM_STAT_WAIT);
                Play_ChangeCameraStatus(play, this->subCamId, CAM_STAT_ACTIVE);
                this->subCamFov = 70;
                this->csTimer = 0;

                player->actor.world.pos.x = (Math_SinS(this->actor.world.rot.y) * 155.0f) + this->actor.world.pos.x;
                player->actor.world.pos.y = this->actor.world.pos.y;
                player->actor.world.pos.z = (Math_CosS(this->actor.world.rot.y) * 155.0f) + this->actor.world.pos.z;

                this->enemyDummy = (EnDummyEnemy*)Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, DUMMYENEMY_ID,
                                                        this->actor.world.pos.x, this->actor.world.pos.y, this->actor.world.pos.z, 0, 0, 0, 0); //Used for clear flag, f oot
                Actor_ChangeCategory(play, &play->actorCtx, &this->actor, ACTORCAT_BOSS);
            }

            EnStalmaster_BlowupDeath(this,play);
        break;

        case STALMASTER_CUTSCENE_DIE_DO:

            if(this->skelAnime.curFrame < 85)
                this->subCamFov -= 0.15f;

            for (i = 0; i < sizeof(stepFrames) / sizeof(stepFrames[0]); ++i) {
                if (Animation_OnFrame(&this->skelAnime, stepFrames[i])) {
                    SoundEffect_PlayOneshot(SOUND_STALMASTER_STEP, 0.2f, Rand_CenteredFloat(0.2f) + 1.2f, &this->actor.shape.feetPos[1], SFX_REVERB, SFX_DIST, this);
                    break;
                }
            }

            Math_ApproachF(&this->subCamAt.x, this->sStalmasterParts[STALMASTER_LIMB_BACKBONE01].skeletonPos.x, 0.2f, 100.0f);
            Math_ApproachF(&this->subCamAt.y, this->sStalmasterParts[STALMASTER_LIMB_BACKBONE01].skeletonPos.y + 5.0f, 0.2f, 100.0f);
            Math_ApproachF(&this->subCamAt.z, this->sStalmasterParts[STALMASTER_LIMB_BACKBONE01].skeletonPos.z, 0.2f, 100.0f);

            if(Animation_OnFrame(&this->skelAnime, 61.0f))
            {
                SoundEffect_PlayOneshot(SOUND_STALMASTER_ROLL, 1.0f, 1.0f, &this->actor.focus.pos, SFX_REVERB, SFX_DIST, this);
            }

            if(Animation_OnFrame(&this->skelAnime, 89.0f))
            {
                if(this->hasCS){
                    mainCam = Play_GetCamera(play, CAM_ID_MAIN);
                    mainCam->eye = this->subCamEye;
                    mainCam->eyeNext = this->subCamEye;
                    mainCam->at = this->subCamAt;
                    func_800C08AC(play, this->subCamId, 0);
                    this->subCamId = SUB_CAM_ID_DONE;
                    func_80064534(play, &play->csCtx);
                    func_8002DF54(play, &this->actor, 7);
                    Audio_QueueSeqCmd(SEQ_PLAYER_BGM_MAIN << 24 | NA_BGM_BOSS_CLEAR);
                    Actor_Spawn(&play->actorCtx, play, ACTOR_DOOR_WARP1,  this->actor.world.pos.x,  this->actor.world.pos.y,
                     this->actor.world.pos.z, 0, 0, 0, 0x53);
                    if(this->enemyDummy != NULL)
                        Actor_Kill(&this->enemyDummy->actor);
                }
                
                Actor_ChangeCategory(play, &play->actorCtx, &this->actor, ACTORCAT_PROP);

                //Actor_Kill(&this->actor); //this keep the corpse if off
                
            }
        break;
    }

    if (this->subCamId != SUB_CAM_ID_DONE){
        Play_CameraSetAtEye(play, this->subCamId, &this->subCamAt, &this->subCamEye);
        Play_CameraSetFov(play, this->subCamId, this->subCamFov);
    }
}

//This is responsible for the damages and knockover
void EnStalmaster_UpdateColliderDamage(EnStalmaster* this, PlayState* play, ColliderJntSph collider) {
    Player* player = GET_PLAYER(play);

    if (collider.base.atFlags & AT_HIT) {
        collider.base.atFlags &= ~AT_HIT;
        player->pushedYaw = this->actor.yawTowardsPlayer;
        player->pushedSpeed = 15.0f;
    }
}

//Handles the sword collisions and the main body ones
void EnStalmaster_UpdateCollisions(EnStalmaster* this, PlayState* play) {
    if (this->actionFunc == EnStalmaster_Introduction || this->actionFunc == EnStalmaster_Death) {
        return;
    }

    Player* player = GET_PLAYER(play);
    int acFlags = this->bodyCollider.base.acFlags;
    int damageEffect = this->actor.colChkInfo.damageEffect;
    int health = this->actor.colChkInfo.health;
    int meleeWeaponAnimation = player->meleeWeaponAnimation;

    // If hit, handle damage effects
    if (acFlags & AC_HIT) {
        this->bodyCollider.base.acFlags &= ~AC_HIT; // Clear the hit flag

        switch (damageEffect) {
            case STALMASTER_DMGEFF_EXPLOSIVE:
                if (this->actionFunc != EnStalmaster_BlowArms) {
                    EnStalmaster_SetupBlowArms(this, play);
                }
                break;

            case STALMASTER_DMGEFF_PARRY:
                if(!this->armBlowed)
                    EnStalmaster_SetupGuard(this, play);
                break;

            case STALMASTER_DMGEFF_SPIN:
            case STALMASTER_DMGEFF_NORMAL:
                if (damageEffect == STALMASTER_DMGEFF_SPIN && this->actionFunc == EnStalmaster_Counter) {
                    EnStalmaster_SetupGuard(this, play);
                } else {
                    if (this->actionFunc == EnStalmaster_Walk || this->actionFunc == EnStalmaster_Guard) {
                        EnStalmaster_SetupCounter(this, play);
                    } else {
                        this->dmgFogEffectTimer = 5;

                        if (!this->helmetOff && health <= 15) {
                            EnStalmaster_BlowupHelmet(this, play);
                        }

                        if (!this->plateOff && health <= 30) {
                            EnStalmaster_BlowupPlate(this, play);
                        }

                        // Rotate torso based on player's weapon animation
                        switch (meleeWeaponAnimation) {
                            case PLAYER_MWA_FORWARD_SLASH_1H:
                                this->torsoRotX = DEG_TO_BINANG(60);
                                break;

                            case PLAYER_MWA_FORWARD_SLASH_2H:
                            case PLAYER_MWA_JUMPSLASH_START:
                            case PLAYER_MWA_FLIPSLASH_START:
                            case PLAYER_MWA_JUMPSLASH_FINISH:
                            case PLAYER_MWA_FLIPSLASH_FINISH:
                                this->torsoRotX = DEG_TO_BINANG(80);
                                break;

                            case PLAYER_MWA_SPIN_ATTACK_1H:
                            case PLAYER_MWA_SPIN_ATTACK_2H:
                            case PLAYER_MWA_BIG_SPIN_1H:
                            case PLAYER_MWA_BIG_SPIN_2H:
                                this->torsoRotX = -DEG_TO_BINANG(50);
                                this->torsoRotZ = -DEG_TO_BINANG(50);
                                break;

                            case PLAYER_MWA_LEFT_SLASH_1H:
                            case PLAYER_MWA_LEFT_COMBO_1H:
                            case PLAYER_MWA_LEFT_SLASH_2H:
                            case PLAYER_MWA_LEFT_COMBO_2H:
                                this->torsoRotZ = -DEG_TO_BINANG(50);
                                break;

                            case PLAYER_MWA_RIGHT_SLASH_1H:
                            case PLAYER_MWA_RIGHT_COMBO_1H:
                            case PLAYER_MWA_RIGHT_SLASH_2H:
                            case PLAYER_MWA_RIGHT_COMBO_2H:
                                this->torsoRotZ = DEG_TO_BINANG(50);
                                break;

                            default:
                                this->torsoRotX = -DEG_TO_BINANG(50);
                                break;
                        }

                        Actor_ApplyDamage(&this->actor);
                        SoundEffect_PlayOneshot(SOUND_SLASH_HEAVY, 1.0f, 1.0f, &this->actor.world.pos, SFX_REVERB, SFX_DIST, this);
                    }
                }
                break;
        }
    }

    Collider_UpdateCylinder(&this->actor, &this->bodyCollider);

    if (!this->dmgFogEffectTimer) {
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->bodyCollider.base);
    }

    CollisionCheck_SetOC(play, &play->colChkCtx, &this->bodyCollider.base);

    if (this->actionFunc != EnStalmaster_BlowArms) {
        if (this->swordAActive) {
            EnStalmaster_UpdateColliderDamage(this, play, this->colliderSwordUpLeft);
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->colliderSwordUpLeft.base);
        }

        if (this->swordBActive) {
            EnStalmaster_UpdateColliderDamage(this, play, this->colliderSwordUpRight);
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->colliderSwordUpRight.base);
        }

        if (this->inSecondPhase) {
            if (this->swordCActive) {
                EnStalmaster_UpdateColliderDamage(this, play, this->colliderSwordDownRight);
                CollisionCheck_SetAT(play, &play->colChkCtx, &this->colliderSwordDownRight.base);
            }

            if (this->swordDActive) {
                EnStalmaster_UpdateColliderDamage(this, play, this->colliderSwordDownLeft);
                CollisionCheck_SetAT(play, &play->colChkCtx, &this->colliderSwordDownLeft.base);
            }

            CollisionCheck_SetAC(play, &play->colChkCtx, &this->colliderSwordDownLeft.base);
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->colliderSwordDownRight.base);
        }

        // These are always active
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->colliderSwordUpLeft.base);
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->colliderSwordUpRight.base);
    }
}


void EnStalmaster_UpdateTorsoRot(EnStalmaster* this, PlayState* play) {  
    Math_ApproachZeroF(&this->torsoRotX, 1.5f, 900.0f);
    Math_ApproachZeroF(&this->torsoRotZ, 1.5f, 900.0f);
}

void EnStalmaster_Update(EnStalmaster* this, PlayState* play) {
    this->actionFunc(this, play);
    SkelAnime_Update(&this->skelAnime);
    
    EnStalmaster_UpdateHeadRotation(this, play);
    EnStalmaster_UpdateTail(this, play);
    EnStalmaster_UpdateCollisions(this, play);
    EnStalmaster_UpdateTorsoRot(this, play);
    EnStalmaster_UpdateEffects(this, play);
    EnStalmaster_UpdateParts(this, play);

    Actor_MoveForward(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 50.0f, 100.0f, 100.0f, UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2);

    DECR(this->dmgFogEffectTimer);
    DECR(this->commonTimer);

    this->actor.shape.rot.y = this->actor.world.rot.y;

    if(!this->inSecondPhase){
        if(this->actor.colChkInfo.health <= STALMASTER_HEALTH*STALMASTER_HEALTH_RATIO){
            EnStalmaster_SetupSecondPhase(this, play);
        }
    }else{
        if(this->actor.colChkInfo.health <= 0 && this->actionFunc != EnStalmaster_Death){
            EnStalmaster_SetupDeath(this, play);
        }
    }
}

void EnStalmaster_Destroy(EnStalmaster* this, PlayState* play) {
    SkelAnime_Free(&this->skelAnime, play);
}

s32 EnStalmaster_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnStalmaster* this = (void*)thisx;
    
    if (limbIndex == STALMASTER_LIMB_HEAD) {
        rot->x += this->headRot.x;
        rot->y += this->headRot.y;
        rot->z += this->headRot.z;
        if(!this->sStalmasterParts[STALMASTER_LIMB_HEAD].invisible)
            *dl = NULL;
    }else if (limbIndex == STALMASTER_LIMB_CHIN) {
        if(!this->sStalmasterParts[STALMASTER_LIMB_HEAD].invisible)
            *dl = NULL;
    }else if (limbIndex == STALMASTER_LIMB_BACKBONE02) {
        if(!this->sStalmasterParts[STALMASTER_LIMB_BACKBONE02].invisible)
            *dl = NULL;
    }else if (limbIndex == STALMASTER_LIMB_HANDRD || limbIndex == STALMASTER_LIMB_FINGERRD01 || limbIndex == STALMASTER_LIMB_FINGERRD02) {
        if(!this->sStalmasterParts[STALMASTER_LIMB_ARMRD02].invisible)
            *dl = NULL;
    }else if (limbIndex == STALMASTER_LIMB_HANDRU || limbIndex == STALMASTER_LIMB_FINGERRU01 || limbIndex == STALMASTER_LIMB_FINGERRU02) {
        if(!this->sStalmasterParts[STALMASTER_LIMB_ARMRU02].invisible)
            *dl = NULL;
    }else if (limbIndex == STALMASTER_LIMB_HANDLD || limbIndex == STALMASTER_LIMB_FINGERLD01 || limbIndex == STALMASTER_LIMB_FINGERLD02) {
        if(!this->sStalmasterParts[STALMASTER_LIMB_ARMLD02].invisible)
            *dl = NULL;
    }else if (limbIndex == STALMASTER_LIMB_HANDLU || limbIndex == STALMASTER_LIMB_FINGERLU01 || limbIndex == STALMASTER_LIMB_FINGERLU02) {
        if(!this->sStalmasterParts[STALMASTER_LIMB_ARMLU02].invisible)
            *dl = NULL;
    }else if (limbIndex == STALMASTER_LIMB_BACKBONE01) {
        rot->x += this->torsoRotX;
        rot->z += this->torsoRotZ;
    }

    if(!this->sStalmasterParts[limbIndex].invisible)
        *dl = NULL;
    return 0;
}

void EnStalmaster_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3s* rot, void* thisx) {
    MtxF mf;
    EnStalmaster* this = (void*)thisx;

    static Vec3f tailOffsets[] = {
        { 0.0f, 40.0f, 0.0f },
        { -8.0f, 60.0f, 8.0f },
        { -24.0f, 80.0f, 25.0f },
        { -40.0f, 100.0f, 34.0f }
    };
    static Vec3f headHandOffset = { 5.0f, -19.0f, 0.0f };

    StalmasterParts* limbInfo = &this->sStalmasterParts[limbIndex];
    
    if (limbIndex == STALMASTER_LIMB_HEAD){
        if(limbInfo->invisible){
            for (int i = 0; i < 4; i++) {
                Matrix_MultVec3f(&tailOffsets[i], &this->tailPos[i]);
            }
            Matrix_MultVec3f(&gZeroVec, &this->actor.focus.pos);
            if(!this->helmetOff){
                gSPMatrix(POLY_OPA_DISP++, NEW_MATRIX(),
                                    G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPDisplayList(POLY_OPA_DISP++, gStalmaster_DlHelmet);
            }
        }else{
            this->actor.focus.pos = limbInfo->pos;
        }
    }else if (limbIndex == STALMASTER_LIMB_BACKBONE02){
        if(limbInfo->invisible && !this->plateOff){
            gSPMatrix(POLY_OPA_DISP++, NEW_MATRIX(),
                                G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_OPA_DISP++, gStalmaster_DlChestplate);
        }
    }else if (limbIndex == STALMASTER_LIMB_LOC_SWORDA){
        if(limbInfo->invisible){
            gSPMatrix(POLY_OPA_DISP++, NEW_MATRIX(),
                                G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_OPA_DISP++, gStalmaster_DlSwordA);
        }
    }else if (limbIndex == STALMASTER_LIMB_LOC_SWORDB){
        if(limbInfo->invisible){
            gSPMatrix(POLY_OPA_DISP++, NEW_MATRIX(),
                                G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_OPA_DISP++, gStalmaster_DlSwordB);
        }
    }else if (limbIndex == STALMASTER_LIMB_LOC_SWORDC2){
        if(limbInfo->invisible && !this->hasTakenWeaponry){
            gSPMatrix(POLY_OPA_DISP++, NEW_MATRIX(),
                                G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_OPA_DISP++, gStalmaster_DlSwordC2);
        }
    }else if (limbIndex == STALMASTER_LIMB_LOC_SWORDD2){
        if(limbInfo->invisible && !this->hasTakenWeaponry){
            gSPMatrix(POLY_OPA_DISP++, NEW_MATRIX(),
                                G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_OPA_DISP++, gStalmaster_DlSwordD2);
        }
    }else if (limbIndex == STALMASTER_LIMB_HANDLD){
        Matrix_MultVec3f(&headHandOffset, &this->headHandPos);
    }else if (limbIndex == STALMASTER_LIMB_FOOTL){
        Matrix_MultVec3f(&gZeroVec,&this->actor.shape.feetPos[0]);
    }else if (limbIndex == STALMASTER_LIMB_FOOTR){
        Matrix_MultVec3f(&gZeroVec,&this->actor.shape.feetPos[1]);
    }else if (limbIndex == STALMASTER_LIMB_LOC_SWORDC){
        if(limbInfo->invisible && this->hasTakenWeaponry){
            gSPMatrix(POLY_OPA_DISP++, NEW_MATRIX(),
                                G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_OPA_DISP++, gStalmaster_DlSwordC);
        }
    }else if (limbIndex == STALMASTER_LIMB_LOC_SWORDD){
        if(limbInfo->invisible && this->hasTakenWeaponry){
            gSPMatrix(POLY_OPA_DISP++, NEW_MATRIX(),
                                G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_OPA_DISP++, gStalmaster_DlSwordD);
        }
    }

    Matrix_Get(&mf);

    VEC_SET(limbInfo->skeletonPos, mf.xw, mf.yw, mf.zw);
    Matrix_MtxFToZYXRotS(&mf, &limbInfo->skeletonRot, 1);

    //Blures pos:
    Vec3f blureA;
	Vec3f blureB;

	static Vec3f vecSwordULOffset = { 10.0f, 0.0f, -20.0f };
	static Vec3f vecSwordULEffectOffset = { 25.0f, 0.0f, 125.0f };

	static Vec3f vecSwordUROffset = { -10.0f, 0.0f, -20.0f };
	static Vec3f vecSwordUREffectOffset = { -25.0f, 0.0f, 125.0f };

    static Vec3f vecSwordDROffset = { -10.0f, 0.0f, -20.0f };
	static Vec3f vecSwordDREffectOffset = { -25.0f, 0.0f, 130.0f };

    static Vec3f vecSwordDLOffset = { 10.0f, 0.0f, -20.0f };
	static Vec3f vecSwordDLEffectOffset = { 25.0f, 0.0f, 125.0f };

    if (limbIndex == STALMASTER_LIMB_HANDRU) {
		Matrix_MultVec3f(&vecSwordUROffset, &blureA);
		Matrix_MultVec3f(&vecSwordUREffectOffset, &blureB);
		if(this->swordBActive)
			EffectBlure_AddVertex(Effect_GetByIndex(this->effectIndex2), &blureB, &blureA);
		else
			EffectBlure_AddSpace(Effect_GetByIndex(this->effectIndex2));

        Collider_UpdateSpheres(limbIndex, &this->colliderSwordUpRight);
	}else if (limbIndex == STALMASTER_LIMB_HANDLU) {
		Matrix_MultVec3f(&vecSwordULOffset, &blureA);
		Matrix_MultVec3f(&vecSwordULEffectOffset, &blureB);
		if(this->swordAActive)
			EffectBlure_AddVertex(Effect_GetByIndex(this->effectIndex), &blureB, &blureA);
		else
			EffectBlure_AddSpace(Effect_GetByIndex(this->effectIndex));

        Collider_UpdateSpheres(limbIndex, &this->colliderSwordUpLeft);
	}else if (limbIndex == STALMASTER_LIMB_HANDRD) {
		Matrix_MultVec3f(&vecSwordDROffset, &blureA);
		Matrix_MultVec3f(&vecSwordDREffectOffset, &blureB);
		if(this->swordCActive)
			EffectBlure_AddVertex(Effect_GetByIndex(this->effectIndex1), &blureB, &blureA);
		else
			EffectBlure_AddSpace(Effect_GetByIndex(this->effectIndex1));

        Collider_UpdateSpheres(limbIndex, &this->colliderSwordDownRight);
	}else if (limbIndex == STALMASTER_LIMB_HANDLD) {
		Matrix_MultVec3f(&vecSwordDLOffset, &blureA);
		Matrix_MultVec3f(&vecSwordDLEffectOffset, &blureB);
		if(this->swordDActive)
			EffectBlure_AddVertex(Effect_GetByIndex(this->effectIndex3), &blureB, &blureA);
		else
			EffectBlure_AddSpace(Effect_GetByIndex(this->effectIndex3));

        Collider_UpdateSpheres(limbIndex, &this->colliderSwordDownLeft);
	}
}

static void DrawTail(Particle* particle, s16 roll, PlayState* play, EnStalmaster* this, s32 segment) {
	Particle* next = particle + 1;
	s16 yaw;
	s16 pitch;

	yaw = Math_Vec3f_Yaw(&particle->pos, &next->pos);
	pitch = Math_Vec3f_Pitch(&particle->pos, &next->pos);

    OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
        Gfx_SetupDL_25Opa(__gfxCtx);
	    Matrix_Translate(particle->pos.x, particle->pos.y, particle->pos.z, MTXMODE_NEW);
        Matrix_RotateY_s(yaw, MTXMODE_APPLY);
        Matrix_RotateX_s(pitch, MTXMODE_APPLY);
        Matrix_RotateZ_s(roll, MTXMODE_APPLY);
        Matrix_Scale(this->actor.scale.x, this->actor.scale.y, this->actor.scale.z, MTXMODE_APPLY);
        gSPMatrix(POLY_OPA_DISP++, NEW_MATRIX(), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        if(segment == this->numTail-1)
            gSPDisplayList(POLY_OPA_DISP++, gStalmaster_DlTip);
        else
            gSPDisplayList(POLY_OPA_DISP++, gStalmaster_DlTail);
    CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
}

void EnStalmaster_Draw(EnStalmaster* this, PlayState* play) {
    OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

    Gfx_SetupDL_25Opa(play->state.gfxCtx);

    if ((this->dmgFogEffectTimer % 2) != 0)
        POLY_OPA_DISP = Gfx_SetFog(POLY_OPA_DISP, 255, 0, 0, 255, 900, 1099);

	SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount, EnStalmaster_OverrideLimbDraw, EnStalmaster_PostLimbDraw, this);

    POLY_OPA_DISP = Play_SetFog(play, POLY_OPA_DISP);

    if(!this->helmetOff){
        for (int i = 0; i < this->numTail; i++)
            DrawTail(&this->particleTail[i], this->headRot.z, play, this, i);
    }

    EnStalmaster_DrawEffects(this,play);
    EnStalmaster_DrawLimbs(this,play);
    
    CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
}

void EnStalmaster_DrawEffects(EnStalmaster* this, PlayState* play) {
    s16 i;

    OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

    Gfx_SetupDL_25Opa(play->state.gfxCtx);

    Matrix_Push();

    for (i = 0; i < ARRAY_COUNT(this->sStalmasterEffects); i++) {
        StalmasterParts* effect = &this->sStalmasterEffects[i];

        if (effect->type == STALMASTER_EFFECT_NONE) {
            continue;
        }

        Matrix_Translate(effect->pos.x, effect->pos.y, effect->pos.z, MTXMODE_NEW);
        Matrix_RotateZ(BINANG_TO_RAD(effect->rot.z), MTXMODE_APPLY);
        Matrix_RotateY(BINANG_TO_RAD(effect->rot.y), MTXMODE_APPLY);
        Matrix_RotateX(BINANG_TO_RAD(effect->rot.x), MTXMODE_APPLY);
        Matrix_Scale(this->actor.scale.x, this->actor.scale.y, this->actor.scale.z, MTXMODE_APPLY);

        gSPMatrix(POLY_OPA_DISP++, NEW_MATRIX(), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        switch (effect->type) {
            case STALMASTER_EFFECT_HELMET:
                gSPDisplayList(POLY_OPA_DISP++, gStalmaster_DlHelmet);
                break;
            case STALMASTER_EFFECT_TAIL:
                gSPDisplayList(POLY_OPA_DISP++, gStalmaster_DlTail);
                break;
            case STALMASTER_EFFECT_TIP:
                gSPDisplayList(POLY_OPA_DISP++, gStalmaster_DlTip);
                break;
            case STALMASTER_EFFECT_PLATE:
                gSPDisplayList(POLY_OPA_DISP++, gStalmaster_DlChestplate);
                break;
        }
    }

    Matrix_Pop();
    
    CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
}


void EnStalmaster_DrawLimbs(EnStalmaster* this, PlayState* play) {
    s16 i;

    OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

    Gfx_SetupDL_25Opa(play->state.gfxCtx);

    Matrix_Push();

    for (i = 0; i < ARRAY_COUNT(this->sStalmasterParts); i++) {
        StalmasterParts* part = &this->sStalmasterParts[i];

        if (part->invisible) {
            continue;
        }

        Matrix_Translate(part->pos.x, part->pos.y, part->pos.z, MTXMODE_NEW);
        Matrix_RotateZ(BINANG_TO_RAD(part->rot.z), MTXMODE_APPLY);
        Matrix_RotateY(BINANG_TO_RAD(part->rot.y), MTXMODE_APPLY);
        Matrix_RotateX(BINANG_TO_RAD(part->rot.x), MTXMODE_APPLY);
        Matrix_Scale(this->actor.scale.x, this->actor.scale.y, this->actor.scale.z, MTXMODE_APPLY);

        gSPMatrix(POLY_OPA_DISP++, NEW_MATRIX(), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        switch (i) {
            case STALMASTER_LIMB_HEAD:
                gSPDisplayList(POLY_OPA_DISP++, gStalmaster_DlHead);
                if (!this->helmetOff) {
                    gSPDisplayList(POLY_OPA_DISP++, gStalmaster_DlHelmet);
                }
                break;
            case STALMASTER_LIMB_BACKBONE02:
                gSPDisplayList(POLY_OPA_DISP++, gStalmaster_DlBackbone);
                if (!this->plateOff) {
                    gSPDisplayList(POLY_OPA_DISP++, gStalmaster_DlChestplate);
                }
                break;
            case STALMASTER_LIMB_ARMRD01:
            case STALMASTER_LIMB_ARMRU01:
                gSPDisplayList(POLY_OPA_DISP++, gStalmaster_DlArmR);
                break;
            case STALMASTER_LIMB_ARMRD02:
            case STALMASTER_LIMB_ARMRU02:
                gSPDisplayList(POLY_OPA_DISP++, gStalmaster_DlForearmR);
                break;
            case STALMASTER_LIMB_PADRD:
            case STALMASTER_LIMB_PADRU:
                gSPDisplayList(POLY_OPA_DISP++, gStalmaster_DlPadR);
                break;
            case STALMASTER_LIMB_ARMLD01:
            case STALMASTER_LIMB_ARMLU01:
                gSPDisplayList(POLY_OPA_DISP++, gStalmaster_DlArmL);
                break;
            case STALMASTER_LIMB_ARMLD02:
            case STALMASTER_LIMB_ARMLU02:
                gSPDisplayList(POLY_OPA_DISP++, gStalmaster_DlForearmL);
                break;
            case STALMASTER_LIMB_PADLD:
            case STALMASTER_LIMB_PADLU:
                gSPDisplayList(POLY_OPA_DISP++, gStalmaster_DlPadL);
                break;
            case STALMASTER_LIMB_LOC_SWORDA:
                gSPDisplayList(POLY_OPA_DISP++, gStalmaster_DlSwordA);
                break;
            case STALMASTER_LIMB_LOC_SWORDB:
                gSPDisplayList(POLY_OPA_DISP++, gStalmaster_DlSwordB);
                break;
            case STALMASTER_LIMB_LOC_SWORDC:
                if (this->inSecondPhase)
                    gSPDisplayList(POLY_OPA_DISP++, gStalmaster_DlSwordC);
                break;
            case STALMASTER_LIMB_LOC_SWORDC2:
                gSPDisplayList(POLY_OPA_DISP++, gStalmaster_DlSwordC);
                break;
            case STALMASTER_LIMB_LOC_SWORDD:
                if (this->inSecondPhase)
                    gSPDisplayList(POLY_OPA_DISP++, gStalmaster_DlSwordD);
                break;
            case STALMASTER_LIMB_LOC_SWORDD2:
                gSPDisplayList(POLY_OPA_DISP++, gStalmaster_DlSwordD);
                break;
        }
    }

    Matrix_Pop();
    
    CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
}

