#ifndef __TELEPORTER_H__
#define __TELEPORTER_H__

#include <uLib.h>
 
struct Teleporter;

typedef void (* TeleporterFunc)(struct Teleporter*, PlayState*);

typedef struct Teleporter {
    union {
        Actor actor;
        DynaPolyActor dyna;
    };
    Actor* targetTeleporter;
    Actor* WarppedEnemy;
    TeleporterFunc actionFunc;
    s16 TeleporterOrder;
    Vec3f targetPos;
    Vec3s targetRot;
    u8 freezePlayerTimer;
    s16 timer;
    s16 switchFlag;
    LightNode* lightNode;
    LightInfo lightInfo;
} Teleporter;

#endif // __TELEPORTER_H__