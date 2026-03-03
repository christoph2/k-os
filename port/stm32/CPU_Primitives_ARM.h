/*
 * Minimal Cortex-M CPU primitives for STM32 builds (self-contained).
 */
#ifndef __CPU_PRIMITIVES_ARM_H
#define __CPU_PRIMITIVES_ARM_H

#include <stdint.h>

#if defined(__ARMCC_VERSION) || defined(__GNUC__) || defined(__clang__)
    #define CPU_DSB() __asm volatile("dsb" ::: "memory")
    #define CPU_ISB() __asm volatile("isb" ::: "memory")
#else
    #define CPU_DSB()
    #define CPU_ISB()
#endif

#define CPU_DISABLE_ALL_INTERRUPTS()    do { __asm volatile("cpsid i" ::: "memory"); CPU_DSB(); CPU_ISB(); } while (0)
#define CPU_ENABLE_ALL_INTERRUPTS()     do { __asm volatile("cpsie i" ::: "memory"); CPU_DSB(); CPU_ISB(); } while (0)

#define CPU_INTERRUPTS_DISABLED()                       \
    ({ uint32_t primask; __asm volatile("mrs %0, primask" : "=r"(primask)); (primask & 0x1u); })

#define CPU_SAVE_AND_DISABLE_INTERRUPTS(state)          \
    do {                                                \
        uint32_t primask;                               \
        __asm volatile("mrs %0, primask" : "=r"(primask)); \
        (state) = (uint8_t)(primask & 0x1u);            \
        __asm volatile("cpsid i" ::: "memory");         \
        CPU_DSB();                                      \
        CPU_ISB();                                      \
    } while (0)

#define CPU_RESTORE_INTERRUPTS(state)                   \
    do {                                                \
        if ((state) == 0u) {                            \
            __asm volatile("cpsie i" ::: "memory");     \
        }                                               \
        CPU_DSB();                                      \
        CPU_ISB();                                      \
    } while (0)

#define CPU_SOFTWARE_INTERRUPT()                        \
    do {                                                \
        volatile uint32_t *icsr = (uint32_t *)0xE000ED04; \
        *icsr = (1u << 28); /* PENDSVSET */             \
        CPU_DSB();                                      \
        CPU_ISB();                                      \
    } while (0)

#define CPU_NO_OPERATION() __asm volatile("nop")
#define CPU_MEMORY_BARRIER() CPU_DSB()

#endif /* __CPU_PRIMITIVES_ARM_H */
