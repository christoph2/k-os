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
#include "Os_Port_Win32.h"
#include "Os_Cfg.h"
#include "Os_Vars.h"

/*////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*                                                                                                      */
/*                  Because of name-clashes we can't include 'windows.h'!                               */
/*                                                                                                      */
/*//////////////////////////////////////////////////////////////////////////////////////////////////////*/

// K_OS now offers tight Windows integration.

#define FIBER_FLAG_FLOAT_SWITCH 0x1

typedef void (*PFIBER_START_ROUTINE)(void *lpFiberParameter);
typedef PFIBER_START_ROUTINE LPFIBER_START_ROUTINE;

typedef void * HANDLE;
typedef unsigned long DWORD;

#define INFINITE        0xfffffffful
#define THREAD_PRIORITY_BELOW_NORMAL    -1

#define DECLSPEC_IMPORT __declspec(dllimport)
#define WINAPI          __stdcall
#define CALLBACK        __stdcall

typedef void (CALLBACK * TIMERPROC)(void *,unsigned short,unsigned short,unsigned long);

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
DECLSPEC_IMPORT DWORD   WINAPI GetCurrentThreadId(void);
DECLSPEC_IMPORT DWORD   WINAPI WaitForSingleObject(HANDLE hHandle, DWORD dwMillisecords);
DECLSPEC_IMPORT boolean WINAPI SetThreadPriority(HANDLE hThread, int nPriority);
DECLSPEC_IMPORT boolean WINAPI SwitchToThread(void);

DECLSPEC_IMPORT unsigned long WINAPI GetLastError(void);
unsigned short SetErrorMode(unsigned short uMode);

DECLSPEC_IMPORT unsigned short WINAPI SetTimer(void * hWnd, unsigned short nIDEvent, unsigned short uElapse, TIMERPROC lpTimerFunc);
/*///////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////*/


#define OS_PORT_ADDITIONAL_STACK_SPACE  ((uint32)0x400)

extern HANDLE Isr2Event;

void fiberFunc(void *param);
void OsPort_TimerInit(void);
boolean OsPort_TimerCreate(HANDLE * timerHandle, uint16 number, uint16 first, uint16 period);
void OsPort_TimerCleanup(void);
void OsPort_TimerDelete(HANDLE timerHandle);
TASK(DispatchingTask);

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

static void * OsPort_BaseFiber;
static void * OsPort_Fibers[OS_NUMBER_OF_TASKS];

HANDLE OsPort_BaseThreadId;

static HANDLE OsPort_TimerHandle[OS_NUMBER_OF_COUNTERS];

#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_OS_START_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */


#define TICKS_PER_MS ((uint16)1000u)

static uint16 timerNumber;
static void * sf;
static HANDLE schedThread;

HANDLE OsPort_GetCurrentThreadHandle(void);

#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_Init(void)
#else
void OsPort_Init(void)
#endif /* KOS_MEMORY_MAPPING */
{
    int tm;
    uint8 idx;
    unsigned long error;
    HANDLE curHandle;

    curHandle = OsPort_GetCurrentThreadHandle();

    OsPort_InitializeCriticalSection();
    OsPort_BaseThreadId = GetCurrentThreadId();
    SetThreadPriority(OsPort_BaseThreadId, THREAD_PRIORITY_BELOW_NORMAL);

#if 0
    SetErrorMode(SEM_FAILCRITICALERRORS); // Controls whether the system will handle the specified types of serious errors or whether the process will handle them.
    // besser (>= Windows 7): SetThreadErrorMode(SEM_FAILCRITICALERRORS, NULL /* &dwOldMode */);
#endif

/*
**  You can call SwitchToFiber with the address of a fiber created by a different thread.
**  To do this, you must have the address returned to the other thread when it called
**  CreateFiber and you must use proper synchronization.
*/

    OsPort_BaseFiber = ConvertThreadToFiber(NULL);  // FIBER_FLAG_FLOAT_SWITCH

    OsPort_TimerInit();

    for (idx = (uint8)0x00; idx < OS_NUMBER_OF_COUNTERS; ++idx) {
        if (!OsPort_TimerCreate(&OsPort_TimerHandle[idx], idx, 10, 10)) {
            printf("Creation of timer failed: %u", GetLastError());
            exit(1);
        }
    }
}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_Init(void)
#else
void OsPort_Shutdown(void)
#endif /* KOS_MEMORY_MAPPING */
{
    uint8 idx;

    for (idx = (uint8)0x00; idx < OS_NUMBER_OF_COUNTERS; ++idx) {
        OsPort_TimerDelete(OsPort_TimerHandle[idx]);
    }

    OsPort_TimerCleanup();
    OsPort_DeleteCriticalSection();
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


uint8 * OsPort_TaskStackInit(TaskType TaskID, TaskFunctionType * TaskFunc, uint8 * sp)
{
    UNREFERENCED_PARAMETER(sp);

    if (OsPort_Fibers[TaskID] != (void*)NULL) {
        DeleteFiber(OsPort_Fibers[TaskID]);
    }

    OsPort_Fibers[TaskID] = CreateFiber(OS_PORT_ADDITIONAL_STACK_SPACE + (uint32)OS_TaskConf[TaskID].StackSize,
        (LPFIBER_START_ROUTINE)*TaskFunc, NULL
    );
    return (uint8 *)NULL;
}


void TC2Timer_Handler(void)
{

}


void OsPort_StartCurrentTask(void)
{
    SwitchToFiber(OsPort_Fibers[Os_CurrentTID]);
}

void OsPort_SaveContext(void)
{

}

void OsPort_RestoreContext(void)
{
    SwitchToFiber(OsPort_Fibers[Os_CurrentTID]);
}

void OsPort_SwitchToISRContext(void)
{

}


// Performance. Determinism. Superior Quality. Period.

void OsPort_EnterPowerdownMode(void)
{
    (void)SwitchToThread();
}


#if 0
void OsPort_Dispatch(void)
{
// NUR VORRÜBERGEHEND!!!
//    SetEvent(DispatchingTask, DispatchingEvent);
}


TASK(DispatchingTask)
{
    FOREVER {
                // NUR VORRÜBERGEHEND!!!
        //WaitEvent(DispatchingEvent);
        //(void)TerminateTask();
    }
}
#endif
