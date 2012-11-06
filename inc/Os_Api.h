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
#if !defined(__OS_API_H)
#define __OS_API_H

/*
**
**  OSEK-OS.
**
*/

/*
**              Task-Functions.
*/
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(StatusType, OSEK_OS_CODE) ActivateTask(TaskType TaskID);
FUNC(StatusType, OSEK_OS_CODE) TerminateTask(void);
FUNC(StatusType, OSEK_OS_CODE) ChainTask(TaskType TaskID);
FUNC(StatusType, OSEK_OS_CODE) Schedule(void);
FUNC(StatusType, OSEK_OS_CODE) GetTaskID(/*@out@*/ TaskRefType TaskID);
FUNC(StatusType, OSEK_OS_CODE) GetTaskState(TaskType TaskID, /*@out@*/ TaskStateRefType State);
#else
StatusType  ActivateTask(TaskType TaskID);
StatusType  TerminateTask(void);
StatusType  ChainTask(TaskType TaskID);
StatusType  Schedule(void);
StatusType  GetTaskID(/*@out@*/ TaskRefType TaskID);
StatusType  GetTaskState(TaskType TaskID, /*@out@*/ TaskStateRefType State);


#endif /* KOS_MEMORY_MAPPING */

/*
**      Interrupt-Handling-Functions.
*/
#define EnableAllInterrupts()   CPU_ENABLE_ALL_INTERRUPTS()
#define DisableAllInterrupts()  CPU_DISABLE_ALL_INTERRUPTS()

#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) ResumeAllInterrupts(void);
FUNC(void, OSEK_OS_CODE) SuspendAllInterrupts(void);
FUNC(void, OSEK_OS_CODE) ResumeOSInterrupts(void);
FUNC(void, OSEK_OS_CODE) SuspendOSInterrupts(void);
#else
void    ResumeAllInterrupts(void);
void    SuspendAllInterrupts(void);
void    ResumeOSInterrupts(void);
void    SuspendOSInterrupts(void);


#endif /* KOS_MEMORY_MAPPING */

/*
**      Resource-Management-Functions.
*/
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(StatusType, OSEK_OS_CODE) GetResource(ResourceType ResID);
FUNC(StatusType, OSEK_OS_CODE) ReleaseResource(ResourceType ResID);
#else
StatusType  GetResource(ResourceType ResID);
StatusType  ReleaseResource(ResourceType ResID);


#endif /* KOS_MEMORY_MAPPING */

/*
**      Event-Control-Functions.
*/
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(StatusType, OSEK_OS_CODE) SetEvent(TaskType TaskID, EventMaskType Mask);
FUNC(StatusType, OSEK_OS_CODE) ClearEvent(EventMaskType Mask);
FUNC(StatusType, OSEK_OS_CODE) GetEvent(TaskType TaskID, /*@out null@*/ EventMaskRefType Event);
FUNC(StatusType, OSEK_OS_CODE) WaitEvent(EventMaskType Mask);
#else
StatusType  SetEvent(TaskType TaskID, EventMaskType Mask);
StatusType  ClearEvent(EventMaskType Mask);
StatusType  GetEvent(TaskType TaskID, /*@out null@*/ EventMaskRefType Event);
StatusType  WaitEvent(EventMaskType Mask);


#endif /* KOS_MEMORY_MAPPING */

/*
**      Counter-Management-Functions.
*/
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(StatusType, OSEK_OS_CODE) InitCounter(CounterType CounterID, TickType InitialValue);
FUNC(StatusType, OSEK_OS_CODE) IncrementCounter(CounterType CounterID);
FUNC(StatusType, OSEK_OS_CODE) GetCounterInfo(CounterType CounterID, /*@out null@*/ CtrInfoRefType Info);
FUNC(StatusType, OSEK_OS_CODE) GetCounterValue(CounterType CounterID, /*@out null@*/ TickRefType Value);
FUNC(StatusType, OSEK_OS_CODE) GetElapsedCounterValue(CounterType CounterID, /*@in @*/ TickRefType Value,
                                                      /*@out null@*/ TickRefType ElapsedValue);
#else
StatusType  InitCounter(CounterType CounterID, TickType InitialValue);
StatusType  IncrementCounter(CounterType CounterID);
StatusType  GetCounterInfo(CounterType CounterID, /*@out null@*/ CtrInfoRefType Info);
StatusType  GetCounterValue(CounterType CounterID, /*@out null@*/ TickRefType Value);
StatusType  GetElapsedCounterValue(CounterType CounterID, /*@in @*/ TickRefType Value, /*@out null@*/ TickRefType ElapsedValue);


#endif /* KOS_MEMORY_MAPPING */

/*
**      Alarm-Management-Functions.
*/
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(StatusType, OSEK_OS_CODE) GetAlarmBase(AlarmType AlarmID, /*@out@*/ AlarmBaseRefType Info);
FUNC(StatusType, OSEK_OS_CODE) GetAlarm(AlarmType AlarmID, /*@out@*/ TickRefType Tick);
FUNC(StatusType, OSEK_OS_CODE) SetRelAlarm(AlarmType AlarmID, TickType increment, TickType cycle);
FUNC(StatusType, OSEK_OS_CODE) SetAbsAlarm(AlarmType AlarmID, TickType start, TickType cycle);
FUNC(StatusType, OSEK_OS_CODE) CancelAlarm(AlarmType AlarmID);
#else
StatusType  GetAlarmBase(AlarmType AlarmID, /*@out@*/ AlarmBaseRefType Info);
StatusType  GetAlarm(AlarmType AlarmID, /*@out@*/ TickRefType Tick);
StatusType  SetRelAlarm(AlarmType AlarmID, TickType increment, TickType cycle);
StatusType  SetAbsAlarm(AlarmType AlarmID, TickType start, TickType cycle);
StatusType  CancelAlarm(AlarmType AlarmID);


#endif /* KOS_MEMORY_MAPPING */

/*
**      Execution-Control-Functions.
*/
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(AppModeType, OSEK_OS_CODE) GetActiveApplicationMode(void);
FUNC(void, OSEK_OS_CODE) StartOS(AppModeType Mode);
FUNC(void, OSEK_OS_CODE) ShutdownOS(StatusType Error);
#else
AppModeType GetActiveApplicationMode(void);
void        StartOS(AppModeType Mode);
void        ShutdownOS(StatusType Error);


#endif /* KOS_MEMORY_MAPPING */

/*
** Hook-Functions.
*/
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) PreTaskHook(void);
FUNC(void, OSEK_OS_CODE) PostTaskHook(void);
FUNC(void, OSEK_OS_CODE) StartupHook(void);
FUNC(void, OSEK_OS_CODE) ShutdownHook(StatusType Error);
#else
void    PreTaskHook(void);
void    PostTaskHook(void);
void    StartupHook(void);
void    ShutdownHook(StatusType Error);


#endif /* KOS_MEMORY_MAPPING */

/*
**
**  AUTOSAR-OS.
**
*/
ISRType GetISRID(void);             /* wird benötigt */


#if 0
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(ApplicationType, OSEK_OS_CODE) GetApplicationID(void);
FUNC(ISRType, OSEK_OS_CODE) GetISRID(void);
StatusType CallTrustedFunction(TrustedFunctionIndexType FunctionIndex, TrustedFunctionParameterRefType FunctionParams);


FUNC(AccessType, OSEK_OS_CODE) CheckISRMemoryAccess(ISRType ISRID, MemoryStartAddressType Address, MemorySizeType Size);
FUNC(AccessType, OSEK_OS_CODE) CheckTaskMemoryAccess(TaskType TaskID, MemoryStartAddressType Address, MemorySizeType Size);
FUNC(ObjectAccessType, OSEK_OS_CODE) CheckObjectAccess(ApplicationType ApplID, ObjectTypeType ObjectType, ...);
FUNC(ApplicationType, OSEK_OS_CODE) CheckObjectOwnership(ObjectTypeType ObjectType, ...);

FUNC(StatusType, OSEK_OS_CODE) StartScheduleTableRel(ScheduleTableType ScheduleTableID, TickType Offset);
FUNC(StatusType, OSEK_OS_CODE) StartScheduleTableAbs(ScheduleTableType ScheduleTableID, TickType Tickvalue);
FUNC(StatusType, OSEK_OS_CODE) StopScheduleTable(ScheduleTableType ScheduleTableID);
FUNC(StatusType, OSEK_OS_CODE) NextScheduleTable(ScheduleTableType ScheduleTableID_From, ScheduleTableType ScheduleTableID_To);
FUNC(StatusType, OSEK_OS_CODE) StartScheduleTableSynchron(ScheduleTableType ScheduleTableID);
FUNC(StatusType, OSEK_OS_CODE) SyncScheduleTable(ScheduleTableType ScheduleTableID, TickType Value);
FUNC(StatusType, OSEK_OS_CODE) SetScheduleTableAsync(ScheduleTableType ScheduleID);
FUNC(StatusType, OSEK_OS_CODE) GetScheduleTableStatus(ScheduleTableType ScheduleID,
                                                      /*@out@*/ ScheduleTableStatusRefType ScheduleStatus);

FUNC(StatusType, OSEK_OS_CODE) TerminateApplication(RestartType RestartOption);
FUNC(StatusType, OSEK_OS_CODE) DisableInterruptSource(ISRType DisableISR);
FUNC(StatusType, OSEK_OS_CODE) EnableInterruptSource(ISRType EnableISR);
FUNC(ProtectionReturnType, OSEK_OS_CODE) ProtectionHook(StatusType FatalError);
#else
ApplicationType     GetApplicationID(void);
ISRType             GetISRID(void);
StatusType          CallTrustedFunction(TrustedFunctionIndexType FunctionIndex, TrustedFunctionParameterRefType FunctionParams);
AccessType          CheckISRMemoryAccess(ISRType ISRID, MemoryStartAddressType Address, MemorySizeType Size);
AccessType          CheckTaskMemoryAccess(TaskType TaskID, MemoryStartAddressType Address, MemorySizeType Size);
ObjectAccessType    CheckObjectAccess(ApplicationType ApplID, ObjectTypeType ObjectType, ...);
ApplicationType     CheckObjectOwnership(ObjectTypeType ObjectType, ...);

StatusType  StartScheduleTableRel(ScheduleTableType ScheduleTableID, TickType Offset);
StatusType  StartScheduleTableAbs(ScheduleTableType ScheduleTableID, TickType Tickvalue);
StatusType  StopScheduleTable(ScheduleTableType ScheduleTableID);
StatusType  NextScheduleTable(ScheduleTableType ScheduleTableID_From, ScheduleTableType ScheduleTableID_To);
StatusType  StartScheduleTableSynchron(ScheduleTableType ScheduleTableID);
StatusType  SyncScheduleTable(ScheduleTableType ScheduleTableID, TickType Value);
StatusType  SetScheduleTableAsync(ScheduleTableType ScheduleID);
StatusType  GetScheduleTableStatus(ScheduleTableType ScheduleID, /*@out@*/ ScheduleTableStatusRefType ScheduleStatus);

StatusType              TerminateApplication(RestartType RestartOption);
StatusType              DisableInterruptSource(ISRType DisableISR);
StatusType              EnableInterruptSource(ISRType EnableISR);
ProtectionReturnType    ProtectionHook(StatusType FatalError);


#endif /* KOS_MEMORY_MAPPING */

#endif

#endif /* __OS_API_H */

