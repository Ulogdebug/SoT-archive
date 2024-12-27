#include <uLib.h>
#include <code/z_actor.h>

Asm_VanillaHook(Actor_Delete);
Actor* Actor_Delete(ActorContext* actorCtx, Actor* actor, PlayState* play) {
    char* name;
    Player* player;
    Actor* newHead;
    ActorOverlay* overlayEntry;

    player = GET_PLAYER(play);

    overlayEntry = actor->overlayEntry;
    name = overlayEntry->name != NULL ? overlayEntry->name : "";

    if (HREG(20) != 0) {
        osSyncPrintf("アクタークラス削除 [%s]\n", name); // "Actor class deleted [%s]"
    }

    if ((player != NULL) && (actor == player->unk_664)) {
        func_8008EDF0(player);
        Camera_ChangeMode(Play_GetCamera(play, Play_GetActiveCamId(play)), 0);
    }

    if (actor == actorCtx->targetCtx.arrowPointedActor) {
        actorCtx->targetCtx.arrowPointedActor = NULL;
    }

    if (actor == actorCtx->targetCtx.unk_8C) {
        actorCtx->targetCtx.unk_8C = NULL;
    }

    if (actor == actorCtx->targetCtx.bgmEnemy) {
        actorCtx->targetCtx.bgmEnemy = NULL;
    }

    Audio_StopSfxByPos(&actor->projectedPos);
    Actor_Destroy(actor, play);

    newHead = Actor_RemoveFromCategory(play, actorCtx, actor);

    ZeldaArena_FreeDebug(actor, "../z_actor.c", 7242);

    if (overlayEntry->vramStart == NULL) {
        if (HREG(20) != 0) {
            osSyncPrintf("オーバーレイではありません\n"); // "Not an overlay"
        }
    } else {
        ASSERT(overlayEntry->loadedRamAddr != NULL, "actor_dlftbl->allocp != NULL", "../z_actor.c", 7251);
        ASSERT(overlayEntry->numLoaded > 0, "actor_dlftbl->clients > 0", "../z_actor.c", 7252);
        overlayEntry->numLoaded--;
        Actor_FreeOverlay(overlayEntry);
    }
    // this is required to avoid having actor data still loaded in RAM after it's supposed to be deleted
    // see func_80031B14
    Actor_Kill(actor);
    return newHead;
}