#include <uLib.h>
#include <code/z_parameter.h>
#include <code/z_map_exp.h>

Asm_VanillaHook(Health_ChangeBy);
s32 Health_ChangeBy(PlayState* play, s16 amount) {
    InterfaceContext* interfaceCtx = &play->interfaceCtx;
    if (amount > 0) { Audio_PlaySfxGeneral(NA_SE_SY_HP_RECOVER, &gSfxDefaultPos, 4, &gSfxDefaultFreqAndVolScale,
                                             &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
    } else if (gSaveContext.isDoubleDefenseAcquired && (amount < 0)) {
        amount >>= 1;
    }
    // clang-format on
    interfaceCtx->unk_226 = true;
    //if(play->sceneId != 0x43){
        gSaveContext.healthAccumulator = amount;
    //}
    if (gSaveContext.health > gSaveContext.healthCapacity) {
        gSaveContext.health = gSaveContext.healthCapacity;
    }

    if (gSaveContext.health <= 0) {
        gSaveContext.health = 0;
        return false;
    } else {
        return true;
    }
}