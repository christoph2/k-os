/*
   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).

   (C) 2007-2010 by Christoph Schueler <chris@konnex-tools.de,
                                       cpu12.gems@googlemail.com>

   All Rights Reserved

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 

   s. FLOSS-EXCEPTION.txt
*/
#if !defined(__OS_ORTI_H)
#define __OS_ORTI_H

#include "Osek.h"

#define OsORTIServiceEntry(ServiceId)
/* set Service-entry-flag. */

#define OsORTIServiceExit(ServiceId)
/* set Service-exit-flag. */

/*
**  Service-Definitions.
*/
#define OSORTIServiceId_NoService   ((uint8)0)  /* No Service, exit Service. */

#if 0
#define OSORTIServiceId_ActivateTask,               /* 0x00 */
#define     OSORTIServiceId_TerminateTask,              /* 0x01 */
#define     OSORTIServiceId_ChainTask,                  /* 0x02 */
#define     OSORTIServiceId_Schedule,                   /* 0x03 */
#define     OSORTIServiceId_GetTaskID,                  /* 0x04 */
#define     OSORTIServiceId_GetTaskState,               /* 0x05 */
#define     OSORTIServiceId_DisableAllInterrupts,       /* 0x06 */
#define     OSORTIServiceId_EnableAllInterrupts,        /* 0x07 */
#define     OSORTIServiceId_SuspendAllInterrupts,       /* 0x08 */
#define     OSORTIServiceId_ResumeAllInterrupts,        /* 0x09 */
#define     OSORTIServiceId_SuspendOSInterrupts,        /* 0x0a */
#define     OSORTIServiceId_ResumeOSInterrupts,         /* 0x0b */
#define     OSORTIServiceId_GetResource,                /* 0x0c */
#define     OSORTIServiceId_ReleaseResource,            /* 0x0d */
#define     OSORTIServiceId_SetEvent,                   /* 0x0e */
#define     OSORTIServiceId_ClearEvent,                 /* 0x0f */
#define     OSORTIServiceId_GetEvent,                   /* 0x10 */
#define     OSORTIServiceId_WaitEvent,                  /* 0x11 */
#define     OSORTIServiceId_GetAlarmBase,               /* 0x12 */
#define     OSORTIServiceId_GetAlarm,                   /* 0x13 */
#define     OSORTIServiceId_SetRelAlarm,                /* 0x14 */
#define     OSORTIServiceId_SetAbsAlarm,                /* 0x15 */
#define     OSORTIServiceId_CancelAlarm,                /* 0x16 */
#define     OSORTIServiceId_GetActiveApplicationMode,   /* 0x17 */
#define     OSORTIServiceId_StartOS,                    /* 0x18 */
#define     OSORTIServiceId_ShutdownOS,                 /* 0x19 */
#define     OSORTIServiceId_GetApplicationID,           /* 0x1a */
#define     OSORTIServiceId_GetISRID,                   /* 0x1b */
#define     OSORTIServiceId_CallTrustedFunction,        /* 0x1c */
#define     OSORTIServiceId_CheckISRMemoryAccess,       /* 0x1d */
#define     OSORTIServiceId_CheckTaskMemoryAccess,      /* 0x1e */
#define     OSORTIServiceId_CheckObjectAccess,          /* 0x1f */
#define     OSORTIServiceId_CheckObjectOwnership,       /* 0x20 */
#define     OSORTIServiceId_StartScheduleTableRel,      /* 0x21 */
#define     OSORTIServiceId_StartScheduleTableAbs,      /* 0x22 */
#define     OSORTIServiceId_StopScheduleTable,          /* 0x23 */
#define     OSORTIServiceId_NextScheduleTable,          /* 0x24 */
#define     OSORTIServiceId_SyncScheduleTable,          /* 0x25 */
#define     OSORTIServiceId_GetScheduleTableStatus,     /* 0x26 */
#define     OSORTIServiceId_SetScheduleTableAsync,      /* 0x27 */
#define     OSORTIServiceId_IncrementCounter,           /* 0x28 */
#define     OSORTIServiceId_TerminateApplication,       /* 0x29 */
#define     OSORTIServiceId_DisableInterruptSource,     /* 0x2a */
#define     OSORTIServiceId_EnableInterruptSource,      /* 0x2b */
#define     OSORTIServiceId_InitCounter,                /* 0x2c */
#define     OSORTIServiceId_CounterTrigger,             /* 0x2d */
#define     OSORTIServiceId_GetCounterInfo,             /* 0x2e */
#define     OSORTIServiceId_GetCounterValue             /* 0x2f */
#endif

extern StatusType OsORTILastError;
extern uint8 OsORTIServiceID;

#endif  /* __OS_ORTI_H */
