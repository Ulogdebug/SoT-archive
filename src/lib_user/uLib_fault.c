#if __has_include("uLib.h")
// z64rom
#include <uLib.h>
#include <uLib_math.h>
#include <code/fault.h>
#include <code/fault_drawer.h>
#include <code/graph.h>
#include <code/main.h>
#include <boot/idle.h>
#include <boot/z_std_dma.h>
#include <boot/boot_main.h>
#include <code/z_parameter.h>
#include <code/z_kaleido_setup.h>
#include <code/z_actor.h>
#else
// decomp, maybe
#include <global.h>

extern OSThread sIdleThread;
extern OSThread gMainThread;
extern OSThread sGraphThread;
extern OSThread sDmaMgrThread;
extern AudioMgr gAudioMgr;
extern FaultMgr* sFaultInstance;
extern void Fault_Sleep(u32 msec);
extern void Fault_UpdatePadImpl(void);
extern void FaultDrawer_FillScreen(void);
extern OSThread* Fault_FindFaultedThread(void);
extern void Fault_UpdatePad(void);
extern void Fault_DisplayFrameBuffer(void);
extern void Fault_HungupFaultClient(const char* exp1, const char* exp2);
#define FAULT_MSG_CPU_BREAK ((OSMesg)1)
#define FAULT_MSG_FAULT     ((OSMesg)2)
#define FAULT_MSG_UNK       ((OSMesg)3)

#define Asm_VanillaHook(...)

static inline void SoundEffect_PlayOneshot(u32 id, ...) {
	Audio_PlaySfxGeneral(id, &gSfxDefaultPos, 4, &gSfxDefaultFreqAndVolScale, &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
}

static inline s32 WrapS(s32 x, s32 min, s32 max) {
	s32 range = max - min;
	
	if (x < min)
		x += range * ((min - x) / range + 1);
	
	return min + (x - min) % range;
}
#endif

static Actor* sActiveActor;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

enum {
	OP_INIT,
	OP_DESTROY,
	OP_UPDATE,
	OP_DRAW,
};

struct FaultBook;

typedef struct {
	void (*func)(struct FaultBook*, OSThread*);
} FaultPage;

typedef struct FaultBook {
	FaultPage page[16];
	int       num;
	int       cur;
} FaultBook;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static u16 sFaultActorID = 0xFFFF;
static u16 sFaultActorVariable = 0;
static u8 sFaultActorOperation = 0;
static Actor* sFaultActor;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define CCC 0.12156862745f
#define R5G5B5A1(r, g, b) (((u16)(r * CCC) << 11) | ((u16)(g * CCC) << 6) | ((u16)(b * CCC) << 1)) | 1

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Asm_VanillaHook(Fault_ThreadEntry);
Asm_VanillaHook(Fault_AddHungupAndCrashImpl);
Asm_VanillaHook(KaleidoSetup_Update);
Asm_VanillaHook(Actor_Init);
Asm_VanillaHook(Actor_UpdateAll);
Asm_VanillaHook(Actor_Draw);
Asm_VanillaHook(Actor_Destroy);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static const char* sFailNames[] = {
	"Interrupt",
	"TLB Mod",
	"TLB Read",
	"TLB Write",
	"Addr Read",
	"Addr Write",
	"Bus Inst",
	"Bus Data",
	"SysCall",
	"Breakpoint",
	"Reserved",
	"CoProc Unusable",
	"Arithmetic OF",
	"Trap",
	"VCoherency Inst",
	"Float Exception",
	"Watchpoint",
	"VCoherency Data",
};

const char* sFloatFailNames[] = {
	"Unimplemented OP",
	"Invalid OP",
	"Zero Div",
	"Overflow",
	"Underflow",
	"Inexact OP",
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static const char* GetActorName() {
	return NULL;
}

static void PrintActorData() {
	if (sFaultActorID != 0xFFFF) {
		FaultDrawer_SetFontColor(0xFFFF);
		
		switch (sFaultActorOperation) {
			case OP_INIT:
				FaultDrawer_Printf("\nActor%s: %04X:%04X ", "Init", (u16)sFaultActorID, (u16)sFaultActorVariable);
				break;
			case OP_DESTROY:
				FaultDrawer_Printf("\nActor%s: %04X:%04X ", "Kill", (u16)sFaultActorID, (u16)sFaultActorVariable);
				break;
			case OP_UPDATE:
				FaultDrawer_Printf("\nActor%s: %04X:%04X ", "Update", (u16)sFaultActorID, (u16)sFaultActorVariable);
				break;
			case OP_DRAW:
				FaultDrawer_Printf("\nActor%s: %04X:%04X ", "Draw", (u16)sFaultActorID, (u16)sFaultActorVariable);
				break;
		}
		
		const char* name = GetActorName();
		if (name) {
			FaultDrawer_SetFontColor(R5G5B5A1(0, 0x20, 0xFF));
			FaultDrawer_Printf("%s", name);
			FaultDrawer_SetFontColor(0xFFFF);
		}
		FaultDrawer_Printf("\n");
	}
}

static volatile int sAssert;

static bool IsAssert(OSThread* thread) {
	if (sAssert) {
		sAssert = false;
		
		return true;
	}
	
	return false;
}

static void FaultBook_AddPage(FaultBook* this, void* func) {
	this->page[this->num++].func = func;
}

static OSThread* GetThread(int id) {
	OSThread* thd[] = {
		[THREAD_ID_IDLE] = &sIdleThread,
		[THREAD_ID_FAULT] = NULL,
		[THREAD_ID_MAIN] = &gMainThread,
		[THREAD_ID_GRAPH] = &sGraphThread,
		[THREAD_ID_SCHED] = NULL,
		[THREAD_ID_PADMGR] = NULL,
		[THREAD_ID_AUDIOMGR] = &gAudioMgr.thread,
		[THREAD_ID_DMAMGR] = &sDmaMgrThread,
		[THREAD_ID_IRQMGR] = NULL,
	};
	
	return thd[id];
}

static const char* GetThreadName(int id) {
	const char* thd[] = {
		[THREAD_ID_IDLE] = "T:Idle",
		[THREAD_ID_FAULT] = "T:Fault",
		[THREAD_ID_MAIN] = "T:Main",
		[THREAD_ID_GRAPH] = "T:Graph",
		[THREAD_ID_SCHED] = "T:Sched",
		[THREAD_ID_PADMGR] = "T:PadMgr",
		[THREAD_ID_AUDIOMGR] = "T:AudioMgr",
		[THREAD_ID_DMAMGR] = "T:DmaMgr",
		[THREAD_ID_IRQMGR] = "T:IrqMgr",
	};
	
	return thd[id];
}

static int IsThread(int id) {
	return GetThread(id) == sFaultInstance->faultedThread;
}

static void OsFaultSound() {
	static int id = -1;
	int ID[] = {
		0,
	};
	
	if (id == -1)
		id = Rand_S16Offset(0, ARRAY_COUNT(ID) * 2.5f) % ARRAY_COUNT(ID);
	
	if (!IsThread(THREAD_ID_AUDIOMGR) && !IsThread(THREAD_ID_DMAMGR))
		SoundEffect_PlayOneshot(ID[id], 0.75f, 1.0f, NULL, 0.0f, 0.0f, (void*)235);
	id = WrapS(id + 1, 0, ARRAY_COUNT(ID));
}

static int InputHandler() {
	Input* input = &sFaultInstance->inputs[0];
	
	while (true) {
		Fault_Sleep(1000 / 60);
		func_800F3054();
		Fault_UpdatePadImpl();
		bool sound = !IsThread(THREAD_ID_AUDIOMGR) && !IsThread(THREAD_ID_DMAMGR);
		
		if (input->press.button & BTN_START) {
			// exit debugger
			if (sound) SoundEffect_PlayOneshot(0, 0.2f, 1.0f, NULL, 0.0f, 0.0f, (void*)0x2345);
			return 4;
		}
		
		if (input->press.button & BTN_DUP) {
			// nop
			if (sound) SoundEffect_PlayOneshot(0, 0.2f, 1.4f, NULL, 0.0f, 0.0f, (void*)0x2345);
			return 2;
		}
		
		if (input->press.button & BTN_DDOWN) {
			// nop
			if (sound) SoundEffect_PlayOneshot(0, 0.2f, 1.5f, NULL, 0.0f, 0.0f, (void*)0x2345);
			return -2;
		}
		
		if (input->press.button & BTN_A) {
			// nop
			if (sound) SoundEffect_PlayOneshot(0, 0.2f, 1.5f, NULL, 0.0f, 0.0f, (void*)0x2345);
			return 3;
		}
		
		if (input->press.button & BTN_B) {
			// nop
			if (sound) SoundEffect_PlayOneshot(0, 0.2f, 1.5f, NULL, 0.0f, 0.0f, (void*)0x2345);
			return -3;
		}
		
		if (input->press.button & (BTN_DRIGHT | BTN_R | BTN_CRIGHT)) {
			// scroll left
			if (sound) SoundEffect_PlayOneshot(0, 0.2f, 1.0f, NULL, 0.0f, 0.0f, (void*)0x2345);
			return 1;
		}
		
		if (input->press.button & (BTN_DLEFT | BTN_L | BTN_Z | BTN_CLEFT)) {
			// scroll right
			if (sound) SoundEffect_PlayOneshot(0, 0.2f, 1.0f, NULL, 0.0f, 0.0f, (void*)0x2345);
			return -1;
		}
		
		if (input->press.button & (BTN_CDOWN))
			OsFaultSound();
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static void FaultBook_Header(OSThread* thread, const char* msg, const char* msg2) {
	s16 causeStrIdx = _SHIFTR((u32)thread->context.cause, 2, 5);
	
	if (causeStrIdx == 23)
		// Watchpoint
		causeStrIdx = 16;
	
	if (causeStrIdx == 31)
		// Virtual coherency on data
		causeStrIdx = 17;
	
	FaultDrawer_FillScreen();
	FaultDrawer_SetCharPad(-1, 0);
	FaultDrawer_SetCursor(22, 20);
	FaultDrawer_SetFontColor(R5G5B5A1(255, 0, 64));
	FaultDrawer_Printf("%s ", msg);
	FaultDrawer_SetFontColor(R5G5B5A1(255, 255, 255));
	if (!msg2)
		FaultDrawer_Printf("%s", sFailNames[causeStrIdx]);
	else
		FaultDrawer_Printf("%s", msg2);
	FaultDrawer_Printf("\n", thread->id, causeStrIdx, sFailNames[causeStrIdx]);
}

static char sActorAssertMessage[256];

static void FaultPage_Assert(FaultBook* book, OSThread* thread) {
	FaultClient* client = sFaultInstance->clients;
	
	FaultBook_Header(thread, "Assert:", "");
	FaultDrawer_Printf("\n");
	
	FaultDrawer_SetFontColor(R5G5B5A1(255, 255, 255));
	if (client->arg0)
		FaultDrawer_Printf("%s\n\n", client->arg0);
	FaultDrawer_SetFontColor(R5G5B5A1(220, 220, 220));
	if (client->arg1 && client->arg1 != (void*)0xDEADBEEF)
		FaultDrawer_Printf("%s\n", client->arg1);
	if (sActorAssertMessage[0])
		FaultDrawer_Printf("\n%s\n", sActorAssertMessage);
	PrintActorData();
}

static void FaultPage_MipsRegs(FaultBook* book, OSThread* thread) {
	__OSThreadContext* ctx = &thread->context;
	
	FaultBook_Header(thread, "Registers:", NULL);
	FaultDrawer_Printf("\n");
	
	FaultDrawer_SetFontColor(0xFFFF);
	FaultDrawer_Printf("PC ");
	FaultDrawer_SetFontColor(0xFF0F);
	FaultDrawer_Printf("%08X ", (u32)ctx->pc);
	
	if (sFaultActorID != 0xFFFF && sFaultActor) {
		u32 loadedRam = 0;
		u32 vram = 0;
		u32 size = 0;
		
		if (sFaultActorID == 0) {
			loadedRam = (u32)gKaleidoMgrCurOvl->loadedRamAddr;
			vram = (u32)gKaleidoMgrCurOvl->vramStart;
			size = (u32)gKaleidoMgrCurOvl->vramEnd - vram;
		} else {
			loadedRam = (u32)sFaultActor->overlayEntry->loadedRamAddr;
			vram = (u32)sFaultActor->overlayEntry->vramStart;
			size = (u32)sFaultActor->overlayEntry->vramEnd - vram;
		}
		
		if ((u32)ctx->pc >= loadedRam && (u32)ctx->pc < loadedRam + size) {
			FaultDrawer_SetFontColor(0xFFFF);
			FaultDrawer_Printf("RL ");
			FaultDrawer_SetFontColor(0xFF0F);
			FaultDrawer_Printf("%08X", vram + ((u32)ctx->pc - loadedRam));
		}
	}
	
	struct {
		const char* reg;
		u64* value;
	} regs[][3] = {
		{ { "A0", &ctx->a0 }, { "S0", &ctx->s0 }, { "T0", &ctx->t0 }, },
		{ { "A1", &ctx->a1 }, { "S1", &ctx->s1 }, { "T1", &ctx->t1 }, },
		{ { "A2", &ctx->a2 }, { "S2", &ctx->s2 }, { "T2", &ctx->t2 }, },
		{ { "A3", &ctx->a3 }, { "S3", &ctx->s3 }, { "T3", &ctx->t3 }, },
		{ { NULL, NULL }, { "S4", &ctx->s4 }, { "T4", &ctx->t4 }, },
		{ { NULL, NULL }, { "S5", &ctx->s5 }, { "T5", &ctx->t5 }, },
		{ { "AT", &ctx->at }, { "S6", &ctx->s6 }, { "T6", &ctx->t6 }, },
		{ { "GP", &ctx->gp }, { "S7", &ctx->s7 }, { "T7", &ctx->t7 }, },
		{ { "SP", &ctx->sp }, { "S8", &ctx->s8 }, { "T8", &ctx->t8 }, },
		{ { "RA", &ctx->ra }, { NULL, NULL }, { "T9", &ctx->t9 }, },
		{},
		{ { "V0", &ctx->v0 }, { "AT", &ctx->at }, { "SP", &ctx->sp } },
		{ { "V1", &ctx->v1 }, { "GP", &ctx->gp }, { "RA", &ctx->ra } },
	};
	
	for (int i = 0; i < ARRAY_COUNT(regs); i++) {
		FaultDrawer_Printf("\n");
		for (int j = 0; j < ARRAY_COUNT(regs[0]); j++) {
			if (!regs[i][j].reg) {
				FaultDrawer_Printf("%s %s ", "  ", "        ");
				continue;
			}
			
			FaultDrawer_SetFontColor(R5G5B5A1(255, 255, 255));
			FaultDrawer_Printf("%s ", regs[i][j].reg);
			FaultDrawer_SetFontColor(R5G5B5A1(255, 0, 64));
			FaultDrawer_Printf("%08X ", (u32) * regs[i][j].value);
		}
	}
	
	FaultDrawer_Printf("\n");
	PrintActorData();
	FaultDrawer_Printf("\n");
	
	GfxPool* pool = &gGfxPools[__gfxCtx->gfxPoolIdx & 1];
	static const f32 div = 1.0f / sizeof(Gfx);
	
	u32 opa = (((u32)__gfxCtx->polyOpa.p - (u32)__gfxCtx->polyOpa.bufp) * div / (f32)ARRAY_COUNT(pool->polyOpaBuffer)) * 100.0f;
	u32 xlu = (((u32)__gfxCtx->polyXlu.p - (u32)__gfxCtx->polyXlu.bufp) * div / (f32)ARRAY_COUNT(pool->polyXluBuffer)) * 100.0f;
	u32 ovl = (((u32)__gfxCtx->overlay.p - (u32)__gfxCtx->overlay.bufp) * div / (f32)ARRAY_COUNT(pool->overlayBuffer)) * 100.0f;
	
	FaultDrawer_SetFontColor(R5G5B5A1(255, 255, 255));
	FaultDrawer_Printf("OPA Buffer:    ");
	FaultDrawer_SetFontColor(R5G5B5A1(255, 0, 64));
	FaultDrawer_Printf("%08X %3d%%\n", __gfxCtx->polyOpaBuffer, ABS(opa));
	
	FaultDrawer_SetFontColor(R5G5B5A1(255, 255, 255));
	FaultDrawer_Printf("XLU Buffer:    ");
	FaultDrawer_SetFontColor(R5G5B5A1(255, 0, 64));
	FaultDrawer_Printf("%08X %3d%%\n", __gfxCtx->polyXluBuffer, ABS(xlu));
	
	FaultDrawer_SetFontColor(R5G5B5A1(255, 255, 255));
	FaultDrawer_Printf("OVL Buffer:    ");
	FaultDrawer_SetFontColor(R5G5B5A1(255, 0, 64));
	FaultDrawer_Printf("%08X %3d%%\n", __gfxCtx->overlayBuffer, ABS(ovl));
}

static void FaultPage_FloatRegs(FaultBook* book, OSThread* thread) {
	__OSThreadContext* ctx = &thread->context;
	u32 flag = FPCSR_CE;
	const char* tinfo = "";
	
	for (int i = 0; i < ARRAY_COUNT(sFloatFailNames); i++) {
		if (ctx->fpcsr & flag) {
			tinfo = sFloatFailNames[i];
			break;
		}
		flag >>= 1;
	}
	
	FaultBook_Header(thread, "Float Registers:", tinfo);
	FaultDrawer_Printf("\n");
	
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 2; j++) {
			__OSfp* fp;
			
			if (!j) fp = &ctx->fp0;
			else fp = &ctx->fp16;
			
			fp += i;
			
			u32 raw = *(u32*)&fp->f.f_even;
			s32 exp = ((raw & 0x7F800000) >> 0x17) - 0x7F;
			int idx = i * 2 + j * 16;
			
			if ((exp >= -0x7E && exp < 0x80) || raw == 0) {
				FaultDrawer_SetFontColor(R5G5B5A1(255, 255, 255));
				FaultDrawer_Printf("F%02d ", idx, fp->f.f_even);
				
				FaultDrawer_SetFontColor(R5G5B5A1(255, 0, 64));
				FaultDrawer_Printf("%14.7e ", idx, fp->f.f_even);
			} else {
				// Print subnormal floats as their ieee-754 hex representation
				FaultDrawer_SetFontColor(R5G5B5A1(255, 255, 255));
				FaultDrawer_Printf("F%02d ", idx, raw);
				
				FaultDrawer_SetFontColor(R5G5B5A1(255, 0, 64));
				FaultDrawer_Printf("      %08X ", raw);
			}
		}
		
		FaultDrawer_Printf("\n");
	}
}

static void FaultBook_Footer(FaultBook* book, OSThread* thread) {
	FaultDrawer_SetCursor(22, 214);
	FaultDrawer_SetFontColor(R5G5B5A1(255, 255, 255));
	FaultDrawer_Printf("Platform: ");
	FaultDrawer_SetFontColor(R5G5B5A1(255, 0, 64));
	FaultDrawer_Printf("%s", "undefined");
	
	FaultDrawer_SetCursor(22 + 140, 214);
	FaultDrawer_SetFontColor(R5G5B5A1(255, 255, 255));
	FaultDrawer_Printf("%s", GetThreadName(thread->id));
	
	FaultDrawer_SetCursor(22 + 230, 214);
	FaultDrawer_SetFontColor(R5G5B5A1(255, 255, 255));
	FaultDrawer_Printf("%d / %d", book->cur + 1, book->num);
	
	static const char ul[40] = {
		[0 ... 38] = '_',
	};
	
	FaultDrawer_SetCursor(22, 202);
	FaultDrawer_SetFontColor(R5G5B5A1(255, 0, 64));
	FaultDrawer_Printf("%s", ul);
	FaultDrawer_SetCursor(22, 22);
	FaultDrawer_Printf("%s", ul);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static int sFaulted;

void Fault_ThreadEntry(void* arg) {
	OSMesg msg;
	OSThread* faultedThread;
	
	// Direct OS event messages to the fault event queue
	osSetEventMesg(OS_EVENT_CPU_BREAK, &sFaultInstance->queue, FAULT_MSG_CPU_BREAK);
	osSetEventMesg(OS_EVENT_FAULT, &sFaultInstance->queue, FAULT_MSG_FAULT);
	
	FaultBook book = {};
	
	while (true) {
		do {
			osRecvMesg(&sFaultInstance->queue, &msg, OS_MESG_BLOCK);
			
			if (msg == FAULT_MSG_CPU_BREAK)
				sFaultInstance->msgId = (u32)FAULT_MSG_CPU_BREAK;
			
			else if (msg == FAULT_MSG_FAULT)
				sFaultInstance->msgId = (u32)FAULT_MSG_FAULT;
			
			else if (msg == FAULT_MSG_UNK) {
				Fault_UpdatePad();
				faultedThread = NULL;
				continue;
				
			} else
				sFaultInstance->msgId = (u32)FAULT_MSG_UNK;
			
			faultedThread = __osGetCurrFaultedThread();
			
			if (faultedThread == NULL)
				faultedThread = Fault_FindFaultedThread();
		} while (faultedThread == NULL);
		
		__osSetFpcCsr(__osGetFpcCsr() & ~(FPCSR_EV | FPCSR_EZ | FPCSR_EO | FPCSR_EU | FPCSR_EI));
		sFaultInstance->faultedThread = faultedThread;
		
		while (!sFaultInstance->faultHandlerEnabled)
			Fault_Sleep(10);
		Fault_Sleep(1000 / 2);
		
		sFaultInstance->autoScroll = false;
		sFaulted = true;
		
		if (IsAssert(faultedThread)) {
			FaultBook_AddPage(&book, FaultPage_Assert);
			FaultBook_AddPage(&book, FaultPage_MipsRegs);
		} else {
			FaultBook_AddPage(&book, FaultPage_MipsRegs);
			FaultBook_AddPage(&book, FaultPage_FloatRegs);
			FaultBook_AddPage(&book, FaultPage_Assert);
		}
		
		OsFaultSound();
		Fault_DisplayFrameBuffer();
		FaultDrawer_SetForeColor(GPACK_RGBA5551(255, 255, 255, 1));
		FaultDrawer_SetBackColor(GPACK_RGBA5551(0, 0, 0, 0));
		FaultDrawer_FillScreen();
		
		do {
			redraw:
			book.page[book.cur].func(&book, faultedThread);
			FaultBook_Footer(&book, faultedThread);
			
			int input;
			
			switch ((input = InputHandler())) {
				case 2:
				case -2:
					goto redraw;
					break;
					
				case 3:
				case -3:
					goto redraw;
					break;
					
				case 4:
					book = (FaultBook) {};
					goto resume;
					break;
			}
			
			int nextPage = book.cur + input;
			book.cur = WrapS(book.cur + input, 0, book.num);
			
			if (!IsThread(THREAD_ID_AUDIOMGR) && !IsThread(THREAD_ID_DMAMGR)) {
				if (book.cur != nextPage) {
					
				}
			}
		} while (!sFaultInstance->exit);
		
		resume:
		sActorAssertMessage[0] = '\0';
		__osFaultedThread = NULL;
		osStartThread(faultedThread);
	}
}

void KaleidoSetup_Update(PlayState* play) {
	PauseContext* pauseCtx = &play->pauseCtx;
	Input* input = &play->state.input[0];
	
	if (sFaulted) {
		sFaulted = false;
		return;
	}
	
	if (pauseCtx->state == 0 && pauseCtx->debugState == 0 && play->gameOverCtx.state == GAMEOVER_INACTIVE &&
		play->transitionTrigger == TRANS_TRIGGER_OFF && play->transitionMode == TRANS_MODE_OFF &&
		gSaveContext.cutsceneIndex < 0xFFF0 && gSaveContext.nextCutsceneIndex < 0xFFF0 && !Play_InCsMode(play) &&
		play->shootingGalleryStatus <= 1 && gSaveContext.magicState != MAGIC_STATE_STEP_CAPACITY &&
		gSaveContext.magicState != MAGIC_STATE_FILL &&
		(play->sceneId != SCENE_BOWLING || !Flags_GetSwitch(play, 0x38))) {
		
		if (CHECK_BTN_ALL(input->cur.button, BTN_L) && CHECK_BTN_ALL(input->press.button, BTN_CUP)) {
			if (BREG(0)) {
				pauseCtx->debugState = 3;
			}
		} else if (CHECK_BTN_ALL(input->press.button, BTN_START)) {
			gSaveContext.unk_13EE = gSaveContext.unk_13EA;
			
			WREG(16) = -175;
			WREG(17) = 155;
			
			pauseCtx->unk_1EA = 0;
			pauseCtx->unk_1E4 = 1;
			
			if (ZREG(48) == 0) {
				pauseCtx->eye.x = sKaleidoSetupEyeX0[pauseCtx->pageIndex];
				pauseCtx->eye.z = sKaleidoSetupEyeZ0[pauseCtx->pageIndex];
				pauseCtx->pageIndex = sKaleidoSetupKscpPos0[pauseCtx->pageIndex];
			} else {
				pauseCtx->eye.x = sKaleidoSetupEyeX1[pauseCtx->pageIndex];
				pauseCtx->eye.z = sKaleidoSetupEyeZ1[pauseCtx->pageIndex];
				pauseCtx->pageIndex = sKaleidoSetupKscpPos1[pauseCtx->pageIndex];
			}
			
			pauseCtx->mode = (u16)(pauseCtx->pageIndex * 2) + 1;
			pauseCtx->state = 1;
			
			osSyncPrintf("Ｍｏｄｅ=%d  eye.x=%f,  eye.z=%f  kscp_pos=%d\n", pauseCtx->mode, pauseCtx->eye.x,
				pauseCtx->eye.z, pauseCtx->pageIndex);
		}
		
		if (pauseCtx->state == 1) {
			WREG(2) = -6240;
			R_UPDATE_RATE = 2;
			
			if (Letterbox_GetSizeTarget() != 0)
				Letterbox_SetSizeTarget(0);
			
			func_800F64E0(1);
		}
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FaultMsg(const char* fmt, ...) {
	if (fmt == NULL) {
		sActorAssertMessage[0] = '\0';
		return;
	}
	
	va_list va;
	
	va_start(va, fmt);
	vsprintf(sActorAssertMessage, fmt, va);
	va_end(va);
}

void Fault_AddHungupAndCrashImpl(const char* exp1, const char* exp2) {
	FaultClient client;
	
	Fault_AddClient(&client, Fault_HungupFaultClient, (void*)exp1, (void*)exp2);
	
	OSThread* thd = __osRunningThread;
	
	sAssert = true;
	__osFaultedThread = thd;
	osSendMesg(&sFaultInstance->queue, FAULT_MSG_CPU_BREAK, OS_MESG_BLOCK);
	osStopThread(thd);
	
	Fault_RemoveClient(&client);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void Actor_Init(Actor* actor, PlayState* play) {
	Actor_SetWorldToHome(actor);
	Actor_SetShapeRotToWorld(actor);
	Actor_SetFocus(actor, 0.0f);
	Math_Vec3f_Copy(&actor->prevPos, &actor->world.pos);
	Actor_SetScale(actor, 0.01f);
	actor->targetMode = 3;
	actor->minVelocityY = -20.0f;
	actor->xyzDistToPlayerSq = FLT_MAX;
	actor->naviEnemyId = NAVI_ENEMY_NONE;
	actor->uncullZoneForward = 1000.0f;
	actor->uncullZoneScale = 350.0f;
	actor->uncullZoneDownward = 700.0f;
	CollisionCheck_InitInfo(&actor->colChkInfo);
	actor->floorBgId = BGCHECK_SCENE;
	ActorShape_Init(&actor->shape, 0.0f, NULL, 0.0f);
	
	if (Object_IsLoaded(&play->objectCtx, actor->objBankIndex)) {
		Actor_SetObjectDependency(play, actor);
		
		Actor* prevFaultActor = sFaultActor;
		u8 prevFaultOperation = sFaultActorOperation;
		u16 prevFaultActorID = sFaultActorID;
		u16 prevFaultActorParam = sFaultActorVariable;
		
		sActiveActor = actor;
		sActorAssertMessage[0] = '\0';
		actor->init(actor, play);
		actor->init = NULL;
		
		sActiveActor = sFaultActor = prevFaultActor;
		sFaultActorOperation = prevFaultOperation;
		sFaultActorID = prevFaultActorID;
		sFaultActorVariable = prevFaultActorParam;
	}
}

void Actor_Destroy(Actor* actor, PlayState* play) {
	if (actor->destroy != NULL) {
		sFaultActorID = actor->id;
		sFaultActorVariable = actor->params;
		sFaultActorOperation = OP_DESTROY;
		sFaultActor = actor;
		
		sActorAssertMessage[0] = '\0';
		actor->destroy(actor, play);
		
		sFaultActorID = 0xFFFF;
		sActiveActor = sFaultActor = NULL;
		
		actor->destroy = NULL;
	}
}

void Actor_UpdateAll(PlayState* play, ActorContext* actorCtx) {
	Actor* refActor;
	Actor* actor;
	Player* player;
	u32* sp80;
	u32 unkFlag;
	u32 unkCondition;
	Actor* sp74;
	ActorEntry* actorEntry;
	s32 i;
	
	player = GET_PLAYER(play);
	
	sp74 = NULL;
	unkFlag = 0;
	
	if (play->numSetupActors != 0) {
		actorEntry = &play->setupActorList[0];
		for (i = 0; i < play->numSetupActors; i++)
			Actor_SpawnEntry(&play->actorCtx, actorEntry++, play);
		play->numSetupActors = 0;
	}
	
	if (actorCtx->unk_02 != 0)
		actorCtx->unk_02--;
	
	if (KREG(0) == -100) {
		refActor = &GET_PLAYER(play)->actor;
		KREG(0) = 0;
		Actor_Spawn(&play->actorCtx, play, ACTOR_EN_CLEAR_TAG, refActor->world.pos.x, refActor->world.pos.y + 100.0f,
			refActor->world.pos.z, 0, 0, 0, 1);
	}
	
	sp80 = &D_80116068[0];
	
	if (player->stateFlags2 & PLAYER_STATE2_27)
		unkFlag = ACTOR_FLAG_25;
	
	if ((player->stateFlags1 & PLAYER_STATE1_6) && ((player->actor.textId & 0xFF00) != 0x600))
		sp74 = player->targetActor;
	
	for (i = 0; i < ARRAY_COUNT(actorCtx->actorLists); i++, sp80++) {
		unkCondition = (*sp80 & player->stateFlags1);
		
		actor = actorCtx->actorLists[i].head;
		while (actor != NULL) {
			if (actor->world.pos.y < -25000.0f) {
				actor->world.pos.y = -25000.0f;
			}
			
			actor->sfx = 0;
			
			sActiveActor = actor;
			sFaultActor = actor;
			sFaultActorOperation = OP_UPDATE;
			sFaultActorID = actor->id;
			sFaultActorVariable = actor->params;
			sActorAssertMessage[0] = '\0';
			
			if (actor->init != NULL) {
				if (Object_IsLoaded(&play->objectCtx, actor->objBankIndex)) {
					Actor_SetObjectDependency(play, actor);
					sFaultActorOperation = OP_INIT;
					actor->init(actor, play);
					actor->init = NULL;
				}
				actor = actor->next;
			} else if (!Object_IsLoaded(&play->objectCtx, actor->objBankIndex)) {
				sFaultActorOperation = OP_DESTROY;
				Actor_Kill(actor);
				actor = actor->next;
			} else if ((unkFlag && !(actor->flags & unkFlag)) ||
				(!unkFlag && unkCondition && (sp74 != actor) && (actor != player->naviActor) &&
				(actor != player->heldActor) && (&player->actor != actor->parent))) {
				CollisionCheck_ResetDamage(&actor->colChkInfo);
				actor = actor->next;
			} else if (actor->update == NULL) {
				sFaultActorOperation = OP_DESTROY;
				if (!actor->isDrawn) {
					actor = Actor_Delete(&play->actorCtx, actor, play);
				} else {
					Actor_Destroy(actor, play);
					actor = actor->next;
				}
			} else {
				Math_Vec3f_Copy(&actor->prevPos, &actor->world.pos);
				actor->xzDistToPlayer = Actor_WorldDistXZToActor(actor, &player->actor);
				actor->yDistToPlayer = Actor_HeightDiff(actor, &player->actor);
				actor->xyzDistToPlayerSq = SQ(actor->xzDistToPlayer) + SQ(actor->yDistToPlayer);
				
				actor->yawTowardsPlayer = Actor_WorldYawTowardActor(actor, &player->actor);
				actor->flags &= ~ACTOR_FLAG_24;
				
				if ((DECR(actor->freezeTimer) == 0) && (actor->flags & (ACTOR_FLAG_4 | ACTOR_FLAG_6))) {
					if (actor == player->unk_664) {
						actor->isTargeted = true;
					} else {
						actor->isTargeted = false;
					}
					
					if ((actor->targetPriority != 0) && (player->unk_664 == NULL)) {
						actor->targetPriority = 0;
					}
					
					Actor_SetObjectDependency(play, actor);
					if (actor->colorFilterTimer != 0) {
						actor->colorFilterTimer--;
					}
					actor->update(actor, play);
					DynaPoly_UnsetAllInteractFlags(play, &play->colCtx.dyna, actor);
				}
				
				CollisionCheck_ResetDamage(&actor->colChkInfo);
				
				actor = actor->next;
			}
		}
		
		if (i == ACTORCAT_BG) {
			DynaPoly_UpdateContext(play, &play->colCtx.dyna);
		}
	}
	
	sActiveActor = NULL;
	sFaultActorID = 0xFFFF;
	
	actor = player->unk_664;
	
	if ((actor != NULL) && (actor->update == NULL)) {
		actor = NULL;
		func_8008EDF0(player);
	}
	
	if ((actor == NULL) || (player->unk_66C < 5)) {
		actor = NULL;
		if (actorCtx->targetCtx.unk_4B != 0) {
			actorCtx->targetCtx.unk_4B = 0;
			func_80078884(NA_SE_SY_LOCK_OFF);
		}
	}
	
	func_8002C7BC(&actorCtx->targetCtx, player, actor, play);
	TitleCard_Update(play, &actorCtx->titleCtx);
	DynaPoly_UpdateBgActorTransforms(play, &play->colCtx.dyna);
}

void Actor_Draw(PlayState* play, Actor* actor) {
	Lights* lights;
	
	sActiveActor = sFaultActor = actor;
	sFaultActorOperation = OP_DRAW;
	sFaultActorID = actor->id;
	sFaultActorVariable = actor->params;
	sActorAssertMessage[0] = '\0';
	
	lights = LightContext_NewLights(&play->lightCtx, play->state.gfxCtx);
	Lights_BindAll(lights, play->lightCtx.listHead, (actor->flags & ACTOR_FLAG_22) ? NULL : &actor->world.pos);
	Lights_Draw(lights, play->state.gfxCtx);
	
	if (actor->flags & ACTOR_FLAG_12) {
		Matrix_SetTranslateRotateYXZ(actor->world.pos.x + play->mainCamera.skyboxOffset.x,
			actor->world.pos.y +
			((actor->shape.yOffset * actor->scale.y) + play->mainCamera.skyboxOffset.y),
			actor->world.pos.z + play->mainCamera.skyboxOffset.z, &actor->shape.rot);
	} else {
		Matrix_SetTranslateRotateYXZ(actor->world.pos.x, actor->world.pos.y + (actor->shape.yOffset * actor->scale.y),
			actor->world.pos.z, &actor->shape.rot);
	}
	
	Matrix_Scale(actor->scale.x, actor->scale.y, actor->scale.z, MTXMODE_APPLY);
	Actor_SetObjectDependency(play, actor);
	
	gSPSegment(POLY_OPA_DISP++, 0x06, play->objectCtx.status[actor->objBankIndex].segment);
	gSPSegment(POLY_XLU_DISP++, 0x06, play->objectCtx.status[actor->objBankIndex].segment);
	
	if (actor->colorFilterTimer != 0) {
		Color_RGBA8 color = { 0, 0, 0, 255 };
		
		if (actor->colorFilterParams & 0x8000) {
			color.r = color.g = color.b = ((actor->colorFilterParams & 0x1F00) >> 5) | 7;
		} else if (actor->colorFilterParams & 0x4000) {
			color.r = ((actor->colorFilterParams & 0x1F00) >> 5) | 7;
		} else {
			color.b = ((actor->colorFilterParams & 0x1F00) >> 5) | 7;
		}
		
		if (actor->colorFilterParams & 0x2000) {
			func_80026860(play, &color, actor->colorFilterTimer, actor->colorFilterParams & 0xFF);
		} else {
			func_80026400(play, &color, actor->colorFilterTimer, actor->colorFilterParams & 0xFF);
		}
	}
	
	actor->draw(actor, play);
	
	if (actor->colorFilterTimer != 0) {
		if (actor->colorFilterParams & 0x2000) {
			func_80026A6C(play);
		} else {
			func_80026608(play);
		}
	}
	
	if (actor->shape.shadowDraw != NULL) {
		actor->shape.shadowDraw(actor, lights, play);
	}
	
	sActiveActor = NULL;
	sFaultActorID = 0xFFFF;
}