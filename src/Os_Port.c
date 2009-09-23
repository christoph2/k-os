/*
 * k_os (Konnex Operating-System based on the OSEK/VDX-Standard).
 *
 * (C) 2007-2009 by Christoph Schueler <chris@konnex-tools.de>
 *
 * All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "Os_Port.h"

#define TICKS_PER_MS        ((TickType)1000)

const SizeType OS_TOS_ISR=(SizeType)((const uint8*)&ISR_Stk+ISR_STACK_SIZE);

#if 0
#define OS_TOS_ISR  ((uint16)((uint8*)&ISR_Stk+ISR_STACK_SIZE))
#endif


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


uint32 OsPortGetTimestamp(void)
{
    return TCNT;   
}


ISR(OSSystemTimerVector)
{
    BYTE_REG(Cfg.BaseAddr,MCFLG)=MCZF;
    (void)IncrementCounter(SystemCounter);
}
