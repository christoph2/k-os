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
#include <stdio.h>
#include <stdarg.h>

/*
**  Global variables for Windows port.
*/
InterruptStateType OsPort_InterruptState = 0;
CRITICAL_SECTION OsPort_GlobalCriticalSection;
HANDLE OsPort_ScheduleEvent = NULL;
HANDLE OsPort_SystemTimer = NULL;
volatile LONG OsPort_InterruptFlag = 0;
LARGE_INTEGER OsPort_StartTime;
LARGE_INTEGER OsPort_PerformanceFrequency;

/* Stack for ISR context simulation (provided by generated config) */
extern uint8_t ISR_Stack[ISR_STACK_SIZE];
const SizeType OS_TOS_ISR = (SizeType)((const uint8_t *)&ISR_Stack + ISR_STACK_SIZE);

/* Console handles for debugging */
static HANDLE hConsoleOutput = NULL;
static HANDLE hConsoleInput = NULL;
static BOOL ConsoleInitialized = FALSE;

#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_OS_START_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */

/*
**  Initialize the Windows port.
*/
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_Init(void)
#else
void OsPort_Init(void)
#endif /* KOS_MEMORY_MAPPING */
{
    BOOL result;

    /* Get high-resolution performance counter frequency */
    QueryPerformanceFrequency(&OsPort_PerformanceFrequency);
    QueryPerformanceCounter(&OsPort_StartTime);

    /* Set high priority for better real-time behavior */
    OsPort_SetHighPriority();

    /* Initialize critical sections */
    OsPort_InitializeCriticalSections();

    /* Initialize console for debugging */
    OsPort_InitializeConsole();

    /* Initialize system timer */
    OsPort_InitializeTimer();

    /* Print system information */
    OsPort_PrintSystemInfo();

    OsPort_ConsolePrintf("k-os Windows port initialized successfully\n");
}

/*
**  Shutdown the Windows port.
*/
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_Shutdown(void)
#else
void OsPort_Shutdown(void)
#endif /* KOS_MEMORY_MAPPING */
{
    OsPort_ConsolePrintf("k-os Windows port shutting down...\n");

    /* Cleanup timer */
    OsPort_CleanupTimer();

    /* Cleanup critical sections */
    OsPort_CleanupCriticalSections();

    /* Cleanup console */
    OsPort_CleanupConsole();

    OsPort_ConsolePrintf("k-os Windows port shutdown complete\n");
}

/*
**  Initialize task stack for Windows thread execution.
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
    /* Windows manages thread stacks automatically */
    /* Create the thread in suspended state */
    HANDLE threadHandle = OsPort_CreateTaskThread(TaskID, (Os_TaskFunctionType*)TaskFunc, OSPORT_DEFAULT_STACK_SIZE);

    if (threadHandle != NULL) {
        OsPort_ConsolePrintf("Task %d thread created successfully\n", TaskID);
    } else {
        OsPort_ConsolePrintf("Failed to create thread for task %d\n", TaskID);
    }

    return sp; /* Stack pointer not relevant for Windows threads */
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
    LARGE_INTEGER currentTime;
    LARGE_INTEGER elapsedTime;

    QueryPerformanceCounter(&currentTime);

    /* Calculate elapsed time in microseconds */
    elapsedTime.QuadPart = currentTime.QuadPart - OsPort_StartTime.QuadPart;
    elapsedTime.QuadPart *= 1000000; /* Convert to microseconds */
    elapsedTime.QuadPart /= OsPort_PerformanceFrequency.QuadPart;

    return (uint32_t)elapsedTime.QuadPart;
}

/*
**  Initialize critical sections.
*/
void OsPort_InitializeCriticalSections(void)
{
    InitializeCriticalSection(&OsPort_GlobalCriticalSection);

    /* Create scheduling event */
    OsPort_ScheduleEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (OsPort_ScheduleEvent == NULL) {
        OsPort_ConsolePrintf("Failed to create scheduling event: %lu\n", GetLastError());
        ExitProcess(1);
    }
}

/*
**  Cleanup critical sections.
*/
void OsPort_CleanupCriticalSections(void)
{
    if (OsPort_ScheduleEvent != NULL) {
        CloseHandle(OsPort_ScheduleEvent);
        OsPort_ScheduleEvent = NULL;
    }

    DeleteCriticalSection(&OsPort_GlobalCriticalSection);
}

/*
**  Initialize system timer.
*/
void OsPort_InitializeTimer(void)
{
    BOOL result;

    /* Create a waitable timer for system tick */
    result = CreateTimerQueueTimer(
        &OsPort_SystemTimer,
        NULL,
        OsPort_TimerCallback,
        NULL,
        OSPORT_TIMER_RESOLUTION_MS,     /* Initial delay */
        OSPORT_TIMER_RESOLUTION_MS,     /* Period */
        WT_EXECUTEDEFAULT
    );

    if (!result) {
        OsPort_ConsolePrintf("Failed to create system timer: %lu\n", GetLastError());
        ExitProcess(1);
    }
}

/*
**  Cleanup system timer.
*/
void OsPort_CleanupTimer(void)
{
    if (OsPort_SystemTimer != NULL) {
        DeleteTimerQueueTimer(NULL, OsPort_SystemTimer, INVALID_HANDLE_VALUE);
        OsPort_SystemTimer = NULL;
    }
}

/*
**  Set high priority for better real-time behavior.
*/
void OsPort_SetHighPriority(void)
{
    HANDLE currentProcess = GetCurrentProcess();
    HANDLE currentThread = GetCurrentThread();

    /* Set process priority class to high */
    if (!SetPriorityClass(currentProcess, HIGH_PRIORITY_CLASS)) {
        OsPort_ConsolePrintf("Warning: Could not set high priority class: %lu\n", GetLastError());
    }

    /* Set thread priority to time critical */
    if (!SetThreadPriority(currentThread, THREAD_PRIORITY_TIME_CRITICAL)) {
        OsPort_ConsolePrintf("Warning: Could not set thread priority: %lu\n", GetLastError());
    }

    /* Set timer resolution to 1ms */
    timeBeginPeriod(1);
}

/*
**  Print system information.
*/
void OsPort_PrintSystemInfo(void)
{
    SYSTEM_INFO sysInfo;
    OSVERSIONINFOEX osInfo;

    GetSystemInfo(&sysInfo);

    ZeroMemory(&osInfo, sizeof(OSVERSIONINFOEX));
    osInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    GetVersionEx((OSVERSIONINFO*)&osInfo);

    OsPort_ConsolePrintf("=== k-os Windows Port System Information ===\n");
    OsPort_ConsolePrintf("OS Version: %lu.%lu Build %lu\n",
                         osInfo.dwMajorVersion, osInfo.dwMinorVersion, osInfo.dwBuildNumber);
    OsPort_ConsolePrintf("Processors: %lu\n", sysInfo.dwNumberOfProcessors);
    OsPort_ConsolePrintf("Page Size: %lu bytes\n", sysInfo.dwPageSize);
    OsPort_ConsolePrintf("Timer Frequency: %lld Hz\n", OsPort_PerformanceFrequency.QuadPart);
    OsPort_ConsolePrintf("=============================================\n\n");
}

/*
**  Create a task thread.
*/
HANDLE OsPort_CreateTaskThread(TaskType TaskID, Os_TaskFunctionType *TaskFunc, uint32_t StackSize)
{
    HANDLE threadHandle;
    DWORD threadId;

    threadHandle = CreateThread(
        NULL,                   /* Security attributes */
        StackSize,              /* Stack size */
        (LPTHREAD_START_ROUTINE)TaskFunc,  /* Thread function */
        (LPVOID)TaskID,         /* Thread parameter */
        CREATE_SUSPENDED,       /* Creation flags */
        &threadId              /* Thread ID */
    );

    if (threadHandle != NULL) {
        /* Set thread priority based on task priority */
        SetThreadPriority(threadHandle, THREAD_PRIORITY_NORMAL);
    }

    return threadHandle;
}

/*
**  Timer callback function.
*/
void CALLBACK OsPort_TimerCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{
    (void)lpParameter;
    (void)TimerOrWaitFired;

    /* Set interrupt flag */
    InterlockedIncrement(&OsPort_InterruptFlag);

    #if defined(OS_FEATURE_INSTALL_RTI_HANDLER)
    /* Call the RTI (Real Time Interrupt) handler */
    OS_RTI_DRIVER_IMPL();
    #endif

    /* Trigger scheduling event */
    SetEvent(OsPort_ScheduleEvent);
}

/*
**  Simulate an interrupt.
*/
void OsPort_SimulateInterrupt(void)
{
    InterlockedIncrement(&OsPort_InterruptFlag);
    SetEvent(OsPort_ScheduleEvent);
}

/*
**  Additional OSEK port functions.
*/
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_StartCurrentTask(void)
#else
void OsPort_StartCurrentTask(void)
#endif /* KOS_MEMORY_MAPPING */
{
    SetEvent(OsPort_ScheduleEvent);
}

#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_SaveContext(void)
#else
void OsPort_SaveContext(void)
#endif /* KOS_MEMORY_MAPPING */
{
    EnterCriticalSection(&OsPort_GlobalCriticalSection);
}

#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_RestoreContext(void)
#else
void OsPort_RestoreContext(void)
#endif /* KOS_MEMORY_MAPPING */
{
    LeaveCriticalSection(&OsPort_GlobalCriticalSection);
    SetEvent(OsPort_ScheduleEvent);
}

#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_SwitchToISRContext(void)
#else
void OsPort_SwitchToISRContext(void)
#endif /* KOS_MEMORY_MAPPING */
{
    EnterCriticalSection(&OsPort_GlobalCriticalSection);
}

#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_CreateEvents(void)
#else
void OsPort_CreateEvents(void)
#endif /* KOS_MEMORY_MAPPING */
{
    /* Events are handled through Windows events */
}

#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_SignalSchedulingEvent(void)
#else
void OsPort_SignalSchedulingEvent(void)
#endif /* KOS_MEMORY_MAPPING */
{
    SetEvent(OsPort_ScheduleEvent);
}

#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_AdjustProcessPriorityClass(void)
#else
void OsPort_AdjustProcessPriorityClass(void)
#endif /* KOS_MEMORY_MAPPING */
{
    OsPort_SetHighPriority();
}

#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_KeyboardLoop(void)
#else
void OsPort_KeyboardLoop(void)
#endif /* KOS_MEMORY_MAPPING */
{
    char input;
    DWORD bytesRead;

    OsPort_ConsolePrintf("k-os Windows port running. Press 'q' + Enter to quit.\n");

    while (TRUE) {
        if (ReadConsole(hConsoleInput, &input, 1, &bytesRead, NULL)) {
            if (input == 'q' || input == 'Q') {
                OsPort_ConsolePrintf("Shutting down k-os...\n");
                break;
            }
        }
        Sleep(100); /* 100ms delay */
    }
}

/*
**  Task suspension hook.
*/
void OsPort_SuspendHook(uint16_t taskID)
{
    (void)taskID; /* Unused parameter */
    /* Yield processor time to other threads */
    SwitchToThread();
}

/*
**  Console I/O functions.
*/
void OsPort_InitializeConsole(void)
{
    if (!AllocConsole()) {
        /* Console might already exist */
        if (GetLastError() != ERROR_ACCESS_DENIED) {
            return;
        }
    }

    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    hConsoleInput = GetStdHandle(STD_INPUT_HANDLE);

    if (hConsoleOutput != INVALID_HANDLE_VALUE && hConsoleInput != INVALID_HANDLE_VALUE) {
        ConsoleInitialized = TRUE;

        /* Set console title */
        SetConsoleTitleA("k-os Windows Port Debug Console");  /* Fix: Use SetConsoleTitleA */

        /* Set console buffer size */
        COORD bufferSize = {120, 3000};
        SetConsoleScreenBufferSize(hConsoleOutput, bufferSize);
    }
}

void OsPort_CleanupConsole(void)
{
    if (ConsoleInitialized) {
        FreeConsole();
        ConsoleInitialized = FALSE;
    }
}

void OsPort_ConsolePrintf(const char* format, ...)
{
    if (!ConsoleInitialized || hConsoleOutput == INVALID_HANDLE_VALUE) {
        return;
    }

    char buffer[1024];
    va_list args;
    DWORD bytesWritten;

    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    WriteConsoleA(hConsoleOutput, buffer, (DWORD)strlen(buffer), &bytesWritten, NULL);  /* Fix: Use WriteConsoleA */
}

#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_OS_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */
