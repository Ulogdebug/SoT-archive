/*
 * File: z_platforms.c
 * Overlay: ovl_Platforms
 * Description: Various floating platforms, from the Hylian Modding actor pack.
 */
#include <uLib.h>
#include "platforms.h"
#include "object/0x0222-TimingPlatformsObj.h"

#define PLATFORM_TYPE(this) (this->dyna.actor.params >> 4 & 0xF)  // 0x00F0
#define SWITCH_FLAG(this) ((this->dyna.actor.params >> 8) & 0xFF) // 0xFF00
#define INVISIBILITY(this) (this->dyna.actor.params & 0xF)         //0x000F
#define TIMER_DURATION(this) (this->dyna.actor.home.rot.x)        // <= 10 minutes (600)

/**
 * Params:
 *  0xF000 = Type
 *     0x0 = Stone Sqaure Platform
 *     0x1 = Demon Sqaure Platform
 *     0x2 = Demon Platform (No timer)
 *
 *  0xFF00 = Switch Flag
 *
 *  Rot X > 0 = Duration timer for spawned platforms (in seconds)
 *
 *  All platform types (except for the Cone Floating Type) will spawn on switch flag activation. After the specified
 * timer amount, they unset the switch and disappear again until the flag is once again set.
 *
 *  Default (Rot X = 0) timer. You can go up to 10 minutes (600).
 *
 */

#define FLAGS (ACTOR_FLAG_4)

#define SECONDS_TO_FRAMES(sec) (sec * (60 / R_UPDATE_RATE))
static s16 sTimer = 0;

void Platforms_Init(Actor* thisx, PlayState* play);
void Platforms_Destroy(Actor* thisx, PlayState* play);
void Platforms_Update(Actor* thisx, PlayState* play);
void Platforms_Draw(Actor* thisx, PlayState* play);

void Platforms_SetupWaitForSwitch(Platforms* this, PlayState* play);
void Platforms_WaitForSwitch(Platforms* this, PlayState* play);
void Platforms_SetupAppear(Platforms* this, PlayState* play);
void Platforms_Appear(Platforms* this, PlayState* play);
void Platforms_SetupDisappear(Platforms* this, PlayState* play);
void Platforms_Disappear(Platforms* this, PlayState* play);

void Platforms_SetupConeFloat(Platforms* this, PlayState* play);
void Platforms_ConeFloat(Platforms* this, PlayState* play);

void Platforms_DoNothing(Platforms* this, PlayState* play);

typedef enum {
    PLATFORM_TYPE_STONE = 0,
    PLATFORM_TYPE_DEMON = 1,
    PLATFORM_TYPE_DEMON_NO_TIMER = 2,
} PlatformType;

const ActorInit Platforms_InitVars = {
    0x023B,
    ACTORCAT_BG,
    FLAGS,
    0x0222,
    sizeof(Platforms),
    (ActorFunc)Platforms_Init,
    (ActorFunc)Platforms_Destroy,
    (ActorFunc)Platforms_Update,
    (ActorFunc)Platforms_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneForward, 3000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 500, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1000, ICHAIN_STOP),
};

void Platforms_Init(Actor* thisx, PlayState* play) {
    Platforms* this = (Platforms*)thisx;
    CollisionHeader* colHeader = NULL;

    Actor_ProcessInitChain(thisx, sInitChain);
    DynaPolyActor_Init(&this->dyna, (DPM_UNK3));

    switch (PLATFORM_TYPE(this)) {
        case PLATFORM_TYPE_STONE:
        case PLATFORM_TYPE_DEMON:
            CollisionHeader_GetVirtual(gTimingPlatformObj_CollTimingPlatformMetalObj, &colHeader);
            break;
        case PLATFORM_TYPE_DEMON_NO_TIMER:
            CollisionHeader_GetVirtual(gTimingPlatformObj_CollTimingPlatformDemonlObj, &colHeader);
            break;
    }

    this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &this->dyna.actor, colHeader);

    this->preTimerDuration = (TIMER_DURATION(this) == 0) ? 0 : CLAMP_MAX(TIMER_DURATION(this), 600);
    sTimer = this->timer = SECONDS_TO_FRAMES(this->preTimerDuration); // Clamped to max of 10 minutes, default of 10 seconds
    

    this->dyna.actor.shape.rot.x = this->dyna.actor.home.rot.x = 0.0f;
    this->dyna.actor.shape.rot.z = this->dyna.actor.home.rot.z = 0.0f;

    //Actor_SetScale(&this->dyna.actor, 0.035f);
    if(INVISIBILITY(this)){
        thisx->flags |= ACTOR_FLAG_7;
    }
    if (PLATFORM_TYPE(this) != PLATFORM_TYPE_DEMON_NO_TIMER) {
        Platforms_SetupWaitForSwitch(this, play);
    } else {
        Platforms_SetupConeFloat(this, play); //No switch, no timer
    }
}

void Platforms_Destroy(Actor* thisx, PlayState* play) {
    Platforms* this = (Platforms*)thisx;

    if (PLATFORM_TYPE(this) != PLATFORM_TYPE_DEMON_NO_TIMER && this->preTimerDuration != 0) {
        Flags_UnsetSwitch(play, SWITCH_FLAG(this));
    }

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

void Platforms_Update(Actor* thisx, PlayState* play) {
    Platforms* this = (Platforms*)thisx;

    if (PLATFORM_TYPE(this) == PLATFORM_TYPE_DEMON_NO_TIMER) {
        this->dyna.actor.world.rot.y = this->dyna.actor.shape.rot.y += 0x50; // slowly rotate
    }

    Math_SmoothStepToS(&this->alpha, this->alphaTarget, 1, 24, 1);

    this->actionFunc(this, play);
}

Gfx* Platforms_SetupTransparent(GraphicsContext* gfxCtx) {
    Gfx* dList;
    Gfx* dListHead;

    dListHead = Graph_Alloc(gfxCtx, 2 * sizeof(Gfx));

    dList = dListHead;
    gDPSetRenderMode(dListHead++,
                     AA_EN | Z_CMP | Z_UPD | IM_RD | CLR_ON_CVG | CVG_DST_WRAP | ZMODE_XLU | FORCE_BL |
                         GBL_c1(G_BL_CLR_FOG, G_BL_A_SHADE, G_BL_CLR_IN, G_BL_1MA),
                     AA_EN | Z_CMP | Z_UPD | IM_RD | CLR_ON_CVG | CVG_DST_WRAP | ZMODE_XLU | FORCE_BL |
                         GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA));
    gSPEndDisplayList(dListHead++);

    return dList;
}

Gfx* Platforms_SetupOpaque(GraphicsContext* gfxCtx) {
    Gfx* dList;
    Gfx* dListHead;

    dListHead = Graph_Alloc(gfxCtx, 2 * sizeof(Gfx));

    dList = dListHead;
    gDPSetRenderMode(dListHead++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2);
    gSPEndDisplayList(dListHead++);

    return dList;
}

void Platforms_Draw(Actor* thisx, PlayState* play) {
    Platforms* this = (Platforms*)thisx;
    Gfx* dlist = NULL;
    void (*drawFunc)(PlayState*, Gfx*);
    
    if (PLATFORM_TYPE(this) == PLATFORM_TYPE_DEMON_NO_TIMER){
        
        if (INVISIBILITY(this)) {
            Gfx_DrawDListXlu(play, gTimingPlatformObj_DlTimingPlatformDemonNoTimerObj);
            Gfx_DrawDListXlu(play, gTimingPlatformObj_DlLensOfTruthMark);
        }else {
            Gfx_DrawDListOpa(play, gTimingPlatformObj_DlTimingPlatformDemonNoTimerObj);
        }
    }

    OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

    
    if (this->alpha == 255 && !INVISIBILITY(this)) {
        gDPPipeSync(POLY_OPA_DISP++);
        Gfx_SetupDL_25Opa(play->state.gfxCtx);
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, 255);
        gSPSegment(POLY_OPA_DISP++, 0x08, Platforms_SetupOpaque(play->state.gfxCtx));
        drawFunc = Gfx_DrawDListOpa;
        // Gfx_DrawDListOpa(play, dlist);
    } else {
        gDPPipeSync(POLY_XLU_DISP++);
        Gfx_SetupDL_25Xlu(play->state.gfxCtx);
        gSPSegment(POLY_XLU_DISP++, 0x08, Platforms_SetupTransparent(play->state.gfxCtx));
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, this->alpha);
        drawFunc = Gfx_DrawDListXlu;
        // Gfx_DrawDListXlu(play, dlist);
    }

    if (this->alpha > 0) {
        if (INVISIBILITY(this)) {
            drawFunc(play, gTimingPlatformObj_DlLensOfTruthMark);
        }
        switch (PLATFORM_TYPE(this)) {
            case PLATFORM_TYPE_STONE:
                drawFunc(play, gTimingPlatformObj_DlTimingPlatformStoneObj);
                break;
            case PLATFORM_TYPE_DEMON:
                drawFunc(play, gTimingPlatformObj_DlTimingPlatformDemonObj);
                break;
        }
    }

    CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
}

void Platforms_SetupWaitForSwitch(Platforms* this, PlayState* play) {
    DynaPoly_DisableCollision(play, &play->colCtx.dyna, this->dyna.bgId);
    this->actionFunc = Platforms_WaitForSwitch;
}

void Platforms_WaitForSwitch(Platforms* this, PlayState* play) {
    if (Flags_GetSwitch(play, SWITCH_FLAG(this))) {
        Platforms_SetupAppear(this, play);
    }
}

void Platforms_SetupAppear(Platforms* this, PlayState* play) {
    DynaPoly_EnableCollision(play, &play->colCtx.dyna, this->dyna.bgId);
    this->timer = sTimer;
    this->alphaTarget = 255;
    this->actionFunc = Platforms_Appear;
}

void Platforms_Appear(Platforms* this, PlayState* play) {
    if(this->preTimerDuration != 0 ){
        func_8002F994(&this->dyna.actor, this->timer);
        if (DECR(this->timer) == 0) {
            Flags_UnsetSwitch(play, SWITCH_FLAG(this));
            Platforms_SetupWaitForSwitch(this, play);
        } else if (this->timer == 10) {
            this->alphaTarget = 0;
        }
    }else{
        if(!Flags_GetSwitch(play, SWITCH_FLAG(this))){
            this->alphaTarget = 0;
            this->actionFunc = Platforms_SetupWaitForSwitch;
        }
    }
}

void Platforms_SetupConeFloat(Platforms* this, PlayState* play) {
    this->actionFunc = Platforms_ConeFloat;
}

void Platforms_ConeFloat(Platforms* this, PlayState* play) {
    f32 dipTarget = 0.0f;

    if (DynaPolyActor_IsPlayerOnTop(&this->dyna)) {
        dipTarget = -35.0f;
    }

    Math_SmoothStepToF(&this->dipOffset, dipTarget, 0.1f, 2.0f, 0.1f);
    this->dyna.actor.world.pos.y =
        (this->dyna.actor.home.pos.y + this->dipOffset) + 20.0f * (Math_SinS(play->gameplayFrames * 0x200)); // float up and down
}

void Platforms_DoNothing(Platforms* this, PlayState* play) {
}