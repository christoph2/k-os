/*
 * Self-contained CPU primitives for k_os.
 */
#ifndef __CPU_PRIMITIVES_H
#define __CPU_PRIMITIVES_H

#include <stdint.h>
#include <stdbool.h>

/* Per-target selection */
#if defined(_WIN32) || defined(_WIN64)
    #include "port/windows/CPU_Primitives_Win32.h"
#elif defined(__unix__) || defined(__unix) || defined(unix) || defined(__linux__)
    #include "port/pthreads/CPU_Primitives_Unix.h"
#elif defined(PICO_RP2040) || defined(PICO_BOARD) || defined(PICO_RP2350)
    #include "port/rp2040/CPU_Primitives_RP2040.h"
#elif defined(__arm__)
    #include "port/stm32/CPU_Primitives_ARM.h"
#else
    /* Fallback no-op implementations. */
    #define CPU_DISABLE_ALL_INTERRUPTS()     do { } while (0)
    #define CPU_ENABLE_ALL_INTERRUPTS()      do { } while (0)
    #define CPU_INTERRUPTS_DISABLED()        ((uint8_t)0)
    #define CPU_SAVE_AND_DISABLE_INTERRUPTS(state) do { (void)(state); } while (0)
    #define CPU_RESTORE_INTERRUPTS(state)    do { (void)(state); } while (0)
    #define CPU_SOFTWARE_INTERRUPT()         do { } while (0)
#endif

#endif /* __CPU_PRIMITIVES_H */
