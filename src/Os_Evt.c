/*
   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).

 * (C) 2007-2010 by Christoph Schueler <github.com/Christoph2,
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


StatusType OsEvtSetEvent(TaskType TaskID,EventMaskType Mask)
{
    SAVE_SERVICE_CONTEXT(OSServiceId_SetEvent,TaskID,Mask,NULL);

    ASSERT_VALID_TASKID(TaskID);
    ASSERT_TASK_IS_EXTENDED(TaskID);
    ASSERT_TASK_IS_NOT_SUSPENDED(TaskID);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK|OS_CL_ISR2);
    ASSERT_INTERRUPTS_ENABLED_AT_TASK_LEVEL();

    DISABLE_ALL_OS_INTERRUPTS();
    OS_TASK_SET_EVENT(TaskID,Mask);

    if (((OS_TASK_GET_EVENTS_SET(TaskID) &
        OS_TASK_GET_EVENTS_WAITING_FOR(TaskID))!=(EventMaskType)0) && OS_IS_TASK_WAITING(TaskID)) {
            OsTask_Ready(TaskID);
    }

    ENABLE_ALL_OS_INTERRUPTS();

    CLEAR_SERVICE_CONTEXT();
    return E_OK;
}


StatusType SetEvent(TaskType TaskID,EventMaskType Mask)
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

    OSCallErrorHookAndReturn(E_OS_ACCESS);  /* no extended tasks, always fail.  */
#elif defined(OS_ECC1) || defined(OS_ECC2)

    StatusType Status=OsEvtSetEvent(TaskID,Mask);

    if (Status!=E_OK) {
        return Status;
    } else {
        OS_COND_SCHEDULE_FROM_TASK_LEVEL();
    }
#endif
    return E_OK;
}


StatusType ClearEvent(EventMaskType Mask)
{

/*
**      Standard-Status:
**              – E_OK – no error.
**      Extended-Status:
**              – E_OS_ACCESS – the calling task is not an Extended Task.
**              – E_OS_CALLEVEL – a call at the interrupt level is not allowed.
*/
    SAVE_SERVICE_CONTEXT(OSServiceId_ClearEvent,Mask,NULL,NULL);

#if defined(OS_BCC1) || defined(OS_BCC2)
    UNREFERENCED_PARAMETER(Mask);

    OSCallErrorHookAndReturn(E_OS_ACCESS);    /* no extended tasks, always fail.  */
#elif defined(OS_ECC1) || defined(OS_ECC2)
    ASSERT_TASK_IS_EXTENDED(OsCurrentTID);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK);
    ASSERT_INTERRUPTS_ENABLED_AT_TASK_LEVEL();

    DISABLE_ALL_OS_INTERRUPTS();
    OS_TASK_CLR_EVENT(OsCurrentTID,Mask);
    ENABLE_ALL_OS_INTERRUPTS();
#endif
    CLEAR_SERVICE_CONTEXT();
    return E_OK;
}


StatusType GetEvent(TaskType TaskID,EventMaskRefType Event)
{
/*
**      Standard-Status:
**              – E_OK – no error.
**      Extended-Status:
**              – E_OS_ID – the task identifier is invalid.
**              – E_OS_ACCESS – the referenced task is not an Extended Task.
**              – E_OS_STATE – the referenced task is in the suspended state.
*/
    SAVE_SERVICE_CONTEXT(OSServiceId_GetEvent,TaskID,/*Event*/NULL,NULL);

#if defined(OS_BCC1) || defined(OS_BCC2)
    UNREFERENCED_PARAMETER(TaskID);
    UNREFERENCED_PARAMETER(Event);

    OSCallErrorHookAndReturn(E_OS_ACCESS);      /* no extended tasks, always fail.  */
#elif defined(OS_ECC1) || defined(OS_ECC2)
    ASSERT_VALID_TASKID(TaskID);
    ASSERT_TASK_IS_EXTENDED(TaskID);
    ASSERT_TASK_IS_NOT_SUSPENDED(TaskID);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK|OS_CL_ISR2|OS_CL_ERROR_HOOK|OS_CL_PRE_TASK_HOOK|OS_CL_POST_TASK_HOOK);
    ASSERT_INTERRUPTS_ENABLED_AT_TASK_LEVEL();

    DISABLE_ALL_OS_INTERRUPTS();
    *Event=OS_TASK_GET_EVENTS_SET(TaskID);
    ENABLE_ALL_OS_INTERRUPTS();
#endif
    CLEAR_SERVICE_CONTEXT();
    return E_OK;
}


StatusType WaitEvent(EventMaskType Mask)
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
    SAVE_SERVICE_CONTEXT(OSServiceId_WaitEvent,Mask,NULL,NULL);

    OSCallErrorHookAndReturn(E_OS_ACCESS);      /* no extended tasks, always fail.  */
#elif defined(OS_ECC1) || defined(OS_ECC2)
    EventMaskType EventsSet;

    SAVE_SERVICE_CONTEXT(OSServiceId_WaitEvent,Mask,NULL,NULL);

    ASSERT_TASK_IS_EXTENDED(OsCurrentTID);
    ASSERT_CURR_TASK_OCCUPIES_NO_RESOURCES();
    ASSERT_VALID_CALLEVEL(OS_CL_TASK);
    ASSERT_INTERRUPTS_ENABLED_AT_TASK_LEVEL();

    DISABLE_ALL_OS_INTERRUPTS();
    EventsSet=OS_TASK_GET_EVENTS_SET(OsCurrentTID);
    OS_TASK_WAIT_FOR_EVENTS(OsCurrentTID,Mask);
    OS_UNLOCK_INTERNAL_RESOURCE();

    if ((EventsSet & Mask)==(EventMaskType)0) {
        /*  no events set, we have to wait...  */
        OsTask_Wait(OsCurrentTID);
        ENABLE_ALL_OS_INTERRUPTS();
        OS_FORCE_SCHEDULE_FROM_TASK_LEVEL();
    } else {
        ENABLE_ALL_OS_INTERRUPTS();
    }
#endif
    CLEAR_SERVICE_CONTEXT();
    return E_OK;
}
