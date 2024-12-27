#ifndef __SUPERLIZARDOBJ_H__
#define __SUPERLIZARDOBJ_H__

extern u16 gSuperLizardObj_TexHeadTongue[];
extern u16 gSuperLizardObj_TexTailBackMouth[];
extern u16 gSuperLizardObj_TexBack[];
extern u16 gSuperLizardObj_TexBelly[];
extern u16 gSuperLizardObj_TexTailUnder[];
extern u16 gSuperLizardObj_TexEye[];
extern u16 gSuperLizardObj_TexArmsLegs[];
extern u16 gSuperLizardObj_TexArmor[];
extern u16 gSuperLizardObj_TexSpear[];
extern Gfx gSuperLizardObj_MtlHead[];
extern Gfx gSuperLizardObj_MtlInnerMouth[];
extern Gfx gSuperLizardObj_MtlBack[];
extern Gfx gSuperLizardObj_MtlBelly[];
extern Gfx gSuperLizardObj_MtlTailBack[];
extern Gfx gSuperLizardObj_MtlTailUnder[];
extern Gfx gSuperLizardObj_MtlEye[];
extern Gfx gSuperLizardObj_MtlArmsLegs[];
extern Gfx gSuperLizardObj_MtlArmor[];
extern Gfx gSuperLizardObj_MtlSpear[];
extern Gfx gSuperLizardObj_DlSuperLizardObjRoot[];
extern Gfx gSuperLizardObj_DlSuperLizardObjSpine1[];
extern Gfx gSuperLizardObj_DlSuperLizardObjSpine2[];
extern Gfx gSuperLizardObj_DlSuperLizardObjSpine3[];
extern Gfx gSuperLizardObj_DlSuperLizardObjSpine4[];
extern Gfx gSuperLizardObj_DlSuperLizardObjNeck[];
extern Gfx gSuperLizardObj_DlSuperLizardObjHead[];
extern Gfx gSuperLizardObj_DlSuperLizardObjJaw[];
extern Gfx gSuperLizardObj_DlSuperLizardObjEyeL[];
extern Gfx gSuperLizardObj_DlSuperLizardObjEyeR[];
extern Gfx gSuperLizardObj_DlSuperLizardObjTongue1[];
extern Gfx gSuperLizardObj_DlSuperLizardObjTongue2[];
extern Gfx gSuperLizardObj_DlSuperLizardObjTongue3[];
extern Gfx gSuperLizardObj_DlSuperLizardObjInnerarmL[];
extern Gfx gSuperLizardObj_DlSuperLizardObjOuterarmL[];
extern Gfx gSuperLizardObj_DlSuperLizardObjHandL[];
extern Gfx gSuperLizardObj_DlSuperLizardObjInnerarmR[];
extern Gfx gSuperLizardObj_DlSuperLizardObjOuterarmR[];
extern Gfx gSuperLizardObj_DlSuperLizardObjHandR[];
extern Gfx gSuperLizardObj_DlSuperLizardObjHalberd[];
extern Gfx gSuperLizardObj_DlSuperLizardObjUpperlegL[];
extern Gfx gSuperLizardObj_DlSuperLizardObjLowerlegL[];
extern Gfx gSuperLizardObj_DlSuperLizardObjFootL[];
extern Gfx gSuperLizardObj_DlSuperLizardObjUpperlegR[];
extern Gfx gSuperLizardObj_DlSuperLizardObjLowerlegR[];
extern Gfx gSuperLizardObj_DlSuperLizardObjFootR[];
extern Gfx gSuperLizardObj_DlSuperLizardObjTail1[];
extern Gfx gSuperLizardObj_DlSuperLizardObjTail2[];
extern Gfx gSuperLizardObj_DlSuperLizardObjTail3[];
extern Gfx gSuperLizardObj_DlSuperLizardObjTail4[];
extern Gfx gSuperLizardObj_DlSuperLizardObjTail5[];
extern Gfx gSuperLizardObj_DlSuperLizardObjTail6[];
extern Gfx gSuperLizardObj_DlSuperLizardObjTail7[];
extern FlexSkeletonHeader gSuperLizardObj_SkelSuperLizardObj[];

typedef enum {
    SUPERLIZARDOBJ_LIMB_ROOT_TRANSLATE,
    SUPERLIZARDOBJ_LIMB_ROOT,
    SUPERLIZARDOBJ_LIMB_SPINE1,
    SUPERLIZARDOBJ_LIMB_SPINE2,
    SUPERLIZARDOBJ_LIMB_SPINE3,
    SUPERLIZARDOBJ_LIMB_SPINE4,
    SUPERLIZARDOBJ_LIMB_NECK,
    SUPERLIZARDOBJ_LIMB_HEAD,
    SUPERLIZARDOBJ_LIMB_JAW,
    SUPERLIZARDOBJ_LIMB_EYE_L,
    SUPERLIZARDOBJ_LIMB_EYE_R,
    SUPERLIZARDOBJ_LIMB_TONGUE0,
    SUPERLIZARDOBJ_LIMB_ACCORDIAN0,
    SUPERLIZARDOBJ_LIMB_ACCORDIAN1,
    SUPERLIZARDOBJ_LIMB_TONGUE1,
    SUPERLIZARDOBJ_LIMB_ACCORDIAN2,
    SUPERLIZARDOBJ_LIMB_ACCORDIAN3,
    SUPERLIZARDOBJ_LIMB_TONGUE2,
    SUPERLIZARDOBJ_LIMB_ACCORDIAN4,
    SUPERLIZARDOBJ_LIMB_ACCORDIAN5,
    SUPERLIZARDOBJ_LIMB_TONGUE3,
    SUPERLIZARDOBJ_LIMB_INNERARM_L,
    SUPERLIZARDOBJ_LIMB_OUTERARM_L,
    SUPERLIZARDOBJ_LIMB_HAND_L,
    SUPERLIZARDOBJ_LIMB_INNERARM_R,
    SUPERLIZARDOBJ_LIMB_OUTERARM_R,
    SUPERLIZARDOBJ_LIMB_HAND_R,
    SUPERLIZARDOBJ_LIMB_HALBERD,
    SUPERLIZARDOBJ_LIMB_UPPERLEG_L,
    SUPERLIZARDOBJ_LIMB_LOWERLEG_L,
    SUPERLIZARDOBJ_LIMB_FOOT_L,
    SUPERLIZARDOBJ_LIMB_UPPERLEG_R,
    SUPERLIZARDOBJ_LIMB_LOWERLEG_R,
    SUPERLIZARDOBJ_LIMB_FOOT_R,
    SUPERLIZARDOBJ_LIMB_TAIL1,
    SUPERLIZARDOBJ_LIMB_TAIL2,
    SUPERLIZARDOBJ_LIMB_TAIL3,
    SUPERLIZARDOBJ_LIMB_TAIL4,
    SUPERLIZARDOBJ_LIMB_TAIL5,
    SUPERLIZARDOBJ_LIMB_TAIL6,
    SUPERLIZARDOBJ_LIMB_TAIL7,
    SUPERLIZARDOBJ_LIMB_MAX,
} SkelSuperLizardObjLimbs;

extern AnimationHeader gSuperLizardObj_AnimBlock[];
extern AnimationHeader gSuperLizardObj_AnimBreatheAttack[];
extern AnimationHeader gSuperLizardObj_AnimCharge[];
extern AnimationHeader gSuperLizardObj_AnimDamageTake[];
extern AnimationHeader gSuperLizardObj_AnimDeath[];
extern AnimationHeader gSuperLizardObj_AnimJumpAttack[];
extern AnimationHeader gSuperLizardObj_AnimJumpBack[];
extern AnimationHeader gSuperLizardObj_AnimPatrolWalk[];
extern AnimationHeader gSuperLizardObj_AnimStandDetection[];
extern AnimationHeader gSuperLizardObj_AnimStandAlert[];
extern AnimationHeader gSuperLizardObj_AnimStrafe[];
extern AnimationHeader gSuperLizardObj_AnimTailAttackLeftRight[];
extern AnimationHeader gSuperLizardObj_AnimTailAttackRightLeft[];
extern AnimationHeader gSuperLizardObj_AnimThrustAttack[];
extern AnimationHeader gSuperLizardObj_AnimTongueAttack[];

#endif /* __SUPERLIZARDOBJ_H__ */
