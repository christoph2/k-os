/*
** k_os (Konnex Operating-System based on the OSEK/VDX-Standard).
**
** (C) 2007-2025 by Christoph Schueler <github.com/Christoph2,
**                                      cpu12.gems@googlemail.com>
**
** All Rights Reserved
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; version 2 of the License.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
**
** s. FLOSS-EXCEPTION.txt
**/

#include "Os_Cfg.h"
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(KOS_POSIX)
#include "Os_Mlq.h"
#include "Os_Task.h"
#include "Os_Exec.h"
#include "Os_Ctr.h"
#include "Os_Alm.h"
#endif

/*
**  Global variables for pthread-based port.
*/
InterruptStateType OsPort_InterruptState = 0;
_Thread_local uint8_t OsPort_InterruptsDisabled = 0;
pthread_mutex_t OsPort_GlobalMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t OsPort_ScheduleCondition = PTHREAD_COND_INITIALIZER;
volatile sig_atomic_t OsPort_InterruptFlag = 0;
struct timespec OsPort_StartTime;

static timer_t OsPort_SystemTimer;
static struct sigaction OsPort_OldSigAction;
static sigset_t OsPort_TimerSignalSet;
static int OsPort_TimerCreated = 0;
static int OsPort_SignalHandlerInstalled = 0;

#if defined(KOS_POSIX)
/* Per-task pthread handles and semaphores for scheduling */
static pthread_t OsPort_TaskThreads[OS_NUMBER_OF_TASKS];
static sem_t OsPort_TaskSemaphores[OS_NUMBER_OF_TASKS];
static volatile uint8_t OsPort_TaskRunning[OS_NUMBER_OF_TASKS];
static volatile uint8_t OsPort_ShutdownRequested = 0;

/* Timer dispatch thread — runs IncrementCounter outside the signal handler */
static pthread_t OsPort_TimerThread;
static sem_t OsPort_TimerSemaphore;
#endif

/* Stack for ISR context */
const SizeType OS_TOS_ISR = (SizeType)((const uint8_t *)&ISR_Stack + ISR_STACK_SIZE);

#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_OS_START_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */

/*
**  Initialize the pthread-based port.
*/
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_Init(void)
#else
void OsPort_Init(void)
#endif /* KOS_MEMORY_MAPPING */
{
#if defined(KOS_POSIX)
    TaskType i;

    /* Record start time for timestamp calculations */
    if (clock_gettime(CLOCK_MONOTONIC, &OsPort_StartTime) == -1) {
		 perror("Failed to read monotonic clock");
		 exit(EXIT_FAILURE);
    }

    /* Setup signal handlers for interrupt simulation */
    OsPort_SetupSignalHandlers();

    /* Initialize per-task semaphores (all initially 0 = blocked) */
    for (i = 0; i < OS_NUMBER_OF_TASKS; ++i) {
        if (sem_init(&OsPort_TaskSemaphores[i], 0, 0) != 0) {
            perror("Failed to initialize task semaphore");
            exit(EXIT_FAILURE);
        }
        OsPort_TaskThreads[i] = (pthread_t)0;
        OsPort_TaskRunning[i] = 0;
    }

    /* Initialize timer dispatch semaphore */
    if (sem_init(&OsPort_TimerSemaphore, 0, 0) != 0) {
        perror("Failed to initialize timer semaphore");
        exit(EXIT_FAILURE);
    }
    OsPort_TimerThread = (pthread_t)0;

    OsPort_ShutdownRequested = 0;

    printf("k-os Unix/Linux pthread port initialized\n");
#else
    (void)clock_gettime(CLOCK_MONOTONIC, &OsPort_StartTime);
    OsPort_SetupSignalHandlers();
#endif
}

/*
**  Shutdown the pthread-based port.
*/
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_Shutdown(void)
#else
void OsPort_Shutdown(void)
#endif /* KOS_MEMORY_MAPPING */
{
#if defined(KOS_POSIX)
    TaskType i;

    OsPort_ShutdownRequested = 1;

    /* Signal all task threads to unblock, then join them */
    for (i = 0; i < OS_NUMBER_OF_TASKS; ++i) {
        sem_post(&OsPort_TaskSemaphores[i]);
    }

    /* Signal timer thread to unblock */
    sem_post(&OsPort_TimerSemaphore);

    /* Give threads a moment to exit */
    usleep(10000);

    for (i = 0; i < OS_NUMBER_OF_TASKS; ++i) {
        if (OsPort_TaskThreads[i] != (pthread_t)0) {
            pthread_join(OsPort_TaskThreads[i], NULL);
            OsPort_TaskThreads[i] = (pthread_t)0;
        }
        sem_destroy(&OsPort_TaskSemaphores[i]);
    }

    if (OsPort_TimerThread != (pthread_t)0) {
        pthread_join(OsPort_TimerThread, NULL);
        OsPort_TimerThread = (pthread_t)0;
    }
    sem_destroy(&OsPort_TimerSemaphore);

    OsPort_TimerCleanup();
    pthread_mutex_destroy(&OsPort_GlobalMutex);
    pthread_cond_destroy(&OsPort_ScheduleCondition);

    printf("k-os Unix/Linux pthread port shutdown\n");
#else
    OsPort_TimerCleanup();
#endif
}

/*
**  Initialize task stack for pthread execution.
*/
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(uint8_t *, OSEK_OS_CODE) OsPort_TaskStackInit(
    P2VAR(Os_TaskFunctionType, AUTOMATIC, OSEK_OS_APPL_DATA) TaskFunc,
    P2VAR(uint8_t, AUTOMATIC, OSEK_OS_APPL_DATA) sp
)
#else
uint8_t * OsPort_TaskStackInit(TaskType TaskID, Os_TaskFunctionType const * TaskFunc, uint8_t * sp)
#endif /* KOS_MEMORY_MAPPING */
{
    /* In pthread implementation, stack initialization is handled by pthread_create */
    (void)TaskFunc;
    return sp;
}

/*
**  Get current timestamp in microseconds.
*/
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(uint32_t, OSEK_OS_CODE) OsPort_GetTimestamp(void)
#else
uint32_t OsPort_GetTimestamp(void)
#endif /* KOS_MEMORY_MAPPING */
{
    struct timespec current_time;
    uint64_t timestamp_ns;

    clock_gettime(CLOCK_MONOTONIC, &current_time);

    /* Calculate nanoseconds since start */
    timestamp_ns = (current_time.tv_sec - OsPort_StartTime.tv_sec) * 1000000000ULL +
                   (current_time.tv_nsec - OsPort_StartTime.tv_nsec);

    /* Convert to microseconds and return as uint32 */
    return (uint32_t)(timestamp_ns / 1000ULL);
}

/*
**  Setup signal handlers for interrupt simulation.
*/
void OsPort_SetupSignalHandlers(void)
{
    struct sigaction sa;

    /* Setup signal handler for timer interrupts (SIGALRM) */
    memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = OsPort_SignalHandler;
    sa.sa_flags = SA_SIGINFO | SA_RESTART;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGALRM, &sa, &OsPort_OldSigAction) == -1) {
        perror("Failed to setup signal handler");
        exit(EXIT_FAILURE);
    }
    OsPort_SignalHandlerInstalled = 1;

    /* Initialize signal set for timer signals */
    sigemptyset(&OsPort_TimerSignalSet);
    sigaddset(&OsPort_TimerSignalSet, SIGALRM);
}

/*
**  Initialize system timer using POSIX timers.
*/
void OsPort_TimerInit(void)
{
    struct sigevent sev;
    struct itimerspec its;

    if (OsPort_TimerCreated) {
		fputs("System timer is already running\n", stderr);
		exit(EXIT_FAILURE);
    }

    /* Create timer */
    memset(&sev, 0, sizeof(sev));
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGALRM;
    sev.sigev_value.sival_ptr = &OsPort_SystemTimer;

    if (timer_create(CLOCK_MONOTONIC, &sev, &OsPort_SystemTimer) == -1) {
        perror("Failed to create system timer");
        exit(EXIT_FAILURE);
    }
    OsPort_TimerCreated = 1;

    /* Configure timer for 1ms intervals (1000 Hz) */
    its.it_value.tv_sec = 0;
    its.it_value.tv_nsec = 1000000;  /* 1ms */
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 1000000;  /* 1ms */

    if (timer_settime(OsPort_SystemTimer, 0, &its, NULL) == -1) {
        perror("Failed to start system timer");
        exit(EXIT_FAILURE);
    }
}

/*
**  Cleanup system timer.
*/
void OsPort_TimerCleanup(void)
{
    if (OsPort_TimerCreated) {
		if (timer_delete(OsPort_SystemTimer) == -1) {
			perror("Failed to delete system timer");
			exit(EXIT_FAILURE);
		}
		OsPort_TimerCreated = 0;
    }
    if (OsPort_SignalHandlerInstalled) {
		if (sigaction(SIGALRM, &OsPort_OldSigAction, NULL) == -1) {
			perror("Failed to restore signal handler");
			exit(EXIT_FAILURE);
		}
		OsPort_SignalHandlerInstalled = 0;
    }
}

/*
**  Signal handler for timer interrupts.
**  Must be async-signal-safe: only sets a flag and posts a semaphore.
**  The actual IncrementCounter call is deferred to the timer dispatch thread.
*/
void OsPort_SignalHandler(int sig, siginfo_t *info, void *context)
{
    (void)info;
    (void)context;
    switch (sig) {
        case SIGALRM:
            OsPort_InterruptFlag = 1;
#if defined(KOS_POSIX)
            /* Wake the timer dispatch thread to process the tick */
            sem_post(&OsPort_TimerSemaphore);
#endif
            break;

        default:
            break;
    }
}

/*
**  Simulate an interrupt by sending a signal.
*/
void OsPort_SimulateInterrupt(int signal_num)
{
    kill(getpid(), signal_num);
}

/*
**  Get system time in nanoseconds.
*/
uint64_t OsPort_GetSystemTimeNs(void)
{
    struct timespec current_time;

    clock_gettime(CLOCK_MONOTONIC, &current_time);

    return (current_time.tv_sec - OsPort_StartTime.tv_sec) * 1000000000ULL +
           (current_time.tv_nsec - OsPort_StartTime.tv_nsec);
}

/*
**  Keyboard loop for interactive debugging (optional).
*/
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_KeyboardLoop(void)
#else
void OsPort_KeyboardLoop(void)
#endif /* KOS_MEMORY_MAPPING */
{
#if defined(KOS_POSIX)
    /* Simple keyboard loop for debugging */
    char input;
    printf("k-os pthread port running. Press 'q' to quit.\n");

    while (!OsPort_ShutdownRequested) {
        input = getchar();
        if (input == 'q' || input == 'Q') {
            printf("Shutting down k-os...\n");
            break;
        }
        usleep(100000); /* 100ms delay */
    }
#else
    (void)0;
#endif
}

#if defined(KOS_POSIX)

/*
**  Timer dispatch thread: receives ticks from the signal handler and
**  calls IncrementCounter in a normal thread context (no signal handler
**  restrictions, no deadlock risk with the global mutex).
*/
static void *OsPort_TimerDispatchThread(void *arg)
{
    (void)arg;
    while (!OsPort_ShutdownRequested) {
        sem_wait(&OsPort_TimerSemaphore);
        if (OsPort_ShutdownRequested) {
            break;
        }
        /* Process the timer tick — this may activate tasks via alarms */
        if (OS_NUMBER_OF_COUNTERS > 0) {
            IncrementCounter(0);
        }
    }
    return NULL;
}

/*
**  Thread entry point for an OSEK task.
**  Each task runs in its own pthread and blocks on a semaphore
**  until the scheduler posts it.
*/
static void *OsPort_TaskThreadEntry(void *arg)
{
    TaskType TaskID = (TaskType)(uintptr_t)arg;

    while (!OsPort_ShutdownRequested) {
        /* Block until this task is scheduled (semaphore posted) */
        sem_wait(&OsPort_TaskSemaphores[TaskID]);

        if (OsPort_ShutdownRequested) {
            break;
        }

        OsPort_TaskRunning[TaskID] = 1;

        /* Execute the task function */
        OS_TaskConf[TaskID].TaskFunction();

        OsPort_TaskRunning[TaskID] = 0;
    }

    return NULL;
}

/*
**  Create a pthread for a task if it doesn't exist yet.
*/
static void OsPort_CreateTaskThread(TaskType TaskID)
{
    if (OsPort_TaskThreads[TaskID] == (pthread_t)0) {
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        /* Use default stack size */
        if (pthread_create(&OsPort_TaskThreads[TaskID], &attr,
                           OsPort_TaskThreadEntry, (void *)(uintptr_t)TaskID) != 0) {
            perror("Failed to create task thread");
            exit(EXIT_FAILURE);
        }
        pthread_attr_destroy(&attr);
    }
}

/*
**  Start the highest-priority ready task.
**  This is the core of the POSIX scheduling loop: it runs in the main
**  thread and dispatches tasks by posting their semaphores.
**  For preemptive tasks, each task runs concurrently in its own thread.
**  For non-preemptive tasks, the scheduler waits for completion.
*/
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_StartCurrentTask(void)
#else
void OsPort_StartCurrentTask(void)
#endif /* KOS_MEMORY_MAPPING */
{
    TaskType i;
    TaskType highest;

    /* Create threads for all tasks upfront */
    for (i = 0; i < OS_NUMBER_OF_TASKS; ++i) {
        OsPort_CreateTaskThread(i);
    }

    /* Create the timer dispatch thread */
    {
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        if (pthread_create(&OsPort_TimerThread, &attr,
                           OsPort_TimerDispatchThread, NULL) != 0) {
            perror("Failed to create timer dispatch thread");
            exit(EXIT_FAILURE);
        }
        pthread_attr_destroy(&attr);
    }

    /* Main scheduling loop */
    while (!OsPort_ShutdownRequested) {
        pthread_mutex_lock(&OsPort_GlobalMutex);
        while (!OsMLQ_TasksAreReady() && !OsPort_ShutdownRequested) {
            Os_CurrentTID = INVALID_TASK;
            pthread_mutex_unlock(&OsPort_GlobalMutex);

            /* If no tasks are ready and no alarms are active, shut down */
            if (OsAlm_GetActiveAlarms() == 0) {
                fprintf(stderr, "k-os: no ready tasks and no active alarms, shutting down\n");
                OsPort_ShutdownRequested = 1;
                break;
            }

            /* Idle — yield CPU and wait briefly for timer ticks */
            usleep(500);  /* 0.5ms — less than timer period */
            pthread_mutex_lock(&OsPort_GlobalMutex);
        }
        pthread_mutex_unlock(&OsPort_GlobalMutex);

        if (OsPort_ShutdownRequested) {
            break;
        }

        /* Pick highest-priority ready task and remove from MLQ */
        pthread_mutex_lock(&OsPort_GlobalMutex);
        highest = OsMLQ_GetHighestPrio();
        if (highest != INVALID_TASK) {
            OsMLQ_RemoveTask(highest);
        }
        pthread_mutex_unlock(&OsPort_GlobalMutex);

        if (highest == INVALID_TASK) {
            continue;
        }

        /* Set task as running */
        OS_TCB[highest].State = RUNNING;
        Os_CurrentTID = highest;
        Os_CurrentTCB = &OS_TCB[highest];

        /* Post semaphore to unblock the task's thread */
        sem_post(&OsPort_TaskSemaphores[highest]);

        /* For non-preemptive tasks, wait for completion before scheduling next */
        if (!OsTask_IsPreemptable(highest)) {
            while (OsPort_TaskRunning[highest] && !OsPort_ShutdownRequested) {
                sched_yield();
            }
        }
    }
}

#endif /* KOS_POSIX */

/*
**  Create events for task synchronization.
*/
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_CreateEvents(void)
#else
void OsPort_CreateEvents(void)
#endif /* KOS_MEMORY_MAPPING */
{
    /* Events are handled through semaphores in this implementation */
}

/*
**  Signal scheduling event.
*/
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_SignalSchedulingEvent(void)
#else
void OsPort_SignalSchedulingEvent(void)
#endif /* KOS_MEMORY_MAPPING */
{
    pthread_cond_broadcast(&OsPort_ScheduleCondition);
}

/*
**  Adjust process priority class (Unix/Linux specific).
*/
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_AdjustProcessPriorityClass(void)
#else
void OsPort_AdjustProcessPriorityClass(void)
#endif /* KOS_MEMORY_MAPPING */
{
    /* Set real-time scheduling policy if available */
    struct sched_param param;
    param.sched_priority = sched_get_priority_max(SCHED_RR);

    if (sched_setscheduler(0, SCHED_RR, &param) == -1) {
        /* Fall back to normal priority if real-time scheduling fails */
        if (nice(-10) == -1) {
            perror("Warning: Could not set high priority");
        }
    }
}

/*
**  Task suspension hook (optional).
*/
void OsPort_SuspendHook(uint16_t taskID)
{
    /* Optional hook for task suspension */
    (void)taskID;
    (void)sched_yield();
}

/*
**  Save context (no-op for pthread-based port — pthreads handle context).
*/
void OsPort_SaveContext(void)
{
    /* Context is managed by pthreads */
}

/*
**  Restore context (no-op for pthread-based port).
*/
void OsPort_RestoreContext(void)
{
    /* Context is managed by pthreads */
}

/*
**  Switch to ISR context (no-op for pthread-based port).
*/
void OsPort_SwitchToISRContext(void)
{
    /* ISR context is the calling thread context */
}

#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_OS_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */
