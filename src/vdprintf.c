// SPDX-FileCopyrightText: 2025 Linblow <dev@linblow.com>
// SPDX-License-Identifier: BSD-3-Clause
#include <stdio.h>
#include <stdarg.h>
#include <pspiofilemgr.h>
#include <pspstdio.h>

static int fileToUID(int fd)
{
    switch (fd)
    {
    case 0:
        return sceKernelStdin();
    case 1:
        return sceKernelStdout();
    case 2:
        return sceKernelStderr();
    default:
        return fd;
    }
}

/* for printf/printf.c */
void putchar_(char c)
{
    const char s[2] = {c, 0};
    // SET_K1(0);
    sceIoWrite(fileToUID(1), s, 1);
    // RESTORE_K1();
}

/* ret > 0 number of chars written, 0 vsnprintf error, or < 0 SCE kernel error. */
int vdprintf_(int fd, const char *fmt, va_list ap)
{
    int len;
    char buf[512];
    len = vsnprintf(buf, sizeof(buf), fmt, ap);
    if (len < 0 || len >= sizeof(buf)) {
        return 0;
    }
    return sceIoWrite(fileToUID(fd), buf, len);
}

int dprintf_(int fd, const char *fmt, ...)
{
    int len;
    va_list ap;
    va_start(ap, fmt);
    len = vdprintf_(fd, fmt, ap);
    va_end(ap);
    return len;
}
