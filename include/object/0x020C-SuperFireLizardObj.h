#ifndef __SUPERFIRELIZARDOBJ_H__
#define __SUPERFIRELIZARDOBJ_H__

extern u16 gSuperFireLizardObj_TexHeadTongue[];
extern u16 gSuperFireLizardObj_TexTailBackMouth[];
extern u16 gSuperFireLizardObj_TexBack[];
extern u16 gSuperFireLizardObj_TexBelly[];
extern u16 gSuperFireLizardObj_TexTailUnder[];
extern u16 gSuperFireLizardObj_TexEye[];
extern u16 gSuperFireLizardObj_TexArmsLegs[];
extern u16 gSuperFireLizardObj_TexArmor[];
extern u16 gSuperFireLizardObj_TexSpear[];
extern Gfx gSuperFireLizardObj_MtlHead[];
extern Gfx gSuperFireLizardObj_MtlInnerMouth[];
extern Gfx gSuperFireLizardObj_MtlBack[];
extern Gfx gSuperFireLizardObj_MtlBelly[];
extern Gfx gSuperFireLizardObj_MtlTailBack[];
extern Gfx gSuperFireLizardObj_MtlTailUnder[];
extern Gfx gSuperFireLizardObj_MtlEye[];
extern Gfx gSuperFireLizardObj_MtlArmsLegs[];
extern Gfx gSuperFireLizardObj_MtlArmor[];
extern Gfx gSuperFireLizardObj_MtlSpear[];
extern Gfx gSuperFireLizardObj_DlSuperFireLizardObjRoot[];
extern Gfx gSuperFireLizardObj_DlSuperFireLizardObjSpine1[];
extern Gfx gSuperFireLizardObj_DlSuperFireLizardObjSpine2[];
extern Gfx gSuperFireLizardObj_DlSuperFireLizardObjSpine3[];
extern Gfx gSuperFireLizardObj_DlSuperFireLizardObjSpine4[];
extern Gfx gSuperFireLizardObj_DlSuperFireLizardObjNeck[];
extern Gfx gSuperFireLizardObj_DlSuperFireLizardObjHead[];
extern Gfx gSuperFireLizardObj_DlSuperFireLizardObjJaw[];
extern Gfx gSuperFireLizardObj_DlSuperFireLizardObjEyeL[];
extern Gfx gSuperFireLizardObj_DlSuperFireLizardObjEyeR[];
extern Gfx gSuperFireLizardObj_DlSuperFireLizardObjTongue1[];
extern Gfx gSuperFireLizardObj_DlSuperFireLizardObjTongue2[];
extern Gfx gSuperFireLizardObj_DlSuperFireLizardObjTongue3[];
extern Gfx gSuperFireLizardObj_DlSuperFireLizardObjInnerarmL[];
extern Gfx gSuperFireLizardObj_DlSuperFireLizardObjOuterarmL[];
extern Gfx gSuperFireLizardObj_DlSuperFireLizardObjHandL[];
extern Gfx gSuperFireLizardObj_DlSuperFireLizardObjInnerarmR[];
extern Gfx gSuperFireLizardObj_DlSuperFireLizardObjOuterarmR[];
extern Gfx gSuperFireLizardObj_DlSuperFireLizardObjHandR[];
extern Gfx gSuperFireLizardObj_DlSuperFireLizardObjHalberd[];
extern Gfx gSuperFireLizardObj_DlSuperFireLizardObjUpperlegL[];
extern Gfx gSuperFireLizardObj_DlSuperFireLizardObjLowerlegL[];
extern Gfx gSuperFireLizardObj_DlSuperFireLizardObjFootL[];
extern Gfx gSuperFireLizardObj_DlSuperFireLizardObjUpperlegR[];
extern Gfx gSuperFireLizardObj_DlSuperFireLizardObjLowerlegR[];
extern Gfx gSuperFireLizardObj_DlSuperFireLizardObjFootR[];
extern Gfx gSuperFireLizardObj_DlSuperFireLizardObjTail1[];
extern Gfx gSuperFireLizardObj_DlSuperFireLizardObjTail2[];
extern Gfx gSuperFireLizardObj_DlSuperFireLizardObjTail3[];
extern Gfx gSuperFireLizardObj_DlSuperFireLizardObjTail4[];
extern Gfx gSuperFireLizardObj_DlSuperFireLizardObjTail5[];
extern Gfx gSuperFireLizardObj_DlSuperFireLizardObjTail6[];
extern Gfx gSuperFireLizardObj_DlSuperFireLizardObjTail7[];
extern FlexSkeletonHeader gSuperFireLizardObj_SkelSuperFireLizardObj[];

typedef enum {
    SUPERFIRELIZARDOBJ_LIMB_ROOT_TRANSLATE,
    SUPERFIRELIZARDOBJ_LIMB_ROOT,
    SUPERFIRELIZARDOBJ_LIMB_SPINE1,
    SUPERFIRELIZARDOBJ_LIMB_SPINE2,
    SUPERFIRELIZARDOBJ_LIMB_SPINE3,
    SUPERFIRELIZARDOBJ_LIMB_SPINE4,
    SUPERFIRELIZARDOBJ_LIMB_NECK,
    SUPERFIRELIZARDOBJ_LIMB_HEAD,
    SUPERFIRELIZARDOBJ_LIMB_JAW,
    SUPERFIRELIZARDOBJ_LIMB_EYE_L,
    SUPERFIRELIZARDOBJ_LIMB_EYE_R,
    SUPERFIRELIZARDOBJ_LIMB_TONGUE0,
    SUPERFIRELIZARDOBJ_LIMB_ACCORDIAN0,
    SUPERFIRELIZARDOBJ_LIMB_ACCORDIAN1,
    SUPERFIRELIZARDOBJ_LIMB_TONGUE1,
    SUPERFIRELIZARDOBJ_LIMB_ACCORDIAN2,
    SUPERFIRELIZARDOBJ_LIMB_ACCORDIAN3,
    SUPERFIRELIZARDOBJ_LIMB_TONGUE2,
    SUPERFIRELIZARDOBJ_LIMB_ACCORDIAN4,
    SUPERFIRELIZARDOBJ_LIMB_ACCORDIAN5,
    SUPERFIRELIZARDOBJ_LIMB_TONGUE3,
    SUPERFIRELIZARDOBJ_LIMB_INNERARM_L,
    SUPERFIRELIZARDOBJ_LIMB_OUTERARM_L,
    SUPERFIRELIZARDOBJ_LIMB_HAND_L,
    SUPERFIRELIZARDOBJ_LIMB_INNERARM_R,
    SUPERFIRELIZARDOBJ_LIMB_OUTERARM_R,
    SUPERFIRELIZARDOBJ_LIMB_HAND_R,
    SUPERFIRELIZARDOBJ_LIMB_HALBERD,
    SUPERFIRELIZARDOBJ_LIMB_UPPERLEG_L,
    SUPERFIRELIZARDOBJ_LIMB_LOWERLEG_L,
    SUPERFIRELIZARDOBJ_LIMB_FOOT_L,
    SUPERFIRELIZARDOBJ_LIMB_UPPERLEG_R,
    SUPERFIRELIZARDOBJ_LIMB_LOWERLEG_R,
    SUPERFIRELIZARDOBJ_LIMB_FOOT_R,
    SUPERFIRELIZARDOBJ_LIMB_TAIL1,
    SUPERFIRELIZARDOBJ_LIMB_TAIL2,
    SUPERFIRELIZARDOBJ_LIMB_TAIL3,
    SUPERFIRELIZARDOBJ_LIMB_TAIL4,
    SUPERFIRELIZARDOBJ_LIMB_TAIL5,
    SUPERFIRELIZARDOBJ_LIMB_TAIL6,
    SUPERFIRELIZARDOBJ_LIMB_TAIL7,
    SUPERFIRELIZARDOBJ_LIMB_MAX,
} SkelSuperFireLizardObjLimbs;

extern AnimationHeader gSuperFireLizardObj_AnimBlock[];
extern AnimationHeader gSuperFireLizardObj_AnimBreatheAttack[];
extern AnimationHeader gSuperFireLizardObj_AnimCharge[];
extern AnimationHeader gSuperFireLizardObj_AnimDamageTake[];
extern AnimationHeader gSuperFireLizardObj_AnimDeath[];
extern AnimationHeader gSuperFireLizardObj_AnimJumpAttack[];
extern AnimationHeader gSuperFireLizardObj_AnimJumpBack[];
extern AnimationHeader gSuperFireLizardObj_AnimPatrolWalk[];
extern AnimationHeader gSuperFireLizardObj_AnimStandDetection[];
extern AnimationHeader gSuperFireLizardObj_AnimStandAlert[];
extern AnimationHeader gSuperFireLizardObj_AnimStrafe[];
extern AnimationHeader gSuperFireLizardObj_AnimTailAttackLeftRight[];
extern AnimationHeader gSuperFireLizardObj_AnimTailAttackRightLeft[];
extern AnimationHeader gSuperFireLizardObj_AnimThrustAttack[];
extern AnimationHeader gSuperFireLizardObj_AnimTongueAttack[];

#endif /* __SUPERFIRELIZARDOBJ_H__ */
