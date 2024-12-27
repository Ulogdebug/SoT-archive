#ifndef __TANK_SHELL_H__
#define __TANK_SHELL_H__

#include <uLib.h>

struct TankShell;

typedef void (* TankShellFunc)(struct TankShell*, PlayState*);

typedef struct TankShell {
    Actor actor;
    TankShellFunc actionFunc;
    ColliderQuad hitbox;
    s16 timer;
    s32 effectIndex;
    WeaponInfo shellInfo;
    s16 yaw;
    s16 yawSpeed;
} TankShell;

typedef enum {
    TANKSHELL_ON_THE_WAY = 0,
    TANKSHELL_EXPLODE = 1,
    GRENADE_MODE = 2
} TankShellParam;

#endif // __TANK_SHELL_H__