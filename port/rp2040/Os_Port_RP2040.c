/*
** k_os (Konnex Operating-System based on the OSEK/VDX-Standard).
**
** (C) 2007-2026 by Christoph Schueler <github.com/Christoph2,
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
extern void OsExec_TaskReturnGuard(void);

/* Global variables */
InterruptStateType OsPort_InterruptState = 0;
volatile uint32_t OsPort_SystemTickCounter = 0;

/* Stack for ISR context (provided by generated config) */
extern uint8_t ISR_Stack[ISR_STACK_SIZE];
const SizeType OS_TOS_ISR = (SizeType)((const uint8_t *)&ISR_Stack + ISR_STACK_SIZE);

/* External references */
extern TaskType Os_CurrentTask;
extern const uint8_t * Os_TaskStackPointer[];
extern TaskType OsExec_GetHighestReadyTask(void);

#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_OS_START_SEC_CODE
    #include "MemMap.h"
#endif

/* Initialize the RP2040 port */
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_Init(void)
#else
void OsPort_Init(void)
#endif
{
    /* Initialize system tick timer */
    OsPort_InitSysTick();

    /* Initialize PendSV for context switching */
    OsPort_InitPendSV();

    /* Enable fault handlers (usage/bus/mem) for easier debugging on Cortex-M) */
    volatile uint32_t *shcsr = (volatile uint32_t *)0xE000ED24;
    *shcsr |= (1UL << 18) | (1UL << 17) | (1UL << 16);

    /* Set thread mode to use PSP */
    /* M0+ CONTROL register: bit 1 (SPSEL) = 1 (PSP) */
    uint32_t control;
    __asm volatile ("mrs %0, control" : "=r" (control));
    control |= 0x02;
    __asm volatile ("msr control, %0" :: "r" (control) : "memory");
    __asm volatile ("isb" ::: "memory");

    /* Initialize PSP with ISR stack or initial stack */
    __asm volatile ("msr psp, %0" :: "r" (OS_TOS_ISR) : "memory");
}

#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_Shutdown(void)
#else
void OsPort_Shutdown(void)
#endif
{
    /* Disable SysTick */
    /* In RP2040, we might need to access registers directly if no SDK is used */
    /* SYST_CSR = 0xE000E010 */
    volatile uint32_t *syst_csr = (volatile uint32_t *)0xE000E010;
    *syst_csr = 0;
}

#if KOS_MEMORY_MAPPING == STD_ON
FUNC(uint8_t *, OSEK_OS_CODE) OsPort_TaskStackInit(
    TaskType TaskID,
    P2VAR(Os_TaskFunctionType, AUTOMATIC, OSEK_OS_APPL_DATA) TaskFunc,
    P2VAR(uint8_t, AUTOMATIC, OSEK_OS_APPL_DATA) sp
)
#else
uint8_t * OsPort_TaskStackInit(TaskType TaskID, Os_TaskFunctionType const * TaskFunc, uint8_t * sp)
#endif
{
    uint32_t *stack_ptr = (uint32_t *)sp;

    /* Ensure stack is 8-byte aligned */
    stack_ptr = (uint32_t *)((uint32_t)stack_ptr & ~0x7UL);

    /* xPSR */
    *--stack_ptr = OSPORT_INITIAL_XPSR;
    /* PC */
    *--stack_ptr = (uint32_t)TaskFunc;
    /* LR */
    *--stack_ptr = (uint32_t)OsExec_TaskReturnGuard;
    /* R12, R3, R2, R1 */
    *--stack_ptr = 0x12121212UL;
    *--stack_ptr = 0x03030303UL;
    *--stack_ptr = 0x02020202UL;
    *--stack_ptr = 0x01010101UL;
    /* R0 */
    *--stack_ptr = (uint32_t)TaskID;

    /* R11, R10, R9, R8, R7, R6, R5, R4 (software saved) */
    *--stack_ptr = 0x11111111UL;
    *--stack_ptr = 0x10101010UL;
    *--stack_ptr = 0x09090909UL;
    *--stack_ptr = 0x08080808UL;
    *--stack_ptr = 0x07070707UL;
    *--stack_ptr = 0x06060606UL;
    *--stack_ptr = 0x05050505UL;
    *--stack_ptr = 0x04040404UL;

    return (uint8_t *)stack_ptr;
}

#if KOS_MEMORY_MAPPING == STD_ON
FUNC(uint32_t, OSEK_OS_CODE) OsPort_GetTimestamp(void)
#else
uint32_t OsPort_GetTimestamp(void)
#endif
{
    /* Simple millisecond based timestamp if no high-res timer is configured */
    return OsPort_SystemTickCounter * 1000;
}

void OsPort_InitSysTick(void)
{
    uint32_t reload = 0;
    if (OSPORT_SYSTICK_FREQUENCY_HZ > 0U) {
        uint64_t tmp = ((uint64_t)OSPORT_SYSCLK_HZ / (uint64_t)OSPORT_SYSTICK_FREQUENCY_HZ);
        if (tmp > 0U) {
            if (tmp > 0xFFFFFFULL) {
                tmp = 0xFFFFFFULL; /* clamp to 24-bit SysTick */
            }
            reload = (uint32_t)(tmp - 1U);
        }
    }

    SysTick->LOAD = reload;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;

    NVIC_SetPriority(SysTick_IRQn, OSPORT_SYSTICK_PRIORITY);
}

void OsPort_InitPendSV(void)
{
    NVIC_SetPriority(PendSV_IRQn, OSPORT_PENDSV_PRIORITY);
}

void OsPort_TriggerContextSwitch(void)
{
    volatile uint32_t *icsr = (volatile uint32_t *)0xE000ED04;
    *icsr = (1UL << 28);
    __asm volatile ("dsb" ::: "memory");
    __asm volatile ("isb" ::: "memory");
}

uint32_t OsPort_GetSystemTick(void)
{
    return OsPort_SystemTickCounter;
}

void OsPort_DelayMs(uint32_t delay)
{
    uint32_t start_tick = OsPort_SystemTickCounter;
    while ((OsPort_SystemTickCounter - start_tick) < delay) {
        __asm volatile ("wfi");
    }
}

#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_StartCurrentTask(void)
#else
void OsPort_StartCurrentTask(void)
#endif
{
    OsPort_TriggerContextSwitch();
}

#if defined(PICO_RP2350)
/* TODO: Save/restore FPU context when hard-float is enabled (RP2350). */
#endif

#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_SaveContext(void)
#else
void OsPort_SaveContext(void)
#endif
{
}

#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_RestoreContext(void)
#else
void OsPort_RestoreContext(void)
#endif
{
}

#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_SwitchToISRContext(void)
#else
void OsPort_SwitchToISRContext(void)
#endif
{
    uint32_t control;
    __asm volatile ("mrs %0, control" : "=r" (control));
    control &= ~0x02;
    __asm volatile ("msr control, %0" :: "r" (control) : "memory");
    __asm volatile ("isb" ::: "memory");
}

#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_CreateEvents(void)
#else
void OsPort_CreateEvents(void)
#endif
{
}

#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_SignalSchedulingEvent(void)
#else
void OsPort_SignalSchedulingEvent(void)
#endif
{
    OsPort_TriggerContextSwitch();
}

#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_AdjustProcessPriorityClass(void)
#else
void OsPort_AdjustProcessPriorityClass(void)
#endif
{
}

#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_KeyboardLoop(void)
#else
void OsPort_KeyboardLoop(void)
#endif
{
}

void OsPort_SuspendHook(uint16_t taskID)
{
    (void)taskID;
    __asm volatile ("wfi");
}

void SysTick_Handler(void)
{
    OsPort_SystemTickCounter++;
    #if defined(OS_FEATURE_INSTALL_RTI_HANDLER)
    OS_RTI_DRIVER_IMPL();
    #endif
}

__attribute__((naked)) void PendSV_Handler(void)
{
    __asm volatile (
        ".syntax unified                \n\t"
        ".thumb                         \n\t"
        "mrs r0, psp                    \n\t" /* r0 = current PSP */
        "isb                            \n\t"

        /* Make room and save r4-r7 */
        "subs r0, #16                   \n\t"
        "subs r0, #16                   \n\t"
        "stmia r0!, {r4-r7}             \n\t"

        /* Save r8-r11 using low registers */
        "mov r4, r8                     \n\t"
        "mov r5, r9                     \n\t"
        "mov r6, r10                    \n\t"
        "mov r7, r11                    \n\t"
        "stmia r0!, {r4-r7}             \n\t"

        /* r0 now points past saved regs; step back to base */
        "subs r0, #16                   \n\t"
        "subs r0, #16                   \n\t"

        /* Store current PSP */
        "ldr r1, =Os_CurrentTask        \n\t"
        "ldr r2, [r1]                   \n\t"
        "ldr r3, =Os_TaskStackPointer   \n\t"
        "lsls r2, r2, #2                \n\t"
        "add  r3, r3, r2                \n\t"
        "str  r0, [r3]                  \n\t"

        /* Select next task */
        "bl OsExec_GetHighestReadyTask  \n\t"
        "str r0, [r1]                   \n\t"

        /* Load next PSP */
        "ldr r3, =Os_TaskStackPointer   \n\t"
        "lsls r0, r0, #2                \n\t"
        "add  r3, r3, r0                \n\t"
        "ldr  r0, [r3]                  \n\t"

        /* Restore r8-r11 */
        "ldmia r0!, {r4-r7}             \n\t"
        "mov r8, r4                     \n\t"
        "mov r9, r5                     \n\t"
        "mov r10, r6                    \n\t"
        "mov r11, r7                    \n\t"

        /* Restore r4-r7 */
        "ldmia r0!, {r4-r7}             \n\t"

        /* Update PSP and return */
        "msr psp, r0                    \n\t"
        "isb                            \n\t"
        "bx lr                          \n\t"
    );
}

void HardFault_Handler(void)
{
    while (1) {
        __asm volatile ("nop");
    }
}

#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_OS_STOP_SEC_CODE
    #include "MemMap.h"
#endif
