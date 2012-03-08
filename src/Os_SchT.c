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

#define INVALID_SCHEDULE_TABLE ((uint8)0xff)

typedef struct tagOsSchTType {
    uint8                   InternalState;
    ScheduleTableStatusType ExternalState;
    TickType                Delay;

    ScheduleTableType   CurrentTable;
    ScheduleTableType   NextTable;
    uint8               CurrentPoint; /* CurrentEntry */
} OsSchTType;

typedef enum tagOsSchTSyncStrategyType {
    SCHT_SYNC_NONE,
    SCHT_SYNC_IMPLICIT,
    SCHT_SYNC_EXPLICIT
} OsSchTSyncStrategyType;

typedef struct tagOsScheduleTableConfigurationType {
/* 1..* ExpiryPoints */

    TickType                TableDuration;
    boolean                 Repeating;
    CounterType             CounterID;
    OsSchTSyncStrategyType  SyncStrategy;
    TickType                ExplicitPrecision;
    TickType                MaxRetard;
    TickType                MaxAdvance;
} OsScheduleTableConfigurationType;

typedef struct tagOsExpiryPointType {
    TickType    Offset;
    TickType    MaxRetard;  /* to be substracted */
    TickType    MaxAdvance; /* to be added */

    /*
    **  todo: TaskActivations + Events.
    **
    */
} OsExpiryPointType;

#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_OS_START_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(StatusType, OSEK_OS_CODE) StartScheduleTableRel(ScheduleTableType ScheduleTableID, TickType Offset)
#else
StatusType StartScheduleTableRel(ScheduleTableType ScheduleTableID, TickType Offset)
#endif /* KOS_MEMORY_MAPPING */
{
    SAVE_SERVICE_CONTEXT(OSServiceId_StartScheduleTableRel, ScheduleTableID, Offset, NULL);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK | OS_CL_ISR2);

    DISABLE_ALL_OS_INTERRUPTS();

    ENABLE_ALL_OS_INTERRUPTS();
    CLEAR_SERVICE_CONTEXT();
    return E_OK;

}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(StatusType, OSEK_OS_CODE) StartScheduleTableAbs(ScheduleTableType ScheduleTableID, TickType Tickvalue)
#else
StatusType StartScheduleTableAbs(ScheduleTableType ScheduleTableID, TickType Tickvalue)
#endif /* KOS_MEMORY_MAPPING */
{
    SAVE_SERVICE_CONTEXT(OSServiceId_StartScheduleTableAbs, ScheduleTableID, Tickvalue, NULL);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK | OS_CL_ISR2);

    DISABLE_ALL_OS_INTERRUPTS();

    ENABLE_ALL_OS_INTERRUPTS();
    CLEAR_SERVICE_CONTEXT();
    return E_OK;

}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(StatusType, OSEK_OS_CODE) StopScheduleTable(ScheduleTableType ScheduleTableID)
#else
StatusType StopScheduleTable(ScheduleTableType ScheduleTableID)
#endif /* KOS_MEMORY_MAPPING */
{
    SAVE_SERVICE_CONTEXT(OSServiceId_StopScheduleTable, ScheduleTableID, NULL, NULL);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK | OS_CL_ISR2);

    DISABLE_ALL_OS_INTERRUPTS();

    ENABLE_ALL_OS_INTERRUPTS();
    CLEAR_SERVICE_CONTEXT();
    return E_OK;

}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(StatusType, OSEK_OS_CODE) NextScheduleTable(ScheduleTableType ScheduleTableID_From,
    ScheduleTableType
    ScheduleTableID_To
)
#else
StatusType NextScheduleTable(ScheduleTableType ScheduleTableID_From, ScheduleTableType ScheduleTableID_To)
#endif /* KOS_MEMORY_MAPPING */
{
    SAVE_SERVICE_CONTEXT(OSServiceId_NextScheduleTable, ScheduleTableID_From, ScheduleTableID_To, NULL);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK | OS_CL_ISR2);

    DISABLE_ALL_OS_INTERRUPTS();

    ENABLE_ALL_OS_INTERRUPTS();
    CLEAR_SERVICE_CONTEXT();
    return E_OK;

}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(StatusType, OSEK_OS_CODE) StartScheduleTableSynchron(ScheduleTableType ScheduleTableID)
#else
StatusType StartScheduleTableSynchron(ScheduleTableType ScheduleTableID)
#endif /* KOS_MEMORY_MAPPING */
{
    SAVE_SERVICE_CONTEXT(OSServiceId_StartScheduleTableSynchron, ScheduleTableID, NULL, NULL);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK | OS_CL_ISR2);

    DISABLE_ALL_OS_INTERRUPTS();

    ENABLE_ALL_OS_INTERRUPTS();
    CLEAR_SERVICE_CONTEXT();
    return E_OK;
}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(StatusType, OSEK_OS_CODE) SyncScheduleTable(ScheduleTableType ScheduleTableID, TickType Value)
#else
StatusType SyncScheduleTable(ScheduleTableType ScheduleTableID, TickType Value)
#endif /* KOS_MEMORY_MAPPING */
{
    SAVE_SERVICE_CONTEXT(OSServiceId_SyncScheduleTable, ScheduleTableID, Value, NULL);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK | OS_CL_ISR2);

    DISABLE_ALL_OS_INTERRUPTS();

    ENABLE_ALL_OS_INTERRUPTS();
    CLEAR_SERVICE_CONTEXT();
    return E_OK;

}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(StatusType, OSEK_OS_CODE) SetScheduleTableAsync(ScheduleTableType ScheduleID)
#else
StatusType SetScheduleTableAsync(ScheduleTableType ScheduleID)
#endif /* KOS_MEMORY_MAPPING */
{
    SAVE_SERVICE_CONTEXT(OSServiceId_SetScheduleTableAsync, ScheduleID, NULL, NULL);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK | OS_CL_ISR2);

    DISABLE_ALL_OS_INTERRUPTS();

    ENABLE_ALL_OS_INTERRUPTS();
    CLEAR_SERVICE_CONTEXT();
    return E_OK;

}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(StatusType, OSEK_OS_CODE) GetScheduleTableStatus(ScheduleTableType ScheduleID,
    ScheduleTableStatusRefType /*@out@*/ ScheduleStatus
)
#else
StatusType GetScheduleTableStatus(ScheduleTableType ScheduleID, ScheduleTableStatusRefType /*@out@*/ ScheduleStatus)
#endif /* KOS_MEMORY_MAPPING */
{
    SAVE_SERVICE_CONTEXT(OSServiceId_GetScheduleTableStatus, ScheduleID, ScheduleStatus, NULL);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK | OS_CL_ISR2);

    DISABLE_ALL_OS_INTERRUPTS();

    ENABLE_ALL_OS_INTERRUPTS();
    CLEAR_SERVICE_CONTEXT();
    return E_OK;

}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsSchTInitScheduleTables(void)
#else
void OsSchTInitScheduleTables(void)
#endif /* KOS_MEMORY_MAPPING */
{

}


#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_OS_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */
