/*
   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).

 * (C) 2007-2012 by Christoph Schueler <github.com/Christoph2,
 *                                      cpu12.gems@googlemail.com>

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

#include "Osek.h"

#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, OSEK_OS_CODE) OsExec_Init(void);
#else
static void OsExec_Init(void);
#endif /* KOS_MEMORY_MAPPING */

#if defined(OS_FEATURE_SAVE_STARTUP_CONTEXT)
static Utl_JumpBufType StartupContext;
#endif  /* OS_FEATURE_SAVE_STARTUP_CONTEXT */

OS_DEFINE_GLOBAL_IF_DEBUGGING(Os_AppMode, AppModeType);


#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_OS_START_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */

/*
**  Global functions.
*/
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) StartOS(AppModeType Mode)
#else
void StartOS(AppModeType Mode)
#endif /* KOS_MEMORY_MAPPING */
{
    SAVE_SERVICE_CONTEXT(OSServiceId_StartOS, Mode, NULL, NULL);

    ASSERT_OS_NOT_RUNNING();

    if (Mode == OSDEFAULTAPPMODE) {
        Os_AppMode = OS_FEATURE_REAL_DEFAULT_APPMODE;
    } else if (Mode > (AppModeType)7) {
        Os_AppMode = OSDEFAULTAPPMODE;
    } else {
        Os_AppMode = Mode;
    }

/*    DISABLE_ALL_OS_INTERRUPTS(); */

    OsPort_Init();

    OsExec_Init();

#if defined(OS_USE_STARTUPHOOK)
    OS_SET_CALLEVEL(OS_CL_STARTUP_HOOK);
    StartupHook();
#endif  /* OS_USE_STARTUPHOOK */
    OS_SET_CALLEVEL(OS_CL_TASK);
    OS_SET_TASK_LEVEL();

    /* check: das nächste zur Sicherheit???*/
    OS_SET_HIGHEST_PRIO_RUNNING();

    CLEAR_SERVICE_CONTEXT();
/*    ENABLE_ALL_OS_INTERRUPTS(); */

#if defined(OS_FEATURE_SAVE_STARTUP_CONTEXT)

    if (Utl_SetJump(&StartupContext) == 0) {
#endif  /* OS_FEATURE_SAVE_STARTUP_CONTEXT */

    OS_START_CURRENT_TASK();

#if defined(OS_FEATURE_SAVE_STARTUP_CONTEXT)
}


#endif  /* OS_FEATURE_SAVE_STARTUP_CONTEXT */
}

#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) ShutdownOS(StatusType Error)
#else
void ShutdownOS(StatusType Error)
#endif /* KOS_MEMORY_MAPPING */
{
    uint8_least i;

    SAVE_SERVICE_CONTEXT(OSServiceId_ShutdownOS, Error, NULL, NULL);
    CPU_DISABLE_ALL_INTERRUPTS();

    for (i = (uint8_least)0; i < OS_NUMBER_OF_TASKS; ++i) {
        OS_TCB[i].State = SUSPENDED;
    }

#if defined(OS_USE_SHUTDOWNHOOK)
    OS_SET_CALLEVEL(OS_CL_SHUTDOWN_HOOK);
    ShutdownHook(Error);
#else   /* OS_USE_SHUTDOWNHOOK */
    UNREFERENCED_PARAMETER(Error);
#endif
    OS_SET_CALLEVEL(OS_CL_INVALID);
    CLEAR_SERVICE_CONTEXT();    /* ??? */

#if defined(OS_FEATURE_SAVE_STARTUP_CONTEXT)
    Utl_LongJump(&StartupContext, -1);
#endif  /* OS_FEATURE_SAVE_STARTUP_CONTEXT */
/*
**  !REQ!AS!OS425!
**  If ShutdownOS() is called and ShutdownHook() returns then the operating
**  system shall disable all interrupts and enter an endless loop.
*/
    FOREVER {

    }
}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(AppModeType, OSEK_OS_CODE) GetActiveApplicationMode(void)
#else
AppModeType GetActiveApplicationMode(void)
#endif /* KOS_MEMORY_MAPPING */
{
    SAVE_SERVICE_CONTEXT(OSServiceId_GetActiveApplicationMode, NULL, NULL, NULL);

#if 0
    ASSERT_VALID_CALLEVEL(CL_TASK | CL_ISR2 | CL_ERROR_HOOK | CL_PRE_TASK_HOOK |
                          CL_POST_TASK_HOOK | CL_STARTUP_HOOK | CL_SHUTDOWN_HOOK);
#endif
    CLEAR_SERVICE_CONTEXT();
    return Os_AppMode;
}


#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, OSEK_OS_CODE) OsExec_Init(void)
#else
static void OsExec_Init(void)
#endif /* KOS_MEMORY_MAPPING */
{
    OsCurrentTID   = INVALID_TASK;
    OsCurrentTCB   = (OsTCBType *)NULL;
    OsFlags        = (uint8)0x00;

    OsMLQ_Init();

    OsIntr_InitInterrupts();

    OsTask_InitTasks();

    OsRes_InitResources();

    OsCtr_InitCounters();

    OsAlm_InitAlarms();
}


TASK(OsExec_IdleTask)
{
    FOREVER {
        WAIT_FOR_READY_TASKS();
#if defined(OS_SCHEDULE_NON) || defined(OS_SCHEDULE_MIX)
        (void)Schedule();
#endif  /* OS_SCHEDULE_NON || OS_SCHEDULE_MIX */
    }
}

#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsExec_TaskReturnGuard(void)
#else
void OsExec_TaskReturnGuard(void)
#endif /* KOS_MEMORY_MAPPING */
{

}


#if defined(OS_SCHED_POLICY_NON)
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsExec_ScheduleFromISR(void)
#else
void OsExec_ScheduleFromISR(void)
#endif /* KOS_MEMORY_MAPPING */
{
}

#elif defined(OS_SCHED_POLICY_PRE) || defined(OS_SCHED_POLICY_MIX)
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsExec_ScheduleFromISR(void)   /*  ISR-Level-Scheduling. */
#else
void OsExec_ScheduleFromISR(void)   /*  ISR-Level-Scheduling. */
#endif /* KOS_MEMORY_MAPPING */
{
#if defined(OS_SCHED_POLICY_MIX)

    if (OS_IS_TASK_PREEMPTABLE(OsCurrentTID) && !OS_IS_SCHEDULER_LOCKED()) {
#else

    if (!OS_IS_SCHEDULER_LOCKED()) {
#endif  /* OS_SCHED_POLICY_NON */
        OS_CALL_POST_TASK_HOOK();

#if defined(OS_ECC1) || defined(OS_ECC2)

        /* Poor workaround!!! */
        if (!OS_IS_TASK_WAITING(OsCurrentTID)) {
            OsCurrentTCB->State = READY;
        }

#endif
        OS_SET_HIGHEST_PRIO_RUNNING();
    }
}


#endif

#if KOS_MEMORY_MAPPING == STD_ON
FUNC(boolean, OSEK_OS_CODE) OsExec_HigherPriorityThenCurrentReady(void)
#else
boolean OsExec_HigherPriorityThenCurrentReady(void)
#endif /* KOS_MEMORY_MAPPING */
{
    uint16  currentPriority;
    boolean res;

    /* TODO: Ändern!!! */
#if defined(OS_USE_RESOURCES) || defined(OS_USE_INTERNAL_RESOURCES)
    currentPriority = Utl_SetBitTab16[OsCurrentTCB->CurrentPriority];
#else
    currentPriority = Utl_SetBitTab16[OS_TaskConf[OsCurrentTID].Priority];
#endif

    res = (OsMLQ_GetBitmap() & (~currentPriority)) > currentPriority;

    return res;

}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsExec_StartHighestReadyTask(void)
#else
void OsExec_StartHighestReadyTask(void)
#endif /* KOS_MEMORY_MAPPING */
{
    OS_SET_HIGHEST_PRIO_RUNNING();
    OS_START_CURRENT_TASK();
}


ISR1(SWI_Vector)
{
    OSEnterISR();
    OSLeaveISR();
}

#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_OS_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */
