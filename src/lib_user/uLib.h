#ifndef __ULIB_H__
#define __ULIB_H__

// # # # # # # # # # # # # # # # # # # # #
// # User Library Patches                #
// # # # # # # # # # # # # # # # # # # # #

// New save patches
#define Patch_SaveStartEntrance 0x0398 //The first spawner of Bar scene
#define Patch_SaveStartSceneID (0x37) //Bar
#define Patch_TitleScreenID (0x37) //Bar
#define Patch_TitleScreeneEntrance (0x37) //Bar
#define Patch_SaveStartAge      LINK_AGE_ADULT
#define Patch_SaveStartTime     0x6AAB
#define Patch_SaveStartCsIndex  0xFFF1
#define KNIFLE_HEALTH 50
// Wield Hylian shield like Kokiri shield
#define Patch_WieldHylianShieldLikeKokiriShield true

// Play cutscene after obtaining Silver Gauntlets
#define Patch_SilverGauntletObtainCS false

// Flush current textbox by pressing B
#define Patch_QuickText true

// Space allocated for GI models, expand if needed
#define Pathch_GetItem_SegmentSize 0x3508 //0x3008

// Extension, these can be adjusted if necessary
#define EXT_DMA_MAX    3800
#define EXT_ACTOR_MAX  1000
#define EXT_OBJECT_MAX 1000
#define EXT_SCENE_MAX  256
#define EXT_EFFECT_MAX 64

//State documentation
#define PLAYER_STATE1_EXITING_SCENE (1 << 0)
#define PLAYER_STATE1_SWINGING_BOTTLE (1 << 1)
#define PLAYER_STATE1_END_HOOKSHOT_MOVE (1 << 2)
#define PLAYER_STATE1_AIMING_FPS_ITEM (1 << 3)
#define PLAYER_STATE1_Z_TARGETING_UNFRIENDLY (1 << 4)
#define PLAYER_STATE1_INPUT_DISABLED (1 << 5)
#define PLAYER_STATE1_TALKING (1 << 6)
#define PLAYER_STATE1_IN_DEATH_CUTSCENE (1 << 7)
#define PLAYER_STATE1_START_CHANGE_ITEM (1 << 8)
#define PLAYER_STATE1_READY_TO_SHOOT (1 << 9)
#define PLAYER_STATE1_GETTING_ITEM (1 << 10)
#define PLAYER_STATE1_HOLDING_ACTOR (1 << 11)
#define PLAYER_STATE1_CHARGING_SPIN_ATTACK (1 << 12)
#define PLAYER_STATE1_HANGING_FROM_LEDGE_SLIP (1 << 13)
#define PLAYER_STATE1_CLIMBING_ONTO_LEDGE (1 << 14)
#define PLAYER_STATE1_UNUSED_Z_TARGETING_FLAG (1 << 15)
#define PLAYER_STATE1_FORCE_STRAFING (1 << 16)
#define PLAYER_STATE1_Z_TARGETING_FRIENDLY (1 << 17)
#define PLAYER_STATE1_JUMPING (1 << 18)
#define PLAYER_STATE1_FREEFALLING (1 << 19)
#define PLAYER_STATE1_IN_FIRST_PERSON_MODE (1 << 20)
#define PLAYER_STATE1_CLIMBING (1 << 21)
#define PLAYER_STATE1_CROUCHING (1 << 22)
#define PLAYER_STATE1_RIDING_HORSE (1 << 23)
#define PLAYER_STATE1_AIMING_BOOMERANG (1 << 24)
#define PLAYER_STATE1_AWAITING_THROWN_BOOMERANG (1 << 25)
#define PLAYER_STATE1_TAKING_DAMAGE (1 << 26)
#define PLAYER_STATE1_SWIMMING (1 << 27)
#define PLAYER_STATE1_SKIP_OTHER_ACTORS_UPDATE (1 << 28)
#define PLAYER_STATE1_IN_CUTSCENE (1 << 29)
#define PLAYER_STATE1_30 (1 << 30)
#define PLAYER_STATE1_FALLING_INTO_GROTTO_OR_VOID (1 << 31)

#define PLAYER_STATE2_CAN_GRAB_PUSH_PULL_WALL (1 << 0)
#define PLAYER_STATE2_CAN_SPEAK_OR_CHECK (1 << 1)
#define PLAYER_STATE2_CAN_CLIMB_PUSH_PULL_WALL (1 << 2)
#define PLAYER_STATE2_MAKING_REACTABLE_NOISE (1 << 3)
#define PLAYER_STATE2_MOVING_PUSH_PULL_WALL (1 << 4)
#define PLAYER_STATE2_DISABLE_MOVE_ROTATION_WHILE_Z_TARGETING (1 << 5)
#define PLAYER_STATE2_ALWAYS_DISABLE_MOVE_ROTATION (1 << 6)
#define PLAYER_STATE2_RESTRAINED_BY_ENEMY (1 << 7)
#define PLAYER_STATE2_ENABLE_PUSH_PULL_CAM (1 << 8)
#define PLAYER_STATE2_SPAWN_DUST_AT_FEET (1 << 9)
#define PLAYER_STATE2_DIVING (1 << 10)
#define PLAYER_STATE2_ENABLE_DIVE_CAMERA_AND_TIMER (1 << 11)
#define PLAYER_STATE2_IDLE_WHILE_CLIMBING (1 << 12)
#define PLAYER_STATE2_USING_SWITCH_Z_TARGETING (1 << 13)
#define PLAYER_STATE2_FROZEN_IN_ICE (1 << 14)
#define PLAYER_STATE2_PAUSE_MOST_UPDATING (1 << 15)
#define PLAYER_STATE2_DO_ACTION_ENTER (1 << 16)
#define PLAYER_STATE2_RELEASING_SPIN_ATTACK (1 << 17)
#define PLAYER_STATE2_CRAWLING (1 << 18)
#define PLAYER_STATE2_BACKFLIPPING_OR_SIDEHOPPING (1 << 19)
#define PLAYER_STATE2_NAVI_IS_ACTIVE (1 << 20)
#define PLAYER_STATE2_NAVI_REQUESTING_TALK (1 << 21)
#define PLAYER_STATE2_CAN_DISMOUNT_HORSE (1 << 22)
#define PLAYER_STATE2_NEAR_OCARINA_ACTOR (1 << 23)
#define PLAYER_STATE2_ATTEMPT_PLAY_OCARINA_FOR_ACTOR (1 << 24)
#define PLAYER_STATE2_PLAYING_OCARINA_FOR_ACTOR (1 << 25)
#define PLAYER_STATE2_ENABLE_REFLECTION (1 << 26)
#define PLAYER_STATE2_PLAYING_OCARINA_GENERAL (1 << 27)
#define PLAYER_STATE2_IDLING (1 << 28)
#define PLAYER_STATE2_DISABLE_DRAW (1 << 29)
#define PLAYER_STATE2_ENABLE_FORWARD_SLIDE_FROM_ATTACK (1 << 30)
#define PLAYER_STATE2_FORCE_VOID_OUT (1 << 31)

#define PLAYER_STATE3_IGNORE_CEILING_FLOOR_AND_WATER (1 << 0)
#define PLAYER_STATE3_MIDAIR (1 << 1)
#define PLAYER_STATE3_PAUSE_ACTION_FUNC (1 << 2)
#define PLAYER_STATE3_ENDING_MELEE_ATTACK (1 << 3)
#define PLAYER_STATE3_CHECKING_FLOOR_AND_WATER_COLLISION (1 << 4)
#define PLAYER_STATE3_FORCE_PULL_OCARINA (1 << 5)
#define PLAYER_STATE3_RESTORE_NAYRUS_LOVE (1 << 6) // Set by ocarina effects actors when destroyed to signal Nayru's Love may be restored (see `ACTOROVL_ALLOC_ABSOLUTE`)
#define PLAYER_STATE3_MOVING_ALONG_HOOKSHOT_PATH (1 << 7)

#define ACTOR_FLAG_TARGETABLE (1 << 0) // Navi will fly over the actor and it can be Z targeted
#define ACTOR_FLAG_UNFRIENDLY (1 << 2) // changes targeting behavior for unfriendly actors (sound, Link's stance)
#define ACTOR_FLAG_FRIENDLY (1 << 3) // opposite of the unfriendly flag. flag is not checked against in the original game
#define ACTOR_FLAG_NO_UPDATE_CULLING (1 << 4) // actor will keep updating even if outside of the uncull zone i.e. "update-culling" does not occur
#define ACTOR_FLAG_NO_DRAW_CULLING (1 << 5) // actor will keep drawing even if outside of the uncull zone i.e. "draw-culling" does not occur
#define ACTOR_FLAG_IN_UNCULL_ZONE (1 << 6) // actor is currently in the uncull zone
#define ACTOR_FLAG_REACT_TO_LENS (1 << 7) // hidden or revealed by Lens of Truth (depending on room flags)
#define ACTOR_FLAG_TALK_REQUESTED (1 << 8) // Player has requested to talk to the actor; Player uses this flag differently than every other actor
#define ACTOR_FLAG_HOOK_CAN_CARRY (1 << 9) // brings the actor back to Player if hookshoted
#define ACTOR_FLAG_HOOK_BRING_PLAYER (1 << 10) // brings Player to the actor if hookshoted
#define ACTOR_FLAG_ENKUSA_CUT (1 << 11) // grass actor has been cut
#define ACTOR_FLAG_IGNORE_QUAKE (1 << 12) // actor will not shake when a quake occurs
#define ACTOR_FLAG_HOOK_ATTACHED (1 << 13) // hookshot has attached to the actor (either a collider or surface)
#define ACTOR_FLAG_ARROW_CAN_CARRY (1 << 14) // when an arrow hits the actor it will attach to the actor and carry it
#define ACTOR_FLAG_ARROW_IS_CARRYING (1 << 15) // an arrow is currently carrying this actor
#define ACTOR_FLAG_IMMEDIATE_TALK (1 << 16) // forces Player to talk when in range. needs to be unset manually to avoid infinite talking
#define ACTOR_FLAG_HEAVYBLOCK (1 << 17) // changes actor carrying behavior specifically for the golden gauntlets block actor
#define ACTOR_FLAG_CHECK_WITH_NAVI (1 << 18) // Navi can be used to trigger dialogue when targeting the actor
#define ACTOR_FLAG_SFX_AT_POS (1 << 19) // play sound from sfx field at the actor's position
#define ACTOR_FLAG_SFX_CENTERED2 (1 << 20) // same as ACTOR_FLAG_SFX_CENTERED, unused in the original game
#define ACTOR_FLAG_SFX_CENTERED (1 << 21) // play sound from sfx field at the center of the screen
#define ACTOR_FLAG_IGNORE_POINT_LIGHTS (1 << 22) // ignores point lights but not directional lights (such as environment lights)
#define ACTOR_FLAG_ALWAYS_THROW (1 << 23) // Player throws held actor even if standing still
#define ACTOR_FLAG_PLAY_BODYHIT_SFX (1 << 24) // when actor hits Player's body, a thump sfx plays
#define ACTOR_FLAG_OCARINA_NO_FREEZE (1 << 25) // actor doesnt freeze when Player has ocarina out or is using a warp song
#define ACTOR_FLAG_CAN_HOLD_SWITCH (1 << 26) // actor can press and hold down switches
#define ACTOR_FLAG_CANT_LOCK_ON (1 << 27) // prevents locking on with Z targeting an actor even if Navi is floating over it
#define ACTOR_FLAG_SFX_TIMER (1 << 28) // actor sfx field is used as timer state instead of an sfx id

#define ACTORFLAG_CAN_TARGET    (1)
#define ACTORFLAG_ENEMY         (1 << 2)
#define ACTORFLAG_FRIENDLY      (1 << 3)
#define ACTORFLAG_ALWAYS_UPDATE (1 << 4)

#define ACTORFLAG_ALWAYS_DRAW  (1 << 5)
#define ACTORFLAG_UNCULLED     (1 << 6)
#define ACTORFLAG_INVISIBLE    (1 << 7)
#define ACTORFLAG_TALK_REQUEST (1 << 8)

#define ACTORFLAG_HOOKSHOT_PULL   (1 << 9)
#define ACTORFLAG_HOOKSHOT_LAUNCH (1 << 10)
#define ACTORFLAG_ENKUSA_CUT      (1 << 11)
#define ACTORFLAG_NO_QUAKE        (1 << 12)

#define ACTORFLAG_HOOKSHOT_ATTACHED (1 << 13)
#define ACTORFLAG_ARROW_CAN_CARRY   (1 << 14)
#define ACTORFLAG_ARROW_ATTACHED    (1 << 15)
#define ACTORFLAG_FORCE_TALK        (1 << 16)

#define ACTORFLAG_HEAVY_OBJECT (1 << 17)
#define ACTORFLAG_CHECKSPOT    (1 << 18)
#define ACTORFLAG_SFX_AT_POS   (1 << 19)
#define ACTORFLAG_SFX_CENTER_2 (1 << 20)

#define ACTORFLAG_SFX_CENTER    (1 << 21)
#define ACTORFLAG_NO_POINTLIGHT (1 << 22)
#define ACTORFLAG_ALWAYS_THROW  (1 << 23)
#define ACTORFLAG_BODY_HIT_SFX  (1 << 24)

#define ACTORFLAG_OCARINA_UPDATE (1 << 25)
#define ACTORFLAG_PRESS_SWITCH   (1 << 26)
#define ACTORFLAG_NO_TARGET_LOCK (1 << 27)
#define ACTORFLAG_SFX_TIMER      (1 << 28)


#define ACTOR_BGCHECK_WALL                (1 << 0) // check wall
#define ACTOR_BGCHECK_CEIL                (1 << 1) // check ceiling
#define ACTOR_BGCHECK_FLOOR               (1 << 2) // check floor and water
#define ACTOR_BGCHECK_FLOOR_ADHESION      (1 << 3)
#define ACTOR_BGCHECK_FLOOR_ADHESION_RSET (1 << 4)
#define ACTOR_BGCHECK_NO_RIPPLE           (1 << 6) // disable water ripples
#define ACTOR_BGCHECK_WALL_NO_LINE_CHECK  (1 << 7) // alternate wall check?

#define new(type)    ZeldaArena_MallocDebug(sizeof(type), __FILE__, __LINE__)
#define delete(type) ZeldaArena_FreeDebug(type, __FILE__, __LINE__)

#define NEW_MATRIX() Matrix_NewMtx(__gfxCtx, __FILE__, __LINE__)

// # # # # # # # # # # # # # # # # # # # #
// # UserLibrary                         #
// # # # # # # # # # # # # # # # # # # # #

#include <z64hdr.h>

#include "uLib_macros.h"
#include "uLib_types.h"
#include "sfx_enum.h"

extern DmaEntry __ext_gDmaDataTable[EXT_DMA_MAX];
extern ActorOverlay __ext_gActorOverlayTable[EXT_ACTOR_MAX];
extern RomFile __ext_gObjectTable[EXT_OBJECT_MAX];
extern SceneTableEntry __ext_gSceneTable[EXT_SCENE_MAX];
extern EffectSsOverlay __ext_gEffectSsOverlayTable[EXT_EFFECT_MAX];
extern u8 gFontOrdering[];
extern Vec3f gZeroVec;

#ifndef NO_Z64ROM_EXT_TABLES
#define gDmaDataTable         __ext_gDmaDataTable
#define gActorOverlayTable    __ext_gActorOverlayTable
#define gObjectTable          __ext_gObjectTable
#define gSceneTable           __ext_gSceneTable
#define gEffectSsOverlayTable __ext_gEffectSsOverlayTable
#endif

extern u32 osMemSize;
extern PlayState gPlayState;
extern LibContext gLibCtx;
extern GraphicsContext* __gfxCtx;
extern ExitParam gExitParam;
extern f32 EPSILON;
asm ("gPlayState = 0x80212020 - 0x38000;");
asm ("__gfxCtx = gPlayState;");
asm ("gExitParam = gPlayState + 0x11E18;"); // 801EBE38
asm ("osMemSize = 0x80000318");

void uLib_Update(GameState* gameState);
void* memset(void* m, int v, unsigned int s);
void* memmove(void* dest, const void* src, size_t len);
f32 fmodf(f32, f32);

#define DEFAULT_REVERB 0.35f
#define DEFAULT_RANGE  800.0f

void SoundEffect_PlayOneshot(SoundFile id, f32 vol, f32 pitch, Vec3f* pos, f32 reverb, f32 range, void* origin);
void SoundEffect_PlayHeld(SoundFile id, f32 vol, f32 pitch, f32 stepRelease, Vec3f* pos, f32 reverb, f32 range, void* origin);
void SoundEffect_PlayDefaultOneshot(SoundFile id, Vec3f* pos, void* origin);
void SoundEffect_PlayDefaultHeld(SoundFile id, f32 stepRelease, Vec3f* pos, void* origin);
void SoundEffect_StopOneshot(SoundFile id, Vec3f* pos, void* origin);
typedef struct SoundEntity SoundEntity;
SoundEntity* SoundEntity_Register(void* origin);
void SoundEntity_Unregister(SoundEntity* sen);
void SoundEntity_SetFilter(SoundEntity* sen, s8 filter);

#ifdef DEV_BUILD

#define Assert(cond)  if (!(cond)) { Fault_AddHungupAndCrashImpl(#cond, #cond); }
#define osInfo(title) title

void Debug_Text(u8 r, u8 g, u8 b, s32 x, s32 y, char* fmt, ...);
void Debug_DmaLog(DmaRequest* req);
void DebugMenu_Update(PlayState* playState);
s32 DebugMenu_CineCamera(Camera* camera, Normal1* norm1, Player* player);
void Profiler_Start(DebugProfiler* profiler);
void Profiler_End(DebugProfiler* profiler);
void osLibPrintf(const char* fmt, ...);
void osLibHex(const char* txt, const void* data, u32 size, u32 dispOffset);

#else /* RELEASE_BUILD */

#define Debug_Text(...)           do {} while (0)
#define Debug_DmaLog(...)         do {} while (0)
#define DebugMenu_Update(...)     do {} while (0)
#define DebugMenu_CineCamera(...) do {} while (0)
#define Profiler_Start(...)       do {} while (0)
#define Profiler_End(...)         do {} while (0)
#define Assert(cond)              if (cond) (void)0;
#define osInfo(title)             do {} while (0)
#define osLibPrintf(...)          do {} while (0)
#define osLibHex(...)             do {} while (0)
#endif

void Audio_PlaySys(u16 flag);
Color_HSL Color_RgbToHsl(f32 r, f32 g, f32 b);
Color_RGB8 Color_HslToRgb(f32 h, f32 s, f32 l);
void Cutscene_PlaySegment(PlayState* play, void* segment);
Vtx* Gfx_AllocQuad(PlayState* playState, s16 x, s16 y, s16 width, s16 height, s16 u, s16 v);
s32 Lights_SortLightList(PlayState* play, LightInfo* sortedLightList[7]);
void Lights_SetPointlight(PlayState* play, Lights* lights, LightParams* params, bool isWiiVC);
void Lights_RebindActor(PlayState* play, Actor* actor, Vec3f* bindPos);
void Lights_RebindPointlightsActor(PlayState* play, Actor* actor, bool isWiiVC);

f32 MaxF(f32 a, f32 b);
f32 MinF(f32 a, f32 b);
s32 MaxS(s32 a, s32 b);
s32 MinS(s32 a, s32 b);
s32 WrapS(s32 x, s32 min, s32 max);
f32 WrapF(f32 x, f32 min, f32 max);
f32 Math_Spline1(f32 k, f32 xm1, f32 x0, f32 x1, f32 x2);
f32 Math_Spline2(f32 k, f32 xm1, f32 x0, f32 x1, f32 x2);
Vec3f Math_Vec3f_Spline1(f32 k, Vec3f xm1, Vec3f x0, Vec3f x1, Vec3f x2);
Vec3f Math_Vec3f_Spline2(f32 k, Vec3f xm1, Vec3f x0, Vec3f x1, Vec3f x2);
Vec3f Math_Vec3f_YawDist(f32 dist, s16 yaw);
Vec3f Math_Vec3f_YawPitchDist(f32 dist, s16 yaw, s16 pitch);
Vec3f Math_Vec3f_PosRelativeTo(Vec3f* target, Vec3f* origin, s16 originYaw);
f32 Math_Vec3f_Length(Vec3f* a);
f32 PowF(f32 a, f32 b);

void Matrix_RotateX_s(s16 binang, MatrixMode mtxMode);
void Matrix_RotateY_s(s16 binang, MatrixMode mtxMode);
void Matrix_RotateZ_s(s16 binang, MatrixMode mtxMode);
void Matrix_RotateX_f(f32 deg, MatrixMode mtxMode);
void Matrix_RotateY_f(f32 deg, MatrixMode mtxMode);
void Matrix_RotateZ_f(f32 deg, MatrixMode mtxMode);
void Matrix_MultX(f32 x, Vec3f* dst);
void Matrix_MultY(f32 y, Vec3f* dst);
void Matrix_MultZ(f32 z, Vec3f* dst);

s32 Actor_FocusPlayer(PlayState* play, Actor* this, Vec3s* headVector, f32 dist);

Particle Particle_New(Vec3f pos, f32 mass);
void Particle_Update(Particle* particle, f32 gravity, Vec3f addForce, f32 c);
Chain Chain_New(Particle* p1, Particle* p2, f32 length);
void Chain_UpdateStep(Chain* chain, f32 step, f32 max);
void Chain_UpdateAverage(Chain* chain);
void Chain_Update(Chain* chain);

void* Segment_Scene_GetHeader(void* segment, s32 setupIndex);
void* Segment_Scene_GetCutscene(void* segment, s32 setupIndex);
CollisionHeader* Segment_Scene_GetCollision(void* segment, s32 setupIndex);
void Segment_Scene_PlayCutscene(void* segment, s32 setupIndex);
u8 PathList_GetNum(u8 index, s8 header);
void* PathList_GetList(u8 index, s8 header);

void SceneAnim_Update(PlayState* playState);

void SkelAnime_InterpFrameTable_Quat(s32 limbCount, Vec3s* dst, Vec3s* start, Vec3s* target, f32 weight);

void Play_SetFadeOut(PlayState* play);
struct Time Play_GetTime(void);
void NewRoom_Draw(PlayState* play, Room* room, u32 flags);

typedef enum {
    OVL_MSG_TALK,
    OVL_MSG_CHECK,
} OvlMsgType;

typedef enum {
    MSGBOX_TYPE_BLACK,
    MSGBOX_TYPE_WOODEN,
    MSGBOX_TYPE_BLUE,
    MSGBOX_TYPE_OCARINA,
    MSGBOX_TYPE_NONE_BOTTOM,
    MSGBOX_TYPE_NONE_NO_SHADOW,
    MSGBOX_TYPE_CREDITS = 11
} MsgBoxType;

typedef enum {
    MSGBOX_POS_VARIABLE,
    MSGBOX_POS_TOP,
    MSGBOX_POS_MIDDLE,
    MSGBOX_POS_BOTTOM
} MsgBoxPos;

#define MSG_COLOR_DEFAULT   "\x05\x00"
#define MSG_COLOR_RED       "\x05\x01"
#define MSG_COLOR_GREEN     "\x05\x02"
#define MSG_COLOR_BLUE      "\x05\x03"
#define MSG_COLOR_LIGHTBLUE "\x05\x04"
#define MSG_COLOR_PURPLE    "\x05\x05"
#define MSG_COLOR_YELLOW    "\x05\x06"
#define MSG_COLOR_BLACK     "\x05\x07"
#define MSG_COLOR_REG_0     "\x05\x08"
#define MSG_COLOR_REG_1     "\x05\x09"
#define MSG_COLOR_REG_2     "\x05\x0A"
#define MSG_COLOR_REG_3     "\x05\x0B"
#define MSG_COLOR_REG_4     "\x05\x0C"
#define MSG_COLOR_REG_5     "\x05\x0D"
#define MSG_COLOR_REG_6     "\x05\x0E"
#define MSG_COLOR_REG_7     "\x05\x0F"

#define MSG_SHIFT(x)             "\x06" x // 1
#define MSG_BOX_BREAK_DELAYED(x) "\x0C" x // 1
#define MSG_FADE(x)              "\x0E" x // 1
#define MSG_FADE2(x, y)          "\x11" x y // 2
#define MSG_SFX(x, y)            "\x12" x y // 2
#define MSG_ITEM_ICON(x)         "\x13" x // 1
#define MSG_TEXT_SPEED(x)        "\x14" x // 1
#define MSG_HIGHSCORE(x)         "\x1E" x // 1

#define MSG_INSTANT_ON    "\x08"
#define MSG_INSTANT_OFF   "\x09"
#define MSG_PERSISTENT    "\xFA"
#define MSG_EVENT         "\x0B"
#define MSG_NAME          "\x0F"
#define MSG_OCARINA       "\x10"
#define MSG_MARATHON_TIME "\x16"
#define MSG_RACE_TIME     "\x17"
#define MSG_POINTS        "\x18"
#define MSG_TOKENS        "\x19"
#define MSG_TWO_CHOICE    "\x02"
#define MSG_THREE_CHOICE  "\x03"
#define MSG_FISH_INFO     "\x1D"
#define MSG_TIME          "\x1F"

typedef struct OvlMessage {
    MsgBoxType type;
    MsgBoxPos  pos;
    char*      txt;
} OvlMessage;

void OvlMessage_Init(Actor* actor, OvlMsgType type);
void OvlMessage_RegisterColor(u8 id, u8 r, u8 g, u8 b);
s8 OvlMessage_Prompt(Actor* actor, OvlMessage* msg, f32 radius, u32 exchangeItemId);
void OvlMessage_Start(Actor* actor, OvlMessage* msg);
void OvlMessage_Continue(Actor* actor, OvlMessage* msg);
s8 OvlMessage_SwapFocus(Actor* actor);
s8 OvlMessage_IsClosed(Actor* actor);
s8 OvlMessage_IsCurrentMessage(OvlMessage* msg);
s8 OvlMessage_SetBranch(OvlMessage* branch);
s8 OvlMessage_GetChoice(Actor* actor);
s8 OvlMessage_GetBoxNum(void);

#endif