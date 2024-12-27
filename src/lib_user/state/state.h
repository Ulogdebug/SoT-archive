#ifndef HACK_STATE_H
#define HACK_STATE_H

#include <uLib.h>

typedef enum {
    /* 0x00 */ NOT_DISARMED,
    /* 0x01 */ STRIPED_OFF,
    /* 0x02 */ SHACKLES_SETUP_LOCKED,
    /* 0x03 */ SHACKLES_LOCKED,
    /* 0x04 */ SHACKLES_SETUP_UNLOCKED,
    /* 0x05 */ SHACKLES_UNLOCKED,
} disarmedState;

typedef enum {
    FIRST_CAPTURE = 0x18,
    FREE_SHACKLES = 0xB,
    LOCKED_SHACKLES = 0xC,
    SWORD_BACK = 0xE,
} custom_disarmedFlag;

typedef enum {
    HAS_SOLD = 0x16,
    MOVE_SWEETSHOP = 0x17,
    FACTORY_BOSS_DIE = 0x5,
    FPS_MODE_SWITCH = 0x7,
    POE_ALL_REWARDED = 0x15,
    GERUDO_CAPTURE = 0x25,
    HAS_BIG_SOWRD = 0x21,
    BLUE_GLIDER = 0x22,
} custom_specialEventFlag;

typedef struct {
    u16 hudFadeTimer;
    u16 warpsongmode;
    u8 gotManuallyShocked;
    s16 quickPotionItem;
    LightNode* lightNode;
    LightInfo lightInfo;
    u8 disArmed;
    u8 grabCoolTimer;
} hackState;


extern hackState ghackSave;

#endif