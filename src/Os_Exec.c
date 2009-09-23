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

#include "Osek.h"

static void OsExec_Init(void);

#if defined(OS_SAVE_STARTUP_CTX)
static Utl_JumpBufType StartupContext;
#endif  /* OS_SAVE_STARTUP_CTX */


static AppModeType Os_AppMode;


void StartOS(AppModeType Mode)
{               
    SAVE_SERVICE_CONTEXT(OSServiceId_StartOS,Mode,NULL,NULL);

    ASSERT_OS_NOT_RUNNING();

    if (Mode>(AppModeType)7) {
        Os_AppMode=OSDEFAULTAPPMODE;
    } else {
        Os_AppMode=Mode;
    }

/*    DISABLE_ALL_OS_INTERRUPTS(); */

    OsPortInit();
    
    OsExec_Init();

#if defined(OS_USE_STARTUPHOOK)
    OS_SET_CALLEVEL(CL_STARTUP_HOOK);
    StartupHook();
#endif  /* OS_USE_STARTUPHOOK */
    OS_SET_CALLEVEL(OS_CL_TASK);
    OS_SET_TASK_LEVEL();

    /* check: das nächste zur Sicherheit???*/
    OS_SET_HIGHEST_PRIO_RUNNING();   
    
    CLEAR_SERVICE_CONTEXT();
/*    ENABLE_ALL_OS_INTERRUPTS(); */
    
#if defined(OS_SAVE_STARTUP_CTX)
    if (Utl_SetJump(&StartupContext)==0) {
#endif  /* OS_SAVE_STARTUP_CTX */

        OS_START_CURRENT_TASK();
                
#if defined(OS_SAVE_STARTUP_CTX)
    }
#endif  /* OS_SAVE_STARTUP_CTX */
}

void ShutdownOS(StatusType Error)
{
    uint8_least i;
    
    SAVE_SERVICE_CONTEXT(OSServiceId_ShutdownOS,Error,NULL,NULL);
    CPU_DISABLE_ALL_INTERRUPTS();
                
    for (i=(uint8_least)0;i<OS_NUM_TASKS;++i) {
        OS_TCB[i].State=SUSPENDED;
    }   
        
#if defined(OS_USE_SHUTDOWNHOOK)
    OS_SET_CALLEVEL(CL_SHUTDOWN_HOOK);
    ShutdownHook(Error);
#else   /* OS_USE_SHUTDOWNHOOK */
    UNREFERENCED_PARAMETER(Error);
#endif          
    OS_SET_CALLEVEL(OS_CL_INVALID);
    CLEAR_SERVICE_CONTEXT();    /* ??? */
    
#if defined(OS_SAVE_STARTUP_CTX)
    Utl_LongJump(&StartupContext,-1);
#endif  /* OS_SAVE_STARTUP_CTX */
/* 
**  !REQ!AS!OS425!
**  If ShutdownOS() is called and ShutdownHook() returns then the operating 
**  system shall disable all interrupts and enter an endless loop.
*/
    FOREVER {   

    }
}


AppModeType GetActiveApplicationMode(void)
{
    SAVE_SERVICE_CONTEXT(OSServiceId_GetActiveApplicationMode,NULL,NULL,NULL);

#if 0    
    ASSERT_VALID_CALLEVEL(CL_TASK|CL_ISR2|CL_ERROR_HOOK|CL_PRE_TASK_HOOK|
                          CL_POST_TASK_HOOK|CL_STARTUP_HOOK|CL_SHUTDOWN_HOOK);
#endif
    CLEAR_SERVICE_CONTEXT();
    return Os_AppMode;
}


static void OsExec_Init(void)
{
    OsCurrentTID=(INVALID_TASK);
    OsCurrentTCB=(OsTCBType*)NULL;
    OsFlags=(uint8)0x00;
        
    OsMLQ_Init();

    OsIntr_InitInterrupts();
    
    OsTask_InitTasks();
    
    OsRes_InitResources();
    
    OsAlm_InitAlarms();
    
    OsCtr_InitCounters();

}


TASK(OsExec_IdleTask)
{
    FOREVER {
        WAIT_FOR_READY_TASKS();
#if defined(OS_SCHEDULE_NON) || defined(OS_SCHEDULE_MIX)
        (void)Schedule();
#endif  /* OS_SCHEDULE_NON || OS_SCHEDULE_MIX */
    }
}


void OsExec_TaskReturnGuard(void)
{

}


#if defined(OS_SCHED_POLICY_NON)
void OsExec_ScheduleFromISR(void) {}

#elif defined(OS_SCHED_POLICY_PRE) || defined(OS_SCHED_POLICY_MIX)
void OsExec_ScheduleFromISR(void)   /*  ISR-Level-Scheduling. */
{
#if defined(OS_SCHED_POLICY_MIX)  
    if (OS_IS_TASK_PREEMPTABLE(OsCurrentTID) && !OS_IS_SCHEDULER_LOCKED()) {
#else
    if (!OS_IS_SCHEDULER_LOCKED()) {
#endif  /* OS_SCHED_POLICY_NON */
        OS_CALL_POST_TASK_HOOK();
        
        if (!OS_IS_TASK_WAITING(OsCurrentTID)) {
            OsCurrentTCB->State=READY;
        }
        OS_SET_HIGHEST_PRIO_RUNNING();
    }
#endif
}


boolean OsExec_HigherPriorityThenCurrentReady(void)
{    
    uint16 tm,t_res;
    boolean res;
    static const uint16 inv_exp_tab[16]={
        (uint16)0x0001,(uint16)0x0002,(uint16)0x0004,(uint16)0x0008,
        (uint16)0x0010,(uint16)0x0020,(uint16)0x0040,(uint16)0x0080,
        (uint16)0x0100,(uint16)0x0200,(uint16)0x0400,(uint16)0x0800,
        (uint16)0x1000,(uint16)0x2000,(uint16)0x4000,(uint16)0x8000            
    };    
    
    tm=inv_exp_tab[OsCurrentTCB->CurrentPriority];
    
    t_res=OsMLQ_GetBitmap() & (~tm);   /* OK? */
    res=t_res>tm;
    
    return res;

}


void OsExec_StartHighestReadyTask(void)
{
    OS_SET_HIGHEST_PRIO_RUNNING();

    OS_START_CURRENT_TASK();
}


ISR1(SWI_Vector)
{
    OSEnterISR();
    OSLeaveISR();
}
