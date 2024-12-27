/*
 * File: ladder.c
 * Overlay: ovl_Ladder
 * Description: Ladder actor from the Hylian Modding actor pack.
 *  * Params: XYZZ
 *      -- X: Model
 *          -- 0: LADDER_SMALL_WOOD
 *          -- 1: LADDER_MEDIUM_WOOD
 *          -- 2: LADDER_LARGE_WOOD
 *          -- 3: LADDER_SMALL_METAL
 *          -- 4: LADDER_MEDIUM_METAL
 *          -- 5: LADDER_LARGE_METAL
 * 
 *          -- Wooden ladders can be hookshotted
 *          -- Small ladders have a climbing distance of 120 units
 *          -- Medium ladders have a climbing distance of 195 units
 *          -- Large ladders have a climbing distance of 270 units
 * 
 *      -- Y: Type
 *          -- 0: LADDER_FADING
 *              -- Fades into existence when switch flag is set, place actor on floor against a wall
 * 
 *          -- 1: LADDER_FALLING
 *              -- Falls down into position when shot with arrow or slingshot, place actor against a wall, then move
 *                 into air desired distance
 * 
 *      -- ZZ: Switch flag (0x00 thru 0x3F)
 */

#include "ladder.h"
#include <uLib.h>

#define FLAGS (ACTOR_FLAG_4 | ACTOR_FLAG_5)

void Ladder_Init(Actor* thisx, PlayState* play);
void Ladder_Destroy(Actor* thisx, PlayState* play);
void Ladder_Update(Actor* thisx, PlayState* play);
void Ladder_Draw(Actor* thisx, PlayState* play);

void Ladder_ClipToGround(Ladder* this, PlayState* play);

void Ladder_IdleInvisible(Ladder* this, PlayState* play);
void Ladder_FadeIn(Ladder* this, PlayState* play);
void Ladder_IdleNotFallen(Ladder* this, PlayState* play);
void Ladder_Falling(Ladder* this, PlayState* play);
void Ladder_DoNothing(Ladder* this, PlayState* play);

const ActorInit Ladder_InitVars = {
    0x023A,
    ACTORCAT_BG,
    FLAGS,
    0x0221,
    sizeof(Ladder),
    (ActorFunc)Ladder_Init,
    (ActorFunc)Ladder_Destroy,
    (ActorFunc)Ladder_Update,
    (ActorFunc)Ladder_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_TREE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { DMG_ARROW | DMG_SLINGSHOT, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON | BUMP_NO_AT_INFO | BUMP_NO_DAMAGE | BUMP_NO_SWORD_SFX | BUMP_NO_HITMARK,
        OCELEM_NONE,
    },
    { 20, 0, 0, { 0, 0, 0 } },
};

#define LADDER_GET_SWITCH_FLAG(this) ((this)->dyna.actor.params & 0x003F)
#define LADDER_GET_MODEL(this) (((this)->dyna.actor.params >> 12) & 0xF)
#define LADDER_GET_TYPE(this) (((this)->dyna.actor.params >> 8) & 0xF)

#define LADDER_FADE_IN_TIMER_MAX (2 * 20)

void Ladder_Init(Actor* thisx, PlayState* play) {
    Ladder* this = (Ladder*)thisx;
    CollisionHeader* colHeader = NULL;
    this->isActivated = Flags_GetSwitch(play, LADDER_GET_SWITCH_FLAG(this));

    Actor_SetScale(&this->dyna.actor, 0.01f);
    this->dyna.actor.gravity = 0.0f;

    this->model = LADDER_GET_MODEL(this);
    this->type = LADDER_GET_TYPE(this);

    this->dyna.actor.colChkInfo.mass = MASS_HEAVY;
    DynaPolyActor_Init(&this->dyna, 0);

    switch (this->model) {
        case LADDER_SMALL_WOOD:
            sCylinderInit.dim.height = 140;
            CollisionHeader_GetVirtual(0x06001290, &colHeader);
            break;
        case LADDER_MEDIUM_WOOD:
            sCylinderInit.dim.height = 210;
            CollisionHeader_GetVirtual(0x06000910, &colHeader);
            break;
        case LADDER_LARGE_WOOD:
            sCylinderInit.dim.height = 290;
            CollisionHeader_GetVirtual(0x06001c00, &colHeader);
            break;
        case LADDER_SMALL_METAL:
            sCylinderInit.dim.height = 140;
            CollisionHeader_GetVirtual(0x06002e40, &colHeader);
            break;
        case LADDER_MEDIUM_METAL:
            sCylinderInit.dim.height = 210;
            CollisionHeader_GetVirtual(0x06002500, &colHeader);
            break;
        case LADDER_LARGE_METAL:
            sCylinderInit.dim.height = 290;
            CollisionHeader_GetVirtual(0x06003770, &colHeader);
            break;
        default:
            break;
    }

    this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &this->dyna.actor, colHeader);

    if (this->isActivated) {
        if (this->type == LADDER_FALLING) {
            Ladder_ClipToGround(this, play);
        }
        this->actionFunc = Ladder_DoNothing;
    } else {
        switch (this->type) {
            case LADDER_FALLING:
                Collider_InitCylinder(play, &this->collider);
                Collider_SetCylinder(play, &this->collider, &this->dyna.actor, &sCylinderInit);
                // Move the ladder forward a bit so that, when falling, it doesn't hit the ledge to climb to
                this->dyna.actor.world.pos.x -= 1.0f;
                this->actionFunc = Ladder_IdleNotFallen;
                break;
            case LADDER_FADING:
                DynaPoly_DisableCollision(play, &play->colCtx.dyna, this->dyna.bgId);
                this->timer = LADDER_FADE_IN_TIMER_MAX;
                this->actionFunc = Ladder_IdleInvisible;
                break;
        }
    }
}

void Ladder_Destroy(Actor* thisx, PlayState* play) {
    Ladder* this = (Ladder*)thisx;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

// HELPERS

void Ladder_ClipToGround(Ladder* this, PlayState* play) {
    f32 newY;
    CollisionPoly* poly;
    s32 bgId;
    Vec3f checkPos;

    checkPos = this->dyna.actor.world.pos;
    checkPos.x -= 1.0f;
    checkPos.y += 1.0f;
    newY = BgCheck_EntityRaycastDown4(&play->colCtx, &poly, &bgId, &this->dyna.actor, &checkPos);
    if (newY != BGCHECK_Y_MIN) {
        this->dyna.actor.world.pos.y = newY;
    }
}

// MAIN

void Ladder_DoNothing(Ladder* this, PlayState* play) {
}

void Ladder_IdleInvisible(Ladder* this, PlayState* play) {
    if (Flags_GetSwitch(play, LADDER_GET_SWITCH_FLAG(this))) {
        // Give some time for the OnePointCutscene to switch its focus to the ladder
        this->timer += 30;
        this->dyna.actor.focus.pos.y += 50.0f;
        // if(play->sceneId == 0x3A){
        //     OnePointCutscene_Attention(play, &this->dyna.actor);
        // }
        this->actionFunc = Ladder_FadeIn;
    }
}

void Ladder_FadeIn(Ladder* this, PlayState* play) {
    if (this->timer <= LADDER_FADE_IN_TIMER_MAX) {
        Audio_PlayActorSfx2(&this->dyna.actor, NA_SE_EV_GOTO_HEAVEN - SFX_FLAG);
        //SoundEffect_PlayHeld(SOUND_MM_LADDER_APPEAR, 1.0f, 1.0f, 1.0f, &this->dyna.actor.world.pos, 0.0f, 1000.0f, &this->dyna.actor);
    }
    if (DECR(this->timer) == 0) {
        this->isActivated = true;
        DynaPoly_EnableCollision(play, &play->colCtx.dyna, this->dyna.bgId);
        this->actionFunc = Ladder_DoNothing;
    }
}

void Ladder_IdleNotFallen(Ladder* this, PlayState* play) {
    if (this->collider.base.acFlags & AC_HIT) {
        this->collider.base.acFlags &= ~AC_HIT;

        Flags_SetSwitch(play, LADDER_GET_SWITCH_FLAG(this));
        this->isActivated = true;

        this->dyna.actor.gravity = -1.0f;
        if (this->model <= LADDER_LARGE_WOOD) {
            Audio_PlayActorSfx2(&this->dyna.actor, NA_SE_EV_WOODPLATE_BOUND);
        } else {
            Audio_PlayActorSfx2(&this->dyna.actor, NA_SE_PL_LAND_HEAVYBOOTS);
        }
        this->actionFunc = Ladder_Falling;
    }
}

#define LADDER_BOUNCE_MAX 2

void Ladder_Falling(Ladder* this, PlayState* play) {
    if (this->bounceCount >= LADDER_BOUNCE_MAX) {
        this->actionFunc = Ladder_DoNothing;
        return;
    }

    if (this->dyna.actor.bgCheckFlags & BGCHECKFLAG_GROUND_TOUCH) {
        if (this->bounceCount == 0) {
            this->dyna.actor.focus.pos = this->dyna.actor.world.pos;
            this->dyna.actor.focus.pos.y += 50.0f;
            //OnePointCutscene_Attention(play, &this->dyna.actor);
        }
        this->bounceCount++;
        this->dyna.actor.velocity.y = 4.0f * (LADDER_BOUNCE_MAX / this->bounceCount);
        if (this->model <= LADDER_LARGE_WOOD) {
            Audio_PlayActorSfx2(&this->dyna.actor, NA_SE_EV_WOODPLATE_BOUND);
        } else {
            Audio_PlayActorSfx2(&this->dyna.actor, NA_SE_PL_LAND_HEAVYBOOTS);
        }
    }
}

void Ladder_Update(Actor* thisx, PlayState* play) {
    Ladder* this = (Ladder*)thisx;

    this->actionFunc(this, play);

    if (this->type == LADDER_FALLING) {
        if (this->isActivated) {
            Actor_MoveForward(&this->dyna.actor);
            Actor_UpdateBgCheckInfo(play, &this->dyna.actor, 0.0f, 0.0f, 0.0f,
                                    UPDBGCHECKINFO_FLAG_2 | UPDBGCHECKINFO_FLAG_3 | UPDBGCHECKINFO_FLAG_4);
        } else {
            Collider_UpdateCylinder(&this->dyna.actor, &this->collider);
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
        }
    }
}

Gfx* Ladder_EmptyDList(GraphicsContext* gfxCtx) {
    Gfx* dListHead;
    Gfx* dList;

    dList = Graph_Alloc(gfxCtx, sizeof(Gfx));

    dListHead = dList;
    gSPEndDisplayList(dListHead++);

    return dList;
}

Gfx* Ladder_SetupTransparent(GraphicsContext* gfxCtx) {
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

Gfx* Ladder_SetupOpaque(GraphicsContext* gfxCtx) {
    Gfx* dList;
    Gfx* dListHead;

    dListHead = Graph_Alloc(gfxCtx, 2 * sizeof(Gfx));

    dList = dListHead;
    gDPSetRenderMode(dListHead++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2);
    gSPEndDisplayList(dListHead++);

    return dList;
}

void Ladder_Draw(Actor* thisx, PlayState* play) {
    Ladder* this = (Ladder*)thisx;
    Gfx* dlist = NULL;

    OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

    switch (this->model) {
        case LADDER_SMALL_WOOD:
            dlist = 0x06001188;
            break;
        case LADDER_MEDIUM_WOOD:
            dlist = 0x06000808;
            break;
        case LADDER_LARGE_WOOD:
            dlist = 0x06001af8;
            break;
        case LADDER_SMALL_METAL:
            dlist = 0x06002d38;
            break;
        case LADDER_MEDIUM_METAL:
            dlist = 0x060023f8;
            break;
        case LADDER_LARGE_METAL:
            dlist = 0x06003668;
            break;
    }

    if (this->type != LADDER_FADING || this->isActivated) {
        gDPPipeSync(POLY_OPA_DISP++);
        Gfx_SetupDL_25Opa(play->state.gfxCtx);
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, 255);
        gSPSegment(POLY_OPA_DISP++, 0x08, Ladder_SetupOpaque(play->state.gfxCtx));
        Gfx_DrawDListOpa(play, dlist);
    } else if (this->timer < LADDER_FADE_IN_TIMER_MAX) {
        s16 alpha = (255 * LADDER_FADE_IN_TIMER_MAX - 255 * this->timer) / LADDER_FADE_IN_TIMER_MAX;

        gDPPipeSync(POLY_XLU_DISP++);
        Gfx_SetupDL_25Xlu(play->state.gfxCtx);
        gSPSegment(POLY_XLU_DISP++, 0x08, Ladder_SetupTransparent(play->state.gfxCtx));
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, alpha);
        Gfx_DrawDListXlu(play, dlist);
    }

    CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
}