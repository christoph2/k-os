/*
   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).

   (C) 2007-2025 by Christoph Schueler <github.com/Christoph2,
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
#if !defined(__OS_PORT_UNIX_PTHREAD_H)
#define __OS_PORT_UNIX_PTHREAD_H

#ifdef __cplusplus
extern "C"
{
#endif  /* __cplusplus */

#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>

/*
**  Port-specific types.
*/
typedef uint8_t InterruptStateType;
typedef pthread_mutex_t OsPort_MutexType;
typedef pthread_cond_t OsPort_ConditionType;
typedef sem_t OsPort_SemaphoreType;

/*
**  Task context structure for pthread-based tasks.
*/
typedef struct {
    pthread_t thread_id;
    uint8_t *stack_base;
    size_t stack_size;
    Os_TaskFunctionType *task_func;
    TaskType task_id;
    uint8_t priority;
    volatile uint8_t state;
} OsPort_TaskContext;

/*
**  Global variables.
*/
extern pthread_mutex_t OsPort_GlobalMutex;
extern pthread_cond_t OsPort_ScheduleCondition;
extern volatile sig_atomic_t OsPort_InterruptFlag;
extern struct timespec OsPort_StartTime;

/*
**  Port-specific functions.
*/
void OsPort_InitializeMutexes(void);
void OsPort_CleanupMutexes(void);
void OsPort_SetupSignalHandlers(void);
void OsPort_TimerInit(void);
void OsPort_TimerCleanup(void);
uint64_t OsPort_GetSystemTimeNs(void);

/*
**  Interrupt simulation functions.
*/
void OsPort_SimulateInterrupt(int signal_num);
void OsPort_SignalHandler(int sig, siginfo_t *info, void *context);

/*
**  Context switching — no-op for pthread-based port.
**  Context is managed by pthreads; these are declared as functions in Os_Port_Unix.c.
*/

/*
**  Interrupt disable/enable macros — provided by CPU_Primitives_Unix.h.
**  OsPort_DisableAllOsInterrupts/EnableAllOsInterrupts are mapped in Os_Port.h
**  to OsPort_DisableOsInterrupts/EnableOsInterrupts on Linux.
*/

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* __OS_PORT_UNIX_PTHREAD_H */