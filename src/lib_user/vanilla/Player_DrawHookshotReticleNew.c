#include <z64hdr.h>
#include "code/z_player_lib.h"
#include "playas/playas.h"
#include "uLib.h"

void Player_DrawHookshotReticleNew(PlayState* play, Player* this, f32 arg2){
	
  static Vec3f D_801260C8 = { -500.0f, -100.0f, 0.0f };
  CollisionPoly* sp9C;
  s32 bgId;
  Vec3f sp8C;
  Vec3f sp80;
  Vec3f sp74;
  Vec3f sp68;
  f32 sp64;
  f32 sp60;

  D_801260C8.z = 0.0f;
  Matrix_MultVec3f(&D_801260C8, &sp8C);
  D_801260C8.z = arg2;
  Matrix_MultVec3f(&D_801260C8, &sp80);

  if (1) {}

  if (BgCheck_AnyLineTest3(&play->colCtx, &sp8C, &sp80, &sp74, &sp9C, 1, 1, 1, 1, &bgId)) {
    OPEN_DISPS(play->state.gfxCtx, "../z_player_lib.c", 2572);

   Gfx_SetupDL_25Xlu(__gfxCtx);

    SkinMatrix_Vec3fMtxFMultXYZW(&play->viewProjectionMtxF, &sp74, &sp68, &sp64);

    sp60 = (sp64 < 200.0f) ? 0.08f : (sp64 / 200.0f) * 0.08f;

    Matrix_Translate(sp74.x, sp74.y, sp74.z, MTXMODE_NEW);
    Matrix_Scale(sp60, sp60, sp60, MTXMODE_APPLY);
    Matrix_ReplaceRotation(&play->billboardMtxF);

   gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, "../z_player_lib.c", 2587),
               G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
   gSPSegment(POLY_XLU_DISP++, 0x06, play->objectCtx.status[this->actor.objBankIndex].segment);

    if (SurfaceType_CanHookshot(&play->colCtx, sp9C, bgId)) {
       gSPDisplayList(POLY_XLU_DISP++, gPlayasAdult_DlHookshotDot);
       u32 frame = play->gameplayFrames % 20;
       f32 frameAngle;
       frameAngle = DEG_TO_RAD(10) / 5 * (ABS((s32)((frame + 5) % 20) - 10) - 5);
       Matrix_ReplaceRotation(&play->billboardMtxF);
       Matrix_RotateZ(frameAngle, MTXMODE_APPLY);
       gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__),
        G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
      gSPDisplayList(POLY_XLU_DISP++, gPlayasAdult_DlHookshotDot2);
    }else{
      gSPDisplayList(POLY_XLU_DISP++, gPlayasAdult_DlHookshotDot);
    }
   CLOSE_DISPS(play->state.gfxCtx, "../z_player_lib.c", 2592);
  }
}