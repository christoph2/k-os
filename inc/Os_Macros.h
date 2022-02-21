/*
 *  k_os (Konnex Operating-System based on the OSEK/VDX-Standard).
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
/** @file Os_Macros.h
 *  @brief Collection of preprocessor defines to get the job done.
 *
 *
 *
 *  @author Christoph Schueler (cpu12.gems@googlemail.com)
 */
#if !defined(__OSMACROS_H)
#define __OSMACROS_H

#if defined(__IAR_SYSTEMS_ICC__)
#pragma diag_suppress=Pe236
#endif

#if defined(__HIWARE__)
#pragma MESSAGE DISABLE C4000
#pragma MESSAGE DISABLE C4001
#endif

#define OSEK_SUCCESS(code)              (((code) == E_OK) ? TRUE : FALSE)

#if !defined(KDK_TOS)
#define KDK_TOS(mem, size)              ((uint8 *)(mem) + ((size) - (uint8)1))
#endif /* KDK_TOS */

#define OsExec_GetHighestPrioReady()    OsMLQ_GetHighestPrio()

#define OsExec_SetHighestPrioRunning()                          \
    _BEGIN_BLOCK                                                \
    Os_CurrentTID           = OsExec_GetHighestPrioReady();     \
    Os_CurrentTCB           = &OS_TCB[Os_CurrentTID];           \
    Os_CurrentTCB->State    = RUNNING;                          \
    OsTask_LockInternalResource();                              \
    OsExec_CallPreTaskHook();                                   \
    _END_BLOCK

/*
** ORTI and other Debuggging-Strategies require global accessible Identifiers.
*/
#if OS_FEATURE_ORTI_DEBUG == STD_ON
#define OS_DECLARE_GLOBAL_IF_DEBUGGING(var, type)   extern type var
#define OS_DEFINE_GLOBAL_IF_DEBUGGING(var, type)    type var
#else
#define OS_DECLARE_GLOBAL_IF_DEBUGGING(var, type)
#define OS_DEFINE_GLOBAL_IF_DEBUGGING(var, type)    static type var
#endif /* OS_FEATURE_ORTI_DEBUG */

#if defined(OS_SCHED_POLICY_NON)
#define OsTask_IsPreemptable(tid)                   (FALSE)
#elif defined(OS_SCHED_POLICY_PRE)
#define OsTask_IsPreemptable(tid)                   (TRUE)
#elif defined(OS_SCHED_POLICY_MIX)
#if (OS_FEATURE_RESOURCES == STD_ON) || (OS_FEATURE_INTERNAL_RESOURCES == STD_ON)
#define OsTask_IsPreemptable(tid)                   (Os_CurrentTCB->CurrentPriority != PRIO_SCHEDULER)
#else
/* todo: CHECK!!! */
/* #define OsTask_IsPreemptable(tid) (OS_TaskConf[(tid)].Priority=!=PRIO_SCHEDULER) */
#define OsTask_IsPreemptable(tid)                   (TRUE)
#endif /* (OS_FEATURE_RESOURCES == STD_ON) || (OS_FEATURE_INTERNAL_RESOURCES == STD_ON) */
#else
#error "unknwon Scheduling-Policy!"
#endif

#define OsTask_IsExtended(tid)            ((OS_TaskConf[(tid)].Flags & OS_TASK_ATTR_EXTENDED) == OS_TASK_ATTR_EXTENDED)

#define OsTask_IsSuspended(tid)           (OS_TCB[(tid)].State == SUSPENDED)
#define OsTask_IsReady(tid)               (OS_TCB[(tid)].State == READY)
#define OsTask_IsWaiting(tid)             (OS_TCB[(tid)].State == WAITING)
#define OsTask_IsRunning(tid)             (Os_CurrentTID == (tid))

#define OsTask_GetEventsSet(tid)            (OS_TCB[(tid)].EventsSet)
#define OsTask_SetEventMask(tid, evt)       (OS_TCB[(tid)].EventsSet |= (evt))
#define OsTask_ClearEventMask(tid, evt)     (OS_TCB[(tid)].EventsSet &= ((uint8) ~(evt)))
#define OsTask_WaitForEventMask(tid, evt)   (OS_TCB[(tid)].EventsWaitingFor = (evt))
#define OsTask_GetEventsWaitingFor(tid)     (OS_TCB[(tid)].EventsWaitingFor)

#if defined(OS_ECC1) || defined(OS_ECC2)
#define OsTask_ClearAllEvents(tid)                              \
    _BEGIN_BLOCK                                                \
    if (OsTask_IsExtended((tid))) {                             \
        OS_TCB[(tid)].EventsSet        = (EventMaskType)0x00;   \
        OS_TCB[(tid)].EventsWaitingFor = (EventMaskType)0x00;   \
    }                                                           \
    _END_BLOCK
#elif defined(OS_BCC1) || defined(OS_BCC2)
        #define OsTask_ClearAllEvents(tid)
#endif

/*
**
**  OsExec_ForceScheduleFromTaskLevel()
**
**  Always schedule, if on Task-Level.
**
**
*/
#define OsExec_ForceScheduleFromTaskLevel()                     \
    _BEGIN_BLOCK                                                \
    if (!Os_RunningOnISRLevel()) {                              \
        CPU_SOFTWARE_INTERRUPT();                               \
    }                                                           \
    _END_BLOCK

/*
**
**  OsExec_CondScheduleFromTaskLevel()
**
**  Conditional Scheduling, if running Task is preempteble
**  and called from Task-Level.
**
*/
#if defined(OS_SCHED_POLICY_PRE)
#define OsExec_CondScheduleFromTaskLevel()                      \
    _BEGIN_BLOCK                                                \
    if (!Os_RunningOnISRLevel()) {                              \
        CPU_SOFTWARE_INTERRUPT();                               \
    }                                                           \
    _END_BLOCK
#elif defined(OS_SCHED_POLICY_NON)
#define OsExec_CondScheduleFromTaskLevel()
#elif defined(OS_SCHED_POLICY_MIX)
#define OsExec_CondScheduleFromTaskLevel()                      \
    _BEGIN_BLOCK                                                \
    if (!Os_RunningOnISRLevel() &&                              \
        OsTask_IsPreemptable(Os_CurrentTID)) {                  \
        CPU_SOFTWARE_INTERRUPT();                               \
    }                                                           \
    _END_BLOCK
#endif

#if     defined(OS_BCC1) || defined(OS_ECC1)
#define OsTask_IncrActivations(tid)
#define OsTask_DecrActivations(tid)
#elif   defined(OS_BCC2) || defined(OS_ECC2) || (OS_FEATURE_ORTI_DEBUG == STD_ON)
#define OsTask_IncrActivations(tid)         OS_TCB[(tid)].Activations++
#define OsTask_DecrActivations(tid)         OS_TCB[(tid)].Activations--
#endif

#if     (OS_FEATURE_RESSCHEDULER == STD_ON)
#define OsExec_LockScheduler()              (Os_Flags |= OS_SYS_FLAG_SCHED_OCCUPIED)
#define OsExec_UnlockScheduler()            (Os_Flags &= (uint8) ~OS_SYS_FLAG_SCHED_OCCUPIED)
#define OsExec_IsSchedulerLocked()          ((Os_Flags & OS_SYS_FLAG_SCHED_OCCUPIED) == OS_SYS_FLAG_SCHED_OCCUPIED)
#else
#define OsExec_LockScheduler()
#define OsExec_UnlockScheduler()
#define OsExec_IsSchedulerLocked()          (FALSE)
#endif  /* OS_FEATURE_RESSCHEDULER */

#define Os_SetISRLevel()                    (Os_Flags |= OS_SYS_FLAG_ISR_LEVEL)
#define Os_SetTaskLevel()                   (Os_Flags &= (uint8) ~OS_SYS_FLAG_ISR_LEVEL)
#define Os_RunningOnISRLevel()              ((Os_Flags & OS_SYS_FLAG_ISR_LEVEL) == OS_SYS_FLAG_ISR_LEVEL)

#define OsExec_IdleTimeAction()             CPU_ENTER_POWERDOWN_MODE() /*  'osconfig.h'  */
/*  #define     OsExec_IdleTimeAction()   IdleTimeHook()  */

#if (OS_EXTENDED_STATUS == STD_ON) && (OS_FEATURE_CALLEVEL_CHECK == STD_ON)
#define Os_SetCallLevel(cl)                 (Os_Callevel = (cl))
#else
#define Os_SetCallLevel(cl)
#endif

#if (OS_EXTENDED_STATUS == STD_ON) && (OS_FEATURE_CALLEVEL_CHECK == STD_ON)
#define Os_GetCallLevel()                   (Os_Callevel)
#else
#define Os_GetCallLevel()
#endif

#if (OS_EXTENDED_STATUS == STD_ON) && (OS_FEATURE_CALLEVEL_CHECK == STD_ON)
#define Os_SaveCallLevel()                  (Os_CallevelSaved = Os_Callevel)
#else
#define Os_SaveCallLevel()
#endif

#if (OS_EXTENDED_STATUS == STD_ON) && (OS_FEATURE_CALLEVEL_CHECK == STD_ON)
#define Os_RestoreCallLevel()               (Os_Callevel = Os_CallevelSaved)
#else
#define Os_RestoreCallLevel()
#endif

#if (OS_EXTENDED_STATUS == STD_ON) && (OS_FEATURE_CALLEVEL_CHECK == STD_ON)
#define ASSERT_VALID_CALLEVEL(cl)                               \
    _BEGIN_BLOCK                                                \
    if ((Os_Callevel & (cl)) == OS_CL_INVALID) {                \
        Os_CallErrorHookAndReturn(E_OS_CALLEVEL);               \
    }                                                           \
    _END_BLOCK
#else
#define ASSERT_VALID_CALLEVEL(cl)
#endif

/*
** !REQ!AS!OS367! "Operating System services wich do not return a StatusType shall not raise the error hook(s)."
*/
#if (OS_EXTENDED_STATUS == STD_ON) && (OS_FEATURE_CALLEVEL_CHECK == STD_ON)
#define ASSERT_OS_NOT_RUNNING()                                 \
    _BEGIN_BLOCK                                                \
    if (Os_Callevel != OS_CL_INVALID) {                         \
        return;                                                 \
    }                                                           \
    _END_BLOCK
#else
#define ASSERT_OS_NOT_RUNNING()
#endif

#define OsExec_WaitForReadyTasks()                              \
    _BEGIN_BLOCK                                                \
    Os_CurrentTID = INVALID_TASK;                               \
    while (!OsMLQ_TasksAreReady()) {                            \
        OsExec_IdleTimeAction();                                \
    }                                                           \
    _END_BLOCK

#define Os_EnterISR()                                           \
    _BEGIN_BLOCK                                                \
    OsPort_SaveContext();                                       \
    OsPort_SwitchToISRContext();                                \
    Os_SetISRLevel();                                           \
    Os_SaveCallLevel();                                         \
    Os_SetCallLevel(OS_CL_ISR2);                                \
    _END_BLOCK

/*
** Os_SetCallLevel(OS_CL_TASK);    \
*/

#define Os_LeaveISR()                                           \
    _BEGIN_BLOCK                                                \
    Os_SetTaskLevel();                                          \
    OsExec_HigherPriorityThenCurrentReady();                    \
    OsExec_ScheduleFromISR();                                   \
    Os_RestoreCallLevel();                                      \
    OsPort_RestoreContext();                                    \
    _END_BLOCK

#if (OS_FEATURE_ERRORHOOK == STD_ON)
#define Os_CallErrorHookAndReturn(Error)                        \
    _BEGIN_BLOCK                                                \
    OsError_CallErrorHook(Error);                               \
    Os_ClearServiceContext();                                   \
    return Error;                                               \
    _END_BLOCK
#else
#define Os_CallErrorHookAndReturn(Error)                        \
    _BEGIN_BLOCK                                                \
    Os_ClearServiceContext();                                   \
    return Error;                                               \
    _END_BLOCK
#endif  /*  OS_FEATURE_ERRORHOOK */

#if defined(COM_USE_ERROR_HOOK)
#define COM_CallErrorHookAndReturn(Error)                       \
    _BEGIN_BLOCK                                                \
    COM_ErrorCallErrorHook(Error);                              \
    Os_ClearServiceContext();                                   \
    return Error;                                               \
    _END_BLOCK
#else
#define COM_CallErrorHookAndReturn(Error)                       \
    _BEGIN_BLOCK                                                \
    Os_ClearServiceContext();                                   \
    return Error;                                               \
    _END_BLOCK
#endif  /*  COM_USE_ERROR_HOOK */

#if (OS_FEATURE_ERRORHOOK == STD_ON)
#define ASSERT_INTERRUPTS_ENABLED_AT_TASK_LEVEL()                   \
    _BEGIN_BLOCK                                                    \
    if ((CPU_INTERRUPTS_DISABLED()) && (!Os_RunningOnISRLevel())) { \
        Os_CallErrorHookAndReturn(E_OS_DISABLEDINT);                \
    }                                                               \
    _END_BLOCK
#else
#define ASSERT_INTERRUPTS_ENABLED_AT_TASK_LEVEL()
#endif  /* OS_FEATURE_ERRORHOOK */

#if (OS_EXTENDED_STATUS == STD_ON)
#define ASSERT_VALID_TASKID(tid)                                    \
    _BEGIN_BLOCK                                                    \
    if (((tid) == INVALID_TASK) || ((tid) > OS_NUMBER_OF_TASKS)) {  \
        Os_CallErrorHookAndReturn(E_OS_ID);                         \
    }                                                               \
    _END_BLOCK
#else
#define ASSERT_VALID_TASKID(tid)
#endif

#if (OS_EXTENDED_STATUS == STD_ON) && (OS_FEATURE_RESOURCES == STD_ON)
#define ASSERT_CURR_TASK_OCCUPIES_NO_RESOURCES()                \
    _BEGIN_BLOCK                                                \
    if ((Os_CurrentTCB->ResourceCount != (uint8)0) ||           \
        (OsExec_IsSchedulerLocked())) {                         \
        Os_CallErrorHookAndReturn(E_OS_RESOURCE);               \
    }                                                           \
    _END_BLOCK
#else
#define ASSERT_CURR_TASK_OCCUPIES_NO_RESOURCES()
#endif

#if (OS_EXTENDED_STATUS == STD_ON)
#define ASSERT_VALID_COUNTERID(cid)                             \
    _BEGIN_BLOCK                                                \
    if ((cid) >= OS_NUMBER_OF_COUNTERS) {                       \
        Os_CallErrorHookAndReturn(E_OS_ID);                     \
    }                                                           \
    _END_BLOCK
#else
#define ASSERT_VALID_COUNTERID(cid)
#endif

#if (OS_EXTENDED_STATUS == STD_ON)
#define ASSERT_VALID_COUNTER_VALUE(cid, value)                  \
    _BEGIN_BLOCK                                                \
    if ((value) > Os_CounterDefs[(cid)].                        \
        CounterParams.maxallowedvalue) {                        \
        Os_CallErrorHookAndReturn(E_OS_VALUE);                  \
    }                                                           \
    _END_BLOCK
#else
#define ASSERT_VALID_COUNTER_VALUE(cid, value)
#endif

#if (OS_EXTENDED_STATUS == STD_ON)
#define ASSERT_VALID_ALARMID(aid)                               \
    _BEGIN_BLOCK                                                \
    if ((aid) >= OS_NUMBER_OF_ALARMS) {                         \
        Os_CallErrorHookAndReturn(E_OS_ID);                     \
    }                                                           \
    _END_BLOCK
#else
#define ASSERT_VALID_ALARMID(aid)
#endif

#define WARN_IF_ALARM_IS_NOT_RUNNING(aid)                       \
    _BEGIN_BLOCK                                                \
    if (!OsAlm_IsRunning((aid))) {                              \
        Os_CallErrorHookAndReturn(E_OS_NOFUNC);                 \
    }                                                           \
    _END_BLOCK

#define WARN_IF_ALARM_IS_RUNNING(aid)                           \
    _BEGIN_BLOCK                                                \
    if (OsAlm_IsRunning((aid))) {                               \
        Os_CallErrorHookAndReturn(E_OS_NOFUNC);                 \
    }                                                           \
    _END_BLOCK

#if (OS_EXTENDED_STATUS == STD_ON)
#define ASSERT_VALID_ALARM_VALUES(aid, value, cycle)                                    \
    _BEGIN_BLOCK                                                                        \
    if (((value) > Os_CounterDefs[OS_AlarmConf[(aid)].                                  \
                                  AttachedCounter].CounterParams.maxallowedvalue) ||    \
        ((cycle != (TickType)0) && (                                                    \
             (cycle > Os_CounterDefs[OS_AlarmConf[(aid)].                               \
                                     AttachedCounter].CounterParams.maxallowedvalue) || \
             (cycle < Os_CounterDefs[OS_AlarmConf[(aid)].                               \
                                     AttachedCounter].CounterParams.mincycle)))) {      \
        Os_CallErrorHookAndReturn(E_OS_VALUE);                                          \
    }                                                                                   \
    _END_BLOCK
#else
#define ASSERT_VALID_ALARM_VALUES(aid, value, cycle)
#endif

#define ASSERT_TASK_IS_EXTENDED(tid)                            \
    _BEGIN_BLOCK                                                \
    if (!OsTask_IsExtended((tid))) {                            \
        Os_CallErrorHookAndReturn(E_OS_ACCESS);                 \
    }                                                           \
    _END_BLOCK

#if (OS_EXTENDED_STATUS == STD_ON)
#define ASSERT_TASK_IS_NOT_SUSPENDED(tid)                       \
    _BEGIN_BLOCK                                                \
    if (OsTask_IsSuspended((tid))) {                            \
        Os_CallErrorHookAndReturn(E_OS_STATE);                  \
    }                                                           \
    _END_BLOCK
#else
#define ASSERT_TASK_IS_NOT_SUSPENDED(tid)
#endif

#if (OS_EXTENDED_STATUS == STD_ON)
#define ASSERT_VALID_RESOURCEID(rid)                                        \
    _BEGIN_BLOCK                                                            \
    if (((rid) != RES_SCHEDULER) && ((rid) >= OS_NUMBER_OF_RESOURCES)) {    \
        Os_CallErrorHookAndReturn(E_OS_ID);                                 \
    }                                                                       \
    _END_BLOCK
#else
#define ASSERT_VALID_RESOURCEID(rid)
#endif

#if (OS_EXTENDED_STATUS == STD_ON)
#define ASSERT_RESOURCE_IS_OCCUPIED(rid)                        \
    _BEGIN_BLOCK                                                \
    if (rid == RES_SCHEDULER) {                                 \
        if (!OsExec_IsSchedulerLocked()) {                      \
            Os_CallErrorHookAndReturn(E_OS_NOFUNC);             \
        }                                                       \
    } else if (Os_Resources[rid].Locker == INVALID_TASK) {      \
        Os_CallErrorHookAndReturn(E_OS_NOFUNC);                 \
    }                                                           \
    _END_BLOCK
#else
#define ASSERT_RESOURCE_IS_OCCUPIED(rid)
#endif

#if (OS_EXTENDED_STATUS == STD_ON)
#define ASSERT_VALID_GET_RESOURCE_ACCESS(rid)                   \
    _BEGIN_BLOCK                                                \
    if (rid == RES_SCHEDULER) {                                 \
        if (OsExec_IsSchedulerLocked()) {                       \
            Os_CallErrorHookAndReturn(E_OS_ACCESS);             \
        }                                                       \
    } else if ((Os_Resources[rid].Locker != INVALID_TASK) ||    \
               (OS_TaskConf[Os_CurrentTID].Priority <           \
                OS_ResourceConf[rid].CeilingPriority)) {        \
        Os_CallErrorHookAndReturn(E_OS_ACCESS);                 \
    }                                                           \
    _END_BLOCK
#else
#define ASSERT_VALID_GET_RESOURCE_ACCESS(rid)
#endif

#if (OS_EXTENDED_STATUS == STD_ON)
#define ASSERT_VALID_RELEASE_RESOURCE_ACCESS(rid)               \
    _BEGIN_BLOCK                                                \
    if (ResID != RES_SCHEDULER) {                               \
        if (OS_ResourceConf[ResID].CeilingPriority              \
            > OS_TaskConf[Os_CurrentTID].Priority) {            \
            Os_CallErrorHookAndReturn(E_OS_ACCESS);             \
        }                                                       \
    }                                                           \
    _END_BLOCK
#else
#define ASSERT_VALID_RELEASE_RESOURCE_ACCESS(rid)
#endif

#if (OS_FEATURE_INTERNAL_RESOURCES == STD_ON)
#define OsTask_LockInternalResource() OsRes_GetInternalResource()
#else
#define OsTask_LockInternalResource()
#endif

#if (OS_FEATURE_INTERNAL_RESOURCES == STD_ON)
#define OsTask_UnlockInternalResource() OsRes_ReleaseInternalResource()
#else
#define OsTask_UnlockInternalResource()
#endif

#if defined(OS_BCC1) || defined(OS_ECC1)
#define WARN_IF_TO_MANY_ACTIVATIONS(tid)                        \
    _BEGIN_BLOCK                                                \
    if (!OsTask_IsSuspended((tid))) {                           \
        Os_CallErrorHookAndReturn(E_OS_LIMIT);                  \
    }                                                           \
    _END_BLOCK
#elif defined(OS_BCC2)
#define WARN_IF_TO_MANY_ACTIVATIONS(tid)                        \
    _BEGIN_BLOCK                                                \
    if (OS_TCB[(tid)].Activations >=                            \
        OS_TaskConf[(tid)].MaxActivations) {                    \
        Os_CallErrorHookAndReturn(E_OS_LIMIT);                  \
    }                                                           \
    _END_BLOCK
#elif defined(OS_ECC2)
#define WARN_IF_TO_MANY_ACTIVATIONS(tid)                        \
    _BEGIN_BLOCK                                                \
    if (OsTask_IsExtended((tid))) {                             \
        if (!OsTask_IsSuspended((tid))) {                       \
            Os_CallErrorHookAndReturn(E_OS_LIMIT);              \
        }                                                       \
    } else if (OS_TCB[(tid)].Activations >=                     \
               OS_TaskConf[(tid)].MaxActivations) {             \
        Os_CallErrorHookAndReturn(E_OS_LIMIT);                  \
    }                                                           \
    _END_BLOCK
#endif

#if defined(OS_USE_PRETASKHOOK)
#define OsExec_CallPreTaskHook()                                \
    _BEGIN_BLOCK                                                \
    Os_SaveCallLevel();                                         \
    Os_SetCallLevel(OS_CL_PRE_TASK_HOOK);                       \
    PreTaskHook();                                              \
    Os_RestoreCallLevel();                                      \
    _END_BLOCK
#else
#define OsExec_CallPreTaskHook()
#endif

#if defined(OS_USE_POSTTASKHOOK)
#define OsExec_CallPostTaskHook()                               \
    _BEGIN_BLOCK                                                \
    Os_SaveCallLevel();                                         \
    Os_SetCallLevel(OS_CL_POST_TASK_HOOK);                      \
    PostTaskHook();                                             \
    Os_RestoreCallLevel();                                      \
    _END_BLOCK
#else
#define OsExec_CallPostTaskHook()
#endif

/*
**
**  COM-Makros.
**
*/

#define OSEK_COM_GET_MESSAGE_OBJECT(obj) Com_MessageObjects[(obj)]

/*
**
**  COM-Assertions.
**
*/
#if defined(COM_EXTENDED_STATUS)
#define ASSERT_COM_IS_INITIALIZED()                             \
    _BEGIN_BLOCK                                                \
    if (Com_Status == COM_UNINIT) {                             \
        COM_CallErrorHookAndReturn(E_COM_SYS_UNINIT);           \
    }                                                           \
    _END_BLOCK
#else
#define ASSERT_COM_IS_INITIALIZED()
#endif

#if defined(COM_EXTENDED_STATUS)
#define ASSERT_VALID_MESSAGE_ID(mid)                            \
    _BEGIN_BLOCK                                                \
    if ((mid) >= COM_NUMBER_OF_MESSAGES) {                      \
        COM_CallErrorHookAndReturn(E_COM_ID);                   \
    }                                                           \
    _END_BLOCK
#else
#define ASSERT_VALID_MESSAGE_ID(mid)
#endif

#if defined(COM_EXTENDED_STATUS)
#define ASSERT_IS_STATIC_SENDING_MESSAGE(mid)                                       \
    _BEGIN_BLOCK                                                                    \
    if ((OSEK_COM_GET_MESSAGE_OBJECT((mid)).Property != SEND_STATIC_INTERNAL) &&    \
           (OSEK_COM_GET_MESSAGE_OBJECT((mid)).Property != SEND_STATIC_EXTERNAL)) { \
        COM_CallErrorHookAndReturn(E_COM_ID);                                       \
    }                                                                               \
    _END_BLOCK
#else
#define ASSERT_IS_STATIC_SENDING_MESSAGE(mid)
#endif

#if defined(COM_EXTENDED_STATUS)
#define ASSERT_IS_STATIC_RECEIVING_MESSAGE(mid)                                     \
    _BEGIN_BLOCK                                                                    \
    if (OSEK_COM_GET_MESSAGE_OBJECT((mid)).Property != RECEIVE_UNQUEUED_INTERNAL) { \
        COM_CallErrorHookAndReturn(E_COM_ID);                                       \
    }                                                                               \
    _END_BLOCK
#else
#define ASSERT_IS_STATIC_RECEIVING_MESSAGE(mid)
#endif

#if defined(COM_EXTENDED_STATUS)
#define ASSERT_CAN_INITIALIZE_MESSAGE(mid)                                        \
    _BEGIN_BLOCK                                                                  \
    if ((OSEK_COM_GET_MESSAGE_OBJECT((mid)).Property == SEND_STATIC_INTERNAL) ||  \
        (OSEK_COM_GET_MESSAGE_OBJECT((mid)).Property == SEND_ZERO_INTERNAL) ||    \
        (OSEK_COM_GET_MESSAGE_OBJECT((mid)).Property == SEND_ZERO_EXTERNAL) ||    \
        (OSEK_COM_GET_MESSAGE_OBJECT((mid)).Property == RECEIVE_ZERO_EXTERNAL)) { \
        COM_CallErrorHookAndReturn(E_COM_ID);                                     \
    }                                                                             \
    _END_BLOCK
#else
#define ASSERT_CAN_INITIALIZE_MESSAGE(mid)
#endif

/* #pragma diag_default=Pe236 */
#if 0
#if defined(__HIWARE__)
#pragma MESSAGE DEFAULT C4000
#pragma MESSAGE DEFAULT C4001
#endif
#endif

#endif  /*  __OSMACROS_H  */

