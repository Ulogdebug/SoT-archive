/*
 * File: z_Obj_Fan.c
 * Overlay: ovl_Obj_Fan
 * Description: Spinning fan actor that can push the player and bombs, from the Hylian Modding actor pack.
 * Params: XXYZ
 *      -- Z: Scale multiplier in 0.1f increments, 0 = 1.0f scale, 1 = 1.1f scale, etc.
 *          -- 1.0f scale has a 114 unit push radius
 *          -- Scale also affects how far the fan's push reaches
 *      -- XX: Switch flag         
 *      -- Y:  Flip flag
 */

#include "z_obj_fan.h"
#include "object/0x0225-VerticalFanObj.h"

#define FLAGS ACTOR_FLAG_4 | ACTOR_FLAG_5 | ACTOR_FLAG_19
#define FAN_REACH 500.0f
#define FAN_RADIUS 114.0f
#define FAN_POWER 20.0f
#define FAN_THRESHOLD 0.5f

void ObjFan_Init(Actor* thisx, PlayState* play);
void ObjFan_Destroy(Actor* thisx, PlayState* play);
void ObjFan_Update(Actor* thisx, PlayState* play);
void ObjFan_Draw(Actor* thisx, PlayState* play);
void ObjFan_VoidOutPlayer(ObjFan* this, PlayState* play);
u8 ObjFan_ActorInRange(Actor* target, ObjFan* this,PlayState* play);
ActorInit Obj_Fan_InitVars = {
    0x0234,
    ACTORCAT_PROP,
    FLAGS,
    0x0225,
    sizeof(ObjFan),
    (ActorFunc)ObjFan_Init,
    (ActorFunc)ObjFan_Destroy,
    (ActorFunc)ObjFan_Update,
    (ActorFunc)ObjFan_Draw,
};

void ObjFan_Init(Actor* thisx, PlayState* play) {
    ObjFan* this = (ObjFan*)thisx;

    CollisionHeader* colHeader = NULL;
    DynaPolyActor_Init(&this->dyna, DPM_PLAYER);
    CollisionHeader_GetVirtual(&gVerticalFanObj_CollVerticalFanObj, &colHeader);
    this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, thisx, colHeader);

    this->scale = 1.0f + (((u16)this->actor.params & 0xF)* 0.1f); 
    this->flip =  this->actor.params >> 4 & 0xF;
    this->flag = (this->actor.params >> 8) & 0xFF; 

    if ((this->flip && (!Flags_GetSwitch(play, this->flag) || (this->flag == 0x0 || this->flag == 0x3F))) || ((!this->flip) && (Flags_GetSwitch(play, this->flag)|| (this->flag == 0x0 || this->flag == 0x3F)))) {
        this->power = FAN_POWER;
    }
}

void ObjFan_Destroy(Actor* thisx, PlayState* play) {
    ObjFan* this = (ObjFan*)thisx;
    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

void ObjFan_SetRelativePos(Actor* target, ObjFan* this) {
    MtxF rotationMatrix;
    Vec3f temp;
    /* Assume that the position of the fan actor is the origin of the new coordinate system, 
    *  then temp is the position of the target actor in the new coordinate system. 
    */
    Vec3f fixedTargetPos;
    Math_Vec3f_Copy(&fixedTargetPos, &target->world.pos);
    fixedTargetPos.y = fixedTargetPos.y + 35.0f;
    Math_Vec3f_Diff(&fixedTargetPos, &this->actor.world.pos, &temp); 
    /* Convert Euler angles to rotation matrix. */
    SkinMatrix_SetRotateZYX(&rotationMatrix, -this->actor.world.rot.x, -this->actor.world.rot.y, -this->actor.world.rot.z);
    /* Multiply the rotation matrix and the vector to obtain the relative position after rotation.*/
    SkinMatrix_Vec3fMtxFMultXYZ(&rotationMatrix, &temp, &this->relativeActorPos);
}

u8 ObjFan_ActorInRange(Actor* target, ObjFan* this,PlayState* play) {
    f32 dist;
    u8 i;
    Vec3f center;
    Vec3f fixedTargetPos;
    s32 bgId;
    Math_Vec3f_Copy(&fixedTargetPos, &target->world.pos);
    Math_Vec3f_Copy(&center, &this->actor.world.pos);
    fixedTargetPos.y = fixedTargetPos.y + 35.0f;
    f32 reach = (FAN_REACH * (this->power / FAN_POWER)) * this->scale;
    CollisionPoly* colPoly;
    ObjFan_SetRelativePos(target, this);
    Vec3f posResult;
    if (ABS(this->relativeActorPos.z) < 0 || ABS(this->relativeActorPos.z) > reach) {
        return 0;
    }

    dist = sqrtf(SQ(this->relativeActorPos.x) + SQ(this->relativeActorPos.y)); // target dist from intercept

    if (dist > ABS(FAN_RADIUS /** this->scale*/)) {
        return 0;
    }
    if(BgCheck_ProjectileLineTest(&play->colCtx, &center, &fixedTargetPos, &posResult,
                           &colPoly, true, false, true, false,
                           &bgId) == 1){
        return 0;
    }
    return 1;
}

void ObjFan_PushPlayer(Player* player, ObjFan* this, f32 distRatio, PlayState* play) {
    // Scale down power based on distance
    // XZ power is scaled up slightly to make Y power relatively weaker, helps accentuate effect of gravity
    if(player->actor.bgCheckFlags & BGCHECKFLAG_GROUND && (GET_PLAYER(play)->currentBoots == PLAYER_BOOTS_IRON)){
        return;
    }
    f32 powerXZ = Math_CosS(this->actor.shape.rot.x) * this->power * distRatio * 1.25f;
    f32 powerY = -Math_SinS(this->actor.shape.rot.x) * this->power * distRatio;
    player->pushedYaw = this->actor.shape.rot.y;
    if((GET_PLAYER(play)->currentBoots == PLAYER_BOOTS_IRON)){
        return;//player->pushedSpeed = powerXZ * 0.5f;
    }else{
        player->pushedSpeed = powerXZ;
    }
     if ((powerY > 0 || powerY <= player->actor.minVelocityY) && (GET_PLAYER(play)->currentBoots != PLAYER_BOOTS_IRON)) {
         player->actor.velocity.y = powerY + (3.0f * Math_SinS(play->gameplayFrames * DEG_TO_BINANG(22.5f)));
     }
    
}


void ObjFan_PushActor(Actor* actor, ObjFan* this, PlayState* play) {
    f32 dist = sqrtf(SQ(this->relativeActorPos.x) + SQ(this->relativeActorPos.y) + SQ(this->relativeActorPos.z));
    f32 maxDist = FAN_REACH * this->scale;
    f32 scaledThreshold = maxDist * FAN_THRESHOLD;
    f32 distRatio;

    // Calculate the distance ratio based on the threshold
    if (dist <= scaledThreshold) {
        distRatio = 1.0f; // Full power within the threshold
    } else if (dist > scaledThreshold && dist < maxDist) {
        distRatio = 1.0f - ((dist - scaledThreshold) / (maxDist - scaledThreshold));
    } else {
        distRatio = 0.0f; // No power beyond max distance
    }

    if ((actor->id == ACTOR_PLAYER)) {
        ObjFan_PushPlayer((Player*)actor, this, distRatio, play);
        return;
    }

    // XZ power is scaled up slightly to make Y power relatively weaker, helps accentuate effect of gravity
    f32 powerXZ = Math_CosS(this->actor.shape.rot.x) * this->power * distRatio * 1.25;
    f32 powerY = -Math_SinS(this->actor.shape.rot.x) * this->power * distRatio;
    
    if (powerXZ > 0) {
        actor->speedXZ = powerXZ;
        actor->world.rot.y = this->actor.shape.rot.y;
    }
    if (powerY > 0 || powerY <= actor->minVelocityY) {
        actor->velocity.y = powerY + (3.0f * Math_SinS(play->gameplayFrames * DEG_TO_BINANG(22.5f)));
    }
}

void ObjFan_VoidOutPlayer(ObjFan* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    player->actor.freezeTimer = 10;
    if(!this->linkStartVoidOut){
        this->linkStartVoidOut = 1;
        func_80078884(NA_SE_OC_ABYSS);
        func_8002F7DC(&this->actor, NA_SE_VO_LI_TAKEN_AWAY);
        Play_TriggerVoidOut(play);
    }
}


void ObjFan_Update(Actor* thisx, PlayState* play) {
    ObjFan* this = (ObjFan*)thisx;
    Actor* linkandHeldActor = &GET_PLAYER(play)->actor;
    Actor* explosive;
    Actor* enemy;
    Player* player = GET_PLAYER(play);
    f32 waterBoxYSurface = this->actor.world.pos.y;
    WaterBox* waterBox;
    if (WaterBox_GetSurface1(play, &play->colCtx, this->actor.world.pos.x, this->actor.world.pos.z, &waterBoxYSurface,&waterBox)) {
        this->distToWater = waterBoxYSurface - this->actor.world.pos.y;
    }
    //Debug_Text(255, 0, 0, 1, 1, "WaterDis : %f ",this->distToWater);
    if (this->power != 0) {
        if ((player->actor.wallPoly && player->actor.wallBgId == this->dyna.bgId)) {
            if(this->relativeActorPos.z < 0){
                ObjFan_VoidOutPlayer(this,play);
            }else{
                func_8002F71C(play, &this->actor, 6.0f, this->actor.world.rot.y, 4.0f);
                 play->damagePlayer(play, -16);
            }
            DynaPoly_DisableCollision(play, &play->colCtx.dyna, this->dyna.bgId);
        }else{
            DynaPoly_EnableCollision(play, &play->colCtx.dyna, this->dyna.bgId);
        }
        if(DynaPolyActor_IsPlayerOnTop(&this->dyna)){
            if(play->sceneId != 0x49){
                ObjFan_VoidOutPlayer(this,play);
            }
        }
        if (ObjFan_ActorInRange(linkandHeldActor, this,play)) {
            ObjFan_PushActor(linkandHeldActor, this, play);
        }

        explosive = play->actorCtx.actorLists[ACTORCAT_EXPLOSIVE].head;
        while (explosive != NULL) {
            if (ObjFan_ActorInRange(explosive, this,play)) {
                ObjFan_PushActor(explosive, this, play);
            }
            explosive = explosive->next;
        }
        if( (this->distToWater > 0)){
            Audio_PlayActorSfx2(thisx, NA_SE_EV_WATER_CONVECTION - SFX_FLAG);
        }else{
            Audio_PlayActorSfx2(&this->actor, NA_SE_EV_WIND_TRAP - SFX_FLAG);
        }
    }

    if ((this->flip && (!Flags_GetSwitch(play, this->flag)|| (this->flag == 0x0 || this->flag == 0x3F)) ) || ((!this->flip) && (Flags_GetSwitch(play, this->flag)|| (this->flag == 0x0 || this->flag == 0x3F))) ) {
        Math_SmoothStepToF(&this->power, FAN_POWER, 1.0f, 0.5f, 0.0f);
    } else {
        Math_SmoothStepToF(&this->power, 0.0f, 1.0f, 0.5f, 0.0f);
        
    }

    this->propellerRot += DEG_TO_BINANG((this->power / FAN_POWER) * 45.0f);
}

void ObjFan_Draw(Actor* thisx, PlayState* play) {
    ObjFan* this = (ObjFan*)thisx;
    MtxF curMatrix;

    OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

    //Matrix_Scale(this->scale, this->scale, this->scale, MTXMODE_APPLY);

    Matrix_Get(&curMatrix);
    Matrix_RotateZYX(0, 0, this->propellerRot, MTXMODE_APPLY);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    gSPDisplayList(POLY_OPA_DISP++, gVerticalFanObj_DlVerticalFanObj);

    Matrix_Put(&curMatrix);

    Matrix_Scale(1.0f, 1.0f, 1.2f * this->power, MTXMODE_APPLY);
    
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    gSPSegment(POLY_XLU_DISP++, 0x08,
                Gfx_TwoTexScroll(play->state.gfxCtx, G_TX_RENDERTILE, 0, (play->gameplayFrames * 10) % 128, 64, 16,
                                1, 0, (play->gameplayFrames * 5) % 128, 64, 64));

    gSPDisplayList(POLY_XLU_DISP++, gWindDL);

    CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
}





