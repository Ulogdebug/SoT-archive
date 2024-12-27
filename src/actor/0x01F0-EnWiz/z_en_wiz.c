/*
 * File: z_En_Wiz.c
 * Overlay: ovl_En_Wiz
 * Description: Wizzrobe enemy from the Hylian Modding actor pack.
 * Params: 0xXXYY, XX is type, YY is the enemy spawn set
 *      -- XX: Type - 0x00 Master (Spawns Enemies), 0x01 Fire, 0x02 Ice
 *      -- YY: Enemy Spawn Set - see sEnemySpawnTable
 * 
 * Spawn position should be in the center of the room!
 * Spawn parameters set the enemy spawn table, sEnemySpawnTable
 */

#include "z_en_wiz.h"
#include <uLib.h>

#define FLAGS (ACTOR_FLAG_0 | ACTOR_FLAG_2 | ACTOR_FLAG_4)


#define COLORFILTER_COLORFLAG_GRAY 0x8000
#define COLORFILTER_COLORFLAG_RED  0x4000
#define COLORFILTER_COLORFLAG_BLUE 0x0000

#define COLORFILTER_INTENSITY_FLAG 0x8000

#define COLORFILTER_BUFFLAG_XLU    0x2000
#define COLORFILTER_BUFFLAG_OPA    0x0000


typedef enum {
    /*  0 */ CASTTYPE_FIRE,
    /*  1 */ CASTTYPE_NONE,
    /*  2 */ CASTTYPE_ENEMY,
    /*  3 */ CASTTYPE_ICE = 4,
    /*  4 */ CASTTYPE_ELECTRIC = 5,
} CastType;

#define TARGET_ON (this->actor.flags |= ACTOR_FLAG_0)
#define TARGET_OFF (this->actor.flags &= ~ACTOR_FLAG_0)

// uncomment this to only run DoNothing
// #define TEST

void EnWiz_Init(Actor* thisx, PlayState* play);
void EnWiz_InitCollision(Actor* thisx, PlayState* play);
void EnWiz_Destroy(Actor* thisx, PlayState* play2);
void EnWiz_Update(Actor* thisx, PlayState* play);
void EnWiz_Draw(Actor* thisx, PlayState* play);

void EnWiz_SetupAppear(EnWiz* this);
void EnWiz_SetupCastAttack(EnWiz* this);
void EnWiz_SetupCastEnemies(EnWiz* this);
void EnWiz_SetupAttack(EnWiz* this);
void EnWiz_SetupDisappear(EnWiz* this);
void EnWiz_SetupIdle(EnWiz* this);
void EnWiz_SetupDamaged(EnWiz* this);
void EnWiz_SetupFrozen(Actor* thisx);
void EnWiz_SetupDeath(EnWiz* this);
void EnWiz_SetupTeleport(EnWiz* this, PlayState* play);

void EnWiz_Appear(EnWiz* this, PlayState* play);
void EnWiz_CastAttack(EnWiz* this, PlayState* play);
void EnWiz_CastEnemies(EnWiz* this, PlayState* play);
void EnWiz_Attack(EnWiz* this, PlayState* play);
void EnWiz_Disappear(EnWiz* this, PlayState* play);
void EnWiz_Idle(EnWiz* this, PlayState* play);
void EnWiz_Damaged(EnWiz* this, PlayState* play);
void EnWiz_Frozen(EnWiz* this, PlayState* play);
void EnWiz_Death(EnWiz* this, PlayState* play);
void EnWiz_Teleport(EnWiz* this, PlayState* play);

void EnWiz_DoNothing(EnWiz* this, PlayState* play);

ActorInit En_Wiz_InitVars = {
    0x01F0,
    ACTORCAT_ENEMY,
    FLAGS,
    0x0198,
    sizeof(EnWiz),
    (ActorFunc)EnWiz_Init,
    (ActorFunc)EnWiz_Destroy,
    (ActorFunc)EnWiz_Update,
    (ActorFunc)EnWiz_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_HIT0,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 55, 70, 0, { 0 } },
};

typedef enum {
    /* 0x0 */ EN_WIZ_DMGEFF_NORMAL,     // Deals no damage
    /* 0x1 */ EN_WIZ_DMGEFF_IMMUNE,     // Deals no damage
              EN_WIZ_DMGEFF_EXTRA,      // Deals extra damage when frozen
    /* 0xE */ EN_WIZ_DMGEFF_FIRE = 0xE, // Damages and sets Ice Wizrobes on fire
    /* 0xF */ EN_WIZ_DMGEFF_FREEZE     // Damages and surrounds Fire Wizrobes with ice
} EnWizDamageEffect;

static DamageTable sDamageTable[] = {
    /* Deku nut      */ DMG_ENTRY(0, EN_WIZ_DMGEFF_IMMUNE),
    /* Deku stick    */ DMG_ENTRY(2, EN_WIZ_DMGEFF_NORMAL),
    /* Slingshot     */ DMG_ENTRY(1, EN_WIZ_DMGEFF_NORMAL),
    /* Explosive     */ DMG_ENTRY(2, EN_WIZ_DMGEFF_EXTRA),
    /* Boomerang     */ DMG_ENTRY(0, EN_WIZ_DMGEFF_IMMUNE),
    /* Normal arrow  */ DMG_ENTRY(2, EN_WIZ_DMGEFF_NORMAL),
    /* Hammer swing  */ DMG_ENTRY(2, EN_WIZ_DMGEFF_EXTRA),
    /* Hookshot      */ DMG_ENTRY(0, EN_WIZ_DMGEFF_IMMUNE),
    /* Kokiri sword  */ DMG_ENTRY(2, EN_WIZ_DMGEFF_NORMAL),
    /* Master sword  */ DMG_ENTRY(2, EN_WIZ_DMGEFF_NORMAL),
    /* Giant's Knife */ DMG_ENTRY(4, EN_WIZ_DMGEFF_NORMAL),
    /* Fire arrow    */ DMG_ENTRY(2, EN_WIZ_DMGEFF_FIRE),
    /* Ice arrow     */ DMG_ENTRY(2, EN_WIZ_DMGEFF_FREEZE),
    /* Light arrow   */ DMG_ENTRY(2, EN_WIZ_DMGEFF_NORMAL),
    /* Unk arrow 1   */ DMG_ENTRY(2, EN_WIZ_DMGEFF_NORMAL),
    /* Unk arrow 2   */ DMG_ENTRY(2, EN_WIZ_DMGEFF_NORMAL),
    /* Unk arrow 3   */ DMG_ENTRY(2, EN_WIZ_DMGEFF_NORMAL),
    /* Fire magic    */ DMG_ENTRY(2, EN_WIZ_DMGEFF_FIRE),
    /* Ice magic     */ DMG_ENTRY(2, EN_WIZ_DMGEFF_FREEZE),
    /* Light magic   */ DMG_ENTRY(0, EN_WIZ_DMGEFF_NORMAL),
    /* Shield        */ DMG_ENTRY(0, EN_WIZ_DMGEFF_NORMAL),
    /* Mirror Ray    */ DMG_ENTRY(0, EN_WIZ_DMGEFF_NORMAL),
    /* Kokiri spin   */ DMG_ENTRY(2, EN_WIZ_DMGEFF_NORMAL),
    /* Giant spin    */ DMG_ENTRY(4, EN_WIZ_DMGEFF_NORMAL),
    /* Master spin   */ DMG_ENTRY(2, EN_WIZ_DMGEFF_NORMAL),
    /* Kokiri jump   */ DMG_ENTRY(4, EN_WIZ_DMGEFF_NORMAL),
    /* Giant jump    */ DMG_ENTRY(8, EN_WIZ_DMGEFF_NORMAL),
    /* Master jump   */ DMG_ENTRY(4, EN_WIZ_DMGEFF_NORMAL),
    /* Unknown 1     */ DMG_ENTRY(0, EN_WIZ_DMGEFF_NORMAL),
    /* Unblockable   */ DMG_ENTRY(0, EN_WIZ_DMGEFF_NORMAL),
    /* Hammer jump   */ DMG_ENTRY(4, EN_WIZ_DMGEFF_EXTRA),
    /* Unknown 2     */ DMG_ENTRY(0, EN_WIZ_DMGEFF_NORMAL),
};

static CollisionCheckInfoInit sColChkInfoInit = { 0, 12, 60, MASS_IMMOVABLE };

// table for spawning different enemy sets
typedef struct {
    u16 actor1;
    s16 params1;
    u16 actor2;
    s16 params2;
    u16 scaleMult; // used to multiply effect scales to account for smaller and larger enemies
    u8 count;
} EnemySpawnTable;

// index for table is defined by spawn params, so 0x0000 will be 0, 0x0001 will be 1 and so on
static EnemySpawnTable sEnemySpawnTable[] = {
    //      actor1                  params1     actor2                  params2     scaleMult       entryCount
    /* 0 */ {ACTOR_EN_BB,           0xFFFF,     0,                      0,          1,              1},
    /* 1 */ {ACTOR_EN_BB,           0xFFFF,     ACTOR_EN_BB,            0xFFFF,     1,              2},
    /* 2 */ {ACTOR_EN_FIREFLY,      0x0001,     ACTOR_EN_FIREFLY,       0x0004,     1,              2},
    /* 3 */ {ACTOR_EN_IK,           0xFF02,     0,                      0,          2,              1},
    /* 4 */ {ACTOR_EN_TEST,         0x0002,     0,                      0,          2,              1},
    /* 5 */ {ACTOR_EN_IK,           0xFF02,     ACTOR_EN_IK,            0xFF03,     2,              2},
    /* 6 */ {ACTOR_EN_TEST,         0x0002,     ACTOR_EN_TEST,          0x0002,     2,              2},
    /* 7 */ {ACTOR_EN_ZF,           0x0080,     0,                      0,          2,              1},
    /* 8 */ {ACTOR_EN_FLOORMAS,     0,          ACTOR_EN_FLOORMAS,      0,          1,              2},
    /* 9 */ {ACTOR_EN_ZF,           0x0080,     ACTOR_EN_ZF,            0x0080,     2,              2},
    /* 10 */ {ACTOR_EN_ZF,          0xFFFE,     0,                      0,          2,              1},
    /* 11 */ {ACTOR_EN_ZF,          0xFFFE,     ACTOR_EN_ZF,            0xFFFE,     2,              2},
    /* 12 */ {ACTOR_EN_BB,          0xFFFF,     ACTOR_EN_POH,           0,          1,              2},
    /* 13 */ {ACTOR_EN_TITE,        0xFFFE,     ACTOR_EN_TITE,          0xFFFF,     1,              2},
    /* 14 */ {ACTOR_EN_BB,          0xFFFE,     ACTOR_EN_BB,            0xFFFE,     1,              2},
    /* 15 */ {ACTOR_EN_BW,          0xFFFF,     ACTOR_EN_BW,            0xFFFF,     1,              2},
    /* 16 */ {ACTOR_EN_RD,          0x7F00,     0,                      0,          1,              1},
    /* 17 */ {ACTOR_EN_RD,          0x7F00,     ACTOR_EN_RD,            0x7F00,     1,              2},
    /* 18 */ {ACTOR_EN_WF,          0xFF00,     0,                      0,          1,              1},
    /* 19 */ {ACTOR_EN_WF,          0xFF00,     ACTOR_EN_WF,            0xFF01,     1,              2},
    /* 20 */ {0x01F2,               4,          0,                      0,          1,              1},
    /* 21 */ {0x01DF,               1,          0x01DF,                 1,          1,              2},
    /* 22 */ {0x0031,               0,          0x0031,                 0,          1,              2},
    /* 23 */ {0x0031,               0,          0x0226,                 0,          1,              2},
    /* 24 */ {0x0078,               0,          0x0078,                 1,          1,              2},
    /* 25 */ {0x0204,               0,          0,                      0,          2,              1},
    /* 26 */ {0x0204,               0,          0x0204,                 0,          2,              2},
    /* 27 */ {ACTOR_EN_MB,          0xFFFF,     ACTOR_EN_MB,            0xFFFF,     2,              2},
    /* 28 */ {ACTOR_EN_DODONGO,     0xFFFF,     ACTOR_EN_DODONGO,       0xFFFF,     2,              2},
    /* 29 */ {ACTOR_EN_DODONGO,     0xFFFF,     ACTOR_EN_DODONGO,       0x0002,     2,              2},
    /* 30 */ {ACTOR_EN_DODONGO,     0xFFFF,     0x0224,                 0xFFFF,     2,              2},
    /* 31 */ {0x0224,               0xFFFF,     0x0224,                 0xFFFF,     2,              2},
    /* 32 */ {0x0224,               0xFFFF,     0x0225,                 0xFFFF,     2,              2},
    /* 33 */ {0x0230,               0xFF02,     0,                      0,          2,              1},
    /* 34 */ {0x0231,               0xFF01,     0,                      0,          2,              1},
    /* 35 */ {ACTOR_EN_GOMA,        6,          ACTOR_EN_GOMA,          6,          1,              2},
    /* 36 */ {ACTOR_EN_NY,          0,          ACTOR_EN_NY,            0,          1,              2},
    /* 37 */ {ACTOR_EN_SB,          0,          ACTOR_EN_SB,            0,          1,              2},
    /* 38 */ {0x0330,               0xFF02,     0,                      0,          2,              1},
    /* 39 */ {ACTOR_EN_BB,          0xFFFA,     ACTOR_EN_BB,            0xFFFA,     1,              2},
    /* 40 */ {ACTOR_EN_RD,          0x7FFE,     ACTOR_EN_RD,            0x7FFE,     1,              2},
    /* 41 */ {ACTOR_EN_ANUBICE_TAG, 0xFFFF,     ACTOR_EN_ANUBICE_TAG,   0xFFFF,     1,              2},
    /* 41 */ {ACTOR_EN_FZ,          0xFFFF,     ACTOR_EN_FZ,            0xFFFF,     1,              2},
};

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(naviEnemyId, 0x03, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 2000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 350, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1000, ICHAIN_CONTINUE),
    ICHAIN_U8(targetMode, 2, ICHAIN_STOP),
};

void EnWiz_InitCollision(Actor* thisx, PlayState* play) {
    EnWiz* this = (EnWiz*)thisx;

    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
    Collider_UpdateCylinder(&this->actor, &this->collider);
}

void EnWiz_Init(Actor* thisx, PlayState* play) {
    EnWiz* this = (EnWiz*)thisx;

    Actor_ProcessInitChain(thisx, sInitChain);
    Actor_SetScale(&this->actor, 0.01f);
    SkelAnime_InitFlex(play, &this->skelAnime, 0x06008638, 0x060088ac, this->jointTable, this->morphTable,
                       12);
    ActorShape_Init(&thisx->shape, -200.0f, ActorShadow_DrawCircle, 70.0f);

    EnWiz_InitCollision(thisx, play);
    CollisionCheck_SetInfo(&thisx->colChkInfo, NULL, &sColChkInfoInit);
    thisx->colChkInfo.damageTable = sDamageTable;
    thisx->colChkInfo.health = 6;

    thisx->shape.yOffset = -200.0f; // maybe remove this?

    Actor_SetScale(&this->actor, 0.008f);

    this->type = (thisx->params & 0xFF00) >> 8;

    // clothing colors
    switch (this->type) {
        case ENWIZ_TYPE_MASTER:
            this->typeColor.r = 255;
            this->typeColor.g = 150;
            this->typeColor.b = 0;
            //this->actor.naviEnemyId = 0x98;
            break;
        case ENWIZ_TYPE_FIRE:
            this->typeColor.r = 150;
            this->typeColor.g = 0;
            this->typeColor.b = 0;
            break;
        case ENWIZ_TYPE_ICE:
            this->typeColor.r = 0;
            this->typeColor.g = 100;
            this->typeColor.b = 255;
            break;
        case ENWIZ_TYPE_ELECTRIC:
            this->typeColor.r = 0;
            this->typeColor.g = 150;
            this->typeColor.b = 0;
            break;
    }
    this->typeColor.a = 0;

    this->alpha = 0; // invisible during spawn
    TARGET_OFF;

    this->spawnCount[0] = this->spawnCount[1] = 0;
    this->enemySpawnSet = thisx->params & 0xFF;

    this->floatPosY = this->actor.home.pos.y;

    EnWiz_SetupTeleport(this, play);
}

void EnWiz_Destroy(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    EnWiz* this = (EnWiz*)thisx;

    Collider_DestroyCylinder(play, &this->collider);
}

/**
 * Helper Functions
 */

void EnWiz_LevitateSetY(EnWiz* this, PlayState* play) {
    this->actor.world.pos.y = Math_SinS(play->gameplayFrames * 0x800) * 8.0f + this->floatPosY + 20.0f; // + 20.0f is only for the bombiwa offset
}

// call this function only once and not every frame, returns 0 when spawns are active or 1 if not, to check for
s32 EnWiz_SpawnCheck(EnWiz* this, PlayState* play) {
    u8 i;

    for (i = 0; i < 2; i++) {
        if (this->spawnCount[i] > 0) {
            if (this->spawnedActor[i] != NULL && this->spawnedActor[i]->update == NULL) { // when spawn is killed
                this->spawnCount[i] = 0;
            }
        }
    }

    if (this->spawnCount[0] == 0 && this->spawnCount[1] == 0) {
        return 1;
    } else {
        return 0;
    }
}

static Vec3f sEffectPosAccel = { 0.0f, 0.0f, 0.0f };
static Vec3f sEffectPosVel = { 0.0f, 10.0f, 0.0f };
static Color_RGBA8 sEffectEnvColor = { 80, 0, 80, 255 };

void EnWiz_SpawnEnemies(EnWiz* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Vec3f spawnPos;
    Vec3f effectPos;
    s16 spawnAngle;
    u8 i;
    f32 distToPlayer;
    s16 yawToPlayer;

    // calculate spawn position distance from home position
    yawToPlayer = Math_Vec3f_Yaw(&this->actor.home.pos, &player->actor.world.pos);

    spawnAngle = yawToPlayer;

    distToPlayer = Math_Vec3f_DistXZ(&this->actor.home.pos, &player->actor.world.pos);
    distToPlayer = distToPlayer * 0.6f;

    spawnPos.x = this->actor.home.pos.x + (Math_SinS(spawnAngle) * distToPlayer);
    spawnPos.y = player->actor.floorHeight;
    spawnPos.z = this->actor.home.pos.z + (Math_CosS(spawnAngle) * distToPlayer);

    // calculate separation distance from spawn position
    if (sEnemySpawnTable[this->enemySpawnSet].count > 1) {
        spawnAngle += DEG_TO_BINANG(90);

        spawnPos.x += (Math_SinS(spawnAngle) * 60.0f);
        spawnPos.z += (Math_CosS(spawnAngle) * 60.0f);
    }

    for (i = 0; i < 4; i++) {
        effectPos.x = spawnPos.x + ((Math_SinS((Rand_ZeroOne() * 65536.0f)) * (20.0f * sEnemySpawnTable[this->enemySpawnSet].scaleMult)) + Rand_CenteredFloat(10.0f));
        effectPos.y = player->actor.floorHeight + 10.0f;
        effectPos.z = spawnPos.z + ((Math_CosS((Rand_ZeroOne() * 65536.0f)) * (20.0f * sEnemySpawnTable[this->enemySpawnSet].scaleMult)) + Rand_CenteredFloat(10.0f));

        func_8002829C(play, &effectPos, &sEffectPosVel, &sEffectPosAccel, &this->castColor, &sEffectEnvColor,
                          1500 * sEnemySpawnTable[this->enemySpawnSet].scaleMult, 20);
    }

    this->spawnedActor[0] = Actor_Spawn(&play->actorCtx, play, sEnemySpawnTable[this->enemySpawnSet].actor1, spawnPos.x, spawnPos.y, spawnPos.z, 0, yawToPlayer, 0, sEnemySpawnTable[this->enemySpawnSet].params1);
    this->spawnCount[0] = 1;
    Audio_PlayActorSfx2(&this->actor, NA_SE_EN_TWINROBA_TRANSFORM);

    // second enemy
    if (sEnemySpawnTable[this->enemySpawnSet].count > 1) {
        // calculate spawn position distance from home position
        spawnAngle = yawToPlayer;

        spawnPos.x = this->actor.home.pos.x + (Math_SinS(spawnAngle) * distToPlayer);
        spawnPos.y = player->actor.floorHeight;
        spawnPos.z = this->actor.home.pos.z + (Math_CosS(spawnAngle) * distToPlayer);

        // calculate separation distance from spawn position
        spawnAngle += DEG_TO_BINANG(270);

        spawnPos.x += (Math_SinS(spawnAngle) * 60.0f);
        spawnPos.z += (Math_CosS(spawnAngle) * 60.0f);

        for (i = 0; i < 4; i++) {
            effectPos.x = spawnPos.x + ((Math_SinS((Rand_ZeroOne() * 65536.0f)) * (20.0f * sEnemySpawnTable[this->enemySpawnSet].scaleMult)) + Rand_CenteredFloat(10.0f));
            effectPos.y = player->actor.floorHeight + 10.0f;
            effectPos.z = spawnPos.z + ((Math_CosS((Rand_ZeroOne() * 65536.0f)) * (20.0f * sEnemySpawnTable[this->enemySpawnSet].scaleMult)) + Rand_CenteredFloat(10.0f));

            func_8002829C(play, &effectPos, &sEffectPosVel, &sEffectPosAccel, &this->castColor, &this->castColor,
                              1500 * sEnemySpawnTable[this->enemySpawnSet].scaleMult, 20);
        }

        this->spawnedActor[1] = Actor_Spawn(&play->actorCtx, play, sEnemySpawnTable[this->enemySpawnSet].actor2, spawnPos.x, spawnPos.y, spawnPos.z, 0, yawToPlayer, 0, sEnemySpawnTable[this->enemySpawnSet].params2);
        this->spawnCount[1] = 1;
        Audio_PlayActorSfx2(&this->actor, NA_SE_EN_TWINROBA_TRANSFORM);
    }
}

void EnWiz_DecideAction(EnWiz* this, PlayState* play) {
    if (Rand_ZeroOne() >= 0.3f) {
        if (EnWiz_SpawnCheck(this, play) && this->type == ENWIZ_TYPE_MASTER) {
            this->castMagicType = CASTTYPE_ENEMY;
            EnWiz_SetupCastEnemies(this);
        } else {
            if ((Rand_ZeroOne() >= 0.5f && this->type == ENWIZ_TYPE_MASTER) || this->type == ENWIZ_TYPE_ICE) {
                this->castMagicType = CASTTYPE_ICE;
                EnWiz_SetupCastAttack(this);
            } else {
                this->castMagicType = CASTTYPE_FIRE;
                EnWiz_SetupCastAttack(this);
            }
        }
    } else {
        if ((Rand_ZeroOne() >= 0.5f && this->type == ENWIZ_TYPE_MASTER) || this->type == ENWIZ_TYPE_FIRE) {
            this->castMagicType = CASTTYPE_FIRE;
            EnWiz_SetupCastAttack(this);
        } else {
            this->castMagicType = CASTTYPE_ICE;
            EnWiz_SetupCastAttack(this);
        }
    } if ((Rand_ZeroOne() >= 0.8f && this->type == ENWIZ_TYPE_MASTER) || (this->type == ENWIZ_TYPE_ELECTRIC)) {
        this->castMagicType = CASTTYPE_ELECTRIC;
        EnWiz_SetupCastAttack(this);
    }
}

void EnWiz_SpawnSparkles(EnWiz* this, PlayState* play, s16 numSparkles, s32 movementType) {
    static Vec3f velocities[] = { { 0.0f, 0.1f, 0.0f }, { 0.0f, 0.0f, 0.0f } };
    static Vec3f accelerations[] = { { 0.0f, 0.01f, 0.0f }, { 0.0f, 0.0f, 0.0f } };
    Vec3f pos;
    Vec3f velocity;
    Vec3f accel;
    Color_RGBA8 envColor;
    s32 i;
    s16 scale;
    s16 life;
    f32 angle;

    if (numSparkles < 1) {
        numSparkles = 1;
    }

    envColor.r = 255;
    envColor.g = 255;
    envColor.b = 255;
    velocity = velocities[movementType];
    accel = accelerations[movementType];
    scale = 3000;
    life = 16;
    angle = (Rand_ZeroOne() - 0.5f) * (2.0f * 3.14f);

    for (i = 0; i < numSparkles; i++) {
        if (movementType == 1) {
            velocity.y = Rand_ZeroFloat(5.0f) + 5.0f;
            scale = 4000;
            pos.x = Math_SinF(angle) * 30.0f + this->actor.world.pos.x;
            pos.z = Math_CosF(angle) * 30.0f + this->actor.world.pos.z;
            pos.y = this->actor.world.pos.y;
        } else {
            accel.x = Rand_CenteredFloat(1.0f);
            accel.y = Rand_CenteredFloat(0.5f);
            accel.z = Rand_CenteredFloat(1.0f);
            pos = this->staffFlamePos;
        }
        EffectSsKiraKira_SpawnDispersed(play, &pos, &velocity, &accel, &this->castColor, &envColor, scale, life);
    }
}

/**
 * Setup & Action Functions
 */

// Appear
void EnWiz_SetupAppear(EnWiz* this) {
    Animation_MorphToLoop(&this->skelAnime, 0x060088ac, -3.0f);
    Audio_PlayActorSfx2(&this->actor, NA_SE_EN_PO_APPEAR);
    SoundEffect_PlayOneshot(SOUND_ENEMY_WW_WIZZROBE_APPEAR, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 2000.0f, &this->actor);
    this->timer = 0;
    TARGET_ON;
    this->actionFunc = EnWiz_Appear;
}

void EnWiz_Appear(EnWiz* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    // Debug_Print(0, "EnWiz_Appear");

    this->timer++;

    this->alpha = CLAMP_MAX((s32)(this->timer * 25.5f), 255);

    if (this->alpha == 255 && this->timer >= 30) {
        this->timer = Rand_S16Offset(80, 10);
        EnWiz_DecideAction(this, play);
    } else {
        this->actor.world.rot.y -= 2000;
    }
}

// Cast magic attacks
void EnWiz_SetupCastAttack(EnWiz* this) {
    Animation_MorphToLoop(&this->skelAnime, 0x06008e84, -3.0f);
    this->timer = Rand_S16Offset(30, 15);
    this->actionFunc = EnWiz_CastAttack;
}

void EnWiz_CastAttack(EnWiz* this, PlayState* play) {
    // Debug_Print(0, "EnWiz_CastAttack");

    EnWiz_SpawnSparkles(this, play, 1, 0);

    Math_ApproachS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 6, 0x400);

    Audio_PlayActorSfx2(&this->actor, NA_SE_EN_GANON_CHARGE_MASIC - SFX_FLAG);

    if (this->timer <= 0) {
        EnWiz_SetupAttack(this);
    }

    this->timer--;
}

// Cast enemies
void EnWiz_SetupCastEnemies(EnWiz* this) {
    Animation_MorphToLoop(&this->skelAnime, 0x060091d4, -3.0f);
    this->timer = Rand_S16Offset(30, 15);
    this->actionFunc = EnWiz_CastEnemies;
}

void EnWiz_CastEnemies(EnWiz* this, PlayState* play) {
    // Debug_Print(0, "EnWiz_CastEnemies");

    EnWiz_SpawnSparkles(this, play, 2, 1);

    Math_ApproachS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 6, 0x400);

    Audio_PlayActorSfx2(&this->actor, NA_SE_EN_GANON_CHARGE_MASIC - SFX_FLAG);

    if (this->timer <= 0) {
        EnWiz_SpawnEnemies(this, play);
        EnWiz_SetupDisappear(this);
        SoundEffect_PlayOneshot(SOUND_ENEMY_WW_WIZZROBE_LAUGH, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 2000.0f, &this->actor);
    }

    this->timer--;
}

// Attack
void EnWiz_SetupAttack(EnWiz* this) {
    Animation_PlayOnce(&this->skelAnime, 0x06008c50);
    this->timer = 0;
    this->actionFunc = EnWiz_Attack;
}

void EnWiz_Attack(EnWiz* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    f32 beamPitch;
    Vec3f pos = this->actor.world.pos;

    if (this->skelAnime.curFrame == 1) {
        Audio_PlayActorSfx2(&this->actor, NA_SE_IT_EXPLOSION_FRAME);
        Audio_PlayActorSfx2(&this->actor, NA_SE_PL_MAGIC_FIRE);
        SoundEffect_PlayOneshot(SOUND_ENEMY_WW_WIZZROBE_ATTACK, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 2000.0f, &this->actor);

        pos.x += Math_SinS(this->actor.world.rot.y) * 40.0f;
        pos.y += 60.0f;
        pos.z += Math_CosS(this->actor.world.rot.y) * 40.0f;

        beamPitch = Math_Vec3f_Pitch(&player->actor.world.pos, &this->staffFlamePos);

        Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, 0x01F1, pos.x, pos.y, pos.z,
                               beamPitch,
                               this->actor.world.rot.y, 0, this->castMagicType);
        this->hasActiveProjectile = true;
    }

    if (SkelAnime_Update(&this->skelAnime)) { // after whole animation has been played
        EnWiz_SetupDisappear(this);
        SoundEffect_PlayOneshot(SOUND_ENEMY_WW_WIZZROBE_LAUGH, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 2000.0f, &this->actor);
    }
}

// Disappear
void EnWiz_SetupDisappear(EnWiz* this) {
    Animation_MorphToLoop(&this->skelAnime, 0x060088ac, -3.0f);
    Audio_PlayActorSfx2(&this->actor, NA_SE_EN_PO_DISAPPEAR);
    this->timer = 20;
    TARGET_OFF;
    this->actionFunc = EnWiz_Disappear;
}

void EnWiz_Disappear(EnWiz* this, PlayState* play) {

    // Debug_Print(0, "EnWiz_Disappear");

    this->alpha = CLAMP((s32)(this->timer * 25.5f), 0, 255);

    if (this->alpha == 0) {
        // EnWiz_GetTeleportPos(this, play);
        EnWiz_SetupTeleport(this, play);
    } else {
        this->actor.world.rot.y += 2000;
    }

    this->timer--;
}

// Idle
void EnWiz_SetupIdle(EnWiz* this) {
    #ifdef TEST
    this->alpha = 100;
    this->timer = 120;
    this->actionFunc = EnWiz_DoNothing;
    #else
    Animation_MorphToLoop(&this->skelAnime, 0x060088ac, -5.0f);
    this->timer = Rand_S16Offset(40, 15); // this->timer = Rand_S16Offset(30, 10);
    this->actionFunc = EnWiz_Idle;
    #endif
    
}

void EnWiz_Idle(EnWiz* this, PlayState* play) {

    // Debug_Print(0, "EnWiz_Idle");

    Math_ApproachS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 6, 0x400);

    // test
    if (this->timer <= 0) {
        if (this->alpha == 0) {
            EnWiz_SetupAppear(this);
        } else {
            SoundEffect_PlayOneshot(SOUND_ENEMY_WW_WIZZROBE_LAUGH, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 2000.0f, &this->actor);
            EnWiz_SetupDisappear(this);
        }
    } else {
        this->timer--;
    }
}

// Damaged
void EnWiz_SetupDamaged(EnWiz* this) {
    Animation_MorphToPlayOnce(&this->skelAnime, 0x06008f1c, -2.0f);
    Audio_PlayActorSfx2(&this->actor, NA_SE_EN_NUTS_CUTBODY);
    SoundEffect_PlayOneshot(SOUND_ENEMY_WW_WIZZROBE_DAMAGE, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 2000.0f, &this->actor);
    this->timer = 20;
    this->actor.speedXZ = -6.0f;
    this->actionFunc = EnWiz_Damaged;
}

void EnWiz_Damaged(EnWiz* this, PlayState* play) {
    if (this->timer <= 0) {
        #ifdef TEST
        Animation_MorphToLoop(&this->skelAnime, 0x060088ac, -3.0f);
        this->actionFunc = EnWiz_DoNothing;
        this->timer = 40;
        #else
        this->actor.speedXZ = 0.0f;
        EnWiz_SetupDisappear(this);
        #endif
    } else {
        Math_SmoothStepToF(&this->actor.speedXZ, 0.0f, 1.0f, 0.5f, 0.0f);
        func_8002D97C(&this->actor);
        this->timer--;
    }
}

// Frozen
void EnWiz_SetupFrozen(Actor* thisx) {
    EnWiz* this = (EnWiz*)thisx;

    Animation_MorphToPlayOnce(&this->skelAnime, 0x06008f1c, -2.0f);
    // this->actor.speedXZ = -6.0f;
    this->actor.world.rot.y = this->actor.yawTowardsPlayer;
    if (thisx->colChkInfo.damageEffect == 0xF) {
        this->spawnIceTimer = 36;
    }
    Audio_PlayActorSfx2(&this->actor, NA_SE_EN_GOMA_JR_FREEZE);

    this->collider.base.colType = COLTYPE_HARD;
    this->collider.base.acFlags |= AC_HARD;

    this->actionFunc = EnWiz_Frozen;
}

void EnWiz_Frozen(EnWiz* this, PlayState* play) {
    if (this->actor.colorFilterTimer == 0) {
        Audio_PlayActorSfx2(&this->actor, NA_SE_PL_ICE_BROKEN);
        if (this->actor.colChkInfo.health == 0) {
            EnWiz_SetupDeath(this);
        } else {
            #ifdef TEST
            Animation_MorphToLoop(&this->skelAnime, 0x060088ac, -3.0f);
            this->actionFunc = EnWiz_DoNothing;
            this->timer = 40;
            #else
            this->collider.base.colType = COLTYPE_HIT0;
            this->collider.base.acFlags &= ~AC_HARD;
            EnWiz_SetupDisappear(this);
            #endif
        }

    }
}

// Death
void EnWiz_SetupDeath(EnWiz* this) {
    Animation_MorphToPlayOnce(&this->skelAnime, 0x06008f1c, -2.0f);
    Audio_PlayActorSfx2(&this->actor, NA_SE_EN_NUTS_CUTBODY);
    SoundEffect_PlayOneshot(SOUND_ENEMY_WW_WIZZROBE_DEAD, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 2000.0f, &this->actor);
    this->actor.colorFilterTimer = 0;
    this->timer = 10;
    this->actionFunc = EnWiz_Death;
}

void EnWiz_Death(EnWiz* this, PlayState* play) {
    Vec3f deadEffectVel = { 0.0f, 0.0f, 0.0f };
    Vec3f accel = { 0.0f, 0.3f, 0.0f };
    Vec3f pos;

    // Math_ApproachZeroF(&this->actor.speedXZ, 1.0f, 2.0f);

    Audio_PlaySfxGeneral(NA_SE_EN_GOMA_LAST - SFX_FLAG, &this->actor.projectedPos, 4,
                                     &gSfxDefaultFreqAndVolScale, &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);

    if (this->timer > 5) {
        pos.x = Rand_CenteredFloat(20.0f) + this->actor.world.pos.x;
        pos.y = (Rand_CenteredFloat(40.0f) + this->actor.world.pos.y) - 10.0f;
        pos.z = (Rand_CenteredFloat(20.0f) + this->actor.world.pos.z) + 5.0f;
        accel = deadEffectVel;
        accel.y = 0.03f;
        EffectSsKFire_Spawn(play, &pos, &deadEffectVel, &accel, (s16)Rand_ZeroFloat(15.0f) + 30, 0);
    } else if (Math_SmoothStepToF(&this->actor.scale.y, 0, 1.0f, 0.00075f, 0.0f) <= 0.001f) {
        Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos, 0xE0);
        Actor_Kill(&this->actor);
    }

    this->actor.world.rot.y += 2000;

    this->timer--;
}

void EnWiz_SetupTeleport(EnWiz* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    this->teleportTimer = Rand_S16Offset(30, 20);
    this->timer = Rand_S16Offset(50, 20);

    this->actor.world.pos = this->actor.home.pos;
    this->floatPosY = player->actor.floorHeight + 120.0f;
    this->actor.world.rot.y = (Rand_ZeroOne() * 65536.0f);
    this->actor.gravity = 0.0f;

    this->actor.speedXZ = 10.0f;

    this->actionFunc = EnWiz_Teleport;
}

void EnWiz_Teleport(EnWiz* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (this->timer >= 0) {
        if ((this->actor.speedXZ != 0.0f) && (this->actor.bgCheckFlags & BGCHECKFLAG_WALL)) {
            this->actor.world.rot.y = this->actor.wallYaw;
        }
        if (this->teleportTimer >= 0) {
            func_8002D97C(&this->actor);
        }

        this->timer--;
        this->teleportTimer--;
    } else {
        this->actor.speedXZ = 0.0f;
        // this->floatPosY = player->actor.floorHeight + 30.0f + Rand_ZeroFloat(180.0f);
        this->floatPosY = player->actor.floorHeight + 80.0f + Rand_CenteredFloat(60.0f);
        this->actor.world.rot.y = this->actor.yawTowardsPlayer;
        EnWiz_SetupAppear(this);
    }
}

// testing function
void EnWiz_DoNothing(EnWiz* this, PlayState* play) {
    // Debug_Print(0, "EnWiz_DoNothing");
}

void EnWiz_CheckDamage(Actor* thisx, PlayState* play) {
    f32 rand01;
    EnWiz* this = (EnWiz*)thisx;

    if ((this->collider.base.acFlags & AC_HIT)) {
        this->collider.base.acFlags &= ~AC_HIT;

        // freeze wizzrobe
        if (thisx->colChkInfo.damageEffect == EN_WIZ_DMGEFF_FREEZE) {
            if (this->actionFunc != EnWiz_Frozen) {
                Actor_SetColorFilter(thisx, COLORFILTER_COLORFLAG_BLUE, 120, COLORFILTER_BUFFLAG_OPA, 80);
                Actor_ApplyDamage(thisx);
                EnWiz_SetupFrozen(thisx);
            }
        // other damage that isn't IMMUNE
        } else if (thisx->colChkInfo.damageEffect != EN_WIZ_DMGEFF_IMMUNE) {
            // regular damage
            if (this->actionFunc != EnWiz_Frozen && ((thisx->colorFilterTimer == 0) || ((thisx->colorFilterParams & 0x4000) == 0))) {
                Actor_SetColorFilter(thisx, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 8);
                Actor_ApplyDamage(thisx);
                EnWiz_SetupDamaged(this);
            }
            // when health is 0 or instant kill when frozen
            if (thisx->colChkInfo.health == 0 || (this->actionFunc == EnWiz_Frozen && 
            (thisx->colChkInfo.damageEffect == EN_WIZ_DMGEFF_FIRE || thisx->colChkInfo.damageEffect == EN_WIZ_DMGEFF_EXTRA))) {
                if (this->actionFunc == EnWiz_Frozen) {
                    Audio_PlayActorSfx2(&this->actor, NA_SE_PL_ICE_BROKEN);
                }
                EnWiz_SetupDeath(this);
            }
        }

    }
}

void EnWiz_Update(Actor* thisx, PlayState* play) {
    EnWiz* this = (EnWiz*)thisx;
    Player* player = GET_PLAYER(play);

    SkelAnime_Update(&this->skelAnime);

    EnWiz_CheckDamage(thisx, play);

    Actor_UpdateBgCheckInfo(play, &this->actor, 10.0f, 25.0f, 0.0f, UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2);
    Collider_UpdateCylinder(&this->actor, &this->collider);
    this->actionFunc(this, play);

    if (this->actionFunc != EnWiz_Frozen) {
        EnWiz_LevitateSetY(this, play);
    }

    this->actor.shape.rot.y = this->actor.world.rot.y;
    this->actor.shape.shadowAlpha = this->alpha;

    #ifdef TEST
    CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
    // CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
    #else
    if (this->actionFunc == EnWiz_CastAttack || this->actionFunc == EnWiz_CastEnemies || this->actionFunc == EnWiz_Attack || this->actionFunc == EnWiz_Frozen) {
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
    }
    if (this->actionFunc != EnWiz_Teleport) {
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
    }
    #endif

}

