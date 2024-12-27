#ifndef __CUSTOM_SAVE_H__
#define __CUSTOM_SAVE_H__

#include <z64hdr.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

typedef enum {
    CUSTOM_FLAG_PLACEHOLDER,
    CUSTOM_FLAG_MAX,
} CustomFlagType;

typedef struct {
    u8 flags[CUSTOM_FLAG_MAX];
} CustomSave;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CustomSave* GetCustomSave(void);

void CustomFlag_Set(CustomFlagType flagType, u8 state);
u8 CustomFlag_Get(CustomFlagType flagType);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif