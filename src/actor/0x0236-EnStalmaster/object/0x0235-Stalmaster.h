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

asm("gStalmaster_TexGloveStart = 0x06000000");
asm("gStalmaster_TexGloveInner = 0x06000800");
asm("gStalmaster_TexPalmOuter = 0x06000C00");
asm("gStalmaster_TexPalmInner = 0x06001400");
asm("gStalmaster_TexSkull = 0x06001C00");
asm("gStalmaster_TexJaw = 0x06002400");
asm("gStalmaster_TexBoneFore = 0x06002C00");
asm("gStalmaster_TexBoneArm = 0x06003400");
asm("gStalmaster_TexShoulder = 0x06003C00");
asm("gStalmaster_TexPins = 0x06004400");
asm("gStalmaster_TexSpine = 0x06004800");
asm("gStalmaster_TexPelvis = 0x06005000");
asm("gStalmaster_TexBoot = 0x06005800");
asm("gStalmaster_TexFoot = 0x06006000");
asm("gStalmaster_TexStalfos4Eye = 0x06006800");
asm("gStalmaster_TexSkirt = 0x06006C00");
asm("gStalmaster_TexBelt = 0x06007400");
asm("gStalmaster_TexHelmet = 0x06007C00");
asm("gStalmaster_TexHelmetSide = 0x06008400");
asm("gStalmaster_TexChest = 0x06008C00");
asm("gStalmaster_TexBack = 0x06009400");
asm("gStalmaster_TexSworda = 0x06009C00");
asm("gStalmaster_TexHilt = 0x0600A400");
asm("gStalmaster_TexSkullHandle = 0x0600A600");
asm("gStalmaster_TexSwordb = 0x0600AA00");
asm("gStalmaster_TexSwordc = 0x0600B200");
asm("gStalmaster_TexSwordd = 0x0600BA00");
asm("gStalmaster_MtlGloveStart = 0x0600E8B0");
asm("gStalmaster_MtlGloveInner = 0x0600E968");
asm("gStalmaster_MtlPalmOuter = 0x0600EA20");
asm("gStalmaster_MtlPalmInner = 0x0600EAD8");
asm("gStalmaster_MtlSkull = 0x0600EB90");
asm("gStalmaster_MtlJaw = 0x0600EC48");
asm("gStalmaster_MtlBoneFore = 0x0600ED00");
asm("gStalmaster_MtlBoneArm = 0x0600EDB8");
asm("gStalmaster_MtlShoulder = 0x0600EE70");
asm("gStalmaster_MtlPins = 0x0600EF28");
asm("gStalmaster_MtlSpine = 0x0600EFE0");
asm("gStalmaster_MtlPelvis = 0x0600F098");
asm("gStalmaster_MtlBoot = 0x0600F150");
asm("gStalmaster_MtlFoot = 0x0600F208");
asm("gStalmaster_MtlEye = 0x0600F2C0");
asm("gStalmaster_MtlSkirt = 0x0600F378");
asm("gStalmaster_MtlBelt = 0x0600F430");
asm("gStalmaster_MtlHelmet = 0x0600F4E8");
asm("gStalmaster_MtlHelmetSide = 0x0600F5A0");
asm("gStalmaster_MtlChest = 0x0600F658");
asm("gStalmaster_MtlBack = 0x0600F710");
asm("gStalmaster_MtlSworda = 0x0600F7C8");
asm("gStalmaster_MtlHilt = 0x0600F880");
asm("gStalmaster_MtlSkullHandle = 0x0600F938");
asm("gStalmaster_MtlSwordb = 0x0600F9F0");
asm("gStalmaster_MtlSwordc = 0x0600FAA8");
asm("gStalmaster_MtlSwordd = 0x0600FB60");
asm("gStalmaster_DlStalmasterBackbone01 = 0x0600FD68");
asm("gStalmaster_DlStalmasterBackbone02 = 0x06012D10");
asm("gStalmaster_DlStalmasterArmLD01 = 0x06013A20");
asm("gStalmaster_DlStalmasterArmLD02 = 0x06013D30");
asm("gStalmaster_DlStalmasterHandLD = 0x060140B0");
asm("gStalmaster_DlStalmasterFingerLD01 = 0x06014520");
asm("gStalmaster_DlStalmasterFingerLD02 = 0x060147E0");
asm("gStalmaster_DlStalmasterPadLD = 0x06014B68");
asm("gStalmaster_DlStalmasterArmLU01 = 0x06014D40");
asm("gStalmaster_DlStalmasterArmLU02 = 0x06015050");
asm("gStalmaster_DlStalmasterHandLU = 0x06015510");
asm("gStalmaster_DlStalmasterFingerLU01 = 0x060158D8");
asm("gStalmaster_DlStalmasterFingerLU02 = 0x06015B60");
asm("gStalmaster_DlStalmasterPadLU = 0x06015EE8");
asm("gStalmaster_DlStalmasterArmRD01 = 0x060160C0");
asm("gStalmaster_DlStalmasterArmRD02 = 0x060163D0");
asm("gStalmaster_DlStalmasterHandRD = 0x06016860");
asm("gStalmaster_DlStalmasterFingerRD01 = 0x06016CA8");
asm("gStalmaster_DlStalmasterFingerRD02 = 0x06016F70");
asm("gStalmaster_DlStalmasterPadRD = 0x060172F8");
asm("gStalmaster_DlStalmasterArmRU01 = 0x060174D0");
asm("gStalmaster_DlStalmasterArmRU02 = 0x060177E0");
asm("gStalmaster_DlStalmasterHandRU = 0x06017C70");
asm("gStalmaster_DlStalmasterFingerRU01 = 0x060180B8");
asm("gStalmaster_DlStalmasterFingerRU02 = 0x06018380");
asm("gStalmaster_DlStalmasterPadRU = 0x06018708");
asm("gStalmaster_DlStalmasterNeck = 0x06018880");
asm("gStalmaster_DlStalmasterHead = 0x0601A460");
asm("gStalmaster_DlStalmasterChin = 0x0601B040");
asm("gStalmaster_DlStalmasterWaist = 0x0601C508");
asm("gStalmaster_DlStalmasterLegL01 = 0x0601CD78");
asm("gStalmaster_DlStalmasterLegL02 = 0x0601D440");
asm("gStalmaster_DlStalmasterFootL = 0x0601D800");
asm("gStalmaster_DlStalmasterLegR01 = 0x0601DD48");
asm("gStalmaster_DlStalmasterLegR02 = 0x0601E438");
asm("gStalmaster_DlStalmasterFootR = 0x0601E800");
asm("gStalmaster_SkelStalmaster = 0x0601EC08");
asm("gStalmaster_DlHelmet = 0x0601F2A8");
asm("gStalmaster_DlChestplate = 0x06020380");
asm("gStalmaster_DlTip = 0x06020868");
asm("gStalmaster_DlTail = 0x06020958");
asm("gStalmaster_DlSwordA = 0x060220E0");
asm("gStalmaster_DlSwordB = 0x06023190");
asm("gStalmaster_DlSwordC = 0x060242C8");
asm("gStalmaster_DlSwordD = 0x06025D88");
asm("gStalmaster_DlSwordC2 = 0x06026F18");
asm("gStalmaster_DlSwordD2 = 0x060289F8");
asm("gStalmaster_DlArmR = 0x06028DF8");
asm("gStalmaster_DlPadR = 0x060290F8");
asm("gStalmaster_DlForearmR = 0x06029980");
asm("gStalmaster_DlHead = 0x0602BE50");
asm("gStalmaster_DlBackbone = 0x0602F300");
asm("gStalmaster_DlArmL = 0x0602FFC8");
asm("gStalmaster_DlPadL = 0x060302C8");
asm("gStalmaster_DlForearmL = 0x06030B50");
asm("gStalmaster_Anim2Counter = 0x06031D44");
asm("gStalmaster_Anim2Guard = 0x06032490");
asm("gStalmaster_Anim2GuardJust = 0x06033F64");
asm("gStalmaster_Anim2Walk = 0x0603509C");
asm("gStalmaster_Anim4Counter = 0x060361B4");
asm("gStalmaster_Anim4Guard = 0x0603692C");
asm("gStalmaster_Anim4GuardJust = 0x0603929C");
asm("gStalmaster_Anim4PoseC = 0x0603BFF0");
asm("gStalmaster_Anim4Walk = 0x0603D314");
asm("gStalmaster_AnimAttackA = 0x0603F804");
asm("gStalmaster_AnimAttackB = 0x06040750");
asm("gStalmaster_AnimAttackC = 0x06044E70");
asm("gStalmaster_AnimAttackD = 0x06046CDC");
asm("gStalmaster_AnimEnd = 0x0604B798");
asm("gStalmaster_AnimOp = 0x06054710");
asm("gStalmaster_AnimSwordPose = 0x06056F7C");

#endif /* __STALMASTER_H__ */
