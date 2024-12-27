#ifndef __TANKHEADOBJ_H__
#define __TANKHEADOBJ_H__

extern u16 gTankHeadObj_TexYellowMetalPlate[];
extern u16 gTankHeadObj_Tex0x7E50[];
extern u16 gTankHeadObj_Tex0x7E500[];
extern u16 gTankHeadObj_Tex0x7E501[];
extern u16 gTankHeadObj_Tex0x7E502[];
extern u16 gTankHeadObj_TexYellowMetalPlate2[];
extern u16 gTankHeadObj_TexYellowMetalGear[];
extern u16 gTankHeadObj_TexMetalPole02[];
extern u16 gTankHeadObj_TexSpikeWallOrTrap1[];
extern u16 gTankHeadObj_TexMetalRailling[];
extern u16 gTankHeadObj_TexYellowMetal[];
extern Gfx gTankHeadObj_MtlGoronSkin[];
extern Gfx gTankHeadObj_MtlMetalEye[];
extern Gfx gTankHeadObj_MtlYellowMetalPlate2[];
extern Gfx gTankHeadObj_MtlMetal[];
extern Gfx gTankHeadObj_MtlStairs2[];
extern Gfx gTankHeadObj_MtlDarkWoodWith2Holes[];
extern Gfx gTankHeadObj_MtlMetalRailling[];
extern Gfx gTankHeadObj_MtlYellowMetal[];
extern Gfx gTankHeadObj_DlTankHeadObjNeck[];
extern Gfx gTankHeadObj_DlTankHeadObjHead[];
extern Gfx gTankHeadObj_DlTankHeadObjInnerGunR[];
extern Gfx gTankHeadObj_DlTankHeadObjOutGunR[];
extern Gfx gTankHeadObj_DlTankHeadObjShellEntryR[];
extern Gfx gTankHeadObj_DlTankHeadObjInnerGunL[];
extern Gfx gTankHeadObj_DlTankHeadObjOutGunL[];
extern Gfx gTankHeadObj_DlTankHeadObjShellEntryL[];
extern Gfx gTankHeadObj_DlTankHeadObjSmokeLauncherUpL[];
extern Gfx gTankHeadObj_DlTankHeadObjSmokeLauncherUpR[];
extern Gfx gTankHeadObj_DlTankHeadObjSmokeLauncherDown0L[];
extern Gfx gTankHeadObj_DlTankHeadObjSmokeLauncherDown0R[];
extern Gfx gTankHeadObj_DlTankHeadObjSmokeLauncherDown1L[];
extern Gfx gTankHeadObj_DlTankHeadObjSmokeLauncherDown1R[];
extern FlexSkeletonHeader gTankHeadObj_SkelTankHeadObj[];

typedef enum {
    TANKHEADOBJ_LIMB_ROOT_TRANSLATE,
    TANKHEADOBJ_LIMB_NECK,
    TANKHEADOBJ_LIMB_HEAD,
    TANKHEADOBJ_LIMB_INNERGUN_R,
    TANKHEADOBJ_LIMB_OUTGUN_R,
    TANKHEADOBJ_LIMB_SHELLENTRY_R,
    TANKHEADOBJ_LIMB_INNERGUN_L,
    TANKHEADOBJ_LIMB_OUTGUN_L,
    TANKHEADOBJ_LIMB_SHELLENTRY_L,
    TANKHEADOBJ_LIMB_SMOKELAUNCHERUP_L,
    TANKHEADOBJ_LIMB_SMOKELAUNCHERUP_R,
    TANKHEADOBJ_LIMB_SMOKELAUNCHERDOWN0_L,
    TANKHEADOBJ_LIMB_SMOKELAUNCHERDOWN0_R,
    TANKHEADOBJ_LIMB_SMOKELAUNCHERDOWN1_L,
    TANKHEADOBJ_LIMB_SMOKELAUNCHERDOWN1_R,
    TANKHEADOBJ_LIMB_MAX,
} SkelTankHeadObjLimbs;

extern AnimationHeader gTankHeadObj_AnimAlert[];
extern AnimationHeader gTankHeadObj_AnimDamaged[];
extern AnimationHeader gTankHeadObj_AnimDead[];
extern AnimationHeader gTankHeadObj_AnimFire[];
extern AnimationHeader gTankHeadObj_AnimFireKillingMode[];
extern AnimationHeader gTankHeadObj_AnimHitGround[];
extern AnimationHeader gTankHeadObj_AnimRest[];
extern AnimationHeader gTankHeadObj_AnimRotateL[];
extern AnimationHeader gTankHeadObj_AnimRotateR[];
extern AnimationHeader gTankHeadObj_AnimSeek[];
extern AnimationHeader gTankHeadObj_AnimStun[];

#endif /* __TANKHEADOBJ_H__ */
