#ifndef Z_EN_JSO_H
#define Z_EN_JSO_H

#include "global.h"

struct EnJso;

#define EN_JSO_GET_HINT_TYPE(thisx) ((thisx)->params)

#define EN_JSO_AFTERIMAGE_COUNT 20

typedef void (*EnJsoActionFunc)(struct EnJso*, PlayState*);


extern Gfx D_801AEFA0[];


MtxF* sCurrentMatrix; //!< original name: "Matrix_now"


// For the four `VARIABLE` hint types, the former hint is used if the player hasn't restored the river in
// Ikana Canyon yet, and the latter hint is used if the player has. If the player has the Elegy of Emptiness,
// then all hints will be replaced with a hint telling the player to use the song to climb Stone Tower.
typedef enum {
    /* 0 */ EN_JSO_HINT_FREEZE_OCTOROKS,
    /* 1 */ EN_JSO_HINT_FLATS_LOCATION,
    /* 2 */ EN_JSO_HINT_VARIABLE_1, // Hints that Pamela's family is in the house or that the Song of Healing can heal Pamela's father
    /* 3 */ EN_JSO_HINT_VARIABLE_2, // Hints that Pamela leaves the house to check something or that the well is connected to Ikana Castle
    /* 4 */ EN_JSO_HINT_VARIABLE_3, // Hints that they can restore the river by going to Sharp's cave or that the Gidbos in the well want items
    /* 5 */ EN_JSO_HINT_VARIABLE_4, // Hints that they need to Song of Storms to stop Sharp or that the items the Gibdos want can be found in the well
    /* 6 */ EN_JSO_HINT_DANCING_REDEADS,
    /* 7 */ EN_JSO_HINT_DESTRUCTIBLE_CEILING_IN_CASTLE,
    /* 8 */ EN_JSO_HINT_SECOND_ENTRANCE_TO_CASTLE,
    /* 9 */ EN_JSO_HINT_KING_WEAK_TO_MIRROR_SHIELD
} EnJsoHintType;

typedef enum {
    /*  0 */ EN_JSO_BODYPART_LEFT_SWORD,
    /*  1 */ EN_JSO_BODYPART_RIGHT_SWORD,
    /*  2 */ EN_JSO_BODYPART_ROBE_TOP,
    /*  3 */ EN_JSO_BODYPART_ROBE_BACK,
    /*  4 */ EN_JSO_BODYPART_ROBE_LEFT,
    /*  5 */ EN_JSO_BODYPART_ROBE_RIGHT,
    /*  6 */ EN_JSO_BODYPART_ROBE_FRONT,
    /*  7 */ EN_JSO_BODYPART_HEAD,
    /*  8 */ EN_JSO_BODYPART_RIGHT_THIGH,
    /*  9 */ EN_JSO_BODYPART_RIGHT_FOOT,
    /* 10 */ EN_JSO_BODYPART_LEFT_THIGH,
    /* 11 */ EN_JSO_BODYPART_LEFT_FOOT,
    /* 12 */ EN_JSO_BODYPART_MAX
} EnJsoBodyPart;


typedef enum GaroLimb {
    /* 0x00 */ GARO_LIMB_NONE,
    /* 0x01 */ GARO_LIMB_ROOT,
    /* 0x02 */ GARO_LIMB_TORSO,
    /* 0x03 */ GARO_LIMB_LEFT_ARM,
    /* 0x04 */ GARO_LIMB_LEFT_SWORD,
    /* 0x05 */ GARO_LIMB_RIGHT_ARM,
    /* 0x06 */ GARO_LIMB_RIGHT_SWORD,
    /* 0x07 */ GARO_LIMB_ROBE_TOP,
    /* 0x08 */ GARO_LIMB_ROBE_BACK,
    /* 0x09 */ GARO_LIMB_ROBE_LEFT,
    /* 0x0A */ GARO_LIMB_ROBE_RIGHT,
    /* 0x0B */ GARO_LIMB_ROBE_FRONT,
    /* 0x0C */ GARO_LIMB_HEAD,
    /* 0x0D */ GARO_LIMB_LOWER_BODY_ROOT,
    /* 0x0E */ GARO_LIMB_RIGHT_THIGH,
    /* 0x0F */ GARO_LIMB_RIGHT_SHIN,
    /* 0x10 */ GARO_LIMB_RIGHT_FOOT,
    /* 0x11 */ GARO_LIMB_LEFT_THIGH,
    /* 0x12 */ GARO_LIMB_LEFT_SHIN,
    /* 0x13 */ GARO_LIMB_LEFT_FOOT,
    /* 0x14 */ GARO_LIMB_MAX
} GaroLimb;


typedef struct EnJso {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ Vec3s jointTable[GARO_LIMB_MAX];
    /* 0x200 */ Vec3s morphTable[GARO_LIMB_MAX];
    /* 0x278 */ EnJsoActionFunc actionFunc;
    /* 0x27C */ s16 action;
    /* 0x27E */ s16 circlingAngle;
    /* 0x280 */ s16 circlingAngularVelocity;
    /* 0x282 */ s16 dashAttackTimer;
    /* 0x284 */ UNK_TYPE1 unk_284[0x2];
    /* 0x286 */ s16 attackWaitTimer; // while bouncing, the Garo cannot start an attack until this reaches 0
    /* 0x288 */ s16 timer;
    /* 0x28A */ s16 isGuarding; // Set to false when the guard animation ends, never checked or set otherwise. Name is inferred based on how it's being set.
    /* 0x28C */ s16 isPlayerLockedOn;
    /* 0x28E */ s16 isAttacking;
    /* 0x290 */ Vec3s robeRightRot;
    /* 0x296 */ Vec3s targetRobeRightRot;
    /* 0x29C */ Vec3s rightArmRot;
    /* 0x2A2 */ Vec3s targetRightArmRot;
    /* 0x2A8 */ s16 drawDmgEffAlpha;
    /* 0x2AA */ s16 drawDmgEffType;
    /* 0x2AC */ f32 drawDmgEffScale;
    /* 0x2B0 */ f32 drawDmgEffFrozenSteamScale;
    /* 0x2B4 */ Vec3f bodyPartsPos[EN_JSO_BODYPART_MAX];
    /* 0x344 */ s16 bodyPartIndex;
    /* 0x346 */ s16 disableAnimations; // Checked but never set, so it's implicitly always false. Name is inferred based on how it's being checked.
    /* 0x348 */ s16 textIndex;
    /* 0x34A */ s16 hintType;
    /* 0x34C */ u8 disableBlure;
    /* 0x34D */ u8 swordState;
    /* 0x34E */ u8 swordHitSomething;
    /* 0x350 */ f32 animEndFrame;
    /* 0x354 */ f32 scale;
    /* 0x358 */ UNK_TYPE1 unk_358[0x4];
    /* 0x35C */ ColliderCylinder bodyCollider;
    /* 0x3A8 */ ColliderQuad rightSwordCollider;
    /* 0x428 */ ColliderQuad leftSwordCollider;
    /* 0x4A8 */ s32 rightSwordBlureIndex;
    /* 0x4AC */ s32 leftSwordBlureIndex;
    /* 0x4B0 */ s16 afterimageIndex;
    /* 0x4B4 */ s32 afterimageCount;
    /* 0x4B8 */ s16 csId;
    /* 0x4BC */ u32 introCsTimer;
    /* 0x4C0 */ s16 introCsState;
    /* 0x4C2 */ s16 subCamId;
    /* 0x4C4 */ UNK_TYPE1 unk_4C4[0x4];
    /* 0x4C8 */ Vec3f subCamEye;
    /* 0x4D4 */ Vec3f subCamAt;
    /* 0x4E0 */ Vec3f subCamUp;  
    /* 0x4EC */ Vec3f subCamEyeNext;
    /* 0x4F8 */ Vec3f subCamAtNext;
    /* 0x504 */ UNK_TYPE1 unk_504[0x10];
    /* 0x514 */ s16 introCsType;
    /* 0x518 */ Vec3f afterimagePos[EN_JSO_AFTERIMAGE_COUNT];
    /* 0x608 */ Vec3s afterimageRot[EN_JSO_AFTERIMAGE_COUNT];
    /* 0x680 */ Vec3s afterimageJointTable[EN_JSO_AFTERIMAGE_COUNT][GARO_LIMB_MAX];
    /* 0xFE0 */ Vec3f knockbackVelocity; // Adds a little push backwards when the Garo is bounced off the player's shield, damaged, or stunned
    /*       */ BodyBreak bodyBreak;
} EnJso; // size = 0xFEC





typedef enum PlayerCsAction {
    /* 0x00 */ PLAYER_CSACTION_NONE,
    /* 0x01 */ PLAYER_CSACTION_1,
    /* 0x02 */ PLAYER_CSACTION_2,
    /* 0x03 */ PLAYER_CSACTION_3,
    /* 0x04 */ PLAYER_CSACTION_4,
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
    /* 0x15 */ PLAYER_CSACTION_21,
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












struct EnEncount3;

typedef void (*EnEncount3ActionFunc)(struct EnEncount3*, PlayState*);

#define ENCOUNT3_GET_SPAWN_INDEX(thisx) (((thisx)->params >> 0xC) & 0xF)
#define ENCOUNT3_GET_PARAM_F80(thisx) (((thisx)->params >> 7) & 0x1F)
#define ENCOUNT3_GET_SWITCH_FLAG(thisx) ((thisx)->params & 0x7F)

#define ENCOUNT3_GET_ROTZ(thisx) ((thisx)->world.rot.z)

#define ENCOUNT3_SWITCH_FLAG_NONE 0x7F

typedef struct EnEncount3 {
    /* 0x000 */ Actor actor;
    /* 0x144 */ EnEncount3ActionFunc actionFunc;
    /* 0x148 */ u8 unk148;
    /* 0x14A */ s16 unk14A;
    /* 0x14C */ UNK_TYPE1 pad14C[0x2];
    /* 0x14E */ s16 unk14E;
    /* 0x150 */ s16 unk150;
    /* 0x152 */ s16 unk152;
    /* 0x154 */ s16 childActorId;
    /* 0x156 */ s16 unk156;
    /* 0x158 */ s16 childParams;
    /* 0x15A */ s16 csId;
    /* 0x15C */ s16 switchFlag;
    /* 0x15E */ s16 timer;
    /* 0x160 */ f32 unk160;
    /* 0x164 */ f32 unk164;
    /* 0x168 */ f32 unk168;
    /* 0x16C */ f32 unk16C;
    /* 0x170 */ f32 unk170;
    /* 0x174 */ f32 unk174;
    /* 0x178 */ f32 unk178;
    /* 0x17C */ Actor* child;
    /* 0x180 */ UNK_TYPE1 unk180[0x4C];
} EnEncount3; // size = 0x1CC

#endif // Z_EN_JSO_H
