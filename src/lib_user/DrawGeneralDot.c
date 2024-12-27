#include <z64hdr.h>
#include "code/z_player_lib.h"
#include "playas/playas.h"
#include "uLib.h"
#define THIRD_SHOOT_MODE (this->stateFlags1 & (PLAYER_STATE1_30 | PLAYER_STATE1_Z_TARGETING_UNFRIENDLY | PLAYER_STATE1_FORCE_STRAFING | PLAYER_STATE1_Z_TARGETING_FRIENDLY))
void DrawGeneralDot(PlayState* play, Player* this) {
 static Vec3f D_801260C8 = { -500.0f, -100.0f, 0.0f };
  CollisionPoly* sp9C;
  s32 bgId;
  Vec3f sp8C;
  Vec3f sp80;
  Vec3f sp74;
  Vec3f sp68;
  f32 sp64;
  f32 sp60;
  f32 dotSize = 0;
   D_801260C8.z = 0.0f;
   Matrix_MultVec3f(&D_801260C8, &sp8C);
   D_801260C8.z = 77600.0f;
   Matrix_MultVec3f(&D_801260C8, &sp80);

  if (1) {}
	BgCheck_AnyLineTest3(&play->colCtx, &sp8C, &sp80, &sp74, &sp9C, 1, 1, 1, 1, &bgId);
  if (1) {
    OPEN_DISPS(play->state.gfxCtx, "../z_player_lib.c", 2572);

   Gfx_SetupDL_25Xlu(__gfxCtx);

    SkinMatrix_Vec3fMtxFMultXYZW(&play->viewProjectionMtxF, &sp74, &sp68, &sp64);
	dotSize = THIRD_SHOOT_MODE ? 0.06f : 0.04f;
    sp60 = (sp64 < 120.0f) ? 0 : (sp64 / 200.0f) * dotSize;
    Matrix_Translate(sp74.x, sp74.y, sp74.z, MTXMODE_NEW);
    Matrix_Scale(sp60, sp60, sp60, MTXMODE_APPLY);
    Matrix_ReplaceRotation(&play->billboardMtxF);

   gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, "../z_player_lib.c", 2587),
               G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
   gSPSegment(POLY_XLU_DISP++, 0x06, play->objectCtx.status[this->actor.objBankIndex].segment);
	gSPDisplayList(POLY_XLU_DISP++, gPlayasAdult_DlHookshotDot);
	if(THIRD_SHOOT_MODE){
    	gSPDisplayList(POLY_XLU_DISP++, gPlayasAdult_DlHookshotDot2);
	}

   CLOSE_DISPS(play->state.gfxCtx, "../z_player_lib.c", 2592);
  }	
}