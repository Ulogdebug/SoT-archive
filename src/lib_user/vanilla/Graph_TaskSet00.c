#include <uLib.h>


#define GFXPOOL_HEAD_MAGIC 0x1234
#define GFXPOOL_TAIL_MAGIC 0x5678
#include <code/graph.h>

Asm_VanillaHook(Graph_TaskSet00);
void Graph_TaskSet00(GraphicsContext* gfxCtx) {
    static Gfx* D_8012D260 = NULL;
    static s32 sGraphCfbInfoIdx = 0;

    OSTime time;
    OSTimer timer;
    OSMesg msg;
    OSTask_t* task = &gfxCtx->task.list.t;
    OSScTask* scTask = &gfxCtx->task;
    CfbInfo* cfb;
    s32 pad1;

    D_8016A528 = osGetTime() - sGraphSetTaskTime - D_8016A558;

    osSetTimer(&timer, OS_USEC_TO_CYCLES(3000000), 0, &gfxCtx->queue, (OSMesg)666);

    osRecvMesg(&gfxCtx->queue, &msg, OS_MESG_BLOCK);
    osStopTimer(&timer);

    if (msg == (OSMesg)666) {
        osSyncPrintf(VT_FGCOL(RED));
        osSyncPrintf("RCPが帰ってきませんでした。"); // "RCP did not return."
        osSyncPrintf(VT_RST);
        LogUtils_LogHexDump((void*)&HW_REG(SP_MEM_ADDR_REG, u32), 0x20);
        LogUtils_LogHexDump((void*)&DPC_START_REG, 0x20);
        LogUtils_LogHexDump(gGfxSPTaskYieldBuffer, sizeof(gGfxSPTaskYieldBuffer));

        SREG(6) = -1;
        if (D_8012D260 != NULL) {
            HREG(80) = 7;
            HREG(81) = 1;
            HREG(83) = 2;
            D_8012D260 = D_8012D260;
            Graph_DisassembleUCode(D_8012D260);
        }
        Fault_AddHungupAndCrashImpl("RCP is HUNG UP!!", "Oh! MY GOD!!");
    }

    osRecvMesg(&gfxCtx->queue, &msg, OS_MESG_NOBLOCK);

    D_8012D260 = gfxCtx->workBuffer;
    if (gfxCtx->callback != NULL) {
        gfxCtx->callback(gfxCtx, gfxCtx->callbackParam);
    }

    time = osGetTime();
    if (D_8016A550 != 0) {
        D_8016A558 = (D_8016A558 + time) - D_8016A550;
        D_8016A550 = time;
    }
    D_8016A520 = D_8016A558;
    D_8016A558 = 0;
    sGraphSetTaskTime = osGetTime();

    task->type = M_GFXTASK;
    task->flags = OS_SC_DRAM_DLIST;
    task->ucode_boot = SysUcode_GetUCodeBoot();
    task->ucode_boot_size = SysUcode_GetUCodeBootSize();
    task->ucode = SysUcode_GetUCode();
    task->ucode_data = SysUcode_GetUCodeData();
    task->ucode_size = SP_UCODE_SIZE;
    task->ucode_data_size = SP_UCODE_DATA_SIZE;
    task->dram_stack = gGfxSPTaskStack;
    task->dram_stack_size = sizeof(gGfxSPTaskStack);
    task->output_buff = gGfxSPTaskOutputBuffer;
    task->output_buff_size = gGfxSPTaskOutputBuffer + ARRAY_COUNT(gGfxSPTaskOutputBuffer);
    task->data_ptr = (u64*)gfxCtx->workBuffer;

    OPEN_DISPS(gfxCtx, "../graph.c", 828);
    task->data_size = (uintptr_t)WORK_DISP - (uintptr_t)gfxCtx->workBuffer;
    CLOSE_DISPS(gfxCtx, "../graph.c", 830);

    { s32 pad2; } // Necessary to match stack usage

    task->yield_data_ptr = gGfxSPTaskYieldBuffer;
    task->yield_data_size = sizeof(gGfxSPTaskYieldBuffer);

    scTask->next = NULL;
    scTask->flags = OS_SC_NEEDS_RSP | OS_SC_NEEDS_RDP | OS_SC_SWAPBUFFER | OS_SC_LAST_TASK;
    if (SREG(33) & 1) {
        SREG(33) &= ~1;
        scTask->flags &= ~OS_SC_SWAPBUFFER;
        gfxCtx->fbIdx--;
    }

    scTask->msgQueue = &gfxCtx->queue;
    scTask->msg = NULL;

    cfb = &sGraphCfbInfos[sGraphCfbInfoIdx++];
    cfb->framebuffer = gfxCtx->curFrameBuffer;
    cfb->swapBuffer = gfxCtx->curFrameBuffer;
    cfb->viMode = gfxCtx->viMode;
    cfb->viFeatures = gfxCtx->viFeatures;
    cfb->xScale = gfxCtx->xScale;
    cfb->yScale = gfxCtx->yScale;
    cfb->unk_10 = 0;
    cfb->updateRate = R_UPDATE_RATE;

    scTask->framebuffer = cfb;
    sGraphCfbInfoIdx = sGraphCfbInfoIdx % ARRAY_COUNT(sGraphCfbInfos);

    if (1) {}

    gfxCtx->schedMsgQueue = &gScheduler.cmdQueue;

    osSendMesg(&gScheduler.cmdQueue, (OSMesg)scTask, OS_MESG_BLOCK);
    Sched_Notify(&gScheduler);
}
