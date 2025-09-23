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

#include "Os_Port_Unix.h"
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

/*
**  Global variables for pthread-based port.
*/
InterruptStateType OsPort_InterruptState = 0;
pthread_mutex_t OsPort_GlobalMutex;
pthread_cond_t OsPort_ScheduleCondition;
volatile sig_atomic_t OsPort_InterruptFlag = 0;
struct timespec OsPort_StartTime;

static timer_t OsPort_SystemTimer;
static struct sigaction OsPort_OldSigAction;
static sigset_t OsPort_TimerSignalSet;

/* Stack for ISR context */
static uint8_t ISR_Stack[ISR_STACK_SIZE];
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
    int result;

    /* Record start time for timestamp calculations */
    clock_gettime(CLOCK_MONOTONIC, &OsPort_StartTime);

    /* Initialize mutexes and condition variables */
    OsPort_InitializeMutexes();

    /* Setup signal handlers for interrupt simulation */
    OsPort_SetupSignalHandlers();

    /* Initialize system timer */
    OsPort_TimerInit();

    printf("k-os Unix/Linux pthread port initialized\n");
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
    OsPort_TimerCleanup();
    OsPort_CleanupMutexes();
    printf("k-os Unix/Linux pthread port shutdown\n");
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
    /* We store the task function pointer for later use */
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
**  Initialize mutexes and condition variables.
*/
void OsPort_InitializeMutexes(void)
{
    int result;

    result = pthread_mutex_init(&OsPort_GlobalMutex, NULL);
    if (result != 0) {
        fprintf(stderr, "Failed to initialize global mutex: %s\n", strerror(result));
        exit(EXIT_FAILURE);
    }

    result = pthread_cond_init(&OsPort_ScheduleCondition, NULL);
    if (result != 0) {
        fprintf(stderr, "Failed to initialize schedule condition: %s\n", strerror(result));
        exit(EXIT_FAILURE);
    }
}

/*
**  Cleanup mutexes and condition variables.
*/
void OsPort_CleanupMutexes(void)
{
    pthread_cond_destroy(&OsPort_ScheduleCondition);
    pthread_mutex_destroy(&OsPort_GlobalMutex);
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

    /* Create timer */
    memset(&sev, 0, sizeof(sev));
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGALRM;
    sev.sigev_value.sival_ptr = &OsPort_SystemTimer;

    if (timer_create(CLOCK_MONOTONIC, &sev, &OsPort_SystemTimer) == -1) {
        perror("Failed to create system timer");
        exit(EXIT_FAILURE);
    }

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
    timer_delete(OsPort_SystemTimer);
    sigaction(SIGALRM, &OsPort_OldSigAction, NULL);
}

/*
**  Signal handler for interrupt simulation.
*/
void OsPort_SignalHandler(int sig, siginfo_t *info, void *context)
{
    switch (sig) {
        case SIGALRM:
            /* System tick interrupt */
            OsPort_InterruptFlag = 1;

            #if defined(OS_FEATURE_INSTALL_RTI_HANDLER)
            /* Call the RTI (Real Time Interrupt) handler */
            OS_RTI_DRIVER_IMPL();
            #endif
            break;

        default:
            /* Handle other signals if needed */
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
    /* Simple keyboard loop for debugging */
    char input;
    printf("k-os pthread port running. Press 'q' to quit.\n");

    while (1) {
        input = getchar();
        if (input == 'q' || input == 'Q') {
            printf("Shutting down k-os...\n");
            break;
        }
        usleep(100000); /* 100ms delay */
    }
}

/*
**  Thread wrapper function for OSEK tasks.
*/
static void* OsPort_TaskWrapper(void* arg)
{
    OsPort_TaskContext* task_ctx = (OsPort_TaskContext*)arg;

    /* Wait for task to be scheduled */
    pthread_mutex_lock(&OsPort_GlobalMutex);
    pthread_cond_wait(&OsPort_ScheduleCondition, &OsPort_GlobalMutex);
    pthread_mutex_unlock(&OsPort_GlobalMutex);

    /* Execute the actual task function */
    if (task_ctx->task_func != NULL) {
        task_ctx->task_func();
    }

    return NULL;
}

/*
**  Create events for task synchronization.
*/
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_CreateEvents(void)
#else
void OsPort_CreateEvents(void)
#endif /* KOS_MEMORY_MAPPING */
{
    /* Events are handled through pthread conditions in this implementation */
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
    pthread_yield();
}

#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_OS_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */
