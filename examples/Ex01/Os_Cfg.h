/*
 * k_os (Konnex Operating-System based on the OSEK/VDX-Standard).
 *
 * (C) 2007-2009 by Christoph Schueler <chris@konnex-tools.de>
 *
 * All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

/*
**
**              !!! AUTOMATICALLY GENERATED FILE - DO NOT EDIT !!!
**
*/
#if !defined(__OSCONFIG_H)
#define __OSCONFIG_H

#ifdef __cplusplus
extern "C"
{
#endif  /* __cplusplus */

#define USE_ORTI
/* #undef   USE_ORTI */

/* #define OS_USE_DO_WHILE_BLOCKS */
#undef OS_USE_DO_WHILE_BLOCKS

#define DEBUG
/*  #undef  DEBUG   */

/* #define OS_STANDARD_STATUS */
#define OS_EXTENDED_STATUS

#if     !defined(OS_EXTENDED_STATUS) && !defined(OS_STANDARD_STATUS)
#define OS_STANDARD_STATUS
/*  #undef OS_STANDARD_STATUS */
/*  #define     OS_EXTENDED_STATUS */
#undef  OS_EXTENDED_STATUS
#endif

#define OS_USE_CALLEVEL_CHECK
/* #undef OS_USE_CALLEVEL_CHECK */

#if defined(OS_USE_CALLEVEL_CHECK) && !defined(OS_EXTENDED_STATUS)
#undef OS_USE_CALLEVEL_CHECK
#endif

/* #define      OS_BCC1 */
#undef  OS_BCC1
/*  #define     OS_BCC2 */
#undef  OS_BCC2
#define OS_ECC1
/*  #undef      OS_ECC1 */
/*  #define     OS_ECC2 */
#undef  OS_ECC2

/*  #define     OS_SCHED_POLICY_NON */
#undef  OS_SCHED_POLICY_NON
/*  #define     OS_SCHED_POLICY_PRE */
#undef  OS_SCHED_POLICY_PRE
#define OS_SCHED_POLICY_MIX
/*  #undef OS_SCHED_POLICY_MIX  */



#define OS_USE_O1SCHEDULER
/* #undef OS_USE_O1_SCHEDULER */

#undef  OS_SCHEDUDLER_BM
#define OS_SCHEDUDLER_PQ
#undef  OS_SCHEDUDLER_MLQ

#define OS_SCHEDUDLER_PQ_SIZE   16

#define OS_USE_RESSCHEDULER
/*  #undef OS_USE_RESSCHEDULER */
#define OS_USE_RESOURCES
/*  #undef      OS_USE_RESOURCES */
#define OS_USE_INTERNAL_RESOURCES
/*  #undef      OS_USE_INTERNAL_RESOURCES */

/*  #define     OS_USE_STARTUPHOOK */
#undef  OS_USE_STARTUPHOOK
/*  #define OS_USE_SHUTDOWNHOOK    */
#undef OS_USE_SHUTDOWNHOOK

/* #define OS_USE_PRETASKHOOK */
#undef OS_USE_PRETASKHOOK

/* #define OS_USE_POSTTASKHOOK */
#undef OS_USE_POSTTASKHOOK

#define OS_USE_ERRORHOOK
/*  #undef      OS_USE_ERRORHOOK */

/*#define OS_USE_STACKCHECKING */
#undef OS_USE_STACKCHECKING

/*  #define OS_USE_IDLETIME_HOOK  */
#undef OS_USE_IDLETIMEHOOK

#define OS_SAVE_STARTUP_CTX
/*  #undef      OS_SAVE_STARTUP_CTX  */

#define OS_USE_GETSERVICEID
/*  #undef      OS_USE_GETSERVICEID */
#define OS_USE_PARAMETERACCESS
/*  #undef      OS_USE_PARAMETERACCESS  */

#define OS_USE_ALARMS
/* #undef OS_USE_ALARMS */

#define OS_USE_SCHEDULE_TABLES
/*#undef OS_USE_SCHEDULE_TABLES */


/*** OSEK-COM ***/
#define OS_USE_COM
/* #undef OS_USE_COM */

#if defined(OS_USE_COM)

#define COM_USE_ERROR_HOOK
/* #undef COM_USE_ERROR_HOOK */

#define COM_USE_GETSERVICEID
/* #undef COM_USE_GETSERVICEID */

#define COM_USE_PARAMETERACCESS
/* #undef COM_USE_PARAMETERACCESS */

/* #define      COM_START_COM_EXTENSION */
#undef COM_START_COM_EXTENSION

/* #define      COM_STANDARD_STATUS */
#undef COM_STANDARD_STATUS

#define COM_EXTENDED_STATUS
/* #undef COM_EXTENDED_STATUS */

#define LOCALMESSAGESONLY 1

#define COM_NUMBER_OF_MESSAGES    ((uint8)2)

#endif /* OS_USE_COM */

/****************/

#if !defined(OS_USE_ERRORHOOK)
        #undef  OS_USE_GETSERVICEID
        #undef  OS_USE_PARAMETERACCESS
#endif

#undef  OS_SCHEDULE_NON
#undef  OS_SCHEDULE_PRE
#define OS_SCHEDULE_MIX

#define OS_NUMBER_OF_TASKS            ((uint8)2+(uint8)1)
#define STACK_SIZE              ((uint8)64)
#define ISR_STACK_SIZE          ((uint8)32)
#define IDLETASK_STACK_SIZE     ((uint8)32)

#define OS_NUMBER_OF_COUNTERS         ((uint8)1)
#define OS_NUMBER_OF_ALARMS           1
#define OS_NUMBER_OF_RESOURCES        ((uint8)1)
#define OS_NUMBER_OF_INT_RESOURCES    ((uint8)2)

#define OSSTACKFILLCHAR         ((uint8)0)
#define OS_ENABLE_WATCHDOG

#define OSDEFINEPRIORITY(p)     (p)

#include "Os_Defs.h"
#include "Os_Port.h"

#if (OS_NUMBER_OF_ALARMS>16) && (OS_NUMBER_OF_ALARMS<=32)
typedef uint32 Os_AlarmStateType;
#elif (OS_NUMBER_OF_ALARMS>8)
typedef uint16 Os_AlarmStateType;
#elif (OS_NUMBER_OF_ALARMS>=0)
typedef uint8 Os_AlarmStateType;
#else
#error OS-Configuration Error: Wrong number of Alarms (<0 or >32).
#endif

extern uint8 Tsk1_Stk[STACK_SIZE];
extern uint8 Tsk2_Stk[STACK_SIZE];
extern uint8 ISR_Stk[ISR_STACK_SIZE];
extern uint8 IdleTask_Stk[IDLETASK_STACK_SIZE];

extern OsTCBType OS_TCB[OS_NUMBER_OF_TASKS];
extern const OsTaskConfigurationType OS_TaskConf[OS_NUMBER_OF_TASKS];

extern const OsResourceConfigurationType OS_ResourceConf[];
extern const OsResourceConfigurationType OS_IntResourceConf[];

extern OsResourceType Os_Resources[];

extern volatile TickType Os_CounterValues[OS_NUMBER_OF_COUNTERS];
extern const CounterConfigurationType __OS_CounterDefs[OS_NUMBER_OF_COUNTERS];

extern volatile OSAlarm OS_AlarmValue[OS_NUMBER_OF_ALARMS];
extern const AlarmConfigurationType OS_AlarmConf[OS_NUMBER_OF_ALARMS];

DeclareTask(Task1);
#define Task1   ((TaskType)1)

DeclareTask(Task2);
#define Task2   ((TaskType)2)

#define SystemCounter   ((CounterType)0)

#define Alarm1          ((AlarmType)0)
DeclareAlarmCallback(Alarm1);

/*
**
**  System-Counter-Parameters.
**
*/
#define OSMAXALLOWEDVALUE_SystemCounter ((uint16)1000U)     /*  Maximum possible allowed value of the system counter in ticks.  */
#define OSTICKSPERBASE_SystemCounter    ((uint16)1000U)     /*  Number of ticks required to reach a specific unit of the system counter.  */
#define OSMINCYCLE_SystemCounter        ((uint16)10U)       /*  Minimum allowed number of ticks for a cyclic alarm of the system counter. */
#define OSTICKDURATION_SystemCounter    ((uint32)1000000UL) /*  Duration of a tick of the system counter in nanoseconds.  */

#define OSMAXALLOWEDVALUE   OSMAXALLOWEDVALUE_SystemCounter
#define OSTICKSPERBASE      OSTICKSPERBASE_SystemCounter
#define OSMINCYCLE          OSMINCYCLE_SystemCounter
#define OSTICKDURATION      OSTICKDURATION_SystemCounter


typedef enum tagISRType_Generated {
    ISRID_NONE,
    ISRID_OSSystemTimerVector,
    ISRID_SWI_Vector
} ISRType_Generated;

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* __OSCONFIG_H */
