/*
 * Windows primitives using critical sections to simulate interrupt masking.
 */
#ifndef __CPU_PRIMITIVES_WIN32_H
#define __CPU_PRIMITIVES_WIN32_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdint.h>

/* Provided by Os_Port_Win32.c */
extern CRITICAL_SECTION OsPort_GlobalCriticalSection;
void OsPort_SimulateInterrupt(void);

#if defined(_MSC_VER)
    #define CPU_THREAD_LOCAL __declspec(thread)
#else
    #define CPU_THREAD_LOCAL _Thread_local
#endif

static CPU_THREAD_LOCAL uint8_t __cpu_interrupt_disabled = 0;

static inline uint8_t cpu_win_get_state(void) { return __cpu_interrupt_disabled; }
static inline void cpu_win_set_state(uint8_t state) { __cpu_interrupt_disabled = state ? 1u : 0u; }

#define CPU_DISABLE_ALL_INTERRUPTS()                             \
    do {                                                         \
        cpu_win_set_state(1u);                                   \
        EnterCriticalSection(&OsPort_GlobalCriticalSection);     \
    } while (0)

#define CPU_ENABLE_ALL_INTERRUPTS()                              \
    do {                                                         \
        cpu_win_set_state(0u);                                   \
        LeaveCriticalSection(&OsPort_GlobalCriticalSection);     \
    } while (0)

#define CPU_INTERRUPTS_DISABLED() cpu_win_get_state()

#define CPU_SAVE_AND_DISABLE_INTERRUPTS(state)                   \
    do {                                                         \
        (state) = cpu_win_get_state();                           \
        cpu_win_set_state(1u);                                   \
        EnterCriticalSection(&OsPort_GlobalCriticalSection);     \
    } while (0)

#define CPU_RESTORE_INTERRUPTS(state)                            \
    do {                                                         \
        cpu_win_set_state((uint8_t)(state));                     \
        if ((state) == 0u) {                                     \
            LeaveCriticalSection(&OsPort_GlobalCriticalSection); \
        }                                                        \
    } while (0)

#define CPU_SOFTWARE_INTERRUPT() OsPort_SimulateInterrupt()
#define CPU_NO_OPERATION() __asm nop
#define CPU_BREAKPOINT() __debugbreak()
#define CPU_MEMORY_BARRIER() MemoryBarrier()

#endif /* __CPU_PRIMITIVES_WIN32_H */
