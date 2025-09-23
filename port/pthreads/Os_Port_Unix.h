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
**  Context switching macros using pthreads.
*/
#define OsPort_SaveContext() \
    _BEGIN_BLOCK \
    pthread_mutex_lock(&OsPort_GlobalMutex); \
    _END_BLOCK

#define OsPort_RestoreContext() \
    _BEGIN_BLOCK \
    pthread_mutex_unlock(&OsPort_GlobalMutex); \
    pthread_cond_signal(&OsPort_ScheduleCondition); \
    _END_BLOCK

#define OsPort_StartCurrentTask() \
    _BEGIN_BLOCK \
    pthread_cond_wait(&OsPort_ScheduleCondition, &OsPort_GlobalMutex); \
    pthread_mutex_unlock(&OsPort_GlobalMutex); \
    _END_BLOCK

#define OsPort_SwitchToISRContext() \
    _BEGIN_BLOCK \
    pthread_mutex_lock(&OsPort_GlobalMutex); \
    _END_BLOCK

/*
**  Interrupt disable/enable macros.
*/
#define OsPort_DisableAllInterrupts() \
    _BEGIN_BLOCK \
    sigset_t new_mask, old_mask; \
    sigfillset(&new_mask); \
    pthread_sigmask(SIG_BLOCK, &new_mask, &old_mask); \
    _END_BLOCK

#define OsPort_EnableAllInterrupts() \
    _BEGIN_BLOCK \
    sigset_t old_mask; \
    sigemptyset(&old_mask); \
    pthread_sigmask(SIG_SETMASK, &old_mask, NULL); \
    _END_BLOCK

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* __OS_PORT_UNIX_PTHREAD_H */