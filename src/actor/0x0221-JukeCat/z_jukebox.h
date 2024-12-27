#ifndef Z_JUKEBOX_H
#define Z_JUKEBOX_H

#include "ultra64.h"
#include "global.h"

struct Jukebox;

typedef void (*JukeboxActionFunc)(struct Jukebox*, PlayState*);

typedef enum {
    /* 0x0 */ SEQCMD_OP_PLAY_SEQUENCE,
    /* 0x1 */ SEQCMD_OP_STOP_SEQUENCE,
    /* 0x2 */ SEQCMD_OP_QUEUE_SEQUENCE,
    /* 0x3 */ SEQCMD_OP_UNQUEUE_SEQUENCE,
    /* 0x4 */ SEQCMD_OP_SET_PLAYER_VOLUME,
    /* 0x5 */ SEQCMD_OP_SET_PLAYER_FREQ,
    /* 0x6 */ SEQCMD_OP_SET_CHANNEL_VOLUME,
    /* 0x7 */ SEQCMD_OP_SET_PLAYER_IO,
    /* 0x8 */ SEQCMD_OP_SET_CHANNEL_IO,
    /* 0x9 */ SEQCMD_OP_SET_CHANNEL_IO_DISABLE_MASK,
    /* 0xA */ SEQCMD_OP_SET_CHANNEL_DISABLE_MASK,
    /* 0xB */ SEQCMD_OP_TEMPO_CMD,
    /* 0xC */ SEQCMD_OP_SETUP_CMD,
    /* 0xD */ SEQCMD_OP_SET_CHANNEL_FREQ,
    /* 0xE */ SEQCMD_OP_GLOBAL_CMD,
    /* 0xF */ SEQCMD_OP_RESET_AUDIO_HEAP
} SeqCmdOp;

#define SEQCMD_SET_PLAYER_VOLUME(seqPlayerIndex, duration, volume)                                              \
    Audio_QueueSeqCmd((SEQCMD_OP_SET_PLAYER_VOLUME << 28) | ((u8)(seqPlayerIndex) << 24) | ((duration) << 16) | \
                      (volume))
#define SEQCMD_PLAY_SEQUENCE(seqPlayerIndex, fadeInDuration, seqArg, seqId)                                           \
    Audio_QueueSeqCmd((SEQCMD_OP_PLAY_SEQUENCE << 28) | ((u8)(seqPlayerIndex) << 24) | ((u8)(fadeInDuration) << 16) | \
                      ((u8)(seqArg) << 8) | (u16)(seqId))

typedef struct Jukebox {
    Actor actor;
    DynaPolyActor dyna;
    ColliderCylinder collider;
    JukeboxActionFunc actionFunc;
    s16 timer;
    s16 seqId;
    s16 scrollX;
    bool i;
    bool hasStarted;
    bool playFlag;
    s16 songIDs;
    LightNode* lightNode;
    LightInfo lightInfo;
} Jukebox;

#endif