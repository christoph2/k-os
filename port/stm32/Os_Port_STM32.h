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
#if !defined(__OS_PORT_STM32_H)
#define __OS_PORT_STM32_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* STM32 HAL includes */
#ifdef STM32F1xx
#include "stm32f1xx_hal.h"
#elif defined(STM32F4xx)
#include "stm32f4xx_hal.h"
#elif defined(STM32F7xx)
#include "stm32f7xx_hal.h"
#elif defined(STM32H7xx)
#include "stm32h7xx_hal.h"
#elif defined(STM32L4xx)
#include "stm32l4xx_hal.h"
#elif defined(STM32L4R5xx)
#include "stm32l4xx_hal.h"
#elif defined(STM32G4xx)
#elif defined(STM32G4xx)
#include "stm32g4xx_hal.h"
#else
#error "Unsupported STM32 family. Please define STM32F1xx, STM32F4xx, STM32F7xx, STM32H7xx, STM32L4xx, or STM32G4xx"
#endif

#include <stdbool.h>
#include <stdint.h>

/*
**  Port-specific types.
*/
typedef uint32_t InterruptStateType;
typedef int * OsPort_StackPointerType;

/*
**  Task context structure for STM32 Cortex-M.
*/
typedef struct {
    uint32_t *stack_pointer;
    uint32_t *stack_base;
    uint32_t stack_size;
    uint8_t priority;
    uint8_t state;
} OsPort_TaskContext;

/*
**  STM32-specific configuration.
*/
#define OSPORT_SYSTICK_FREQUENCY_HZ 1000U /* 1ms tick */
#define OSPORT_PENDSV_PRIORITY 0xFF /* Lowest priority */
#define OSPORT_SYSTICK_PRIORITY 0x00 /* Highest priority */

/*
**  Stack frame size for Cortex-M context switching.
*/
#define OSPORT_INITIAL_XPSR 0x01000000U /* Thumb state */

/*
**  ARM Cortex-M specific registers and definitions.
*/
#define NVIC_ICSR_PENDSVSET_Pos 28U
#define NVIC_ICSR_PENDSVSET_Msk (1UL << NVIC_ICSR_PENDSVSET_Pos)

#define NVIC_ICSR_PENDSVCLR_Pos 27U
#define NVIC_ICSR_PENDSVCLR_Msk (1UL << NVIC_ICSR_PENDSVCLR_Pos)

/*
**  Global variables.
*/
extern volatile uint32_t OsPort_SystemTickCounter;
// extern TIM_HandleTypeDef OsPort_SystemTimer;

/*
**  Port-specific function declarations.
*/
void OsPort_InitSysTick(void);
void OsPort_InitPendSV(void);
void OsPort_TriggerContextSwitch(void);
uint32_t OsPort_GetSystemTick(void);
void OsPort_DelayMs(uint32_t delay);

/*
**  Interrupt handlers.
*/
void SysTick_Handler(void);
void PendSV_Handler(void);
void HardFault_Handler(void);

/*
**  Context switching macros using ARM Cortex-M specific instructions.
*/
#define OsPort_SaveContext()                                                                                           \
    __asm volatile("mrs r0, psp                \n\t"                                                                   \
                   "stmdb r0!, {r4-r11}        \n\t"                                                                   \
                   "msr psp, r0                \n\t" ::                                                                \
                           : "r0", "memory")

#define OsPort_RestoreContext()                                                                                        \
    __asm volatile("mrs r0, psp                \n\t"                                                                   \
                   "ldmia r0!, {r4-r11}        \n\t"                                                                   \
                   "msr psp, r0                \n\t" ::                                                                \
                           : "r0", "memory")

#define OsPort_StartCurrentTask()                                                                                      \
    __asm volatile("ldr r0, =0xE000ED04        \n\t" /* NVIC_ICSR */                                                   \
                   "ldr r1, =0x10000000        \n\t" /* PENDSVSET bit */                                               \
                   "str r1, [r0]               \n\t"                                                                   \
                   "dsb                        \n\t"                                                                   \
                   "isb                        \n\t" ::                                                                \
                           : "r0", "r1", "memory")

#define OsPort_SwitchToISRContext() __asm volatile("mrs r0, msp                \n\t" ::: "r0", "memory")

/*
**  Interrupt control macros for Cortex-M.
*/
#define OsPort_DisableAllInterrupts()                                                                                  \
    __asm volatile("cpsid i                    \n\t"                                                                   \
                   "dsb                        \n\t"                                                                   \
                   "isb                        \n\t" ::                                                                \
                           : "memory")

#define OsPort_EnableAllInterrupts()                                                                                   \
    __asm volatile("cpsie i                    \n\t"                                                                   \
                   "dsb                        \n\t"                                                                   \
                   "isb                        \n\t" ::                                                                \
                           : "memory")

#define OsPort_DisableAllOsInterrupts()                                                                                \
    do {                                                                                                               \
        OsPort_InterruptState = __get_BASEPRI();                                                                       \
        __set_BASEPRI(OSPORT_PENDSV_PRIORITY << (8 - __NVIC_PRIO_BITS));                                               \
        __DSB();                                                                                                       \
        __ISB();                                                                                                       \
    } while (0)

#define OsPort_EnableAllOsInterrupts()                                                                                 \
    do {                                                                                                               \
        __set_BASEPRI(OsPort_InterruptState);                                                                          \
        __DSB();                                                                                                       \
        __ISB();                                                                                                       \
    } while (0)

/*
**  Memory barrier macros.
*/
#define OsPort_DataMemoryBarrier() __DSB()
#define OsPort_InstructionMemoryBarrier() __ISB()
#define OsPort_DataSyncBarrier() __DMB()

/*
**  Stack initialization macros.
*/
#define OSPORT_STACK_FILL_PATTERN 0xDEADBEEF
#define OSPORT_STACK_ALIGNMENT 8U

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __OS_PORT_STM32_H */
