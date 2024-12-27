#include "assets/objects/gameplay_keep/gameplay_keep.h"
#include <uLib.h>
#include "vt.h"
#include "z64cutscene.h"
void func_800E0348(Camera* camera);
void CutsceneManager_Queue(s16 csId);
s16 CutsceneManager_IsNext(s16 csId);
s16 CutsceneManager_StartWithPlayerCs(s16 csId, Actor* actor);
s16 CutsceneManager_FindEntranceCsId(void);
s16 CutsceneManager_Stop(s16 csId);
s16 CutsceneManager_StartWithPlayerCs(s16 csId, Actor* actor);
s16 CutsceneManager_Start(s16 csId, Actor* actor);
//s16 Camera_SetStateFlag(Camera* camera, s16 flags);
//s16 Camera_UnsetStateFlag(Camera* camera, s16 flags);
s32 Camera_ChangeActorCsCamIndex(Camera* camera, s32 bgCamIndex);
s32 Cutscene_GetCueChannel(PlayState* play, u16 cueType);
s32 Cutscene_IsCueInChannel(PlayState* play, u16 cueType);
#define BODYPART_NONE -1
#define SHADOW_BODY_PARTS_NUM 14
#define SWITCH_FLAG_NONE -1
#define COLORFILTER_COLORFLAG_NONE 0xC000
#define COLORFILTER_COLORFLAG_GRAY 0x8000
#define COLORFILTER_COLORFLAG_RED  0x4000
#define COLORFILTER_COLORFLAG_BLUE 0x0000

#define COLORFILTER_BUFFLAG_XLU    0x2000
#define COLORFILTER_BUFFLAG_OPA    0x0000
#define COLORFILTER_INTENSITY_FLAG 0x8000
#define ABS_ALT(x) ((x) < 0 ? -(x) : (x))
#define BINANG_ROT180(angle) ((s16)(angle + 0x8000))
#define BINANG_ADD(a, b) ((s16)(a + b))
#define BINANG_SUB(a, b) ((s16)(a - b))
#define RAD_TO_BINANG_ALT2(radians) TRUNCF_BINANG(((radians) * 0x8000) / M_PI)
#define TRUNCF_BINANG(f) (s16)(s32)(f)

#define PATH_INDEX_NONE -1

#define ACTOR_FLAG_TARGETABLE    (1 << 0)
#define ACTOR_FLAG_UNFRIENDLY    (1 << 2)
#define TARGET_MODE_5   5

#define SEGMENTED_TO_K0(addr) (void*)((gSegments[SEGMENT_NUMBER(addr)] + K0BASE) + SEGMENT_OFFSET(addr))
#define Gfx_SetupDL44_Xlu Gfx_SetupDL_47Xlu
#define Gfx_SetupDL25_Opa Gfx_SetupDL_25Opa
#define Gfx_SetupDL25_Xlu Gfx_SetupDL_25Xlu
#define PLAYER_STATE3_1000       (1 << 12)
#define speed speedXZ
#define ACTORCAT_EXPLOSIVES ACTORCAT_EXPLOSIVE
#define GRAPH_ALLOC Graph_Alloc
#define unk_ADC unk_844
#define unk_D57 unk_A73
#define gZeroVec3f gZeroVec
#define SoundSource_PlaySfxAtFixedWorldPos SfxSource_PlaySfxAtFixedWorldPos
#define Math_Atan2F_XY Math_Atan2S_XY
#define Play_SetCameraAtEyeUp Play_CameraSetAtEyeUp
#define Play_SetCameraAtEye Play_CameraSetAtEye
#define CLEAR_TAG_PARAMS(type) (type)
#define titleCtxt titleCtx
#define hintId naviEnemyId
#define transformation linkAgeOnLoad
#define Actor_MoveWithGravity linkAgeOnLoad
#define CURRENT_DAY (((void)0, gSaveContext.totalDays) % 5)
#define terminalVelocity minVelocityY
#define Actor_OfferTalk func_8002F2CC

#define CLOCK_TIME_MINUTE  (CLOCK_TIME(0, 1))
#define CLOCK_TIME_HOUR (CLOCK_TIME(1, 0))
#define DAY_LENGTH (CLOCK_TIME(24, 0))

#define TIME_TO_HOURS_F(time) ((time) * (24.0f / 0x10000))
#define TIME_TO_HOURS_F_ALT(time) (TIME_TO_MINUTES_F(time) / 60.0f)
#define TIME_TO_MINUTES_F(time) ((time) * ((24.0f * 60.0f) / 0x10000)) // 0.021972656f
#define TIME_TO_MINUTES_ALT_F(time) ((time) / (0x10000 / (24.0f * 60.0f)))
#define TIME_TO_SECONDS_F(time) ((time) * ((24.0f * 60.0f * 60.0f) / 0x10000))

#define CLOCK_TIME_F(hr, min) (((hr) * 60.0f + (min)) * (0x10000 / (24.0f * 60.0f)))
#define CLOCK_TIME_ALT_F(hr, min) (((hr) * 60.0f + (min)) / (24.0f * 60.0f / 0x10000))
#define CLOCK_TIME_ALT2_F(hr, min) ((((hr) + (min) / 60.0f) * 60.0f) / (24.0f * 60.0f / 0x10000))
#define CLOCK_TIME_HOUR_F (CLOCK_TIME_F(1, 0))

#define GET_CURRENT_CLOCK_HOUR(this) ((s32)TIME_TO_HOURS_F((this)->clockTime))
#define GET_CURRENT_CLOCK_MINUTE(this) ((s32)((this)->clockTime * (360 * 2.0f / 0x10000)) % 30)
#define GET_CLOCK_FACE_ROTATION(currentClockHour) ((s16)(currentClockHour * (0x10000 / 24.0f)))
#define GET_MINUTE_RING_OR_EXTERIOR_GEAR_ROTATION(currentClockMinute) \
    ((s16)(currentClockMinute * (0x10000 * 12.0f / 360)))

#define PLAYER_FORM_FIERCE_DEITY LINK_AGE_ADULT

#define DMG_SWORD_BEAM DMG_SWORD

#define CLEAR_TAG_LARGE_LIGHT_RAYS 5

#define BHEART_PARAM_NORMAL 0
#define ENDOORWARP1_FF_1 WARP_DUNGEON_CHILD

//#define SEQCMD_PLAY_SEQUENCE Audio_QueueSeqCmd
#define NA_BGM_CLEAR_BOSS NA_BGM_BOSS_CLEAR
#define NA_SE_PL_DEKUNUTS_DROP_BOMB NA_SE_EV_OBJECT_FALL
#define NA_SE_EV_ROCK_FALL NA_SE_EV_DROP_FALL
#define SEQ_FLAG_ASYNC 0x8000
//#define Audio_PlaySfx func_80078914
#define Audio_PlaySfx_AtPos func_80078914
//#define SEQCMD_STOP_SEQUENCE Audio_PlayActorSfx2
#define func_800B7298 func_8002DF54

#define DYNA_TRANSFORM_POS (1 << 0) // Position of the actors on top follows the dynapoly actor's movement.

#define ACTOR_FLAG_31      (1 << 31) 

#define CAM_CHANGE_SETTING_0 (1 << 0)
#define CAM_CHANGE_SETTING_1 (1 << 1)
#define CAM_CHANGE_SETTING_2 (1 << 2)
#define CAM_CHANGE_SETTING_3 (1 << 3)
#define CS_STATE_START CS_STATE_UNSKIPPABLE_EXEC
#define Cutscene_StartManual func_80064520
#define Cutscene_StopManual func_80064534
#define playerCue linkAction
#define CS_STATE_STOP CS_CMD_STOP
#define CS_STATE_RUN_UNSTOPPABLE CS_STATE_UNSKIPPABLE_EXEC

#define Actor_PlaySfx Audio_PlayActorSfx2
#define CHECK_EVENTINF CLEAR_EVENTINF
#define EVENTINF_54 2

#define func_800B0EB0 func_8002836C
#define func_800BC154 Actor_ChangeCategory
#define func_800B8C20 func_8002F5C4
#define func_800B0DE0 func_8002829C
#define func_800B8D50 func_8002F6D4
#define func_80169AFC func_800C08AC

#define Quake_Request Quake_Add
#define Quake_SetPerturbations Quake_SetQuakeValues
#define Quake_SetDuration Quake_SetCountdown
#define QUAKE_TYPE_3 3
#define NA_SE_EV_BIGWALL_BOUND NA_SE_EV_STONE_BOUND

#define RELOAD_PARAMS(camera) ((camera->animState == 0) || (camera->animState == 10) || (camera->animState == 20))
#define Camera_UnsetStateFlag Camera_UnsetParam
#define CS_CMD_ACTOR_CUE_124 CS_CMD_FADEBGM
//#define actorCues npcActions
//#define id action

#define SUBS_SHADOW_TEX_WIDTH 64
#define SUBS_SHADOW_TEX_HEIGHT 64
#define SUBS_SHADOW_TEX_SIZE ((s32)sizeof(u8[SUBS_SHADOW_TEX_HEIGHT][SUBS_SHADOW_TEX_WIDTH]))
extern MtxF* sCurrentMatrix; //!< original name: "Matrix_now"

extern Mtx sActorHiliteMtx;
asm("sActorHiliteMtx = 0x8015BBA8");

typedef void* TexturePtr;


#define REG_GROUPS 29 // number of REG groups, i.e. REG, SREG, OREG, etc.
#define REG_PAGES 6
#define REG_PER_PAGE 16
#define REG_PER_GROUP REG_PAGES * REG_PER_PAGE

// To be used with OLib_Vec3fAdd()
typedef enum {
    /* 0 */ OLIB_ADD_COPY, // Copy `b` to dest
    /* 1 */ OLIB_ADD_OFFSET, // Add `a` and `b` to dest, and also add the yaw of `a` to the dest
    /* 2 */ OLIB_ADD // Add `a` and `b` to dest
} OlibVec3fAdd;


typedef struct {
    /* 0x0 */ f32 r; // radius
    /* 0x4 */ s16 pitch; // depends on coordinate system. See below.
    /* 0x6 */ s16 yaw; // azimuthal angle
} VecSphGeo; // size = 0x8

// Defines a point in the spherical coordinate system.
// Pitch is 0 along the positive y-axis (up)
typedef VecSphGeo VecSph_MM;

// Defines a point in the geographic coordinate system.
// Pitch is 0 along the xz-plane (horizon)
typedef VecSphGeo VecGeo;



void Actor_RequestQuake(PlayState* play, s16 y, s16 duration) {
    s16 quakeIndex = Quake_Add(Play_GetCamera(play, CAM_ID_MAIN), 3);

    Quake_SetSpeed(quakeIndex, 20000);
    Quake_SetQuakeValues(quakeIndex, y, 0, 0, 0);
    Quake_SetCountdown(quakeIndex, duration);
}

void Actor_RequestQuakeWithSpeed(PlayState* play, s16 y, s16 duration, s16 speed) {
    s16 quakeIndex = Quake_Add(Play_GetCamera(play, CAM_ID_MAIN), 3);

    Quake_SetSpeed(quakeIndex, speed);
    Quake_SetQuakeValues(quakeIndex, y, 0, 0, 0);
    Quake_SetCountdown(quakeIndex, duration);
}

void Actor_RequestQuakeAndRumble(Actor* actor, PlayState* play, s16 quakeY, s16 quakeDuration) {
    if (quakeY >= 5) {
        Rumble_Request(actor->xyzDistToPlayerSq, 255, 20, 150);
    } else {
        Rumble_Request(actor->xyzDistToPlayerSq, 180, 20, 100);
    }
    Actor_RequestQuake(play, quakeY, quakeDuration);
}


void* Lib_SegmentedToVirtual(void* ptr) {
    return SEGMENTED_TO_VIRTUAL(ptr);
}

void* Lib_SegmentedToVirtualNull(void* ptr) {
    if (((uintptr_t)ptr >> 28) == 0) {
        return ptr;
    } else {
        return SEGMENTED_TO_VIRTUAL(ptr);
    }
}



static Gfx SetupDL25[] = {

        /* SETUPDL_25 */
        gsDPPipeSync(),
        gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
        gsDPSetCombineMode(G_CC_MODULATEIDECALA, G_CC_MODULATEIA_PRIM2),
        gsDPSetOtherMode(G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE |
                             G_TD_CLAMP | G_TP_PERSP | G_CYC_2CYCLE | G_PM_NPRIMITIVE,
                         G_AC_NONE | G_ZS_PIXEL | G_RM_FOG_SHADE_A | G_RM_AA_ZB_OPA_SURF2),
        gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
        gsSPEndDisplayList(),
};


u32 SurfaceType_IsWallDamage(CollisionContext* colCtx, CollisionPoly* poly, s32 bgId) {
    return (SurfaceType_GetData(colCtx, poly, bgId, 1) & 0x8000000) ? true : false;
}


s32 SubS_CopyPointFromPath(Path* path, s32 pointIndex, Vec3f* dst) {
    Vec3s* point;

    if (path == NULL) {
        return false;
    }

    point = Lib_SegmentedToVirtual(path->points);
    point = &point[pointIndex];
    dst->x = point->x;
    dst->y = point->y;
    dst->z = point->z;

    return true;
}





s32 SubS_CopyPointFromPathCheckBounds(Path* path, s32 pointIndex, Vec3f* dst) {
    Vec3s* point;

    if ((path == NULL) || (pointIndex >= path->count) || (pointIndex < 0)) {
        return false;
    }

    point = Lib_SegmentedToVirtual(path->points);
    point = &point[pointIndex];
    dst->x = point->x;
    dst->y = point->y;
    dst->z = point->z;
    return true;
}



Path* SubS_GetPathByIndex(PlayState* play, s16 pathIndex, s16 pathIndexNone) {
    return (pathIndex != pathIndexNone) ? &play->setupPathList[pathIndex] : NULL;
}


Gfx* func_8012CB28(GraphicsContext* gfxCtx, u32 x, u32 y) {
    return Gfx_TexScroll(gfxCtx, x, y, 0, 0);
}




void Math_Vec3s_Copy(Vec3s* dest, Vec3s* src) {
    s16 x = src->x;
    s16 y = src->y;
    s16 z = src->z;

    dest->x = x;
    dest->y = y;
    dest->z = z;
}

typedef void (*TransformLimbDrawOpa)(struct PlayState* play, s32 limbIndex, struct Actor* thisx);

MtxF* Matrix_GetCurrent(void) {
    return sCurrentMatrix;
}


Gfx* Hilite_Draw(Vec3f* object, Vec3f* eye, Vec3f* lightDir, GraphicsContext* gfxCtx, Gfx* dl, Hilite** hiliteP) {
    LookAt* lookAt = Graph_Alloc(gfxCtx, sizeof(LookAt));
    f32 correctedEyeX = (eye->x == object->x) && (eye->z == object->z) ? eye->x + 0.001f : eye->x;

    *hiliteP = Graph_Alloc(gfxCtx, sizeof(Hilite));

    guLookAtHilite(&sActorHiliteMtx, lookAt, *hiliteP, correctedEyeX, eye->y, eye->z, object->x, object->y, object->z,
                   0.0f, 1.0f, 0.0f, lightDir->x, lightDir->y, lightDir->z, lightDir->x, lightDir->y, lightDir->z, 0x10,
                   0x10);

    gSPLookAt(dl++, lookAt);
    gDPSetHilite1Tile(dl++, 1, *hiliteP, 0x10, 0x10);

    return dl;
}


Hilite* Hilite_DrawXlu(Vec3f* object, Vec3f* eye, Vec3f* lightDir, GraphicsContext* gfxCtx) {
    Hilite* hilite;

    OPEN_DISPS(gfxCtx, __FILE__, __LINE__);

    POLY_XLU_DISP = Hilite_Draw(object, eye, lightDir, gfxCtx, POLY_XLU_DISP, &hilite);

    CLOSE_DISPS(gfxCtx, __FILE__, __LINE__);

    return hilite;
}


void Matrix_RotateXS(s16 x, MatrixMode mode) {
    MtxF* cmf;
    f32 sin;
    f32 cos;
    f32 tempY;
    f32 tempZ;

    if (mode == MTXMODE_APPLY) {
        if (x != 0) {
            cmf = sCurrentMatrix;

            sin = Math_SinS(x);
            cos = Math_CosS(x);

            tempY = cmf->xy;
            tempZ = cmf->xz;
            cmf->xy = tempY * cos + tempZ * sin;
            cmf->xz = tempZ * cos - tempY * sin;

            tempY = cmf->yy;
            tempZ = cmf->yz;
            cmf->yy = tempY * cos + tempZ * sin;
            cmf->yz = tempZ * cos - tempY * sin;

            tempY = cmf->zy;
            tempZ = cmf->zz;
            cmf->zy = tempY * cos + tempZ * sin;
            cmf->zz = tempZ * cos - tempY * sin;

            tempY = cmf->wy;
            tempZ = cmf->wz;
            cmf->wy = tempY * cos + tempZ * sin;
            cmf->wz = tempZ * cos - tempY * sin;
        }
    } else {
        cmf = sCurrentMatrix;

        if (x != 0) {
            sin = Math_SinS(x);
            cos = Math_CosS(x);
        } else {
            sin = 0.0f;
            cos = 1.0f;
        }

        cmf->yx = 0.0f;
        cmf->zx = 0.0f;
        cmf->wx = 0.0f;
        cmf->xy = 0.0f;
        cmf->wy = 0.0f;
        cmf->xz = 0.0f;
        cmf->wz = 0.0f;
        cmf->xw = 0.0f;
        cmf->yw = 0.0f;
        cmf->zw = 0.0f;
        cmf->xx = 1.0f;
        cmf->ww = 1.0f;
        cmf->yy = cos;
        cmf->zz = cos;
        cmf->zy = sin;
        cmf->yz = -sin;
    }
}


void Matrix_RotateXF(f32 x, MatrixMode mode) {
    MtxF* cmf;
    f32 sin;
    f32 cos;
    f32 tempY;
    f32 tempZ;
    f32 zero = 0.0;
    f32 one = 1.0;

    if (mode == MTXMODE_APPLY) {
        if (x != 0) {
            cmf = sCurrentMatrix;

            sin = sinf(x);
            cos = cosf(x);

            tempY = cmf->xy;
            tempZ = cmf->xz;
            cmf->xy = tempY * cos + tempZ * sin;
            cmf->xz = tempZ * cos - tempY * sin;

            tempY = cmf->yy;
            tempZ = cmf->yz;
            cmf->yy = tempY * cos + tempZ * sin;
            cmf->yz = tempZ * cos - tempY * sin;

            tempY = cmf->zy;
            tempZ = cmf->zz;
            cmf->zy = tempY * cos + tempZ * sin;
            cmf->zz = tempZ * cos - tempY * sin;

            tempY = cmf->wy;
            tempZ = cmf->wz;
            cmf->wy = tempY * cos + tempZ * sin;
            cmf->wz = tempZ * cos - tempY * sin;
        }
    } else {
        cmf = sCurrentMatrix;

        if (x != 0) {
            sin = sinf(x);
            cos = cosf(x);
        } else {
            sin = zero;
            cos = one;
        }

        cmf->xx = one;
        cmf->yx = zero;
        cmf->zx = zero;
        cmf->wx = zero;
        cmf->xy = zero;
        cmf->yy = cos;
        cmf->zy = sin;
        cmf->wy = zero;
        cmf->xz = zero;
        cmf->yz = -sin;
        cmf->zz = cos;
        cmf->wz = zero;
        cmf->xw = zero;
        cmf->yw = zero;
        cmf->zw = zero;
        cmf->ww = one;
    }
}


void Matrix_RotateXFApply(f32 x) {
    MtxF* cmf;
    f32 sin;
    f32 cos;
    f32 tempY;
    f32 tempZ;
    //s32 pad;

    if (x != 0.0f) {
        cmf = sCurrentMatrix;

        sin = sins(RAD_TO_BINANG(x)) * SHT_MINV;
        cos = coss(RAD_TO_BINANG(x)) * SHT_MINV;

        tempY = cmf->xy;
        tempZ = cmf->xz;
        cmf->xy = (tempY * cos) + (tempZ * sin);
        cmf->xz = (tempZ * cos) - (tempY * sin);

        tempY = cmf->yy;
        tempZ = cmf->yz;
        cmf->yy = (tempY * cos) + (tempZ * sin);
        cmf->yz = (tempZ * cos) - (tempY * sin);

        tempY = cmf->zy;
        tempZ = cmf->zz;
        cmf->zy = (tempY * cos) + (tempZ * sin);
        cmf->zz = (tempZ * cos) - (tempY * sin);

        tempY = cmf->wy;
        tempZ = cmf->wz;
        cmf->wy = (tempY * cos) + (tempZ * sin);
        cmf->wz = (tempZ * cos) - (tempY * sin);
    }
}
void Matrix_RotateYS(s16 y, MatrixMode mode) {
    MtxF* cmf;
    f32 sin;
    f32 cos;
    f32 tempX;
    f32 tempZ;

    if (mode == MTXMODE_APPLY) {
        if (y != 0) {
            cmf = sCurrentMatrix;

            sin = Math_SinS(y);
            cos = Math_CosS(y);

            tempX = cmf->xx;
            tempZ = cmf->xz;
            cmf->xx = tempX * cos - tempZ * sin;
            cmf->xz = tempX * sin + tempZ * cos;

            tempX = cmf->yx;
            tempZ = cmf->yz;
            cmf->yx = tempX * cos - tempZ * sin;
            cmf->yz = tempX * sin + tempZ * cos;

            tempX = cmf->zx;
            tempZ = cmf->zz;
            cmf->zx = tempX * cos - tempZ * sin;
            cmf->zz = tempX * sin + tempZ * cos;

            tempX = cmf->wx;
            tempZ = cmf->wz;
            cmf->wx = tempX * cos - tempZ * sin;
            cmf->wz = tempX * sin + tempZ * cos;
        }
    } else {
        cmf = sCurrentMatrix;

        if (y != 0) {
            sin = Math_SinS(y);
            cos = Math_CosS(y);
        } else {
            sin = 0.0f;
            cos = 1.0f;
        }

        cmf->yx = 0.0f;
        cmf->wx = 0.0f;
        cmf->xy = 0.0f;
        cmf->zy = 0.0f;
        cmf->wy = 0.0f;
        cmf->yz = 0.0f;
        cmf->wz = 0.0f;
        cmf->xw = 0.0f;
        cmf->yw = 0.0f;
        cmf->zw = 0.0f;
        cmf->yy = 1.0f;
        cmf->ww = 1.0f;
        cmf->xx = cos;
        cmf->zz = cos;
        cmf->zx = -sin;
        cmf->xz = sin;
    }
}

void Matrix_RotateYF(f32 y, MatrixMode mode) {
    MtxF* cmf;
    f32 sin;
    f32 cos;
    f32 tempX;
    f32 tempZ;
    f32 zero = 0.0;
    f32 one = 1.0;

    if (mode == MTXMODE_APPLY) {
        if (y != 0.0f) {
            cmf = sCurrentMatrix;

            sin = sinf(y);
            cos = cosf(y);

            tempX = cmf->xx;
            tempZ = cmf->xz;
            cmf->xx = tempX * cos - tempZ * sin;
            cmf->xz = tempX * sin + tempZ * cos;

            tempX = cmf->yx;
            tempZ = cmf->yz;
            cmf->yx = tempX * cos - tempZ * sin;
            cmf->yz = tempX * sin + tempZ * cos;

            tempX = cmf->zx;
            tempZ = cmf->zz;
            cmf->zx = tempX * cos - tempZ * sin;
            cmf->zz = tempX * sin + tempZ * cos;

            tempX = cmf->wx;
            tempZ = cmf->wz;
            cmf->wx = tempX * cos - tempZ * sin;
            cmf->wz = tempX * sin + tempZ * cos;
        }
    } else {
        cmf = sCurrentMatrix;

        if (y != 0.0f) {
            sin = sinf(y);
            cos = cosf(y);
        } else {
            cos = one;
            sin = zero;
        }

        cmf->yx = zero;
        cmf->wx = zero;
        cmf->xy = zero;
        cmf->zy = zero;
        cmf->wy = zero;
        cmf->yz = zero;
        cmf->wz = zero;
        cmf->xw = zero;
        cmf->yw = zero;
        cmf->zw = zero;
        cmf->yy = one;
        cmf->ww = one;
        cmf->xx = cos;
        cmf->zz = cos;
        cmf->zx = -sin;
        cmf->xz = sin;
    }
}
void Matrix_RotateZS(s16 z, MatrixMode mode) {
    MtxF* cmf;
    f32 sin;
    f32 cos;
    f32 tempX;
    f32 tempY;
    f32 zero = 0.0;
    f32 one = 1.0;

    if (mode == MTXMODE_APPLY) {
        if (z != 0) {
            cmf = sCurrentMatrix;

            sin = Math_SinS(z);
            cos = Math_CosS(z);

            tempX = cmf->xx;
            tempY = cmf->xy;
            cmf->xx = tempX * cos + tempY * sin;
            cmf->xy = tempY * cos - tempX * sin;

            tempX = cmf->yx;
            tempY = cmf->yy;
            cmf->yx = tempX * cos + tempY * sin;
            cmf->yy = tempY * cos - tempX * sin;

            tempX = cmf->zx;
            tempY = cmf->zy;
            cmf->zx = tempX * cos + tempY * sin;
            cmf->zy = tempY * cos - tempX * sin;

            tempX = cmf->wx;
            tempY = cmf->wy;
            cmf->wx = tempX * cos + tempY * sin;
            cmf->wy = tempY * cos - tempX * sin;
        }
    } else {
        cmf = sCurrentMatrix;

        if (z != 0) {
            sin = Math_SinS(z);
            cos = Math_CosS(z);
        } else {
            sin = zero;
            cos = one;
        }

        cmf->zx = zero;
        cmf->wx = zero;
        cmf->zy = zero;
        cmf->wy = zero;
        cmf->xz = zero;
        cmf->yz = zero;
        cmf->wz = zero;
        cmf->xw = zero;
        cmf->yw = zero;
        cmf->zw = zero;
        cmf->zz = one;
        cmf->ww = one;
        cmf->xx = cos;
        cmf->yy = cos;
        cmf->yx = sin;
        cmf->xy = -sin;
    }
}

void Matrix_RotateZF(f32 z, MatrixMode mode) {
    MtxF* cmf;
    f32 sin;
    f32 cos;
    f32 tempX;
    f32 tempY;

    if (mode == MTXMODE_APPLY) {
        if (z != 0) {
            cmf = sCurrentMatrix;

            sin = sinf(z);
            cos = cosf(z);

            tempX = cmf->xx;
            tempY = cmf->xy;
            cmf->xx = tempX * cos + tempY * sin;
            cmf->xy = tempY * cos - tempX * sin;

            tempX = cmf->yx;
            tempY = cmf->yy;
            cmf->yx = tempX * cos + tempY * sin;
            cmf->yy = tempY * cos - tempX * sin;

            tempX = cmf->zx;
            tempY = cmf->zy;
            cmf->zx = tempX * cos + tempY * sin;
            cmf->zy = tempY * cos - tempX * sin;

            tempX = cmf->wx;
            tempY = cmf->wy;
            cmf->wx = tempX * cos + tempY * sin;
            cmf->wy = tempY * cos - tempX * sin;
        }
    } else {
        cmf = sCurrentMatrix;

        if (z != 0) {
            sin = sinf(z);
            cos = cosf(z);
        } else {
            sin = 0.0f;
            cos = 1.0f;
        }

        cmf->zx = 0.0f;
        cmf->wx = 0.0f;
        cmf->zy = 0.0f;
        cmf->wy = 0.0f;
        cmf->xz = 0.0f;
        cmf->yz = 0.0f;
        cmf->wz = 0.0f;
        cmf->xw = 0.0f;
        cmf->yw = 0.0f;
        cmf->zw = 0.0f;
        cmf->zz = 1.0f;
        cmf->ww = 1.0f;
        cmf->xx = cos;
        cmf->yy = cos;
        cmf->yx = sin;
        cmf->xy = -sin;
    }
}

void Matrix_RotateXFNew(f32 x) {
    MtxF* cmf = sCurrentMatrix;
    //s32 pad[2];
    f32 sin;
    f32 cos;

    cmf->xx = 1.0f;
    cmf->yx = 0.0f;
    cmf->zx = 0.0f;
    cmf->wx = 0.0f;
    cmf->xy = 0.0f;
    cmf->wy = 0.0f;
    cmf->xz = 0.0f;
    cmf->wz = 0.0f;
    cmf->xw = 0.0f;
    cmf->yw = 0.0f;
    cmf->zw = 0.0f;
    cmf->ww = 1.0f;

    if (x != 0.0f) {
        sin = sinf(x);
        cos = cosf(x);

        cmf->yy = cos;
        cmf->zz = cos;
        cmf->yz = -sin;
        cmf->zy = sin;
    } else {
        cmf->yy = 1.0f;
        cmf->zy = 0.0f;
        cmf->yz = 0.0f;
        cmf->zz = 1.0f;
    }
}

void Matrix_MtxFToYXZRot(MtxF* src, Vec3s* dest, s32 nonUniformScale) {
    f32 temp;
    f32 temp2;
    f32 temp3;
    //f32 temp4;

    temp = src->xz;
    temp *= temp;
    temp += SQ(src->zz);
    //dest->x = Math_Atan2S(-src->yz, sqrtf(temp));
    dest->x = Math_Atan2S(sqrtf(temp), -src->yz);

    if ((dest->x == 0x4000) || (dest->x == -0x4000)) {
        // cos(x) = 0 if either of these is true, and we get gimbal locking
        // (https://en.wikipedia.org/wiki/Gimbal_lock#Loss_of_a_degree_of_freedom_with_Euler_angles); fix z to make y
        // well-defined.
        dest->z = 0;

        //dest->y = Math_Atan2S(-src->zx, src->xx);
        dest->y = Math_Atan2S(-src->xx, -src->zx);
    } else {
        //dest->y = Math_Atan2S(src->xz, src->zz);
        dest->y = Math_Atan2S(src->zz, src->xz);

        if (!nonUniformScale) {
            // assume the columns have the same normalisation
            //dest->z = Math_Atan2S(src->yx, src->yy);
            dest->z = Math_Atan2S(src->yy, src->yx);
        } else {
            temp = src->xx;
            temp2 = src->zx;
            temp3 = src->zy;

            // find norm of the first column
            temp *= temp;
            temp += SQ(temp2);
            temp2 = src->yx;
            temp += SQ(temp2);
            // temp = xx^2+zx^2+yx^2 == 1 for a rotation matrix
            temp = sqrtf(temp);
            temp = temp2 / temp; // yx in normalised column

            // find norm of the second column
            temp2 = src->xy;
            temp2 *= temp2;
            temp2 += SQ(temp3);
            temp3 = src->yy;
            temp2 += SQ(temp3);
            // temp2 = xy^2+zy^2+yy^2 == 1 for a rotation matrix
            temp2 = sqrtf(temp2);
            temp2 = temp3 / temp2; // yy in normalised column

            // for a rotation matrix, temp == yx and temp2 == yy which is the same as in the !nonUniformScale branch
            //dest->z = Math_Atan2S(temp, temp2);
            dest->z = Math_Atan2S(temp2, temp);
        }
    }
}

void Matrix_RotateAxisF(f32 angle, Vec3f* axis, MatrixMode mode) {
    MtxF* cmf;
    f32 sin;
    f32 cos;
    f32 versin;
    f32 temp1;
    f32 temp2;
    f32 temp3;
    f32 temp4;
    //f32 temp5;

    if (mode == MTXMODE_APPLY) {
        if (angle != 0) {
            cmf = sCurrentMatrix;

            sin = sinf(angle);
            cos = cosf(angle);

            temp1 = cmf->xx;
            temp2 = cmf->xy;
            temp3 = cmf->xz;
            temp4 = (axis->x * temp1 + axis->y * temp2 + axis->z * temp3) * (1.0f - cos);
            cmf->xx = temp1 * cos + axis->x * temp4 + sin * (temp2 * axis->z - temp3 * axis->y);
            cmf->xy = temp2 * cos + axis->y * temp4 + sin * (temp3 * axis->x - temp1 * axis->z);
            cmf->xz = temp3 * cos + axis->z * temp4 + sin * (temp1 * axis->y - temp2 * axis->x);

            temp1 = cmf->yx;
            temp2 = cmf->yy;
            temp3 = cmf->yz;
            temp4 = (axis->x * temp1 + axis->y * temp2 + axis->z * temp3) * (1.0f - cos);
            cmf->yx = temp1 * cos + axis->x * temp4 + sin * (temp2 * axis->z - temp3 * axis->y);
            cmf->yy = temp2 * cos + axis->y * temp4 + sin * (temp3 * axis->x - temp1 * axis->z);
            cmf->yz = temp3 * cos + axis->z * temp4 + sin * (temp1 * axis->y - temp2 * axis->x);

            temp1 = cmf->zx;
            temp2 = cmf->zy;
            temp3 = cmf->zz;
            temp4 = (axis->x * temp1 + axis->y * temp2 + axis->z * temp3) * (1.0f - cos);
            cmf->zx = temp1 * cos + axis->x * temp4 + sin * (temp2 * axis->z - temp3 * axis->y);
            cmf->zy = temp2 * cos + axis->y * temp4 + sin * (temp3 * axis->x - temp1 * axis->z);
            cmf->zz = temp3 * cos + axis->z * temp4 + sin * (temp1 * axis->y - temp2 * axis->x);
        }
    } else {
        cmf = sCurrentMatrix;

        if (angle != 0) {
            sin = sinf(angle);
            cos = cosf(angle);
            versin = 1.0f - cos;

            cmf->xx = axis->x * axis->x * versin + cos;
            cmf->yy = axis->y * axis->y * versin + cos;
            cmf->zz = axis->z * axis->z * versin + cos;

            if (0) {}

            temp2 = axis->x * versin * axis->y;
            temp3 = axis->z * sin;
            cmf->yx = temp2 + temp3;
            cmf->xy = temp2 - temp3;

            temp2 = axis->x * versin * axis->z;
            temp3 = axis->y * sin;
            cmf->zx = temp2 - temp3;
            cmf->xz = temp2 + temp3;

            temp2 = axis->y * versin * axis->z;
            temp3 = axis->x * sin;
            cmf->zy = temp2 + temp3;
            cmf->yz = temp2 - temp3;

            cmf->wx = cmf->wy = cmf->wz = cmf->xw = cmf->yw = cmf->zw = 0.0f;
            cmf->ww = 1.0f;
        } else {
            cmf->xx = 1.0f;
            cmf->yx = 0.0f;
            cmf->zx = 0.0f;
            cmf->wx = 0.0f;
            cmf->xy = 0.0f;
            cmf->yy = 1.0f;
            cmf->zy = 0.0f;
            cmf->wy = 0.0f;
            cmf->xz = 0.0f;
            cmf->yz = 0.0f;
            cmf->zz = 1.0f;
            cmf->wz = 0.0f;
            cmf->xw = 0.0f;
            cmf->yw = 0.0f;
            cmf->zw = 0.0f;
            cmf->ww = 1.0f;
        }
    }
}

void Matrix_TranslateRotateZYX(Vec3f* translation, Vec3s* rot) {
    MtxF* cmf = sCurrentMatrix;
    f32 sin = Math_SinS(rot->z);
    f32 cos = Math_CosS(rot->z);
    f32 temp1;
    f32 temp2;

    // No check for z != 0, presumably since translation is interleaved.
    temp1 = cmf->xx;
    temp2 = cmf->xy;
    cmf->xw += temp1 * translation->x + temp2 * translation->y + cmf->xz * translation->z;
    cmf->xx = temp1 * cos + temp2 * sin;
    cmf->xy = temp2 * cos - temp1 * sin;

    temp1 = cmf->yx;
    temp2 = cmf->yy;
    cmf->yw += temp1 * translation->x + temp2 * translation->y + cmf->yz * translation->z;
    cmf->yx = temp1 * cos + temp2 * sin;
    cmf->yy = temp2 * cos - temp1 * sin;

    temp1 = cmf->zx;
    temp2 = cmf->zy;
    cmf->zw += temp1 * translation->x + temp2 * translation->y + cmf->zz * translation->z;
    cmf->zx = temp1 * cos + temp2 * sin;
    cmf->zy = temp2 * cos - temp1 * sin;

    temp1 = cmf->wx;
    temp2 = cmf->wy;
    cmf->ww += temp1 * translation->x + temp2 * translation->y + cmf->wz * translation->z;
    cmf->wx = temp1 * cos + temp2 * sin;
    cmf->wy = temp2 * cos - temp1 * sin;

    if (rot->y != 0) {
        sin = Math_SinS(rot->y);
        cos = Math_CosS(rot->y);

        temp1 = cmf->xx;
        temp2 = cmf->xz;
        cmf->xx = temp1 * cos - temp2 * sin;
        cmf->xz = temp1 * sin + temp2 * cos;

        temp1 = cmf->yx;
        temp2 = cmf->yz;
        cmf->yx = temp1 * cos - temp2 * sin;
        cmf->yz = temp1 * sin + temp2 * cos;

        temp1 = cmf->zx;
        temp2 = cmf->zz;
        cmf->zx = temp1 * cos - temp2 * sin;
        cmf->zz = temp1 * sin + temp2 * cos;

        temp1 = cmf->wx;
        temp2 = cmf->wz;
        cmf->wx = temp1 * cos - temp2 * sin;
        cmf->wz = temp1 * sin + temp2 * cos;
    }

    if (rot->x != 0) {
        sin = Math_SinS(rot->x);
        cos = Math_CosS(rot->x);

        temp1 = cmf->xy;
        temp2 = cmf->xz;
        cmf->xy = temp1 * cos + temp2 * sin;
        cmf->xz = temp2 * cos - temp1 * sin;

        temp1 = cmf->yy;
        temp2 = cmf->yz;
        cmf->yy = temp1 * cos + temp2 * sin;
        cmf->yz = temp2 * cos - temp1 * sin;

        temp1 = cmf->zy;
        temp2 = cmf->zz;
        cmf->zy = temp1 * cos + temp2 * sin;
        cmf->zz = temp2 * cos - temp1 * sin;

        temp1 = cmf->wy;
        temp2 = cmf->wz;
        cmf->wy = temp1 * cos + temp2 * sin;
        cmf->wz = temp2 * cos - temp1 * sin;
    }
}

/**
 * @brief Set current to a general translation and rotation using YXZ Tait-Bryan angles: T Ry Rx Rz -> current
 *
 * This means a (column) vector is first rotated around Y, then around X, then around Z, then translated, then gets
 * transformed by whatever the matrix was previously.
 *
 * @param x amount to translate in X direction.
 * @param y amount to translate in Y direction.
 * @param z amount to translate in Z direction.
 * @param rot vector of rotation angles.
 *
 * @remark original name appears to be "Matrix_softcv3_load"
 */
void Matrix_SetTranslateRotateYXZ(f32 x, f32 y, f32 z, Vec3s* rot) {
    MtxF* cmf = sCurrentMatrix;
    f32 sinY = Math_SinS(rot->y);
    f32 cosY = Math_CosS(rot->y);
    f32 cosTemp;
    f32 sinTemp;

    cmf->xx = cosY;
    cmf->zx = -sinY;
    cmf->xw = x;
    cmf->yw = y;
    cmf->zw = z;
    cmf->wx = 0.0f;
    cmf->wy = 0.0f;
    cmf->wz = 0.0f;
    cmf->ww = 1.0f;

    if (rot->x != 0) {
        sinTemp = Math_SinS(rot->x);
        cosTemp = Math_CosS(rot->x);

        cmf->zz = cosY * cosTemp;
        cmf->zy = cosY * sinTemp;
        cmf->xz = sinY * cosTemp;
        cmf->xy = sinY * sinTemp;
        cmf->yz = -sinTemp;
        cmf->yy = cosTemp;
    } else {
        cmf->zz = cosY;
        cmf->xz = sinY;
        cmf->yz = 0.0f;
        cmf->zy = 0.0f;
        cmf->xy = 0.0f;
        cmf->yy = 1.0f;
    }

    if (rot->z != 0) {
        sinTemp = Math_SinS(rot->z);
        cosTemp = Math_CosS(rot->z);

        sinY = cmf->xx;
        cosY = cmf->xy;
        cmf->xx = sinY * cosTemp + cosY * sinTemp;
        cmf->xy = cosY * cosTemp - sinY * sinTemp;

        sinY = cmf->zx;
        cosY = cmf->zy;
        cmf->zx = sinY * cosTemp + cosY * sinTemp;
        cmf->zy = cosY * cosTemp - sinY * sinTemp;

        cosY = cmf->yy;
        cmf->yx = cosY * sinTemp;
        cmf->yy = cosY * cosTemp;
    } else {
        cmf->yx = 0.0f;
    }
}



// Match the OoT implementation of Math_Atan2S
s16 Math_Atan2S_XY(f32 x, f32 y) {
    return Math_Atan2S(y, x);
}



typedef struct {
    /* 0x0 */ s8 segment;
    /* 0x2 */ s16 type;
    /* 0x4 */ void* params;
} AnimatedMaterial; // size = 0x8

typedef struct {
    /* 0x0 */ u8 r;
    /* 0x1 */ u8 g;
    /* 0x2 */ u8 b;
    /* 0x3 */ u8 a;
    /* 0x4 */ u8 lodFrac;
} F3DPrimColor; // size = 0x5

typedef struct {
    /* 0x0 */ u8 r;
    /* 0x1 */ u8 g;
    /* 0x2 */ u8 b;
    /* 0x3 */ u8 a;
} F3DEnvColor; // size = 0x4

typedef struct {
    /* 0x0 */ u16 keyFrameLength;
    /* 0x2 */ u16 keyFrameCount;
    /* 0x4 */ F3DPrimColor* primColors;
    /* 0x8 */ F3DEnvColor* envColors;
    /* 0xC */ u16* keyFrames;
} AnimatedMatColorParams; // size = 0x10

typedef struct {
    /* 0x0 */ s8 xStep;
    /* 0x1 */ s8 yStep;
    /* 0x2 */ u8 width;
    /* 0x3 */ u8 height;
} AnimatedMatTexScrollParams; // size = 0x4

typedef struct {
    /* 0x0 */ u16 keyFrameLength;
    /* 0x4 */ TexturePtr* textureList;
    /* 0x8 */ u8* textureIndexList;
} AnimatedMatTexCycleParams; // size = 0xC


s32 sMatAnimStep;
u32 sMatAnimFlags;
f32 sMatAnimAlphaRatio;



extern Gfx D_801AEF88[];
extern Gfx D_801AEFA0[];


typedef enum {
    /*  0 */ ACTOR_DRAW_DMGEFF_FIRE,
    /*  1 */ ACTOR_DRAW_DMGEFF_BLUE_FIRE,
    /* 10 */ ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX = 10,
    /* 11 */ ACTOR_DRAW_DMGEFF_FROZEN_SFX,
    /* 20 */ ACTOR_DRAW_DMGEFF_LIGHT_ORBS = 20,
    /* 21 */ ACTOR_DRAW_DMGEFF_BLUE_LIGHT_ORBS,
    /* 30 */ ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_SMALL = 30,
    /* 31 */ ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_MEDIUM,
    /* 32 */ ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_LARGE
} ActorDrawDamageEffectType;



u64 gEffFireMask1Tex[] = {
#include "eff_fire_mask_1.i4.inc.h"
};

u64 gEffFireMask2Tex[] = {
#include "eff_fire_mask_2.i4.inc.h"
};

u64 gEffIceFragmentTex[] = {
#include "eff_ice_fragment.i8.inc.h"
};

u64 gElectricSpark1Tex[] = {
#include "electric_spark_1.i4.inc.h"
};

u64 gElectricSpark2Tex[] = {
#include "electric_spark_2.i4.inc.h"
};

u64 gElectricSpark3Tex[] = {
#include "electric_spark_3.i4.inc.h"
};

u64 gElectricSpark4Tex[] = {
#include "electric_spark_4.i4.inc.h"
};

u64 gFlashTex[] = {
#include "flash.i8.inc.h"
};

Vtx gameplay_keepVtx_0502C0[] = {
#include "gameplay_keepVtx_0502C0.vtx.inc"
};

Vtx gameplay_keepVtx_050740[] = {
#include "gameplay_keepVtx_050740.vtx.inc"
};

u64 gEffIceSmokeTex[] = {
#include "eff_ice_smoke.i4.inc.h"
};

Vtx gameplay_keepVtx_051080[] = {
#include "gameplay_keepVtx_051080.vtx.inc"
};

Vtx gameplay_keepVtx_023440[] = {
#include "gameplay_keepVtx_023440.vtx.inc"
};

Vtx gameplay_keepVtx_023308[] = {
#include "gameplay_keepVtx_023308.vtx.inc"
};

Gfx gEffIceFragment1DL[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0x0FA0, 0x0FA0, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(gEffIceFragmentTex, G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP,
                         G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD, 15),
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_I, G_IM_SIZ_8b, 4, 0x0000, 1, 0, G_TX_NOMIRROR | G_TX_WRAP, 5, G_TX_NOLOD, G_TX_NOMIRROR |
                G_TX_WRAP, 5, 1),
    gsDPSetTileSize(1, 0, 0, 0x007C, 0x007C),
    gsDPSetCombineLERP(TEXEL1, PRIMITIVE, PRIM_LOD_FRAC, TEXEL0, TEXEL1, TEXEL0, PRIM_LOD_FRAC, TEXEL0, PRIMITIVE,
                       ENVIRONMENT, COMBINED, ENVIRONMENT, COMBINED, 0, ENVIRONMENT, 0),
    gsDPSetRenderMode(G_RM_PASS, G_RM_AA_ZB_XLU_SURF2),
    gsSPClearGeometryMode(G_CULL_BACK | G_FOG),
    gsSPSetGeometryMode(G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsDPSetPrimColor(0, 0x80, 170, 255, 255, 255),
    gsSPDisplayList(0x08000000),
    gsSPVertex(gameplay_keepVtx_0502C0, 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSPVertex(&gameplay_keepVtx_0502C0[3], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(6, 7, 8, 0, 9, 10, 11, 0),
    gsSP2Triangles(12, 13, 14, 0, 15, 16, 17, 0),
    gsSP2Triangles(18, 19, 20, 0, 21, 22, 23, 0),
    gsSP2Triangles(24, 25, 26, 0, 27, 28, 29, 0),
    gsSPVertex(&gameplay_keepVtx_0502C0[33], 8, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 0, 4, 0),
    gsSP1Triangle(5, 6, 7, 0),
    gsSPEndDisplayList(),
};

Gfx gEffIceFragment2MaterialDL[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0x0FA0, 0x0FA0, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(gEffIceFragmentTex, G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP,
                         G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD, 15),
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_I, G_IM_SIZ_8b, 4, 0x0000, 1, 0, G_TX_NOMIRROR | G_TX_WRAP, 5, G_TX_NOLOD, G_TX_NOMIRROR |
                G_TX_WRAP, 5, 1),
    gsDPSetTileSize(1, 0, 0, 0x007C, 0x007C),
    gsDPSetCombineLERP(TEXEL1, PRIMITIVE, PRIM_LOD_FRAC, TEXEL0, TEXEL1, TEXEL0, PRIM_LOD_FRAC, TEXEL0, PRIMITIVE,
                       ENVIRONMENT, COMBINED, ENVIRONMENT, COMBINED, 0, ENVIRONMENT, 0),
    gsDPSetRenderMode(G_RM_PASS, G_RM_AA_ZB_XLU_SURF2),
    gsSPClearGeometryMode(G_CULL_BACK | G_FOG),
    gsSPSetGeometryMode(G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPDisplayList(0x08000000),
    gsSPEndDisplayList(),
};

Gfx gEffIceFragment2ModelDL[] = {
    gsSPVertex(gameplay_keepVtx_0502C0, 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSPVertex(&gameplay_keepVtx_0502C0[3], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(6, 7, 8, 0, 9, 10, 11, 0),
    gsSP2Triangles(12, 13, 14, 0, 15, 16, 17, 0),
    gsSP2Triangles(18, 19, 20, 0, 21, 22, 23, 0),
    gsSP2Triangles(24, 25, 26, 0, 27, 28, 29, 0),
    gsSPVertex(&gameplay_keepVtx_0502C0[33], 8, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 0, 4, 0),
    gsSP1Triangle(5, 6, 7, 0),
    gsSPEndDisplayList(),
};

Gfx gEffIceFragment3DL[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0x0FA0, 0x0FA0, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(gEffIceFragmentTex, G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP,
                         G_TX_NOMIRROR | G_TX_WRAP, 5, 5, 1, G_TX_NOLOD),
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_I, G_IM_SIZ_8b, 4, 0x0000, 1, 0, G_TX_NOMIRROR | G_TX_WRAP, 5, 1, G_TX_NOMIRROR | G_TX_WRAP, 5,
                2),
    gsDPSetTileSize(1, 0, 0, 0x007C, 0x007C),
    gsDPSetCombineLERP(TEXEL1, PRIMITIVE, PRIM_LOD_FRAC, TEXEL0, TEXEL1, TEXEL0, PRIM_LOD_FRAC, TEXEL0, PRIMITIVE,
                       ENVIRONMENT, COMBINED, ENVIRONMENT, COMBINED, 0, ENVIRONMENT, 0),
    gsDPSetRenderMode(G_RM_PASS, G_RM_AA_ZB_XLU_SURF2),
    gsSPClearGeometryMode(G_CULL_BACK | G_FOG),
    gsSPSetGeometryMode(G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsDPSetPrimColor(0x80, 0x80, 170, 255, 255, 255),
    gsSPDisplayList(0x08000000),
    gsSPVertex(gameplay_keepVtx_050740, 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSPVertex(&gameplay_keepVtx_050740[3], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(3, 5, 6, 0, 7, 8, 9, 0),
    gsSP2Triangles(10, 11, 12, 0, 13, 14, 15, 0),
    gsSP2Triangles(13, 15, 16, 0, 17, 18, 19, 0),
    gsSP2Triangles(17, 19, 20, 0, 21, 22, 23, 0),
    gsSP2Triangles(24, 25, 26, 0, 24, 2, 1, 0),
    gsSP2Triangles(27, 28, 29, 0, 27, 29, 30, 0),
    gsSPVertex(&gameplay_keepVtx_050740[34], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0),
    gsSP2Triangles(8, 9, 10, 0, 8, 10, 11, 0),
    gsSP2Triangles(12, 13, 14, 0, 12, 14, 15, 0),
    gsSP2Triangles(16, 17, 18, 0, 16, 18, 19, 0),
    gsSP2Triangles(20, 17, 21, 0, 22, 23, 24, 0),
    gsSP2Triangles(25, 24, 26, 0, 27, 28, 29, 0),
    gsSP1Triangle(27, 29, 30, 0),
    gsSPVertex(&gameplay_keepVtx_050740[65], 28, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 1, 4, 0),
    gsSP2Triangles(5, 6, 7, 0, 5, 8, 9, 0),
    gsSP2Triangles(10, 11, 12, 0, 13, 14, 15, 0),
    gsSP2Triangles(4, 16, 3, 0, 17, 18, 19, 0),
    gsSP2Triangles(9, 20, 5, 0, 21, 22, 23, 0),
    gsSP2Triangles(24, 25, 26, 0, 0, 2, 27, 0),
    gsSPEndDisplayList(),
};

Gfx gEffIceSmokeDL[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock_4b(gEffFireMask1Tex, G_IM_FMT_I, 32, 64, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR |
                            G_TX_WRAP, 5, 6, G_TX_NOLOD, 15),
    gsDPLoadMultiBlock_4b(gEffIceSmokeTex, 0x0100, 1, G_IM_FMT_I, 32, 32, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR |
                          G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineLERP(TEXEL0, 0, ENV_ALPHA, TEXEL1, TEXEL0, 1, ENVIRONMENT, TEXEL1, PRIMITIVE, ENVIRONMENT, COMBINED,
                       ENVIRONMENT, COMBINED, 0, PRIMITIVE, 0),
    gsDPSetRenderMode(G_RM_PASS, G_RM_ZB_CLD_SURF2),
    gsSPClearGeometryMode(G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPSetGeometryMode(G_CULL_BACK),
    gsDPSetEnvColor(255, 255, 255, 128),
    gsSPDisplayList(0x08000000),
    gsSPVertex(gameplay_keepVtx_051080, 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSPEndDisplayList(),
};

Gfx gFrozenSteamMaterialDL[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock_4b(gEffFireMask1Tex, G_IM_FMT_I, 32, 64, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR |
                            G_TX_WRAP, 5, 6, G_TX_NOLOD, 15),
    gsDPLoadMultiBlock_4b(gEffIceSmokeTex, 0x0100, 1, G_IM_FMT_I, 32, 32, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR |
                          G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineLERP(TEXEL0, 0, ENV_ALPHA, TEXEL1, TEXEL0, 1, ENVIRONMENT, TEXEL1, PRIMITIVE, ENVIRONMENT, COMBINED,
                       ENVIRONMENT, COMBINED, 0, PRIMITIVE, 0),
    gsDPSetRenderMode(G_RM_PASS, G_RM_ZB_CLD_SURF2),
    gsSPClearGeometryMode(G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPSetGeometryMode(G_CULL_BACK),
    gsDPSetEnvColor(255, 255, 255, 128),
    gsSPEndDisplayList(),
};

Gfx gFrozenSteamModelDL[] = {
    gsSPDisplayList(0x08000000),
    gsSPVertex(gameplay_keepVtx_051080, 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSPEndDisplayList(),
};

Gfx gElectricSparkMaterialDL[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock_4b(0x08000000, G_IM_FMT_I, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP,
                            G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineLERP(PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED, 0, 0, 0,
                       COMBINED),
    gsDPSetRenderMode(G_RM_PASS, G_RM_ZB_OVL_SURF2),
    gsSPClearGeometryMode(G_CULL_BACK | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPEndDisplayList(),
};

Gfx gElectricSparkModelDL[] = {
    gsSPVertex(gameplay_keepVtx_023440, 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 1, 3, 2, 0),
    gsSPEndDisplayList(),
};

Gfx gLightOrbMaterial1DL[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(gFlashTex, G_IM_FMT_I, G_IM_SIZ_8b, 64, 64, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR |
                         G_TX_CLAMP, 6, 6, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineLERP(PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED, 0, 0, 0,
                       COMBINED),
    gsDPSetRenderMode(G_RM_PASS, G_RM_ZB_CLD_SURF2),
    gsSPClearGeometryMode(G_CULL_BACK | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPEndDisplayList(),
};

Gfx gLightOrbMaterial2DL[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(gFlashTex, G_IM_FMT_I, G_IM_SIZ_8b, 64, 64, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR |
                         G_TX_CLAMP, 6, 6, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineLERP(PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED, 0, 0, 0,
                       COMBINED),
    gsDPSetRenderMode(G_RM_PASS, G_RM_AA_ZB_XLU_DECAL2),
    gsSPClearGeometryMode(G_CULL_BACK | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPEndDisplayList(),
};

Gfx gLightOrbModelDL[] = {
    gsSPVertex(gameplay_keepVtx_023308, 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSPEndDisplayList(),
};


extern MtxF* sCurrentMatrix; //!< original name: "Matrix_now"


Hilite* func_800BCC68(Vec3f* arg0, PlayState* play) {
    Vec3f lightDir;

    lightDir.x = play->envCtx.dirLight1.params.dir.x;
    lightDir.y = play->envCtx.dirLight1.params.dir.y;
    lightDir.z = play->envCtx.dirLight1.params.dir.z;

    return Hilite_DrawXlu(arg0, &play->view.eye, &lightDir, play->state.gfxCtx);
}


TexturePtr sElectricSparkTextures[] = {
    gElectricSpark1Tex,
    gElectricSpark2Tex,
    gElectricSpark3Tex,
    gElectricSpark4Tex,
};

/**
 * Draw common damage effects applied to each body part provided in bodyPartsPos
 */
void Actor_DrawDamageEffects(PlayState* play, Actor* actor, Vec3f bodyPartsPos[], s16 bodyPartsCount, f32 effectScale,
                             f32 frozenSteamScale, f32 effectAlpha, u8 type) {
    if (effectAlpha > 0.001f) {
        s32 twoTexScrollParam;
        s16 bodyPartIndex;
        MtxF* currentMatrix;
        f32 alpha;
        f32 frozenScale;
        f32 lightOrbsScale;
        f32 electricSparksScale;
        f32 steamScale;
        Vec3f* bodyPartsPosStart = bodyPartsPos;
        u32 gameplayFrames = play->gameplayFrames;
        f32 effectAlphaScaled;

        currentMatrix = Matrix_GetCurrent();

        // Apply sfx along with damage effect
        if ((actor != NULL) && (effectAlpha > 0.05f) && (play->gameOverCtx.state == GAMEOVER_INACTIVE)) {
            if (type == ACTOR_DRAW_DMGEFF_FIRE) {
                Audio_PlayActorSfx2(actor, NA_SE_EV_BURN_OUT - SFX_FLAG);
            } else if (type == ACTOR_DRAW_DMGEFF_BLUE_FIRE) {
                //Audio_PlayActorSfx2(actor, NA_SE_EN_EXTINCT_LOOP - SFX_FLAG);
                SoundEffect_PlayHeld(SOUND_MM_ENEMY_EXTINCT_LOOP, 0.5f, 1.0f, 1.0f, &actor->world.pos, 0.0f, 1000.0f, &actor);
            } else if (type == ACTOR_DRAW_DMGEFF_FROZEN_SFX) {
                Audio_PlayActorSfx2(actor, NA_SE_EV_ICE_FREEZE - SFX_FLAG);
            } else if ((type == ACTOR_DRAW_DMGEFF_LIGHT_ORBS) || (type == ACTOR_DRAW_DMGEFF_BLUE_LIGHT_ORBS)) {
                //Audio_PlayActorSfx2(actor, NA_SE_EN_COMMON_DEADLIGHT - SFX_FLAG);
                SoundEffect_PlayHeld(SOUND_MM_LIGHT_DAMAGE, 1.0f, 1.0f, 1.0f, &actor->world.pos, 0.0f, 1000.0f, &actor);
            }
        }

        OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

        Gfx_SetupDL_25Xlu(play->state.gfxCtx);

        switch (type) {
            case ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX:
            case ACTOR_DRAW_DMGEFF_FROZEN_SFX:
                frozenScale = ((KREG(19) * 0.01f) + 2.3f) * effectScale;
                steamScale = ((KREG(28) * 0.0001f) + 0.035f) * frozenSteamScale;
                func_800BCC68(bodyPartsPos, play);

                // Setup to draw ice over frozen actor

                gSPSegment(POLY_XLU_DISP++, 0x08,
                           Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, gameplayFrames & 0xFF, 32, 16, 1, 0,
                                            (gameplayFrames * 2) & 0xFF, 64, 32));
                gDPSetPrimColor(POLY_XLU_DISP++, 0, 0x80, 170, 255, 255, 255);
                gSPDisplayList(POLY_XLU_DISP++, gEffIceFragment2MaterialDL);

                effectAlphaScaled = effectAlpha * 255.0f;

                // Apply and draw ice over each body part of frozen actor
                for (bodyPartIndex = 0; bodyPartIndex < bodyPartsCount; bodyPartIndex++, bodyPartsPos++) {
                    alpha = bodyPartIndex & 3;
                    alpha = effectAlphaScaled - (30.0f * alpha);
                    if (effectAlphaScaled < (30.0f * (bodyPartIndex & 3))) {
                        alpha = 0.0f;
                    }
                    if (alpha > 255.0f) {
                        alpha = 255.0f;
                    }

                    gDPSetEnvColor(POLY_XLU_DISP++, KREG(20) + 200, KREG(21) + 200, KREG(22) + 255, (u8)alpha);

                    Matrix_Translate(bodyPartsPos->x, bodyPartsPos->y, bodyPartsPos->z, MTXMODE_NEW);
                    Matrix_Scale(frozenScale, frozenScale, frozenScale, MTXMODE_APPLY);

                    if (bodyPartIndex & 1) {
                        Matrix_RotateYF(M_PI, MTXMODE_APPLY);
                    }

                    if (bodyPartIndex & 2) {
                        Matrix_RotateZF(M_PI, MTXMODE_APPLY);
                    }

                    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__),
                              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

                    gSPDisplayList(POLY_XLU_DISP++, gEffIceFragment2ModelDL);
                }

                bodyPartsPos = bodyPartsPosStart; // reset bodyPartsPos

                // Setup to draw steam over frozen actor

                gDPSetColorDither(POLY_XLU_DISP++, G_CD_BAYER);
                gDPSetAlphaDither(POLY_XLU_DISP++, G_AD_PATTERN);
                gSPDisplayList(POLY_XLU_DISP++, gFrozenSteamMaterialDL);

                alpha = effectAlpha * 100.0f;
                if (alpha > 100.0f) {
                    alpha = 100.0f;
                }

                gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 195, 225, 235, (u8)alpha);

                // Apply and draw steam over each body part of frozen actor
                for (bodyPartIndex = 0; bodyPartIndex < bodyPartsCount; bodyPartIndex++, bodyPartsPos++) {
                    twoTexScrollParam = ((bodyPartIndex * 3) + gameplayFrames);
                    gSPSegment(POLY_XLU_DISP++, 0x08,
                               Gfx_TwoTexScroll(play->state.gfxCtx, 0, twoTexScrollParam * 3, twoTexScrollParam * -12,
                                                32, 64, 1, 0, 0, 32, 32));

                    Matrix_Translate(bodyPartsPos->x, bodyPartsPos->y, bodyPartsPos->z, MTXMODE_NEW);
                    Matrix_ReplaceRotation(&play->billboardMtxF);
                    Matrix_Scale(steamScale, steamScale, 1.0f, MTXMODE_APPLY);

                    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__),
                              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

                    gSPDisplayList(POLY_XLU_DISP++, gFrozenSteamModelDL);
                }
                break;

            case ACTOR_DRAW_DMGEFF_FIRE:
            case ACTOR_DRAW_DMGEFF_BLUE_FIRE:
                if (type == ACTOR_DRAW_DMGEFF_FIRE) {
                    gDPSetEnvColor(POLY_XLU_DISP++, 255, 10, 0, 0);
                } else {
                    gDPSetEnvColor(POLY_XLU_DISP++, 0, 255, 255, 0);
                    // Reuse type for blue primitive color
                    type = 255;
                }

                Matrix_Put(&play->billboardMtxF);
                Matrix_Scale((effectScale * 0.005f) * 1.35f, (effectScale * 0.005f), (effectScale * 0.005f) * 1.35f,
                             MTXMODE_APPLY);

                effectAlphaScaled = effectAlpha * 255.0f;

                // Apply and draw fire on every body part
                for (bodyPartIndex = 0; bodyPartIndex < bodyPartsCount; bodyPartIndex++, bodyPartsPos++) {
                    alpha = bodyPartIndex & 3;
                    alpha = effectAlphaScaled - 30.0f * alpha;
                    if (effectAlphaScaled < 30.0f * (bodyPartIndex & 3)) {
                        alpha = 0.0f;
                    }
                    if (alpha > 255.0f) {
                        alpha = 255.0f;
                    }

                    // Use type for blue primitive color
                    // = 0 for ACTOR_DRAW_DMGEFF_FIRE
                    // = 255 for ACTOR_DRAW_DMGEFF_BLUE_FIRE
                    gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, 255, 255, type, (u8)alpha);

                    gSPSegment(POLY_XLU_DISP++, 0x08,
                               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, 0, 32, 64, 1, 0,
                                                ((bodyPartIndex * 10 + gameplayFrames) * -20) & 0x1FF, 32, 128));

                    Matrix_RotateYF(M_PI, MTXMODE_APPLY);
                    currentMatrix->mf[3][0] = bodyPartsPos->x;
                    currentMatrix->mf[3][1] = bodyPartsPos->y;
                    currentMatrix->mf[3][2] = bodyPartsPos->z;

                    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__),
                              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

                    gSPDisplayList(POLY_XLU_DISP++, gEffFire1DL);
                }
                break;

            case ACTOR_DRAW_DMGEFF_LIGHT_ORBS:
            case ACTOR_DRAW_DMGEFF_BLUE_LIGHT_ORBS:

                // Setup to draw light orbs on actor

                lightOrbsScale = ((KREG(19) * 0.01f) + 4.0f) * effectScale;

                gSPDisplayList(POLY_XLU_DISP++, gLightOrbMaterial1DL);

                alpha = effectAlpha * 255.0f;
                if (alpha > 255.0f) {
                    alpha = 255.0f;
                }

                if (type == ACTOR_DRAW_DMGEFF_BLUE_LIGHT_ORBS) {
                    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, (u8)(sREG(16) + 255), (u8)(sREG(17) + 255),
                                    (u8)(sREG(18) + 255), (u8)alpha);

                    gDPSetEnvColor(POLY_XLU_DISP++, (u8)sREG(19), (u8)(sREG(20) + 255), (u8)(sREG(21) + 255), 128);
                } else {
                    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 200, (u8)alpha);

                    gDPSetEnvColor(POLY_XLU_DISP++, 255, 255, 100, 128);
                }

                Matrix_Put(&play->billboardMtxF);
                Matrix_Scale(lightOrbsScale, lightOrbsScale, 1.0f, MTXMODE_APPLY);

                // Apply and draw a light orb over each body part of frozen actor
                for (bodyPartIndex = 0; bodyPartIndex < bodyPartsCount; bodyPartIndex++, bodyPartsPos++) {
                    Matrix_RotateZF(Rand_CenteredFloat(2 * M_PI), MTXMODE_APPLY);
                    currentMatrix->mf[3][0] = bodyPartsPos->x;
                    currentMatrix->mf[3][1] = bodyPartsPos->y;
                    currentMatrix->mf[3][2] = bodyPartsPos->z;

                    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__),
                              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

                    gSPDisplayList(POLY_XLU_DISP++, gLightOrbModelDL);
                }
                break;

            case ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_SMALL:
            case ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_MEDIUM:
            case ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_LARGE:
                if (type == ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_SMALL) {
                    electricSparksScale = (KREG(19) * 0.01f + 1.0f) * effectScale;
                } else if (type == ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_MEDIUM) {
                    electricSparksScale = (KREG(19) * 0.01f + 1.5f) * effectScale;
                } else {
                    electricSparksScale = (KREG(19) * 0.01f + 2.0f) * effectScale;
                }

                gSPSegment(POLY_XLU_DISP++, 0x08,
                           Lib_SegmentedToVirtual(sElectricSparkTextures[play->gameplayFrames % 4]));

                gSPDisplayList(POLY_XLU_DISP++, gElectricSparkMaterialDL);

                gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, (u8)(sREG(16) + 255), (u8)(sREG(17) + 255), (u8)(sREG(18) + 150),
                                (u8)(sREG(19) + 255));

                gDPSetEnvColor(POLY_XLU_DISP++, (u8)(sREG(20) + 255), (u8)(sREG(21) + 255), (u8)sREG(22), (u8)sREG(23));

                Matrix_Put(&play->billboardMtxF);
                Matrix_Scale(electricSparksScale, electricSparksScale, electricSparksScale, MTXMODE_APPLY);

                // Every body part draws two electric sparks at random orientations
                for (bodyPartIndex = 0; bodyPartIndex < bodyPartsCount; bodyPartIndex++, bodyPartsPos++) {
                    // first electric spark
                    Matrix_RotateXFApply(Rand_ZeroFloat(2 * M_PI));
                    Matrix_RotateZF(Rand_ZeroFloat(2 * M_PI), MTXMODE_APPLY);
                    currentMatrix->mf[3][0] = Rand_CenteredFloat((f32)sREG(24) + 30.0f) + bodyPartsPos->x;
                    currentMatrix->mf[3][1] = Rand_CenteredFloat((f32)sREG(24) + 30.0f) + bodyPartsPos->y;
                    currentMatrix->mf[3][2] = Rand_CenteredFloat((f32)sREG(24) + 30.0f) + bodyPartsPos->z;

                    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__),
                              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

                    gSPDisplayList(POLY_XLU_DISP++, gElectricSparkModelDL);

                    // second electric spark
                    Matrix_RotateXFApply(Rand_ZeroFloat(2 * M_PI));
                    Matrix_RotateZF(Rand_ZeroFloat(2 * M_PI), MTXMODE_APPLY);
                    currentMatrix->mf[3][0] = Rand_CenteredFloat((f32)sREG(24) + 30.0f) + bodyPartsPos->x;
                    currentMatrix->mf[3][1] = Rand_CenteredFloat((f32)sREG(24) + 30.0f) + bodyPartsPos->y;
                    currentMatrix->mf[3][2] = Rand_CenteredFloat((f32)sREG(24) + 30.0f) + bodyPartsPos->z;

                    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__),
                              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

                    gSPDisplayList(POLY_XLU_DISP++, gElectricSparkModelDL);
                }

                break;
        }

        CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
    }
}


void Actor_SpawnIceEffects(PlayState* play, Actor* actor, Vec3f limbPos[], s32 limbPosCount, s32 effectsPerLimb,
                           f32 scale, f32 scaleRange) {
    static Color_RGBA8 primColor = { 170, 255, 255, 255 };
    static Color_RGBA8 envColor = { 200, 200, 255, 255 };
    static Vec3f accel = { 0.0f, -1.0f, 0.0f };
    s32 i;
    //s32 pad;
    Vec3f velocity;
    s16 randomYaw;
    s16 yaw;
    s32 j;

    SfxSource_PlaySfxAtFixedWorldPos(play, &actor->world.pos, 30, NA_SE_EV_ICE_BROKEN);
    switch (Rand_S16Offset(0,2))
    {
        case 0:
            //SoundEffect_PlayOneshot(SOUND_MM_GLASS_SHATTER, 1.0f, 1.5f, &actor->world.pos, 0.0f, 1000.0f, &actor);
        break;
        case 1:
        default:
            //SoundEffect_PlayOneshot(SOUND_RAREWARE_GLASS_SHATTER, 1.0f, 1.5f, &actor->world.pos, 0.0f, 1000.0f, &actor);
        break;
    }

    for (i = 0; i < limbPosCount; i++) {
        yaw = Actor_WorldYawTowardPoint(actor, limbPos);

        for (j = 0; j < effectsPerLimb; j++) {
            randomYaw = ((s32)Rand_Next() >> 0x13) + yaw;

            velocity.z = Rand_ZeroFloat(5.0f);

            velocity.x = Math_SinS(randomYaw) * velocity.z;
            velocity.y = Rand_ZeroFloat(4.0f) + 8.0f;
            velocity.z *= Math_CosS(randomYaw);

            EffectSsEnIce_Spawn(play, limbPos, Rand_ZeroFloat(scaleRange) + scale, &velocity, &accel, &primColor,
                                &envColor, 30);
        }

        limbPos++;
    }
}

extern MtxF* sCurrentMatrix; //!< original name: "Matrix_now"



void Matrix_MultZero(Vec3f* dest) {
    MtxF* cmf = sCurrentMatrix;

    dest->x = cmf->xw;
    dest->y = cmf->yw;
    dest->z = cmf->zw;
}

void Matrix_MultVecY(f32 y, Vec3f* dest) {
    MtxF* cmf = sCurrentMatrix;

    dest->x = cmf->xw + cmf->xy * y;
    dest->y = cmf->yw + cmf->yy * y;
    dest->z = cmf->zw + cmf->zy * y;
}

void Matrix_MultVecX(f32 x, Vec3f* dest) {
    MtxF* cmf = sCurrentMatrix;

    dest->x = cmf->xw + cmf->xx * x;
    dest->y = cmf->yw + cmf->yx * x;
    dest->z = cmf->zw + cmf->zx * x;
}

void Matrix_MultVecZ(f32 z, Vec3f* dest) {
    MtxF* cmf = sCurrentMatrix;

    dest->x = cmf->xw + cmf->xz * z;
    dest->y = cmf->yw + cmf->yz * z;
    dest->z = cmf->zw + cmf->zz * z;
}

Vtx codeVtx_120260[] = {
#include "codeVtx_120260.vtx.inc"
};

Gfx gShadowMaterialDL[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPEndDisplayList(),
};

Gfx gShadowModelDL[] = {
    gsDPSetCombineLERP(PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED, 0, 0, 0,
                       COMBINED),
    gsDPSetRenderMode(G_RM_PASS, G_RM_ZB_OVL_SURF2),
    gsSPClearGeometryMode(G_CULL_BACK | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPVertex(codeVtx_120260, 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSPEndDisplayList(),
};


u64 gSun1Tex[] = {
#include "sun_1.i4.inc.h"
};

u64 gSun2Tex[] = {
#include "sun_2.i4.inc.h"
};

u64 gSun3Tex[] = {
#include "sun_3.i4.inc.h"
};


Vtx gameplay_keepVtx_07ACF8[] = {
#include "gameplay_keepVtx_07ACF8.vtx.inc"
};


Gfx gSunSparkleMaterialDL[] = {
    gsDPSetCombineLERP(PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, PRIMITIVE, 0, TEXEL0, 0, PRIMITIVE, ENVIRONMENT,
                       TEXEL0, ENVIRONMENT, PRIMITIVE, 0, TEXEL0, 0),
    gsDPLoadTextureBlock_4b(0x08000000, G_IM_FMT_I, 64, 64, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP,
                            G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD),
    gsSPEndDisplayList(),
};

Gfx gSunSparkleModelDL[] = {
    gsSPVertex(&gameplay_keepVtx_07ACF8[12], 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 1, 3, 2, 0),
    gsSPEndDisplayList(),
};


const u8 sShadowMaps[4][12][12] = {
    {
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },
        { 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0 },
        { 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0 },
        { 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0 },
        { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    },
    {
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },
        { 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0 },
        { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
        { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
        { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
        { 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0 },
        { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    },
    {
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },
        { 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0 },
        { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
        { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
        { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
        { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
        { 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0 },
        { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    },
    {
        { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },
        { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
        { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
        { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
        { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
        { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
        { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },
    },
};


Actor* SubS_FindActor(PlayState* play, Actor* actorListStart, u8 actorCategory, s16 actorId) {
    Actor* actor = actorListStart;

    if (actor == NULL) {
        actor = play->actorCtx.actorLists[actorCategory].head;
    }

    while ((actor != NULL) && (actorId != actor->id)) {
        actor = actor->next;
    }

    return actor;
}


void SubS_FillShadowTex(s32 startCol, s32 startRow, u8* tex, s32 size) {
    s32 i;
    s32 j;
    s32 start;

    for (i = 0; i < 12; i++) {
        start = ((startRow + i) * 64) + startCol - 390;
        for (j = 0; j < 12; j++) {
            if (sShadowMaps[size][i][j] != 0) {
                if ((start + j >= 0) && (start + j < SUBS_SHADOW_TEX_SIZE)) {
                    tex[start + j] = 255;
                }
            }
        }
    }
}


void SubS_GenShadowTex(Vec3f bodyPartsPos[], Vec3f* worldPos, u8* tex, f32 tween, u8 bodyPartsNum, u8 sizes[],
                       s8 parentBodyParts[]) {
    Vec3f pos;
    Vec3f startVec;
    s32 i;
    s32 parentBodyPart;
    Vec3f* bodyPartPos;
    s32 startCol;
    s32 startRow;

    for (i = 0; i < bodyPartsNum; i++) {
        if (parentBodyParts[i] >= 0) {
            parentBodyPart = parentBodyParts[i];
            bodyPartPos = &bodyPartsPos[i];

            pos.x = (bodyPartsPos[parentBodyPart].x - bodyPartPos->x) * tween + (bodyPartPos->x - worldPos->x);
            pos.y = (bodyPartsPos[parentBodyPart].y - bodyPartPos->y) * tween + (bodyPartPos->y - worldPos->y);
            pos.z = (bodyPartsPos[parentBodyPart].z - bodyPartPos->z) * tween + (bodyPartPos->z - worldPos->z);
        } else {
            bodyPartPos = &bodyPartsPos[i];

            pos.x = bodyPartPos->x - worldPos->x;
            pos.y = bodyPartPos->y - worldPos->y;
            pos.z = bodyPartPos->z - worldPos->z;
        }

        Matrix_MultVec3f(&pos, &startVec);
        startCol = 64.0f + startVec.x;
        startRow = 64.0f - startVec.z;
        SubS_FillShadowTex(startCol >> 1, startRow >> 1, tex, sizes[i]);
    }
}


void SubS_DrawShadowTex(Actor* actor, GameState* gameState, u8* tex) {
    //s32 pad;
    GraphicsContext* gfxCtx = gameState->gfxCtx;

    OPEN_DISPS(gfxCtx, __FILE__, __LINE__);

    Gfx_SetupDL_25Opa(gfxCtx);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 0, 0, 0, 100);
    gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, 0);
    Matrix_Translate(actor->world.pos.x, 0.0f, actor->world.pos.z, MTXMODE_NEW);
    Matrix_Scale(0.6f, 1.0f, 0.6f, MTXMODE_APPLY);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(gfxCtx, __FILE__, __LINE__), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, gShadowMaterialDL);
    gDPLoadTextureBlock(POLY_OPA_DISP++, tex, G_IM_FMT_I, G_IM_SIZ_8b, SUBS_SHADOW_TEX_WIDTH, SUBS_SHADOW_TEX_HEIGHT, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, 6, 6, G_TX_NOLOD, G_TX_NOLOD);
    gSPDisplayList(POLY_OPA_DISP++, gShadowModelDL);

    CLOSE_DISPS(gfxCtx, __FILE__, __LINE__);
}




void SkelAnime_DrawTransformFlexLimbOpa(PlayState* play, s32 limbIndex, void** skeleton, Vec3s* jointTable,
                                        OverrideLimbDrawOpa overrideLimbDraw, PostLimbDrawOpa postLimbDraw,
                                        TransformLimbDrawOpa transformLimbDraw, Actor* actor, Mtx** mtx) {
    StandardLimb* limb;
    Gfx* newDList;
    Gfx* limbDList;
    Vec3f pos;
    Vec3s rot;

    OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

    Matrix_Push();

    limb = Lib_SegmentedToVirtual(skeleton[limbIndex]);
    limbIndex++;

    rot = jointTable[limbIndex];
    pos.x = limb->jointPos.x;
    pos.y = limb->jointPos.y;
    pos.z = limb->jointPos.z;

    newDList = limbDList = limb->dList;

    if ((overrideLimbDraw == NULL) || !overrideLimbDraw(play, limbIndex, &newDList, &pos, &rot, actor)) {
        Matrix_TranslateRotateZYX(&pos, &rot);
        Matrix_Push();

        transformLimbDraw(play, limbIndex, actor);

        if (newDList != NULL) {
            Gfx* polyTemp = POLY_OPA_DISP;

            gSPMatrix(&polyTemp[0], Matrix_ToMtx(*mtx, __FILE__, __LINE__), G_MTX_LOAD);
            gSPDisplayList(&polyTemp[1], newDList);
            POLY_OPA_DISP = &polyTemp[2];
            (*mtx)++;
        } else {
            if (limbDList != NULL) {
                Matrix_ToMtx(*mtx, __FILE__, __LINE__);
                (*mtx)++;
            }
        }
        Matrix_Pop();
    }

    if (postLimbDraw != NULL) {
        postLimbDraw(play, limbIndex, &limbDList, &rot, actor);
    }

    if (limb->child != LIMB_DONE) {
        SkelAnime_DrawTransformFlexLimbOpa(play, limb->child, skeleton, jointTable, overrideLimbDraw, postLimbDraw,
                                           transformLimbDraw, actor, mtx);
    }

    Matrix_Pop();

    if (limb->sibling != LIMB_DONE) {
        SkelAnime_DrawTransformFlexLimbOpa(play, limb->sibling, skeleton, jointTable, overrideLimbDraw, postLimbDraw,
                                           transformLimbDraw, actor, mtx);
    }

    CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
}

void SkelAnime_DrawTransformFlexOpa(PlayState* play, void** skeleton, Vec3s* jointTable, s32 dListCount,
                                    OverrideLimbDrawOpa overrideLimbDraw, PostLimbDrawOpa postLimbDraw,
                                    TransformLimbDrawOpa transformLimbDraw, Actor* actor) {
    StandardLimb* rootLimb;
    //s32 pad;
    Gfx* newDList;
    Gfx* limbDList;
    Vec3f pos;
    Vec3s rot;
    Mtx* mtx;

    if (skeleton == NULL) {
        return;
    }

    OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

    mtx = Graph_Alloc(play->state.gfxCtx, dListCount * sizeof(Mtx));

    gSPSegment(POLY_OPA_DISP++, 0x0D, mtx);

    Matrix_Push();

    rootLimb = Lib_SegmentedToVirtual(skeleton[0]);

    pos.x = jointTable[0].x;
    pos.y = jointTable[0].y;
    pos.z = jointTable[0].z;
    rot = jointTable[1];

    newDList = limbDList = rootLimb->dList;

    if ((overrideLimbDraw == NULL) || !overrideLimbDraw(play, 1, &newDList, &pos, &rot, actor)) {
        Matrix_TranslateRotateZYX(&pos, &rot);
        Matrix_Push();

        transformLimbDraw(play, 1, actor);

        if (newDList != NULL) {
            Gfx* polyTemp = POLY_OPA_DISP;

            gSPMatrix(&polyTemp[0], Matrix_ToMtx(mtx, __FILE__, __LINE__), G_MTX_LOAD);
            gSPDisplayList(&polyTemp[1], newDList);
            POLY_OPA_DISP = &polyTemp[2];
            mtx++;
        } else {
            if (limbDList != NULL) {
                Matrix_ToMtx(mtx++, __FILE__, __LINE__);
            }
        }
        Matrix_Pop();
    }

    if (postLimbDraw != NULL) {
        postLimbDraw(play, 1, &limbDList, &rot, actor);
    }

    if (rootLimb->child != LIMB_DONE) {
        SkelAnime_DrawTransformFlexLimbOpa(play, rootLimb->child, skeleton, jointTable, overrideLimbDraw, postLimbDraw,
                                           transformLimbDraw, actor, &mtx);
    }

    Matrix_Pop();

    CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
}




/**
 * Returns a pointer to a single layer texture scroll displaylist.
 */
Gfx* AnimatedMat_TexScroll(PlayState* play, AnimatedMatTexScrollParams* params) {
    return Gfx_TexScroll(play->state.gfxCtx, params->xStep * sMatAnimStep, -(params->yStep * sMatAnimStep),
                         params->width, params->height);
}

/**
 * Animated Material Type 0:
 * Scrolls a single layer texture using the provided `AnimatedMatTexScrollParams`.
 */
void AnimatedMat_DrawTexScroll(PlayState* play, s32 segment, void* params) {
    AnimatedMatTexScrollParams* texScrollParams = (AnimatedMatTexScrollParams*)params;
    Gfx* texScrollDList = AnimatedMat_TexScroll(play, texScrollParams);

    OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

    if (sMatAnimFlags & 1) {
        gSPSegment(POLY_OPA_DISP++, segment, texScrollDList);
    }

    if (sMatAnimFlags & 2) {
        gSPSegment(POLY_XLU_DISP++, segment, texScrollDList);
    }

    CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
}

/**
 * Returns a pointer to a two layer texture scroll displaylist.
 */
Gfx* AnimatedMat_TwoLayerTexScroll(PlayState* play, AnimatedMatTexScrollParams* params) {
    return Gfx_TwoTexScroll(play->state.gfxCtx, 0, params[0].xStep * sMatAnimStep, -(params[0].yStep * sMatAnimStep),
                            params[0].width, params[0].height, 1, params[1].xStep * sMatAnimStep,
                            -(params[1].yStep * sMatAnimStep), params[1].width, params[1].height);
}

/**
 * Animated Material Type 1:
 * Scrolls a two layer texture using the provided `AnimatedMatTexScrollParams`.
 */
void AnimatedMat_DrawTwoTexScroll(PlayState* play, s32 segment, void* params) {
    AnimatedMatTexScrollParams* texScrollParams = (AnimatedMatTexScrollParams*)params;
    Gfx* texScrollDList = AnimatedMat_TwoLayerTexScroll(play, texScrollParams);

    OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

    if (sMatAnimFlags & 1) {
        gSPSegment(POLY_OPA_DISP++, segment, texScrollDList);
    }

    if (sMatAnimFlags & 2) {
        gSPSegment(POLY_XLU_DISP++, segment, texScrollDList);
    }

    CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
}

/**
 * Generates a displaylist that sets the prim and env color, and stores it in the provided segment ID.
 */
void AnimatedMat_SetColor(PlayState* play, s32 segment, F3DPrimColor* primColorResult, F3DEnvColor* envColor) {
    Gfx* gfx = Graph_Alloc(play->state.gfxCtx, 3 * sizeof(Gfx));

    OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

    // clang-format off
    if (sMatAnimFlags & 1) { gSPSegment(POLY_OPA_DISP++, segment, gfx); }
    if (sMatAnimFlags & 2) { gSPSegment(POLY_XLU_DISP++, segment, gfx); }
    // clang-format on

    gDPSetPrimColor(gfx++, 0, primColorResult->lodFrac, primColorResult->r, primColorResult->g, primColorResult->b,
                    (u8)(primColorResult->a * sMatAnimAlphaRatio));

    if (envColor != NULL) {
        gDPSetEnvColor(gfx++, envColor->r, envColor->g, envColor->b, envColor->a);
    }

    gSPEndDisplayList(gfx++);

    CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
}

/**
 * Animated Material Type 2:
 * Color key frame animation without linear interpolation.
 */
void AnimatedMat_DrawColor(PlayState* play, s32 segment, void* params) {
    AnimatedMatColorParams* colorAnimParams = (AnimatedMatColorParams*)params;
    F3DPrimColor* primColor = Lib_SegmentedToVirtual(colorAnimParams->primColors);
    F3DEnvColor* envColor;
    s32 curFrame = sMatAnimStep % colorAnimParams->keyFrameLength;

    primColor += curFrame;
    envColor = (colorAnimParams->envColors != NULL)
                   ? (F3DEnvColor*)Lib_SegmentedToVirtual(colorAnimParams->envColors) + curFrame
                   : NULL;

    AnimatedMat_SetColor(play, segment, primColor, envColor);
}

/**
 * Linear Interpolation
 */
s32 AnimatedMat_Lerp(s32 min, s32 max, f32 norm) {
    return (s32)((max - min) * norm) + min;
}

/**
 * Animated Material Type 3:
 * Color key frame animation with linear interpolation.
 */
void AnimatedMat_DrawColorLerp(PlayState* play, s32 segment, void* params) {
    AnimatedMatColorParams* colorAnimParams = (AnimatedMatColorParams*)params;
    F3DPrimColor* primColorMax = Lib_SegmentedToVirtual(colorAnimParams->primColors);
    F3DEnvColor* envColorMax;
    u16* keyFrames = Lib_SegmentedToVirtual(colorAnimParams->keyFrames);
    s32 curFrame = sMatAnimStep % colorAnimParams->keyFrameLength;
    s32 endFrame;
    s32 relativeFrame; // relative to the start frame
    s32 startFrame;
    f32 norm;
    F3DPrimColor* primColorMin;
    F3DPrimColor primColorResult;
    F3DEnvColor* envColorMin;
    F3DEnvColor envColorResult;
    s32 i;

    keyFrames++;
    i = 1;

    while (colorAnimParams->keyFrameCount > i) {
        if (curFrame < *keyFrames) {
            break;
        }
        i++;
        keyFrames++;
    }

    startFrame = keyFrames[-1];
    endFrame = keyFrames[0] - startFrame;
    relativeFrame = curFrame - startFrame;
    norm = (f32)relativeFrame / (f32)endFrame;

    primColorMax += i;
    primColorMin = primColorMax - 1;
    primColorResult.r = AnimatedMat_Lerp(primColorMin->r, primColorMax->r, norm);
    primColorResult.g = AnimatedMat_Lerp(primColorMin->g, primColorMax->g, norm);
    primColorResult.b = AnimatedMat_Lerp(primColorMin->b, primColorMax->b, norm);
    primColorResult.a = AnimatedMat_Lerp(primColorMin->a, primColorMax->a, norm);
    primColorResult.lodFrac = AnimatedMat_Lerp(primColorMin->lodFrac, primColorMax->lodFrac, norm);

    if (colorAnimParams->envColors) {
        envColorMax = Lib_SegmentedToVirtual(colorAnimParams->envColors);
        envColorMax += i;
        envColorMin = envColorMax - 1;
        envColorResult.r = AnimatedMat_Lerp(envColorMin->r, envColorMax->r, norm);
        envColorResult.g = AnimatedMat_Lerp(envColorMin->g, envColorMax->g, norm);
        envColorResult.b = AnimatedMat_Lerp(envColorMin->b, envColorMax->b, norm);
        envColorResult.a = AnimatedMat_Lerp(envColorMin->a, envColorMax->a, norm);
    } else {
        envColorMax = NULL;
    }

    AnimatedMat_SetColor(play, segment, &primColorResult, (envColorMax != NULL) ? &envColorResult : NULL);
}

/**
 * Lagrange interpolation
 */
f32 Scene_LagrangeInterp(s32 n, f32 x[], f32 fx[], f32 xp) {
    f32 weights[50];
    f32 xVal;
    f32 m;
    f32 intp;
    f32* xPtr1;
    f32* fxPtr;
    f32* weightsPtr;
    f32* xPtr2;
    s32 i;
    s32 j;

    for (i = 0, xPtr1 = x, fxPtr = fx, weightsPtr = weights; i < n; i++) {
        for (xVal = *xPtr1, m = 1.0f, j = 0, xPtr2 = x; j < n; j++) {
            if (j != i) {
                m *= xVal - (*xPtr2);
            }
            xPtr2++;
        }

        xPtr1++;
        *weightsPtr = (*fxPtr) / m;
        fxPtr++;
        weightsPtr++;
    }

    for (intp = 0.0f, i = 0, weightsPtr = weights; i < n; i++) {
        for (m = 1.0f, j = 0, xPtr2 = x; j < n; j++) {
            if (j != i) {
                m *= xp - (*xPtr2);
            }
            xPtr2++;
        }

        intp += (*weightsPtr) * m;
        weightsPtr++;
    }

    return intp;
}

/**
 * Lagrange interpolation specifically for colors
 */
u8 Scene_LagrangeInterpColor(s32 n, f32 x[], f32 fx[], f32 xp) {
    s32 intp = Scene_LagrangeInterp(n, x, fx, xp);

    // Clamp between 0 and 255 to ensure the color value does not overflow in either direction
    return CLAMP(intp, 0, 255);
}

void Scene_SetRenderModeXlu(PlayState* play, s32 index, u32 flags) {
    static Gfx renderModeSetNoneDL[] = {
        gsSPEndDisplayList(),
        // These instructions will never get executed
        gsSPEndDisplayList(),
        gsSPEndDisplayList(),
        gsSPEndDisplayList(),
    };
    static Gfx renderModeSetXluSingleCycleDL[] = {
        gsDPSetRenderMode(AA_EN | Z_CMP | IM_RD | CLR_ON_CVG | CVG_DST_WRAP | ZMODE_XLU | FORCE_BL |
                              GBL_c1(G_BL_CLR_IN, G_BL_0, G_BL_CLR_IN, G_BL_1),
                          G_RM_AA_ZB_XLU_SURF2),
        gsSPEndDisplayList(),
        // These instructions will never get executed
        gsDPSetRenderMode(AA_EN | Z_CMP | IM_RD | CLR_ON_CVG | CVG_DST_WRAP | ZMODE_XLU | FORCE_BL |
                              GBL_c1(G_BL_CLR_FOG, G_BL_A_SHADE, G_BL_CLR_IN, G_BL_1MA),
                          G_RM_AA_ZB_XLU_SURF2),
        gsSPEndDisplayList(),
    };
    static Gfx renderModeSetXluTwoCycleDL[] = {
        gsDPSetRenderMode(AA_EN | Z_CMP | Z_UPD | IM_RD | CLR_ON_CVG | CVG_DST_WRAP | ZMODE_XLU | FORCE_BL |
                              GBL_c1(G_BL_CLR_IN, G_BL_0, G_BL_CLR_IN, G_BL_1),
                          AA_EN | Z_CMP | Z_UPD | IM_RD | CLR_ON_CVG | CVG_DST_WRAP | ZMODE_XLU | FORCE_BL |
                              GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)),
        gsSPEndDisplayList(),
        // These instructions will never get executed
        gsDPSetRenderMode(AA_EN | Z_CMP | Z_UPD | IM_RD | CLR_ON_CVG | CVG_DST_WRAP | ZMODE_XLU | FORCE_BL |
                              GBL_c1(G_BL_CLR_FOG, G_BL_A_SHADE, G_BL_CLR_IN, G_BL_1MA),
                          AA_EN | Z_CMP | Z_UPD | IM_RD | CLR_ON_CVG | CVG_DST_WRAP | ZMODE_XLU | FORCE_BL |
                              GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)),
        gsSPEndDisplayList(),
    };
    static Gfx* dLists[] = {
        renderModeSetNoneDL,
        renderModeSetXluSingleCycleDL,
        renderModeSetXluTwoCycleDL,
    };
    Gfx* dList = dLists[index];

    OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

    if (flags & 1) {
        gSPSegment(POLY_OPA_DISP++, 0x0C, dList);
    }

    if (flags & 2) {
        gSPSegment(POLY_XLU_DISP++, 0x0C, dList);
    }

    CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
}

/**
 * Animated Material Type 4:
 * Color key frame animation with non-linear interpolation.
 */
void AnimatedMat_DrawColorNonLinearInterp(PlayState* play, s32 segment, void* params) {
    AnimatedMatColorParams* colorAnimParams = (AnimatedMatColorParams*)params;
    F3DPrimColor* primColorCur = Lib_SegmentedToVirtual(colorAnimParams->primColors);
    F3DEnvColor* envColorCur = Lib_SegmentedToVirtual(colorAnimParams->envColors);
    u16* keyFrames = Lib_SegmentedToVirtual(colorAnimParams->keyFrames);
    f32 curFrame = sMatAnimStep % colorAnimParams->keyFrameLength;
    F3DPrimColor primColorResult;
    F3DEnvColor envColorResult;
    f32 x[50];
    f32 fxPrimR[50];
    f32 fxPrimG[50];
    f32 fxPrimB[50];
    f32 fxPrimA[50];
    f32 fxPrimLodFrac[50];
    f32 fxEnvR[50];
    f32 fxEnvG[50];
    f32 fxEnvB[50];
    f32 fxEnvA[50];
    f32* xPtr = x;
    f32* fxPrimRPtr = fxPrimR;
    f32* fxPrimGPtr = fxPrimG;
    f32* fxPrimBPtr = fxPrimB;
    f32* fxPrimAPtr = fxPrimA;
    f32* fxPrimLodFracPtr = fxPrimLodFrac;
    f32* fxEnvRPtr = fxEnvR;
    f32* fxEnvGPtr = fxEnvG;
    f32* fxEnvBPtr = fxEnvB;
    f32* fxEnvAPtr = fxEnvA;
    s32 i;

    for (i = 0; i < colorAnimParams->keyFrameCount; i++) {
        *xPtr = *keyFrames;
        *fxPrimRPtr = primColorCur->r;
        *fxPrimGPtr = primColorCur->g;
        *fxPrimBPtr = primColorCur->b;
        *fxPrimAPtr = primColorCur->a;
        *fxPrimLodFracPtr = primColorCur->lodFrac;

        primColorCur++;
        fxPrimRPtr++;
        fxPrimGPtr++;
        fxPrimBPtr++;
        fxPrimAPtr++;
        fxPrimLodFracPtr++;

        if (envColorCur != NULL) {
            *fxEnvRPtr = envColorCur->r;
            *fxEnvGPtr = envColorCur->g;
            *fxEnvBPtr = envColorCur->b;
            *fxEnvAPtr = envColorCur->a;

            envColorCur++;
            fxEnvRPtr++;
            fxEnvGPtr++;
            fxEnvBPtr++;
            fxEnvAPtr++;
        }

        keyFrames++;
        xPtr++;
    }

    primColorResult.r = Scene_LagrangeInterpColor(colorAnimParams->keyFrameCount, x, fxPrimR, curFrame);
    primColorResult.g = Scene_LagrangeInterpColor(colorAnimParams->keyFrameCount, x, fxPrimG, curFrame);
    primColorResult.b = Scene_LagrangeInterpColor(colorAnimParams->keyFrameCount, x, fxPrimB, curFrame);
    primColorResult.a = Scene_LagrangeInterpColor(colorAnimParams->keyFrameCount, x, fxPrimA, curFrame);
    primColorResult.lodFrac = Scene_LagrangeInterpColor(colorAnimParams->keyFrameCount, x, fxPrimLodFrac, curFrame);

    if (colorAnimParams->envColors != NULL) {
        envColorCur = Lib_SegmentedToVirtual(colorAnimParams->envColors);
        envColorResult.r = Scene_LagrangeInterpColor(colorAnimParams->keyFrameCount, x, fxEnvR, curFrame);
        envColorResult.g = Scene_LagrangeInterpColor(colorAnimParams->keyFrameCount, x, fxEnvG, curFrame);
        envColorResult.b = Scene_LagrangeInterpColor(colorAnimParams->keyFrameCount, x, fxEnvB, curFrame);
        envColorResult.a = Scene_LagrangeInterpColor(colorAnimParams->keyFrameCount, x, fxEnvA, curFrame);
    } else {
        envColorCur = NULL;
    }

    AnimatedMat_SetColor(play, segment, &primColorResult, (envColorCur != NULL) ? &envColorResult : NULL);
}

/**
 * Animated Material Type 5:
 * Cycles between a list of textures (imagine like a GIF)
 */
void AnimatedMat_DrawTexCycle(PlayState* play, s32 segment, void* params) {
    AnimatedMatTexCycleParams* texAnimParams = params;
    TexturePtr* texList = Lib_SegmentedToVirtual(texAnimParams->textureList);
    u8* texId = Lib_SegmentedToVirtual(texAnimParams->textureIndexList);
    s32 curFrame = sMatAnimStep % texAnimParams->keyFrameLength;
    TexturePtr tex = Lib_SegmentedToVirtual(texList[texId[curFrame]]);

    OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

    if (sMatAnimFlags & 1) {
        gSPSegment(POLY_OPA_DISP++, segment, tex);
    }

    if (sMatAnimFlags & 2) {
        gSPSegment(POLY_XLU_DISP++, segment, tex);
    }

    CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
}

/**
 * This is the main function that handles the animated material system.
 * There are six different animated material types, which should be set in the provided `AnimatedMaterial`.
 */
void AnimatedMat_DrawMain(PlayState* play, AnimatedMaterial* matAnim, f32 alphaRatio, u32 step, u32 flags) {
    static void (*matAnimDrawHandlers[])(PlayState*, s32 segment, void* params) = {
        AnimatedMat_DrawTexScroll, AnimatedMat_DrawTwoTexScroll,         AnimatedMat_DrawColor,
        AnimatedMat_DrawColorLerp, AnimatedMat_DrawColorNonLinearInterp, AnimatedMat_DrawTexCycle,
    };
    s32 segmentAbs;
    s32 segment;

    sMatAnimAlphaRatio = alphaRatio;
    sMatAnimStep = step;
    sMatAnimFlags = flags;

    if ((matAnim != NULL) && (matAnim->segment != 0)) {
        do {
            segment = matAnim->segment;
            segmentAbs = ABS_ALT(segment) + 7;
            matAnimDrawHandlers[matAnim->type](play, segmentAbs, Lib_SegmentedToVirtual(matAnim->params));
            matAnim++;
        } while (segment >= 0);
    }
}

/**
 * Draws an animated material to both OPA and XLU buffers.
 */
void AnimatedMat_Draw(PlayState* play, AnimatedMaterial* matAnim) {
    AnimatedMat_DrawMain(play, matAnim, 1, play->gameplayFrames, 3);
}


VecGeo* OLib_Vec3fToVecGeo(VecGeo* dest, Vec3f* vec) {
    VecSph_MM sph;

    OLib_Vec3fToVecSph(&sph, vec);
    sph.pitch = 0x4000 - sph.pitch;

    *dest = sph;

    return dest;
}

Vec3f* OLib_VecGeoToVec3f(Vec3f* dest, VecGeo* geo) {
    VecSph_MM sph;

    sph.r = geo->r;
    sph.pitch = 0x4000 - geo->pitch;
    sph.yaw = geo->yaw;

    return OLib_VecSphToVec3f(dest, &sph);
}

Vec3f* OLib_AddVecGeoToVec3f(Vec3f* dest, Vec3f* a, VecGeo* geo) {
    Vec3f sum;
    Vec3f b;

    OLib_VecSphGeoToVec3f(&b, geo);
    sum.x = a->x + b.x;
    sum.y = a->y + b.y;
    sum.z = a->z + b.z;

    *dest = sum;

    return dest;
}

void OLib_Vec3fAdd(PosRot* a, Vec3f* b, Vec3f* dest, s16 mode) {
    VecGeo geo;

    switch (mode) {
        case OLIB_ADD_OFFSET:
            OLib_Vec3fToVecSphGeo(&geo, b);
            geo.yaw += a->rot.y;
            OLib_AddVecGeoToVec3f(dest, &a->pos, &geo);
            break;

        case OLIB_ADD:
            dest->x = a->pos.x + b->x;
            dest->y = a->pos.y + b->y;
            dest->z = a->pos.z + b->z;
            break;

        default: // OLIB_ADD_COPY
            *dest = *b;
            break;
    }
}


Vtx gameplay_keepVtx_075A00[] = {
#include "gameplay_keepVtx_075A00.vtx.inc"
};

u64 gameplay_keep_Tex_075AA8[] = {
#include "tex_075AA8.i4.inc.h"
};

Gfx gSquareShadowDL[] = {
    gsDPSetAlphaCompare(G_AC_THRESHOLD),
    gsDPLoadTextureBlock_4b(gameplay_keep_Tex_075AA8, G_IM_FMT_I, 16, 16, 15, G_TX_MIRROR | G_TX_WRAP, G_TX_MIRROR |
                            G_TX_WRAP, 4, 4, G_TX_NOLOD, G_TX_NOLOD),
    gsSPVertex(gameplay_keepVtx_075A00, 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsDPPipeSync(),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPEndDisplayList(),
};


u64 gameplay_keep_Tex_06ABF8[] = {
#include "tex_06ABF8.rgba16.inc.h"
};

Vtx gameplay_keepVtx_06A980[] = {
#include "gameplay_keepVtx_06A980.vtx.inc"
};

Gfx gameplay_keep_DL_06AB30[] = {
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPLoadTextureBlock(gameplay_keep_Tex_06ABF8, G_IM_FMT_RGBA, G_IM_SIZ_16b, 36, 36, 0, G_TX_NOMIRROR | G_TX_CLAMP,
                         G_TX_NOMIRROR | G_TX_CLAMP, 6, 6, G_TX_NOLOD, G_TX_NOLOD),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(gameplay_keepVtx_06A980, 12, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(6, 7, 8, 0, 9, 10, 11, 0),
    gsSP2Triangles(10, 0, 2, 0, 10, 2, 11, 0),
    gsSPVertex(&gameplay_keepVtx_06A980[12], 7, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(5, 1, 0, 0, 6, 4, 3, 0),
    gsSPVertex(&gameplay_keepVtx_06A980[19], 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 2, 3, 0, 0),
    gsSPVertex(&gameplay_keepVtx_06A980[23], 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 2, 3, 0, 0),
    gsSPEndDisplayList(),
};


void ActorShadow_DrawSquare(Actor* actor, Lights* lights, PlayState* play) {
    ActorShadow_Draw(actor, lights, play, gSquareShadowDL, NULL);
}

/**
 * Fully initializes a ColliderJntSph using the values in `src`, placing the element array in elements.
 */
s32 Collider_InitAndSetJntSph(PlayState* play, ColliderJntSph* sphereGroup, Actor* actor, ColliderJntSphInit* src,
                              ColliderJntSphElement* elements) {
    Collider_InitJntSph(play, sphereGroup);
    Collider_SetJntSph(play, sphereGroup, actor, src, elements);
    return 0;
}

/**
 * Fully initializes a ColliderCylinder using the values in src.
 */
s32 Collider_InitAndSetCylinder(PlayState* play, ColliderCylinder* collider, Actor* actor, ColliderCylinderInit* src) {
    Collider_InitCylinder(play, collider);
    Collider_SetCylinder(play, collider, actor, src);
    return 1;
}

typedef enum {
    /* -1 */ CS_HUD_VISIBILITY_ALL_ALT = -1,
    /*  0 */ CS_HUD_VISIBILITY_NONE,
    /*  1 */ CS_HUD_VISIBILITY_ALL,
    /*  2 */ CS_HUD_VISIBILITY_A_HEARTS_MAGIC,
    /*  3 */ CS_HUD_VISIBILITY_C_HEARTS_MAGIC,
    /*  4 */ CS_HUD_VISIBILITY_ALL_NO_MINIMAP,
    /*  5 */ CS_HUD_VISIBILITY_A_B_C,
    /*  6 */ CS_HUD_VISIBILITY_B_MINIMAP,
    /*  7 */ CS_HUD_VISIBILITY_A
} CutsceneHudVisibility;

typedef enum {
    /*   0 */ CS_END_SFX_NONE,
    /*   1 */ CS_END_SFX_TRE_BOX_APPEAR,
    /*   2 */ CS_END_SFX_CORRECT_CHIME,
    /* 255 */ CS_END_SFX_NONE_ALT = 0xFF
} CutsceneEndSfx;

typedef enum {
    /*   -1 */ CS_ID_NONE = -1,
    // CsId's 0 - 119 are sceneLayer-specific and index `ActorCutscene`
    /* 0x78 */ CS_ID_GLOBAL_78 = 120,
    /* 0x79 */ CS_ID_GLOBAL_79,
    /* 0x7A */ CS_ID_GLOBAL_7A,
    /* 0x7B */ CS_ID_GLOBAL_ELEGY,
    /* 0x7C */ CS_ID_GLOBAL_TALK,
    /* 0x7D */ CS_ID_GLOBAL_DOOR,
    /* 0x7E */ CS_ID_GLOBAL_RETURN_TO_CAM, // smoothly return to the previous camera
    /* 0x7F */ CS_ID_GLOBAL_END
} CutsceneId;

#include "oot/z_camera_data.c"

s16 Camera_ChangeSettingFlags(Camera* camera, s16 setting, s16 flags);

void Camera_ResetActionFuncState(Camera* camera, s32 mode) {
    camera->animState = 0;
}

#define CS_CAM_ID_NONE 0
#define CS_CAM_ID_GLOBAL_ELEGY 0
#define CS_CAM_ID_GLOBAL_CONNECT 0
#define CS_SCRIPT_ID_NONE -1

#define NUM_CAMS 4

// Camera IDs are indices into `cameraPtrs`
#define CAM_ID_MAIN 0 // The index of the main camera
#define CAM_ID_SUB_FIRST 1 // The index sub cameras start at
#define CAM_ID_NONE -1 // Used to indicate no camera. Can be used to default to the active camera in some scenarios

#define SUB_CAM_ID_DONE 0 // Used in some actors for variables holding sub camera IDs to indicate "subcam is finished"

#define CAM_HUD_VISIBILITY_SHIFT 8
#define CAM_HUD_VISIBILITY_MASK (0x0F00)
#define CAM_HUD_VISIBILITY(hudVisibility) (((hudVisibility) & 0xF) << CAM_HUD_VISIBILITY_SHIFT)

#define CAM_HUD_VISIBILITY_ALL \
    CAM_HUD_VISIBILITY(0) // HUD_VISIBILITY_ALL
#define CAM_HUD_VISIBILITY_NONE \
    CAM_HUD_VISIBILITY(HUD_VISIBILITY_NONE)
#define CAM_HUD_VISIBILITY_NONE_ALT \
    CAM_HUD_VISIBILITY(HUD_VISIBILITY_NONE_ALT)
#define CAM_HUD_VISIBILITY_HEARTS_WITH_OVERWRITE \
    CAM_HUD_VISIBILITY(HUD_VISIBILITY_HEARTS_WITH_OVERWRITE)
#define CAM_HUD_VISIBILITY_A \
    CAM_HUD_VISIBILITY(HUD_VISIBILITY_A)
#define CAM_HUD_VISIBILITY_A_HEARTS_MAGIC_WITH_OVERWRITE \
    CAM_HUD_VISIBILITY(HUD_VISIBILITY_A_HEARTS_MAGIC_WITH_OVERWRITE)
#define CAM_HUD_VISIBILITY_A_HEARTS_MAGIC_MINIMAP_WITH_OVERWRITE \
    CAM_HUD_VISIBILITY(HUD_VISIBILITY_A_HEARTS_MAGIC_MINIMAP_WITH_OVERWRITE)
#define CAM_HUD_VISIBILITY_ALL_NO_MINIMAP_W_DISABLED \
    CAM_HUD_VISIBILITY(HUD_VISIBILITY_ALL_NO_MINIMAP_W_DISABLED)
#define CAM_HUD_VISIBILITY_B \
    CAM_HUD_VISIBILITY(HUD_VISIBILITY_B)
#define CAM_HUD_VISIBILITY_HEARTS_MAGIC \
    CAM_HUD_VISIBILITY(HUD_VISIBILITY_HEARTS_MAGIC)
#define CAM_HUD_VISIBILITY_B_ALT \
    CAM_HUD_VISIBILITY(HUD_VISIBILITY_B_ALT)
#define CAM_HUD_VISIBILITY_HEARTS \
    CAM_HUD_VISIBILITY(HUD_VISIBILITY_HEARTS)
#define CAM_HUD_VISIBILITY_A_B_MINIMAP \
    CAM_HUD_VISIBILITY(HUD_VISIBILITY_A_B_MINIMAP)
#define CAM_HUD_VISIBILITY_HEARTS_MAGIC_WITH_OVERWRITE \
    CAM_HUD_VISIBILITY(HUD_VISIBILITY_HEARTS_MAGIC_WITH_OVERWRITE)
#define CAM_HUD_VISIBILITY_HEARTS_MAGIC_C \
    CAM_HUD_VISIBILITY(HUD_VISIBILITY_HEARTS_MAGIC_C)
// HUD_VISIBILITY_ALL_NO_MINIMAP and above can not be set using `CAM_HUD_VISIBILITY`
#define CAM_HUD_VISIBILITY_IGNORE \
    CAM_HUD_VISIBILITY(0xF)

/**
 * letterboxFlag: determines the size of the letter-box window. See CAM_LETTERBOX_* enums.
 *                  Can also add on the flag ( | CAM_LETTERBOX_INSTANT) to make the size change immediately
 * hudVisibility: hides certain hud icons
 *    - A value of 0 in camera is translated to a hudVisibility of 50, 
 *      which is the value to restore all hud icons to the screen (CAM_HUD_VISIBILITY_ALL)
 *    - A value of 0xF in camera results in no change in the hudVisibility (CAM_HUD_VISIBILITY_IGNORE)
 * funcFlags: Custom flags for functions
 */
#define CAM_INTERFACE_FLAGS(letterboxFlag, hudVisibility, funcFlags) \
    (((letterboxFlag) & CAM_LETTERBOX_MASK) | (hudVisibility) | ((funcFlags) & 0xFF))

// Shrinking the window from the top and bottom with black borders (letterboxing)
#define CAM_LETTERBOX_MASK 0xF000

#define CAM_LETTERBOX_SIZE_MASK 0x7000
#define CAM_LETTERBOX_NONE 0x0000
// small/medium/large black borders
#define CAM_LETTERBOX_SMALL 0x1000
#define CAM_LETTERBOX_MEDIUM 0x2000
#define CAM_LETTERBOX_LARGE 0x3000
#define CAM_LETTERBOX_NONE_4 0x4000

#define CAM_LETTERBOX_INSTANT 0x8000 // Bit to determine whether to set the current value directly (on), or to set the size target (off)

#define CAM_LETTERBOX_IGNORE 0xF000 // No change in letterbox size, keep the previous size


// Camera behaviorFlags. Flags specifically for settings, modes, and bgCam
// Used to store current state, only CAM_BEHAVIOR_SETTING_1 and CAM_BEHAVIOR_BG_2 are read from and used in logic
// Setting (0x1, 0x10)
#define CAM_BEHAVIOR_SETTING_USE_PRIORITY (1 << 0) // Use settings priority system
#define CAM_BEHAVIOR_SETTING_2 (1 << 4)
// Mode (0x2, 0x20)
#define CAM_BEHAVIOR_MODE_1 (1 << 1)
#define CAM_BEHAVIOR_MODE_VALID (1 << 5)
// bgCam (0x4, 0x40)
#define CAM_BEHAVIOR_BGCAM_1 (1 << 2)
#define CAM_BEHAVIOR_BGCAM_2 (1 << 6)

// Camera stateFlags. Variety of generic flags
#define CAM_STATE_0 (1 << 0) // Must be set for the camera from changing settings based on the bg surface
#define CAM_STATE_CHECK_WATER (1 << 1)
#define CAM_STATE_2 (1 << 2)
#define CAM_STATE_3 (1 << 3)
#define CAM_STATE_4 (1 << 4)
#define CAM_STATE_DISABLE_MODE_CHANGE (1 << 5)
#define CAM_STATE_6 (1 << 6)
#define CAM_STATE_7 (1 << 7)
#define CAM_STATE_UNDERWATER (1 << 8)
#define CAM_STATE_9 (1 << 9)
#define CAM_STATE_10 (1 << 10) // Surpresses the camera from changing settings based on the bg surface
#define CAM_STATE_11 (1 << 11)
#define CAM_STATE_12 (1 << 12)
#define CAM_STATE_13 (1 << 13)
#define CAM_STATE_INITIALIZED (1 << 14)
#define CAM_STATE_15 ((s16)(1 << 15))

// Camera viewFlags. Set params related to view
#define CAM_VIEW_AT (1 << 0) // camera->at
#define CAM_VIEW_EYE (1 << 1) // camera->eye and camera->eyeNext
#define CAM_VIEW_UP (1 << 2) // camera->up
#define CAM_VIEW_TARGET (1 << 3) // camera->target
#define CAM_VIEW_TARGET_POS (1 << 4) // camera->targetPosRot.pos
#define CAM_VIEW_FOV (1 << 5) // camera->fov
#define CAM_VIEW_ROLL (1 << 6) // camera->roll

/**
 * Camera Status type
 * Determines how much the camera is updated each frame
 * Higher values represents higher levels of activity
 */
// Camera is on
#define CAM_STATUS_CUT        0 // The camera is not updated at all
#define CAM_STATUS_WAIT       1 // There is minimally/partially updated, action function is not run
#define CAM_STATUS_UNK3       3 // The camera is mostly updated including running its action function, but data is not set to view
#define CAM_STATUS_ACTIVE     7 // The camera is fully updated, info is sent to view
// Camera is off
#define CAM_STATUS_INACTIVE   0x100

#define ACTOR_FLAG_100000        (1 << 20)

typedef enum {
    /* 0x00 */ CAM_SET_NONE_MM,
    /* 0x01 */ CAM_SET_NORMAL0_MM, // Generic camera 0, used in various places "NORMAL0"
    /* 0x02 */ CAM_SET_NORMAL3_MM, // Generic camera 3, used in various places "NORMAL3"
    /* 0x03 */ CAM_SET_PIVOT_DIVING, // Player diving from the surface of the water to underwater not as zora "CIRCLE5"
    /* 0x04 */ CAM_SET_HORSE_MM, // Reiding a horse "HORSE0"
    /* 0x05 */ CAM_SET_ZORA_DIVING, // Parallel's Pivot Diving, but as Zora. However, Zora does not dive like a human. So this setting appears to not be used "ZORA0"
    /* 0x06 */ CAM_SET_PREREND_FIXED_MM, // Unused remnant of OoT: camera is fixed in position and rotation "PREREND0"
    /* 0x07 */ CAM_SET_PREREND_PIVOT_MM, // Unused remnant of OoT: Camera is fixed in position with fixed pitch, but is free to rotate in the yaw direction 360 degrees "PREREND1"
    /* 0x08 */ CAM_SET_DOORC_MM, // Generic room door transitions, camera moves and follows player as the door is open and closed "DOORC"
    /* 0x09 */ CAM_SET_DEMO0, // Unknown, possibly related to treasure chest game as goron? "DEMO0"
    /* 0x0A */ CAM_SET_FREE0_MM, // Free Camera, manual control is given, no auto-updating eye or at "FREE0"
    /* 0x0B */ CAM_SET_BIRDS_EYE_VIEW_0, // Appears unused. Camera is a top-down view "FUKAN0"
    /* 0x0C */ CAM_SET_NORMAL1_MM, // Generic camera 1, used in various places "NORMAL1"
    /* 0x0D */ CAM_SET_NANAME, // Unknown, slanted or tilted. Behaves identical to Normal0 except with added roll "NANAME"
    /* 0x0E */ CAM_SET_CIRCLE0, // Used in Curiosity Shop, Pirates Fortress, Mayor's Residence "CIRCLE0"
    /* 0x0F */ CAM_SET_FIXED0, // Used in Sakon's Hideout puzzle rooms, milk bar stage "FIXED0"
    /* 0x10 */ CAM_SET_SPIRAL_DOOR, // Exiting a Spiral Staircase "SPIRAL"
    /* 0x11 */ CAM_SET_DUNGEON0_MM, // Generic dungeon camera 0, used in various places "DUNGEON0"
    /* 0x12 */ CAM_SET_ITEM0, // Getting an item and holding it above Player's head (from small chest, freestanding, npc, ...) "ITEM0"
    /* 0x13 */ CAM_SET_ITEM1, // Looking at player while playing the ocarina "ITEM1"
    /* 0x14 */ CAM_SET_ITEM2, // Bottles: drinking, releasing fairy, dropping fish "ITEM2"
    /* 0x15 */ CAM_SET_ITEM3, // Bottles: catching fish or bugs, showing an item "ITEM3"
    /* 0x16 */ CAM_SET_NAVI, // Song of Soaring, variations of playing Song of Time "NAVI"
    /* 0x17 */ CAM_SET_WARP_PAD_MOON, // Warp circles from Goron Trial on the moon "WARP0"
    /* 0x18 */ CAM_SET_DEATH, // Player death animation when health goes to 0 "DEATH"
    /* 0x19 */ CAM_SET_REBIRTH, // Unknown set with camDataId = -9 (it's not being revived by a fairy) "REBIRTH"
    /* 0x1A */ CAM_SET_LONG_CHEST_OPENING, // Long cutscene when opening a big chest with a major item "TREASURE"
    /* 0x1B */ CAM_SET_MASK_TRANSFORMATION, // Putting on a transformation mask "TRANSFORM"
    /* 0x1C */ CAM_SET_ATTENTION, // Unknown, set with camDataId = -15 "ATTENTION"
    /* 0x1D */ CAM_SET_WARP_PAD_ENTRANCE, // Warp pad from start of a dungeon to the boss-room "WARP1"
    /* 0x1E */ CAM_SET_DUNGEON1_MM, // Generic dungeon camera 1, used in various places "DUNGEON1"
    /* 0x1F */ CAM_SET_FIXED1, // Fixes camera in place, used in various places eg. entering Stock Pot Inn, hiting a switch, giving witch a red potion, shop browsing "FIXED1"
    /* 0x20 */ CAM_SET_FIXED2, // Used in Pinnacle Rock after defeating Sea Monsters, and by Tatl in Fortress "FIXED2"
    /* 0x21 */ CAM_SET_MAZE, // Unused. Set to use Camera_Parallel2(), which is only Camera_Noop() "MAZE"
    /* 0x22 */ CAM_SET_REMOTEBOMB, // Unused. Set to use Camera_Parallel2(), which is only Camera_Noop(). But also related to Play_ChangeCameraSetting? "REMOTEBOMB"
    /* 0x23 */ CAM_SET_CIRCLE1, // Unknown "CIRCLE1"
    /* 0x24 */ CAM_SET_CIRCLE2, // Looking at far-away NPCs eg. Garo in Road to Ikana, Hungry Goron, Tingle "CIRCLE2"
    /* 0x25 */ CAM_SET_CIRCLE3, // Used in curiosity shop, goron racetrack, final room in Sakon's hideout, other places "CIRCLE3"
    /* 0x26 */ CAM_SET_CIRCLE4, // Used during the races on the doggy racetrack "CIRCLE4"
    /* 0x27 */ CAM_SET_FIXED3, // Used in Stock Pot Inn Toilet and Tatl cutscene after woodfall "FIXED3"
    /* 0x28 */ CAM_SET_TOWER_ASCENT, // Various climbing structures (Snowhead climb to the temple entrance) "TOWER0"
    /* 0x29 */ CAM_SET_PARALLEL0, // Unknown "PARALLEL0"
    /* 0x2A */ CAM_SET_NORMALD, // Unknown, set with camDataId = -20 "NORMALD"
    /* 0x2B */ CAM_SET_SUBJECTD, // Unknown, set with camDataId = -21 "SUBJECTD"
    /* 0x2C */ CAM_SET_START0_MM, // Entering a room, either Dawn of a New Day reload, or entering a door where the camera is fixed on the other end "START0"
    /* 0x2D */ CAM_SET_START2, // Entering a scene, camera is put at a low angle eg. Grottos, Deku Palace, Stock Pot Inn "START2"
    /* 0x2E */ CAM_SET_STOP0, // Called in z_play "STOP0"
    /* 0x2F */ CAM_SET_BOAT_CRUISE, //  Koume's boat cruise "JCRUISING"
    /* 0x30 */ CAM_SET_VERTICAL_CLIMB, // Large vertical climbs, such as Mountain Village wall or Pirates Fortress ladder. "CLIMBMAZE"
    /* 0x31 */ CAM_SET_SIDED, // Unknown, set with camDataId = -24 "SIDED"
    /* 0x32 */ CAM_SET_DUNGEON2_MM, // Generic dungeon camera 2, used in various places "DUNGEON2"
    /* 0x33 */ CAM_SET_BOSS_ODOLWA, // Odolwa's Lair, also used in GBT entrance: "BOSS_SHIGE"
    /* 0x34 */ CAM_SET_KEEPBACK, // Unknown. Possibly related to climbing something? "KEEPBACK"
    /* 0x35 */ CAM_SET_CIRCLE6, // Used in select regions from Ikana "CIRCLE6"
    /* 0x36 */ CAM_SET_CIRCLE7, // Unknown "CIRCLE7"
    /* 0x37 */ CAM_SET_MINI_BOSS, // Used during the various minibosses of the "CHUBOSS"
    /* 0x38 */ CAM_SET_RFIXED1, // Talking to Koume stuck on the floor in woods of mystery "RFIXED1"
    /* 0x39 */ CAM_SET_TREASURE_CHEST_MINIGAME, // Treasure Chest Shop in East Clock Town, minigame location "TRESURE1"
    /* 0x3A */ CAM_SET_HONEY_AND_DARLING_1, // Honey and Darling Minigames "BOMBBASKET"
    /* 0x3B */ CAM_SET_CIRCLE8, // Used by Stone Tower moving platforms, Falling eggs in Marine Lab, Bugs into soilpatch cutscene "CIRCLE8"
    /* 0x3C */ CAM_SET_BIRDS_EYE_VIEW_1, // Camera is a top-down view. Used in Fisherman's minigame and Deku Palace "FUKAN1"
    /* 0x3D */ CAM_SET_DUNGEON3, // Generic dungeon camera 3, used in various places "DUNGEON3"
    /* 0x3E */ CAM_SET_TELESCOPE, // Observatory telescope and Curiosity Shop Peep-Hole "TELESCOPE"
    /* 0x3F */ CAM_SET_ROOM0, // Certain rooms eg. inside the clock tower "ROOM0"
    /* 0x40 */ CAM_SET_RCIRC0, // Used by a few NPC cutscenes, focus close on the NPC "RCIRC0"
    /* 0x41 */ CAM_SET_CIRCLE9, // Used by Sakon Hideout entrance and Deku Palace Maze "CIRCLE9"
    /* 0x42 */ CAM_SET_ONTHEPOLE, // Somewhere in Snowhead Temple and Woodfall Temple "ONTHEPOLE"
    /* 0x43 */ CAM_SET_INBUSH, // Various bush environments eg. grottos, Swamp Spider House, Termina Field grass bushes, Deku Palace near bean "INBUSH"
    /* 0x44 */ CAM_SET_BOSS_MAJORA, // Majora's Lair: "BOSS_LAST" 
    /* 0x45 */ CAM_SET_BOSS_TWINMOLD, // Twinmold's Lair: "BOSS_INI"
    /* 0x46 */ CAM_SET_BOSS_GOHT, // Goht's Lair: "BOSS_HAK" 
    /* 0x47 */ CAM_SET_BOSS_GYORG, // Gyorg's Lair: "BOSS_KON"
    /* 0x48 */ CAM_SET_CONNECT0, // Smoothly and gradually return camera to Player after a cutscene "CONNECT0"
    /* 0x49 */ CAM_SET_PINNACLE_ROCK, // Pinnacle Rock pit "MORAY"
    /* 0x4A */ CAM_SET_NORMAL2_MM, // Generic camera 2, used in various places "NORMAL2"
    /* 0x4B */ CAM_SET_HONEY_AND_DARLING_2, // "BOMBBOWL"
    /* 0x4C */ CAM_SET_CIRCLEA, // Unknown, Circle 10 "CIRCLEA"
    /* 0x4D */ CAM_SET_WHIRLPOOL, // Great Bay Temple Central Room Whirlpool "WHIRLPOOL"
    /* 0x4E */ CAM_SET_CUCCO_SHACK, // "KOKKOGAME"
    /* 0x4F */ CAM_SET_GIANT, // Giants Mask in Twinmold's Lair "GIANT"
    /* 0x50 */ CAM_SET_SCENE0, // Entering doors to a new scene "SCENE0"
    /* 0x51 */ CAM_SET_ROOM1, // Certain rooms eg. some rooms in Stock Pot Inn "ROOM1"
    /* 0x52 */ CAM_SET_WATER2, // Swimming as Zora in Great Bay Temple "WATER2"
    /* 0x53 */ CAM_SET_WOODFALL_SWAMP, // Woodfall inside the swamp, but not on the platforms, "SOKONASI"
    /* 0x54 */ CAM_SET_FORCEKEEP, // Unknown "FORCEKEEP"
    /* 0x55 */ CAM_SET_PARALLEL1, // Unknown "PARALLEL1"
    /* 0x56 */ CAM_SET_START1_MM, // Used when entering the lens cave "START1"
    /* 0x57 */ CAM_SET_ROOM2, // Certain rooms eg. Deku King's Chamber, Ocean Spider House "ROOM2"
    /* 0x58 */ CAM_SET_NORMAL4_MM, // Generic camera 4, used in Ikana Graveyard "NORMAL4"
    /* 0x59 */ CAM_SET_ELEGY_SHELL, // cutscene after playing elegy of emptyness and spawning a shell "SHELL"
    /* 0x5A */ CAM_SET_DUNGEON4, // Used in Pirates Fortress Interior, hidden room near hookshot "DUNGEON4"
    /* 0x5B */ CAM_SET_MAX_MM
} CameraSettingTypeMM;

//#define PLAYER_CSACTION_4 5

typedef enum PlayerCsAction {
    /*   -1 */ PLAYER_CSACTION_NEG1 = -1, // Specific to Kafei, any negative number works
    /* 0x00 */ PLAYER_CSACTION_NONE,
    /* 0x01 */ PLAYER_CSACTION_1,
    /* 0x02 */ PLAYER_CSACTION_2,
    /* 0x03 */ PLAYER_CSACTION_3,
    /* 0x04 */ PLAYER_CSACTION_4 = 0x0005,	// Shocked
    /* 0x05 */ PLAYER_CSACTION_5,
    /* 0x06 */ PLAYER_CSACTION_END,
    /* 0x07 */ PLAYER_CSACTION_WAIT,
    /* 0x08 */ PLAYER_CSACTION_8,
    /* 0x09 */ PLAYER_CSACTION_9,
    /* 0x0A */ PLAYER_CSACTION_10,
    /* 0x0B */ PLAYER_CSACTION_11,
    /* 0x0C */ PLAYER_CSACTION_12,
    /* 0x0D */ PLAYER_CSACTION_13,
    /* 0x0E */ PLAYER_CSACTION_14,
    /* 0x0F */ PLAYER_CSACTION_15,
    /* 0x10 */ PLAYER_CSACTION_16,
    /* 0x11 */ PLAYER_CSACTION_17,
    /* 0x12 */ PLAYER_CSACTION_18, // Strangled by Wallmaster
    /* 0x13 */ PLAYER_CSACTION_19,
    /* 0x14 */ PLAYER_CSACTION_20,
    /* 0x15 */ PLAYER_CSACTION_21 = 0x0039,	// Look Around
    /* 0x16 */ PLAYER_CSACTION_22,
    /* 0x17 */ PLAYER_CSACTION_23,
    /* 0x18 */ PLAYER_CSACTION_24,
    /* 0x19 */ PLAYER_CSACTION_25,
    /* 0x1A */ PLAYER_CSACTION_26, // Halt!
    /* 0x1B */ PLAYER_CSACTION_27,
    /* 0x1C */ PLAYER_CSACTION_28,
    /* 0x1D */ PLAYER_CSACTION_29,
    /* 0x1E */ PLAYER_CSACTION_30,
    /* 0x1F */ PLAYER_CSACTION_31,
    /* 0x20 */ PLAYER_CSACTION_32,
    /* 0x21 */ PLAYER_CSACTION_33,
    /* 0x22 */ PLAYER_CSACTION_34,
    /* 0x23 */ PLAYER_CSACTION_35,
    /* 0x24 */ PLAYER_CSACTION_36,
    /* 0x25 */ PLAYER_CSACTION_37,
    /* 0x26 */ PLAYER_CSACTION_38,
    /* 0x27 */ PLAYER_CSACTION_39,
    /* 0x28 */ PLAYER_CSACTION_40,
    /* 0x29 */ PLAYER_CSACTION_41,
    /* 0x2A */ PLAYER_CSACTION_42,
    /* 0x2B */ PLAYER_CSACTION_43,
    /* 0x2C */ PLAYER_CSACTION_44,
    /* 0x2D */ PLAYER_CSACTION_45,
    /* 0x2E */ PLAYER_CSACTION_46,
    /* 0x2F */ PLAYER_CSACTION_47,
    /* 0x30 */ PLAYER_CSACTION_48,
    /* 0x31 */ PLAYER_CSACTION_49,
    /* 0x32 */ PLAYER_CSACTION_50,
    /* 0x33 */ PLAYER_CSACTION_51,
    /* 0x34 */ PLAYER_CSACTION_52,
    /* 0x35 */ PLAYER_CSACTION_53,
    /* 0x36 */ PLAYER_CSACTION_54,
    /* 0x37 */ PLAYER_CSACTION_55,
    /* 0x38 */ PLAYER_CSACTION_56,
    /* 0x39 */ PLAYER_CSACTION_57,
    /* 0x3A */ PLAYER_CSACTION_58,
    /* 0x3B */ PLAYER_CSACTION_59,
    /* 0x3C */ PLAYER_CSACTION_60,
    /* 0x3D */ PLAYER_CSACTION_61,
    /* 0x3E */ PLAYER_CSACTION_62,
    /* 0x3F */ PLAYER_CSACTION_63,
    /* 0x40 */ PLAYER_CSACTION_64,
    /* 0x41 */ PLAYER_CSACTION_65,
    /* 0x42 */ PLAYER_CSACTION_66, // Look side-to-side with chin down
    /* 0x43 */ PLAYER_CSACTION_67,
    /* 0x44 */ PLAYER_CSACTION_68,
    /* 0x45 */ PLAYER_CSACTION_69,
    /* 0x46 */ PLAYER_CSACTION_70,
    /* 0x47 */ PLAYER_CSACTION_71,
    /* 0x48 */ PLAYER_CSACTION_72,
    /* 0x49 */ PLAYER_CSACTION_73,
    /* 0x4A */ PLAYER_CSACTION_74, // Give a big nod of approval
    /* 0x4B */ PLAYER_CSACTION_75,
    /* 0x4C */ PLAYER_CSACTION_76,
    /* 0x4D */ PLAYER_CSACTION_77,
    /* 0x4E */ PLAYER_CSACTION_78,
    /* 0x4F */ PLAYER_CSACTION_79,
    /* 0x50 */ PLAYER_CSACTION_80,
    /* 0x51 */ PLAYER_CSACTION_81, // Look side-to-side with chin up
    /* 0x52 */ PLAYER_CSACTION_82, // Close eyes and sway body in circles
    /* 0x53 */ PLAYER_CSACTION_83,
    /* 0x54 */ PLAYER_CSACTION_84, // Sucked by the moon
    /* 0x55 */ PLAYER_CSACTION_85,
    /* 0x56 */ PLAYER_CSACTION_86,
    /* 0x57 */ PLAYER_CSACTION_87,
    /* 0x58 */ PLAYER_CSACTION_88,
    /* 0x59 */ PLAYER_CSACTION_89,
    /* 0x5A */ PLAYER_CSACTION_90,
    /* 0x5B */ PLAYER_CSACTION_91,
    /* 0x5C */ PLAYER_CSACTION_92,
    /* 0x5D */ PLAYER_CSACTION_93,
    /* 0x5E */ PLAYER_CSACTION_94,
    /* 0x5F */ PLAYER_CSACTION_95,
    /* 0x60 */ PLAYER_CSACTION_96,
    /* 0x61 */ PLAYER_CSACTION_97,
    /* 0x62 */ PLAYER_CSACTION_98,
    /* 0x63 */ PLAYER_CSACTION_99,
    /* 0x64 */ PLAYER_CSACTION_100,
    /* 0x65 */ PLAYER_CSACTION_101,
    /* 0x66 */ PLAYER_CSACTION_102,
    /* 0x67 */ PLAYER_CSACTION_103,
    /* 0x68 */ PLAYER_CSACTION_104,
    /* 0x69 */ PLAYER_CSACTION_105,
    /* 0x6A */ PLAYER_CSACTION_106,
    /* 0x6B */ PLAYER_CSACTION_107,
    /* 0x6C */ PLAYER_CSACTION_108,
    /* 0x6D */ PLAYER_CSACTION_109,
    /* 0x6E */ PLAYER_CSACTION_110,
    /* 0x6F */ PLAYER_CSACTION_111,
    /* 0x70 */ PLAYER_CSACTION_112,
    /* 0x71 */ PLAYER_CSACTION_113,
    /* 0x72 */ PLAYER_CSACTION_114,
    /* 0x73 */ PLAYER_CSACTION_115,
    /* 0x74 */ PLAYER_CSACTION_116,
    /* 0x75 */ PLAYER_CSACTION_117,
    /* 0x76 */ PLAYER_CSACTION_118,
    /* 0x77 */ PLAYER_CSACTION_119,
    /* 0x78 */ PLAYER_CSACTION_120,
    /* 0x79 */ PLAYER_CSACTION_121,
    /* 0x7A */ PLAYER_CSACTION_122,
    /* 0x7B */ PLAYER_CSACTION_123,
    /* 0x7C */ PLAYER_CSACTION_124,
    /* 0x7D */ PLAYER_CSACTION_125,
    /* 0x7E */ PLAYER_CSACTION_126,
    /* 0x7F */ PLAYER_CSACTION_127,
    /* 0x80 */ PLAYER_CSACTION_128,
    /* 0x81 */ PLAYER_CSACTION_129,
    /* 0x82 */ PLAYER_CSACTION_130,
    /* 0x83 */ PLAYER_CSACTION_131,
    /* 0x84 */ PLAYER_CSACTION_132,
    /* 0x85 */ PLAYER_CSACTION_133,
    /* 0x86 */ PLAYER_CSACTION_134,
    /* 0x87 */ PLAYER_CSACTION_135,
    /* 0x88 */ PLAYER_CSACTION_136,
    /* 0x89 */ PLAYER_CSACTION_137,
    /* 0x8A */ PLAYER_CSACTION_138,
    /* 0x8B */ PLAYER_CSACTION_139,
    /* 0x8C */ PLAYER_CSACTION_MAX
} PlayerCsAction;

typedef enum HudVisibility {
    /*  0 */ HUD_VISIBILITY_IDLE,
    /*  1 */ HUD_VISIBILITY_NONE,
    /*  2 */ HUD_VISIBILITY_NONE_ALT, // Identical to HUD_VISIBILITY_NONE
    /*  3 */ HUD_VISIBILITY_HEARTS_WITH_OVERWRITE, // Uses Interface_UpdateButtonAlphas so gives the opportunity to dim only disabled buttons
    /*  4 */ HUD_VISIBILITY_A,
    /*  5 */ HUD_VISIBILITY_A_HEARTS_MAGIC_WITH_OVERWRITE, // Uses Interface_UpdateButtonAlphas so gives the opportunity to dim only disabled buttons
    /*  6 */ HUD_VISIBILITY_A_HEARTS_MAGIC_MINIMAP_WITH_OVERWRITE, // Uses Interface_UpdateButtonAlphas so gives the opportunity to dim only disabled buttons
    /*  7 */ HUD_VISIBILITY_ALL_NO_MINIMAP_W_DISABLED, // Only raises button alphas if not disabled
    /*  8 */ HUD_VISIBILITY_B,
    /*  9 */ HUD_VISIBILITY_HEARTS_MAGIC,
    /* 10 */ HUD_VISIBILITY_B_ALT,
    /* 11 */ HUD_VISIBILITY_HEARTS,
    /* 12 */ HUD_VISIBILITY_A_B_MINIMAP,
    /* 13 */ HUD_VISIBILITY_HEARTS_MAGIC_WITH_OVERWRITE, // Uses Interface_UpdateButtonAlphas so gives the opportunity to dim only disabled buttons
    /* 14 */ HUD_VISIBILITY_HEARTS_MAGIC_C,
    /* 15 */ HUD_VISIBILITY_ALL_NO_MINIMAP,
    /* 16 */ HUD_VISIBILITY_A_B_C,
    /* 17 */ HUD_VISIBILITY_B_MINIMAP,
    /* 18 */ HUD_VISIBILITY_HEARTS_MAGIC_MINIMAP,
    /* 19 */ HUD_VISIBILITY_A_HEARTS_MAGIC_MINIMAP,
    /* 20 */ HUD_VISIBILITY_B_MAGIC,
    /* 21 */ HUD_VISIBILITY_A_B,
    /* 22 */ HUD_VISIBILITY_A_B_HEARTS_MAGIC_MINIMAP,
    /* 50 */ HUD_VISIBILITY_ALL = 50,
    /* 52 */ HUD_VISIBILITY_NONE_INSTANT = 52
} HudVisibility;

#define Interface_SetHudVisibility Interface_ChangeAlpha

typedef enum {
    /* 0 */ CS_END_CAM_0,
    /* 1 */ CS_END_CAM_1,
    /* 2 */ CS_END_CAM_SMOOTH
} CutsceneEndCam;

#define CAM_STATE_UNDERWATER (1 << 8)

#define behaviorFlags unk_14A
#define CamerastateFlags unk_14C

/* Actor Cutscenes, which encompasses all cutscenes */

typedef struct {
    /* 0x00 */ s16 priority; // Lower means higher priority. -1 means it ignores priority
    /* 0x02 */ s16 length;
    /* 0x04 */ s16 csCamId; // Index of CsCameraEntry to use. Negative indices use sGlobalCamDataSettings. Indices 0 and above use CsCameraEntry from a sceneLayer
    /* 0x06 */ s16 scriptIndex;
    /* 0x08 */ s16 additionalCsId;
    /* 0x0A */ u8 endSfx;
    /* 0x0B */ u8 customValue; // 0 - 99: actor-specific custom value. 100+: spawn. 255: none
    /* 0x0C */ s16 hudVisibility; 
    /* 0x0E */ u8 endCam;
    /* 0x0F */ u8 letterboxSize;
} ActorCutscene; // size = 0x10
// TODO: rename `ActorCutscene` to `CutsceneEntry` once ZAPD uses `CutsceneScriptEntry`
// typedef CutsceneEntry ActorCutscene;

#define R_USE_DEBUG_CUTSCENE              dREG(95)

void Cutscene_StartScripted(PlayState* play, u8 scriptIndex) {
    if (!R_USE_DEBUG_CUTSCENE) {
        //play->csCtx.scriptIndex = scriptIndex;
        //play->csCtx.script = Lib_SegmentedToVirtual(play->csCtx.scriptList[scriptIndex].script);
    }

    gSaveContext.cutsceneTrigger = 1;
}

#define Player_SetCsActionWithHaltedActors func_8002DF54


#define CAM_FUNC_UNIQ2 CAM_FUNC_UNIQUE2
#define sSetNormal3ModeFreeFallData sSetNormal1ModeFreeFallData
#define CAM_FUNC_NORMAL1 CAM_FUNC_NORM1


/**
 * @file z_eventmgr.c
 *
 * Manages all cutscenes except for manual
 */

#include "z64shrink_window.h"
//#include "string.h"

ActorCutscene sGlobalCutsceneList[] = {
    // CS_ID_GLOBAL_78
    { -100, -1, CS_CAM_ID_NONE, CS_SCRIPT_ID_NONE, CS_ID_NONE, CS_END_SFX_NONE_ALT, 255, CS_HUD_VISIBILITY_ALL_ALT, 255,
      255 },
    // CS_ID_GLOBAL_79
    { -100, -1, CS_CAM_ID_NONE, CS_SCRIPT_ID_NONE, CS_ID_NONE, CS_END_SFX_NONE_ALT, 255, CS_HUD_VISIBILITY_ALL_ALT, 255,
      255 },
    // CS_ID_GLOBAL_7A
    { -100, -1, CS_CAM_ID_NONE, CS_SCRIPT_ID_NONE, CS_ID_NONE, CS_END_SFX_NONE_ALT, 255, CS_HUD_VISIBILITY_ALL_ALT, 255,
      255 },
    // CS_ID_GLOBAL_ELEGY
    { 2, -1, CS_CAM_ID_GLOBAL_ELEGY, CS_SCRIPT_ID_NONE, CS_ID_NONE, CS_END_SFX_NONE_ALT, 255, CS_HUD_VISIBILITY_NONE,
      CS_END_CAM_0, 32 },
    // CS_ID_GLOBAL_TALK
    { 32765, -1, CS_CAM_ID_NONE, CS_SCRIPT_ID_NONE, CS_ID_NONE, CS_END_SFX_NONE_ALT, 255, CS_HUD_VISIBILITY_ALL_ALT,
      CS_END_CAM_0, 255 },
    // CS_ID_GLOBAL_DOOR
    { 32764, -1, CS_CAM_ID_NONE, CS_SCRIPT_ID_NONE, CS_ID_NONE, CS_END_SFX_NONE_ALT, 255, CS_HUD_VISIBILITY_ALL_ALT,
      CS_END_CAM_0, 255 },
    // CS_ID_GLOBAL_RETURN_TO_CAM
    { 32766, -2, CS_CAM_ID_GLOBAL_CONNECT, CS_SCRIPT_ID_NONE, CS_ID_NONE, CS_END_SFX_NONE, 255,
      CS_HUD_VISIBILITY_ALL_ALT, CS_END_CAM_0, 32 },
    // CS_ID_GLOBAL_END
    { 0, -1, CS_CAM_ID_NONE, CS_SCRIPT_ID_NONE, CS_ID_NONE, CS_END_SFX_NONE, 255, CS_HUD_VISIBILITY_ALL_ALT,
      CS_END_CAM_0, 32 },
};

typedef enum {
    /* 0 */ CS_START_0,
    /* 1 */ CS_START_1,
    /* 2 */ CS_START_2
} ActorCutsceneStartMethod;

typedef struct {
    /* 0x00 */ s16 csId;
    /* 0x02 */ s16 length;
    /* 0x04 */ s16 endCsId;
    /* 0x06 */ s16 subCamId;
    /* 0x08 */ Actor* targetActor;
    /* 0x0C */ s32 startMethod;
    /* 0x10 */ PlayState* play;
    /* 0x14 */ s16 retCamId;
    /* 0x16 */ s16 isCameraStored;
} CutsceneManager; // size = 0x18

CutsceneManager sCutsceneMgr = {
    CS_ID_NONE, 0, CS_ID_NONE, SUB_CAM_ID_DONE, NULL, CS_START_0, NULL, CAM_ID_MAIN, false,
};

ActorCutscene* sSceneCutsceneList;
s16 sSceneCutsceneCount;
u8 sWaitingCutsceneList[16];
//static s32 sBssPad;
u8 sNextCutsceneList[16];
//static s32 sBssPad2;

s16 CutsceneManager_SetHudVisibility(s16 csHudVisibility) {
    u16 hudVisibility;

    switch (csHudVisibility) {
        case CS_HUD_VISIBILITY_NONE:
            hudVisibility = HUD_VISIBILITY_NONE_ALT;
            break;

        case CS_HUD_VISIBILITY_ALL:
            hudVisibility = HUD_VISIBILITY_ALL;
            break;

        case CS_HUD_VISIBILITY_A_HEARTS_MAGIC:
            hudVisibility = HUD_VISIBILITY_A_HEARTS_MAGIC_WITH_OVERWRITE;
            break;

        case CS_HUD_VISIBILITY_C_HEARTS_MAGIC:
            hudVisibility = HUD_VISIBILITY_HEARTS_MAGIC_C;
            break;

        case CS_HUD_VISIBILITY_ALL_NO_MINIMAP:
            hudVisibility = HUD_VISIBILITY_ALL_NO_MINIMAP;
            break;

        case CS_HUD_VISIBILITY_A_B_C:
            hudVisibility = HUD_VISIBILITY_A_B_C;
            break;

        case CS_HUD_VISIBILITY_B_MINIMAP:
            hudVisibility = HUD_VISIBILITY_B_MINIMAP;
            break;

        case CS_HUD_VISIBILITY_A:
            hudVisibility = HUD_VISIBILITY_A;
            break;

        default:
            hudVisibility = HUD_VISIBILITY_ALL;
            break;
    }

    Interface_SetHudVisibility(hudVisibility);

    return hudVisibility;
}

ActorCutscene* CutsceneManager_GetCutsceneEntryImpl(s16 csId) {
    if (csId < CS_ID_GLOBAL_78) {
        return &sSceneCutsceneList[csId];
    } else {
        csId -= CS_ID_GLOBAL_78;
        return &sGlobalCutsceneList[csId];
    }
}

void CutsceneManager_Init(PlayState* play, ActorCutscene* cutsceneList, s16 numEntries) {
    s32 i;

    sSceneCutsceneList = cutsceneList;
    sSceneCutsceneCount = numEntries;

    for (i = 0; i < ARRAY_COUNT(sWaitingCutsceneList); i++) {
        sWaitingCutsceneList[i] = 0;
        sNextCutsceneList[i] = 0;
    }

    sCutsceneMgr.endCsId = CS_ID_NONE;
    sCutsceneMgr.play = play;
    sCutsceneMgr.length = -1;
    sCutsceneMgr.targetActor = NULL;
    sCutsceneMgr.subCamId = SUB_CAM_ID_DONE;
    sCutsceneMgr.isCameraStored = false;
    sCutsceneMgr.csId = sCutsceneMgr.endCsId;
}

/**
 * Store camera into subCam 2, and keep subCam 2 INACTIVE to preserve the struct.
 */
void CutsceneManager_StoreCamera(Camera* camera) {
    if (camera != NULL) {
        memcpy(&sCutsceneMgr.play->subCameras[2], camera, sizeof(Camera));
        sCutsceneMgr.play->subCameras[2].camId = camera->camId;
        Camera_ChangeStatus(&sCutsceneMgr.play->subCameras[2], CAM_STATUS_INACTIVE);
        sCutsceneMgr.isCameraStored = true;
    }
}

void CutsceneManager_ClearWaiting(void) {
    s32 i;

    for (i = 0; i < ARRAY_COUNT(sWaitingCutsceneList); i++) {
        sWaitingCutsceneList[i] = 0;
    }
}

void CutsceneManager_ClearNextCutscenes(void) {
    s32 i;

    for (i = 0; i < ARRAY_COUNT(sNextCutsceneList); i++) {
        sNextCutsceneList[i] = 0;
    }
}

s16 CutsceneManager_MarkNextCutscenes(void) {
    s16 bit;
    s32 i;
    s32 j;
    s32 count = 0;
    s16 csIdMax = CS_ID_NONE;
    s16 priorityMax = SHT_MAX; // lower number means higher priority
    s16 csId;
    s16 priority;

    for (i = 0; i < ARRAY_COUNT(sNextCutsceneList); i++) {
        for (bit = 1, j = 0; j < 8; j++) {
            if (sWaitingCutsceneList[i] & bit) {
                csId = (i << 3) | j;
                priority = CutsceneManager_GetCutsceneEntryImpl(csId)->priority;

                if ((priority ^ 0) == -1) {
                    sNextCutsceneList[i] |= bit;
                } else if ((priority < priorityMax) && (priority > 0)) {
                    csIdMax = csId;
                    priorityMax = priority;
                }
                count++;
            }
            bit <<= 1;
        }
    }
    if (csIdMax != CS_ID_NONE) {
        sNextCutsceneList[csIdMax >> 3] |= 1 << (csIdMax & 7);
    }
    return count;
}

#define RET_CAM sCutsceneMgr.play->cameraPtrs[sCutsceneMgr.retCamId]
#define CUR_CAM sCutsceneMgr.play->cameraPtrs[sCutsceneMgr.subCamId]

void CutsceneManager_End(void) {
    ActorCutscene* csEntry;
    s16 oldCamId;
    s16 oldCamerastateFlags;

    switch (sCutsceneMgr.startMethod) {
        case CS_START_2:
            sCutsceneMgr.targetActor->flags &= ~ACTOR_FLAG_100000;
            // fallthrough
        case CS_START_1:
            Player_SetCsActionWithHaltedActors(sCutsceneMgr.play, NULL, PLAYER_CSACTION_END);
            sCutsceneMgr.startMethod = CS_START_0;
            break;

        default:
            break;
    }

    csEntry = CutsceneManager_GetCutsceneEntryImpl(sCutsceneMgr.csId);

    switch (csEntry->endSfx) {
        case CS_END_SFX_TRE_BOX_APPEAR:
            //Audio_PlaySfx(NA_SE_SY_TRE_BOX_APPEAR);
            break;

        case CS_END_SFX_CORRECT_CHIME:
            //Audio_PlaySfx(NA_SE_SY_CORRECT_CHIME);
            break;

        default: // CS_END_SFX_NONE
            break;
    }

    switch (csEntry->endCam) {
        case CS_END_CAM_SMOOTH:
            Play_CopyCamera(sCutsceneMgr.play, sCutsceneMgr.retCamId, sCutsceneMgr.subCamId);
            RET_CAM->CamerastateFlags =
                (RET_CAM->CamerastateFlags & ~CAM_STATE_UNDERWATER) | (CUR_CAM->CamerastateFlags & CAM_STATE_UNDERWATER);
            CutsceneManager_Queue(CS_ID_GLOBAL_RETURN_TO_CAM);
            break;

        case CS_END_CAM_0:
        default:
            Play_CopyCamera(sCutsceneMgr.play, sCutsceneMgr.retCamId, sCutsceneMgr.subCamId);
            RET_CAM->CamerastateFlags =
                (RET_CAM->CamerastateFlags & ~CAM_STATE_UNDERWATER) | (CUR_CAM->CamerastateFlags & CAM_STATE_UNDERWATER);
            break;

        case CS_END_CAM_1:
            oldCamId = RET_CAM->camId;
            oldCamerastateFlags = RET_CAM->CamerastateFlags;

            if (sCutsceneMgr.isCameraStored) {
                // Restore the camera that was stored in subCam 2
                memcpy(RET_CAM, &sCutsceneMgr.play->subCameras[2], sizeof(Camera));

                RET_CAM->CamerastateFlags =
                    (RET_CAM->CamerastateFlags & ~CAM_STATE_UNDERWATER) | (CUR_CAM->CamerastateFlags & CAM_STATE_UNDERWATER);

                RET_CAM->CamerastateFlags = (RET_CAM->CamerastateFlags & ~CAM_STATE_2) | (oldCamerastateFlags & CAM_STATE_2);
                sCutsceneMgr.isCameraStored = false;
            }
            RET_CAM->camId = oldCamId;
            break;
    }

    if (sCutsceneMgr.subCamId != SUB_CAM_ID_DONE) {
        Play_ClearCamera(sCutsceneMgr.play, sCutsceneMgr.subCamId);
        Play_ChangeCameraStatus(sCutsceneMgr.play, sCutsceneMgr.retCamId, CAM_STATUS_ACTIVE);
    }

    sCutsceneMgr.csId = CS_ID_NONE;
    sCutsceneMgr.endCsId = CS_ID_NONE;
    sCutsceneMgr.length = -1;
    sCutsceneMgr.targetActor = NULL;
    sCutsceneMgr.subCamId = SUB_CAM_ID_DONE;
}

s16 CutsceneManager_Update(void) {
    s16 sp1E = 0;

    if (CutsceneManager_IsNext(CS_ID_GLOBAL_RETURN_TO_CAM)) {
        CutsceneManager_StartWithPlayerCs(CS_ID_GLOBAL_RETURN_TO_CAM, &GET_PLAYER(sCutsceneMgr.play)->actor);
    }

    if (sCutsceneMgr.endCsId == CS_ID_NONE) {
        if (sCutsceneMgr.csId != CS_ID_NONE) {
            if (sCutsceneMgr.length > 0) {
                sCutsceneMgr.length--;
            }
            sp1E = 1;
            if (sCutsceneMgr.length == 0) {
                CutsceneManager_Stop(sCutsceneMgr.csId);
            }
        }
    }

    if (sCutsceneMgr.endCsId != CS_ID_NONE) {
        CutsceneManager_End();
        sp1E = 2;
    }

    CutsceneManager_ClearNextCutscenes();

    if (sCutsceneMgr.csId == CS_ID_NONE) {
        if ((CutsceneManager_MarkNextCutscenes() == 0) && (sp1E != 0)) {
            //ShrinkWindow_Letterbox_SetSizeTarget(0);
        } else if (sp1E == 0) {
            CutsceneManager_StoreCamera(Play_GetCamera(sCutsceneMgr.play, sCutsceneMgr.retCamId));
        }
    }
    return sp1E;
}

void CutsceneManager_Queue(s16 csId) {
    if (csId >= 0) {
        sWaitingCutsceneList[csId >> 3] |= 1 << (csId & 7);
    }
}

s16 CutsceneManager_IsNext(s16 csId) {
    if (csId == CS_ID_GLOBAL_END) {
        if (sCutsceneMgr.csId == CS_ID_NONE) {
            return 0x7F;
        } else {
            return 0;
        }
    }
    if (csId <= CS_ID_NONE) {
        return -1;
    }
    return (sNextCutsceneList[csId >> 3] & (1 << (csId & 7))) ? 1 : 0;
}

/**
 * Start an actor cutscene, activate Player Cutscene Action "Wait"
 */
s16 CutsceneManager_StartWithPlayerCs(s16 csId, Actor* actor) {
    s16 startCsId = CutsceneManager_Start(csId, actor);

    if (startCsId >= 0) {
        Player_SetCsActionWithHaltedActors(sCutsceneMgr.play, NULL, PLAYER_CSACTION_WAIT);
        if (sCutsceneMgr.length == 0) {
            CutsceneManager_Stop(sCutsceneMgr.csId);
        }
        sCutsceneMgr.startMethod = CS_START_1;
    }
    return startCsId;
}

/**
 * Start an actor cutscene, activate Player Cutscene Action "Wait", turn on ACTOR_FLAG_100000
 */
s16 CutsceneManager_StartWithPlayerCsAndSetFlag(s16 csId, Actor* actor) {
    s16 startCsId = CutsceneManager_Start(csId, actor);

    if (startCsId >= 0) {
        Player_SetCsActionWithHaltedActors(sCutsceneMgr.play, NULL, PLAYER_CSACTION_WAIT);
        if (sCutsceneMgr.length == 0) {
            CutsceneManager_Stop(sCutsceneMgr.csId);
        }
        if (actor != NULL) {
            actor->flags |= ACTOR_FLAG_100000;
            sCutsceneMgr.startMethod = CS_START_2;
        } else {
            sCutsceneMgr.startMethod = CS_START_1;
        }
    }
    return startCsId;
}

s16 CutsceneManager_Start(s16 csId, Actor* actor) {
    ActorCutscene* csEntry;
    Camera* subCam;
    Camera* retCam;
    s32 csType = 0;

    if ((csId <= CS_ID_NONE) || (sCutsceneMgr.csId != CS_ID_NONE)) {
        return csId;
    }

    sCutsceneMgr.startMethod = CS_START_0;
    csEntry = CutsceneManager_GetCutsceneEntryImpl(csId);

    //ShrinkWindow_Letterbox_SetSizeTarget(csEntry->letterboxSize);
    CutsceneManager_SetHudVisibility(csEntry->hudVisibility);

    if (csId == CS_ID_GLOBAL_END) {
        csType = 1;
    } else if (csEntry->scriptIndex != CS_SCRIPT_ID_NONE) {
        // scripted cutscene
        csType = 1;
    } else if ((csId != CS_ID_GLOBAL_DOOR) && (csId != CS_ID_GLOBAL_TALK)) {
        csType = 2;
    }

    if (csType != 0) {
        sCutsceneMgr.retCamId = Play_GetActiveCamId(sCutsceneMgr.play);
        sCutsceneMgr.subCamId = Play_CreateSubCamera(sCutsceneMgr.play);

        subCam = Play_GetCamera(sCutsceneMgr.play, sCutsceneMgr.subCamId);
        retCam = Play_GetCamera(sCutsceneMgr.play, sCutsceneMgr.retCamId);

        if ((retCam->setting == CAM_SET_START0) || (retCam->setting == CAM_SET_START2) ||
            (retCam->setting == CAM_SET_START1)) {
            if (CutsceneManager_FindEntranceCsId() != csId) {
                func_800E0348(retCam);
            } else {
                Camera_UnsetStateFlag(retCam, CAM_STATE_2);
            }
        }

        memcpy(subCam, retCam, sizeof(Camera));
        subCam->camId = sCutsceneMgr.subCamId;
        Camera_UnsetStateFlag(subCam, CAM_STATE_6 | CAM_STATE_0);

        Play_ChangeCameraStatus(sCutsceneMgr.play, sCutsceneMgr.retCamId, CAM_STATUS_WAIT);
        Play_ChangeCameraStatus(sCutsceneMgr.play, sCutsceneMgr.subCamId, CAM_STATUS_ACTIVE);

        subCam->target = sCutsceneMgr.targetActor = actor;
        subCam->behaviorFlags = 0;

        if (csType == 1) {
            Camera_ChangeSetting(subCam, CAM_SET_FREE0);
            Cutscene_StartScripted(sCutsceneMgr.play, csEntry->scriptIndex);
            sCutsceneMgr.length = csEntry->length;
        } else {
            if (csEntry->csCamId != CS_CAM_ID_NONE) {
                Camera_ChangeActorCsCamIndex(subCam, csEntry->csCamId);
            } else {
                Camera_ChangeSetting(subCam, CAM_SET_FREE0);
            }
            sCutsceneMgr.length = csEntry->length;
        }
    }
    sCutsceneMgr.csId = csId;

    return csId;
}

s16 CutsceneManager_Stop(s16 csId) {
    ActorCutscene* csEntry;

    if (csId <= CS_ID_NONE) {
        return csId;
    }

    csEntry = CutsceneManager_GetCutsceneEntryImpl(sCutsceneMgr.csId);
    if ((sCutsceneMgr.length > 0) && (csEntry->scriptIndex == CS_SCRIPT_ID_NONE)) {
        return -2;
    }
    if ((csId != CS_ID_GLOBAL_END) && (csEntry->scriptIndex != CS_SCRIPT_ID_NONE)) {
        return -3;
    }

    if (csId == CS_ID_GLOBAL_END) {
        csId = sCutsceneMgr.csId;
    }
    if (csId == sCutsceneMgr.csId) {
        sCutsceneMgr.endCsId = sCutsceneMgr.csId;
        return sCutsceneMgr.endCsId;
    }
    return -1;
}

s16 CutsceneManager_GetCurrentCsId(void) {
    return sCutsceneMgr.csId;
}

ActorCutscene* CutsceneManager_GetCutsceneEntry(s16 csId) {
    return CutsceneManager_GetCutsceneEntryImpl(csId);
}

s16 CutsceneManager_GetAdditionalCsId(s16 csId) {
    if (csId <= CS_ID_NONE) {
        return CS_ID_NONE;
    }
    return CutsceneManager_GetCutsceneEntryImpl(csId)->additionalCsId;
}

s16 CutsceneManager_GetLength(s16 csId) {
    if (csId <= CS_ID_NONE) {
        return -1;
    }
    return CutsceneManager_GetCutsceneEntryImpl(csId)->length;
}

s16 CutsceneManager_GetCutsceneScriptIndex(s16 csId) {
    if (csId <= CS_ID_NONE) {
        return -1;
    }
    return CutsceneManager_GetCutsceneEntryImpl(csId)->scriptIndex;
}

s16 CutsceneManager_GetCutsceneCustomValue(s16 csId) {
    if (csId <= CS_ID_NONE) {
        return -1;
    }
    return CutsceneManager_GetCutsceneEntryImpl(csId)->customValue;
}

s16 CutsceneManager_GetCurrentSubCamId(s16 csId) {
    return sCutsceneMgr.subCamId;
}


s16 CutsceneManager_FindEntranceCsId(void) {
    PlayState* play;
    s32 csId;

    for (csId = 0; csId < sSceneCutsceneCount; csId++) {
        //! FAKE:
        /*if ((sSceneCutsceneList[csId].scriptIndex != CS_SCRIPT_ID_NONE) &&
            (sSceneCutsceneList[csId].scriptIndex < (play = sCutsceneMgr.play)->csCtx2.scriptListCount) &&
            (sCutsceneMgr.play->curSpawn ==
             sCutsceneMgr.play->csCtx2.scriptList[sSceneCutsceneList[csId].scriptIndex].spawn)) {
            return csId;
        }*/
    }

    for (csId = 0; csId < sSceneCutsceneCount; csId++) {
        if ((sSceneCutsceneList[csId].customValue >= 100) &&
            (sSceneCutsceneList[csId].customValue == (sCutsceneMgr.play->curSpawn + 100))) {
            return csId;
        }
    }

    return -1;
}

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  data1;
    /* 0x4 */ u32 data2;
} SCmdBaseMM; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  data1;
    /* 0x4 */ void* segment;
} SCmdSpawnListMM; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  num;
    /* 0x4 */ void* segment;
} SCmdActorListMM; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  data1;
    /* 0x4 */ void* segment;
} SCmdCsCameraListMM; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  data1;
    /* 0x4 */ void* segment;
} SCmdColHeaderMM; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  num;
    /* 0x4 */ void* segment;
} SCmdRoomListMM; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  data1;
    /* 0x2 */ UNK_TYPE1 pad2[2];
    /* 0x4 */ s8  west;
    /* 0x5 */ s8  vertical;
    /* 0x6 */ s8  south;
    /* 0x7 */ u8  clothIntensity;
} SCmdWindSettingsMM; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  data1;
    /* 0x4 */ void* segment;
} SCmdEntranceListMM; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  naviQuestHintFileId;
    /* 0x4 */ u32 subKeepId;
} SCmdSpecialFilesMM; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  gpFlag1;
    /* 0x4 */ u32 gpFlag2;
} SCmdRoomBehaviorMM; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  data1;
    /* 0x4 */ void* segment;
} SCmdMeshMM; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  num;
    /* 0x4 */ void* segment;
} SCmdObjectListMM; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  num;
    /* 0x4 */ void* segment;
} SCmdLightListMM; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  data1;
    /* 0x4 */ void* segment;
} SCmdPathListMM; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  num;
    /* 0x4 */ void* segment;
} SCmdTransitionActorListMM; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  num;
    /* 0x4 */ void* segment;
} SCmdLightSettingListMM; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  data1;
    /* 0x2 */ UNK_TYPE1 pad2[2];
    /* 0x4 */ u8  hour;
    /* 0x5 */ u8  min;
    /* 0x6 */ u8  timeSpeed;
} SCmdTimeSettingsMM; // size = 0x7

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  data1;
    /* 0x2 */ UNK_TYPE1 pad2[2];
    /* 0x4 */ u8  skyboxId;
    /* 0x5 */ u8  skyboxConfig;
    /* 0x6 */ u8  envLightMode;
} SCmdSkyboxSettingsMM; // size = 0x7

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  data1;
    /* 0x2 */ UNK_TYPE1 pad2[2];
    /* 0x4 */ u8  unk4;
    /* 0x5 */ u8  unk5;
} SCmdSkyboxDisablesMM; // size = 0x6

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  data1;
    /* 0x4 */ void* segment;
} SCmdExitListMM; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  data1;
    /* 0x4 */ u32 data2;
} SCmdEndMarkerMM; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  specId;
    /* 0x2 */ UNK_TYPE1 unk_02[4];
    /* 0x6 */ u8  ambienceId;
    /* 0x7 */ u8  seqId;
} SCmdSoundSettingsMM; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  data1;
    /* 0x2 */ UNK_TYPE1 unk_02[5];
    /* 0x7 */ u8  echo;
} SCmdEchoSettingsMM; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  data1;
    /* 0x4 */ void* segment;
} SCmdCutsceneDataMM; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  scriptListCount;
    /* 0x4 */ void* segment;
} SCmdCsScriptListMM; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  data1;
    /* 0x4 */ void* segment;
} SCmdAltHeadersMM; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ UNK_TYPE1 pad[7];
} SCmdRegionVisitedMM; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  data1;
    /* 0x4 */ void* segment;
} SCmdTextureAnimationsMM; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  num;
    /* 0x4 */ void* segment;
} SCmdCutsceneListMM; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  data1;
    /* 0x4 */ void* segment;
} SCmdMapDataMM; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  num;
    /* 0x4 */ void* segment;
} SCmdMapDataChestsMM; // size = 0x8

// TODO: consider merging with bgCamInfo?
typedef struct {
    /* 0x0 */ s16 setting; // camera setting described by CameraSettingType enum
    /* 0x2 */ s16 count;
    /* 0x4 */ Vec3s* actorCsCamFuncData; // s16 data grouped in threes
} ActorCsCamInfo; // size = 0x8

typedef union {
    /* Command: N/A  */ SCmdBaseMM                base;
    /* Command: 0x00 */ SCmdSpawnListMM           spawnList;
    /* Command: 0x01 */ SCmdActorListMM           actorList;
    /* Command: 0x02 */ SCmdCsCameraListMM        actorCsCamList;
    /* Command: 0x03 */ SCmdColHeaderMM           colHeader;
    /* Command: 0x04 */ SCmdRoomListMM            roomList;
    /* Command: 0x05 */ SCmdWindSettingsMM        windSettings;
    /* Command: 0x06 */ SCmdEntranceListMM        entranceList;
    /* Command: 0x07 */ SCmdSpecialFilesMM        specialFiles;
    /* Command: 0x08 */ SCmdRoomBehaviorMM        roomBehavior;
    /* Command: 0x09 */ // Unused
    /* Command: 0x0A */ SCmdMeshMM                mesh;
    /* Command: 0x0B */ SCmdObjectListMM          objectList;
    /* Command: 0x0C */ SCmdLightListMM           lightList;
    /* Command: 0x0D */ SCmdPathListMM            pathList;
    /* Command: 0x0E */ SCmdTransitionActorListMM transitionActorList;
    /* Command: 0x0F */ SCmdLightSettingListMM    lightSettingList;
    /* Command: 0x10 */ SCmdTimeSettingsMM        timeSettings;
    /* Command: 0x11 */ SCmdSkyboxSettingsMM      skyboxSettings;
    /* Command: 0x12 */ SCmdSkyboxDisablesMM      skyboxDisables;
    /* Command: 0x13 */ SCmdExitListMM            exitList;
    /* Command: 0x14 */ SCmdEndMarkerMM           endMarker;
    /* Command: 0x15 */ SCmdSoundSettingsMM       soundSettings;
    /* Command: 0x16 */ SCmdEchoSettingsMM        echoSettings;
    /* Command: 0x17 */ SCmdCsScriptListMM        scriptList;
    /* Command: 0x18 */ SCmdAltHeadersMM          altHeaders;
    /* Command: 0x19 */ SCmdRegionVisitedMM      regionVisited;
    /* Command: 0x1A */ SCmdTextureAnimationsMM   textureAnimations;
    /* Command: 0x1B */ SCmdCutsceneListMM        cutsceneList;
    /* Command: 0x1C */ SCmdMapDataMM             mapData;
    /* Command: 0x1D */ // Unused
    /* Command: 0x1E */ SCmdMapDataChestsMM       mapDataChests;
} SceneCmdMM; // size = 0x8

#define actorCsCamList unk_11DFC

u16 Play_GetActorCsCamSetting(PlayState* this, s32 csCamDataIndex) {
    ActorCsCamInfo* actorCsCamList = &this->actorCsCamList[csCamDataIndex];

    return actorCsCamList->setting;
}

s32 func_800F22C4(s16 csId, Actor* actor) {
    f32 dist;
    s16 screenPosX;
    s16 screenPosY;

    if ((sCutsceneMgr.csId == CS_ID_NONE) || (csId == CS_ID_NONE)) {
        return 4;
    }

    Actor_GetScreenPos(sCutsceneMgr.play, actor, &screenPosX, &screenPosY);

    dist = OLib_Vec3fDist(&actor->focus.pos, &Play_GetCamera(sCutsceneMgr.play, sCutsceneMgr.subCamId)->eye);

    if ((screenPosX > 40) && (screenPosX < SCREEN_WIDTH - 40) && (screenPosY > 40) &&
        (screenPosY < SCREEN_HEIGHT - 40) && (dist < 700.0f)) {
        return 1;
    }
    if (sCutsceneMgr.length < 6) {
        return 2;
    }
    if (csId == sCutsceneMgr.csId) {
        return 0;
    }
    return 3;
}

void CutsceneManager_SetReturnCamera(s16 camId) {
    sCutsceneMgr.retCamId = camId;
}

void func_800E0348(Camera* camera) {
    if (!RELOAD_PARAMS(camera)) {
        camera->animState = 999;
        //Camera_SetStateFlag(camera, CAM_STATE_10 | CAM_STATE_4 | CAM_STATE_2 | CAM_STATE_CHECK_WATER);
    } else {
        camera->animState = 666;
    }
}

#define CAM_DATA_IS_BG (1 << 12) // if not set, then cam data is for actor cutscenes

/**
 * Returns the CameraSettingType of the camera from either the bgCam or the actorCsCam at index `camDataId`
 */
s16 Camera_GetBgCamOrActorCsCamSetting(Camera* camera, u32 camDataId) {
    if (camDataId & CAM_DATA_IS_BG) {
        return BgCheck_GetBgCamSettingImpl(&camera->play->colCtx, camDataId & ~CAM_DATA_IS_BG, BGCHECK_SCENE);
    } else {
        return Play_GetActorCsCamSetting(camera->play, camDataId);
    }
}

s16 sGlobalCamDataSettings[] = {
    /* -25 */ CAM_SET_ELEGY_SHELL,         // CS_CAM_ID_GLOBAL_ELEGY
    /* -24 */ CAM_SET_SIDED,               // CS_CAM_ID_GLOBAL_SIDED
    /* -23 */ CAM_SET_BOAT_CRUISE,         // CS_CAM_ID_GLOBAL_BOAT_CRUISE
    /* -22 */ CAM_SET_NONE,                // CS_CAM_ID_GLOBAL_N16
    /* -21 */ CAM_SET_SUBJECTD,            // CS_CAM_ID_GLOBAL_SUBJECTD
    /* -20 */ CAM_SET_NORMALD,             // CS_CAM_ID_GLOBAL_NORMALD
    /* -19 */ CAM_SET_NONE,                // CS_CAM_ID_GLOBAL_N13
    /* -18 */ CAM_SET_NONE,                // CS_CAM_ID_GLOBAL_N12
    /* -17 */ CAM_SET_NONE,                // CS_CAM_ID_GLOBAL_N11
    /* -16 */ CAM_SET_WARP_PAD_ENTRANCE,   // CS_CAM_ID_GLOBAL_WARP_PAD_ENTRANCE
    /* -15 */ CAM_SET_ATTENTION,           // CS_CAM_ID_GLOBAL_ATTENTION
    /* -14 */ CAM_SET_CONNECT0,            // CS_CAM_ID_GLOBAL_CONNECT
    /* -13 */ CAM_SET_REMOTEBOMB,          // CS_CAM_ID_GLOBAL_REMOTE_BOMB
    /* -12 */ CAM_SET_NONE,                // CS_CAM_ID_GLOBAL_N0C
    /* -11 */ CAM_SET_MASK_TRANSFORMATION, // CS_CAM_ID_GLOBAL_MASK_TRANSFORMATION
    /* -10 */ CAM_SET_LONG_CHEST_OPENING,  // CS_CAM_ID_GLOBAL_LONG_CHEST_OPENING
    /*  -9 */ CAM_SET_REBIRTH,             // CS_CAM_ID_GLOBAL_REVIVE
    /*  -8 */ CAM_SET_DEATH,               // CS_CAM_ID_GLOBAL_DEATH
    /*  -7 */ CAM_SET_WARP_PAD_MOON,       // CS_CAM_ID_GLOBAL_WARP_PAD_MOON
    /*  -6 */ CAM_SET_NAVI,                // CS_CAM_ID_GLOBAL_SONG_WARP
    /*  -5 */ CAM_SET_ITEM3,               // CS_CAM_ID_GLOBAL_ITEM_SHOW
    /*  -4 */ CAM_SET_ITEM2,               // CS_CAM_ID_GLOBAL_ITEM_BOTTLE
    /*  -3 */ CAM_SET_ITEM1,               // CS_CAM_ID_GLOBAL_ITEM_OCARINA
    /*  -2 */ CAM_SET_ITEM0,               // CS_CAM_ID_GLOBAL_ITEM_GET
    /*  -1 */ CAM_SET_STOP0,               // CS_CAM_ID_NONE
    /*   0 */ CAM_SET_NONE,
};

s16* sGlobalCamDataSettingsPtr = &sGlobalCamDataSettings[ARRAY_COUNT(sGlobalCamDataSettings) - 1];


#define CAM_SETTING_MODE_ENTRY(func, data) \
    { func, ARRAY_COUNT(data), data }

#define CAM_SETTING_MODE_NULL \
    { CAM_FUNC_NONE, 0, NULL }


s32 Camera_ChangeActorCsCamIndex(Camera* camera, s32 bgCamIndex) {
    s16 setting;

    if ((bgCamIndex == -1) || (bgCamIndex == camera->bgCamIndex)) {
        camera->behaviorFlags |= CAM_BEHAVIOR_BGCAM_2;
        return -1;
    }

    if (bgCamIndex < 0) {
        setting = sGlobalCamDataSettingsPtr[bgCamIndex];
    } else if (!(camera->behaviorFlags & CAM_BEHAVIOR_BGCAM_2)) {
        setting = Camera_GetBgCamOrActorCsCamSetting(camera, bgCamIndex);
    } else {
        return -1;
    }

    camera->behaviorFlags |= CAM_BEHAVIOR_BGCAM_2;

    // Sets camera setting based on bg/scene data
    //if ((Camera_ChangeSettingFlags(camera, setting, CAM_CHANGE_SETTING_2 | CAM_CHANGE_SETTING_0) >= 0) ||
        //(sCameraSettings[camera->setting].flags & 0x80000000)) {
    if ((Camera_ChangeSettingFlags(camera, setting, CAM_CHANGE_SETTING_2 | CAM_CHANGE_SETTING_0) >= 0) ||
        (sCameraSettings[camera->setting].unk_bit1 & 0x80000000)) {
        camera->bgCamIndex = bgCamIndex;
        camera->behaviorFlags |= CAM_BEHAVIOR_BGCAM_1;
        Camera_ResetActionFuncState(camera, camera->mode);
    }

    return bgCamIndex | 0x80000000;
}

u16 sCueTypeList[10];

/**
 * @param play See `PlayState`
 * @param cueType See `cmdType`
 * @return cue channel
 */
s32 Cutscene_GetCueChannel(PlayState* play, u16 cueType) {
    s32 i;
    s32 cueChannel = -1;

    for (i = 0; i < ARRAY_COUNT(sCueTypeList); i++) {
        if (cueType == sCueTypeList[i]) {
            cueChannel = i;
        }
    }

    return cueChannel;
}

s32 Cutscene_IsCueInChannel(PlayState* play, u16 cueType) {
    if (play->csCtx.state != CS_STATE_IDLE) {
        s32 cueChannel = Cutscene_GetCueChannel(play, cueType);

        if (cueChannel != -1) {
            return play->csCtx.npcActions[cueChannel] != NULL;
            return 0;
        }
    }
    return false;
}

/**
 * The main function to request a sfx. All sfx requests begin here.
 */
/*void AudioSfx_PlaySfx(u16 sfxId, Vec3f* pos, u8 token, f32* freqScale, f32* volume, s8* reverbAdd) {
    u8 i;
    SfxRequest* reqWrite;
    SfxRequest* reqRead;

    if (!gSfxBankMuted[SFX_BANK_SHIFT(sfxId)]) {
        reqWrite = &sSfxRequests[sSfxRequestWriteIndex];

        for (i = sSfxRequestReadIndex; sSfxRequestWriteIndex != i; i++) {
            reqRead = &sSfxRequests[i];
            if ((reqRead->pos == pos) && (reqRead->sfxId == sfxId)) {
                return;
            }
        }

        reqWrite->sfxId = sfxId;
        reqWrite->pos = pos;
        reqWrite->token = token;
        reqWrite->freqScale = freqScale;
        reqWrite->volume = volume;
        reqWrite->reverbAdd = reverbAdd;
        sSfxRequestWriteIndex++;
    }
}

void Audio_PlaySfx_AtPos(Vec3f* pos, u16 sfxId) {
    AudioSfx_PlaySfx(sfxId, pos, 4, &gSfxDefaultFreqAndVolScale, &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
}*/

/*void Cutscene_StopManual(PlayState* play, CutsceneContext* csCtx) {
    if (csCtx->state != CS_STATE_RUN_UNSTOPPABLE) {
        csCtx->state = CS_STATE_STOP;
    }
}*/

f32 sActorMovementScale = 1.0f;

void Actor_SetMovementScale(s32 scale) {
    sActorMovementScale = scale * 0.5f;
}

/**
 * Update actor position using velocity and any push from z_collision_check.
 */
void Actor_UpdatePos(Actor* actorCtx) {
    f32 speedRate = sActorMovementScale;

    actorCtx->world.pos.x += (actorCtx->velocity.x * speedRate) + actorCtx->colChkInfo.displacement.x;
    actorCtx->world.pos.y += (actorCtx->velocity.y * speedRate) + actorCtx->colChkInfo.displacement.y;
    actorCtx->world.pos.z += (actorCtx->velocity.z * speedRate) + actorCtx->colChkInfo.displacement.z;
}

/**
 * Updates actor's velocity accounting for gravity (without exceeding terminal velocity)
 * The operation is performed in cylindrical coordinates
 *
 * It is recommended to not call this function directly and use `Actor_MoveWithGravity` instead
 */
void Actor_UpdateVelocityWithGravity(Actor* actorCtx) {
    actorCtx->velocity.x = actorCtx->speedXZ * Math_SinS(actorCtx->world.rot.y);
    actorCtx->velocity.z = actorCtx->speedXZ * Math_CosS(actorCtx->world.rot.y);

    actorCtx->velocity.y += actorCtx->gravity;
    if (actorCtx->velocity.y < actorCtx->terminalVelocity) {
        actorCtx->velocity.y = actorCtx->terminalVelocity;
    }
}

/**
 * Moves actor accounting for its current velocity and applying gravity
 * The operation is performed in cylindrical coordinates
 */
void Actor_MoveWithGravity(Actor* actorCtx) {
    Actor_UpdateVelocityWithGravity(actorCtx);
    Actor_UpdatePos(actorCtx);
}

typedef enum {
    /* 0x0 */ SEQCMD_OP_PLAY_SEQUENCE,
    /* 0x1 */ SEQCMD_OP_STOP_SEQUENCE,
    /* 0x2 */ SEQCMD_OP_QUEUE_SEQUENCE,
    /* 0x3 */ SEQCMD_OP_UNQUEUE_SEQUENCE,
    /* 0x4 */ SEQCMD_OP_SET_SEQPLAYER_VOLUME,
    /* 0x5 */ SEQCMD_OP_SET_SEQPLAYER_FREQ,
    /* 0x6 */ SEQCMD_OP_SET_CHANNEL_VOLUME,
    /* 0x7 */ SEQCMD_OP_SET_SEQPLAYER_IO,
    /* 0x8 */ SEQCMD_OP_SET_CHANNEL_IO,
    /* 0x9 */ SEQCMD_OP_SET_CHANNEL_IO_DISABLE_MASK,
    /* 0xA */ SEQCMD_OP_SET_CHANNEL_DISABLE_MASK,
    /* 0xB */ SEQCMD_OP_TEMPO_CMD,
    /* 0xC */ SEQCMD_OP_SETUP_CMD,
    /* 0xD */ SEQCMD_OP_SET_CHANNEL_FREQ,
    /* 0xE */ SEQCMD_OP_GLOBAL_CMD,
    /* 0xF */ SEQCMD_OP_RESET_AUDIO_HEAP
} SeqCmdOp;


Gfx D_801AEF88[] = {
    gsDPSetRenderMode(AA_EN | Z_CMP | Z_UPD | IM_RD | CLR_ON_CVG | CVG_DST_WRAP | ZMODE_XLU | FORCE_BL |
                          G_RM_FOG_SHADE_A,
                      AA_EN | Z_CMP | Z_UPD | IM_RD | CLR_ON_CVG | CVG_DST_WRAP | ZMODE_XLU | FORCE_BL |
                          GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)),
    gsDPSetAlphaCompare(G_AC_THRESHOLD),
    gsSPEndDisplayList(),
};

Gfx D_801AEFA0[] = {
    gsSPEndDisplayList(),
};



#include <code/z_actor.h>
Actor* Actor_SpawnAsChildMM(ActorContext* actorCtx, Actor* parent, PlayState* play, s16 actorId, f32 posX, f32 posY, f32 posZ, s16 rotX, s16 rotY, s16 rotZ, s16 params) {
#define Actor_SpawnAsChild Actor_SpawnAsChildMM
	Actor* actor;
	ActorInit* actorInit;
	s32 objBankIndex;
	ActorOverlay* overlayEntry;
	uintptr_t temp;
	char* name;
	u32 overlaySize;
	
	overlayEntry = &gActorOverlayTable[actorId];
	//Assert(actorId < ACTOR_ID_MAX);
	
	name = overlayEntry->name != NULL ? overlayEntry->name : "";
	overlaySize = (u32)overlayEntry->vramEnd - (u32)overlayEntry->vramStart;
	
	if (actorCtx->total > ACTOR_NUMBER_MAX)
		return NULL;
	
	if (overlayEntry->vramStart == NULL) {
		actorInit = overlayEntry->initInfo;
	} else {
		if (overlayEntry->loadedRamAddr != NULL) {
		} else {
			if (overlayEntry->allocType & ACTOROVL_ALLOC_ABSOLUTE) {
				Assert(overlaySize <= ACTOROVL_ABSOLUTE_SPACE_SIZE);
				
				if (actorCtx->absoluteSpace == NULL) {
					actorCtx->absoluteSpace = ZeldaArena_MallocRDebug(ACTOROVL_ABSOLUTE_SPACE_SIZE, "AMF:絶対魔法領域", 0);
				}
				
				overlayEntry->loadedRamAddr = actorCtx->absoluteSpace;
			} else if (overlayEntry->allocType & ACTOROVL_ALLOC_PERSISTENT) {
				overlayEntry->loadedRamAddr = ZeldaArena_MallocRDebug(overlaySize, name, 0);
			} else {
				overlayEntry->loadedRamAddr = ZeldaArena_MallocDebug(overlaySize, name, 0);
			}
			
			if (overlayEntry->loadedRamAddr == NULL) {
				return NULL;
			}
			
			Overlay_Load(overlayEntry->vromStart, overlayEntry->vromEnd, overlayEntry->vramStart, overlayEntry->vramEnd,
				overlayEntry->loadedRamAddr);
			
			overlayEntry->numLoaded = 0;
		}
		
		actorInit = (void*)(u32)((overlayEntry->initInfo != NULL)
									 ? (void*)((u32)overlayEntry->initInfo -
			(s32)((u32)overlayEntry->vramStart - (u32)overlayEntry->loadedRamAddr))
									 : NULL);
	}
	
	objBankIndex = Object_GetIndex(&play->objectCtx, actorInit->objectId);
	
	if ((objBankIndex < 0) || ((actorInit->category == ACTORCAT_ENEMY) && Flags_GetClear(play, play->roomCtx.curRoom.num))) {
		Actor_FreeOverlay(overlayEntry);
		return NULL;
	}
	
	actor = ZeldaArena_MallocDebug(actorInit->instanceSize, name, 1);
	
	if (actor == NULL) {
		Actor_FreeOverlay(overlayEntry);
		return NULL;
	}
	
	Assert((u8)overlayEntry->numLoaded < 255);
	
	overlayEntry->numLoaded++;
	
	Lib_MemSet((u8*)actor, actorInit->instanceSize, 0);
	actor->overlayEntry = overlayEntry;
	actor->id = actorInit->id;
	actor->flags = actorInit->flags;
	
	if (actorInit->id == ACTOR_EN_PART) {
		actor->objBankIndex = rotZ;
		rotZ = 0;
	} else {
		actor->objBankIndex = objBankIndex;
	}
	
	actor->init = actorInit->init;
	actor->destroy = actorInit->destroy;
	actor->update = actorInit->update;
	actor->draw = actorInit->draw;
	actor->room = play->roomCtx.curRoom.num;
	actor->home.pos.x = posX;
	actor->home.pos.y = posY;
	actor->home.pos.z = posZ;
	actor->home.rot.x = rotX;
	actor->home.rot.y = rotY;
	actor->home.rot.z = rotZ;
	actor->params = params;
	parent->child = actor;
	actor->parent = parent;
	
	Actor_AddToCategory(actorCtx, actor, actorInit->category);
	
	temp = gSegments[6];
	Actor_Init(actor, play);
	gSegments[6] = temp;
	
	return actor;
}