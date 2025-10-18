// SPDX-FileCopyrightText: 2025 Linblow <dev@linblow.com>
// SPDX-License-Identifier: BSD-3-Clause
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <pspkernel.h>
#include <psptypes.h>
#include <psploadcore.h>
#include <pspintrman.h>
#include <psputils.h>
#include <pspsysmem.h>

#include <module.h>
#include <printf/printf.h>

// vdprintf.c
extern int dprintf_(int fd, const char *fmt, ...);

#define LOG(fmt, ...) dprintf_(1, "[%s] " fmt, g_appName, ##__VA_ARGS__)

typedef enum {
	STATE_INIT = 0,
	/** Main thread has finished its work, and will now wait for unload. */
	STATE_DONE,
	/** Signal to main thread it can now unloads the module (cannot be done in module_start). */
	STATE_READY_FOR_UNLOAD
} State;

static const char g_appName[] = APP_NAME;
static volatile State g_state = STATE_INIT;

/* Loading this in the user RAM may "shift" the game's module load address.
   To avoid this, we run this is kernel mode to load it in kernel memory. */
PSP_MODULE_INFO(APP_NAME, PSP_MODULE_KERNEL, APP_VER_MAJOR, APP_VER_MINOR);

static inline State setState(State st)
{
	State prev = g_state;
	g_state = st;
	return prev;
}

static inline void waitState(State st)
{
	while (g_state != st) {
		sceKernelDelayThread(10*1000);
	}
}

static _SceModule *findModule(const char *modname, u32 timeoutMs)
{
	u32 t = 0, delay = 1000, timeoutUs = timeoutMs * 1000;
	void *mod = NULL;
	while ((mod = sceKernelFindModuleByName(modname)) == NULL) {
		sceKernelDelayThread(delay);
		t += delay;
		if (t >= timeoutUs) {
			break;
		}
	}
	return (_SceModule *)mod;
}

/* Patches FTB 2 EU's SocomPSPLoader module (BOOT.BIN). */
static int patchLoader(u32 textAddr)
{
	int ret;
	int intr = sceKernelCpuSuspendIntr();
	ret = 1;
	if (memcmp((void *)textAddr + 0x7a7e, "UCES00543", 9) == 0) {
		_sw(0, textAddr + 0x2d4); // jal initGu -> nop
		_sw(0, textAddr + 0x2e0); // jal drawFrame -> nop
		_sw(0, textAddr + 0x2f0); // jal sceGuSwapBuffers -> nop
		sceKernelIcacheInvalidateRange((void *)textAddr + 0x2c0, 0x40);
		ret = 0;
	}
	sceKernelCpuResumeIntr(intr);
	return ret;
}

int main(SceSize args, void *argp)
{
	_SceModule *mod;
	mod = findModule("SocomPSPLoader", 10*1000);
	if (mod) {
		if (patchLoader(mod->textAddr) == 0) {
			LOG("SocomPSPLoader module was patched\n");
		}
		else {
			LOG("Abort, game is not SOCOM: FireTeam Bravo 2 EU (UCES00543)\n");
		}
	}
	else {
		LOG("SocomPSPLoader module was not found\n");
	}
	setState(STATE_DONE);
	waitState(STATE_READY_FOR_UNLOAD);
	sceKernelDelayThread(50*1000);
	LOG("Unloading plugin\n");
	sceKernelStopUnloadSelfModule(0, NULL, NULL, NULL);
	return 0;
}

static int isEmulator()
{
   /* PPSSPP/JPCSP always return 0 for success (ignore res).
      PSP always returns an error since "emulator:" is not a device. */
   int res;
   int ret = sceIoDevctl("emulator:", 3, &res, 4, NULL, 0);
   return ret == 0;
}

static SceUID createStartThread(const char *name, void *entry, int initPriority, int stackSizeKib, u32 attrs, SceSize args, void *argp)
{
	int ret;
	SceUID thid;
	thid = sceKernelCreateThread(name, (SceKernelThreadEntry)entry, initPriority, stackSizeKib*1024, attrs, NULL);
	if (thid >= 0) {
		ret = sceKernelStartThread(thid, args, argp);
		if (ret < 0) {
			sceKernelDeleteThread(thid);
			return ret;
		}
	}
	return thid;
}

int module_start(SceSize args, void *argp)
{
	SceUID thid;
	LOG("Plugin was started\n");
	if (!isEmulator()) {
		LOG("Abort, not running in emulator\n");
		return 1;
	}
	thid = createStartThread(APP_NAME "_thread", main, 16, 4, 0, args, argp);
	if (thid < 0) {
		LOG("Failed to create main thread\n");
		return 1;
	}
	/* Wait for main thread to finish (ie. keep blocking game load in PPSSPP). */
	waitState(STATE_DONE);
	/* Have the main thread unload the plugin, cause PPSSPP won't do it regardless 
	   of the module_start return value, and it can't be done in the start routine. */
	setState(STATE_READY_FOR_UNLOAD);
	return 0;
}
