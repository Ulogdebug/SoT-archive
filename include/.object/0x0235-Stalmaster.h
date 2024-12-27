#ifndef __STALMASTER_H__
#define __STALMASTER_H__

extern u16 gStalmaster_TexGloveStart[];
extern u16 gStalmaster_TexGloveInner[];
extern u16 gStalmaster_TexPalmOuter[];
extern u16 gStalmaster_TexPalmInner[];
extern u16 gStalmaster_TexSkull[];
extern u16 gStalmaster_TexJaw[];
extern u16 gStalmaster_TexBoneFore[];
extern u16 gStalmaster_TexBoneArm[];
extern u16 gStalmaster_TexShoulder[];
extern u16 gStalmaster_TexPins[];
extern u16 gStalmaster_TexSpine[];
extern u16 gStalmaster_TexPelvis[];
extern u16 gStalmaster_TexBoot[];
extern u16 gStalmaster_TexFoot[];
extern u16 gStalmaster_TexStalfos4Eye[];
extern u16 gStalmaster_TexSkirt[];
extern u16 gStalmaster_TexBelt[];
extern u16 gStalmaster_TexHelmet[];
extern u16 gStalmaster_TexHelmetSide[];
extern u16 gStalmaster_TexChest[];
extern u16 gStalmaster_TexBack[];
extern u16 gStalmaster_TexSworda[];
extern u16 gStalmaster_TexHilt[];
extern u16 gStalmaster_TexSkullHandle[];
extern u16 gStalmaster_TexSwordb[];
extern u16 gStalmaster_TexSwordc[];
extern u16 gStalmaster_TexSwordd[];
extern Gfx gStalmaster_MtlGloveStart[];
extern Gfx gStalmaster_MtlGloveInner[];
extern Gfx gStalmaster_MtlPalmOuter[];
extern Gfx gStalmaster_MtlPalmInner[];
extern Gfx gStalmaster_MtlSkull[];
extern Gfx gStalmaster_MtlJaw[];
extern Gfx gStalmaster_MtlBoneFore[];
extern Gfx gStalmaster_MtlBoneArm[];
extern Gfx gStalmaster_MtlShoulder[];
extern Gfx gStalmaster_MtlPins[];
extern Gfx gStalmaster_MtlSpine[];
extern Gfx gStalmaster_MtlPelvis[];
extern Gfx gStalmaster_MtlBoot[];
extern Gfx gStalmaster_MtlFoot[];
extern Gfx gStalmaster_MtlEye[];
extern Gfx gStalmaster_MtlSkirt[];
extern Gfx gStalmaster_MtlBelt[];
extern Gfx gStalmaster_MtlHelmet[];
extern Gfx gStalmaster_MtlHelmetSide[];
extern Gfx gStalmaster_MtlChest[];
extern Gfx gStalmaster_MtlBack[];
extern Gfx gStalmaster_MtlSworda[];
extern Gfx gStalmaster_MtlHilt[];
extern Gfx gStalmaster_MtlSkullHandle[];
extern Gfx gStalmaster_MtlSwordb[];
extern Gfx gStalmaster_MtlSwordc[];
extern Gfx gStalmaster_MtlSwordd[];
extern Gfx gStalmaster_DlStalmasterBackbone01[];
extern Gfx gStalmaster_DlStalmasterBackbone02[];
extern Gfx gStalmaster_DlStalmasterArmLD01[];
extern Gfx gStalmaster_DlStalmasterArmLD02[];
extern Gfx gStalmaster_DlStalmasterHandLD[];
extern Gfx gStalmaster_DlStalmasterFingerLD01[];
extern Gfx gStalmaster_DlStalmasterFingerLD02[];
extern Gfx gStalmaster_DlStalmasterPadLD[];
extern Gfx gStalmaster_DlStalmasterArmLU01[];
extern Gfx gStalmaster_DlStalmasterArmLU02[];
extern Gfx gStalmaster_DlStalmasterHandLU[];
extern Gfx gStalmaster_DlStalmasterFingerLU01[];
extern Gfx gStalmaster_DlStalmasterFingerLU02[];
extern Gfx gStalmaster_DlStalmasterPadLU[];
extern Gfx gStalmaster_DlStalmasterArmRD01[];
extern Gfx gStalmaster_DlStalmasterArmRD02[];
extern Gfx gStalmaster_DlStalmasterHandRD[];
extern Gfx gStalmaster_DlStalmasterFingerRD01[];
extern Gfx gStalmaster_DlStalmasterFingerRD02[];
extern Gfx gStalmaster_DlStalmasterPadRD[];
extern Gfx gStalmaster_DlStalmasterArmRU01[];
extern Gfx gStalmaster_DlStalmasterArmRU02[];
extern Gfx gStalmaster_DlStalmasterHandRU[];
extern Gfx gStalmaster_DlStalmasterFingerRU01[];
extern Gfx gStalmaster_DlStalmasterFingerRU02[];
extern Gfx gStalmaster_DlStalmasterPadRU[];
extern Gfx gStalmaster_DlStalmasterNeck[];
extern Gfx gStalmaster_DlStalmasterHead[];
extern Gfx gStalmaster_DlStalmasterChin[];
extern Gfx gStalmaster_DlStalmasterWaist[];
extern Gfx gStalmaster_DlStalmasterLegL01[];
extern Gfx gStalmaster_DlStalmasterLegL02[];
extern Gfx gStalmaster_DlStalmasterFootL[];
extern Gfx gStalmaster_DlStalmasterLegR01[];
extern Gfx gStalmaster_DlStalmasterLegR02[];
extern Gfx gStalmaster_DlStalmasterFootR[];
extern FlexSkeletonHeader gStalmaster_SkelStalmaster[];

typedef enum {
    STALMASTER_LIMB_ROOT_TRANSLATE,
    STALMASTER_LIMB_CENTER,
    STALMASTER_LIMB_BACKBONE01,
    STALMASTER_LIMB_BACKBONE02,
    STALMASTER_LIMB_ARMLD01,
    STALMASTER_LIMB_ARMLD02,
    STALMASTER_LIMB_HANDLD,
    STALMASTER_LIMB_FINGERLD01,
    STALMASTER_LIMB_FINGERLD02,
    STALMASTER_LIMB_LOC_SWORDD,
    STALMASTER_LIMB_PADLD,
    STALMASTER_LIMB_ARMLU01,
    STALMASTER_LIMB_ARMLU02,
    STALMASTER_LIMB_HANDLU,
    STALMASTER_LIMB_FINGERLU01,
    STALMASTER_LIMB_FINGERLU02,
    STALMASTER_LIMB_LOC_SWORDA,
    STALMASTER_LIMB_PADLU,
    STALMASTER_LIMB_ARMRD01,
    STALMASTER_LIMB_ARMRD02,
    STALMASTER_LIMB_HANDRD,
    STALMASTER_LIMB_FINGERRD01,
    STALMASTER_LIMB_FINGERRD02,
    STALMASTER_LIMB_LOC_SWORDC,
    STALMASTER_LIMB_PADRD,
    STALMASTER_LIMB_ARMRU01,
    STALMASTER_LIMB_ARMRU02,
    STALMASTER_LIMB_HANDRU,
    STALMASTER_LIMB_FINGERRU01,
    STALMASTER_LIMB_FINGERRU02,
    STALMASTER_LIMB_LOC_SWORDB,
    STALMASTER_LIMB_PADRU,
    STALMASTER_LIMB_NECK,
    STALMASTER_LIMB_HEAD,
    STALMASTER_LIMB_BAND01,
    STALMASTER_LIMB_BAND02,
    STALMASTER_LIMB_BAND03,
    STALMASTER_LIMB_BAND04,
    STALMASTER_LIMB_BAND05,
    STALMASTER_LIMB_CHIN,
    STALMASTER_LIMB_WAIST,
    STALMASTER_LIMB_LEGL01,
    STALMASTER_LIMB_LEGL02,
    STALMASTER_LIMB_FOOTL,
    STALMASTER_LIMB_TOEL,
    STALMASTER_LIMB_LEGR01,
    STALMASTER_LIMB_LEGR02,
    STALMASTER_LIMB_FOOTR,
    STALMASTER_LIMB_TOER,
    STALMASTER_LIMB_LOC_SWORDC2,
    STALMASTER_LIMB_LOC_SWORDD2,
    STALMASTER_LIMB_MAX,
} SkelStalmasterLimbs;

extern Gfx gStalmaster_DlHelmet[];
extern Gfx gStalmaster_DlChestplate[];
extern Gfx gStalmaster_DlTip[];
extern Gfx gStalmaster_DlTail[];
extern Gfx gStalmaster_DlSwordA[];
extern Gfx gStalmaster_DlSwordB[];
extern Gfx gStalmaster_DlSwordC[];
extern Gfx gStalmaster_DlSwordD[];
extern Gfx gStalmaster_DlSwordC2[];
extern Gfx gStalmaster_DlSwordD2[];
extern Gfx gStalmaster_DlArmR[];
extern Gfx gStalmaster_DlPadR[];
extern Gfx gStalmaster_DlForearmR[];
extern Gfx gStalmaster_DlHead[];
extern Gfx gStalmaster_DlBackbone[];
extern Gfx gStalmaster_DlArmL[];
extern Gfx gStalmaster_DlPadL[];
extern Gfx gStalmaster_DlForearmL[];
extern AnimationHeader gStalmaster_Anim2Counter[];
extern AnimationHeader gStalmaster_Anim2Guard[];
extern AnimationHeader gStalmaster_Anim2GuardJust[];
extern AnimationHeader gStalmaster_Anim2Walk[];
extern AnimationHeader gStalmaster_Anim4Counter[];
extern AnimationHeader gStalmaster_Anim4Guard[];
extern AnimationHeader gStalmaster_Anim4GuardJust[];
extern AnimationHeader gStalmaster_Anim4PoseC[];
extern AnimationHeader gStalmaster_Anim4Walk[];
extern AnimationHeader gStalmaster_AnimAttackA[];
extern AnimationHeader gStalmaster_AnimAttackB[];
extern AnimationHeader gStalmaster_AnimAttackC[];
extern AnimationHeader gStalmaster_AnimAttackD[];
extern AnimationHeader gStalmaster_AnimEnd[];
extern AnimationHeader gStalmaster_AnimOp[];
extern AnimationHeader gStalmaster_AnimSwordPose[];

#endif /* __STALMASTER_H__ */
