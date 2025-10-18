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


int readDiscId(char discId[10], u32 timeoutMs)
{
    SceUID fd;
    char buf[16] = {0};
	int n, timeoutUs;

    if (discId == NULL) {
        return -1;
	}

	timeoutUs = (timeoutMs ? timeoutMs : 1) * 1000;
	while (timeoutUs > 0) {
		fd = sceIoOpen("disc0:/UMD_DATA.BIN", PSP_O_RDONLY, 0644);
		if (fd >= 0) {
			break;
		}
		sceKernelDelayThread(1000);
		timeoutUs -= 1000;
	}
	if (fd < 0) {
		return fd;
	}

	n = sceIoRead(fd, buf, 10);
    sceIoClose(fd);
    if (n < 0) {
        return n;
	}
    if (n != 10) {
        return -2;
	}

    // eg. "UCUS-98645" becomes "UCUS98645"
    memmove(buf + 4, buf + 5, 5);
    buf[9] = '\0';
    memcpy(discId, buf, 10);
    return 0;
}

int isRunningThisGame(const char *discId, u32 timeoutMs)
{
	char cur[10];
	if (readDiscId(cur, timeoutMs) < 0) {
		return 0;
	}
	return memcmp(cur, discId, 9) == 0;
}
