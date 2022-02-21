/*
   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).

   (C) 2007-2018 by Christoph Schueler <github.com/Christoph2,
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
#
   s. FLOSS-EXCEPTION.txt
 */


#include "Osek.h"
#include "Utl.h"
#include "Os_Alm.h"

/*
**  Local function prototypes.
*/
#if KOS_MEMORY_MAPPING == STD_ON
STATIC  FUNC(void, OSEK_OS_CODE) OsCtr_UpdateAttachedAlarms(CounterType CounterID);
STATIC  FUNC(void, OSEK_OS_CODE) OsCtr_UpdateAttachedScheduleTables(CounterType CounterID);
#else
static void OsCtr_UpdateAttachedAlarms(CounterType CounterID);
static void OsCtr_UpdateAttachedScheduleTables(CounterType CounterID);
#endif /* KOS_MEMORY_MAPPING */

#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_OS_START_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */

/*
**  Local Function-like Macros.
*/
/* Yields to better code, at least on the CPU12. */
#define OS_INCREMENT_COUNTER_VALUE(CounterID)                                                         \
    _BEGIN_BLOCK                                                                                      \
    Os_CounterValues[(CounterID)]++;                                                                  \
    if (Os_CounterValues[(CounterID)] >= Os_CounterDefs[(CounterID)].CounterParams.maxallowedvalue) { \
        Os_CounterValues[(CounterID)] = (TickType)0;                                                  \
    }                                                                                                 \
    _END_BLOCK


/*
**  Global functions.
*/
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(StatusType, OSEK_OS_CODE) InitCounter(CounterType CounterID, TickType InitialValue)
#else
StatusType InitCounter(CounterType CounterID, TickType InitialValue)
#endif /* KOS_MEMORY_MAPPING */
{
/*
    Standard-Status:
        – E_OK – no error.
    Extended-Status:
        – E_OS_ID – the counter identifier is invalid.
        – E_OS_VALUE – the counter initialization value exceeds
          the maximum admissible value.
        – E_OS_CALLEVEL – a call at interrupt level (not allowed).
 */
/* todo: Callout-Funktion, um den Initialisierungs-Parameter abzufragen ... dann wird 'InitialValue' nicht benötigt!!! */
    Os_SaveServiceContext(OSServiceId_InitCounter, CounterID, InitialValue, NULL);
    ASSERT_VALID_COUNTERID(CounterID);
    ASSERT_VALID_COUNTER_VALUE(CounterID, InitialValue);
#if 0
    ASSERT_VALID_CALLEVEL(CL_TASK | CL_ISR2);
#endif

    OsPort_DisableAllOsInterrupts();
    Os_CounterValues[CounterID] = InitialValue;
    OsPort_EnableAllOsInterrupts();
    Os_ClearServiceContext();
    return E_OK;
}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(StatusType, OSEK_OS_CODE) IncrementCounter(CounterType CounterID)
#else
StatusType IncrementCounter(CounterType CounterID)
#endif /* KOS_MEMORY_MAPPING */
{
/*
**      Standard-Status:
**              – E_OK – no error.
**      Extended-Status:
**              – E_OS_ID – the counter identifier is invalid or
**                belongs to hardware counter.
*/

/*  CtrInfoRefType counter; */

/*      "The CounterID was not valid or Counter is implemented in Hardware  */
/*      and cannot be incremented in Software". */
    Os_SaveServiceContext(OSServiceId_IncrementCounter, CounterID, NULL, NULL);
    ASSERT_VALID_COUNTERID(CounterID);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK | OS_CL_ISR2);

    OsPort_DisableAllOsInterrupts();
    OS_INCREMENT_COUNTER_VALUE(CounterID);
    OsPort_EnableAllOsInterrupts();

#if (OS_FEATURE_ALARMS == STD_ON)
    OsCtr_UpdateAttachedAlarms(CounterID);
#endif

#if (OS_FEATURE_SCHEDULE_TABLES == STD_ON)
    OsCtr_UpdateAttachedScheduleTables(CounterID);
#endif

    OsExec_CondScheduleFromTaskLevel();

    Os_ClearServiceContext();
    return E_OK;
}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(StatusType, OSEK_OS_CODE) GetCounterInfo(CounterType CounterID, CtrInfoRefType Info)
#else
StatusType GetCounterInfo(CounterType CounterID, CtrInfoRefType Info)
#endif /* KOS_MEMORY_MAPPING */
{
/*      Standard-Status:
**              – E_OK – no error.
**      Extended-Status:
**              – E_OS_ID – the counter identifier is invalid.
*/

/* TODO: NULL pointer test!? */
    Os_SaveServiceContext(OSServiceId_GetCounterInfo, CounterID, /*Info*/ NULL, NULL);
    ASSERT_VALID_COUNTERID(CounterID);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK | OS_CL_ISR2);

    *Info = Os_CounterDefs[CounterID].CounterParams;

    Os_ClearServiceContext();
    return E_OK;
}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(StatusType, OSEK_OS_CODE) GetCounterValue(CounterType CounterID, TickRefType Value)
#else
StatusType GetCounterValue(CounterType CounterID, TickRefType Value)
#endif /* KOS_MEMORY_MAPPING */
{
/*
**      Standard-Status:
**              – E_OK – no error.
**      Extended-Status:
**              – E_OS_ID – the counter identifier is invalid.
*/
    Os_SaveServiceContext(OSServiceId_GetCounterValue, CounterID, /*Value*/ NULL, NULL);
    ASSERT_VALID_COUNTERID(CounterID);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK | OS_CL_ISR2);

    OsPort_DisableAllOsInterrupts();
    *Value = Os_CounterValues[CounterID];
    OsPort_EnableAllOsInterrupts();

    Os_ClearServiceContext();
    return E_OK;
}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(StatusType, OSEK_OS_CODE) GetElapsedCounterValue(CounterType CounterID, TickRefType Value, TickRefType ElapsedValue)
#else
StatusType GetElapsedCounterValue(CounterType CounterID, TickRefType Value, TickRefType ElapsedValue)
#endif /* KOS_MEMORY_MAPPING */
{
    Os_SaveServiceContext(OSServiceId_GetElapsedCounterValue, CounterID, Value, /*ElapsedValue*/ NULL);
    ASSERT_VALID_COUNTERID(CounterID);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK | OS_CL_ISR2);
    ASSERT_IS_NOT_NULL(Value);

    OsPort_DisableAllOsInterrupts();

    if (Os_CounterValues[CounterID] < (*Value)) {
        *ElapsedValue = Os_CounterDefs[CounterID].CounterParams.maxallowedvalue -
                        ((*Value) - (Os_CounterValues[CounterID] + (TickType)1));
    } else {
        *ElapsedValue = Os_CounterValues[CounterID] - (*Value);
    }

    OsPort_EnableAllOsInterrupts();

    Os_ClearServiceContext();
    return E_OK;
}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC( void, OSEK_OS_CODE) OsCtr_InitCounters(void)
#else
void OsCtr_InitCounters(void)
#endif /* KOS_MEMORY_MAPPING */
{
    CounterType i;

    for (i = (CounterType)0; i < OS_NUMBER_OF_COUNTERS; ++i) {
        (void)InitCounter(i, (TickType)0);
    }
}


/*
**  Local functions.
*/
#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, OSEK_OS_CODE) OsCtr_UpdateAttachedAlarms(CounterType CounterID)
#else
static void OsCtr_UpdateAttachedAlarms(CounterType CounterID)
#endif /* KOS_MEMORY_MAPPING */
{
    uint8       idx    = (uint8)0x00;
    uint16      Alarms = OsAlm_GetActiveAlarms();
    AlarmType   AlarmID;

    while (Alarms != (uint8)0x00) {
        if ((Alarms & BIT0) == BIT0) {
            AlarmID = Os_CounterDefs[CounterID].AlarmsForCounter[idx];

            if (--OS_AlarmValue[AlarmID].ExpireCounter == (uint16)0) {
                OsAlm_NotifyAlarm(AlarmID);
            }
        }

        Alarms >>= 1;
        idx++;
    }
}


#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC( void, OSEK_OS_CODE) OsCtr_UpdateAttachedScheduleTables(CounterType CounterID)
#else
static void OsCtr_UpdateAttachedScheduleTables(CounterType CounterID)
#endif /* KOS_MEMORY_MAPPING */
{
    UNREFERENCED_PARAMETER(CounterID);
}


#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_OS_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */
