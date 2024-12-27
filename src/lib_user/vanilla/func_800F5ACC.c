#include <uLib.h>
#include <code/code_800EC960.h>

Asm_VanillaHook(func_800F5ACC);
/**
 * Plays a sequence on the main bgm player, but stores the previous sequence to return to later
 * Designed for the mini-boss sequence, but also used by mini-game 2 sequence
 */
void func_800F5ACC(u16 seqId) {
    u16 curSeqId = func_800FA0B4(SEQ_PLAYER_BGM_MAIN);

    if (/*(curSeqId & 0xFF) != NA_BGM_GANON_TOWER && (curSeqId & 0xFF) != NA_BGM_ESCAPE &&*/ curSeqId != seqId) {
        Audio_SetSequenceMode(SEQ_MODE_IGNORE);
        if (curSeqId != NA_BGM_DISABLED) {
            sPrevMainBgmSeqId = curSeqId;
        } else {
            osSyncPrintf("Middle Boss BGM Start not stack \n");
        }

        Audio_StartSeq(SEQ_PLAYER_BGM_MAIN, 0, seqId);
    }
}


/**
 * Restores the previous sequence to the main bgm player before func_800F5ACC was called
 */
Asm_VanillaHook(func_800F5B58);
void func_800F5B58(void) {
    if ((func_800FA0B4(SEQ_PLAYER_BGM_MAIN) != NA_BGM_DISABLED) && (sPrevMainBgmSeqId != NA_BGM_DISABLED) &&
        (sSeqFlags[func_800FA0B4(SEQ_PLAYER_BGM_MAIN) & 0xFF] & SEQ_FLAG_RESTORE)) {
        if (sPrevMainBgmSeqId == NA_BGM_DISABLED) {
            Audio_SeqCmd1(SEQ_PLAYER_BGM_MAIN, 0);
        } else {
            Audio_StartSeq(SEQ_PLAYER_BGM_MAIN, 0, sPrevMainBgmSeqId);
        }

        sPrevMainBgmSeqId = NA_BGM_DISABLED;
    }
}