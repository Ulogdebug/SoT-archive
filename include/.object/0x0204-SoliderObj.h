#ifndef __SOLIDER_H__
#define __SOLIDER_H__

extern u16 gSolider_TexZELDAMAJORASMASK8EDEDBA921B2354BFDCiByRGBA[];
extern u16 gSolider_TexBlack[];
extern u16 gSolider_TexDT0x1BBE032x64CI8[];
extern u16 gSolider_TexZELDAMAJORASMASKC70E4C2B201B72EA64CiByRGBA[];
extern u16 gSolider_Tex00016258128x32I4[];
extern u16 gSolider_TexHyruleFlagRy[];
extern u16 gSolider_TexZELDAMAJORASMASKC30FED3402All[];
extern Gfx gSolider_MtlIron005[];
extern Gfx gSolider_MtlBlackColor[];
extern Gfx gSolider_MtlDirt014[];
extern Gfx gSolider_MtlIronHole[];
extern Gfx gSolider_MtlInscription004[];
extern Gfx gSolider_MtlHyruleFlag[];
extern Gfx gSolider_MtlSymbol002[];
extern Gfx gSolider_DlSoliderObjTorso[];
extern Gfx gSolider_DlSoliderObjNeck[];
extern Gfx gSolider_DlSoliderObjHead[];
extern Gfx gSolider_DlSoliderObjUpperArmL[];
extern Gfx gSolider_DlSoliderObjForearmL[];
extern Gfx gSolider_DlSoliderObjHandL[];
extern Gfx gSolider_DlSoliderObjUpperArmR[];
extern Gfx gSolider_DlSoliderObjForearmR[];
extern Gfx gSolider_DlSoliderObjHandR[];
extern Gfx gSolider_DlSoliderObjThighL[];
extern Gfx gSolider_DlSoliderObjShinL[];
extern Gfx gSolider_DlSoliderObjFootL[];
extern Gfx gSolider_DlSoliderObjThighR[];
extern Gfx gSolider_DlSoliderObjShinR[];
extern Gfx gSolider_DlSoliderObjFootR[];
extern Gfx gSolider_DlSoliderObjWaist[];
extern FlexSkeletonHeader gSolider_SkelSoliderObj[];

typedef enum {
    SOLIDEROBJ_LIMB_ROOT_TRANSLATE,
    SOLIDEROBJ_LIMB_ROOT,
    SOLIDEROBJ_LIMB_TORSO,
    SOLIDEROBJ_LIMB_NECK,
    SOLIDEROBJ_LIMB_HEAD,
    SOLIDEROBJ_LIMB_UPPER_ARM_L,
    SOLIDEROBJ_LIMB_FOREARM_L,
    SOLIDEROBJ_LIMB_HAND_L,
    SOLIDEROBJ_LIMB_UPPER_ARM_R,
    SOLIDEROBJ_LIMB_FOREARM_R,
    SOLIDEROBJ_LIMB_HAND_R,
    SOLIDEROBJ_LIMB_THIGH_L,
    SOLIDEROBJ_LIMB_SHIN_L,
    SOLIDEROBJ_LIMB_FOOT_L,
    SOLIDEROBJ_LIMB_THIGH_R,
    SOLIDEROBJ_LIMB_SHIN_R,
    SOLIDEROBJ_LIMB_FOOT_R,
    SOLIDEROBJ_LIMB_WAIST,
    SOLIDEROBJ_LIMB_MAX,
} SkelSoliderObjLimbs;

extern AnimationHeader gSolider_AnimIdle[];
extern AnimationHeader gSolider_AnimTalk[];
extern AnimationHeader gSolider_AnimWalk[];

#endif /* __SOLIDER_H__ */
