#include "CPU_Primitives_Unix.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

static CPU_THREAD_LOCAL unsigned int OsPort_OsInterruptDepth;
static CPU_THREAD_LOCAL uint8_t OsPort_PreviousInterruptState;

void OsPort_DisableOsInterrupts(void)
{
	if (OsPort_OsInterruptDepth == UINT_MAX) {
		fputs("POSIX port: OS interrupt nesting overflow\n", stderr);
		abort();
	}
	if (OsPort_OsInterruptDepth == 0) {
		CPU_SAVE_AND_DISABLE_INTERRUPTS(OsPort_PreviousInterruptState);
	}
	++OsPort_OsInterruptDepth;
}

void OsPort_EnableOsInterrupts(void)
{
	if (OsPort_OsInterruptDepth == 0) {
		fputs("POSIX port: unbalanced OS interrupt enable\n", stderr);
		abort();
	}
	if (--OsPort_OsInterruptDepth == 0) {
		CPU_RESTORE_INTERRUPTS(OsPort_PreviousInterruptState);
	}
}
