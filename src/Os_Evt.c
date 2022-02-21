/*
 * k_os (Konnex Operating-System based on the OSEK/VDX-Standard).
 *
 * (C) 2007-2018 by Christoph Schueler <github.com/Christoph2,
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
#include "Os_Task.h"

#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_OS_START_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */

/*
**  Global functions.
*/
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(StatusType, OSEK_OS_CODE) OsEvt_SetEvent(TaskType TaskID, EventMaskType Mask)
#else
StatusType OsEvt_SetEvent(TaskType TaskID, EventMaskType Mask)
#endif /* KOS_MEMORY_MAPPING */
{
#if defined(OS_BCC1) || defined(OS_BCC2)
    UNREFERENCED_PARAMETER(TaskID);
    UNREFERENCED_PARAMETER(Mask);

    Os_CallErrorHookAndReturn(E_OS_ACCESS);    /* no extended tasks, always fail.  */
#elif defined(OS_ECC1) || defined(OS_ECC2)
    Os_SaveServiceContext(OSServiceId_SetEvent, TaskID, Mask, NULL);

    ASSERT_VALID_TASKID(TaskID);
    ASSERT_TASK_IS_EXTENDED(TaskID);
    ASSERT_TASK_IS_NOT_SUSPENDED(TaskID);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK | OS_CL_ISR2);
    ASSERT_INTERRUPTS_ENABLED_AT_TASK_LEVEL();

    OsPort_DisableAllOsInterrupts();
    OsTask_SetEventMask(TaskID, Mask);

    if (((OsTask_GetEventsSet(TaskID) &
          OsTask_GetEventsWaitingFor(TaskID)) != (EventMaskType)0) && OsTask_IsWaiting(TaskID)
    ) {
        OsTask_Ready(TaskID);
    }

    OsPort_EnableAllOsInterrupts();
    Os_ClearServiceContext();
    return E_OK;
#endif
}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(StatusType, OSEK_OS_CODE) Os_SetEvent(TaskType TaskID, EventMaskType Mask)
#else
StatusType Os_SetEvent(TaskType TaskID, EventMaskType Mask)
#endif /* KOS_MEMORY_MAPPING */
{
/*
**
**      Standard-Status:
**              – E_OK – no error.
**      Extended-Status:
**              – E_OS_ID – the task identifier is invalid.
**              – E_OS_ACCESS – the referenced task is not an Extended Task.
**              – E_OS_STATE – the referenced task is in the suspended state.
*/

#if defined(OS_BCC1) || defined(OS_BCC2)
    UNREFERENCED_PARAMETER(TaskID);
    UNREFERENCED_PARAMETER(Mask);

    Os_CallErrorHookAndReturn(E_OS_ACCESS);  /* no extended tasks, always fail.  */
#elif defined(OS_ECC1) || defined(OS_ECC2)
    StatusType Status = OsEvt_SetEvent(TaskID, Mask);

    Os_SaveServiceContext(OSServiceId_SetEvent, TaskID, Mask, NULL);

    Os_ClearServiceContext();

    if (Status != E_OK) {
        return Status;
    } else {
        OsExec_CondScheduleFromTaskLevel();
    }

    return E_OK;
#endif
}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(StatusType, OSEK_OS_CODE) Os_ClearEvent(EventMaskType Mask)
#else
StatusType Os_ClearEvent(EventMaskType Mask)
#endif /* KOS_MEMORY_MAPPING */
{

/*
**      Standard-Status:
**              – E_OK – no error.
**      Extended-Status:
**              – E_OS_ACCESS – the calling task is not an Extended Task.
**              – E_OS_CALLEVEL – a call at the interrupt level is not allowed.
*/

#if defined(OS_BCC1) || defined(OS_BCC2)
    UNREFERENCED_PARAMETER(Mask);

    Os_CallErrorHookAndReturn(E_OS_ACCESS);    /* no extended tasks, always fail.  */
#elif defined(OS_ECC1) || defined(OS_ECC2)
    Os_SaveServiceContext(OSServiceId_ClearEvent, Mask, NULL, NULL);

    ASSERT_TASK_IS_EXTENDED(Os_CurrentTID);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK);
    ASSERT_INTERRUPTS_ENABLED_AT_TASK_LEVEL();

    OsPort_DisableAllOsInterrupts();
    OsTask_ClearEventMask(Os_CurrentTID, Mask);
    OsPort_EnableAllOsInterrupts();

    Os_ClearServiceContext();
    return E_OK;
#endif
}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(StatusType, OSEK_OS_CODE) Os_GetEvent(TaskType TaskID, EventMaskRefType Event)
#else
StatusType Os_GetEvent(TaskType TaskID, EventMaskRefType Event)
#endif /* KOS_MEMORY_MAPPING */
{
/*
**      Standard-Status:
**              – E_OK – no error.
**      Extended-Status:
**              – E_OS_ID – the task identifier is invalid.
**              – E_OS_ACCESS – the referenced task is not an Extended Task.
**              – E_OS_STATE – the referenced task is in the suspended state.
*/
#if defined(OS_BCC1) || defined(OS_BCC2)
    UNREFERENCED_PARAMETER(TaskID);
    UNREFERENCED_PARAMETER(Event);

    Os_CallErrorHookAndReturn(E_OS_ACCESS);      /* no extended tasks, always fail.  */
#elif defined(OS_ECC1) || defined(OS_ECC2)
    Os_SaveServiceContext(OSServiceId_GetEvent, TaskID, /*Event*/ NULL, NULL);

    ASSERT_VALID_TASKID(TaskID);
    ASSERT_TASK_IS_EXTENDED(TaskID);
    ASSERT_TASK_IS_NOT_SUSPENDED(TaskID);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK | OS_CL_ISR2 | OS_CL_ERROR_HOOK | OS_CL_PRE_TASK_HOOK | OS_CL_POST_TASK_HOOK);
    ASSERT_INTERRUPTS_ENABLED_AT_TASK_LEVEL();

    OsPort_DisableAllOsInterrupts();
    *Event = OsTask_GetEventsSet(TaskID);
    OsPort_EnableAllOsInterrupts();

    Os_ClearServiceContext();
    return E_OK;
#endif
}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(StatusType, OSEK_OS_CODE) Os_WaitEvent(EventMaskType Mask)
#else
StatusType Os_WaitEvent(EventMaskType Mask)
#endif /* KOS_MEMORY_MAPPING */
{
/*
**      former state: 'running', new state 'waiting'.   - only ETs.
**      "This call enforces the rescheduling, if the wait condition occurs."
**
**      Standard-Status:
**              - E_OK – no error.
**      Extended-Status:
**              – E_OS_ACCESS – the calling task is not an Extended Task.
**              – E_OS_RESOURCE – the calling task occupies resources.
**              – E_OS_CALLEVEL – a call at the interrupt level is not allowed.
*/
#if defined(OS_BCC1) || defined(OS_BCC2)
    UNREFERENCED_PARAMETER(Mask);

    Os_CallErrorHookAndReturn(E_OS_ACCESS);      /* no extended tasks, always fail.  */
#elif defined(OS_ECC1) || defined(OS_ECC2)
    EventMaskType EventsSet;

    Os_SaveServiceContext(OSServiceId_WaitEvent, Mask, NULL, NULL);

    ASSERT_TASK_IS_EXTENDED(Os_CurrentTID);
    ASSERT_CURR_TASK_OCCUPIES_NO_RESOURCES();
    ASSERT_VALID_CALLEVEL(OS_CL_TASK);
    ASSERT_INTERRUPTS_ENABLED_AT_TASK_LEVEL();

    OsPort_DisableAllOsInterrupts();
    EventsSet = OsTask_GetEventsSet(Os_CurrentTID);
    OsTask_WaitForEventMask(Os_CurrentTID, Mask);
    OsTask_UnlockInternalResource();

    if ((EventsSet & Mask) == (EventMaskType)0) {
        /*  no events set, we have to wait...  */
        OsTask_Wait(Os_CurrentTID);
        Os_ClearServiceContext();
        OsPort_EnableAllOsInterrupts();
        OsExec_ForceScheduleFromTaskLevel();
    } else {
        OsPort_EnableAllOsInterrupts();
    }

    Os_ClearServiceContext();
    return E_OK;
#endif
}


#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_OS_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */
