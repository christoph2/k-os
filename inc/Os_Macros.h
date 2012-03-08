/*
 *  k_os (Konnex Operating-System based on the OSEK/VDX-Standard).
 *
 * (C) 2007-2012 by Christoph Schueler <github.com/Christoph2,
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
#if !defined(__OSMACROS_H)
#define __OSMACROS_H

#if defined(__IAR_SYSTEMS_ICC__)
#pragma diag_suppress=Pe236
#endif

#if defined(__HIWARE__)
#pragma MESSAGE DISABLE C4000
#pragma MESSAGE DISABLE C4001
#endif

#define OSEK_SUCCESS(code)          (((code) == E_OK) ? TRUE : FALSE)

#define OS_GET_HIGHEST_PRIO_READY() OsMLQ_GetHighestPrio()

#define OS_SET_HIGHEST_PRIO_RUNNING()                     \
    _BEGIN_BLOCK                                          \
    OsCurrentTID           = OS_GET_HIGHEST_PRIO_READY(); \
    OsCurrentTCB           = &OS_TCB[OsCurrentTID];       \
    OsCurrentTCB->State    = RUNNING;                     \
    OS_LOCK_INTERNAL_RESOURCE();                          \
    OS_CALL_PRE_TASK_HOOK();                              \
    _END_BLOCK

/*
** ORTI and other Debuggging-Strategies require global accessible Identifiers.
*/
#if defined(OS_FEATURE_ORTI_DEBUG)
#define OS_DECLARE_GLOBAL_IF_DEBUGGING(var, type)   extern type var
#define OS_DEFINE_GLOBAL_IF_DEBUGGING(var, type)    type var
#else
#define OS_DECLARE_GLOBAL_IF_DEBUGGING(var, type)
#define OS_DEFINE_GLOBAL_IF_DEBUGGING(var, type)    static type var
#endif /* OS_FEATURE_ORTI_DEBUG */

#if defined(OS_SCHED_POLICY_NON)
#define OS_IS_TASK_PREEMPTABLE(tid)                 ((boolean)FALSE)
#elif defined(OS_SCHED_POLICY_PRE)
#define OS_IS_TASK_PREEMPTABLE(tid)                 ((boolean)TRUE)
#elif defined(OS_SCHED_POLICY_MIX)
#if defined(OS_USE_RESOURCES) || defined(OS_USE_INTERNAL_RESOURCES)
#define OS_IS_TASK_PREEMPTABLE(tid)                 (OsCurrentTCB->CurrentPriority != PRIO_SCHEDULER)
#else
/* todo: CHECK!!! */
/* #define OS_IS_TASK_PREEMPTABLE(tid) (OS_TaskConf[(tid)].Priority=!=PRIO_SCHEDULER) */
#define OS_IS_TASK_PREEMPTABLE(tid)                 ((boolean)TRUE)
#endif /* (OS_USE_RESOURCES) || (OS_USE_INTERNAL_RESOURCES) */
#else
#error "unknwon Scheduling-Policy!"
#endif

#define OS_IS_TASK_EXTENDED(tid)            ((OS_TaskConf[(tid)].Flags & OS_TASK_ATTR_EXTENDED) == OS_TASK_ATTR_EXTENDED)

#define OS_IS_TASK_SUSPENDED(tid)           (OS_TCB[(tid)].State == SUSPENDED)
#define OS_IS_TASK_READY(tid)               (OS_TCB[(tid)].State == READY)
#define OS_IS_TASK_WAITING(tid)             (OS_TCB[(tid)].State == WAITING)
#define OS_IS_TASK_RUNNING(tid)             (OsCurrentTID == (tid))

#define OS_TASK_GET_EVENTS_SET(tid)         (OS_TCB[(tid)].EventsSet)
#define OS_TASK_SET_EVENT(tid, evt)         (OS_TCB[(tid)].EventsSet |= (evt))
#define OS_TASK_CLR_EVENT(tid, evt)         (OS_TCB[(tid)].EventsSet &= (~(evt)))
#define OS_TASK_WAIT_FOR_EVENTS(tid, evt)   (OS_TCB[(tid)].EventsWaitingFor = (evt))
#define OS_TASK_GET_EVENTS_WAITING_FOR(tid) (OS_TCB[(tid)].EventsWaitingFor)

#if defined(OS_ECC1) || defined(OS_ECC2)
#define OS_TASK_CLEAR_ALL_EVENTS(tid)                         \
    _BEGIN_BLOCK                                              \
    if (OS_IS_TASK_EXTENDED((tid))) {                         \
        OS_TCB[(tid)].EventsSet        = (EventMaskType)0x00; \
        OS_TCB[(tid)].EventsWaitingFor = (EventMaskType)0x00; \
    }                                                         \
    _END_BLOCK
#elif defined(OS_BCC1) || defined(OS_BCC2)
        #define OS_TASK_CLEAR_ALL_EVENTS(tid)
#endif

/*
**
**  OS_FORCE_SCHEDULE_FROM_TASK_LEVEL()
**
**  Always schedule, if on Task-Level.
**
**
*/
#define OS_FORCE_SCHEDULE_FROM_TASK_LEVEL() \
    _BEGIN_BLOCK                            \
    if (!OS_IS_ISR_LEVEL()) {               \
        CPU_SOFTWARE_INTERRUPT();           \
    }                                       \
    _END_BLOCK

/*
**
**  OS_COND_SCHEDULE_FROM_TASK_LEVEL()
**
**  Conditional Scheduling, if running Task is preempteble
**  and called from Task-Level.
**
*/
#if defined(OS_SCHED_POLICY_PRE)
#define OS_COND_SCHEDULE_FROM_TASK_LEVEL() \
    _BEGIN_BLOCK                           \
    if (!OS_IS_ISR_LEVEL()) {              \
        CPU_SOFTWARE_INTERRUPT();          \
    }                                      \
    _END_BLOCK
#elif defined(OS_SCHED_POLICY_NON)
#define OS_COND_SCHEDULE_FROM_TASK_LEVEL()
#elif defined(OS_SCHED_POLICY_MIX)
#define OS_COND_SCHEDULE_FROM_TASK_LEVEL()      \
    _BEGIN_BLOCK                                \
    if (!OS_IS_ISR_LEVEL() &&                   \
        OS_IS_TASK_PREEMPTABLE(OsCurrentTID)) { \
        CPU_SOFTWARE_INTERRUPT();               \
    }                                           \
    _END_BLOCK
#endif

#if     defined(OS_BCC1) || defined(OS_ECC1)
#define OS_TASK_INCR_ACTIVATIONS(tid)
#define OS_TASK_DECR_ACTIVATIONS(tid)
#elif   defined(OS_BCC2) || defined(OS_ECC2) || defined(OS_FEATURE_ORTI_DEBUG)
#define OS_TASK_INCR_ACTIVATIONS(tid)   OS_TCB[(tid)].Activations++
#define OS_TASK_DECR_ACTIVATIONS(tid)   OS_TCB[(tid)].Activations--
#endif

#if     defined(OS_USE_RESSCHEDULER)
#define OS_LOCK_SCHEDULER()             (OsFlags |= OS_SYS_FLAG_SCHED_OCCUPIED)
#define OS_UNLOCK_SCHEDULER()           (OsFlags &= ~OS_SYS_FLAG_SCHED_OCCUPIED)
#define OS_IS_SCHEDULER_LOCKED()        ((OsFlags & OS_SYS_FLAG_SCHED_OCCUPIED) == OS_SYS_FLAG_SCHED_OCCUPIED)
#else
#define OS_LOCK_SCHEDULER()
#define OS_UNLOCK_SCHEDULER()
#define OS_IS_SCHEDULER_LOCKED()        ((boolean)FALSE)
#endif  /* OS_USE_RESSCHEDULER */

#define OS_SET_ISR_LEVEL()              (OsFlags |= OS_SYS_FLAG_ISR_LEVEL)
#define OS_SET_TASK_LEVEL()             (OsFlags &= ~OS_SYS_FLAG_ISR_LEVEL)
#define OS_IS_ISR_LEVEL()               ((OsFlags & OS_SYS_FLAG_ISR_LEVEL) == OS_SYS_FLAG_ISR_LEVEL)

#define OS_IDLE_TIME_ACTION()           CPU_ENTER_POWERDOWN_MODE() /*  'osconfig.h'  */
/*  #define     OS_IDLE_TIME_ACTION()   IdleTimeHook()  */

#if defined(OS_EXTENDED_STATUS) && defined(OS_FEATURE_CALLEVEL_CHECK)
#define OS_SET_CALLEVEL(cl)             (OsCallevel = (cl))
#else
#define OS_SET_CALLEVEL(cl)
#endif

#if defined(OS_EXTENDED_STATUS) && defined(OS_FEATURE_CALLEVEL_CHECK)
#define OS_GET_CALLEVEL() (OsCallevel)
#else
#define OS_GET_CALLEVEL()
#endif

#if defined(OS_EXTENDED_STATUS) && defined(OS_FEATURE_CALLEVEL_CHECK)
#define OS_SAVE_CALLEVEL() (OsCallevelSaved = OsCallevel)
#else
#define OS_SAVE_CALLEVEL()
#endif

#if defined(OS_EXTENDED_STATUS) && defined(OS_FEATURE_CALLEVEL_CHECK)
#define OS_RESTORE_CALLEVEL() (OsCallevel = OsCallevelSaved)
#else
#define OS_RESTORE_CALLEVEL()
#endif

#if defined(OS_EXTENDED_STATUS) && defined(OS_FEATURE_CALLEVEL_CHECK)
#define ASSERT_VALID_CALLEVEL(cl)                \
    _BEGIN_BLOCK                                 \
    if ((OsCallevel & (cl)) == OS_CL_INVALID) {  \
        OSCallErrorHookAndReturn(E_OS_CALLEVEL); \
    }                                            \
    _END_BLOCK
#else
#define ASSERT_VALID_CALLEVEL(cl)
#endif

/*
** !REQ!AS!OS367! "Operating System services wich do not return a StatusType shall not raise the error hook(s)."
*/
#if defined(OS_EXTENDED_STATUS) && defined(OS_FEATURE_CALLEVEL_CHECK)
#define ASSERT_OS_NOT_RUNNING()        \
    _BEGIN_BLOCK                       \
    if (OsCallevel != OS_CL_INVALID) { \
        return;                        \
    }                                  \
    _END_BLOCK
#else
#define ASSERT_OS_NOT_RUNNING()
#endif

#define WAIT_FOR_READY_TASKS()       \
    _BEGIN_BLOCK                     \
    OsCurrentTID = INVALID_TASK;     \
    while (!OsMLQ_TasksAreReady()) { \
        OS_IDLE_TIME_ACTION();       \
    }                                \
    _END_BLOCK

#define OSEnterISR()             \
    _BEGIN_BLOCK                 \
    OS_SAVE_CONTEXT();           \
    OS_ISR_CONTEXT();            \
    OS_SET_ISR_LEVEL();          \
    OS_SAVE_CALLEVEL();          \
    OS_SET_CALLEVEL(OS_CL_ISR2); \
    _END_BLOCK

/*
** OS_SET_CALLEVEL(OS_CL_TASK);    \
*/

#define OSLeaveISR()                         \
    _BEGIN_BLOCK                             \
    OS_SET_TASK_LEVEL();                     \
    OsExec_HigherPriorityThenCurrentReady(); \
    OsExec_ScheduleFromISR();                \
    OS_RESTORE_CALLEVEL();                   \
    OS_RESTORE_CONTEXT();                    \
    _END_BLOCK

#if defined(OS_USE_ERRORHOOK)
#define OSCallErrorHookAndReturn(Error) \
    _BEGIN_BLOCK                        \
    OsErrorCallErrorHook(Error);        \
    CLEAR_SERVICE_CONTEXT();            \
    return Error;                       \
    _END_BLOCK
#else
#define OSCallErrorHookAndReturn(Error) \
    _BEGIN_BLOCK                        \
    CLEAR_SERVICE_CONTEXT();            \
    return Error;                       \
    _END_BLOCK
#endif  /*  OS_USE_ERRORHOOK */

#if defined(COM_USE_ERROR_HOOK)
#define COMCallErrorHookAndReturn(Error) \
    _BEGIN_BLOCK                         \
    COMErrorCallErrorHook(Error);        \
    CLEAR_SERVICE_CONTEXT();             \
    return Error;                        \
    _END_BLOCK
#else
#define COMCallErrorHookAndReturn(Error) \
    _BEGIN_BLOCK                         \
    CLEAR_SERVICE_CONTEXT();             \
    return Error;                        \
    _END_BLOCK
#endif  /*  COM_USE_ERROR_HOOK */

#if defined(OS_USE_ERRORHOOK)
#define ASSERT_INTERRUPTS_ENABLED_AT_TASK_LEVEL()              \
    _BEGIN_BLOCK                                               \
    if ((CPU_INTERRUPTS_DISABLED()) && (!OS_IS_ISR_LEVEL())) { \
        OSCallErrorHookAndReturn(E_OS_DISABLEDINT);            \
    }                                                          \
    _END_BLOCK
#else
#define ASSERT_INTERRUPTS_ENABLED_AT_TASK_LEVEL()
#endif  /*  OS_USE_ERRORHOOK */

#if defined(OS_EXTENDED_STATUS)
#define ASSERT_VALID_TASKID(tid)                                   \
    _BEGIN_BLOCK                                                   \
    if (((tid) == INVALID_TASK) || ((tid) > OS_NUMBER_OF_TASKS)) { \
        OSCallErrorHookAndReturn(E_OS_ID);                         \
    }                                                              \
    _END_BLOCK
#else
#define ASSERT_VALID_TASKID(tid)
#endif

#if defined(OS_EXTENDED_STATUS) && defined(OS_USE_RESOURCES)
#define ASSERT_CURR_TASK_OCCUPIES_NO_RESOURCES()     \
    _BEGIN_BLOCK                                     \
    if ((OsCurrentTCB->ResourceCount != (uint8)0) || \
        (OS_IS_SCHEDULER_LOCKED())) {                \
        OSCallErrorHookAndReturn(E_OS_RESOURCE);     \
    }                                                \
    _END_BLOCK
#else
#define ASSERT_CURR_TASK_OCCUPIES_NO_RESOURCES()
#endif

#if defined(OS_EXTENDED_STATUS)
#define ASSERT_VALID_COUNTERID(cid)        \
    _BEGIN_BLOCK                           \
    if ((cid) >= OS_NUMBER_OF_COUNTERS) {  \
        OSCallErrorHookAndReturn(E_OS_ID); \
    }                                      \
    _END_BLOCK
#else
#define ASSERT_VALID_COUNTERID(cid)
#endif

#if defined(OS_EXTENDED_STATUS)
#define ASSERT_VALID_COUNTER_VALUE(cid, value) \
    _BEGIN_BLOCK                               \
    if ((value) > Os_CounterDefs[(cid)].       \
        CounterParams.maxallowedvalue) {       \
        OSCallErrorHookAndReturn(E_OS_VALUE);  \
    }                                          \
    _END_BLOCK
#else
#define ASSERT_VALID_COUNTER_VALUE(cid, value)
#endif

#if defined(OS_EXTENDED_STATUS)
#define ASSERT_VALID_ALARMID(aid)          \
    _BEGIN_BLOCK                           \
    if ((aid) >= OS_NUMBER_OF_ALARMS) {    \
        OSCallErrorHookAndReturn(E_OS_ID); \
    }                                      \
    _END_BLOCK
#else
#define ASSERT_VALID_ALARMID(aid)
#endif

#define WARN_IF_ALARM_IS_NOT_RUNNING(aid)      \
    _BEGIN_BLOCK                               \
    if (!OsAlm_IsRunning((aid))) {             \
        OSCallErrorHookAndReturn(E_OS_NOFUNC); \
    }                                          \
    _END_BLOCK

#define WARN_IF_ALARM_IS_RUNNING(aid)          \
    _BEGIN_BLOCK                               \
    if (OsAlm_IsRunning((aid))) {              \
        OSCallErrorHookAndReturn(E_OS_NOFUNC); \
    }                                          \
    _END_BLOCK

#if defined(OS_EXTENDED_STATUS)
#define ASSERT_VALID_ALARM_VALUES(aid, value, cycle)                                    \
    _BEGIN_BLOCK                                                                        \
    if (((value) > Os_CounterDefs[OS_AlarmConf[(aid)].                                  \
                                  AttachedCounter].CounterParams.maxallowedvalue) ||    \
        ((cycle != (TickType)0) && (                                                    \
             (cycle > Os_CounterDefs[OS_AlarmConf[(aid)].                               \
                                     AttachedCounter].CounterParams.maxallowedvalue) || \
             (cycle < Os_CounterDefs[OS_AlarmConf[(aid)].                               \
                                     AttachedCounter].CounterParams.mincycle)))) {      \
        OSCallErrorHookAndReturn(E_OS_VALUE);                                           \
    }                                                                                   \
    _END_BLOCK
#else
#define ASSERT_VALID_ALARM_VALUES(aid, value, cycle)
#endif

#define ASSERT_TASK_IS_EXTENDED(tid)           \
    _BEGIN_BLOCK                               \
    if (!OS_IS_TASK_EXTENDED((tid))) {         \
        OSCallErrorHookAndReturn(E_OS_ACCESS); \
    }                                          \
    _END_BLOCK

#if defined(OS_EXTENDED_STATUS)
#define ASSERT_TASK_IS_NOT_SUSPENDED(tid)     \
    _BEGIN_BLOCK                              \
    if (OS_IS_TASK_SUSPENDED((tid))) {        \
        OSCallErrorHookAndReturn(E_OS_STATE); \
    }                                         \
    _END_BLOCK
#else
#define ASSERT_TASK_IS_NOT_SUSPENDED(tid)
#endif

#if defined(OS_EXTENDED_STATUS)
#define ASSERT_VALID_RESOURCEID(rid)                                     \
    _BEGIN_BLOCK                                                         \
    if (((rid) != RES_SCHEDULER) && ((rid) >= OS_NUMBER_OF_RESOURCES)) { \
        OSCallErrorHookAndReturn(E_OS_ID);                               \
    }                                                                    \
    _END_BLOCK
#else
#define ASSERT_VALID_RESOURCEID(rid)
#endif

#if defined(OS_EXTENDED_STATUS)
#define ASSERT_RESOURCE_IS_OCCUPIED(rid)                   \
    _BEGIN_BLOCK                                           \
    if (rid == RES_SCHEDULER) {                            \
        if (!OS_IS_SCHEDULER_LOCKED()) {                   \
            OSCallErrorHookAndReturn(E_OS_NOFUNC);         \
        }                                                  \
    } else if (Os_Resources[rid].Locker == INVALID_TASK) { \
        OSCallErrorHookAndReturn(E_OS_NOFUNC);             \
    }                                                      \
    _END_BLOCK
#else
#define ASSERT_RESOURCE_IS_OCCUPIED(rid)
#endif

#if defined(OS_EXTENDED_STATUS)
#define ASSERT_VALID_GET_RESOURCE_ACCESS(rid)                \
    _BEGIN_BLOCK                                             \
    if (rid == RES_SCHEDULER) {                              \
        if (OS_IS_SCHEDULER_LOCKED()) {                      \
            OSCallErrorHookAndReturn(E_OS_ACCESS);           \
        }                                                    \
    } else if ((Os_Resources[rid].Locker != INVALID_TASK) || \
               (OS_TaskConf[OsCurrentTID].Priority <         \
                OS_ResourceConf[rid].CeilingPriority)) {     \
        OSCallErrorHookAndReturn(E_OS_ACCESS);               \
    }                                                        \
    _END_BLOCK
#else
#define ASSERT_VALID_GET_RESOURCE_ACCESS(rid)
#endif

#if defined(OS_EXTENDED_STATUS)
#define ASSERT_VALID_RELEASE_RESOURCE_ACCESS(rid)   \
    _BEGIN_BLOCK                                    \
    if (ResID != RES_SCHEDULER) {                   \
        if (OS_ResourceConf[ResID].CeilingPriority  \
            > OS_TaskConf[OsCurrentTID].Priority) { \
            OSCallErrorHookAndReturn(E_OS_ACCESS);  \
        }                                           \
    }                                               \
    _END_BLOCK
#else
#define ASSERT_VALID_RELEASE_RESOURCE_ACCESS(rid)
#endif

#if defined(OS_USE_INTERNAL_RESOURCES)
#define OS_LOCK_INTERNAL_RESOURCE() OsRes_GetInternalResource()
#else
#define OS_LOCK_INTERNAL_RESOURCE()
#endif

#if defined(OS_USE_INTERNAL_RESOURCES)
#define OS_UNLOCK_INTERNAL_RESOURCE() OsRes_ReleaseInternalResource()
#else
#define OS_UNLOCK_INTERNAL_RESOURCE()
#endif

#if defined(OS_BCC1) || defined(OS_ECC1)
#define WARN_IF_TO_MANY_ACTIVATIONS(tid)      \
    _BEGIN_BLOCK                              \
    if (!OS_IS_TASK_SUSPENDED((tid))) {       \
        OSCallErrorHookAndReturn(E_OS_LIMIT); \
    }                                         \
    _END_BLOCK
#elif defined(OS_BCC2)
#define WARN_IF_TO_MANY_ACTIVATIONS(tid)      \
    _BEGIN_BLOCK                              \
    if (OS_TCB[(tid)].Activations >=          \
        OS_TaskConf[(tid)].MaxActivations) {  \
        OSCallErrorHookAndReturn(E_OS_LIMIT); \
    }                                         \
    _END_BLOCK
#elif defined(OS_ECC2)
#define WARN_IF_TO_MANY_ACTIVATIONS(tid)            \
    _BEGIN_BLOCK                                    \
    if (OS_IS_TASK_EXTENDED((tid))) {               \
        if (!OS_IS_TASK_SUSPENDED((tid))) {         \
            OSCallErrorHookAndReturn(E_OS_LIMIT);   \
        }                                           \
    } else if (OS_TCB[(tid)].Activations >=         \
               OS_TaskConf[(tid)].MaxActivations) { \
        OSCallErrorHookAndReturn(E_OS_LIMIT);       \
    }                                               \
    _END_BLOCK
#endif

#if defined(OS_USE_PRETASKHOOK)
#define OS_CALL_PRE_TASK_HOOK()           \
    _BEGIN_BLOCK                          \
    OS_SAVE_CALLEVEL();                   \
    OS_SET_CALLEVEL(OS_CL_PRE_TASK_HOOK); \
    PreTaskHook();                        \
    OS_RESTORE_CALLEVEL();                \
    _END_BLOCK
#else
#define OS_CALL_PRE_TASK_HOOK()
#endif

#if defined(OS_USE_POSTTASKHOOK)
#define OS_CALL_POST_TASK_HOOK()           \
    _BEGIN_BLOCK                           \
    OS_SAVE_CALLEVEL();                    \
    OS_SET_CALLEVEL(OS_CL_POST_TASK_HOOK); \
    PostTaskHook();                        \
    OS_RESTORE_CALLEVEL();                 \
    _END_BLOCK
#else
#define OS_CALL_POST_TASK_HOOK()
#endif

/*
**
**  COM-Makros.
**
*/

#define GET_MESSAGE_OBJECT(obj) Com_MessageObjects[(obj)]

/*
**
**  COM-Assertions.
**
*/
#if defined(COM_EXTENDED_STATUS)
#define ASSERT_COM_IS_INITIALIZED()                  \
    _BEGIN_BLOCK                                     \
    if (Com_Status == COM_UNINIT) {                  \
        COMCallErrorHookAndReturn(E_COM_SYS_UNINIT); \
    }                                                \
    _END_BLOCK
#else
#define ASSERT_COM_IS_INITIALIZED()
#endif

#if defined(COM_EXTENDED_STATUS)
#define ASSERT_VALID_MESSAGE_ID(mid)         \
    _BEGIN_BLOCK                             \
    if ((mid) >= COM_NUMBER_OF_MESSAGES) {   \
        COMCallErrorHookAndReturn(E_COM_ID); \
    }                                        \
    _END_BLOCK
#else
#define ASSERT_VALID_MESSAGE_ID(mid)
#endif

#if defined(COM_EXTENDED_STATUS)
#define ASSERT_IS_STATIC_SENDING_MESSAGE(mid)                         \
    _BEGIN_BLOCK                                                      \
    if (GET_MESSAGE_OBJECT((mid)).Property != SEND_STATIC_INTERNAL) { \
        COMCallErrorHookAndReturn(E_COM_ID);                          \
    }                                                                 \
    _END_BLOCK
#else
#define ASSERT_IS_STATIC_SENDING_MESSAGE(mid)
#endif

#if defined(COM_EXTENDED_STATUS)
#define ASSERT_IS_STATIC_RECEIVING_MESSAGE(mid)                            \
    _BEGIN_BLOCK                                                           \
    if (GET_MESSAGE_OBJECT((mid)).Property != RECEIVE_UNQUEUED_INTERNAL) { \
        COMCallErrorHookAndReturn(E_COM_ID);                               \
    }                                                                      \
    _END_BLOCK
#else
#define ASSERT_IS_STATIC_RECEIVING_MESSAGE(mid)
#endif

#if defined(COM_EXTENDED_STATUS)
#define ASSERT_CAN_INITIALIZE_MESSAGE(mid)                             \
    _BEGIN_BLOCK                                                       \
    if (GET_MESSAGE_OBJECT((mid)).Property == SEND_STATIC_INTERNAL ||  \
        GET_MESSAGE_OBJECT((mid)).Property == SEND_ZERO_INTERNAL ||    \
        GET_MESSAGE_OBJECT((mid)).Property == SEND_ZERO_EXTERNAL ||    \
        GET_MESSAGE_OBJECT((mid)).Property == RECEIVE_ZERO_EXTERNAL) { \
        COMCallErrorHookAndReturn(E_COM_ID);                           \
    }                                                                  \
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
