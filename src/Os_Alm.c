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
#include "Osek.h"

static Os_AlarmStateType OsAlm_ActiveAlarms;    /* todo: Name!!! */

void OsAlm_StartAlarm(uint8 num)
{
    OsAlm_ActiveAlarms=Utl_BitSet((uint16)OsAlm_ActiveAlarms,num);
}

void OsAlm_StopAlarm(uint8 num)
{
    OsAlm_ActiveAlarms=Utl_BitReset((uint16)OsAlm_ActiveAlarms,num);
}

boolean OsAlm_IsRunning(uint8 num)
{
    return Utl_BitGet((uint16)OsAlm_ActiveAlarms,num);
}

/*Os_AlarmStateType*/uint16  OsAlm_GetActiveAlarms(void)
{
    return OsAlm_ActiveAlarms;
}

/*
**
** Gets the Configuration-Parameters of the attached Counter.
**
*/

StatusType GetAlarmBase(AlarmType AlarmID,AlarmBaseRefType Info)
{
/*
**      Standard-Status:
**              – E_OK – no error.
**
**      Extended-Status:
**              – E_OS_ID – the alarm identifier is invalid.
**      OSCallErrorHookAndReturn(E_OS_ID);
*/
    SAVE_SERVICE_CONTEXT(OSServiceId_GetAlarmBase,AlarmID,Info,NULL);
    ASSERT_VALID_ALARMID(AlarmID);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK|OS_CL_ISR2|OS_CL_ERROR_HOOK|OS_CL_PRE_TASK_HOOK|OS_CL_POST_TASK_HOOK);

    CLEAR_SERVICE_CONTEXT();
    return GetCounterInfo(OS_AlarmConf[AlarmID].AttachedCounter,Info);
}


StatusType GetAlarm(AlarmType AlarmID,TickRefType Tick)
{
/*
**      Standard:
**              – E_OK – no error.
**              – E_OS_NOFUNC – the alarm is not in use.
**      Extended-Status:
**              – E_OS_ID – the alarm identifier is invalid.
*/
    SAVE_SERVICE_CONTEXT(OSServiceId_GetAlarm,AlarmID,Tick,NULL);
    ASSERT_VALID_ALARMID(AlarmID);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK|OS_CL_ISR2|OS_CL_ERROR_HOOK|OS_CL_PRE_TASK_HOOK|OS_CL_POST_TASK_HOOK);
    WARN_IF_ALARM_IS_NOT_RUNNING(AlarmID);

    DISABLE_ALL_OS_INTERRUPTS();
    *Tick=OS_AlarmValue[AlarmID].ExpireCounter;
    ENABLE_ALL_OS_INTERRUPTS();

    CLEAR_SERVICE_CONTEXT();
    return E_OK;
}


StatusType SetRelAlarm(AlarmType AlarmID,TickType increment,TickType cycle)
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
    SAVE_SERVICE_CONTEXT(OSServiceId_SetRelAlarm,AlarmID,increment,cycle);
    ASSERT_VALID_ALARMID(AlarmID);
    WARN_IF_ALARM_IS_RUNNING(AlarmID);
    ASSERT_VALID_ALARM_VALUES(AlarmID,increment,cycle);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK|OS_CL_ISR2);

    if (increment==(TickType)0) {   /* todo: in 'ASSERT_VALID_ALARM_VALUES' einbauen!!! */
        CLEAR_SERVICE_CONTEXT();
        return E_OS_VALUE;  /*  !REQ!AS!OS!OS304!  */
    }

    DISABLE_ALL_OS_INTERRUPTS();
    OS_AlarmValue[AlarmID].ExpireCounter=increment;
    OS_AlarmValue[AlarmID].CycleCounter=cycle;
    OsAlm_StartAlarm(AlarmID);  //    OS_AlarmValue[AlarmID].State=ALM_RUNNING;
    ENABLE_ALL_OS_INTERRUPTS();

    CLEAR_SERVICE_CONTEXT();
    return E_OK;
}


StatusType SetAbsAlarm(AlarmType AlarmID,TickType start,TickType cycle)
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

    SAVE_SERVICE_CONTEXT(OSServiceId_SetAbsAlarm,AlarmID,start,cycle);
    ASSERT_VALID_ALARMID(AlarmID);
    WARN_IF_ALARM_IS_RUNNING(AlarmID);
    ASSERT_VALID_ALARM_VALUES(AlarmID,start,cycle);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK|OS_CL_ISR2);

    DISABLE_ALL_OS_INTERRUPTS();

    CurrentCounterValue=Os_CounterValues[OS_AlarmConf[AlarmID].AttachedCounter];
    if (start==CurrentCounterValue) {
        OS_AlarmValue[AlarmID].ExpireCounter=(TickType)0;
        OS_AlarmValue[AlarmID].CycleCounter=cycle;
        OsAlm_StartAlarm(AlarmID);

        OsAlm_NotifyAlarm(AlarmID);
        OS_COND_SCHEDULE_FROM_TASK_LEVEL();
    } else {
        if (start>CurrentCounterValue) {
           OS_AlarmValue[AlarmID].ExpireCounter=start-CurrentCounterValue;
        } else {
            OS_AlarmValue[AlarmID].ExpireCounter=
                Os_CounterDefs[OS_AlarmConf[AlarmID].AttachedCounter].CounterParams.maxallowedvalue-
                (CurrentCounterValue+start+((TickType)1));
        }
        OS_AlarmValue[AlarmID].CycleCounter=cycle;
        OsAlm_StartAlarm(AlarmID);
    }

    ENABLE_ALL_OS_INTERRUPTS();

    CLEAR_SERVICE_CONTEXT();
    return E_OK;
}


StatusType CancelAlarm(AlarmType AlarmID)
{
/*
**      Standard-Status:
**              – E_OK – no error.
**              – E_OS_NOFUNC – the alarm is not in use.
**      Extended-Status:
**              – E_OS_ID – the alarm identifier is invalid.
**
*/
    SAVE_SERVICE_CONTEXT(OSServiceId_CancelAlarm,AlarmID,NULL,NULL);
    ASSERT_VALID_ALARMID(AlarmID);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK|OS_CL_ISR2);
    WARN_IF_ALARM_IS_NOT_RUNNING(AlarmID);

    DISABLE_ALL_OS_INTERRUPTS();
    OsAlm_StopAlarm(AlarmID);
    ENABLE_ALL_OS_INTERRUPTS();

    CLEAR_SERVICE_CONTEXT();
    return E_OK;
}


void OsAlm_InitAlarms(void)
{
    uint8_least i;
#if defined(OS_FEATURE_AUTOSTART_ALARMS)
    AlarmConfigurationType *alarm_def;
#endif /* OS_FEATURE_AUTOSTART_ALARMS */

    for (i=(uint8_least)0;i<OS_NUMBER_OF_ALARMS;++i) {
#if defined(OS_FEATURE_AUTOSTART_ALARMS)
        alarm_def=(AlarmConfigurationType*)&OS_AlarmConf[i];
        if ((uint16)alarm_def->Autostart & GetActiveApplicationMode()) {
            OS_AlarmValue[i].ExpireCounter=alarm_def->AlarmTime;
            OS_AlarmValue[i].CycleCounter=alarm_def->CycleTime;
            OsAlm_StartAlarm(i);
        } else {
#endif /* OS_FEATURE_AUTOSTART_ALARMS */
            OS_AlarmValue[i].ExpireCounter=(TickType)0;
            OS_AlarmValue[i].CycleCounter=(TickType)0;
            OsAlm_StopAlarm(i);
#if defined(OS_FEATURE_AUTOSTART_ALARMS)
        }
#endif /* OS_FEATURE_AUTOSTART_ALARMS */
    }
}


void OsAlm_NotifyAlarm(AlarmType AlarmID)
{
    AlarmConfigurationType *Alarm;
#if defined(OS_EXTENDED_STATUS) && defined(OS_USE_CALLEVEL_CHECK)
    OsCallevelType CallevelSaved;
#endif

    Alarm=(AlarmConfigurationType *)&OS_AlarmConf[AlarmID];

    DISABLE_ALL_OS_INTERRUPTS();
    if (OS_AlarmValue[AlarmID].CycleCounter!=(TickType)0) {
        /* cyclic Alarm. */
        OS_AlarmValue[AlarmID].ExpireCounter=OS_AlarmValue[AlarmID].CycleCounter;
    } else {
        /* one-shot Alarm. */
        OsAlm_StopAlarm(AlarmID);  // OS_AlarmValue[AlarmID].State=ALM_STOPPED;
    }
    ENABLE_ALL_OS_INTERRUPTS();

    switch (Alarm->ActionType) {
        case ALM_SETEVENT:
            (void)OsEvtSetEvent(Alarm->Action.Event->TaskID,Alarm->Action.Event->Mask);
            break;
        case ALM_ACTIVATETASK:
            (void)OsTask_Activate(Alarm->Action.TaskID);
            break;
        case ALM_CALLBACK:
            DISABLE_ALL_OS_INTERRUPTS();
            #if defined(OS_EXTENDED_STATUS) && defined(OS_USE_CALLEVEL_CHECK)
            CallevelSaved=OS_GET_CALLEVEL();
            #endif
            OS_SET_CALLEVEL(OS_CL_ALARM_CALLBACK);
            (Alarm->Action.AlarmCallback)();
            #if defined(OS_EXTENDED_STATUS) && defined(OS_USE_CALLEVEL_CHECK)
            OS_SET_CALLEVEL(CallevelSaved);
            #endif
            ENABLE_ALL_OS_INTERRUPTS();
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
