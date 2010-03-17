/*
   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).

   (C) 2007-2010 by Christoph Schueler <chris@konnex-tools.de,
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
#include "Osek.h"


static void OsCtr_UpdateAttachedAlarms(CounterType CounterID);
static void OsCtr_UpdateAttachedScheduleTables(CounterType CounterID);


StatusType InitCounter(CounterType CounterID,TickType InitialValue)
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

    SAVE_SERVICE_CONTEXT(OSServiceId_InitCounter,CounterID,InitialValue,NULL);
    ASSERT_VALID_COUNTERID(CounterID);
    ASSERT_VALID_COUNTER_VALUE(CounterID,InitialValue);
#if 0
    ASSERT_VALID_CALLEVEL(CL_TASK|CL_ISR2);
#endif

    DISABLE_ALL_OS_INTERRUPTS();
    Os_CounterValues[CounterID]=InitialValue;
    ENABLE_ALL_OS_INTERRUPTS();

    CLEAR_SERVICE_CONTEXT();
    return E_OK;
}


StatusType IncrementCounter(CounterType CounterID)
{
/*
**	Standard-Status:
**		– E_OK – no error.
**	Extended-Status:
**		– E_OS_ID – the counter identifier is invalid or
**		  belongs to hardware counter.
*/

/*  CtrInfoRefType counter; */

/*	"The CounterID was not valid or Counter is implemented in Hardware  */
/*	and cannot be incremented in Software". */
    SAVE_SERVICE_CONTEXT(OSServiceId_IncrementCounter,CounterID,NULL,NULL);
    ASSERT_VALID_COUNTERID(CounterID);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK|OS_CL_ISR2);

    SuspendAllInterrupts();   /*    DISABLE_ALL_OS_INTERRUPTS();         */

    Os_CounterValues[CounterID]=(Os_CounterValues[CounterID]+(TickType)1) % __OS_CounterDefs[CounterID].CounterParams.maxallowedvalue;

    ResumeAllInterrupts();    /* ENABLE_ALL_OS_INTERRUPTS(); */

#if defined(OS_USE_ALARMS)
    OsCtr_UpdateAttachedAlarms(CounterID);
#endif

#if defined(OS_USE_SCHEDULE_TABLES)
    OsCtr_UpdateAttachedScheduleTables(CounterID);
#endif

    OS_COND_SCHEDULE_FROM_TASK_LEVEL();

    CLEAR_SERVICE_CONTEXT();
    return E_OK;
}


StatusType GetCounterInfo(CounterType CounterID,CtrInfoRefType Info)
{
/*	Standard-Status:
**		– E_OK – no error.
**	Extended-Status:
**		– E_OS_ID – the counter identifier is invalid.
*/
    SAVE_SERVICE_CONTEXT(OSServiceId_GetCounterInfo,CounterID,/*Info*/NULL,NULL);
    ASSERT_VALID_COUNTERID(CounterID);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK|OS_CL_ISR2);

    *Info=__OS_CounterDefs[CounterID].CounterParams;

    CLEAR_SERVICE_CONTEXT();
    return E_OK;
}


StatusType GetCounterValue(CounterType CounterID,TickRefType Value)
{
/*
**	Standard-Status:
**		– E_OK – no error.
**	Extended-Status:
**		– E_OS_ID – the counter identifier is invalid.
*/
    SAVE_SERVICE_CONTEXT(OSServiceId_GetCounterValue,CounterID,/*Value*/NULL,NULL);
    ASSERT_VALID_COUNTERID(CounterID);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK|OS_CL_ISR2);

    DISABLE_ALL_OS_INTERRUPTS();
    *Value=Os_CounterValues[CounterID];
    ENABLE_ALL_OS_INTERRUPTS();

    CLEAR_SERVICE_CONTEXT();
    return E_OK;
}


StatusType GetElapsedCounterValue(CounterType CounterID,TickRefType Value,TickRefType ElapsedValue)
{
    SAVE_SERVICE_CONTEXT(OSServiceId_GetElapsedCounterValue,CounterID,Value,/*ElapsedValue*/NULL);
    ASSERT_VALID_COUNTERID(CounterID);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK|OS_CL_ISR2);
    ASSERT_IS_NOT_NULL(Value);

    DISABLE_ALL_OS_INTERRUPTS();
    if (Os_CounterValues[CounterID]<(*Value)) {
        *ElapsedValue=__OS_CounterDefs[CounterID].CounterParams.maxallowedvalue-
            (*Value)-Os_CounterValues[CounterID]+(TickType)1;
    } else {
        *ElapsedValue=Os_CounterValues[CounterID]-(*Value);
    }
    ENABLE_ALL_OS_INTERRUPTS();

    CLEAR_SERVICE_CONTEXT();
    return E_OK;
}


void OsCtr_InitCounters(void)
{
    CounterType i;

    for (i=(CounterType)0;i<OS_NUMBER_OF_COUNTERS;++i) {
        (void)InitCounter(i,(TickType)0);
    }
}


static void OsCtr_UpdateAttachedAlarms(CounterType CounterID)
{
    uint8_least idx=(uint8_least)0x00;
    /*Os_AlarmStateType*/uint16 Alarms=OsAlm_GetActiveAlarms();
    AlarmType AlarmID;

    while (Alarms!=(uint8)0x00) {
        if ((Alarms & BIT0)==BIT0) {
            AlarmID=__OS_CounterDefs[CounterID].AlarmsForCounter[idx];
            if (--OS_AlarmValue[AlarmID].ExpireCounter==(uint16)0) {
                OsAlm_NotifyAlarm(AlarmID);
            }
        }
        Alarms>>=1;
        idx++;
    }
}


static void OsCtr_UpdateAttachedScheduleTables(CounterType CounterID)
{
}
