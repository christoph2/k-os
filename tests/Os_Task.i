
%module Os_Task

%{
#include "Osek.h"
%}

%inline {
StatusType  ActivateTask(TaskType TaskID);
StatusType  TerminateTask(void);
StatusType  ChainTask(TaskType TaskID);
StatusType  Schedule(void);
StatusType  GetTaskID(TaskRefType TaskID);
StatusType  GetTaskState(TaskType TaskID, TaskStateRefType State);

void        OsTask_InitTasks(void);
void        OsTask_Ready(TaskType TaskID);
void        OsTask_Suspend(TaskType TaskID);
void        OsTask_Wait(TaskType TaskID);
StatusType  OsTask_Activate(TaskType TaskID);
}

