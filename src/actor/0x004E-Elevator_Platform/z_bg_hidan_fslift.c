/*
 * File: z_bg_hidan_fslift.c
 * Overlay: ovl_Bg_Hidan_Fslift
 * Description: Hookshot Elevator
 */

#include "z_bg_hidan_fslift.h"
#include "assets/objects/object_hidan_objects/object_hidan_objects.h"

#define FLAGS ACTOR_FLAG_4
#define DynaPolyActor_IsPlayerAbove DynaPolyActor_IsPlayerAbove
#define DynaPolyActor_IsPlayerOnTop DynaPolyActor_IsPlayerOnTop

void BgHidanFslift_Init(Actor* thisx, PlayState* play);
void BgHidanFslift_Destroy(Actor* thisx, PlayState* play);
void BgHidanFslift_Update(Actor* thisx, PlayState* play);
void BgHidanFslift_Draw(Actor* thisx, PlayState* play);

void BgHidanFslift_BackhomeJudge(BgHidanFslift* this, PlayState* play);
void BgHidanFslift_Down(BgHidanFslift* this, PlayState* play);
void BgHidanFslift_Up(BgHidanFslift* this, PlayState* play);

const ActorInit Bg_Hidan_Fslift_InitVars = {
    ACTOR_BG_HIDAN_FSLIFT,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_HIDAN_OBJECTS,
    sizeof(BgHidanFslift),
    (ActorFunc)BgHidanFslift_Init,
    (ActorFunc)BgHidanFslift_Destroy,
    (ActorFunc)BgHidanFslift_Update,
    (ActorFunc)BgHidanFslift_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 300, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 350, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 2000, ICHAIN_STOP),
};

void BgHidanFslift_Init(Actor* thisx, PlayState* play) {
    s32 pad1;
    BgHidanFslift* this = (BgHidanFslift*)thisx;
    CollisionHeader* colHeader = NULL;
    s32 pad2;
    this->height = ((thisx->params >> 8) & 0xFF) * 100;
    Actor_ProcessInitChain(&this->dyna.actor, sInitChain);
    DynaPolyActor_Init(&this->dyna, DPM_PLAYER);
    CollisionHeader_GetVirtual(&gFireTempleHookshotElevatorCol, &colHeader);//first: &gFireTempleHookshotElevatorCol
    this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, thisx, colHeader);
    /*if (Actor_SpawnAsChild(&play->actorCtx, &this->dyna.actor, play, ACTOR_OBJ_HSBLOCK, this->dyna.actor.world.pos.x,
                           this->dyna.actor.world.pos.y + 40.0f, this->dyna.actor.world.pos.z + -28.0f, 0, 0, 0,
                           2) == NULL) {
        Actor_Kill(&this->dyna.actor);
        return;
    }*/
    
    this->actionFunc = BgHidanFslift_BackhomeJudge;
}

void BgHidanFslift_ChildPosUpdate(BgHidanFslift* this) {
    if (this->dyna.actor.child != NULL && this->dyna.actor.child->update != NULL) {
        this->dyna.actor.child->world.pos.x = this->dyna.actor.world.pos.x;
        this->dyna.actor.child->world.pos.y = this->dyna.actor.world.pos.y + 40.0f;
        this->dyna.actor.child->world.pos.z = this->dyna.actor.world.pos.z + -28.0f;
    } else {
        this->dyna.actor.child = NULL;
    }
}

void BgHidanFslift_Destroy(Actor* thisx, PlayState* play) {
    BgHidanFslift* this = (BgHidanFslift*)thisx;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

void BgHidanFslift_BackhomeJudgeSetup(BgHidanFslift* this) {
    this->timer = 40;
    this->actionFunc = BgHidanFslift_BackhomeJudge;
}

void BgHidanFslift_BackhomeJudge(BgHidanFslift* this, PlayState* play) {
    s32 backhome;//home poistion check

    if (this->timer) {
        this->timer--;
    }

    if (this->timer == 0) {
        backhome = false;
        if ((this->dyna.actor.world.pos.y - this->dyna.actor.home.pos.y) < 0.5f) {
            backhome = true;
        }
        if (DynaPolyActor_IsPlayerAbove(&this->dyna) && backhome) {
            this->actionFunc = BgHidanFslift_Up;
        } else if (!backhome) {
            this->actionFunc = BgHidanFslift_Down;
        }
    }
}

void BgHidanFslift_Down(BgHidanFslift* this, PlayState* play) {
    if (Math_StepToF(&this->dyna.actor.world.pos.y, this->dyna.actor.home.pos.y, 4.0f)) {
        Audio_PlayActorSfx2(&this->dyna.actor, NA_SE_EV_BLOCK_BOUND);
        BgHidanFslift_BackhomeJudgeSetup(this);
    } else {
        func_8002F974(&this->dyna.actor, NA_SE_EV_ELEVATOR_MOVE3 - SFX_FLAG);
    }
    BgHidanFslift_ChildPosUpdate(this);
}

void BgHidanFslift_Up(BgHidanFslift* this, PlayState* play) {
    if (DynaPolyActor_IsPlayerAbove(&this->dyna)) {
        if (Math_StepToF(&this->dyna.actor.world.pos.y, this->dyna.actor.home.pos.y + this->height, 4.0f)) {
            Audio_PlayActorSfx2(&this->dyna.actor, NA_SE_EV_BLOCK_BOUND);
            BgHidanFslift_BackhomeJudgeSetup(this);
        } else {
            func_8002F974(&this->dyna.actor, NA_SE_EV_ELEVATOR_MOVE3 - SFX_FLAG);
        }
    } else {
        BgHidanFslift_BackhomeJudgeSetup(this);
    }
    BgHidanFslift_ChildPosUpdate(this);
}

void BgHidanFslift_Update(Actor* thisx, PlayState* play) {
    BgHidanFslift* this = (BgHidanFslift*)thisx;

    this->actionFunc(this, play);
    if (DynaPolyActor_IsPlayerOnTop(&this->dyna)) {
        if (this->unk_16A == 0) {
            this->unk_16A = 3;
        }
        //Camera_ChangeSetting(play->cameraPtrs[CAM_ID_MAIN], CAM_SET_FIRE_PLATFORM);
    } else if (!DynaPolyActor_IsPlayerOnTop(&this->dyna)) {
        if (this->unk_16A != 0) {
            //Camera_ChangeSetting(play->cameraPtrs[CAM_ID_MAIN], CAM_SET_DUNGEON0);
        }
        this->unk_16A = 0;
    }
}

void BgHidanFslift_Draw(Actor* thisx, PlayState* play) {
    Gfx_DrawDListOpa(play, gFireTempleHookshotElevatorDL);//gFireTempleHookshotElevatorDL
}
