/*
 * Cortex-M0+/M33 primitives for RP2040/RP2350.
 */
#ifndef __CPU_PRIMITIVES_RP2040_H
#define __CPU_PRIMITIVES_RP2040_H

#include <stdint.h>

#if defined(PICO_RP2350) || defined(__ARM_ARCH_8M_MAIN__)
    #include "core_cm33.h"
#else
    #include "core_cm0plus.h"
    #include "hardware/regs/addressmap.h"
    #include "hardware/regs/m0plus.h"
#endif

#define CPU_DSB() __asm volatile("dsb" ::: "memory")
#define CPU_ISB() __asm volatile("isb" ::: "memory")
#define CPU_NOP() __asm volatile("nop")

#define CPU_ENTER_POWERDOWN_MODE() \
    do { CPU_DSB(); __asm volatile("wfi"); CPU_ISB(); } while (0)
#define CPU_LEAVE_POWERDOWN_MODE() /* Wake up by interrupt */

#define CPU_DISABLE_WATCHDOG_TIMER() /* Not available */

#define CPU_SOFTWARE_INTERRUPT()                      \
    do {                                              \
        SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;           \
        CPU_DSB();                                    \
        CPU_ISB();                                    \
    } while (0)

#define CPU_RETURN_FROM_INTERRUPT() __asm volatile("bx lr" ::: "memory")
#define CPU_BREAKPOINT() __asm volatile("bkpt 0")
#define CPU_NO_OPERATION() CPU_NOP()

static inline uint32_t cpu_get_primask(void)
{
    return __get_PRIMASK();
}

static inline void cpu_set_primask(uint32_t primask)
{
    __set_PRIMASK(primask);
}

#define CPU_DISABLE_ALL_INTERRUPTS()                    \
    do {                                                \
        __disable_irq();                                \
        CPU_DSB();                                      \
        CPU_ISB();                                      \
    } while (0)

#define CPU_ENABLE_ALL_INTERRUPTS()                     \
    do {                                                \
        __enable_irq();                                 \
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
        SCB->AIRCR = (0x5FAUL << SCB_AIRCR_VECTKEY_Pos) | SCB_AIRCR_SYSRESETREQ_Msk;                                   \
        CPU_DSB();                                                                                                     \
        CPU_ISB();                                                                                                     \
    } while (0)

#define CPU_ENTER_CRITICAL()  uint32_t __cpu_int_state; CPU_SAVE_AND_DISABLE_INTERRUPTS(__cpu_int_state)
#define CPU_LEAVE_CRITICAL()  CPU_RESTORE_INTERRUPTS(__cpu_int_state)

#endif /* __CPU_PRIMITIVES_RP2040_H */
