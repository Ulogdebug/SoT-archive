#ifndef Z_EN_DINOFOS_H
#define Z_EN_DINOFOS_H

#include "global.h"

struct EnDinofos;

typedef void (*EnDinofosActionFunc)(struct EnDinofos*, PlayState*);


typedef void* TexturePtr;



typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ Vec3f velocity;
    /* 0x18 */ Vec3f accel;
    /* 0x24 */ s16 scale;
    /* 0x26 */ s16 scaleStep;
    /* 0x28 */ s16 alpha;
    /* 0x2A */ s16 fadeDelay;
    /* 0x2C */ s32 life;
} EffectSsDFireInitParams; // size = 0x30


typedef enum PlayerCsMode {
    /* 0x00 */ PLAYER_CSMODE_0,
    /* 0x01 */ PLAYER_CSMODE_1,
    /* 0x02 */ PLAYER_CSMODE_2,
    /* 0x03 */ PLAYER_CSMODE_3,
    /* 0x04 */ PLAYER_CSMODE_4,
    /* 0x05 */ PLAYER_CSMODE_5,
    /* 0x06 */ PLAYER_CSMODE_6,
    /* 0x07 */ PLAYER_CSMODE_7,
    /* 0x08 */ PLAYER_CSMODE_8,
    /* 0x09 */ PLAYER_CSMODE_9,
    /* 0x0A */ PLAYER_CSMODE_10,
    /* 0x0B */ PLAYER_CSMODE_11,
    /* 0x0C */ PLAYER_CSMODE_12,
    /* 0x0D */ PLAYER_CSMODE_13,
    /* 0x0E */ PLAYER_CSMODE_14,
    /* 0x0F */ PLAYER_CSMODE_15,
    /* 0x10 */ PLAYER_CSMODE_16,
    /* 0x11 */ PLAYER_CSMODE_17,
    /* 0x12 */ PLAYER_CSMODE_18,
    /* 0x13 */ PLAYER_CSMODE_19,
    /* 0x14 */ PLAYER_CSMODE_20,
    /* 0x15 */ PLAYER_CSMODE_21,
    /* 0x16 */ PLAYER_CSMODE_22,
    /* 0x17 */ PLAYER_CSMODE_23,
    /* 0x18 */ PLAYER_CSMODE_24,
    /* 0x19 */ PLAYER_CSMODE_25,
    /* 0x1A */ PLAYER_CSMODE_26,
    /* 0x1B */ PLAYER_CSMODE_27,
    /* 0x1C */ PLAYER_CSMODE_28,
    /* 0x1D */ PLAYER_CSMODE_29,
    /* 0x1E */ PLAYER_CSMODE_30,
    /* 0x1F */ PLAYER_CSMODE_31,
    /* 0x20 */ PLAYER_CSMODE_32,
    /* 0x21 */ PLAYER_CSMODE_33,
    /* 0x22 */ PLAYER_CSMODE_34,
    /* 0x23 */ PLAYER_CSMODE_35,
    /* 0x24 */ PLAYER_CSMODE_36,
    /* 0x25 */ PLAYER_CSMODE_37,
    /* 0x26 */ PLAYER_CSMODE_38,
    /* 0x27 */ PLAYER_CSMODE_39,
    /* 0x28 */ PLAYER_CSMODE_40,
    /* 0x29 */ PLAYER_CSMODE_41,
    /* 0x2A */ PLAYER_CSMODE_42,
    /* 0x2B */ PLAYER_CSMODE_43,
    /* 0x2C */ PLAYER_CSMODE_44,
    /* 0x2D */ PLAYER_CSMODE_45,
    /* 0x2E */ PLAYER_CSMODE_46,
    /* 0x2F */ PLAYER_CSMODE_47,
    /* 0x30 */ PLAYER_CSMODE_48,
    /* 0x31 */ PLAYER_CSMODE_49,
    /* 0x32 */ PLAYER_CSMODE_50,
    /* 0x33 */ PLAYER_CSMODE_51,
    /* 0x34 */ PLAYER_CSMODE_52,
    /* 0x35 */ PLAYER_CSMODE_53,
    /* 0x36 */ PLAYER_CSMODE_54,
    /* 0x37 */ PLAYER_CSMODE_55,
    /* 0x38 */ PLAYER_CSMODE_56,
    /* 0x39 */ PLAYER_CSMODE_57,
    /* 0x3A */ PLAYER_CSMODE_58,
    /* 0x3B */ PLAYER_CSMODE_59,
    /* 0x3C */ PLAYER_CSMODE_60,
    /* 0x3D */ PLAYER_CSMODE_61,
    /* 0x3E */ PLAYER_CSMODE_62,
    /* 0x3F */ PLAYER_CSMODE_63,
    /* 0x40 */ PLAYER_CSMODE_64,
    /* 0x41 */ PLAYER_CSMODE_65,
    /* 0x42 */ PLAYER_CSMODE_66,
    /* 0x43 */ PLAYER_CSMODE_67,
    /* 0x44 */ PLAYER_CSMODE_68,
    /* 0x45 */ PLAYER_CSMODE_69,
    /* 0x46 */ PLAYER_CSMODE_70,
    /* 0x47 */ PLAYER_CSMODE_71,
    /* 0x48 */ PLAYER_CSMODE_72,
    /* 0x49 */ PLAYER_CSMODE_73,
    /* 0x4A */ PLAYER_CSMODE_74,
    /* 0x4B */ PLAYER_CSMODE_75,
    /* 0x4C */ PLAYER_CSMODE_76,
    /* 0x4D */ PLAYER_CSMODE_77,
    /* 0x4E */ PLAYER_CSMODE_78,
    /* 0x4F */ PLAYER_CSMODE_79,
    /* 0x50 */ PLAYER_CSMODE_80,
    /* 0x51 */ PLAYER_CSMODE_81,
    /* 0x52 */ PLAYER_CSMODE_82,
    /* 0x53 */ PLAYER_CSMODE_83,
    /* 0x54 */ PLAYER_CSMODE_84,
    /* 0x55 */ PLAYER_CSMODE_85,
    /* 0x56 */ PLAYER_CSMODE_86,
    /* 0x57 */ PLAYER_CSMODE_87,
    /* 0x58 */ PLAYER_CSMODE_88,
    /* 0x59 */ PLAYER_CSMODE_89,
    /* 0x5A */ PLAYER_CSMODE_90,
    /* 0x5B */ PLAYER_CSMODE_91,
    /* 0x5C */ PLAYER_CSMODE_92,
    /* 0x5D */ PLAYER_CSMODE_93,
    /* 0x5E */ PLAYER_CSMODE_94,
    /* 0x5F */ PLAYER_CSMODE_95,
    /* 0x60 */ PLAYER_CSMODE_96,
    /* 0x61 */ PLAYER_CSMODE_97,
    /* 0x62 */ PLAYER_CSMODE_98,
    /* 0x63 */ PLAYER_CSMODE_99,
    /* 0x64 */ PLAYER_CSMODE_100,
    /* 0x65 */ PLAYER_CSMODE_101,
    /* 0x66 */ PLAYER_CSMODE_102,
    /* 0x67 */ PLAYER_CSMODE_103,
    /* 0x68 */ PLAYER_CSMODE_104,
    /* 0x69 */ PLAYER_CSMODE_105,
    /* 0x6A */ PLAYER_CSMODE_106,
    /* 0x6B */ PLAYER_CSMODE_107,
    /* 0x6C */ PLAYER_CSMODE_108,
    /* 0x6D */ PLAYER_CSMODE_109,
    /* 0x6E */ PLAYER_CSMODE_110,
    /* 0x6F */ PLAYER_CSMODE_111,
    /* 0x70 */ PLAYER_CSMODE_112,
    /* 0x71 */ PLAYER_CSMODE_113,
    /* 0x72 */ PLAYER_CSMODE_114,
    /* 0x73 */ PLAYER_CSMODE_115,
    /* 0x74 */ PLAYER_CSMODE_116,
    /* 0x75 */ PLAYER_CSMODE_117,
    /* 0x76 */ PLAYER_CSMODE_118,
    /* 0x77 */ PLAYER_CSMODE_119,
    /* 0x78 */ PLAYER_CSMODE_120,
    /* 0x79 */ PLAYER_CSMODE_121,
    /* 0x7A */ PLAYER_CSMODE_122,
    /* 0x7B */ PLAYER_CSMODE_123,
    /* 0x7C */ PLAYER_CSMODE_124,
    /* 0x7D */ PLAYER_CSMODE_125,
    /* 0x7E */ PLAYER_CSMODE_126,
    /* 0x7F */ PLAYER_CSMODE_127,
    /* 0x80 */ PLAYER_CSMODE_128,
    /* 0x81 */ PLAYER_CSMODE_129,
    /* 0x82 */ PLAYER_CSMODE_130,
    /* 0x83 */ PLAYER_CSMODE_131,
    /* 0x84 */ PLAYER_CSMODE_132,
    /* 0x85 */ PLAYER_CSMODE_133,
    /* 0x86 */ PLAYER_CSMODE_134,
    /* 0x87 */ PLAYER_CSMODE_135,
    /* 0x88 */ PLAYER_CSMODE_136,
    /* 0x89 */ PLAYER_CSMODE_137,
    /* 0x8A */ PLAYER_CSMODE_138,
    /* 0x8B */ PLAYER_CSMODE_139,
    /* 0x8C */ PLAYER_CSMODE_MAX
} PlayerCsMode;


typedef struct Actor_Additional {
/*       */ s8 cutscene;
} Actor_Additional; // size = 0x14C


typedef struct EnDinofos {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ Vec3s jointTable[21];
    /* 0x206 */ Vec3s morphTable[21];
    /* 0x284 */ EnDinofosActionFunc actionFunc;
    /* 0x288 */ u8 unk_288;
    /* 0x289 */ u8 unk_289;
    /* 0x28A */ u8 drawDmgEffType;
    /* 0x28B */ u8 unk_28B;
    /* 0x28C */ s16 unk_28C;
    /* 0x28E */ s16 unk_28E;
    /* 0x290 */ s16 unk_290;
    /* 0x292 */ s16 unk_292;
    /* 0x294 */ UNK_TYPE1 unk_294[4];
    /* 0x298 */ s16 subCamId;
    /* 0x29A */ Vec3s unk_29A;
    /* 0x2A0 */ s32 unk_2A0;
    /* 0x2A4 */ f32 unk_2A4;
    /* 0x2A8 */ f32 unk_2A8;
    /* 0x2AC */ f32 unk_2AC;
    /* 0x2B0 */ f32 drawDmgEffAlpha;
    /* 0x2B4 */ f32 drawDmgEffScale;
    /* 0x2B8 */ f32 drawDmgEffFrozenSteamScale;
    /* 0x2BC */ Vec3f unk_2BC;
    /* 0x2C8 */ Vec3f unk_2C8;
    /* 0x2D4 */ Vec3f limbPos[12];
    /* 0x364 */ ColliderJntSph colliderJntSph;
    /* 0x384 */ ColliderJntSphElement colliderJntSphElement[9];
    /* 0x5C4 */ ColliderQuad colliderQuad;
} EnDinofos; // size = 0x644

#endif // Z_EN_DINOFOS_H
