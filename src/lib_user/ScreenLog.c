#include <z64hdr.h>

extern PlayState gPlayState;
asm ("gPlayState = 0x80212020 - 0x38000;");
asm ("__gfxCtx = gPlayState;");

#ifdef DEV_BUILD

#define BUFFER_SIZE 128
#define LOG_COUNT 40


static struct {
    u32 data;
    char buffer[BUFFER_SIZE];
} sScreenLogEntries[LOG_COUNT];

static u32 ScreenIndex(u32 data) {
    for (u32 i = 0; i < LOG_COUNT; i++)
        if (sScreenLogEntries[i].data == data)
            return i;
    
    for (u32 i = 0; i < LOG_COUNT; i++)
        if (sScreenLogEntries[i].buffer[0] == '\0')
            return sScreenLogEntries[i].data = data, i;
    
    return ~0U;
}

void ScreenLog(u32 data, const char* fmt, ...) {
    u32 i = ScreenIndex(data);
    if (i == ~0U)
        return;
    
    va_list va;
    
    va_start(va, fmt);
    vsprintf(sScreenLogEntries[i].buffer, fmt, va);
    va_end(va);
}

void DrawScreenLogs(void) {
    GfxPrint printer;
    Gfx* opa = POLY_OPA_DISP;
    Gfx* gfx = Graph_GfxPlusOne(opa);
    
    gSPDisplayList(OVERLAY_DISP++, gfx);
    GfxPrint_Init(&printer);
    GfxPrint_Open(&printer, gfx);
    GfxPrint_SetColor(&printer, 255, 255, 255, 255);
    
    for (u32 i = 0; i < LOG_COUNT; i++) {
        if (!sScreenLogEntries[i].buffer[0])
            continue;
        
        GfxPrint_SetPos(&printer, 1, 4+i);
        GfxPrint_Printf(&printer, "%04X %s", sScreenLogEntries[i].data, sScreenLogEntries[i].buffer);
    }
    
    gfx = GfxPrint_Close(&printer);
    GfxPrint_Destroy(&printer);
    gSPEndDisplayList(gfx++);
    Graph_BranchDlist(opa, gfx);
    POLY_OPA_DISP = gfx;
    
    if ((~((gPlayState.state.input[0].press.button) | ~(BTN_A | BTN_B)) == 0))
        for (u32 i = 0; i < LOG_COUNT; i++)
            sScreenLogEntries[i].buffer[0] = '\0', sScreenLogEntries[i].data = 0;
}

#endif