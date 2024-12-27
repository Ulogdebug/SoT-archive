#include <uLib.h>
#include <code/code_800EC960.h>

    OcarinaSongButtons hackOcarinaSongButtons[OCARINA_SONG_MAX] = {
    // OCARINA_SONG_MINUET
    { 6,
      {
          OCARINA_BTN_A,
          OCARINA_BTN_C_UP,
          OCARINA_BTN_C_LEFT,
          OCARINA_BTN_C_RIGHT,
          OCARINA_BTN_C_LEFT,
          OCARINA_BTN_C_RIGHT,
      } },
    // OCARINA_SONG_BOLERO
    { 8,
      {
          OCARINA_BTN_C_DOWN,
          OCARINA_BTN_A,
          OCARINA_BTN_C_DOWN,
          OCARINA_BTN_A,
          OCARINA_BTN_C_RIGHT,
          OCARINA_BTN_C_DOWN,
          OCARINA_BTN_C_RIGHT,
          OCARINA_BTN_C_DOWN,
      } },
    // OCARINA_SONG_SERENADE
    { 5,
      {
          OCARINA_BTN_A,
          OCARINA_BTN_C_DOWN,
          OCARINA_BTN_C_RIGHT,
          OCARINA_BTN_C_RIGHT,
          OCARINA_BTN_C_LEFT,
      } },
    // OCARINA_SONG_REQUIEM
    { 6,
      {
          OCARINA_BTN_A,
          OCARINA_BTN_C_DOWN,
          OCARINA_BTN_A,
          OCARINA_BTN_C_RIGHT,
          OCARINA_BTN_C_DOWN,
          OCARINA_BTN_A,
      } },
    // OCARINA_SONG_NOCTURNE
    { 7,
      {
          OCARINA_BTN_C_LEFT,
          OCARINA_BTN_C_RIGHT,
          OCARINA_BTN_C_RIGHT,
          OCARINA_BTN_A,
          OCARINA_BTN_C_LEFT,
          OCARINA_BTN_C_RIGHT,
          OCARINA_BTN_C_DOWN,
      } },
    // OCARINA_SONG_PRELUDE
    { 6,
      {
          OCARINA_BTN_C_UP,
          OCARINA_BTN_C_RIGHT,
          OCARINA_BTN_C_UP,
          OCARINA_BTN_C_RIGHT,
          OCARINA_BTN_C_LEFT,
          OCARINA_BTN_C_UP,
      } },
    // OCARINA_SONG_SARIAS
    { 6,
      {
          OCARINA_BTN_C_DOWN,
          OCARINA_BTN_C_RIGHT,
          OCARINA_BTN_C_LEFT,
          OCARINA_BTN_C_DOWN,
          OCARINA_BTN_C_RIGHT,
          OCARINA_BTN_C_LEFT,
      } },
    // OCARINA_SONG_EPONAS
    { 6,
      {
          OCARINA_BTN_C_UP,
          OCARINA_BTN_C_LEFT,
          OCARINA_BTN_C_RIGHT,
          OCARINA_BTN_C_UP,
          OCARINA_BTN_C_LEFT,
          OCARINA_BTN_C_RIGHT,
      } },
    // OCARINA_SONG_LULLABY
    { 8,
      {
          OCARINA_BTN_C_LEFT,
          OCARINA_BTN_C_RIGHT,
          OCARINA_BTN_C_LEFT,
          OCARINA_BTN_C_RIGHT,
          OCARINA_BTN_C_LEFT,
          OCARINA_BTN_C_LEFT,
          OCARINA_BTN_C_RIGHT,
          OCARINA_BTN_C_LEFT,
      } },
    // OCARINA_SONG_SUNS
    { 6,
      {
          OCARINA_BTN_C_RIGHT,
          OCARINA_BTN_C_DOWN,
          OCARINA_BTN_C_UP,
          OCARINA_BTN_C_RIGHT,
          OCARINA_BTN_C_DOWN,
          OCARINA_BTN_C_UP,
      } },
    // OCARINA_SONG_TIME
    { 6,
      {
          OCARINA_BTN_C_LEFT,
          OCARINA_BTN_C_RIGHT,
          OCARINA_BTN_C_LEFT,
          OCARINA_BTN_C_UP,
          OCARINA_BTN_C_LEFT,
          OCARINA_BTN_C_RIGHT,
      } },
    // OCARINA_SONG_STORMS
    { 6,
      {
          OCARINA_BTN_A,
          OCARINA_BTN_C_DOWN,
          OCARINA_BTN_C_UP,
          OCARINA_BTN_A,
          OCARINA_BTN_C_DOWN,
          OCARINA_BTN_C_UP,
      } },
    // OCARINA_SONG_SCARECROW_SPAWN
    { 8, { 0 } },
    // OCARINA_SONG_MEMORY_GAME
    { 0, { 0 } },
};

OcarinaNote hackOcarinaSongNotes[OCARINA_SONG_MAX][20] = {
    // OCARINA_SONG_MINUET
    {
        { OCARINA_PITCH_D4, 18, 86, 0, 0, 0 },
        { OCARINA_PITCH_D5, 18, 92, 0, 0, 0 },
        { OCARINA_PITCH_B4, 72, 86, 0, 0, 0 },
        { OCARINA_PITCH_A4, 18, 80, 0, 0, 0 },
        { OCARINA_PITCH_B4, 18, 88, 0, 0, 0 },
        { OCARINA_PITCH_A4, 144, 86, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 0, 86, 0, 0, 0 },
    },

    // OCARINA_SONG_BOLERO
    {
        { OCARINA_PITCH_F4, 15, 80, 0, 0, 0 },
        { OCARINA_PITCH_D4, 15, 72, 0, 0, 0 },
        { OCARINA_PITCH_F4, 15, 84, 0, 0, 0 },
        { OCARINA_PITCH_D4, 15, 76, 0, 0, 0 },
        { OCARINA_PITCH_A4, 15, 84, 0, 0, 0 },
        { OCARINA_PITCH_F4, 15, 74, 0, 0, 0 },
        { OCARINA_PITCH_A4, 15, 78, 0, 0, 0 },
        { OCARINA_PITCH_F4, 135, 66, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 0, 66, 0, 0, 0 },
    },

    // OCARINA_SONG_SERENADE
    {
        { OCARINA_PITCH_D4, 36, 60, 0, 0, 0 },
        { OCARINA_PITCH_F4, 36, 78, 0, 0, 0 },
        { OCARINA_PITCH_A4, 33, 82, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 3, 82, 0, 0, 0 },
        { OCARINA_PITCH_A4, 36, 84, 0, 0, 0 },
        { OCARINA_PITCH_B4, 144, 90, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 0, 90, 0, 0, 0 },
    },

    // OCARINA_SONG_REQUIEM
    {
        { OCARINA_PITCH_D4, 45, 88, 0, 0, 0 },
        { OCARINA_PITCH_F4, 23, 86, 0, 0, 0 },
        { OCARINA_PITCH_D4, 22, 84, 0, 0, 0 },
        { OCARINA_PITCH_A4, 45, 86, 0, 0, 0 },
        { OCARINA_PITCH_F4, 45, 94, 0, 0, 0 },
        { OCARINA_PITCH_D4, 180, 94, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 0, 94, 0, 0, 0 },
    },

    // OCARINA_SONG_NOCTURNE
    {
        { OCARINA_PITCH_B4, 36, 88, 0, 0, 0 },
        { OCARINA_PITCH_A4, 33, 84, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 3, 84, 0, 0, 0 },
        { OCARINA_PITCH_A4, 18, 82, 0, 0, 0 },
        { OCARINA_PITCH_D4, 18, 60, 0, 0, 0 },
        { OCARINA_PITCH_B4, 18, 90, 0, 0, 0 },
        { OCARINA_PITCH_A4, 18, 88, 0, 0, 0 },
        { OCARINA_PITCH_F4, 144, 96, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 0, 96, 0, 0, 0 },
    },

    // OCARINA_SONG_PRELUDE
    {
        { OCARINA_PITCH_D5, 15, 84, 0, 0, 0 },
        { OCARINA_PITCH_A4, 45, 88, 0, 0, 0 },
        { OCARINA_PITCH_D5, 15, 88, 0, 0, 0 },
        { OCARINA_PITCH_A4, 15, 82, 0, 0, 0 },
        { OCARINA_PITCH_B4, 15, 86, 0, 0, 0 },
        { OCARINA_PITCH_D5, 60, 90, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 75, 90, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 0, 90, 0, 0, 0 },
    },

    // OCARINA_SONG_SARIAS
    {
        { OCARINA_PITCH_F4, 17, 84, 0, 0, 0 },
        { OCARINA_PITCH_A4, 17, 88, 0, 0, 0 },
        { OCARINA_PITCH_B4, 34, 80, 0, 0, 0 },
        { OCARINA_PITCH_F4, 17, 84, 0, 0, 0 },
        { OCARINA_PITCH_A4, 17, 88, 0, 0, 0 },
        { OCARINA_PITCH_B4, 136, 80, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 0, 90, 0, 0, 0 },
    },

    // OCARINA_SONG_EPONAS
    {
        { OCARINA_PITCH_D5, 18, 84, 0, 0, 0 },
        { OCARINA_PITCH_B4, 18, 88, 0, 0, 0 },
        { OCARINA_PITCH_A4, 72, 80, 0, 0, 0 },
        { OCARINA_PITCH_D5, 18, 84, 0, 0, 0 },
        { OCARINA_PITCH_B4, 18, 88, 0, 0, 0 },
        { OCARINA_PITCH_A4, 144, 80, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 0, 90, 0, 0, 0 },
    },

    // OCARINA_SONG_LULLABY, slave's song
    {
        { OCARINA_PITCH_B4, 12, 84, 0, 0, 0 },//L
        { OCARINA_PITCH_A4, 12, 88, 0, 0, 0 },//R
        { OCARINA_PITCH_B4, 40, 80, 0, 0, 0 },//L
        { OCARINA_PITCH_A4, 10, 84, 0, 0, 0 },
        { OCARINA_PITCH_B4, 10, 88, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 9, 90, 0, 0, 0 },
        { OCARINA_PITCH_B4, 10, 85, 0, 0, 0 },
        { OCARINA_PITCH_A4, 10, 90, 0, 0, 0 },
        { OCARINA_PITCH_B4, 120, 84, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 0, 90, 0, 0, 0 },
    },

    // OCARINA_SONG_SUNS
    {
        { OCARINA_PITCH_A4, 12, 84, 0, 0, 0 },
        { OCARINA_PITCH_F4, 13, 88, 0, 0, 0 },
        { OCARINA_PITCH_D5, 29, 80, 2, 0, 0 },
        { OCARINA_PITCH_NONE, 9, 84, 0, 0, 0 },
        { OCARINA_PITCH_A4, 12, 84, 0, 0, 0 },
        { OCARINA_PITCH_F4, 13, 88, 0, 0, 0 },
        { OCARINA_PITCH_D5, 120, 80, 3, 0, 0 },
        { OCARINA_PITCH_NONE, 0, 90, 0, 0, 0 },
    },

    // OCARINA_SONG_TIME song of flames
    {
        { OCARINA_PITCH_B4, 25, 84, 0, 0, 0 },
        { OCARINA_PITCH_A4, 25, 88, 0, 0, 0 },
        { OCARINA_PITCH_A4, 18, 80, 0, 0, 0 },
        { OCARINA_PITCH_A4, 8, 80, 0, 0, 0 },
        { OCARINA_PITCH_B4, 8, 84, 0, 0, 0 },
        { OCARINA_PITCH_D5, 12, 88, 0, 0, 0 },
        { OCARINA_PITCH_B4, 12, 80, 0, 0, 0 },
        { OCARINA_PITCH_B4, 12, 80, 0, 0, 0 },
        { OCARINA_PITCH_A4, 120, 135, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 0, 66, 0, 0, 0 },


    },

    // OCARINA_SONG_STORMS
    {
        { OCARINA_PITCH_D4, 11, 84, 0, 0, 0 },
        { OCARINA_PITCH_F4, 11, 88, 0, 0, 0 },
        { OCARINA_PITCH_D5, 45, 80, 0, 0, 0 },
        { OCARINA_PITCH_D4, 11, 84, 0, 0, 0 },
        { OCARINA_PITCH_F4, 11, 88, 0, 0, 0 },
        { OCARINA_PITCH_D5, 90, 80, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 0, 90, 0, 0, 0 },
    },

    // OCARINA_SONG_SCARECROW_SPAWN
    {
        { OCARINA_PITCH_D4, 3, 0, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 0, 255, 0, 0, 0 },
    },

    // OCARINA_SONG_MEMORY_GAME
    {
        { OCARINA_PITCH_D4, 3, 0, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 0, 0, 0, 0, 0 },
    },
};


Asm_VanillaHook(Audio_Init);
void Audio_Init(void) {
    s16 a,b,numButtons = 0;
    for(a=0;a<OCARINA_SONG_MAX;a++){
        numButtons = hackOcarinaSongButtons[a].numButtons;
        gOcarinaSongButtons[a].numButtons = numButtons;
        for(b=0;b<numButtons;b++){
            gOcarinaSongButtons[a].buttonsIndex[b] = hackOcarinaSongButtons[a].buttonsIndex[b];
        }
        
    }  
    for(a=0;a<OCARINA_SONG_MAX;a++){
        for(b=0;b<20;b++){           
            sOcarinaSongNotes[a][b].pitch = hackOcarinaSongNotes[a][b].pitch;
            sOcarinaSongNotes[a][b].length = hackOcarinaSongNotes[a][b].length;
            sOcarinaSongNotes[a][b].volume = hackOcarinaSongNotes[a][b].volume;
            sOcarinaSongNotes[a][b].bend = hackOcarinaSongNotes[a][b].bend;
            sOcarinaSongNotes[a][b].bFlat4Flag = hackOcarinaSongNotes[a][b].bFlat4Flag;
        }
        
    } 
    AudioLoad_Init(NULL, 0);
}