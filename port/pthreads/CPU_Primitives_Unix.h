/*
 * Pthreads/Linux CPU primitives for k_os.
 */
#ifndef __CPU_PRIMITIVES_UNIX_H
#define __CPU_PRIMITIVES_UNIX_H

#include <pthread.h>
#include <signal.h>
#include <stdint.h>

extern pthread_mutex_t OsPort_GlobalMutex;
extern pthread_cond_t OsPort_ScheduleCondition;
extern volatile sig_atomic_t OsPort_InterruptFlag;

#if defined(_MSC_VER)
    #define CPU_THREAD_LOCAL __declspec(thread)
#else
    #define CPU_THREAD_LOCAL _Thread_local
#endif

static CPU_THREAD_LOCAL uint8_t __cpu_unix_disabled = 0;

#define CPU_DISABLE_ALL_INTERRUPTS()                \
    do {                                            \
        if (!__cpu_unix_disabled) {                 \
            __cpu_unix_disabled = 1;                \
            pthread_mutex_lock(&OsPort_GlobalMutex);\
        }                                           \
    } while (0)

#define CPU_ENABLE_ALL_INTERRUPTS()                 \
    do {                                            \
        if (__cpu_unix_disabled) {                  \
            __cpu_unix_disabled = 0;                \
            pthread_mutex_unlock(&OsPort_GlobalMutex);\
        }                                           \
    } while (0)

#define CPU_INTERRUPTS_DISABLED() (__cpu_unix_disabled != 0)

#define CPU_SAVE_AND_DISABLE_INTERRUPTS(state)      \
    do {                                            \
        (state) = __cpu_unix_disabled;              \
        if (!__cpu_unix_disabled) {                 \
            __cpu_unix_disabled = 1;                \
            pthread_mutex_lock(&OsPort_GlobalMutex);\
        }                                           \
    } while (0)

#define CPU_RESTORE_INTERRUPTS(state)               \
    do {                                            \
        if ((state) == 0u && __cpu_unix_disabled) { \
            __cpu_unix_disabled = 0;                \
            pthread_mutex_unlock(&OsPort_GlobalMutex);\
        }                                           \
    } while (0)

#define CPU_SOFTWARE_INTERRUPT()               \
    do {                                       \
        OsPort_InterruptFlag = 1;              \
        pthread_cond_signal(&OsPort_ScheduleCondition); \
    } while (0)

#define CPU_NO_OPERATION() do { __asm__ __volatile__("" ::: "memory"); } while (0)
#define CPU_MEMORY_BARRIER() __sync_synchronize()

#endif /* __CPU_PRIMITIVES_UNIX_H */
