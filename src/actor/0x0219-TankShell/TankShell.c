#include <uLib.h>
#include "object/0x020F-TankShellObj.h"
#include "TankShell.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h"
#include "overlays/actors/ovl_En_Bom/z_en_bom.h"

void TankShell_Init(TankShell* this, PlayState* play);
void TankShell_Destroy(TankShell* this, PlayState* play);
void TankShell_Update(TankShell* this, PlayState* play);
void TankShell_Draw(TankShell* this, PlayState* play);
void TankShell_Fly(TankShell* this, PlayState* play) ;
void TankShell_SetupFly(TankShell* this, PlayState* play) ;
void TankShell_Explode(TankShell* this, PlayState* play);


const ActorInit sTankShell_InitVars = {
    .id = 0x0219,
    .category     = ACTORCAT_PROP,
    .flags        = 0x00000030,
    .objectId = 0x020F,
    .instanceSize = sizeof(TankShell),
    .init         = (ActorFunc)TankShell_Init,
    .destroy      = (ActorFunc)TankShell_Destroy,
    .update       = (ActorFunc)TankShell_Update,
    .draw         = (ActorFunc)TankShell_Draw
};

static ColliderQuadInit sHitboxInit = {
    {
        COLTYPE_METAL,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_QUAD,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000008, 0x00, 0x08 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_NEAREST | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_ON,
    },
    { { { 0.0f, 0.0f, 0.0f}, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } } },
};


void TankShell_Init(TankShell* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    //ActorShape_Init(&this->actor.shape, 1100.0f, ActorShadow_DrawCircle, 18.0f);
    Collider_InitQuad(play, &this->hitbox);
    Collider_SetQuad(play, &this->hitbox, &this->actor, &sHitboxInit);
    if(this->actor.params == GRENADE_MODE){
        this->actor.gravity = - 8.0f;
    }else{
        this->actor.gravity = - 0.5f;
    }
    this->actor.speedXZ = (this->actor.params == GRENADE_MODE) ? 9.0f : 18.0f;
    TankShell_SetupFly(this,play);

}

void TankShell_SetupFly(TankShell* this, PlayState* play) {
    this->timer = 80;
    this->actionFunc = TankShell_Fly;
}

void TankShell_Fly(TankShell* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    
}

void TankShell_Explode(TankShell* this, PlayState* play) {
    EnBom* bomb;
    Audio_PlayActorSfx2(&this->actor, NA_SE_EN_FANTOM_HIT_THUNDER);
    bomb = (EnBom*)Actor_Spawn(&play->actorCtx, play, ACTOR_EN_BOM, this->actor.world.pos.x, this->actor.world.pos.y,
                        this->actor.world.pos.z, 0, 0, 0, BOMB_BODY);
    if (bomb != NULL) {
            bomb->timer = 0;
    }
            
    Actor_Kill(&this->actor);
}

void TankShell_Destroy(TankShell* this, PlayState* play) {
    Collider_DestroyQuad(play, &this->hitbox);

}


void TankShell_Update(TankShell* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    u8 Explode = 0;
    u8 kickPlayer = 0;
    Actor_MoveForward(&this->actor);
    this->actor.velocity.y = -Math_SinS(this->actor.world.rot.x) * this->actor.speedXZ;
    if (this->actor.bgCheckFlags & BGCHECKFLAG_WALL || (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) || (this->actor.bgCheckFlags & BGCHECKFLAG_CEILING)) {
        Explode = 1;
    }
    if (this->hitbox.base.ocFlags1 & OC1_HIT) {  
        if(this->hitbox.base.oc->id != 0x0219){
            this->actor.speedXZ = 0;
            if(&player->actor == this->hitbox.base.oc){
                kickPlayer = 1;
            }
            Explode = 1;      
        }     
    }
    if(this->hitbox.base.atFlags & AT_HIT){
        if(&player->actor == this->hitbox.base.at){
            kickPlayer = 1;
        }
        Explode = 1;
    }
    if(this->hitbox.base.acFlags & AC_HIT){
        Explode = 1;
    }
    if(this->timer == 0 ){
        Explode = 1;
    }
    this->actor.shape.rot.y =  this->actor.world.rot.y;
    this->actor.shape.rot.z =  this->actor.world.rot.z;
    this->actor.shape.rot.x =  this->actor.world.rot.x;
    if(kickPlayer){
        func_8002F71C(play, &this->actor, 8.0f, this->actor.world.rot.y, 8.0f);
    }
    if(Explode){
        if(ABS(Actor_WorldDistXYZToActor(&this->actor, &player->actor)) >= 500.0f){
            Actor_Kill(&this->actor);
        }else{
            TankShell_Explode(this,play);
        }
    }
    if(this->timer >0){
        this->timer--;
    }

    Actor_UpdateBgCheckInfo(play, &this->actor, 10.0f, 15.0f, 30.0f, UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2 | UPDBGCHECKINFO_FLAG_1);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->hitbox.base);
    CollisionCheck_SetAT(play, &play->colChkCtx, &this->hitbox.base);
    this->actionFunc(this, play);

}

s32 TankShell_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3f* pos, Vec3s* rot, void* thisx) {
    TankShell* this = (void*)thisx;
    
    return 0;
}

void TankShell_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3s* rot, void* thisx) {
    TankShell* this = (void*)thisx;
}

void TankShell_Draw(TankShell* this, PlayState* play) {
    static Vec3f sMultVec1 = { -960.0f, 0.0f, 0.0f };
    static Vec3f sMultVec2 = { 960.0f, 0.0f, 0.0f };
    Vec3f vec1;
    Vec3f vec2;
    this->yawSpeed = this->yawSpeed + 0x300;
    this->yaw = this->yaw + this->yawSpeed;
    Matrix_MultVec3f(&sMultVec1, &vec1);
    Matrix_MultVec3f(&sMultVec2, &vec2);
    if (func_80090480(play, &this->hitbox, &this->shellInfo, &vec1, &vec2)) {
         Matrix_RotateZ(this->yaw, MTXMODE_APPLY) ;
    }
    Gfx_DrawDListOpa(play, gTankShellObj_DlTankShellObj);


}
