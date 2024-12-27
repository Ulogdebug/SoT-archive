#ifndef SCREEN_LOG_H
#define SCREEN_LOG_H

#include "ultra64.h"

#if DEV_BUILD
void ScreenLog(u32 data, const char* fmt, ...);
void DrawScreenLogs(void);
#else
#define ScreenLog(...) (void)0
#define DrawScreenLogs(...) (void)0
#endif

#endif