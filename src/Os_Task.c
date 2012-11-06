/*
   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).

   (C) 2007-2012 by Christoph Schueler <github.com/Christoph2,
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

#include "Osek.h"
#include "Os_Task.h"
#include "Os_Mlq.h"
#include "Os_Exec.h"

/* Fixed incorrect task activation on ISR level. */

/*
**  Local functions prototypes.
*/
#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, OSEK_OS_CODE) OsTask_Init(TaskType TaskID, boolean Schedule);


#else
static void OsTask_Init(TaskType TaskID, boolean Schedule);


#endif /* KOS_MEMORY_MAPPING */

#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_OS_START_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */

/*
**  Global Functions.
*/
#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, OSEK_OS_CODE) OsTask_Ready(TaskType TaskID)
#else
void OsTask_Ready(TaskType TaskID)
#endif /* KOS_MEMORY_MAPPING */
{
#if defined(OS_USE_RESOURCES)
    OsMLQ_AddTaskLast(TaskID, OS_TCB[TaskID].CurrentPriority);
#else
    OsMLQ_AddTaskLast(TaskID, OS_TaskConf[TaskID].Priority);
#endif
    OS_TCB[TaskID].State = READY;
}


#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, OSEK_OS_CODE) OsTask_Suspend(TaskType TaskID)
#else
void OsTask_Suspend(TaskType TaskID)
#endif /* KOS_MEMORY_MAPPING */
{
    OsMLQ_RemoveTask(TaskID);
    OS_TCB[TaskID].State = SUSPENDED;
}


#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, OSEK_OS_CODE) OsTask_Wait(TaskType TaskID)
#else
void OsTask_Wait(TaskType TaskID)
#endif /* KOS_MEMORY_MAPPING */
{
    OsMLQ_RemoveTask(TaskID);
    OS_TCB[TaskID].State = WAITING;
}


#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(StatusType, OSEK_OS_CODE) OsTask_Activate(TaskType TaskID)
#else
StatusType OsTask_Activate(TaskType TaskID)
#endif /* KOS_MEMORY_MAPPING */
{
    SAVE_SERVICE_CONTEXT(OSServiceId_ActivateTask, TaskID, NULL, NULL);

    ASSERT_VALID_TASKID(TaskID);
    ASSERT_INTERRUPTS_ENABLED_AT_TASK_LEVEL();
    ASSERT_VALID_CALLEVEL(OS_CL_TASK | OS_CL_ISR2);
    WARN_IF_TO_MANY_ACTIVATIONS(TaskID);

    DISABLE_ALL_OS_INTERRUPTS();
    OS_TASK_INCR_ACTIVATIONS(TaskID);

#if defined(OS_BCC2) || defined(OS_ECC2)

    if (OS_IS_TASK_SUSPENDED(TaskID)) {
#endif
    OS_TASK_CLEAR_ALL_EVENTS(TaskID);
    OsTask_Init(TaskID, FALSE);
    OsTask_Ready(TaskID);
#if     defined(OS_BCC2) || defined(OS_ECC2)
}


#endif
    ENABLE_ALL_OS_INTERRUPTS();

    CLEAR_SERVICE_CONTEXT();
    return E_OK;
}

#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(StatusType, OSEK_OS_CODE) ActivateTask(TaskType TaskID)
#else
StatusType ActivateTask(TaskType TaskID)
#endif /* KOS_MEMORY_MAPPING */
{
/*
**
**      'E_OS_LIMIT': maximum Number of Activations reached; 'ActivateTask' on
**              ready/running/waiting Extended Task (gilt auch für 'ChainTask()').
**
**      'E_OS_ID': invalid TaskID. (EXTENDED_STATUS)
**
*/
    const StatusType Status = OsTask_Activate(TaskID);

    if (Status != E_OK) {
        return Status;
    } else {
        OS_COND_SCHEDULE_FROM_TASK_LEVEL();
        return E_OK;
    }
}


#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(StatusType, OSEK_OS_CODE) TerminateTask(void)
#else
StatusType TerminateTask(void)
#endif /* KOS_MEMORY_MAPPING */
{
/*
**      Standard-Status: no return to Calllevel.
**      Extended-Status:
**              – E_OS_RESOURCE – the task still occupies resources.
**              – E_OS_CALLEVEL – a call at the interrupt level.
**
*/
    SAVE_SERVICE_CONTEXT(OSServiceId_TerminateTask, NULL, NULL, NULL);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK);
    ASSERT_INTERRUPTS_ENABLED_AT_TASK_LEVEL();
    ASSERT_CURR_TASK_OCCUPIES_NO_RESOURCES();

    DISABLE_ALL_OS_INTERRUPTS();
    OS_TASK_DECR_ACTIVATIONS(OsCurrentTID);
    OsTask_Suspend(OsCurrentTID);

#if defined(OS_BCC2) || defined(OS_ECC2)

    if (OsCurrentTCB->Activations > 0) {  /* ???  */
        OS_TASK_CLEAR_ALL_EVENTS(OsCurrentTID);
        OsTask_Init(OsCurrentTID, TRUE);
        OsTask_Ready(OsCurrentTID);
    }

#endif

    OS_UNLOCK_INTERNAL_RESOURCE();
    ENABLE_ALL_OS_INTERRUPTS();

    OsExec_StartHighestReadyTask();

    CLEAR_SERVICE_CONTEXT();
    return E_OK; /* never reached */
}


#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(StatusType, OSEK_OS_CODE) ChainTask(TaskType TaskID)
#else
StatusType ChainTask(TaskType TaskID)
#endif /* KOS_MEMORY_MAPPING */
{
/*
**      Terminates running Task and activates another, enforces Rescheduling.
**
**       Standard-Status:
**              – No return to call level.
**              – E_OS_LIMIT – too many activations of <TaskID>.
**       Extended-Status:
**              – E_OS_ID – the task identifier is invalid.
**              – E_OS_RESOURCE – the calling task still occupies resources.
**              – E_OS_CALLEVEL – a call at the interrupt level.
*/
    SAVE_SERVICE_CONTEXT(OSServiceId_ChainTask, TaskID, NULL, NULL);

    ASSERT_VALID_TASKID(TaskID);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK);
    ASSERT_INTERRUPTS_ENABLED_AT_TASK_LEVEL();
    ASSERT_CURR_TASK_OCCUPIES_NO_RESOURCES();

    if (TaskID != OsCurrentTID) {
        WARN_IF_TO_MANY_ACTIVATIONS(TaskID);
    }

    DISABLE_ALL_OS_INTERRUPTS();
    OS_UNLOCK_INTERNAL_RESOURCE();
    OS_TASK_DECR_ACTIVATIONS(OsCurrentTID);
    OsTask_Suspend(OsCurrentTID);

    OS_TASK_INCR_ACTIVATIONS(TaskID);
    OS_TASK_CLEAR_ALL_EVENTS(TaskID);
    OsTask_Init(TaskID, ((TaskID == OsCurrentTID) ? TRUE : FALSE));
    OsTask_Ready(TaskID);

    ASSERT(OS_TCB[OsCurrentTID].State == SUSPENDED);

    ENABLE_ALL_OS_INTERRUPTS();

    OsExec_StartHighestReadyTask();

    CLEAR_SERVICE_CONTEXT();
    return E_OK;
}


#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(StatusType, OSEK_OS_CODE) GetTaskID(TaskRefType TaskID)
#else
StatusType GetTaskID(TaskRefType TaskID)
#endif /* KOS_MEMORY_MAPPING */
{
/*
    "If no task currently running, the service returns INVALID_TASK
    constant."
    Hinweis zur Test-Prozedur: alle Tasks beenden, so dass nur noch
    die IdleTask läuft und dann aus einer ISR 'GetTaskID' aufrufen!

 */
    /* TaskID ist not known at this point!!! */
    SAVE_SERVICE_CONTEXT(OSServiceId_GetTaskID, /*TaskID*/ NULL, NULL, NULL);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK | OS_CL_ISR2 | OS_CL_ERROR_HOOK |
                          OS_CL_PRE_TASK_HOOK | OS_CL_POST_TASK_HOOK | OS_CL_PROTECTION_HOOK);

/*    ASSERT_INTERRUPTS_ENABLED_AT_TASK_LEVEL(); */

    DISABLE_ALL_OS_INTERRUPTS();
    *TaskID = OsCurrentTID;
    ENABLE_ALL_OS_INTERRUPTS();

    CLEAR_SERVICE_CONTEXT();
    return E_OK;
}


#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(StatusType, OSEK_OS_CODE) GetTaskState(TaskType TaskID, TaskStateRefType State)
#else
StatusType GetTaskState(TaskType TaskID, TaskStateRefType State)
#endif /* KOS_MEMORY_MAPPING */
{
/*
**      "Within a full-preemptive system, calling this operating system
**      service only provides a meaningful result if the task runs in an
**      interrupt disabling state at the time of calling. When a call is made
**      from a task in a full-preemptive system, the result may already be
**      incorrect at the time of evaluation."
*/
    /* State is mot known @ this point!!! */
    SAVE_SERVICE_CONTEXT(OSServiceId_GetTaskState, TaskID, /*State*/ NULL, NULL);
    ASSERT_VALID_TASKID(TaskID);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK | OS_CL_ISR2 | OS_CL_ERROR_HOOK |
                          OS_CL_PRE_TASK_HOOK | OS_CL_POST_TASK_HOOK | OS_CL_PROTECTION_HOOK);

    DISABLE_ALL_OS_INTERRUPTS();
    *State = OS_TCB[TaskID].State;
    ENABLE_ALL_OS_INTERRUPTS();
/*
**      Standard-Status:
**              – E_OK – no error.
**      Extended-Status:
**               – E_OS_ID – the task identifier is invalid.
*/

    CLEAR_SERVICE_CONTEXT();
    return E_OK;
}


#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(StatusType, OSEK_OS_CODE) Schedule(void)
#else
StatusType Schedule(void)
#endif /* KOS_MEMORY_MAPPING */
{
    SAVE_SERVICE_CONTEXT(OSServiceId_Schedule, NULL, NULL, NULL);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK);
    ASSERT_CURR_TASK_OCCUPIES_NO_RESOURCES();
    ASSERT_INTERRUPTS_ENABLED_AT_TASK_LEVEL();
/*    ASSERT_TASK_HAS_NO_SHARED_STACK();    */

/*
**       Standard-Status:
**              – E_OK – no error.
**       Extended-Status:
**              – E_OS_CALLEVEL – a call at the interrupt level.
**              – E_OS_RESOURCE - calling task occupies resources.
*/
#if defined(OS_USE_INTERNAL_RESOURCES)

    if (OS_TaskConf[OsCurrentTID].InternalResource != INTERNAL_RES_NONE) {
#if defined(OS_SCHED_POLICY_NON) ||  defined(OS_SCHED_POLICY_MIX)
        OS_UNLOCK_INTERNAL_RESOURCE();
#endif
        OS_FORCE_SCHEDULE_FROM_TASK_LEVEL();
    }

#endif
    CLEAR_SERVICE_CONTEXT();
    return E_OK;
}


#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, OSEK_OS_CODE) OsTask_InitTasks(void)
#else
void OsTask_InitTasks(void)
#endif /* KOS_MEMORY_MAPPING */
{
#if defined(OS_FEATURE_AUTOSTART_TASKS)
    uint8_least i;
#endif /* OS_FEATURE_AUTOSTART_TASKS */

    OsTask_Init((TaskType)0, FALSE);

#if defined(OS_FEATURE_AUTOSTART_TASKS)

    for (i = (uint8_least)1; i < OS_NUMBER_OF_TASKS; ++i) {
        OsTask_Init(i, FALSE);

        if (OS_TaskConf[i].Autostart & GetActiveApplicationMode()) {
            OsTask_Ready(i);
            OS_TASK_INCR_ACTIVATIONS(i);
        }
    }

#endif /* OS_FEATURE_AUTOSTART_TASKS */
}


/*
**  Local functions.
*/
#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, OSEK_OS_CODE) OsTask_Init(TaskType TaskID, boolean Schedule)
#else
static void OsTask_Init(TaskType TaskID, boolean Schedule)
#endif /* KOS_MEMORY_MAPPING */
{
    OsTaskConfigurationType *   task_def;
    OsTCBType *                 tcb;

    if (TaskID > (OS_NUMBER_OF_TASKS - (uint8)1)) {
        return; /* todo: Only in EXTENDED-Status!!! */
    }

    task_def   = (OsTaskConfigurationType *)&OS_TaskConf[TaskID];
    tcb        = &OS_TCB[TaskID];

#if defined(OS_USE_STACKCHECKING)
    OsUtilMemSet((void *)task_def->stack_addr, (uint8)OSSTACKFILLCHAR, (uint16)task_def->stack_size);
#endif

    tcb->Stackpointer =
        OsPort_TaskStackInit(TaskID, &task_def->TaskFunction,
                             ((uint8 *)task_def->StackStart + (task_def->StackSize - (uint8)1))
                             );
    tcb->State = SUSPENDED;
#if defined(OS_BCC2) || defined(OS_ECC2)
    tcb->Activations = (uint8)0x00;
#endif

#if defined(OS_USE_RESOURCES)
    tcb->CurrentPriority   = task_def->Priority;
    tcb->ResourceCount     = (uint8)0x00;
#endif

#if defined(OS_ECC1) || defined(OS_ECC2)
    tcb->EventsSet         = (EventMaskType)0x00;
    tcb->EventsWaitingFor  = (EventMaskType)0x00;
#endif

    if (Schedule) {
        OsExec_StartHighestReadyTask();
    }
}


#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_OS_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */
