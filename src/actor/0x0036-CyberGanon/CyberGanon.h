#ifndef __CYBER_GANON_H__
#define __CYBER_GANON_H__

#include <uLib.h>
#include "object/0x0011-CyberGanonObj.h"
#include "overlays/actors/ovl_En_Bom/z_en_bom.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h"
#include "damageCollider.h"
#include "bodyArmorCollider.h"

struct CyberGanon;

typedef void (* CyberGanonFunc)(struct CyberGanon*, PlayState*);



typedef struct CyberGanon {
    Actor actor;
    SkelAnime skelAnime;
    Vec3s jointTable[CYBERGANONOBJ_LIMB_MAX];
    Vec3s morphTable[CYBERGANONOBJ_LIMB_MAX];
    CyberGanonFunc actionFunc;
    ColliderQuad clubCollider;
    ColliderJntSph bodyArmorCollider;
    ColliderJntSphElement bodyArmorColliderItems[ARRAY_COUNT(sbodyArmorColliderElems)];
    ColliderJntSph damageCollider;
    ColliderJntSphElement damageColliderItems[ARRAY_COUNT(sdamageColliderElems)];
    Vec3f feetPos[2];
    Vec3s bodyPartsPos[10];
    Vec3f outterArmLPos;
    Vec3f outterArmRPos;
    Vec3f cannonGunPos;
    Vec3f targetGunPos;
    Vec3f tailVentPos;
    Vec3f headPos;
    Vec3f chestPos;
    Vec3f cannonGunRot;
    Vec3f outterClubPos;
    s16 clubAlpha;
    s16 scrollX;
    s16 scrollY;
    f32 walkSpeed;
    s16 fallDownTimer;
    s16 feetEffectTimer[2];
    WeaponInfo clubInfo;
    s16 headHitcCounter;
    s8 clubAttack;
    s8 lastDamageEffect;
    s8 cannonMeeleAttack;
    s8 headDamageable;
    s8 hasClub;
    s8 clubMeeleAttack;
    s8 hasGrabPlayer;
    s8 leftHandBlur;
    s8 upwardMode;
    s8 shouldBackHome;
    PosRot grabPosRot;
    s16 slowTurnTimer;
    s16 missileCoolTimer;
    s32 effectIndex;
    s16 actionTimer;
    s16 eyeTexIndex;
    s16 blinkTimer;
    s16 pitch;
    s16 failToHitCount;
    Color_RGBA8 bodyPrimColor;
    Color_RGBA8 smokePrimColor;
    Color_RGBA8 smokeEnvColor;
    Color_RGBA8 smokePrimColor2;
    Color_RGBA8 smokeEnvColor2;
} CyberGanon;

#endif // __CYBER_GANON_H__