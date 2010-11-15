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
StatusType ActivateTask(TaskType TaskID);
StatusType TerminateTask(void);
StatusType ChainTask(TaskType TaskID);
StatusType Schedule(void);
StatusType GetTaskID(/*@out@*/TaskRefType TaskID);
StatusType GetTaskState(TaskType TaskID,/*@out@*/TaskStateRefType State);

/*
**      Interrupt-Handling-Functions.
*/
#define EnableAllInterrupts()           CPU_ENABLE_ALL_INTERRUPTS()
#define DisableAllInterrupts()          CPU_DISABLE_ALL_INTERRUPTS()

void ResumeAllInterrupts(void);
void SuspendAllInterrupts(void);
void ResumeOSInterrupts(void);
void SuspendOSInterrupts(void);

/*
**      Resource-Management-Functions.
*/
StatusType GetResource(ResourceType ResID);
StatusType ReleaseResource(ResourceType ResID);

/*
**      Event-Control-Functions.
*/
StatusType SetEvent(TaskType TaskID,EventMaskType Mask);
StatusType ClearEvent(EventMaskType Mask);
StatusType GetEvent(TaskType TaskID,/*@out null@*/EventMaskRefType Event);
StatusType WaitEvent(EventMaskType Mask);

/*
**      Counter-Management-Functions.
*/

StatusType InitCounter(CounterType CounterID,TickType InitialValue);
StatusType IncrementCounter(CounterType CounterID);
StatusType GetCounterInfo(CounterType CounterID,/*@out null@*/CtrInfoRefType Info);
StatusType GetCounterValue(CounterType CounterID,/*@out null@*/TickRefType Value);
StatusType GetElapsedCounterValue(CounterType CounterID,/*@in @*/TickRefType Value,/*@out null@*/TickRefType ElapsedValue);

/*
**      Alarm-Management-Functions.
*/
StatusType GetAlarmBase(AlarmType AlarmID,/*@out@*/AlarmBaseRefType Info);
StatusType GetAlarm(AlarmType AlarmID,/*@out@*/TickRefType Tick);
StatusType SetRelAlarm(AlarmType AlarmID,TickType increment,TickType cycle);
StatusType SetAbsAlarm(AlarmType AlarmID,TickType start,TickType cycle);
StatusType CancelAlarm(AlarmType AlarmID);

/*
**      Execution-Control-Functions.
*/
AppModeType GetActiveApplicationMode(void);
void StartOS(AppModeType Mode);
void ShutdownOS(StatusType Error);

/*
** Hook-Functions.
*/
void PreTaskHook(void);
void PostTaskHook(void);
void StartupHook(void);
void ShutdownHook(StatusType Error);

/*
**
**  AUTOSAR-OS.
**
*/

ApplicationType GetApplicationID(void);
ISRType GetISRID(void);
StatusType CallTrustedFunction(TrustedFunctionIndexType FunctionIndex,TrustedFunctionParameterRefType FunctionParams);
AccessType CheckISRMemoryAccess(ISRType ISRID,MemoryStartAddressType Address,MemorySizeType Size);
AccessType CheckTaskMemoryAccess(TaskType TaskID,MemoryStartAddressType Address,MemorySizeType Size);
ObjectAccessType CheckObjectAccess(ApplicationType ApplID,ObjectTypeType ObjectType,...);
ApplicationType CheckObjectOwnership(ObjectTypeType ObjectType,...);

StatusType StartScheduleTableRel(ScheduleTableType ScheduleTableID,TickType Offset);
StatusType StartScheduleTableAbs(ScheduleTableType ScheduleTableID,TickType Tickvalue);
StatusType StopScheduleTable(ScheduleTableType ScheduleTableID);
StatusType NextScheduleTable(ScheduleTableType ScheduleTableID_From,ScheduleTableType ScheduleTableID_To);
StatusType StartScheduleTableSynchron(ScheduleTableType ScheduleTableID);
StatusType SyncScheduleTable(ScheduleTableType ScheduleTableID,TickType Value);
StatusType SetScheduleTableAsync(ScheduleTableType ScheduleID);
StatusType GetScheduleTableStatus(ScheduleTableType ScheduleID,/*@out@*/ScheduleTableStatusRefType ScheduleStatus);

StatusType TerminateApplication(RestartType RestartOption);
StatusType DisableInterruptSource(ISRType DisableISR);
StatusType EnableInterruptSource(ISRType EnableISR);
ProtectionReturnType ProtectionHook(StatusType FatalError);

#endif /* __OS_API_H */

