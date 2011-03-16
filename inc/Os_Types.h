/*
   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).

  (C) 2007-2011 by Christoph Schueler <github.com/Christoph2,
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
#if !defined(__OS_TYPES_H)
#define __OS_TYPES_H


/*
Internal errors of the operating system:
These errors are implementation specific and not part of the
portable section. The error names reside in the same name-space as
the errors for API services mentioned above, i.e. the range of
numbers must not overlap. To show the difference in use,
the names internal errors must start with E_OS_SYS_
Examples:
• E_OS_SYS_STACK
• E_OS_SYS_PARITY
• ... and other implementation-specific errors, which
        have to be described in the vendorspecific document.
*/

#include "Std_Types.h"

#define E_AS        ((uint8)16)
#define E_COM       ((uint8)32)
#define E_COM_SYS   ((uint8)48)
#define E_NM        ((uint8)64)
#define E_TT        ((uint8)96)
#define E_FT        ((uint8)128)
#define E_RESERVED  ((uint8)160)


#if !defined(STATUSTYPEDEFINED)
#define STATUSTYPEDEFINED

typedef enum tagStatusType {
    E_OK,
    /* OSEK OS error codes (1 to 31). */
    E_OS_ACCESS,
    E_OS_CALLEVEL,
    E_OS_ID,
    E_OS_LIMIT,
    E_OS_NOFUNC,
    E_OS_RESOURCE,
    E_OS_STATE,
    E_OS_VALUE,
    /* Implementation specific OS errors. */
    E_OS_SYS_ASSERTION_FAILED,

    /*  Autosar-OS-Extensions.  */
    E_OS_SERVICEID=E_AS,
    E_OS_ILLEGAL_ADDRESS,
    E_OS_MISSINGEND,
    E_OS_DISABLEDINT,
    E_OS_STACKFAULT,
    E_OS_PROTECTION_MEMORY,
    E_OS_PROTECTION_TIME,
    E_OS_PROTECTION_ARRIVAL,
    E_OS_PROTECTION_LOCKED,
    E_OS_PROTECTION_EXCEPTION,
    /* OSEK COM error codes (32 to 63). */
    E_COM_ID=E_COM, /*  Given message or mode identifier is out of range or invalid. */
    E_COM_LENGTH,   /*  Given data length is out of range.  */
    E_COM_LIMIT,    /*  Overflow of message queue.  */
    E_COM_NOMSG,    /*  Message queue is empty. */
    /* OSEK COM Implementation-specific error codes. */
    E_COM_SYS_UNINIT=E_COM_SYS
} StatusType;

#endif  /* STATUSTYPEDEFINED  */


/*
** Datatypes / Application-Management.
*/
typedef uint8 AppModeType;

typedef uint8 PriorityType;

#define OS_AUTOSTART_NEVER     ((AppModeType)0x00)
#define OS_AUTOSTART_ALWAYS    ((AppModeType)0xff)


/*
**      Datatypes / Task-Management.
*/
typedef /*@abstract@*/ uint8 TaskType;                 /*  This data type identifies a task. */
typedef /*@abstract@*/ TaskType* TaskRefType;          /*  This data type points to a variable of TaskType.  */
typedef enum tagTaskStateType {
    SUSPENDED,
    READY,
    RUNNING,
    WAITING
} TaskStateType;                          /*  This data type identifies the state of a task.  */

typedef TaskStateType* TaskStateRefType;  /*  This data type points to a variable of the data type TaskStateType. */
typedef void (*TaskFunctionType)(void);

/*
**      Datatypes / Resource-Management.
*/
typedef uint8 ResourceType;
typedef ResourceType* ResourceRefType;


/*
** Datatypes / Event-Control.
*/
typedef uint8 EventType;
typedef uint8 EventMaskType;
typedef EventMaskType* EventMaskRefType;

typedef uint8 MessageType;


/*
**      Datatypes / Counter-Management.
*/
typedef uint8 CounterType;
typedef CounterType* CounterRefType;


/*
**      Datatypes / Alarm-Management.
*/
typedef uint16 TickType;            /* This data type represents count values in ticks.  */
                                    /* todo: Implementation-defined */
typedef TickType* TickRefType;      /* This data type points to the data type TickType.  */

typedef struct tagAlarmBaseType {   /*  Hinweis: meint die Definition eines Counters.   */
    TickType maxallowedvalue;       /*  Maximum possible allowed count value in ticks.  */
    TickType ticksperbase;          /*  Number of ticks required to reach a counter-specific (significant) unit.  */
    TickType mincycle;              /*  Smallest allowed value for the cycle-parameter  */
                                    /*  of SetRelAlarm/SetAbsAlarm) (only for systems with extended status).  */
} AlarmBaseType,*AlarmBaseRefType,CtrInfoType,*CtrInfoRefType;


typedef uint8 AlarmType;            /*  This data type represents an alarm object.  */
typedef void (*AlarmCallbackType)(void);


typedef struct tagAlarmSetEventType {
    TaskType TaskID;
    EventMaskType Mask;
} AlarmSetEventType;


typedef enum tagAlarmActionTypeType {
    ALM_SETEVENT,
    ALM_ACTIVATETASK,
    ALM_CALLBACK,
    ALM_COUNTER
} AlarmActionTypeType;


typedef union tagAlarmActionType {
    void *Dummy;
    AlarmSetEventType *Event;
    uint16 TaskID;
    AlarmCallbackType AlarmCallback;
    CounterType CounterID;
} AlarmActionType;


typedef struct tagOSAlarm {
    TickType ExpireCounter;
    TickType CycleCounter;
} OSAlarm;


typedef struct tagAlarmConfigurationType {
    CounterType AttachedCounter;
    AlarmActionTypeType ActionType;
    AlarmActionType Action;
#if defined(OS_FEATURE_AUTOSTART_ALARMS)
    AppModeType Autostart;
    TickType AlarmTime;
    TickType CycleTime;
#endif /* OS_FEATURE_AUTOSTART_ALARMS */
} AlarmConfigurationType;


typedef enum tagCounterDriverType {
    COUNTER_DRIVER_HARDWARE,
    COUNTER_DRIVER_SOFTWARE_BY_ISR,
    COUNTER_DRIVER_SOFTWARE_BY_USER
} CounterDriverType;


typedef struct tagCounterConfigurationType {
    /* CounterDriverType Driver;  */
    volatile TickType *Value;
    CtrInfoType CounterParams;
    uint8 NumAlarms;
    const AlarmType *AlarmsForCounter;
} CounterConfigurationType;


typedef struct tagOsTCBType {
    //OsPort_StackPointerType Stackpointer;
    uint8 * Stackpointer; /* todo: 'StackPointerType! */
    TaskStateType State;
#if defined(OS_BCC2) || defined(OS_ECC2) || defined(OS_FEATURE_ORTI_DEBUG)
    uint8 Activations;
#endif
#if defined(OS_ECC1) || defined(OS_ECC2)
    EventMaskType EventsSet,EventsWaitingFor;
#endif

#if defined(OS_USE_RESOURCES)
    uint8 ResourceCount;
#endif
#if defined(OS_USE_RESOURCES) || defined(OS_USE_INTERNAL_RESOURCES) // || defined(OS_FEATURE_ORTI_DEBUG)
    PriorityType CurrentPriority;
#endif
} OsTCBType;


typedef struct tagOsTaskConfigurationType {
    TaskFunctionType TaskFunction;
    uint8 *StackStart;
    uint8 StackSize;
    PriorityType Priority;
    uint8 Flags;
#if defined(OS_BCC2) || defined(OS_ECC2)
    uint8 MaxActivations;	    /* or ORTI */
#endif
#if defined(OS_FEATURE_AUTOSTART_TASKS)
    AppModeType Autostart;
#endif
#if defined(OS_USE_INTERNAL_RESOURCES)
    ResourceType InternalResource;
#endif
} OsTaskConfigurationType;


typedef struct tagOsResourceConfigurationType {
    PriorityType CeilingPriority;
} OsResourceConfigurationType;


typedef struct tagOsResourceType {
//#if defined(OS_FEATURE_ORTI_DEBUG)
    TaskType Locker;
//#endif
    PriorityType PriorPriorityOfTask;
} OsResourceType;


typedef enum tagOsCallevelType {
    OS_CL_INVALID=          ((uint16)0x0000u),
    OS_CL_TASK=             ((uint16)0x0001u),
    OS_CL_ISR2=             ((uint16)0x0002u),
    OS_CL_ERROR_HOOK=       ((uint16)0x0004u),
    OS_CL_PRE_TASK_HOOK=    ((uint16)0x0008u),
    OS_CL_POST_TASK_HOOK=   ((uint16)0x0010u),
    OS_CL_STARTUP_HOOK=     ((uint16)0x0020u),
    OS_CL_SHUTDOWN_HOOK=    ((uint16)0x0040u),
    OS_CL_ALARM_CALLBACK=   ((uint16)0x0080u),
    OS_CL_PROTECTION_HOOK=  ((uint16)0x0100u),
    OS_CL_ANY=              OS_CL_TASK|OS_CL_ISR2|OS_CL_ERROR_HOOK|OS_CL_PRE_TASK_HOOK|     \
                            OS_CL_POST_TASK_HOOK|OS_CL_STARTUP_HOOK|OS_CL_SHUTDOWN_HOOK|    \
                            OS_CL_ALARM_CALLBACK|OS_CL_PROTECTION_HOOK
} OsCallevelType;

/*
**
**  Scheduler.
**
*/
typedef struct tagOsMLQ_QueueConfigurationType {
    uint8 size;
    TaskType * const data;
} OsMLQ_QueueConfigurationType;


typedef struct tagOsMLQ_QueueType {
    uint8 head,tail;
    uint8 entries;
} OsMLQ_QueueType;

/*
**      AUTOSAR-OS.
*/

typedef uint8 ApplicationType;

typedef uint8 TrustedFunctionIndexType;

typedef void* TrustedFunctionParameterRefType;  /*  t.b.d.  */

typedef uint8 AccessType;

typedef enum tagObjectAccessType {
    ACCESS,
    NO_ACCESS
} ObjectAccessType;

typedef enum tagObjectTypeType {
    OBJECT_TASK,OBJECT_ISR,OBJECT_ALARM,OBJECT_RESOURCE,
    OBJECT_COUNTER,OBJECT_SCHEDULETABLE/*,OBJECT_MESSAGE*/
} ObjectTypeType;

typedef void *MemoryStartAddressType;

typedef uint16 MemorySizeType;

typedef uint8 ISRType;

typedef uint8 ScheduleTableType;

typedef enum tagScheduleTableStatusType {
    /*SCHEDULETABLE_NOT_STARTED,*/
    SCHEDULETABLE_STOPPED,SCHEDULETABLE_NEXT,SCHEDULETABLE_WAITING,
    SCHEDULETABLE_RUNNING,SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS
} ScheduleTableStatusType,*ScheduleTableStatusRefType;

typedef enum tagProtectionReturnType {
    PRO_IGNORE,PRO_TERMINATETASKISR,PRO_TERMINATEAPPL,PRO_TERMINATEAPPL_RESTART,PRO_SHUTDOWN
} ProtectionReturnType;

typedef enum tagRestartType {
    RESTART,NO_RESTART
} RestartType;

typedef uint16 PhysicalTimeType;

#endif /* __OS_TYPES_H */
