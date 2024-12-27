#ifndef PROJECT_EFFECT_H
#define PROJECT_EFFECT_H
#define EFFECT_INIT_ONLY

/**
 * V i s u a l   E f f e c t s
 * 
 * Only includes headers of vfx that has magic word:
 * "z64vfx-magic-word"
 * 
 * Anything else will be considered vanilla replacement which
 * already have their own initializer struct and index defined.
 * 
 * Include this header whenever you are planning to spawn your
 * custom vfx.
 * 
 * void main() {
 *     VfxMyNewCloudInitParams params = {
 *         .vel = { 1, 0, 0},
 *         .accel = { 0, 1, 0},
 *     };
 *     
 *     Vfx_Spawn(play, VFX_MY_NEW_CLOUD, 128, &params);
 * }
**/

#include <global.h>
#include <asm_macros.h>

#define VFX_PRIORITY_DEFAULT 128

void Vfx_Spawn(PlayState*, s32 index, s32 priority, void* initParams);
Asm_SymbolAlias("Vfx_Spawn", EffectSs_Spawn);

enum {
	VFX_MAX,
};


#undef EFFECT_INIT_ONLY
#endif
