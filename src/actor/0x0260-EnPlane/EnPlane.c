#include "EnPlane.h"
#include <uLib_vector.h>
#include <uLib_math.h>

#define ACTOR_ID 0x0260 /* define your actor index here */

#if ACTOR_ID == 0x0000
#error "please define proper actor index"
#endif

/**
    EnPlane by default does a ceiling check to set height to itself. This
    might fail though depending on the scene so this value will be used
    instead for the height.
 */
#define SETTING_FALLBACK_HEIGHT 120.0f

/**
    The height for checking ceiling for automatic height.
 */
#define SETTING_CEIL_CHECK_HEIGHT 500.0f

/**
    Amount of units to check below the actor for Player. Helps to deal if
    the plane is located on a slope.
 */
#define SETTING_Y_CHECK_PADDING 50.0f

/**
    Parameter unit multiplier.
 */
#define SETTING_UNIT_MULT 10

/**
    Mask values for param/variable.
 */
#define PARAM_WIDTH  0b0000000000011111
#define PARAM_RADIUS 0b0000000111100000
#define PARAM_FADE   0b0000001000000000
#define PARAM_TID    0b1111110000000000

#ifdef DEV_BUILD
#define DEBUG_DRAW true
#else
#define DEBUG_DRAW false
#endif

#ifndef rmask
#define rmask(value, mask) \
    ((s32)((value >> __builtin_ctz(mask)) & (mask >> __builtin_ctz(mask))))
#endif

void Init(EnPlane* this, PlayState* play);
void Destroy(EnPlane* this, PlayState* play);
void Update(EnPlane* this, PlayState* play);
void Draw(EnPlane* this, PlayState* play);

void Action_Transition(EnPlane* this, PlayState* play, RoomSide side, bool cameraOnSameSide);
void Action_Idle(EnPlane* this, PlayState* play, RoomSide side, bool cameraOnSameSide);

const ActorInit sEnPlane_InitVars = {
    .id           = ACTOR_ID,
    .category     = ACTORCAT_DOOR,
    .flags        = (1<<4), // always update
    .objectId     = 0x0001,
    .instanceSize = sizeof(EnPlane),
    .init         = (ActorFunc)Init,
    .destroy      = (ActorFunc)Destroy,
    .update       = (ActorFunc)Update,
    .draw         = (ActorFunc)Draw
};

extern s32 Room_RequestNewPrimary(PlayState*, RoomContext*, s32);
asm("Room_RequestNewPrimary = func_8009728C");
extern void Room_FinalizeChange(PlayState*, RoomContext*);
asm("Room_FinalizeChange = func_80097534");

void Room_SwapRooms(PlayState* play, RoomContext* roomCtx) {
    Room tempRoom;

    tempRoom = roomCtx->curRoom;
    roomCtx->curRoom = roomCtx->prevRoom;
    roomCtx->prevRoom = tempRoom;
    play->roomCtx.unk_30 ^= 1;
}

Vec3f Vec3f_ProjectAndClampOnLine(Vec3f input, Vec3f linePointA, Vec3f linePointB) {
    Vec3f lineDir = Vec3f_Sub(linePointB, linePointA);
    Vec3f toTargetDir = Vec3f_Sub(input, linePointA);
    f32 lt = Vec3f_Dot(lineDir, toTargetDir);
    f32 ll = Vec3f_Dot(lineDir, lineDir);

    return Vec3f_Add(linePointA, Vec3f_MulVal(lineDir, CLAMP(lt / ll, 0.0f, 1.0f)));
}

Vec3f Vec3f_ProjectOnLine(Vec3f input, Vec3f linePointA, Vec3f linePointB) {
    Vec3f lineDir = Vec3f_Sub(linePointB, linePointA);
    Vec3f toTargetDir = Vec3f_Sub(input, linePointA);
    f32 lt = Vec3f_Dot(lineDir, toTargetDir);
    f32 ll = Vec3f_Dot(lineDir, lineDir);

    return Vec3f_Add(linePointA, Vec3f_MulVal(lineDir, lt / ll));
}

#define SetAction(Action) \
    this->action = Action

void Action_Transition(EnPlane* this, PlayState* play, RoomSide side, bool cameraOnSameSide) {
    if (side > ROOM_SIDE_NONE) {
        this->actor.room = this->roomIndices[side];

        if (play->roomCtx.status == 0 && this->roomIndices[side] != play->roomCtx.curRoom.num)
            Room_SwapRooms(play, &play->roomCtx);

        return;
    }

    if (!cameraOnSameSide)
        return;

    Room_FinalizeChange(play, &play->roomCtx);
    SetAction(Action_Idle);
}

void Action_Idle(EnPlane* this, PlayState* play, RoomSide side, bool cameraOnSameSide) {
    if (side == ROOM_SIDE_NONE)
        return;

    // accessing on active side, load opposite
    if (this->roomIndices[side] == play->roomCtx.curRoom.num)
        if (Room_RequestNewPrimary(play, &play->roomCtx, this->roomIndices[side ^ 1]))
            SetAction(Action_Transition);
}

void Init(EnPlane* this, PlayState* play) {
    f32 width = CLAMP_MIN(rmask(this->actor.params, PARAM_WIDTH) * SETTING_UNIT_MULT, 1);
    s32 radius = CLAMP_MIN(rmask(this->actor.params, PARAM_RADIUS) * SETTING_UNIT_MULT, 1);

    Vec3f vecL = Math_Vec3f_YawDist(1.0f, this->actor.rot.y + DEG_TO_BINANG(-90));
    Vec3f vecR = Math_Vec3f_YawDist(1.0f, this->actor.rot.y + DEG_TO_BINANG(90));
    vecL = Vec3f_MulVal(vecL, width / Vec3f_Magnitude(vecL));
    vecR = Vec3f_MulVal(vecR, width / Vec3f_Magnitude(vecR));

    this->corners[0] = Vec3f_Add(vecL, this->actor.pos);
    this->corners[1] = Vec3f_Add(vecR, this->actor.pos);
    this->action = Action_Idle;

    Vec3f rayStart = Vec3f_Add(this->actor.pos, (Vec3f){0.0f, 5.0f, 0.0f});
    Vec3f rayEnd = Vec3f_Add(this->actor.pos, (Vec3f){0.0f, SETTING_CEIL_CHECK_HEIGHT, 0.0f});
    Vec3f hit;
    CollisionPoly* poly;
    s32 height;

    if (BgCheck_AnyLineTest1(&play->colCtx, &rayStart, &rayEnd, &hit, &poly, true))
        height = this->height = hit.y - rayStart.y;
    else
        height = this->height = SETTING_FALLBACK_HEIGHT;

    this->planeVertices[0] = (Vtx)VTX((width + radius) *  100,               -2000, 0, 0, 0, 0, 0, 0, 255);
    this->planeVertices[1] = (Vtx)VTX((width + radius) * -100,               -2000, 0, 0, 0, 0, 0, 0, 255);
    this->planeVertices[2] = (Vtx)VTX((width + radius) * -100, height * 100 + 2000, 0, 0, 0, 0, 0, 0, 255);
    this->planeVertices[3] = (Vtx)VTX((width + radius) *  100, height * 100 + 2000, 0, 0, 0, 0, 0, 0, 255);

    if (DEBUG_DRAW) {
        Vtx* vtx = this->dbgVertices;
        vtx[0] = (Vtx)VTX((s32)(width * 100), 0, 0, 0, 0, 255, 255, 255, 255);
        vtx[1] = (Vtx)VTX((s32)(-width * 100), 0, 0, 0, 0, 255, 255, 255, 255);
        vtx[2] = (Vtx)VTX((s32)(-width * 100), 0, -radius * 100, 0, 0, 255, 255, 255, 255);
        vtx[3] = (Vtx)VTX((s32)(width * 100), 0, -radius * 100, 0, 0, 255, 255, 255, 255);

        for (u32 i = 0; i < 14; i++) {
            Vec3f posA = Math_Vec3f_YawDist(radius * 100, DEG_TO_BINANG(90.0f * (i / 13.0f)));
            Vec3f posB = Math_Vec3f_YawDist(radius * 100, DEG_TO_BINANG(-90.0f * (i / 13.0f)));
            posA.x += width * 100;
            posB.x -= width * 100;
            vtx[4 + i].v.ob[0] = (s32)posA.x;
            vtx[4 + i].v.ob[1] = (s32)posA.y;
            vtx[4 + i].v.ob[2] = -(s32)posA.z;
            vtx[18 + i].v.ob[0] = (s32)posB.x;
            vtx[18 + i].v.ob[1] = (s32)posB.y;
            vtx[18 + i].v.ob[2] = -(s32)posB.z;
        }
    }

    TransitionActorEntry* trans = play->transiActorCtx.list + rmask(this->actor.params, PARAM_TID);

    for (RoomSide i = ROOM_SIDE_FRONT; i < ROOM_SIDE_MAX; i++)
        this->roomIndices[i] = trans->sides[i].room;

    if (!DEBUG_DRAW) {
        s32 draw = rmask(this->actor.params, PARAM_FADE);

        if (!draw)
            this->actor.draw = NULL;
        else
            this->actor.draw = (ActorFunc)Draw;
    }
}

void Destroy(EnPlane* this, PlayState* play) {
    TransitionActorEntry* trans = play->transiActorCtx.list + rmask(this->actor.params, PARAM_TID);

    trans->id = -trans->id;
}

void Update(EnPlane* this, PlayState* play) {
    Actor* player = (Actor*)GET_PLAYER(play);
    Camera* camera = GET_ACTIVE_CAM(play);
    f32 radius = CLAMP_MIN(rmask(this->actor.params, PARAM_RADIUS) * SETTING_UNIT_MULT, 1);
    f32 height = this->height;
    Vec3f playerRelativePos;
    Vec3f cameraRelativePos;

    func_8002DBD0(&this->actor, &playerRelativePos, &player->pos);
    func_8002DBD0(&this->actor, &cameraRelativePos, &camera->eye);

    this->checkPos = Vec3f_ProjectAndClampOnLine(player->pos, this->corners[0], this->corners[1]);

    typedef struct Float {
        u32 sign : 1;
        u32 exponent : 8;
        u32 significand : 23;
    } FP;

    s32 inRangeXZ = Vec3f_DistXZ(this->checkPos, player->pos) <= radius;
    s32 inRangeY = player->pos.y >= (this->actor.pos.y - SETTING_Y_CHECK_PADDING) && player->pos.y <= (this->actor.pos.y + height);
    RoomSide side = (inRangeXZ && inRangeY)
                    ? (playerRelativePos.z < 0.0f ? ROOM_SIDE_FRONT : ROOM_SIDE_BACK)
                    : ROOM_SIDE_NONE;
    bool cameraOnSameSide = ((FP*)&playerRelativePos.z)->sign == ((FP*)&cameraRelativePos.z)->sign;

    this->action(this, play, side, cameraOnSameSide);
}

void Draw(EnPlane* this, PlayState* play) {
    s32 draw = rmask(this->actor.params, PARAM_FADE);
    s32 radius = CLAMP_MIN(rmask(this->actor.params, PARAM_RADIUS) * SETTING_UNIT_MULT, 1);
    f32 height = this->height;

    static Gfx sDList[] = {
        gsDPSetCombineLERP(0, 0, 0, SHADE,
                           0, 0, 0, PRIMITIVE,
                           0, 0, 0, SHADE,
                           0, 0, 0, PRIMITIVE),
        gsSPGeometryMode(G_CULL_FRONT | G_LIGHTING, G_ZBUFFER | G_SHADE | G_CULL_BACK),
        gsSPVertex(0x08000000, 4, 0),
        gsSP2Triangles(0, 1, 2, 0, 2, 3, 0, 0),
        gsSPEndDisplayList(),
    };

    Actor* player = (Actor*)GET_PLAYER(play);
    f32 dist = Vec3f_DistXZ(this->checkPos, player->pos);
    s32 alpha = CLAMP((s32)Remap(dist, radius, 0, 0x110, 0), 0, 0xFF);

    RoomSide side = this->actor.room == this->roomIndices[ROOM_SIDE_FRONT]
                    ? ROOM_SIDE_FRONT
                    : ROOM_SIDE_BACK;

    if (side == ROOM_SIDE_BACK)
        Matrix_RotateY_f(180, MTXMODE_APPLY);

    if (draw) {
        POLY_XLU_DISP = Gfx_SetupDL(POLY_XLU_DISP, SETUPDL_0);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 0, 0, 0, alpha);
        gSPSegment(POLY_XLU_DISP++, 0x8, this->planeVertices);
        gSPMatrix(POLY_XLU_DISP++, NEW_MATRIX(), G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, sDList);
    }

    if (DEBUG_DRAW) {
        static const Gfx sDebugDList[] = {
            gsDPSetCombineLERP(
                    0, 0, 0, PRIMITIVE,
                    0, 0, 0, PRIMITIVE,
                    0, 0, 0, PRIMITIVE,
                    0, 0, 0, PRIMITIVE),
            gsSPGeometryMode(G_CULL_BOTH | G_LIGHTING, G_ZBUFFER | G_SHADE),
            gsSP2Triangles(0, 1, 2, 0, 2, 3, 0, 0),

            gsSP2Triangles(0, 4, 5, 0, 0, 5, 6, 0),
            gsSP2Triangles(0, 6, 7, 0, 0, 7, 8, 0),
            gsSP2Triangles(0, 8, 9, 0, 0, 9, 10, 0),
            gsSP2Triangles(0, 10, 11, 0, 0, 11, 12, 0),
            gsSP2Triangles(0, 12, 13, 0, 0, 13, 14, 0),
            gsSP2Triangles(0, 14, 15, 0, 0, 15, 16, 0),
            gsSP1Triangle(0, 16, 17, 0),

            gsSP2Triangles(1, 18, 19, 0, 1, 19, 20, 0),
            gsSP2Triangles(1, 20, 21, 0, 1, 21, 22, 0),
            gsSP2Triangles(1, 22, 23, 0, 1, 23, 24, 0),
            gsSP2Triangles(1, 24, 25, 0, 1, 25, 26, 0),
            gsSP2Triangles(1, 26, 27, 0, 1, 27, 28, 0),
            gsSP2Triangles(1, 28, 29, 0, 1, 29, 30, 0),
            gsSP1Triangle(1, 30, 31, 0),

            gsSPEndDisplayList(),
        };

        Gfx_SetupDL_25Opa(__gfxCtx);

        // front
        Matrix_Push();
        Matrix_Translate(0, 4 / 0.01f, 0, MTXMODE_APPLY);
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 66, 178, 236, 255);
        gSPMatrix(POLY_OPA_DISP++, NEW_MATRIX(), G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPVertex(POLY_OPA_DISP++, this->dbgVertices, 32, 0);
        gSPDisplayList(POLY_OPA_DISP++, sDebugDList);

        Matrix_Translate(0, (height - 4) / 0.01f, 0, MTXMODE_APPLY);
        gSPMatrix(POLY_OPA_DISP++, NEW_MATRIX(), G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPVertex(POLY_OPA_DISP++, this->dbgVertices, 32, 0);
        gSPDisplayList(POLY_OPA_DISP++, sDebugDList);
        Matrix_Pop();

        // back
        Matrix_RotateY_f(180, MTXMODE_APPLY);
        Matrix_Translate(0, 4 / 0.01f, 0, MTXMODE_APPLY);
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 252, 87, 113, 255);
        gSPMatrix(POLY_OPA_DISP++, NEW_MATRIX(), G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPVertex(POLY_OPA_DISP++, this->dbgVertices, 32, 0);
        gSPDisplayList(POLY_OPA_DISP++, sDebugDList);

        Matrix_Translate(0, (height - 4) / 0.01f, 0, MTXMODE_APPLY);
        gSPMatrix(POLY_OPA_DISP++, NEW_MATRIX(), G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPVertex(POLY_OPA_DISP++, this->dbgVertices, 32, 0);
        gSPDisplayList(POLY_OPA_DISP++, sDebugDList);
    }
}