void EnWiz_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx, Gfx** gfx) {
    EnWiz* this = (EnWiz*)thisx;
    Vec3f zeroVec = { 0.0f, 0.0f, 0.0f }; // position offset
    Vec3f staffFlamePos = { 0.0f, 0.0f, 0.0f };
    Vec3f bodyPartPos;
    s32 bodyPart = -1;

    if (limbIndex == ARMATURE_STAFF_R_LIMB) {
        staffFlamePos.x = 0.0f;
        staffFlamePos.y = 5000.0f;
        if (this->actionFunc != EnWiz_CastAttack) {
            staffFlamePos.y = 0.0f;
            staffFlamePos.x = 5300.0f;
        }

        Matrix_Translate(0.0f, 0.0f, 0.0f, MTXMODE_APPLY);
        Matrix_MultVec3f(&staffFlamePos, &this->staffFlamePos);
    }

    if (limbIndex == ARMATURE_HEAD_LIMB) {
        // set navi target position to head
        Matrix_MultVec3f(&zeroVec, &this->actor.focus.pos);
    }

    if (this->spawnIceTimer != 0) {
        switch (limbIndex) {
            case ARMATURE_HEAD_LIMB:
                bodyPart = 0;
                break;
            case ARMATURE_STAFF_R_LIMB:
                bodyPart = 1;
                break;
            case ARMATURE_FOREARM_L_LIMB:
                bodyPart = 2;
                break;
            case ARMATURE_FOREARM_R_LIMB:
                bodyPart = 3;
                break;
            case ARMATURE_UPPERARM_R_LIMB:
                bodyPart = 4;
                break;
            case ARMATURE_UPPERARM_L_LIMB:
                bodyPart = 5;
                break;
            case ARMATURE_UPPERBODY_LIMB:
                bodyPart = 6;
                break;
            case ARMATURE_LOWERBODY_POS_LIMB:
                bodyPart = 7;
                break;
            case ARMATURE_LOWERJAW_LIMB:
                bodyPart = 8;
                break;
        }

        if (bodyPart >= 0) {
            Matrix_MultVec3f(&zeroVec, &bodyPartPos);

            this->bodyPartsPos[bodyPart].x = bodyPartPos.x;
            this->bodyPartsPos[bodyPart].y = bodyPartPos.y;
            this->bodyPartsPos[bodyPart].z = bodyPartPos.z;
        }
    }
}

Gfx* EnWiz_SetEnvColor(GraphicsContext* gfxCtx, u8 r, u8 g, u8 b, u8 a) {
    Gfx* dList = Graph_Alloc(gfxCtx, sizeof(Gfx) * 2);

    gDPSetEnvColor(dList, r, g, b, a);
    gSPEndDisplayList(dList + 1);
    return dList;
}

void EnWiz_Draw(Actor* thisx, PlayState* play) {
    EnWiz* this = (EnWiz*)thisx;

    OPEN_DISPS(play->state.gfxCtx, "../z_en_wiz.c", __LINE__);

    if (this->alpha == 255) {
        gSPSegment(POLY_OPA_DISP++, 0x09,
                   EnWiz_SetEnvColor(play->state.gfxCtx, this->typeColor.r, this->typeColor.g, this->typeColor.b, 255));

        // pulsating casting effect
        if (this->actionFunc == EnWiz_CastAttack || this->actionFunc == EnWiz_CastEnemies) {
            switch (this->castMagicType) {
                case CASTTYPE_FIRE:
                    this->castColor.r = 255;
                    this->castColor.g = 150;
                    this->castColor.b = 0;
                    break;
                case CASTTYPE_ICE:
                    this->castColor.r = 0;
                    this->castColor.g = 150;
                    this->castColor.b = 255;
                    break;
                case CASTTYPE_ENEMY:
                    this->castColor.r = 160;
                    this->castColor.g = 0;
                    this->castColor.b = 160;
                    break;
                case CASTTYPE_ELECTRIC:
                    this->castColor.r = 150;
                    this->castColor.g = 255;
                    this->castColor.b = 0;
            }
            POLY_OPA_DISP =
                Gfx_SetFog2(POLY_OPA_DISP, this->castColor.r, this->castColor.g, this->castColor.b, 0, 0, 4000 - (s32)(Math_CosS(play->gameplayFrames * 4000) * 2000.0f));
        }

        func_80034BA0(play, &this->skelAnime, NULL, EnWiz_PostLimbDraw, &this->actor, this->alpha);

        // pulsating casting effect
        if (this->actionFunc == EnWiz_CastAttack || this->actionFunc == EnWiz_CastEnemies) {
            POLY_OPA_DISP = Play_SetFog(play, POLY_OPA_DISP);
        }

    } else {
        gSPSegment(POLY_XLU_DISP++, 0x09,
                   EnWiz_SetEnvColor(play->state.gfxCtx, this->typeColor.r, this->typeColor.g, this->typeColor.b, this->alpha));
                   
        func_80034CC4(play, &this->skelAnime, NULL, EnWiz_PostLimbDraw, &this->actor, this->alpha);
    }

    CLOSE_DISPS(play->state.gfxCtx, "../z_en_wiz.c", __LINE__);

    if (this->spawnIceTimer != 0) {
        thisx->colorFilterTimer++;
        this->spawnIceTimer--;

        if ((this->spawnIceTimer % 4) == 0) {
            s32 iceIndex = this->spawnIceTimer >> 2;

            EffectSsEnIce_SpawnFlyingVec3s(play, thisx, &this->bodyPartsPos[iceIndex], 150, 150, 150, 250, 235, 245,
                                           255, 1.8f);
        }
    }
}
