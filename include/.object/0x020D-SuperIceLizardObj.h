#ifndef __SUPERICELIZARDOBJ_H__
#define __SUPERICELIZARDOBJ_H__

extern u16 gSuperIceLizardObj_TexHeadTongue[];
extern u16 gSuperIceLizardObj_TexTailBackMouth[];
extern u16 gSuperIceLizardObj_TexBack[];
extern u16 gSuperIceLizardObj_TexBelly[];
extern u16 gSuperIceLizardObj_TexTailUnder[];
extern u16 gSuperIceLizardObj_TexEye[];
extern u16 gSuperIceLizardObj_TexArmsLegs[];
extern u16 gSuperIceLizardObj_TexArmor[];
extern u16 gSuperIceLizardObj_TexSpear[];
extern Gfx gSuperIceLizardObj_MtlHead[];
extern Gfx gSuperIceLizardObj_MtlInnerMouth[];
extern Gfx gSuperIceLizardObj_MtlBack[];
extern Gfx gSuperIceLizardObj_MtlBelly[];
extern Gfx gSuperIceLizardObj_MtlTailBack[];
extern Gfx gSuperIceLizardObj_MtlTailUnder[];
extern Gfx gSuperIceLizardObj_MtlEye[];
extern Gfx gSuperIceLizardObj_MtlArmsLegs[];
extern Gfx gSuperIceLizardObj_MtlArmor[];
extern Gfx gSuperIceLizardObj_MtlSpear[];
extern Gfx gSuperIceLizardObj_DlSuperIceLizardObjRoot[];
extern Gfx gSuperIceLizardObj_DlSuperIceLizardObjSpine1[];
extern Gfx gSuperIceLizardObj_DlSuperIceLizardObjSpine2[];
extern Gfx gSuperIceLizardObj_DlSuperIceLizardObjSpine3[];
extern Gfx gSuperIceLizardObj_DlSuperIceLizardObjSpine4[];
extern Gfx gSuperIceLizardObj_DlSuperIceLizardObjNeck[];
extern Gfx gSuperIceLizardObj_DlSuperIceLizardObjHead[];
extern Gfx gSuperIceLizardObj_DlSuperIceLizardObjJaw[];
extern Gfx gSuperIceLizardObj_DlSuperIceLizardObjEyeL[];
extern Gfx gSuperIceLizardObj_DlSuperIceLizardObjEyeR[];
extern Gfx gSuperIceLizardObj_DlSuperIceLizardObjTongue1[];
extern Gfx gSuperIceLizardObj_DlSuperIceLizardObjTongue2[];
extern Gfx gSuperIceLizardObj_DlSuperIceLizardObjTongue3[];
extern Gfx gSuperIceLizardObj_DlSuperIceLizardObjInnerarmL[];
extern Gfx gSuperIceLizardObj_DlSuperIceLizardObjOuterarmL[];
extern Gfx gSuperIceLizardObj_DlSuperIceLizardObjHandL[];
extern Gfx gSuperIceLizardObj_DlSuperIceLizardObjInnerarmR[];
extern Gfx gSuperIceLizardObj_DlSuperIceLizardObjOuterarmR[];
extern Gfx gSuperIceLizardObj_DlSuperIceLizardObjHandR[];
extern Gfx gSuperIceLizardObj_DlSuperIceLizardObjHalberd[];
extern Gfx gSuperIceLizardObj_DlSuperIceLizardObjUpperlegL[];
extern Gfx gSuperIceLizardObj_DlSuperIceLizardObjLowerlegL[];
extern Gfx gSuperIceLizardObj_DlSuperIceLizardObjFootL[];
extern Gfx gSuperIceLizardObj_DlSuperIceLizardObjUpperlegR[];
extern Gfx gSuperIceLizardObj_DlSuperIceLizardObjLowerlegR[];
extern Gfx gSuperIceLizardObj_DlSuperIceLizardObjFootR[];
extern Gfx gSuperIceLizardObj_DlSuperIceLizardObjTail1[];
extern Gfx gSuperIceLizardObj_DlSuperIceLizardObjTail2[];
extern Gfx gSuperIceLizardObj_DlSuperIceLizardObjTail3[];
extern Gfx gSuperIceLizardObj_DlSuperIceLizardObjTail4[];
extern Gfx gSuperIceLizardObj_DlSuperIceLizardObjTail5[];
extern Gfx gSuperIceLizardObj_DlSuperIceLizardObjTail6[];
extern Gfx gSuperIceLizardObj_DlSuperIceLizardObjTail7[];
extern FlexSkeletonHeader gSuperIceLizardObj_SkelSuperIceLizardObj[];

typedef enum {
    SUPERICELIZARDOBJ_LIMB_ROOT_TRANSLATE,
    SUPERICELIZARDOBJ_LIMB_ROOT,
    SUPERICELIZARDOBJ_LIMB_SPINE1,
    SUPERICELIZARDOBJ_LIMB_SPINE2,
    SUPERICELIZARDOBJ_LIMB_SPINE3,
    SUPERICELIZARDOBJ_LIMB_SPINE4,
    SUPERICELIZARDOBJ_LIMB_NECK,
    SUPERICELIZARDOBJ_LIMB_HEAD,
    SUPERICELIZARDOBJ_LIMB_JAW,
    SUPERICELIZARDOBJ_LIMB_EYE_L,
    SUPERICELIZARDOBJ_LIMB_EYE_R,
    SUPERICELIZARDOBJ_LIMB_TONGUE0,
    SUPERICELIZARDOBJ_LIMB_ACCORDIAN0,
    SUPERICELIZARDOBJ_LIMB_ACCORDIAN1,
    SUPERICELIZARDOBJ_LIMB_TONGUE1,
    SUPERICELIZARDOBJ_LIMB_ACCORDIAN2,
    SUPERICELIZARDOBJ_LIMB_ACCORDIAN3,
    SUPERICELIZARDOBJ_LIMB_TONGUE2,
    SUPERICELIZARDOBJ_LIMB_ACCORDIAN4,
    SUPERICELIZARDOBJ_LIMB_ACCORDIAN5,
    SUPERICELIZARDOBJ_LIMB_TONGUE3,
    SUPERICELIZARDOBJ_LIMB_INNERARM_L,
    SUPERICELIZARDOBJ_LIMB_OUTERARM_L,
    SUPERICELIZARDOBJ_LIMB_HAND_L,
    SUPERICELIZARDOBJ_LIMB_INNERARM_R,
    SUPERICELIZARDOBJ_LIMB_OUTERARM_R,
    SUPERICELIZARDOBJ_LIMB_HAND_R,
    SUPERICELIZARDOBJ_LIMB_HALBERD,
    SUPERICELIZARDOBJ_LIMB_UPPERLEG_L,
    SUPERICELIZARDOBJ_LIMB_LOWERLEG_L,
    SUPERICELIZARDOBJ_LIMB_FOOT_L,
    SUPERICELIZARDOBJ_LIMB_UPPERLEG_R,
    SUPERICELIZARDOBJ_LIMB_LOWERLEG_R,
    SUPERICELIZARDOBJ_LIMB_FOOT_R,
    SUPERICELIZARDOBJ_LIMB_TAIL1,
    SUPERICELIZARDOBJ_LIMB_TAIL2,
    SUPERICELIZARDOBJ_LIMB_TAIL3,
    SUPERICELIZARDOBJ_LIMB_TAIL4,
    SUPERICELIZARDOBJ_LIMB_TAIL5,
    SUPERICELIZARDOBJ_LIMB_TAIL6,
    SUPERICELIZARDOBJ_LIMB_TAIL7,
    SUPERICELIZARDOBJ_LIMB_MAX,
} SkelSuperIceLizardObjLimbs;

extern AnimationHeader gSuperIceLizardObj_AnimBlock[];
extern AnimationHeader gSuperIceLizardObj_AnimBreatheAttack[];
extern AnimationHeader gSuperIceLizardObj_AnimCharge[];
extern AnimationHeader gSuperIceLizardObj_AnimDamageTake[];
extern AnimationHeader gSuperIceLizardObj_AnimDeath[];
extern AnimationHeader gSuperIceLizardObj_AnimJumpAttack[];
extern AnimationHeader gSuperIceLizardObj_AnimJumpBack[];
extern AnimationHeader gSuperIceLizardObj_AnimPatrolWalk[];
extern AnimationHeader gSuperIceLizardObj_AnimStandDetection[];
extern AnimationHeader gSuperIceLizardObj_AnimStandAlert[];
extern AnimationHeader gSuperIceLizardObj_AnimStrafe[];
extern AnimationHeader gSuperIceLizardObj_AnimTailAttackLeftRight[];
extern AnimationHeader gSuperIceLizardObj_AnimTailAttackRightLeft[];
extern AnimationHeader gSuperIceLizardObj_AnimThrustAttack[];
extern AnimationHeader gSuperIceLizardObj_AnimTongueAttack[];

#endif /* __SUPERICELIZARDOBJ_H__ */
