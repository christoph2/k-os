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

#include "Os_SchT.h"

/*
**
**  internal States: STOPPED,WAITING,RUNNING,TERMINATING.
**
*/

/*
**  !REQ!AS!OS429! "A schedule table that is implicitly synchronized shall have a 
**  Duration equal to OsCounterMaxAllowedValue+1 of is associated OSEK OS counter."
*/

/*
**  !REQ!AS!OS430! "The OS shall prevent a schedule table that is implicitly
**  synchronized from being started ât a relative count value."
*/

#define INVALID_SCHEDULE_TABLE  ((uint8)0xff)


typedef struct tagOsSchTType {
    uint8 InternalState;
    ScheduleTableStatusType ExternalState;
    TickType Delay;
    
    ScheduleTableType CurrentTable;
    ScheduleTableType NextTable;
    uint8 CurrentPoint;  /* CurrentEntry */
} OsSchTType;


typedef enum tagOsSchTSyncStrategyType {
    SCHT_SYNC_NONE,
    SCHT_SYNC_IMPLICIT,
    SCHT_SYNC_EXPLICIT
} OsSchTSyncStrategyType;


typedef struct tagOsScheduleTableConfigurationType {
/* 1..* ExpiryPoints */
    
    TickType TableDuration;
    boolean Repeating;
    CounterType CounterID;
    OsSchTSyncStrategyType SyncStrategy; 
    TickType ExplicitPrecision;
    TickType MaxRetard;
    TickType MaxAdvance;
} OsScheduleTableConfigurationType;


typedef struct tagOsExpiryPointType {    
    TickType Offset;
    TickType MaxRetard;  /* to be substracted */
    TickType MaxAdvance; /* to be added */

	/*
**  todo: TaskActivations + Events.
**
*/    
} OsExpiryPointType;


StatusType StartScheduleTableRel(ScheduleTableType ScheduleTableID,TickType Offset)
{
    SAVE_SERVICE_CONTEXT(OSServiceId_StartScheduleTableRel,ScheduleTableID,Offset,NULL);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK|OS_CL_ISR2);

    DISABLE_ALL_OS_INTERRUPTS();
    
    ENABLE_ALL_OS_INTERRUPTS();    
    CLEAR_SERVICE_CONTEXT();
    return E_OK;
    
}


StatusType StartScheduleTableAbs(ScheduleTableType ScheduleTableID,TickType Tickvalue)
{
    SAVE_SERVICE_CONTEXT(OSServiceId_StartScheduleTableAbs,ScheduleTableID,Tickvalue,NULL);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK|OS_CL_ISR2);

    DISABLE_ALL_OS_INTERRUPTS();
    
    ENABLE_ALL_OS_INTERRUPTS();    
    CLEAR_SERVICE_CONTEXT();
    return E_OK;
    
}


StatusType StopScheduleTable(ScheduleTableType ScheduleTableID)
{
    SAVE_SERVICE_CONTEXT(OSServiceId_StopScheduleTable,ScheduleTableID,NULL,NULL);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK|OS_CL_ISR2);

    DISABLE_ALL_OS_INTERRUPTS();
    
    ENABLE_ALL_OS_INTERRUPTS(); 
    CLEAR_SERVICE_CONTEXT();
    return E_OK;
    
}


StatusType NextScheduleTable(ScheduleTableType ScheduleTableID_From,ScheduleTableType ScheduleTableID_To)
{
    SAVE_SERVICE_CONTEXT(OSServiceId_NextScheduleTable,ScheduleTableID_From,ScheduleTableID_To,NULL);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK|OS_CL_ISR2);

    DISABLE_ALL_OS_INTERRUPTS();
    
    ENABLE_ALL_OS_INTERRUPTS();    
    CLEAR_SERVICE_CONTEXT();
    return E_OK;
    
}


StatusType StartScheduleTableSynchron(ScheduleTableType ScheduleTableID)
{
    SAVE_SERVICE_CONTEXT(OSServiceId_StartScheduleTableSynchron,ScheduleTableID,NULL,NULL);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK|OS_CL_ISR2);

    DISABLE_ALL_OS_INTERRUPTS();
    
    ENABLE_ALL_OS_INTERRUPTS();    
    CLEAR_SERVICE_CONTEXT();
    return E_OK;    
}


StatusType SyncScheduleTable(ScheduleTableType ScheduleTableID,TickType Value)
{
    SAVE_SERVICE_CONTEXT(OSServiceId_SyncScheduleTable,ScheduleTableID,Value,NULL);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK|OS_CL_ISR2);

    DISABLE_ALL_OS_INTERRUPTS();
    
    ENABLE_ALL_OS_INTERRUPTS();    
    CLEAR_SERVICE_CONTEXT();
    return E_OK;
    
}


StatusType SetScheduleTableAsync(ScheduleTableType ScheduleID)
{
    SAVE_SERVICE_CONTEXT(OSServiceId_SetScheduleTableAsync,ScheduleID,NULL,NULL);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK|OS_CL_ISR2);

    DISABLE_ALL_OS_INTERRUPTS();
    
    ENABLE_ALL_OS_INTERRUPTS();    
    CLEAR_SERVICE_CONTEXT();
    return E_OK;
    
}


StatusType GetScheduleTableStatus(ScheduleTableType ScheduleID,ScheduleTableStatusRefType /*@out@*/ScheduleStatus)
{
    SAVE_SERVICE_CONTEXT(OSServiceId_GetScheduleTableStatus,ScheduleID,ScheduleStatus,NULL);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK|OS_CL_ISR2);

    DISABLE_ALL_OS_INTERRUPTS();
    
    ENABLE_ALL_OS_INTERRUPTS();   
    CLEAR_SERVICE_CONTEXT();
    return E_OK;
    
}


void OsSchTInitScheduleTables(void)
{
    
}
