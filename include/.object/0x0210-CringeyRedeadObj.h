#ifndef __REDEADOBJ_H__
#define __REDEADOBJ_H__

extern u16 gRedeadObj_Tex21064308[];
extern u16 gRedeadObj_TexRGBA160600B3E0[];
extern u16 gRedeadObj_TexRustyMetalHandle[];
extern u16 gRedeadObj_TexCrap[];
extern u16 gRedeadObj_TexMetalRailling[];
extern u16 gRedeadObj_Tex0x1CA0[];
extern u16 gRedeadObj_TexDC0x2C1D832x32RGBA16[];
extern Gfx gRedeadObj_MtlFleshred[];
extern Gfx gRedeadObj_MtlMask[];
extern Gfx gRedeadObj_MtlRedMetal[];
extern Gfx gRedeadObj_MtlGuts[];
extern Gfx gRedeadObj_MtlRailling[];
extern Gfx gRedeadObj_MtlBloodyLedge[];
extern Gfx gRedeadObj_MtlTeeth001[];
extern Gfx gRedeadObj_DlRedeadObjLimb02[];
extern Gfx gRedeadObj_DlRedeadObjLimb03[];
extern Gfx gRedeadObj_DlRedeadObjLimb05[];
extern Gfx gRedeadObj_DlRedeadObjLimb07[];
extern Gfx gRedeadObj_DlRedeadObjLimb08[];
extern Gfx gRedeadObj_DlRedeadObjLimb10[];
extern Gfx gRedeadObj_DlRedeadObjLimb13[];
extern Gfx gRedeadObj_DlRedeadObjLimb15[];
extern Gfx gRedeadObj_DlRedeadObjLimb16[];
extern Gfx gRedeadObj_DlRedeadObjLimb17[];
extern Gfx gRedeadObj_DlRedeadObjLimb19[];
extern Gfx gRedeadObj_DlRedeadObjLimb20[];
extern Gfx gRedeadObj_DlRedeadObjLimb21[];
extern Gfx gRedeadObj_DlRedeadObjLimb23[];
extern Gfx gRedeadObj_DlRedeadObjLimb24[];
extern FlexSkeletonHeader gRedeadObj_SkelRedeadObj[];

typedef enum {
    REDEADOBJ_LIMB_ROOT_TRANSLATE,
    REDEADOBJ_LIMB_LIMB_00,
    REDEADOBJ_LIMB_LIMB_01,
    REDEADOBJ_LIMB_LIMB_02,
    REDEADOBJ_LIMB_LIMB_03,
    REDEADOBJ_LIMB_LIMB_04,
    REDEADOBJ_LIMB_LIMB_05,
    REDEADOBJ_LIMB_LIMB_06,
    REDEADOBJ_LIMB_LIMB_07,
    REDEADOBJ_LIMB_LIMB_08,
    REDEADOBJ_LIMB_LIMB_09,
    REDEADOBJ_LIMB_LIMB_10,
    REDEADOBJ_LIMB_LIMB_11,
    REDEADOBJ_LIMB_LIMB_12,
    REDEADOBJ_LIMB_LIMB_13,
    REDEADOBJ_LIMB_LIMB_14,
    REDEADOBJ_LIMB_LIMB_15,
    REDEADOBJ_LIMB_LIMB_16,
    REDEADOBJ_LIMB_LIMB_17,
    REDEADOBJ_LIMB_LIMB_18,
    REDEADOBJ_LIMB_LIMB_19,
    REDEADOBJ_LIMB_LIMB_20,
    REDEADOBJ_LIMB_LIMB_21,
    REDEADOBJ_LIMB_LIMB_22,
    REDEADOBJ_LIMB_LIMB_23,
    REDEADOBJ_LIMB_LIMB_24,
    REDEADOBJ_LIMB_MAX,
} SkelRedeadObjLimbs;

extern AnimationHeader gRedeadObj_AnimChokePlayer[];
extern AnimationHeader gRedeadObj_AnimCroucharmswipe[];
extern AnimationHeader gRedeadObj_AnimCrouching[];
extern AnimationHeader gRedeadObj_AnimDamaged[];
extern AnimationHeader gRedeadObj_AnimDying[];
extern AnimationHeader gRedeadObj_AnimGrabPlayer[];
extern AnimationHeader gRedeadObj_AnimIdle[];
extern AnimationHeader gRedeadObj_AnimReleasePlayer[];
extern AnimationHeader gRedeadObj_AnimRest[];
extern AnimationHeader gRedeadObj_AnimSlowWalk[];
extern AnimationHeader gRedeadObj_AnimStandUp[];
extern AnimationHeader gRedeadObj_AnimTurn90Deg[];

#endif /* __REDEADOBJ_H__ */
