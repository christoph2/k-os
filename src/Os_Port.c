/*
   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).

 * (C) 2007-2010 by Christoph Schueler <github.com/Christoph2,
 *                                      cpu12.gems@googlemail.com>

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
#include "Os_Port.h"

#define TICKS_PER_MS        ((TickType)1000)

// todo: Anpassungen für den HC12!!!
#define PORT_ACKNOWLEDGE_MDCU_INTR()   S12ECT_ACKNOWLEDGE_MDCU_INTR()
#define PORT_ACKNOWLEDGE_RTI_INTR()    S12CRG_ACKNOWLEDGE_RTI_INTR()
#define PORT_ACKNOWLEDGE_TC0_INTR()    S12ECT_ACKNOWLEDGE_TC0_INTR()
#define PORT_ACKNOWLEDGE_TC1_INTR()    S12ECT_ACKNOWLEDGE_TC1_INTR()
#define PORT_ACKNOWLEDGE_TC2_INTR()    S12ECT_ACKNOWLEDGE_TC2_INTR()
#define PORT_ACKNOWLEDGE_TC3_INTR()    S12ECT_ACKNOWLEDGE_TC3_INTR()
#define PORT_ACKNOWLEDGE_TC4_INTR()    S12ECT_ACKNOWLEDGE_TC4_INTR()
#define PORT_ACKNOWLEDGE_TC5_INTR()    S12ECT_ACKNOWLEDGE_TC5_INTR()
#define PORT_ACKNOWLEDGE_TC6_INTR()    S12ECT_ACKNOWLEDGE_TC6_INTR()
#define PORT_ACKNOWLEDGE_TC7_INTR()    S12ECT_ACKNOWLEDGE_TC7_INTR()

const SizeType OS_TOS_ISR=(SizeType)((const uint8*)&ISR_Stack+ISR_STACK_SIZE);

#if 0
#define OS_TOS_ISR  ((uint16)((uint8*)&ISR_Stack+ISR_STACK_SIZE))
#endif

#define OS_ACKNOWLEDGE_xxxTIMER_INTERRUPT()

static const S12Ect_ConfigType Cfg={BASE_ADDR_ECT};


uint8* OsPortTaskStackInit(TaskType TaskID,TaskFunctionType *TaskFunc,uint8 *sp,boolean Reschedule)
{
    /* Return-Adresse / for  E_MISSING_END  */
#if 0
    *sp--=LOBYTE((uint16)(uint8*)OsExec_TaskReturnGuard);  /* StackGuardFunction */
    *sp--=HIBYTE((uint16)(uint8*)OsExec_TaskReturnGuard);
#endif

    *sp--=LOBYTE((SizeType)*TaskFunc);
    *sp--=HIBYTE((SizeType)*TaskFunc);
    *sp--=((uint8)0xff);     /*  Y/Lo  */
    *sp--=((uint8)0xee);     /*  Y/Hi  */
    *sp--=((uint8)0xdd);     /*  X/Lo  */
    *sp--=((uint8)0xcc);     /*  X/Hi  */
    *sp--=((uint8)0xaa);     /*  AccA  */
    *sp--=((uint8)0xbb);     /*  AccB  */
    *sp--=((uint8)0xc0);     /*  CCR/Interrupts enabled.     (Parameter: XIRQ-Enable)  */
    ++sp;

    if (Reschedule) {
        OS_TCB[TaskID].Stackpointer=sp;
        OsExec_StartHighestReadyTask();
    }

    return sp;
}


void OsPortInit(void)
{
        BYTE_REG(Cfg.BaseAddr,TSCR1)=(uint8)0x00;
        BYTE_REG(Cfg.BaseAddr,TIOS)=IOS0;
        BYTE_REG(Cfg.BaseAddr,TCTL1)=(uint8)0x00;
        BYTE_REG(Cfg.BaseAddr,TCTL2)=(uint8)0x00;
        BYTE_REG(Cfg.BaseAddr,TSCR2)=/*TOI|*/PR0|PR1;
        BYTE_REG(Cfg.BaseAddr,TFLG1)=(uint8)0xFF;
        BYTE_REG(Cfg.BaseAddr,TSCR1)=TEN|TSFRZ/*|TFFCA*/;
        BYTE_REG(Cfg.BaseAddr,MCCTL)=MCZI|MODMC|MCEN|MCPR1;
        WORD_REG(Cfg.BaseAddr,MCCNT)=TICKS_PER_MS/*-1*/;
        BYTE_REG(Cfg.BaseAddr,MCCTL)|=FLMC;
}


// **********************************************************
// **********************************************************
// **********************************************************
/*
*/

#if defined(OS_FEATURE_INSTALL_MDCU_TIMER_HANDLER)
ISR(MDCUTimer)
{
    PORT_ACKNOWLEDGE_MDCU_INTR();
    OS_MDCU_TIMER_DRIVER_IMPL();
}
#endif /* OS_FEATURE_INSTALL_MDCU_TIMER_HANDLER */


#if defined(OS_FEATURE_INSTALL_RTI_TIMER_HANDLER)
ISR(RTITimer)
{
    PORT_ACKNOWLEDGE_RTI_INTR();
    OS_RTI_TIMER_DRIVER_IMPL();
}
#endif /* OS_FEATURE_INSTALL_RTI_TIMER_HANDLE */


#if defined(OS_FEATURE_INSTALL_MDCU_TC0_HANDLER)
ISR(TC0Timer)
{
    PORT_ACKNOWLEDGE_TC0_INTR();
    OS_TC0_TIMER_DRIVER_IMPL();
}
#endif /* OS_FEATURE_INSTALL_MDCU_TC0_HANDLER */


#if defined(OS_FEATURE_INSTALL_MDCU_TC1_HANDLER)
ISR(TC1Timer)
{
    PORT_ACKNOWLEDGE_TC1_INTR();
    OS_TC1_TIMER_DRIVER_IMPL();
}
#endif /* OS_FEATURE_INSTALL_MDCU_TC1_HANDLER */


#if defined(OS_FEATURE_INSTALL_MDCU_TC2_HANDLER)
ISR(TC2Timer)
{
    PORT_ACKNOWLEDGE_TC2_INTR();
    OS_TC2_TIMER_DRIVER_IMPL();
}
#endif /* OS_FEATURE_INSTALL_MDCU_TC2_HANDLER */


#if defined(OS_FEATURE_INSTALL_MDCU_TC3_HANDLER)
ISR(TC3Timer)
{
    PORT_ACKNOWLEDGE_TC3_INTR();
    OS_TC3_TIMER_DRIVER_IMPL();
}
#endif /* OS_FEATURE_INSTALL_MDCU_TC3_HANDLER */


#if defined(OS_FEATURE_INSTALL_MDCU_TC4_HANDLER)
ISR(TC4Timer)
{
    PORT_ACKNOWLEDGE_TC4_INTR();
    OS_TC4_TIMER_DRIVER_IMPL();
}
#endif /* OS_FEATURE_INSTALL_MDCU_TC4_HANDLER */


#if defined(OS_FEATURE_INSTALL_MDCU_TC5_HANDLER)
ISR(TC5Timer)
{
    PORT_ACKNOWLEDGE_TC5_INTR();
    OS_TC5_TIMER_DRIVER_IMPL();
}
#endif /* OS_FEATURE_INSTALL_MDCU_TC5_HANDLER */


#if defined(OS_FEATURE_INSTALL_MDCU_TC6_HANDLER)
ISR(TC6Timer)
{
    PORT_ACKNOWLEDGE_TC6_INTR();
    OS_TC6_TIMER_DRIVER_IMPL();
}
#endif /* OS_FEATURE_INSTALL_MDCU_TC6_HANDLER */


#if defined(OS_FEATURE_INSTALL_MDCU_TC7_HANDLER)
ISR(TC7Timer)
{
    PORT_ACKNOWLEDGE_TC7_INTR();
    OS_TC7_TIMER_DRIVER_IMPL();
}
#endif /* OS_FEATURE_INSTALL_MDCU_TC7_HANDLER */

// **********************************************************
// **********************************************************
// **********************************************************
