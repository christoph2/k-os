/*
   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).

   (C) 2007-2012 by Christoph Schueler <github.com/Christoph2,
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

#include "Os_Port.h"
#include "Os_Cfg.h"

/*////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*                                                                                                      */
/*                  Because of name-clashes we can't include 'windows.h'!                               */
/*                                                                                                      */
/*//////////////////////////////////////////////////////////////////////////////////////////////////////*/

#define FIBER_FLAG_FLOAT_SWITCH 0x1

typedef void (*PFIBER_START_ROUTINE)(void *lpFiberParameter);
typedef PFIBER_START_ROUTINE LPFIBER_START_ROUTINE;    

#define DECLSPEC_IMPORT __declspec(dllimport)
#define WINAPI          __stdcall

DECLSPEC_IMPORT void *  WINAPI CreateFiber(uint32 dwStackSize,LPFIBER_START_ROUTINE lpStartAddress, void * lpParameter);
DECLSPEC_IMPORT void *  WINAPI CreateFiberEx(uint32 dwStackCommitSize, uint32 dwStackReserveSize, uint32 dwFlags,
    LPFIBER_START_ROUTINE lpStartAddress, void * lpParameter
);
DECLSPEC_IMPORT void    WINAPI DeleteFiber(void *lpFiber);
DECLSPEC_IMPORT void *  WINAPI ConvertThreadToFiber(void * lpParameter);
DECLSPEC_IMPORT void *  WINAPI ConvertThreadToFiberEx(void * lpParameter, uint32 dwFlags);
DECLSPEC_IMPORT boolean WINAPI ConvertFiberToThread(void);
DECLSPEC_IMPORT void    WINAPI SwitchToFiber(void *lpFiber);
DECLSPEC_IMPORT boolean WINAPI SwitchToThread(void);
/*///////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////*/


void fiberFunc(void *param);

static const uint8 ReversedLog2Tab[] = {
    0x00,0x08,0x07,0x00,0x06,0x00,0x00,0x00,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x01
};

#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_OS_START_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */


#define TICKS_PER_MS ((uint16)1000u)


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_Init(void)
#else
void OsPort_Init(void)
#endif /* KOS_MEMORY_MAPPING */
{
    void * fp, * main_fiber;

    main_fiber = ConvertThreadToFiber(NULL);
    fp = CreateFiber(0x40, (LPFIBER_START_ROUTINE)fiberFunc, NULL);
    //SwitchToFiber(main_fiber);
    //SwitchToFiber(fp);
}

uint8 OsMLQ_GetLowestBitNumber(uint16 Bitmap)
{
    uint8 const * table = ReversedLog2Tab;
    uint8 res;
    uint8 hb, lb;
    
    hb = (Bitmap & 0xff00u) >> 8;
    lb = Bitmap & 0x00ffu;

    if (hb == (uint8)0x00) {
        res = ReversedLog2Tab[lb];
    } else {
        res = ReversedLog2Tab[hb];
    }
    return res;
}

void TC2Timer_Handler(void)
{

}

uint8 * OsPort_TaskStackInit(TaskFunctionType * TaskFunc, uint8 * sp)
{
    return (uint8 *)NULL;
}

void OS_START_CURRENT_TASK(void)
{

}

void OS_SAVE_CONTEXT(void){

}

void OS_RESTORE_CONTEXT(void)
{

}

void OS_ISR_CONTEXT(void)
{

}

void fiberFunc(void *param)
{
    uint8 i;
    i=0;
}

