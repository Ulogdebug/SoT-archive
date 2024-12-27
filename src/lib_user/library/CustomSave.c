#include "uLib.h"
#include "CustomSave.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//ensures that CustomSave struct doesn't go over scarecrowLongSong size
_Static_assert(sizeof(CustomSave) <= sizeof(gSaveContext.scarecrowLongSong), "ran out of space");

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//Gets safely aligned pointer from scarecrowLongSong
CustomSave* GetCustomSave(void) {
    return (void*)ALIGN16((u32)(void*)gSaveContext.scarecrowLongSong);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//S
void CustomFlag_Set(CustomFlagType flagType, u8 state) {
    CustomSave* this = GetCustomSave();
    
    this->flags[flagType] = state;
}

u8 CustomFlag_Get(CustomFlagType flagType) {
    CustomSave* this = GetCustomSave();
    
    return this->flags[flagType];
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */