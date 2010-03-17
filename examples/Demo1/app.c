/*
   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).

   (C) 2007-2010 by Christoph Schueler <chris@konnex-tools.de,
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
 
/*
**
**  todo; Modulo-Divisionen 'N' durch durch AND 'N'-1 ersetzen!!!
**
*/

#include "Osek.h"
/* clude "com.h" */

#include "S12_Fls.h"
#include "S12_Eep.h"
#include "S12_Iic.h"
#include "S12_Atd.h"
#include "S12_Crg.h"

#include "Com.h"

#include "Mem.h"


/* STATIC_ASSERT(FALSE,"Statische Assertion!!!"); */

/* #pragma message("Hello") */

/*__IAR_SYSTEMS_ICC__ */

#if 0
void test(void);
void foo(int *pi);

void foo(int const * const pi)
{
    *pi*=4;    
}

void test(void);
void test(void)
{
    int apple,p;
    
    p=1;
    p=p++ + ++p;
    
    /*    _=_+++++_; */ /* check: die Sache mit der Greediness. */    
    p=3;
    
    apple=sizeof(int) * p;
}
#endif

#if  0
void test(void);

void test(void)
{
    char *  p;
    
    p="hello world!";
    p[0]='H';
    p[6]='W';
    (char*){"pello world!!!"}[0]='H';
}
#endif


int main(void)
{   
#if 0
    Utl_DivremType dt;
    void *FreePtr;    
    
    FreePtr=Mem_SBrk((SizeType)0x0100);    
    FreePtr=Mem_SBrk((SizeType)0x0050);            
    (void)S12Crg_Init((uint8)8);
    Utl_Divrem(100,13,&dt);
    
    test();    
#endif    
    
    SCI_Init(SCI0);
    SCI_SetFormat(SCI0,115200,SCI_PARITY_EVEN,8);
    SCI_SetFormat(SCI0,2400,SCI_PARITY_EVEN,7);    
   
#if 0
    Clock_Init(&CRG,8);

    res=S12Iic_PresenceCheck(&IIC0,0,0xff,NULL);
    
    S12Eep_Init();
    res=S12Eep_SectorModify(0x0d00,0x11112222L);    
#endif

#if 0    
    S12Atd_Init(ATD0);
    S12Atd_Init(ATD1);
#endif    
    
#if 0    
    S12Iic_Init(&IIC0);
    
/*    S12Crg_ResetMCU(); */
    
    res=S12Iic_PresenceCheck(&IIC0,0,0xff,NULL);
    
    while (1) {         /* todo: Das I²C-Teil braucht Referenz-Spannung!!! */
        S12Iic_Start(&IIC0);
        ack=S12Iic_Write(&IIC0,0x58);
        if (ack) {
            S12Iic_Write(&IIC0,0x00);    /* Command-Byte. */
            S12Iic_Write(&IIC0,j);       /* Output-Byte. */
        }
        S12Iic_Stop(&IIC0);
        j++;
/*        adc=ADC_GetChannel(0); */
    }    
#endif
    

#if 0    
    S12Fls_Init();
    res=S12Fls_SectorErase(0x3a,0xb000);
    res=S12Fls_BurstProgram(0x3a,0xb000,fd,10);
#endif
        
    StartOS(OSDEFAULTAPPMODE);

    return 0;
}

static uint32 snd_data=0UL;

static uint32 TimesCalled[2];

TASK(Task1)
{
/*      (void)ActivateTask(Task1);  */
/*    ChainTask(Task1); */    
    EventMaskType evt;

/*    ShutdownOS(E_OK); */
    
#if 0
    (void)StartCOM((COMApplicationModeType)0);
#endif
    
#if 0    
    (void)GetResource((ResourceType)0);
    (void)ReleaseResource((ResourceType)0);
#endif
        
    FOREVER {   

        TimesCalled[0]++;

        ASSERT(!Cpu_InterruptsDisabled());
        
        (void)WaitEvent((EventMaskType)0x08);
        (void)GetEvent(Task1,&evt);
        (void)ClearEvent(evt);
#if 0
        (void)ReceiveMessage(1,&rcv_data);
#endif
    }

}

TASK(Task2)
{
/*    Schedule(); */
    
    FOREVER {
        TimesCalled[1]++;
        
        ASSERT(!Cpu_InterruptsDisabled());
               
        ASSERT((TimesCalled[1]-TimesCalled[0])<2);
#if 0
        (void)SendMessage(0,&snd_data);
#endif
        snd_data++;
        (void)SetEvent(Task1,(EventMaskType)0x08);
/*        (void)ChainTask(Task1);   */
    }

}

ALARMCALLBACK(Alarm1)
{
   static unsigned long cnt=0UL;
   
   cnt++;
}
 
void ErrorHook(StatusType Error)
{
    UNREFERENCED_PARAMETER(Error);
}

void PreTaskHook(void)
{
    TaskType TaskID;
    TaskStateType State;
    
    (void)GetTaskID(&TaskID);
    (void)GetTaskState(TaskID,&State);
}

void PostTaskHook(void)
{
    TaskType TaskID;
    TaskStateType State;
    
    (void)GetTaskID(&TaskID);
    (void)GetTaskState(TaskID,&State);
}

void COMErrorHook(StatusType Error)
{
    UNREFERENCED_PARAMETER(Error);
}
