#include "Os_Mlq.h"

#include <stdio.h>
#include <stdlib.h>

static TaskType ReadyTasks[OS_NUMBER_OF_TASKS];
static unsigned int ReadyCount;

static PriorityType task_priority(TaskType task)
{
#if (OS_FEATURE_RESOURCES == STD_ON) || (OS_FEATURE_INTERNAL_RESOURCES == STD_ON)
	return OS_TCB[task].CurrentPriority;
#else
	return OS_TaskConf[task].Priority;
#endif
}

static void insert_task(TaskType task, boolean front)
{
	unsigned int i;

	if (task == INVALID_TASK || task >= OS_NUMBER_OF_TASKS || ReadyCount >= OS_NUMBER_OF_TASKS - 1) {
		fputs("POSIX port: invalid or full ready queue\n", stderr);
		abort();
	}
	for (i = 0; i < ReadyCount; ++i) {
		if (ReadyTasks[i] == task) {
			fputs("POSIX port: task already in ready queue\n", stderr);
			abort();
		}
	}
	if (front) {
		for (i = ReadyCount; i > 0; --i) ReadyTasks[i] = ReadyTasks[i - 1];
		ReadyTasks[0] = task;
	} else {
		ReadyTasks[ReadyCount] = task;
	}
	++ReadyCount;
}

void OsMLQ_Init(void)
{
	ReadyCount = 0;
}

TaskType OsMLQ_GetHighestPrio(void)
{
	unsigned int i;
	TaskType highest = INVALID_TASK;
	for (i = 0; i < ReadyCount; ++i) {
		TaskType task = ReadyTasks[i];
		if (highest == INVALID_TASK || task_priority(task) > task_priority(highest)) highest = task;
	}
	return highest;
}

boolean OsMLQ_TasksAreReady(void)
{
	return (boolean)(ReadyCount != 0);
}

void OsMLQ_AddTaskFirst(TaskType task, PriorityType priority)
{
	(void)priority;
	insert_task(task, TRUE);
}

void OsMLQ_AddTaskLast(TaskType task, PriorityType priority)
{
	(void)priority;
	insert_task(task, FALSE);
}

void OsMLQ_RemoveTask(TaskType task)
{
	unsigned int i;
	for (i = 0; i < ReadyCount; ++i) {
		if (ReadyTasks[i] == task) {
			for (++i; i < ReadyCount; ++i) ReadyTasks[i - 1] = ReadyTasks[i];
			--ReadyCount;
			return;
		}
	}
	/* Task not in ready queue — may happen on POSIX when the scheduler
	   already removed it before TerminateTask/ChainTask calls Suspend. */
}

#if (OS_FEATURE_RESOURCES == STD_ON) || (OS_FEATURE_INTERNAL_RESOURCES == STD_ON)
void OsMLQ_ChangePrio(TaskType task, PriorityType old_priority, PriorityType new_priority)
{
	(void)old_priority;
	OS_TCB[task].CurrentPriority = new_priority;
}
#endif
