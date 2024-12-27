
#include <uLib.h>
#include "ChangableWater.h"
#include "object/0x019E-ChangableWaterObj.h"


#define FLAGS (ACTOR_FLAG_NO_UPDATE_CULLING | ACTOR_FLAG_5)

void ChangableWater_Init(Actor* thisx, PlayState* play);
void ChangableWater_Destroy(Actor* thisx, PlayState* play);
void ChangableWater_Update(Actor* thisx, PlayState* play);
void ChangableWater_Draw(Actor* thisx, PlayState* play);

void ChangableWater_UpdateWaterHeight(ChangableWater* this, PlayState* play);
void ChangableWater_Wait(ChangableWater* this, PlayState* play);
void ChangableWater_ChangeWaterLevel(ChangableWater* this, PlayState* play);

const ActorInit Bg_Haka_Water_InitVars = {
    0x020A,
    ACTORCAT_PROP,
    FLAGS,
    0x019E,
    sizeof(ChangableWater),
    (ActorFunc)ChangableWater_Init,
    (ActorFunc)ChangableWater_Destroy,
    (ActorFunc)ChangableWater_Update,
    (ActorFunc)ChangableWater_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 500, ICHAIN_STOP), //It determines the scale of water plane
};

void ChangableWater_Init(Actor* thisx, PlayState* play) {
    ChangableWater* this = (ChangableWater*)thisx;
    if(play->sceneId != 0x3B){
        Actor_ProcessInitChain(&this->actor, sInitChain);
    }
    //this->actor.room = -1;
    this->waterBoxOrder = (this->actor.params >> 8) & 0xF; //Variable Form: 0*00
    this->switchFlags = (this->actor.params & 0xFF); //Variable Form: 00**
    this->waterDepth = ((this->actor.params >> 12) & 0xF) * 50.0f; //Variable Form: *000
    this->reverse = this->actor.world.rot.z;
    this->actor.world.rot.z = 0;
    if(this->reverse == false){
        if (Flags_GetSwitch(play, this->switchFlags)) {
            this->isLowered = true;
            this->actor.home.pos.y -= this->waterDepth;
            this->actor.world.pos.y = this->actor.home.pos.y;
        } else {
            this->isLowered = false;
        }
    }else{
        if (!Flags_GetSwitch(play, this->switchFlags)) {
            this->isLowered = true;
            this->actor.home.pos.y -= this->waterDepth;
            this->actor.world.pos.y = this->actor.home.pos.y;
        } else {
            this->isLowered = false;
        }
    }
    ChangableWater_UpdateWaterHeight(this, play);
    
    this->actionFunc = ChangableWater_Wait;
}

void ChangableWater_Destroy(Actor* thisx, PlayState* play) {
}

void ChangableWater_UpdateWaterHeight(ChangableWater* this, PlayState* play) {
        play->colCtx.colHeader->waterBoxes[this->waterBoxOrder].ySurface = (s16)this->actor.world.pos.y + 1.0f;
}

void ChangableWater_Wait(ChangableWater* this, PlayState* play) {
    if(this->reverse == false){
        if ((!this->isLowered && Flags_GetSwitch(play, this->switchFlags)) ||
            (this->isLowered && !Flags_GetSwitch(play, this->switchFlags))) {
            if (this->isLowered) {
                this->isLowered = false;
                this->actor.draw = ChangableWater_Draw;
                this->actor.home.pos.y += this->waterDepth;
            } else {
                this->isLowered = true;
                this->actor.home.pos.y -= this->waterDepth;
            }
            this->actionFunc = ChangableWater_ChangeWaterLevel;
        }
    }else{
        if ((this->isLowered && Flags_GetSwitch(play, this->switchFlags)) ||
            (!this->isLowered && !Flags_GetSwitch(play, this->switchFlags))) {
            if (this->isLowered) {
                this->isLowered = false;
                this->actor.draw = ChangableWater_Draw;
                this->actor.home.pos.y += this->waterDepth;
            } else {
                this->isLowered = true;
                this->actor.home.pos.y -= this->waterDepth;
            }
            this->actionFunc = ChangableWater_ChangeWaterLevel;
        }
    }
}

void ChangableWater_ChangeWaterLevel(ChangableWater* this, PlayState* play) {
    if(this->reverse == false){
        if (!this->isLowered && Flags_GetSwitch(play, this->switchFlags)) {
            this->isLowered = true;
            this->actor.home.pos.y -= this->waterDepth;
        } else if (this->isLowered && !Flags_GetSwitch(play, this->switchFlags)) {
            this->isLowered = false;
            this->actor.home.pos.y += this->waterDepth;
        }
    }else{
        if (!this->isLowered && !Flags_GetSwitch(play, this->switchFlags)) {
            this->isLowered = true;
            this->actor.home.pos.y -= this->waterDepth;
        } else if (this->isLowered && Flags_GetSwitch(play, this->switchFlags)) {
            this->isLowered = false;
            this->actor.home.pos.y += this->waterDepth;
        }
    }
    func_8002F948(&this->actor, NA_SE_EV_WATER_LEVEL_DOWN - SFX_FLAG); //plays SFX.

    //Before checking the condition, execute the Math_StepToF function firstly
    //Math_StepToF would make the current height of this actor reaches the original height smoothly, 
    //and the function returns true when the height reaches, otherwise it returns false.
    if (Math_StepToF(&this->actor.world.pos.y, this->actor.home.pos.y, 0.5f) != 0) {        
        this->actionFunc = ChangableWater_Wait;
    }
    ChangableWater_UpdateWaterHeight(this, play);
}

void ChangableWater_Update(Actor* thisx, PlayState* play) {
    ChangableWater* this = (ChangableWater*)thisx;

    this->actionFunc(this, play);
}

void ChangableWater_Draw(Actor* thisx, PlayState* play) {
    ChangableWater* this = (ChangableWater*)thisx;
    if(play->sceneId == 0x3B){
        Gfx_DrawDListXlu(play, gChangableWater_DlToiletWaterObj); 
    }else{
        OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__),
                    G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gsDPSetPrimColor(0x00, 0x00, 255, 255, 255, 178);
        //gDPSetEnvColor(POLY_XLU_DISP++, bottleColor->r, bottleColor->g, bottleColor->b, 0);
        gSPDisplayList(POLY_XLU_DISP++, gChangableWater_DlWaterObj);
        CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
        // gsDPSetPrimColor(0x00, 0x00, 255, 255, 255, 20);
        // Gfx_DrawDListXlu(play,gChangableWater_DlWaterObj);
    }
}
