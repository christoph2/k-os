/*
 * k_os (Konnex Operating-System based on the OSEK/VDX-Standard).
 *
 * (C) 2007-2013 by Christoph Schueler <github.com/Christoph2,
 *                                      cpu12.gems@googlemail.com>
 *
 * All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * s. FLOSS-EXCEPTION.txt
 */

#include "Osek.h"
#include "Os_Mlq.h"
#include "Os_Intr.h"
#include "Os_Task.h"
#include "Os_Evt.h"
#include "Os_Exec.h"
#include "Os_Res.h"
#include "Os_Alm.h"
/*/ #include "Os_Ctr.h" */
#include "Utl.h"

#define API_TRACE()     /* TODO: Nur zum Testen!!! */

#include "common/inc/apitracer.h"

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
    Os_SaveServiceContext(OSServiceId_StartOS, Mode, NULL, NULL);

    ASSERT_OS_NOT_RUNNING();

//    API_TRACE("StartOS - Appmode: %u\n", Mode);

    if (Mode == OSDEFAULTAPPMODE) {
        Os_AppMode = OS_FEATURE_REAL_DEFAULT_APPMODE;
    } else if (Mode > (AppModeType)7) {
        Os_AppMode = OSDEFAULTAPPMODE;
    } else {
        Os_AppMode = Mode;
    }

/*    OsPort_DisableAllOsInterrupts(); */

    OsPort_Init();

    OsExec_Init();

#if defined(OS_USE_STARTUPHOOK)
    Os_SetCallLevel(OS_CL_STARTUP_HOOK);
    StartupHook();
#endif  /* OS_USE_STARTUPHOOK */
    Os_SetCallLevel(OS_CL_TASK);
    Os_SetTaskLevel();

    /* check: das nächste zur Sicherheit???*/
    OsExec_SetHighestPrioRunning();

    Os_ClearServiceContext();
/*    OsPort_EnableAllOsInterrupts(); */

#if defined(OS_FEATURE_SAVE_STARTUP_CONTEXT)

    if (Utl_SetJump(&StartupContext) == 0) {
#endif  /* OS_FEATURE_SAVE_STARTUP_CONTEXT */

    OsPort_StartCurrentTask();

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

    Os_SaveServiceContext(OSServiceId_ShutdownOS, Error, NULL, NULL);
    CPU_DISABLE_ALL_INTERRUPTS();

    for (i = (uint8_least)0; i < OS_NUMBER_OF_TASKS; ++i) {
        OS_TCB[i].State = SUSPENDED;
    }

#if defined(OS_USE_SHUTDOWNHOOK)
    Os_SetCallLevel(OS_CL_SHUTDOWN_HOOK);
    ShutdownHook(Error);
#else   /* OS_USE_SHUTDOWNHOOK */
    UNREFERENCED_PARAMETER(Error);
#endif
    Os_SetCallLevel(OS_CL_INVALID);
    Os_ClearServiceContext();    /* ??? */

#if defined(OS_FEATURE_SHUTDOWN_OSPORT) /* TODO: Automatisch setzen, wenn Port Shutdown erforderlich!!! */
    OsPort_Shutdown();
#endif

#if defined(OS_FEATURE_SAVE_STARTUP_CONTEXT)
    Utl_LongJump(&StartupContext, -1);
#endif  /* OS_FEATURE_SAVE_STARTUP_CONTEXT */
/*
**  !REQ!AS!OS425!
**  If ShutdownOS() is called and ShutdownHook() returns then the operating
**  system shall disable all interrupts and enter an endless loop.
*/
    /*lint -e716 */
    FOREVER {

    }
}


/*lint +e716 */

#if KOS_MEMORY_MAPPING == STD_ON
FUNC(AppModeType, OSEK_OS_CODE) GetActiveApplicationMode(void)
#else
AppModeType GetActiveApplicationMode(void)
#endif /* KOS_MEMORY_MAPPING */
{
    Os_SaveServiceContext(OSServiceId_GetActiveApplicationMode, NULL, NULL, NULL);

#if 0
    ASSERT_VALID_CALLEVEL(CL_TASK | CL_ISR2 | CL_ERROR_HOOK | CL_PRE_TASK_HOOK |
                          CL_POST_TASK_HOOK | CL_STARTUP_HOOK | CL_SHUTDOWN_HOOK);
#endif
    Os_ClearServiceContext();
    return Os_AppMode;
}


#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, OSEK_OS_CODE) OsExec_Init(void)
#else
static void OsExec_Init(void)
#endif /* KOS_MEMORY_MAPPING */
{
    Os_CurrentTID   = INVALID_TASK;
    Os_CurrentTCB   = (Os_TCBType *)NULL;
    Os_Flags        = (uint8)0x00;

    OsMLQ_Init();

    OsIntr_InitInterrupts();

    OsTask_InitTasks();

    OsRes_InitResources();

    OsCtr_InitCounters();

    OsAlm_InitAlarms();
}


TASK(OsExec_IdleTask)
{
    /*lint -e716 */
    FOREVER {
        OsExec_WaitForReadyTasks();
#if defined(OS_SCHEDULE_NON) || defined(OS_SCHEDULE_MIX)
        (void)Schedule();
#endif  /* OS_SCHEDULE_NON || OS_SCHEDULE_MIX */
    }
}
/*lint +e716 */

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
void OsExec_ScheduleFromISR(void)                       /*  ISR-Level-Scheduling. */
#endif /* KOS_MEMORY_MAPPING */
{
#if defined(OS_SCHED_POLICY_MIX)

    if (OsTask_IsPreemptable(Os_CurrentTID) && !OsExec_IsSchedulerLocked()) {
#else

    if (!OsExec_IsSchedulerLocked()) {
#endif  /* OS_SCHED_POLICY_NON */
        OsExec_CallPostTaskHook();

#if defined(OS_ECC1) || defined(OS_ECC2)

        /* Poor workaround!!! */
        if (!OsTask_IsWaiting(Os_CurrentTID)) {
            Os_CurrentTCB->State = READY;
        }

#endif
        OsExec_SetHighestPrioRunning();
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
    currentPriority = Utl_SetBitTab16[Os_CurrentTCB->CurrentPriority];
#else
    currentPriority = Utl_SetBitTab16[OS_TaskConf[Os_CurrentTID].Priority];
#endif

    res = (boolean)((OsMLQ_GetBitmap() & ((uint16) ~currentPriority)) > currentPriority);

    return res;

}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsExec_StartHighestReadyTask(void)
#else
void OsExec_StartHighestReadyTask(void)
#endif /* KOS_MEMORY_MAPPING */
{
    OsExec_SetHighestPrioRunning();
    OsPort_StartCurrentTask();
}


ISR1(SWI_Vector)    /* TODO: Os_Port! */
{
    Os_EnterISR();
    Os_LeaveISR();
}

#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_OS_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */

