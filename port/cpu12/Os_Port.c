/*
** k_os (Konnex Operating-System based on the OSEK/VDX-Standard).
**
** (C) 2007-2014 by Christoph Schueler <github.com/Christoph2,
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

#include "Hw_Cfg.h"
#include "kdk/mcu/s12/S12_Ect.h"

#if defined(CPU12_S12)
#define PORT_ACKNOWLEDGE_MDCU_INTR()    S12ECT_ACKNOWLEDGE_MDCU_INTR()
#define PORT_ACKNOWLEDGE_RTI_INTR()     S12CRG_ACKNOWLEDGE_RTI_INTR()
#define PORT_ACKNOWLEDGE_TC0_INTR()     S12ECT_ACKNOWLEDGE_TC0_INTR()
#define PORT_ACKNOWLEDGE_TC1_INTR()     S12ECT_ACKNOWLEDGE_TC1_INTR()
#define PORT_ACKNOWLEDGE_TC2_INTR()     S12ECT_ACKNOWLEDGE_TC2_INTR()
#define PORT_ACKNOWLEDGE_TC3_INTR()     S12ECT_ACKNOWLEDGE_TC3_INTR()
#define PORT_ACKNOWLEDGE_TC4_INTR()     S12ECT_ACKNOWLEDGE_TC4_INTR()
#define PORT_ACKNOWLEDGE_TC5_INTR()     S12ECT_ACKNOWLEDGE_TC5_INTR()
#define PORT_ACKNOWLEDGE_TC6_INTR()     S12ECT_ACKNOWLEDGE_TC6_INTR()
#define PORT_ACKNOWLEDGE_TC7_INTR()     S12ECT_ACKNOWLEDGE_TC7_INTR()
#elif defined(CPU12_HC12)
#define PORT_ACKNOWLEDGE_MDCU_INTR()    HC12ECT_ACKNOWLEDGE_MDCU_INTR()
#define PORT_ACKNOWLEDGE_RTI_INTR()     HC12CLK_ACKNOWLEDGE_RTI_INTR()
#define PORT_ACKNOWLEDGE_TC0_INTR()     HC12ECT_ACKNOWLEDGE_TC0_INTR()
#define PORT_ACKNOWLEDGE_TC1_INTR()     HC12ECT_ACKNOWLEDGE_TC1_INTR()
#define PORT_ACKNOWLEDGE_TC2_INTR()     HC12ECT_ACKNOWLEDGE_TC2_INTR()
#define PORT_ACKNOWLEDGE_TC3_INTR()     HC12ECT_ACKNOWLEDGE_TC3_INTR()
#define PORT_ACKNOWLEDGE_TC4_INTR()     HC12ECT_ACKNOWLEDGE_TC4_INTR()
#define PORT_ACKNOWLEDGE_TC5_INTR()     HC12ECT_ACKNOWLEDGE_TC5_INTR()
#define PORT_ACKNOWLEDGE_TC6_INTR()     HC12ECT_ACKNOWLEDGE_TC6_INTR()
#define PORT_ACKNOWLEDGE_TC7_INTR()     HC12ECT_ACKNOWLEDGE_TC7_INTR()
#elif defined(CPU12_S12X)
#error There is currently no support for the CPU12X family.
#endif

const SizeType OS_TOS_ISR = (SizeType)((const uint8 *)&ISR_Stack + ISR_STACK_SIZE);

static const S12Ect_ConfigType Cfg = {BASE_ADDR_ECT};

InterruptStateType OsPort_InterruptState;

#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_OS_START_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */

#if 0
static const OsPort_InitialStackData[] = {
    ((uint16)0xffee),   /*  Y */
    ((uint16)0xddcc),   /*  X */
    ((uint16)0xaabb),   /*  D  */
    ((uint16)0xc0),     /*  CCR */
};
#endif

#if 0
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(uint8 *, OSEK_OS_CODE) OsPort_TaskStackInit(TaskType TaskID,
    P2VAR(TaskFunctionType, AUTOMATIC, OSEK_OS_APPL_DATA) TaskFunc,
    P2VAR(uint8, AUTOMATIC,  OSEK_OS_APPL_DATA) sp,
    boolean Reschedule
)
#else
uint8 * OsPort_TaskStackInit(TaskType TaskID, TaskFunctionType * TaskFunc, uint8 * sp, boolean Reschedule)
#endif /* KOS_MEMORY_MAPPING */
{
    /* Return-Adresse / for  E_MISSING_END  */
#if 0
    *sp--  = LOBYTE((uint16)(uint8 *)OsExec_TaskReturnGuard); /* StackGuardFunction */
    *sp--  = HIBYTE((uint16)(uint8 *)OsExec_TaskReturnGuard);
#endif

/*  *(uint16*)sp--=(uint16)(uint16*)TaskFunc; */

    *sp--  = LOBYTE((SizeType) * TaskFunc);
    *sp--  = HIBYTE((SizeType) * TaskFunc);
    *sp--  = ((uint8)0xff); /*  Y/Lo  */
    *sp--  = ((uint8)0xee); /*  Y/Hi  */
    *sp--  = ((uint8)0xdd); /*  X/Lo  */
    *sp--  = ((uint8)0xcc); /*  X/Hi  */
    *sp--  = ((uint8)0xaa); /*  AccA  */
    *sp--  = ((uint8)0xbb); /*  AccB  */
    *sp--  = ((uint8)0xc0); /*  CCR/Interrupts enabled.     (Parameter: XIRQ-Enable)  */
    ++sp;                   /* 57 Cycles */

#if 0
    *(uint16 *)sp--    = (uint16)(uint16 *)TaskFunc;
    *sp--              = ((uint16)0xeeff);
    *sp--              = ((uint16)0xccdd);
    *sp--              = ((uint16)0xbbaa);
    *(uint8 *)sp       = (uint8)0xc0;
#endif

    /* 0x10e0   00 00 00 00 c0 bb aa cc dd ee ff 4d e4 */
/*    OS_TCB[TaskID].Stackpointer=sp; */

    if (Reschedule) {
        OsExec_StartHighestReadyTask();
    }

    return sp;
}
#endif

#define TICKS_PER_MS ((uint16)1000u)

#if 0
static const S12Ect_ConfigType ECT_CFG = {
    (uint16)TICKS_PER_MS,           /* cycle - (od. Period/TimeBase???) in Nano-Secs. */
    ((uint8)IOS0),                  /* TIos     */
    TEN | TSWAI | TSFRZ,            /* TScr1    */
    /*TOI|*/ PR0 | PR1,             /* TScr2    */
    ((uint8)0x00),                  /* TCtl1    */
    ((uint8)0x00),                  /* TCtl2    */
    ((uint8)0x00),                  /* TCtl3    */
    ((uint8)0x00),                  /* TCtl4    */
    ((uint8)0x00),                  /* TIe Interrupt-Freigabe je nach Cfg. */
    PAEN | PAOVI | PEDGE,           /* PActl    */
    PBEN | PBOVI,                   /* PBctl    */
    ((uint8)0x00),                  /* ICpar    */
    MCZI | MODMC | MCEN | MCPR1,    /* MCctl    */
    TICKS_PER_MS,                   /* MCcnt    */
    ((uint8)0x00),                  /* DLyct    */
    ((uint8)0x00),                  /* ICovw    */
    ((uint8)0x00),                  /* ICsys    */
};
#endif


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_Init(void)
#else
void OsPort_Init(void)
#endif /* KOS_MEMORY_MAPPING */
{
#if 0
    BYTE_REG(Cfg.BaseAddr, TSCR1)  = (uint8)0x00;
    BYTE_REG(Cfg.BaseAddr, TIOS)   = IOS0;
    BYTE_REG(Cfg.BaseAddr, TCTL1)  = (uint8)0x00;
    BYTE_REG(Cfg.BaseAddr, TCTL2)  = (uint8)0x00;
    BYTE_REG(Cfg.BaseAddr, TSCR2)  = /*TOI|*/ PR0 | PR1;
    BYTE_REG(Cfg.BaseAddr, TFLG1)  = (uint8)0xFF;
    BYTE_REG(Cfg.BaseAddr, TSCR1)  = TEN | TSFRZ /*|TFFCA*/;
    BYTE_REG(Cfg.BaseAddr, MCCTL)  = MCZI | MODMC | MCEN | MCPR1;
    WORD_REG(Cfg.BaseAddr, MCCNT)  = TICKS_PER_MS;
    BYTE_REG(Cfg.BaseAddr, MCCTL) |= FLMC;
#endif
    S12Ect_Init(&ECT_CFG);
}


/* todo: nach Os_Port_Irq !? */
#if defined(OS_FEATURE_INSTALL_MDCU_HANDLER)
ISR(MDCUTimer)
{
    PORT_ACKNOWLEDGE_MDCU_INTR();
    OS_MDCU_DRIVER_IMPL();
}
#endif /* OS_FEATURE_INSTALL_MDCU_HANDLER */

#if defined(OS_FEATURE_INSTALL_RTI_HANDLER)
ISR(RTITimer)
{
    PORT_ACKNOWLEDGE_RTI_INTR();
    OS_RTI_DRIVER_IMPL();
}
#endif /* OS_FEATURE_INSTALL_RTI_HANDLE */

#if defined(OS_FEATURE_INSTALL_TC0_HANDLER)
ISR(TC0Timer)
{
    PORT_ACKNOWLEDGE_TC0_INTR();
    OS_TC0_DRIVER_IMPL();
}
#endif /* OS_FEATURE_INSTALL_TC0_HANDLER */

#if defined(OS_FEATURE_INSTALL_TC1_HANDLER)
ISR(TC1Timer)
{
    PORT_ACKNOWLEDGE_TC1_INTR();
    OS_TC1_DRIVER_IMPL();
}
#endif /* OS_FEATURE_INSTALL_TC1_HANDLER */

#if defined(OS_FEATURE_INSTALL_TC2_HANDLER)
ISR(TC2Timer)
{
    PORT_ACKNOWLEDGE_TC2_INTR();
    //OS_TC2_DRIVER_IMPL();
}
#endif /* OS_FEATURE_INSTALL_TC2_HANDLER */

#if defined(OS_FEATURE_INSTALL_TC3_HANDLER)
ISR(TC3Timer)
{
    PORT_ACKNOWLEDGE_TC3_INTR();
    OS_TC3_DRIVER_IMPL();
}
#endif /* OS_FEATURE_INSTALL_TC3_HANDLER */

#if defined(OS_FEATURE_INSTALL_TC4_HANDLER)
ISR(TC4Timer)
{
    PORT_ACKNOWLEDGE_TC4_INTR();
    OS_TC4_DRIVER_IMPL();
}
#endif /* OS_FEATURE_INSTALL_TC4_HANDLER */

#if defined(OS_FEATURE_INSTALL_TC5_HANDLER)
ISR(TC5Timer)
{
    PORT_ACKNOWLEDGE_TC5_INTR();
    OS_TC5_DRIVER_IMPL();
}
#endif /* OS_FEATURE_INSTALL_TC5_HANDLER */

#if defined(OS_FEATURE_INSTALL_TC6_HANDLER)
ISR(TC6Timer)
{
    PORT_ACKNOWLEDGE_TC6_INTR();
    OS_TC6_DRIVER_IMPL();
}
#endif /* OS_FEATURE_INSTALL_TC6_HANDLER */

#if defined(OS_FEATURE_INSTALL_TC7_HANDLER)
ISR(TC7Timer)
{
    PORT_ACKNOWLEDGE_TC7_INTR();
    OS_TC7_DRIVER_IMPL();
}
#endif /* OS_FEATURE_INSTALL_TC7_HANDLER */


#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_OS_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */
