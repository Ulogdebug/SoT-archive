/*
 * File: invisible_trigger_switch.c
 * Overlay: ovl_Invisible_Trigger_Switch
 * Description: Sets a switch flag based on proximity to Link, from the Hylian Modding actor pack.
 * Params: XXYY
 *      -- XX: Scale the radius used in the sphere to detect Link
 *      -- YY: Switch flag (0x00 thru 0x3F)
 */
#include "invisible_trigger_switch.h"

#define FLAGS (ACTOR_FLAG_NO_UPDATE_CULLING )
#define SWITCH_FLAG(this) ((this->actor.params >> 8) & 0xFF)
#define RADIUS_SCALE(this) ((this->actor.params) & 0xFF)

void InvisibleTriggerSwitch_Init(Actor* thisx, PlayState* play);
void InvisibleTriggerSwitch_Destroy(Actor* thisx, PlayState* play);
void InvisibleTriggerSwitch_Update(Actor* thisx, PlayState* play);

void InvisibleTriggerSwitch_WaitForPlayer(InvisibleTriggerSwitch* this, PlayState* play);

const ActorInit Invisible_Trigger_Switch_InitVars = {
    0x0238,
    ACTORCAT_SWITCH,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(InvisibleTriggerSwitch),
    (ActorFunc)InvisibleTriggerSwitch_Init,
    (ActorFunc)InvisibleTriggerSwitch_Destroy,
    (ActorFunc)InvisibleTriggerSwitch_Update,
    NULL,
};


void InvisibleTriggerSwitch_Init(Actor* thisx, PlayState* play) {
    InvisibleTriggerSwitch* this = (InvisibleTriggerSwitch*)thisx;

    this->detectionRadius = (RADIUS_SCALE(this) + 1) * 50.0f;
    this->actionFunc = InvisibleTriggerSwitch_WaitForPlayer;

    if (Flags_GetSwitch(play, SWITCH_FLAG(this))) {
        Actor_Kill(&this->actor);
    }
}

void InvisibleTriggerSwitch_Destroy(Actor* thisx, PlayState* play) {
    InvisibleTriggerSwitch* this = (InvisibleTriggerSwitch*)thisx;
}

void InvisibleTriggerSwitch_Update(Actor* thisx, PlayState* play) {
    InvisibleTriggerSwitch* this = (InvisibleTriggerSwitch*)thisx;

    this->actionFunc(this, play);
}

void InvisibleTriggerSwitch_WaitForPlayer(InvisibleTriggerSwitch* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    if (ABS(Actor_WorldDistXYZToActor(&this->actor, &player->actor)) < this->detectionRadius || (RADIUS_SCALE(this) == 0xFF)) {
        Flags_SetSwitch(play, SWITCH_FLAG(this));
        Actor_Kill(&this->actor);
    }
}
