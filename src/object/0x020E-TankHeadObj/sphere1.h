#ifndef SPHERE1_H
#define SPHERE1_H

#include <global.h>

static ColliderJntSphElementInit ssphere1Elems[] = {
    /* InnerGun.l */ {
        .info.bumperFlags = BUMP_ON,
        
        .dim.limb = 6,
        .dim.modelSphere ={
            { 0, 0, -34 },
            34,
        },
        .dim.scale = 100,
    },
    /* InnerGun.r */ {
        .info.bumperFlags = BUMP_ON,
        
        .dim.limb = 3,
        .dim.modelSphere ={
            { 0, 0, -34 },
            34,
        },
        .dim.scale = 100,
    },
    /* outGun.l */ {
        .info.bumperFlags = BUMP_ON,
        
        .dim.limb = 7,
        .dim.modelSphere ={
            { 0, 0, -34 },
            34,
        },
        .dim.scale = 100,
    },
    /* outGun.r */ {
        .info.bumperFlags = BUMP_ON,
        
        .dim.limb = 4,
        .dim.modelSphere ={
            { 0, 0, -34 },
            34,
        },
        .dim.scale = 100,
    },
    /* shellEntry.l */ {
        .info.bumperFlags = BUMP_ON,
        
        .dim.limb = 8,
        .dim.modelSphere ={
            { -16, -1, -3 },
            32,
        },
        .dim.scale = 100,
    },
    /* shellEntry.r */ {
        .info.bumperFlags = BUMP_ON,
        
        .dim.limb = 5,
        .dim.modelSphere ={
            { 16, 1, -3 },
            32,
        },
        .dim.scale = 100,
    },
};

static ColliderJntSphInit ssphere1Init = {
    .base.shape   = COLSHAPE_JNTSPH,
    .base.colType = COLTYPE_NONE,
    .base.acFlags = AC_ON,
    
    .count        = ARRAY_COUNT(ssphere1Elems),
    .elements     = ssphere1Elems,
};

#endif
