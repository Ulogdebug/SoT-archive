#ifndef __BOB_H__
#define __BOB_H__

extern u16 gBoB_TexBoBody1[];
extern u16 gBoB_TexBoBFace1[];
extern u16 gBoB_TexBone2001[];
extern u16 gBoB_TexEffectBall[];
extern u16 gBoB_TexBoneSpear[];
extern Gfx gBoB_MtlBody[];
extern Gfx gBoB_MtlHead[];
extern Gfx gBoB_MtlBones[];
extern Gfx gBoB_MtlBallEffect[];
extern Gfx gBoB_MtlProjectile[];
extern Gfx gBoB_DlBodyMainBody[];
extern Gfx gBoB_DlBodyLegs[];
extern Gfx gBoB_DlBodyArm1L[];
extern Gfx gBoB_DlBodyArm2L[];
extern Gfx gBoB_DlBodyArm3L[];
extern Gfx gBoB_DlBodyHead[];
extern Gfx gBoB_DlBodyArm1R[];
extern Gfx gBoB_DlBodyArm2R[];
extern Gfx gBoB_DlBodyArm3R[];
extern FlexSkeletonHeader gBoB_SkelBody[];

typedef enum {
    BODY_LIMB_ROOT_TRANSLATE,
    BODY_LIMB_MAINBODY,
    BODY_LIMB_LEGS,
    BODY_LIMB_ARM1_L,
    BODY_LIMB_ARM2_L,
    BODY_LIMB_ARM3_L,
    BODY_LIMB_HEAD,
    BODY_LIMB_ARM1_R,
    BODY_LIMB_ARM2_R,
    BODY_LIMB_ARM3_R,
    BODY_LIMB_MAX,
} SkelBodyLimbs;

extern Gfx gBoB_DlFlatBone[];
extern Gfx gBoB_DlEffect[];
extern Gfx gBoB_DlEffect2[];
extern Gfx gBoB_DlProjectile[];
extern AnimationHeader gBoB_AnimCrawl[];
extern AnimationHeader gBoB_AnimCry[];
extern AnimationHeader gBoB_AnimDamage[];
extern AnimationHeader gBoB_AnimDeath[];
extern AnimationHeader gBoB_AnimPray[];
extern AnimationHeader gBoB_AnimShootAttack[];

#endif /* __BOB_H__ */
