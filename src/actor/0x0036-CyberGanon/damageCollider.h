#ifndef DAMAGECOLLIDER_H
#define DAMAGECOLLIDER_H

#include <global.h>

static ColliderJntSphElementInit sdamageColliderElems[] = {
    /* foot.l */ {
        .info.bumperFlags = BUMP_ON,
        
        .dim.limb = CYBERGANONOBJ_LIMB_FOOT_L,
        .dim.modelSphere ={
            { 41, -246, 838 },
            1422,
        },
        .dim.scale = 1,
        .info.elemType = ELEMTYPE_UNK1,
        .info.toucher = { DMG_SLASH, 0x00, 8 },
        .info.toucherFlags = TOUCH_ON | TOUCH_SFX_HARD, //AT
        .info.bumper = { ~(DMG_SHIELD | DMG_MIR_RAY | DMG_HOOKSHOT | DMG_SLINGSHOT | DMG_MAGIC_FIRE), 0x00, 0x00 },
        .info.bumperFlags = BUMP_ON, //AC
        .info.ocElemFlags = OCELEM_ON,  //OC
    },
    /* leg.l */ {
        .info.bumperFlags = BUMP_ON,
        
        .dim.limb = CYBERGANONOBJ_LIMB_LEG_L,
        .dim.modelSphere ={
            { -29, -714, -491 },
            1881,
        },
        .dim.scale = 1,
         .info.elemType = ELEMTYPE_UNK1,
        .info.toucher = { DMG_SLASH, 0x00, 8 },
        .info.toucherFlags = TOUCH_ON | TOUCH_SFX_HARD, //AT
        .info.bumper = { ~(DMG_SHIELD | DMG_MIR_RAY | DMG_HOOKSHOT | DMG_SLINGSHOT | DMG_MAGIC_FIRE), 0x00, 0x00 },
        .info.bumperFlags = BUMP_ON, //AC
        .info.ocElemFlags = OCELEM_ON,  //OC
    },
    /* foot.r */ {
        .info.bumperFlags = BUMP_ON,
        
        .dim.limb = CYBERGANONOBJ_LIMB_FOOT_R,
        .dim.modelSphere ={
            { -22, -236, 837 },
            1422,
        },
        .dim.scale = 1,
         .info.elemType = ELEMTYPE_UNK1,
        .info.toucher = { DMG_SLASH, 0x00, 8 },
        .info.toucherFlags = TOUCH_ON | TOUCH_SFX_HARD, //AT
        .info.bumper = { ~(DMG_SHIELD | DMG_MIR_RAY | DMG_HOOKSHOT | DMG_SLINGSHOT | DMG_MAGIC_FIRE), 0x00, 0x00 },
        .info.bumperFlags = BUMP_ON, //AC
        .info.ocElemFlags = OCELEM_ON,  //OC
    },
    /* leg.r */ {
        .info.bumperFlags = BUMP_ON,
        
        .dim.limb = CYBERGANONOBJ_LIMB_LEG_R,
        .dim.modelSphere ={
            { 69, -714, -472 },
            1881,
        },
        .dim.scale = 1,
         .info.elemType = ELEMTYPE_UNK1,
        .info.toucher = { DMG_SLASH, 0x00, 8 },
        .info.toucherFlags = TOUCH_ON | TOUCH_SFX_HARD, //AT
        .info.bumper = { ~(DMG_SHIELD | DMG_MIR_RAY | DMG_HOOKSHOT | DMG_SLINGSHOT | DMG_MAGIC_FIRE), 0x00, 0x00 },
        .info.bumperFlags = BUMP_ON, //AC
        .info.ocElemFlags = OCELEM_ON,  //OC
    },
    /* head */ {
        .info.bumperFlags = BUMP_ON,
        
        .dim.limb = CYBERGANONOBJ_LIMB_HEAD,
        .dim.modelSphere ={
            { 0, 146, 1351 },
            1590,
        },
        .dim.scale = 1,

         .info.elemType = ELEMTYPE_UNK1,
        .info.bumper = { DMG_EXPLOSIVE, 0x00, 0x00 },
        .info.bumperFlags = BUMP_ON, //AC
        .info.ocElemFlags = OCELEM_ON,  //OC

    },
};

static ColliderJntSphInit sdamageColliderInit = {
    .base.shape   = COLSHAPE_JNTSPH,
    .base.colType = COLTYPE_HIT7,
    .base.atFlags = AT_ON | AT_TYPE_ENEMY,
    .base.acFlags = AC_ON | AC_TYPE_PLAYER,
    .base.ocFlags1 = OC1_ON | OC1_TYPE_PLAYER,
    .base.ocFlags2 = OC2_TYPE_1,
    .count        = ARRAY_COUNT(sdamageColliderElems),
    .elements     = sdamageColliderElems,
};


#endif
