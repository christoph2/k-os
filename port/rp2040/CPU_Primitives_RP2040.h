/*
 * Cortex-M0+/M33 primitives for RP2040/RP2350.
 */
#ifndef __CPU_PRIMITIVES_RP2040_H
#define __CPU_PRIMITIVES_RP2040_H

#include <stdint.h>
#include "hardware/regs/addressmap.h"
#include "hardware/regs/m0plus.h"

#define CPU_DSB() __asm volatile("dsb" ::: "memory")
#define CPU_ISB() __asm volatile("isb" ::: "memory")
#define CPU_NOP() __asm volatile("nop")

#define CPU_ENTER_POWERDOWN_MODE() \
    do { CPU_DSB(); __asm volatile("wfi"); CPU_ISB(); } while (0)
#define CPU_LEAVE_POWERDOWN_MODE() /* Wake up by interrupt */

#define CPU_DISABLE_WATCHDOG_TIMER() /* Not available */

#define CPU_SOFTWARE_INTERRUPT()                                                                                       \
    do {                                                                                                               \
        *(volatile uint32_t *)(PPB_BASE + M0PLUS_ICSR_OFFSET) = M0PLUS_ICSR_PENDSVSET_BITS;                            \
        CPU_DSB();                                                                                                     \
        CPU_ISB();                                                                                                     \
    } while (0)

#define CPU_RETURN_FROM_INTERRUPT() __asm volatile("bx lr" ::: "memory")
#define CPU_BREAKPOINT() __asm volatile("bkpt 0")
#define CPU_NO_OPERATION() CPU_NOP()

static inline uint32_t cpu_get_primask(void)
{
    uint32_t primask;
    __asm volatile("mrs %0, PRIMASK" : "=r"(primask));
    return primask;
}

static inline void cpu_set_primask(uint32_t primask)
{
    __asm volatile("msr PRIMASK, %0" : : "r"(primask) : "memory");
}

#define CPU_DISABLE_ALL_INTERRUPTS()                    \
    do {                                                \
        __asm volatile("cpsid i" ::: "memory");         \
        CPU_DSB();                                      \
        CPU_ISB();                                      \
    } while (0)

#define CPU_ENABLE_ALL_INTERRUPTS()                     \
    do {                                                \
        __asm volatile("cpsie i" ::: "memory");         \
        CPU_DSB();                                      \
        CPU_ISB();                                      \
    } while (0)

#define CPU_INTERRUPTS_DISABLED() (cpu_get_primask() != 0)

#define CPU_SAVE_AND_DISABLE_INTERRUPTS(state)          \
    do {                                                \
        (state) = cpu_get_primask();                    \
        __asm volatile("cpsid i" ::: "memory");         \
        CPU_DSB();                                      \
        CPU_ISB();                                      \
    } while (0)

#define CPU_RESTORE_INTERRUPTS(state)                   \
    do {                                                \
        cpu_set_primask(state);                         \
        CPU_DSB();                                      \
        CPU_ISB();                                      \
    } while (0)

#define CPU_MEMORY_BARRIER() CPU_DSB()
#define CPU_COMPILER_BARRIER() __asm volatile("" ::: "memory")
#define CPU_DATA_SYNC_BARRIER() CPU_DSB()
#define CPU_INSTRUCTION_SYNC_BARRIER() CPU_ISB()

#define CPU_SYSTEM_RESET()                                                                                             \
    do {                                                                                                               \
        CPU_DSB();                                                                                                     \
        *(volatile uint32_t *)(PPB_BASE + M0PLUS_AIRCR_OFFSET) =                                                       \
            (0x5FAu << M0PLUS_AIRCR_VECTKEY_Pos) | M0PLUS_AIRCR_SYSRESETREQ_BITS;                                      \
        CPU_DSB();                                                                                                     \
        CPU_ISB();                                                                                                     \
    } while (0)

#define CPU_ENTER_CRITICAL()  uint32_t __cpu_int_state; CPU_SAVE_AND_DISABLE_INTERRUPTS(__cpu_int_state)
#define CPU_LEAVE_CRITICAL()  CPU_RESTORE_INTERRUPTS(__cpu_int_state)

#endif /* __CPU_PRIMITIVES_RP2040_H */
