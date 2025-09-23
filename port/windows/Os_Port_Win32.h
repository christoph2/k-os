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
#if !defined(__OS_PORT_WIN32_H)
#define __OS_PORT_WIN32_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
**  Standard C headers first
*/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/*
**  Force all Windows types to be defined - BEFORE including any Windows headers
*/
#if defined(__MINGW32__) || defined(__MINGW64__)
    /* Force MinGW to include all string types */
    #define _INC_STRING
    #define __need_wchar_t
    #define __need_wint_t

    /* Ensure Windows version is set */
    #undef WINVER
    #undef _WIN32_WINNT
    #undef _WIN32_WINDOWS
    #undef _WIN32_IE

    #define WINVER 0x0601
    #define _WIN32_WINNT 0x0601
    #define _WIN32_WINDOWS 0x0601
    #define _WIN32_IE 0x0800

    /* Force all API sets to be included */
    #define _APISETSTRING_
    #define _APISETUTIL_
    #define _APISETTEXTCOMPAT_

    /* Pre-define the problematic types manually */
    #ifndef CHAR
    typedef char CHAR;
    #endif

    #ifndef WCHAR
    typedef wchar_t WCHAR;
    #endif

    #ifndef CONST
    #define CONST const
    #endif

    /* Pre-define string pointer types */
    #ifndef LPCSTR
    typedef const char *LPCSTR;
    #endif

    #ifndef LPCWSTR
    typedef const wchar_t *LPCWSTR;
    #endif

    #ifndef LPSTR
    typedef char *LPSTR;
    #endif

    #ifndef LPWSTR
    typedef wchar_t *LPWSTR;
    #endif

    #ifndef LPCTSTR
        #ifdef UNICODE
            typedef LPCWSTR LPCTSTR;
        #else
            typedef LPCSTR LPCTSTR;
        #endif
    #endif

    #ifndef LPTSTR
        #ifdef UNICODE
            typedef LPWSTR LPTSTR;
        #else
            typedef LPSTR LPTSTR;
        #endif
    #endif
#endif

/*
**  Now include Windows headers with all types pre-defined
*/
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>
#include <process.h>

/*
**  Additional MinGW fixes after windows.h
*/
#if defined(__MINGW32__) || defined(__MINGW64__)
    /* Ensure multimedia timer functions are available */
    #ifndef MMRESULT
    typedef UINT MMRESULT;
    #endif

    /* Forward declare multimedia functions if not available */
    #ifndef timeBeginPeriod
    WINMMAPI MMRESULT WINAPI timeBeginPeriod(UINT uPeriod);
    WINMMAPI MMRESULT WINAPI timeEndPeriod(UINT uPeriod);
    #endif

    /* Ensure GetVersionEx is available */
    #ifndef GetVersionExA
    WINBASEAPI WINBOOL WINAPI GetVersionExA(LPOSVERSIONINFOA lpVersionInformation);
    #endif

    /* Ensure Console functions are available */
    #ifndef SetConsoleTitleA
    WINBASEAPI WINBOOL WINAPI SetConsoleTitleA(LPCSTR lpConsoleTitle);
    #endif

    #ifndef WriteConsoleA
    WINBASEAPI WINBOOL WINAPI WriteConsoleA(
        HANDLE hConsoleOutput,
        CONST VOID* lpBuffer,
        DWORD nNumberOfCharsToWrite,
        LPDWORD lpNumberOfCharsWritten,
        LPVOID lpReserved
    );
    #endif
#endif

/*
**  OSEK types - avoid conflicts with existing definitions
*/
#ifndef TaskType
typedef uint8_t TaskType;
#endif

#ifndef Os_TaskFunctionType
typedef void Os_TaskFunctionType(void);
#endif

/*
**  Port-specific types.
*/
typedef DWORD InterruptStateType;
typedef CRITICAL_SECTION OsPort_MutexType;
typedef HANDLE OsPort_EventType;
typedef HANDLE OsPort_ThreadType;
typedef HANDLE OsPort_SemaphoreType;

/*
**  Task context structure for Windows threads.
*/
typedef struct {
    HANDLE thread_handle;
    DWORD thread_id;
    uint8_t *stack_base;
    size_t stack_size;
    Os_TaskFunctionType *task_func;
    TaskType task_id;
    int priority;
    volatile LONG state;
    HANDLE suspend_event;
    HANDLE resume_event;
} OsPort_TaskContext;

/*
**  Windows-specific configuration.
*/
#define OSPORT_TIMER_RESOLUTION_MS      1       /* 1ms resolution */
#define OSPORT_DEFAULT_STACK_SIZE       65536   /* 64KB default stack */
#define OSPORT_MAX_TASKS                64      /* Maximum number of tasks */

/*
**  Task states for Windows port.
*/
#define OSPORT_TASK_STATE_READY         0x01
#define OSPORT_TASK_STATE_RUNNING       0x02
#define OSPORT_TASK_STATE_SUSPENDED     0x04
#define OSPORT_TASK_STATE_TERMINATED    0x08

/*
**  Global variables.
*/
extern CRITICAL_SECTION OsPort_GlobalCriticalSection;
extern HANDLE OsPort_ScheduleEvent;
extern HANDLE OsPort_SystemTimer;
extern volatile LONG OsPort_InterruptFlag;
extern LARGE_INTEGER OsPort_StartTime;
extern LARGE_INTEGER OsPort_PerformanceFrequency;

/*
**  Port-specific function declarations.
*/
void OsPort_InitializeCriticalSections(void);
void OsPort_CleanupCriticalSections(void);
void OsPort_InitializeTimer(void);
void OsPort_CleanupTimer(void);
void OsPort_SetHighPriority(void);
void OsPort_PrintSystemInfo(void);

/*
**  Thread management functions.
*/
HANDLE OsPort_CreateTaskThread(TaskType TaskID, Os_TaskFunctionType *TaskFunc, uint32_t StackSize);
void OsPort_SuspendTask(HANDLE ThreadHandle);
void OsPort_ResumeTask(HANDLE ThreadHandle);
void OsPort_TerminateTask(HANDLE ThreadHandle);

/*
**  Timer callback and interrupt simulation.
*/
void CALLBACK OsPort_TimerCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired);
void OsPort_SimulateInterrupt(void);

/*
**  Context switching macros using Windows synchronization.
*/
#define OsPort_SaveContext() \
    do { \
        EnterCriticalSection(&OsPort_GlobalCriticalSection); \
    } while(0)

#define OsPort_RestoreContext() \
    do { \
        LeaveCriticalSection(&OsPort_GlobalCriticalSection); \
        SetEvent(OsPort_ScheduleEvent); \
    } while(0)

#define OsPort_StartCurrentTask() \
    do { \
        WaitForSingleObject(OsPort_ScheduleEvent, INFINITE); \
        ResetEvent(OsPort_ScheduleEvent); \
        LeaveCriticalSection(&OsPort_GlobalCriticalSection); \
    } while(0)

#define OsPort_SwitchToISRContext() \
    do { \
        EnterCriticalSection(&OsPort_GlobalCriticalSection); \
    } while(0)

/*
**  Interrupt disable/enable macros using critical sections.
*/
#define OsPort_DisableAllInterrupts() \
    do { \
        EnterCriticalSection(&OsPort_GlobalCriticalSection); \
    } while(0)

#define OsPort_EnableAllInterrupts() \
    do { \
        LeaveCriticalSection(&OsPort_GlobalCriticalSection); \
    } while(0)

/*
**  Memory and synchronization barriers.
*/
#define OsPort_MemoryBarrier()          MemoryBarrier()
#define OsPort_YieldProcessor()         YieldProcessor()

/*
**  Debug and profiling macros.
*/
#ifdef _DEBUG
    #define OsPort_DebugBreak()         __debugbreak()
    #define OsPort_OutputDebugString(s) OutputDebugStringA(s)
#else
    #define OsPort_DebugBreak()         ((void)0)
    #define OsPort_OutputDebugString(s) ((void)0)
#endif

/*
**  Console I/O for debugging.
*/
void OsPort_InitializeConsole(void);
void OsPort_CleanupConsole(void);
void OsPort_ConsolePrintf(const char *format, ...);

/*
**  Error handling functions
*/
void OsPort_PrintLastError(const char* function_name);
const char* OsPort_GetErrorString(DWORD error_code);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __OS_PORT_WIN32_H */