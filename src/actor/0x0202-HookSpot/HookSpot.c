#include "HookSpot.h"
#include "object/0x0199-HookSpotObj.h"
#define FLAGS (ACTOR_FLAG_0 | ACTOR_FLAG_10)


void HookSpot_Init(Actor* thisx, PlayState* play);
void HookSpot_HasSwitch_CollisionInit(Actor* thisx, PlayState* play);
void HookSpot_HasSwitch_CheckCollision(HookSpot* this, PlayState* play);
void HookSpot_NoSwitch_Idle(HookSpot* this, PlayState* play);
void HookSpot_Destroy(HookSpot* this, PlayState* play);
void HookSpot_Update(HookSpot* this, PlayState* play);
void HookSpot_Draw(HookSpot* this, PlayState* play);

const ActorInit sHookSpot_InitVars = {
    .id = 0x0202,
    .category     = ACTORCAT_PROP,
    .flags        = FLAGS,
    .objectId = 0x0199,
    .instanceSize = sizeof(HookSpot),
    .init         = (ActorFunc)HookSpot_Init,
    .destroy      = (ActorFunc)HookSpot_Destroy,
    .update       = (ActorFunc)HookSpot_Update,
    .draw         = (ActorFunc)HookSpot_Draw
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_METAL,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_NO_PUSH,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON | BUMP_HOOKABLE,
        OCELEM_ON,
    },
    { 22, 8, 0, {  0, 0, 0  } },//height 25
};



static CollisionCheckInfoInit sColChkInfoInit = { 0, 12, 60, MASS_IMMOVABLE };


void HookSpot_Init(Actor* thisx, PlayState* play) {
    HookSpot* this = (HookSpot*)thisx;
    CollisionHeader* colHeader = NULL;
    s32 Type = (this->actor.params & 7); //Variable Form : 000* (hex)
    if(this->actor.world.rot.x != 0x0 ){
        this->actor.focus.pos.y = this->actor.world.pos.y + 15.0f;
    }
    else{
            this->actor.focus.pos.y = this->actor.world.pos.y - 5.0f;
    }
    if(Type == HAS_SWITCH){
        HookSpot_HasSwitch_CollisionInit(thisx,play);
        CollisionCheck_SetInfo(&thisx->colChkInfo, NULL, &sColChkInfoInit);

        this->switchFlags = (this->actor.params >> 8) & 0xFF; //Variable Form : **00 (hex)
        if(play->sceneId == 0x2A || (play->sceneId == 0x04 && play->roomCtx.curRoom.num == 0)){
            this->actor.targetMode = 5;
        }        
        this->actionFunc = HookSpot_HasSwitch_CheckCollision;
    }
    else if(Type == NO_SWITCH){
        DynaPolyActor_Init(&this->dyna, DPM_PLAYER);
        CollisionHeader_GetVirtual(&gHookSpotObj_CollHookSpotObj, &colHeader);
        this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, thisx, colHeader);
        this->actionFunc = HookSpot_NoSwitch_Idle;
        this->actor.focus.pos.y = this->actor.world.pos.y + 5.0f;

    }
    else{
        Actor_Kill(&this->actor);
        return;
    }
    
}

void HookSpot_HasSwitch_CollisionInit(Actor* thisx, PlayState* play) {
    HookSpot* this = (HookSpot*)thisx;
    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
    Collider_UpdateCylinder(&this->actor, &this->collider); 
     if(this->actor.world.rot.x != 0x0 ){
        this->collider.dim.height = 40.0f;  
        this->collider.dim.pos.y = this->actor.world.pos.y - 20.0f;  
    }
    else{
    this->collider.dim.pos.y = this->actor.world.pos.y - 5.0f;  
    }
}

void HookSpot_HasSwitch_CheckCollision(HookSpot* this, PlayState* play){

    if ((this->collider.base.acFlags & AC_HIT) && (this->collider.info.acHitInfo->toucher.dmgFlags & DMG_HOOKSHOT)){

            if( (this->switchFlags != 0X3F) && (!Flags_GetSwitch(play, this->switchFlags)) ){
            Flags_SetSwitch(play, this->switchFlags);
            Audio_PlayActorSfx2(&this->actor, NA_SE_EV_DIAMOND_SWITCH);
            }
            else if( (this->switchFlags != 0X3F) && (Flags_GetSwitch(play, this->switchFlags)) ){
            Flags_UnsetSwitch(play, this->switchFlags);
            SfxSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 80, NA_SE_SY_FSEL_ERROR);
            }
            this->collider.base.acFlags &= ~AC_HIT;

     } else {
        this->collider.base.acFlags &= ~AC_HIT;
        if (this->actor.xzDistToPlayer < 600.0f) {
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
            CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
        }
    }

}

void HookSpot_NoSwitch_Idle(HookSpot* this, PlayState* play){


}


void HookSpot_Destroy(HookSpot* this, PlayState* play) {

    if((this->actor.params & 7) == HAS_SWITCH){
        Collider_DestroyCylinder(play, &this->collider);
    }
    else{
        DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
    }


}

void HookSpot_Update(HookSpot* this, PlayState* play) {
    this->actionFunc(this, play);

    
}



s32 HookSpot_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3f* pos, Vec3s* rot, void* thisx) {
    HookSpot* this = (void*)thisx;
    
    return 0;
}

void HookSpot_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3s* rot, void* thisx) {
    HookSpot* this = (void*)thisx;
}

void HookSpot_Draw(HookSpot* this, PlayState* play) {

     if((this->actor.params & 7) == HAS_SWITCH){
         Gfx_DrawDListOpa(play, &gHookSpotObj_DlHookSpotObj);
        gDPSetPrimColor(POLY_XLU_DISP++, 0xC0, 0xC0, 255, 200, 0, 0);
        gDPSetEnvColor(POLY_XLU_DISP++, 255, 0, 0, 0);
    }
    else{
        Gfx_DrawDListOpa(play, &gHookSpotObj_DlHookSpotNoSwitchObj);
    }
     
}
