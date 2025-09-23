
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

#include "Os_Port.h"
#include "Os_Cfg.h"

/*
**  Global variables for STM32 port.
*/
InterruptStateType OsPort_InterruptState = 0;
volatile uint32_t OsPort_SystemTickCounter = 0;
TIM_HandleTypeDef OsPort_SystemTimer;

/* Stack for ISR context */
static uint8_t ISR_Stack[ISR_STACK_SIZE] __attribute__((aligned(8)));
const SizeType OS_TOS_ISR = (SizeType)((const uint8_t *)&ISR_Stack + ISR_STACK_SIZE);

/* External references */
extern TaskType Os_CurrentTask;
extern const uint8_t * Os_TaskStackPointer[];

#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_OS_START_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */

/*
**  Initialize the STM32 port.
*/
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_Init(void)
#else
void OsPort_Init(void)
#endif /* KOS_MEMORY_MAPPING */
{
    /* Initialize system tick timer */
    OsPort_InitSysTick();

    /* Initialize PendSV for context switching */
    OsPort_InitPendSV();

    /* Set thread mode to use PSP */
    __set_CONTROL(__get_CONTROL() | 0x02);
    __ISB();

    /* Initialize PSP with ISR stack */
    __set_PSP(OS_TOS_ISR);

    /* Enable fault handlers for debugging */
    SCB->SHCSR |= (SCB_SHCSR_USGFAULTENA_Msk |
                   SCB_SHCSR_BUSFAULTENA_Msk |
                   SCB_SHCSR_MEMFAULTENA_Msk);
}

/*
**  Shutdown the STM32 port.
*/
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_Shutdown(void)
#else
void OsPort_Shutdown(void)
#endif /* KOS_MEMORY_MAPPING */
{
    /* Disable SysTick */
    SysTick->CTRL = 0;

    /* Reset NVIC priorities */
    NVIC_SetPriority(PendSV_IRQn, 0);
    NVIC_SetPriority(SysTick_IRQn, 0);
}

/*
**  Initialize task stack for ARM Cortex-M execution.
*/
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(uint8_t *, OSEK_OS_CODE) OsPort_TaskStackInit(
    TaskType TaskID,
    P2VAR(Os_TaskFunctionType, AUTOMATIC, OSEK_OS_APPL_DATA) TaskFunc,
    P2VAR(uint8_t, AUTOMATIC, OSEK_OS_APPL_DATA) sp
)
#else
uint8_t * OsPort_TaskStackInit(TaskType TaskID, Os_TaskFunctionType const * TaskFunc, uint8_t * sp)
#endif /* KOS_MEMORY_MAPPING */
{
    uint32_t *stack_ptr = (uint32_t *)sp;

    /* Ensure stack is 8-byte aligned */
    stack_ptr = (uint32_t *)((uint32_t)stack_ptr & ~0x7UL);

    /* Initialize stack frame for initial task execution */
    /* ARM Cortex-M stack frame (pushed by hardware on exception entry) */
    *--stack_ptr = OSPORT_INITIAL_XPSR;                 /* xPSR */
    *--stack_ptr = (uint32_t)TaskFunc;                  /* PC (Return Address) */
    *--stack_ptr = (uint32_t)OsExec_TaskReturnGuard;    /* LR */
    *--stack_ptr = 0x12121212UL;                        /* R12 */
    *--stack_ptr = 0x03030303UL;                        /* R3 */
    *--stack_ptr = 0x02020202UL;                        /* R2 */
    *--stack_ptr = 0x01010101UL;                        /* R1 */
    *--stack_ptr = (uint32_t)TaskID;                    /* R0 (Task parameter) */

    /* ARM Cortex-M additional registers (pushed by software in context switch) */
    *--stack_ptr = 0x11111111UL;                        /* R11 */
    *--stack_ptr = 0x10101010UL;                        /* R10 */
    *--stack_ptr = 0x09090909UL;                        /* R9 */
    *--stack_ptr = 0x08080808UL;                        /* R8 */
    *--stack_ptr = 0x07070707UL;                        /* R7 */
    *--stack_ptr = 0x06060606UL;                        /* R6 */
    *--stack_ptr = 0x05050505UL;                        /* R5 */
    *--stack_ptr = 0x04040404UL;                        /* R4 */

    return (uint8_t *)stack_ptr;
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
    uint32_t tick_count;
    uint32_t tick_value;

    /* Read SysTick current value and counter atomically */
    do {
        tick_count = OsPort_SystemTickCounter;
        tick_value = SysTick->VAL;
    } while (tick_count != OsPort_SystemTickCounter);

    /* Calculate microseconds */
    /* SysTick counts down from LOAD to 0 */
    uint32_t elapsed_ticks = SysTick->LOAD - tick_value;
    uint32_t microseconds = (tick_count * 1000) +
                           (elapsed_ticks * 1000) / SysTick->LOAD;

    return microseconds;
}

/*
**  Initialize SysTick timer for 1ms ticks.
*/
void OsPort_InitSysTick(void)
{
    uint32_t system_core_clock = HAL_RCC_GetSysClockFreq();

    /* Configure SysTick for 1ms interrupts */
    SysTick->LOAD = (system_core_clock / OSPORT_SYSTICK_FREQUENCY_HZ) - 1UL;
    SysTick->VAL = 0UL;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_TICKINT_Msk |
                    SysTick_CTRL_ENABLE_Msk;

    /* Set SysTick priority */
    NVIC_SetPriority(SysTick_IRQn, OSPORT_SYSTICK_PRIORITY);
}

/*
**  Initialize PendSV for context switching.
*/
void OsPort_InitPendSV(void)
{
    /* Set PendSV to lowest priority */
    NVIC_SetPriority(PendSV_IRQn, OSPORT_PENDSV_PRIORITY);
}

/*
**  Trigger a context switch by setting PendSV.
*/
void OsPort_TriggerContextSwitch(void)
{
    /* Set PendSV interrupt pending bit */
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;

    /* Ensure write completes before returning */
    __DSB();
    __ISB();
}

/*
**  Get system tick counter.
*/
uint32_t OsPort_GetSystemTick(void)
{
    return OsPort_SystemTickCounter;
}

/*
**  Delay in milliseconds.
*/
void OsPort_DelayMs(uint32_t delay)
{
    uint32_t start_tick = OsPort_SystemTickCounter;

    while ((OsPort_SystemTickCounter - start_tick) < delay) {
        __WFI(); /* Wait for interrupt */
    }
}

/*
**  Additional port functions for OSEK compatibility.
*/
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_StartCurrentTask(void)
#else
void OsPort_StartCurrentTask(void)
#endif /* KOS_MEMORY_MAPPING */
{
    OsPort_TriggerContextSwitch();
}

#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_SaveContext(void)
#else
void OsPort_SaveContext(void)
#endif /* KOS_MEMORY_MAPPING */
{
    /* Context saving is handled by hardware and PendSV handler */
}

#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_RestoreContext(void)
#else
void OsPort_RestoreContext(void)
#endif /* KOS_MEMORY_MAPPING */
{
    /* Context restoration is handled by hardware and PendSV handler */
}

#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_SwitchToISRContext(void)
#else
void OsPort_SwitchToISRContext(void)
#endif /* KOS_MEMORY_MAPPING */
{
    /* Switch to MSP (Main Stack Pointer) */
    __set_CONTROL(__get_CONTROL() & ~0x02);
    __ISB();
}

#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_CreateEvents(void)
#else
void OsPort_CreateEvents(void)
#endif /* KOS_MEMORY_MAPPING */
{
    /* Events are handled through the scheduler */
}

#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_SignalSchedulingEvent(void)
#else
void OsPort_SignalSchedulingEvent(void)
#endif /* KOS_MEMORY_MAPPING */
{
    OsPort_TriggerContextSwitch();
}

#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_AdjustProcessPriorityClass(void)
#else
void OsPort_AdjustProcessPriorityClass(void)
#endif /* KOS_MEMORY_MAPPING */
{
    /* Not applicable for bare-metal STM32 */
}

#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_KeyboardLoop(void)
#else
void OsPort_KeyboardLoop(void)
#endif /* KOS_MEMORY_MAPPING */
{
    /* Not applicable for STM32 without console */
}

/*
**  Task suspension hook.
*/
void OsPort_SuspendHook(uint16_t taskID)
{
    /* Optional hook for task suspension */
    (void)taskID; /* Unused parameter */
    __WFI(); /* Wait for interrupt to save power */
}

/*
**  Interrupt Handlers
*/

/*
**  SysTick interrupt handler.
*/
void SysTick_Handler(void)
{
    /* Increment system tick counter */
    OsPort_SystemTickCounter++;

    #if defined(OS_FEATURE_INSTALL_RTI_HANDLER)
    /* Call the RTI (Real Time Interrupt) handler */
    OS_RTI_DRIVER_IMPL();
    #endif

    /* Trigger scheduler if needed */
    /* This depends on your specific OSEK implementation */
}

/*
**  PendSV interrupt handler for context switching.
*/
__attribute__((naked)) void PendSV_Handler(void)
{
    __asm volatile (
        "mrs r0, psp                    \n\t"
        "isb                            \n\t"

        /* Save current task context */
        "stmdb r0!, {r4-r11}            \n\t"

        /* Save current stack pointer */
        "ldr r1, =Os_CurrentTask        \n\t"
        "ldr r2, [r1]                   \n\t"
        "ldr r3, =Os_TaskStackPointer   \n\t"
        "str r0, [r3, r2, lsl #2]       \n\t"

        /* Load new task context */
        /* This would call the scheduler to get next task */
        "bl OsExec_GetHighestReadyTask  \n\t"
        "str r0, [r1]                   \n\t"

        /* Load new stack pointer */
        "ldr r0, [r3, r0, lsl #2]       \n\t"

        /* Restore new task context */
        "ldmia r0!, {r4-r11}            \n\t"

        "msr psp, r0                    \n\t"
        "isb                            \n\t"

        /* Return to thread mode with PSP */
        "bx lr                          \n\t"
    );
}

/*
**  Hard Fault handler for debugging.
*/
void HardFault_Handler(void)
{
    /* Infinite loop for debugging */
    while (1) {
        __NOP();
    }
}

#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_OS_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */