
%module Os_Alm

%include Types.i

%{
#include "Osek.h"
%}

%inline {
StatusType  GetAlarmBase(AlarmType AlarmID, AlarmBaseRefType Info);
StatusType  GetAlarm(AlarmType AlarmID, TickRefType Tick);
StatusType  SetRelAlarm(AlarmType AlarmID, TickType increment, TickType cycle);
StatusType  SetAbsAlarm(AlarmType AlarmID, TickType start, TickType cycle);
StatusType  CancelAlarm(AlarmType AlarmID);
}

