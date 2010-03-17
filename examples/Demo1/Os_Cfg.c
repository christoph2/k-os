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
**		!!! AUTOMATICALLY GENERATED FILE - DO NOT EDIT !!!
**
*/


#include "Os_Cfg.h"

/*  todo: Makro für die Task-Definition (TCB) definieren. */

uint8 Tsk1_Stk[STACK_SIZE];
uint8 Tsk2_Stk[STACK_SIZE];
uint8 ISR_Stk[ISR_STACK_SIZE];	/* todo: OS_ISR_STACK_SIZE * OS_ISR_LEVELS */
uint8 IdleTask_Stk[IDLETASK_STACK_SIZE];

/*
** todo: 'OSDEFINEPRIORITY(INVERT_NIBBLE(x))' ist zu umständlich!!!
*/

const OsTaskConfigurationType OS_TaskConf[OS_NUM_TASKS]={
{
    GetTaskName(OsExec_IdleTask),
    (uint8*)IdleTask_Stk,
    IDLETASK_STACK_SIZE,
    ((PriorityType)0),
    OS_TASK_ATTR_FULLPREEMPT,
#if defined(OS_BCC2) || defined(OS_ECC2)	
    1,
#endif  /* OS_BCC2 || OS_ECC2 */
    OS_AUTOSTART_ALWAYS,           /* IdleTask always started. */
#if defined(OS_USE_INTERNAL_RESOURCES)
    INTERNAL_RES_NONE,
#endif
}
,
{
    GetTaskName(Task1),                             /*  Task-Entry.  - todo: GetTaskAddr() oder so!!! */
    (uint8*)Tsk1_Stk,                               /*  Stack-Addr. */
    STACK_SIZE,                                     /*  Stack-Size. */
    OSDEFINEPRIORITY(INVERT_NIBBLE(2-1)),             /*  Priority. */
    OS_TASK_ATTR_FULLPREEMPT|OS_TASK_ATTR_EXTENDED, /*  Flags.  */
#if defined(OS_BCC2) || defined(OS_ECC2)	
    1,                                              /*  max. Activations. */
#endif  /* OS_BCC2 || OS_ECC2 */
    (AppModeType)0x01,                              /*  Autostart */
#if defined(OS_USE_INTERNAL_RESOURCES)
INTERNAL_RES_NONE /*INTERNAL_RES_SCHEDULER - besser: INTERNAL_NONPREEMPT_SCHEDULER*/, 
#endif
},

{ 
    GetTaskName(Task2),                             /*  Task-Entry. */
    (uint8*)Tsk2_Stk,                               /*  Stack-Addr. */
    STACK_SIZE,                                     /*  Stack-Size. */
    OSDEFINEPRIORITY(INVERT_NIBBLE(1-1)),             /*  Priority. */
    OS_TASK_ATTR_FULLPREEMPT|OS_TASK_ATTR_EXTENDED, /*  Flags.  */
#if defined(OS_BCC2) || defined(OS_ECC2)	
    1,                                              /*  max. Activations. */
#endif  /* OS_BCC2 || OS_ECC2 */
    (AppModeType)0x01,                              /*  Autostart */
#if defined(OS_USE_INTERNAL_RESOURCES)
    INTERNAL_RES_NONE,
#endif
}
};

OsTCBType OS_TCB[OS_NUM_TASKS];

const OsResourceConfigurationType OS_ResourceConf[OS_NUM_RESOURCES]={{INVERT_NIBBLE(2)}};
const OsResourceConfigurationType OS_IntResourceConf[OS_NUM_INT_RESOURCES]={{PRIO_SCHEDULER},{INVERT_NIBBLE(2)}}; /* Index #0 ==> PRIO_SCHEDULER */

OsResourceType Os_Resources[OS_NUM_RESOURCES];    /* dynamic parameters of Resources*/

volatile TickType Os_CounterValues[OS_NUM_COUNTERS];

const AlarmType AlarmsForSysCounter[1]={Alarm1};

const CounterConfigurationType __OS_CounterDefs[OS_NUM_COUNTERS]=
{
    {&Os_CounterValues[0],{OSMAXALLOWEDVALUE_SystemCounter,OSTICKSPERBASE_SystemCounter,OSMINCYCLE_SystemCounter},(uint8)1,AlarmsForSysCounter}
};

volatile OSAlarm OS_AlarmValue[OS_NUM_ALARMS];

const AlarmConfigurationType OS_AlarmConf[OS_NUM_ALARMS]={
    {SystemCounter,ALM_CALLBACK,{(void*)GetAlarmCallbackName(Alarm1)},((AppModeType)0x01),(TickType)1000,(TickType)1000},
};

/*
**  ISR2-Wrapper.
*/
IMPLEMENT_ISR2_VECTOR(OSSystemTimerVector)

