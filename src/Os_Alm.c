/*
 * k_os (Konnex Operating-System based on the OSEK/VDX-Standard).
 *
 * (C) 2007-2013 by Christoph Schueler <github.com/Christoph2,
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

#include "Osek.h"
#include "Utl.h"
#include "Os_Alm.h"
#include "Os_Task.h"
#include "Os_Evt.h"

/*
**  Local variables.
*/

/*
**
**  TODO: 'Os_AlarmStateType' in anhängkeit der Anzahl der Alarme
**          dito: 'UTL_BIT_xxx' (Makros!!!)
**
*/

static Os_AlarmStateType OsAlm_ActiveAlarms = (Os_AlarmStateType)0x00;

#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_OS_START_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */

/*
**  Global functions.
*/
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsAlm_StartAlarm(uint8 num)
#else
void OsAlm_StartAlarm(uint8 num)
#endif /* KOS_MEMORY_MAPPING */
{
/*    OsAlm_ActiveAlarms=Utl_BitSet((uint16)OsAlm_ActiveAlarms,num); */
    UTL_BIT_SET8(OsAlm_ActiveAlarms, num);
}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsAlm_StopAlarm(uint8 num)
#else
void OsAlm_StopAlarm(uint8 num)
#endif /* KOS_MEMORY_MAPPING */
{
/*    OsAlm_ActiveAlarms=Utl_BitReset((uint16)OsAlm_ActiveAlarms,num); */
    UTL_BIT_RESET8(OsAlm_ActiveAlarms, num);
}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(boolean, OSEK_OS_CODE) OsAlm_IsRunning(uint8 num)
#else
boolean OsAlm_IsRunning(uint8 num)
#endif /* KOS_MEMORY_MAPPING */
{
/*    return Utl_BitGet((uint16)OsAlm_ActiveAlarms,num); */
    return (boolean)UTL_BIT_GET8(OsAlm_ActiveAlarms, num);
}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(uint16, OSEK_OS_CODE) OsAlm_GetActiveAlarms(void)
#else
/*Os_AlarmStateType*/ uint16  OsAlm_GetActiveAlarms(void)
#endif /* KOS_MEMORY_MAPPING */
{
    return (uint16)OsAlm_ActiveAlarms;
}


/*
**
** Gets the Configuration-Parameters of the attached Counter.
**
*/

#if KOS_MEMORY_MAPPING == STD_ON
FUNC(StatusType, OSEK_OS_CODE) GetAlarmBase(AlarmType AlarmID, AlarmBaseRefType Info)
#else
StatusType GetAlarmBase(AlarmType AlarmID, AlarmBaseRefType Info)
#endif /* KOS_MEMORY_MAPPING */
{
/*
**      Standard-Status:
**              – E_OK – no error.
**
**      Extended-Status:
**              – E_OS_ID – the alarm identifier is invalid.
**      Os_CallErrorHookAndReturn(E_OS_ID);
*/
    Os_SaveServiceContext(OSServiceId_GetAlarmBase, AlarmID, Info, NULL);
    ASSERT_VALID_ALARMID(AlarmID);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK | OS_CL_ISR2 | OS_CL_ERROR_HOOK | OS_CL_PRE_TASK_HOOK | OS_CL_POST_TASK_HOOK);

    Os_ClearServiceContext();
    return GetCounterInfo(OS_AlarmConf[AlarmID].AttachedCounter, (CtrInfoRefType)Info);
}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(StatusType, OSEK_OS_CODE) GetAlarm(AlarmType AlarmID, TickRefType Tick)
#else
StatusType GetAlarm(AlarmType AlarmID, TickRefType Tick)
#endif /* KOS_MEMORY_MAPPING */
{
/*
**      Standard:r
**              – E_OK – no error.
**              – E_OS_NOFUNC – the alarm is not in use.
**      Extended-Status:
**              – E_OS_ID – the alarm identifier is invalid.
*/
    Os_SaveServiceContext(OSServiceId_GetAlarm, AlarmID, Tick, NULL);
    ASSERT_VALID_ALARMID(AlarmID);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK | OS_CL_ISR2 | OS_CL_ERROR_HOOK | OS_CL_PRE_TASK_HOOK | OS_CL_POST_TASK_HOOK);
    WARN_IF_ALARM_IS_NOT_RUNNING(AlarmID);

    OsPort_DisableAllOsInterrupts();
    *Tick = OS_AlarmValue[AlarmID].ExpireCounter;
    OsPort_EnableAllOsInterrupts();

    Os_ClearServiceContext();
    return E_OK;
}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(StatusType, OSEK_OS_CODE) SetRelAlarm(AlarmType AlarmID, TickType increment, TickType cycle)
#else
StatusType SetRelAlarm(AlarmType AlarmID, TickType increment, TickType cycle)
#endif /* KOS_MEMORY_MAPPING */
{
/*
**      Standard-Status:
**              – E_OK – no error.
**              – E_OS_STATE – the alarm is already in use.
**      Extended-Status:
**              – E_OS_ID – the alarm identifier is invalid.
**              – E_OS_VALUE - an alarm initialization value is outside of
**                the admissible limits (lower than zero or greater than the
**                maximum allowed value of the counter), or alarm cycle
**                value is unequal to 0 and outside of the admissible
**                counter limits (less than the minimum cycle value of the
**                counter or greater than the maximum allowed value of
**                the counter).
*/
    Os_SaveServiceContext(OSServiceId_SetRelAlarm, AlarmID, increment, cycle);
    ASSERT_VALID_ALARMID(AlarmID);
    WARN_IF_ALARM_IS_RUNNING(AlarmID);
    ASSERT_VALID_ALARM_VALUES(AlarmID, increment, cycle);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK | OS_CL_ISR2);

    if (increment == (TickType)0) { /* todo: in 'ASSERT_VALID_ALARM_VALUES' einbauen!!! */
        Os_ClearServiceContext();
        return E_OS_VALUE;          /*  !REQ!AS!OS!OS304!  */
    }

    OsPort_DisableAllOsInterrupts();
    OS_AlarmValue[AlarmID].ExpireCounter   = increment;
    OS_AlarmValue[AlarmID].CycleCounter    = cycle;
    OsAlm_StartAlarm(AlarmID);  /*    OS_AlarmValue[AlarmID].State=ALM_RUNNING; */
    OsPort_EnableAllOsInterrupts();

    Os_ClearServiceContext();
    return E_OK;
}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(StatusType, OSEK_OS_CODE) SetAbsAlarm(AlarmType AlarmID, TickType start, TickType cycle)
#else
StatusType SetAbsAlarm(AlarmType AlarmID, TickType start, TickType cycle)
#endif /* KOS_MEMORY_MAPPING */
{
/*
**      Standard-Status:
**              – E_OK – no error.
**              – E_OS_STATE – the alarm is already in use.
**      Extended-Status:
**              – E_OS_ID – the alarm identifier is invalid.
**              – E_OS_VALUE - an alarm initialization value is outside of
**                the admissible limits (lower than zero or greater than the
**                maximum allowed value of the counter), or alarm cycle
**                value is unequal to 0 and outside of the admissible
**                counter limits (less than the minimum cycle value of the
**                counter or greater than the maximum allowed value of
**                the counter).
*/
    TickType CurrentCounterValue;

    Os_SaveServiceContext(OSServiceId_SetAbsAlarm, AlarmID, start, cycle);
    ASSERT_VALID_ALARMID(AlarmID);
    WARN_IF_ALARM_IS_RUNNING(AlarmID);
    ASSERT_VALID_ALARM_VALUES(AlarmID, start, cycle);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK | OS_CL_ISR2);

    OsPort_DisableAllOsInterrupts();

    CurrentCounterValue = Os_CounterValues[OS_AlarmConf[AlarmID].AttachedCounter];

    if (start == CurrentCounterValue) {
        OS_AlarmValue[AlarmID].ExpireCounter   = (TickType)0;
        OS_AlarmValue[AlarmID].CycleCounter    = cycle;
        OsAlm_StartAlarm(AlarmID);

        OsAlm_NotifyAlarm(AlarmID);
        OsExec_CondScheduleFromTaskLevel();
    } else {
        if (start > CurrentCounterValue) {
            OS_AlarmValue[AlarmID].ExpireCounter = start - CurrentCounterValue;
        } else {
            OS_AlarmValue[AlarmID].ExpireCounter =
                Os_CounterDefs[OS_AlarmConf[AlarmID].AttachedCounter].CounterParams.maxallowedvalue -
                (CurrentCounterValue + start + ((TickType)1)
                );
        }

        OS_AlarmValue[AlarmID].CycleCounter = cycle;
        OsAlm_StartAlarm(AlarmID);
    }

    OsPort_EnableAllOsInterrupts();

    Os_ClearServiceContext();
    return E_OK;
}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(StatusType, OSEK_OS_CODE) CancelAlarm(AlarmType AlarmID)
#else
StatusType CancelAlarm(AlarmType AlarmID)
#endif /* KOS_MEMORY_MAPPING */
{
/*
**      Standard-Status:
**              – E_OK – no error.
**              – E_OS_NOFUNC – the alarm is not in use.
**      Extended-Status:
**              – E_OS_ID – the alarm identifier is invalid.
**
*/
    Os_SaveServiceContext(OSServiceId_CancelAlarm, AlarmID, NULL, NULL);
    ASSERT_VALID_ALARMID(AlarmID);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK | OS_CL_ISR2);
    WARN_IF_ALARM_IS_NOT_RUNNING(AlarmID);

    OsPort_DisableAllOsInterrupts();
    OsAlm_StopAlarm(AlarmID);
    OsPort_EnableAllOsInterrupts();

    Os_ClearServiceContext();
    return E_OK;
}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsAlm_InitAlarms(void)
#else
void OsAlm_InitAlarms(void)
#endif /* KOS_MEMORY_MAPPING */
{
    uint8 i;

#if defined(OS_FEATURE_AUTOSTART_ALARMS)
    AlarmConfigurationType * alarm_def;
#endif /* OS_FEATURE_AUTOSTART_ALARMS */

    for (i = (uint8)0; i < OS_NUMBER_OF_ALARMS; ++i) {
#if defined(OS_FEATURE_AUTOSTART_ALARMS)
        alarm_def = (AlarmConfigurationType *)&OS_AlarmConf[i];

        if ((uint16)alarm_def->Autostart & GetActiveApplicationMode()) {
            OS_AlarmValue[i].ExpireCounter = alarm_def->AlarmTime;
            OS_AlarmValue[i].CycleCounter  = alarm_def->CycleTime;
            OsAlm_StartAlarm(i);
        } else {
#endif  /* OS_FEATURE_AUTOSTART_ALARMS */
        OS_AlarmValue[i].ExpireCounter = (TickType)0;
        OS_AlarmValue[i].CycleCounter  = (TickType)0;
        OsAlm_StopAlarm(i);
#if defined(OS_FEATURE_AUTOSTART_ALARMS)
    }

#endif  /* OS_FEATURE_AUTOSTART_ALARMS */
    }
}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsAlm_NotifyAlarm(AlarmType AlarmID)
#else
void OsAlm_NotifyAlarm(AlarmType AlarmID)
#endif /* KOS_MEMORY_MAPPING */
{
    AlarmConfigurationType const * const Alarm = (AlarmConfigurationType *)&OS_AlarmConf[AlarmID];

#if defined(OS_EXTENDED_STATUS) && defined(OS_FEATURE_CALLEVEL_CHECK)
    Os_CallevelType CallevelSaved;
#endif

    OsPort_DisableAllOsInterrupts();

    if (OS_AlarmValue[AlarmID].CycleCounter != (TickType)0) {
        /* cyclic Alarm. */
        OS_AlarmValue[AlarmID].ExpireCounter = OS_AlarmValue[AlarmID].CycleCounter;
    } else {
        /* one-shot Alarm. */
        OsAlm_StopAlarm(AlarmID);  /* OS_AlarmValue[AlarmID].State=ALM_STOPPED; */
    }

/*    printf("Alarm %u.\n", AlarmID); */

    OsPort_EnableAllOsInterrupts();

    switch (Alarm->ActionType) {
        case ALM_SETEVENT:
            (void)OsEvt_SetEvent(Alarm->Action.Event->TaskID, Alarm->Action.Event->Mask);
            break;
        case ALM_ACTIVATETASK:
            (void)OsTask_Activate((TaskType)Alarm->Action.TaskID);
            break;
        case ALM_CALLBACK:
            OsPort_DisableAllOsInterrupts();
            #if defined(OS_EXTENDED_STATUS) && defined(OS_FEATURE_CALLEVEL_CHECK)
            CallevelSaved = Os_GetCallLevel();
            #endif
            Os_SetCallLevel(OS_CL_ALARM_CALLBACK);
            (Alarm->Action.AlarmCallback)();
            #if defined(OS_EXTENDED_STATUS) && defined(OS_FEATURE_CALLEVEL_CHECK)
            Os_SetCallLevel(CallevelSaved);
            #endif
            OsPort_EnableAllOsInterrupts();
            break;
#if 0
        case ALM_COUNTER:
            (void)IncrementCounter(Alarm->Action.CounterID);
            break;
#endif
        default:
            ASSERT(FALSE);
    }
}


#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_OS_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */

