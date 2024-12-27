#ifndef BODYARMORCOLLIDER_H
#define BODYARMORCOLLIDER_H

#include <global.h>

static ColliderJntSphElementInit sbodyArmorColliderElems[] = {
    /* chest */ {
        
        .dim.limb = CYBERGANONOBJ_LIMB_CHEST,
        .dim.modelSphere ={
            { 0, 835, -6 },
            2072,
        },
        .dim.scale = 1,

        .info.elemType = ELEMTYPE_UNK1,
        .info.bumper = { 0xFFCFFFFF, 0x00, 0x00 },
        .info.bumperFlags = BUMP_ON, //AC
        .info.ocElemFlags = OCELEM_ON,  //OC
    },
    /* crotch */ {
        
        .dim.limb = CYBERGANONOBJ_LIMB_CROTCH,
        .dim.modelSphere ={
            { 0, 900, 0 },
            2306,
        },
        .dim.scale = 1,

        .info.elemType = ELEMTYPE_UNK1,
        .info.bumper = { 0xFFCFFFFF, 0x00, 0x00 },
        .info.bumperFlags = BUMP_ON, //AC
        .info.ocElemFlags = OCELEM_ON,  //OC

    },
    /* innerarm.l */ {
        
        .dim.limb = CYBERGANONOBJ_LIMB_INNERARM_L,
        .dim.modelSphere ={
            { 2013, 116, -575 },
            2097,
        },
        .dim.scale = 1,

        .info.elemType = ELEMTYPE_UNK1,
        .info.bumper = { 0xFFCFFFFF, 0x00, 0x00 },
        .info.bumperFlags = BUMP_ON, //AC
        .info.ocElemFlags = OCELEM_ON,  //OC
    },
    /* innerarm.r */ {
        
        .dim.limb = CYBERGANONOBJ_LIMB_INNERARM_R,
        .dim.modelSphere ={
            { -2283, 27, -429 },
            2324,
        },
        .dim.scale = 1,
        .info.elemType = ELEMTYPE_UNK1,
        .info.bumper = { 0xFFCFFFFF, 0x00, 0x00 },
        .info.bumperFlags = BUMP_ON, //AC
        .info.ocElemFlags = OCELEM_ON,  //OC
    },
    /* shin.l */ {
        
        .dim.limb = CYBERGANONOBJ_LIMB_SHIN_L,
        .dim.modelSphere ={
            { 253, -1167, 460 },
            1491,
        },
        .dim.scale = 1,
        .info.elemType = ELEMTYPE_UNK1,
        .info.bumper = { 0xFFCFFFFF, 0x00, 0x00 },
        .info.bumperFlags = BUMP_ON, //AC
        .info.ocElemFlags = OCELEM_ON,  //OC
    },
    /* shin.r */ {
        
        .dim.limb = CYBERGANONOBJ_LIMB_SHIN_R,
        .dim.modelSphere ={
            { -284, -1166, 445 },
            1491,
        },
        .dim.scale = 1,
        .info.elemType = ELEMTYPE_UNK1,
        .info.bumper = { 0xFFCFFFFF, 0x00, 0x00 },
        .info.bumperFlags = BUMP_ON, //AC
        .info.ocElemFlags = OCELEM_ON,  //OC
    },
    /* waist */ {
        
        .dim.limb = CYBERGANONOBJ_LIMB_WAIST,
        .dim.modelSphere ={
            { 0, 1819, 496 },
            2254,
        },
        .dim.scale = 1,
        .info.elemType = ELEMTYPE_UNK1,
        .info.bumper = { 0xFFCFFFFF, 0x00, 0x00 },
        .info.bumperFlags = BUMP_ON, //AC
        .info.ocElemFlags = OCELEM_ON,  //OC
    },
    /* outterarm.r */ {

        .dim.limb = CYBERGANONOBJ_LIMB_OUTTERARM_R,
        .dim.modelSphere ={
            { -3439, 0, 572 },
            2914,
        },
        .dim.scale = 1,

        .info.toucher = { 0xFFCFFFFF, 0x00, 0x10 },
        .info.toucherFlags = TOUCH_ON | TOUCH_SFX_NORMAL, //AT
        .info.elemType = ELEMTYPE_UNK1,
        .info.bumper = { 0xFFCFFFFF, 0x00, 0x00 },
        .info.bumperFlags = BUMP_ON, //AC
        .info.ocElemFlags = OCELEM_ON,  //OC
    },

    /* hand.l */ {
        
        .dim.limb = CYBERGANONOBJ_LIMB_HAND_L,
        .dim.modelSphere ={
            { 1853, 0, -91 },
            1855,
        },
        .dim.scale = 1,

        .info.toucher = { DMG_UNBLOCKABLE, 0x00, 28 },
        .info.toucherFlags = TOUCH_ON | TOUCH_SFX_NORMAL, //AT
         .info.elemType = ELEMTYPE_UNK1,
        .info.bumper = { 0xFFCFFFFF, 0x00, 0x00 },
        .info.bumperFlags = BUMP_ON, //AC
        .info.ocElemFlags = OCELEM_ON,  //OC
    },
    /* outterarm.l */ {
        
        .dim.limb = CYBERGANONOBJ_LIMB_OUTTERARM_L,
        .dim.modelSphere ={
            { 1386, 108, 132 },
            1396,
        },
        .dim.scale = 1,

        .info.toucher = { DMG_UNBLOCKABLE, 0x00, 0x16 },
        .info.toucherFlags = TOUCH_ON | TOUCH_SFX_NORMAL, //AT
         .info.elemType = ELEMTYPE_UNK1,
        .info.bumper = { 0xFFCFFFFF, 0x00, 0x00 },
        .info.bumperFlags = BUMP_ON, //AC
        .info.ocElemFlags = OCELEM_ON,  //OC
    },
};

static ColliderJntSphInit sbodyArmorColliderInit = {
    .base.shape   = COLSHAPE_JNTSPH,
    .base.colType = COLTYPE_METAL,
    .base.atFlags = AT_ON | AT_TYPE_ENEMY,
    .base.acFlags = AC_ON | AC_TYPE_PLAYER,
    .base.ocFlags1 = OC1_ON | OC1_TYPE_PLAYER,
    .base.ocFlags2 = OC2_TYPE_1,
    
    .count        = ARRAY_COUNT(sbodyArmorColliderElems),
    .elements     = sbodyArmorColliderElems,
};

#endif
